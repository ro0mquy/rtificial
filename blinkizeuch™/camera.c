#include "camera.h"

static void camera_move(
	camera_t* const camera,
	const vec3 movement,
	const float distance
);

void camera_init(camera_t* const camera) {
	camera->position  = vec3_new(0., 0., 0.);
	camera->direction = vec3_new(0., 0.,-1.);
	camera->up        = vec3_new(0., 1., 0.);
	camera->right     = vec3_new(1., 0., 0.);
}

void camera_update_uniforms(
	const camera_t* const camera,
	const GLint uniform_position,
	const GLint uniform_direction,
	const GLint uniform_up
) {
	const vec3* const pos = &camera->position;
	const vec3* const dir = &camera->direction;
	const vec3* const up  = &camera->up;
	glUniform3f(uniform_position , pos->x, pos->y, pos->z);
	glUniform3f(uniform_direction, dir->x, dir->y, dir->z);
	glUniform3f(uniform_up       , up->x , up->y , up->z );
}

void camera_rotate_x(
	camera_t* const camera,
	const float angle
) {
	camera->direction = vec3_rotate(camera->direction, camera->right, angle);
	camera->up        = vec3_rotate(camera->up       , camera->right, angle);
}

void camera_rotate_y(
	camera_t* const camera,
	const float angle
) {
	camera->direction = vec3_rotate(camera->direction, camera->up, angle);
	camera->right     = vec3_rotate(camera->right    , camera->up, angle);
}

void camera_rotate_z(
	camera_t* const camera,
	const float angle
) {
	camera->right     = vec3_rotate(camera->right    , camera->direction, angle);
	camera->up        = vec3_rotate(camera->up       , camera->direction, angle);
}

void camera_move_x(
	camera_t* const camera,
	const float distance
) {
	camera_move(camera, camera->right, distance);
}

void camera_move_y(
	camera_t* const camera,
	const float distance
) {
	camera_move(camera, camera->up, distance);
}

void camera_move_z(
	camera_t* const camera,
	const float distance
) {
	camera_move(camera, camera->direction, -distance);
}

static void camera_move(
	camera_t* const camera,
	const vec3 direction,
	const float distance
) {
	camera->position = vec3_add(camera->position, vec3_s_mult(distance, direction));
}
