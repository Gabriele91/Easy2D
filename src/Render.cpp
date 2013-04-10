#include <stdafx.h>
#include <Render.h>
/////////////////////////
using namespace Easy2D;
/////////////////////////

Render::Render(){
	initOpenGL();
}

void Render::initOpenGL(){
	//enable culling
    glEnable( GL_CULL_FACE );        
    glCullFace( GL_BACK );        
    //default status for blending
    glEnable( GL_BLEND );   
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );   
    //projection is always the same
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();               
    //always active!
    glEnableClientState(GL_VERTEX_ARRAY);
	//set orientation
	setOrientation(Application::instance()->getScreen()->getOrientation());
}

void Render::draw(){
	//old state
	RenderState *oldState=NULL;
	//for all layers
	for(auto layer:layers){
		//It is visible?
		if(layer->isVisible()){
			//update layer
			layer->update();
			//for all renderable
			for(auto renderable:(*layer)){
				//set model view matrix
				glMatrixMode(GL_MODELVIEW);
				glLoadMatrixf(
					(const float*)
					(camera->getGlobalMatrix().mul2D(renderable->getGlobalMatrix()))
					);
				//It is visible?
				if(renderable->isVisible()){
					//draw
					if(oldState==NULL)
						renderable->draw();
					else
						renderable->draw(oldState);
					//set old state
					oldState=renderable;
				}
			}
		}
	}
}

void Render::setOrientation(Screen::Orientation _orientation){
	//orientation
	orientation=_orientation;
	//
	static float orientations[] =   { 0, 180, 90, -90 };
    screenAngle = orientations[ (uint)orientation ] + orientations[ (uint)orientation ];
	//set viewport
	viewport.x=Application::instance()->getScreen()->getWidth();
	viewport.y=Application::instance()->getScreen()->getHeight();
	//
}