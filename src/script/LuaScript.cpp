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
    if(objects.size())
    {
        //call init
        switch (state)
        {
        case Easy2D::LuaScript::ON_INIT:
            try 
            {
                //init alls
                for(auto obj:objects) obj->luaOnInit(getObject());
                state=ON_RUN;
            }
            catch (luabridge::LuaException const& e) 
            {
                DEBUG_ASSERT_MSG(0,"LuaState call \'onInit\', error:\n" << e.what() );
            }
            catch (std::exception const& e) 
            {
                DEBUG_ASSERT_MSG(0,"LuaState call \'onInit\', error:\n" << e.what() );
            }
            break;
        case Easy2D::LuaScript::ON_RUN:
            try 
            {
                //run alls
                for(auto obj:objects) obj->luaOnRun(getObject(),dt);
            }
            catch (luabridge::LuaException const& e) 
            {
                DEBUG_ASSERT_MSG(0,"LuaState call \'onRun\', error:\n" << e.what() );
            }
            catch (std::exception const& e) 
            {
                DEBUG_ASSERT_MSG(0,"LuaState call \'onRun\', error:\n" << e.what() );
            }
            break;
        case Easy2D::LuaScript::ON_END:
            try 
            {
                //end alls
                for(auto obj:objects) obj->luaOnEnd(getObject());
                state=ON_VOID;
            }
            catch (luabridge::LuaException const& e) 
            {
                DEBUG_ASSERT_MSG(0,"LuaState call \'onEnd\', error:\n" << e.what() );
            }
            catch (std::exception const& e) 
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
    for(auto& scrobj:objects)
    {
        table.createTable(scrobj->luaClass->getName());
    }
}
void LuaScript::deserialize(const Table& table)
{
    auto rsmanager=table.getResourcesManager();
    DEBUG_ASSERT(rsmanager);
    auto rsgroup=rsmanager->getResourcesGroup();
    DEBUG_ASSERT(rsgroup);
    
    for (auto& element:table)
    {
        
        bool table=element.first.isString() && element.second->asType(Table::TABLE) ;
        bool name =!element.first.isString() && element.second->asType(Table::STRING) ;
        
        if( table || name )
        {
            //alloc new obj
            auto obj=ScriptObject::ptr(new ScriptObject());
            //get name class
            const String& nameClass= table ? element.first.string() : element.second->get<String>();
            //get class object
            obj->luaClass=rsgroup->load<Script>(nameClass);
            //new object script
            obj->luaObject=obj->luaClass->newObject();
            //get fields
            obj->luaObject->field("onInit",obj->luaOnInit);
            obj->luaObject->field("onRun",obj->luaOnRun);
            obj->luaObject->field("onEnd",obj->luaOnEnd);
            //set default params
            if(table)
            {
                const Table& parameters=element.second->get<Table>();
                for(auto param:parameters)
                {
                    if(param.first.isString())
                    {
                        //set ref
                        switch (param.second->type)
                        {
                            case Table::FLOAT:
                                obj->luaObject->setField(param.first.string(),param.second->get<float>());
                                break;
                            case Table::STRING:
                                obj->luaObject->setField(param.first.string(),param.second->get<String>().c_str());
                                break;
                            case Table::VECTOR2D:
                                obj->luaObject->setField(param.first.string(),param.second->get<Vec2>());
                                break;
                            case Table::VECTOR3D:
                                obj->luaObject->setField(param.first.string(),param.second->get<Vec3>());
                                break;
                            case Table::VECTOR4D:
                                obj->luaObject->setField(param.first.string(),param.second->get<Vec4>());
                                break;
                            case Table::MATRIX4X4:
                                obj->luaObject->setField(param.first.string(),param.second->get<Mat4>());
                                break;
                            default: /* wrong */ break;
                        }
                    }
                }
            }
            //push
            objects.push_back(obj);
        }
    }
    state=ON_INIT;
}


