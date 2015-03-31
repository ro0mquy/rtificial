#version 430

in vec2 tc;

uniform sampler2D color; // vec3
uniform sampler2D coc; // float
out vec3 out_color;
out float out_coc;

void main() {
	vec2 c = tc;
	out_color = texture2D(color, c).rgb;
	out_coc = texture2D(coc, c).r;
	//bar = vec3(tc, 0.);
}
