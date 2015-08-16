#include "Frontend.h"

#include "Shader.h"
#include "Framebuffer.h"
#include "Scene.h"
#include "strahlenwerk_export_interface.h"
#include "glcorearb.h"
#include "gl_identifiers.h"
#include "shaders/ladebalken.h"
#include "DiffuseFilterSource.h"
#include "SpecularFilterSource.h"
#include "BRDFLut.h"

#ifdef __linux
	using Frontend = LinuxFrontend;
#elif _WINDOWS
	using Frontend = WindowsFrontend;
#endif

const int width = 1280;
const int height = 720;
const bool fullscreen = true;
const bool use_sound_thread = true;

#ifdef _DEBUG
void CALLBACK debugOutputGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, void *userParam);
#endif

RT_MAIN {
	RT_INIT
	Frontend frontend;
	frontend.init(width, height, fullscreen);
	frontend.initAudio(use_sound_thread);

#	ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	// enable all debug messages
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	glDebugMessageCallback((GLDEBUGPROC) &debugOutputGL, NULL);
#endif


	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	const int n_scenes = sizeof(scenes) / sizeof(Shader);
	const int n_postproc = sizeof(postproc) / sizeof(Shader);

	ladebalken.compile();

	float progress = 0.;
	float progress_step = 1./ (2 * n_scenes + n_postproc + 3.);

	for (int i = 0; i < n_scenes; i++) {
#		ifdef _DEBUG
		RT_DEBUG(("Compiling scene " + std::to_string(i) + "...\n").c_str());
#		endif
		scenes[i].compile();
#		ifdef _DEBUG
		RT_DEBUG("Compile done.\n");
#		endif
		frontend.beforeFrame();
		ladebalken.bind();
		progress += progress_step;
		glUniform1f(74, progress);
		ladebalken.draw(width, height, -1);
		frontend.afterFrame();
		frontend.sleep(200);
	}
	for (int i = 0; i < n_postproc; i++) {
#		ifdef _DEBUG
		RT_DEBUG(("Compiling postproc " + std::to_string(i) + "...\n").c_str());
#		endif
		postproc[i].compile();
#		ifdef _DEBUG
		RT_DEBUG("Compile done.\n");
#		endif
		fbos[i].create(width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		frontend.beforeFrame();
		ladebalken.bind();
		progress += progress_step;
		glUniform1f(74, progress);
		ladebalken.draw(width, height, -1);
		frontend.afterFrame();
		frontend.sleep(200);
	}
	Shader diffuseFilterShader(diffuseFilterSource, 0, nullptr);
	Shader specularFilterShader(specularFilterSource, 0, nullptr);
	BRDFLut brdfLut;
	diffuseFilterShader.compile();
	specularFilterShader.compile();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	frontend.beforeFrame();
	ladebalken.bind();
	progress += progress_step * 2;
	glUniform1f(74, progress);
	ladebalken.draw(width, height, -1);
	frontend.afterFrame();
#		ifdef _DEBUG
		RT_DEBUG("Begin environments\n");
#		endif
	for (int i = 0; i < n_scenes; i++) {
		if (environments[i].isValid()) {
#		ifdef _DEBUG
			RT_DEBUG(("Creating environment " + std::to_string(i) + "...\n").c_str());
#		endif
			environments[i].create(diffuseFilterShader, specularFilterShader);
#		ifdef _DEBUG
		RT_DEBUG("Compile done.\n");
#		endif
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		frontend.beforeFrame();
		ladebalken.bind();
		progress += progress_step;
		glUniform1f(74, progress);
		ladebalken.draw(width, height, -1);
		frontend.afterFrame();
	}
#		ifdef _DEBUG
	RT_DEBUG("End environments\n");
#		endif

	for (int i = 0; i < n_scenes; i++) {
		fbos[0].bind();
		scenes[i].draw(fbos[0].width, fbos[0].height, 0);
	}

	frontend.sleep(2000);

	frontend.playAudio();

	int scene_id = 0;
	int shader_id = scenes_data[scene_id].sceneId;
	const int last_scene_id = sizeof(scenes_data) / sizeof(Scene) - 1;
	while(frontend.beforeFrame()) {
		if(scenes_data[scene_id].end < frontend.getTime()) {
			if(scene_id == last_scene_id) {
				break;
			} else {
				scene_id++;
				shader_id = scenes_data[scene_id].sceneId;
			}
		}

		const int currentTime = frontend.getTime();

		if (environments[shader_id].isValid()) {
			environments[shader_id].bind();
		}
		fbos[0].bind();
		scenes[shader_id].draw(fbos[0].width, fbos[0].height, currentTime);
		fbos[0].unbind();
		for(int i = 0; i < n_postproc - 1; i++) {
			fbos[i + 1].bind();
			postproc[i].draw(fbos[i + 1].width, fbos[i + 1].height, currentTime);
			fbos[i + 1].unbind();
		}

		glEnable(GL_FRAMEBUFFER_SRGB);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		postproc[n_postproc - 1].draw(width, height, currentTime);
		glDisable(GL_FRAMEBUFFER_SRGB);
		frontend.afterFrame();

#		ifdef _DEBUG
			// RT_DEBUG((std::to_string(frontend.getTime()) + "\n").c_str());
#		endif
	}

	for(int i = 0; i < n_scenes; i++) {
		scenes[i].destroy();
		if (environments[shader_id].isValid()) {
			environments[shader_id].destroy();
		}
	}
	for(int i = 0; i < n_postproc; i++) {
		postproc[i].destroy();
		fbos[i].destroy();
	}
	diffuseFilterShader.destroy();
	specularFilterShader.destroy();

	frontend.cleanup();
	RT_DEINIT
}

#ifdef _DEBUG
void CALLBACK debugOutputGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, void *userParam) {
	char* debugMessage = new char[length + 2];
	for (GLsizei i = 0; i < length; i++) {
		debugMessage[i] = message[i];
	}
	debugMessage[length] = '\n';
	debugMessage[length+1] = '\0';
	RT_DEBUG(debugMessage);
	delete[] debugMessage;
}
#endif
