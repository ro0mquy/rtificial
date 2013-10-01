#include <stdlib.h>
#include <math.h>

#include <libzeuch/shader.h>

#include "timeline.h"
#include "timeline_vertex.h"
#include "timeline_fragment.h"

typedef struct {
	GLfloat x, y, w, h;
	GLfloat r, g, b, a;
} rect_t;

static void draw_rect(const timeline_t* timeline, const rect_t* rect);

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
	*timeline = (timeline_t) {
		.program = program,
		.attribute_coord2d = attribute_coord2d,
		.uniform_color = uniform_color,
		.zoom = 1.,
	};
	return timeline;
}

void timeline_draw(timeline_t* const timeline) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(timeline->program);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(timeline->attribute_coord2d);

	const GLfloat height = .2;

	// actual drawing begins here
	draw_rect(timeline, &(rect_t) {
		.x = 0., .y = 0., .w = 1., .h = height,
		.r = 1., .g = 1., .b = 1., .a = .5,
	});

	const int segments = 20 * timeline->zoom;
	const GLfloat segment_width = 1. / segments;
	for(int i = 0; i < ceil((double) segments / 2); i++) {
		draw_rect(timeline, &(rect_t) {
			.x = 2 * i * segment_width, .y = 0., .w = segment_width, .h = height,
			.r = 0., .g = 0., .b = 0., .a = .5,
		});
	}

	glDisableVertexAttribArray(timeline->attribute_coord2d);
	glDisable(GL_BLEND);
}

void timeline_destroy(timeline_t* const timeline) {
	glDeleteProgram(timeline->program);
}

bool timeline_handle_sdl_event(timeline_t* const timeline, const SDL_Event* const event) {
	const float factor = .9;
	if(event->type == SDL_MOUSEBUTTONDOWN) {
		if(event->button.button == SDL_BUTTON_WHEELUP) {
			timeline->zoom *= factor;
			return true;
		} else if( event->button.button == SDL_BUTTON_WHEELDOWN) {
			timeline->zoom /= factor;
			return true;
		}
	}
	return false;
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
