#include <stdafx.h>
#include <AnimatedSprite.h>

///////////////////////
using namespace Easy2D;
///////////////////////


AnimatedSprite::AnimatedSprite(Layer *layer)
    :Renderable(NULL,NULL,layer,true)
    ,crtAnimation(0)
{
    //blend mode
    enableBlend();
    setBlend(BLEND::SRC::ALPHA, BLEND::ONE::MINUS::SRC::ALPHA);
}
AnimatedSprite::~AnimatedSprite()
{
    for(auto anim:animations)
        delete anim;
}


void AnimatedSprite::setFrame(int animation,int frame)
{
    //set animation
    setAnimation(animation);
    //set frame
    animations[animation]->setForcedFrame(frame);
    //set sprite mesh
    setMesh(animations[crtAnimation]->getCurrentFrame());
}

int AnimatedSprite::addAnimation(FrameSet::ptr frames)
{
    //set current animation:
    Renderable::setTexture(frames->getTexture());
    Animation* anim=new Animation(frames,frames->getDefaultTime());
    crtAnimation=animations.size();
    animations.push_back(anim);
    //set sprite mesh
    setMesh(anim->getCurrentFrame());
    //return id
    return crtAnimation;
}
int AnimatedSprite::addAnimation(FrameSet::ptr frames,
                                 float time)
{
    //set current animation:
    int anim=addAnimation(frames);
    animations[anim]->setFrameTime(time);
    //return id
    return anim;
}

void AnimatedSprite::setAnimation(int i)
{

    DEBUG_ASSERT( ((int)crtAnimation) >= 0 );
    DEBUG_ASSERT( ((int)animations.size()) > i );
    //set animation
    crtAnimation=i;
    //set sprite mesh
    setMesh(animations[crtAnimation]->getCurrentFrame());
}
void AnimatedSprite::setAnimation(int i, float time)
{

    DEBUG_ASSERT( ((int)crtAnimation) >= 0 );
    DEBUG_ASSERT( ((int)animations.size()) > i );
    //set animation
    crtAnimation=i;
    //set sprite mesh
    setMesh(animations[crtAnimation]->getCurrentFrame());
    //set time
    animations[crtAnimation]->setFrameTime(time);
}

void AnimatedSprite::setTime(float timePerFrame)
{
    //set time
    animations[crtAnimation]->setFrameTime(timePerFrame);
}
void AnimatedSprite::setChangeTime(float timePerFrame)
{
    //set time
    animations[crtAnimation]->setChangeFrameTime(timePerFrame);
}

void AnimatedSprite::setAnimationTime(int i,float timePerFrame)
{

    DEBUG_ASSERT( ((int)crtAnimation) >= 0 );
    DEBUG_ASSERT( ((int)animations.size()) > i );
    animations[i]->setFrameTime(timePerFrame);
}
