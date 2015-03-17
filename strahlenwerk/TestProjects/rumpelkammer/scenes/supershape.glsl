#include "rtificial.glsl"
#line 3

out vec4 out_color;

void main() {
	vec3 o = camera_position;
	float screenDist;
	vec3 d = get_direction(screenDist);
	float t = march(o, d, 100., screenDist);

	if (isinf(t)) {
		out_color.rgb = vec3(0.);
	} else {
		out_color.rgb = vec3(max(dot(calc_normal(o + t * d, false), normalize(vec3(1., 2., .5))), 0.) + .1);
	}
}

vec2 f(vec3 p, bool last_step) {
	const float a  = super_a_rt_float;
	const float b  = super_b_rt_float;
	const float m  = super_m_rt_float;
	const float n1 = super_n1_rt_float;
	const float n2 = super_n2_rt_float;
	const float n3 = super_n3_rt_float;

	p.yz -= -2.;
	float f_super = supershape(p.xz, a, b, m, n1, n2, n3);
	f_super = max(f_super, abs(p.y) - 1.);

	float f_super2 = supershape(p.xy, a, b, m, n1, n2, n3);

	float f = smax(f_super, f_super2, super_smooth_rt_float);
	return vec2(f, 0.);
}
