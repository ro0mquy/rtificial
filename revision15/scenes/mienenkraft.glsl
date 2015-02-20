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

/*
vec3 schotter(vec3 p, vec3 domrep_size) {
	vec3 cell = floor(p/domrep_size);
	p.xz = domrep(p, domrep_size).xz;
	p.xy = rot2D(cell.x * domrep_size.x)*p.xy;
	p.xz = rot2D(cell.y * domrep_size.y)*p.xz;
	p.yz = rot2D(cell.z * domrep_size.z)*p.yz;
	return p;
}
*/

float fels_noise(vec3 p_fels, vec2 domrep_size, vec3 box_size) {
	vec2 cell_fels = floor(p_fels.xz / domrep_size);
	p_fels.xz = domrep(p_fels.xz, domrep_size);

	//    1   ^z
	//  4 0 2  >x
	//    3
	vec3 p_fels_0 = p_fels;
	vec3 p_fels_1 = trans(p_fels, 0., 0., -domrep_size.y);
	vec3 p_fels_2 = trans(p_fels,  domrep_size.x, 0., 0.);
	vec3 p_fels_3 = trans(p_fels, 0., 0.,  domrep_size.y);
	vec3 p_fels_4 = trans(p_fels, -domrep_size.x, 0., 0.);

	vec2 cell_fels_0 = cell_fels;
	vec2 cell_fels_1 = cell_fels + vec2( 0., -1.);
	vec2 cell_fels_2 = cell_fels + vec2( 1.,  0.);
	vec2 cell_fels_3 = cell_fels + vec2( 0.,  1.);
	vec2 cell_fels_4 = cell_fels + vec2(-1.,  0.);

	p_fels_0.xy *= rot2D(cell_fels_0.x * domrep_size.x * 32.73101);
	p_fels_1.xy *= rot2D(cell_fels_1.x * domrep_size.x * 32.73101);
	p_fels_2.xy *= rot2D(cell_fels_2.x * domrep_size.x * 32.73101);
	p_fels_3.xy *= rot2D(cell_fels_3.x * domrep_size.x * 32.73101);
	p_fels_4.xy *= rot2D(cell_fels_4.x * domrep_size.x * 32.73101);

	p_fels_0.xz *= rot2D(cell_fels_0.y * domrep_size.y * 49.29012);
	p_fels_1.xz *= rot2D(cell_fels_1.y * domrep_size.y * 49.29012);
	p_fels_2.xz *= rot2D(cell_fels_2.y * domrep_size.y * 49.29012);
	p_fels_3.xz *= rot2D(cell_fels_3.y * domrep_size.y * 49.29012);
	p_fels_4.xz *= rot2D(cell_fels_4.y * domrep_size.y * 49.29012);

	p_fels_0.yz *= rot2D(cell_fels_0.y * domrep_size.x * 52.40165);
	p_fels_1.yz *= rot2D(cell_fels_1.y * domrep_size.x * 52.40165);
	p_fels_2.yz *= rot2D(cell_fels_2.y * domrep_size.x * 52.40165);
	p_fels_3.yz *= rot2D(cell_fels_3.y * domrep_size.x * 52.40165);
	p_fels_4.yz *= rot2D(cell_fels_4.y * domrep_size.x * 52.40165);

	float fels_0 = box(p_fels_0, box_size);
	float fels_1 = box(p_fels_1, box_size);
	float fels_2 = box(p_fels_2, box_size);
	float fels_3 = box(p_fels_3, box_size);
	float fels_4 = box(p_fels_4, box_size);

	float fels_12 = min(fels_1, fels_2);
	float fels_34 = min(fels_3, fels_4);
	float fels_1234 = min(fels_12, fels_34);
	float fels_01234 = smin(fels_0, fels_1234, mk_smin_felsen_rt_float * box_size.x);

	return fels_01234;
}

vec2 f(vec3 p, bool last_step) {
	vec3 p_fels = p;

	vec2 domrep_size = vec2(10.);
	vec3 box_size = vec3(1.5);

	float fels_klein = fels_noise(p_fels, domrep_size, box_size);
	p_fels.xz *= rot2D(TAU * .1);
	float fels_mittel = fels_noise(p_fels, 2. * domrep_size, 2. * box_size);
	p_fels.xz *= rot2D(TAU * .05);
	float fels_gross = fels_noise(p_fels, 4. * domrep_size, 4. * box_size);

	float fels = smin(fels_klein, fels_mittel, mk_smin_felsen_rt_float * 2.);
	fels = smin(fels, fels_gross, mk_smin_felsen_rt_float * 3.);

	float boden = p.y;
	fels = smin(fels, boden, mk_smin_boden_rt_float);

	vec3 p_kristall = p;
	p_kristall = trans(p_kristall, -25., 0., -35.);
	p_kristall.xy *= rot2D(TAU * .1);
	float height_kristall = mk_kristall_h_rt_float;
	float radius_kristall = mk_kristall_r_rt_float;
	float size_cap = mk_kristall_cap_rt_float;
	float r_kristall = radius_kristall * min((height_kristall - radius_kristall) - p_kristall.y, size_cap) / size_cap;
	p_kristall.y -= height_kristall * .5;
	float f_kristall = hexprism(p_kristall.xzy, vec2(r_kristall, height_kristall));

	fels = smin(fels, f_kristall, mk_smin_felsen_rt_float);

	vec2 obj_fels = vec2(fels, fels_id);
	return obj_fels;
	/*
	vec3 domrep_size = vec3(6);
	//vec3 cell = p_fels/domrep_size;
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
	*/
}
