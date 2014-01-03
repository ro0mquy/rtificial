#ifndef FLIGHT_H
#define FLIGHT_H

#include <stdbool.h>

#include "camera.h"

typedef struct flight_t {
	// points for cubic bezier curves
	camera_t* p0; // start
	camera_t* p1;
	camera_t* p2;
	camera_t* p3; // end
	int start_time;
	int duration;
} flight_t;

flight_t flight_new(const camera_t* p0, const camera_t* p1, const camera_t* p2, const camera_t* p3, const int start_time, const int duration);

camera_t flight_get_camera(const flight_t* const flight, const int time);

bool flight_is_finished(const flight_t* const flight, const int time);

#endif
