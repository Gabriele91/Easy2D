#include <stdafx.h>
#include <OpenGLLinux.h>
#include <GL/glx.h>
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
#if !defined(__linux__)
//multi texture
PFNGLACTIVETEXTUREPROC glActiveTexture=NULL;
PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture=NULL;
PFNGLDRAWRANGEELEMENTSEXTPROC glDrawRangeElements=NULL;
//blend effect
PFNGLBLENDEQUATIONPROC glBlendEquation=NULL;
//vsync
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT=NULL;
#endif
//FBO
PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT =NULL;
PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT=NULL;
PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT =NULL;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT =NULL;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT =NULL;
//RBO
PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT=NULL;
PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT=NULL;
PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT=NULL;
PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT=NULL;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT=NULL;
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
PFNGLUNIFORM4UIPROC	glUniform4ui=NULL;
PFNGLUNIFORM4UIVPROC	glUniform4uiv=NULL;
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

void Easy2D::initOpenGL2()
{
    //Out debug
    DEBUG_MESSAGE("Setup OpenGL2");
    DEBUG_MESSAGE("Vendor:" << glGetString(GL_VENDOR)  );
    DEBUG_MESSAGE("Renderer:" << glGetString(GL_RENDERER));
    DEBUG_MESSAGE("OpenGL Version: " << glGetString(GL_VERSION) );
    DEBUG_MESSAGE("GLSL Version:" << glGetString(GL_SHADING_LANGUAGE_VERSION) );
    //VBO / edit vbo
    glEnableVertexAttribArray=(PFNGLENABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress((unsigned char *)"glEnableVertexAttribArray");
    DEBUG_ASSERT(glEnableVertexAttribArray);
    glVertexAttribPointer=(PFNGLVERTEXATTRIBPOINTERPROC)glXGetProcAddress((unsigned char *)"glVertexAttribPointer");
    DEBUG_ASSERT(glVertexAttribPointer);
    glGenBuffers=(PFNGLGENBUFFERSARBPROC)glXGetProcAddress((unsigned char *)"glGenBuffers");
    DEBUG_ASSERT(glGenBuffers);
    glBindBuffer=(PFNGLBINDBUFFERARBPROC)glXGetProcAddress((unsigned char *)"glBindBuffer");
    DEBUG_ASSERT(glBindBuffer);
    glBufferData=(PFNGLBUFFERDATAARBPROC)glXGetProcAddress((unsigned char *)"glBufferData");
    DEBUG_ASSERT(glBufferData);
    glDeleteBuffers=(PFNGLDELETEBUFFERSARBPROC)glXGetProcAddress((unsigned char *)"glDeleteBuffers");
    DEBUG_ASSERT(glDeleteBuffers);
    glBufferSubData=(PFNGLBUFFERSUBDATAARBPROC)glXGetProcAddress((unsigned char *)"glBufferSubData");
    DEBUG_ASSERT(glBufferSubData);
    glMapBuffer=(PFNGLMAPBUFFERARBPROC)glXGetProcAddress((unsigned char *)"glMapBuffer");
    DEBUG_ASSERT(glMapBuffer);
    glUnmapBuffer=(PFNGLUNMAPBUFFERARBPROC)glXGetProcAddress((unsigned char *)"glUnmapBuffer");
    DEBUG_ASSERT(glUnmapBuffer);
    //vba
    glGenVertexArrays=(PFNGLGENVERTEXARRAYSPROC)glXGetProcAddress((unsigned char *)"glGenVertexArrays");
    DEBUG_ASSERT(glGenVertexArrays);
    glBindVertexArray=(PFNGLBINDVERTEXARRAYPROC)glXGetProcAddress((unsigned char *)"glBindVertexArray");
    DEBUG_ASSERT(glBindVertexArray);
    glDeleteVertexArrays=(PFNGLDELETEVERTEXARRAYSPROC)glXGetProcAddress((unsigned char *)"glDeleteVertexArrays");
    DEBUG_ASSERT(glDeleteVertexArrays);
#if !defined(__linux__)
    //multi texture
    glActiveTexture=(PFNGLACTIVETEXTUREPROC)glXGetProcAddress((unsigned char *)"glActiveTexture");
    glClientActiveTexture=(PFNGLCLIENTACTIVETEXTUREPROC)glXGetProcAddress((unsigned char *)"glClientActiveTexture");
    glDrawRangeElements=(PFNGLDRAWRANGEELEMENTSEXTPROC)glXGetProcAddress((unsigned char *)"glDrawRangeElementsEXT");
    //blend effect
    glBlendEquation = (PFNGLBLENDEQUATIONPROC)glXGetProcAddress((unsigned char *)"glBlendEquation");
    //vsync //glXGetProcAddress
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) glXGetProcAddress((unsigned char *)"wglSwapIntervalEXT");
    if ( wglSwapIntervalEXT )
        wglSwapIntervalEXT(0); //nVidia
#endif
    //FBO
    glGenFramebuffersEXT		= (PFNGLGENFRAMEBUFFERSEXTPROC)		  glXGetProcAddress((unsigned char *)"glGenFramebuffersEXT");
    DEBUG_ASSERT(glGenFramebuffersEXT);
    glDeleteFramebuffersEXT    = (PFNGLDELETEFRAMEBUFFERSEXTPROC)    glXGetProcAddress((unsigned char *)"glDeleteFramebuffersEXT");
    DEBUG_ASSERT(glDeleteFramebuffersEXT);
    glBindFramebufferEXT		= (PFNGLBINDFRAMEBUFFEREXTPROC)		  glXGetProcAddress((unsigned char *)"glBindFramebufferEXT");
    DEBUG_ASSERT(glBindFramebufferEXT);
    glFramebufferTexture2DEXT	= (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)  glXGetProcAddress((unsigned char *)"glFramebufferTexture2DEXT");
    DEBUG_ASSERT(glFramebufferTexture2DEXT);
    glCheckFramebufferStatusEXT	= (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)glXGetProcAddress((unsigned char *)"glCheckFramebufferStatusEXT");
    DEBUG_ASSERT(glCheckFramebufferStatusEXT);
    //RBO
    glGenRenderbuffersEXT		= (PFNGLGENRENDERBUFFERSEXTPROC)		  glXGetProcAddress((unsigned char *)"glGenRenderbuffersEXT");
    DEBUG_ASSERT(glGenRenderbuffersEXT);
    glDeleteRenderbuffersEXT    = (PFNGLDELETERENDERBUFFERSEXTPROC)    glXGetProcAddress((unsigned char *)"glDeleteRenderbuffersEXT");
    DEBUG_ASSERT(glDeleteRenderbuffersEXT);
    glBindRenderbufferEXT		= (PFNGLBINDRENDERBUFFEREXTPROC)		  glXGetProcAddress((unsigned char *)"glBindRenderbufferEXT");
    DEBUG_ASSERT(glBindRenderbufferEXT);
    glRenderbufferStorageEXT	= (PFNGLRENDERBUFFERSTORAGEEXTPROC)  glXGetProcAddress((unsigned char *)"glRenderbufferStorageEXT");
    DEBUG_ASSERT(glRenderbufferStorageEXT);
    glFramebufferRenderbufferEXT= (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)glXGetProcAddress((unsigned char *)"glFramebufferRenderbufferEXT");
    DEBUG_ASSERT(glFramebufferRenderbufferEXT);
    //shader
    glCreateShader=(PFNGLCREATESHADERPROC)glXGetProcAddress((unsigned char *)"glCreateShader");
    DEBUG_ASSERT(glCreateShader);
    glDeleteShader=(PFNGLDELETESHADERPROC)glXGetProcAddress((unsigned char *)"glDeleteShader");
    DEBUG_ASSERT(glDeleteShader);
    glShaderSource=(PFNGLSHADERSOURCEPROC)glXGetProcAddress((unsigned char *)"glShaderSource");
    DEBUG_ASSERT(glShaderSource);

    glCompileShader=(PFNGLCOMPILESHADERPROC)glXGetProcAddress((unsigned char *)"glCompileShader");
    DEBUG_ASSERT(glCompileShader);
    glCreateProgram=(PFNGLCREATEPROGRAMPROC)glXGetProcAddress((unsigned char *)"glCreateProgram");
    DEBUG_ASSERT(glCreateProgram);
    glDeleteProgram=(PFNGLDELETEPROGRAMPROC)glXGetProcAddress((unsigned char *)"glDeleteProgram");
    DEBUG_ASSERT(glDeleteProgram);
    glAttachShader=(PFNGLATTACHSHADERPROC)glXGetProcAddress((unsigned char *)"glAttachShader");
    DEBUG_ASSERT(glAttachShader);
    glDetachShader=(PFNGLDETACHSHADERPROC)glXGetProcAddress((unsigned char *)"glDetachShader");
    DEBUG_ASSERT(glDetachShader);
    glLinkProgram=(PFNGLLINKPROGRAMPROC)glXGetProcAddress((unsigned char *)"glLinkProgram");
    DEBUG_ASSERT(glLinkProgram);

    glUseProgram=(PFNGLUSEPROGRAMPROC)glXGetProcAddress((unsigned char *)"glUseProgram");
    glGetUniformLocation=(PFNGLGETUNIFORMLOCATIONPROC)glXGetProcAddress((unsigned char *)"glGetUniformLocation");
    DEBUG_ASSERT(glGetUniformLocation);
    glBindAttribLocation=(PFNGLBINDATTRIBLOCATIONPROC)glXGetProcAddress((unsigned char *)"glBindAttribLocation");
    DEBUG_ASSERT(glBindAttribLocation);
    glUniform1f=(PFNGLUNIFORM1FPROC)glXGetProcAddress((unsigned char *)"glUniform1f");
    DEBUG_ASSERT(glUniform1f);
    glUniform2f=(PFNGLUNIFORM2FPROC)glXGetProcAddress((unsigned char *)"glUniform2f");
    DEBUG_ASSERT(glUniform2f);
    glUniform3f=(PFNGLUNIFORM3FPROC)glXGetProcAddress((unsigned char *)"glUniform3f");
    DEBUG_ASSERT(glUniform3f);
    glUniform4f=(PFNGLUNIFORM4FPROC)glXGetProcAddress((unsigned char *)"glUniform4f");
    DEBUG_ASSERT(glUniform4f);
    glUniform1i=(PFNGLUNIFORM1IPROC)glXGetProcAddress((unsigned char *)"glUniform1i");
    DEBUG_ASSERT(glUniform1i);
    glUniform4ui=(PFNGLUNIFORM4UIPROC)glXGetProcAddress((unsigned char *)"glUniform4ui");
    DEBUG_ASSERT(glUniform4ui);
    glUniform4uiv=(PFNGLUNIFORM4UIVPROC)glXGetProcAddress((unsigned char *)"glUniform4uiv");
    DEBUG_ASSERT(glUniform4uiv);
    glUniform1iv=(PFNGLUNIFORM1IVPROC)glXGetProcAddress((unsigned char *)"glUniform1iv");
    DEBUG_ASSERT(glUniform1iv);
    glUniform2iv=(PFNGLUNIFORM2IVPROC)glXGetProcAddress((unsigned char *)"glUniform2iv");
    DEBUG_ASSERT(glUniform2iv);
    glUniform3iv=(PFNGLUNIFORM3IVPROC)glXGetProcAddress((unsigned char *)"glUniform3iv");
    DEBUG_ASSERT(glUniform3iv);
    glUniform4iv=(PFNGLUNIFORM4IVPROC)glXGetProcAddress((unsigned char *)"glUniform4iv");
    DEBUG_ASSERT(glUniform4iv);
    glUniform1fv=(PFNGLUNIFORM1FVPROC)glXGetProcAddress((unsigned char *)"glUniform1fv");
    DEBUG_ASSERT(glUniform1fv);
    glUniform2fv=(PFNGLUNIFORM2FVPROC)glXGetProcAddress((unsigned char *)"glUniform2fv");
    DEBUG_ASSERT(glUniform2fv);
    glUniform3fv=(PFNGLUNIFORM3FVPROC)glXGetProcAddress((unsigned char *)"glUniform3fv");
    DEBUG_ASSERT(glUniform3fv);
    glUniform4fv=(PFNGLUNIFORM4FVPROC)glXGetProcAddress((unsigned char *)"glUniform4fv");
    DEBUG_ASSERT(glUniform4fv);
    glUniformMatrix4fv=(PFNGLUNIFORMMATRIX4FVPROC)glXGetProcAddress((unsigned char *)"glUniformMatrix4fv");
    DEBUG_ASSERT(glUniformMatrix4fv);
    //log compilatore shader
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glXGetProcAddress((unsigned char *)"glGetShaderInfoLog");
    DEBUG_ASSERT(glGetShaderInfoLog);
    glGetShaderiv = (PFNGLGETSHADERIVPROC)glXGetProcAddress((unsigned char *)"glGetShaderiv");
    DEBUG_ASSERT(glGetShaderiv);
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)glXGetProcAddress((unsigned char *)"glGetProgramiv");
    DEBUG_ASSERT(glGetProgramiv);
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)glXGetProcAddress((unsigned char *)"glGetProgramInfoLog");
    DEBUG_ASSERT(glGetProgramInfoLog);

}
