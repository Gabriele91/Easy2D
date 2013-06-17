#ifndef LAYER_H
#define LAYER_H

#include <Config.h>
#include <Camera.h>
#include <Renderable.h>

namespace Easy2D {
	//
	class Layer {
	
		bool visible;
	
	protected:

		DFORCEINLINE void updateRenderable(Renderable *rnd, float dt){
			rnd->update(dt);
		}
		Layer():visible(true){}
        
	public:
		//
		virtual Renderable* next()=0;
		//
		virtual void change()=0;	
		virtual void dosort()=0;	
		virtual void update(float dt)=0;		
		//
		virtual void addRenderable(Renderable *rnd){			
			rnd->rlayer=this;
		};
		virtual void erseRenderable(Renderable *rnd){			
			rnd->rlayer=NULL;
		};
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
	
		//std::list
		std::list<Renderable *> renderables;
		//it list
		std::list<Renderable *>::iterator it;

	public:
		//
		LayerUnorder():Layer(),it(renderables.begin()){}
		//get Renderable
		virtual Renderable* next(){
			if(it!=renderables.end()){
				Renderable *tmp=*it;
				it++;
				return tmp;
			}
			else it=renderables.begin();
			return NULL;
		}
		//
		virtual void change(){};	
		virtual void dosort(){};	
		virtual void update(float dt){
			for(auto renderable:renderables)
				updateRenderable(renderable,dt);
		}	
		//
		virtual void addRenderable(Renderable *rnd){
			this->Layer::addRenderable(rnd);
			renderables.push_back(rnd);
		}
		virtual void erseRenderable(Renderable *rnd){
			this->Layer::erseRenderable(rnd);
			renderables.remove(rnd);
		}
		//

	};

	class LayerOrder : public Layer {
	
		//reorder?
		bool reorder;
		//std vector
		std::vector<Renderable*> renderables;
		//it
		int it;		
		//vector comparation items
		static bool operator_lt(const Renderable* lrs,const Renderable* rrs);

	public:
		//
		LayerOrder():Layer(),reorder(true),it(0){}
		//get Renderable
		virtual Renderable* next(){
			if(it<renderables.size())
				return renderables[it++];
			else it=0;
			return NULL;
		}
		//
		virtual void change(){
			reorder=true;
		}	
		virtual void dosort(){
			if(reorder){
				std::sort(renderables.begin(), 
					      renderables.end(),
						  operator_lt);
				reorder=false;
			}
		}	
		virtual void update(float dt){
			for(auto renderable:renderables)
				updateRenderable(renderable,dt);
		}
		//
		virtual void addRenderable(Renderable *rnd){
			this->Layer::addRenderable(rnd);
			renderables.push_back(rnd);
			reorder=true;
		}
		virtual void erseRenderable(Renderable *rnd){
			this->Layer::erseRenderable(rnd);
			auto it=std::find(renderables.begin(), renderables.end(), rnd);
			renderables.erase(it);
		}
		//

	};	
};

#endif