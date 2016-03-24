#include "march.glsl"
#include "layer.glsl"
#line 4

layout(binding = 7) uniform sampler2D tex_endcard;

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

float f2Endcard(vec2 p, float height) {
	return f2TextBox(p, tex_endcard, height, 5000);
}

MatWrap wInner(vec2 p, inout float f_frame, float t) {
	//float f = f2Box(p, 8 * vec2(1, size.y/size.x));
	f_frame = Inf;

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
	float fwhole = f2Endcard(p_middle1, text_size);
	// remove "rtificial invites you to"
	float fbottom = max(fwhole, -f2Plane(p_middle1 - vec2(0, 8), vec2(0, -1)));

	if (t >= 0) {
		if (t < 120) {
		// "rtificial"
			f = max(fwhole, -f2Plane(p_middle1 - vec2(0, 11), vec2(0, 1)));
		}
	}
	if (t >= 32 && t < 120) {
		// "invites you to"
		f = max(fwhole, -f2Plane(p_middle1 - vec2(0, 8), vec2(0, 1)));
	}
	if (t >= 64) {
		// "function 2016"
		f = max(fbottom, -f2Plane(p_middle1 - vec2(0, -8), vec2(0, 1)));
	}

	if (t >= 136 && t < 200) {
		// "september 9--11"
		f = max(fbottom, -f2Plane(p_middle1 - vec2(0, -11.4), vec2(0, 1)));
	}
	if (t >= 200 && t < 344) {
		// "budapest"
		f = fbottom;
	}

	return MatWrap(f, layerMaterialId(p, t));
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
