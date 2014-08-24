#version 430

layout(location = 0) uniform vec2 res;
uniform sampler2D color; // vec3

in vec2 tc;

out vec3 upwards;
out vec3 both;

void main() {
	vec2 pixelSize = 1./res;
	int n = 10;
	vec3 up_accum = vec3(0.);
	vec3 down_left_accum = vec3(0.);
	vec2 down_left = vec2(cos(radians(30.)), sin(radians(30.)));
	for(int i = 0; i < n; i++) {
		up_accum += texture2D(color, tc + i * pixelSize * vec2(0., -1.)).rgb;
		down_left_accum += texture2D(color, tc + i * pixelSize * down_left).rgb;
	}
	upwards = up_accum / n;
	both = (up_accum + down_left_accum) / n;
}
