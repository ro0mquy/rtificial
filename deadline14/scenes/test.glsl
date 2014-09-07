#include "scene_head.glsl"
#include "rtificial.glsl"

void main() {
	vec3 color = vec3(gl_FragCoord.xy / res, 0.);
	color += 10. * step(distance(gl_FragCoord.xy / res, vec2(.5)), .1);
	output_color(color, 0.);
}
