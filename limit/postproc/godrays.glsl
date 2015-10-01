#include "post.glsl"
#include "helper.glsl"
#line 4

uniform sampler2D color; // vec3
out vec3 out_color;

vec3 godRays() {
	vec3 accumulation = vec3(0.0);
	int iterations = 30;
	vec2 position = tc / 2.;
	vec2 temp_position = position;
	vec2 movement = post_godray_origin_rt_vec2 / 2;
	vec2 move_dir = movement - position;
	if (post_godray_parallel_rt_bool) {
		move_dir = post_godray_dir_rt_vec2;
	}

	float fadefactor = 1. / iterations;
	float multiplier = 1.0;
	for (int i=0; i<iterations; i++) {
		vec3 texturesample = textureLod(color, position + temp_position, 0).rgb;
		accumulation += multiplier * pow(texturesample, vec3(post_godray_gamma_rt_float));
		//smoothstep(0.1, 1.0, texturesample * texturesample);
		multiplier *= 1.0 - fadefactor;
		temp_position += move_dir / iterations;
	}
	accumulation /= iterations;
	return textureLod(color, tc, 0).rgb + accumulation * post_godray_intesity_rt_float;
}

void main() {
	if (post_disable_godrays_rt_bool) {
		out_color = textureLod(color, tc, 0).rgb;
	} else {
		out_color = godRays();
	}
}
