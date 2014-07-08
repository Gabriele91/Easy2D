#include <stdafx.h>
#include <ResourcesGroup.h>
#include <Table.h>
#include <Debug.h>
#include <Script.h>
#include <ELuaScript.h>

using namespace Easy2D;

void LuaScript::onRun(float dt)
{
    //call run
    if(luaObject)
    {
        //call init
        switch (state)
        {
        case Easy2D::LuaScript::ON_INIT:
            try 
            {
                luaOnInit(getObject());
                state=ON_RUN;
            }
            catch (luabridge::LuaException const& e) 
            {
                DEBUG_ASSERT_MSG(0,"LuaState call \'onInit\', error:\n" << e.what() );
            }
            break;
        case Easy2D::LuaScript::ON_RUN:
            try 
            {
                luaOnRun(getObject(),dt);
            }
            catch (luabridge::LuaException const& e) 
            {
                DEBUG_ASSERT_MSG(0,"LuaState call \'onRun\', error:\n" << e.what() );
            }
            break;
        case Easy2D::LuaScript::ON_END:
            try 
            {
                luaOnEnd(getObject());
                state=ON_VOID;
            }
            catch (luabridge::LuaException const& e) 
            {
                DEBUG_ASSERT_MSG(0,"LuaState call \'onEnd\', error:\n" << e.what() );
            }
            break;
        case Easy2D::LuaScript::ON_VOID:
        default: 
            break;
        }

    }
}

void  LuaScript::serialize(Table& table)
{
    if(luaObject && luaClass)
    {
        table.set("class",luaClass->getName());
    }
}
void LuaScript::deserialize(const Table& table)
{
    if(table.existsAsType("class",Table::STRING))
    {
        auto rsmanager=table.getResourcesManager();
        DEBUG_ASSERT(rsmanager);
        auto rsgroup=rsmanager->getResourcesGroup();
        DEBUG_ASSERT(rsgroup);
        //save class 
        String className=table.getString("class");
        luaClass=rsgroup->load<Script>(className);
        //new object script
        luaObject=luaClass->newObject();
        //get fields
        luaObject->field("onInit",luaOnInit);
        luaObject->field("onRun",luaOnRun);
        luaObject->field("onEnd",luaOnEnd);
        //set some global variable
        if(table.existsAsType("parameters",Table::TABLE))
        {
            const Table& parameters=table.getConstTable("parameters");

            for(auto param:parameters)
            {
                if(param.first.isString())
                {
                    //set ref
                    switch (param.second->type)
                    {
                        case Table::FLOAT: 
                            luaObject->setField(param.first.string(),param.second->get<float>());
                        break;
                        case Table::STRING: 
                            luaObject->setField(param.first.string(),param.second->get<String>().c_str());
                        break;
                        case Table::VECTOR2D: 
                            luaObject->setField(param.first.string(),param.second->get<Vec2>());
                        break;
                        case Table::VECTOR3D: 
                            luaObject->setField(param.first.string(),param.second->get<Vec3>());
                        break;/*
                        case Table::VECTOR4D: 
                            luaObject->setField(param.first.string(),param.second->get<Vec4>());
                        break;
                        case Table::MATRIX4X4: 
                            luaObject->setField(param.first.string(),param.second->get<Mat4>());
                        break;*/
                        default: /* wrong */ break;
                    }
                }
            }
        }

        state=ON_INIT;
    }    
}