#include "scene_head.glsl"
#include "rtificial.glsl"
#include "noise.glsl"
#include "fels_color.glsl"
#include "biene.glsl"
#include "background.glsl"
#line 7

const float boden_id = 0.;
const float fels_id = 1.;
const float berg_id = 2.;
const float kristall_id = 3.;
const float kristAlle_id = 4.;
const float kristall2_id = 5.;

vec3 p_kristall;

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
	float t = march(o, d, 300., screenDist);

	float material_id = f(o+t*d, true)[1];

	vec3 out_color;

	vec3 sky_color = environmentColor(o, d, 300.);
	if (isinf(t)) {
		out_color.rgb = sky_color;
	} else {
		vec3 p = o + t * d;
		vec3 normal = calc_normal(p, false);
		out_color.rgb = vec3(1.);

		if (material_id == kristall_id || material_id == kristall2_id) {
			vec3 p_noise;
			if (material_id == kristall_id) {
				p_noise = p_kristall;
			} else {
				p_noise = p;
			}
			out_color = augenlicht(p_noise, d, normal);
		} else if (material_id == kristAlle_id) {
			float rough = mk_kristAlle_roughness_rt_float;
			float metallic = mk_kristAlle_metallic_rt_float;
			fels_color(p, out_color, rough);

			out_color.rgb = ambientColor(normal, -d, out_color.rgb, rough, metallic);
			out_color = mix(out_color, sky_color, pow(smoothstep(200., 300., t), 3.));

			vec3 kristAllColor = augenlicht(p, d, normal);
			out_color = kristAll_color(p, out_color, kristAllColor);

			out_color.rgb *= clamp(ao(o + t * d, normal, .4, 5), 0., 1.);
			out_color.rgb = applyFog(out_color.rgb, t, o, d);
		} else {
			float rough = 1.;
			float metallic = 0.;
			//if (material_id == fels_id || material_id == berg_id) {
				fels_color(p, out_color, rough);
			//}

			out_color.rgb = ambientColor(normal, -d, out_color.rgb, rough, metallic);
			out_color = mix(out_color, sky_color, pow(smoothstep(200., 300., t), 3.));

			//out_color = vec3(cellnoise);

			//out_color.rgb = abs(normal);
			out_color.rgb *= clamp(ao(o + t * d, normal, .4, 5), 0., 1.);
			out_color.rgb = applyFog(out_color.rgb, t, o, d);
		}
	}
	output_color(out_color, t);

}

vec2 kristallgruppe(vec3 p){
	vec3 p_kristall1 = trans(p, mk_kristall1_trans_relative_rt_vec3);
	// invert rotation
	vec4 rot_quat = vec4(-mk_kristall_rot_rt_quat.xyz, mk_kristall_rot_rt_quat.w);
	rot_quat = normalize(mix(vec4(0.,0.,0.,1.), rot_quat, mk_kristall1_fullrot_rt_float));
	p_kristall1 = quat_rotate(p_kristall1, rot_quat);
	p_kristall1.xz *= rot2D(TAU * mk_kristall1_eigenrot_rt_float);
	p_kristall = p_kristall1;
	float f_kristall = kristall(
		abs(p_kristall1),
		mk_kristall_h_rt_float,
		mk_kristall_r_rt_float,
		mk_kristall_cap_rt_float
	);

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

	vec2 m_kristall = vec2(f_kristall, kristall_id);
	vec2 m_kristall2 = vec2(min(kristall2, kristall3), kristall2_id);
	return min_material(m_kristall, m_kristall2);
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

	vec2 obj_fels = vec2(fels, fels_id);
	vec2 obj_hintergrund_kristAlle = vec2(f_hintergrund_kristAlle, kristAlle_id);
	if (use_bounding) {
		obj_fels = min_material(obj_fels, obj_hintergrund_kristAlle);
	} else {
		obj_fels = smin_material(obj_fels, obj_hintergrund_kristAlle, mk_smin_felsen_rt_float);
	}

	vec3 p_huegel = trans(p, mk_huegel_pos_rt_vec3);
	vec2 obj_huegel = huegel(p_huegel);

	if (use_bounding) {
		obj_fels = min_material(obj_fels, obj_huegel);
	} else {
		obj_fels = smin_material(obj_fels, obj_huegel, mk_smin_felsen_rt_float);
	}

	return obj_fels;
}
