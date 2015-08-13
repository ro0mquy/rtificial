uniform float width, height;
uniform vec3 val;
uniform vec3 avg;
uniform vec3 cum;
uniform float time;

vec3 position;

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float sphere(vec3 p, float r) {
	return length(p) - r;
}

mat3 rotation;

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

float sdCross(vec3 p) {
	p = abs(p);
	vec3 d = max(p.xyz, p.yzx);
	return min(d.x, min(d.y, d.z)) - 1.;
}

float box(vec3 p, vec3 b) {
	vec3 d = abs(p) - b;
	return min(max(d.x, max(d.y, d.z)), 0.) + length(max(d, 0.));
}
float menger(vec3 p) {
	//p += vec3(0.0, 0.0, 2.0);
	//float rot = 20.0/180.0*3.14;
	//p = rY(rX(p, rot), rot);
	float d = box(p, vec3(1.0));
	float s = 1.0;
	for(int m = 0; m < 4; m++) {
		vec3 a = mod(p * s, 2.0) - 1.0;
		s *= 3.0;
		vec3 r = 1.0 - 3.0 * abs(a);

		float c = sdCross(r)/s;
		d = max(d, c);
	}
	return d;
}

vec2 f(vec3 p) {
	float bounding = -sphere(p - position, 50.);
	float s = menger(rotation * (p - vec3(avg.y + 1., avg.x , avg.z * 3.)) / 2.) * 2.;
	if(s < bounding) {
		return vec2(s, 0.);
	} else {
		return vec2(bounding, 1.);
	}
}

float ao(vec3 p, vec3 n, float d, float i) { // ambient occlusion ans sub surface scattering
	float o, s = sign(d);
	for (o=s*.5+.5;i>0.;i--) {
		o-=(i*d-f(p+n*i*d*s)[0])/exp2(i);
	}
	return o;
}
vec3 calc_normal(vec3 p) {
	vec2 epilepsilon = vec2(.01, 0.);
	return normalize(vec3(
		f(p + epilepsilon.xyy)[0] - f(p - epilepsilon.xyy)[0],
		f(p + epilepsilon.yxy)[0] - f(p - epilepsilon.yxy)[0],
		f(p + epilepsilon.yyx)[0] - f(p - epilepsilon.yyx)[0]
	));
}

vec4 effect(vec4 color, Image texture, vec2 texture_coords, vec2 screen_coords) {
	position = vec3(0., 0., 8.);
	vec3 up = vec3(0., 1., 0.);
	vec3 right = vec3(1., 0., 0.);
	vec3 view_direction = cross(up, right);

	rotation = rX(cum.x * .1) * rY(cum.y * .2);

	mat3 camera = mat3(right, up, -view_direction);
	vec3 direction = camera * normalize(vec3((screen_coords - .5 * vec2(width, height)) / height, -1.));
	vec3 p = position;

	int i;
	float walked = 0.;
	float a = 0.;
	for(i = 0; i < 100; i++) {
		float dist = f(p)[0];
		p += direction * dist;
		dist = abs(dist);
		a += dist * i/100. * i/100.;
		walked += dist;
		if(dist < .001 * walked) break;
	}

	vec3 col;
	if(i < 100) {
		vec3 normal = calc_normal(p);
		vec3 light = position;
		float diffuse = dot(normal, normalize(light - p));
		if(f(p)[1] == 0.) {
			col = hsv2rgb(vec3(sin(p.x + cum.x * .3 + time) * cos(p.y + time * 2. + cum.y * .5) * .5 + .5, .7 + .2 * sin(p.z + .7 + time * 3.), 1.));
		} else {
			col = hsv2rgb(vec3(
				.5 + .5 * sin(p.x + avg.y + time * 3. + p.x * .3 * cos(p.y + avg.x * 3.)),
				1.,
				.5	
			));
		}
		col *= (.7 * diffuse + .3);
		col *= ao(p, normal, 0.15, 5.);
	} else {
		col = vec3(0.);
	}
	vec3 glow_col = vec3(sin((time /2. + cum.x)/3.), .8 + avg.y, 1.);
 	col += a * hsv2rgb(glow_col) * (avg.y * 2. + .1);
	return vec4(col, 1.) + .000001 * (val.x + avg.x + cum.x + time);
}
