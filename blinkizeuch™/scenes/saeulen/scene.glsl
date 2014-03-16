void initValues();
vec2 f(vec3 p);
float octoBox(vec3 p, float d, float h);
float ao(vec3 p, vec3 n, float d, float i);
vec3 lighting(vec3 p, vec3 color, vec3 direction, vec3 normal, out vec3 light_color);
float softshadow(vec3 ro, vec3 rd, float k);

DEFINE_MARCH(march, f)
DEFINE_NORMAL(calc_normal, f)
DEFINE_SOFTSHADOW(softshadow, f)
DEFINE_AO(ao, f)

uniform float foo1;
uniform float foo2;

uniform float light1_x;
uniform float light1_y;
uniform float light1_z;

uniform float light2_x;
uniform float light2_y;
uniform float light2_z;

const int number_lights = 2;
vec3 lights[number_lights];

vec3 color_background = vec3(0.05, 0.05, 0.05);
vec3 color_fog = vec3(.67, .1, .8); // in hsv
vec3 color_saeulen = vec3(1., 1., 1.);
vec3 color_lights[number_lights];

float intensity_lights[number_lights];

void main() {
	initValues();

	vec3 color = vec3(0.);
	vec3 p = view_position;
	vec3 direction = get_direction();
	vec3 light_color_factor = vec3(1.);
	float reflection_factor = 1.;

	for (int reflections = 0; reflections < 1; reflections++) {
		int i;
		p = march(p, direction, i);
		if(i < 100) {
			vec3 normal = calc_normal(p);
			vec3 light_color;
			vec3 newColor = lighting(p, color_saeulen, direction, normal, light_color);
			newColor += smoothstep(10., 50., float(i)); // iteration glow

			// fog
			vec3 hsv_newColor = rgb2hsv(newColor);
			float fog_intensity = smoothstep(5., 35., distance(p, view_position));
			newColor = hsv2rgb(mix(hsv_newColor, vec3(hsv_newColor.x, color_fog.yz), fog_intensity));

			color += newColor * reflection_factor * light_color_factor;

			light_color_factor *= light_color;
			reflection_factor *= .4;
			direction = reflect(direction, normal);
		} else {
			color = hsv2rgb(color_fog);
			break;
		}
	}
	//color += vec3(.8, .8, .9) * smoothstep(0., res.y, gl_FragCoord.y);
	out_color = vec4(color, 1.);
}

void initValues() {
	lights[0] = vec3(light1_x, light1_y, light1_z);
	lights[1] = vec3(light2_x, light2_y, light2_z);
	//lights[0] = vec3(-1.5, 3.5, 1.);
	//lights[1] = vec3(-1.5, 3.5, 4.);

	color_lights[0] = vec3(.9, .9, 1.);
	color_lights[1] = vec3(.9, .9, 1.);

	intensity_lights[0] = .7;
	intensity_lights[1] = .7;
}

vec2 f(vec3 p) {
	float sphery = -sphere(p - view_position, 50.); // bounding sphere

	float floor_plane = p.y;
	float ceiling_plane = -p.y+15.;
	float room = min(floor_plane, ceiling_plane);

	vec3 b = domrep(p, 12., 3., 15.);

	float saeulen = octoBox(b, 2., 3.);
	/*
	float fugen_abstand = .5;
	float fugen_hoehe = 0.02;
	float fugen = .004 * (
		1. - smoothstep(fugen_abstand - fugen_hoehe, fugen_abstand - .5 * fugen_hoehe, mod(p.y, fugen_abstand))
		+ smoothstep(fugen_abstand - .5 * fugen_hoehe, fugen_abstand, mod(p.y, fugen_abstand))
	);
	saeulen -= fugen;
	*/

	return vec2(min(min(sphery,saeulen), room), 0.);
}

float invsmax(float a, float b) {
	float k = .05;
	float h = clamp(.5 - .5 * (b - a) / k, 0., 1. );
	return mix(b, a, h) + 3. * k * h * (1. - h);
}

float octoBox(vec3 p, float d, float h) {
	float box = p.x - d;
	p = rY(TAU/8.) * p;
	box = invsmax(box, p.x - d);
	p = rY(TAU/8.) * p;
	box = invsmax(box, p.x - d);
	p = rY(TAU/8.) * p;
	box = invsmax(box, p.x - d);
	p = rY(TAU/8.) * p;
	box = invsmax(box, p.x - d);
	p = rY(TAU/8.) * p;
	box = invsmax(box, p.x - d);
	p = rY(TAU/8.) * p;
	box = invsmax(box, p.x - d);
	p = rY(TAU/8.) * p;
	box = invsmax(box, p.x - d);
	box = invsmax(box, -p.y - h / 2.);
	box = invsmax(box, p.y - h / 2.);
	return box;
}

vec3 lighting(vec3 p, vec3 color, vec3 direction, vec3 normal, out vec3 light_color) {
	light_color = vec3(0.);
	for (int i = 0; i < number_lights; i++) {
		// normale (diffuse) lighting
		vec3 to_light = normalize(lights[i] - p);
		float diffuse = max(dot(normal, to_light), 0.);

		// specular lighting
		float specular = 0.;
		specular = pow(max(dot(reflect(to_light, normal), direction), 0.), 50.);

		// softshadows
		float shadow = 1.;
		//shadow = softshadow(lights[i], p, 32.);

		light_color += color_lights[i] * intensity_lights[i] * (diffuse + specular) * shadow;
	}

	light_color *= ao(p, normal, 0.15, 5.); // ambient occlusion
	light_color += 7. * (1. - ao(p, -direction, 0.15, 5.)); // sub surface scattering

	return color * light_color;
}
