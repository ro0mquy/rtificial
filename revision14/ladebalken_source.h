static const char scene_ladebalken_source[] = "\
#version 400\n\
uniform float progress;\n\
uniform vec2 res;\n\
out vec3 out_color;\n\
\n\
void main(void){\n\
	vec2 pos = gl_FragCoord.xy / res;\n\
	pos.x = pos.x + 0.01;\n\
\n\
	float progress = floor(progress * 10)/ 10;\n\
\n\
	vec3 color_backround = vec3(.01);\n\
	vec3 color_bar = vec3(.9);\n\
\n\
	vec3 final_color = color_backround;\n\
\n\
	if(pos.y > 0.45 && pos.y < 0.55 ){\n\
		if(pos.x < progress){\n\
			final_color = color_bar;\n\
			final_color *= .9;\n\
		}\n\
	}\n\
\n\
	if(int(floor(pos.x*50)) % 5 == 0){\n\
		final_color = color_backround;\n\
	}\n\
\n\
	if(progress > 1.){\n\
		final_color = color_bar;\n\
	}\n\
\n\
	out_color.rgb = final_color;\n\
}\n\
";
