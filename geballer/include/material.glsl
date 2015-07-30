#include "helper.glsl"
#line 3 "material"

struct MaterialId {
	float id;
	vec3 coord;
};

struct Material {
	vec3 color;
	float roughness;
	float metallic;
	float height;
};

bool calculate_material = false;
float current_dist = Inf;
MaterialId current_material = MaterialId(0., vec3(0.));

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

Material defaultMaterial(vec3 color) {
	return Material(
		color,
		.5,
		0,
		0
	);
}
