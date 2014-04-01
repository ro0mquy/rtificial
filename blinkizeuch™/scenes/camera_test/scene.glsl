uniform vec3 floor_color1;
uniform vec3 floor_color2;
uniform vec3 sphere1_color;
uniform vec3 sphere2_color;

uniform sampler2D tex1;
uniform sampler2D tex2;

vec2 f(vec3 p);
float cubicPulse(float c, float w, float x);

DECLARE_MARCH(march)
DECLARE_NORMAL(calc_normal)
DECLARE_AO(ao)

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_AO(ao, f)

vec3 colors[4];

void main(void) {
	vec3 direction = get_direction();

	colors[0] = sphere1_color;
	colors[1] = sphere2_color;
	colors[3] = vec3(.03, .0, .0);

	int i;
	vec3 p = march(view_position, direction, i);

	vec3 color = vec3(0.);
	if(i < 100) {
		int material = int(f(p).y);
		vec3 normal = calc_normal(p);
		mat4x3 to_lights = mat4x3(
			normalize(vec3(2.1, -.7, 1.4) - p),
			normalize(vec3(-2.4, .5, 1.3) - p),
			normalize(vec3(2.3, -.4, -1.4) - p),
			normalize(vec3(-2.2, .8, -1.7) - p)
		);
		vec4 l = max(normal * to_lights, vec4(0.));
		if(material == 2) {
			// float size = 2.;
			// int n = int(floor(p.z / size));
			// int m = int(floor(p.x / size));
			// if(mod(n, 2) == mod(m, 2)) {
			// 	color = floor_color1;
			// } else {
			// 	color = floor_color2;
			// }
			vec3 color1 = texture2D(tex1, p.xz/3.).rgb;
			vec3 color2 = texture2D(tex2, p.xz).rgb;
			color = mix(color1, color2, sin(time + length(p.xz)));
		} else {
			color = colors[material];
		}
		color *= dot(l, vec4(.7, .8, .9, .5));
		color += .3 * vec3(1., 0., 0.) * phong(to_lights[0], normal, -direction, 50.);
		color += .3 * vec3(0., 1., 0.) * phong(to_lights[1], normal, -direction, 32.);
		color += .3 * vec3(0., 0., 1.) * phong(to_lights[2], normal, -direction, 40.);
		color += .3 * vec3(1., 1., 0.) * phong(to_lights[3], normal, -direction, 43.);
	}

	out_color = color;
	out_depth = distance(view_position, p);
}

vec2 f(vec3 p) {
	p.x -= 1.;
	vec2 sphere1 = vec2(sphere(p, .7), 0.);
	p.x += 2.;
	vec2 sphere2 = vec2(sphere(p, .7), 1.);
	vec2 bottom = vec2(p.y + 2., 2.);
	vec2 bounding = vec2(-sphere(p - view_position, 50.), 3.);
	return min_material(min_material(sphere1, sphere2), min_material(bottom, bounding));
}
