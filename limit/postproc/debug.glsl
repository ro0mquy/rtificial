#include "post.glsl"
#line 3

uniform sampler2D value; // float
//uniform sampler2D depth; // float
out vec3 out_color;

void main() {
	out_color = vec3(texture(value, tc).r / 50);
}
