// you need #include "font.glsl"

void greetings(vec2 p, inout vec3 out_color, inout float zbuffer){
	p.y = -p.y;

	vec2 positions[4] = vec2[](
		/* top    right */ vec2( .0, -.7),
		/* top    left  */ vec2(-.8, -.7),
		/* bottom right */ vec2( .0,  .5),
		/* bottom left  */ vec2(-.8,  .5)
	);

	vec3 font_color = font_color_rt_color * font_brightness_rt_float;
	int which = int(greetings_rt_float);
	vec2 position_correction = greetings_position_rt_vec3.xy;
	float carret_inc = 3.;
	float carret;
	vec2 ap;
	float d;

	switch(which){
	case 1: /* XayaX */
		ap = (p - positions[which%4] - position_correction) * font_size_rt_float;
		carret = 0.;
		d = X(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
		d = min(d, A(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, Y(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, A(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, X(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		break;
	case 2: /* stroboholics */
		ap = (p - positions[which%4] - position_correction) * font_size_rt_float;
		carret = 0.;
		d = S(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
		d = min(d, T(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, O(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, B(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, O(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, H(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, O(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, L(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, I(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, C(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, S(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		break;
	case 3: /* still */
		ap = (p - positions[which%4] - position_correction) * font_size_rt_float;
		carret = 0.;
		d = S(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
		d = min(d, T(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, I(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, L(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, L(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		break;
	case 4: /* peisik */
		ap = (p - positions[which%4] - position_correction) * font_size_rt_float;
		carret = 0.;
		d = P(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
		d = min(d, E(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, I(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, S(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, I(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, K(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		break;
	case 5: /* nerdarzt */
		ap = (p - positions[which%4] - position_correction) * font_size_rt_float;
		carret = 0.;
		d = N(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
		d = min(d, E(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, D(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, A(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, Z(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, T(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		break;
	case 6: /* Mercury */
		ap = (p - positions[which%4] - position_correction) * font_size_rt_float;
		carret = 0.;
		d = M(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
		d = min(d, E(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, C(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, U(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, Y(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		break;
	case 7: /* LNX */
		ap = (p - positions[which%4] - position_correction) * font_size_rt_float;
		carret = 0.;
		d = L(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
		d = min(d, N(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, X(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		break;
	case 8: /* farbrausch */
		ap = (p - positions[which%4] - position_correction) * font_size_rt_float;
		carret = 0.;
		d = F(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
		d = min(d, A(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, B(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, A(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, U(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, S(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, C(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, H(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		break;
	case 9: /* conspiracy */
		ap = (p - positions[which%4] - position_correction) * font_size_rt_float;
		carret = 0.;
		d = C(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
		d = min(d, O(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, N(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, S(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, P(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, I(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, A(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, C(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, Y(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		break;
	case 10: /* code red */
		ap = (p - positions[which%4] - position_correction) * font_size_rt_float;
		carret = 0.;
		d = C(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
		d = min(d, O(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, D(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, E(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		carret += carret_inc;
		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, E(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, D(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		break;
	case 11: /* brain control */
		ap = (p - positions[which%4] - position_correction) * font_size_rt_float;
		carret = 0.;
		d = B(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, A(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, I(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, N(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		carret += carret_inc;
		d = min(d, C(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, O(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, N(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, T(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, R(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, O(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, L(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		break;
	case 12: /* ASD */
		ap = (p - positions[which%4] - position_correction) * font_size_rt_float;
		carret = 0.;
		d = A(ap - vec2(carret, 0.)) - font_thickness_rt_float; carret += carret_inc;
		d = min(d, S(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		d = min(d, D(ap - vec2(carret, 0.)) - font_thickness_rt_float); carret += carret_inc;
		break;
	}

	if(which != 0){
		out_color = mix(out_color, font_color, step(d, 0.));
		zbuffer = mix(zbuffer, camera_focus_dist, step(d, 0.));
	}
}

