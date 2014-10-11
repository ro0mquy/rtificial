#ifndef BACKEND_H
#define BACKEND_H

using int16_t = short;

#ifdef _WINDOWS

#include <windows.h>
#include "stdlib.h"

// las said this was good
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#	define RT_MAIN int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#	define RT_INIT
#	define RT_DEINIT \
		ExitProcess(0); \
		return 0;
#else
#	define RT_MAIN void WinMainCRTStartup()
#	define RT_INIT \
		GlobalsStaticsInit();
#	define RT_DEINIT \
		GlobalsStaticsFree(); \
		ExitProcess(0);
#endif

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

#define RT_MAIN int main()
#define RT_INIT
#define RT_DEINIT \
	return 0;

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
