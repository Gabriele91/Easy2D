#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <Config.h>

namespace Easy2D {

	class Renderable {

		bool visible;

	public:

		virtual void render()=0;
		DFORCEINLINE bool isVisible(){ 
			return visible;
		}
		DFORCEINLINE void show(){
			visible=true;
		}
		DFORCEINLINE void hide(){
			visible=false;
		}
	};

};

#endif