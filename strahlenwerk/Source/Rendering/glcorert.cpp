#undef GL_GLEXT_FUNCTIONS
#include "glcorert.h"

#ifdef _WINDOWS

#ifdef __cplusplus
extern "C" {
#endif

PFNGLGENQUERIESPROC              glGenQueries;
PFNGLACTIVETEXTUREPROC              glActiveTexture;
PFNGLGENERATEMIPMAPPROC              glGenerateMipmap;
PFNGLDRAWBUFFERSPROC              glDrawBuffers;
PFNGLDELETEQUERIESPROC              glDeleteQueries;
PFNGLENDQUERYPROC              glEndQuery;
PFNGLBEGINQUERYPROC              glBeginQuery;
PFNGLGETQUERYOBJECTUI64VPROC              glGetQueryObjectui64v;

void initializeOpenGLFunctions() {
	glGenQueries = (PFNGLGENQUERIESPROC)OpenGLHelpers::getExtensionFunction("glGenQueries");
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)OpenGLHelpers::getExtensionFunction("glActiveTexture");
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)OpenGLHelpers::getExtensionFunction("glGenerateMipmap");
	glDrawBuffers = (PFNGLDRAWBUFFERSPROC)OpenGLHelpers::getExtensionFunction("glDrawBuffers");
	glDeleteQueries = (PFNGLDELETEQUERIESPROC)OpenGLHelpers::getExtensionFunction("glDeleteQueries");
	glEndQuery = (PFNGLENDQUERYPROC)OpenGLHelpers::getExtensionFunction("glEndQuery");
	glBeginQuery = (PFNGLBEGINQUERYPROC)OpenGLHelpers::getExtensionFunction("glBeginQuery");
	glGetQueryObjectui64v = (PFNGLGETQUERYOBJECTUI64VPROC)OpenGLHelpers::getExtensionFunction("glGetQueryObjectui64v");
}

#ifdef __cplusplus
}
#endif

#endif