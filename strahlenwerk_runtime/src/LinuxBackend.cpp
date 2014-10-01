#ifdef BUILD_LINUX
#include "Backend.h"

void LinuxBackend::init(int width, int height, bool fullscreen) {
}

void LinuxBackend::initAudio(bool threaded) {
}

bool LinuxBackend::beforeFrame() {
	return false;
}

void LinuxBackend::afterFrame() {
}

void LinuxBackend::cleanup() {
}
#endif
