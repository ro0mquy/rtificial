#include "scene_head.glsl"
#include "rtificial.glsl"
#line 4

uniform vec3 background_color; // color
uniform float zapfen_kreise;
uniform float zapfen_leit_freq;

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
				p = march_adv(camera_position + t * dir, dir, i, 20, .7);
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
	} else if(materialId >= 3. && materialId <= 4.) {
		Material material = Material(vec3(1., 0., 0.), 1., 0.);
		vec3 nonglowing = vec3(0.);
		vec3 glowing = apply_light(p, normal, -dir, material, light1);
		color = mix(nonglowing, glowing, materialId - 3.);
	}

	output_color(color, distance(camera_position, p));
}

float boden(vec3 p);
float boden_implicit(vec3 p);
vec2 leitungen(vec3 p, float rotation, float radius, float freq);
vec3 boden_transform(vec3 p);

vec2 f(vec3 p) {
	vec2 bounding = vec2(-sphere(camera_position - p, 1000.), 0.);

	float height = 40.; // .5 * height
	vec3 q = domrepv(p, vec3(50.));
	q.y = p.y - height;
	float k = smoothstep(-height * 1.2, height, -q.y);
	vec2 ij = floor(p.xz / 50.);
	q.xz += rand(ij + 10.) * 5.;
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

			leit.y = 1.;
			vec2 leit1 = leitungen(q, 0., .6, 1.7);
			vec2 leit2 = leitungen(q, 50., .6, 2.3);
			vec2 leit3 = leitungen(q, 20., .6, 3.2);
			vec2 leit4 = leitungen(q, 80., .6, 2.7);
			leit = min_material(min_material(leit1, leit2), min_material(leit3, leit4));
			leit.y += 3.;
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
	return trans(p, 0., 10. * (vnoise(.05 * p.xz) * .5 + .5), 0.);
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

vec2 leitungen(vec3 p, float rotation, float radius, float freq) {
	// estimate coordinate of next zapfen
	vec2 floored = floor(p.xz/50.) * 50.;
	vec2 ceiled = ceil(p.xz/50.) * 50.;
	vec2 next = floored;
	if(abs(p.x - floored.x) > abs(p.x - ceiled.x)) {
		next.x = ceiled.x;
	}
	if(abs(p.y - floored.y) > abs(p.y - ceiled.y)) {
		next.y = ceiled.y;
	}

	p.xz *= rot2D(rotation);

	////float glow = smoothstep(.2, .8, vnoise(vec2(time * 3. + p.z * .2, 2000. * floor(p.x / 10. /freq))));
	float glow = smoothstep(.2, .8, vnoise(vec2(distance(next, p.xz) * .5 + 1.3 * time, 2000. * rand(floor((p.xz + 25.) / 50.)))));

	float t = vnoise(p.xz * zapfen_leit_freq + 333. * freq) * .5 + .5;
	p.xz += rot2D(radians(10.) * t) * vec2(10.) - 10.;
	p = trans(p, 0., 4. * (t * .5 + .5), 0.);
	//p.x = mod(p.x, 20. / freq) - 10. / freq;
	//return vec2(length(p.xy) - radius, glow);

	vec3 q = p;
	q.xz = mod(q.xz, 40.);
	float angle = atan(q.x, q.z);
	angle = mod(angle, radians(20.));
	float r = length(q.xz);
	q.x = r * sin(angle);
	return vec2(length(q.xy) - radius, glow);
}
