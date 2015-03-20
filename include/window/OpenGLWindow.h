#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H
#define GL_2_1_EXT_TO_STANDARD
//#define GL_VERSION_2_1
#include <gletx_header.h>

//vbo
extern PFNGLGENBUFFERSARBPROC glGenBuffers;
extern PFNGLBINDBUFFERARBPROC glBindBuffer;
extern PFNGLBUFFERSUBDATAARBPROC glBufferSubData;
extern PFNGLBUFFERDATAARBPROC glBufferData;
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffers;
//edit vbo
extern PFNGLMAPBUFFERARBPROC glMapBuffer;
extern PFNGLUNMAPBUFFERARBPROC glUnmapBuffer;
//vba
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC  glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
//multi texture
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture;
extern PFNGLDRAWRANGEELEMENTSEXTPROC glDrawRangeElements;
//blend effect
extern PFNGLBLENDEQUATIONPROC		glBlendEquation;
//vsync
typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC)(int interval);
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
//FBO
extern PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT ;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT ;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT ;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT ;
//RBO
extern PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT;
extern PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT;
extern PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT;
//Blend
extern PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;
//shader
extern PFNGLENABLEVERTEXATTRIBARRAYPROC	    glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC	        glVertexAttribPointer;
extern PFNGLCREATESHADERPROC                glCreateShader;
extern PFNGLDELETESHADERPROC				glDeleteShader;
extern PFNGLSHADERSOURCEPROC                glShaderSource;
extern PFNGLCOMPILESHADERPROC               glCompileShader;
extern PFNGLCREATEPROGRAMPROC               glCreateProgram;
extern PFNGLDELETEPROGRAMPROC               glDeleteProgram;
extern PFNGLATTACHSHADERPROC                glAttachShader;
extern PFNGLDETACHSHADERPROC				glDetachShader;
extern PFNGLLINKPROGRAMPROC                 glLinkProgram;
extern PFNGLUSEPROGRAMPROC                  glUseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC          glGetUniformLocation;
extern PFNGLBINDATTRIBLOCATIONPROC     		glBindAttribLocation;
extern PFNGLUNIFORM1FPROC                   glUniform1f;
extern PFNGLUNIFORM2FPROC                   glUniform2f;
extern PFNGLUNIFORM3FPROC                   glUniform3f;
extern PFNGLUNIFORM4FPROC                   glUniform4f;
extern PFNGLUNIFORM1IPROC                   glUniform1i;
extern PFNGLUNIFORM1IVPROC                  glUniform1iv;
extern PFNGLUNIFORM2IVPROC                  glUniform2iv;
extern PFNGLUNIFORM3IVPROC                  glUniform3iv;
extern PFNGLUNIFORM4IVPROC				    glUniform4iv;
extern PFNGLUNIFORM1FVPROC                  glUniform1fv;
extern PFNGLUNIFORM2FVPROC                  glUniform2fv;
extern PFNGLUNIFORM3FVPROC                  glUniform3fv;
extern PFNGLUNIFORM4FVPROC				    glUniform4fv;
extern PFNGLUNIFORMMATRIX4FVPROC		    glUniformMatrix4fv;
//Info Log
extern PFNGLGETSHADERINFOLOGPROC     glGetShaderInfoLog;
extern PFNGLGETSHADERIVPROC			 glGetShaderiv;
extern PFNGLGETPROGRAMIVPROC		 glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC	 glGetProgramInfoLog;

namespace Easy2D
{
//init openGL2
extern void initOpenGL2();
};


#ifdef GL_2_1_EXT_TO_STANDARD
	//VSYNC
	#define wglSwapInterval wglSwapIntervalEXT
	//FBO
	#define glGenFramebuffers glGenFramebuffersEXT  
	#define glDeleteFramebuffers glDeleteFramebuffersEXT
	#define glBindFramebuffer glBindFramebufferEXT
	#define glFramebufferTexture2D glFramebufferTexture2DEXT
	#define glCheckFramebufferStatus glCheckFramebufferStatusEXT
	//RBO
	#define glGenRenderbuffers glGenRenderbuffersEXT
	#define glDeleteRenderbuffers glDeleteRenderbuffersEXT
	#define glBindRenderbuffer glBindRenderbufferEXT
	#define glRenderbufferStorage glRenderbufferStorageEXT
	#define glFramebufferRenderbuffer glFramebufferRenderbufferEXT

	//defines //Not necesary GL_DEPTH_COMPONENT32 GL_DEPTH_COMPONENT32_EXT
	#ifndef GL_RENDERBUFFER
		#define GL_RENDERBUFFER GL_RENDERBUFFER_EXT
	#endif
	#ifndef GL_DEPTH_ATTACHMENT
		#define GL_DEPTH_ATTACHMENT GL_DEPTH_ATTACHMENT_EXT
	#endif
	#ifndef GL_COLOR_ATTACHMENT0
		#define GL_COLOR_ATTACHMENT0 GL_COLOR_ATTACHMENT0_EXT
	#endif
	#ifndef GL_FRAMEBUFFER
		#define GL_FRAMEBUFFER GL_FRAMEBUFFER_EXT
	#endif
#endif

#endif