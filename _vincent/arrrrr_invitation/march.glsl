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

vec2 f(vec3 p) {
	float bounding = -sphere(p - position, 50.);
	float c = 4.;
	float s = sphere(mod(p, c) - .5 * c, 1.);
	if(s < bounding) {
		return vec2(s, 0.);
	} else {
		return vec2(bounding, 1.);
	}
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
	position = vec3(0., 0., 0.);
	vec3 up = vec3(0., 1., 0.);
	vec3 right = vec3(1., 0., 0.);
	vec3 view_direction = cross(up, right);
	position += cum.x * right;
	position += cum.y * up;

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
			col = hsv2rgb(vec3(sin(p.x + time * 3.) * cos(p.y + time * 5.) * .5 + .5, .7 + .2 * sin(p.z + .7 + time * 3.), 1.));
		} else {
			col = vec3(0.);
		}
		col *= (.7 * diffuse + .3);
	} else {
		col = vec3(0.);
	}
	vec3 glow_col = vec3(sin((time /2. + cum.x)/3.), .8 + avg.y, 1.);
 	col += a * hsv2rgb(glow_col) * (avg.y * 2. + .1);
	return vec4(col, 1.) + .000001 * (val.x + avg.x + cum.x + time);
}
