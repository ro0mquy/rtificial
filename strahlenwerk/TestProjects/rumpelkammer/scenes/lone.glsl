#include "march.glsl"

// unions two objects and adds n stairs within a radius of r
// use only with orthogonal objects
float opUnionStairs(float f1, float f2, float r, float n) {
	// Speckröllchen
	float f_min = min(f1, f2);
	//if (f1 < 2.*r && f2 < 2.*r) {
		vec2 q = vec2(f1, f2);
		float radius = r / n * sqrt(.5);
		q.y -= r - sqrt(.5) * radius;
		q.x -= sqrt(.5) * radius;
		pRot(q, -Tau / 8.);
		pDomrep(q.x, 2. * radius);

		q.x = abs(q.x);
		float offset = radius * sqrt(.5);
		float f_columns = dot(q, vec2(sqrt(.5))) - offset;

		f_columns = min(f_columns, q.y);
		return min(f_columns, f_min);
	//}
	//return f_min;
}

void pFlip(inout float p) {
	p = -p;
}

// see pMirrorGrid(vec3, float)
vec2 pMirrorGrid(inout vec2 p, float c) {
	vec2 q = p;
	pMirror(q);
	vec2 s = vec2(0);
	if (q.y > q.x) {
		p = p.yx;
		s.x = 1;
	}
	s.y = pMirrorTrans(p.x, c);
	p.y *= s.y;
	return s;
}

float fScene(vec3 p) {
	vec3 p_cliff = p;
	pFlip(p.z);
	float f_cliff = fCorner(p);

	vec3 p_house = p;
	pRotY(p_house, radians(-10));
	vec2 house_trans = lone_house_position_rt_vec2;
	pTrans(p_house.xz, house_trans);
	float house_base_height = lone_house_base_height_rt_float;
	pTrans(p_house.y, house_base_height);
	float house_size = lone_house_size_rt_float;
	float f_house = fBox(p_house, vec3(house_size, house_base_height, house_size));

	vec3 p_windows = p_house;
	float windows_height = lone_house_windows_height_rt_float;
	pTrans(p_windows.y, house_base_height + windows_height);
	pMirrorLoco(p_windows.xz, vec2(lone_post_house_size_rt_float));
	vec3 p_holes = p_windows;
	pTrans(p_holes.z, -house_size);
	pDomrepInterval(p_holes.z, .5, -1, 1);
	float holes_height = windows_height * .9;
	float f_holes = f2Box(p_holes.yz, vec2(holes_height, .15));
	float f_windows = f2Box(p_windows.yx, vec2(windows_height, .1));
	f_windows = opSubtractChamfer(f_windows, f_holes, .03);
	f_house = opUnionStairs(f_house, f_windows, .1, 3);

	vec3 p_roof = p_house;
	float roof_height = .1;
	pTrans(p_roof.y, house_base_height + windows_height * 2 + roof_height);
	vec3 p_bars = p_roof;
	pTrans(p_bars.y, .7);
	pRotY(p_bars, Tau / 8);
	pMirrorGrid(p_bars.zx, 0);
	pRotX(p_bars, radians(20));
	float f_bars = fBox(p_bars, vec3(.1, .3, 2));
	float f_roof = fBox(p_roof, vec3(house_size, roof_height, house_size));
	f_roof = min(f_roof, f_bars);
	f_house = min(f_house, f_roof);

	vec3 p_post = p;
	pTrans(p_post.xz, lone_post_translation_rt_vec2 + house_trans);
	pQuatRotate(p_post, lone_post_rotation_rt_quat);
	float f_post = fCylinder(p_post, lone_post_radius_rt_float, lone_post_height_rt_float);

	float f = f_cliff;
	f = min(f, f_post);
	f = min(f, f_house);

	mUnion(f, MaterialId(0., p));
	return f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	return applyNormalLights(origin, marched, direction, hit, normal, material);
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .1;
	mat.height = sin(materialId.coord.x * 2) * cos(materialId.coord.y * 2) * .1;
	return mat;
}
