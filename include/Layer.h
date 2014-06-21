#ifndef LAYER_H
#define LAYER_H

#include <Config.h>
#include <Camera.h>
#include <Object.h>
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

    Layer(Render *rnd=nullptr)
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
    virtual Object* next()=0;
    //
    virtual void addRenderable(Object *rnd)
    {
        rnd->getComponent<Renderable>()->rlayer=this;
    };
    virtual void erseRenderable(Object *rnd)
    {
        rnd->getComponent<Renderable>()->rlayer=NULL;
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
    std::list<Object *> renderables;
    //it list
    std::list<Object *>::iterator it;

public:
    //
    LayerUnorder(Render *render):Layer(render)
                                ,it(renderables.begin()) 
    {
    }
    //get Renderable
    virtual Object* next()
    {
        if(it!=renderables.end())
        {
            Object *tmp=*it;
            it++;
            return tmp;
        }
        else it=renderables.begin();
        return NULL;
    }
    //
    virtual void change() {};
    virtual void dosort() {};
    //
    virtual void addRenderable(Object *rnd)
    {
        this->Layer::addRenderable(rnd);
        renderables.push_back(rnd);
    }
    virtual void erseRenderable(Object *rnd)
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
    std::vector<Object*> renderables;
    //it
    size_t it;
    //vector comparation items
    static bool operator_lt(const Object* lrs,const Object* rrs);

public:
    //
    LayerOrder(Render *render):Layer(render)
                              ,reorder(true),it(0) {}
    //get Renderable
    virtual Object* next()
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
    //
    virtual void addRenderable(Object *rnd)
    {
        this->Layer::addRenderable(rnd);
        renderables.push_back(rnd);
        reorder=true;
    }
    virtual void erseRenderable(Object *rnd)
    {
        this->Layer::erseRenderable(rnd);
        auto it=std::find(renderables.begin(), renderables.end(), rnd);
        renderables.erase(it);
    }
    //

};
};

#endif
