#ifndef RENDER_H
#define RENDER_H

#include <queue>
#include <Config.h>
#include <Screen.h>
#include <Camera.h>
#include <Color.h>
#include <Sortable.h>
#include <Renderable.h>
#include <RenderState.h>
#include <BatchingMesh.h>

namespace Easy2D
{

class Render /*: public Sortable*/
{
protected:
    //info screen
    Camera *camera;
    //colors
    Color clearClr;
    Color ambientClr;
    //Batching
    BatchingMesh batchingMesh;
    //redner queue
    class Queue
    {
        std::list<Object*> objs;
        typedef std::list<Object*>::iterator ItObjs;
        typedef std::list<Object*>::reverse_iterator revItObjs;

    public:

        void push(Object* obj);
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
    //render queue
    Queue queue;
    //ricorsive append a child
    void append(Object* obj);
    //called from scene
    void buildQueue(const std::list<Object*>& objs);
    //draw
    void draw();

public:
    //
    Render();
    virtual ~Render() {}
    //setting
    DFORCEINLINE void setCamera(Camera *cam)
    {
        camera=cam;
    }
    DFORCEINLINE Camera* getCamera()
    {
        return camera;
    }
    //
    DFORCEINLINE void setClear(const Color& color)
    {
        clearClr=color;
    }
    DFORCEINLINE const Color& getClear() const
    {
        return clearClr;
    }
    DFORCEINLINE void setAmbientLight(const Color& color)
    {
        ambientClr=color;
    }
    DFORCEINLINE const Color& getAmbientLight() const
    {
        return ambientClr;
    }
    //
    Object* picking(const Vec2& point);
    void aabox2Draw();
    //
    size_t queueSize()
    {
        return queue.size();
    }
    //deserrialize
    void deserialize(const Table& table)
    {
        Color clear,ambient;
        //set clear color
        clear.fromVec4(table.getVector4D("clearColor",Vec4(255,255,255,255)));
        setClear(clear);
        //get ambient color
        ambient.fromVec4(table.getVector4D("ambientLight",Vec4(255,255,255,255)));
        setAmbientLight(ambient);
    }

};

};

#endif
