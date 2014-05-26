#include <stdafx.h>
#include <World.h>
#include <Debug.h>
///////////////////////
using namespace Easy2D;
///////////////////////
#define B2(x)  cast(x)

World::World(const Vec2& gravity){
	world = new b2World(B2(gravity));
}
World::~World(){
	if(world)
		delete world;
}
void World::setGravity(const Vec2& gravity){
	DEBUG_ASSERT(world);
	world->SetGravity(B2(gravity));
}
void World::step(float dt, uint velocityIterations, uint positionIterations){
	DEBUG_ASSERT(world);
	world->Step(dt,velocityIterations,positionIterations);
}
	