#include <stdafx.h>
#include <Sprite.h>

///////////////////////
using namespace Easy2D;
///////////////////////
Sprite::Sprite(Texture::ptr texture,Layer *layer)
			  :Renderable(NULL,texture,layer,true){
	//blend mode
	enableBlend();
	setBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//if texture exist
	if(rtexture){
		//setScale
		setScale(Vec2(rtexture->getWidth(),rtexture->getHeight()));
		//get sprite mesh
		setMesh(texture->getPO2Sprite());
	}
}

void Sprite::setTexture(Texture::ptr texture){
	//texture load
	if(!texture->isLoad()) texture->load();
	//set texture
	Renderable::setTexture(texture);
	//get sprite mesh
	setMesh(texture->getPO2Sprite());	
	//setScale
	setScale(Vec2(texture->getWidth(),texture->getHeight()));
}