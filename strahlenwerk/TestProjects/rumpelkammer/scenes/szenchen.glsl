#include "march.glsl"
#line 3 "szenchen"

out vec4 out_color;

void main() {
	vec3 o = camera_position;
	vec3 d;
	float screen_dist = camGetDirection(d);
	switchDebugParameters(false);
	float t = sdfMarch(o, d, 200., screen_dist);

	if (isinf(t)) {
		out_color.rgb = vec3(0.);
	} else {
		vec3 hit = o + t * d;
		float dist = fMain(hit, true);
		Material material = current_material;
		vec3 normal = sdfNormal(hit);

		out_color.rgb = vec3(max(dot(normal, normalize(vec3(1., .5, 0.))), 0.) + .1);
		out_color.rgb = .5 * normal + .5;

		if (material.id == debug_plane_material_id) {
			out_color.rgb = debugColorIsolines(hit, o, t);

		}
	}
}

float fScene(vec3 p) {
	pMirrorAtPlane(p, normalize(-vec3(1., 2., 3.)), 5.);
	float f = fBoxRounded(p, vec3(2.), 1.);
	f = min(f, fBox(p - vec3(1., 1., 0.), 1.));
	f = min(f, fBox(p - vec3(1., 1., 1.), 1.5));
	mUnion(f, Material(0., p));
	return f;
}
