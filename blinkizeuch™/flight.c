#include "flight.h"

static float flight_get_t(const flight_t* const flight, const int time);
static float flight_min(float a, float b);

flight_t flight_new(camera_t* p0, camera_t* p1, camera_t* p2, camera_t* p3, const int start_time, const int duration) {
	flight_t flight = {
		// points for bezier curves
		.p0 = p0, // start
		.p1 = p1,
		.p2 = p2,
		.p3 = p3, // end
		.start_time = start_time,
		.duration = duration,
	};
	return flight;
}

//camera_t flight_get_camera(const flight_t* const flight, const int time) {
//	return camera_lerp(flight->p0, flight->p3, flight_min(flight_get_t(flight, time), 1.));
//}
camera_t flight_get_camera(const flight_t* const flight, const int time) {
	return camera_cubic_bezier(flight->p0, flight->p1, flight->p2, flight->p3, flight_min(flight_get_t(flight, time), 1.));
}

bool flight_is_finished(const flight_t* const flight, const int time) {
	return flight_get_t(flight, time) >= 1.;
}

static float flight_get_t(const flight_t* const flight, const int time) {
	return (float) (time - flight->start_time) / flight->duration;
}

static float flight_min(float a, float b) {
	return (a < b) ? a : b;
}
