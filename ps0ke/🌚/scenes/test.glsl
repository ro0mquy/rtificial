#version 430

#include "helper.glsl"
#line 5

// inactive uniforms lead to crashes (!)
layout(location = 0) uniform vec2 res;
uniform vec3 camera_position;
uniform float time;

out vec4 out_color;

void main() {
	vec2 pos = gl_FragCoord.xy;
	pos -= 0.5*res;
	pos = floor(pos/10);

	float n = n_rt_float;
//*
	float num = pos.x * pos.y;

	float rem = mod(num, n);
	float rem_rel = rem / n;

	//vec3 col = hsv2rgb(vec3(rem / n, 1, 1));
	vec3 col = 1-ceil(vec3(rem_rel));


	if(col.x < 1) {
		col = color_0_rt_color;
		col *= hsv2rgb(vec3(rem_rel, 1, 1));
	} else {
		col = color_1_rt_color;
	}


	//out_color.rgb = col;
	out_color.rgb = pow(col, vec3(2.2));
}
