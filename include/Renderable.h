#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <Config.h>
#include <Mesh.h>
#include <Component.h>
#include <RenderState.h>
#include <Blend.h>

namespace Easy2D
{
//
class Renderable : public Component, 
                   public RenderState
{
    
    bool visible;

public:

    Renderable(Mesh::ptr rmesh=nullptr,
               Texture::ptr rtex=nullptr,
               bool visible=true);
    //I can batching!?
    virtual bool canBatching(Renderable *oldstate);
    virtual bool doBatching()
    {
        return true && (!getMesh() || getMesh()->supportBatching());
    }
    virtual bool canTransform()
    {
        return true;
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
    //component
    ADD_COMPONENT_METHOS(Renderable)
    //serialize/deserialize
    virtual void serialize(Table& table);
    virtual void deserialize(const Table& table);
};

};

#endif