typedef struct camera_t {
	vec3 position;
	quat rotation;
} camera_t;

static void camera_update_uniforms(
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

static camera_t camera_cubic_bezier(const camera_t* const p0, const camera_t* const p1, const camera_t* const p2, const camera_t* const p3, float t) {
	camera_t camera = {
		.position = vec3_cubic_bezier(p0->position, p1->position, p2->position, p3->position, t),
		.rotation = quat_cubic_bezier(p0->rotation, p1->rotation, p2->rotation, p3->rotation, t),
	};
	return camera;
}

static mat4x4 camera_world_to_camera_matrix(camera_t* const camera) {
	mat4x4 translation = mat4x4_new((float[4][4]) {
			{ 1, 0, 0, camera->position.x },
			{ 0, 1, 0, camera->position.y },
			{ 0, 0, 1, camera->position.z },
			{ 0, 0, 0,                  1 },
			});
	mat4x4 rotation = quat_to_mat4x4(camera->rotation);
	return mat4x4_mult(rotation, translation);
}
