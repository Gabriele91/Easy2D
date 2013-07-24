#include <stdafx.h>
#include <Sprite.h>

///////////////////////
using namespace Easy2D;
///////////////////////
Sprite::Sprite(Texture::ptr texture,Layer *layer)
			  :Renderable(NULL,texture,layer,true){
	//blend mode
	enableBlend();
	setBlend(BLEND::SRC::ALPHA, 
			 BLEND::ONE::MINUS::SRC::ALPHA);
	//if texture exist
	if(rtexture){
		//get sprite mesh
		setMesh(texture->getPO2Sprite());
	}
}

void Sprite::setTexture(Texture::ptr texture){
	//texture load
	if(!texture->isLoad()) 
			texture->load();
	//set texture
	Renderable::setTexture(texture);
	//get sprite mesh
	setMesh(texture->getPO2Sprite());	
}