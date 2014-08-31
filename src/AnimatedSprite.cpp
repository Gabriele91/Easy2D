#include <stdafx.h>
#include <AnimatedSprite.h>
#include <ResourcesManager.h>
#include <ResourcesGroup.h>

///////////////////////
using namespace Easy2D;
///////////////////////

AnimatedSprite::AnimatedSprite()
    :Renderable(nullptr,nullptr,true)
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

void AnimatedSprite::setFrame(int frame)
{
    setFrame(crtAnimation,frame);
}

void AnimatedSprite::setFrame(int animation,int frame)
{
    //set animation
    setAnimation(animation);
    //set frame
    animations[animation]->setForcedFrame(frame);
    //set sprite mesh
    setMesh(animations[crtAnimation]->getCurrentFrameMesh());
}

int AnimatedSprite::addAnimation(FrameSet::ptr frames)
{
    //return id
    return addAnimation(frames,frames->getDefaultTime(),frames->getDefaultLoop());
}
int AnimatedSprite::addAnimation(FrameSet::ptr frames,
                                 float time)
{
    //return id
    return addAnimation(frames,time,frames->getDefaultLoop());
}
int AnimatedSprite::addAnimation(FrameSet::ptr frames,
                                 float time,
                                 bool loop)
{
    //set current animation:
    setTexture(frames->getTexture());
    Animation* anim=new Animation(frames, time, loop);
    crtAnimation=animations.size();
    animations.push_back(anim);
    //set sprite mesh
    setMesh(anim->getCurrentFrameMesh());
    //return id
    return crtAnimation;
}

void AnimatedSprite::clearAnimations()
{
    //clear animations
    for(auto anim:animations)
        delete anim;
    animations.clear();
    //init animation
    crtAnimation=0;
    //set sprite mesh
    setMesh(nullptr);
    setTexture(nullptr);
}

void AnimatedSprite::getFrameSets(std::vector<FrameSet::ptr>& frames)
{
    for(auto anim:animations)
    {
        frames.push_back(anim->getFrameSet());
    }
}

void AnimatedSprite::setAnimation(int i)
{    
    DEBUG_ASSERT( ((int)crtAnimation) >= 0 );
    DEBUG_ASSERT( ((int)animations.size()) > i );
    //set animation
    crtAnimation=i;
    //set sprite mesh
    setMesh(animations[crtAnimation]->getCurrentFrameMesh());
    setTexture(animations[crtAnimation]->getTexture());
}
void AnimatedSprite::setAnimation(int i, float time)
{
    setAnimation(i);
    animations[crtAnimation]->setFrameTime(time);
}
void AnimatedSprite::setAnimation(int i, float time, bool  loop)
{
    setAnimation(i);
    animations[crtAnimation]->setFrameTime(time);
    animations[crtAnimation]->setLoop(loop);
}
void AnimatedSprite::setAnimation(int i, bool loop)
{    
    setAnimation(i);
    animations[crtAnimation]->setLoop(loop);
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
void AnimatedSprite::setPause(bool pause)
{
    animations[crtAnimation]->setPause(pause);
}
void AnimatedSprite::setLoop(bool loop)
{
    animations[crtAnimation]->setLoop(loop);
}
void AnimatedSprite::setAnimationTime(int i,float timePerFrame)
{

    DEBUG_ASSERT( ((int)crtAnimation) >= 0 );
    DEBUG_ASSERT( ((int)animations.size()) > i );
    animations[i]->setFrameTime(timePerFrame);
}
void AnimatedSprite::setAnimationPause(int i, bool pause)
{
    DEBUG_ASSERT( ((int)crtAnimation) >= 0 );
    DEBUG_ASSERT( ((int)animations.size()) > i );
    animations[i]->setPause(pause);
}
void AnimatedSprite::setAnimationLoop(int i, bool loop)
{
    DEBUG_ASSERT( ((int)crtAnimation) >= 0 );
    DEBUG_ASSERT( ((int)animations.size()) > i );
    animations[i]->setLoop(loop);
}


void AnimatedSprite::serialize(Table& table)
{
    //serialize render state
    rsSerialize(table);
    //size?
    if(!animations.size()) return;
    //serialize ASprite
    table.set("currentAnimation",(float)getCurrentAnimation());
    table.set("currentFrame",(float)getCurrentFrame());
    //table list frames
    Table& frames=table.createTable("frameSets");
    //
    for(auto anim : animations)
    {
        Table& tanim=frames.createTable();
        tanim.set("frameSet",anim->getFrameSet()->getName());
        if(anim->getFrameSet()->getDefaultTime()!=anim->getTimePerFrame())
            tanim.set("time",anim->getTotalTime());
        if(anim->getLoop())
            tanim.set("loop",anim->getLoop());
    }
}
void AnimatedSprite::deserialize(const Table& table)
{
    //get resource
    auto rsmanager=table.getResourcesManager();
    DEBUG_ASSERT(rsmanager);
    auto rsgroup=rsmanager->getResourcesGroup();
    DEBUG_ASSERT(rsgroup);
    //deserialize rander state
    rsDeserialize(table);
    //current animation
    crtAnimation=table.getFloat("currentAnimation",(float)getCurrentAnimation());
    
    //frames table
    auto frames=table.getConstTable("frameSets");
    
    for(auto rsp:frames)
    {
        if(rsp.second->asType(Table::TABLE))
        {
            Table& tbl=rsp.second->get<Table>();
            //frame set
            FrameSet::ptr fms=rsgroup->load<FrameSet>(tbl.getString("frameSet"));
            //default time
            float time=tbl.getFloat("time",fms->getDefaultTime());
            //default time
            bool loop=tbl.getFloat("loop",0.0)!=0.0;
            //times
            addAnimation(fms,time,loop);
        }
    }
    //no animation?
    if(!animations.size()) return;
    //else get current frame
    setFrame(table.getFloat("currentFrame",(float)getCurrentFrame()));

}
