#include "fragment.glsl"
#include "camera.glsl"
#include "noise.glsl"
#line 5

struct Intersection {
	bool hit;
	float d;
	vec3 normal;
};

struct Ray {
	vec3 pos;
	vec3 dir;
};

struct Plane {
	vec3 normal;
	float offset;
};

Intersection intersectPlane(Ray ray, Plane plane) {
	float LdotN = dot(ray.dir, plane.normal);
	if (LdotN > -1e-6) {
		return Intersection(false, 0, vec3(0));
	} else {
		float d = (plane.offset - dot(ray.pos, plane.normal)) / LdotN;
		if (d > 1000) {
			return Intersection(false, 0, vec3(0));
		}
		return Intersection(true, d, plane.normal);
	}
}

Intersection combineIntersection(Intersection intersection1, Intersection intersection2) {
	if (!intersection1.hit) {
		return intersection2;
	}
	if (!intersection2.hit) {
		return intersection1;
	}
	if (intersection1.d < intersection2.d) {
		return intersection1;
	} else {
		return intersection2;
	}
}

vec3 camGetDirectionOffset(vec2 offset) {
	vec2 coord = gl_FragCoord.xy + offset;
	float screenD;
	vec3 dir = camGetDirectionSS(coord, screenD);
	dir.z = -dir.z;
	if (spectatormode_active) {
		pQuatRotate(dir, spectatormode_rotation);
	} else {
		pQuatRotate(dir, camera_rotation);
	}
	return dir;
}

// pretty bad, but good enough for now
float randBad(vec2 co){
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec3 sampleGGXVNDF(vec3 V_, float alpha, vec2 U) {
	// stretch view
	vec3 V = normalize(vec3(alpha * V_.xy, V_.z));

	// orthonormal basis
	vec3 T1 = (V.z < 0.9999) ? normalize(cross(V, vec3(0, 0, 1))) : vec3(1, 0, 0);
	vec3 T2 = cross(T1, V);

	// sample point with polar coordinates (r, phi)
	float a = 1./ ( 1. + V.z);
	float r = sqrt(U.x);
	float phi = (U.y < a) ? U.y / a * Pi : Pi + (U.y - a) / (1. - a) * Pi;
	float P1 = r * cos(phi);
	float P2 = r * sin(phi) * ((U.y < a) ? 1. : V.z);

	// compute normal
	vec3 N = P1 * T1 + P2 * T2 + sqrt(1. - P1 * P1 - P2 * P2) * V;

	// unstretch
	N = normalize(vec3(alpha * N.xy, N.z));
	return N;
}

bool sampleDirection(vec3 wiWorld, vec3 normal, float alpha, vec2 U, out vec3 woWorld, out vec3 weight) {
	vec3 t;
	if (abs(normal.x) > abs(normal.y)) {
		float invLength = 1./length(normal.xz);
		t = vec3(normal.z * invLength, 0, -normal.x * invLength);
	} else {
		float invLength = 1./length(normal.yz);
		t = vec3(0, normal.z * invLength, -normal.y * invLength);
	}
	vec3 s = cross(t, normal);

	vec3 wiLocal = vec3(
		dot(wiWorld, s),
		dot(wiWorld, t),
		dot(wiWorld, normal)
	);
	vec3 normalLocal = sampleGGXVNDF(wiLocal, alpha, U);
	vec3 woLocal = reflect(-wiLocal, normalLocal);
	if (woLocal.z < 1e-6) {
		return false;
	}
	woWorld = s * woLocal.x + t * woLocal.y + normal * woLocal.z;
	float theta = dot(normalLocal, wiLocal);
	vec3 f0 = vec3(.5, .1, .3);
	vec3 schlick = f0 + (1. - f0) * pow(1. - theta, 5.);
	if (dot(woLocal, normalLocal) < 1e-6) {
		return false;
	}
	weight = schlick;
	float temp = 1 - woLocal.z * woLocal.z;
	float tanTheta;
	if (temp < 0.) {
		tanTheta = 0;
	} else {
		tanTheta = sqrt(temp);
	}
	float root = alpha * tanTheta;
	weight *= 2.0 / (1. + length(vec2(1, root)));
	return true;
}

void main() {
	out_color = vec3(0.0);
	int numSamples = 64;
	int numBounces = 3;
	Plane ground = Plane(vec3(0, 1, 0), -10);
	Plane wall = Plane(vec3(0, 0, 1), -10);
	Plane wall2 = Plane(vec3(0, 0, -1), -10);
	Plane ceiling = Plane(vec3(0, -1, 0), -10);
	for (int i = 0; i < numSamples; i++) {
		vec2 jitter = vec2(
			randBad(gl_FragCoord.xy * randBad(gl_FragCoord.yx * (i + 1) + time)),
			randBad(gl_FragCoord.yx * randBad(gl_FragCoord.xy * (i + 1) + time))
		) - .5;
		Ray ray = Ray(camGetPosition(), camGetDirectionOffset(jitter));
		ray.pos.x -= time * 100;
		vec3 throughput = vec3(1);
		for (int j = 0; j < numBounces; j++) {
			Intersection intersectGround = intersectPlane(ray, ground);
			Intersection intersectWall = intersectPlane(ray, wall);
			Intersection intersectWall2 = intersectPlane(ray, wall2);
			Intersection intersectCeil = intersectPlane(ray, ceiling);
			Intersection intersect = combineIntersection(intersectGround, intersectWall);
			intersect = combineIntersection(intersect, intersectCeil);
			intersect = combineIntersection(intersect, intersectWall2);

			if (intersect.hit) {
				vec3 p = ray.pos + intersect.d * ray.dir;
				//out_color = intersect.normal;//vec3(sin(p.x) * .5 + .5);
				vec3 q = p;
				float a = atan(q.y, q.x);
				//out_color += vec3(pow(sin(length(q) + a * 10.) * .5 + .5, 30.)) * throughput;
				out_color += vec3(pow(randBad(vec2(floor(p.x * .5), floor(1e-6 + p.y * .5))), 10.)) * throughput;
				//ray.dir = reflect(ray.dir, intersect.normal);
				vec2 U = vec2(
					randBad(gl_FragCoord.xy * (23. + i * numSamples + j)),
					randBad(gl_FragCoord.yx * (12. + i * numSamples + j))
				);
				vec3 newDir;
				vec3 weight;
				float alpha  = .05;//= mix(.01, .1, saturate(smoothNoise(.1 * p + valueNoise(p * .2))));
				if (!sampleDirection(-ray.dir, intersect.normal, alpha, U, newDir, weight)) {
					break;
				}
				ray.dir = newDir;
				throughput *= weight;
				ray.pos = p + 1e-6 * ray.dir;
			} else {
				out_color += vec3(1., 1., 1.) * throughput;
				break;
			}
		}
	}
	out_color /= numSamples;
	out_color = pow(out_color, vec3(1./2.2));
	out_depth = 0;
}
