#include <stdafx.h>
#include <Application.h>
#if defined( PLATFORM_IOS ) 
#elif defined( PLATFORM_OSX )
#elif defined( PLATFORM_WINDOW )
#include <WindowsApp.h>
#elif defined( PLATFORM_LINUX )
#elif defined( PLATFORM_ANDROID )
#endif
///////////////////////
using namespace Easy2D;
///////////////////////
Application *Application::appSingleton=NULL;
///////////////////////
Application::Application()
	:game(NULL)	
	,screen(NULL)
	,input(NULL)
	,audio(NULL){

}

Application::~Application(){

}

Application *Application::create(){	

	DEBUG_ASSERT(!appSingleton);

#if defined( PLATFORM_IOS ) 
#elif defined( PLATFORM_OSX )
#elif defined( PLATFORM_WINDOW )
	appSingleton=new WindowsApp();
#elif defined( PLATFORM_LINUX )
#elif defined( PLATFORM_ANDROID )
#endif

	return appSingleton;
}

Application *Application::instance(){
	return appSingleton;
}
