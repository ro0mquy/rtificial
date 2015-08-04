#include "march.glsl"
#line 3 "szenchen"

const float id_floor = 0.;
const float id_aman = 1.;

float fAmanBox(vec3 p, vec2 pos, vec2 dim) {
	float r = aman_cube_r_rt_float;
	float d = 2. * r;

	vec3 dimension = vec3(dim, 1) * r;
	p.xy -= pos * d;
	p -= dimension;
	return fBox(p, dimension);
}

float fAman(vec3 p) {
	float r = aman_cube_r_rt_float;
	float d = 2. * r;
	p.y -= r;

	float f_foot_row = fAmanBox(p, vec2(0, 0), vec2(12, 1));
	float f_foot_left = fAmanBox(p, vec2(0, 1), vec2(1, 1));
	float f_foot_left_top = fAmanBox(p, vec2(1, 2), vec2(3, 1));
	float f_foot_right = fAmanBox(p, vec2(5, 1), vec2(1, 1));

	float f_leg_left = fAmanBox(p, vec2(2, 3), vec2(1, 3));
	float f_leg_middle = fAmanBox(p, vec2(6, 2), vec2(1, 3));
	float f_leg_right = fAmanBox(p, vec2(11, 1), vec2(1, 3));
	float f_leg_right_block1 = fAmanBox(p, vec2(10, 1), vec2(1, 1));
	float f_leg_right_block2 = fAmanBox(p, vec2(10, 3), vec2(1, 1));

	float f_body_middle = fAmanBox(p, vec2(-2, 6), vec2(15, 1));
	float f_body_left = fAmanBox(p, vec2(1, 7), vec2(1, 3));
	float f_body_right = fAmanBox(p, vec2(9, 7), vec2(1, 3));

	float f_hand_left = fAmanBox(p, vec2(0, 4), vec2(2, 1));
	float f_hand_left_thumb = fAmanBox(p, vec2(-1, 5), vec2(1, 1));
	float f_hand_right = fAmanBox(p, vec2(11, 4), vec2(2, 1));
	float f_hand_right_thumb = fAmanBox(p, vec2(10, 5), vec2(1, 1));

	float f_arm_left = fAmanBox(p, vec2(-2, 7), vec2(1, 4));
	float f_arm_left_block = fAmanBox(p, vec2(0, 8), vec2(1, 1));
	float f_arm_right = fAmanBox(p, vec2(13, 4), vec2(1, 7));
	float f_arm_right_block = fAmanBox(p, vec2(12, 8), vec2(1, 1));

	float f_shoulder_left = fAmanBox(p, vec2(0, 12), vec2(2, 1));
	float f_shoulder_left_block = fAmanBox(p, vec2(-1, 11), vec2(1, 1));
	float f_shoulder_right = fAmanBox(p, vec2(10, 12), vec2(2, 1));
	float f_shoulder_right_block = fAmanBox(p, vec2(12, 11), vec2(1, 1));

	float f_head_left = fAmanBox(p, vec2(2, 12), vec2(1, 3));
	float f_head_right = fAmanBox(p, vec2(9, 12), vec2(1, 3));
	float f_head_top = fAmanBox(p, vec2(3, 15), vec2(6, 1));
	float f_head_bottom = fAmanBox(p, vec2(3, 11), vec2(6, 1));

	float f_eye_left = fAmanBox(p, vec2(4, 13), vec2(1, 1));
	float f_eye_right = fAmanBox(p, vec2(6, 13), vec2(1, 1));


	float f = f_foot_row;
	f = min(f, f_foot_left);
	f = min(f, f_foot_left_top);
	f = min(f, f_foot_right);

	f = min(f, f_leg_left);
	f = min(f, f_leg_middle);
	f = min(f, f_leg_right);
	f = min(f, f_leg_right_block1);
	f = min(f, f_leg_right_block2);

	f = min(f, f_body_middle);
	f = min(f, f_body_left);
	f = min(f, f_body_right);

	f = min(f, f_hand_left);
	f = min(f, f_hand_left_thumb);
	f = min(f, f_hand_right);
	f = min(f, f_hand_right_thumb);

	f = min(f, f_arm_left);
	f = min(f, f_arm_left_block);
	f = min(f, f_arm_right);
	f = min(f, f_arm_right_block);

	f = min(f, f_shoulder_left);
	f = min(f, f_shoulder_left_block);
	f = min(f, f_shoulder_right);
	f = min(f, f_shoulder_right_block);

	f = min(f, f_head_left);
	f = min(f, f_head_right);
	f = min(f, f_head_top);
	f = min(f, f_head_bottom);

	f = min(f, f_eye_left);
	f = min(f, f_eye_right);

	return f;
}

float fScene(vec3 p) {
	mUnion(p.y, newMaterialId(id_floor, p));
	mUnion(fAman(p), newMaterialId(id_aman, p));

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
	mat.roughness = .1;

	if (materialId.id == id_floor) {
		mat.color = vec3(96., 110., 113.) / 255.;
	} else if (materialId.id == id_aman) {
		mat.color = vec3(.0);
	}
	return mat;
}
