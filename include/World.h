#ifndef WORLD_H
#define WORLD_H

#include <Config.h>
#include <Math3D.h>
#include <Box2D.hpp>

namespace Easy2D {

	class World{
	
		b2World *world;
		
	public:
		
		World(const Vec2& gravity=Vec2::ZERO);
		virtual ~World();
		void setGravity(const Vec2& gravity);
		void step(float dt, uint velocityIterations=8, uint positionIterations=3);
	
	};


};

#endif