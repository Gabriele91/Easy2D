#ifndef ELUASCRIPT_H
#define ELUASCRIPT_H

#include <Config.h>
#include <Component.h>
#include <ELuaState.h>


namespace Easy2D
{
   
class LuaScript :  public Component
{
    
    //Component state
    enum State
    {
        ON_VOID,
        ON_INIT,
        ON_RUN,
        ON_END
    };
    //script objects
    class ScriptObject : public Pointers<ScriptObject>
    {
	public:
		State state;
        Script::ptr luaClass;
        LuaState::LuaObject* luaObject;
        
        LuaState::LuaRef luaOnInit;
        LuaState::LuaRef luaOnRun;
        LuaState::LuaRef luaOnEnd;

		void callOnInit(Object*);
		void callOnRun(Object*,float);
		void callOnEnd(Object*);

        ScriptObject():
         state(ON_VOID)
        ,luaObject(nullptr)
        ,luaOnInit(LuaState::getVM())
        ,luaOnRun(LuaState::getVM())
        ,luaOnEnd(LuaState::getVM())
        {
        }
        ~ScriptObject()
        {
            if(luaObject)
                delete luaObject;
        }
    };
    //list of objects
    std::vector<ScriptObject::ptr> objects;

    public:

    LuaScript():Component()
    {

	}
	LuaScript(Script::ptr script) :Component()
	{
		addScript(script);
	}
    virtual ~LuaScript()
    {
    }
    //methods
    LuaState::LuaObject* getLuaObject(int i) const
    {
        return objects[i]->luaObject;
    }
    Script::ptr getScript(int i) const
    {
        return objects[i]->luaClass;
    }
    const String& getScriptName(int i) const
    {
        return getScript(i)->getName();
    }
    size_t countScripts()
    {
        return objects.size();
    }
	//add lua script
	int addScript(Script::ptr script);
	int addScript(Script::ptr script,const Table& table);
    //update
	virtual void onSetScene(Scene* scene);
    virtual void onRun(float dt);
	virtual void onEraseScene();
    //component
    ADD_COMPONENT_METHOS(LuaScript)
    //serialize/deserialize
    virtual void serialize(Table& table);
    virtual void deserialize(const Table& table);

};
REGISTERED_COMPONENT(LuaScript,"LuaScript")

};
#endif