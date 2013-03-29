#include <stdafx.h>
#include <Debug.h>
#include <iostream>
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
void Debug::doassert(int v,const char* value,const char* fileSource,int line){
	if(!v){
		message()<<value<<":"<<line<<":"<<fileSource<<"\n";
		breakpoint();
	}
}
