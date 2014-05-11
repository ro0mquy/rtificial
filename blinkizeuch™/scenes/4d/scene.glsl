uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;
uniform vec3 color_background;
uniform vec3 color_cube;

float intersect(vec3 origin, vec3 dir, vec3 v1, vec3 v2, vec3 v3);
mat4 calc_matrix();
vec4 cross4(vec4 a, vec4 b, vec4 c);
mat4 rXY(float theta);
mat4 rYZ(float theta);
mat4 rZX(float theta);
mat4 rXW(float theta);
mat4 rYW(float theta);
mat4 rZW(float theta);

vec4 from = vec4(4, 0, 0, 0);
vec4 to = vec4(0);
vec4 up = vec4(0, 1, 0, 0);
vec4 over = vec4(0, 0, 1, 0);

void main(void){
	vec3 dir = get_direction();
	vec3 final_color = color_background;

	const vec4 p1  = vec4(-1, -1,  1, -1);
	const vec4 p2  = vec4( 1, -1,  1, -1);
	const vec4 p3  = vec4( 1, -1, -1, -1);
	const vec4 p4  = vec4(-1, -1, -1, -1);
	const vec4 p5  = vec4(-1,  1,  1, -1);
	const vec4 p6  = vec4( 1,  1,  1, -1);
	const vec4 p7  = vec4( 1,  1, -1, -1);
	const vec4 p8  = vec4(-1,  1, -1, -1);
	const vec4 p9  = vec4(-1, -1,  1,  1);
	const vec4 p10 = vec4( 1, -1,  1,  1);
	const vec4 p11 = vec4( 1, -1, -1,  1);
	const vec4 p12 = vec4(-1, -1, -1,  1);
	const vec4 p13 = vec4(-1,  1,  1,  1);
	const vec4 p14 = vec4( 1,  1,  1,  1);
	const vec4 p15 = vec4( 1,  1, -1,  1);
	const vec4 p16 = vec4(-1,  1, -1,  1);

	const vec4 triangles[] = vec4[](
		// first cube
		p1, p4, p3, p1, p3, p2, // bottom
		p5, p7, p8, p5, p6, p7, // top
		p1, p5, p8, p1, p8, p4, // left
		p2, p7, p6, p2, p3, p7, // right
		p4, p8, p7, p4, p7, p3, // back
		p1, p6, p5, p1, p2, p6,  // front

		// middle thing
		//bottom connections
		p1, p10, p9, p1, p2, p10,
		p2, p11, p10, p2, p3, p11,
		p3, p12, p11, p3, p4, p12,
		p4, p9, p12, p4, p1, p9,
		// middle connections
		p1, p13, p5, p1, p9, p13,
		p2, p14, p6, p2, p10, p14,
		p3, p15, p7, p3, p11, p15,
		p4, p16, p8, p4, p12, p16,
		// top connections
		p5, p14, p13, p5, p6, p14,
		p6, p15, p14, p6, p7, p15,
		p7, p16, p15, p7, p8, p16,
		p8, p13, p16, p8, p5, p13,

		// second cube
		p9, p12, p11, p9, p11, p10, // bottom
		p13, p15, p16, p13, p14, p15, // top
		p9, p13, p16, p9, p16, p12, // left
		p10, p15, p14, p10, p11, p15, // right
		p12, p16, p15, p12, p15, p11, // back
		p9, p14, p13, p9, p10, p14  // front
	);
	float angle = radians(50.) * time;
	mat4 rotation = rXY(angle) * rYZ(angle) * rZX(angle) * rXW(angle) * rYW(angle) * rZW(angle);
	int n = 48;
	int index = -1;
	float t = 1e6; // far far away
	mat4 projection = calc_matrix();
	float opacity = .3;
	vec3 color = color_cube;
	for(int i = 0; i < n; i++) {
		vec4 v1 = projection * ((rotation * triangles[3 * i]) - from);
		vec4 v2 = projection * ((rotation * triangles[3 * i + 1]) - from);
		vec4 v3 = projection * ((rotation * triangles[3 * i + 2]) - from);
		float t2 = intersect(view_position, dir, v1.xyz/v1.w, v2.xyz/v2.w, v3.xyz/v3.w);
		/*
		if(t2 > 0. && t2 < t) {
			index = i;
			t = t2;
		}
		*/
		if(t2 > 0.) {
			if(t2 < t) {
				final_color = mix(final_color, color, opacity);
			} else {
				final_color = mix(color, final_color, opacity);
			}
		}
	}
	if(index >= 0) {
		int i = index;
		//vec3 v1 = /*rotation */ triangles[3 * i];
		//vec3 v2 = /*rotation */ triangles[3 * i + 1];
		//vec3 v3 = /*rotation */ triangles[3 * i + 2];
		//vec3 normal = normalize(cross(v2 - v1, v3 - v1));
		vec3 light = normalize(vec3(2, 3, 10) - view_position);
		//final_color = .1 + vec3(.5) * lambert(normal, light);
		//final_color += phong(normal, light, -dir, 32.);
		//final_color = vec3(1);
	}

	out_color.rgb = final_color;
	out_color.a = 0.;
}

// möller trumbore
// so mit determinanten und stuff
float intersect(vec3 origin, vec3 dir, vec3 v1, vec3 v2, vec3 v3) {
	const float epilepsilon = 1e-6;
	vec3 e1 = v2 - v1;
	vec3 e2 = v3 - v1;
	vec3 P = cross(dir, e2);
	float det = dot(e1, P);
	if(abs(det) < epilepsilon) return -1;
	float inv_det = 1./det;
	vec3 T = origin - v1;
	float u = dot(T, P) * inv_det;
	if(u < 0. || u > 1.) return -1;
	vec3 Q = cross(T, e1);
	float v = dot(dir, Q) * inv_det;
	if(v < 0. || u + v > 1.) return -1;
	float t = dot(e2, Q) * inv_det;
	return mix(-1, t, t > epilepsilon);
}

mat4 calc_matrix() {
	vec4 wa, wb, wc, wd;
	wd = normalize(to - from);
	wa = normalize(cross4(up, over, wd));
	wb = normalize(cross4(over, wd, wa));
	wc = cross4(wd, wa, wb);
	return mat4(wa, wb, wc, wd);
}

vec4 cross4(vec4 a, vec4 b, vec4 c) {
	return vec4(
		dot(a.yzw, cross(b.yzw, c.yzw)),
		-dot(a.xzw, cross(b.xzw, c.xzw)),
		dot(a.xyw, cross(b.xyw, c.xyw)),
		-dot(a.xyz, cross(b.xyz, c.xyz))
	);
}

mat4 rXY(float theta) {
	return mat4(
		cos(theta), sin(theta), 0, 0,
		-sin(theta), cos(theta), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}

mat4 rYZ(float theta) {
	return mat4(
		1, 0, 0, 0,
		0, cos(theta), sin(theta), 0,
		0, -sin(theta), cos(theta), 0,
		0, 0, 0, 1
	);
}

mat4 rZX(float theta) {
	return mat4(
		cos(theta), 0, -sin(theta), 0,
		0, 1, 0, 0,
		sin(theta), 0, cos(theta), 0,
		0, 0, 0, 1
	);
}

mat4 rXW(float theta) {
	return mat4(
		cos(theta), 0, 0, sin(theta),
		0, 1, 0, 0,
		0, 0, 1, 0,
		-sin(theta), 0, 0, cos(theta)
	);
}

mat4 rYW(float theta) {
	return mat4(
		1, 0, 0, 0,
		0, cos(theta), 0, -sin(theta),
		0, 0, 1, 0,
		0, sin(theta), 0, cos(theta)
	);
}

mat4 rZW(float theta) {
	return mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, cos(theta), -sin(theta),
		0, 0, sin(theta), cos(theta)
	);
}
