#include "scene_head.glsl"
#include "rtificial.glsl"
#line 4

uniform vec3 background_color; // color
uniform float zapfen_kreise;

bool add_boden = false;
bool normal_mapping = false;

void main() {
	vec3 dir = get_direction();

	int i;
	vec3 p = march_adv(camera_position, dir, i, 150, .9);
	if(abs(f(p)[1]) <= 1e-6) {
		if(dir.y != 0) {
			float t = (-(camera_position.y - 20.) / dir.y);
			if(t > 0.) {
				add_boden = true;
				p = march_adv(camera_position + t * dir, dir, i, 20, .8);
			}
		}
	}

	float materialId = f(p)[1];

	vec3 color = vec3(0.);
	normal_mapping = true;
	vec3 normal = calc_normal(p);
	SphereLight light1 = SphereLight(vec3(0., 1000., 10.), vec3(1.), 3., 10000.);
	if(materialId == 0.) {
		color += .01 * background_color;
	} else if(materialId >= 1. && materialId <= 2.) {
		Material material1 = Material(vec3(0.), 1., .0);
		vec3 color1 = apply_light(p, normal, -dir, material1, light1);

		Material material2 = Material(vec3(0.), 1., 0.);
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
	} else if(materialId == 3.) {
		Material material = Material(vec3(1., 0., 0.), 1., 0.);
		color = apply_light(p, normal, -dir, material, light1);
	}

	output_color(color, distance(camera_position, p));
}

float boden(vec3 p);
float boden_implicit(vec3 p);
float leitungen(vec3 p, float radius, float freq);
vec3 boden_transform(vec3 p);

vec2 f(vec3 p) {
	vec2 bounding = vec2(-sphere(camera_position - p, 1000.), 0.);

	float height = 40.; // .5 * height
	vec3 q = domrepv(p, vec3(50.));
	q.y = p.y - height;
	float k = smoothstep(-height * 1.2, height, -q.y);
	vec2 ij = floor(p.xz / 50.);
	float rotation = sin(7. * dot(ij, ij) + time * .5);
	float rotation2 = cos(11. * dot(ij, ij) + time * .5);
	mat3 rotX = rX(rotation * (1. - k) * radians(7.));
	mat3 rotZ = rZ(rotation2 * (1. - k) * radians(7.));
	mat3 rotY = rY((1. - k) * radians(100.));
	q = rotZ * rotX * rotY * q;
	float radius = 10. * k;

	float d = max(length(q.xz) - radius, abs(q.y) - height);
	if(normal_mapping) {
		d += .06 * cfbm((2. * vnoise(q) + q) * .5);
	}

	vec2 object = vec2(d, 1.);

	if(add_boden) {
		vec2 ground = vec2(boden_implicit(p), 2.);

		vec2 boden_object = smin_smaterial(object, ground, 10.);

		vec2 leit;
		if(p.y < 20.) {
			vec3 q = boden_transform(p);
			float leit1 = leitungen(q, .2, 1.7);
			float leit2 = leitungen(rY(radians(50.)) * q, .2, 2.3);
			float leit3 = leitungen(rY(radians(30.)) * q, .2, 3.2);
			float leit4 = leitungen(rY(radians(70.)) * q, .2, 2.7);
			leit = vec2(min(min(leit1, leit2), min(leit3, leit4)), 3.);
		} else {
			leit = vec2(p.y, 3.);
		}

		object = min_material(boden_object, leit);
	} else {
		object = vec2(smax(object.x, 20. - p.y, 2.), 1.);
	}


	return min_material(bounding, object);
}

vec3 boden_transform(vec3 p) {
	return trans(p, 0., 10. * vnoise(.05 * p.xz), 0.);
}

float boden_implicit(vec3 p) {
	return boden_transform(p).y;
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

float leitungen(vec3 p, float radius, float freq) {
	float t = vnoise(.7 * p.xz + 333. * freq);
	p.xz += rot2D(radians(20.) * t) * vec2(10.) - 10.;
	p = trans(p, 0., 4. * (t * .5 + .5), 0.);
	p.x = mod(p.x, 10. / freq) - 5. / freq;
	return length(p.xy) - radius;
}
