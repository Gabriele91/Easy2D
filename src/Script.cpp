#include <stdafx.h>
#include <Script.h>
#include <Application.h>

///////////////////////
using namespace Easy2D;
///////////////////////
Script::Script(ResourcesManager<Script> *rsmr,
               const String& pathfile)
    :Resource(rsmr,pathfile)
    ,luaClass(nullptr)
{
    //is not loaded
    loaded=false;
}
Script::~Script()
{
    //release resource
    release();
}
//load methods
bool Script::load()
{
    //load
    void* buffer;
    size_t len;
    Application::instance()->loadData(getPath(),buffer,len);
    //set text
    String text=(char*)buffer;
    //free
    free(buffer);
    //compile
    luaClass=new LuaState::LuaClass(getName(),text);
    //is loaded
    loaded=true;
    //return
    return loaded;
}
bool Script::unload()
{
    if(luaClass)
    {
        delete luaClass;
        luaClass=nullptr;
    }
    //is not loaded
    loaded=false;
    //return
    return !loaded;
}