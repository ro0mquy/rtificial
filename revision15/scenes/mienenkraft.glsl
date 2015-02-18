#include "rtificial.glsl"
#line 3

out vec4 out_color;

const float boden_id = 0.;
const float fels_id = 1.;

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

	if (isinf(t)) {
		out_color.rgb = vec3(0.);

	} else {
		if (material_id <= boden_id) {
			out_color.rgb = vec3(0.2*max(dot(calc_normal(o + t * d, false), normalize(vec3(1., .5, 0.))), 0.) + .1);
		} else if (material_id <= fels_id) {
			out_color.rgb = vec3(max(dot(calc_normal(o + t * d, false), normalize(vec3(1., .5, 2.))), 0.) + .1);
		}
		out_color.rgb = applyFog(out_color.rgb, t, o, d);
	}
}

vec3 schotter(vec3 p, vec3 domrep_size) {
	vec3 cell = floor(p/domrep_size);
	p.xz = domrep(p, domrep_size).xz;
	p.xy = rot2D(cell.x * domrep_size.x)*p.xy;
	p.xz = rot2D(cell.y * domrep_size.y)*p.xz;
	p.yz = rot2D(cell.z * domrep_size.z)*p.yz;
	return p;
}

vec2 f(vec3 p, bool last_step) {
	vec3 p_fels = p;

	vec3 domrep_size = vec3(6);
	vec3 cell = p_fels/domrep_size;
	//vec3 huegel = vec3(0.,2.5*sin(cell.x *0.05) * 2.5*cos(cell.z * 0.05),0.);
	//p_fels = trans(p_fels, huegel);

	vec3 p_fels_1 = p_fels;
	vec3 p_fels_2 = p_fels;
	vec3 p_fels_3 = p_fels;

	p_fels_1.xz = rot2D(TAU/3.)*p_fels_1.xz;
	p_fels_1 = schotter(p_fels_1, domrep_size);

	p_fels_2.xz = rot2D(TAU/4.)*p_fels_2.xz;
	p_fels_2 = schotter(p_fels_2, domrep_size * 3);

	p_fels_3.xz = rot2D(TAU/6.)*p_fels_3.xz;
	p_fels_3 = schotter(p_fels_3, domrep_size * 4);

	float fels_1 = box(p_fels_1, vec3(.5));
	float fels_2 = box(p_fels_2, vec3(2));
	float fels_3 = box(p_fels_3, vec3(4));
	float fels = smin(fels_1, smin(fels_2, fels_3, mk_smin_felsen_rt_float), mk_smin_felsen_rt_float);
	vec2 obj_fels = vec2(fels, fels_id);

	vec3 p_boden = p;
	p_boden = trans(p_boden, 0., .5,0.);
	//p_boden = trans(p_boden, huegel);
	float boden = plane(p_boden, vec3(0.,1.,0.));
	vec2 obj_boden = vec2(boden, boden_id);

	vec3 p_kristall = p;
	p_kristall = trans(p_kristall, -25., 0., -35.);
	p_kristall.xy *= rot2D(TAU * .1);
	float height_kristall = mk_kristall_h_rt_float;
	float radius_kristall = mk_kristall_r_rt_float;
	float size_cap = mk_kristall_cap_rt_float;
	float r_kristall = radius_kristall * min((height_kristall - radius_kristall) - p_kristall.y, size_cap) / size_cap;
	//r_kristall = p_kristall.y;
	p_kristall.y -= height_kristall * .5;
	float f_kristall = hexprism(p_kristall.xzy, vec2(r_kristall, height_kristall));

	obj_fels.x = smin(obj_fels.x, f_kristall, mk_smin_felsen_rt_float);

	return smin_material(obj_fels, obj_boden, mk_smin_boden_rt_float);
}
