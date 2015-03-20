#ifndef RENDERQUEUE_H
#define RENDERQUEUE_H

#include <Config.h>
#include <queue>
#include <Object.h>
#include <RenderContext.h>

namespace Easy2D
{
//class declaretion
class Render;
class RenderQueue;
class PostEffects;

//render queue
class RenderQueue : public Pointers < RenderQueue >
{
	std::list<Object*> objs;
	typedef std::list<Object*>::iterator ItObjs;
	typedef std::list<Object*>::reverse_iterator revItObjs;
	RenderContext::RenderTarget target;
	Render* render;
	
public:
	//init
	RenderQueue(Render* render); 
	//delete
	virtual ~RenderQueue();
	//target
	const RenderContext::RenderTarget& getTarget() const
	{
		return target;
	}
    //append objects to queue
    void append(Object* obj);
	//add elements
	void push(Object* obj);
	void draw(Render* render);
	void clear()
	{
		objs.clear();
	}
	size_t size()
	{
		return objs.size();
	}
	ItObjs begin()
	{
		return objs.begin();
	}
	ItObjs end()
	{
		return objs.end();
	}
	revItObjs rbegin()
	{
		return objs.rbegin();
	}
	revItObjs rend()
	{
		return objs.rend();
	}

};

};
#endif