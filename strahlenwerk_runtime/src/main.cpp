#include "Backend.h"

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
	using Backend = LinuxBackend;
#elif _WINDOWS
	using Backend = WindowsBackend;
#endif

const int width = 1920;
const int height = 1080;
const bool fullscreen = true;
const bool use_sound_thread = true;

RT_MAIN {
	RT_INIT
	Backend backend;
	backend.init(width, height, fullscreen);
	backend.initAudio(use_sound_thread);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	const int n_scenes = sizeof(scenes) / sizeof(Shader);
	const int n_postproc = sizeof(postproc) / sizeof(Shader);

	ladebalken.compile();

	float progress = 0.;
	float progress_step = 1./ (2 * n_scenes + n_postproc + 3.);

	for(int i = 0; i < n_scenes; i++) {
		scenes[i].compile();
		backend.beforeFrame();
		ladebalken.bind();
		progress += progress_step;
		glUniform1f(74, progress);
		ladebalken.draw(width, height, -1);
		backend.afterFrame();
		backend.sleep(200);
	}
	for(int i = 0; i < n_postproc; i++) {
		postproc[i].compile();
		fbos[i].create(width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		backend.beforeFrame();
		ladebalken.bind();
		progress += progress_step;
		glUniform1f(74, progress);
		ladebalken.draw(width, height, -1);
		backend.afterFrame();
		backend.sleep(200);
	}
	Shader diffuseFilterShader(diffuseFilterSource, 0, nullptr);
	Shader specularFilterShader(specularFilterSource, 0, nullptr);
	BRDFLut brdfLut;
	diffuseFilterShader.compile();
	specularFilterShader.compile();
	backend.beforeFrame();
	ladebalken.bind();
	progress += progress_step * 2;
	glUniform1f(74, progress);
	ladebalken.draw(width, height, -1);
	backend.afterFrame();
	for (int i = 0; i < n_scenes; i++) {
		if (environments[i].isValid()) {
			environments[i].create(diffuseFilterShader, specularFilterShader);
		}
		backend.beforeFrame();
		ladebalken.bind();
		progress += progress_step;
		glUniform1f(74, progress);
		ladebalken.draw(width, height, -1);
		backend.afterFrame();
	}

	backend.sleep(2000);

	backend.playAudio();

	int scene_id = 0;
	int shader_id = scenes_data[scene_id].sceneId;
	const int last_scene_id = sizeof(scenes_data) / sizeof(Scene) - 1;
	while(backend.beforeFrame()) {
		if(scenes_data[scene_id].end < backend.getTime()) {
			if(scene_id == last_scene_id) {
				break;
			} else {
				scene_id++;
				shader_id = scenes_data[scene_id].sceneId;
			}
		}

		const int currentTime = backend.getTime();

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
		backend.afterFrame();

#		ifdef _DEBUG
			RT_DEBUG((std::to_string(backend.getTime()) + "\n").c_str());
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

	backend.cleanup();
	RT_DEINIT
}
