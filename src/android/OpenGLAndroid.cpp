#include <EGL/egl.h>
#include <OpenGLAndroid.h>
#include <Debug.h>

PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOESEXT=NULL;
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOESEXT=NULL;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOESEXT=NULL;

void Easy2D::initOpenGLES(){     

	DEBUG_MESSAGE("Setup OpenGLES");   
	
	glGenVertexArraysOESEXT=(PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
	DEBUG_ASSERT(glGenVertexArraysOESEXT);
	
	glBindVertexArrayOESEXT=(PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES");
	DEBUG_ASSERT(glBindVertexArrayOESEXT);
	
	glDeleteVertexArraysOESEXT=(PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArraysOES");
	DEBUG_ASSERT(glDeleteVertexArraysOESEXT);
	
}