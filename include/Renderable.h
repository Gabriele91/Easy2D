#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <Config.h>
#include <Mesh.h>
#include <Math3D.h>
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
    bool canBatch;
    
public:

    Renderable(Mesh::ptr rmesh=nullptr,
               Texture::ptr rtex=nullptr,
               bool visible=true);
    //I can batching!?
    virtual bool canBatching(Renderable *oldstate);
    virtual bool doBatching()
    {
        return canBatch && (!getMesh() || getMesh()->supportBatching());
    }
    virtual bool canTransform()
    {
        return true;
    }
    //get box
    virtual AABox2 getBox();
    //get base box
    virtual AABox2 getBaseBox();
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
    DFORCEINLINE bool getCanBatch()
    {
        return canBatch;
    }
    DFORCEINLINE void setCanBatch(bool batch)
    {
        canBatch=batch;
    }
    //component
    ADD_COMPONENT_METHOS(Renderable)
    //serialize/deserialize
    virtual void serialize(Table& table);
    virtual void deserialize(const Table& table);
};

};

#endif