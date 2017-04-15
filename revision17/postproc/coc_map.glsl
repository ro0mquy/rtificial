#include "post.glsl"
#include "helper.glsl"
#include "camera.glsl"
#line 5

uniform sampler2D color; // vec3
uniform sampler2D depth; // float

uniform vec3 camera_focus_target;
uniform float camera_focus_stop;
uniform bool post_disable_dof;

float focus_dist = distance(camGetPosition(), camera_focus_target);

out vec4 out_color_and_coc;

void main() {
	if (post_disable_dof) {
		return;
	}
	float depthValue = textureLod(depth, tc, 0).x;

	float c = (abs(depthValue - focus_dist) / depthValue)
		* (square(camera_focal_length) / (camera_focus_stop * (focus_dist - camera_focal_length)));
	float percentOfSensor = c / .024; // sensor height
	float maxCoC = .05;
	float blurFactor = clamp(percentOfSensor, 0., maxCoC);

	out_color_and_coc.rgb = textureLod(color, tc, 0).rgb;
	out_color_and_coc.a = blurFactor;
}
