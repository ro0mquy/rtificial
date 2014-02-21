#ifndef TIMELINE_H
#define TIMELINE_H

#include <stdbool.h>

#include <SDL.h>

#include <libzeuch/gl.h>

#include "camera.h"
#include "font.h"

struct keyframe_list_t;

typedef struct {
	GLuint program;
	GLint attribute_coord2d;
	GLint uniform_color;
	font_t font;
	float zoom;
	int focus_position;
	struct keyframe_list_t* keyframes;
	struct keyframe_list_t* controlPoints;
	bool hidden;
	int cursor_position;
	bool is_playing;
	bool camera_changed;
} timeline_t;

typedef struct {
	int time;
	camera_t camera;
} keyframe_t;

timeline_t* timeline_new(void);

void timeline_draw(timeline_t* timeline);

void timeline_destroy(timeline_t* timeline);

bool timeline_handle_sdl_event(timeline_t* timeline, const SDL_Event* event);

void timeline_add_frame(timeline_t* timeline, camera_t camera);

void timeline_remove_frame(timeline_t* const timeline);

void timeline_update(timeline_t* timeline, int dtime);

bool timeline_camera_changed(const timeline_t* timeline);

camera_t timeline_get_camera(timeline_t* timeline);

#endif
