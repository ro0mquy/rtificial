#include "post.glsl"
#include "camera.glsl"
#line 4

uniform sampler2D color; // vec3
uniform sampler2D depth; // float
out vec3 out_color;

void main() {
	float amount = post_blur_edges_amount_rt_float * .01;

	float centerDepth = textureLod(depth, tc, 0).r;
	float screen_d;
	vec3 centerViewPos = centerDepth * camGetDirectionSS(gl_FragCoord.xy, screen_d);
	vec3 centerNormal = normalize(cross(dFdy(centerViewPos), dFdx(centerViewPos)));
	vec2 normal = normalize(centerNormal.xy);
	if (any(isnan(normal)) || any(isinf(normal))) {
		normal = vec2(0);
	}

	float x = atan(normal.y, normal.x);

	out_color = vec3(0);
	int n = int(post_blur_edges_n_rt_float);
	for (int i = -n; i <= n; i++) {
		out_color += textureLod(color, tc + (amount * i) / n, 0).rgb;
	}
	out_color /= 2 * n + 1;
}
