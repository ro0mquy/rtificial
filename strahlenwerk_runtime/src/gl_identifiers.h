#ifndef GL_IDENTIFIERS_H
#define GL_IDENTIFIERS_H

#ifdef BUILD_WINDOWS
#	include <Windows.h>
#	include <GL/gl.h>
#	include "glcorearb.h"
#elif BUILD_LINUX
#	include <GL/gl.h>
#endif

#ifdef BUILD_WINDOWS // wtf
extern PFNGLACTIVETEXTUREPROC            glActiveTexture;
#endif
extern PFNGLGENBUFFERSPROC               glGenFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC          glBindFramebuffer;
extern PFNGLGENERATEMIPMAPPROC           glGenerateMipmap;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC     glFramebufferTexture2D;
extern PFNGLDRAWBUFFERSPROC              glDrawBuffers;
extern PFNGLDELETEFRAMEBUFFERSPROC       glDeleteFramebuffers;
extern PFNGLCREATESHADERPROC             glCreateShader;
extern PFNGLSHADERSOURCEPROC             glShaderSource;
extern PFNGLCOMPILESHADERPROC            glCompileShader;
extern PFNGLCREATEPROGRAMPROC            glCreateProgram;
extern PFNGLATTACHSHADERPROC             glAttachShader;
extern PFNGLDELETESHADERPROC             glDeleteShader;
extern PFNGLLINKPROGRAMPROC              glLinkProgram;
extern PFNGLUSEPROGRAMPROC               glUseProgram;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer;
extern PFNGLUNIFORM2FPROC                glUniform2f;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLDELETEPROGRAMPROC            glDeleteProgram;

#endif