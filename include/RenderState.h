#ifndef RENDERSTATE_H
#define RENDERSTATE_H

#include <Config.h>
#include <Mesh.h>
#include <Texture.h>

namespace Easy2D {

	class RenderState{
	
	protected:

		Texture::ptr rtexture;
		Mesh::ptr rmesh;
		bool blending;
		uint blendSrc,blendDst;
		uint cullmode;

	public:

		//
		void draw();
		void forceDraw();
		//
		void setTexture(Texture::ptr rtex){ rtexture=rtex; }
		void setMesh(Mesh::ptr rm){ rmesh=rm; }
		//blend state
		void enableBlend(){ blending=true; }
		void disbaleBlend(){ blending=false;  }
		void setBlend(uint src,uint dst){
			blendSrc=src;
			blendDst=dst;
		}
		//cullmode
		void setCull(uint cullmode);


	};

};

#endif