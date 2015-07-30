#include "post.glsl"
#include "textureNN.glsl"
#include "dof.glsl"
#line 5

uniform sampler2D value; // float
//uniform sampler2D depth; // float
out vec3 out_color;

void main() {
	out_color = vec3(textureNN(value, tc).r / 50);
}
