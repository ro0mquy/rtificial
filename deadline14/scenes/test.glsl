#include "head.glsl"
#include "rtifical.glsl"

void main() {
	out_color = vec3(gl_FragCoord.xy / res, 0.);
}
