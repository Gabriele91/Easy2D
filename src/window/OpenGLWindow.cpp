#include <stdafx.h>
#include <OpenGLWindow.h>
#include <Debug.h>

//vbo
PFNGLGENBUFFERSARBPROC glGenBuffers=NULL;
PFNGLBINDBUFFERARBPROC glBindBuffer=NULL;
PFNGLBUFFERSUBDATAARBPROC glBufferSubData=NULL;
PFNGLBUFFERDATAARBPROC glBufferData=NULL;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffers=NULL;
//edit vbo
PFNGLMAPBUFFERARBPROC glMapBuffer=NULL;
PFNGLUNMAPBUFFERARBPROC glUnmapBuffer=NULL;
//vba
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays=NULL;
PFNGLBINDVERTEXARRAYPROC  glBindVertexArray=NULL;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays=NULL;
//multi texture
PFNGLACTIVETEXTUREPROC glActiveTexture=NULL;
PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture=NULL;
PFNGLDRAWRANGEELEMENTSEXTPROC glDrawRangeElements=NULL;
//blend effect
PFNGLBLENDEQUATIONPROC glBlendEquation=NULL;
//vsync
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT=NULL;
//FBO
PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT =NULL;
PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT=NULL;
PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT =NULL;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT =NULL;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT =NULL;
//RBO
PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT =NULL;
PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT =NULL;
PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT =NULL;
PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT =NULL;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT =NULL;
//Blend
PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate =NULL;
//shader
PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray=NULL;
PFNGLVERTEXATTRIBPOINTERPROC       glVertexAttribPointer=NULL;
PFNGLCREATESHADERPROC	glCreateShader=NULL;
PFNGLDELETESHADERPROC	glDeleteShader=NULL;
PFNGLSHADERSOURCEPROC	glShaderSource=NULL;
PFNGLCOMPILESHADERPROC	glCompileShader=NULL;
PFNGLCREATEPROGRAMPROC	glCreateProgram=NULL;
PFNGLDELETEPROGRAMPROC	glDeleteProgram=NULL;
PFNGLATTACHSHADERPROC	glAttachShader=NULL;
PFNGLDETACHSHADERPROC	glDetachShader=NULL;
PFNGLLINKPROGRAMPROC	glLinkProgram=NULL;
PFNGLUSEPROGRAMPROC		glUseProgram=NULL;
PFNGLGETUNIFORMLOCATIONPROC	glGetUniformLocation=NULL;
PFNGLBINDATTRIBLOCATIONPROC	glBindAttribLocation=NULL;
PFNGLUNIFORM1FPROC	glUniform1f=NULL;
PFNGLUNIFORM2FPROC	glUniform2f=NULL;
PFNGLUNIFORM3FPROC	glUniform3f=NULL;
PFNGLUNIFORM4FPROC	glUniform4f=NULL;
PFNGLUNIFORM1IPROC	glUniform1i=NULL;
PFNGLUNIFORM1IVPROC	glUniform1iv=NULL;
PFNGLUNIFORM2IVPROC	glUniform2iv=NULL;
PFNGLUNIFORM3IVPROC	glUniform3iv=NULL;
PFNGLUNIFORM4IVPROC	glUniform4iv=NULL;
PFNGLUNIFORM1FVPROC	glUniform1fv=NULL;
PFNGLUNIFORM2FVPROC	glUniform2fv=NULL;
PFNGLUNIFORM3FVPROC	glUniform3fv=NULL;
PFNGLUNIFORM4FVPROC	glUniform4fv=NULL;
PFNGLUNIFORMMATRIX4FVPROC	glUniformMatrix4fv=NULL;
//Info Log
PFNGLGETSHADERINFOLOGPROC	glGetShaderInfoLog=NULL;
PFNGLGETSHADERIVPROC	glGetShaderiv=NULL;
PFNGLGETPROGRAMIVPROC	glGetProgramiv=NULL;
PFNGLGETPROGRAMINFOLOGPROC	glGetProgramInfoLog=NULL;
PFNGLGETACTIVEUNIFORMPROC	glGetActiveUniform=NULL;

void Easy2D::initOpenGL2()
{
    //out debug
    DEBUG_MESSAGE("Setup OpenGL2");
    DEBUG_MESSAGE("Vendor:" << glGetString(GL_VENDOR)  );
    DEBUG_MESSAGE("Renderer:" << glGetString(GL_RENDERER));
    DEBUG_MESSAGE("OpenGL Version: " << glGetString(GL_VERSION) );
    DEBUG_MESSAGE("GLSL Version:" << glGetString(GL_SHADING_LANGUAGE_VERSION) );
    //VBO / edit vbo
    glEnableVertexAttribArray=(PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
    DEBUG_ASSERT(glEnableVertexAttribArray);
    glVertexAttribPointer=(PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
    DEBUG_ASSERT(glVertexAttribPointer);
    glGenBuffers=(PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffers");
    DEBUG_ASSERT(glGenBuffers);
    glBindBuffer=(PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBuffer");
    DEBUG_ASSERT(glBindBuffer);
    glBufferData=(PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferData");
    DEBUG_ASSERT(glBufferData);
    glDeleteBuffers=(PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffers");
    DEBUG_ASSERT(glDeleteBuffers);
    glBufferSubData=(PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubData");
    DEBUG_ASSERT(glBufferSubData);
    glMapBuffer=(PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBuffer");
    DEBUG_ASSERT(glMapBuffer);
    glUnmapBuffer=(PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBuffer");
    DEBUG_ASSERT(glUnmapBuffer);
    //vba
    glGenVertexArrays=(PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
    DEBUG_ASSERT(glGenVertexArrays);
    glBindVertexArray=(PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
    DEBUG_ASSERT(glBindVertexArray);
    glDeleteVertexArrays=(PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
    DEBUG_ASSERT(glDeleteVertexArrays);
    //multi texture
    glActiveTexture=(PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
    glClientActiveTexture=(PFNGLCLIENTACTIVETEXTUREPROC)wglGetProcAddress("glClientActiveTexture");
    glDrawRangeElements=(PFNGLDRAWRANGEELEMENTSEXTPROC)wglGetProcAddress("glDrawRangeElementsEXT");
    //blend effect
    glBlendEquation = (PFNGLBLENDEQUATIONPROC)wglGetProcAddress("glBlendEquation");
    //vsync //wglGetProcAddress
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");
    if ( wglSwapIntervalEXT )
        wglSwapIntervalEXT(0); //nVidia
    //FBO
    glGenFramebuffersEXT		= (PFNGLGENFRAMEBUFFERSEXTPROC)		  wglGetProcAddress("glGenFramebuffersEXT");
    DEBUG_ASSERT(glGenFramebuffersEXT);
    glDeleteFramebuffersEXT    = (PFNGLDELETEFRAMEBUFFERSEXTPROC)    wglGetProcAddress("glDeleteFramebuffersEXT");
    DEBUG_ASSERT(glDeleteFramebuffersEXT);
    glBindFramebufferEXT		= (PFNGLBINDFRAMEBUFFEREXTPROC)		  wglGetProcAddress("glBindFramebufferEXT");
    DEBUG_ASSERT(glBindFramebufferEXT);
    glFramebufferTexture2DEXT	= (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)  wglGetProcAddress("glFramebufferTexture2DEXT");
    DEBUG_ASSERT(glFramebufferTexture2DEXT);
    glCheckFramebufferStatusEXT	= (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
    DEBUG_ASSERT(glCheckFramebufferStatusEXT);
    //RBO
    glGenRenderbuffersEXT		= (PFNGLGENRENDERBUFFERSEXTPROC)		  wglGetProcAddress("glGenRenderbuffersEXT");
    DEBUG_ASSERT(glGenRenderbuffersEXT);
    glDeleteRenderbuffersEXT    = (PFNGLDELETERENDERBUFFERSEXTPROC)    wglGetProcAddress("glDeleteRenderbuffersEXT");
    DEBUG_ASSERT(glDeleteRenderbuffersEXT);
    glBindRenderbufferEXT		= (PFNGLBINDRENDERBUFFEREXTPROC)		  wglGetProcAddress("glBindRenderbufferEXT");
    DEBUG_ASSERT(glBindRenderbufferEXT);
    glRenderbufferStorageEXT	= (PFNGLRENDERBUFFERSTORAGEEXTPROC)  wglGetProcAddress("glRenderbufferStorageEXT");
    DEBUG_ASSERT(glRenderbufferStorageEXT);
    glFramebufferRenderbufferEXT= (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
    DEBUG_ASSERT(glFramebufferRenderbufferEXT);
	//Blend
	glBlendFuncSeparate=(PFNGLBLENDFUNCSEPARATEPROC) wglGetProcAddress("glBlendFuncSeparate");
	DEBUG_ASSERT(glBlendFuncSeparate);
    //shader
    glCreateShader=(PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
    DEBUG_ASSERT(glCreateShader);
    glDeleteShader=(PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
    DEBUG_ASSERT(glDeleteShader);
    glShaderSource=(PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
    DEBUG_ASSERT(glShaderSource);

    glCompileShader=(PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
    DEBUG_ASSERT(glCompileShader);
    glCreateProgram=(PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
    DEBUG_ASSERT(glCreateProgram);
    glDeleteProgram=(PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
    DEBUG_ASSERT(glDeleteProgram);
    glAttachShader=(PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
    DEBUG_ASSERT(glAttachShader);
    glDetachShader=(PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
    DEBUG_ASSERT(glDetachShader);
    glLinkProgram=(PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
    DEBUG_ASSERT(glLinkProgram);

    glUseProgram=(PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
    glGetUniformLocation=(PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
    DEBUG_ASSERT(glGetUniformLocation);
    glBindAttribLocation=(PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
    DEBUG_ASSERT(glBindAttribLocation);
    glUniform1f=(PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
    DEBUG_ASSERT(glUniform1f);
    glUniform2f=(PFNGLUNIFORM2FPROC)wglGetProcAddress("glUniform2f");
    DEBUG_ASSERT(glUniform2f);
    glUniform3f=(PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
    DEBUG_ASSERT(glUniform3f);
    glUniform4f=(PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");
    DEBUG_ASSERT(glUniform4f);
    glUniform1i=(PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
    DEBUG_ASSERT(glUniform1i);
    glUniform1iv=(PFNGLUNIFORM1IVPROC)wglGetProcAddress("glUniform1iv");
    DEBUG_ASSERT(glUniform1iv);
    glUniform2iv=(PFNGLUNIFORM2IVPROC)wglGetProcAddress("glUniform2iv");
    DEBUG_ASSERT(glUniform2iv);
    glUniform3iv=(PFNGLUNIFORM3IVPROC)wglGetProcAddress("glUniform3iv");
    DEBUG_ASSERT(glUniform3iv);
    glUniform4iv=(PFNGLUNIFORM4IVPROC)wglGetProcAddress("glUniform4iv");
    DEBUG_ASSERT(glUniform4iv);
    glUniform1fv=(PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");
    DEBUG_ASSERT(glUniform1fv);
    glUniform2fv=(PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");
    DEBUG_ASSERT(glUniform2fv);
    glUniform3fv=(PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
    DEBUG_ASSERT(glUniform3fv);
    glUniform4fv=(PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
    DEBUG_ASSERT(glUniform4fv);
    glUniformMatrix4fv=(PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
    DEBUG_ASSERT(glUniformMatrix4fv);
    //log compilatore shader
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
    DEBUG_ASSERT(glGetShaderInfoLog);
    glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
    DEBUG_ASSERT(glGetShaderiv);
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
    DEBUG_ASSERT(glGetProgramiv);
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
    DEBUG_ASSERT(glGetProgramInfoLog);
    glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)wglGetProcAddress("glGetActiveUniform");
    DEBUG_ASSERT(glGetActiveUniform);

}