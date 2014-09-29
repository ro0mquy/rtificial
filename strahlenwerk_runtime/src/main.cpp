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

	// TODO
	// musik
	// ladebalken/precalc

	while(backend.beforeFrame()) {
		// TODO draw stuff
		backend.afterFrame();
	}

	backend.cleanup();

	return 0;
}
