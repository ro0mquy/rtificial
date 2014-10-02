#ifndef BACKEND_H
#define BACKEND_H

using int16_t = short;

// interface for backends
class EmptyBackend {
	void init(int width, int height, bool fullscreen);
	void play_music(int16_t* audio);
	// return false for abort
	bool beforeFrame();
	void afterFrame();
	void cleanup();
};

#ifdef BUILD_WINDOWS

#include <windows.h>
#include "music/4klang.h"

class WindowsBackend {
	public:
		void init(int width, int height, bool fullscreen);
		void initAudio(bool threaded);
		bool beforeFrame();
		void afterFrame();
		void cleanup();

	private:
		HDC hDC;
};
#endif

#ifdef BUILD_LINUX
class LinuxBackend {
	public:
		void init(int width, int height, bool fullscreen);
		void initAudio(bool threaded);
		bool beforeFrame();
		void afterFrame();
		void cleanup();
};
#endif

#endif
