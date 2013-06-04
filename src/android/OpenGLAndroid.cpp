#include <EGL/egl.h>
#include <OpenGLAndroid.h>
#include <Debug.h>

PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOESEXT=NULL;
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOESEXT=NULL;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOESEXT=NULL;

void Easy2D::initOpenGLES(){     

	DEBUG_MESSAGE("Setup OpenGLES");
	DEBUG_MESSAGE("Vendor:" << glGetString(GL_VENDOR)  );
    DEBUG_MESSAGE("Renderer:" << glGetString(GL_RENDERER));
    DEBUG_MESSAGE("OpenGL Version: " << glGetString(GL_VERSION) );
	 
	
	glGenVertexArraysOESEXT=(PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
	DEBUG_ASSERT(glGenVertexArraysOESEXT);
	
	glBindVertexArrayOESEXT=(PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES");
	DEBUG_ASSERT(glBindVertexArrayOESEXT);
	
	glDeleteVertexArraysOESEXT=(PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArraysOES");
	DEBUG_ASSERT(glDeleteVertexArraysOESEXT);
	
}