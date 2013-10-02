#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <libzeuch/shader.h>

#include "window.h"
#include "util.h"

#include "timeline.h"
#include "timeline_vertex.h"
#include "timeline_fragment.h"

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
static keyframe_t* list_get(keyframe_list_t* list, size_t position);
static size_t list_find(keyframe_list_t* list, int time);

const GLfloat timeline_height = .2;
const int segment_length = 10000;
const int num_segments = 20;
const GLfloat marker_width = .003;
const GLfloat cursor_width = .003;

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
	keyframe_list_t* const list = malloc(sizeof(keyframe_list_t) + sizeof(keyframe_t));
	if(list == NULL) {
		glDeleteProgram(program);
		return NULL;
	}
	*list = (keyframe_list_t) {
		.length = 0,
		.allocated = 1,
	};

	*timeline = (timeline_t) {
		.program = program,
		.attribute_coord2d = attribute_coord2d,
		.uniform_color = uniform_color,
		.zoom = 1.,
		.list = list,
		.hidden = false,
		.cursor_position = 0,
		.is_playing = false,
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
	const int segments = num_segments * zoom;
	const GLfloat segment_width = 1. / segments;
	for(int i = 0; i < ceil((double) segments / 2); i++) {
		draw_rect(timeline, &(rect_t) {
			.x = 2 * i * segment_width, .y = 0., .w = segment_width, .h = timeline_height,
			.r = 0., .g = 0., .b = 0., .a = .5,
		});
	}

	for(size_t i = 0; i < timeline->list->length; i++) {
		const int time = list_get(timeline->list, i)->time;
		const GLfloat x = (float) time / ((int) (num_segments * zoom) * segment_length) - marker_width / zoom / 2;
		draw_rect(timeline, &(rect_t) {
			.x = x, .y = 0., .w = marker_width / zoom, .h = timeline_height,
			.r = 1., .g = 0., .b = 0., .a = 1.,
		});
	}
	const GLfloat x = (float) timeline->cursor_position / ((int) (num_segments * zoom) * segment_length) - cursor_width / zoom / 2;
	draw_rect(timeline, &(rect_t) {
		.x = x, .y = 0., .w = cursor_width / zoom, .h = timeline_height,
		.r = 0., .g = 1., .b = 0., .a = 1.,
	});

	glDisableVertexAttribArray(timeline->attribute_coord2d);
	glDisable(GL_BLEND);
}

void timeline_destroy(timeline_t* const timeline) {
	util_safe_free(timeline->list);
	glDeleteProgram(timeline->program);
}

bool timeline_handle_sdl_event(timeline_t* const timeline, const SDL_Event* const event) {
	const float factor = .9;
	if(event->type == SDL_MOUSEBUTTONDOWN) {
		if(1. - ((float) event->button.y / window_get_height()) > timeline_height) {
			return false;
		}
		const float horizontal = ((float) event->button.x / window_get_width());
		switch(event->button.button) {
			case SDL_BUTTON_WHEELUP:
				timeline->zoom *= factor;
				return true;
			case SDL_BUTTON_WHEELDOWN:
				timeline->zoom /= factor;
				return true;
			case SDL_BUTTON_LEFT:
				timeline->cursor_position = horizontal * ((int) (num_segments * timeline->zoom) * segment_length);
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
			default:
				break;
		}
	}
	return false;
}

void timeline_add_frame(timeline_t* const timeline, const camera_t camera) {
	const int time = timeline->cursor_position;
	const size_t pos = list_find(timeline->list, time);
	timeline->list = list_insert(timeline->list, (keyframe_t) {
		.time = time,
		.camera = camera,
	}, pos);
}

void timeline_update(timeline_t* timeline, int dtime) {
	if(timeline->is_playing) {
		timeline->cursor_position += dtime;
	}
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
