#include <stdafx.h>
#include <Render.h>
/////////////////////////
using namespace Easy2D;
/////////////////////////

Render::Render(){
	//set orientation
	setOrientation(Application::instance()->getScreen()->getOrientation());
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
	//set projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection);
	//set view port
	glViewport( 0, 0, viewport.x, viewport.y );
	//for all layers
	for(auto layer:layers){
		//layer is visible?
		if(layer->isVisible()){
			//update layer
			layer->dosort();
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
void Render::update(float dt){
	//update all layers
	for(auto layer:layers) 
		layer->update(dt);
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
	projection.setOrtho(-viewport.x*0.5,
		                 viewport.x*0.5,
					    -viewport.y*0.5,
						 viewport.y*0.5,
						 0,1);
}