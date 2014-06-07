#ifndef LAYER_H
#define LAYER_H

#include <Config.h>
#include <Camera.h>
#include <Renderable.h>
#include <Sortable.h>

namespace Easy2D
{
//Class declaration
class Layer;
class Render;
class LayerOrder;
class LayerUnorder;
//
class Layer : public Sortable
{

    Render *render;
    bool visible;
    Vec2 parallax;
    uint z;

protected:

    friend class Render;
    DFORCEINLINE void updateRenderable(Renderable *rnd, float dt)
    {
        rnd->update(dt);
    }

    Layer(Render *render=NULL)
        :render(render)
        ,visible(true)
        ,parallax(Vec2::ONE)
        ,z(0) {}

    DFORCEINLINE void setRender(Render *rnd)
    {
        render=rnd;
    }

    DFORCEINLINE void dtRender()
    {
        render=NULL;
    }


public:
    //
    virtual Renderable* next()=0;
    //
    virtual void update(float dt)=0;
    //
    virtual void addRenderable(Renderable *rnd)
    {
        rnd->rlayer=this;
    };
    virtual void erseRenderable(Renderable *rnd)
    {
        rnd->rlayer=NULL;
    };
    //
    DFORCEINLINE bool isVisible()
    {
        return visible;
    }
    DFORCEINLINE void show()
    {
        visible=true;
    }
    DFORCEINLINE void hide()
    {
        visible=false;
    }
    DFORCEINLINE void setParallax(const Vec2& offset)
    {
        parallax=offset;
    }
    DFORCEINLINE Vec2 getParallax() const
    {
        return parallax;
    }

    DFORCEINLINE void setZ(uchar zvalue)
    {
        //save value
        z=zvalue;
        //do sort
        if(render)
            ((Sortable*)render)->change();
    }
    DFORCEINLINE uchar getZ() const
    {
        return z;
    }

    DFORCEINLINE Render* getRender() const
    {
        return render;
    }

};

class LayerUnorder : public Layer
{

    //std::list
    std::list<Renderable *> renderables;
    //it list
    std::list<Renderable *>::iterator it;

public:
    //
    LayerUnorder(Render *render):Layer(render),it(renderables.begin()) {}
    //get Renderable
    virtual Renderable* next()
    {
        if(it!=renderables.end())
        {
            Renderable *tmp=*it;
            it++;
            return tmp;
        }
        else it=renderables.begin();
        return NULL;
    }
    //
    virtual void change() {};
    virtual void dosort() {};
    virtual void update(float dt)
    {
        for(auto renderable:renderables)
            updateRenderable(renderable,dt);
    }
    //
    virtual void addRenderable(Renderable *rnd)
    {
        this->Layer::addRenderable(rnd);
        renderables.push_back(rnd);
    }
    virtual void erseRenderable(Renderable *rnd)
    {
        this->Layer::erseRenderable(rnd);
        renderables.remove(rnd);
    }
    //

};

class LayerOrder : public Layer
{

    //reorder?
    bool reorder;
    //std vector
    std::vector<Renderable*> renderables;
    //it
    size_t it;
    //vector comparation items
    static bool operator_lt(const Renderable* lrs,const Renderable* rrs);

public:
    //
    LayerOrder(Render *render):Layer(render),reorder(true),it(0) {}
    //get Renderable
    virtual Renderable* next()
    {
        if(it<renderables.size())
            return renderables[it++];
        else it=0;
        return NULL;
    }
    //
    virtual void change()
    {
        reorder=true;
    }
    virtual void dosort();
    virtual void update(float dt)
    {
        for(auto renderable:renderables)
            updateRenderable(renderable,dt);
    }
    //
    virtual void addRenderable(Renderable *rnd)
    {
        this->Layer::addRenderable(rnd);
        renderables.push_back(rnd);
        reorder=true;
    }
    virtual void erseRenderable(Renderable *rnd)
    {
        this->Layer::erseRenderable(rnd);
        auto it=std::find(renderables.begin(), renderables.end(), rnd);
        renderables.erase(it);
    }
    //

};
};

#endif
