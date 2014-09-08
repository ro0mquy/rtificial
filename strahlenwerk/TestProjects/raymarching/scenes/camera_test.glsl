#version 430

layout(location = 0) uniform vec2 res;

uniform vec3 camera_position;
uniform vec3 camera_direction;
uniform vec3 camera_up;

uniform vec3 sphere1_color; // color
uniform vec3 sphere2_color; // color

out vec4 out_color;

vec2 f(vec3 p);
mat3 get_camera();
vec3 get_direction();
vec3 march(vec3 p, vec3 direction, out int i);
vec3 calc_normal(vec3 p);
float sphere(vec3 p, float s);
vec2 min_material(vec2 a, vec2 b);
float pdot(vec3 a, vec3 b);
float phong_norm(vec3 to_light, vec3 normal, vec3 to_view, float exponent);
float phong(vec3 to_light, vec3 normal, vec3 to_view, float exponent);

vec3 colors[] = vec3[](
		vec3(.1),
		vec3(.5),
		vec3(.03, .0, .0),
		vec3(1.)
		);

void main(void) {
	vec3 direction = get_direction();

	int i;
	vec3 hit = march(camera_position, direction, i);

	vec3 color = vec3(0.);
	if(i < 150) {
		int material = int(f(hit).y);
		vec3 normal = calc_normal(hit);
		mat4x3 to_lights = mat4x3(
			normalize(vec3(2.1, -.7, 1.4) - hit),
			normalize(vec3(-2.4, .5, 1.3) - hit),
			normalize(vec3(2.3, -.4, -1.4) - hit),
			normalize(vec3(-2.2, .8, -1.7) - hit)
		);
		vec4 l = max(normal * to_lights, vec4(0.));
		color = colors[material];

		color *= dot(l, vec4(.7, .8, .9, .5));
		color += .3 * vec3(1., 0., 0.) * phong(to_lights[0], normal, -direction, 50.);
		color += .3 * vec3(0., 1., 0.) * phong(to_lights[1], normal, -direction, 32.);
		color += .3 * vec3(0., 0., 1.) * phong(to_lights[2], normal, -direction, 40.);
		color += .3 * vec3(1., 1., 0.) * phong(to_lights[3], normal, -direction, 43.);
	}

	out_color.rgb = color;
}

vec2 f(vec3 p) {
	p.z += 3.;
	p.x -= 1.;
	vec2 sphere1 = vec2(sphere(p, .7), 0.);
	p.x += 2.;
	vec2 sphere2 = vec2(sphere(p, .7), 1.);
	vec2 bottom = vec2(p.y + 2., 2.);
	vec2 bounding = vec2(-sphere(p - camera_position, 50.), 3.);
	return min_material(min_material(sphere1, sphere2), min_material(bottom, bounding));
}

///////////////////// legacy libblink code ///////////////////
mat3 get_camera() {
	vec3 view_right = cross(camera_direction, camera_up);
	return mat3(view_right, camera_up, -camera_direction);
}

vec3 get_direction() {
	return get_camera() * normalize(vec3((gl_FragCoord.xy - .5 * res) / res.y , -1.));
}

vec3 march(vec3 p, vec3 direction, out int i) {
	float walked = 0.;
	for (i=0; i < 150; i++) {
		float dist = f(p)[0];
		p += direction * dist;
		dist = abs(dist);
		walked += dist;

		if (dist < .001 * walked) break;
	}
	return p;
}

vec3 calc_normal(vec3 p) {
	vec2 epilepsilon = vec2(.001, 0.);
	return normalize(vec3(
		f(p + epilepsilon.xyy)[0] - f(p - epilepsilon.xyy)[0],
		f(p + epilepsilon.yxy)[0] - f(p - epilepsilon.yxy)[0],
		f(p + epilepsilon.yyx)[0] - f(p - epilepsilon.yyx)[0]
	));
}

float sphere(vec3 p, float s) {
	return length(p) - s;
}

// hier kommt der witz!
vec2 min_material(vec2 a, vec2 b) {
	return mix(a, b, a.x > b.x);
}

// positive dot product
float pdot(vec3 a, vec3 b) {
	return max(0., dot(a, b));
}

// calculate specular term of phong model
// input vectors have to be of unit length
float phong_norm(vec3 to_light, vec3 normal, vec3 to_view, float exponent) {
	vec3 reflection = 2. * normal * dot(normal, to_light) - to_light;
	return pow(pdot(reflection, to_view), exponent);
}

// calculate specular term of phong model
float phong(vec3 to_light, vec3 normal, vec3 to_view, float exponent) {
	return phong_norm(normalize(to_light), normalize(normal), normalize(to_view), exponent);
}
