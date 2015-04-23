#include "rtificial.glsl"
#include "march.glsl"
#line 4 "szenchen"

out vec4 out_color;

float sdf(vec3);

vec3 sampleDebugTexture(vec3 p, vec3 camera_pos, float camera_dist) {
	float sdf_dist = f(p, true);

	float small_lines = abs(sin(Pi * 10. * sdf_dist));
	small_lines = 1. - (1. - smoothstep(8., 15., camera_dist)) * (1. - small_lines);

	float medium_lines = abs(sin(Pi * 1. * sdf_dist));
	medium_lines = 1. - (.8 + .2 * smoothstep(6., 10., camera_dist)) * (1. - smoothstep(60., 80., camera_dist)) * (1. - medium_lines);

	float big_lines = abs(sin(Pi * 1./10. * sdf_dist));
	big_lines = 1. - (.8 + .2 * smoothstep(30., 50., camera_dist)) * (1. - smoothstep(80., 150., camera_dist)) * (1. - big_lines);

	float height = fDebugPlane(camera_pos);

	vec3 lines_color = vec3(0.);
	vec3 near_color = debug_color_near_rt_color; // vec3(0.47044, 0.07593, 0.00259) // vec3(0.13035, 0.00080, 0.35865)
	vec3 far_color = debug_color_far_rt_color; // vec3(0.30663, 0.72992, 0.01794) // vec3(0.01794, 0.72992, 0.21204)
	vec3 inner_color = debug_color_inner_rt_color;

	vec3 base_color = mix(near_color, far_color, smoothstep(.1 * height, height, sdf_dist));
	if (sdf_dist < 0.) {
		base_color = inner_color;
	}
	base_color = rgb2hsv(base_color);
	base_color.y *= 1. - smoothstep(height, 10. * height, abs(sdf_dist)); // desaturate
	base_color = hsv2rgb(base_color);

	base_color = mix(lines_color, base_color, small_lines);
	base_color = mix(lines_color, base_color, medium_lines);
	base_color = mix(lines_color, base_color, big_lines);

	return base_color;
}

vec3 sampleDebugTextureFiltered(vec3 p, vec3 pX, vec3 pY, vec3 camera_pos, float camera_dist) {
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
			no += sampleDebugTexture(p + st.x * pX + st.y * pY, camera_pos, camera_dist);
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
	int mode = int(debug_mode);
	switch (mode) {
		case 0: // normal
			scene_visible = true;
			debug_plane_visible = false;
			break;
		case 1: // debug plane
			scene_visible = true;
			debug_plane_visible = !last_step;
			break;
		case 2: // debug plane without scene geometry
			scene_visible = last_step;
			debug_plane_visible = !last_step;
			break;
		default:
			scene_visible = true;
			debug_plane_visible = false;
			break;
	}

	return fMain(p);
}

float fScene(vec3 p) {
	pMirrorAtPlane(p, normalize(-vec3(1., 2., 3.)), 5.);
	float f = fBoxRounded(p, vec3(2.), 1.);
	f = min(f, fBox(p - vec3(1., 1., 0.), 1.));
	f = min(f, fBox(p - vec3(1., 1., 1.), 1.5));
	return f;
}
