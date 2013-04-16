#include <stdafx.h>
#include <Sprite.h>

///////////////////////
using namespace Easy2D;
///////////////////////

void Sprite::_buildMesh(){
	if(rtexture){
		//sprite
		if(!rmesh)
			setMesh(Mesh::ptr(new Mesh()));
		//add vertexs
		rmesh->addVertex(  0.5,  
						  -0.5,  
						  rtexture->getOffestUV().x, 
						  rtexture->getOffestUV().y);
		rmesh->addVertex(  0.5,   
						   0.5,  
						   rtexture->getOffestUV().x, 
						   0.0);
		rmesh->addVertex( -0.5,  
						  -0.5,  
						   0.0, 
						   rtexture->getOffestUV().y);
		rmesh->addVertex( -0.5,   
						   0.5,  
						   0.0, 
						   0.0);
		//end add vertexs
		//set draw mode
		rmesh->setDrawMode(Mesh::TRIANGLE_STRIP);
		//build mesh
		rmesh->build();
	}
}

Sprite::Sprite(Texture::ptr texture,Layer *layer)
			  :Renderable(NULL,texture,layer,true){
	//blend mode
	enableBlend();
	setBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//if texture exist
	if(rtexture){
		//setScale
		setScale(Vec2(rtexture->getWidth(),rtexture->getHeight()));
		//build mesh
		_buildMesh();
	}
}

void Sprite::setTexture(Texture::ptr texture){
	//texture load
	if(!texture->isLoad()) texture->load();
	//if the new texture different  texture uv, make a new sprite...
	if(!rtexture || rtexture->getOffestUV()!=texture->getOffestUV()){	
		Renderable::setTexture(texture);
		_buildMesh();
	}
	//else set texture
	else{	
		Renderable::setTexture(texture);		
	}
	//setScale
	setScale(Vec2(texture->getWidth(),texture->getHeight()));
}