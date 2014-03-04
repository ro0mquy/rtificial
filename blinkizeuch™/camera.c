#include <math.h>

#include "camera.h"

static void camera_move(
	camera_t* const camera,
	const vec3 v
);

static void camera_rotate(camera_t* const camera, const vec3 v, const float angle);

void camera_init(camera_t* const camera) {
	camera->position = vec3_new(0., 0., 0.);
	camera->rotation = quat_new(vec3_new(0., 0., 0.), 1.);
}

void camera_update_uniforms(
	const camera_t* const camera,
	const GLint uniform_position,
	const GLint uniform_direction,
	const GLint uniform_up
) {
	const vec3 pos = camera->position;
	const vec3 dir = quat_rotate_vec3(camera->rotation, vec3_new(0., 0., -1));
	const vec3 up  = quat_rotate_vec3(camera->rotation, vec3_new(0., 1., 0.));
	glUniform3f(uniform_position , pos.x, pos.y, pos.z);
	glUniform3f(uniform_direction, dir.x, dir.y, dir.z);
	glUniform3f(uniform_up       , up.x , up.y , up.z );
}

void camera_rotate_x(
	camera_t* const camera,
	const float angle
) {
	camera_rotate(camera, vec3_new(1., 0. ,0.), angle);
}

void camera_rotate_y(
	camera_t* const camera,
	const float angle
) {
	camera_rotate(camera, vec3_new(0., 1. ,0.), angle);
}

void camera_rotate_z(
	camera_t* const camera,
	const float angle
) {
	camera_rotate(camera, vec3_new(0., 0., -1.), angle);
}

static void camera_rotate(camera_t* const camera, const vec3 v, const float angle) {
	const float half_angle = .5 * angle;
	quat rot = quat_new(
		vec3_s_mult(sin(half_angle), v),
		cos(half_angle)
	);
	camera->rotation = quat_mult(camera->rotation, rot);
	// maybe normalize after a few rotations
}

void camera_move_x(
	camera_t* const camera,
	const float distance
) {
	camera_move(camera, vec3_new(distance, 0., 0.));
}

void camera_move_y(
	camera_t* const camera,
	const float distance
) {
	camera_move(camera, vec3_new(0., distance, 0.));
}

void camera_move_z(
	camera_t* const camera,
	const float distance
) {
	camera_move(camera, vec3_new(0., 0., distance));
}

static void camera_move(
	camera_t* const camera,
	const vec3 v
) {
	camera->position = vec3_add(camera->position, quat_rotate_vec3(camera->rotation, v));
}

quat camera_get_rotation(camera_t* const camera) {
	return camera->rotation;
}

void camera_set_rotation(camera_t* const camera, quat rotation) {
	camera->rotation = rotation;
}

camera_t camera_lerp(const camera_t* const start, const camera_t* const end, float t) {
	camera_t camera = {
		.position = vec3_lerp(start->position, end->position, t),
		.rotation = quat_slerp(start->rotation, end->rotation, t),
	};
	return camera;
}

camera_t camera_cubic_bezier(const camera_t* const p0, const camera_t* const p1, const camera_t* const p2, const camera_t* const p3, float t) {
	camera_t camera = {
		.position = vec3_cubic_bezier(p0->position, p1->position, p2->position, p3->position, t),
		.rotation = quat_cubic_bezier(p0->rotation, p1->rotation, p2->rotation, p3->rotation, t),
	};
	return camera;
}

mat4x4 camera_world_to_camera_matrix(camera_t* const camera) {
	mat4x4 translation = mat4x4_new((float[4][4]) {
			{ 1, 0, 0, camera->position.x },
			{ 0, 1, 0, camera->position.y },
			{ 0, 0, 1, camera->position.z },
			{ 0, 0, 0,                  1 },
			});
	mat4x4 rotation = quat_to_mat4x4(camera->rotation);
	return mat4x4_mult(rotation, translation);
}
