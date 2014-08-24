#version 430

layout(location = 0) uniform vec2 res;
uniform sampler2D upwards; // vec3
uniform sampler2D both; // vec3

out vec3 color;

in vec2 tc;

void main() {
	int n = 10;
	vec2 down_left = vec2(cos(radians(30.)), sin(radians(30.)));
	vec2 down_right = vec2(cos(radians(150.)), sin(radians(150.)));
	vec3 left_accum = vec3(0.);
	vec3 right_accum = vec3(0.);
	vec2 pixelSize = 1./res;
	for(int i = 0; i < n; i++) {
		left_accum += texture2D(upwards, tc + i * pixelSize * down_left).rgb;
		right_accum += texture2D(both, tc + i * pixelSize * down_right).rgb;
	}
	left_accum /= n;
	right_accum /= n;
	color = (left_accum + right_accum) / 3.;
}
