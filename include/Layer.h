#ifndef LAYER_H
#define LAYER_H

#include <Config.h>
#include <Camera.h>

namespace Easy2D {
	//
	class Renderable;
	//
	class Layer {
	
		bool visible;

	public:
		
		/* layer it */
		class iterator{

		public:
			virtual bool operator==(const iterator& it){return (*this)==it;}
			virtual bool operator!=(const iterator& it){return (*this)!=it;}
			virtual iterator operator++(){ return (*this);};
			virtual Renderable* operator*(){ return NULL; };
		};
		/* template it */
		template <class T>
		class stditerator : public iterator{

			T::iterator it;
		
		public:

			stditerator(T::iterator& it):it(it){}
			virtual bool operator==(const iterator& it){return this->it==((stditerator<T>*)(&it))->it;}
			virtual bool operator!=(const iterator& it){return this->it!=((stditerator<T>*)(&it))->it;}
			virtual iterator operator++(){ return stditerator<T>((this->it)++);};
			virtual Renderable* operator*(){ return *(this->it); };

		};
		//foreach
		virtual iterator begin()=0;
		virtual iterator end()=0;
		//
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
		typedef stditerator< std::list<Renderable *> > listIterator;

	public:
		//templates foreach
		virtual iterator begin(){
			return listIterator(renderables.begin());
		}
		virtual iterator end(){
			return listIterator(renderables.end());		
		}
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
		//std vector
		std::vector<Renderable*> renderables;
		//layer std iterator
		typedef stditerator< std::vector<Renderable*> > vectorIterator;
		//vector comparation items
		static bool operator_lt(const Renderable* lrs,const Renderable* rrs);

	public:
		//templates foreach
		virtual iterator begin(){
			return vectorIterator(renderables.begin());
		}
		virtual iterator end(){
			return vectorIterator(renderables.end());		
		}
		//
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