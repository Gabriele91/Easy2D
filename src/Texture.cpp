#include <stdafx.h>
#include <Texture.h>
#include <Debug.h>
#include <Application.h>
#define IMAGE_LOADER_OPENGL
#include "Image/Image.h"

using namespace Easy2D;


Texture::Texture(ResourcesGroup *rsmr,
				 const String& pathfile)
				:Resource(rsmr,pathfile)
				,bBilinear(true)
				,chBlr(true)
				,bMipmaps(true)
				,chMps(true)
				,bFlipVertical(false)
				,width(0)
				,height(0)
				,gpuid(0)
				,offsetUV(1,1)
				,po2Srpite(NULL){
	//is not loaded
	loaded=false;
}

Texture::~Texture(){
	//release resource
	release();
}
//flip vertical
bool Texture::flipVertical(){
	return bFlipVertical;
}
bool Texture::flipVertical(bool value){
	return bFlipVertical=value;
}
//texture enable
void Texture::bind(uint ntexture){
	//
	DEBUG_ASSERT(gpuid);
	//
    glActiveTexture( GL_TEXTURE0 + ntexture );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, (GLuint)gpuid );
	//settings
#ifndef DISABLE_MIDMAP
	if(chBlr)
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,bBilinear?GL_LINEAR:GL_NEAREST);
	if(chMps)
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,bMipmaps?GL_LINEAR_MIPMAP_NEAREST:GL_LINEAR);
#else
	if(chBlr)
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	if(chMps)
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
#endif
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
//operators
bool Texture::operator ==(const Texture& t) const{
	return gpuid==t.gpuid;
}
bool Texture::operator !=(const Texture& t) const{
	return gpuid!=t.gpuid;
}

//return mesh
Mesh::ptr Texture::getPO2Sprite(){
	//create texture
	if(!po2Srpite)
		po2Srpite=Mesh::ptr (new Mesh());
	//if loaded build mesh
	__build();
	//
	return po2Srpite;
}
//texture
void Texture::__build(){
	  //set loaded values
	  if(isLoad()&&po2Srpite){
		//clear cpu mesh
		po2Srpite->cpuClear();
		//unload gpu mesh
		if(po2Srpite->isLoad())
			po2Srpite->unload();
		//set size mesh
		float hlSizeX=width*0.5;
		float hlSizeY=height*0.5;
		//add vertexs
		po2Srpite->addVertex(  hlSizeX,
							  -hlSizeY,
							  offsetUV.x,
							  offsetUV.y);
		po2Srpite->addVertex(  hlSizeX,
							   hlSizeY,
							   offsetUV.x,
							   0.0);
		po2Srpite->addVertex( -hlSizeX,
							  -hlSizeY,
							   0.0,
							   offsetUV.y);
		po2Srpite->addVertex( -hlSizeX,
							   hlSizeY,
							   0.0,
							   0.0);
		//end add vertexs
		//set draw mode
		po2Srpite->setDrawMode(Mesh::TRIANGLE_STRIP);
		//build mesh
		po2Srpite->build();
	  }
  }

//load methods
bool Texture::load(){
	/////////////////////////////////////////////////////////////////////
	//cpu load
	//get raw file
	void *data=NULL; size_t len=0;
	Application::instance()->loadData(rpath,data,len);
	//load image
	Image image;
	image.loadFromData(data,
					   len,
					   Image::getTypeFromExtetion(rpath.getExtension()));
	if(bFlipVertical)
		image.flipY();
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
		//
		if(!Math::isPowerOfTwo(realWidth))
			realWidth=Math::nextPowerOfTwo(realWidth);
		if(!Math::isPowerOfTwo(realHeight))
			realHeight=Math::nextPowerOfTwo(realHeight);
		//calc offset uv
		offsetUV.x=(float)width/realWidth;
		offsetUV.y=(float)height/realHeight;
	}
#ifdef OPENGL_ES
	DEBUG_MESSAGE_IF(image.type==TYPE_ALPHA8,"WARRNING Texture: android not support alpha texture");
	if(image.type==TYPE_ALPHA8)
		image.convertAlphaTo32bit();	
	GLuint type=image.type;
#else	
	GLuint type= image.type== GL_ALPHA8 ? GL_ALPHA : image.type;
#endif
	//save internal type
	GLuint typeInternal=image.type;
	//create a gpu texture
	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			typeInternal,
			realWidth,
			realHeight,
			0,
			type,
			GL_UNSIGNED_BYTE,
			NULL );
#ifndef DISABLE_MIDMAP
	//create mipmaps
	if(bMipmaps) 
		glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, bMipmaps );
#endif
	//send to GPU
    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0,
					 width,
					 height,
					 type,
					 GL_UNSIGNED_BYTE,
					 image.bytes );
	
    CHECK_GPU_ERRORS();
	//is loaded
	loaded=true;
	//if olready getted, build mesh
	__build();
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
	//unbuild mesh
	if(po2Srpite)
		if(po2Srpite->isLoad())
			po2Srpite->unload();
	//is not loaded
	loaded=false;
	//return
    return !loaded;
}
bool Texture::loadFromBinaryData(std::vector<uchar>& bytes,
								 uint argWidth,
								 uint argHeight,
								 uint format,
								 uint type){
	//error if olready loaded
	DEBUG_ASSERT(!loaded);
	/////////////////////////////////////////////////////////////////////
	//gen gpu
	//create an GPU texture
	glGenTextures( 1, &gpuid );
	//build
	bind();
	//save width end height
	width=realWidth=argWidth;
	height=realHeight=argHeight;
	//resize
	//create a gpu texture
	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			format,
			realWidth,
			realHeight,
			0,
			type,
			GL_UNSIGNED_BYTE,
			NULL );

#ifndef DISABLE_MIDMAP
	//create mipmaps
	glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, bMipmaps );
#endif

	//send to GPU
    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0,
					 width,
					 height,
					 type,
					 GL_UNSIGNED_BYTE,
					 &bytes[0] );
	
    CHECK_GPU_ERRORS();
	//is loaded
	loaded=true;
	//is not relodable
	reloadable=false;
	//if olready getted, build mesh
	__build();
	//return
    return loaded;

}
