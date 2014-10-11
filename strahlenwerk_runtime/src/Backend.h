#ifndef BACKEND_H
#define BACKEND_H

using int16_t = short;

#ifdef _WINDOWS

#include <windows.h>
#include "music/4klang.h"
#include "stdlib.h"

// las said this was good
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

class WindowsBackend {
	public:
		void init(int width, int height, bool fullscreen);
		void initAudio(bool threaded);
		void playAudio();
		double getTime();
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
		void playAudio();
		double getTime();
		bool beforeFrame();
		void afterFrame();
		void cleanup();
};

#endif

#endif
