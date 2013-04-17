#ifndef RENDER_H
#define RENDER_H

#include <Config.h>
#include <Screen.h>
#include <Camera.h>
#include <Layer.h>
#include <Color.h>
#include <Renderable.h>
#include <RenderState.h>

namespace Easy2D {
	
	class Render {		
		//
		std::vector<Layer *> layers;
		//info screen
		Camera *camera;
		Vector2D viewport;
		float screenAngle;
		Screen::Orientation orientation;
		//colors
		Color clearClr;
		Color ambientClr;


	public:
		//init openGL
		void initOpenGL();
		//
		Render();
		//setting
		DFORCEINLINE void setCamera(Camera *cam){ camera=cam; }
		DFORCEINLINE Camera* getCamera(){ return camera; }
		//
		Layer* addLayer(bool order);
		void erseLayer(Layer* layer);
		//
		void draw();
		//
		DFORCEINLINE void setClear(Color& color){ clearClr=color; }
		//
		void setOrientation(Screen::Orientation orientation);
	};

};

#endif