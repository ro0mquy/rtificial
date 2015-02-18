#include "rtificial.glsl"
#line 3

out vec4 out_color;

void main() {
	vec3 o = camera_position;
	float screenDist;
	vec3 d = get_direction(screenDist);
	float t = march(o, d, 50., screenDist);

	if (isinf(t)) {
		out_color.rgb = vec3(0.);
	} else {
		out_color.rgb = vec3(max(dot(calc_normal(o + t * d, false), normalize(vec3(1., .5, 2.))), 0.) + .1);
	}
}

float expStep(float x, float k, float n) {
    return exp(-k * pow(x, n));
}

vec2 f(vec3 p, bool last_step) {
	vec3 p_body = p;

	//float body_deform_rt_float = smoothstep(0., 1., body_deform_rt_float);

	float ring_dist = mix(1., 16., smoothstep(body_rings_rt_float, body_rings_rt_float + 30., p_body.z));
	float t = clamp(p_body.z + body_delle_trans_rt_float, 0., 2. / .4);
	float u = abs(mod(t * 3. + ring_dist * .5, ring_dist) * 2. - ring_dist);
	float dellen = 1. - expStep(u, 20., 5.);

	p_body.z -= body_deform_rt_float * (1. - pow(min(length(p_body.xy), 1.), 1.47)) * .6;
	p_body.z *= mix(1., .4, body_deform_rt_float);

	float d_body = sphere(p_body, 1. - dellen * .03);

	float d = d_body;

	return vec2(d, .0);
}
