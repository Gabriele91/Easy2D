#include <stdafx.h>
#include <Debug.h>
#include <Screen.h>
#include <Application.h>
#include <Render.h>
/////////////////////////
using namespace Easy2D;
/////////////////////////

Render::Render(){
	camera=NULL;
	screenAngle=0.0;
	//set orientation
	updateProjection();
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
#if 1
	#define MAX_BUFFER_SIZE ((256) << 10) //512 KB
	//create buffer
	if(!batchingMesh.getBufferSize())
		batchingMesh.createBuffer(MAX_BUFFER_SIZE);
	//old state
	Renderable *lastDraw=NULL;
	Renderable *rCurrent=NULL;
	Renderable *rNext=NULL;
	//clear
	glClearColor(clearClr.rNormalize(),
				 clearClr.gNormalize(),
				 clearClr.bNormalize(),
				 clearClr.aNormalize());
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//set projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection);
	//set view port
	glViewport( 0, 0, viewport.x, viewport.y );				
	//set model view matrix
	glMatrixMode(GL_MODELVIEW);
	//matrix camera
	if(camera)
		glLoadMatrixf(camera->getGlobalMatrix());
	else
		glLoadIdentity();
	//for all layers
	for(auto layer:layers){
		//layer is visible?
		if(layer->isVisible()){
			//update layer
			layer->dosort();
			//get firt randarable
			rNext=layer->next();
			//for all renderable
			while(rNext!=NULL){
				//is drawed!?
				//get next
				rCurrent=rNext;
				rNext=layer->next();
				//renderable is visible?
				if(rCurrent->isVisible()){
					//add mesh
					batchingMesh.addMesh(rCurrent->getGlobalMatrix(),
										 rCurrent->getMesh(),
									     rCurrent->getZ()/FLT_MAX);
					//draw!?
					if(!rNext || !rCurrent->canBatching(rNext) || !batchingMesh.canAdd(rNext->getMesh())){
						//enable info draw
						if(lastDraw)
							rCurrent->enableStates(lastDraw);
						else
							rCurrent->enableStates();
						//draw
						batchingMesh.draw();
						//save last draw
						lastDraw=rCurrent;
						//draw errors
						CHECK_GPU_ERRORS();
						//restart batching
						batchingMesh.relase();
					}
				}
			}
		}
	}
#else
	//old state
	RenderState *oldState=NULL;
	//clear
	glClearColor(clearClr.rNormalize(),
				 clearClr.gNormalize(),
				 clearClr.bNormalize(),
				 clearClr.aNormalize());
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//set projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection);
	//set view port
	glViewport( 0, 0, viewport.x, viewport.y );
	//set model view matrix
	glMatrixMode(GL_MODELVIEW);
	//for all layers
	for(auto layer:layers){
		//layer is visible?
		if(layer->isVisible()){
			//update layer
			layer->dosort();
			//for all renderable
			while(Renderable *renderable=layer->next()){
				//renderable is visible?
				if(renderable->isVisible()){
				//calc m4x4
				if(camera)
					glLoadMatrixf(
						camera->getGlobalMatrix().mul2D(renderable->getGlobalMatrix()).entries
						);
				else
					glLoadMatrixf(
						renderable->getGlobalMatrix().entries
						);

					//draw
					if(oldState==NULL){
						renderable->draw();
					}
					else{
						renderable->draw(oldState);
					}
					//set old state
					oldState=(RenderState*)renderable;
					//draw errors
					CHECK_GPU_ERRORS();
				}
			}
		}
	}
#endif
}
void Render::update(float dt){
	//update all layers
	for(auto layer:layers) 
		layer->update(dt);
}

void Render::updateProjection(){
	//set viewport
	viewport.x=Application::instance()->getScreen()->getWidth();
	viewport.y=Application::instance()->getScreen()->getHeight();
    //update projection is always the same
	projection.setOrtho(-viewport.x*0.5,viewport.x*0.5, -viewport.y*0.5,viewport.y*0.5, 1,-1);
}