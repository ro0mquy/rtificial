#version 420

in vec2 tc;

uniform sampler2D foo; // vec3
out vec3 bar;

void main() {
	vec2 c = tc;
	bar = texture2D(foo, c).rgb;
	//bar = vec3(tc, 0.);
}
