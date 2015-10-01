#include "march.glsl"
#line 3

const float id_floor = 0.;
const float id_aman = 1.;
const float id_matrix = 2.;
const float id_prism = 3.;
const float id_verbindung = 4.;
const float id_verbindung_chamfer = 5.;

uniform float aman_cube_d;
float aman_cube_r = .5 * aman_cube_d;

uniform bool aman_hand_up_left;
uniform bool aman_hand_up_right;

uniform float overlay_offset_x;
uniform float overlay_offset_y;
uniform float overlay_scale;

layout(binding = 2) uniform sampler2D tex_rt_loves_deadline;

float fAmanBox(vec3 p, vec2 pos, vec2 dim) {
	vec3 dimension = vec3(dim, 1) * aman_cube_r;
	p.xy -= pos * aman_cube_d;
	p -= dimension;
	return fBoxEdge(p, dimension);
}

MatWrap wAman(vec3 p) {
	if (aman_domrep_rt_bool) {
		float cell_x = pDomrep(p.x, aman_domrep_cell_rt_vec2.x);
		float cell_z = pDomrepSingle(p.z, aman_domrep_cell_rt_vec2.y);
		p.x *= mod(cell_x, 2.) * 2. - 1.;

		if (cell_x == 0 && cell_z == 15) {
			p.y -= aman_cube_d * aman_single_jump_height_rt_float;
		}
	}
	p.x = aman_mirror_rt_bool ? (aman_mirror_plane_rt_float*aman_cube_d - p.x) : p.x;
	//p.y -= aman_cube_r;

	p.y -= aman_cube_d * aman_jump_h_rt_float * sqrt(aman_jump_anim_rt_float);

	vec3 p_bounding = p;
	pTrans(p_bounding, vec3(6., 8., .5) * aman_cube_d);
	float f_bounding = fBoxEdge(p_bounding, vec3(8., 8., .5) * aman_cube_d);
	if (f_bounding > .5) {
		return MatWrap(f_bounding, newMaterialId(id_aman, p));
	}

	float f_foot_row = fAmanBox(p, vec2(0, 0), vec2(12, 1));
	float f_foot_left = fAmanBox(p, vec2(0, 1), vec2(1, 1));
	float f_foot_left_top = fAmanBox(p, vec2(1, 2), vec2(3, 1));
	float f_foot_right = fAmanBox(p, vec2(5, 1), vec2(1, 1));
	float f = f_foot_row;
	f = min(f, f_foot_left);
	f = min(f, f_foot_left_top);
	f = min(f, f_foot_right);

	float f_leg_left = fAmanBox(p, vec2(2, 3), vec2(1, 3));
	float f_leg_middle = fAmanBox(p, vec2(6, 2), vec2(1, 3));
	float f_leg_right_block1 = fAmanBox(p, vec2(10, 1), vec2(1, 1));
	float f_leg_right_block2 = fAmanBox(p, vec2(10, 3), vec2(1, 1));
	float f_leg_right_block3 = fAmanBox(p, vec2(10, 5), vec2(1, 1));
	f = min(f, f_leg_left);
	f = min(f, f_leg_middle);
	f = min(f, f_leg_right_block1);
	f = min(f, f_leg_right_block2);
	f = min(f, f_leg_right_block3);

	float f_head_left = fAmanBox(p, vec2(2, 12), vec2(1, 3));
	float f_head_right = fAmanBox(p, vec2(9, 12), vec2(1, 3));
	float f_head_top = fAmanBox(p, vec2(3, 15), vec2(6, 1));
	float f_head_bottom = fAmanBox(p, vec2(3, 11), vec2(6, 1));
	f = min(f, f_head_left);
	f = min(f, f_head_right);
	f = min(f, f_head_top);
	f = min(f, f_head_bottom);

	float f_eye_left = fAmanBox(p, vec2(4, 13), vec2(1, 1));
	float f_eye_right = fAmanBox(p, vec2(6, 13), vec2(1, 1));
	f = min(f, f_eye_left);
	f = min(f, f_eye_right);

	if (aman_hand_up_left) {
		float f_body_left = fAmanBox(p, vec2(1, 7), vec2(1, 1));
		f = min(f, f_body_left);

		float f_arm_left_bow = fAmanBox(p, vec2(-1, 8), vec2(3, 1));
		float f_arm_left_side = fAmanBox(p, vec2(-2, 9), vec2(1, 6));
		float f_arm_left_block = fAmanBox(p, vec2(-1, 10), vec2(1, 1));
		f = min(f, f_arm_left_bow);
		f = min(f, f_arm_left_side);
		f = min(f, f_arm_left_block);

		float f_hand_left = fAmanBox(p, vec2(-1, 12), vec2(2, 1));
		float f_hand_left_fingers = fAmanBox(p, vec2(-1, 15), vec2(3, 1));
		float f_hand_left_thumb = fAmanBox(p, vec2(1, 13), vec2(1, 1));
		f = min(f, f_hand_left);
		f = min(f, f_hand_left_fingers);
		f = min(f, f_hand_left_thumb);
	} else {
		float f_body_left = fAmanBox(p, vec2(1, 7), vec2(1, 3));
		f = min(f, f_body_left);

		float f_hand_left = fAmanBox(p, vec2(0, 4), vec2(2, 1));
		float f_hand_left_thumb = fAmanBox(p, vec2(-1, 5), vec2(1, 1));
		f = min(f, f_hand_left);
		f = min(f, f_hand_left_thumb);

		float f_arm_left = fAmanBox(p, vec2(-2, 7), vec2(1, 4));
		float f_arm_left_block = fAmanBox(p, vec2(0, 8), vec2(1, 1));
		f = min(f, f_arm_left);
		f = min(f, f_arm_left_block);

		float f_shoulder_left = fAmanBox(p, vec2(0, 12), vec2(2, 1));
		float f_shoulder_left_block = fAmanBox(p, vec2(-1, 11), vec2(1, 1));
		f = min(f, f_shoulder_left);
		f = min(f, f_shoulder_left_block);
	}

	if (aman_hand_up_right) {
		float f_leg_right = fAmanBox(p, vec2(11, 1), vec2(1, 7));
		f = min(f, f_leg_right);

		float f_arm_right_bow = fAmanBox(p, vec2(11, 8), vec2(2, 1));
		float f_arm_right_side = fAmanBox(p, vec2(13, 9), vec2(1, 6));
		float f_arm_right_block = fAmanBox(p, vec2(12, 10), vec2(1, 1));
		f = min(f, f_arm_right_bow);
		f = min(f, f_arm_right_side);
		f = min(f, f_arm_right_block);

		float f_hand_right = fAmanBox(p, vec2(11, 12), vec2(2, 1));
		float f_hand_right_fingers = fAmanBox(p, vec2(10, 15), vec2(3, 1));
		float f_hand_right_thumb = fAmanBox(p, vec2(10, 13), vec2(1, 1));
		f = min(f, f_hand_right);
		f = min(f, f_hand_right_fingers);
		f = min(f, f_hand_right_thumb);
	} else {
		float f_leg_right = fAmanBox(p, vec2(11, 1), vec2(1, 3));
		f = min(f, f_leg_right);

		float f_body_right = fAmanBox(p, vec2(9, 7), vec2(1, 3));
		f = min(f, f_body_right);

		float f_hand_right = fAmanBox(p, vec2(11, 4), vec2(2, 1));
		f = min(f, f_hand_right);

		float f_arm_right = fAmanBox(p, vec2(13, 4), vec2(1, 7));
		float f_arm_right_block = fAmanBox(p, vec2(12, 8), vec2(1, 1));
		f = min(f, f_arm_right);
		f = min(f, f_arm_right_block);

		float f_shoulder_right = fAmanBox(p, vec2(10, 12), vec2(2, 1));
		float f_shoulder_right_block = fAmanBox(p, vec2(12, 11), vec2(1, 1));
		f = min(f, f_shoulder_right);
		f = min(f, f_shoulder_right_block);
	}

	// extra switch for body_middle part
	if (aman_hand_up_left && aman_hand_up_right) {
		// both up
		float f_body_middle = fAmanBox(p, vec2(1, 6), vec2(10, 1));
		f = min(f, f_body_middle);
	} else if (aman_hand_up_left && !aman_hand_up_right) {
		// only left
		float f_body_middle = fAmanBox(p, vec2(1, 6), vec2(12, 1));
		f = min(f, f_body_middle);
	} else if (!aman_hand_up_left && aman_hand_up_right) {
		// only right
		float f_body_middle = fAmanBox(p, vec2(-2, 6), vec2(13, 1));
		f = min(f, f_body_middle);
	} else {
		// non up
		float f_body_middle = fAmanBox(p, vec2(-2, 6), vec2(15, 1));
		f = min(f, f_body_middle);
	}

	f = max(f, -p.y + aman_cube_d * floor(aman_cut_h_rt_float));
	MatWrap w = MatWrap(f, newMaterialId(id_aman, p));

	return w;
}

MatWrap wMatrix(vec3 p) {
	p.y -= matrix_cut_rt_float;

	vec3 p_domrep = p;
	vec2 i_domrep = pDomrepMirror(p_domrep.xz, aman_domrep_cell_rt_vec2);
	vec3 p_verbindung = p_domrep;
	if (rand(ivec2(i_domrep)) > .5) {
		pFlip(p_domrep.y);
	}

	vec3 p_prism = p_domrep;
	float f_prism = f2Hexprism(p_prism.xz, matrix_prism_r_rt_float);
	f_prism = abs(f_prism) - matrix_prism_thick_rt_float;
	MatWrap w_prism = MatWrap(f_prism, MaterialId(id_prism, p_prism, vec4(i_domrep, 0, 0.)));

	//pDomrepAngle(p_verbindung.xz, 6, matrix_prism_r_rt_float);
	vec2 i_mirror = pMirrorTrans(p_verbindung.zx, matrix_prism_r_rt_float * unitVector(Tau / 12.));
	float f_verbindung = f2Hexprism(p_verbindung.xz, matrix_prism_thick_rt_float);

	pDomrep(p_verbindung.y, matrix_verbindung_domrep_rt_float);
	pMirrorGrid(p_verbindung.xz, 0.);
	float f_verbindung_up = f2Hexprism(p_verbindung.yz, matrix_prism_thick_rt_float);

	f_verbindung = min(f_verbindung, f_verbindung_up);

	MatWrap w_verbindung = MatWrap(f_verbindung, MaterialId(id_verbindung, p_verbindung, vec4(i_domrep, i_mirror)));

	MatWrap w_matrix = w_prism;
	w_matrix = mUnion(w_matrix, w_verbindung);
	w_matrix.f = max(w_matrix.f, -p.y);
	float f_cut = p.y - matrix_cut_rt_float;

	return w_matrix;
}

float fScene(vec3 p) {
	mUnion(wAman(p));
	if (aman_ceiling_rt_bool) {
		mUnion(wMatrix(p));
	}

	return current_dist;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	if (aman_2D_mode_rt_bool) {
		return material.color;
	}
	vec3 emission = material.emission;
	return ambientColor(normal, -direction, material) + emission;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	if (overlay_visible_rt_loves_deadline_rt_bool) {
		vec2 tex_coords;
		ivec2 texture_size = textureSize(tex_rt_loves_deadline, 0);
		tex_coords = gl_FragCoord.xy / res.xy;
		tex_coords.y = 1.-tex_coords.y;
		tex_coords.y *= texture_size.x / texture_size.y;
		tex_coords.x -= overlay_offset_x;
		tex_coords.y += overlay_offset_y * (texture_size.x / texture_size.y);
		tex_coords /= overlay_scale;
		vec4 tex_color = texture(tex_rt_loves_deadline, tex_coords);
		color = mix(color, tex_color.rgb, tex_color.a);
	}
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = .1;

	if (materialId.id == id_floor) {
		mat.color = vec3(96., 110., 113.) / 255.;
	} else if (materialId.id == id_aman) {
		vec3 p_aman = materialId.coord;
		vec3 p_cell = p_aman;
		p_cell.xy = mod(p_cell.xy, aman_cube_d) / aman_cube_r - 1.;
		vec2 i_cell = floor(p_aman.xy / aman_cube_d);

		float t_glow = 0.;
		if (i_cell == vec2(0., 1.) || // (0, 1) pixel with neighbors
				i_cell == vec2(-1., 1.) ||

				i_cell == vec2(4., 6.) || // (4, 6) pixel with neighbors
				i_cell == vec2(4., 7.) ||
				i_cell == vec2(4., 5.)
		   ) {
					t_glow = 1. - length(p_cell) / sqrt(3.);
					t_glow = pow(t_glow, aman_glow_gamma_rt_float);
		}

		mat.emission = aman_color_glow_rt_color * 1000. * aman_glow_intensity_rt_float * t_glow;
		mat.color = vec3(0.);
	} else if (materialId.id == id_prism || materialId.id == id_verbindung_chamfer) {
		mat.color = vec3(1.);
	} else if (materialId.id == id_verbindung) {
		mat.color = vec3(.2);
	}
	return mat;
}
