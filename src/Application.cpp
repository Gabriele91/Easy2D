#include <stdafx.h>
#include <Debug.h>
#include <Math3D.h>
#include <Application.h>
#include <ETime.h>
#if defined( PLATFORM_IOS )
#elif defined( PLATFORM_OSX )
    #include <CocoaApp.h>
#elif defined( PLATFORM_WINDOW )
    #include <WindowsApp.h>
#elif defined( PLATFORM_LINUX )
    #include <LinuxApp.h>
#elif defined( PLATFORM_ANDROID )
    #include <AndroidApp.h>
#elif defined( PLATFORM_EMSCRIPTEN )
    #include <EmscriptenApp.h>
#endif
///////////////////////
using namespace Easy2D;
///////////////////////
Application *Application::appSingleton=NULL;
///////////////////////
Application::Application()
	:lastDeltaTime(0.0f)
    ,mainInstance(NULL)
	,screen(NULL)
	,input(NULL)
	,audio(NULL){

}
Application::~Application(){
	appSingleton=NULL;	
}

Application *Application::create(){

	DEBUG_ASSERT(!appSingleton);
	
	Math::seedRandom((uint)GetTime());

#if defined( PLATFORM_IOS )
#elif defined( PLATFORM_OSX )    
	appSingleton=new CocoaApp();
#elif defined( PLATFORM_WINDOW )
	appSingleton=new WindowsApp();
#elif defined( PLATFORM_LINUX )
	appSingleton=new LinuxApp();
#elif defined( PLATFORM_ANDROID )
	appSingleton=new AndroidApp();
#elif defined( PLATFORM_EMSCRIPTEN )
	appSingleton=new EmscriptenApp();
#endif
	//registration delete at exit
	atexit([](){ 
		delete Application::instance(); 
	});
	//
	return appSingleton;
}

Application *Application::instance(){
	return appSingleton;
}
