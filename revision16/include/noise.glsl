// jenkins hash
uint hash(uint a) {
	a = (a + 0x7ed55d16u) + (a << 12u);
	a = (a ^ 0xc761c23cu) ^ (a >> 19u);
	a = (a + 0x165667b1u) + (a << 5u);
	a = (a + 0xd3a2646cu) ^ (a << 9u);
	a = (a + 0xfd7046c5u) + (a << 3u);
	a = (a ^ 0xb55a4f09u) ^ (a >> 16u);
	return a;
}

// http://webstaff.itn.liu.se/~stegu/simplexnoise/SimplexNoise.java

vec3 simplexGradients[12] = vec3[12](
	vec3(1,1,0), vec3(-1,1,0), vec3(1,-1,0), vec3(-1,-1,0),
	vec3(1,0,1), vec3(-1,0,1), vec3(1,0,-1), vec3(-1,0,-1),
	vec3(0,1,1), vec3(0,-1,1), vec3(0,1,-1), vec3(0,-1,-1)
);

float uintToFloat(uint x) {
	return float(x % (1u << 20u)) / float(1u << 20u);
}

float rand(int x) {
	return uintToFloat(hash(x));
}

float rand(ivec2 x) {
	return uintToFloat(hash(x.x + hash(x.y)));
}

float rand(ivec3 x) {
	return uintToFloat(hash(x.x + hash(x.y + hash(x.z))));
}

float valueNoise(float p) {
	int index = int(floor(p));
	float d = fract(p);
	float v0 = rand(index);
	float v1 = rand(index + 1);
	return mix(v0, v1, d);
}

float valueNoise(vec2 p) {
	ivec2 index = ivec2(floor(p));
	vec2 d = fract(p);
	float result = 0;
	float v00 = rand(index);
	float v01 = rand(index + ivec2(0, 1));
	float v10 = rand(index + ivec2(1, 0));
	float v11 = rand(index + ivec2(1, 1));
	return mix(mix(v00, v10, d.x), mix(v01, v11, d.x), d.y) * 2. - 1.;
}

float smoothNoise(vec2 p) {
	float F2 = .5 * (sqrt(3.) - 1.);
	ivec2 index = ivec2(floor(p + (p.x + p.y) * F2));
	float G2 = (3. - sqrt(3.)) / 6.;
	p -= index - (index.x + index.y) * G2;
	ivec2 index1 = p.x > p.y ? ivec2(1, 0) : ivec2(0, 1);
	vec2 pV[3] = vec2[3](p, p - index1 + G2, p - 1. + 2. * G2);
	ivec3 vertexX = ivec3(index.x) + ivec3(0, index1.x, 1);
	ivec3 vertexY = ivec3(index.y) + ivec3(0, index1.y, 1);
	float result = 0;
	for (uint i = 0; i < 3; i++) {
		float t = max(0, .5 - pV[i].x * pV[i].x - pV[i].y * pV[i].y);
		t *= t;
		uint gradientIndex = hash(vertexX[i] + hash(vertexY[i])) % 12u;
		result += t * t * dot(simplexGradients[gradientIndex].xy, pV[i]);
	}
	return 70. * result;
}

float smoothFbm(vec2 p) {
	float result = 0.;
	for (int i = 0; i < 3; i++) {
		result += exp2(-i) * smoothNoise(p);
		p *= 2.;
	}
	return result / 1.75;
}

float cheapHash(ivec2 c) {
	int x = int(0x3504f333)*c.x*c.x + c.y;
	int y = int(0xf1bbcdcb)*c.y*c.y + c.x;
	return float(x*y)*(2.0/8589934592.0)+0.5;
}

