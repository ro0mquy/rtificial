#include "scene_head.glsl"
#include "rtificial.glsl"
#line 4

uniform float pyramid_h; // float
uniform float pyramid_s; // float
uniform float pyramid_animation; // float

Material materials[] = Material[](
	Material(vec3(.1, .1, .1), .5, 0.),
	Material(vec3(1.), .5, 0.),
	Material(vec3(.1,.3,.1), 1., 0.),
	Material(vec3(.7, .3, .2), 1., 0.),
	Material(vec3(1.), .5,0.)
);

#define MATERIAL_ID_FLOOR 0.
#define MATERIAL_ID_BOUNDING 1.
#define MATERIAL_ID_PYRAMID 2.
#define MATERIAL_ID_CUBE 3.
#define MATERIAL_ID_LIGHTBALL 4.

float pyramid(vec3 p, float s, float h);
float star(vec2 p, float num_wings, float thickness_wings);

void main(void) {
	vec3 direction = get_direction();

	int i;
	vec3 hit = march(camera_position, direction, i);

	vec3 color = vec3(0.);
	if(i < 150) {
		int material = int(f(hit).y);
		vec3 normal = calc_normal(hit);

		Material mat = materials[material];
		if (material == MATERIAL_ID_FLOOR) {
			vec3 col = vec3(0.);
			float f_col = 0.;

			float num_wings = 16.;
			float thickness_wings = .003;
			vec3 hit = hit;
			hit.xz = rot2D(.5 + TAU / 200. * vnoise(hit.xz * .5)) * hit.xz;

			f_col = max(f_col, star(rot2D(0.0) * (hit.xz - vec2(  0.,   0.)), num_wings, thickness_wings));
			f_col = max(f_col, star(rot2D(0.5) * (hit.xz - vec2( 10.,  19.)), num_wings, thickness_wings));
			f_col = max(f_col, star(rot2D(1.0) * (hit.xz - vec2(-10.,  15.)), num_wings, thickness_wings));
			f_col = max(f_col, star(rot2D(1.5) * (hit.xz - vec2(  7., -19.)), num_wings, thickness_wings));
			f_col = max(f_col, star(rot2D(2.0) * (hit.xz - vec2(- 2., - 9.)), num_wings, thickness_wings));
			f_col = max(f_col, star(rot2D(2.5) * (hit.xz - vec2(- 5.,   4.)), num_wings, thickness_wings));
			f_col = max(f_col, star(rot2D(3.0) * (hit.xz - vec2(-17., - 3.)), num_wings, thickness_wings));
			f_col = max(f_col, star(rot2D(3.5) * (hit.xz - vec2( 14., -13.)), num_wings, thickness_wings));
			f_col = max(f_col, star(rot2D(4.0) * (hit.xz - vec2( 22.,   7.)), num_wings, thickness_wings));

			f_col = max(f_col, star(rot2D(.5 + TAU / 100. * vnoise(hit.xz * .5)) * (hit.xz - vec2(- 7.,   4.)), num_wings/2., thickness_wings));

			col = vec3(f_col);
			mat.color = col;
		}

		color = apply_light(hit, normal, -direction, mat, SphereLight(vec3(5., 9., 10.), vec3(1.), 2., 100.));
	}

	output_color(color, 4.1);// distance(hit, camera_position));
}

float star(vec2 p_star, float num_wings, float thickness_wings) {
	float angle = atan(p_star.x, p_star.y);
	float length_2 = dot(p_star, p_star);
	float sin_angle = .5 * (1. - cos(2. * angle * .5 * num_wings)); // sin^2
	float limit = thickness_wings * num_wings * num_wings / length_2;
	return 1. - step(limit, sin_angle);
}

float pyramid(vec3 p, float s, float h){
	float alpha = atan(s/(2*h));
	// float alpha = s/(2*h*sqrt(s*s/(4*h*h)+1)

	vec3 n1 = vec3( cos(alpha), sin(alpha), 0.);
	vec3 n2 = vec3(-cos(alpha), sin(alpha), 0.);
	vec3 n3 = vec3(0., sin(alpha),  cos(alpha));
	vec3 n4 = vec3(0., sin(alpha), -cos(alpha));

	float pyr;
	pyr = plane(trans(p, 0.,-h,0.), vec3(0.,-1.,0.));
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

	float t_fall = 1. - pow((max(0.,min(.9, pyramid_animation))-0.)/.9, 2.);
	float pyr_closed = 4. * t_fall;

	qq = trans(qq, 0., pyr_closed/2., 0.);

	float t_stampf = (max(0.9,min(1., pyramid_animation))-0.9)/.1;

	qq = trans(qq, 0.,  -  (1-pow(1.-t_stampf/2., 20.)), 0.);
	qq.y = -abs(qq.y);
	qq = trans(qq, 0., -pyr_closed/2., 0.); // pyramid distance
	qq.y = -pyramid_size*pyramid_h-qq.y;
	float pyr = pyramid(qq, pyramid_size * pyramid_s, pyramid_size * pyramid_h);
	vec2 pyramid = vec2(pyr, MATERIAL_ID_PYRAMID);

	// box-torus-morph
	q = trans(q, 0., pyramid_size* pyramid_h + 1., 0.);
	float cube = roundbox(q, vec3(1.), .5);
	float d = .75 + smoothstep(.70,1., pyramid_animation)*7.5;
	float torus = torus(trans(q, 0.,-.5,0.), vec2(.5 + d, .5));
	vec2 schalter_cube = vec2(mix(cube, torus, smoothstep(.65, .75, pyramid_animation)), MATERIAL_ID_CUBE);

	float lightballs_dist = sphere(domrep(p, 7, 10.,8.), 2.);
	float no_lightballs = sphere(p, 20.);
	vec2 lightballs = vec2(max(lightballs_dist, -no_lightballs), MATERIAL_ID_LIGHTBALL);

	vec2 bottom = vec2(p.y + 2., MATERIAL_ID_FLOOR);
	vec2 bounding = vec2(-sphere(p - camera_position, 50.), MATERIAL_ID_BOUNDING);
	return min_material(min_material(schalter_cube, pyramid), min_material(lightballs, min_material(bottom, bounding)));
}
