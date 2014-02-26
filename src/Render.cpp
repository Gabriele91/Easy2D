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
    reorder=false;
	//set orientation
	updateProjection();
}

/**
 * Sorting layers
 */
bool Render::operator_lt(const Layer* lrs,const Layer* rrs){
    return lrs->getZ()<rrs->getZ();
}
void Render::change(){
    this->reorder=true;
}
void Render::dosort(){
    this->reorder=false;
    std::sort(layers.begin(), layers.end(),  Render::operator_lt);
}

Layer* Render::addLayer(bool order){
    //make a layer
	Layer *newLayer=order? (Layer*)new LayerOrder(this):
						   (Layer*)new LayerUnorder(this);
	layers.push_back(newLayer);
    //resort
    change();
    //return new layer
	return newLayer;
}
void Render::erseLayer(Layer* layer){
	auto it=std::find(layers.begin(), layers.end(), layer);
	(*it)->dtRender();
    layers.erase(it);
    
}

void Render::draw(){
#ifdef ENABLE_CPU_BATCHING_MESH 
	#define MAX_BUFFER_TRIANGLES 2000 //2K
	//create buffer
	if(!batchingMesh.getBufferSize())
			batchingMesh.createBufferByTriangles(MAX_BUFFER_TRIANGLES);
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
	//no matrix camera
	if(!camera)
		glLoadIdentity();
    //reourder layers
    if(reorder)
        dosort();
	//for all layers
	for(auto layer:layers){
		//layer is visible?
		if(layer->isVisible()){
			//update layer
			layer->dosort();
			//get the first rendarable
			rNext=layer->next();
            //parallax
            if(camera){
                Matrix4x4 view=camera->getGlobalMatrix();
                view[12]*=layer->getParallax().x;
                view[13]*=layer->getParallax().y;
                glLoadMatrixf(view);
            }
			//for all renderables
			while(rNext!=NULL){
				//get next rendarable
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
    //reourder layers
    if(reorder)
        dosort();
	//for all layers
	for(auto layer:layers){
		//layer is visible?
		if(layer->isVisible()){
			//update layer
			layer->dosort();
			//for all renderables
			while(Renderable *renderable=layer->next()){
				//renderable is visible?
				if(renderable->isVisible()){
				//calc m4x4
                if(camera){
                    //parallax
                    Matrix4x4 view=camera->getGlobalMatrix();
                    view[12]*=layer->getParallax().x;
                    view[13]*=layer->getParallax().y;
                    //modelview
					glLoadMatrixf(view.mul2D(renderable->getGlobalMatrix()));
                }
				else
					glLoadMatrixf(renderable->getGlobalMatrix());

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
	updateProjection(Application::instance()->getScreen()->getSize());
}

void Render::updateProjection(const Vec2& argViewport){
	//set viewport
	viewport=argViewport;
    //update projection is always the same
	projection.setOrtho(-viewport.x*0.5,viewport.x*0.5, -viewport.y*0.5,viewport.y*0.5, 1,-1);

}
void Render::updateViewport(const Vec2& argViewport){
	//set viewport
	viewport=argViewport;
}