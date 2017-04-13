#include "post.glsl"
#include "dof.glsl"
#line 4

uniform sampler2D color_and_coc; // vec4
uniform sampler2D depth; // float
out vec4 out_color_and_coc;
uniform bool post_disable_dof;

void main() {
	if (post_disable_dof) {
		return;
	}
	out_color_and_coc = dof(color_and_coc, depth, Tau / 8.);
}
