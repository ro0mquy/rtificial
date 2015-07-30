#include "helper.glsl"
#include "sdf/operators.glsl"
#line 4 "material"

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


void mUnionChamfer(float f2, MaterialId m, float r, float id_chamfer) {
	//if (calculate_material) {
		float f1 = current_dist;
		float f_chamfer = sqrt(.5) * (f1 + f2 - r);
		mUnion(f2, m);
		mUnion(f_chamfer, newMaterialId(id_chamfer, vec3(f1, f2, 0.)));
	//} else {
		//current_dist = opUnionChamfer(current_dist, f2, r);
	//}
}

void mUnionStairs(float f2, MaterialId m, float r, float n, float id_stairs) {
	//if (calculate_material) {
		float f1 = current_dist;
		//float f_min = min(f1, f2);
		//if (f1 < 2.*r && f2 < 2.*r) {
			vec2 q = vec2(f1, f2);
			float radius = r / n * sqrt(.5);
			q.y -= r - sqrt(.5) * radius;
			q.x -= sqrt(.5) * radius;
			pRot(q, -Tau / 8.);
			pDomrep(q.x, 2. * radius);

			q.x = abs(q.x);
			float offset = radius * sqrt(.5);
			float f_columns = dot(q, vec2(sqrt(.5))) - offset;

			f_columns = min(f_columns, q.y);

			mUnion(f2, m);
			mUnion(f_columns, newMaterialId(id_stairs, vec3(q, 0.)));
			//return min(f_columns, f_min);
		//}
		//return f_min;
	//} else {
		//current_dist = opUnionStairs(current_dist, f2, r, n);
	//}
}

MatWrap mUnionChamfer(MatWrap w1, MatWrap w2, float r, float id_chamfer) {
	//if (calculate_material) {
		MatWrap w_min = mUnion(w1, w2);
		float f_chamfer = sqrt(.5) * (w1.f + w2.f - r);
		return mUnion(w_min, MatWrap(f_chamfer, newMaterialId(id_chamfer, vec3(w1.f, w2.f, 0.))));
	//} else {
		//current_dist = opUnionChamfer(current_dist, f2, r);
	//}
}
