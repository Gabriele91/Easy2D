#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <Config.h>
#include <Mesh.h>
#include <Object.h>
#include <RenderState.h>
#include <Layer.h>

namespace Easy2D {

	class Renderable : public Object, public RenderState {
		
		Layer *rlayer;
		bool visible;
		float zvalue;

	public:

		Renderable(Mesh::ptr rmesh,
				   Texture::ptr rtex,
				   Layer *rlayer,
				   bool visible=true)
			:rlayer(rlayer)
			,visible(visible)
			,zvalue(0.0){
				setTexture(rtex);
				setMesh(rmesh);
		}
		//z order
		DFORCEINLINE float getZ(){ return zvalue; }
		DFORCEINLINE void  setZ(float z){ zvalue=z; rlayer->change(); };
		//sort:		
		bool operator <(const Renderable& rhs) const { 
			return zvalue < rhs.zvalue;
		}
		//other...
		DFORCEINLINE bool isVisible(){ 
			return visible;
		}
		DFORCEINLINE void show(){
			visible=true;
		}
		DFORCEINLINE void hide(){
			visible=false;
		}
		//draw
		virtual void draw()=0;
		virtual void draw(RenderState *oldstate)=0;

	};

};

#endif