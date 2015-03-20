#ifndef OPENGLANGLE_H
#define OPENGLANGLE_H

#define OPENGL_ES
#define OPENGL_ES2
#define ENABLE_SHADER
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2platform.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define glClearDepth                glClearDepthf

#ifndef GL_DEPTH_COMPONENT32
    #define GL_DEPTH_COMPONENT32 GL_DEPTH_COMPONENT32_OES
#endif
#ifndef GL_DEPTH_COMPONENT24
    #define GL_DEPTH_COMPONENT24 GL_DEPTH_COMPONENT24_OES
#endif
#ifndef GL_DEPTH_COMPONENT16
    #define GL_DEPTH_COMPONENT16 GL_DEPTH_COMPONENT16_OES
#endif
#ifndef GL_NONE
    #define GL_NONE EGL_NONE
#endif

#ifdef EXT_OES

#define glDeleteVertexArrays        glDeleteVertexArraysOES
#define glGenVertexArrays           glGenVertexArraysOES
#define glBindVertexArray           glBindVertexArrayOES

extern PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOES;
extern PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOES;
extern PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOES;

#else
namespace Easy2D
{
	void glGenVertexArrays(GLsizei n, GLuint *arrays);
	void glBindVertexArray(GLuint array);
	void glDeleteVertexArrays(GLsizei n, const GLuint *arrays);
	void* glMapBuffer(GLenum target, GLenum access);
	GLboolean glUnmapBuffer(GLenum targets);
};

#endif

namespace Easy2D
{
//init openGLES
extern void initOpenGLES();
};

#endif