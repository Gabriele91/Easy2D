#include <stdafx.h>
#include <Texture.h>
#include <Application.h>
#define IMAGE_LOADER_OPENGL
#include "Image/image.h"

using namespace Easy2D;


Texture::Texture(ResourcesManager<Texture> *rsmr,
				 const String& pathfile)
				:Resource(rsmr,pathfile)
				,bBilinear(true)
				,chBlr(true)
				,bMipmaps(true)
				,chMps(true)
				,width(0)
				,height(0)
				,gpuid(0){
	//is not loaded
	loaded=false;
}
		
void Texture::bind(uint ntexture){
	//
	DEBUG_ASSERT(gpuid);
	//
    glActiveTexture( GL_TEXTURE0 + ntexture );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, (GLuint)gpuid );
	//settings
	if(chBlr)
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,bBilinear?GL_LINEAR:GL_NEAREST);
	if(chMps)
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,bMipmaps?GL_LINEAR_MIPMAP_NEAREST:GL_LINEAR);  
}
//setting
bool Texture::bilinear(){
	return bBilinear;
}
bool Texture::bilinear(bool value){
	chBlr=bBilinear!=value;
	return bBilinear=value;	
}
bool Texture::mipmaps(){
	return bMipmaps;
}
bool Texture::mipmaps(bool value){
	chMps=bMipmaps!=value;
	return bMipmaps=value;
}

//load methods
bool Texture::load(){	
	/////////////////////////////////////////////////////////////////////
	//cpu load
	//get raw file
	void *data=NULL; uint len=0;
	Application::instance()->loadData(rpath,data,len);
	//load image
	Image image;
	image.loadFromData(data,
					   len,
					   Image::getTypeFromExtetion(rpath.getExtension()));
	//free raw file
	free(data);
	/////////////////////////////////////////////////////////////////////
	//gen gpu
	//create an GPU texture
	glGenTextures( 1, &gpuid );
	//build 
	bind();   
	//save width end height
	width=realWidth=image.width;
	height=realHeight=image.height;
	//support only pow of 2?
	if(Application::instance()->onlyPO2()){
		if(!Math::isPowerOfTwo(realWidth))
			realWidth=Math::nextPowerOfTwo(realWidth);
		if(!Math::isPowerOfTwo(realHeight))
			realHeight=Math::nextPowerOfTwo(realHeight);
	}	
	//create a gpu texture
	glTexImage2D(
			GL_TEXTURE_2D, 
			0, 
			image.type,
			realWidth, 
			realHeight,
			0, 
			image.type,
			GL_UNSIGNED_BYTE, 
			NULL );
	//create mipmaps
	glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, bMipmaps );
	//send to GPU
    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 
					 width, 
					 height, 
					 image.type, 
					 GL_UNSIGNED_BYTE, 
					 image.bytes );
	//is loaded
	loaded=true;
	//return
    return loaded;
}
bool Texture::unload(){
	//unload
	DEBUG_ASSERT(gpuid);
    glDeleteTextures(1, &gpuid );
	//reset values
    width = height = 0;
    realWidth = realHeight = 0;
    gpuid = 0;
	//is not loaded
	loaded=false;
	//return
    return !loaded;
}