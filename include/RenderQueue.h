#ifndef RENDERQUEUE_H
#define RENDERQUEUE_H

#include <Config.h>
#include <queue>
#include <Mesh.h>
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
    typedef std::list<Object*>::reverse_iterator RevItObjs;
    typedef std::list<Object*>::const_iterator CItObjs;
    typedef std::list<Object*>::const_reverse_iterator CRevItObjs;
	RenderContext::RenderTarget target;
	
public:
	//init
	RenderQueue(); 
	//delete
	virtual ~RenderQueue();
	//target
	const RenderContext::RenderTarget& getTarget() const
	{
		return target;
    }
    //rebuilt target texture (screen resize)
    void rebuildTarget();
    //draw
    void draw() const;
    //draw
    void draw(Mesh::ptr batchingMesh) const;
    //append objects to queue
    void append(DFUNCTION<bool(const AABox2&)> filter, Object* obj);
    //add elements
    void push(Object* obj);
    //
    size_t size() const
    {
        return objs.size();
    }
    //
	void clear()
	{
		objs.clear();
	}
    //
    ItObjs begin()
    {
        return objs.begin();
    }
    ItObjs end()
    {
        return objs.end();
    }
    RevItObjs rbegin()
    {
        return objs.rbegin();
    }
    RevItObjs rend()
    {
        return objs.rend();
    }
    CItObjs begin() const
    {
        return objs.begin();
    }
    CItObjs end() const
    {
        return objs.end();
    }
    CRevItObjs rbegin() const
    {
        return objs.rbegin();
    }
    CRevItObjs rend() const
    {
        return objs.rend();
    }

};

};
#endif