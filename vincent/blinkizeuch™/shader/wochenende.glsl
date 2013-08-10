#version 120

float f(vec3);
float sphere(vec3 p, float r);
float spikeball(vec3);
vec3 rX(vec3, float);
vec3 rY(vec3, float);
vec3 rZ(vec3, float);

varying vec2 pos;
uniform float aspect;
uniform float time;

vec3 gray = vec3(0.05, 0.05, 0.05);
vec3 pink = vec3(1., 0., 0.5);

void main() {
	vec3 camera = vec3(0., 0., 1. / tan(radians(45.)));
	vec3 direction = normalize(vec3(pos.x, pos.y / aspect, 0.) - camera);
	vec3 light = vec3(-1., 1., 1.);

	vec3 p = camera;

	int i = 0;
	float walked = 0.;
	for (; i < 100; i++) {
		float dist = f(p);
		p += direction * dist;
		dist = abs(dist);
		walked += dist;

		if (dist < .001 * walked) break;
	}

	vec3 color;
	if (i == 100) {
		color = gray;
	} else {
		vec2 epilepsilon = vec2(.001, 0.);
		vec3 normal = normalize(vec3(
					f(p + epilepsilon.xyy) - f(p - epilepsilon.xyy),
					f(p + epilepsilon.yxy) - f(p - epilepsilon.yxy),
					f(p + epilepsilon.yyx) - f(p - epilepsilon.yyx)
				));
		vec3 toLight = normalize(light - p);
		float lambert = dot(normal, toLight);
		float fog = exp( -2. * pow(distance(p, camera) / 20., 2.));
		color = pink * lambert * fog + float(i) / 100.;
	}

	gl_FragColor = vec4(color, 1.);
}

float f(vec3 p) {
	//p.z += 6.;
	vec3 modulo = mod(p, 3.) - .5 * 3.;
	vec3 rotatedX = rX(modulo, radians(time/1000. * 10));
	vec3 rotatedY = rY(rotatedX, radians(time/1000. * 8));
	vec3 rotatedZ = rZ(rotatedY, radians(time/1000. * 15));
	float spikey = spikeball(rotatedZ / .5) * .5;
	return min(spikey, -sphere(p, 20.));
}

float sphere(vec3 p, float r) {
	return length(p) - r;
}

// This will generate the distance function for some kind of spikeball.
// It's a bit magic - it's based on the paper "Generalized Distance Functions"
// - don't ask - play.
float spikeball(vec3 p){
	vec3 c[19];
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

	float l = length(p);
	p = normalize(p);
	float b = 0.;
	for (int i=3; i<19; i++)
		b=max(abs(dot(p, c[i])), b);
	b=1. - acos(b - .01)/(acos(-1.) * .5); 
	b=smoothstep(.78, 1., b);
	return l-2.2*pow(1.5,b);
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

