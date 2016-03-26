#ifndef ladebalken_H
#define ladebalken_H
static const char scene_ladebalken_source[] = R"shader_source(
#version 430
layout(location = 74) uniform float progress;
layout(location = 0) uniform vec2 res;
out vec3 out_color;

void main(void){
	vec2 pos = gl_FragCoord.xy / res;

	vec3 final_color = vec3(1.);
	final_color *= 1. - step(.05, abs(.5 - pos.y));
    final_color *= 1. - step(progress, pos.x);
	out_color.rgb = final_color;
}
)shader_source";

Shader ladebalken(scene_ladebalken_source, 0, nullptr);
#endif