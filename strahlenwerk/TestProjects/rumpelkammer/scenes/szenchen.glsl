#include "march.glsl"
#line 3 "szenchen"

void main() {
	setDebugParameters();

	vec3 o = camera_position;
	vec3 d;
	float screen_dist = camGetDirection(d);
	float t = sdfMarch(o, d, 200., screen_dist);

	if (isinf(t)) {
		out_color.rgb = vec3(0.);
	} else {
		vec3 hit = o + t * d;
		float dist = fMain(hit, true);
		Material material = current_material;
		vec3 normal = sdfNormal(hit);

		if (material.id == debug_plane_material_id) {
			out_color = debugColorIsolines(hit, o, t);
		} else {
			if (debug_gradient_visualization) {
				out_color = debugColorGradient(hit);
			} else {
				out_color = .5 * normal + .5;
			}
		}

		out_color *= .95 * pdot(normal, normalize(vec3(1., 2., .5))) + .05;

	}
}

float fScene(vec3 p) {
	pMirrorAtPlane(p, normalize(-vec3(1., 2., 3.)), 5.);
	float f = fBoxRounded(p, vec3(2.), 1.);
	f = min(f, fBox(p - vec3(1., 1., 0.), 1.));
	f = smin(f, fBox(p - vec3(1., 1., 1.), 1.5), 1.);
	f = min(f, fSupershape2(p.xz +vec2(5.), 1., 1., 6., 1., 1., 1.));
	mUnion(f, Material(0., p));
	return f;
}
