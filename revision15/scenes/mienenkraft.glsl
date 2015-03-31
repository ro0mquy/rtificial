#include "scene_head.glsl"
#include "rtificial.glsl"
#line 4

const float boden_id = 0.;
const float fels_id = 1.;
const float berg_id = 2.;
const float kristall_id = 3.;

vec3 applyFog( in vec3  rgb,      // original color of the pixel
               in float distance, // camera to point distance
               in vec3  rayOri,   // camera position
               in vec3  rayDir )  // camera to point vector
{
	float b = mk_fog_invheight_rt_float;
	float c = mk_fog_amount_rt_float;
    float fogAmount = c * exp(-rayOri.y*b) * (1.0-exp( -distance*rayDir.y*b ))/rayDir.y;
	fogAmount = clamp(fogAmount, 0., 1.);
    vec3  fogColor  = vec3(0.5,0.6,0.7);
    return mix( rgb, fogColor, fogAmount );
}

void main() {
	vec3 o = camera_position;
	float screenDist;
	vec3 d = get_direction(screenDist);
	float t = march(o, d, 200., screenDist);

	float material_id = f(o+t*d, true)[1];

	vec3 out_color;

	if (isinf(t)) {
		// hmmmmm…
		float r = 200.;
		float radicand = dot(d, o) * dot(d, o) - dot(o, o) + r * r;
		if (radicand < 0.) discard; // hupsi
		t = -dot(d, o) + sqrt(radicand);
		out_color.rgb = textureLod(environment, normalize(o + t * d), 0.).rgb;
	} else {
		vec3 normal = calc_normal(o + t * d, false);
		out_color.rgb = vec3(1.);

		if (material_id == boden_id) {
			out_color *= 0.2;
		} else if (material_id == berg_id) {
			out_color.r *= 0.2;
		} else if (material_id == kristall_id) {
			out_color.b *= 0.2;
		}

		float rough = 1.;
		float metallic = 0.;
		out_color.rgb = ambientColor(normal, -d, out_color.rgb, rough, metallic);

		//out_color.rgb = abs(normal);
		out_color.rgb = applyFog(out_color.rgb, t, o, d);

		out_color.rgb *= clamp(ao(o + t * d, normal, .4, 5), 0., 1.);
	}
	output_color(out_color, t);

}

float fels_noise(vec3 p_fels, vec2 domrep_size, vec3 box_size) {
	vec2 cell_fels = floor(p_fels.xz / domrep_size);
	//cell_fels = vec2(0.);
	p_fels.xz = domrep(p_fels.xz, domrep_size);

	//  8 1 5
	//  4 0 2  >x
	//  7 3 6 vz
	vec3 p_fels_0 = p_fels;
	vec3 p_fels_1 = trans(p_fels, 0., 0., -domrep_size.y);
	vec3 p_fels_2 = trans(p_fels,  domrep_size.x, 0., 0.);
	vec3 p_fels_3 = trans(p_fels, 0., 0.,  domrep_size.y);
	vec3 p_fels_4 = trans(p_fels, -domrep_size.x, 0., 0.);
	vec3 p_fels_5 = trans(p_fels,  domrep_size.x, 0., -domrep_size.y);
	vec3 p_fels_6 = trans(p_fels,  domrep_size.x, 0.,  domrep_size.y);
	vec3 p_fels_7 = trans(p_fels, -domrep_size.x, 0.,  domrep_size.y);
	vec3 p_fels_8 = trans(p_fels, -domrep_size.x, 0., -domrep_size.y);

	vec2 cell_fels_0 = cell_fels;
	vec2 cell_fels_1 = cell_fels + vec2( 0., -1.);
	vec2 cell_fels_2 = cell_fels + vec2( 1.,  0.);
	vec2 cell_fels_3 = cell_fels + vec2( 0.,  1.);
	vec2 cell_fels_4 = cell_fels + vec2(-1.,  0.);
	vec2 cell_fels_5 = cell_fels + vec2( 1., -1.);
	vec2 cell_fels_6 = cell_fels + vec2( 1.,  1.);
	vec2 cell_fels_7 = cell_fels + vec2(-1.,  1.);
	vec2 cell_fels_8 = cell_fels + vec2(-1., -1.);

	p_fels_0.xy *= rot2D(cell_fels_0.x * domrep_size.x * 32.73101);
	p_fels_1.xy *= rot2D(cell_fels_1.x * domrep_size.x * 32.73101);
	p_fels_2.xy *= rot2D(cell_fels_2.x * domrep_size.x * 32.73101);
	p_fels_3.xy *= rot2D(cell_fels_3.x * domrep_size.x * 32.73101);
	p_fels_4.xy *= rot2D(cell_fels_4.x * domrep_size.x * 32.73101);
	p_fels_5.xy *= rot2D(cell_fels_5.x * domrep_size.x * 32.73101);
	p_fels_6.xy *= rot2D(cell_fels_6.x * domrep_size.x * 32.73101);
	p_fels_7.xy *= rot2D(cell_fels_7.x * domrep_size.x * 32.73101);
	p_fels_8.xy *= rot2D(cell_fels_8.x * domrep_size.x * 32.73101);

	p_fels_0.xz *= rot2D(cell_fels_0.y * domrep_size.y * 49.29012);
	p_fels_1.xz *= rot2D(cell_fels_1.y * domrep_size.y * 49.29012);
	p_fels_2.xz *= rot2D(cell_fels_2.y * domrep_size.y * 49.29012);
	p_fels_3.xz *= rot2D(cell_fels_3.y * domrep_size.y * 49.29012);
	p_fels_4.xz *= rot2D(cell_fels_4.y * domrep_size.y * 49.29012);
	p_fels_5.xz *= rot2D(cell_fels_5.y * domrep_size.y * 49.29012);
	p_fels_6.xz *= rot2D(cell_fels_6.y * domrep_size.y * 49.29012);
	p_fels_7.xz *= rot2D(cell_fels_7.y * domrep_size.y * 49.29012);
	p_fels_8.xz *= rot2D(cell_fels_8.y * domrep_size.y * 49.29012);

	/*
	p_fels_0.yz *= rot2D(cell_fels_0.y * domrep_size.x * 52.40165);
	p_fels_1.yz *= rot2D(cell_fels_1.y * domrep_size.x * 52.40165);
	p_fels_2.yz *= rot2D(cell_fels_2.y * domrep_size.x * 52.40165);
	p_fels_3.yz *= rot2D(cell_fels_3.y * domrep_size.x * 52.40165);
	p_fels_4.yz *= rot2D(cell_fels_4.y * domrep_size.x * 52.40165);
	p_fels_5.yz *= rot2D(cell_fels_5.y * domrep_size.x * 52.40165);
	p_fels_6.yz *= rot2D(cell_fels_6.y * domrep_size.x * 52.40165);
	p_fels_7.yz *= rot2D(cell_fels_7.y * domrep_size.x * 52.40165);
	p_fels_8.yz *= rot2D(cell_fels_8.y * domrep_size.x * 52.40165);
	// */

	float fels_0 = box(p_fels_0, box_size);
	float fels_1 = box(p_fels_1, box_size);
	float fels_2 = box(p_fels_2, box_size);
	float fels_3 = box(p_fels_3, box_size);
	float fels_4 = box(p_fels_4, box_size);
	float fels_5 = box(p_fels_5, box_size);
	float fels_6 = box(p_fels_6, box_size);
	float fels_7 = box(p_fels_7, box_size);
	float fels_8 = box(p_fels_8, box_size);

	float fels_12 = min(fels_1, fels_2);
	float fels_34 = min(fels_3, fels_4);
	float fels_56 = min(fels_5, fels_6);
	float fels_78 = min(fels_7, fels_8);
	float fels_1234 = min(fels_12, fels_34);
	float fels_5678 = min(fels_56, fels_78);
	float fels_12345678 = min(fels_1234, fels_5678);
	float fels_012345678 = smin(fels_0, fels_12345678, mk_smin_felsen_rt_float * box_size.x);
	fels_012345678 = smin(fels_0, fels_1234, mk_smin_felsen_rt_float * box_size.x); // TODO: maybe remove this line for less/more? artefacts

	return fels_012345678;
}

float kristall(vec3 p_kristall, float height_kristall, float radius_kristall, float size_cap) {
	float r_kristall = radius_kristall * min((height_kristall - radius_kristall) - p_kristall.y, size_cap) / size_cap;
	p_kristall.y -= height_kristall * .5;
	return hexprism(p_kristall.xzy, vec2(r_kristall, height_kristall));
}
vec2 kristallgruppe(vec3 p){
	float f_kristall = kristall(p, mk_kristall_h_rt_float, mk_kristall_r_rt_float, mk_kristall_cap_rt_float);

	vec3 p_kristall2 = trans(p, mk_kristall2_trans_relative_rt_vec3);
	float kristall2 = kristall(
		quat_rotate(p_kristall2, mk_kristall2_rot_relative_rt_quat),
		mk_kristall_h_rt_float * mk_kristall2_h_relative_rt_float,
		mk_kristall_r_rt_float * mk_kristall2_r_relative_rt_float,
		mk_kristall_cap_rt_float * mk_kristall2_cap_relative_rt_float
	);

	vec3 p_kristall3 = trans(p, mk_kristall3_trans_relative_rt_vec3);
	float kristall3 = kristall(
		quat_rotate(p_kristall3, mk_kristall3_rot_relative_rt_quat),
		mk_kristall_h_rt_float * mk_kristall3_h_relative_rt_float,
		mk_kristall_r_rt_float * mk_kristall3_r_relative_rt_float,
		mk_kristall_cap_rt_float * mk_kristall3_cap_relative_rt_float
	);

	f_kristall = min(f_kristall, kristall2);
	f_kristall = min(f_kristall, kristall3);
	return vec2(f_kristall, kristall_id);
}

vec2 huegel(vec3 p) {
	float berg_smin = mk_smin_felsen_rt_float; // 5.0

	vec3 p_berg_0 = p;
	//p_berg_0.xz *= rot2D(TAU * 0.5);
	p_berg_0.xy *= rot2D(TAU * 1.2);
	p_berg_0.yz *= rot2D(TAU * 0.7);
	float berg_0 = box(p_berg_0, vec3(1.5));

	vec3 p_berg_1 = trans(p, 1.2,0.5,0.3);
	p_berg_1.xz *= rot2D(TAU * 0.2);
	p_berg_1.xy *= rot2D(TAU * 0.3);
	p_berg_1.yz *= rot2D(TAU * 0.1);
	float berg_1 = box(p_berg_1, vec3(3.));

	vec3 p_berg_2 = trans(p, 3.,3.,5.);
	p_berg_2.xz *= rot2D(TAU * 0.53);
	p_berg_2.xy *= rot2D(TAU * 0.76);
	p_berg_2.yz *= rot2D(TAU * 0.35);
	float berg_2 = box(p_berg_2, vec3(3.,5.,2.));

	vec3 p_berg_3 = trans(p, 3.,7.,8.);
	p_berg_3.xz *= rot2D(TAU * 0.2);
	p_berg_3.xy *= rot2D(TAU * 0.45);
	p_berg_3.yz *= rot2D(TAU * 0.1);
	float berg_3 = box(p_berg_3, vec3(.7,1.2,1.));

	vec3 p_berg_4 = trans(p, 7.,4.,6.);
	p_berg_4.xz *= rot2D(TAU * 0.35);
	p_berg_4.xy *= rot2D(TAU * 0.48);
	p_berg_4.yz *= rot2D(TAU * 0.12);
	float berg_4 = box(p_berg_4, vec3(2.,2.5,1.5));

	berg_3 = smin(berg_3, berg_4, berg_smin);
	berg_2 = smin(berg_2, berg_3, berg_smin);
	berg_1 = smin(berg_1, berg_2, berg_smin);
	float berg = smin(berg_0, berg_1, berg_smin);

	vec2 obj_berg = vec2(berg, berg_id);

	vec3 kristall_p = trans(p, mk_kristall_pos_rt_vec3);
	kristall_p = quat_rotate(kristall_p, mk_kristall_rot_rt_quat);
	vec2 obj_kristall = kristallgruppe(kristall_p);

	return min_material(obj_berg, obj_kristall);
}

vec2 f(vec3 p, bool last_step) {
	float f_bounding = p.y - mk_bounding_height_rt_float;
	bool use_bounding = f_bounding > mk_bounding_offset_rt_float;

	float fels = f_bounding;

	if (!use_bounding) {
		vec3 p_fels = p;

		vec2 domrep_size = vec2(10.);
		vec3 box_size = vec3(1.5);

		float fels_klein = fels_noise(p_fels, domrep_size, box_size);
		p_fels.xz *= rot2D(TAU * .1);
		float fels_mittel = fels_noise(p_fels, 2. * domrep_size, 2. * box_size);
		p_fels.xz *= rot2D(TAU * .05);
		float fels_gross = fels_noise(p_fels, 4. * domrep_size, 4. * box_size);

		fels = smin(fels_klein, fels_mittel, mk_smin_felsen_rt_float);
		fels = smin(fels, fels_gross, mk_smin_felsen_rt_float);
	}

	vec3 p_hintergrund_kristall1 = trans(p, -35., 10., -5.);
	p_hintergrund_kristall1.xy *= rot2D(TAU * .1);
	float f_hintergrund_kristall1 = kristall(p_hintergrund_kristall1, mk_hintergrund_kristall_h_rt_float, mk_hintergrund_kristall_r_rt_float, mk_hintergrund_kristall_cap_rt_float);

	vec3 p_hintergrund_kristall2 = trans(p, 85., 10., -35.);
	p_hintergrund_kristall2.zy *= rot2D(TAU * .08);
	float f_hintergrund_kristall2 = kristall(p_hintergrund_kristall2, mk_hintergrund_kristall_h_rt_float, mk_hintergrund_kristall_r_rt_float, mk_hintergrund_kristall_cap_rt_float);

	vec3 p_hintergrund_kristall3 = trans(p, 5., 10., -95.);
	p_hintergrund_kristall3.zy *= rot2D(TAU * -.12);
	float f_hintergrund_kristall3 = kristall(p_hintergrund_kristall3, mk_hintergrund_kristall_h_rt_float, mk_hintergrund_kristall_r_rt_float, mk_hintergrund_kristall_cap_rt_float);

	float f_hintergrund_kristAlle = f_hintergrund_kristall1;
	f_hintergrund_kristAlle = min(f_hintergrund_kristAlle, f_hintergrund_kristall2);
	f_hintergrund_kristAlle = min(f_hintergrund_kristAlle, f_hintergrund_kristall3);

	if (use_bounding) {
		fels = min(fels, f_hintergrund_kristAlle);
	} else {
		fels = smin(fels, f_hintergrund_kristAlle, mk_smin_felsen_rt_float);
	}

	vec2 obj_fels = vec2(fels, fels_id);

	vec3 p_huegel = trans(p, mk_huegel_pos_rt_vec3);
	vec2 obj_huegel = huegel(p_huegel);

	if (use_bounding) {
		obj_fels = min_material(obj_fels, obj_huegel);
	} else {
		obj_fels = smin_material(obj_fels, obj_huegel, mk_smin_felsen_rt_float);
	}

	return obj_fels;
}
