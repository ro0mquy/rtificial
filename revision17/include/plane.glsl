const float mat_id_ground = 34.;

bool traceScenePlane(vec3 o, vec3 d, float t_max, out float marched, out vec3 normal) {
	o.x -= extbg_ground_offset_rt_float;

	float radius = extbg_ground_round_r_rt_float;
	vec2 center = vec2(radius);
	vec2 l = d.xy;
	float l_square = dot(l, l);
	vec2 OminusC = o.xy - center;
	float LdotOminusC = dot(l, OminusC);
	float radicand = square(LdotOminusC) - l_square * (dot(OminusC, OminusC) - square(radius));

	float local_inf = t_max;
	float current_d = local_inf;
	vec3 current_normal;
	if (radicand > 0) {
		float root = sqrt(radicand);
		float summand = - dot(l, OminusC);
		float d1 = (summand - root) / l_square;
		float d2 = (summand + root) / l_square;
		float d = d2;
		if (d > 0) {
			vec2 point = o.xy + d * l;
			vec2 pointLocal = point - center;
			if (pointLocal.x < 0 && pointLocal.y < 0 && d < current_d) {
				current_d = d;
				current_normal = -normalize(vec3(pointLocal, 0));
			}
		}
	}

	float d_plane1 = -o.x / d.x;
	if (d_plane1 > 0 && d_plane1 < current_d) {
		current_d = d_plane1;
		current_normal = vec3(1, 0, 0);
	}

	float d_plane2 = -o.y / d.y;
	if (d_plane2 > 0 && d_plane2 < current_d) {
		current_d = d_plane2;
		current_normal = vec3(0, 1, 0);
	}

	marched = current_d;
	normal = current_normal;
	return current_d != local_inf;
}
