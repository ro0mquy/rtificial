#include "rtificial.glsl"
#line 3

out vec4 out_color;

float sdf(vec3);

void main() {
	vec3 o = camera_position;
	float screenDist;
	vec3 d = get_direction(screenDist);
	float t = march(o, d, 200., screenDist);

	if (isinf(t)) {
		out_color.rgb = vec3(0.);
	} else {
		vec3 hit = o + t * d;
		vec3 normal = calc_normal(hit, false);
		float material = f(hit, false)[1];

		out_color.rgb = vec3(max(dot(normal, normalize(vec3(1., .5, 0.))), 0.) + .1);
		out_color.rgb = .5 * normal + .5;

		if (material == 42.) {
			float dist = f(hit, true);
			dist = abs(sin(10. * dist));
			out_color.rgb = exp(-.1 * t) * dist * kk_color_rt_color;//vec3(.9, 1., .9);
		}
	}
}

vec2 f(vec3 p, bool last_step) {
	vec2 m_f = vec2(sdf(p), 0.);

	if (!last_step) {
		float f_debug = p.y - debug_height_rt_float;
		vec2 m_debug = vec2(f_debug, 42.);
		m_f = min_material(m_f, m_debug);
	}

	return m_f;
}

float sdf(vec3 p) {
	float f = torus(p.xzy, kk_torus_dim_rt_vec2);
	return f;
}
