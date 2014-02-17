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

#define glColor4fv(a) glColor4f(a[0], a[1], a[2], a[3])
#define glColor3fv(a) glColor4f(a[0], a[1], a[2], 1.0f)
#define glColor3f(a,b,c) glColor4f(a, b, c, 1.0f)

extern PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOESEXT;
extern PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOESEXT;
extern PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOESEXT;

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

namespace Easy2D {
	//init openGLES
	extern void initOpenGLES();
};

#endif