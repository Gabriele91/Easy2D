#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include <Config.h>
#include <Math3D.h>
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

public:
    //costructor
    Animation( FrameSet::ptr frameset, float timePerFrame)
    {
        setFrameSet(frameset,timePerFrame);
    }
    //animation update
    void setFrame(float dt)
    {

        DEBUG_ASSERT( frames!=NULL );
        if( timePerFrame == 0 )return;
        if( frames->size() <= 1 ) return;
        //new time
        animationTime = dt;
        //update
        while( animationTime >= timePerFrame )
        {
            ++currentFrame;
            currentFrame%=frames->size();
            animationTime -= timePerFrame;
        }
        //??
        while( animationTime < 0 )
            animationTime += totalTime;
    }
    //next frame
    void update(float dt)
    {
        setFrame(animationTime+dt);
    }
    //setters
    void setFrameSet( FrameSet::ptr frameset, float tPerFrame )
    {
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
    //getters
    DFORCEINLINE Mesh::ptr getCurrentFrame()
    {
        return frames->getFrame(currentFrame);
    }
    DFORCEINLINE Vec2 getCurrentFrameSize()
    {
        return frames->sizeFrame(currentFrame);
    }
    DFORCEINLINE float getCurrentTime()
    {
        return animationTime;
    }
    DFORCEINLINE float getTotalTime()
    {
        return totalTime;
    }
    DFORCEINLINE float getTimePerFrame()
    {
        return timePerFrame;
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
    //update frame
    virtual void update(float dt)
    {
        if(crtAnimation>=0)
        {
            animations[crtAnimation]->update(dt);
            //set sprite mesh
            setMesh(animations[crtAnimation]->getCurrentFrame());
        }
    }

public:

    //costructor
    AnimatedSprite(Layer *layer=NULL);
    virtual ~AnimatedSprite();
    //set animation frame
    void setFrame(int animation,int frame);
    //add an animation
    int addAnimation(FrameSet::ptr frames);
    int addAnimation(FrameSet::ptr frames, float time);
    //change an animation
    void setAnimation(int i);
    void setAnimation(int i, float timePerFrame);
    //change time animation
    void setTime(float timePerFrame);
    void setChangeTime(float timePerFrame);
    void setAnimationTime(int i, float time);
    //query
    int getCurrentAnimation()
    {
        return crtAnimation;
    }
    float getCurrentTimePerFrame()
    {
        return animations[crtAnimation]->getTimePerFrame();
    }
    float getCurrentTotalTime()
    {
        return animations[crtAnimation]->getTotalTime();
    }
    float getCurrentTime()
    {
        return animations[crtAnimation]->getCurrentTime();
    }
    //get pixel scale
    Vec2 getBoxScale()
    {
        //get box
        const AABox2& box=getMesh()->getAABox();
        Vec2 scale=box.getMax()-box.getMin();
        return scale*getScale();
    }
};

};

#endif