#include "helper.glsl"
#include "sdf/operators.glsl"
#line 4

struct MaterialId {
	float id;
	vec3 coord;
	vec4 misc;
};

struct MatWrap {
	float f;
	MaterialId m;
};

struct Material {
	vec3 color;
	float roughness;
	float metallic;
	float height;
	vec3 emission;
};

bool calculate_material = false;
float current_dist = Inf;
MaterialId current_material = MaterialId(0., vec3(0.), vec4(0.));

// der witz: jetzt in einmal komplett neu!
void mUnion(float f, MaterialId m) {
	if (calculate_material) {
		if (f < current_dist) {
			current_dist = f;
			current_material = m;
		}
	} else {
		current_dist = min(current_dist, f);
	}
}

void mUnion(MatWrap w) {
	mUnion(w.f, w.m);
}

MatWrap mUnion(MatWrap w1, MatWrap w2) {
	return w1.f < w2.f ? w1 : w2;
}

MatWrap mIntersect(MatWrap w1, MatWrap w2) {
	return w1.f > w2.f ? w1 : w2;
}

MatWrap mSubtract(MatWrap w1, MatWrap w2) {
	w2.f = -w2.f;
	return mIntersect(w1, w2);
}

// und hier der andere neue witz
void mIntersect(float f, MaterialId m) {
	if (calculate_material) {
		if (f > current_dist) {
			current_dist = f;
			current_material = m;
		}
	} else {
		current_dist = max(current_dist, f);
	}
}

MaterialId newMaterialId(float id, vec3 coord) {
	return MaterialId(id, coord, vec4(0.));
}

Material defaultMaterial(vec3 color) {
	return Material(
		color,
		.5,
		0,
		0,
		vec3(0)
	);
}

