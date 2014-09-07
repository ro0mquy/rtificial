#include "scene_head.glsl"
#include "rtificial.glsl"

void main() {
	output_color(vec3(gl_FragCoord.xy / res, 0.), 0.);
}
