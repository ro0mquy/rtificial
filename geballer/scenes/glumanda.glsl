#include "march.glsl"
#include "noise.glsl"
#line 4

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

float fSchwurbelScheisse(vec3 p) {
	float radius = 1;
	float fBoundingCylinder = f2Sphere(p.xz, radius);
	// TODO maybe this can be tuned for performance
	if (fBoundingCylinder > radius * .5) {
		return fBoundingCylinder;
	}
	float a = .5 + .5 * 1;
	pRotY(p, sin(a * p.y) + valueNoise(time) * Pi);
	float lipTwist = length(vec2(radius, Pi * a));
	pDomrepAngle(p.xz, 3, radius);
	pMirror(p.z);
	pFlip(p.z);
	pRotY(p, Tau / 5);
	return p.x / lipTwist;
}

float fScene(vec3 p) {
	/* rotationswalzenspimischlonz
	pRotY(p, Tau * time * .2);
	pRotZ(p, Tau * time * .2);
	pMirrorGrid(p, 10);
	p = p.zxy;
	vec3 p_dings = p;
	pRotY(p, Tau * time * .1);
	pRotZ(p, atan(1/sqrt(2)));
	pRotX(p, Tau / 8);
	vec3 p_spimi = p;
	pMirrorGrid(p_spimi, 5);
	float spimi_scale = .2;
	p_spimi.x *= -1;
	pTrans(p_spimi.y, spimi_scale * glum_spimi_disc_height_rt_float);
	float f = fSpimi(p_spimi.yxz, spimi_scale);
	f = min(f, fBox(p, 1));

	pTrans(p_dings.y, -10);
	float f_dings = fHexprism(p_dings, 1, 10);
	f = opUnionChamfer(f, f_dings, .1);
	//*/

	/* tunneldings vielleicht
	//pMirrorLoco(p.yx, vec2(12));
	float f;// = f2Triprism(p.xy, 1);
	vec3 p1 = p;
	float i = pDomrep(p1.z, 3);
	vec3 p2 = p1;
	pTrans(p1.x, rand(int(i)) * 10);
	pDomrepMirror(p1.x, 1);
	float sin_v = sin(.5 *  i + 3 * time);
	sin_v = pow(abs(sin_v), .2) * sgn(sin_v);
	pRotZ(p1, Tau / (20 * (1. + .4 * sin_v)));
	f = f2Box(p1.xz, vec2(.1, 1));
	pMirrorTrans(p2.z, 1.25);
	f = opUnionChamfer(f, f2BoxEdge(p2.yz, vec2(1.3, .25)), .1);
	//*/

	/* auf-und-zu-mit-klingen
	// more like hakenkreuzdingsi mit schrecklichen stacheln
	float mirroring = pMirrorTrans(p.y, 2 * (.5 * .66 + .125) + .05);
	vec3 p_base = p;
	float f_base = fHexprism(p_base, .5, .1);
	vec3 p_pole1 = p_base;
	pTrans(p_pole1.y, -.5 * .66 - .05);
	float f_pole1 = fCylinder(p_pole1, .1, .5 * .66);
	vec3 p_pole2 = p_pole1;
	pTrans(p_pole2.y, -.5 * .66 - .125);
	float f_pole2 = fCylinder(p_pole2, .05, .125);
	f_pole1 = min(f_pole1, f_pole2);

	vec3 p_blade = p_pole1;
	float f_blade = fCylinder(p_blade, 2, .01);
	if (f_blade < .1) {
		pRotY(p_blade, Tau / 3 * .25 * mirroring);
		pDomrepAngle(p_blade.xz, 3, 1);
		float t = 0.5;
		vec2 mirror_normal = unitVector(Tau * .5 - radians(45) * (1. - t));
		pMirrorAtPlane(p_blade, vec3(mirror_normal.x, 0., mirror_normal.y), 0);
		f_blade = fBox(p_blade, vec3(1, .01, .1));
		pMirror(p_blade.z);
		pTrans(p_blade.z, .1 + .025);
		pTrans(p_blade.x, .01);
		p_blade.x = -p_blade.x;
		pDomrepInterval(p_blade.x, .2, 1, 5);
		p_blade = p_blade.yzx;
		float f_spikes = fConeCapped(p_blade, .01, .0, .05);
		f_blade = min(f_blade, f_spikes);
		f_blade = min(f_blade, p_pole2.y);
	}

	float f = min(f_base, f_pole1);
	f = min(f, f_blade);
	//*/

	//float f = fSchwurbelScheisse(p);
	//f = max(f, p.y);

	float f = fSphere(p, 1);

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
