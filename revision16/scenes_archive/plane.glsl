#include "march.glsl"
#include "layer.glsl"
#line 4

float fGuard(vec2 p, float t) {
	return 0;
}

float f2Line(vec2 p, float r, vec2 v) {
	float h = saturate(dot(p, v)/ dot(v, v));
	return f2Sphere(p - v*h, r);
}

float f2Arch(vec2 p, vec2 dim, float r) {
	float f = f2Box(p, dim);
	pTrans(p.y, dim.y);
	pMirrorTrans(p.x, dim.x - r);
	return min(f, f2Sphere(p, r));
}

float fSeagull(vec2 p, float t) {
	pTrans(p.y, -1.5);
	float f;
	vec2 seagull_d = vec2(seagull_body_width_rt_float, seagull_body_height_rt_float);
	float t3 = p.y / seagull_d.y;
	pTrans(p.y, -seagull_d.y);
	float t1 = t3 - 1;
	float head = -exp(-square(t1) * 20);
	float middle = exp(-t3 * t3 * 8);
	float t2 = t3 + 1;
	float end = exp(-square(t2) * 15);
	f = f2Line(p, seagull_d.x * (1. + .5 * middle + .6 * end + .4 * head), vec2(0, 2 * seagull_d.y));
	pTrans(p.y, seagull_d.y);

	vec2 p_wings = p;
	vec2 wings_d = vec2(seagull_wings_length_rt_float, seagull_wings_width_rt_float);
	pTrans(p_wings.y, seagull_wings_position_rt_float * seagull_d.y);
	pMirrorTrans(p_wings.x, seagull_d.x);
	pTrans(p_wings.y, -wings_d.y);
	pRot(p_wings, seagull_wings_rotation_rt_float * Tau + sin(t * Tau * .04) * .4);
	pTrans(p_wings.y, wings_d.y);
	pTrans(p_wings.x, wings_d.x);
	float f_wings_inner = f2Box(p_wings, wings_d);
	vec2 wings_outer_d = vec2(seagull_wings_outer_length_rt_float, seagull_wings_width_rt_float);
	vec2 p_wings_outer = p_wings;
	pTrans(p_wings_outer.x, wings_d.x);
	pTrans(p_wings_outer.y, wings_outer_d.y);
	pRot(p_wings_outer, seagull_wings_outer_rotation_rt_float * Tau + sin(t * Tau * .04) * .1);
	pTrans(p_wings_outer.y, -wings_outer_d.y);
	pTrans(p_wings_outer.x, wings_outer_d.x);
	float f_wings_outer = f2Box(p_wings_outer, wings_outer_d);
	vec2 p_wings_outer_cut = p_wings_outer;
	pTrans(p_wings_outer_cut, wings_outer_d);
	float f_wings_outer_cut = f2Plane(p_wings_outer_cut, unitVector(-Tau / 8));
	f_wings_outer = opIntersectSmooth(f_wings_outer, f_wings_outer_cut, .3);
	float f_wings = opUnionSmooth(f_wings_inner, f_wings_outer, .0);
	//float f_wings = min(f_wings_inner, f_wings_outer);

	f = opUnionRounded(f, f_wings, .2);

	return f;
}

float fInner(vec2 p, inout float f_frame, float t) {
	pRot(p, Tau / 12);
	pTrans(p.y, (t - 30) * .2);
	pMirror(p.x);
	float wingspread = plane_wingspread_rt_float;

	vec2 p_wing = p;
	float wing_distance = .142 * wingspread;
	vec2 wing_dim = wingspread * vec2(.356, .446);
	pTrans(p_wing.x, 2 * wing_distance + wing_dim.x);
	float f_wing = abs(p_wing.y) - wing_dim.y;
	float wing_end_length = .129 * wingspread;
	vec2 p_wing_cutoff = p_wing;
	pTrans(p_wing_cutoff, vec2(-wing_dim.x, wing_dim.y));
	float f_wing_cutoff = f2Plane(p_wing_cutoff, normalize(vec2(wing_dim.y - wing_end_length, wing_dim.x)));
	p_wing_cutoff = p_wing;
	pTrans(p_wing_cutoff.x, wing_dim.x);
	f_wing_cutoff = max(f_wing_cutoff, p_wing_cutoff.x);
	f_wing = max(f_wing, f_wing_cutoff);

	vec2 p_front = p;
	vec2 front_dim = wingspread * vec2(.142, .275);
	pTrans(p_front.y, wing_dim.y + front_dim.y);
	float front_radius = wingspread * 1.79;
	float f_front = f2Arch(p_front, front_dim, front_radius);

	vec2 p_front_bottom = p;
	vec2 front_bottom_dim = vec2(wing_distance, wingspread * .443);
	pTrans(p_front_bottom.y, wing_dim.y + front_bottom_dim.y);
	pTrans(p_front_bottom.x, front_bottom_dim.x);
	float f_front_bottom = f2Box(p_front_bottom, front_bottom_dim);
	vec2 p_front_bottom_cutoff = p_front_bottom;
	float front_bottom_front_width = .0675 * wingspread;
	pTrans(p_front_bottom_cutoff, vec2(2 * front_bottom_front_width - front_bottom_dim.x, front_bottom_dim.y));
	float f_front_bottom_cutoff = f2Plane(p_front_bottom_cutoff, normalize(vec2(front_bottom_dim.y, front_bottom_dim.x - front_bottom_front_width)));
	f_front_bottom = max(f_front_bottom, f_front_bottom_cutoff);
	f_front = min(f_front, f_front_bottom);

	vec2 p_tail = p;
	pTrans(p_tail.y, -wing_dim.y);
	pTrans(p_tail.x, .236 * wingspread);
	float f_tail = f2Corner(p_tail);
	pTrans(p_tail.y, -2 * .116 * wingspread);
	f_tail = min(f_tail, f2Plane(p_tail, normalize(vec2(1))));
	pTrans(p_tail.y, -.541 * wingspread);
	f_tail = max(f_tail, -p_tail.y);
	f_tail = max(f_tail, p_tail.x - .368 * wingspread);

	float f = min(f_wing, f_front);
	f = min(f, f_tail);
	//return min(fSeagull(p, 0), f);
	return mix(fSeagull(p, t), f, clamp(t / 30. - .2, 0, 1));
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
