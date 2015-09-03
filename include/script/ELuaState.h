#ifndef ELUASTATE_H
#define ELUASTATE_H

#include <Config.h>
#include <EString.h>
#include <RefCountedPtr.h>

namespace Easy2D
{

template <>
struct luabridge::Stack <String>
{

    static void push (lua_State* L,const String& s)
    {
        lua_pushstring (L, s.cStr());
    }
    
    static String get (lua_State* L, int index)
    {
        return String (luaL_checkstring (L, index));
    }
};
/**
Stack specialization for `String const&`.
*/
template <>
struct luabridge::Stack <String const&>
{
	static inline void push(lua_State* L, String const& str)
	{
		lua_pushlstring(L, str.cStr(), str.size());
	}

	static inline String get(lua_State* L, int index)
	{
		size_t len;
		const char *str = luaL_checklstring(L, index, &len);
		return std::string(str, len);
	}
}; 

template<class C, void(*function)(C*, lua_State*, int)>
int LuaConstructor(lua_State* lvm)
{
	int const nargs = lua_gettop(lvm);
	//memory
	C* o = (C*)luabridge::UserdataValue <C>::place(lvm);
	//function
	function(o, lvm, nargs);
	//call 
	return 1;
}
template<class C, C* (*function)(lua_State*, int)>
int LuaRefConstructor(lua_State* lvm)
{
	int const nargs = lua_gettop(lvm);
	//alloc
	C* o = (C*)function(lvm, nargs);
	//new
	luabridge::UserdataSharedHelper <RefCountedPtr <C>, false>::push(lvm, o);
	//call 
	return 1;
}
    
class LuaState
{
    //count script loaded
    static size_t classCount;
    //save a vm
    static lua_State* luaVM;
    

    public:
    //
    class LuaClass;
    class LuaObject;
    class LuaField;
    typedef luabridge::LuaRef LuaRef;
    //
    class LuaObject
    {
        //class can access to obj
        friend class LuaClass;
        //object ref
        luabridge::LuaRef objectRef;
        //init
        LuaObject(lua_State* luaVM):objectRef(luaVM){}

        public:
        //get a field
        void field(const String& var,LuaRef& ref);
        //set field
		template<class T>
		void setField(const String& var, T ref)
		{
			//get object
			objectRef[var.cStr()] = ref;
		};
		//get fields
		luabridge::Iterator getFields()
		{
			return  luabridge::Iterator(objectRef);
		}
        //get fields
		template<class K>
		luabridge::LuaRef::Proxy operator[](const K& key)
		{
			return objectRef[key];
		}
        //get all values
        void forAllValues(DFUNCTION<void (      LuaState::LuaRef& table,
                                          const LuaState::LuaRef& key,
                                          const LuaState::LuaRef& value)> callback);
        //get all values of all tables
        void forAllValuesForAlltables(DFUNCTION<void (      LuaState::LuaRef& table,
                                                      const LuaState::LuaRef& key,
                                                      const LuaState::LuaRef& value)> callback);
    
    private:
        //get all values of all tables
        static void forAllValuesForAlltables_aux(LuaState::LuaRef& table,DFUNCTION<void (        LuaState::LuaRef& table,
                                                                                           const LuaState::LuaRef& key,
                                                                                           const LuaState::LuaRef& value)> callback);
    };
    //script class
    class LuaClass
    {
        String className;
        LuaRef classScript;

        public:
        LuaClass(const String& className,const String& textFile);
        LuaObject* newObject();
    };
    //functions
    static lua_State* getVM()
    {
        return luaVM;
    }
    static void init();
    static void addMath3DLib();
    static void addEasy2DLib();
    static void addBodyLib();
    static void addRenderableLib();
    static void addComponentsLib();
    static void compile(const String& script);
    static void execute(const String& smain);
    static void destroy();
    static LuaRef* createRef()
    {
        return new luabridge::LuaRef(luaVM);
    }
    //lua stack dump
    static void stackDump ();
    //lua print table in stack
    static void printTable();
};

};
#endif