#include <stdafx.h>
#include <ELuaState.h>
#include <Input.h>
#include <Screen.h>
#include <Application.h>
#include <Color.h>
#include <Object.h>
//components
#include <Renderable.h>
#include <ParticleSystem.h>
#include <Sprite.h>
#include <AnimatedSprite.h>
#include <Parallax.h>
#include <Body.h>
#include <State.h>
#include <ResourcesGroup.h>
//end component
using namespace Easy2D;

#define GeS(X) X,X
template<class T> T* getLuaComponent(const Object *costObj)
{
    //unsafe
    Object* obj=(Object*)costObj;
    //olready added?
    if(obj->hasComponent<T>())
        return (T*)obj->getComponent<T>();
    else 
        throw luabridge::LuaException(nullptr,"component","component not exist",0);
    //retutn new component
    return nullptr;
}

static int LuaNewColor(lua_State* lVM)
{
	int const nargs = lua_gettop(lVM);
	//memory
	Color* c = (Color*)luabridge::UserdataValue <Color>::place(lVM);
	//push a new vec2
	if (nargs == 1)
	{
		new (c)Color();
	}
	else
	if (lua_isuserdata(lVM, 2) && nargs == 2)
	{
		new (c) Color(luabridge::Stack <Color>::get(lVM, 2));
	}
	else
	if (lua_isnumber(lVM, 2) &&
		lua_isnumber(lVM, 3) &&
		lua_isnumber(lVM, 4) &&
		lua_isnumber(lVM, 5) && nargs == 5)
	{
		new (c)Color(luabridge::Stack <uchar>::get(lVM, 2),
					 luabridge::Stack <uchar>::get(lVM, 3),
					 luabridge::Stack <uchar>::get(lVM, 4),
					 luabridge::Stack <uchar>::get(lVM, 5));
	}
	else
	{
		luaL_argerror(lVM, nargs, "Color() fail");
	}
	return 1;
}

class LuaColor : public Color
{
    public:
    int __tostring (lua_State* luaVM)
    {
        //good cast
        auto rsthis=((Color*)(this));//1=self/this
        //call
        String r=String::toString((int)rsthis->r)+", ";
        String g=String::toString((int)rsthis->g)+", ";
        String b=String::toString((int)rsthis->b)+", ";
        String a=String::toString((int)rsthis->a);
        luabridge::Stack<const char*>::push(luaVM,(r+g+b+a).c_str());
        //
        return 1;
    }
    int __towatch (lua_State* luaVM)
    {
        //good cast
        auto rsthis=((Color*)(this));//1=self/this
        //call
        luabridge::LuaRef t = luabridge::newTable (luaVM);
        t["r"]=rsthis->r;
        t["g"]=rsthis->g;
        t["b"]=rsthis->b;
        t["a"]=rsthis->a;
        t.push(luaVM);
        //
        return 1;
    }
};

class LuaE2Object : public Object
{
    public:
    /*
    void setPosition(Vec2[,bool])
    void setScale(Vec2[,bool])
    void setRotation(float[,bool])
    */
    int setPosition (lua_State* luaVM)
    { 
        int const nargs = lua_gettop (luaVM);
        //good cast
        auto rsthis=((Object*)(this));//1=self/this
        //set pos
        if(nargs==2)
            rsthis->setPosition(luabridge::Stack<Vec2>::get(luaVM,2));
        else
        if(nargs==3)
            rsthis->setPosition(luabridge::Stack<Vec2>::get(luaVM,2),
                                luabridge::Stack<bool>::get(luaVM,2));
        else
            luaL_argerror(luaVM,nargs,"Object:setPosition fail");

        return 0;
    }
    int setScale (lua_State* luaVM)
    { 
        int const nargs = lua_gettop (luaVM);
        //good cast
        auto rsthis=((Object*)(this));//1=self/this
        //set scale
        if(nargs==2)
            rsthis->setScale(luabridge::Stack<Vec2>::get(luaVM,2));
        else
        if(nargs==3)
            rsthis->setScale(luabridge::Stack<Vec2>::get(luaVM,2),
                             luabridge::Stack<bool>::get(luaVM,2));
        else
            luaL_argerror(luaVM,nargs,"Object:setScale fail");

        return 0;
	}
	int setRotation(lua_State* luaVM)
	{
		int const nargs = lua_gettop(luaVM);
		//good cast
		auto rsthis = ((Object*)(this));//1=self/this
		//set scale
		if (nargs == 2)
			rsthis->setRotation(luabridge::Stack<Angle>::get(luaVM, 2));
		else
			if (nargs == 3)
				rsthis->setRotation(luabridge::Stack<Angle>::get(luaVM, 2),
				luabridge::Stack<bool>::get(luaVM, 2));
			else
				luaL_argerror(luaVM, nargs, "Object:setRotation fail");

		return 0;
	}
	int setZ(lua_State* luaVM)
	{
		int const nargs = lua_gettop(luaVM);
		//good cast
		auto rsthis = ((Object*)(this));//1=self/this
		//set scale
		if (nargs == 2)
			rsthis->setZ(luabridge::Stack<float>::get(luaVM, 2));
		else
			if (nargs == 3)
				rsthis->setZ(luabridge::Stack<float>::get(luaVM, 2),
							 luabridge::Stack<bool>::get(luaVM, 2));
			else
				luaL_argerror(luaVM, nargs, "Object:setZ fail");

		return 0;
	}
    /*
    Vec2 getPosition([bool])
    Vec2 getScale([bool])
    float getRotation([bool])
    */
    int getPosition (lua_State* luaVM)
    { 
        int const nargs = lua_gettop (luaVM);
        //good cast
        auto rsthis=((Object*)(this));//1=self/this
        //get pos
        if(nargs==1)
            luabridge::Stack<Vec2>::push(luaVM,rsthis->getPosition());
        else
        if(nargs==2)
            luabridge::Stack<Vec2>::push(luaVM,rsthis->getPosition(
                luabridge::Stack<bool>::get(luaVM,2)
            ));
        else
        {
            luaL_argerror(luaVM,nargs,"Object:getPosition fail");
            return 0;
        }
        return 1;
    }
    int getScale (lua_State* luaVM)
    { 
        int const nargs = lua_gettop (luaVM);
        //good cast
        auto rsthis=((Object*)(this));//1=self/this
        //get scale
        if(nargs==1)
            luabridge::Stack<Vec2>::push(luaVM,rsthis->getScale());
        else
        if(nargs==2)
            luabridge::Stack<Vec2>::push(luaVM,rsthis->getScale(
                luabridge::Stack<bool>::get(luaVM,2)
            ));
        else
        {
            luaL_argerror(luaVM,nargs,"Object:getScale fail");
            return 0;
        }
        return 1;
    }
    int getRotation(lua_State* luaVM)
	{
		int const nargs = lua_gettop(luaVM);
		//good cast
		auto rsthis = ((Object*)(this));//1=self/this
		//get scale
		if (nargs == 1)
			luabridge::Stack<Angle>::push(luaVM, rsthis->getRotation());
		else
			if (nargs == 2)
				luabridge::Stack<Angle>::push(luaVM, rsthis->getRotation(
				luabridge::Stack<bool>::get(luaVM, 2)
				));
			else
			{
				luaL_argerror(luaVM, nargs, "Object:getRotation fail");
				return 0;
			}
		return 1;
	}    
    int getZ(lua_State* luaVM)
	{
		int const nargs = lua_gettop(luaVM);
		//good cast
		auto rsthis = ((Object*)(this));//1=self/this
		//get scale
		if (nargs == 1)
			luabridge::Stack<float>::push(luaVM, rsthis->getZ());
		else
			if (nargs == 2)
				luabridge::Stack<float>::push(luaVM, rsthis->getZ(
						luabridge::Stack<bool>::get(luaVM, 2)
				));
			else
			{
				luaL_argerror(luaVM, nargs, "Object:getZ fail");
				return 0;
			}
		return 1;
	}
    /*
    Object* getObject(string)
    Object* getChild(string)
    */
	int addChild(lua_State* luaVM)
	{
		int const nargs = lua_gettop(luaVM);
		//good cast
		auto rsthis = ((Object*)(this));
		//get obj
		if (nargs == 2)
		{
			RefCountedPtr <Object> ref(luabridge::Stack<Object*>::get(luaVM, 2));
			ref.disable();
			Object::addChild(ref.get());
		}
		else
		{
			luaL_argerror(luaVM, nargs, "Object:addChild fail");
		}
		return 0;
    }
	int eraseChild(lua_State* luaVM)
	{
		int const nargs = lua_gettop(luaVM);
		//good cast
		auto rsthis = ((Object*)(this));
		//get obj
		if (nargs == 2)
		{
			Object* obj = luabridge::Stack<Object*>::get(luaVM, 2);
			Object::eraseChild(obj);
			RefCountedPtr <Object> ref(obj); ref.enable();
			luabridge::Stack< RefCountedPtr <Object> >::push(luaVM, ref);
			return 1;
		}
		else
		{
			luaL_argerror(luaVM, nargs, "Object:eraseChild fail");
			return 0;
		}
	}
	int getObject(lua_State* luaVM)
    { 
        int const nargs = lua_gettop (luaVM);
        //good cast
        auto rsthis=((Object*)(this));//1=self/this
        //get obj
		if (nargs == 2)
		{
			luabridge::Stack<Object*>::push(luaVM, rsthis->getObject(
				luabridge::Stack<std::string>::get(luaVM, 2)
				));
		}
        else
        {
            luaL_argerror(luaVM,nargs,"Object:getObject fail");
            return 0;
        }
        return 1;
    }
    int getChild (lua_State* luaVM)
    { 
        int const nargs = lua_gettop (luaVM);
        //good cast
        auto rsthis=((Object*)(this));//1=self/this
        //get obj
        if(nargs==2)
            luabridge::Stack<Object*>::push(luaVM,rsthis->getChild(
                luabridge::Stack<std::string>::get(luaVM,2)
            ));
        else
        {
            luaL_argerror(luaVM,nargs,"Object:getChild fail");
            return 0;
        }
        return 1;
    }
	//add component
	int addComponent(lua_State* luaVM)
	{
		int const nargs = lua_gettop(luaVM);
		//good cast
		auto rsthis = ((Object*)(this));//1=self/this       
		//get obj
		if (nargs == 2)
		{
			RefCountedPtr <Component> ref(luabridge::Stack<Component*>::get(luaVM, 2));
			ref.disable();
			Object::addComponent(ref.get());
		}
		else
		{
			luaL_argerror(luaVM, nargs, "Object:addComponent fail");
		}
		return 0;
	}
	//add component
	int removeComponent(lua_State* luaVM)
	{
		int const nargs = lua_gettop(luaVM);
		//good cast
		auto rsthis = ((Object*)(this));//1=self/this
		//get obj
		if (nargs == 2)
		{
			String cmpName = luabridge::Stack<String>::get(luaVM, 2);
			Component* cmp = Object::removeComponent(cmpName);
			if (cmp)
			{
				RefCountedPtr <Component> ref(cmp); ref.enable();
				luabridge::Stack< RefCountedPtr <Component> >::push(luaVM, ref);
			}
			else lua_pushnil(luaVM);
			return 1;
		}
		else
		{
			luaL_argerror(luaVM, nargs, "Object:eraseChild fail");
			return 0;
		}
	}
    //get name / set name
    int getName(lua_State* luaVM)
    {
        int const nargs = lua_gettop (luaVM);
        //good cast
        auto rsthis=((Object*)(this));//1=self/this
        //get obj
        if(nargs==1)
        {
            luabridge::Stack<const char*>::push(luaVM,rsthis->getName());
            return 1;
        }
        luaL_argerror(luaVM,nargs,"Object:getName fail");
        return 0;
    }
    //get name / set name
    int setName(lua_State* luaVM)
    {
        
        int const nargs = lua_gettop (luaVM);
        //good cast
        auto rsthis=((Object*)(this));//1=self/this
        //get obj
        if(nargs==2)
        {
            rsthis->Easy2D::Object::setName(luabridge::Stack<const char*>::get(luaVM,2));
            return 0;
        }
        luaL_argerror(luaVM,nargs,"Object:setName fail");
        return 0;
    }
    //towatch
    int __towatch (lua_State* luaVM)
    {
        //good cast
        auto rsthis=((Object*)(this));//1=self/this 
        //call
        luabridge::LuaRef t = luabridge::newTable (luaVM);
        t["type"]="Object";
        t["name"]=std::string( rsthis ? rsthis->getName() : "");//no ref!
        t.push(luaVM);
        //
        return 1;
    }
};
/*
template <>
struct luabridge::ContainerConstructionTraits< Texture::ptr >
{
	static Texture::ptr constructContainer(Texture *t)
	{
		return t->getSharedPtr();
	}
};
*/

class LuaResourcesGroup : public ResourcesGroup
{
public:
/*
	Texture::ptr  loadTexture(const String& name)
	{
		return load < Texture >(name);
	} 
*/
};

#define aFUN(args) (void(*)(args))&[](args)

void LuaState::addEasy2DLib()
{
    /** Color class */
    luabridge::getGlobalNamespace(luaVM)
        .beginClass<Color>("Color")
      //.addConstructor <void (*) (void)> ()
        .addStaticCFunction("__call",LuaNewColor)
        .addData("r",&Color::r)
        .addData("g",&Color::g)
        .addData("b",&Color::b)
        .addData("a",&Color::a)
        .addFunction("__add",&Color::operator+)
        .addFunction("__sub",&Color::operator-)
        .addCFunction("__tostring",(int (Color::*) (lua_State*))&LuaColor::__tostring)
        .addCFunction("__towatch",(int (Color::*) (lua_State*))&LuaColor::__towatch)
        .addFunction("toNormalize",&Color::toNormalize)
        .addFunction("fromNormalize",&Color::fromNormalize)
        .addFunction("toVec4",&Color::toVec4)
        .addFunction("fromVec4",&Color::fromVec4);
    
	/** Component */
	luabridge::getGlobalNamespace(luaVM)
		.beginClass<Component>("Component")
		.addConstructor <void(*) (void), RefCountedPtr <Component> >()
		.addFunction<Object* (Component::*)()>("getObject", &Component::getObject)
		.addFunction<Scene* (Component::*)()>("getScene", &Component::getScene)

		.addFunction<void (Component::*)(Object*)>("onSetObject",     &Component::onSetObject)
		.addFunction<void (Component::*)(void)   >("onChangedMatrix", &Component::onChangedMatrix)
		.addFunction<void (Component::*)(void)   >("onEraseObject",   &Component::onEraseObject)

		.addFunction<void (Component::*)(Scene*) >("onSetScene", &Component::onSetScene)
		.addFunction<void (Component::*)(void)   >("onScenePause", &Component::onScenePause)
		.addFunction<void (Component::*)(void)   >("onSceneResume", &Component::onSceneResume)
		.addFunction<void (Component::*)(void)   >("onEraseScene", &Component::onEraseScene)

		.addFunction<void (Component::*)(uint)   >("onMessage", &Component::onMessage)
		.addFunction<void (Component::*)(float)   >("onRun", &Component::onRun)
		.addFunction<void (Component::*)(float)   >("onFixedRun", &Component::onFixedRun)
		.addFunction<const char* (Component::*)(void) const>("getComponentName", &Component::getComponentName)
		.addFunction<uint(Component::*)(void) const>("getComponentName", &Component::getComponentFamily)

		.addFunction<void(Component::*)(Table&)>("serialize", &Component::serialize)
		.addFunction<void(Component::*)(const Table&)>("deserialize", &Component::deserialize);

	/** resource texture */
	luabridge::getGlobalNamespace(luaVM)
		.beginClass<Texture::ptr>("Texture")
		.addConstructor < void(*) (void)>()
		.addFunction("getName", &Texture::getName)
		.addFunction("size", &Texture::getSize)
		.addFunction("load", &Texture::load)
		.addFunction("unload", &Texture::unload);
	/** resource mesh */
	luabridge::getGlobalNamespace(luaVM)
		.beginClass<Mesh::ptr>("Mesh")
		.addConstructor < void(*) (void)>()
		.addFunction("getName", &Mesh::getName)
		.addFunction("addIndex", &Mesh::addIndex)
		.addFunction("addTriangleIndexs", &Mesh::addTriangleIndexs)
		.addFunction("addTriangleIndexs", &Mesh::addQuadIndexs)
		.addFunction<void(Mesh::*)(const Vec4&)>("addVertex", &Mesh::addVertex)
		//todo...
		.addFunction("load", &Mesh::load)
		.addFunction("unload", &Mesh::unload);
	/** resource frame set */
	luabridge::getGlobalNamespace(luaVM)
		.beginClass<FrameSet::ptr>("FrameSet")
		.addConstructor < void(*) (void)>()
		.addFunction("getName", &FrameSet::getName)
		.addFunction("getTexture", &FrameSet::getTexture)
		.addFunction("getDefaultLoop", &FrameSet::getDefaultLoop)
		.addFunction("getDefaultTime", &FrameSet::getDefaultTime)
		.addFunction("getFrame", &FrameSet::getFrame)
		.addFunction("load", &FrameSet::load)
		.addFunction("unload", &FrameSet::unload);

	/** resource font */
	luabridge::getGlobalNamespace(luaVM)
		.beginClass<Font::ptr>("Font")
		.addConstructor < void(*) (void)>()
		.addFunction("getName", &Font::getName)
		.addFunction("getFontName", &Font::getFontName)
		.addFunction("text", &Font::text)
		.addFunction("textSize", &Font::textSize)
		.addFunction("size", &Font::size)
		.addFunction("load", &Font::load)
		.addFunction("unload", &Font::unload);

	/** resource sound */
	luabridge::getGlobalNamespace(luaVM)
		.beginClass<Sound::ptr>("Sound")
		.addConstructor < void(*) (void)>()
		.addFunction("getName", &Sound::getName)
		.addFunction("load", &Sound::load)
		.addFunction("unload", &Sound::unload);

	//typedef LuaShared<Texture>(*sharedLoad)(ResourcesGroup*,const String&);
	/** resource group */
	luabridge::getGlobalNamespace(luaVM)
		.beginClass<ResourcesGroup>("ResourcesGroup")
		.addConstructor < void(ResourcesGroup::*) (const String&, const String&, const String&) >()
		.addFunction("loadTexture", &ResourcesGroup::load<Texture>)
		.addFunction("getTexture",  &ResourcesGroup::get<Texture>)
		.addFunction("findTexture", &ResourcesGroup::find<Texture>)

		.addFunction("loadMesh", &ResourcesGroup::load<Mesh>)
		.addFunction("getMesh", &ResourcesGroup::get<Mesh>)
		.addFunction("findMesh", &ResourcesGroup::find<Mesh>)

		.addFunction("loadFrameSet", &ResourcesGroup::load<FrameSet>)
		.addFunction("getFrameSet", &ResourcesGroup::get<FrameSet>)
		.addFunction("findFrameSet", &ResourcesGroup::find<FrameSet>)

		.addFunction("loadFont", &ResourcesGroup::load<Font>)
		.addFunction("getFont", &ResourcesGroup::get<Font>)
		.addFunction("findFont", &ResourcesGroup::find<Font>)

		.addFunction("loadSound", &ResourcesGroup::load<Sound>)
		.addFunction("getSound", &ResourcesGroup::get<Sound>)
		.addFunction("findSound", &ResourcesGroup::find<Sound>)
		/*
		.addFunction<Script::ptr(ResourcesGroup::*)(const String&)>("loadScript", &ResourcesGroup::load<Script>)
		.addFunction<Script::ptr(ResourcesGroup::*)(const String&)>("getScript", &ResourcesGroup::get<Script>)
		.addFunction<Script::ptr(ResourcesGroup::*)(const String&)>("findScript", &ResourcesGroup::find<Script>)


		.addFunction<Table::ptr(ResourcesGroup::*)(const String&)>("loadTable", &ResourcesGroup::load<Table>)
		.addFunction<Table::ptr(ResourcesGroup::*)(const String&)>("getTable", &ResourcesGroup::get<Table>)
		.addFunction<Table::ptr(ResourcesGroup::*)(const String&)>("findTable", &ResourcesGroup::find<Table>)
		*/
		.addFunction<void(ResourcesGroup::*)(void)>("load", &ResourcesGroup::load)
		.addFunction<void(ResourcesGroup::*)(bool)>("unload", &ResourcesGroup::unload)
		.addFunction<String(ResourcesGroup::*)(void)>("getResourcesDirectory", &ResourcesGroup::getResourcesDirectory);

	/** Object */
    luabridge::getGlobalNamespace(luaVM)
		.beginClass<Object>("Object")
		.addConstructor < void(*) (void), RefCountedPtr <Object> >()
	  //.addConstructor < void(*) (void) >()
        .addCFunction("setPosition",(int (Object::*) (lua_State*))&LuaE2Object::setPosition)
        .addCFunction("setRotation",(int (Object::*) (lua_State*))&LuaE2Object::setRotation)
		.addCFunction("setScale", (int (Object::*) (lua_State*))&LuaE2Object::setScale)
		.addCFunction("setZ", (int (Object::*) (lua_State*))&LuaE2Object::setZ)
		.addFunction("setTranslation", &Object::setTranslation)
		.addFunction("setTurn", &Object::setTurn)
        .addFunction("setMove",&Object::setMove)
        .addCFunction("getPosition",(int (Object::*) (lua_State*))&LuaE2Object::getPosition)
		.addCFunction("getRotation", (int (Object::*) (lua_State*))&LuaE2Object::getRotation)
		.addCFunction("getScale", (int (Object::*) (lua_State*))&LuaE2Object::getScale)
		.addCFunction("getZ", (int (Object::*) (lua_State*))&LuaE2Object::getZ)
		.addCFunction("addChild",  (int (Object::*) (lua_State*))&LuaE2Object::addChild)
		.addCFunction("getObject", (int (Object::*) (lua_State*))&LuaE2Object::getObject)
		.addCFunction("getChild", (int (Object::*) (lua_State*))&LuaE2Object::getChild)
		.addCFunction("eraseChild", (int (Object::*) (lua_State*))&LuaE2Object::eraseChild)
        //names
        .addCFunction("getName",(int (Object::*) (lua_State*))&LuaE2Object::getName)
        .addCFunction("setName",(int (Object::*) (lua_State*))&LuaE2Object::setName)
        //Components
        .addProperty<GeS(Renderable*)>("renderable",getLuaComponent<Renderable>)
        .addProperty<GeS(Emitter*)>("emitter",getLuaComponent<Emitter>)
        .addProperty<GeS(Sprite*)>("sprite",getLuaComponent<Sprite>)
        .addProperty<GeS(AnimatedSprite*)>("animatedSprite",getLuaComponent<AnimatedSprite>)
        //no randerable
        .addProperty<GeS(Parallax*)>("parallax",getLuaComponent<Parallax>)
        .addProperty<GeS(StateManager*)>("stateManager",getLuaComponent<StateManager>)
        .addProperty<GeS(Body*)>("body",getLuaComponent<Body>)
        ///others (unsafe??)
		.addFunction<Component* (LuaE2Object::*)(const Easy2D::String&)>("getComponent", &LuaE2Object::getComponent)
		.addCFunction("addComponent", (int (Object::*) (lua_State*))&LuaE2Object::addComponent)
		.addCFunction("removeComponent", (int (Object::*) (lua_State*))&LuaE2Object::removeComponent)
        //show var
        .addCFunction("__towatch",  (int (Object::*) (lua_State*))&LuaE2Object::__towatch);

	typedef  String(*getStr)();
	typedef  float(*getFloat)();
	typedef  void(*setFloat)(float);
	typedef  bool(*getBool)();
	typedef  void(*setBool)(bool);
	typedef  uint(*getUInt)();
	typedef  void(*setUInt)(uint);
	typedef  Vec2(*getVec2)();
	typedef  void(*setVec2)(Vec2);

	typedef  Vec2(*getKey2Input)(int);
	typedef  bool(*getKeyInput) (int);
	typedef  ResourcesGroup* (*getResourcesGroup) (const char*);

    /// Application
    luabridge::getGlobalNamespace(luaVM)
    .beginNamespace("Application")
        .addFunction("onlyPO2",                (getBool)      ([]()->bool   {  return Application::instance()->onlyPO2(); }))
		.addFunction("get_last_delta_time",	   (getFloat)     ([]()->float  {  return Application::instance()->get_last_delta_time(); }))
		.addFunction("app_data_directory",       (getStr)       ([]()->String {  return Application::instance()->app_data_directory(); }))
        .addFunction("app_working_directory",    (getStr)       ([]()->String {  return Application::instance()->app_working_directory(); }))
		.addFunction("app_resources_directory",  (getStr)       ([]()->String {  return Application::instance()->app_resources_directory(); }))
		.addFunction("get_last_delta_time",	   (getFloat)([]()->float  {  return Application::instance()->get_last_delta_time(); }))
		.addFunction("get_resources_group", 
					(getResourcesGroup)([](const char* name)->ResourcesGroup*  { 
								return Application::instance()->get_resources_group(name);
					}));

    /// Screen
    luabridge::getGlobalNamespace(luaVM)
    .beginNamespace("Screen")
	    .addFunction("set_orientation",    (setUInt)    [](uint e)  {         Application::instance()->get_screen()->set_orientation((Screen::Orientation)e); })
		.addFunction("get_orientation",    (getUInt)    []()->uint  {  return (uint)Application::instance()->get_screen()->get_orientation(); })
		.addFunction("get_width",          (getUInt)    []()->uint  {  return Application::instance()->get_screen()->get_width(); })
		.addFunction("get_height",		  (getUInt)    []()->uint  {  return Application::instance()->get_screen()->get_height(); })
		.addFunction("get_native_width",    (getUInt)    []()->uint  {  return Application::instance()->get_screen()->get_native_width(); })
		.addFunction("get_native_height",   (getUInt)    []()->uint  {  return Application::instance()->get_screen()->get_native_height(); })
        .addFunction("getSize",           (getVec2)    []()->Vec2  {  return Application::instance()->get_screen()->get_size(); })
		.addFunction("set_cursor",		  (setBool)    [](bool e)  {         Application::instance()->get_screen()->set_cursor(e); })
		.addFunction("get_cursor",         (getBool)      []()->bool  {  return Application::instance()->get_screen()->get_cursor(); })
		.addFunction("set_position_cursor", (setVec2)      [](Vec2 v)  {         Application::instance()->get_screen()->set_position_cursor(v); })
        .addFunction("set_fullscreen",     (setBool)      [](bool f)  {         Application::instance()->get_screen()->set_fullscreen(f); })
		.addFunction("isFullscreen",      (getBool)      []()->bool  {  return Application::instance()->get_screen()->is_fullscreen(); });

    /// Input
    luabridge::getGlobalNamespace(luaVM)
        .beginNamespace("Input")
		.addFunction("get_mouse",         (getVec2)[]()->Vec2            {  return Application::instance()->get_input()->get_mouse(); })
		.addFunction("get_finger",   (getKey2Input)[](int finger)->Vec2  {  return Application::instance()->get_input()->get_finger((Key::Finger)finger); })
		.addFunction("get_key_down",   (getKeyInput)[](int key)->bool     {  return Application::instance()->get_input()->get_key_down((Key::Keyboard)key); })
		.addFunction("get_key_up",     (getKeyInput)[](int key)->bool     {  return Application::instance()->get_input()->get_key_up((Key::Keyboard)key); })
		.addFunction("get_key_hit",    (getKeyInput)[](int key)->bool     {  return Application::instance()->get_input()->get_key_hit((Key::Keyboard)key); })
		.addFunction("get_mouse_down", (getKeyInput)[](int key)->bool     {  return Application::instance()->get_input()->get_mouse_down((Key::Mouse)key); })
		.addFunction("get_mouse_up",   (getKeyInput)[](int key)->bool     {  return Application::instance()->get_input()->get_mouse_up((Key::Mouse)key); })
		.addFunction("get_mouse_hit",  (getKeyInput)[](int key)->bool     {  return Application::instance()->get_input()->get_mouse_hit((Key::Mouse)key); })
		.addFunction("get_finger_down",(getKeyInput)[](int key)->bool     {  return Application::instance()->get_input()->get_finger_down((Key::Finger)key); })
		.addFunction("get_finger_up",  (getKeyInput)[](int key)->bool     {  return Application::instance()->get_input()->get_finger_up((Key::Finger)key); })
		.addFunction("get_finger_hit", (getKeyInput)[](int key)->bool     {  return Application::instance()->get_input()->get_finger_hit((Key::Finger)key); })
		.addFunction("get_focus",        (getBool) []()->bool            {  return Application::instance()->get_input()->get_focus(); })
		.addFunction("get_minimized",    (getBool) []()->bool            {  return Application::instance()->get_input()->get_minimized(); })
		.addFunction("get_maximized",    (getBool) []()->bool            {  return Application::instance()->get_input()->get_maximized(); })
		.addFunction("get_close",        (getBool) []()->bool            {  return Application::instance()->get_input()->get_close(); })
		.addFunction("get_resize",       (getBool) []()->bool            {  return Application::instance()->get_input()->get_resize(); })
		.addFunction("get_resize_values", (getVec2) []()->Vec2            {  return Application::instance()->get_input()->get_resize_values(); })
      //.addFunction("getAccelerometer",[]()->uint            {  return Application::instance()->get_screen()->getAccelerometer(); })
        ;

}
void LuaState::addComponentsLib()
{
    addRenderableLib();
    addBodyLib();
}