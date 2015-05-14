#include "helper.glsl"
#line 3 "material"

struct Material {
	float id;
	vec3 coord;
};

bool calculate_material = false;
float current_dist = Inf;
Material current_material = Material(0., vec3(0.));

// der witz: jetzt in einmal komplett neu!
void mUnion(float f, Material m) {
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
void mIntersect(float f, Material m) {
	if (calculate_material) {
		if (f > current_dist) {
			current_dist = f;
			current_material = m;
		}
	} else {
		current_dist = max(current_dist, f);
	}
}
