#ifndef LAYER_H
#define LAYER_H

#include <Config.h>
#include <Camera.h>

namespace Easy2D {

	class Renderable;

	class Layer {
	
		bool visible;

	public:
		
		virtual void change()=0;	
		virtual void update()=0;		
		//
		virtual void addRenderable(Renderable *rnd)=0;
		virtual void erseRenderable(Renderable *rnd)=0;
		//
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

	class LayerUnorder : public Layer {
	
		std::list<Renderable *> renderables;

	public:
		//
		virtual void change(){};	
		virtual void update(){};		
		//
		virtual void addRenderable(Renderable *rnd){
			renderables.push_back(rnd);
		}
		virtual void erseRenderable(Renderable *rnd){
			renderables.remove(rnd);
		}
		//

	};

	class LayerOrder : public Layer {
	
		bool reorder;
		std::vector<Renderable*> renderables;
		static bool operator_lt(const Renderable* lrs,const Renderable* rrs);

	public:
		//
		virtual void change(){
			reorder=true;
		}	
		virtual void update(){
			if(reorder){
				std::sort(renderables.begin(), renderables.end(),operator_lt);
				reorder=false;
			}
		}
		//
		virtual void addRenderable(Renderable *rnd){
			renderables.push_back(rnd);
		}
		virtual void erseRenderable(Renderable *rnd){
			auto it=std::find(renderables.begin(), renderables.end(), rnd);
			renderables.erase(it);
		}
		//
	};	
};

#endif