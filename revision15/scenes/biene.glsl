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
		out_color.rgb = vec3(max(dot(calc_normal(o + t * d, false), normalize(vec3(1., .5, 0.))), 0.) + .1);
	}
}

vec2 bee_body(vec3 p, bool last_step) {
	vec3 p_thorax = p;
	p_thorax.z /= biene_body_thorax_stretch_rt_float;
	float thorax_radius = biene_body_max_thick_rt_float * biene_body_thorax_scale_rt_float;
	float thorax_length = thorax_radius * biene_body_thorax_stretch_rt_float;
	float d_thorax = sphere(p_thorax, thorax_radius);
	d_thorax *= min(biene_body_thorax_stretch_rt_float, 1.);

	vec3 p_head = p;
	float head_radius = thorax_radius * biene_body_head_radius_rt_float;
	float head_length = head_radius * biene_body_head_stretch_rt_float;
	p_head.z += thorax_length + head_length;
	p_head.z /= biene_body_head_stretch_rt_float;
	float d_head = sphere(p_head, head_radius);
	d_head *= min(biene_body_head_stretch_rt_float, 1.);

	vec3 p_abdomen = p;
	float abdomen_length = biene_body_length_rt_float + .5 * biene_body_radius_rt_float;
	p_abdomen.z -= (abdomen_length + thorax_length) * (1. + biene_body_abdomen_offset_rt_float);
	float t_body = clamp(p_abdomen.z / (biene_body_length_rt_float + .5 * biene_body_radius_rt_float), -1., 1.);
	p_abdomen.zy *= rot2D(radians(biene_body_bend_rt_float) * (t_body * .5 + .5));
	t_body = clamp(p_abdomen.z / (biene_body_length_rt_float + .5 * biene_body_radius_rt_float), -1., 1.);
	p_abdomen.xy /= (biene_body_max_thick_rt_float - biene_body_min_thick_rt_float) * (1. - t_body * t_body) + biene_body_min_thick_rt_float;

	vec3 a = vec3(0., 0., biene_body_length_rt_float);
	vec3 b = -a;
	float d_abdomen = line(p_abdomen - a, b - a, biene_body_radius_rt_float);

	d_abdomen *= min(biene_body_min_thick_rt_float, 1.);

	vec3 p_dellen = p_abdomen;
	//p_dellen.z = domrep(p_dellen.z, biene_dellen_space_rt_float);
	p_dellen.z = squarerep(p_dellen.z, biene_dellen_space_rt_float, biene_dellen_anim_rt_float);
	float d_dellen = max(
		cylinder(p_dellen.xy, biene_body_radius_rt_float * (1. + biene_dellen_depth_rt_float)),
		abs(p_dellen.z) - biene_dellen_thick_rt_float
	);
	d_dellen = max(d_dellen, abs(p_abdomen.z) - biene_body_length_rt_float);

	d_abdomen = smin(d_abdomen, d_dellen, biene_dellen_smooth_rt_float * biene_dellen_depth_rt_float);

	float d_body = smin(d_abdomen, smin(d_thorax, d_head, biene_body_head_smooth_rt_float * head_radius),
		biene_body_abdomen_smooth_rt_float * biene_body_max_thick_rt_float);

	return vec2(d_body, 0.);
}

vec2 bee_fluegel(vec3 p, bool last_step) {
	vec3 p_fluegel = p;
	p_fluegel.x = abs(p_fluegel.x);
	p_fluegel.zx *= rot2D(radians(1.) * biene_fluegel_rot_rt_float);
	p_fluegel.yz *= rot2D(radians(1.) * biene_fluegel_angle_rt_float);
	p_fluegel.yx *= rot2D(radians(1.) * biene_fluegel_raise_rt_float);
	p_fluegel.x -= biene_fluegel_start_rt_float;
	vec2 c = normalize(biene_fluegel_c_rt_vec2);
	float d_fluegel = cone(vec3(p_fluegel.z, p_fluegel.y, -p_fluegel.x), c);
	float fluegel_length = biene_fluegel_length_rt_float;
	float fluegel_thick = biene_fluegel_thick_rt_float;
	d_fluegel = smax(d_fluegel, cylinder(p_fluegel.xz - vec2(fluegel_length, 0.), fluegel_length),
		biene_fluegel_end_smooth_rt_float * fluegel_thick); // cap ends
	d_fluegel = smax(d_fluegel, abs(p_fluegel.y) - fluegel_thick,
		biene_fluegel_thick_smooth_rt_float * fluegel_thick); // cap bottom/top
	return vec2(d_fluegel, 0.);
}

vec2 f(vec3 p, bool last_step) {
	vec2 body = bee_body(p, last_step);
	vec2 fluegel = bee_fluegel(p, last_step);
	return smin_material(body, fluegel, biene_fluegel_smooth_rt_float * biene_fluegel_thick_rt_float);
}

/*
float expStep( float x, float k, float n ) {
    return exp( -k*pow(x,n) );
}

vec2 fluegel(vec3 p, bool last_step) {
	p.x = abs(p.x);
	p.y /= .1;
	vec2 c = normalize(vec2(2., .4));
	float d = cone(vec3(p.z, p.y, -p.x), c);
	d = max(d , cylinder(p.xz - vec2(3.5, 0.), 3.));
	d *= .1;
	return vec2(d, 0.);
}

vec2 body(vec3 p, bool last_step) {
	vec3 p_body = p;
	float t = clamp(p_body.z, -1. / .4, 1. / .4);
	float dellen = expStep(abs(fract(t * 3.) * 2. - 1.), 20., 5.);
	p_body.z -= (1. - pow(min(length(p_body.xy), 1.), 1.47)) * .6;
	p_body.z *= .4;
	float d_body = sphere(p_body, 1. - dellen * .03);
	return vec2(d_body, 0.);
}

vec2 f(vec3 p, bool last_step) {
	float d = body(p, last_step)[0];
	return vec2(d, 0.);
}
*/

/*
vec2 f(vec3 p, bool last_step) {
	float scale = (p.z < 0.) ? .5: 1.;
	p.z /= (1. + scale * smoothstep(0., 4., p.x));
	float d = cylinder(p.yz, 1.);
	d = max(d, abs(p.y) - .1);
	d = max(d, -p.x);
	return vec2(d, 0.);
}
*/

/*
vec2 f(vec3 p, bool last_step) {
	float d;
	vec3 p_fluegel = p;
	p_fluegel.y /= .1;
	float scale = (p.z < 0.) ? .6: .3;
	p_fluegel.z /= 1. + scale * smoothstep(.0, 5., p_fluegel.x);
	p_fluegel.z /= 1. - scale * smoothstep(5., 7., p_fluegel.x);
	d = cylinder(p_fluegel.yz, 1.);
	d *= .1;
	d = max(d, -p.x);
	d = max(d, sphere(p, 6.5));
	return vec2(d, 0.);
}
*/


/*
vec2 f(vec3 p, bool last_step) {
	vec3 p_body = p;
	float t = clamp(p_body.z, -1. / .4, 1. / .4);
	float dellen = expStep(abs(fract(t * 3.) * 2. - 1.), 20., 5.);
	p_body.z -= (1. - pow(min(length(p_body.xy), 1.), 1.47)) * .6;
	p_body.z *= .4;
	float d_body = sphere(p_body, 1. - dellen * .03);

	vec3 p_fluegel = p;
	p_fluegel.x = abs(p_fluegel.x);
	p_fluegel.x -= 1.2;
	p_fluegel.xy *= rot2D(radians(30.));
	p_fluegel.xz *= rot2D(radians(20.));
	p_fluegel.y -= .5;
	p_fluegel.z /= 2.3;
	float d_fluegel = length(p_fluegel.xz) - 1.;
	p_fluegel.y = abs(p_fluegel.y);
	float biegung = 3.;
	p_fluegel.y += biegung - .2;
	d_fluegel = max(d_fluegel, sphere(p_fluegel, biegung));

	vec3 p_kopf = p;
	p_kopf.z += 2.2;
	float kopf_radius = .7;
	float d_kopf = sphere(p_kopf, kopf_radius);
	vec3 p_fuelerchen = p_kopf; // ordentlich fuel
	p_fuelerchen.z += .4;
	p_fuelerchen.y -= .35;
	float t_fueler = clamp(-p_fuelerchen.z, 0., 1.);
	p_fuelerchen.yz *= rot2D(radians(20.) * p_fuelerchen.z);
	p_fuelerchen.x = abs(p_fuelerchen.x);
	p_fuelerchen.x -= .3;
	p_fuelerchen.xz *= rot2D(radians(-30.) * t_fueler);
	float d_fuelerchen = line(p_fuelerchen, vec3(0., 0., -1.), .1);
	d_kopf = smin(d_kopf, d_fuelerchen, .1);

	float d = min(d_body, d_fluegel);
	d = min(d, d_kopf);

	return vec2(d, .0);
}
*/
