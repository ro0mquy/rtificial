#include "post_head.glsl"
#line 4

uniform sampler2D dof; // vec4
uniform sampler2D color; // vec3

out vec3 out_color;

void main() {
	vec4 dofColor = textureLod(dof, tc, 0.);
	vec3 color = textureLod(color, tc, 0.).rgb;
	out_color = mix(color, dofColor.rgb, smoothstep(0., 1., abs(dofColor.a) - .5));
}
