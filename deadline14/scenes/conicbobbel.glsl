#include "scene_head.glsl"
#include "rtificial.glsl"
#include "bobbel.glsl"
#line 5

uniform vec3 conic_bobbel_color; // color
uniform vec3 conic_ring_color; // color
uniform vec3 conic_lampe_color; // color

uniform vec3 conic_light1_pos;
uniform vec3 conic_light2_pos;
uniform vec3 conic_light1_col; // color
uniform vec3 conic_light2_col; // color
uniform float conic_light_radius;
uniform float conic_light_intensity;

uniform float conic_smooth_factor; // float
uniform float conic_ring_intensity; // float
uniform float conic_bobbel_xcoord;
uniform float conic_domrep_spacing;
uniform bool  conic_domrep_enabled;

vec3 colors[5] = vec3[5](
		vec3(.0),
		vec3(.03, .0, .0),
		conic_lampe_color,
		conic_bobbel_color,
		conic_ring_color
		);

const float material_bounding = 0.;
const float material_boden    = 1.;
const float material_lampe    = 2.;
const float material_bobbel   = 3.;
const float material_ring     = 4.;

vec3 apply_lights(vec3 p, vec3 N, vec3 V, Material mat) {
	vec3 color = vec3(0.);
	color += apply_light(p, N, V, mat, SphereLight(conic_light1_pos, conic_light1_col, conic_light_radius, conic_light_intensity));
	color += apply_light(p, N, V, mat, SphereLight(conic_light2_pos, conic_light2_col, conic_light_radius, conic_light_intensity));
	return color;
}

void main(void) {
	vec3 direction = get_direction();

	int i;
	vec3 hit = march_adv(camera_position, direction, i, 100, 1.);

	vec3 color = vec3(0.);
	float materialId = f(hit)[1];
	vec3 normal = calc_normal(hit);

	float bobbel_metallic = 1.;
	float metallic_bias = pow(cfbm(vnoise(hit.zy) * 30. + cross(hit - time * .4, hit.yzx + time * 3.) * .02) * .5 + .5, .05);
	float bobbel_rough = .3;
	bobbel_rough += .07 * cnoise(hit * 9. + 23. + metallic_bias * 15. + time * .5);

	SphereLight light1 = SphereLight(vec3(5., 9., 10.) * 3., vec3(1.), 2., 100.);
	if(materialId == material_boden) {
		float size = 2.;
		float stripes = mod(floor(hit.x / size), 2.);
		Material mat = Material(colors[int(material_boden)] * stripes, 0.5, 0.);
		color = apply_lights(hit, normal, -direction, mat);
	} else if(materialId == material_bounding) {
		Material mat = Material(colors[int(material_bounding)], 1., 0.);
		color = apply_lights(hit, normal, -direction, mat);
	} else if (materialId >= material_lampe && materialId <= material_bobbel) {
		Material material1 = Material(colors[int(material_lampe)], 0.9, .9);
		vec3 color1 = apply_lights(hit, normal, -direction, material1);
		Material material2 = Material(colors[int(material_bobbel)], bobbel_rough, bobbel_metallic);
		vec3 color2 = apply_lights(hit, normal, -direction, material2);
		float mixfactor = pow(materialId - material_lampe, 3.); // change the exponent for sharpness of transition
		color = mix(color1, color2, mixfactor);
	} else if (materialId >= material_bobbel && materialId <= material_ring) {
		Material material1 = Material(colors[int(material_bobbel)], bobbel_rough, bobbel_metallic);
		vec3 color1 = apply_lights(hit, normal, -direction, material1);
		vec3 color2 = emit_light(colors[int(material_ring)], conic_ring_intensity);
		float mixfactor = pow(materialId - material_bobbel, 6.); // change the exponent for sharpness of transition
		color = mix(color1, color2, mixfactor);
	} else {
		Material mat = Material(colors[int(materialId)], 0.2, 1.);
		color = apply_lights(hit, normal, -direction, mat);
	}

	output_color(color, distance(hit, camera_position));
}

vec2 f(vec3 p) {
	vec3 p_bobbel = trans(p, conic_bobbel_xcoord, 0., -3.);

	vec3 s_domrep = conic_domrep_spacing * vec3(10, 13., 9.); // domrep cell size, 1. probably means no domrep
	vec3 p_pre_cone = p_bobbel;
	//p_pre_cone = trans(p_bobbel, 10. * time, 0., 0.); // move with time
	// find the current cell
	vec3 cell = floor(p_pre_cone / s_domrep) + 100.;
	// move single cells a bit sidewards, maybe amplitude should be smaller than domrep cell
	vec3 translation_vector = conic_domrep_spacing * vec3(
			sin(cell.z * cell.z) * 4.,
			sin(cell.z * cell.x) * 3.,
			sin(cell.x * cell.x) * 3.6);
	p_pre_cone = transv(p_pre_cone, translation_vector);
	vec3 p_cone = domrepv(p_pre_cone, s_domrep);
	//p_cone.y = p_pre_cone.y; // dont't domrep y
	if (!conic_domrep_enabled) {
		// remove for repetition and movement
		p_cone = p_bobbel;
	}

	// get bobbel body and rings
	float l_body = 2.; // length of body
	float f_cone = conicbobbel(p_cone, l_body);
	float f_ring = bobbelring(p_cone, l_body, 0.);
	//f_ring = min(f_ring, bobbelring(p_cone, l_body, 1/2.5));

	////// assembling and boundings
	vec2 m_cone = vec2(f_cone, material_bobbel);
	vec2 m_ring = vec2(f_ring, material_ring);
	vec2 m_bobbel = smin_smaterial(m_cone, m_ring, conic_smooth_factor);

	// lampen sphere and tunnel for bobbel
	float radius_lampe = 10.;
	vec3 p_lampe = trans(p, -15, 0, -3);
	float f_lampe = sphere(p_lampe, radius_lampe);

	vec3 p_delle = p_bobbel;
	float f_delle = line(p_delle, vec3(1.7, 0., 0.), vec3(.1), 1.);
	f_lampe = smax(f_lampe, -f_delle, 1.);

	float lampe2 = sphere(trans(p_lampe, -7., -20., -50.), 1.4 * radius_lampe);
	float lampe3 = sphere(trans(p_lampe, 8., 4., 50.), 1.7 * radius_lampe);
	float lampe4 = sphere(trans(p_lampe, 3., 17., -40.), .7 * radius_lampe);
	f_lampe = min(min(f_lampe, lampe2), min(lampe3, lampe4));

	vec2 m_lampe = vec2(f_lampe, material_lampe);

	vec2 m_bobbel_lampe = smin_smaterial(m_bobbel, m_lampe, 1.);

	vec2 m_content = m_bobbel_lampe;
	vec2 bounding = vec2(-sphere(p - camera_position, 100.), material_bounding);
	return min_material(m_content, bounding);
}
