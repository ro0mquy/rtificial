#include "scene_head.glsl"
#include "rtificial.glsl"
#line 4

uniform float pyramid_h; // float
uniform float pyramid_s; // float
uniform float aa; // float

Material materials[] = Material[](
	Material(vec3(.1, .1, .1), .5, 0.),
	Material(vec3(1.), .5, 0.),
	Material(vec3(.1,.3,.1), 1., 0.),
	Material(vec3(.7, .3, .2), 1., 0.)
);

#define MATERIAL_ID_FLOOR 0.
#define MATERIAL_ID_BOUNDING 1.
#define MATERIAL_ID_PYRAMID 2.
#define MATERIAL_ID_CUBE 3.

float pyramid(vec3 p, float s, float h);

void main(void) {
	vec3 direction = get_direction();

	int i;
	vec3 hit = march(camera_position, direction, i);

	vec3 color = vec3(0.);
	if(i < 150) {
		int material = int(f(hit).y);
		vec3 normal = calc_normal(hit);

		Material mat = materials[material];
		color = apply_light(hit, normal, -direction, mat, SphereLight(vec3(5., 9., 10.), vec3(1.), 2., 100.));
	}

	output_color(color, 4.1);// distance(hit, camera_position));
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
	q = trans(q, 0., 1.*sin(0.75 * time), 0.);

	q = trans(q, 0.,12.,0.);
	float pyr = pyramid(q, 10. * pyramid_s, 10. * pyramid_h);
	vec2 pyramid1 = vec2(pyr, MATERIAL_ID_PYRAMID);

	q = trans(q, 0.,-12.,0.);
	q = rX(TAU * .5) * q;
	pyr = pyramid(q, 10. * pyramid_s, 10. * pyramid_h);
	vec2 pyramid2 = vec2(pyr, MATERIAL_ID_PYRAMID);

	q = trans(q, 0., -10.* pyramid_h - .75, 0.);
	float cube = box(q, vec3(.75,.75,.75));
	vec2 schalter_cube = vec2(cube, MATERIAL_ID_CUBE);

	vec2 bottom = vec2(p.y + 2., MATERIAL_ID_FLOOR);
	vec2 bounding = vec2(-sphere(p - camera_position, 50.), MATERIAL_ID_BOUNDING);
	return min_material(min_material(schalter_cube, min_material(pyramid1, pyramid2)), min_material(bottom, bounding));
}
