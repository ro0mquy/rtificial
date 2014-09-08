#version 430

// inactive uniforms lead to crashes (!)
layout(location = 0) uniform vec2 res;

out vec3 out_color;

void main() {
	vec2 c = gl_FragCoord.xy / res.xy * 2. - 1.;
	c.x *= res.x / res.y;
	c = c * .5 + .5;
	out_color = mix(vec3(1., 0., 0.), vec3(10. * step(distance(c, vec2(.5)), .13)), .9);
}
