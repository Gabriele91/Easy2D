//external includes
#ifndef BOX2D_HPP
#define BOX2D_HPP

#include <Config.h>
#include <Math3D.h>
#include <Box2D/Box2D.h>
    
namespace Easy2D{
		
	inline Vec2 cast(const b2Vec2& v){
		return Vec2(v.x,v.y);
	}    
	inline b2Vec2 cast(const Vec2& v){
		return b2Vec2(v.x,v.y);
	}

};

#endif