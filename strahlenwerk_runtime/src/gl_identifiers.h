#ifndef GL_IDENTIFIERS_H
#define GL_IDENTIFIERS_H

#ifdef _WINDOWS
#	include <Windows.h>
#	include <GL/gl.h>
#	include "glcorearb.h"
#elif __linux
#	include <GL/gl.h>
#endif

#ifdef _WINDOWS // wtf
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
extern PFNGLUNIFORM1FPROC                glUniform1f;
extern PFNGLUNIFORM2FPROC                glUniform2f;
extern PFNGLUNIFORM3FPROC                glUniform3f;
extern PFNGLUNIFORM4FPROC                glUniform4f;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLDELETEPROGRAMPROC            glDeleteProgram;
extern PFNGLGETPROGRAMIVPROC             glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog;

#endif
