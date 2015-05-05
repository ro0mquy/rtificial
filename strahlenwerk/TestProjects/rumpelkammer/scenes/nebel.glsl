#include "march.glsl"
#include "noise.glsl"
#include "sky1.glsl"
#line 5 "nebel"

float fScene(vec3 p) {
	float f = Inf;

	float f_boden = p.y - 2. * (1. - smoothstep(5., 10., length(p.xz)));
	f = min(f, f_boden);

	vec3 p_steine = p;
	p_steine.y -= 2. + 1.;
	pDomrepAngle(p_steine.xz, 8., 4.);
	float f_steine = fBoxRounded(p_steine, vec3(.2, 1.1, .4), .01);
	f = min(f, f_steine);

	vec3 p_altar = p;
	p_altar.y -= 3.;
	pMirrorTrans(p_altar.y, .5);
	p_altar.y = -p_altar.y;
	float f_altar = fPyramid(p_altar, .3, Tau * .1);
	//f_altar = max(f_altar, -p_altar.y);
	f = min(f, f_altar);

	mUnion(f, Material(0., p));
	return f;
}

vec3 applyLights(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, Material material) {
	vec3 color = .05 * ambientColor(normal, -direction, vec3(.9), .1, 0.);
	return color;
}

// build your fog here, output should be in intervall [0, 1]
float fogNoise(vec3 p) {
	float eps = fog_density_falloff_rt_float;
	vec3 q = p + 348.4016;
	float noise = valueFbm(fog_density_freq_rt_float * q);
	noise = .5 + .5 * noise;
	noise = fog_density_factor_rt_float * noise + fog_density_bias_rt_float;
	noise *= 1. - smoothstep(fog_edge_top_rt_float - eps, fog_edge_top_rt_float + eps, p.y);
	noise *= smoothstep(fog_edge_bottom_rt_float - eps, fog_edge_bottom_rt_float + eps, p.y);
	//noise *= 1. - smoothstep(20., 30., length(p));
	noise = saturate(noise);
	return noise;
}

// factor how much light is scattered
// cos_phi: cosine of angle between incoming and outgoing light (eg. dot(-dir, sun_dir))
float fogScatteringPhaseFactor(float cos_phi) {
	// g tweaks how much light gets backscattered
	// -1 full backscattering, 1 full forward scattering
	float g = fog_scattering_g_rt_float;

	/* Henyey-Greenstein phase
	float g2 = g * g;
	float val = 1. - g2;
	val /= 4 * Pi * pow(1. + g2 - 2. * g * cos_phi, 1.5);
	// */

	//* Schlick phase, much cheaper, almost same as HG
	float val = 1. - g * g;
	val /= 4. * Pi * square(1. + g * cos_phi);
	// */

	return val;
}

// Exponential Integral http://en.wikipedia.org/wiki/Exponential_integral
float Ei(float z) {
	return 0.5772156649015328606065 + log( 1e-4 + abs(z) ) + z * (1.0 + z * (0.25 + z * ( (1.0/18.0) + z * ( (1.0/96.0) + z * (1.0/600.0) ) ) ) ); // for z != 0
}

// height: distance to border (pos.y - height_bottom)
// extinction_coeff: extinction_factor * density;
// multiply with color of ambient light
float fogAmbientFactor(float height, float coeff_extinction) {
	float a = -coeff_extinction * height;
	float isotropic_scattering = max(0., exp(a) - a * Ei(a));
	return isotropic_scattering;
}

// mainly from http://patapom.com/topics/Revision2013/Revision%202013%20-%20Real-time%20Volumetric%20Rendering%20Course%20Notes.pdf
vec3 applyFog(vec3 origin, float marched, vec3 direction, vec3 color) {
	// tweak performance and quality here
	float step_nums = fog_rendering_steps_rt_float;
	float step_size = fog_rendering_distance_rt_float / step_nums;

	// upper and lower edge of the fog layer
	float edge_top = fog_edge_top_rt_float;
	float edge_bottom = fog_edge_bottom_rt_float;

	// prefer scattering or extinction (0 for scattering, 1 for extinction)
	float factor_scattering = 1. - fog_scattering_extinction_bias_rt_float;
	float factor_extinction = fog_scattering_extinction_bias_rt_float;

	// color of sun- and ambient light
	vec3 pure_color_sun = fog_color_sun_rt_color;
	pure_color_sun = sun_radiance;
	vec3 pure_color_ambient_top = fog_color_ambient_top_rt_color;
	vec3 pure_color_ambient_bottom = fog_color_ambient_bottom_rt_color;

	// direction of sun rays and ambient light (eg. vec3(0, 1, 0) for bottom)
	vec3 dir_sun = normalize(fog_dir_sun_rt_vec3);
	dir_sun = sun_dir;
	vec3 dir_ambient_top = normalize(fog_dir_ambient_top_rt_vec3);
	vec3 dir_ambient_bottom = normalize(fog_dir_ambient_bottom_rt_vec3);

	// how much light is scattered
	float phase_sun = fogScatteringPhaseFactor(dot(-direction, dir_sun));
	float phase_ambient_top = fogScatteringPhaseFactor(dot(-direction, dir_ambient_top));
	float phase_ambient_bottom = fogScatteringPhaseFactor(dot(-direction, dir_ambient_bottom));

	// we're going to change this values
	float total_dist = step_size;
	float total_extinction = 1.;
	vec3 total_scattering = vec3(0.);

	for (int i = 0; i < step_nums; i++) {
		vec3 pos = origin + total_dist * direction;;

		// current fog density
		float density = fogNoise(pos);
		float coeff_scattering = factor_scattering * density;
		float coeff_extinction = factor_extinction * density;

		// color of direct sunlight
		vec3 color_sun = pure_color_sun;
		color_sun *= phase_sun;

		// ambient color from the sky
		vec3 color_ambient_top = pure_color_ambient_top;
		color_ambient_top *= fogAmbientFactor(edge_top - pos.y, coeff_extinction);
		color_ambient_top *= phase_ambient_top;

		// ambient color from the ground
		vec3 color_ambient_bottom = pure_color_ambient_bottom;
		color_ambient_bottom *= fogAmbientFactor(pos.y - edge_bottom, coeff_extinction);
		color_ambient_bottom *= phase_ambient_bottom;

		// all colors together
		vec3 color_combined = color_sun + color_ambient_top + color_ambient_bottom;

		// total scattering for this step
		vec3 current_scattering = coeff_scattering * step_size * color_combined;

		if (total_dist > marched) {
			// hit object, do only partial fogging
			float t = (total_dist - marched) / step_size;
			total_extinction *= mix(exp(-coeff_extinction * step_size), 1., t);
			total_scattering += mix(total_extinction * current_scattering, vec3(0.), t);
			break;
		}

		total_extinction *= exp(-coeff_extinction * step_size);
		total_scattering += total_extinction * current_scattering;

		total_dist += step_size;
	}

	// blending with premultiplied alpha
	color = total_scattering + total_extinction * color;

	return color;
}

vec3 applyAfterEffects(vec3 origin, float marched, vec3 direction, vec3 color) {
	return applyFog(origin, marched, direction, color);
}
