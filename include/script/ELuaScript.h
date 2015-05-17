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
        
        void forAllValues(DFUNCTION<void (      LuaState::LuaRef& table,
                                          const LuaState::LuaRef& key,
                                          const LuaState::LuaRef& value)> callback)
        {
            luaObject->forAllValues(callback);
        }
        void forAllValuesForAlltables(DFUNCTION<void (      LuaState::LuaRef& table,
                                                      const LuaState::LuaRef& key,
                                                      const LuaState::LuaRef& value)> callback)
        {
            luaObject->forAllValuesForAlltables(callback);
        }
    };
    
    //list of objects
    typedef std::vector<ScriptObject::ptr> VObjs;
    VObjs objects;
    
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
    //iterators
    VObjs::iterator bigin()
    {
        return objects.begin();
    }
    VObjs::iterator end()
    {
        return objects.end();
    }
    VObjs::const_iterator bigin() const
    {
        return objects.begin();
    }
    VObjs::const_iterator end() const
    {
        return objects.end();
    }
    //for all values
    void forAllValues(DFUNCTION<void (      LuaState::LuaRef& table,
                                      const LuaState::LuaRef& key,
                                      const LuaState::LuaRef& value)> callback)
    {
        for(auto& obj:objects) obj->forAllValues(callback);
    }
    //for all values and sub tables
    void forAllValuesForAllTables(DFUNCTION<void (      LuaState::LuaRef& table,
                                                  const LuaState::LuaRef& key,
                                                  const LuaState::LuaRef& value)> callback)
    {
        for(auto& obj:objects) obj->forAllValuesForAlltables(callback);
    }
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