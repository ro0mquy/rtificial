vec2 f(vec3);

uniform vec3 color_foo1;
uniform vec3 color_foo2;
uniform float foo1;
uniform float foo2;

float intersect(vec3 origin, vec3 dir, vec3 v1, vec3 v2, vec3 v3);

void main(void){
	vec3 dir = get_direction();
	vec3 final_color = vec3(0);

	const vec3 p1 = vec3(-1, -1,  1);
	const vec3 p2 = vec3( 1, -1,  1);
	const vec3 p3 = vec3( 1, -1, -1);
	const vec3 p4 = vec3(-1, -1, -1);
	const vec3 p5 = vec3(-1,  1,  1);
	const vec3 p6 = vec3( 1,  1,  1);
	const vec3 p7 = vec3( 1,  1, -1);
	const vec3 p8 = vec3(-1,  1, -1);

	const vec3 triangles[] = vec3[](
		p1, p4, p3, p1, p3, p2, // bottom
		p5, p7, p8, p5, p6, p7, // top
		p1, p5, p8, p1, p8, p4, // left
		p2, p7, p6, p2, p3, p7, // right
		p4, p8, p7, p4, p7, p3, // back
		p1, p6, p5, p1, p2, p6  // front
	);
	mat3 rotation = rY(radians(40.) * time) * rX(radians(30.) * time);
	int n = 12;
	int index = -1;
	float t = 1e6; // far far away
	for(int i = 0; i < n; i++) {
		vec3 v1 = rotation * triangles[3 * i];
		vec3 v2 = rotation * triangles[3 * i + 1];
		vec3 v3 = rotation * triangles[3 * i + 2];
		float t2 = intersect(view_position, dir, v1, v2, v3);
		if(t2 > 0. && t2 < t) {
			index = i;
			t = t2;
		}
	}
	if(index >= 0) {
		int i = index;
		vec3 v1 = rotation * triangles[3 * i];
		vec3 v2 = rotation * triangles[3 * i + 1];
		vec3 v3 = rotation * triangles[3 * i + 2];
		vec3 normal = normalize(cross(v2 - v1, v3 - v1));
		vec3 light = normalize(vec3(2, 3, 10) - view_position);
		final_color = .1 + vec3(.5) * lambert(normal, light);
		final_color += phong(normal, light, -dir, 32.);
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
