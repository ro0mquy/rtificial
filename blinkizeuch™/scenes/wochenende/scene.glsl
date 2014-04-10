uniform vec3 light_color1;
uniform vec3 light_color2;
uniform float reflectivity;
uniform int enable_ao;

void initValues();
vec2 f(vec3 p);
float spikeball(vec3 p);
float ao(vec3 p, vec3 n, float d, float i);
vec3 lighting(vec3 p, vec3 color, vec3 direction, vec3 normal, out vec3 light_color);

DECLARE_NORMAL(calc_normal)
DECLARE_MARCH(march)

DEFINE_NORMAL(calc_normal, f)
DEFINE_MARCH(march, f)

const int number_lights = 2;
//vec3 light1 = vec3(-1., 1., 1.);
//vec3 light2 = vec3(5., 3., 0.);
vec3 lights[number_lights];

vec3 color_background = vec3(0.05, 0.05, 0.05);
vec3 color_spikeballs = vec3(1., 1., 1.);
//vec3 color_light1 = vec3(0., 1., 0.);
//vec3 color_light2 = vec3(1., 0., 1.);
vec3 color_lights[number_lights];

mat3 rotation_spikeballs;

float intensity_lights[number_lights];

vec3 c[19];

void main() {
	initValues();

	vec3 color = vec3(0.);
	vec3 p = view_position;
	vec3 direction = get_direction();
	vec3 light_color_factor = vec3(1.);
	float reflection_factor = 1.;

	for (int reflections = 0; reflections < 2; reflections++) {
		int i;
		p = march(p, direction, i);
		if(i < 100) {
			vec3 normal = calc_normal(p);
			vec3 light_color;
			vec3 newColor = lighting(p, color_spikeballs, direction, normal, light_color);
			//newColor *= 2. - exp( -2. * pow(distance(p, camera) / 20., 7.)); // fog
			//newColor += float(i) / 100.; // iteration glow
			color += newColor * reflection_factor * light_color_factor;
			light_color_factor *= light_color;
			reflection_factor *= reflectivity;
			direction = reflect(direction, normal);
		} else {
			break;
		}
	}

	out_color.rgb = color;
}

void initValues() {
	lights[0] = vec3(-1., 1., 1.);
	lights[1] = vec3(5., 3., 0.);

	color_lights[0] = vec3(0., 1., 0.);
	color_lights[1] = vec3(1., 0., 1.);
	color_lights[0] = light_color1;
	color_lights[1] = light_color2;

	intensity_lights[0] = .5;
	intensity_lights[1] = .3;

	c[0] = vec3(1., 0., 0.);
	c[1] = vec3(0., 1., 0.);
	c[2] = vec3(0., 0., 1.);
	c[3] = vec3(.577, .577, .577);
	c[4] = vec3(-.577, .577, .577);
	c[5] = vec3(.577, -.577, .577);
	c[6] = vec3(.577, .577, -.577);
	c[7] = vec3(0., .357, .934);
	c[8] = vec3(0., -.357, .934);
	c[9] = vec3(.934, 0., .357);
	c[10] = vec3(-.934, 0., .357);
	c[11] = vec3(.357, .934, 0.);
	c[12] = vec3(-.357, .934, 0.);
	c[13] = vec3(0., .851, .526);
	c[14] = vec3(0., -.851, .526);
	c[15] = vec3(.526, 0., .851);
	c[16] = vec3(-.526, 0., .851);
	c[17] = vec3(.851, .526, 0.);
	c[18] = vec3(-.851, .526, 0.);

	rotation_spikeballs =
		rZ(radians(time/1000. * 15)) *
		rY(radians(time/1000. * 8)) *
		rX(radians(time/1000. * 10));
}

vec2 f(vec3 p) {
	float sphery = -sphere(p - view_position, 50.); // bounding sphere

	//p.z -= 6.;
	p = mod(p, 3.) - .5 * 3.; // repeating spikeball
	//return min(sphere(p, 1.), sphery);

	p = rotation_spikeballs * p;

	return vec2(min(box(p, vec3(.5)), sphery), 0.);
	float spikey = scale(spikeball, p, .5);

	return vec2(min(spikey, sphery), 0.); // return spikeball or sphere around scene
}

// This will generate the distance function for some kind of spikeball.
// It's a bit magic - it's based on the paper "Generalized Distance Functions"
// - don't ask - play.
float spikeball(vec3 p){
	float l = length(p);
	p = normalize(p);
	float b = 0.;
	for (int i = 3; i < 19; i++)
		b = max(abs(dot(p, c[i])), b);
	b = 1. - acos(b - .01)/(acos(-1.) * .5);
	b = smoothstep(.78, 1., b);
	return l - 2.2 * pow(1.5, b);
}

float ao(vec3 p, vec3 n, float d, float i) { // ambient occlusion ans sub surface scattering
	float o, s = sign(d);
	for (o=s*.5+.5;i>0.;i--) {
		o-=(i*d-f(p+n*i*d*s)[0])/exp2(i);
	}
	return o;
}

vec3 lighting(vec3 p, vec3 color, vec3 direction, vec3 normal, out vec3 light_color) {
	light_color = vec3(0.);
	for (int i = 0; i < number_lights; i++) {
		vec3 point_to_light = normalize(lights[i] - p);
		float diffuse = max(dot(normal, point_to_light), 0.); // diffuse light
		float specular = pow(max(dot(reflect(point_to_light, normal), direction), 0.), 50.); // specular light
		light_color += color_lights[i] * intensity_lights[i] * (diffuse + specular);
	}
	if(enable_ao != 0)
		light_color *= ao(p, normal, 0.15, 5.); // ambient occlusion
	//light_color *= ao(p, direction, -0.3, 10.); // sub surface scattering

	return color * light_color;
}
