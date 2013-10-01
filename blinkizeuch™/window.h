#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>

#include <SDL.h>

bool window_is_fullscreen(void);

int window_get_width(void);

int window_get_height(void);

void window_set_desktop_dim(int width, int height);

SDL_Surface* window_handle_resize(bool fullscreen, int new_width, int new_height);

bool window_should_ignore_next_resize();

void window_dont_ignore_next_resize(void);

#endif
