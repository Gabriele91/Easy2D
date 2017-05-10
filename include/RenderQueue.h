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
//Cache List
//render queue
class RenderQueue : public Pointers < RenderQueue >
{
    //types
    typedef std::vector<Object*> List;
    typedef List::iterator ItObjs;
    typedef List::reverse_iterator RevItObjs;
    typedef List::const_iterator CItObjs;
    typedef List::const_reverse_iterator CRevItObjs;
    //attributes
    List objs;
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
    void append(Function<bool(const AABox2&)> filter, Object* obj);
    void append(const AABox2& viewPort,
                const Mat4& view,
                Object* obj);
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
