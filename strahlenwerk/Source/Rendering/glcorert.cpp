#undef GL_GLEXT_FUNCTIONS
#include "glcorert.h"

#ifdef _WINDOWS

#ifdef __cplusplus
extern "C" {
#endif

PFNGLGENQUERIESPROC           glGenQueries;
PFNGLACTIVETEXTUREPROC        glActiveTexture;
PFNGLGENERATEMIPMAPPROC       glGenerateMipmap;
PFNGLDRAWBUFFERSPROC          glDrawBuffers;
PFNGLDELETEQUERIESPROC        glDeleteQueries;
PFNGLENDQUERYPROC             glEndQuery;
PFNGLBEGINQUERYPROC           glBeginQuery;
PFNGLGETQUERYOBJECTUI64VPROC  glGetQueryObjectui64v;
PFNGLUNIFORM3FPROC            glUniform3f;
PFNGLUNIFORM1IPROC            glUniform1i;
PFNGLGENFRAMEBUFFERSPROC      glGenFramebuffers;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
PFNGLDELETEFRAMEBUFFERSPROC   glDeleteFramebuffer;
PFNGLCLEARTEXIMAGEPROC        glClearTexImage;
PFNGLBINDFRAMEBUFFERPROC      glBindFramebuffer;
PFNGLDELETEFRAMEBUFFERSPROC   glDeleteFramebuffers;


void initializeOpenGLFunctions() {
	glGenQueries           = (PFNGLGENQUERIESPROC)           OpenGLHelpers::getExtensionFunction("glGenQueries");
	glActiveTexture        = (PFNGLACTIVETEXTUREPROC)        OpenGLHelpers::getExtensionFunction("glActiveTexture");
	glGenerateMipmap       = (PFNGLGENERATEMIPMAPPROC)       OpenGLHelpers::getExtensionFunction("glGenerateMipmap");
	glDrawBuffers          = (PFNGLDRAWBUFFERSPROC)          OpenGLHelpers::getExtensionFunction("glDrawBuffers");
	glDeleteQueries        = (PFNGLDELETEQUERIESPROC)        OpenGLHelpers::getExtensionFunction("glDeleteQueries");
	glEndQuery             = (PFNGLENDQUERYPROC)             OpenGLHelpers::getExtensionFunction("glEndQuery");
	glBeginQuery           = (PFNGLBEGINQUERYPROC)           OpenGLHelpers::getExtensionFunction("glBeginQuery");
	glGetQueryObjectui64v  = (PFNGLGETQUERYOBJECTUI64VPROC)  OpenGLHelpers::getExtensionFunction("glGetQueryObjectui64v");
	glUniform3f            = (PFNGLUNIFORM3FPROC)            OpenGLHelpers::getExtensionFunction("glUniform3f");
	glUniform1i            = (PFNGLUNIFORM1IPROC)            OpenGLHelpers::getExtensionFunction("glUniform1i");
	glGenFramebuffers      = (PFNGLGENFRAMEBUFFERSPROC)      OpenGLHelpers::getExtensionFunction("glGenFramebuffers");
	glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) OpenGLHelpers::getExtensionFunction("glFramebufferTexture2D");
	glDeleteFramebuffer    = (PFNGLDELETEFRAMEBUFFERSPROC)   OpenGLHelpers::getExtensionFunction("glDeleteFramebuffer");
	glClearTexImage        = (PFNGLCLEARTEXIMAGEPROC)        OpenGLHelpers::getExtensionFunction("glClearTexImage");
	glBindFramebuffer      = (PFNGLBINDFRAMEBUFFERPROC)      OpenGLHelpers::getExtensionFunction("glBindFramebuffer");
	glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)     OpenGLHelpers::getExtensionFunction("glDeleteFramebuffers");
}

#ifdef __cplusplus
}
#endif

#endif