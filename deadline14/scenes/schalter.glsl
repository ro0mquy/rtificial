#include "scene_head.glsl"
#include "rtificial.glsl"
#include "leitungen.glsl"
#line 5

uniform float pyramid_h; // float
uniform float pyramid_s; // float
uniform float pyramid_animation; // float
uniform float pyramid_wave_animation; // float
uniform float pyramid_bottom; // float
uniform vec3 pyramid_color; // color
uniform vec3 pyramid_color2; // color
uniform vec3 pyramid_color_leitungen; // color
uniform vec3 pyramid_color_boden; // color

Material materials[6] = Material[6](
	Material(vec3(1.), .5, 0.),
	Material(pyramid_color, 1., 1.),
	Material(pyramid_color_boden, .5, 0.),
	Material(vec3(.7, .3, .2), 1., 0.),
	Material(vec3(1.), .5, 0.),
	Material(pyramid_color_leitungen, 1., 0.)
);

const int MATERIAL_ID_BOUNDING  = 0;
const int MATERIAL_ID_PYRAMID   = 1;
const int MATERIAL_ID_FLOOR     = 2;
const int MATERIAL_ID_CUBE      = 3;
const int MATERIAL_ID_LIGHTBALL = 4;
const int MATERIAL_ID_LEITUNGEN = 5;

float pyramid(vec3 p, float s, float h);

vec3 p_pyramid;

vec3 apply_lights(vec3 p, vec3 N, vec3 V, Material mat) {
	vec3 color = vec3(0.);
	float radius = 2.;
	float intensity = 50.;
	color += apply_light(p, N, V, mat, SphereLight(
		vec3(5., 6., 3.),
		vec3(1.),
		radius,
		intensity));
	color += apply_light(p, N, V, mat, SphereLight(
		vec3(-3., -5., 4.),
		vec3(1.),
		radius,
		intensity));
	return color;
}

void main(void) {
	vec3 direction = get_direction();

	int i;
	vec3 hit = march(camera_position, direction, i);

	vec3 color = vec3(0.);
	float materialFloat = f(hit).y;
	int material = int(materialFloat);
	vec3 normal = calc_normal(hit);

	Material mat = materials[material];
	SphereLight light1 = SphereLight(vec3(5., 9., 10.), vec3(1.), 2., 100.);

	float factor = 1.;
	if(material == MATERIAL_ID_PYRAMID) {
		float t = vnoise(p_pyramid * .4) * cfbm(p_pyramid * 3.);
		mat.roughness = .5 + .2 * sign(t) * pow(abs(t), .7) + .3 * cnoise(p_pyramid * .2 + 726.);

		Material rust = Material(pyramid_color2, .9, .8);
		float rustiness = .1 + .06 * cnoise(p_pyramid * .5);
		rustiness *= 3.;
		color = apply_lights(hit, normal, -direction, rust) * rustiness;
		factor = 1. - rustiness;
	}

	if (MATERIAL_ID_LIGHTBALL == material || MATERIAL_ID_CUBE == material) {
		color = emit_light(materials[MATERIAL_ID_LIGHTBALL].color, 5. * (smoothstep(-6., 0., pdot(-direction, normal)) * 10));
	} else if (material >= MATERIAL_ID_FLOOR && material <= MATERIAL_ID_CUBE) {
		vec3 nonglowing_floor = factor * apply_lights(hit, normal, -direction, mat);
		vec3 glowing_floor = emit_light(mat.color, 5.);
		float wave_height = hit.y + 2.;
		vec3 color_floor = mix(nonglowing_floor, glowing_floor, smoothstep(0., 2., wave_height));

		vec3 color_cube = emit_light(materials[MATERIAL_ID_LIGHTBALL].color, 5.);

		color += mix(color_floor, color_cube, material - MATERIAL_ID_FLOOR);
	} else {
		color += factor * apply_lights(hit, normal, -direction, mat);
	}

	output_color(color, distance(hit, camera_position));
}

float pyramid(vec3 p, float s, float h){
	float alpha = atan(s/(2*h));
	// float alpha = s/(2*h*sqrt(s*s/(4*h*h)+1)

	vec3 n1 = vec3( cos(alpha), sin(alpha), 0.);
	vec3 n2 = vec3(-cos(alpha), sin(alpha), 0.);
	vec3 n3 = vec3(0., sin(alpha),  cos(alpha));
	vec3 n4 = vec3(0., sin(alpha), -cos(alpha));

	float pyr;
	pyr = -p.y - h;
	pyr = max(pyr, plane(p, n1));
	pyr = max(pyr, plane(p, n2));
	pyr = max(pyr, plane(p, n3));
	pyr = max(pyr, plane(p, n4));
	return pyr;
}

vec2 f(vec3 p) {
	vec3 q = rY(TAU * 0.05 * time) * p;
	//q = trans(q, 0., 1.*sin(0.75 * time), 0.);

	// double pyramid
	float pyramid_size = 10.;
	vec3 qq = q;
	qq = trans(qq, 0., pyramid_h*pyramid_size, 0.); // y-axis translation
	qq = trans(qq, 0., pyramid_bottom, 0.);

	float t_fall_closing = 1. - pow((max(0.1,min(.9, pyramid_animation))-0.)/.9, 2.);
	float t_fall_opening = pow((max(0.,min(.1, pyramid_animation))-0.)/.1, 2.);
	float t_fall = t_fall_opening*(1-step(0.1,pyramid_animation)) + t_fall_closing*step(0.1,pyramid_animation);

	float pyr_closed = 4. * t_fall;

	qq = trans(qq, 0., pyr_closed/2., 0.);

	float t_stampf = linstep(.9,1.,pyramid_animation);

	qq = trans(qq, 0.,  -  (1-pow(1.-t_stampf/2., 20.)), 0.);
	qq.y = -abs(qq.y);
	qq = trans(qq, 0., -pyr_closed/2., 0.); // pyramid distance
	qq.y = -pyramid_size*pyramid_h-qq.y;
	float pyr = pyramid(qq, pyramid_size * pyramid_s, pyramid_size * pyramid_h);
	float b = pyramid_size * pyramid_s * .5 * .9;
	pyr = smax(pyr, -roundbox(trans(qq, 0., -pyramid_size * pyramid_h, 0.), vec3(b, .1, b), .3), .05);
	p_pyramid = qq;
	vec2 pyramid = vec2(pyr, MATERIAL_ID_PYRAMID);

	// box-torus-morph
	q = trans(q, 0., pyramid_size* pyramid_h + 1., 0.);
	q = trans(q, 0., pyramid_bottom, 0.);
	// .2 fuer schale
	vec3 cube_dimension = vec3(smoothstep(0., 0.1, pyramid_animation) + .2);
	float cube = roundbox(q, cube_dimension, .5);
	float d = .75 + smoothstep(.70,1., pyramid_animation)*7.5;
	float down_anim = - (pyramid_size * pyramid_h + 3. + pyramid_bottom) * smoothstep(0.75, 1., pyramid_animation);
	float mytorus = torus(trans(q, 0.,-.5 + down_anim,0.), vec2(.5 + d, .5));
	float f_schalter_cube = vec2(mix(cube, mytorus, smoothstep(.65, .75, pyramid_animation)), MATERIAL_ID_CUBE);

	/*
	float torus_2_down = -(pyramid_size * pyramid_h + 2. + pyramid_bottom) - 2.*linstep(.5,1.,pyramid_wave_animation);
	float torus_2_d = (.75+7.5 ) + pyramid_wave_animation * 10;
	float torus_2_dicke = 0.5 - 0.5 * linstep(.5,1.,pyramid_wave_animation);
	float torus_2 = torus(trans(q, 0.,-.5 + torus_2_down,0.), vec2(.5 + torus_2_d, torus_2_dicke));
	torus_2 += 100 * (1. - step(0.001, pyramid_wave_animation));
	//vec2 schalter_cube = vec2(min(f_schalter_cube, torus_2), MATERIAL_ID_CUBE);
	*/
	vec2 schalter_cube = vec2(f_schalter_cube, MATERIAL_ID_CUBE);

	vec3 p_lb = trans(p, 0., -2., 0.);
	vec3 dr_factor = vec3(7.,10.,8.);
	vec3 p_lb_dr = domrepv(p_lb, dr_factor);
	p_lb_dr = trans(p_lb_dr, 0.,2*vnoise(floor(p_lb/dr_factor)),0.);
	float lightballs_dist = sphere(p_lb_dr, 2+0.75*vnoise(floor(p_lb/dr_factor)));
	float no_lightballs = sphere(p, 20.);
	vec2 lightballs = vec2(max(lightballs_dist, -no_lightballs), MATERIAL_ID_LIGHTBALL);

	float wave_position =
		+ 9. * step(.85, pyramid_animation)
		+ 1.5 * linstep(.9, 1., pyramid_animation)
		+ 10.5 * step(.00001, pyramid_wave_animation)
		+ 15. * pyramid_wave_animation
		;
	float wave_amplitude = 0.
		+ 2. * smoothstep(.87, .95, pyramid_animation)
		+ 2. * step(.00001, pyramid_wave_animation)
		- 2. * smoothstep(.00001, 1., pyramid_wave_animation)
		;
	float wave_width = 2.;

	vec3 p_boden = trans(p, 0., -2., 0.);
	float wave = length(p_boden.xz);
	wave -= wave_position;
	wave /= wave_width;
	wave = wave_amplitude / (wave * wave + 1.);

	p_boden.y -= wave;
	p_boden = trans(p_boden, 10., 0., 5.);
	float smoothing_leitungen = .5;
	float f_leitungen = 0.;
	f_leitungen = leitungen(p_boden, 0., .2, 1.7, 20.);
	f_leitungen = smin(f_leitungen, leitungen(p_boden, 80., .2, 27.3, 15.), smoothing_leitungen);
	f_leitungen = smin(f_leitungen, leitungen(p_boden, 20., .2, 549.2, 30.), smoothing_leitungen);
	vec2 m_leitungen = vec2(f_leitungen, MATERIAL_ID_LEITUNGEN);

	vec2 bottom = vec2(p_boden.y, MATERIAL_ID_FLOOR);
	vec2 m_cube_bottom = smin_smaterial(schalter_cube, bottom, 3.);
	vec2 m_cube_bottom_leitungen = min_material(m_cube_bottom, m_leitungen);

	vec2 bounding = vec2(-sphere(p - camera_position, 50.), MATERIAL_ID_BOUNDING);
	return min_material(m_cube_bottom_leitungen, min_material(lightballs, min_material(pyramid, bounding)));
}
