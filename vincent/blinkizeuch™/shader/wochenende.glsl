#version 120

void initValues();
float f(vec3 p);
float sphere(vec3 p, float r);
float spikeball(vec3 p);
vec3 rX(vec3 p, float theta);
vec3 rY(vec3 p, float theta);
vec3 rZ(vec3 p, float theta);
float ao(vec3 p, vec3 n, float d, float i);
vec3 lighting(vec3 p, vec3 color, vec3 direction);
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

float intensity_lights[number_lights];

vec3 c[19];

void main() {
	initValues();

	vec3 viewRight = cross(viewUp, viewDirection);
	mat3 viewCamera = transpose(mat3(viewRight, viewUp, -viewDirection));

	vec3 camera = viewPosition;
	vec3 direction = normalize(vec3((gl_FragCoord.xy - .5 * res) / res.y, -1.)) * viewCamera;

	int i = 0;
	vec3 p = marching(camera, direction, i);

	vec3 color;
	if (i == 100) {
		color = color_background;
	} else {
		color = lighting(p, color_spikeballs, direction);

		color *= 2. - exp( -2. * pow(distance(p, camera) / 20., 7.)); // fog
		color += float(i) / 100.; // iteration glow
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
}

float f(vec3 p) {
	float sphery = -sphere(p, 20.); // generating outside in sphere

	//p.z -= 6.;
	p = mod(p, 3.) - .5 * 3.; // repeating spikeball

	p = rX(p, radians(time/1000. * 10)); // rotating around X axis
	p = rY(p, radians(time/1000. * 8)); // rotating around Y axis
	p = rZ(p, radians(time/1000. * 15)); // rotating around Z axis

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

vec3 rX(vec3 p, float theta) {
	return mat3(
		1., 0., 0.,
		0., cos(theta), sin(theta),
		0., -sin(theta), cos(theta)
	) * p;
}

vec3 rY(vec3 p, float theta) {
	return mat3(
		cos(theta), 0., -sin(theta),
		0., 1., 0.,
		sin(theta), 0., cos(theta)
	) * p;
}

vec3 rZ(vec3 p, float theta) {
	return mat3(
		cos(theta), sin(theta), 0.,
		-sin(theta), cos(theta), 0.,
		0., 0., 1.
	) * p;
}

float ao(vec3 p, vec3 n, float d, float i) { // ambient occlusion ans sub surface scattering
	float o, s = sign(d);
	for (o=s*.5+.5;i>0.;i--) {
		o-=(i*d-f(p+n*i*d*s))/exp2(i);
	}
	return o;
}

vec3 lighting(vec3 p, vec3 color, vec3 direction) {
	vec2 epilepsilon = vec2(.001, 0.);
	vec3 normal = normalize(vec3(
				f(p + epilepsilon.xyy) - f(p - epilepsilon.xyy),
				f(p + epilepsilon.yxy) - f(p - epilepsilon.yxy),
				f(p + epilepsilon.yyx) - f(p - epilepsilon.yyx)
			));

	vec3 toLights[number_lights];
	for (int i = 0; i < number_lights; i++) {
		toLights[i] = normalize(lights[i] - p);
	}

	vec3 lambert;
	for (int i = 0; i < number_lights; i++) {
		lambert += intensity_lights[i] * color_lights[i] * dot(normal, toLights[i]); // diffuse lighting
	}
	color *= lambert;

	for (int i = 0; i < number_lights; i++) {
		color += intensity_lights[i] * pow(max(dot(reflect(toLights[i], normal), direction), 0.), 50.) * color_lights[i]; // specular light
	}

	color *= ao(p, normal, 0.15, 5.); // ambient occlusion
	//color *= ao(p, direction, -0.3, 10.); // sub surface scattering
	
	return color;
}

vec3 marching(vec3 p, vec3 direction, out int i) {
	float walked = 0.;
	for (; i < 100; i++) {
		float dist = f(p);
		p += direction * dist;
		dist = abs(dist);
		walked += dist;

		if (dist < .001 * walked) break;
	}
	return p;
}
