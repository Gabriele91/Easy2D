#ifndef SCRIPT_H
#define SCRIPT_H

#include <Config.h>
#include <Resource.h>
#include <ELuaState.h>

namespace Easy2D {

	class Script : public Resource<Script>
    {

        LuaState::LuaClass* luaClass;

	public:
        
        Script(ResourcesManager<Script> *rsmr=NULL,
               const String& pathfile="");
        virtual ~Script();
        //new lua object
        LuaState::LuaObject* newObject()
        {
            return luaClass->newObject();
        }
        //load methods
        virtual bool load();
        virtual bool unload();

	};

};

#endif