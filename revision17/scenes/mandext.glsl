#include "march.glsl"
#include "extruder_lighting.glsl"
#line 4

const float mat_id_plane = 0.;
const float mat_id_ext = 1.;

float pDomrepGridOnlyX(inout vec2 p, float c) {
	float i = pDomrepMirror(p.x, c);
	p.x -= .5 * c;
	if (p.x > p.y) {
		p.xy = p.yx;
	}
	return floor(.5 * i);
}

float rot_cell = mand_rot_cell_rt_float;
vec2 trans_inner = mand_trans_inner_rt_vec2;

MatWrap wMandala(vec3 p) {
	// domain geschwurbel
	vec3 p_cyl = p;

	pRotY(p_cyl, Tau * mand_rot_global_rt_float);

	// domrep angle
	float c_circle = pDomrepAngle(p_cyl.zx, mand_domrep_angle_num_rt_float, mand_domrep_angle_r_rt_float);

	pTrans(p_cyl.x, 4.);

	// grid domrep
	float c_grid = pDomrepGridOnlyX(p_cyl.xz, 8.);

	pRotY(p_cyl, Tau * rot_cell);
	pTrans(p_cyl.xz, trans_inner);

	// distance function geschwurbel
	pRotX(p_cyl, 2. * mand_rot_3D_rt_float * Tau);
	pRotZ(p_cyl,      mand_rot_3D_rt_float * Tau);
	float f_cyl = f2Box(p_cyl.zx, 1., 2.*Golden_Ratio);
	float c_distfunc = pDomrepInterval(f_cyl, mand_f_domrep_rt_float, 0., 1.);
	f_cyl = f2Box(vec2(f_cyl, p_cyl.y), mand_f_thickness_rt_float);

	MatWrap w_cyl = MatWrap(f_cyl, MaterialId(mat_id_ext, p_cyl, vec4(c_circle, c_grid, c_distfunc, 0.)));
	return w_cyl;
}

float fScene(vec3 p) {
	vec3 p_ext = p;
	//pTrans(p_ext.y, 10.);

	// extruder geschwurbel
	pTrans(p_ext.x, -ext_extrude_h_rt_float);
	float px_before = p_ext.x;
	float px_clamped = clamp(p_ext.x, -ext_extrude_h_rt_float, ext_extrude_h_rt_float);
	p_ext.x -= px_clamped;

	float px_param = px_clamped * ext_extrude_freq_rt_float;

	pRotX(p_ext, .5 * (px_param + ext_rot_rt_float) * Tau);
	/*
	pTrans(p_ext.z, 10 * sin((5. * px_param + ext_trans_rt_float) * Tau));
	pRotY(p_ext,      (px_param + ext_rot_rt_float) * Tau);
	pRotZ(p_ext, 4. * (px_param + ext_rot_rt_float) * Tau);
	// */

	rot_cell = .3 * sin(1. * (px_param + ext_rot_rt_float) * Tau) + mand_rot_cell_rt_float;
	trans_inner = vec2(1., 3.6) * sin(1. * (px_param + ext_rot_rt_float) * Tau) + mand_trans_inner_rt_vec2;


	// assemble object
	MatWrap w_ext = wMandala(p_ext.yxz);

	// ground plane
	float f_ground = p.x - mandext_plane_offset_rt_float;
	MatWrap w_ground = MatWrap(f_ground, newMaterialId(mat_id_plane, p));

	// combine everything
	MatWrap w = w_ext;
	w = mUnion(w, w_ground);

	mUnion(w);
	return w.f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	vec3 result = applyExtruderLights(origin, marched, direction, hit, normal, materialId, material);
	return result;
}

float fGeo(vec2 p) {
	float rot_angle = (-mand_rot_global_rt_float + .5 * (ext_extrude_h_rt_float + ext_rot_rt_float) + geo_rotation_offset_rt_float) * Tau;
	pRot(p, rot_angle);

	float radius = geo_radius_rt_float * mand_domrep_angle_r_rt_float;
	float f;

	int mode = int(geo_mode_rt_float) % 7;

	if (mode != 5) {
		if (mode == 0) {
			f = f2Triprism(p, radius);
		} else if (mode == 1) {
			f = f2Pentaprism(p, radius);
		} else if (mode == 2) {
			f = f2Quadprism(p, radius);
		} else if (mode == 3) {
			f = f2Hexprism(p, radius);
		} else if (mode == 4) {
			vec2 p_domrep = p;
			pDomrepAngle(p_domrep, 8, radius);
			pMirror(p_domrep.y);
			f = f2PlaneAngle(p_domrep, 3./16. * Tau);
		} else if (mode == 6) {
			vec2 p_tri = p;
			float f1 = f2Triprism(p_tri, radius);
			p_tri.y = -p_tri.y;
			float f2 = f2Triprism(p_tri, radius * geo_tri_scale_rt_float);
			f = max(f1, f2);
		}

		pMirrorTrans(f, geo_thickness_rt_float);

	} else {
		float box_offset = geo_box_offset_rt_float * mand_domrep_angle_r_rt_float * .01;
		float f1 = f2Quadprism(p - vec2(box_offset, 0.), radius - box_offset);
		float f2 = f2Quadprism(p + vec2(box_offset, 0.), radius - box_offset);
		float f_line = f2LineX(p, 0., radius - 2. * box_offset);

		pMirrorTrans(f1, geo_thickness_rt_float * .01);
		pMirrorTrans(f2, geo_thickness_rt_float * .01);
		pMirrorTrans(f_line, geo_thickness_rt_float * .01);

		f = min(f1, f2);
		//f = min(f, f_line);
	}

	f = smoothstep(-geo_thickness_rt_float * .01, 0., -f);

	return f;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	if (geo_enabled_rt_bool) {
		for (int i = 0; i < geo_plane_num_rt_float; i++) {
			// http://geomalgorithms.com/a05-_intersect-1.html
			float intersection_dist = ((geo_plane_xpos_rt_float + i * geo_plane_distance_rt_float) - origin.x) / direction.x;
			if (intersection_dist > 0. && intersection_dist < marched) {
				vec2 p = (origin + intersection_dist * direction).zy;
				float hipster_shit = fGeo(p);

				float brightness = geo_brightness_rt_float * .001 * square(float(i+1)/geo_plane_num_rt_float);
				color += hipster_shit * brightness * geo_color_rt_color;
			}
		}
	}

	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));

	if (materialId.id == mat_id_ext) {
		float c_circle = (materialId.misc.x + floor(mand_domrep_angle_num_rt_float/2.));// / (mand_domrep_angle_num_rt_float-1);
		float c_grid = materialId.misc.y / 2. + .5; // 0,1
		float c_distfunc = materialId.misc.z; // null oder 1 nicer wert

		if (c_distfunc > 0) {
			mat.color = 2* mand_color_circle_rt_color;
		} else {
			mat.color = mand_color_2_rt_color;
		}
		if (c_grid > 0) {
			mat.color += 0.5;
		}

		mat.metallic = 1.;
		mat.roughness = mandlight_obj_roughness_rt_float;
	} else if (materialId.id == mat_id_plane) {
		mat.color = extbg_ground_color_rt_color;
		mat.metallic = 0.;
		mat.roughness = extbg_ground_roughness_rt_float;
	}

	return mat;
}
