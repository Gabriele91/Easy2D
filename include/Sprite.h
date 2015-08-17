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
    void setMesh(Mesh::ptr mesh);

public:

    //costructor
    Sprite(Texture::ptr image=nullptr);
    //overload setTexture
    void setTexture(Texture::ptr texture);
    //get pixel scale
    Vec2 getPixelScale();
    //set pixel scale
    void setPixelScale(const Vec2& scale,bool global=true);
    //is a component
    DERIVATE_COMPONENT(Sprite)
    //serialize/deserialize
    virtual void serialize(Table& table);
    virtual void deserialize(const Table& table);

};
REGISTERED_COMPONENT(Sprite, "Sprite")

};

#endif