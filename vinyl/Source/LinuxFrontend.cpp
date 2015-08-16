#ifdef __linux

#include "Frontend.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>

//PFNGLACTIVETEXTUREPROC            glActiveTexture;
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
PFNGLGETSHADERIVPROC              glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC         glGetShaderInfoLog;
PFNGLDEBUGMESSAGECONTROLPROC      glDebugMessageControl;
PFNGLDEBUGMESSAGECALLBACKPROC     glDebugMessageCallback;

void LinuxFrontend::init(int width, int height, bool /*fullscreen*/) {
	// get local X display
	x_display = XOpenDisplay(NULL);
#	ifdef _DEBUG
		if (NULL == x_display) {
			RT_DEBUG("error: cannot connect to X server");
		}
#	endif

	// get root window
	Window x_root_window;
	x_root_window = DefaultRootWindow(x_display);

	// set the visual attributes required for the OpenGL context
	GLint gl_attributes[] = {
		GLX_RGBA,
		GLX_DEPTH_SIZE,
		24,
		GLX_DOUBLEBUFFER,
		None
	};
	XVisualInfo* x_visual_info;
	x_visual_info = glXChooseVisual(x_display, 0, gl_attributes);
#	ifdef _DEBUG
		if (NULL == x_visual_info) {
			RT_DEBUG("error: no visual found");
		}
#	endif

	// create color map for the window
	Colormap x_color_map;
	x_color_map = XCreateColormap(x_display, x_root_window, x_visual_info->visual, AllocNone);

	// initialize XSetWindowAttributes
	XSetWindowAttributes x_set_window_attributes;
	x_set_window_attributes.colormap = x_color_map;
	x_set_window_attributes.event_mask = ExposureMask | KeyPressMask;

	// create window
	x_window = XCreateWindow(
			/* display       */ x_display,
			/* parent window */ x_root_window,
			/* initial x     */ 0,
			/* initial y     */ 0,
			/* width         */ width,
			/* height        */ height,
			/* border        */ 0,
			/* depth         */ x_visual_info->depth,
			/* window type   */ InputOutput,
			/* visual info   */ x_visual_info->visual,
			/* win-attrs set */ CWColormap | CWEventMask,
			/* XSetWinAttr   */ &x_set_window_attributes
			);

	// full screen
	const Atom x_wm_state = XInternAtom(x_display, "_NET_WM_STATE", False);
	const Atom x_fullscreen = XInternAtom(x_display, "NET_WM_STATE_FULLSCREEN", False);
	XEvent fullscreen_message;
//	memset(&fullscreen_message, 0, sizeof(fullscreen_message));
	fullscreen_message.xclient.type = ClientMessage;
	fullscreen_message.xclient.window = x_window;
	fullscreen_message.xclient.message_type = x_wm_state;
	fullscreen_message.xclient.format = 32;
	fullscreen_message.xclient.data.l[0] = 1;
	fullscreen_message.xclient.data.l[1] = x_fullscreen;
	fullscreen_message.xclient.data.l[2] = 0;

	XSendEvent(
			x_display,
			x_root_window,
			False,
			SubstructureRedirectMask | SubstructureNotifyMask,
			&fullscreen_message
	);

	// make the window appear
	XMapWindow(x_display, x_window);

	XStoreName(x_display, x_window, "rtificial");

	// create OpenGL context
	gl_context = glXCreateContext(x_display, x_visual_info, NULL, GL_TRUE);
	glXMakeCurrent(x_display, x_window, gl_context);


	// Load OpenGL functions manually
//	glActiveTexture            = (PFNGLACTIVETEXTUREPROC)            glXGetProcAddressARB((unsigned char*) "glActiveTexture");
	glGenFramebuffers          = (PFNGLGENBUFFERSPROC)               glXGetProcAddressARB((unsigned char*) "glGenFramebuffers");
	glBindFramebuffer          = (PFNGLBINDFRAMEBUFFERPROC)          glXGetProcAddressARB((unsigned char*) "glBindFramebuffer");
	glGenerateMipmap           = (PFNGLGENERATEMIPMAPPROC)           glXGetProcAddressARB((unsigned char*) "glGenerateMipmap");
	glFramebufferTexture2D     = (PFNGLFRAMEBUFFERTEXTURE2DPROC)     glXGetProcAddressARB((unsigned char*) "glFramebufferTexture2D");
	glDrawBuffers              = (PFNGLDRAWBUFFERSPROC)              glXGetProcAddressARB((unsigned char*) "glDrawBuffers");
	glDeleteFramebuffers       = (PFNGLDELETEFRAMEBUFFERSPROC)       glXGetProcAddressARB((unsigned char*) "glDeleteFramebuffers");
	glCreateShader             = (PFNGLCREATESHADERPROC)             glXGetProcAddressARB((unsigned char*) "glCreateShader");
	glShaderSource             = (PFNGLSHADERSOURCEPROC)             glXGetProcAddressARB((unsigned char*) "glShaderSource");
	glCompileShader            = (PFNGLCOMPILESHADERPROC)            glXGetProcAddressARB((unsigned char*) "glCompileShader");
	glCreateProgram            = (PFNGLCREATEPROGRAMPROC)            glXGetProcAddressARB((unsigned char*) "glCreateProgram");
	glAttachShader             = (PFNGLATTACHSHADERPROC)             glXGetProcAddressARB((unsigned char*) "glAttachShader");
	glDeleteShader             = (PFNGLDELETESHADERPROC)             glXGetProcAddressARB((unsigned char*) "glDeleteShader");
	glLinkProgram              = (PFNGLLINKPROGRAMPROC)              glXGetProcAddressARB((unsigned char*) "glLinkProgram");
	glUseProgram               = (PFNGLUSEPROGRAMPROC)               glXGetProcAddressARB((unsigned char*) "glUseProgram");
	glEnableVertexAttribArray  = (PFNGLENABLEVERTEXATTRIBARRAYPROC)  glXGetProcAddressARB((unsigned char*) "glEnableVertexAttribArray");
	glVertexAttribPointer      = (PFNGLVERTEXATTRIBPOINTERPROC)      glXGetProcAddressARB((unsigned char*) "glVertexAttribPointer");
	glUniform1f                = (PFNGLUNIFORM1FPROC)                glXGetProcAddressARB((unsigned char*) "glUniform1f");
	glUniform2f                = (PFNGLUNIFORM2FPROC)                glXGetProcAddressARB((unsigned char*) "glUniform2f");
	glUniform3f                = (PFNGLUNIFORM3FPROC)                glXGetProcAddressARB((unsigned char*) "glUniform3f");
	glUniform4f                = (PFNGLUNIFORM4FPROC)                glXGetProcAddressARB((unsigned char*) "glUniform4f");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) glXGetProcAddressARB((unsigned char*) "glDisableVertexAttribArray");
	glDeleteProgram            = (PFNGLDELETEPROGRAMPROC)            glXGetProcAddressARB((unsigned char*) "glDeleteProgram");
	glGetProgramiv             = (PFNGLGETPROGRAMIVPROC)             glXGetProcAddressARB((unsigned char*) "glGetProgramiv");
	glGetProgramInfoLog        = (PFNGLGETPROGRAMINFOLOGPROC)        glXGetProcAddressARB((unsigned char*) "glGetProgramInfoLog");
	glGetShaderiv              = (PFNGLGETSHADERIVPROC)              glXGetProcAddressARB((unsigned char*) "glGetShaderiv");
	glGetShaderInfoLog         = (PFNGLGETSHADERINFOLOGPROC)         glXGetProcAddressARB((unsigned char*) "glGetShaderInfoLog");
	glDebugMessageControl      = (PFNGLDEBUGMESSAGECONTROLPROC)      glXGetProcAddressARB((unsigned char*) "glDebugMessageControl");
	glDebugMessageCallback     = (PFNGLDEBUGMESSAGECALLBACKPROC)     glXGetProcAddressARB((unsigned char*) "glDebugMessageCallback");
}


#include <pthread.h>
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>

#include "music/4klang.linux.h"
#define AUDIO_CHANNELS 2
#ifndef LINUX_OBJECT
#	error "4klang object type does not match target architecture"
#endif

static SAMPLE_TYPE audio_buffer[MAX_SAMPLES * AUDIO_CHANNELS];

static snd_pcm_t* alsa_handle;
static snd_pcm_uframes_t audio_frames;

void LinuxFrontend::initAudio(bool threaded) {
	// run 4klang rendering in a thread
	if (threaded) {
		pthread_t audio_render_thread;
		pthread_create(&audio_render_thread, NULL, __4klang_render, audio_buffer);
	} else {
		__4klang_render(audio_buffer);
	}

	// setup alsa
	int rounding_direction = 0;

	// open PCM device for playback
	snd_pcm_open(&alsa_handle, "default", SND_PCM_STREAM_PLAYBACK, 0);

	// allocate hardware parameters object with default values
	snd_pcm_hw_params_t* params;
	snd_pcm_hw_params_alloca(&params);
	snd_pcm_hw_params_any(alsa_handle, params);

	// set hardware parameters
	// interleaved mode
	snd_pcm_hw_params_set_access(alsa_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
	// signed 16bit little endian
#	ifdef INTEGER_16BIT
		snd_pcm_hw_params_set_format(alsa_handle, params, SND_PCM_FORMAT_S16_LE);
#	else
#		error "4klang export sample type does not match alsa sample format"
#	endif
	// stereo
	snd_pcm_hw_params_set_channels(alsa_handle, params, AUDIO_CHANNELS);
	// 44.1 kb/s sampling rate
	unsigned int sampling_rate = SAMPLE_RATE;
	snd_pcm_hw_params_set_rate_near(alsa_handle, params, &sampling_rate, &rounding_direction);
	// set period size to 32 frames
	audio_frames = 32;
	snd_pcm_hw_params_set_period_size_near(alsa_handle, params, &audio_frames, &rounding_direction);
	// write parameters
	snd_pcm_hw_params(alsa_handle, params);
}

static void* __playAudio(void* /*arg*/) {
	size_t audio_buffer_size = audio_frames * AUDIO_CHANNELS;

	for (size_t audio_buffer_offset = 0;
			audio_buffer_offset < MAX_SAMPLES * AUDIO_CHANNELS;
			audio_buffer_offset += audio_buffer_size
		) {
		// write data directly from 4klang buffer to sound card
		int bytes_written = snd_pcm_writei(alsa_handle,
				audio_buffer + audio_buffer_offset,
				audio_frames
				);

		if (-EPIPE == bytes_written){
			// handle underrun
			snd_pcm_prepare(alsa_handle);
		}
	}

	// exit thread
	return NULL;
}

void LinuxFrontend::playAudio(){
	pthread_t audio_play_thread;
	pthread_create(&audio_play_thread, NULL, __playAudio, NULL);
}

// returns time in milli beats
static int rt_time = 0;
int LinuxFrontend::getTime(){
	return rt_time += 1000;
}


bool LinuxFrontend::beforeFrame() {
	XEvent x_event;
	const KeySym escape_key = XKeysymToKeycode(x_display, XK_Escape);

	// wait asynchronly for events
	if (XPending(x_display)) {
		XNextEvent(x_display, &x_event);
		switch (x_event.type) {
			case Expose:
				XWindowAttributes x_window_attributes;
				XGetWindowAttributes(x_display, x_window, &x_window_attributes);
				glViewport(0, 0, x_window_attributes.width, x_window_attributes.height);
				break;
			case KeyPress:
				if (x_event.xkey.keycode == escape_key) {
					// will initiate program termination
					return false;
				}
				break;
		}
	}

	return true;
}

void LinuxFrontend::afterFrame() {
	glXSwapBuffers(x_display, x_window);
}


#include <unistd.h>
void LinuxFrontend::sleep(int miliseconds) {
	usleep(miliseconds * 1000);
}


void LinuxFrontend::cleanup() {
	// stop audio immediately
	snd_pcm_drop(alsa_handle);
	snd_pcm_close(alsa_handle);

	glXMakeCurrent(x_display, None, NULL);
	glXDestroyContext(x_display, gl_context);
	XDestroyWindow(x_display, x_window);
	XCloseDisplay(x_display);
}

#endif
