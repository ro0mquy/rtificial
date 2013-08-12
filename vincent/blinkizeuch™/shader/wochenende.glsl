#version 120

void initValues();
float f(vec3 p);
float sphere(vec3 p, float r);
float spikeball(vec3 p);
float sdBox(vec3 p, vec3 b);
mat3 rX(float theta);
mat3 rY(float theta);
mat3 rZ(float theta);
vec3 calcNormal(vec3 p);
float ao(vec3 p, vec3 n, float d, float i);
vec3 lighting(vec3 p, vec3 color, vec3 direction, vec3 normal, out vec3 light_color);
vec3 marching(vec3 p, vec3 direction, out int i);

uniform float aspect;
uniform vec2 res;
uniform float time;

uniform vec3 viewPosition;
uniform vec3 viewDirection;
uniform vec3 viewUp;

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

	vec3 viewRight = cross(viewDirection, viewUp);
	mat3 viewCamera = transpose(mat3(viewRight, viewUp, -viewDirection));

	vec3 camera = viewPosition;
	vec3 direction = normalize(vec3((gl_FragCoord.xy - .5 * res) / res.y, -1.)) * viewCamera;

	vec3 color = vec3(0.);
	vec3 p = camera;
	vec3 light_color_factor = vec3(1.);
	float reflection_factor = 1.;

	for (int reflections = 0; reflections < 2; reflections++) {
		int i;
		p = marching(p, direction, i);
		if(i < 100) {
			vec3 normal = calcNormal(p);
			vec3 light_color;
			vec3 newColor = lighting(p, color_spikeballs, direction, normal, light_color);
			//newColor *= 2. - exp( -2. * pow(distance(p, camera) / 20., 7.)); // fog
			//newColor += float(i) / 100.; // iteration glow
			color += newColor * reflection_factor * light_color_factor;
			light_color_factor *= light_color;
			reflection_factor *= .4;
			direction = reflect(direction, normal);
		} else {
			break;
		}
	}

	gl_FragColor = vec4(color, 1.);
}

void initValues() {
	lights[0] = vec3(-1., 1., 1.);
	lights[1] = vec3(5., 3., 0.);

	color_lights[0] = vec3(0., 1., 0.);
	color_lights[1] = vec3(1., 0., 1.);

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

float f(vec3 p) {
	float sphery = -sphere(p, 20.); // generating outside in sphere

	//p.z -= 6.;
	p = mod(p, 3.) - .5 * 3.; // repeating spikeball
	//return min(sphere(p, 1.), sphery);

	p = rotation_spikeballs * p;

	return min(sdBox(p, vec3(.5)), sphery);
	float spikey = spikeball(p / .5) * .5; // scaling spikeball

	return min(spikey, sphery); // return spikeball or sphere around scene
}

float sphere(vec3 p, float r) {
	return length(p) - r;
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

float sdBox(vec3 p, vec3 b) {
	vec3 d = abs(p) - b;
	return min(max(d.x, max(d.y, d.z)), 0.0) +
		length(max(d,0.0));
}

mat3 rX(float theta) {
	return mat3(
		1., 0., 0.,
		0., cos(theta), sin(theta),
		0., -sin(theta), cos(theta)
	);
}

mat3 rY(float theta) {
	return mat3(
		cos(theta), 0., -sin(theta),
		0., 1., 0.,
		sin(theta), 0., cos(theta)
	);
}

mat3 rZ(float theta) {
	return mat3(
		cos(theta), sin(theta), 0.,
		-sin(theta), cos(theta), 0.,
		0., 0., 1.
	);
}

vec3 calcNormal(vec3 p) {
	vec2 epilepsilon = vec2(.001, 0.);

	return normalize(vec3(
				f(p + epilepsilon.xyy) - f(p - epilepsilon.xyy),
				f(p + epilepsilon.yxy) - f(p - epilepsilon.yxy),
				f(p + epilepsilon.yyx) - f(p - epilepsilon.yyx)
			     ));
}

float ao(vec3 p, vec3 n, float d, float i) { // ambient occlusion ans sub surface scattering
	float o, s = sign(d);
	for (o=s*.5+.5;i>0.;i--) {
		o-=(i*d-f(p+n*i*d*s))/exp2(i);
	}
	return o;
}

vec3 lighting(vec3 p, vec3 color, vec3 direction, vec3 normal, out vec3 light_color) {
	light_color = vec3(0.);
	for (int i = 0; i < number_lights; i++) {
		vec3 point_to_light = normalize(lights[i] - p);
		float diffuse = dot(normal, point_to_light); // diffuse light
		float specular = pow(max(dot(reflect(point_to_light, normal), direction), 0.), 50.); // specular light
		light_color += color_lights[i] * intensity_lights[i] * (diffuse + specular);
	}

	light_color *= ao(p, normal, 0.15, 5.); // ambient occlusion
	//light_color *= ao(p, direction, -0.3, 10.); // sub surface scattering

	return color * light_color;
}

vec3 marching(vec3 p, vec3 direction, out int i) {
	float walked = 0.;
	for (i=0; i < 100; i++) {
		float dist = f(p);
		p += direction * dist;
		dist = abs(dist);
		walked += dist;

		if (dist < .001 * walked) break;
	}
	return p;
}
