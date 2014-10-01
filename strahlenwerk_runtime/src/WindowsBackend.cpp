#ifdef BUILD_WINDOWS
#include "Backend.h"
#include "gl_indentifiers.h"

void WindowsBackend::init(int width, int height, bool fullscreen) {
	// Load OpenGL functions manually
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
	glGenFramebuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
	glGenTextures = (PFNGLGENTEXTURESPROC)wglGetProcAddress("glGenTextures");
	glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
	glBindTexture = (PFNGLBINDTEXTUREPROC)wglGetProcAddress("glBindTexture");
	glTexImage2D = (PFNGLTEXIMAGE2DPROC)wglGetProcAddress("glTexImage2D");
	glTexParameteri = (PFNGLTEXPARAMETERIPROC)wglGetProcAddress("glTexParameteri");
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
	glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
	glDrawBuffers = (PFNGLDRAWBUFFERSPROC)wglGetProcAddress("glDrawBuffers");
	glViewport = (PFNGLVIEWPORTPROC)wglGetProcAddress("glViewport");
	glDeleteTextures = (PFNGLDELETETEXTURESPROC)wglGetProcAddress("glDeleteTextures");
	glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");
	glEnable = (PFNGLENABLEPROC)wglGetProcAddress("glEnable");
	glDisable = (PFNGLDISABLEPROC)wglGetProcAddress("glDisable");
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glTexParameterf = (PFNGLTEXPARAMETERFPROC)wglGetProcAddress("glTexParameterf");
	glClear = (PFNGLCLEARPROC)wglGetProcAddress("glClear");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	glUniform2f = (PFNGLUNIFORM2FPROC)wglGetProcAddress("glUniform2f");
	glDrawArrays = (PFNGLDRAWARRAYSPROC)wglGetProcAddress("glDrawArrays");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");

}

void WindowsBackend::play_music(int16_t* audio) {
}

bool WindowsBackend::beforeFrame() {
	return false;
}

void WindowsBackend::afterFrame() {
}

void WindowsBackend::cleanup() {
}
#endif
