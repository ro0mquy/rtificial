#ifndef TIMELINE_H
#define TIMELINE_H

#include <stdbool.h>

#include <SDL.h>

#include <libzeuch/gl.h>

#include "camera.h"

struct keyframe_list_t;

typedef struct {
	GLuint program;
	GLint attribute_coord2d;
	GLint uniform_color;
	float zoom;
	struct keyframe_list_t* list;
} timeline_t;

typedef struct {
	int time;
	camera_t camera;
} keyframe_t;

timeline_t* timeline_new(void);

void timeline_draw(timeline_t* timeline);

void timeline_destroy(timeline_t* timeline);

bool timeline_handle_sdl_event(timeline_t* timeline, const SDL_Event* event);

#endif
