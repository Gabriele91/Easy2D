#ifndef WORLD_H
#define WORLD_H

#include <Config.h>
#include <Math3D.h>
#include <Box2D.hpp>
#include <Object.h>

namespace Easy2D
{
/////////////////////
class Body;
/////////////////////
class World
{

    friend class Body;
    b2World *world;
    b2BlockAllocator blockAllocator;

public:

    World(const Vec2& gravity=Vec2::ZERO);
    virtual ~World();
    void  addPhysics(Object* obj);
    void  deletePhysics(Object* obj);
    void  setGravity(const Vec2& gravity);
    void  physics(float dt, uint velocityIterations=8, uint positionIterations=3);

};


};

#endif