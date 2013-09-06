#ifndef CAMERA_H
#define CAMERA_H

#include <libzeuch/gl.h>
#include <libzeuch/vector.h>
#include <libzeuch/quaternion.h>


typedef struct camera_t {
	vec3 position;
	quat rotation;
} camera_t;

void camera_init(camera_t* const camera);

void camera_update_uniforms(
	const camera_t* const camera,
	const GLint uniform_position,
	const GLint uniform_direction,
	const GLint uniform_up
);

void camera_rotate_x(
	camera_t* const camera,
	const float angle
);

void camera_rotate_y(
	camera_t* const camera,
	const float angle
);

void camera_rotate_z(
	camera_t* const camera,
	const float angle
);

void camera_move_x(
	camera_t* const camera,
	const float distance
);

void camera_move_y(
	camera_t* const camera,
	const float distance
);

void camera_move_z(
	camera_t* const camera,
	const float distance
);

quat camera_get_rotation(camera_t* const camera);

void camera_set_rotation(camera_t* const camera, quat rotation);

camera_t camera_lerp(const camera_t* const start, const camera_t* const end, float t);

#endif // CAMERA_H
