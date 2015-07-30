#include "fragment.glsl"
#include "camera.glsl"
#include "material.glsl"
#include "sdf/domain.glsl"
#include "sdf/operators.glsl"
#include "sdf/distances.glsl"
#include "lighting.glsl"
#include "helper.glsl"
#line 11 "march"

/// marchingloopkram.glsl

uniform float main_normal_epsilon_bias;

uniform float debug_mode;
uniform vec3 debug_plane_normal;
uniform float debug_plane_height;

const float debug_plane_material_id = 423.;

bool debug_default_pass_scene_visible = true;
bool debug_default_pass_plane_visible = false;
bool debug_isoline_pass_scene_visible = false;
bool debug_isoline_pass_plane_visible = false;
bool debug_gradient_visualization = false;
bool debug_gradient_pass_scene_visible = false;
bool debug_gradient_pass_plane_visible = false;

bool scene_visible = debug_default_pass_scene_visible;
bool debug_plane_visible = debug_default_pass_plane_visible;

float fScene(vec3 p);

float fDebugPlane(vec3 p) {
	return abs(fPlane(p, normalize(debug_plane_normal)) - debug_plane_height);
}

float fMain(vec3 p, bool calc_m) {
	current_dist = Inf;
	calculate_material = calc_m;

	if (debug_plane_visible) {
		mUnion(fDebugPlane(p), newMaterialId(debug_plane_material_id, p));
	}

	if (scene_visible) {
		fScene(p);
	}

	return current_dist;
}

vec3 sdfNormalRaw(vec3 p, float e) {
	// writing the gradient this way, causes the compiler not to inline f six times
	// thanks to mercury, stupid compilers
	vec3 s[6] = vec3[6](vec3(e,0,0), vec3(0,e,0), vec3(0,0,e), vec3(-e,0,0), vec3(0,-e,0), vec3(0,0,-e));
	float d[6] = float[6](0,0,0,0,0,0);
	for(int i = 0; i < 6; i++) {
		d[i] = fMain(p+s[i], false);
	}
	return vec3(d[0]-d[3], d[1]-d[4], d[2]-d[5]);
}

vec3 sdfNormal(vec3 p, float epsilon) {
	return normalize(sdfNormalRaw(p, epsilon));
}

vec3 sdfNormal(vec3 p) {
	float epsilon = max(.00005, camGetPixelSize(distance(p, camera_position)));
	epsilon *= main_normal_epsilon_bias;
	return sdfNormal(p, epsilon);
}

vec3 sdfNormalForeward(vec3 p, vec3 direction) {
	vec3 normal = sdfNormal(p);
	// try eliminating backfacing normals
	if (dot(direction, normal) > 0.) {
		normal = normalize(dFdx(normal) + normal);
		if (dot(direction, normal) > 0.) {
			normal = normalize(dFdy(normal) + normal);
		}
	}
	return normal;
}

vec3 sdfGradient(vec3 p, float epsilon) {
	return sdfNormalRaw(p, epsilon) / (2. * epsilon);
}

vec3 sdfGradient(vec3 p) {
	float epsilon = max(.00005, camGetPixelSize(distance(p, camera_position)));
	epsilon *= main_normal_epsilon_bias;
	return sdfGradient(p, epsilon);
}

// ein fachmenschich kopierter marchingloop
float sdfMarchAdvanced(vec3 o, vec3 d, float t_min, float t_max, float pixelRadius, int max_iterations, float omega, bool forceHit) {
	// o, d : ray origin, direction (normalized)
	// t_min, t_max: minimum, maximum t values
	// pixelRadius: radius of a pixel at t = 1
	// forceHit: boolean enforcing to use the
	//           candidate_t value as result
	float t = t_min;
	float candidate_error = Real_Inf;
	float candidate_t = t_min;
	float previousRadius = 0;
	float stepLength = 0;
	float functionSign = sgn(fMain(o, false));

	for (int i = 0; i < max_iterations; ++i) {
		float signedRadius = functionSign * fMain(d*t + o, false);
		float radius = abs(signedRadius);

		bool sorFail = omega > 1 && (radius + previousRadius) < stepLength;
		if (sorFail) {
			stepLength -= omega * stepLength;
			omega = 1;
		} else {
			stepLength = signedRadius * omega;
		}

		previousRadius = radius;
		float error = radius / t;
		if (!sorFail && error < candidate_error) {
			candidate_t = t;
			candidate_error = error;
		}

		if (!sorFail && error < pixelRadius || t > t_max) {
			break;
		}

		t += stepLength;
	}

	if ((t > t_max || candidate_error > pixelRadius) && !forceHit) {
		return Real_Inf;
	}

	return candidate_t;
}

float sdfMarch(vec3 o, vec3 d, float t_max) {
	float marched = sdfMarchAdvanced(o, d, .001, t_max, camGetPixelSize(1), 256, 1.2, false);

	if (isinf(marched)) {
		return marched;
	}

	// discontinuity reduction
	for (int i = 0; i < 3; i++) {
		marched += fMain(o + marched * d, false) - camGetPixelSize(marched);
	}

	return marched;
}

void setDebugParameters() {
	int mode = int(debug_mode);
	switch (mode) {
		case 0: // default
			debug_default_pass_scene_visible = true;
			debug_default_pass_plane_visible = false;
			debug_isoline_pass_scene_visible = false;
			debug_isoline_pass_plane_visible = false;
			debug_gradient_visualization = false;
			debug_gradient_pass_scene_visible = false;
			debug_gradient_pass_plane_visible = false;
			break;
		case 1: // debug plane
			debug_default_pass_scene_visible = true;
			debug_default_pass_plane_visible = true;
			debug_isoline_pass_scene_visible = true;
			debug_isoline_pass_plane_visible = false;
			debug_gradient_visualization = false;
			debug_gradient_pass_scene_visible = false;
			debug_gradient_pass_plane_visible = false;
			break;
		case 2: // debug plane without scene geometry
			debug_default_pass_scene_visible = false;
			debug_default_pass_plane_visible = true;
			debug_isoline_pass_scene_visible = true;
			debug_isoline_pass_plane_visible = false;
			debug_gradient_visualization = false;
			debug_gradient_pass_scene_visible = false;
			debug_gradient_pass_plane_visible = false;
			break;
		case 3: // visualize gradient length
			debug_default_pass_scene_visible = true;
			debug_default_pass_plane_visible = false;
			debug_isoline_pass_scene_visible = false;
			debug_isoline_pass_plane_visible = false;
			debug_gradient_visualization = true;
			debug_gradient_pass_scene_visible = true;
			debug_gradient_pass_plane_visible = false;
			break;
		case 4: // visualize gradient length with debug plane
			debug_default_pass_scene_visible = true;
			debug_default_pass_plane_visible = true;
			debug_isoline_pass_scene_visible = true;
			debug_isoline_pass_plane_visible = false;
			debug_gradient_visualization = true;
			debug_gradient_pass_scene_visible = true;
			debug_gradient_pass_plane_visible = false;
			break;
		case 5: // visualize gradient length with debug plane and without geometry
			debug_default_pass_scene_visible = false;
			debug_default_pass_plane_visible = true;
			debug_isoline_pass_scene_visible = true;
			debug_isoline_pass_plane_visible = false;
			debug_gradient_visualization = true;
			debug_gradient_pass_scene_visible = true;
			debug_gradient_pass_plane_visible = false;
			break;
		default: // same as default
			debug_default_pass_scene_visible = true;
			debug_default_pass_plane_visible = false;
			debug_isoline_pass_scene_visible = false;
			debug_isoline_pass_plane_visible = false;
			debug_gradient_visualization = false;
			debug_gradient_pass_scene_visible = false;
			debug_gradient_pass_plane_visible = false;
			break;
	}

	scene_visible = debug_default_pass_scene_visible;
	debug_plane_visible = debug_default_pass_plane_visible;
}



////// beleuchtungskram.glsl

vec3 debugIsolineTexture(float sdf_dist, vec3 camera_pos, float camera_dist) {
	float small_lines = abs(sin(Pi * 10. * sdf_dist));
	small_lines = 1. - (1. - smoothstep(8., 15., camera_dist)) * (1. - small_lines);

	float medium_lines = abs(sin(Pi * 1. * sdf_dist));
	medium_lines = 1. - (.8 + .2 * smoothstep(6., 10., camera_dist)) * (1. - smoothstep(60., 80., camera_dist)) * (1. - medium_lines);

	float big_lines = abs(sin(Pi * 1./10. * sdf_dist));
	big_lines = 1. - (.8 + .2 * smoothstep(30., 50., camera_dist)) * (1. - smoothstep(80., 150., camera_dist)) * (1. - big_lines);

	float height = fDebugPlane(camera_pos);

	vec3 lines_color = vec3(0.);
	vec3 near_color = vec3(0.29804, 0.18824, 0.43922);
	vec3 far_color = vec3(0.12549, 0.52941, 0.36078);
	vec3 inner_color = vec3(0.02095, 0.19046, 0.60548);

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

vec3 debugIsolineTextureFiltered(vec3 p, vec3 camera_pos, float camera_dist) {
	scene_visible = debug_isoline_pass_scene_visible;
	debug_plane_visible = debug_isoline_pass_plane_visible;

	float sdf_dist = fMain(p, false);
	vec3 sdf_normal = sdfNormal(p);

	scene_visible = debug_default_pass_scene_visible;
	debug_plane_visible = debug_default_pass_plane_visible;

	vec3 pX = dFdx(p);
	vec3 pY = dFdy(p);

	/*
	float detail = 100.;
	int MaxSamples = 10;
	int sx = 1 + clamp( int( detail*length(pX) ), 0, MaxSamples-1 );
	int sy = 1 + clamp( int( detail*length(pY) ), 0, MaxSamples-1 );
	// */

	//*
	// fuck it - just supersample everything!
	int sx = 5;
	int sy = 5;
	// */

	vec3 no = vec3(0);
	for(int j = 0; j < sy; j++ ) {
		for(int i = 0; i < sx; i++ ) {
			vec2 st = (vec2(i, j) + .5)/vec2(sx, sy) - .5;
			vec3 delta = st.x * pX + st.y * pY;
			float f_dist = sdf_dist + dot(sdf_normal, delta);
			no += debugIsolineTexture(f_dist, camera_pos, camera_dist);
		}
	}

	return no / float(sx*sy);
}

vec3 debugColorIsolines(vec3 origin, float marched, vec3 hit) {
	return debugIsolineTextureFiltered(hit, origin, marched);
}

vec3 debugColorGradient(vec3 p) {
	scene_visible = debug_gradient_pass_scene_visible;
	debug_plane_visible = debug_gradient_pass_plane_visible;

	vec3 gradient = sdfGradient(p);
	float len_grad = length(gradient);

	scene_visible = debug_default_pass_scene_visible;
	debug_plane_visible = debug_default_pass_plane_visible;

	vec3 under_color = vec3(0.18014, 0.74453, 0.03288);
	vec3 over_color = vec3(0.71547, 0.03995, 0.02537);

	vec3 base_color = vec3(1.);
	base_color = mix(base_color, under_color, 1. - smoothstep(.8, 1., len_grad));
	base_color = mix(base_color, over_color, smoothstep(1., 1.2, len_grad));

	return base_color;
}

Material getMaterial(MaterialId materialId);
vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material);
vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color);

uniform float main_marching_distance;

// Bump Mapping Unparametrized Surfaces on the GPU [Mikkelsen2010]
void perturbNormal(vec3 p, inout vec3 n, float height) {
	vec3 sigma_s = dFdx(p);
	vec3 sigma_t = dFdy(p);
	vec3 a = cross(sigma_t, n);
	vec3 b = cross(n, sigma_s);
	float determinant = dot(sigma_s, a);
	float d_beta_s = dFdx(height);
	float d_beta_t = dFdy(height);
	vec3 surface_gradient = (d_beta_s * a + d_beta_t * b) * sign(determinant);
	vec3 n_perturbed = normalize(abs(determinant) * n - surface_gradient);
	n = n_perturbed;
}

void main() {
	setDebugParameters();

	vec3 origin = camera_position;
	vec3 direction = camGetDirection();
	float marched = sdfMarch(origin, direction, main_marching_distance);

	if (isinf(marched)) {
		out_color = .07 * environmentColor(origin, direction, main_marching_distance);
		out_depth = main_marching_distance;
	} else {
		vec3 hit = origin + marched * direction;

		float marching_error = fMain(hit, true);
		MaterialId materialId = current_material;

		vec3 normal = sdfNormalForeward(hit, direction);

		if (materialId.id == debug_plane_material_id) {
			vec3 c_isoline = debugColorIsolines(origin, marched, hit);
			if (debug_gradient_visualization) {
				vec3 c_gradient = debugColorGradient(hit);
				c_isoline = mix(c_isoline, c_gradient, .5);
			}
			Material m_isoline = defaultMaterial(c_isoline);
			m_isoline.roughness = 1.;
			out_color = .1 * ambientColor(normal, -direction, m_isoline);
		} else {
			if (debug_gradient_visualization) {
				vec3 c_gradient = debugColorGradient(hit);
				Material m_gradient = defaultMaterial(c_gradient);
				m_gradient.roughness = 0.;
				out_color = .1 * ambientColor(normal, -direction, m_gradient);
			} else {
				Material material = getMaterial(materialId);
				// TODO move this somewhere else
				// should be done for debug materials too to avoid false differentials
				//perturbNormal(hit, normal, material.height);
				material.roughness = square(material.roughness);
				out_color = applyLights(origin, marched, direction, hit, normal, materialId, material);
			}
		}
		out_depth = marched;
	}
	out_color = applyAfterEffects(origin, marched, direction, out_color);
}
