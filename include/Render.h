#ifndef RENDER_H
#define RENDER_H

#include <Config.h>
#include <Camera.h>
#include <Layer.h>
#include <Renderable.h>
#include <RenderState.h>

namespace Easy2D {
	
	class Render {		
		//
		std::vector<Layer *> layers;
		Camera *camera;
		void initOpenGL();
		//
	public:
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
	};

};

#endif