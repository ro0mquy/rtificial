vec3 layerLight(vec3 origin, float marched, vec3 direction, vec3 hit, vec3 normal, Material material) {
	vec3 position = lay_light_position_rt_vec3;
	position.xy += camera_position.xy;
	SphereLight light1 = SphereLight(
		position,
		lay_light_radius_rt_float,
		lay_light_color_rt_color,
		exp2(lay_light_intensity_rt_float)
	);
	return applySphereLight(origin, marched, direction, hit, normal, material, light1);
}
