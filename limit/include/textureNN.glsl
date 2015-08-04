#include "helper.glsl"

vec4 textureNNLod(sampler2D tex, vec2 uv, int level) {
	uv = saturate(uv);
	vec2 uv_nn = (floor((uv * textureSize(tex, level))) + .5) / textureSize(tex, level);
	return textureLod(tex, uv_nn, level);
}

vec4 textureNN(sampler2D tex, vec2 uv) {
	return textureNNLod(tex, uv, 0);
}

