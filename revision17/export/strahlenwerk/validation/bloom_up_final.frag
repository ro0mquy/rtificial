#version 430

layout(location = 0) uniform vec2 res;
vec2 tc = gl_FragCoord.xy / res;

layout(location = 64) uniform float time;


layout(location = 65) uniform float post_bloom_radius;

vec3 upsample(sampler2D inputTexture, vec2 position, vec2 pixelSize) {
	vec3 primaries = vec3(610., 550., 440.)/440.;
	vec3 color = .25 * textureLod(inputTexture, position, 0.).rgb;
	for (int i = 0; i < 3; i++) {
		vec2 tapRadius = pixelSize * post_bloom_radius * primaries[i];
		color[i] += .125 * textureLod(inputTexture, position - vec2(0., 1.) * tapRadius, 0.)[i];
		color[i] += .125 * textureLod(inputTexture, position + vec2(0., 1.) * tapRadius, 0.)[i];
		color[i] += .125 * textureLod(inputTexture, position - vec2(1., 0.) * tapRadius, 0.)[i];
		color[i] += .125 * textureLod(inputTexture, position + vec2(1., 0.) * tapRadius, 0.)[i];
		color[i] += .0625 * textureLod(inputTexture, position - tapRadius, 0.)[i];
		color[i] += .0625 * textureLod(inputTexture, position + vec2(1., -1.) * tapRadius, 0.)[i];
		color[i] += .0625 * textureLod(inputTexture, position + vec2(-1., 1.) * tapRadius, 0.)[i];
		color[i] += .0625 * textureLod(inputTexture, position + tapRadius, 0.)[i];
	}
	return color;
}

#line 5

layout(binding = 35) uniform sampler2D color; // vec3
layout(binding = 49) uniform sampler2D previous; // vec3
layout(location = 0) out vec3 out_color;

layout(location = 66) uniform float post_bloom_amount;

// level(0)

void main() {
	//out_color = mix(textureLod(color, tc, 0.).rgb, textureLod(previous, tc, 0.).rgb, post_bloom_amount);
	out_color = textureLod(color, tc, 0.).rgb + textureLod(previous, tc, 0.).rgb / 7 * post_bloom_amount;
}
