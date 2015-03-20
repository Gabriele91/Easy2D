#include <stdafx.h>
#include <EGL/egl.h>
#include <OpenGLAngle.h>
#include <Debug.h>

#ifdef EXT_OES
	PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOES=NULL;
	PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOES=NULL;
	PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOES=NULL;
#else
namespace Easy2D
{
	void glGenVertexArrays(GLsizei n, GLuint *arrays)
	{
		Debug::message() << "AngleProject does not implement glGenVertexArraysOES";
	}

	void glBindVertexArray(GLuint array)
	{
		Debug::message() <<  "AngleProject does not implement glBindVertexArrayOES";
	}

	void glDeleteVertexArrays(GLsizei n, const GLuint *arrays)
	{
		Debug::message() <<  "AngleProject does not implement glDeleteVertexArraysOES";
	}

	void* glMapBuffer(GLenum target, GLenum access)
	{
	   Debug::message() <<  "AngleProject does not implement glMapBufferOES"; 
	   return NULL;
	}

	GLboolean glUnmapBuffer(GLenum targets)
	{
	   Debug::message() <<  "AngleProject does not implement glUnmapBufferOES"; 
	   return false;
	}
};
#endif

void Easy2D::initOpenGLES()
{

    DEBUG_MESSAGE("Setup OpenGLES");
    DEBUG_MESSAGE("Vendor:" << glGetString(GL_VENDOR)  );
    DEBUG_MESSAGE("Renderer:" << glGetString(GL_RENDERER));
    DEBUG_MESSAGE("OpenGL Version: " << glGetString(GL_VERSION) );
#ifdef EXT_OES
    glGenVertexArraysOES=(PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
    DEBUG_ASSERT(glGenVertexArraysOES);

    glBindVertexArrayOES=(PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES");
    DEBUG_ASSERT(glBindVertexArrayOES);

    glDeleteVertexArraysOES=(PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArraysOES");
    DEBUG_ASSERT(glDeleteVertexArraysOES);
#endif
}
