#version 430

layout(location = 0) uniform vec2 res;
uniform sampler2D orig_color; // vec4
uniform sampler2D half_color; // vec3
uniform sampler2D quart_color; // vec3 
uniform sampler2D eighth_color; // vec3
in vec2 tc;
out vec3 out_color;

void main() {
	vec3 half_col = textureLod(half_color, tc, 0.).rgb;
	vec3 quart_col = textureLod(quart_color, tc, 0.).rgb;
	vec3 eighth_col = textureLod(eighth_color, tc, 0.).rgb;
	vec3 orig_col = textureLod(orig_color, tc, 0.).rgb;
	vec3 bloom = half_col + quart_col + eighth_col;
	bloom /= 3.;
	//out_color = mix(orig_col, bloom, .2);
	//out_color = half_col;
	//out_color = half_col;
	//out_color = eighth_col;
	out_color = mix(orig_col, bloom, .2);
	out_color = bloom;
}
