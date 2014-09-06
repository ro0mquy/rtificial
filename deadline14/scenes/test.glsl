#include "scene_head.glsl"
#include "rtifical.glsl"

void main() {
	output_color(vec3(gl_FragCoord.xy / res, 0.), 0.);
}
