#include "rtificial.glsl"
#include "march.glsl"
#line 4

out vec4 out_color;

float sdf(vec3);

vec3 sampleDebugTexture(vec3 p) {
	float dist = f(p, true);
	float isolines = abs(sin(30. * dist));
	return exp(-.1 * dist) * isolines * kk_color_rt_color;//vec3(.9, 1., .9);
}

vec3 sampleDebugTextureFiltered(vec3 p, vec3 pX, vec3 pY) {
	//float detail = 100.;
	//int MaxSamples = 10;
	//int sx = 1 + clamp( int( detail*length(pX) ), 0, MaxSamples-1 );
	//int sy = 1 + clamp( int( detail*length(pY) ), 0, MaxSamples-1 );
	// fuck it - just supersample everything!
	int sx = 5;
	int sy = 5;

	vec3 no = vec3(0);
	for(int j = 0; j < sy; j++ ) {
		for(int i = 0; i < sx; i++ ) {
			vec2 st = (vec2(i, j) + .5)/vec2(sx, sy) - .5;
			no += sampleDebugTexture(p + st.x * pX + st.y * pY);
		}
	}
	return no / float(sx*sy);
}

void main() {
	vec3 o = camera_position;
	float screenDist;
	vec3 d = get_direction(screenDist);
	float t = march(o, d, 200., screenDist);

	if (isinf(t)) {
		out_color.rgb = vec3(0.);
	} else {
		vec3 hit = o + t * d;
		vec3 normal = calc_normal(hit, false);
		float material = f(hit, false)[1];

		out_color.rgb = vec3(max(dot(normal, normalize(vec3(1., .5, 0.))), 0.) + .1);
		out_color.rgb = .5 * normal + .5;

		if (material == 42.) {
			vec3 p = hit;
			vec3 pX = dFdx(p);
			vec3 pY = dFdy(p);
			out_color.rgb = sampleDebugTextureFiltered(p, pX, pY);
		}
	}
}

vec2 f(vec3 p, bool last_step) {
	vec2 m_f = vec2(sdf(p), 0.);

	if (!last_step) {
		float f_debug = p.y - debug_height_rt_float;
		vec2 m_debug = vec2(f_debug, 42.);
		m_f = min_material(m_f, m_debug);
	}

	return m_f;
}

float sdf(vec3 p) {
	float f = fTorusSphereBox(p.xzy, 1., .5);
	return f;
}
