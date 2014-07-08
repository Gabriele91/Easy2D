#include <stdafx.h>
#include <ELuaState.h>
#include <LuaBridge/LuaBridge.h>
#include <Renderable.h>
#include <Sprite.h>
#include <AnimatedSprite.h>
#include <ParticleSystem.h>
#include <Body.h>
#include <State.h>
#include <Parallax.h>

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

class LuaRenderable : public Renderable
{
    public:
    int getLuaColor (lua_State* luaVM)
    { 
        //good cast
        auto rsthis=((RenderState*)(this));
        //push
        luabridge::Stack<Color>::push(luaVM,rsthis->getColor());
        //return
        return 1; 
    }
    int setLuaColor (lua_State* luaVM)
    { 
        //good cast
        auto rsthis=((RenderState*)(this));
        //get
        rsthis->setColor(luabridge::Stack<Color>::get(luaVM,2));//1=self/this
        //return
        return 0; 
    }
    int enableLuaBlend (lua_State* luaVM)
    {
        //good cast
        auto rsthis=((RenderState*)(this));
        //call
        rsthis->enableBlend();
        //
        return 0;
    }
    int disableLuaBlend (lua_State* luaVM)
    {
        //good cast
        auto rsthis=((RenderState*)(this));
        //call
        rsthis->disbaleBlend();
        //
        return 0;
    }
    int setLuaBlend (lua_State* luaVM)
    {
        //good cast
        auto rsthis=((RenderState*)(this));//1=self/this
        //call
        rsthis->setBlend(luabridge::Stack<int>::get(luaVM,2),
                         luabridge::Stack<int>::get(luaVM,3));
        //
        return 0;
    }
    //towatch
    int __towatch (lua_State* luaVM)
    {
        //good cast
        auto rsthis=((Renderable*)(this));//1=self/this
        //call
        luabridge::LuaRef t = luabridge::newTable (luaVM);
        t["type"]="Renderable";
        t.push(luaVM);
        //
        return 1;
    }
};

class LuaAnimatedSprite : public AnimatedSprite
{
    public:
    /*
    (not supported in script)
    int addAnimation(FrameSet::ptr frames);
    int addAnimation(FrameSet::ptr frames, float time);
    int addAnimation(FrameSet::ptr frames, float time,bool loop);
    */
    int addAnimation (lua_State* luaVM)
    { 
        int const nargs = lua_gettop (luaVM);
        //good cast
        auto rsthis=((AnimatedSprite*)(this));//1=self/this
        //unsupported
        luaL_argerror(luaVM,nargs,"AnimatedSprite:addAnimation not support");
        //
        if(lua_isuserdata(luaVM,2) && nargs==2)
        {
           // rsthis->addAnimation(luabridge::Stack <FrameSet>::get(luaVM,2));
        }
        else
        if(lua_isuserdata(luaVM,2) && lua_isnumber(luaVM,3) && nargs==3)
        {
           // rsthis->setAnimation(luabridge::Stack <FrameSet>::get(luaVM,2),
           //                      luabridge::Stack <float>::get(luaVM,3));
        }
        else
        if(lua_isuserdata(luaVM,2) && lua_isnumber(luaVM,3) && lua_isboolean(luaVM,4) && nargs==4)
        {
           // rsthis->setAnimation(luabridge::Stack <FrameSet>::get(luaVM,2),
           //                      luabridge::Stack <float>::get(luaVM,3),
           //                      luabridge::Stack <bool>::get(luaVM,4));
        }
        else
        {
            luaL_argerror(luaVM,nargs,"AnimatedSprite:addAnimation fail");
        } 
        return 0;
    }
    /*
    void setAnimation(int i);
    void setAnimation(int i, float timePerFrame);
    void setAnimation(int i, bool  loop);
    void setAnimation(int i, float timePerFrame, bool  loop);
    */
    int setAnimation (lua_State* luaVM)
    { 
        int const nargs = lua_gettop (luaVM);
        //good cast
        auto rsthis=((AnimatedSprite*)(this));//1=self/this
        //
        if(lua_isnumber(luaVM,2) && nargs==2)
        {
            rsthis->setAnimation(luabridge::Stack <int>::get(luaVM,2));
        }
        else
        if(lua_isnumber(luaVM,2) && lua_isnumber(luaVM,3) && nargs==3)
        {
            rsthis->setAnimation(luabridge::Stack <int>::get(luaVM,2),
                                 luabridge::Stack <float>::get(luaVM,3));
        }
        else
        if(lua_isnumber(luaVM,2) && lua_isboolean(luaVM,3) && nargs==3)
        {
            rsthis->setAnimation(luabridge::Stack <int>::get(luaVM,2),
                                 luabridge::Stack <bool>::get(luaVM,3));
        }
        else
        if(lua_isnumber(luaVM,2) && lua_isnumber(luaVM,3) && lua_isboolean(luaVM,4) && nargs==4)
        {
            rsthis->setAnimation(luabridge::Stack <int>::get(luaVM,2),
                                 luabridge::Stack <float>::get(luaVM,3),
                                 luabridge::Stack <bool>::get(luaVM,4));
        }
        else
        {
            luaL_argerror(luaVM,nargs,"AnimatedSprite:setAnimation fail");
        } 
        return 0;
    }
    //towatch
    int __towatch (lua_State* luaVM)
    {
        //good cast
        auto rsthis=((Renderable*)(this));//1=self/this
        //call
        luabridge::LuaRef t = luabridge::newTable (luaVM);
        t["type"]="AnimatedSprite";
        t.push(luaVM);
        //
        return 1;
    }
};


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
        .addProperty<GeS(Renderable*)>("renderable",getLuaComponent<Renderable>)
        .addProperty<GeS(Emitter*)>("emitter",getLuaComponent<Emitter>)
        .addProperty<GeS(Sprite*)>("sprite",getLuaComponent<Sprite>)
        .addProperty<GeS(AnimatedSprite*)>("animatedSprite",getLuaComponent<AnimatedSprite>)
        //no randerable
        .addProperty<GeS(Parallax*)>("parallax",getLuaComponent<Parallax>)
        .addProperty<GeS(StateManager*)>("stateManager",getLuaComponent<StateManager>)
        .addProperty<GeS(Body*)>("body",getLuaComponent<Body>)
        //show var
        .addCFunction("__towatch",  (int (Object::*) (lua_State*))&LuaE2Object::__towatch);

}
void LuaState::addComponentsLib()
{
    /** Randerable class */
    luabridge::getGlobalNamespace(luaVM)
        .beginClass <Renderable>("Renderable")
        .addConstructor <void (*) (void)> ()
        .addFunction("canBatching",&Renderable::canBatching)
        .addFunction("canTransform",&Renderable::canTransform)
        .addFunction("getObject",&Renderable::getObject)
        .addFunction("show",&Renderable::show)
        .addFunction("hide",&Renderable::hide)
        .addFunction("isVisible",&Renderable::isVisible)
        .addFunction("isAdded",&Renderable::isAdded)
        /////////////////////////////////////////////////
        .addCFunction("getColor",(int (Renderable::*) (lua_State*))&LuaRenderable::getLuaColor)
        .addCFunction("setColor",(int (Renderable::*) (lua_State*))&LuaRenderable::setLuaColor)
        /////////////////////////////////////////////////
        .addFunction("enableBlend",(int (Renderable::*) (lua_State*))&LuaRenderable::enableLuaBlend)
        .addFunction("disbaleBlend",(int (Renderable::*) (lua_State*))&LuaRenderable::disableLuaBlend)
        .addFunction("setBlend",(int (Renderable::*) (lua_State*))&LuaRenderable::setLuaBlend)
        .addCFunction("__towatch",  (int (Renderable::*) (lua_State*))&LuaRenderable::__towatch);
        /////////////////////////////////////////////////

    /** Emitter class */
    luabridge::getGlobalNamespace(luaVM)
        .deriveClass <Emitter,Renderable> ("Emitter")
        .addConstructor <void (*) (void)> ()
        
        //set duration
        .addFunction("setDuration",&Emitter::setDuration)
        .addFunction("getDuration",&Emitter::getDuration)
        //relative
        .addFunction("getRelative",&Emitter::getRelative)
        .addFunction("setRelative",&Emitter::setRelative)
        //gravity
        .addFunction("setGravity",&Emitter::setGravity)
        .addFunction("getGravity",&Emitter::getGravity)
        //info 
        .addFunction("setPause",&Emitter::setPause)
        .addFunction("getPause",&Emitter::getPause)
        .addFunction("setStop",&Emitter::setStop)
        .addFunction("getStop",&Emitter::getStop)
        .addFunction("restart",&Emitter::restart)
        //emission
        .addFunction("setMaxParticles",&Emitter::setMaxParticles)
        .addFunction("getMaxParticles",&Emitter::getMaxParticles)
        .addFunction("setEmissionRate",&Emitter::setEmissionRate)
        .addFunction("getEmissionRate",&Emitter::getEmissionRate)
        //life
        .addFunction("setLife",&Emitter::setLife)
        .addFunction("setLifeVar",&Emitter::setLifeVar)
        .addFunction("getLife",&Emitter::getLife)
        .addFunction("getLifeVar",&Emitter::getLifeVar)
        //position
        .addFunction("setPosition",&Emitter::setPosition)
        .addFunction("setPositionVar",&Emitter::setPositionVar)
        .addFunction("getPosition",&Emitter::getPosition)
        .addFunction("getPositionVar",&Emitter::getPositionVar)
        //direction
        .addFunction("setDirection",&Emitter::setDirection)
        .addFunction("setDirectionVar",&Emitter::setDirectionVar)
        .addFunction("getDirection",&Emitter::getDirection)
        .addFunction("getDirectionVar",&Emitter::getDirectionVar)
        //scale
        .addFunction("setStartScale",&Emitter::setStartScale)
        .addFunction("setStartScaleVar",&Emitter::setStartScaleVar)
        .addFunction("getStartScale",&Emitter::getStartScale)
        .addFunction("getStartScaleVar",&Emitter::getStartScaleVar)
        .addFunction("setEndScale",&Emitter::setEndScale)
        .addFunction("setEndScaleVar",&Emitter::setEndScaleVar)
        .addFunction("getEndScale",&Emitter::getEndScale)
        .addFunction("getEndScaleVar",&Emitter::getEndScaleVar)
        //color
        .addFunction("setStartColor",&Emitter::setStartColor)
        .addFunction("setStartColorVar",&Emitter::setStartColorVar)
        .addFunction("getStartColor",&Emitter::getStartColor)
        .addFunction("getStartColorVar",&Emitter::getStartColorVar)
        .addFunction("setEndColor",&Emitter::setEndColor)
        .addFunction("setEndColorVar",&Emitter::setEndColorVar)
        .addFunction("getEndColor",&Emitter::getEndColor)
        .addFunction("getEndColorVar",&Emitter::getEndColorVar)
        //spin
        .addFunction("setStartSpin",&Emitter::setStartSpin)
        .addFunction("setStartSpinVar",&Emitter::setStartSpinVar)
        .addFunction("getStartSpin",&Emitter::getStartSpin)
        .addFunction("getStartSpinVar",&Emitter::getStartSpinVar)
        .addFunction("setEndSpin",&Emitter::setEndSpin)
        .addFunction("setEndSpinVar",&Emitter::setEndSpinVar)
        .addFunction("getEndSpin",&Emitter::getEndSpin)
        .addFunction("getEndSpinVar",&Emitter::getEndSpinVar);
        /////////////////////////////////////////////////
    
    /** Sprite class */
    luabridge::getGlobalNamespace(luaVM)
        .deriveClass <Sprite,Renderable> ("Sprite")
        .addConstructor <void (*) (void)> ()
        .addFunction("getPixelScale",&Sprite::getPixelScale);
        /////////////////////////////////////////////////

    /** AnimatedSprite */
    luabridge::getGlobalNamespace(luaVM)
        .deriveClass <AnimatedSprite,Renderable> ("Sprite")
        .addConstructor <void (*) (void)> () //set animation frame
        .addFunction("setFrame",&AnimatedSprite::setFrame)
        //add an animation
        .addCFunction("addAnimation",(int (AnimatedSprite::*) (lua_State*))&LuaAnimatedSprite::addAnimation)
        //change an animation
        .addCFunction("setAnimation",(int (AnimatedSprite::*) (lua_State*))&LuaAnimatedSprite::setAnimation)
        //change time animation
        .addFunction("setTime",&AnimatedSprite::setTime)
        .addFunction("setChangeTime",&AnimatedSprite::setChangeTime)
        .addFunction("setPause",&AnimatedSprite::setPause)
        .addFunction("setLoop",&AnimatedSprite::setLoop)
        .addFunction("setAnimationTime",&AnimatedSprite::setAnimationTime)
        .addFunction("setAnimationPause",&AnimatedSprite::setAnimationPause)
        .addFunction("setAnimationLoop",&AnimatedSprite::setAnimationLoop)
        //restart
        .addFunction("currentRestart",&AnimatedSprite::currentRestart)
        //query
        .addFunction("getCurrentAnimation",&AnimatedSprite::getCurrentAnimation)
        .addFunction("getCurrentTimePerFrame",&AnimatedSprite::getCurrentTimePerFrame)
        .addFunction("getCurrentTotalTime",&AnimatedSprite::getCurrentTotalTime)
        .addFunction("getCurrentTime",&AnimatedSprite::getCurrentTime)
        .addFunction("getCurrentLoop",&AnimatedSprite::getCurrentLoop)
        .addFunction("getCurrentPause",&AnimatedSprite::getCurrentPause)
        .addFunction("getCurrentIsStop",&AnimatedSprite::getCurrentIsStop)
        //get pixel scale
        .addFunction("getBoxScale",&AnimatedSprite::getBoxScale)
        .addCFunction("__towatch",  (int (AnimatedSprite::*) (lua_State*))&LuaAnimatedSprite::__towatch);
        /////////////////////////////////////////////////
    /** Body */
}