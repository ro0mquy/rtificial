#ifdef BUILD_WINDOWS
#include "Backend.h"

void WindowsBackend::init(int width, int height, bool fullscreen) {
}

void WindowsBackend::play_music(int16_t* audio) {
}

bool WindowsBackend::beforeFrame() {
	return false;
}

void WindowsBackend::afterFrame() {
}

void WindowsBackend::cleanup() {
}
#endif
