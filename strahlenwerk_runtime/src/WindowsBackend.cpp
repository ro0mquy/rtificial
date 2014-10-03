#ifdef _WINDOWS

#include "Backend.h"
#include <windows.h>
#include "glcorearb.h"

PFNGLACTIVETEXTUREPROC            glActiveTexture;
PFNGLGENBUFFERSPROC               glGenFramebuffers;
PFNGLBINDFRAMEBUFFERPROC          glBindFramebuffer;
PFNGLGENERATEMIPMAPPROC           glGenerateMipmap;
PFNGLFRAMEBUFFERTEXTURE2DPROC     glFramebufferTexture2D;
PFNGLDRAWBUFFERSPROC              glDrawBuffers;
PFNGLDELETEFRAMEBUFFERSPROC       glDeleteFramebuffers;
PFNGLCREATESHADERPROC             glCreateShader;
PFNGLSHADERSOURCEPROC             glShaderSource;
PFNGLCOMPILESHADERPROC            glCompileShader;
PFNGLCREATEPROGRAMPROC            glCreateProgram;
PFNGLATTACHSHADERPROC             glAttachShader;
PFNGLDELETESHADERPROC             glDeleteShader;
PFNGLLINKPROGRAMPROC              glLinkProgram;
PFNGLUSEPROGRAMPROC               glUseProgram;
PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer;
PFNGLUNIFORM1FPROC                glUniform1f;
PFNGLUNIFORM2FPROC                glUniform2f;
PFNGLUNIFORM3FPROC                glUniform3f;
PFNGLUNIFORM4FPROC                glUniform4f;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLDELETEPROGRAMPROC            glDeleteProgram;
PFNGLGETPROGRAMIVPROC             glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog;


void WindowsBackend::init(int width, int height, bool fullscreen) {

	DEVMODE dm = {
		"", 0, 0, sizeof(dm), 0, DM_PELSWIDTH | DM_PELSHEIGHT, { { 0, 0, 0, 0, 0, 0, 0, 0 } }, 0, 0, 0, 0, 0,
		"", 0, 0, width, height, { 0 }, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	PIXELFORMATDESCRIPTOR pfd =	{
		sizeof(pfd), 1, PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0
	};

	// open Windows window
	if (fullscreen){
		ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
	}

	hDC = GetDC(CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE , 0, 0, 0, 0, 0, 0, 0, 0));
	SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);
	wglMakeCurrent(hDC, wglCreateContext(hDC));
	ShowCursor(FALSE);

	// Load OpenGL functions manually
	glActiveTexture            = (PFNGLACTIVETEXTUREPROC)            wglGetProcAddress("glActiveTexture");
	glGenFramebuffers          = (PFNGLGENBUFFERSPROC)               wglGetProcAddress("glGenFramebuffers");
	glBindFramebuffer          = (PFNGLBINDFRAMEBUFFERPROC)          wglGetProcAddress("glBindFramebuffer");
	glGenerateMipmap           = (PFNGLGENERATEMIPMAPPROC)           wglGetProcAddress("glGenerateMipmap");
	glFramebufferTexture2D     = (PFNGLFRAMEBUFFERTEXTURE2DPROC)     wglGetProcAddress("glFramebufferTexture2D");
	glDrawBuffers              = (PFNGLDRAWBUFFERSPROC)              wglGetProcAddress("glDrawBuffers");
	glDeleteFramebuffers       = (PFNGLDELETEFRAMEBUFFERSPROC)       wglGetProcAddress("glDeleteFramebuffers");
	glCreateShader             = (PFNGLCREATESHADERPROC)             wglGetProcAddress("glCreateShader");
	glShaderSource             = (PFNGLSHADERSOURCEPROC)             wglGetProcAddress("glShaderSource");
	glCompileShader            = (PFNGLCOMPILESHADERPROC)            wglGetProcAddress("glCompileShader");
	glCreateProgram            = (PFNGLCREATEPROGRAMPROC)            wglGetProcAddress("glCreateProgram");
	glAttachShader             = (PFNGLATTACHSHADERPROC)             wglGetProcAddress("glAttachShader");
	glDeleteShader             = (PFNGLDELETESHADERPROC)             wglGetProcAddress("glDeleteShader");
	glLinkProgram              = (PFNGLLINKPROGRAMPROC)              wglGetProcAddress("glLinkProgram");
	glUseProgram               = (PFNGLUSEPROGRAMPROC)               wglGetProcAddress("glUseProgram");
	glEnableVertexAttribArray  = (PFNGLENABLEVERTEXATTRIBARRAYPROC)  wglGetProcAddress("glEnableVertexAttribArray");
	glVertexAttribPointer      = (PFNGLVERTEXATTRIBPOINTERPROC)      wglGetProcAddress("glVertexAttribPointer");
	glUniform1f                = (PFNGLUNIFORM1FPROC)                wglGetProcAddress("glUniform1f");
	glUniform2f                = (PFNGLUNIFORM2FPROC)                wglGetProcAddress("glUniform2f");
	glUniform3f                = (PFNGLUNIFORM3FPROC)                wglGetProcAddress("glUniform3f");
	glUniform4f                = (PFNGLUNIFORM4FPROC)                wglGetProcAddress("glUniform4f");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress("glDisableVertexAttribArray");
	glDeleteProgram            = (PFNGLDELETEPROGRAMPROC)            wglGetProcAddress("glDeleteProgram");
	glGetProgramiv             = (PFNGLGETPROGRAMIVPROC)             wglGetProcAddress("glGetProgramiv");
	glGetProgramInfoLog        = (PFNGLGETPROGRAMINFOLOGPROC)        wglGetProcAddress("glGetProgramInfoLog");
}


// audio
//#include <mmsystem.h>
//#include <mmreg.h>

// some song information
#include "music/4klang.h"

// MAX_SAMPLES gives you the number of samples for the whole song. we always produce stereo samples, so times 2 for the buffer
SAMPLE_TYPE	lpSoundBuffer[MAX_SAMPLES * 2];
HWAVEOUT	hWaveOut;

WAVEFORMATEX WaveFMT = {
#ifdef FLOAT_32BIT	
	WAVE_FORMAT_IEEE_FLOAT,
#else
	WAVE_FORMAT_PCM,
#endif		
	2, // channels
	SAMPLE_RATE, // samples per sec
	SAMPLE_RATE*sizeof(SAMPLE_TYPE) * 2, // bytes per sec
	sizeof(SAMPLE_TYPE) * 2, // block alignment;
	sizeof(SAMPLE_TYPE) * 8, // bits per sample
	0 // extension not needed
};

WAVEHDR WaveHDR = {
	(LPSTR)lpSoundBuffer,
	MAX_SAMPLES*sizeof(SAMPLE_TYPE) * 2,			// MAX_SAMPLES*sizeof(float)*2(stereo)
	0,
	0,
	0,
	0,
	0,
	0
};

MMTIME MMTime = {
	TIME_SAMPLES,
	0
};

extern "C" {
	int _fltused = 1;
}

void WindowsBackend::initAudio(bool threaded) {
	if (threaded){
		// thx to xTr1m/blu-flame for providing a smarter and smaller way to create the thread :)
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)_4klang_render, lpSoundBuffer, 0, 0);
	}
	else {
		_4klang_render(lpSoundBuffer);
	}

	waveOutOpen(&hWaveOut, WAVE_MAPPER, &WaveFMT, NULL, 0, CALLBACK_NULL);
	waveOutPrepareHeader(hWaveOut, &WaveHDR, sizeof(WaveHDR));
}

void WindowsBackend::playAudio() {
	waveOutWrite(hWaveOut, &WaveHDR, sizeof(WaveHDR));
}

double WindowsBackend::getTime(){
	MMTIME time;
	time.wType = TIME_SAMPLES;
	waveOutGetPosition(hWaveOut, &time, sizeof(MMTIME));
	return double(time.u.sample) / SAMPLE_RATE * BPM / 60;
}

bool WindowsBackend::beforeFrame() {

	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		//TranslateMessage( &msg ); // Don't have any keys that need translating to WM_CHAR messages
		DispatchMessage(&msg);
	}

	return !GetAsyncKeyState(VK_ESCAPE);
}

void WindowsBackend::afterFrame() {
	SwapBuffers(hDC);
}

void WindowsBackend::cleanup() {
//	ExitProcess(0);
}
#endif
