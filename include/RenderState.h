#ifndef RENDERSTATE_H
#define RENDERSTATE_H

#include <Config.h>
#include <Mesh.h>
#include <Texture.h>
#include <Blend.h>

namespace Easy2D {

	class RenderState{
	
	protected:

		Texture::ptr rtexture;
		Mesh::ptr rmesh;
		bool blending;
		uint blendSrc,blendDst;
		uint cullmode;

	public:

		RenderState()
			:rtexture(NULL)
			,rmesh(NULL)
			,blending(false)
			,blendSrc(BLEND::ONE)
			,blendDst(BLEND::ZERO)
			,cullmode(GL_BACK)
		{}
		//draw
		void draw();
		void draw(RenderState *oldstate);
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
		void setCull(uint mode){
			cullmode=mode;
		}


	};

};

#endif