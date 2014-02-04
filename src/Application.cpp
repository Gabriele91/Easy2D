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

Application *Application::create(const String& name){

	DEBUG_ASSERT(!appSingleton);
	
	Math::seedRandom((uint)GetTime());

#if defined( PLATFORM_IOS )
#elif defined( PLATFORM_OSX )    
	appSingleton=new CocoaApp(name);
#elif defined( PLATFORM_WINDOW )
	appSingleton=new WindowsApp(name);
#elif defined( PLATFORM_LINUX )
	appSingleton=new LinuxApp(name);
#elif defined( PLATFORM_ANDROID )
	appSingleton=new AndroidApp(name);
#elif defined( PLATFORM_EMSCRIPTEN )
	appSingleton=new EmscriptenApp(name);
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

/**
* save a resourcesGroup
*/
void Application::subscriptionResourcesGroup(const String& name,ResourcesGroup *rsGr){
	DEBUG_ASSERT(rsGr);
	groups[name]=rsGr;
}
/**
* get a resourcesGroup
*/
ResourcesGroup* Application::getResourcesGroup(const String& name){
	auto it=groups.find(name);
	if(it!=groups.end()) return it->second;
	return NULL;
}
/**
* erase a resourcesGroup
*/
void Application::unsubscriptionResourcesGroup(const String& name){
	auto it=groups.find(name);
	if(it!=groups.end()) groups.erase(it);
}