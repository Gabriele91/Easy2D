//external includes
#ifndef BOX2D_HPP
#define BOX2D_HPP

#include <Config.h>
#include <Math3D.h>
    
namespace Easy2D{

	namespace Box2D{
		#include "Box2D/Box2D.h"
	};
	
	inline Vec2 cast(const Box2D::b2Vec2& v){
		return Vec2(v.x,v.y);
	}    
    
	inline Box2D::b2Vec2 cast(const Vec2& v){
		return Box2D::b2Vec2(v.x,v.y);
	}
};

#endif