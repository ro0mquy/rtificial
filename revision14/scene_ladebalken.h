static const char source_ladebalken[] = "\
#version 400\n\
out vec3 out_color;\n\
uniform float progress;\n\
uniform vec2 res;\n\
\n\
void main(void){\n\
	vec2 pos = gl_FragCoord.xy / res;\n\
	pos.x = pos.x + 0.01;\n\
\n\
	float progress = floor(progress * 10)/ 10;\n\
\n\
	vec3 color_backround = vec3(0.);\n\
	vec3 color_bar = vec3(1.);\n\
\n\
	vec3 final_color = color_backround;\n\
\n\
	if(pos.y > 0.45 && pos.y < 0.55 ){\n\
		if(pos.x < progress){\n\
			final_color = color_bar;\n\
			final_color *= .1 + smoothstep(0.45, 0.55, pos.y) * .9;\n\
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
}";

static GLuint balken_program;
static GLuint balken_attrib_c2d;
static GLint uniform_progress;

void ladebalken_init(GLuint vertex) {
	const GLuint fragment = shader_load_strings(1, "balken", (const GLchar* []) { source_ladebalken }, GL_FRAGMENT_SHADER);
	balken_program = shader_link_program(vertex, fragment);
	balken_attrib_c2d = glGetAttribLocation(balken_program, "c");
	GLint res = shader_get_uniform(balken_program, "res");
	uniform_progress = shader_get_uniform(balken_program, "progress");
	glUseProgram(balken_program);
	glUniform2f(res, WIDTH, HEIGHT);
}

void ladebalken_draw(float progress) {
	glUseProgram(balken_program);
	glUniform1f(uniform_progress, progress);
	draw_quad(balken_attrib_c2d);
	SDL_GL_SwapBuffers();
}
