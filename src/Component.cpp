#include <stdafx.h>
#include <Component.h>
#include <Object.h>
#include <Debug.h>

///////////////////////
using namespace Easy2D;
///////////////////////
UnorderedMap<String,ComponentMap::createComponent>* ComponentMap::cmap=nullptr;
UnorderedMap<String,uint>* ComponentMap::fmap=nullptr;

Object* Component::getObject() 
{
	return object;
}
Scene* Component::getScene()
{
	if (object)
		return object->getScene();
	return nullptr;
}
const Object* Component::getObject() const
{
	return object;
}
const Scene* Component::getScene() const
{
	if (object)
		return object->getScene();
	return nullptr;
}

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

uint ComponentMap::getFamily(const String& name)
{
    //assert...
    DEBUG_ASSERT(fmap);
    //find
    auto it=fmap->find(name);
    //exist?
    if(it!=fmap->end())
    {
        return it->second;
    }
    //else null
    return 0;
}

void ComponentMap::append(const String& name,ComponentMap::createComponent fun,uint family)
{
    //map exist?
    if(!cmap)
    {
        cmap=new UnorderedMap<String,createComponent>();
        fmap=new UnorderedMap<String,uint>();
    }
    //assert...
    DEBUG_ASSERT(cmap->find(name)==cmap->end());
    DEBUG_ASSERT(fmap->find(name)==fmap->end());
    DEBUG_MESSAGE("Component: "<<name<<", family: "<<family);
    //append component
    (*cmap)[name]=fun;
    (*fmap)[name]=family;
}
