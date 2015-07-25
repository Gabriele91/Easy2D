#include <stdafx.h>
#include <Debug.h>
#include <Math3D.h>
#include <Application.h>
#include <ETime.h>
#include <Script.h>
#if defined( PLATFORM_IOS )
#include <AppiOS.h>
#elif defined( PLATFORM_OSX )
#include <CocoaApp.h>
#elif defined( PLATFORM_WINDOW )
	#ifndef ES2_ANGLEPROJECT
		#include <WindowsApp.h>
	#else
		#include <AngleApp.h>
	#endif
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
/**
 * default stream use C filelib i/o
 */
class CResouceStream : public Application::ResouceStream
{

    FILE *pFile;

public:

    CResouceStream(const String& path)
    {
        /**
         * Open file
         */
        pFile=fopen(path, "rb");
        DEBUG_MESSAGE("open stream file: "<<path);
        DEBUG_ASSERT_MSG(pFile,"error open stream file: "<<path);
    }

    virtual ~CResouceStream()
    {
        if(pFile)
            close();
    }
    ///close file
    virtual void close()
    {
        fclose(pFile);
        pFile=NULL;
    }
    ///read from file
    virtual size_t read(void * ptr, size_t size, size_t count)
    {
        return fread(ptr,size,count,pFile);
    }
    ///seek from file
    virtual size_t seek (size_t offset, Application::Seek origin )
    {
        int cseek=0;
        switch (origin)
        {
        case Application::Seek::CUR:
            cseek=SEEK_CUR;
            break;
        case Application::Seek::SET:
            cseek=SEEK_SET;
            break;
        case Application::Seek::END:
            cseek=SEEK_END;
            break;
        default:
            break;
        }
        return fseek (pFile, offset, cseek);
    }
    ///returns the current value of the position indicator of the stream
    virtual size_t tell()
    {
        return ftell(pFile);
    }
    ///get file size
    virtual size_t size()
    {
        size_t pos=tell();
        seek(0,Application::Seek::END);
        size_t sizefile=tell();
        seek(pos,Application::Seek::SET);
        return sizefile;
    }
    ///return a uchar cast in int
    virtual int getc()
    {
        char c;
        read(&c, 1, 1);
        return c;
    }
    ///rewind from file
    virtual void rewind ()
    {
        seek(0,Application::Seek::SET);
    }

};
///////////////////////
Application *Application::appSingleton=NULL;
///////////////////////
Application::Application()
    :lastDeltaTime(0.0f)
    ,mainInstance(NULL)
    ,screen(NULL)
    ,input(NULL)
    ,audio(NULL)
{

}
Application::~Application()
{
    appSingleton=nullptr;
}

Application *Application::create(const String& name,Application* implementation)
{

    DEBUG_ASSERT(!appSingleton);
    //init random value
    Math::seedRandom((uint)GetTime());
    //init vm
    LuaState::init();
    //init app
    if(!implementation)
    {
    #if defined( PLATFORM_IOS )
        appSingleton=new AppiOS(name);
    #elif defined( PLATFORM_OSX )
        appSingleton=new CocoaApp(name);
    #elif defined( PLATFORM_WINDOW )
		#ifndef ES2_ANGLEPROJECT
			appSingleton=new WindowsApp(name);
		#else
			appSingleton=new AngleApp(name);
		#endif
    #elif defined( PLATFORM_LINUX )
        appSingleton=new LinuxApp(name);
    #elif defined( PLATFORM_ANDROID )
        appSingleton=new AndroidApp(name);
    #elif defined( PLATFORM_EMSCRIPTEN )
        appSingleton=new EmscriptenApp(name);
    #endif
    }
    else
    {
        appSingleton=implementation;
    }
    //registration delete at exit
    Easy2D::atexit(Application::release);
    //
    return appSingleton;
}
/**
* delete singleton (if not already created raises an exception)
*/
void Application::release()
{
    if(appSingleton)
    {
        //delete vm
        LuaState::destroy();
        //delete app
        delete Application::instance();
        //safe delete
        appSingleton=nullptr;
    }
}
Application *Application::instance()
{
    return appSingleton;
}
/**
 * stream resource
 * @return ResouceStream object
 */
Application::ResouceStream* Application::getResouceStream(const String& path)
{
    return new CResouceStream(/* appResourcesDirectory()+'/'+ */ path);
}
/**
* save a resourcesGroup
*/
void Application::subscriptionResourcesGroup(const String& name,ResourcesGroup *rsGr)
{
    DEBUG_ASSERT(rsGr);
    DEBUG_ASSERT(groups.find(name)==groups.end());
    groups[name]=rsGr;
}
/**
* get a resourcesGroup
*/
ResourcesGroup* Application::getResourcesGroup(const String& name)
{
    auto it=groups.find(name);
    if(it!=groups.end()) return it->second;
    return NULL;
}
/**
* erase a resourcesGroup
*/
void Application::unsubscriptionResourcesGroup(const String& name)
{
    auto it=groups.find(name);
    if(it!=groups.end()) groups.erase(it);
}