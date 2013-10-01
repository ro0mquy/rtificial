#ifndef TIMELINE_H
#define TIMELINE_H

#include <stdbool.h>

#include <SDL.h>

#include <libzeuch/gl.h>

typedef struct {
	GLuint program;
	GLint attribute_coord2d;
	GLint uniform_color;
	float zoom;
} timeline_t;

timeline_t* timeline_new(void);

void timeline_draw(timeline_t* timeline);

void timeline_destroy(timeline_t* timeline);

bool timeline_handle_sdl_event(timeline_t* timeline, const SDL_Event* event);

#endif
