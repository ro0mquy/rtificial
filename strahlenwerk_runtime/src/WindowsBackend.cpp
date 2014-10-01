#ifdef BUILD_WINDOWS

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
PFNGLUNIFORM2FPROC                glUniform2f;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLDELETEPROGRAMPROC            glDeleteProgram;
 

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
	glUniform2f                = (PFNGLUNIFORM2FPROC)                wglGetProcAddress("glUniform2f");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress("glDisableVertexAttribArray");
	glDeleteProgram            = (PFNGLDELETEPROGRAMPROC)            wglGetProcAddress("glDeleteProgram");
}

void WindowsBackend::play_music(int16_t* audio) {
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
