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

int LuaNewColor(lua_State* lVM)
{  
    int const nargs = lua_gettop (lVM);
    //push a new vec4
    if(!nargs)
    {
        luabridge::Stack <Color>::push(lVM,Color());
    }
    else
    if(lua_isuserdata(lVM,1) && nargs==1)
    {
        luabridge::Stack <Color>::push(lVM, 
            Color(luabridge::Stack <Color>::get(lVM, 1)));
    }
    else
    if(lua_isnumber(lVM,1) && 
       lua_isnumber(lVM,2) && 
       lua_isnumber(lVM,3) && 
       lua_isnumber(lVM,4) && nargs==4)
    {
        luabridge::Stack <Color>::push(lVM,
           Color(luabridge::Stack <uchar>::get(lVM, 1),
                 luabridge::Stack <uchar>::get(lVM, 2),
                 luabridge::Stack <uchar>::get(lVM, 3),
                 luabridge::Stack <uchar>::get(lVM, 4)));
    }
    else
    {
        luaL_argerror(lVM,nargs,"Color.new fail");
        return 0;
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
    int setRotation (lua_State* luaVM)
    { 
        int const nargs = lua_gettop (luaVM);
        //good cast
        auto rsthis=((Object*)(this));//1=self/this
        //set scale
        if(nargs==2)
            rsthis->setRotation(luabridge::Stack<float>::get(luaVM,2));
        else
        if(nargs==3)
            rsthis->setRotation(luabridge::Stack<float>::get(luaVM,2),
                                luabridge::Stack<bool>::get(luaVM,2));
        else
            luaL_argerror(luaVM,nargs,"Object:setRotation fail");

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
    int getRotation (lua_State* luaVM)
    { 
        int const nargs = lua_gettop (luaVM);
        //good cast
        auto rsthis=((Object*)(this));//1=self/this
        //get scale
        if(nargs==1)
            luabridge::Stack<float>::push(luaVM,rsthis->getRotation());
        else
        if(nargs==2)
            luabridge::Stack<float>::push(luaVM,rsthis->getRotation(
                luabridge::Stack<bool>::get(luaVM,2)
            ));
        else
        {
            luaL_argerror(luaVM,nargs,"Object:getRotation fail");
            return 0;
        }
        return 1;
    }
    /*
    Object* getObject(string)
    Object* getChild(string)
    */
    int getObject (lua_State* luaVM)
    { 
        int const nargs = lua_gettop (luaVM);
        //good cast
        auto rsthis=((Object*)(this));//1=self/this
        //get obj
        if(nargs==2)
            luabridge::Stack<Object*>::push(luaVM,rsthis->getObject(
                luabridge::Stack<std::string>::get(luaVM,2)
            ));
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

#define aFUN(args) (void(*)(args))&[](args)

void LuaState::addEasy2DLib()
{
    /** Color class */
    luabridge::getGlobalNamespace(luaVM)
        .beginClass<Color>("Color")
        .addConstructor <void (*) (void)> ()
        .addStaticCFunction("new",LuaNewColor)
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
    
    
    /** Object */
    luabridge::getGlobalNamespace(luaVM)
        .beginClass<Object>("Object")
        .addConstructor <void (*) (void)> ()
        .addCFunction("setPosition",(int (Object::*) (lua_State*))&LuaE2Object::setPosition)
        .addCFunction("setRotation",(int (Object::*) (lua_State*))&LuaE2Object::setRotation)
        .addCFunction("setScale",   (int (Object::*) (lua_State*))&LuaE2Object::setScale)
        .addFunction("setTranslation",&Object::setTranslation)
        .addFunction("setTurn",&Object::setTurn)
        .addFunction("setMove",&Object::setMove)
        .addCFunction("getPosition",(int (Object::*) (lua_State*))&LuaE2Object::getPosition)
        .addCFunction("getRotation",(int (Object::*) (lua_State*))&LuaE2Object::getRotation)
        .addCFunction("getScale",   (int (Object::*) (lua_State*))&LuaE2Object::getScale)
        .addCFunction("getObject",  (int (Object::*) (lua_State*))&LuaE2Object::getObject)
        .addCFunction("getChild",   (int (Object::*) (lua_State*))&LuaE2Object::getChild)
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
        //show var
        .addCFunction("__towatch",  (int (Object::*) (lua_State*))&LuaE2Object::__towatch);
    
    /// Application
    luabridge::getGlobalNamespace(luaVM)
    .beginNamespace("Application")
        .addFunction("onlyPO2",               (bool(*)())    []()->bool   {  return Application::instance()->onlyPO2(); })
        .addFunction("getLastDeltaTime",      (float(*)())   []()->float  {  return Application::instance()->getLastDeltaTime(); })
        .addFunction("appDataDirectory",      (String(*)())  []()->String {  return Application::instance()->appDataDirectory(); })
        .addFunction("appWorkingDirectory",   (String(*)())  []()->String {  return Application::instance()->appWorkingDirectory(); })
        .addFunction("appResourcesDirectory", (String(*)())  []()->String {  return Application::instance()->appResourcesDirectory(); })
        .addFunction("getLastDeltaTime",      (float(*)())   []()->float  {  return Application::instance()->getLastDeltaTime(); });

    /// Screen
    luabridge::getGlobalNamespace(luaVM)
    .beginNamespace("Screen")
        .addFunction("setOrientation",    (void(*)(uint))[](uint e)  {         Application::instance()->getScreen()->setOrientation((Screen::Orientation)e); })
        .addFunction("getOrientation",    (uint(*)())    []()->uint  {  return (uint)Application::instance()->getScreen()->getOrientation(); })
        .addFunction("getWidth",          (uint(*)())    []()->uint  {  return Application::instance()->getScreen()->getWidth(); })
        .addFunction("getHeight",         (uint(*)())    []()->uint  {  return Application::instance()->getScreen()->getHeight(); })
        .addFunction("getNativeWidth",    (uint(*)())    []()->uint  {  return Application::instance()->getScreen()->getNativeWidth(); })
        .addFunction("getNativeHeight",   (uint(*)())    []()->uint  {  return Application::instance()->getScreen()->getNativeHeight(); })
        .addFunction("getSize",           (Vec2(*)())    []()->Vec2  {  return Application::instance()->getScreen()->getSize(); })
        .addFunction("setCursor",         (void(*)(bool))[](bool e)  {         Application::instance()->getScreen()->setCursor(e); })
        .addFunction("getCursor",         (bool(*)())    []()->bool  {  return Application::instance()->getScreen()->getCursor(); })
        .addFunction("setPositionCursor", (void(*)(Vec2))[](Vec2 v)  {         Application::instance()->getScreen()->setPositionCursor(v); })
        .addFunction("setFullscreen",     (void(*)(bool))[](bool f)  {         Application::instance()->getScreen()->setFullscreen(f); })
        .addFunction("isFullscreen",      (bool(*)())    []()->bool  {  return Application::instance()->getScreen()->isFullscreen(); });
    /*
    /// Input
    luabridge::getGlobalNamespace(luaVM)
        .beginNamespace("Input")
        .addFunction("getMouse",         []()->Vec2            {  return Application::instance()->getInput()->getMouse(); })
        .addFunction("getFinger",        [](int finger)->Vec2  {  return Application::instance()->getInput()->getFinger((Key::Finger)finger); })
        .addFunction("getKeyDown",       [](int key)->bool     {  return Application::instance()->getInput()->getKeyDown((Key::Keyboard)key); })
        .addFunction("getKeyUp",         [](int key)->bool     {  return Application::instance()->getInput()->getKeyUp((Key::Keyboard)key); })
        .addFunction("getKeyHit",        [](int key)->bool     {  return Application::instance()->getInput()->getKeyHit((Key::Keyboard)key); })
        .addFunction("getMouseDown",     [](int key)->bool     {  return Application::instance()->getInput()->getMouseDown((Key::Mouse)key); })
        .addFunction("getMouseUp",       [](int key)->bool     {  return Application::instance()->getInput()->getMouseUp((Key::Mouse)key); })
        .addFunction("getMouseHit",      [](int key)->bool     {  return Application::instance()->getInput()->getMouseHit((Key::Mouse)key); })
        .addFunction("getFingerDown",    [](int key)->bool     {  return Application::instance()->getInput()->getFingerDown((Key::Finger)key); })
        .addFunction("getFingerUp",      [](int key)->bool     {  return Application::instance()->getInput()->getFingerUp((Key::Finger)key); })
        .addFunction("getFingerHit",     [](int key)->bool     {  return Application::instance()->getInput()->getFingerHit((Key::Finger)key); })
        .addFunction("getFocus",         []()->bool            {  return Application::instance()->getInput()->getFocus(); })
        .addFunction("getMinimized",     []()->bool            {  return Application::instance()->getInput()->getMinimized(); })
        .addFunction("getMaximized",     []()->bool            {  return Application::instance()->getInput()->getMaximized(); })
        .addFunction("getClose",         []()->bool            {  return Application::instance()->getInput()->getClose(); })
        .addFunction("getResize",        []()->bool            {  return Application::instance()->getInput()->getResize(); })
        .addFunction("getResizeValues",  []()->Vec2            {  return Application::instance()->getInput()->getResizeValues(); })
      //.addFunction("getAccelerometer",[]()->uint            {  return Application::instance()->getScreen()->getAccelerometer(); })
        ;
    */

}
void LuaState::addComponentsLib()
{
    addRenderableLib();
    addBodyLib();
}