#include "helper.glsl"
#include "sdf/domain.glsl"
#line 3 "sdf/operators"

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

// unions two objects and adds some kind of smooth chamfer with radius r
// use only with orthogonal objects
float opUnionChamferSmooth(float f1, float f2, float r) {
	float f_min = min(f1, f2);
	//if (f1 < r && f2 < r) {
		float f_chamfer = sqrt(.5) * (f1 + f2 - r);
		return opUnionSmooth(f_chamfer, f_min, .1 * r);
	//}
	//return f_min;
}

// intersects two objects and adds some kind of chamfer with radius r
// use only with orthogonal objects
float opIntersectChamferSmooth(float f1, float f2, float r) {
	float f_max = max(f1, f2);
	//if (f1 > -r && f2 > -r) {
		float f_chamfer = sqrt(.5) * (f1 + f2 + r);
		return opIntersectSmooth(f_chamfer, f_max, .1 * r);
	//}
	//return f_max;
}

// subtracts f2 from f1 and adds some kind of chamfer with radius r
// use only with orthogonal objects
float opSubtractChamferSmooth(float f1, float f2, float r) {
	return opIntersectChamferSmooth(f1, -f2, r);
}

// unions two objects and adds n columns within a radius of r
// use only with orthogonal objects
float opUnionColumns(float f1, float f2, float r, float n) {
	// Speckröllchen
	float f_min = min(f1, f2);
	if (f1 < 2.*r && f2 < 2.*r) {
		vec2 q = vec2(f1, f2);
		//float radius = sqrt(2.) * .5 * r / (n + sqrt(2.) - 1.);
		//q.x -= radius;
		float radius = sqrt(.5) * r / (n + sqrt(.5) - 1.);
		q.y -= r - radius;
		pRot(q, -Tau / 8.);
		pDomrepInterval(q.x, 2. * radius, 0., n-1.);
		float f_columns = length(q) - radius;
		f_columns = min(f_columns, q.y);
		return min(f_columns, f_min);
	}
	return f_min;
}

// intersects two objects and adds n columns within a radius of r
// use only with orthogonal objects
float opIntersectColumns(float f1, float f2, float r, float n) {
	// Speckröllchen
	float f_max = max(f1, f2);
	if (f1 > -r && f2 > -r) {
		vec2 q = vec2(f1, f2);
		//float radius = sqrt(2.) * .5 * r / (n + sqrt(2.) - 1.);
		//q.x -= radius;
		float radius = sqrt(.5) * r / (n + sqrt(.5) - 1.);
		q.x -= -r;
		q.y -= -radius;
		pRot(q, -Tau / 8.);
		pDomrepInterval(q.x, 2. * radius, 0., n-1.);
		float f_columns = length(q) - radius;
		f_columns = min(f_columns, q.y);
		return max(f_columns, f_max);
	}
	return f_max;
}

// subtracts f2 from f1 and adds n columns within a radius of r
// use only with orthogonal objects
float opSubtractColumns(float f1, float f2, float r, float n) {
	return opIntersectColumns(f1, -f2, r, n);
}

// unions two objects and adds n stairs within a radius of r
// use only with orthogonal objects
float opUnionStairs(float f1, float f2, float r, float n) {
	// Speckröllchen
	float f_min = min(f1, f2);
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
		return min(f_columns, f_min);
	//}
	//return f_min;
}

// intersects two objects and adds n stairs within a radius of r
// use only with orthogonal objects
float opIntersectStairs(float f1, float f2, float r, float n) {
	// stairs are symmetric, so we can use opUnionStairs
	return -opUnionStairs(-f1, -f2, r, n);
}

// subtracts f2 from f1 and adds n stairs within a radius of r
// use only with orthogonal objects
float opSubtractStairs(float f1, float f2, float r, float n) {
	return opIntersectStairs(f1, -f2, r, n);
}

// like normal min()-union but with correct distance at corners
// gives correct interior
// use only with orthogonal objects
float opUnionEuclid(float f1, float f2) {
	vec2 q = vec2(f1, f2);
	return max(minV(q), 0.) - length(min(q, 0.));
}

// like normal max()-intersection but with correct distance at corners
// use only with orthogonal objects
float opIntersectEuclid(float f1, float f2) {
	vec2 q = vec2(f1, f2);
	return min(maxV(q), 0.) + length(max(q, 0.));
}

// like normal max()-subtraction but with correct distance at corners
// use only with orthogonal objects
float opSubtractEuclid(float f1, float f2) {
	return opIntersectEuclid(f1, -f2);
}
