#include <stdafx.h>
#include <AnimatedSprite.h>

///////////////////////
using namespace Easy2D;
///////////////////////


AnimatedSprite::AnimatedSprite(Layer *layer)
							 :Renderable(NULL,NULL,layer,true)
							 ,crtAnimation(0){
	//blend mode
	enableBlend();
	setBlend(BLEND::SRC::ALPHA, BLEND::ONE::MINUS::SRC::ALPHA);
}
AnimatedSprite::~AnimatedSprite(){
	for(auto anim:animations)
		delete anim;
}

int AnimatedSprite::addAnimation(FrameSet::ptr frames){
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
								 float timePerFrame){
	//set current animation:
	int anim=addAnimation(frames);
	animations[anim]->setFrameTime(timePerFrame);
	//return id
	return anim; 
}

void AnimatedSprite::setAnimation(int i){
	
	DEBUG_ASSERT( crtAnimation >= 0 );
	DEBUG_ASSERT( animations.size() > i );
	//set animation
	crtAnimation=i;	
	//set sprite mesh
	setMesh(animations[crtAnimation]->getCurrentFrame());	
}