#include <stdafx.h>
#include <Renderable.h>
#include <Layer.h>
///////////////////////
using namespace Easy2D;
///////////////////////
Renderable::Renderable(Mesh::ptr rmesh, Texture::ptr rtex, Layer *rlayer, bool visible)
					  :RenderState()
					  ,rlayer(rlayer)
					  ,visible(visible)
					  ,zvalue(0.0){
	setTexture(rtex);
	setMesh(rmesh);
}
void  Renderable::setZ(float z){ 
	zvalue=z; 
	if(rlayer){
		rlayer->change();
	}
};