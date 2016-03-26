#include "march.glsl"
#include "layer.glsl"
#include "materials.glsl"
#line 5

layout(binding = 7) uniform sampler2D tex_endcard;

int mat_id_bigtext = 1337;
int mat_id_smalltext = 1338;
int mat_id_middletext = 1339;

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

	float t_move = smoothstep(400, 464, t);
	pTrans(p.y, - 20 * t_move);

	vec2 p_middle1 = p;
	pTrans(p_middle1.y, line_height);

	MaterialId mat_id_big = newMaterialId(mat_id_bigtext, vec3(1));
	MaterialId mat_id_small = newMaterialId(mat_id_smalltext, vec3(1));
	mat_id_small.misc.x = t;
	MaterialId mat_id_middle = newMaterialId(mat_id_middletext, vec3(1));

	float fwhole = f2Endcard(p_middle1, text_size);

	float frtificial = max(fwhole, -f2Plane(p_middle1 - vec2(0, 11), vec2(0, 1)));
	MatWrap mw_rtificial = MatWrap(frtificial, mat_id_middle);

	float finvites = max(fwhole, -f2Plane(p_middle1 - vec2(0, 8), vec2(0, 1)));
	finvites = max(finvites, -f2Plane(p_middle1 - vec2(0, 11), vec2(0,-1)));
	MatWrap mw_invites = MatWrap(finvites, mat_id_middle);

	float ffunction = max(fwhole, -f2Plane(p_middle1 - vec2(0, -8), vec2(0, 1)));
	ffunction = max(ffunction, -f2Plane(p_middle1 - vec2(0, 8), vec2(0,-1)));
	MatWrap mw_function = MatWrap(ffunction, mat_id_big);

	float fdate = max(fwhole, -f2Plane(p_middle1 - vec2(0, -9), vec2(0, -1)));
	fdate = max(fdate, -f2Plane(p_middle1 - vec2(0, -11.4), vec2(0,1)));
	MatWrap mw_date = MatWrap(fdate, mat_id_small);

	float flocation = max(fwhole, -f2Plane(p_middle1 - vec2(0, -11.4), vec2(0,-1)));
	MatWrap mw_location = MatWrap(flocation, mat_id_small);

	MatWrap mw = MatWrap(Inf, mat_id_small);

	if (t >= 4 && t < 120) {
		mw = mw_rtificial;
	}
	if (t >= 36 && t < 120) {
		mw = mUnion(mw_rtificial, mw_invites);
	}
	if (t >= 68) {
		mw = mw_function;
	}
	if (t >= 132 && t < 196) {
		mw = mUnion(mw, mw_date);
	}
	if (t >= 196 && t < 324) {
		mw = mUnion(mUnion(mw, mw_date), mw_location);
	}

	mw.m.misc[1] = -mw.f;
	return mw;
}

float fScene(vec3 p) {
	mUnion(wLayerEffect(p));

	return current_dist;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 front_light = layerLight(origin, marched, direction, hit, normal, material);
	return ambientColor(normal, -direction, material) + material.emission + front_light;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .5;
	float rand_for_color = rand(ivec2(floor(materialId.misc.x)));
	mat.color = mix(lay_color1_rt_color, lay_color2_rt_color, rand_for_color);

	float glow_intensity = end_bigtext_glow_intensity_rt_float;
	vec3 glow_color = end_bigtext_glow_color_rt_color;
	if( materialId.id == mat_id_bigtext && int(materialId.misc[2]) == 0 ) {
		mOutline(mat, materialId, glow_color, glow_intensity);
	}
	if(materialId.id == mat_id_smalltext && (int(materialId.misc.z) == 0 || materialId.misc.x == 343)) {
		mat.emission = end_smalltext_color_rt_color * end_smalltext_intensity_rt_float;
		mat.emission *= 1 - pow(saturate(materialId.misc.z / (-lay_last_layer_index)), .25);
	}
	if(materialId.id == mat_id_middletext && int(materialId.misc[2]) == 0) {
		mat.emission = glow_color * glow_intensity;
	}

	return mat;
}
