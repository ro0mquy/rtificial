#ifndef blend_H
#define blend_H
const char blend_source[] =R"shader_source(#version 430
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 0) uniform vec2 res;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 0) in vec2 tc;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#line 1
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(#line 4
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(binding = 10) uniform sampler2D bloom_big; // vec3
)shader_source"
R"shader_source(layout(binding = 9) uniform sampler2D bloom_medium; // vec3
)shader_source"
R"shader_source(layout(binding = 8) uniform sampler2D bloom_small; // vec3
)shader_source"
R"shader_source(layout(binding = 7) uniform sampler2D dof; // vec4
)shader_source"
R"shader_source(layout(binding = 0) uniform sampler2D orig; // vec3
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 35) uniform float bloom_amount;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(layout(location = 0) out vec3 out_color;
)shader_source"
R"shader_source(layout(location = 1) out float luminance;
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(void main() {
)shader_source"
R"shader_source(	vec4 dofColor = textureLod(dof, tc, 0.);
)shader_source"
R"shader_source(	vec3 color = textureLod(orig, tc, 0.).rgb;
)shader_source"
R"shader_source(	color = mix(color, dofColor.rgb, smoothstep(0., 1., abs(dofColor.a) - .5));
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	// bloom
)shader_source"
R"shader_source(	vec3 bloom = vec3(0.);
)shader_source"
R"shader_source(	bloom += textureLod(bloom_big, tc, 0.).rgb;
)shader_source"
R"shader_source(	bloom += textureLod(bloom_medium, tc, 0.).rgb;
)shader_source"
R"shader_source(	bloom += textureLod(bloom_small, tc, 0.).rgb;
)shader_source"
R"shader_source(	bloom /= 3.;
)shader_source"
R"shader_source(	color = mix(color, bloom, bloom_amount * .1);
)shader_source"
R"shader_source(
)shader_source"
R"shader_source(	out_color = color;
)shader_source"
R"shader_source(	luminance = log(max(dot(color, vec3(.2126, .7152, .0722)), 1e-6)); // TODO calculate
)shader_source"
R"shader_source(}
)shader_source"
;
#endif
