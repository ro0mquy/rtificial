#ifdef BUILD_LINUX
#include "Backend.h"

void LinuxBackend::init(int width, int height, bool fullscreen) {
}

void LinuxBackend::play_music(int16_t* audio) {
}

bool LinuxBackend::beforeFrame() {
	return false;
}

void LinuxBackend::afterFrame() {
}

void LinuxBackend::cleanup() {
}
#endif
