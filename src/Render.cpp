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
    glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
    glBlendFunc( GL_ONE , GL_ZERO ); 
	//disable light
	glDisable(GL_LIGHTING);
	//enable texturing	
	glEnable( GL_TEXTURE_2D );
    //always active!
    glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	//set orientation
	setOrientation(Application::instance()->getScreen()->getOrientation());
	//find errors:
	CHECK_GPU_ERRORS();
}

Layer* Render::addLayer(bool order){
	Layer *newLayer=order? (Layer*)new LayerOrder():
						   (Layer*)new LayerUnorder();
	layers.push_back(newLayer);
	return newLayer;
}
void Render::erseLayer(Layer* layer){
	auto it=std::find(layers.begin(), layers.end(), layer);
	layers.erase(it);
}

void Render::draw(){
	//old state
	RenderState *oldState=NULL;
	//clear
	glClearColor(clearClr.rNormalize(),
				 clearClr.gNormalize(),
				 clearClr.bNormalize(),
				 clearClr.aNormalize());
	glClear(GL_COLOR_BUFFER_BIT);
	//set view port
	glViewport( 0, 0, viewport.x, viewport.y );
	//for all layers
	for(auto layer:layers){
		//layer is visible?
		if(layer->isVisible()){
			//update layer
			layer->update();
			//for all renderable
			while(Renderable *renderable=layer->next()){
				//set model view matrix
				glMatrixMode(GL_MODELVIEW);
				//calc m4x4
				glLoadMatrixf(
					camera->getGlobalMatrix().mul2D(renderable->getGlobalMatrix()).entries
					);
				//renderable is visible?
				if(renderable->isVisible()){
					//draw
					if(oldState==NULL)
						renderable->draw();
					else
						renderable->draw(oldState);
					//set old state
					oldState=(RenderState*)renderable;
					//draw errors
					CHECK_GPU_ERRORS();
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
    //update projection is always the same
    glMatrixMode(GL_PROJECTION);
	Matrix4x4 projection;
	projection.setOrtho(-viewport.x*0.5,
		                 viewport.x*0.5,
					    -viewport.y*0.5,
						 viewport.y*0.5,
						 0,1);
    glLoadMatrixf(projection);
}