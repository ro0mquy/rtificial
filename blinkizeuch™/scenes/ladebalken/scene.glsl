uniform float progress;
//uniform vec2 res;

void main(void){
	vec2 pos = gl_FragCoord.xy / res;
	pos.x = pos.x + 0.01;

	float progress = floor(progress * 10)/ 10;

	vec3 color_backround = vec3(0.);
	vec3 color_bar = vec3(1.);

	vec3 final_color = color_backround;

	if(pos.y > 0.45 && pos.y < 0.55 ){
		if(pos.x < progress){
			final_color = color_bar;
			final_color *= .1 + smoothstep(0.45, 0.55, pos.y) * .9;
		}
	}

	if(int(floor(pos.x*50)) % 5 == 0){
		final_color = color_backround;
	}

	if(progress > 1.){
		final_color = color_bar;
	}

	out_color.rgb = final_color;
	out_color.a = 0.; // temp
}
