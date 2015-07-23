#ifndef OPENGLANDROID_H
#define OPENGLANDROID_H

#include <EGL/egl.h>

#ifdef ANDROID_GLES1

    #define  OPENGL_ES1

    #include <GLES/gl.h>
    #include <GLES/glext.h>

    #define glGenRenderbuffers                          glGenRenderbuffersOES
    #define glGenFramebuffers                           glGenFramebuffersOES
    #define glFramebufferRenderbuffer           glFramebufferRenderbufferOES
    #define glDeleteRenderbuffers               glDeleteRenderbuffersOES
    #define glDeleteFramebuffers                glDeleteFramebuffersOES
    #define glBindRenderbuffer                          glBindRenderbufferOES
    #define glBindFramebuffer                           glBindFramebufferOES
    #define glGetRenderbufferParameteriv    glGetRenderbufferParameterivOES
    #define glCheckFramebufferStatus            glCheckFramebufferStatusOES
    #define glBlendEquation                 glBlendEquationOES

    #define glColor4fv(a) glColor4f(a[0], a[1], a[2], a[3])
    #define glColor3fv(a) glColor4f(a[0], a[1], a[2], 1.0f)
    #define glColor3f(a,b,c) glColor4f(a, b, c, 1.0f)
    /* stream buffer */
    #if !defined( GL_STREAM_DRAW )
    #ifdef ANDROID_FORCE_STREAM_BUFFER
        #ifndef GL_DYNAMIC_DRAW
        #error "Can't force stream buffer"
    #endif
    #define GL_STREAM_DRAW GL_DYNAMIC_DRAW
        #define ENABLE_STREAM_BUFFER
    #else
        #define DISABLE_STREAM_BUFFER
    #endif
    #endif

#else

    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
    #define  OPENGL_ES2

    #ifndef GL_STACK_OVERFLOW
        #define GL_STACK_OVERFLOW ((GLint)0xffffffff)
    #endif

    #ifndef GL_STACK_UNDERFLOW
        #define GL_STACK_UNDERFLOW ((GLint)0xfffffffe)
    #endif

#endif

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


#define glBindVertexArray           glBindVertexArrayOESEXT
#define glGenVertexArrays           glGenVertexArraysOESEXT
#define glDeleteVertexArrays        glDeleteVertexArraysOESEXT

extern PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOESEXT;
extern PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOESEXT;
extern PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOESEXT;


namespace Easy2D
{
//init openGLES
extern void initOpenGLES();
};

#endif