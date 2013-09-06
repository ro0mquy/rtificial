#include "flight.h"

static float flight_get_t(const flight_t* const flight, const int time);

flight_t flight_new(const camera_t start, const camera_t end, const int start_time, const int duration) {
	flight_t flight = {
		.start = start,
		.end = end,
		.start_time = start_time,
		.duration = duration,
	};
	return flight;
}

camera_t flight_get_camera(const flight_t* const flight, const int time) {
	return camera_lerp(&flight->start, &flight->end, flight_get_t(flight, time));
}

bool flight_is_finished(const flight_t* const flight, const int time) {
	return flight_get_t(flight, time) >= 1.;
}

static float flight_get_t(const flight_t* const flight, const int time) {
	return (float) (time - flight->start_time) / flight->duration;
}
