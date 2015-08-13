uniform float width, height;
uniform vec3 val;
uniform vec3 avg;
uniform vec3 cum;
uniform float time;

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec4 effect(vec4 color, Image texture, vec2 texture_coords, vec2 screen_coords) {
	float v = min((avg.x + 4. * avg.y + avg.z) * .3 + .7, 2.);
	float w = cum.x / 5. - time / 3. + 2. * val.z;
	w /= 2.;
	vec2 c = vec2(sin(w), cos(w)) * v;
	vec2 coords = screen_coords / vec2(width, height);
	vec2 z = coords - .5;
	z.x *= 3.;
	z.y *= 2.;

	int i;
	for(i = 0; i < 200; i++) {
		vec2 foo = c;
		foo.x += z.x * z.x - z.y * z.y;
		foo.y += dot(z.xy, z.yx);
		
		if(dot(foo, foo) > 4.) break;
		z = foo;
	}
	vec3 hsv = vec3(sin(i/(20. * (avg.z * 5. + .5)) + 5. * time) * .5 + .5, avg.y + .5, 1.);
	return vec4(hsv2rgb(hsv), 1.) + .000001 * (val.x + avg.x + cum.x + time);
}
