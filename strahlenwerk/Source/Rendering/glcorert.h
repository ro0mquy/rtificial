#ifndef __glcorert_h_
#define __glcorert_h_ 1

#ifdef __linux
#include "glcorearb.h"
#endif

#ifdef _WINDOWS

//#define NOMINMAX // http://support.microsoft.com/kb/143208
#include <juce>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#define GLAPI extern
#endif

#define GL_DRAW_FRAMEBUFFER               0x8CA9
#define GL_TEXTURE_LOD_BIAS               0x8501
#define GL_RG                             0x8227
#define GL_RG_INTEGER                     0x8228
#define GL_R8                             0x8229
#define GL_R16                            0x822A
#define GL_RG8                            0x822B
#define GL_RG16                           0x822C
#define GL_R16F                           0x822D
#define GL_R32F                           0x822E
#define GL_RG16F                          0x822F
#define GL_RG32F                          0x8230
#define GL_R8I                            0x8231
#define GL_R8UI                           0x8232
#define GL_R16I                           0x8233
#define GL_R16UI                          0x8234
#define GL_R32I                           0x8235
#define GL_R32UI                          0x8236
#define GL_RG8I                           0x8237
#define GL_RG8UI                          0x8238
#define GL_RG16I                          0x8239
#define GL_RG16UI                         0x823A
#define GL_RG32I                          0x823B
#define GL_RG32UI                         0x823C
#define GL_RGBA16F                        0x881A
#define GL_RGB16F                         0x881B
#define GL_TIME_ELAPSED                   0x88BF
#define GL_FRAMEBUFFER_SRGB               0x8DB9
#define GL_QUERY_RESULT                   0x8866
#define GL_TEXTURE_MAX_LEVEL              0x813D

#ifndef GLEXT_64_TYPES_DEFINED
	/* This code block is duplicated in glxext.h, so must be protected */
#define GLEXT_64_TYPES_DEFINED
	/* Define int32_t, int64_t, and uint64_t types for UST/MSC */
	/* (as used in the GL_EXT_timer_query extension). */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#include <inttypes.h>
#elif defined(__sun__) || defined(__digital__)
#include <inttypes.h>
#if defined(__STDC__)
#if defined(__arch64__) || defined(_LP64)
	typedef long int int64_t;
	typedef unsigned long int uint64_t;
#else
	typedef long long int int64_t;
	typedef unsigned long long int uint64_t;
#endif /* __arch64__ */
#endif /* __STDC__ */
#elif defined( __VMS ) || defined(__sgi)
#include <inttypes.h>
#elif defined(__SCO__) || defined(__USLC__)
#include <stdint.h>
#elif defined(__UNIXOS2__) || defined(__SOL64__)
	typedef long int int32_t;
	typedef long long int int64_t;
	typedef unsigned long long int uint64_t;
#elif defined(_WIN32) && defined(__GNUC__)
#include <stdint.h>
#elif defined(_WIN32)
	typedef __int32 int32_t;
	typedef __int64 int64_t;
	typedef unsigned __int64 uint64_t;
#else
	/* Fallback if nothing above works */
#include <inttypes.h>
#endif
#endif
typedef uint64_t GLuint64;

typedef void (APIENTRYP PFNGLGENQUERIESPROC) (GLsizei n, GLuint *ids);
typedef void (APIENTRYP PFNGLACTIVETEXTUREPROC) (GLenum texture);
typedef void (APIENTRYP PFNGLGENERATEMIPMAPPROC) (GLenum target);
typedef void (APIENTRYP PFNGLDRAWBUFFERSPROC) (GLsizei n, const GLenum *bufs);
typedef void (APIENTRYP PFNGLDELETEQUERIESPROC) (GLsizei n, const GLuint *ids);
typedef void (APIENTRYP PFNGLENDQUERYPROC) (GLenum target);
typedef void (APIENTRYP PFNGLBEGINQUERYPROC) (GLenum target, GLuint id);
typedef void (APIENTRYP PFNGLGETQUERYOBJECTUI64VPROC) (GLuint id, GLenum pname, GLuint64 *params);

#ifdef GL_GLEXT_PROTOTYPES
/*
GLAPI void APIENTRY glGenQueries(GLsizei n, GLuint *ids);
GLAPI void APIENTRY glActiveTexture(GLenum texture);
GLAPI void APIENTRY glGenerateMipmap(GLenum target);
GLAPI void APIENTRY glDrawBuffers(GLsizei n, const GLenum *bufs);
GLAPI void APIENTRY glDeleteQueries(GLsizei n, const GLuint *ids);
GLAPI void APIENTRY glEndQuery(GLenum target);
GLAPI void APIENTRY glBeginQuery(GLenum target, GLuint id);
GLAPI void APIENTRY glGetQueryObjectui64v(GLuint id, GLenum pname, GLuint64 *params);
*/

extern PFNGLGENQUERIESPROC              glGenQueries;
extern PFNGLACTIVETEXTUREPROC              glActiveTexture;
extern PFNGLGENERATEMIPMAPPROC              glGenerateMipmap;
extern PFNGLDRAWBUFFERSPROC              glDrawBuffers;
extern PFNGLDELETEQUERIESPROC              glDeleteQueries;
extern PFNGLENDQUERYPROC              glEndQuery;
extern PFNGLBEGINQUERYPROC              glBeginQuery;
extern PFNGLGETQUERYOBJECTUI64VPROC              glGetQueryObjectui64v;

#endif

void initializeOpenGLFunctions();

#ifdef __cplusplus
}
#endif

#endif // _WINDOWS

#endif // __glcorert_h_
