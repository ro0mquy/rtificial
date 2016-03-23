#include "march.glsl"
#include "layer.glsl"
#line 4

layout(binding = 1) uniform sampler2D tex_rtificial;
layout(binding = 2) uniform sampler2D tex_september;
layout(binding = 3) uniform sampler2D tex_budapest;
layout(binding = 4) uniform sampler2D tex_function;
layout(binding = 5) uniform sampler2D tex_2016;
layout(binding = 6) uniform sampler2D tex_invite;

float fGuard(vec2 p, float t) {
	return 0;
}

float f2TextBox(vec2 p, sampler2D tex, float height, float orig_width) {
	vec2 size = textureSize(tex, 0).xy;
	vec2 dim = height * vec2(size.x/size.y, 1);
	float spread = 32.;
	vec2 coordinates = .5 * p.xy / dim.xy + .5;
	coordinates.y = 1 - coordinates.y;
	float d = texture(tex, coordinates).r - .5;
	d *= spread / orig_width * dim.x * 2;
	// maybe uncomment if something breaks
	//d += length(max(abs(p.xy) - dim.xy * .5, 0.));
	return max(f2Box(p, dim), -d);
}

float f2Rtificial(vec2 p, float height) {
	return f2TextBox(p, tex_rtificial, height, 866);
}

float f2September(vec2 p, float height) {
	return f2TextBox(p, tex_september, height, 2120);
}

float f2Budapest(vec2 p, float height) {
	return f2TextBox(p, tex_budapest, height, 1213);
}

float f2Function(vec2 p, float height) {
	return f2TextBox(p, tex_function, height, 1039);
}

float f2TwentySixteen(vec2 p, float height) {
	return f2TextBox(p, tex_2016, height, 651);
}

float f2Invite(vec2 p, float height) {
	return f2TextBox(p, tex_invite, height, 1751);
}

float fInner(vec2 p, inout float f_frame, float t) {
	//float f = f2Box(p, 8 * vec2(1, size.y/size.x));

	float text_size = end_text_size_rt_float;
	float line_height = text_size * end_line_height_rt_float;

	float t_move = smoothstep(0, 1, (t - 400) / (464 - 400));
	pTrans(p.y, 2 * line_height * t_move);

	vec2 p_top = p;
	vec2 p_middle1 = p;
	vec2 p_middle2 = p;
	vec2 p_bottom = p;
	pTrans(p_top.y, 3 * line_height);
	pTrans(p_middle1.y, line_height);
	pTrans(p_middle2.y, -line_height);
	pTrans(p_bottom.y, -3 * line_height);

	float f = Inf;
/*
	float f = f2Rtificial(p_top, text_size);
	f = min(f, f2Invite(p_middle1, text_size));
	f = min(f, f2Function(p_middle2, text_size));
	f = min(f, f2TwentySixteen(p_bottom, text_size));
*/

	if (t >= 8) {
		f = min(f, f2TwentySixteen(p_bottom, text_size));
		if (t < 128) {
			f = min(f, f2Rtificial(p_top, text_size));
		}
	}
	if (t >= 40 && t < 128) {
		f = min(f, f2Invite(p_middle1, text_size));
	}
	if (t >= 72) {
		f = min(f, f2Function(p_middle2, text_size));
	}

	if (t >= 128 && t < 144) {
		float f_invite = f2Invite(p_middle1, text_size);
		float f_september = f2September(p_middle1, text_size);
		float t_mix = (t - 128)/(144-128);
		float f_box = f2Box(p_middle1, vec2(20, text_size * .5));
		float f_mixed = mix(mix(f_invite, f_box, t_mix), mix(f_box, f_september, t_mix), t_mix);
		f = min(f, f_mixed);
	}

	if (t >= 144 && t < 240) {
		f = min(f, f2September(p_middle1, text_size));
	}
	if (t >= 240 && t < 256) {
		float f_september = f2September(p_middle1, text_size);
		float f_budapest = f2Budapest(p_middle1, text_size);
		float t_mix = (t - 240)/(256-240);
		float f_box = f2Box(p_middle1, vec2(20, text_size * .5));
		float f_mixed = mix(mix(f_september, f_box, t_mix), mix(f_box, f_budapest, t_mix), t_mix);
		f = min(f, f_mixed);
	}
	if (t >= 256 && t < 352) {
		f = min(f, f2Budapest(p_middle1, text_size));
	}

	return f;
}

float fScene(vec3 p) {
	mUnion(wLayerEffect(p));

	return current_dist;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	return ambientColor(normal, -direction, material);
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .5;
	float rand_for_color = rand(ivec2(floor(materialId.misc.x)));
	mat.color = mix(lay_color1_rt_color, lay_color2_rt_color, rand_for_color);
	return mat;
}
