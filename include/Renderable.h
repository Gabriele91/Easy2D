#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <Config.h>
#include <Mesh.h>
#include <Object.h>
#include <RenderState.h>
#include <Blend.h>

namespace Easy2D
{
//
class Layer;
//
class Renderable : public Object, public RenderState
{

    Layer *rlayer;
    bool visible;
    float zvalue;
    friend class Layer;
    virtual void update(float dt) {}

public:

    Renderable(Mesh::ptr rmesh=NULL,
               Texture::ptr rtex=NULL,
               Layer *rlayer=NULL,
               bool visible=true);
    //I can batching!?
    bool canBatching(Renderable *oldstate);
    //z order
    DFORCEINLINE float getZ() const
    {
        return zvalue;
    }
    void  setZ(float z);
    //sort:
    bool operator <(const Renderable& rhs) const
    {
        return zvalue < rhs.zvalue;
    }
    //other...
    DFORCEINLINE bool isVisible() const
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
};

};

#endif