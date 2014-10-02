#ifndef fxaa_H
#define fxaa_H
const char fxaa_source[] = R"shader_source(#version 430

layout(location = 0) uniform vec2 res;

layout(location = 0) in vec2 tc;

#line 1


layout(binding = 13) uniform sampler2D color; // vec3
layout(location = 0) out vec3 out_color;

void main() {
	// TODO
	out_color = textureLod(color, tc, 0.).rgb;
}
)shader_source";
#endif
