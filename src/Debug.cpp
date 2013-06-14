#include <stdafx.h>
#include <Debug.h>
#include <EString.h>
#include <iostream>

#ifdef PLATFORM_UNIX
    #include <signal.h>
#endif
///////////////////////
using namespace Easy2D;
void Debug::breakpoint(){
#if defined( PLATFORM_IOS ) || defined( PLATFORM_OSX )
	__asm__("int $3");
#elif defined( PLATFORM_WINDOW )
	DebugBreak();
#elif defined( PLATFORM_LINUX )
	raise( SIGTRAP );
#elif defined( PLATFORM_ANDROID )
	//https://groups.google.com/forum/#!msg/android-ndk/jZG9avVjDBY/22WaArngxqYJ
	//__asm__ ("bkpt 0");
	//TODO implement something
	raise( SIGTRAP );
#else
	#error unsupported platform
#endif
}
std::ostream&  Debug::message(){
	return std::cout;
}

void Debug::doassert(void* v,const char* value,const char* fileSource,int line){
	if(!v){
		message()<<"Assert : "<<value<<" : "<<line<<" : "<<fileSource<<"\n";
		breakpoint();
	}
}
void Debug::doassert(int v,const char* value,const char* fileSource,int line){
	if(!v){
		message()<<"Assert : "<<value<<" : "<<line<<" : "<<fileSource<<"\n";
		breakpoint();
	}
}

void Debug::gpucheckerrors(const char* fileSource,int line){

	String err;
	bool glerror=false;

    for (GLint g = glGetError(); g != GL_NONE; g = glGetError()) {
        glerror=true;
		switch(g)
		{
			case GL_NO_ERROR:
				return;
			case GL_INVALID_ENUM:           err = "GL_INVALID_ENUM";        break;
			case GL_INVALID_VALUE:          err = "GL_INVALID_VALUE";       break;
			case GL_INVALID_OPERATION:		err = "GL_INVALID_OPERATION";   break;
			case GL_STACK_OVERFLOW:         err = "GL_STACK_OVERFLOW";      break;
			case GL_STACK_UNDERFLOW:		err = "GL_STACK_UNDERFLOW";     break;
			case GL_OUT_OF_MEMORY:          err = "GL_OUT_OF_MEMORY";       break;
		};
		DEBUG_MESSAGE( "OpenGL Error : " << err <<" : "<< line <<" : "<< fileSource  );
	}

	if(glerror){
		DEBUG_ASSERT_MSG(0,"OpenGL encountered");
	}
}
