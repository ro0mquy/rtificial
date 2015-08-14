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
#	include <string>

#	define RT_MAIN int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)

#	define RT_INIT

#	define RT_DEINIT \
		ExitProcess(0); \
		return 0;

#	define RT_DEBUG(str) \
		OutputDebugString(str);
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
		int getTime();

		bool beforeFrame();
		void afterFrame();

		void sleep(int milliseconds);

		void cleanup();

	private:
		HDC window_handle;
		HWAVEOUT audio_wave_out;
		WAVEHDR audio_wave_header;
};

#endif


#ifdef __linux

#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>

#define RT_MAIN int main()
#define RT_INIT
#define RT_DEINIT \
	return 0;

#ifdef _DEBUG
#	include <string>
#	include <iostream>

	// http://stackoverflow.com/questions/8311058/n-or-n-or-stdendl-to-stdcout
#	define RT_DEBUG(str) \
		std::cerr << str << '\n';
#endif

class LinuxBackend {
	public:
		void init(int width, int height, bool fullscreen);

		void initAudio(bool threaded);
		void playAudio();
		int getTime();

		bool beforeFrame();
		void afterFrame();

		void sleep(int milliseconds);

		void cleanup();

	private:
		Display* x_display;
		Window x_window;
		GLXContext gl_context;
};

#endif

#endif