#ifdef BUILD_LINUX

#include "Backend.h"
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

void LinuxBackend::init(int width, int height, bool fullscreen) {
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
}


void LinuxBackend::initAudio(bool threaded) {
}

void LinuxBackend::playAudio(){
}

static float time = 0.0f;
double LinuxBackend::getTime(){
	return time++;
}


bool LinuxBackend::beforeFrame() {
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

void LinuxBackend::afterFrame() {
	glXSwapBuffers(x_display, x_window);
}


void LinuxBackend::sleep(int seconds) {
}


void LinuxBackend::cleanup() {
	glXMakeCurrent(x_display, None, NULL);
	glXDestroyContext(x_display, gl_context);
	XDestroyWindow(x_display, x_window);
	XCloseDisplay(x_display);
}

#endif
