uniform float post_bloom_radius;

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
