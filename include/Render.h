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
	protected:

		std::vector<Layer *> layers;
		//projection matrix
		Matrix4x4 projection;
		//info screen
		Camera *camera;
		Vector2D viewport;
		float screenAngle;
		//colors
		Color clearClr;
		Color ambientClr;
		//Batching
		#ifdef ENABLE_CPU_BATCHING_MESH 
		BatchingMesh batchingMesh;
		#endif

	public:
		//
		Render();
        virtual ~Render(){}
		//setting
		DFORCEINLINE void setCamera(Camera *cam){ camera=cam; }
		DFORCEINLINE Camera* getCamera(){ return camera; }
		//
		Layer* addLayer(bool order);
		void erseLayer(Layer* layer);
		//
		void draw();
		void update(float dt);
		//
		DFORCEINLINE void setClear(const Color& color){ clearClr=color; }
		//
		void updateProjection();
		void updateProjection(const Vec2& viewport);
		void updateViewport(const Vec2& viewport);
	};

};

#endif
