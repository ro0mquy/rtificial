#include "fragment.glsl"
#include "camera.glsl"
#include "material.glsl"
#include "sdf/domain.glsl"
#include "sdf/operators.glsl"
#include "sdf/distances.glsl"
#include "lighting.glsl"
#include "helper.glsl"
#include "live_background.glsl"
#line 11

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


////// beleuchtungskram.glsl


Material getMaterial(MaterialId materialId);
vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, MaterialId materialId, Material material);
vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color);

uniform float main_marching_distance;


void main() {
	vec3 origin = camera_position;
	vec3 direction = camGetDirection();
	float marched = sdfMarch(origin, direction, main_marching_distance);

	if (isinf(marched)) {
		out_color = live_background(direction);//environmentColor(origin, direction, main_marching_distance);
		out_depth = main_marching_distance;
	} else {
		vec3 hit = origin + marched * direction;

		float marching_error = fMain(hit, true);
		MaterialId materialId = current_material;

		vec3 normal = sdfNormalForeward(hit, direction);

				Material material = getMaterial(materialId);
				// TODO move this somewhere else
				// should be done for debug materials too to avoid false differentials
				//perturbNormal(hit, normal, material.height);
				material.roughness = square(material.roughness);
				out_color = applyLights(origin, marched, direction, hit, normal, materialId, material);
		out_depth = marched;
	}
	out_color = applyAfterEffects(origin, marched, direction, out_color);
	out_color *= exp2(camera_exposure_rt_float);
}
