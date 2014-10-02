#include "Backend.h"

#include "strahlenwerk_export.h"

#include "glcorearb.h"
#include "gl_identifiers.h"

#ifdef BUILD_LINUX
	using Backend = LinuxBackend;

#elif BUILD_WINDOWS
	// las said this was good
#	define VC_EXTRALEAN
#	define WIN32_LEAN_AND_MEAN

#	include <windows.h>
#	include <GL/gl.h>

	using Backend = WindowsBackend;
#endif

const int width = 1920;
const int height = 1200;
const bool fullscreen = true;
const bool use_sound_thread = true;

#ifdef BUILD_WINDOWS
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#else
	int main() {
#endif

	// init backend
	Backend backend;
	backend.init(width, height, fullscreen);
	backend.initAudio(use_sound_thread);

	const int n_scenes = sizeof(scenes) / sizeof(Shader);
	const int n_postproc = sizeof(postproc) / sizeof(Shader);

	// TODO
	// musik
	// ladebalken/precalc

	for(int i = 0; i < n_scenes; i++) {
		scenes[i].compile();
	}
	for(int i = 0; i < n_postproc; i++) {
		postproc[i].compile();
		fbos[i].create(width, height);
	}

	int time = 0.; // TODO
	int scene_id = 0;
	int shader_id = scenes_data[scene_id].sceneId;
	const int last_scene_id = sizeof(scenes_data) / sizeof(Scene) - 1;
	while(backend.beforeFrame()) {
		if(scenes_data[scene_id].end < time) {
			if(scene_id == last_scene_id) {
				break;
			} else {
				scene_id++;
				shader_id = scenes_data[scene_id].sceneId;
			}
		}

		fbos[0].bind();
		scenes[shader_id].draw(fbos[0].width, fbos[0].height);
		fbos[0].unbind();
		for(int i = 0; i < n_postproc - 1; i++) {
			fbos[i + 1].bind();
			postproc[i].draw(fbos[i + 1].width, fbos[i + 1].height);
			fbos[i + 1].unbind();
		}
		glEnable(GL_FRAMEBUFFER_SRGB);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		postproc[n_postproc - 1].draw(width, height);
		glDisable(GL_FRAMEBUFFER_SRGB);
		backend.afterFrame();
	}

	for(int i = 0; i < n_scenes; i++) {
		scenes[i].destroy();
	}
	for(int i = 0; i < n_postproc; i++) {
		postproc[i].destroy();
		fbos[i].destroy();
	}

	backend.cleanup();

	return 0;
}
