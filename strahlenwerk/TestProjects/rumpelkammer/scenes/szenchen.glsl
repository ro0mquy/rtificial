#include "rtificial.glsl"
#include "march.glsl"
#line 4

out vec4 out_color;

float sdf(vec3);

vec3 sampleDebugTexture(vec3 p, vec3 o, float t) {
	float dist = f(p, true);

	float small_lines = abs(sin(Tau * 10. * dist));
	small_lines = 1. - (1. - smoothstep(5., 10., t)) * (1. - small_lines);

	float medium_lines = abs(sin(Tau * 1. * dist));
	medium_lines = 1. - (.8 + .2 * smoothstep(4., 7., t)) * (1. - smoothstep(40., 60., t)) * (1. - medium_lines);

	float big_lines = abs(sin(Tau * 1./10. * dist));
	big_lines = 1. - (.8 + .2 * smoothstep(20., 30., t)) * (1. - smoothstep(80., 150., t)) * (1. - big_lines);

	float height = o.y - debug_height_rt_float;

	vec3 lines_color = vec3(0.);
	vec3 near_color = debug_color_near_rt_color; // vec3(0.47044, 0.07593, 0.00259) // vec3(0.13035, 0.00080, 0.35865)
	vec3 far_color = debug_color_far_rt_color; // vec3(0.30663, 0.72992, 0.01794) // vec3(0.01794, 0.72992, 0.21204)

	vec3 base_color = mix(near_color, far_color, smoothstep(.1 * height, height, dist));
	base_color = rgb2hsv(base_color);
	base_color.y *= 1. - smoothstep(height, 10. * height, dist); // desaturate
	base_color = hsv2rgb(base_color);

	base_color = mix(lines_color, base_color, small_lines);
	base_color = mix(lines_color, base_color, medium_lines);
	base_color = mix(lines_color, base_color, big_lines);

	return base_color;
}

vec3 sampleDebugTextureFiltered(vec3 p, vec3 pX, vec3 pY, vec3 o, float t) {
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
			no += sampleDebugTexture(p + st.x * pX + st.y * pY, o, t);
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
			out_color.rgb = sampleDebugTextureFiltered(p, pX, pY, o, t);

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
	pMirrorAtPlane(p, normalize(-vec3(1., 2., 3.)), 5.);
	float f = fBoxRounded(p, vec3(2.), 1.);
	f = min(f, fBox(p - vec3(1., 1., 0.), 1.));
	f = min(f, fBox(p - vec3(1., 1., 1.), 1.5));
	return f;
}
