#include "scene_head.glsl"
#include "rtificial.glsl"
#line 4

uniform vec3 background_color; // color
uniform float zapfen_kreise;
uniform float zapfen_leit_freq;
uniform vec3 zapfen_color1; // color
uniform vec3 zapfen_color2; // color
uniform float zapfen_mat_freq;
uniform float zapfen_rough1;
uniform float zapfen_rough2;

uniform vec3 zapfen_light1_pos;
uniform vec3 zapfen_light2_pos;
uniform vec3 zapfen_light1_col; // color
uniform vec3 zapfen_light2_col; // color
uniform float zapfen_light_radius;
uniform float zapfen_light_intensity;

bool add_boden = false;
bool normal_mapping = false;

vec3 apply_lights(vec3 p, vec3 N, vec3 V, Material mat) {
	vec3 color = vec3(0.);
	color += apply_light(p, N, V, mat, SphereLight(zapfen_light1_pos, zapfen_light1_col, zapfen_light_radius, zapfen_light_intensity));
	color += apply_light(p, N, V, mat, SphereLight(zapfen_light2_pos, zapfen_light2_col, zapfen_light_radius, zapfen_light_intensity));
	return color;
}

void main() {
	vec3 dir = get_direction();

	int i;
	vec3 p = march_adv(camera_position, dir, i, 100, .9);
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
	if(materialId == 0.) {
		color += .01 * background_color;
	} else if(materialId >= 1. && materialId <= 2.) {
		Material material1 = Material(zapfen_color1, zapfen_rough1, .0);
		Material material3 = Material(zapfen_color2, zapfen_rough2, .0);
		float foo = pow(smoothstep(-80., -20., -p.y), 7.) * smoothstep(.3, .9, cfbm(p * zapfen_mat_freq * vec3(vnoise(.2 * p + 7.) * .01 + 1., .3, vnoise(.2 * p + 3.) * .01 + 1.)) * .5 + .5);
		vec3 color1 = mix(
			apply_lights(p, normal, -dir, material1),
			apply_lights(p, normal, -dir, material3),
			foo
			);

		Material material2 = Material(vec3(1.), 1., 0.);
		vec3 color2 = apply_lights(p, normal, -dir, material2);
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

		color = mix(color1, color2, pow(materialId - 1., 6.));
	} else if(materialId >= 3. && materialId <= 4.) {
		Material material = Material(vec3(1., 0., 0.), 1., 0.);
		vec3 nonglowing = vec3(0., 0., 0.) * .2;
		vec3 glowing = apply_lights(p, normal, -dir, material);
		color = mix(nonglowing, glowing, materialId - 3.);
	}

	output_color(color, distance(camera_position, p));
}

float boden(vec3 p);
float boden_implicit(vec3 p);
vec2 leitungen(vec3 p, float rotation, float radius, float freq);
vec3 boden_transform(vec3 p);

vec2 f(vec3 p) {
	vec2 bounding = vec2(-sphere(camera_position - p, 300.), 0.);

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
			vec2 leit2 = leitungen(q, 50., .6, 27.3);
			vec2 leit3 = leitungen(q, 20., .6, 549.2);
			vec2 leit4 = leitungen(q, 80., .6, 123.7);

			//vec2 leit1 = leitungen(q, 0., .2, 1.7);
			//vec2 leit2 = leitungen(q, 50., .2, 2.3);
			//vec2 leit3 = leitungen(q, 30., .2, 3.2);
			//vec2 leit4 = leitungen(q, 70., .2, 2.7);
			float k = .5;
			leit = smin_smaterial(smin_smaterial(leit1, leit2, k), smin_smaterial(leit3, leit4, k), k);
			//leit = min_material(leit1, min_material(leit2, leit3));
			//leit = min_material(leit1, leit2);
			//leit = leit1;
			leit.y += 3.;
		} else {
			leit = vec2(p.y, 3.);
		}

		object = smin_material(boden_object, leit, 2.);
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
	float glow = smoothstep(.2, .8, vnoise(vec2(distance(next, p.xz) * .1 + 1.3 * time, 2000. * rand(floor((p.xz + 25.) / 50.)))));
	//glow = 1.;

	float t = vnoise(vec2(p.z, floor(p.x/80.) * floor(p.z/80.)) * zapfen_leit_freq + 333. * freq) * .5 + .5;
	//float t = vnoise(vec2(p.z, 0.) * zapfen_leit_freq + 333. * freq) * .5 + .5;
	p.xz += rot2D(radians(20.) * t) * vec2(10.) - 10.;
	//p = trans(p, 0., 2. * (t * .5 + .5), 0.);
	//p.x = mod(p.x, 20. / freq) - 10. / freq;
	//return vec2(length(p.xy) - radius, glow);

	vec3 q = p;
	q.xz = mod(q.xz, 40.) + 5.;
	float angle = atan(q.x, q.z);
	angle = mod(angle, radians(20.));
	float r = length(q.xz);
	q.x = r * sin(angle);
	return vec2(length(q.xy) - radius, glow);
}
