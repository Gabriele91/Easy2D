#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include <Config.h>
#include <Renderable.h>
#include <FrameSet.h>
#include <Debug.h>

namespace Easy2D {

	class Animation{

		//save frame set
		FrameSet::ptr frames;
		//frame info
		float animationTime, 
			  totalTime,
			  timePerFrame;
		int   currentFrame;	

	public:
		//costructor
		Animation( FrameSet::ptr frameset, float timePerFrame){
			setFrameSet(frameset,timePerFrame);
		}
		//animation update
		void setFrame(float dt){

				DEBUG_ASSERT( frames!=NULL );				
				if( timePerFrame == 0 )return;				
				if( frames->size() <= 1 ) return;								
				animationTime = dt;
				
				//clamp in the time interval
				while( animationTime >= totalTime ){
					++currentFrame;
					currentFrame%=frames->size();
					animationTime -= totalTime;
				}
				
				while( animationTime < 0 ) animationTime += totalTime;
		}
		//next frame
		void update(float dt){
			setFrame(animationTime+dt);
		}
		//setters
		void setFrameSet( FrameSet::ptr frameset, float tPerFrame ){	
				frames = frameset;
				setFrameTime(tPerFrame);
			}
		void setFrameTime( float tPerFrame ){
				DEBUG_ASSERT( tPerFrame >= 0 );				
				animationTime = 0;
				timePerFrame = tPerFrame;		
				currentFrame = 0;				
				if( frames ) 
					totalTime = timePerFrame * frames->size();
				else
					totalTime = 1;
			}
		//getters
		DFORCEINLINE Mesh::ptr getCurrentFrame(){
			return frames->getFrame(currentFrame);
		}	
		DFORCEINLINE Vec2 getCurrentFrameSize(){
			return frames->sizeFrame(currentFrame);
		}	
		DFORCEINLINE float getCurrentTime(){
			return animationTime;
		}		
		DFORCEINLINE float getTotalTime(){
			return totalTime;
		}
		DFORCEINLINE float getTimePerFrame(){
				return timePerFrame;
		}

	};

	class AnimatedSprite : public Renderable {

	protected:
		//protected set mesh
		DFORCEINLINE void setMesh(Mesh::ptr mesh){
			Renderable::setMesh(mesh);
		}		
		//animations
		std::vector<Animation*> animations;
		int crtAnimation;
		//update frame
		virtual void update(float dt){ 
			if(crtAnimation>=0){
				animations[crtAnimation]->update(dt);
				//set sprite mesh
				setMesh(animations[crtAnimation]->getCurrentFrame());	
			}
		}

	public:

		//costructor
		AnimatedSprite(Layer *layer=NULL);
		virtual ~AnimatedSprite();
		//add an animation 
		int addAnimation(FrameSet::ptr frames);
		int addAnimation(FrameSet::ptr frames, float timePerFrame);
		void setAnimation(int i);
	};

};

#endif