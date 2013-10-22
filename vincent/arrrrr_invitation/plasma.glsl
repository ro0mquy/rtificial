uniform float width, height;
uniform vec3 val;
uniform vec3 avg;
uniform vec3 cum;
uniform float time;
uniform float alpha;

const float pi = 3.14159;

vec3 hsv2rgb(vec3 c) {
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec4 effect(vec4 color, Image texture, vec2 texture_coords, vec2 screen_coords) {
	vec2 coord = screen_coords / vec2(width, height);
	float v = 0.;
	v += sin(coord.x * 20. + cum.x);
	v += sin(coord.y * 14. + 2. * cum.x);
	v += sin(17. * (coord.x * sin(time / 4.) + coord.y * cos(time / 7.)) + cum.x);
	vec2 c = coord + .5 * sin(vec2(cum.x/5., cum.x/3.));
	v += sin(sqrt(300. * dot(c,c) + 1.) + cum.x);
	v /= 3.;
	vec3 col = vec3(
		sin(2. * v * pi + avg.y + avg.x * 2.),
		.7 + sin(1.482 * v * pi + 2 * pi / 3 + avg.x * 11.) * avg.y,
		.6 - .7 * val.x + .2 * cos(v * pi + time) + avg.y * 3.
	);
	col = hsv2rgb(col);
	return vec4(col, alpha) + length(val) * .0000001 + length(avg) * .00000001;
}
