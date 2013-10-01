#include <AntTweakBar.h>

#include <libzeuch/gl.h>

#include "window.h"

bool is_fullscreen = false;
bool ignore_next_resize = false;

int width, height;
int desktop_width = 0, desktop_height = 0;

void window_set_desktop_dim(int width, int height) {
	desktop_width = width;
	desktop_height = height;
}

int window_get_width(void) {
	return is_fullscreen ? desktop_width : width;
}

int window_get_height(void) {
	return is_fullscreen ? desktop_height : height;
}

bool window_is_fullscreen(void) {
	return is_fullscreen;
}

SDL_Surface* window_handle_resize(const bool fullscreen, int new_width, int new_height) {
	Uint32 flags =
		  SDL_HWSURFACE
		| SDL_DOUBLEBUF
		| SDL_OPENGL
		| SDL_RESIZABLE;
	if(fullscreen != is_fullscreen) {
		ignore_next_resize = true;
	}
	if(fullscreen) {
		new_width = desktop_width;
		new_height = desktop_height;
		flags |= SDL_FULLSCREEN;
	} else if(new_width == 0 && new_height == 0) {
		new_width = width;
		new_height = height;
	} else {
		width = new_width;
		height = new_height;
	}
	SDL_Surface* const screen = SDL_SetVideoMode(new_width, new_height, 32, flags);
	TwWindowSize(new_width, new_height);
	is_fullscreen = fullscreen;
	glViewport(0, 0, new_width, new_height);
	return screen;
}

bool window_should_ignore_next_resize(void) {
	return ignore_next_resize;
}

void window_dont_ignore_next_resize(void) {
	ignore_next_resize = false;
}
