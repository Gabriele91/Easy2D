#include <stdafx.h>
#include <World.h>
#include <Debug.h>
#include <Body.h>

///////////////////////
using namespace Easy2D;
///////////////////////

World::World(const Vec2& gravity)
{
    world = new b2World(cast(gravity));
}
World::~World()
{
    if(world)
        delete world;
}
void World::setGravity(const Vec2& gravity)
{
    world->SetGravity(cast(gravity));
}
void World::physics(float dt, uint velocityIterations, uint positionIterations)
{
    world->Step(dt,velocityIterations,positionIterations);
}
void World::addPhysics(Object* obj)
{
    obj->physics.registerWorld(this);
}
void  World::deletePhysics(Object* obj)
{
    obj->physics.unregisterWorld(this);
}