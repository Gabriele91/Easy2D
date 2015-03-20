#include <stdafx.h>
#include <ResourcesGroup.h>
#include <Table.h>
#include <Debug.h>
#include <Script.h>
#include <ELuaScript.h>

using namespace Easy2D;

void LuaScript::ScriptObject::callOnInit(Object* object)
{
	try
	{
		luaOnInit(object);
		state = ON_RUN;
	}
	catch (luabridge::LuaException const& e)
	{
		DEBUG_ASSERT_MSG(0, "LuaState call \'onInit\', error:\n" << e.what());
	}
	catch (std::exception const& e)
	{
		DEBUG_ASSERT_MSG(0, "LuaState call \'onInit\', error:\n" << e.what());
	}
}
void LuaScript::ScriptObject::callOnRun(Object* object, float dt)
{
	try
	{
		luaOnRun(object,dt);
	}
	catch (luabridge::LuaException const& e)
	{
		DEBUG_ASSERT_MSG(0, "LuaState call \'onRun\', error:\n" << e.what());
	}
	catch (std::exception const& e)
	{
		DEBUG_ASSERT_MSG(0, "LuaState call \'onRun\', error:\n" << e.what());
	}
}
void LuaScript::ScriptObject::callOnEnd(Object* object)
{
	try
	{
		luaOnEnd(object);
		state = ON_VOID;
	}
	catch (luabridge::LuaException const& e)
	{
		DEBUG_ASSERT_MSG(0, "LuaState call \'onEnd\', error:\n" << e.what());
	}
	catch (std::exception const& e)
	{
		DEBUG_ASSERT_MSG(0, "LuaState call \'onEnd\', error:\n" << e.what());
	}
}

void LuaScript::onRun(float dt)
{
    for (auto obj : objects)
    switch (obj->state)
    {
    case Easy2D::LuaScript::ON_INIT:
        obj->callOnInit(getObject());
    break;
    case Easy2D::LuaScript::ON_RUN:
        obj->callOnRun(getObject(),dt);
    break;
    case Easy2D::LuaScript::ON_END:
        obj->callOnEnd(getObject());
    break;
    case Easy2D::LuaScript::ON_VOID:
    default: break;
    };
}
int LuaScript::addScript(Script::ptr script)
{
	//alloc new obj
	auto obj = ScriptObject::ptr(new ScriptObject());
	//save class
	obj->luaClass = script;
	//new object script
	obj->luaObject = obj->luaClass->newObject();
	//get fields
	obj->luaObject->field("onInit", obj->luaOnInit);
	obj->luaObject->field("onRun", obj->luaOnRun);
	obj->luaObject->field("onEnd", obj->luaOnEnd);
	//do init
	obj->state = ON_INIT;
	//push
	objects.push_back(obj);
	//return id
	return objects.size() - 1;
}
int LuaScript::addScript(Script::ptr script, const Table& parameters)
{
	//add script
	int scriptID = addScript(script);
	auto obj     = objects[scriptID];
	//set default params
	for (auto param : parameters)
	{
		if (param.first.isString())
		{
			//set ref
			switch (param.second->type)
			{
			case Table::FLOAT:
				(*obj->luaObject)[param.first.string()] = param.second->get<float>();
				break;
			case Table::STRING:
				(*obj->luaObject)[param.first.string()] = param.second->get<String>().c_str();
				break;
			case Table::VECTOR2D:
				(*obj->luaObject)[param.first.string()] = param.second->get<Vec2>();
				break;
			case Table::VECTOR3D:
				(*obj->luaObject)[param.first.string()] = param.second->get<Vec3>();
				break;
			case Table::VECTOR4D:
				(*obj->luaObject)[param.first.string()] = &param.second->get<Vec4>();
				break;
			case Table::MATRIX4X4:
				(*obj->luaObject)[param.first.string()] = &param.second->get<Mat4>();
				break;
			default: /* wrong */ break;
			}
		}
	}
	return scriptID;
}

void LuaScript::onSetScene(Scene* scene)
{
	for (auto obj : objects)
		obj->callOnInit(getObject());
}
void LuaScript::onEraseScene()
{
	for (auto obj : objects)
		obj->callOnEnd(getObject());
}

void  LuaScript::serialize(Table& table)
{
	for (auto& scrobj : objects)
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
            obj->state = ON_INIT;
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
								(*obj->luaObject)[param.first.string()] = param.second->get<float>();
                                break;
							case Table::STRING:
								(*obj->luaObject)[param.first.string()] = param.second->get<String>().c_str();
                                break;
							case Table::VECTOR2D:
								(*obj->luaObject)[param.first.string()] = param.second->get<Vec2>();
                                break;
							case Table::VECTOR3D:
								(*obj->luaObject)[param.first.string()] = param.second->get<Vec3>();
                                break;
                            case Table::VECTOR4D:
								(*obj->luaObject)[param.first.string()]=&param.second->get<Vec4>();
                                break;
                            case Table::MATRIX4X4:
								(*obj->luaObject)[param.first.string()]=&param.second->get<Mat4>();
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
}


