#ifdef BUILD_LINUX

#include "Backend.h"
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
PFNGLUNIFORM2FPROC                glUniform2f;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLDELETEPROGRAMPROC            glDeleteProgram;

void LinuxBackend::init(int width, int height, bool fullscreen) {
	// Load OpenGL functions manually
	//glActiveTexture            = (PFNGLACTIVETEXTUREPROC)            glXGetProcAddressARB((unsigned char*) "glActiveTexture");
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
	glUniform2f                = (PFNGLUNIFORM2FPROC)                glXGetProcAddressARB((unsigned char*) "glUniform2f");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) glXGetProcAddressARB((unsigned char*) "glDisableVertexAttribArray");
	glDeleteProgram            = (PFNGLDELETEPROGRAMPROC)            glXGetProcAddressARB((unsigned char*) "glDeleteProgram");
}

void LinuxBackend::initAudio(bool threaded) {
}

bool LinuxBackend::beforeFrame() {
	return false;
}

void LinuxBackend::afterFrame() {
}

void LinuxBackend::cleanup() {
}
#endif
