#version 430

in vec2 tc;

uniform sampler2D color; // vec3
out vec3 out_color;

void main() {
	vec2 c = tc;
	out_color = texture2D(color, c).rgb;
	//bar = vec3(tc, 0.);
}
