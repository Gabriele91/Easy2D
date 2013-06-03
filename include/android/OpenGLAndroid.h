#ifndef OPENGLANDROID_H
#define OPENGLANDROID_H

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>

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

extern PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOESEXT;
extern PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOESEXT;
extern PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOESEXT;

namespace Easy2D {
	//init openGLES
	extern void initOpenGLES();
};

#endif