#ifndef SPRITE_H
#define SPRITE_H

#include <Config.h>
#include <Renderable.h>
#include <Object.h>


namespace Easy2D
{

class Sprite : public Renderable
{

protected:
    //protected set mesh
    DFORCEINLINE void setMesh(Mesh::ptr mesh)
    {
        Renderable::setMesh(mesh);
    }

public:

    //costructor
    Sprite(Texture::ptr image=NULL,Layer *layer=NULL);
    //overload setTexture
    void setTexture(Texture::ptr texture);
    //get pixel scale
    Vec2 getPixelScale()
    {
        return getTexture()->getSpriteSize()*getObject()->getScale();
    }
    //component name
    virtual const char* getComponentName() const
    {
        return "Sprite";
    }
    
    //serialize/deserialize
    virtual void serialize(Table& table);
    virtual void deserialize(const Table& table);

};

};

#endif