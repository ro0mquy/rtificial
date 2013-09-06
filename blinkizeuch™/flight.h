#ifndef FLIGHT_H
#define FLIGHT_H

#include <stdbool.h>

#include "camera.h"

typedef struct flight_t {
	camera_t start;
	camera_t end;
	int start_time;
	int duration;
} flight_t;

flight_t flight_new(const camera_t start, const camera_t end, const int start_time, const int duration);

camera_t flight_get_camera(const flight_t* const flight, const int time);

bool flight_is_finished(const flight_t* const flight, const int time);

#endif
