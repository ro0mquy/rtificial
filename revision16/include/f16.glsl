float id_f16 = 1;
float id_f16_flame = 1000;

MatWrap wF16Ground(vec2 p, float len, float exhaust, float wheel_rotation) {
	pFlip(p.x);
	// horizontal offset
	pTrans(p.x, .0758 * len);

	vec2 mid_dim = vec2(.4775, .0921) * len;
	float f_mid = f2Box(p, mid_dim);

	vec2 p_engine = p;
	pTrans(p_engine.x, mid_dim.x);
	pMirror(p_engine.y);
	float engine_end_height = .0507 * len;
	float engine_width = .1606 * len;
	vec2 a = vec2(0, mid_dim.y);
	vec2 b = vec2(2 * engine_width, engine_end_height);
	float engine_radius = distance(a, b) * f16_ground_engine_radius_rt_float;
	vec2 p_engine_sphere = p_engine;
	pTrans(p_engine_sphere, centerOfCircle(b, a, engine_radius));
	float f_engine = f2Sphere(p_engine_sphere, engine_radius);
	pTrans(p_engine.x, engine_width);
	float engine_fix = .05;
	vec2 p_exhaust = p_engine;
	pTrans(p_engine.x, -engine_fix);
	pMirror(p_engine.x);
	f_engine = max(f_engine, p_engine.x - (engine_width + engine_fix));

	vec2 p_wheel_connection = p;
	float p_wheel_connection_top_y = p_wheel_connection.y;
	pTrans(p_wheel_connection.x, -0.1262 * len);
	float back_wheel = pMirrorTrans(p_wheel_connection.x, .2382 * len);
	pRot(p_wheel_connection, -Tau / 4. * (f16_ground_wheel_rotation_rt_float + wheel_rotation));
	vec2 wheel_connection_dim = vec2(.0076, .0589) * len;
	pTrans(p_wheel_connection.y, -(mid_dim.y + wheel_connection_dim.y));
	float f_wheel_connection = f2Box(p_wheel_connection, wheel_connection_dim);
	vec2 p_wheels = p_wheel_connection;
	pRot(p_wheel_connection, radians(60));
	float f_wheel_connection2 = abs(p_wheel_connection.x) - wheel_connection_dim.x;
	f_wheel_connection2 = max(max(p_wheels.x, p_wheel_connection_top_y), f_wheel_connection2);
	f_wheel_connection = min(f_wheel_connection, f_wheel_connection2);
	pTrans(p_wheels, vec2(wheel_connection_dim.x, -wheel_connection_dim.y));
	float front_wheel_radius = .0296 * len;
	float wheel_radius = (back_wheel > 0.) ? .0446 * len : front_wheel_radius;
	pTrans(p_wheels.y, wheel_radius - front_wheel_radius);
	float f_wheels = f2Sphere(p_wheels, wheel_radius);
	f_wheels = min(f_wheels, f_wheel_connection);

	vec2 p_nose = p;
	pTrans(p_nose.x, -(.556 * len + mid_dim.x));
	pRot(p_nose, Tau * f16_ground_nose_rotation_rt_float);
	vec2 p_nose_sphere = p_nose;
	pMirror(p_nose_sphere.y);
	vec2 nose_dim = vec2(2 * .3845, .0635) * len;
	vec2 nose_b = vec2(nose_dim);
	float nose_radius = length(nose_b) * f16_ground_nose_radius_rt_float;
	pTrans(p_nose_sphere, centerOfCircle(nose_b, vec2(0), nose_radius));
	float f_nose = f2Sphere(p_nose_sphere, nose_radius);
	f_nose = max(f_nose, p_nose.x - nose_dim.x);

	vec2 p_cockpit = p_nose;
	pTrans(p_cockpit.x, f16_ground_cockpit_offset_rt_float * len);
	float cockpit_radius = f16_ground_cockpit_radius_rt_float * len;
	pTrans(p_cockpit.y, f16_ground_cockpit_height_rt_float * len + nose_dim.y - cockpit_radius);
	float f_cockpit = f2Sphere(p_cockpit, cockpit_radius);
	f_cockpit = abs(f_cockpit) - f16_ground_cockpit_thickness_rt_float * len;
	f_cockpit = max(f_cockpit, -p.y);
	f_nose = min(f_nose, f_cockpit);

	vec2 p_fin = p;
	pTrans(p_fin.x, .06 * len);
	pTrans(p_fin.y, mid_dim.y);
	vec2 p_fin_left = p_fin;
	vec2 fin_n1 = unitVector(radians(104.5));
	float f_fin = f2Plane(p_fin_left, fin_n1);
	pTrans(p_fin_left, 2 * .155 * len * vec2(fin_n1.y, -fin_n1.x));
	f_fin = min(f_fin, f2Plane(p_fin_left, unitVector(radians(132.3))));
	vec2 p_fin_right = p_fin;
	pTrans(p_fin_right.x, .61 * len);
	float f_fin_right = p_fin_right.x;
	pTrans(p_fin_right.y, .066 * len);
	f_fin_right = min(f_fin_right, f2Plane(p_fin_right, unitVector(radians(270 + 65))));
	//f_fin = max(f_fin, abs(p_fin.y - .17 * len) - .17 * len);
	f_fin = max(f_fin, abs(p_fin.y - .17 * len) - .175 * len);
	f_fin = max(f_fin, f_fin_right);


	f_mid = opUnionRounded(f_mid, f_nose, f16_ground_nose_smooth_rt_float * nose_dim.y);
	float f = min(f_mid, f_engine);
	f = min(f, f_wheels);
	f = min(f, f_fin);

	MatWrap w = MatWrap(f, newMaterialId(id_f16, vec3(p, 0)));

	if (exhaust > 0.1) {
		float exhaust_dist = f16_exhaust_dist_rt_float * len * .01;
		float exhaust_width = f16_exhaust_width_rt_float * len * .01 * exhaust;
		float exhaust_height = f16_exhaust_height_rt_float * len * .01;
		pTrans(p_exhaust.x, engine_width + exhaust_dist + exhaust_width);
		float f_exhaust = f2ConeCapped(p_exhaust.yx, exhaust_height, 0, exhaust_width);

		MatWrap w_flame = MatWrap(f_exhaust, newMaterialId(id_f16_flame, vec3(p, 0)));
		w = mUnion(w, w_flame);
	}

	return w;
}

MatWrap wF16Ground(vec2 p, float len) {
	return wF16Ground(p, len, 0, 1);
}

MatWrap wF16Air(vec2 p, float len) {
	return wF16Ground(p, len, 1, 1);
}

void pF16Landscape(inout vec2 p_f16, float t) {
	float duration = 512;
	float speedup_duration = 32;
	float zoomout_duration = 64;
	float speedup_start = duration - speedup_duration;
	float zoomout_t = smoothstep(0, zoomout_duration, t);
	pTrans(p_f16, land_f16_offset_rt_vec2 * zoomout_t);
	float speedup_percentage = .4;
	vec2 motion = saturate(t / (duration - speedup_duration)) * land_f16_motion_rt_vec2 * (1 - speedup_percentage);
	pTrans(p_f16, motion * zoomout_t);
	float speedup_t = pow(saturate((t - (duration - speedup_duration)) / (speedup_duration)), 1.5);
	pTrans(p_f16, speedup_t * land_f16_motion_rt_vec2 * speedup_percentage);
	pTrans(p_f16.x, clamp(t - duration, 0, 64) * .3);

	// noise moving
	float noise_t = smoothstep(zoomout_duration, speedup_start, t);
	vec2 noise_displacement = vec2(
			sin(cos(0.07*t) + 1.),
			cos(0.014*t + 3.)
			);
	noise_displacement *= min(1., noise_t * 10.);
	pTrans(p_f16, noise_displacement);
}

void pF16Bridge(inout vec2 p_f16, float t) {
	pTrans(p_f16.x, t * .3);
	pF16Landscape(p_f16, 576);
}

void pF16Parl(inout vec2 p_f16, float t) {
	pTrans(p_f16.x, 84 * .3);
	float speedup_start = 56;
	if (t >= speedup_start) {
		float duration = 48;
		float speedup = duration * pow(max(0, (t - speedup_start)/duration), 8.);
		pTrans(p_f16.x, speedup * .1); // speedup
	}
	pF16Bridge(p_f16, t);
}
