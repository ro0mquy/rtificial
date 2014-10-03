uniform float zapfen_leit_freq;

float leitungen(vec3 p, float rotation, float radius, float freq, float domrep_size) {
	p.xz *= rot2D(rotation);
	float t = vnoise(vec2(p.z, floor(p.x/80.) * floor(p.z/80.)) * zapfen_leit_freq + 333. * freq) * .5 + .5;
	//float t = vnoise(vec2(p.z, 0.) * zapfen_leit_freq + 333. * freq) * .5 + .5;
	//p.xz += rot2D(radians(20.) * t) * vec2(10.) - 10.;

	vec3 q = p;
	q.xz = mod(q.xz, domrep_size) + 5.;
	float angle = atan(q.x, q.z);
	angle = mod(angle, radians(20.));
	float r = length(q.xz);
	q.x = r * sin(angle);
	return length(q.xy) - radius;
}
