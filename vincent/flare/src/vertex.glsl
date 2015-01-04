#version 400

in vec2 position;
in ivec2 interface12;

out vec2 tex_position;

uniform float time;
uniform int index;

struct Element {
	float radius;
	float thickness;
	vec3 refractive_index;
};

Element elements[] = Element[](
	Element(1./0., 0., vec3(1.)),
	// 1.6652
	Element(30.81, 7.7, vec3(1.6661, 1.676, 1.67979)),
	// 1.603
	Element(-89.35, .85, vec3(1.59874, 1.61, 1.61556)),
	Element(580.38, 3.52, vec3(1.)),
	// 1.643
	Element(-80.63, 1.85, vec3(1.63956, 1.649, 1.65354)),
	Element(28.34, 4.18, vec3(1.)),
	Element(1./0., 3., vec3(1.)),
	// 1.581
	Element(1./0., 1.85, vec3(1.5775, 1.587, 1.59232)),
	// 1.694
	Element(32.19, 7.27, vec3(1.69, 1.699, 1.70334)),
	Element(-52.99, 81.857, vec3(1.))
);

mat2 trans(float d) {
	return mat2(
		1.0, 0.0,
		d, 1.0
	);
}

mat2 refr(float r, float n1, float n2) {
	return mat2(
		1., (n1 - n2) / (n2 * r),
		0., n1 / n2
	);
}

mat2 refl(float r) {
	return mat2(
		1.0, 2./r,
		0., 1.
	);
}

mat2 matT(int i) {
	return trans(elements[i].thickness);
}

mat2 matL(int i) {
	return refl(elements[i].radius);
}

mat2 matR(int i) {
	Element e0 = elements[i - 1];
	Element e1 = elements[i];
	return refr(e1.radius, e0.refractive_index[index], e1.refractive_index[index]);
}

mat2 matD(int i) {
	return matT(i) * matR(i);
}

float ray_to_pos(vec2 r) {
	return r.x * 2. / 35.;
}
uniform vec2 resolution;

void main() {
	//mat2 m_alpha = matD(5) * matD(4) * matD(3)
	//* matT(2) * inverse(matL(2)) * matT(2) * inverse(matR(3)) * matT(3) * matL(4)
	//* matD(3) * matD(2) * matD(1);
	//mat2 m_s = matD(9) * matD(8) * matD(7) * matT(6);
	int interface1 = interface12[0];
	int interface2 = interface12[1];
	//if (interface1 != 4 || interface2 != 2) {
	//	tex_position = position;
	//	gl_Position = vec4(position, 0., 1.);
	//	return;
	//}
	int aperture = 6;
	int interfaces_len = 10;

	mat2 m_alpha = mat2(1.);
	int i = 1;
	if (interface1 < aperture) {
		for (int j = 1; j < interface1; j++) {
			m_alpha = matD(j) * m_alpha;
		}
		m_alpha = matL(interface1) * m_alpha;
		for (int j = interface1 - 1; j > interface2; j--) {
			m_alpha = inverse(matR(j)) * matT(j) * m_alpha;
		}
		m_alpha = matT(interface2) * inverse(matL(interface2)) * matT(interface2) * m_alpha;
		i = interface2 + 1;
	}
	for (; i < aperture; i++) {
		m_alpha = matD(i) * m_alpha;
	}

	mat2 m_s = matT(aperture);

	i = aperture + 1;
	if (interface1 > aperture) {
		for (int j = 1; j < interface1; j++) {
			m_s = matD(j) * m_s;
		}
		m_s = matL(interface1) * m_s;
		for (int j = interface1 - 1; j > interface2; j--) {
			m_s = inverse(matR(j)) * matT(j) * m_s;
		}
		m_s = matT(interface2) * inverse(matL(interface2)) * matT(interface2) * m_s;
		i = interface2 + 1;
	}
	for (; i < interfaces_len; i++) {
		m_s = matD(i) * m_s;
	}

	vec2 rayY = vec2(position.y * 17.5, .08 * sin(time));
	vec2 rayX = vec2(position.x * 17.5, .1);
	vec2 ray_apertureY = m_alpha * rayY;
	vec2 ray_sensorY = m_s * ray_apertureY;
	vec2 ray_apertureX = m_alpha * rayX;
	vec2 ray_sensorX = m_s * ray_apertureX;

	tex_position = vec2(ray_to_pos(ray_apertureX), ray_to_pos(ray_apertureY));
	//tex_position = ray_aperture;
	vec2 out_position = vec2(ray_to_pos(ray_sensorX), ray_to_pos(ray_sensorY));
	//out_position.x *= resolution.x / resolution.y;
	gl_Position = vec4(out_position, 0.0, 1.0);
	//gl_Position = vec4(position, 0.0, 1.0);
}
