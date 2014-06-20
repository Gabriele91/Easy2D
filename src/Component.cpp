#include <stdafx.h>
#include <Component.h>
#include <Debug.h>

///////////////////////
using namespace Easy2D;
///////////////////////
DUNORDERED_MAP<String,ComponentMap::createComponent>* ComponentMap::cmap=nullptr;

Component* ComponentMap::create(const String& name)
{    
    //assert...
    DEBUG_ASSERT(cmap);
    //find
    auto it=cmap->find(name);
    //exist?
    if(it!=cmap->end())
    {
        return it->second();
    }
    //else null
    return nullptr;
}

void ComponentMap::append(const String& name,ComponentMap::createComponent fun)
{
    //map exist?
    if(!cmap)
        cmap=new DUNORDERED_MAP<String,createComponent>();
    //assert...
    DEBUG_ASSERT(cmap->find(name)==cmap->end());
    DEBUG_MESSAGE("Component: "<<name);
    //append component
    (*cmap)[name]=fun;
}