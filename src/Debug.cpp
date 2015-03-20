#include <stdafx.h>
#include <Debug.h>
#include <EString.h>
#include <iostream>
#include <RenderContext.h>

#ifdef PLATFORM_UNIX
#include <signal.h>
#endif
///////////////////////
using namespace Easy2D;
void Debug::breakpoint()
{
#if defined( PLATFORM_IPHONE_SIMULATOR ) || defined( PLATFORM_OSX )
    __asm__("int $3");
#elif defined( PLATFORM_IPHONE )
    raise(SIGTRAP);
#elif defined( PLATFORM_WINDOW )
    DebugBreak();
#elif defined( PLATFORM_LINUX )
    raise( SIGTRAP );
#elif defined( PLATFORM_ANDROID )
    //https://groups.google.com/forum/#!msg/android-ndk/jZG9avVjDBY/22WaArngxqYJ
    //__asm__ ("bkpt 0");
    //TODO implement something
    raise( SIGTRAP );
#elif defined( PLATFORM_EMSCRIPTEN )
#else
#error unsupported platform
#endif
}
std::ostream&  Debug::message()
{
    return std::cout;
}
void Debug::gpucheckerrors(const char* fileSource,int line)
{

    String err;
    bool glerror=false;
	size_t count=0;

    for (GLint g = glGetError(); g != GL_NONE; g = glGetError())
    {
        glerror=true;
        switch(g)
        {
        case GL_NO_ERROR:
            return;
        case GL_INVALID_ENUM:
            err = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            err = "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            err = "GL_INVALID_OPERATION";
            break;
        case GL_STACK_OVERFLOW:
            err = "GL_STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            err = "GL_STACK_UNDERFLOW";
            break;
        case GL_OUT_OF_MEMORY:
            err = "GL_OUT_OF_MEMORY";
            break;
        };
        DEBUG_MESSAGE( "OpenGL Error : " << err <<" : "<< line <<" : "<< fileSource  );
		//safe output
		if(++count == 100) break;
    }

    if(glerror)
    {
        DEBUG_MESSAGE( "OpenGL Local state" );
        DEBUG_CODE(RenderContext::debugCurrentState());
        DEBUG_MESSAGE( "OpenGL Native state" );
        DEBUG_CODE(RenderContext::debugNativeState());
        DEBUG_ASSERT_MSG(0,"OpenGL encountered");
    }
}
