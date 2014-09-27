#include "scene_head.glsl"
#include "rtificial.glsl"
#line 4

uniform vec3 background_color; // color
uniform float zapfen_kreise;

void main() {
	vec3 dir = get_direction();

	int i;
	vec3 p = march_adv(camera_position, dir, i, 150, .8);

	float materialId = f(p)[1];

	vec3 color = vec3(0.);
	vec3 normal = calc_normal(p);
	SphereLight light1 = SphereLight(vec3(0., 1000., 10.), vec3(1.), 3., 10000.);
	if(materialId == 0.) {
		color += .01 * mix(background_color, background_color_top, normal.y);
	} else if(materialId >= 1. && materialId <= 2.) {
		Material material1 = Material(vec3(0.), 1., .0);
		vec3 color1 = apply_light(p, normal, -dir, material1, light1);

		Material material2 = Material(vec3(0.), 0., 0.);
		vec3 color2 = apply_light(p, normal, -dir, material2, light1);
		float grid = 1.;
		vec3 q = p;
		float theta = radians(5.) * q.y * sin(time + q.y) + radians(5.) * zapfen_kreise * 10. * sin(q.x + time) * cos(q.z + time * 2.);
		q.xz *= mat2(
			cos(theta), sin(theta),
			-sin(theta), cos(theta)
		);
		float intensity = .01 * (sin(length(q.xz) * .1 -3. *  time) * .5 + .5);
		q.xz = mod(q.xz, 30.);
		grid = step(q.x, .5) + step(q.z, .5);
		//color2 += emit_light(vec3(1., 0., 0.), grid * intensity);

		color = mix(color1, color2, materialId - 1.);
	}

	output_color(color, distance(camera_position, p));
}

float boden(vec3 p);
float boden_implicit(vec3 p);

vec2 f(vec3 p) {
	vec2 bounding = vec2(-sphere(camera_position - p, 1000.), 0.);

	float height = 40.; // .5 * height
	vec3 q = domrepv(p, vec3(50.));
	q.y = p.y - height;
	//mat3 xrot = rX(radians(10.) * sin(q.y + p.x));
	//mat3 yrot = rY(radians(50.) * cos(q.y + p.y));
	//q = xrot * yrot * q;
	float k = smoothstep(height * 1.2, -height, q.y);
	float i = floor(p.x/50.);
	float j = floor(p.z/50.);
	float l = sin(i + sin(time + 3.)) * cos(j + sin(time + 5.));
	float m = cos(i + sin(time + 1.)) * sin(j + sin(time + 2.));
	mat3 rotX = rX(radians( 10.) * ((1. - k) * l + (.02 * sin(q.y))));
	mat3 rotZ = rZ(radians(-10.) * ((1. - k) * m + (.02 * sin(q.y))));
	q = rotX * rotZ * q;
	float radius = 10. * k;
	float d = max(length(q.xz) - radius, abs(q.y) - height);

	vec2 object = vec2(d, 1.);

	vec2 ground = vec2(boden_implicit(p), 2.);

	return min_material(bounding, smin_smaterial(object, ground, 10.));
}

float boden_implicit(vec3 p) {
	return p.y - 10. * cnoise(.03 * p.xz);
}

float boden(vec3 p) {
	//return p.y;
	vec2 e = vec2(.01, .0);
	vec3 grad = vec3(
		boden_implicit(p + e.xyy) - boden_implicit(p - e.xyy),
		boden_implicit(p + e.yxy) - boden_implicit(p - e.yxy),
		boden_implicit(p + e.yxx) - boden_implicit(p - e.yxx)
	);
	return boden_implicit(p) / length(grad) * 2. * e.x;
}
