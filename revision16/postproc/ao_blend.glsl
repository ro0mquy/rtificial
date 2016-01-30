#include "post.glsl"
#include "helper.glsl"
#line 4

uniform sampler2D ao; // float
uniform sampler2D color; // vec3
out vec3 out_color;

void main() {
	vec3 originalColor = textureLod(color, tc, 0).rgb;
	float centerAO = textureLod(ao, tc, 0).r;
	out_color = originalColor * centerAO;
}
