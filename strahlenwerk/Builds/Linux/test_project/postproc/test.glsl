#version 420

in vec2 tc;

uniform sampler2D foo; // vec3
out vec3 bar;

void main() {
	bar = texture2D(foo, vec2(.5, .5)).rgb;
	//bar = vec3(tc, 0.);
}
