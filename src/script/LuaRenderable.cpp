#include <stdafx.h>
#include <Renderable.h>
#include <Sprite.h>
#include <AnimatedSprite.h>
#include <ParticleSystem.h>
#include <ELuaState.h>
#include <LuaBridge/LuaBridge.h>
////////////////////////
using namespace Easy2D;

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
    /*
     void setFrame(int frame)
     void setFrame(int animation,int frame);
     */
    int setFrame (lua_State* luaVM)
    {
        int const nargs = lua_gettop (luaVM);
        //good cast
        auto rsthis=((AnimatedSprite*)(this));//1=self/this
        //args
        if(lua_isnumber(luaVM,2) && nargs==2)
        {
            rsthis->setFrame(luabridge::Stack <int>::get(luaVM,2));
        }
        else
            if(lua_isnumber(luaVM,2) && lua_isnumber(luaVM,3) && nargs==3)
            {
                rsthis->setFrame(luabridge::Stack <int>::get(luaVM,2),
                                 luabridge::Stack <int>::get(luaVM,3));
            }
            else
            {
                luaL_argerror(luaVM,nargs,"AnimatedSprite:setFrame fail");
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



void LuaState::addRenderableLib()
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
    .deriveClass <AnimatedSprite,Renderable> ("AnimatedSprite")
    .addConstructor <void (*) (void)> () //set animation frame
    .addCFunction("setFrame",(int (AnimatedSprite::*) (lua_State*))&LuaAnimatedSprite::setFrame)
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
    .addFunction("getCurrentStop",&AnimatedSprite::getCurrentStop)
    
    .addFunction("getTimePerFrame",&AnimatedSprite::getTimePerFrame)
    .addFunction("getTotalTime",&AnimatedSprite::getTotalTime)
    .addFunction("getTime",&AnimatedSprite::getTime)
    .addFunction("getLoop",&AnimatedSprite::getLoop)
    .addFunction("getPause",&AnimatedSprite::getPause)
    .addFunction("getStop",&AnimatedSprite::getStop)
    //get pixel scale
    .addFunction("getBoxScale",&AnimatedSprite::getBoxScale)
    .addCFunction("__towatch",  (int (AnimatedSprite::*) (lua_State*))&LuaAnimatedSprite::__towatch);
    /////////////////////////////////////////////////
}