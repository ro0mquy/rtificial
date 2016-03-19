#include "march.glsl"
#include "layer.glsl"
#line 4

float f2Arch(vec2 p, vec2 dim, float r) {
	float f = f2Box(p, dim);
	pTrans(p.y, dim.y);
	pMirrorTrans(p.x, dim.x - r);
	return min(f, f2Sphere(p, r));
}

float f2BaseElement(vec2 p) {
	vec2 dim = parl_base_elem_dim_rt_vec2 * parl_base_elem_scale_rt_float;
	float f = f2Box(p, dim);

	vec2 bottom_dim = vec2(dim.x, parl_base_elem_bottom_height_rt_float * dim.y);
	vec2 top_dim = vec2(dim.x, dim.y - bottom_dim.y);

	vec2 p_bottom = p;
	vec2 bottom_arch_dim = bottom_dim * parl_base_elem_bottom_arch_dim_rt_vec2;
	float bottom_arch_radius = bottom_dim.y * parl_base_elem_bottom_arch_radius_rt_float;
	pTrans(p_bottom.y, -dim.y + bottom_arch_dim.y);
	float f_bottom_arch = f2Arch(p_bottom, bottom_arch_dim, bottom_arch_radius);

	vec2 p_top = p;
	vec2 top_arch_dim = top_dim * parl_base_elem_top_arch_dim_rt_vec2;
	pDomrep(p_top.x, .5 * (dim.x + top_arch_dim.x));
	float top_arch_radius = bottom_dim.y * parl_base_elem_top_arch_radius_rt_float;
	pTrans(p_top.y, dim.y - 2 * top_dim.y + top_arch_dim.y);
	float f_top_arch = f2Arch(p_top, top_arch_dim, top_arch_radius);

	vec2 p_spire = p;
	float spire_height = parl_base_elem_spire_height_rt_float * parl_base_elem_scale_rt_float;
	pTrans(p_spire.y, dim.y + spire_height);
	float f_spire = f2ConeCapped(p_spire, dim.x, 0., spire_height);

	f = min(f, f_spire);
	f = max(f, -f_bottom_arch);
	f = max(f, -f_top_arch);

	return f;
}

const vec2 middle_dim = vec2(2.2, 1.) * parl_base_elem_dim_rt_vec2.y * parl_base_elem_scale_rt_float;

float f2Middle(vec2 p) {
	vec2 dim = middle_dim;
	float f_middle_base = f2Box(p, dim);

	vec2 p_middle_top = p;
	vec2 dim_middle_top = vec2(.45) * dim.x;
	float middle_top_radius = parl_middle_top_radius_rt_float * parl_base_elem_scale_rt_float;
	pTrans(p_middle_top.y, dim.y + dim_middle_top.y);
	float f_middle_top = f2Arch(p_middle_top, dim_middle_top, middle_top_radius);

	vec2 p_middle_top_spire = p_middle_top;
	vec2 dim_middle_top_spire = dim.x * vec2(.112, .641);
	pTrans(p_middle_top_spire.y, dim_middle_top.y + dim_middle_top_spire.y);
	float f_middle_top_spire = f2ConeCapped(p_middle_top_spire, dim_middle_top_spire.x,
		0., dim_middle_top_spire.y);

	vec2 p_middle_spires = p;
	vec2 dim_middle_spires = dim.x * vec2(.186, .76);
	pMirrorTrans(p_middle_spires.x, dim.x - dim_middle_spires.x);
	pTrans(p_middle_spires.y, dim.y + dim_middle_spires.y);
	float f_middle_spires = f2ConeCapped(p_middle_spires, dim_middle_spires.x, 0., dim_middle_spires.y);


	float f = min(f_middle_base, f_middle_top);
	f = min(f, f_middle_spires);
	f = min(f, f_middle_top_spire);

	return f;
}

float f2SideTop(vec2 p, float width) {
	float height = .835 * width;
	pTrans(p.y, height);
	return f2Box(p, vec2(width, height));
}

float fInner(vec2 p, inout float f_frame, float t) {
	vec2 p_base = p;
	float base_elem_width = parl_base_elem_dim_rt_vec2.x * parl_base_elem_scale_rt_float;
	pMirrorTrans(p_base.x, middle_dim.x);
	vec2 p_side_top = p_base;
	pTrans(p_base.x, base_elem_width);
	pDomrepInterval(p_base.x, 2 * base_elem_width, 0, 13);
	float f_base = f2BaseElement(p_base);

	pTrans(p_side_top,
		vec2(13 * base_elem_width, parl_base_elem_dim_rt_vec2.y * parl_base_elem_scale_rt_float));
	float f_side_top = f2SideTop(p_side_top, 5 * base_elem_width);
	f_base = min(f_base, f_side_top);

	vec2 p_middle = p;
	float f_middle = f2Middle(p_middle);

	float f = min(f_base, f_middle);

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
