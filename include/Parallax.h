#ifndef PARALLAX_H
#define PARALLAX_H

#include <Config.h>
#include <Component.h>
#include <Camera.h>
#include <Scene.h>

namespace Easy2D {

	class Parallax : public Component
    {

    Vec2 parallax;
    Vec2 moved;

	public:

    Parallax():parallax(Vec2::ONE)
    {
    }
    
    void setScale(const Vec2& scale)
    {
        parallax=scale;
    }
    const Vec2& getScale() const
    {
        return parallax;
    }
    //event: run
    virtual void onFixedRun(float dt);
    //component
    ADD_COMPONENT_METHOS(Parallax)
    //serialize/deserialize
    virtual void serialize(Table& table);
    virtual void deserialize(const Table& table);

	};
    REGISTERED_COMPONENT(Parallax,"Parallax");
};

#endif