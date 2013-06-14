#include <stdafx.h>
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
#endif
///////////////////////
using namespace Easy2D;
///////////////////////
Application *Application::appSingleton=NULL;
///////////////////////
Application::Application()
	:mainInstance(NULL)
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
