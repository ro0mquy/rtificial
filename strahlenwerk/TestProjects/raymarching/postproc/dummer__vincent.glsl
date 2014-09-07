#version 420

in vec2 tc;

out vec3 out_color;

uniform sampler2D in_color; // vec4

void main() {
	out_color = texture2D(in_color, tc).rgb;
}
