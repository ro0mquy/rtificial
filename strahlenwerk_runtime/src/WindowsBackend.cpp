#ifdef _WINDOWS

#include "Backend.h"
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
	DEVMODE device_mode = {
		"", 0, 0, sizeof(device_mode), 0, DM_PELSWIDTH | DM_PELSHEIGHT, { { 0, 0, 0, 0, 0, 0, 0, 0 } }, 0, 0, 0, 0, 0,
		"", 0, 0, width, height, { 0 }, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	PIXELFORMATDESCRIPTOR pixel_format_descriptor =	{
		sizeof(pixel_format_descriptor), 1, PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0
	};

	// open Windows window
	if (fullscreen) {
		ChangeDisplaySettings(&device_mode, CDS_FULLSCREEN);
	}

	window_handle = GetDC(CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE , 0, 0, 0, 0, 0, 0, 0, 0));
	SetPixelFormat(window_handle, ChoosePixelFormat(window_handle, &pixel_format_descriptor), &pixel_format_descriptor);
	wglMakeCurrent(window_handle, wglCreateContext(window_handle));
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


//#include <mmsystem.h>
//#include <mmreg.h>
#include "music/4klang.h"
#define AUDIO_CHANNELS 2

static SAMPLE_TYPE audio_buffer[MAX_SAMPLES * AUDIO_CHANNELS];

extern "C" {
	int _fltused = 1;
}

void WindowsBackend::initAudio(bool threaded) {
	if (threaded) {
		// thx to xTr1m/blu-flame for providing a smarter and smaller way to create the thread :)
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE) _4klang_render, audio_buffer, 0, 0);
	} else {
		_4klang_render(audio_buffer);
	}

	audio_wave_header = {
		(LPSTR) audio_buffer,
		MAX_SAMPLES * sizeof(SAMPLE_TYPE) * AUDIO_CHANNELS,
		0,
		0,
		0,
		0,
		0,
		0
	};

	WAVEFORMATEX wave_format = {
#		ifdef FLOAT_32BIT	
			WAVE_FORMAT_IEEE_FLOAT,
#		else
			WAVE_FORMAT_PCM,
#		endif		
		/* channels        */ AUDIO_CHANNELS,
		/* samples/second  */ SAMPLE_RATE,
		/* bytes/second    */ SAMPLE_RATE*sizeof(SAMPLE_TYPE) * AUDIO_CHANNELS,
		/* block alignment */ sizeof(SAMPLE_TYPE) * AUDIO_CHANNELS,
		/* bits/sample     */ sizeof(SAMPLE_TYPE) * 8,
		/* no extensions   */ 0
	};

	waveOutOpen(&audio_wave_out, WAVE_MAPPER, &wave_format, NULL, 0, CALLBACK_NULL);
	waveOutPrepareHeader(audio_wave_out, &audio_wave_header, sizeof(audio_wave_header));
}

void WindowsBackend::playAudio() {
	waveOutWrite(audio_wave_out, &audio_wave_header, sizeof(audio_wave_header));
}

double WindowsBackend::getTime(){
	MMTIME time;
	time.wType = TIME_SAMPLES;
	waveOutGetPosition(audio_wave_out, &time, sizeof(MMTIME));
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
	SwapBuffers(window_handle);
}


void WindowsBackend::cleanup() {}

#endif
