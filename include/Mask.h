#ifndef MASK_H
#define MASK_H

#include <Config.h>
#include <RenderContext.h>
#include <Renderable.h>
#include <Sprite.h>
#include <RenderQueue.h>

namespace Easy2D
{
    class MaskBox : public Renderable
    {

        OBBox2 box;
        RenderQueue::ptr queue;
        bool objCanDrawChilds;
        virtual AABox2 getBaseBox();
        virtual void setMesh(Mesh::ptr mesh){ assert(0); };
        virtual void setTexture(Texture::ptr mesh){ assert(0); };

    public:

        MaskBox();
        MaskBox(const OBBox2& box);
        virtual ~MaskBox();
        /////////////////////////////////////	
        void setBox(const OBBox2& box){ this->box=box; }
        const OBBox2& getBox() const { return box; }
        /////////////////////////////////////
        virtual void onSetObject(Object* obj);
        virtual void onEraseObject();
        virtual void draw();
        /////////////////////////////////////
        //is a component
        DERIVATE_COMPONENT(MaskBox)
        //serialize/deserialize
        virtual void serialize(Table& table);
        virtual void deserialize(const Table& table);

    };
    REGISTERED_COMPONENT(MaskBox, "MaskBox")

    class MaskSprite : public Sprite
    {
        RenderQueue::ptr queue { nullptr };
        uint  uAlphaMask       { 0    };
        float alphaMask        { 0.5f };
        bool  objCanDrawChilds { true };

    public:

        MaskSprite(Texture::ptr image = nullptr, float alpha=0.5f);
        virtual ~MaskSprite();
        /////////////////////////////////////
        virtual void onSetObject(Object* obj);
        virtual void onEraseObject();
        virtual void draw();
        /////////////////////////////////////
        void setAlphaMask(float val);
        float getAlphaMask();
        /////////////////////////////////////
        //is a component
        DERIVATE_COMPONENT(MaskSprite)
        //serialize/deserialize
        virtual void serialize(Table& table);
        virtual void deserialize(const Table& table);

    };
    REGISTERED_COMPONENT(MaskSprite, "MaskSprite")

};

#endif
