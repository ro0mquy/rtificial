#include "march.glsl"
#line 3

void pFlip(inout float p) {
	p = -p;
}

// see pMirrorGrid(vec3, float)
vec2 pMirrorGrid(inout vec2 p, float c) {
	vec2 q = p;
	pMirror(q);
	vec2 s = vec2(0);
	if (q.y > q.x) {
		p = p.yx;
		s.x = 1;
	}
	s.y = pMirrorTrans(p.x, c);
	p.y *= s.y;
	return s;
}

float f2Torus(vec2 p, float rBig, float rSmall) {
	 float d = f2Sphere(p, rBig);
	 pDomrepSingle(d, 1);
	 pMirrorTrans(d, rSmall);
	 return d;
}

float f2TorusBox(vec2 p, float rBig, float rSmall) {
	 float d = f2BoxEdge(p, rBig);
	 pMirrorTrans(d, rSmall);
	 return d;
}

float f2Line(vec2 p, float r, vec2 v) {
	float h = saturate(dot(p, v)/ dot(v, v));
	return f2Sphere(p - v*h, r);
}


float fInner(vec3 p, float t) {
	float torus_mirror_grid_1 = t;
	float torus_mirror_grid_2 = 1 - t;
	float torus_rotz = iqPowerCurve(.1, .3, fract(t));
	pMirrorGrid(p.xy, torus_mirror_grid_1);
	pRotZ(p, (torus_rotz - .5) * Tau);
	pMirrorGrid(p.xy, torus_mirror_grid_2);
	float f1 = f2TorusBox(p.xy, 1, .05);
	float f2 = f2Torus(p.xy, 1, .1);
	float f = min(f1, f2);
	f = f2;
	return f;
}

float fScene(vec3 p) {
	float i = pDomrepInterval(p.z, 3., -20, 0);
	//float i = 0;
	float frame = f2Box(p.xy, vec2(5));
	float t = (i/20) +1 + layers_animation_t_rt_float;
	float inner = fInner(p, t);
	pMirrorTrans(p.z, .01);
	float f = p.z;
	f = max(f, min(-frame, inner));

	mUnion(f, MaterialId(0., vec3(p.x, p.y, t)));
	return f;
}

vec3 applyNormalLights2(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, Material material) {
	material.color = .5 * normal + .5;
	material.roughness = 1.;
	vec3 color = .1 * ambientColor(normal, -direction, material);
	return color;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material) {
	//return applyNormalLights2(origin, marched, direction, hit, normal, material);
	return .1 * ambientColor(normal, -direction, material);
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return color;
}

Material getMaterial(MaterialId materialId) {
	Material mat = defaultMaterial(vec3(1));
	mat.roughness = 1;
	mat.height = sin(materialId.coord.x * 2) * cos(materialId.coord.y * 2) * .1;
	mat.color = mix(layers_color1_rt_color, layers_color2_rt_color, fract(materialId.coord.z));
	mat.color *= .3;
	//mat.color = vec3(.5);
	return mat;
}
