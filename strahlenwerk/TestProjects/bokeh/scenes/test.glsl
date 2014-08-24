#version 430

layout(location = 0) uniform vec2 res;

out vec3 color;

void main() {
	vec2 foo = abs(gl_FragCoord.xy/res - vec2(.5, .5));
	color = vec3(5. * step(foo.x, 3./res.x) * step(foo.y, 3./res.y));
}
