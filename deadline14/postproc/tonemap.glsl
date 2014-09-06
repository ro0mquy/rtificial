#include "post_head.glsl"

// blend, tonemap, color grade

uniform sampler2D luminance; // float level(11)
uniform sampler2D bloom_big; // vec3
uniform sampler2D bloom_medium; // vec3
uniform sampler2D bloom_small; // vec3
uniform sampler2D dof; // vec3

out vec3 out_color;

void main() {
}
