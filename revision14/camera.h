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
		vec3_cubic_bezier(p0->position, p1->position, p2->position, p3->position, t),
		quat_cubic_bezier(p0->rotation, p1->rotation, p2->rotation, p3->rotation, t),
	};
	return camera;
}