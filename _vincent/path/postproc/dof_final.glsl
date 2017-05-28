#include "post.glsl"
#line 3

uniform sampler2D color; // vec3
uniform sampler2D color1; // vec4
uniform sampler2D color2; // vec4
//uniform sampler2D depth; // float
out vec3 out_color;
uniform bool post_disable_dof;

void main() {
	if (post_disable_dof) {
		out_color = textureLod(color, tc, 0).rgb;
	} else {
		out_color = min(textureLod(color1, tc, 0).rgb, textureLod(color2, tc, 0).rgb);
	}
}
