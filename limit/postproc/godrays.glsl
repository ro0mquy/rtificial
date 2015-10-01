#include "post.glsl"
#include "helper.glsl"
#line 4

uniform sampler2D color; // vec3
out vec3 out_color;

vec3 godRays() {
	vec2 position = tc / 2.;
	vec2 temp_position = position;
	vec3 accumulation = vec3(0.0);
	int iterations = 30;
	float contrast = 2;
	vec2 movement = vec2(.5, .5);

	float fadefactor = 1. / iterations;
	float multiplier = 1.0;
	for (int i=0; i<iterations; i++) {
		vec3 texturesample = textureLod(color, position + temp_position, 0).rgb;
		accumulation += multiplier * texturesample;
		//smoothstep(0.1, 1.0, texturesample * texturesample);
		multiplier *= 1.0 - fadefactor;
		temp_position += ((movement * .5) - position) / iterations;
	}
	accumulation /= iterations;
	//contrast enhance to accentuate bright fragments
	return textureLod(color, tc, 0).rgb + (accumulation * (contrast / (1.0 + dot(position, position))));
}

void main() {
	if (post_disable_godrays_rt_bool) {
		out_color = textureLod(color, tc, 0).rgb;
	} else {
		out_color = godRays();
	}
}
