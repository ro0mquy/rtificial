#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

#include <jansson.h>

#include <libzeuch/shader.h>

#include "window.h"
#include "util.h"
#include "flight.h"

#include "timeline.h"
#include "timeline_vertex.h"
#include "timeline_fragment.h"

#define TIMELINE_DEFAULT_SCALE .5

typedef struct {
	GLfloat x, y, w, h;
	GLfloat r, g, b, a;
} rect_t;

typedef struct keyframe_list_t {
	size_t length;
	size_t allocated;
	keyframe_t elements[];
} keyframe_list_t;

static void draw_rect(const timeline_t* timeline, const rect_t* rect);
static keyframe_list_t* list_insert(keyframe_list_t* list, keyframe_t frame, size_t position);
static keyframe_list_t* list_remove(keyframe_list_t* list, size_t position);
static keyframe_list_t* list_clear(keyframe_list_t* list);
static keyframe_t* list_get(keyframe_list_t* list, size_t position);
static size_t list_find(keyframe_list_t* list, int time);
static keyframe_list_t* timeline_get_bezier_spline(keyframe_list_t* controlPoints, keyframe_list_t* knots, float scale);

const GLfloat timeline_height = .15;
const int segment_length = 10000;
const int num_segments = 20;
const GLfloat marker_width = .003;
const GLfloat cursor_width = .003;
const float zoom_factor = .9;
const int scroll_factor = 7000;

timeline_t* timeline_new() {
	const GLuint vertex_shader = shader_load_strings(1, "timeline_vertex", (const GLchar* []) { timeline_vertex_source }, GL_VERTEX_SHADER);
	const GLuint fragment_shader = shader_load_strings(1, "timeline_fragment", (const GLchar* []) { timeline_fragment_source }, GL_FRAGMENT_SHADER);
	const GLuint program = shader_link_program(vertex_shader, fragment_shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	const GLint attribute_coord2d = glGetAttribLocation(program, "coord2d");
	const GLint uniform_color = shader_get_uniform(program, "color");

	timeline_t* const timeline = malloc(sizeof(timeline_t));
	if(timeline == NULL) {
		glDeleteProgram(program);
		return NULL;
	}
	keyframe_list_t* keyframes = malloc(sizeof(keyframe_list_t) + sizeof(keyframe_t));
	if(keyframes == NULL) {
		glDeleteProgram(program);
		return NULL;
	}
	*keyframes = (keyframe_list_t) {
		.length = 0,
		.allocated = 1,
	};
	keyframe_list_t* controlPoints = malloc(sizeof(keyframe_list_t) + sizeof(keyframe_t));
	if(controlPoints == NULL) {
		glDeleteProgram(program);
		return NULL;
	}
	*controlPoints = (keyframe_list_t) {
		.length = 0,
		.allocated = 1,
	};
	camera_t start_cam, end_cam;
	camera_init(&start_cam);
	camera_init(&end_cam);
	// TODO dirty fix, to prevent black screen
	camera_move_y(&end_cam, 0.00001);
	keyframes = list_insert(keyframes, (keyframe_t) {
		.time = 0,
		.camera = start_cam,
	}, 0);
	keyframes = list_insert(keyframes, (keyframe_t) {
		.time = INT_MAX - 10, // inf oder so aehnlich
		.camera = end_cam,
	}, 1);

	controlPoints = timeline_get_bezier_spline(controlPoints, keyframes, TIMELINE_DEFAULT_SCALE);

	*timeline = (timeline_t) {
		.program = program,
		.attribute_coord2d = attribute_coord2d,
		.uniform_color = uniform_color,
		.zoom = 1.,
		.focus_position = num_segments * segment_length / 2,
		.keyframes = keyframes,
		.controlPoints = controlPoints,
		.hidden = false,
		.cursor_position = 0,
		.is_playing = false,
		.camera_changed = false,
	};
	return timeline;
}

void timeline_draw(timeline_t* const timeline) {
	if(timeline->hidden) return;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(timeline->program);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(timeline->attribute_coord2d);

	// actual drawing begins here
	draw_rect(timeline, &(rect_t) {
		.x = 0., .y = 0., .w = 1., .h = timeline_height,
		.r = 1., .g = 1., .b = 1., .a = .5,
	});

	const float zoom = timeline->zoom;
	const int focus = timeline->focus_position;
	const int window_width = num_segments * segment_length * zoom;
	const int left_edge = focus - window_width / 2;
	const int right_edge = focus + window_width / 2;
	const int _start_first_rect = left_edge - left_edge % (2*segment_length); // floor left_edge
	const int start_first_rect = _start_first_rect < 0 ? 0 : _start_first_rect; // -nan

	// draw black boxes
	for (int i = start_first_rect; i < right_edge; i += 2 * segment_length) {
		draw_rect(timeline, &(rect_t) {
			.x = (GLfloat) (i - left_edge) / window_width, .y = 0.,
			.w = (GLfloat) segment_length / window_width, .h = timeline_height,
			.r = 0., .g = 0., .b = 0., .a = .5,
		});
	}

	// draw keyframe markers
	for (size_t i = 0; i < timeline->keyframes->length; i++) {
		const int time = list_get(timeline->keyframes, i)->time;
		const GLfloat x = (GLfloat) (time - left_edge) / window_width - marker_width / 2 / zoom;
		draw_rect(timeline, &(rect_t) {
			.x = x, .y = 0., .w = marker_width / zoom, .h = timeline_height,
			.r = 1., .g = 0., .b = 0., .a = 1.,
		});
	}

	// draw cursor postion
	const GLfloat x = (GLfloat) (timeline->cursor_position - left_edge) / window_width - marker_width / 2 / zoom;
	draw_rect(timeline, &(rect_t) {
		.x = x, .y = 0., .w = cursor_width / zoom, .h = timeline_height,
		.r = 0., .g = 1., .b = 0., .a = 1.,
	});

	glDisableVertexAttribArray(timeline->attribute_coord2d);
	glDisable(GL_BLEND);
}

void timeline_destroy(timeline_t* const timeline) {
	util_safe_free(timeline->keyframes);
	util_safe_free(timeline->controlPoints);
	glDeleteProgram(timeline->program);
}

void timeline_save(timeline_t* const timeline, char* path){
	size_t num_frames = timeline->keyframes->length;

	json_t* keyframes_json_array = json_array();
	for(size_t i = 0; i < num_frames; i++){
		keyframe_t keyframe = timeline->keyframes->elements[i];
		json_t* keyframe_json_object = json_object();

		json_object_set(keyframe_json_object, "time", json_integer(keyframe.time));
		json_object_set(
			keyframe_json_object,
			"position",
			json_pack(
				"[fff]",
				keyframe.camera.position.x,
				keyframe.camera.position.y,
				keyframe.camera.position.z
			)
		);
		json_object_set(
			keyframe_json_object,
			"rotation",
			json_pack(
				"[ffff]",
				keyframe.camera.rotation.v.x,
				keyframe.camera.rotation.v.y,
				keyframe.camera.rotation.v.z,
				keyframe.camera.rotation.w
			)
		);

		json_array_append(keyframes_json_array, keyframe_json_object);
	}

	json_t* timeline_json = json_object();
	json_object_set(timeline_json, "keyframes", keyframes_json_array);

	if(json_dump_file(timeline_json, path, JSON_INDENT(4) | JSON_PRESERVE_ORDER)){
		fprintf(stderr, "error: timeline export: json file \"%s\" could not be written\n", path);
	}
	else{
		printf("timeline file \"%s\" written\n", path);
	}
}

void timeline_load(timeline_t* timeline, char* path){
	json_error_t error;
	json_t* const timeline_json = json_load_file(path, 0, &error);
	if(timeline_json == NULL) {
		fprintf(stderr, "error: timeline_load: error  parsing json in %s line: %d column: %d!\n", path, error.line, error.column);
		fprintf(stderr, error.text, path);
		fprintf(stderr, "\n");
		return;
	}

	json_t* keyframes_json_array = json_object_get(timeline_json, "keyframes");

	timeline->keyframes = list_clear(timeline->keyframes);

	size_t i;
	json_t* value;
	json_array_foreach(keyframes_json_array, i, value){
		int time = json_integer_value(json_object_get(value, "time"));

		json_t* json_position_array = json_object_get(value, "position");
		vec3 position = {
			.x = json_real_value(json_array_get(json_position_array, 0)),
			.y = json_real_value(json_array_get(json_position_array, 1)),
			.z = json_real_value(json_array_get(json_position_array, 2)),
		};

		json_t* json_rotation_array = json_object_get(value, "rotation");
		quat rotation = {
			.v = {
				.x = json_real_value(json_array_get(json_rotation_array, 0)),
				.y = json_real_value(json_array_get(json_rotation_array, 1)),
				.z = json_real_value(json_array_get(json_rotation_array, 2))
			},
			.w = json_real_value(json_array_get(json_rotation_array, 3))
		};

		camera_t camera = {
			.position = position,
			.rotation = rotation
		};

		timeline->keyframes = list_insert(timeline->keyframes, (keyframe_t) {
			.time = time,
			.camera = camera,
		}, i);

	}

	timeline->controlPoints = timeline_get_bezier_spline(timeline->controlPoints, timeline->keyframes, TIMELINE_DEFAULT_SCALE);
}

bool timeline_handle_sdl_event(timeline_t* const timeline, const SDL_Event* const event) {
	if(event->type == SDL_MOUSEBUTTONDOWN) {
		if(1. - ((float) event->button.y / window_get_height()) > timeline_height) {
			return false;
		}
		if(timeline->hidden) {
			return false;
		}

		// calculate the position of the mouse
		const float horizontal = ((float) event->button.x / window_get_width());
		const float zoom = timeline->zoom;
		const int focus = timeline->focus_position;
		const int window_width = num_segments * segment_length * zoom;
		const int _mouse_pos = focus + (horizontal - .5) * window_width;
		const int mouse_pos = _mouse_pos < 0 ? 0 : _mouse_pos;

		// check for pressed ctrl-key
		Uint8* keystate= SDL_GetKeyState(NULL);;

		switch(event->button.button) {
			case SDL_BUTTON_WHEELUP:
				// scroll the timeline
				if (keystate[SDLK_LCTRL] || keystate[SDLK_RCTRL]) {
					timeline->focus_position -= scroll_factor * timeline->zoom;
					return true;
				}
				// zoom the timeline
				timeline->focus_position = (focus - mouse_pos) * zoom_factor + mouse_pos;
				timeline->zoom *= zoom_factor;
				return true;
			case SDL_BUTTON_WHEELDOWN:
				// scroll the timeline
				if (keystate[SDLK_LCTRL] || keystate[SDLK_RCTRL]) {
					timeline->focus_position += scroll_factor * timeline->zoom;
					return true;
				}
				// zoom the timeline
				// personally I prefer when zooming out is not centered on the mouse
				//timeline->focus_position = (focus - mouse_pos) / zoom_factor + mouse_pos;
				timeline->zoom /= zoom_factor;
				return true;
			case SDL_BUTTON_LEFT:
				timeline->cursor_position = mouse_pos;
				if (keystate[SDLK_LCTRL] || keystate[SDLK_RCTRL]) {
					// only change camera when ctrl was pressed
					timeline->camera_changed = true;
				}
				return true;
		}
	} else if(event->type == SDL_KEYDOWN) {
		switch(event->key.keysym.sym) {
			case SDLK_h:
				timeline->hidden = !timeline->hidden;
				return true;
			case SDLK_p:
				timeline->is_playing = !timeline->is_playing;
				return true;
			case SDLK_r:
				// remove nearest keyframe
				timeline_remove_frame(timeline);
				return true;
			default:
				break;
		}
	}
	return false;
}

void timeline_add_frame(timeline_t* const timeline, const camera_t camera) {
	const int time = timeline->cursor_position;
	const size_t pos = list_find(timeline->keyframes, time);

	// check if there is already a frame for this time point
	if (list_get(timeline->keyframes, pos)->time == time) {
		timeline->keyframes = list_remove(timeline->keyframes, pos);
	}

	timeline->keyframes = list_insert(timeline->keyframes, (keyframe_t) {
		.time = time,
		.camera = camera,
	}, pos);
	timeline->controlPoints = timeline_get_bezier_spline(timeline->controlPoints, timeline->keyframes, TIMELINE_DEFAULT_SCALE);
}

void timeline_remove_frame(timeline_t* const timeline) {
	// remove nearest keyframe
	const int time = timeline->cursor_position;
	const size_t pos = list_find(timeline->keyframes, time);

	// don't remove first or last keyframe
	if (pos == 0 || pos == timeline->keyframes->length) return;

	int dt_before = abs(time - list_get(timeline->keyframes, pos - 1)->time);
	int dt_after = abs(time - list_get(timeline->keyframes, pos)->time);
	if (dt_before < dt_after) {
		// still don't remove first keyframe
		if (pos == 1) return;
		timeline->keyframes = list_remove(timeline->keyframes, pos - 1);
	} else {
		// don't remove last keyframe
		if (pos == timeline->keyframes->length) return;
		timeline->keyframes = list_remove(timeline->keyframes, pos);
	}

	timeline->controlPoints = timeline_get_bezier_spline(timeline->controlPoints, timeline->keyframes, TIMELINE_DEFAULT_SCALE);
}

void timeline_update(timeline_t* timeline, int dtime) {
	if(timeline->is_playing) {
		timeline->cursor_position += dtime;
		timeline->camera_changed = true;
	}
}

bool timeline_camera_changed(const timeline_t* const timeline) {
	return timeline->camera_changed;
}

camera_t timeline_get_camera(timeline_t* const timeline) {
	timeline->camera_changed = false;
	// interpolate and stuff
	const size_t pos = list_find(timeline->keyframes, timeline->cursor_position);
	if (pos == 0) {
		// if cursor_position is at the very first keyframe, just return it
		return list_get(timeline->controlPoints, 0)->camera;
	}

	const size_t nodeIndex = 3 * (pos - 1);
        keyframe_t* const p0 = list_get(timeline->controlPoints, nodeIndex);
        keyframe_t* const p1 = list_get(timeline->controlPoints, nodeIndex + 1);
        keyframe_t* const p2 = list_get(timeline->controlPoints, nodeIndex + 2);
        keyframe_t* const p3 = list_get(timeline->controlPoints, nodeIndex + 3);

	flight_t flight = flight_new(&p0->camera, &p1->camera, &p2->camera, &p3->camera, p0->time, p3->time - p0->time);
	return flight_get_camera(&flight, timeline->cursor_position);
}

static void draw_rect(const timeline_t* const timeline, const rect_t* const rect) {
	const GLfloat vertices[] = {
		rect->x, rect->y + rect->h,
		rect->x, rect->y,
		rect->x + rect->w, rect->y + rect->h,
		rect->x + rect->w, rect->y,
	};
	glVertexAttribPointer(
		timeline->attribute_coord2d,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		vertices
	);
	glUniform4f(timeline->uniform_color, rect->r, rect->g, rect->b, rect->a);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

static keyframe_list_t* list_insert(keyframe_list_t* list, const keyframe_t frame, const size_t position) {
	if(list->length == list->allocated) {
		keyframe_list_t* const new_list = realloc(list, sizeof(keyframe_list_t) + 2 * list->length * sizeof(keyframe_t));
		new_list->allocated *= 2;
		list = new_list;
	}
	memmove(list->elements + position + 1, list->elements + position, (list->length - position) * sizeof(keyframe_t));
	list->elements[position] = frame;
	list->length++;
	return list;
}

static keyframe_list_t* list_remove(keyframe_list_t* const list, const size_t position) {
	if(position < list->length - 1) {
		memmove(list->elements + position, list->elements + position + 1, (list->length - position - 1) * sizeof(keyframe_t));
	}
	list->length--;
	if(list->length * 4 <= list->allocated) {
		keyframe_list_t* const new_list = realloc(list, sizeof(keyframe_list_t) + 2 * list->length * sizeof(keyframe_t));
		new_list->allocated = new_list->length * 2;
		return new_list;
	} else {
		return list;
	}
}

static keyframe_list_t* list_clear(keyframe_list_t* const list) {
	list->length = 0;
	return list;
}

static keyframe_t* list_get(keyframe_list_t* list, size_t position) {
	return &list->elements[position];
}

static size_t list_find(keyframe_list_t* list, int time) {
	for(size_t i = 0; i < list->length; i++) {
		if(list_get(list, i)->time >= time) {
			return i;
		}
	}
	return list->length;
}

//helper functions for quaternion bezier curves
// cut q0 - q1 in two halfs
static quat bisect(quat q0, quat q1) {
	return quat_normalize(quat_add(q0, q1));
}

// mirror q0 at q1
static quat mirror(quat q0, quat q1) {
	// original version by shoemake does not work somehow
	//quat product = quat_mult(quat_mult(q0, q1), q1);
	//return quat_sub(quat_s_mult(2, product), q0));
	return quat_mult(q1, quat_mult(quat_conjugate(q0), q1));
}

// taken from http://devmag.org.za/2011/06/23/bzier-path-algorithms/
// quaternion from http://embots.dfki.de/doc/seminar_ca/Kremer_Quaternions.pdf
// http://caig.cs.nctu.edu.tw/course/CA/Lecture/slerp.pdf
static keyframe_list_t* timeline_get_bezier_spline(keyframe_list_t* controlPoints, keyframe_list_t* knots, float scale) {
	controlPoints = list_clear(controlPoints);
	for (uint i = 0; i < knots->length; i++) {
		if (i == 0) { // is first
			vec3 p1 = knots->elements[i].camera.position;
			quat v1 = knots->elements[i].camera.rotation;
			vec3 p2 = knots->elements[i+1].camera.position;
			quat v2 = knots->elements[i+1].camera.rotation;

			vec3 tangent = vec3_sub(p2, p1); // (p2 - p1)
			vec3 q1 = vec3_add(p1, vec3_s_mult(scale, tangent)); // p1 + scale * tangent

			quat a1 = bisect(v1, v2);

			// add p1
			controlPoints = list_insert(controlPoints, knots->elements[i], controlPoints->length);
			// add q1 and a1
			controlPoints = list_insert(controlPoints, (keyframe_t) {
					.time = 0,
					.camera = (camera_t) { .position = q1, .rotation = a1 }
					}, controlPoints->length);
		} else if (i == knots->length - 1) { // last
			vec3 p0 = knots->elements[i-1].camera.position;
			quat v0 = knots->elements[i-1].camera.rotation;
			vec3 p1 = knots->elements[i].camera.position;
			quat v1 = knots->elements[i].camera.rotation;

			vec3 tangent = vec3_sub(p1, p0); // (p1 - p0)
			vec3 q0 = vec3_sub(p1, vec3_s_mult(scale, tangent)); // p1 - scale * tangent

			quat b1 = bisect(v1, v0);

			// add q0 and b1
			controlPoints = list_insert(controlPoints, (keyframe_t) {
					.time = 0,
					.camera = (camera_t) { .position = q0, .rotation = b1 }
					}, controlPoints->length);
			// add p1
			controlPoints = list_insert(controlPoints, knots->elements[i], controlPoints->length);
		} else {
			vec3 p0 = knots->elements[i-1].camera.position;
			quat v0 = knots->elements[i-1].camera.rotation;
			vec3 p1 = knots->elements[i].camera.position;
			quat v1 = knots->elements[i].camera.rotation;
			vec3 p2 = knots->elements[i+1].camera.position;
			quat v2 = knots->elements[i+1].camera.rotation;

			vec3 tangent = vec3_normalize(vec3_sub(p2, p0)); // (p2 - p0).normalized
			vec3 q0 = vec3_sub(p1, vec3_s_mult(scale * vec3_length(vec3_sub(p1, p0)), tangent)); // p1 - scale * tangent * (p1 - p0).length
			vec3 q1 = vec3_add(p1, vec3_s_mult(scale * vec3_length(vec3_sub(p2, p1)), tangent)); // p1 + scale * tangent * (p2 - p1).length

			quat a1 = bisect(mirror(v0, v1), v2);
			quat b1 = mirror(a1, v1);


			// add q0 and b1
			controlPoints = list_insert(controlPoints, (keyframe_t) {
					.time = 0,
					.camera = (camera_t) { .position = q0, .rotation = b1 }
					}, controlPoints->length);
			// add p1
			controlPoints = list_insert(controlPoints, knots->elements[i], controlPoints->length);
			// add q1 and a1
			controlPoints = list_insert(controlPoints, (keyframe_t) {
					.time = 0,
					.camera = (camera_t) { .position = q1, .rotation = a1 }
					}, controlPoints->length);
		}
	}

	//curveCount = (controlPoints->length - 1) / 3;
	return controlPoints;
}
