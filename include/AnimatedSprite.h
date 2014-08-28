#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include <Config.h>
#include <Math3D.h>
#include <Object.h>
#include <Renderable.h>
#include <FrameSet.h>
#include <Debug.h>

namespace Easy2D
{

class Animation
{

    //save frame set
    FrameSet::ptr frames;
    //frame info
    float animationTime,
          totalTime,
          timePerFrame;
    int   currentFrame;
    bool  loop;
    bool  pause;

public:
    //costructor
    Animation( FrameSet::ptr frameset, float timePerFrame, bool loop)
    {
        setFrameSet(frameset,timePerFrame,loop);
    }
    //animation update
    void setFrame(float dt)
    {

        DEBUG_ASSERT( frames!=NULL );
        if( timePerFrame == 0 )return;
        if( frames->size() <= 1 ) return;
        if( isStop() ) return;
        //new time
        animationTime = dt;
        //update
        while( animationTime >= timePerFrame )
        {
            ++currentFrame;
            animationTime -= timePerFrame;
        }
        //fix last frame in next loop
        while( animationTime < 0 )
            animationTime += totalTime;
        //is loop?
        if(loop)
            currentFrame%=frames->size();
        else if(currentFrame>lastFrame())
            currentFrame=lastFrame();
    }
    //next frame
    void update(float dt)
    {
        if(!pause)
            setFrame(animationTime+dt);
    }
    //setters
    void setFrameSet( FrameSet::ptr frameset, 
                      float tPerFrame,
                      bool  doloop,
                      bool  dopause=false)
    {
        loop = doloop;
        pause = dopause;
        frames = frameset;
        setFrameTime(tPerFrame);
    }
    void setFrameTime( float time )
    {
        DEBUG_ASSERT( frames );
        DEBUG_ASSERT( time >= 0 );
        animationTime = 0;
        timePerFrame =  time / frames->size();
        currentFrame = 0;
        totalTime = time;
    }
    void setChangeFrameTime( float time )
    {
        DEBUG_ASSERT( frames );
        DEBUG_ASSERT( time >= 0 );
        timePerFrame =  time / frames->size();
        totalTime = time;
    }
    void setForcedFrame(int i)
    {
        currentFrame=i%frames->size();
    }
    void setLoop(bool doloop)
    {
        loop=doloop;
    }
    void setPause(bool dopause)
    {
        pause=dopause;
    }
    //if is stopped
    bool isStop()
    {
        return !loop && currentFrame==lastFrame();
    }
    void restart()
    {
        currentFrame=0;
    }
    int lastFrame()
    {
        return frames->size()-1;
    }
    //getters
    DFORCEINLINE FrameSet::ptr getFrameSet()
    {
        return frames;
    }
    DFORCEINLINE Mesh::ptr getCurrentFrameMesh()
    {
        return frames->getFrame(currentFrame);
    }
    DFORCEINLINE Texture::ptr getTexture()
    {
        return frames->getTexture();
    }
    DFORCEINLINE int getCurrentFrame()
    {
        return currentFrame;
    }
    DFORCEINLINE Vec2 getCurrentFrameSize()
    {
        return frames->sizeFrame(currentFrame);
    }
    DFORCEINLINE float getCurrentTime()
    {
        return animationTime;
    }
    DFORCEINLINE float getTimePerFrame()
    {
        return timePerFrame;
    }
    DFORCEINLINE float getTotalTime()
    {
        return totalTime;
    }
    DFORCEINLINE bool getLoop()
    {
        return loop;
    }
    DFORCEINLINE bool getPause()
    {
        return pause;
    }

};

class AnimatedSprite : public Renderable
{

protected:
    //protected set mesh
    DFORCEINLINE void setMesh(Mesh::ptr mesh)
    {
        Renderable::setMesh(mesh);
    }
    //animations
    std::vector<Animation*> animations;
    int crtAnimation;

public:

    //costructor
    AnimatedSprite();
    virtual ~AnimatedSprite();
    //set animation frame
    void setFrame(int frame);
    void setFrame(int animation,int frame);
    //add an animation
    int addAnimation(FrameSet::ptr frames);
    int addAnimation(FrameSet::ptr frames, float time);
    int addAnimation(FrameSet::ptr frames, float time,bool loop);
    void clearAnimations();
    //return frameset list
    void getFrameSets(std::vector<FrameSet::ptr>& frames);
    //change an animation
    void setAnimation(int i);
    void setAnimation(int i, float timePerFrame);
    void setAnimation(int i, bool  loop);
    void setAnimation(int i, float timePerFrame, bool  loop);
    //change time animation
    void setTime(float timePerFrame);
    void setChangeTime(float timePerFrame);
    void setPause(bool pause);
    void setLoop(bool loop);
    void setAnimationTime(int i, float time);
    void setAnimationPause(int i, bool pause);
    void setAnimationLoop(int i, bool loop);
    //restart
    void currentRestart()
    {
        return animations[crtAnimation]->restart();
    }
    //info
    int countAnimations()
    {
        return animations.size();
    }
    //query
    int getCurrentAnimation()
    {
        return crtAnimation;
    }
    int getCurrentFrame()
    {
        DEBUG_ASSERT(animations.size());
        return animations[crtAnimation]->getCurrentFrame();
    }
    float getCurrentTimePerFrame()
    {
        DEBUG_ASSERT(animations.size());
        return animations[crtAnimation]->getTimePerFrame();
    }
    float getCurrentTotalTime()
    {
        DEBUG_ASSERT(animations.size());
        return animations[crtAnimation]->getTotalTime();
    }
    float getCurrentTime()
    {
        DEBUG_ASSERT(animations.size());
        return animations[crtAnimation]->getCurrentTime();
    }
    bool getCurrentLoop()
    {
        DEBUG_ASSERT(animations.size());
        return animations[crtAnimation]->getLoop();
    }
    bool getCurrentPause()
    {
        DEBUG_ASSERT(animations.size());
        return animations[crtAnimation]->getPause();
    }
    bool getCurrentStop()
    {
        DEBUG_ASSERT(animations.size());
        return animations[crtAnimation]->isStop();
    }
    int getCurrentAnimationSize()
    {
        DEBUG_ASSERT(animations.size());
        return animations[crtAnimation]->getFrameSet()->size();
    }
    
    //query oid
    int getCurrentFrame(int anim)
    {
        DEBUG_ASSERT(anim<animations.size());
        return animations[anim]->getCurrentFrame();
    }
    float getTimePerFrame(int anim)
    {
        DEBUG_ASSERT(anim<animations.size());
        return animations[anim]->getTimePerFrame();
    }
    float getTotalTime(int anim)
    {
        DEBUG_ASSERT(anim<animations.size());
        return animations[anim]->getTotalTime();
    }
    float getTime(int anim)
    {
        DEBUG_ASSERT(anim<animations.size());
        return animations[anim]->getCurrentTime();
    }
    bool getLoop(int anim)
    {
        DEBUG_ASSERT(anim<animations.size());
        return animations[anim]->getLoop();
    }
    bool getPause(int anim)
    {
        DEBUG_ASSERT(anim<animations.size());
        return animations[anim]->getPause();
    }
    bool getStop(int anim)
    {
        DEBUG_ASSERT(anim<animations.size());
        return animations[anim]->isStop();
    }
    int getAnimationSize(int anim)
    {
        DEBUG_ASSERT(anim<animations.size());
        return animations[anim]->getFrameSet()->size();
    }
    //get pixel scale
    Vec2 getBoxScale()
    {
        //get box
        const AABox2& box=getMesh()->getAABox();
        Vec2 scale=box.getMax()-box.getMin();
        return scale*getObject()->getScale();
    }
    //component
    virtual void onRun(float dt)
    {
        if(animations.size())
        {
            animations[crtAnimation]->update(dt);
            //set sprite mesh
            setMesh(animations[crtAnimation]->getCurrentFrameMesh());
        }
    }
    //is a component
    DERIVATE_COMPONENT(AnimatedSprite)
    //serialize/deserialize
    virtual void serialize(Table& table);
    virtual void deserialize(const Table& table);
};
REGISTERED_COMPONENT(AnimatedSprite, "AnimatedSprite")

};

#endif
