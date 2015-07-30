#include "post.glsl"
#include "helper.glsl"
#include "textureNN.glsl"
#line 5

uniform sampler2D color; // vec4
uniform sampler2D fullres; // vec3
out vec3 out_color;

void main() {
	vec4 halfres_color = textureLod(color, tc, 0).rgba;
	vec3 fullres_color = textureLod(fullres, tc, 0).rgb;
	out_color = mix(fullres_color, halfres_color.rgb, halfres_color.a);
}
