#ifndef ELUASCRIPT_H
#define ELUASCRIPT_H

#include <Config.h>
#include <Component.h>
#include <ELuaState.h>


namespace Easy2D
{
   
class LuaScript :  public Component
{
    
    Script::ptr luaClass;
    LuaState::LuaObject* luaObject;

    enum State
    {
        ON_VOID,
        ON_INIT,
        ON_RUN,
        ON_END
    };
    State state;
    LuaState::LuaRef luaOnInit;
    LuaState::LuaRef luaOnRun;
    LuaState::LuaRef luaOnEnd;

    public:

    LuaScript():Component()
               ,luaObject(nullptr)
               ,luaOnInit(LuaState::getVM())
               ,luaOnRun(LuaState::getVM())
               ,luaOnEnd(LuaState::getVM())
               ,state(ON_VOID)
    {

    }
    virtual ~LuaScript()
    {
        if(luaObject)
            delete luaObject;
    }

    virtual void onRun(float dt);
    //component
    ADD_COMPONENT_METHOS(LuaScript)
    //serialize/deserialize
    virtual void serialize(Table& table);
    virtual void deserialize(const Table& table);

};
REGISTERED_COMPONENT(LuaScript,"LuaScript")

};
#endif