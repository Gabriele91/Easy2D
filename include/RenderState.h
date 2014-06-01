#ifndef RENDERSTATE_H
#define RENDERSTATE_H

#include <Config.h>
#include <Mesh.h>
#include <Texture.h>
#include <Blend.h>
#include <Color.h>

namespace Easy2D {

	class RenderState{
	
	protected:

		Texture::ptr rtexture;
		Mesh::ptr rmesh;
		bool blending;
		uint blendSrc,blendDst;
		uint cullmode;
		Color color;

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
		//enable setting (no draw)
		void enableStates();
		void enableStates(RenderState *oldstate);
		//
		void setColor(Color rcolor){ color=rcolor; }
		void setTexture(Texture::ptr rtex){ rtexture=rtex; }
		Texture::ptr getTexture(){ return rtexture; }
		void setMesh(Mesh::ptr rm){ rmesh=rm; }
		Mesh::ptr getMesh(){ return rmesh; }
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
		//operators
		bool operator==(const RenderState& rs){
			return rtexture==rs.rtexture &&
				   rmesh==rs.rmesh &&
				   blending==rs.blending&&
				   blendSrc==rs.blendSrc&&
				   blendDst==rs.blendDst&&
				   cullmode==rs.cullmode&&
				   color==rs.color ;
		}
		bool operator!=(const RenderState& rs){
			return !((*this)==rs);
		}

	};

};

#endif