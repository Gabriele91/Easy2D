#ifndef OPENGLEM_H
#define OPENGLEM_H

#include <SDL/SDL_opengl.h>

#ifndef GL_NONE
#define GL_NONE EGL_NONE
#endif

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

#define glBindVertexArray           glBindVertexArrayOESEXT
#define glGenVertexArrays           glGenVertexArraysOESEXT
#define glDeleteVertexArrays        glDeleteVertexArraysOESEXT

#ifndef GL_ALPHA8
#define GL_ALPHA8 -1
#endif
#define glColor4fv(a) glColor4f(a[0], a[1], a[2], a[3])
#define glColor3fv(a) glColor4f(a[0], a[1], a[2], 1.0f)
#define glColor3f(a,b,c) glColor4f(a, b, c, 1.0f)

#endif
