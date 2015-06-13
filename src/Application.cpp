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
Application *Application::app_singleton=nullptr;
///////////////////////
Application::Application()
    : m_last_delta_time(0.0f)
    , m_main_instance(nullptr)
    , m_screen(nullptr)
    , m_input(nullptr)
    , m_audio(nullptr)
{

}
Application::~Application()
{
    app_singleton=nullptr;
}

Application *Application::create(const String& name,Application* implementation)
{

    DEBUG_ASSERT(!app_singleton);
    //init random value
    Math::seedRandom((uint)GetTime());
    //init vm
    LuaState::init();
    //init app
    if(!implementation)
    {
    #if defined( PLATFORM_IOS )
        app_singleton=new AppiOS(name);
    #elif defined( PLATFORM_OSX )
        app_singleton=new CocoaApp(name);
    #elif defined( PLATFORM_WINDOW )
		#ifndef ES2_ANGLEPROJECT
			app_singleton=new WindowsApp(name);
		#else
			app_singleton=new AngleApp(name);
		#endif
    #elif defined( PLATFORM_LINUX )
        app_singleton=new LinuxApp(name);
    #elif defined( PLATFORM_ANDROID )
        app_singleton=new AndroidApp(name);
    #elif defined( PLATFORM_EMSCRIPTEN )
        app_singleton=new EmscriptenApp(name);
    #endif
    }
    else
    {
        app_singleton=implementation;
    }
    //registration delete at exit
    atexit(Application::release);
    //
    return app_singleton;
}
/**
* delete singleton (if not already created raises an exception)
*/
void Application::release()
{
    if(app_singleton)
    {
        //delete vm
        LuaState::destroy();
        //delete app
        delete Application::instance();
        //safe delete
        app_singleton=nullptr;
    }
}
Application *Application::instance()
{
    return app_singleton;
}
/**
 * stream resource
 * @return ResouceStream object
 */
Application::ResouceStream* Application::get_resouce_stream(const String& path)
{
    return new CResouceStream(/* appResourcesDirectory()+'/'+ */ path);
}
/**
* save a resourcesGroup
*/
void Application::subscription_resources_group(const String& name,ResourcesGroup *rsGr)
{
    DEBUG_ASSERT(rsGr);
    DEBUG_ASSERT(m_groups.find(name)==m_groups.end());
    m_groups[name]=rsGr;
}
/**
* get a resourcesGroup
*/
ResourcesGroup* Application::get_resources_group(const String& name)
{
    auto it=m_groups.find(name);
    if(it!=m_groups.end()) return it->second;
    return NULL;
}
/**
* erase a resourcesGroup
*/
void Application::unsubscription_resources_group(const String& name)
{
    auto it=m_groups.find(name);
    if(it!=m_groups.end()) m_groups.erase(it);
}