#version 430

layout(location = 0) uniform vec2 res;
vec2 tc = gl_FragCoord.xy / res;

layout(location = 64) uniform float time;

#line 3

layout(binding = 29) uniform sampler2D color; // vec3
layout(binding = 34) uniform sampler2D color1; // vec4
layout(binding = 33) uniform sampler2D color2; // vec4
//uniform sampler2D depth; // float
layout(location = 0) out vec3 out_color;
layout(location = 87) uniform bool post_disable_dof;

void main() {
	if (post_disable_dof) {
		out_color = textureLod(color, tc, 0).rgb;
	} else {
		out_color = min(textureLod(color1, tc, 0).rgb, textureLod(color2, tc, 0).rgb);
	}
}
