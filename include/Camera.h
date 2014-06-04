#ifndef CAMERA_H
#define CAMERA_H

#include <Config.h>
#include <Object.h>


namespace Easy2D {

	class Camera : public Object {

	public:
		//constructor
		Camera(){}
		//get mouse
		Vec2 getWMouse();
		Vec2 getSMouse();

	};

};

#endif