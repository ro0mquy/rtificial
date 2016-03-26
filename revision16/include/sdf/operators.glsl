#include "helper.glsl"
#include "sdf/domain.glsl"
#line 3

// unions two object and produces a very smooth transition
// affects an area of r between the meet point, underestimates the distance
// can be used with any kind of objects
float opUnionSmooth(float f1, float f2, float r) {
	// maybe remove this min() and if() stuff
	//float f_min = min(f1, f2);
	if (f1 < r && f2 < r) {
		float h = clamp(.5 + .5 * (f2 - f1) / r, 0., 1. );
		float f_smooth = mix(f2, f1, h) - r * h * (1. - h);
		return f_smooth;
		//return min(f_smooth, f_min);
	}
	float f_min = min(f1, f2);
	return f_min;
}

// intersects two object and produces a very smooth transition
// affects an area of r between the meet point, underestimates the distance
// can be used with any kind of objects
float opIntersectSmooth(float f1, float f2, float r) {
	// maybe remove this max() and if() stuff
	//float f_max = max(f1, f2);
	//if (f1 > -r && f2 > -r) {
	if (abs(f1) < r && abs(f2) < r) {
		float h = clamp(.5 - .5 * (f2 - f1) / r, 0., 1. );
		float f_smooth = mix(f2, f1, h) + r * h * (1. - h);
		return f_smooth;
		//return max(f_smooth, f_max);
	}
	float f_max = max(f1, f2);
	return f_max;
}

// subtracts f2 from f1 and produces a very smooth transition
// affects an area of r between the meet point, underestimates the distance
// can be used with any kind of objects
float opSubtractSmooth(float f1, float f2, float r) {
	return opIntersectSmooth(f1, -f2, r);
}

// all hail the smin
float smin(float f1, float f2, float r) {
	return opUnionSmooth(f1, f2, r);
}

float smax(float f1, float f2, float r) {
	return opIntersectSmooth(f1, f2, r);
}

// unions two objects and adds rounded edges with radius r
// use only with orthogonal objects
float opUnionRounded(float f1, float f2, float r) {
	float f_min = min(f1, f2);
	if (f1 < r && f2 < r) {
		vec2 q = vec2(f1, f2);
		vec2 center = vec2(r);
		float f_round = r - distance(center, q); // inside of fSphere2
		return min(f_round, f_min);
	}
	return f_min;
}

// intersects two objects and adds rounded edges with radius r
// use only with orthogonal objects
float opIntersectRounded(float f1, float f2, float r) {
	float f_max = max(f1, f2);
	if (f1 > -r && f2 > -r) {
		vec2 q = vec2(f1, f2);
		vec2 center = vec2(-r);
		float f_round = distance(center, q) - r;
		return max(f_round, f_max);
	}
	return f_max;
}

// subtracts f2 from f1 and adds rounded edges with radius r
// use only with orthogonal objects
float opSubtractRounded(float f1, float f2, float r) {
	return opIntersectRounded(f1, -f2, r);
}

// unions two objects and adds some kind of chamfer with radius r
// use only with orthogonal objects
float opUnionChamfer(float f1, float f2, float r) {
	float f_min = min(f1, f2);
	//if (f1 < r && f2 < r) {
		float f_chamfer = sqrt(.5) * (f1 + f2 - r);
		return min(f_chamfer, f_min);
	//}
	//return f_min;
}

// intersects two objects and adds some kind of chamfer with radius r
// use only with orthogonal objects
float opIntersectChamfer(float f1, float f2, float r) {
	float f_max = max(f1, f2);
	//if (f1 > -r && f2 > -r) {
		float f_chamfer = sqrt(.5) * (f1 + f2 + r);
		return max(f_chamfer, f_max);
	//}
	//return f_max;
}

// subtracts f2 from f1 and adds some kind of chamfer with radius r
// use only with orthogonal objects
float opSubtractChamfer(float f1, float f2, float r) {
	return opIntersectChamfer(f1, -f2, r);
}

// like normal max()-intersection but with correct distance at corners
// use only with orthogonal objects
float opIntersectEuclid(float f1, float f2) {
	vec2 q = vec2(f1, f2);
	return min(maxV(q), 0.) + length(max(q, 0.));
}
