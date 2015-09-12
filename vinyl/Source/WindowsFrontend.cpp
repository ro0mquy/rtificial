#ifdef _WINDOWS

// http://stackoverflow.com/questions/1583196/building-visual-c-app-that-doesnt-use-crt-functions-still-references-some
extern "C" {
	int _fltused = 1;
}

#include "Frontend.h"
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
PFNGLUNIFORM1IPROC                glUniform1i;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLDELETEPROGRAMPROC            glDeleteProgram;
PFNGLGETPROGRAMIVPROC             glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog;
PFNGLGETSHADERIVPROC              glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC         glGetShaderInfoLog;
PFNGLDEBUGMESSAGECONTROLPROC      glDebugMessageControl;
PFNGLDEBUGMESSAGECALLBACKPROC     glDebugMessageCallback;


void WindowsFrontend::init(int width, int height, bool fullscreen) {
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
	glUniform1i                = (PFNGLUNIFORM1IPROC)                wglGetProcAddress("glUniform1i");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress("glDisableVertexAttribArray");
	glDeleteProgram            = (PFNGLDELETEPROGRAMPROC)            wglGetProcAddress("glDeleteProgram");
	glGetProgramiv             = (PFNGLGETPROGRAMIVPROC)             wglGetProcAddress("glGetProgramiv");
	glGetProgramInfoLog        = (PFNGLGETPROGRAMINFOLOGPROC)        wglGetProcAddress("glGetProgramInfoLog");
	glGetShaderiv              = (PFNGLGETSHADERIVPROC)              wglGetProcAddress("glGetShaderiv");
	glGetShaderInfoLog         = (PFNGLGETSHADERINFOLOGPROC)         wglGetProcAddress("glGetShaderInfoLog");
	glDebugMessageControl      = (PFNGLDEBUGMESSAGECONTROLPROC)      wglGetProcAddress("glDebugMessageControl");
	glDebugMessageCallback     = (PFNGLDEBUGMESSAGECALLBACKPROC)     wglGetProcAddress("glDebugMessageCallback");

	/* adaptive vsync
	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC) wglGetProcAddress("wglSwapIntervalEXT");
	wglSwapIntervalEXT(-1);
	*/
}



#ifdef SYNTH_4KLANG
//#include <mmsystem.h>
//#include <mmreg.h>
#include "music/4klang.windows.h"
#define AUDIO_CHANNELS 2
#ifndef WINDOWS_OBJECT
#	error "4klang object type does not match target architecture"
#endif

static SAMPLE_TYPE audio_buffer[MAX_SAMPLES * AUDIO_CHANNELS];
#endif

#ifdef SYNTH_V2
// use GetTickCount() instead of V2 for time
// #define SYSTEM_TIME
#include "v2mplayer.h"
#include "libv2.h"
static V2MPlayer player;
extern "C" const sU8 soundtrack[];
#ifdef SYSTEM_TIME
	DWORD starttime;
#endif
	long startPosition;
#endif

#ifdef SYNTH_VORBIS
#include "stb_vorbis.h"
#include "music/vorbis_info.h"
static stb_vorbis *vorbis_decoder;
extern "C" const unsigned char soundtrack[];
#define SAMPLE_TYPE short
#ifndef SAMPLE_RATE
	#define SAMPLE_RATE_UNDEFINED
	int SAMPLE_RATE;
#endif
static void _vorbis_decode(int *num_samples /* see blow ... SAMPLE_TYPE *audio_buffer */);
static SAMPLE_TYPE *audio_buffer;
#endif

void WindowsFrontend::initAudio(bool threaded) {
#ifdef SYNTH_4KLANG
	if (threaded) {
		// thx to xTr1m/blu-flame for providing a smarter and smaller way to create the thread :)
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE) _4klang_render, audio_buffer, 0, 0);
	} else {
		_4klang_render(audio_buffer);
	}
#endif

#ifdef SYNTH_V2
	player.Init();
	player.Open(soundtrack);
	dsInit(player.RenderProxy, &player, GetForegroundWindow());
#endif

#ifdef SYNTH_VORBIS
	#if !(defined(AUDIO_CHANNELS) && defined(SAMPLE_RATE))
		int vorbis_error = 0;
		vorbis_decoder = stb_vorbis_open_memory(soundtrack, VORBIS_SOUNDTRACK_SIZE, &vorbis_error, NULL);
		stb_vorbis_info info = stb_vorbis_get_info(vorbis_decoder);
	
		#ifdef _DEBUG
			int err = stb_vorbis_get_error(vorbis_decoder);
			RT_DEBUG(("vorbis error code: " + std::to_string(err) + "\n").c_str());
			RT_DEBUG((std::to_string(info.channels) + " channels, " + std::to_string(info.sample_rate) + "samples/sec\n").c_str());
			RT_DEBUG(("predicted memory usage: " + std::to_string(info.setup_memory_required) + " setup + " + std::to_string(info.temp_memory_required) + " temp\n").c_str());
		#endif
	#endif
	
	#ifndef AUDIO_CHANNELS
		const int AUDIO_CHANNELS = info.channels;
	#endif
	#ifdef SAMPLE_RATE_UNDEFINED
		SAMPLE_RATE = info.sample_rate;
	#endif

	/*
	 * don't thread because we need the number of decoded threads for the WAVEHDR
	 * if another way of optaining this is found, threading can be enabled again.
	 * the parameter to _vorbis_decode() should then be SAMPLE_TYPE *audiobuffer
	 * again to be symmetric to the 4klang flow.
	 */
//	if (threaded) {
//		CreateThread(0, 0, (LPTHREAD_START_ROUTINE) _vorbis_decode, audio_buffer, 0, 0);
//	} else {
		int MAX_SAMPLES;
		_vorbis_decode(&MAX_SAMPLES);
//	}
#endif

#if defined(SYNTH_4KLANG) || defined(SYNTH_VORBIS)
	audio_wave_header = {
		(LPSTR)audio_buffer,
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
#endif
}

#ifdef SYNTH_VORBIS
static void _vorbis_decode(int *num_samples_decoded /* see above ... SAMPLE_TYPE *audio_buffer */){
	int num_samples, channels, sample_rate;
	num_samples = stb_vorbis_decode_memory(soundtrack, VORBIS_SOUNDTRACK_SIZE, &channels, &sample_rate, &audio_buffer);
	#ifdef _DEBUG
		int err = stb_vorbis_get_error(vorbis_decoder);
		RT_DEBUG(("vorbis error code: " + std::to_string(err) + "\n").c_str());
		RT_DEBUG(("number of samples decoded: " + std::to_string(num_samples) + "\n").c_str());
	#endif
	*num_samples_decoded = num_samples;
}
#endif


void WindowsFrontend::playAudio() {
#if defined(SYNTH_4KLANG) || defined(SYNTH_VORBIS)
	waveOutWrite(audio_wave_out, &audio_wave_header, sizeof(audio_wave_header));
#endif
#ifdef SYNTH_V2
	#ifdef SYSTEM_TIME
		starttime = GetTickCount();
	#endif
		startPosition = dsGetCurSmp();
	player.Play();
#endif
}


// returns time in milli beats
int WindowsFrontend::getTime(){
#if defined(SYNTH_4KLANG) || defined(SYNTH_VORBIS)
	MMTIME time;
	time.wType = TIME_SAMPLES;
	waveOutGetPosition(audio_wave_out, &time, sizeof(MMTIME));
	return int(.5 + (double) time.u.sample / SAMPLE_RATE * BPM / 60. * 1000.);
#endif
#ifdef SYNTH_V2
	#ifdef SYSTEM_TIME
		return int(.5 + double(GetTickCount() - starttime) * BPM / 60.);
	#else
		return int(.5 + (dsGetCurSmp() - startPosition) / (44.100 * sizeof(float))) * (BPM / 60.);
	#endif
#endif
#ifdef SYNTH_VORBIS
	return 0;
#endif
}


bool WindowsFrontend::checkMessageLoop() {
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		//TranslateMessage( &msg ); // Don't have any keys that need translating to WM_CHAR messages
		DispatchMessage(&msg);
	}

	return !GetAsyncKeyState(VK_ESCAPE);
}


void WindowsFrontend::beforeFrame() {
}

void WindowsFrontend::afterFrame() {
	SwapBuffers(window_handle);
}


void WindowsFrontend::sleep(int milliseconds) {
	Sleep(milliseconds);
}


void WindowsFrontend::cleanup() {
#ifdef SYNTH_V2
	dsClose();
	player.Close();
#endif
#ifdef SYNTH_VORBIS
	stb_vorbis_close(vorbis_decoder);
#endif
}

#endif
