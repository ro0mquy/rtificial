#ifndef bloom_up_1st_H
#define bloom_up_1st_H
const char bloom_up_1st_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 64) uniform float post_bloom_radius;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(vec3 upsample(sampler2D inputTexture, vec2 position, vec2 pixelSize) {
)shader_source"
R"shader_source(	vec3 primaries = vec3(610., 550., 440.)/440.;
)shader_source"
R"shader_source(	vec3 color = .25 * textureLod(inputTexture, position, 0.).rgb;
)shader_source"
R"shader_source(	for (int i = 0; i < 3; i++) {
)shader_source"
R"shader_source(		vec2 tapRadius = pixelSize * post_bloom_radius * primaries[i];
)shader_source"
R"shader_source(		color[i] += .125 * textureLod(inputTexture, position - vec2(0., 1.) * tapRadius, 0.)[i];
)shader_source"
R"shader_source(		color[i] += .125 * textureLod(inputTexture, position + vec2(0., 1.) * tapRadius, 0.)[i];
)shader_source"
R"shader_source(		color[i] += .125 * textureLod(inputTexture, position - vec2(1., 0.) * tapRadius, 0.)[i];
)shader_source"
R"shader_source(		color[i] += .125 * textureLod(inputTexture, position + vec2(1., 0.) * tapRadius, 0.)[i];
)shader_source"
R"shader_source(		color[i] += .0625 * textureLod(inputTexture, position - tapRadius, 0.)[i];
)shader_source"
R"shader_source(		color[i] += .0625 * textureLod(inputTexture, position + vec2(1., -1.) * tapRadius, 0.)[i];
)shader_source"
R"shader_source(		color[i] += .0625 * textureLod(inputTexture, position + vec2(-1., 1.) * tapRadius, 0.)[i];
)shader_source"
R"shader_source(		color[i] += .0625 * textureLod(inputTexture, position + tapRadius, 0.)[i];
)shader_source"
R"shader_source(	}
)shader_source"
R"shader_source(	return color;
)shader_source"
R"shader_source(}
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#line 5
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(in vec2 tc;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 0) uniform vec2 res;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(binding = 13) uniform sampler2D color; // vec3
)shader_source"
R"shader_source(layout(binding = 21) uniform sampler2D previous; // vec3
)shader_source"
R"shader_source(layout(location = 0) out vec3 out_color;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(// level(0)
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void main() {
)shader_source"
R"shader_source(	out_color = upsample(color, tc, 1./res) + textureLod(previous, tc, 0.).rgb;
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
