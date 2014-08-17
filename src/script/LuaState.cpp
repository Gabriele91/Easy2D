#include <stdafx.h>
#include <Debug.h>
#include <Application.h>
#include <ELuaState.h>

using namespace Easy2D;

//#define LUA_REGINDEX LUA_REGISTRYINDEX //ONLY C ACCESS
#define LUA_REGINDEX LUA_GLOBALSINDEX

lua_State* LuaState::luaVM=nullptr;
size_t LuaState::classCount=0;

static void addBaseLibs (lua_State *L,const luaL_Reg *lib) {
  for (; lib->func; lib++) {
    lua_pushcfunction(L, lib->func);
    lua_pushstring(L, lib->name);
    lua_call(L, 1, 0);
  }
}


void LuaState::init()
{
    //init lua
    luaVM=lua_open();
    //libs
    static const luaL_Reg lualibs[] = {
      {"", luaopen_base},
      {"package", luaopen_package},
      {LUA_TABLIBNAME, luaopen_table},
      {LUA_STRLIBNAME, luaopen_string},
      {LUA_MATHLIBNAME, luaopen_math},
      {LUA_DBLIBNAME, luaopen_debug},
      {NULL, NULL}
    };
    addBaseLibs(luaVM,lualibs);
    //Easy 2D lib
    addMath3DLib();
    addEasy2DLib();
    addComponentsLib();
    //compile
    //indexing and object (table obj or global)
    compile(
    "__context__mt={               \n" 
    "    __index=function (ctx,k)  \n"
    "      return ctx.__TMP_G[k]   \n"
    "    end                       \n"
    "}                             \n"
    );

}
void LuaState::compile(const String& script)
{
    int error=luaL_dostring(luaVM,script.c_str());
    DEBUG_ASSERT_MSG(!error, "error running script:" << lua_tostring(luaVM, -1) );
}
void LuaState::execute(const String& smain)
{
    luabridge::LuaRef scriptMain = luabridge::getGlobal (luaVM, smain);

    try 
    {
        scriptMain();
    }
    catch (luabridge::LuaException const& e) 
    {
        DEBUG_ASSERT_MSG(0,"LuaState call \'"<<
                           smain <<"\', error:\n" <<
                           e.what() );
    }
}
void LuaState::destroy()
{
    lua_close(luaVM);
    luaVM=nullptr;
}


LuaState::LuaClass::LuaClass(const String& argName,const String& argText)
    :classScript(LuaState::luaVM)
{               
    //save names
    className=argName;
    //count class
    ++LuaState::classCount;
    //debug name
    String debugName; debugName+="Class "+className;
    //load file
    DEBUG_ASSERT_MSG(!luaL_loadbuffer(LuaState::luaVM, argText, argText.size(),debugName), 
        "LuaState compile class \'"<< argName <<"\', error"
    );
    //name
    lua_typename(LuaState::luaVM, -1);//no value
    //save ref function
    classScript=LuaRef::fromStack(LuaState::luaVM,-1);
    //pop function
    lua_pop(LuaState::luaVM,1);
}


LuaState::LuaObject* LuaState::LuaClass::newObject()
{
    auto newobj=new LuaState::LuaObject(LuaState::luaVM);
    ///////////////////////////////////////////////////////////////////// 
    //COPY CONTEXT TABLE
    //create a new table 
    newobj->objectRef=luabridge::newTable(LuaState::luaVM);
    //set global table
    newobj->objectRef["__TMP_G"]=luabridge::LuaRef::getGlobal(LuaState::luaVM,"_G");
    //set metatable
    newobj->objectRef.push(LuaState::luaVM);
    lua_getglobal(LuaState::luaVM,"__context__mt");
    lua_setmetatable(LuaState::luaVM, -2);
    lua_pop(LuaState::luaVM,1);
    /////////////////////////////////////////////////////////////////////
    //SET FUNCTION ENV (context)
    //get class (aka function)
    classScript.push(LuaState::luaVM);                      //push 1 (-2)
    //get env (table/context)
    newobj->objectRef.push(LuaState::luaVM);                //push 2 (-1)
    //set env
    DEBUG_ASSERT(lua_setfenv(LuaState::luaVM, -2) == 1);
    /////////////////////////////////////////////////////////////////////
    //INIT OBJECT CONTEXT
    //get class (aka function)
    classScript.push(LuaState::luaVM);                      //push 1 (-1)
    //call
    DEBUG_ASSERT_MSG(lua_pcall(LuaState::luaVM, 0, 0, 0)==0, 
                    "error contructor "<< className 
                    <<": "
                    << lua_tostring(LuaState::luaVM, -1));
    //return obj
    return newobj;
}

void LuaState::LuaObject::field(const String& var,LuaState::LuaRef& ref)
{
    //get object
    objectRef.push(LuaState::luaVM);                    //push 1 (-2)
    //get field
    lua_getfield(LuaState::luaVM, -1, var);             //push 2 (-1)
    //save ref
    ref=luabridge::LuaRef::fromStack(LuaState::luaVM,-1);
    //pop object ref & local field
    lua_pop(LuaState::luaVM,2);
    
}
//lua stack dump
void LuaState::stackDump()
{
    printf("Lua stack dump:\n");
    int i;
    int top = lua_gettop(LuaState::luaVM);
    for (i = 1; i <= top; i++) 
    {  /* repeat for each level */

        int t = lua_type(LuaState::luaVM, i);
        printf("\tStack[%d]=",i);

        switch (t)
        {
    
            case LUA_TSTRING:  /* strings */
            printf("`%s'", lua_tostring(LuaState::luaVM, i));
            break;
    
            case LUA_TBOOLEAN:  /* booleans */
            printf(lua_toboolean(LuaState::luaVM, i) ? "true" : "false");
            break;
    
            case LUA_TNUMBER:  /* numbers */
            printf("%g", lua_tonumber(LuaState::luaVM, i));
            break;

            case LUA_TFUNCTION:
            { 
                //c/lua function
                Debug::message() << (lua_iscfunction(LuaState::luaVM, i) ? "c" : "lua") << " function : "; 
                //get name
                lua_Debug lDebug;
                lua_getinfo(LuaState::luaVM, ">n", &lDebug);
                Debug::message() << (lDebug.name ? lDebug.name : "????") << " (" << lDebug.namewhat << ")";
                break;
            }
            break;
    
            default:  /* other values */
            printf("%s", lua_typename(LuaState::luaVM, t));
            break;
    
        }
        printf("\n");  /* put a separator */
    }
    printf("\n");  /* end the listing */
}
//lua print table in stack
void LuaState::printTable()
{
    lua_pushnil(luaVM);

    while(lua_next(luaVM, -2) != 0)
    {
        if(lua_isnil(luaVM, -1))
            printf("%s is nil\n", lua_tostring(luaVM, -2));
        if(lua_isstring(luaVM, -1))
            printf("%s = %s\n", lua_tostring(luaVM, -2), lua_tostring(luaVM, -1));
        else if(lua_isnumber(luaVM, -1))
            printf("%s = %d\n", lua_tostring(luaVM, -2), lua_tonumber(luaVM, -1));
        else if(lua_isboolean(luaVM, -1))
            printf("%s = %s\n", lua_tostring(luaVM, -2), lua_toboolean(luaVM, -1) ? "true" : "false");
        else if(lua_iscfunction(luaVM, -1))
            printf("CFunction: %s\n", lua_tostring(luaVM, -2));
        else if(lua_isfunction(luaVM, -1))
            printf("Function: %s\n", lua_tostring(luaVM, -2));
        else if(lua_istable(luaVM, -1))
            printf("Table: %s\n", lua_tostring(luaVM, -2));
        else if(lua_isuserdata(luaVM, -1))
            printf("Userdata: %s\n", lua_tostring(luaVM, -2));

        lua_pop(luaVM, 1);
    }
}


/*
void LuaState::Script::execute(const String& funname)
{
    //get table
    lua_getfield(LuaState::luaVM, LUA_REGINDEX, className); //push 1
    //get function
    lua_getfield(LuaState::luaVM, -1, funname);             //push 1
    //execute
    int error=lua_pcall(LuaState::luaVM, 0, 0, 0);
    //pop global field
    lua_pop(LuaState::luaVM,1);
    //return
    return;

    luabridge::Stack<bool> b;
}*/
/*
luabridge::LuaRef LuaState::Script::getRef(const String& var)
{
    //get table
    lua_getfield(LuaState::luaVM, LUA_REGINDEX, tableName); //push 1
    //get function
    lua_getfield(LuaState::luaVM, -1, var);             //push 1
    //create ref
    auto ref=luabridge::LuaRef::fromStack(LuaState::luaVM,-1);
    //pop global field & local field
    lua_pop(LuaState::luaVM,2);
    //
    return ref;
}*/