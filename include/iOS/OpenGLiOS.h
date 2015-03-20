#ifndef OPENGLIOS_H
#define OPENGLIOS_H


#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#define  OPENGL_ES2

#ifndef GL_STACK_OVERFLOW
    #define GL_STACK_OVERFLOW 0xffffff
#endif

#ifndef GL_STACK_UNDERFLOW
    #define GL_STACK_UNDERFLOW 0xfffffe
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
#ifndef GL_ALPHA8  
    //I know, is dangerous,
    //but is only a fake constant
    //per image.h
    #define GL_ALPHA8 GL_ALPHA
#endif

#define glGenVertexArrays glGenVertexArraysOES
#define glBindVertexArray glBindVertexArrayOES
#define glDeleteVertexArrays glDeleteVertexArraysOES



#endif