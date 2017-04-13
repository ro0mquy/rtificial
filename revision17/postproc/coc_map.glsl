#include "post.glsl"
#include "helper.glsl"
#line 4

uniform sampler2D color; // vec3
uniform sampler2D depth; // float

uniform float camera_focus_dist;
uniform float camera_focal_length;
uniform float camera_f_stop;
uniform bool post_disable_dof;

out vec4 out_color_and_coc;

void main() {
	if (post_disable_dof) {
		return;
	}
	float depthValue = textureLod(depth, tc, 0).x;

	float c = (abs(depthValue - camera_focus_dist) / depthValue)
		* (square(camera_focal_length) / (camera_f_stop * (camera_focus_dist - camera_focal_length)));
	float percentOfSensor = c / .024; // sensor height
	float maxCoC = .05;
	float blurFactor = clamp(percentOfSensor, 0., maxCoC);

	out_color_and_coc.rgb = textureLod(color, tc, 0).rgb;
	out_color_and_coc.a = blurFactor;
}
