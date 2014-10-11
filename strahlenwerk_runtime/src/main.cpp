#include "Backend.h"

#include "Shader.h"
#include "Framebuffer.h"
#include "Scene.h"
#include "strahlenwerk_export_interface.h"
#include "glcorearb.h"
#include "gl_identifiers.h"
#include "shaders/ladebalken.h"

#ifdef BUILD_LINUX
	using Backend = LinuxBackend;
#elif _WINDOWS
	using Backend = WindowsBackend;
#endif

const int width = 1920;
const int height = 1080;
const bool fullscreen = true;
const bool use_sound_thread = true;

#ifdef _WINDOWS
#	ifdef _DEBUG
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#	else
void WinMainCRTStartup(){
	GlobalsStaticsInit();
#	endif
#else
	int main() {
#endif

	// init backend
	Backend backend;
	backend.init(width, height, fullscreen);
	backend.initAudio(use_sound_thread);

	const int n_scenes = sizeof(scenes) / sizeof(Shader);
	const int n_postproc = sizeof(postproc) / sizeof(Shader);

	ladebalken.compile();

	float progress = 0.;
	float progress_step = 1./ (n_scenes + n_postproc);

	for(int i = 0; i < n_scenes; i++) {
		scenes[i].compile();
		backend.beforeFrame();
		ladebalken.bind();
		progress += progress_step;
		glUniform1f(74, progress);
		ladebalken.draw(width, height, -1.);
		backend.afterFrame();
		Sleep(100);
	}
	for(int i = 0; i < n_postproc; i++) {
		postproc[i].compile();
		fbos[i].create(width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		backend.beforeFrame();
		ladebalken.bind();
		progress += progress_step;
		glUniform1f(74, progress);
		ladebalken.draw(width, height, -1.);
		backend.afterFrame();
		Sleep(100);
	}

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

		const double currentTime = backend.getTime();

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

#		ifdef _DEBUG && _WINDOWS
		OutputDebugString((std::to_string(backend.getTime()) + "\n").c_str());
#		endif
	}

	for(int i = 0; i < n_scenes; i++) {
		scenes[i].destroy();
	}
	for(int i = 0; i < n_postproc; i++) {
		postproc[i].destroy();
		fbos[i].destroy();
	}

	backend.cleanup();
	GlobalsStaticsFree();
	ExitProcess(0);
#	ifdef _DEBUG
	return 0;
#	endif
}
