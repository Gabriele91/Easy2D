#include <stdafx.h>
#include <RenderState.h>
///////////////////////
using namespace Easy2D;
void RenderState::enableStates(){
	///////////////////////////////////
	//enable blend
	if(blending){
		glEnable(GL_BLEND);
		glBlendFunc(blendSrc,blendDst);
	}
	//disable blend
	else
		glDisable(GL_BLEND);
	///////////////////////////////////
	//cull mode
	glCullFace(cullmode);
	///////////////////////////////////
	//color mode
	glColor4ub(color.r,color.g,
			   color.b,color.a);
	///////////////////////////////////
	//set texture
	rtexture->bind();
}
void RenderState::enableStates(RenderState *oldstate){
	///////////////////////////////////
	//enable blend
	if(blending){
		if(oldstate->blending!=blending)
			glEnable(GL_BLEND);
		if(oldstate->blendSrc!=blendSrc||
		   oldstate->blendDst!=blendDst)
		   glBlendFunc(blendSrc,blendDst);
	}
	//disable blend
	else if(oldstate->blending!=blending)
		glDisable(GL_BLEND);
	///////////////////////////////////
	//cull mode
	if(oldstate->cullmode!=cullmode)
		glCullFace(cullmode);
	///////////////////////////////////
	//color mode
	if(oldstate->color!=color)
		glColor4ub(color.r,color.g,
				   color.b,color.a);
	///////////////////////////////////
	//set texture
	if((*oldstate->rtexture)!=(*rtexture))
		rtexture->bind();
}

void RenderState::draw(){
	///////////////////////////////////
	//enable state settings
	enableStates();
	///////////////////////////////////
	//draw mesh
	rmesh->draw();
}

void RenderState::draw(RenderState *oldstate){
	///////////////////////////////////
	//enable state settings
	enableStates(oldstate);
	///////////////////////////////////
	//draw mesh
	rmesh->draw();
}