static GLuint post_framebuffer;
static GLuint post_program = 0;
static GLuint post_tex_buffer;
static GLuint post_depth_buffer;
static GLuint bloom_framebuffers[3];
static GLuint bloom_programs[3] = { 0, 0, 0 };
static GLuint bloom_tex_buffers[3];
static GLuint gamma_framebuffer;
static GLuint gamma_program = 0;
static GLuint gamma_tex_buffer;
static GLuint fxaa_framebuffer;
static GLuint fxaa_program = 0;
static GLuint fxaa_tex_buffer;
static GLint post_attribute_coord2d;
static GLint bloom_attribute_coord2ds[3];
static GLint gamma_attribute_coord2d;
static GLint fxaa_attribute_coord2d;
static GLint uniform_bloom_vertical;
static GLint uniform_inv_world_camera_matrix, uniform_prev_world_camera_matrix;

#include <stdio.h>

#define snprintf c99_snprintf

inline int c99_vsnprintf(char* str, size_t size, const char* format, va_list ap)
{
    int count = -1;

    if (size != 0)
        count = _vsnprintf_s(str, size, _TRUNCATE, format, ap);
    if (count == -1)
        count = _vscprintf(format, ap);

    return count;
}
inline int c99_snprintf(char* str, size_t size, const char* format, ...)
{
    int count;
    va_list ap;

    va_start(ap, format);
    count = c99_vsnprintf(str, size, format, ap);
    va_end(ap);

    return count;
}

void postproc_init(GLuint vertex_shader) {
	// compile postprocessing program
	GLuint post_fragment_shader;// = shader_load_files(1, (const char* []) { post_path }, GL_FRAGMENT_SHADER);
	// if post.glsl file doesn't exist, load default shader which does nothing
	//if (post_fragment_shader == 0) {
		const GLchar* temp_glchar[] = { post_default_fragment_source };
		post_fragment_shader = shader_load_strings(1, "post default", temp_glchar , GL_FRAGMENT_SHADER);
	//}

	if(post_program != 0) glDeleteProgram(post_program);
	post_program = shader_link_program(vertex_shader, post_fragment_shader);
	glDeleteShader(post_fragment_shader);

	const char post_attribute_coord2d_name[] = "c";
	post_attribute_coord2d = glGetAttribLocation(post_program, post_attribute_coord2d_name);
	if(post_attribute_coord2d == -1) {
		fprintf(stderr, "Could not bind attribute %s for postprocessing\n", post_attribute_coord2d_name);
		return;
	}

	glUseProgram(post_program);
	uniform_prev_world_camera_matrix = shader_get_uniform(post_program, "previous_world_to_camera_matrix");
	uniform_inv_world_camera_matrix = shader_get_uniform(post_program, "inverse_world_to_camera_matrix");
	GLuint post_uniform_tex = shader_get_uniform(post_program, "tex");
	GLuint post_uniform_depth = shader_get_uniform(post_program, "tex_depth");
	glUniform1i(post_uniform_tex, /*GL_TEXTURE*/0);
	glUniform1i(post_uniform_depth, /*GL_TEXTURE*/1);

	// compile bloom programs
	for (size_t i = 0; i < 3; i++) {
		char bloom_shader_name[7];
		snprintf(bloom_shader_name, 7, "bloom%zu", i);
		const GLchar* tmp_glchar[] = { bloom_fragment_sources[i] };
		GLuint bloom_fragment_shader = shader_load_strings(1, bloom_shader_name, tmp_glchar, GL_FRAGMENT_SHADER);
		if (bloom_programs[i] != 0) glDeleteProgram(bloom_programs[i]);
		bloom_programs[i] = shader_link_program(vertex_shader, bloom_fragment_shader);
		glDeleteShader(bloom_fragment_shader);

		const char bloom_attribute_coord2d_name[] = "c";
		bloom_attribute_coord2ds[i] = glGetAttribLocation(bloom_programs[i], bloom_attribute_coord2d_name);
		if(bloom_attribute_coord2ds[i] == -1) {
			fprintf(stderr, "Could not bind attribute %s for bloom%zu\n", bloom_attribute_coord2d_name, i);
			return;
		}

		glUseProgram(bloom_programs[i]);
		GLuint bloom_uniform_tex = shader_get_uniform(bloom_programs[i], "tex");
		glUniform1i(bloom_uniform_tex, /*GL_TEXTURE*/0);
		if (i == 1) {
			// should we blur vertical or horizontal
			uniform_bloom_vertical = shader_get_uniform(bloom_programs[i], "vertical");
		} else if (i == 2) {
			// last shader takes original and blured image as input
			GLuint bloom_uniform_blurtex = shader_get_uniform(bloom_programs[i], "blurtex");
			glUniform1i(bloom_uniform_blurtex, /*GL_TEXTURE*/1);
		}
	}

	// compile gamma correction program
	const GLchar* temp_glchar_foo[] = { gamma_fragment_source };
	GLuint gamma_fragment_shader = shader_load_strings(1, "gamma", temp_glchar_foo, GL_FRAGMENT_SHADER);
	if (gamma_program != 0) glDeleteProgram(gamma_program);
	gamma_program = shader_link_program(vertex_shader, gamma_fragment_shader);
	glDeleteShader(gamma_fragment_shader);

	const char gamma_attribute_coord2d_name[] = "c";
	gamma_attribute_coord2d = glGetAttribLocation(gamma_program, gamma_attribute_coord2d_name);
	if(gamma_attribute_coord2d == -1) {
		fprintf(stderr, "Could not bind attribute %s for anti-aliasing\n", gamma_attribute_coord2d_name);
		return;
	}

	glUseProgram(gamma_program);
	GLuint gamma_uniform_tex = shader_get_uniform(gamma_program, "tex");
	glUniform1i(gamma_uniform_tex, /*GL_TEXTURE*/0);

	// compile anti-aliasing program
	const GLchar* foo_tmp_glchar[] =  { fxaa_fragment_source };
	GLuint fxaa_fragment_shader = shader_load_strings(1, "fxaa", foo_tmp_glchar, GL_FRAGMENT_SHADER);
	if (fxaa_program != 0) glDeleteProgram(fxaa_program);
	fxaa_program = shader_link_program(vertex_shader, fxaa_fragment_shader);
	glDeleteShader(fxaa_fragment_shader);

	const char fxaa_attribute_coord2d_name[] = "c";
	fxaa_attribute_coord2d = glGetAttribLocation(fxaa_program, fxaa_attribute_coord2d_name);
	if(fxaa_attribute_coord2d == -1) {
		fprintf(stderr, "Could not bind attribute %s for anti-aliasing\n", fxaa_attribute_coord2d_name);
		return;
	}

	glUseProgram(fxaa_program);
	GLuint fxaa_uniform_tex = shader_get_uniform(fxaa_program, "tex");
	glUniform1i(fxaa_uniform_tex, /*GL_TEXTURE*/0);



	// create target framebuffer and texture for postprocessing
	glGenTextures(1, &post_tex_buffer);
	glBindTexture(GL_TEXTURE_2D, post_tex_buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// depth buffer
	glGenTextures(1, &post_depth_buffer);
	glBindTexture(GL_TEXTURE_2D, post_depth_buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// bloom buffers
	// one for the as a render target
	// two smaller for bluring
	glGenTextures(3, bloom_tex_buffers);
	for (size_t i = 0; i < 3; i++) {
		glBindTexture(GL_TEXTURE_2D, bloom_tex_buffers[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	}

	// gamma correction buffer
	glGenTextures(1, &gamma_tex_buffer);
	glBindTexture(GL_TEXTURE_2D, gamma_tex_buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// anti-aliasing buffer
	glGenTextures(1, &fxaa_tex_buffer);
	glBindTexture(GL_TEXTURE_2D, fxaa_tex_buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// make both textures as big as the window
	// postprocessing
	glBindTexture(GL_TEXTURE_2D, post_tex_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16,
			WIDTH,
			HEIGHT,
			0, GL_RGBA, GL_UNSIGNED_SHORT, NULL);

	glBindTexture(GL_TEXTURE_2D, post_depth_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F,
			WIDTH,
			HEIGHT,
			0, GL_RED, GL_FLOAT, NULL);

	// bloom
	glBindTexture(GL_TEXTURE_2D, bloom_tex_buffers[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16,
			WIDTH,
			HEIGHT,
			0, GL_RGBA, GL_UNSIGNED_SHORT, NULL);

	float shrink = .5;
	glBindTexture(GL_TEXTURE_2D, bloom_tex_buffers[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16,
			WIDTH * shrink,
			HEIGHT * shrink,
			0, GL_RGB, GL_UNSIGNED_SHORT, NULL);

	glBindTexture(GL_TEXTURE_2D, bloom_tex_buffers[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16,
			WIDTH * shrink,
			HEIGHT * shrink,
			0, GL_RGB, GL_UNSIGNED_SHORT, NULL);

	// gamma correction
	glBindTexture(GL_TEXTURE_2D, gamma_tex_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16,
			WIDTH,
			HEIGHT,
			0, GL_RGB, GL_UNSIGNED_SHORT, NULL);

	// fxaa
	glBindTexture(GL_TEXTURE_2D, fxaa_tex_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16,
			WIDTH,
			HEIGHT,
			0, GL_RGBA, GL_UNSIGNED_SHORT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);


	// postprocessing framebuffer
	glGenFramebuffers(1, &post_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, post_framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, post_tex_buffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, post_depth_buffer, 0);
	// specify the attachments to be drawn to
	const GLenum tempdings[] =  { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, tempdings);

	// zomg error checking!1!elf1
	GLenum status;
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "Postprocessing framebuffer not complete: error %X\n", status);
	}

	// bloom framebuffers
	glGenFramebuffers(3, bloom_framebuffers);
	for (size_t i = 0; i < 3; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, bloom_framebuffers[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloom_tex_buffers[i], 0);
		if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
			fprintf(stderr, "Bloom framebuffer #%zu not complete: error %X\n", i, status);
		}
	}

	// gamma correction framebuffer
	glGenFramebuffers(1, &gamma_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gamma_framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gamma_tex_buffer, 0);
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "Gamma framebuffer not complete: error %X\n", status);
	}

	// anti-aliasing framebuffer
	glGenFramebuffers(1, &fxaa_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, fxaa_framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fxaa_tex_buffer, 0);
	// specify the attachments to be drawn to
	GLenum glpenis[]= { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, glpenis);

	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "FXAA framebuffer not complete: error %X\n", status);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static void postproc_before_draw(void) {
	glBindFramebuffer(GL_FRAMEBUFFER, post_framebuffer);
}

static void postproc_after_draw(void) {
	int bloom_enabled = true;
	if (bloom_enabled) {
		// switch to bloom buffer
		glBindFramebuffer(GL_FRAMEBUFFER, bloom_framebuffers[0]);
	} else {
		// switch to gamma correction buffer
		glBindFramebuffer(GL_FRAMEBUFFER, gamma_framebuffer);
	}

	// apply postprocessing
	glUseProgram(post_program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, post_tex_buffer);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, post_depth_buffer);

	draw_quad(post_attribute_coord2d);

	if (bloom_enabled) {
		// filter brights and scale down
		glViewport(0, 0, WIDTH * .5, HEIGHT * .5);
		glBindFramebuffer(GL_FRAMEBUFFER, bloom_framebuffers[1]);
		glUseProgram(bloom_programs[0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, bloom_tex_buffers[0]);
		draw_quad(bloom_attribute_coord2ds[0]);

		// apply first (vertical) blur to scaled down image
		glBindFramebuffer(GL_FRAMEBUFFER, bloom_framebuffers[2]);
		glUseProgram(bloom_programs[1]);
		glUniform1i(uniform_bloom_vertical, 1); // true
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, bloom_tex_buffers[1]);
		draw_quad(bloom_attribute_coord2ds[1]);

		// apply second (horizontal) blur
		glBindFramebuffer(GL_FRAMEBUFFER, bloom_framebuffers[1]);
		glUniform1i(uniform_bloom_vertical, 0); // false
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, bloom_tex_buffers[2]);
		draw_quad(bloom_attribute_coord2ds[1]);

		// combine blured and original image
		glViewport(0, 0, WIDTH, HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, gamma_framebuffer);
		glUseProgram(bloom_programs[2]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, bloom_tex_buffers[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, bloom_tex_buffers[1]);
		draw_quad(bloom_attribute_coord2ds[2]);
	}

	// correct gamma and store luma in alpha channel
	glBindFramebuffer(GL_FRAMEBUFFER, fxaa_framebuffer);
	glUseProgram(gamma_program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gamma_tex_buffer);
	draw_quad(gamma_attribute_coord2d);

	// switch back to normal screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// apply anti-aliasing
	glUseProgram(fxaa_program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fxaa_tex_buffer);
	draw_quad(fxaa_attribute_coord2d);
}
