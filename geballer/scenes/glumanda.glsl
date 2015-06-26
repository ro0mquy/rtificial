#include "march.glsl"
#line 3

float fSpimiDisc(vec3 p, float scale, float i, float discAtan) {
	float innerRadius = glum_spimi_radius_rt_float + sin(i) * glum_spimi_radius_var_rt_float;
	innerRadius *= scale;
	// TODO parameterize
	float rotationAngle = Tau / 10 * i / (5 + 3.5 * sin(time));
	pRotY(p, rotationAngle);
	pDomrepAngleWithAtan(p.xz, glum_spimi_n_spikes_rt_float, innerRadius, rotationAngle + discAtan);
	float disc_height = glum_spimi_disc_height_rt_float * scale;
	float d = fBox(p, vec3(disc_height, disc_height, scale));
	pTrans(p.x, disc_height + glum_spimi_spike_length_rt_float * scale);
	float d2 = fConeCapped(p.zxy, disc_height - .05 * scale, disc_height * glum_spimi_spike_cap_rt_float, glum_spimi_spike_length_rt_float * scale);
	return opUnionChamfer(d, d2, .05 * scale);
}

float fSpimi(vec3 p, float scale) {
	float maxSpimiRadius = glum_spimi_radius_rt_float + glum_spimi_radius_var_rt_float;
	maxSpimiRadius += glum_spimi_disc_height_rt_float + glum_spimi_spike_length_rt_float * 2;
	maxSpimiRadius *= scale;
	float f_bounding = max(f2Sphere(p.xz, maxSpimiRadius), -p.y);
	if (f_bounding > 1) {
		return f_bounding;
	}
	float c = 2 * glum_spimi_disc_height_rt_float * scale;
	float i = pDomrepSingle(p.y, c);
	//float i = 0;
	float discAtan = atan(p.z, p.x);
	// TODO use simpler objects as guards
	pTrans(p.y, c);
	float d1 = fSpimiDisc(p, scale, i + 1, discAtan);
	pTrans(p.y, -c);
	float d2 = fSpimiDisc(p, scale, i, discAtan);
	pTrans(p.y, -c);
	float d3 = fSpimiDisc(p, scale, i - 1, discAtan);
	return min(d1, min(d2, d3));
}

float fScene(vec3 p) {
	pRotY(p, Tau * time * .1);
	pRotZ(p, atan(1/sqrt(2)));
	pRotX(p, Tau / 8);
	vec3 p_spimi = p;
	pMirrorGrid(p_spimi.xzy, 2);
	float f = fSpimi(p_spimi, .2);
	f = min(f, fBox(p, 2));

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
	return mat;
}
