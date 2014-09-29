#include "Backend.h"

#include "strahlenwerk_export.h"

const int width = 800;
const int height = 600;
const bool fullscreen = false;

#ifdef BUILD_LINUX
	using Backend = LinuxBackend;
#elif BUILD_WINDOWS
	using Backend = WindowsBackend;
#endif

int main() {
	// init backend
	Backend backend;
	backend.init(width, height, fullscreen);

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

	while(backend.beforeFrame()) {
		fbos[0].bind();
		// TODO find scene
		scenes[0].draw(fbos[0].width, fbos[0].height);
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
