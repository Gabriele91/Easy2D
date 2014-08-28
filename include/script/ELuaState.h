#ifndef ELUASTATE_H
#define ELUASTATE_H

#include <Config.h>
#include <EString.h>

namespace Easy2D
{
    
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
        void setField(const String& var,T ref)
        {
            //get object
            objectRef[var.c_str()]=ref;                 
        };
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