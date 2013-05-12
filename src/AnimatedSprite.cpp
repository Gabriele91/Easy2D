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
	setBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
AnimatedSprite::~AnimatedSprite(){
	for(auto anim:animations)
		delete anim;
}

int AnimatedSprite::addAnimation(FrameSet::ptr frames,
								 float timePerFrame){
	//set current animation:
	Renderable::setTexture(frames->getTexture());
	Animation* anim=new Animation(frames,timePerFrame);
	crtAnimation=animations.size();
	animations.push_back(anim);
	//set sprite mesh
	setMesh(anim->getCurrentFrame());	
	//setScale
	setScale(anim->getCurrentFrameSize());
	//return id
	return crtAnimation; 
}

void AnimatedSprite::setAnimation(int i){
	
	DEBUG_ASSERT( crtAnimation >= 0 );
	DEBUG_ASSERT( animations.size() > i );
	//set animation
	crtAnimation=i;	
	//set sprite mesh
	setMesh(animations[crtAnimation]->getCurrentFrame());	
	//setScale
	setScale(animations[crtAnimation]->getCurrentFrameSize());
}