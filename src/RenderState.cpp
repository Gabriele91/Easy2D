#include <stdafx.h>
#include <RenderState.h>
///////////////////////
using namespace Easy2D;
void RenderState::enableStates()
{
    ///////////////////////////////////
    //enable blend
    if(blending)
    {
        RenderContext::setBlend(true);
        RenderContext::setBlendFunction(blendSrc,blendDst);
    }
    //disable blend
    else
        RenderContext::setBlend(false);
    ///////////////////////////////////
    //cull mode
    RenderContext::setCullFace(cullmode);
    ///////////////////////////////////
    //color mode
    RenderContext::setColor(color);
    ///////////////////////////////////
    //set shader
    if(getShader()) getShader()->bind();
    //else
    else RenderContext::disableProgram();
    ///////////////////////////////////
    //set texture
    if(rtexture) rtexture->bind();
}

void RenderState::draw()
{
    ///////////////////////////////////
    //enable state settings
    enableStates();
    ///////////////////////////////////
    //draw mesh
    if(rmesh) rmesh->draw();
}
inline String cullToString(int cull)
{
    switch (cull)
    {
        case BACK: return "BACK";
        case FRONT: return "FRONT";
        default: return "DISABLE";
    }
}
inline CullFace stringToCall(const String& cull)
{
    if(cull.toUpper()=="FRONT") return FRONT;
    if(cull.toUpper()=="BACK") return BACK;
    return DISABLE;
}

void RenderState::rsSerialize(Table& table)
{
    table.set("color",color.toVec4());
    table.set("culling",cullToString(cullmode));
    if(blending)
    {
        Table& blend=table.createTable("blend");
        blend.set("src",BLEND::toString(blendSrc));
        blend.set("dst",BLEND::toString(blendDst));
    }
}
void RenderState::rsDeserialize(const Table& table)
{
    color.fromVec4(table.getVector4D("color",Color().toVec4()));
    cullmode=stringToCall(table.getString("culling","BACK"));
    if(table.existsAsType("blend",Table::TABLE))
    {
        blending=true;
        const Table& blend=table.getConstTable("blend");
        blendSrc=BLEND::fromString(blend.getString("src","ONE"),BLEND::ONE);
        blendDst=BLEND::fromString(blend.getString("dst","ZERO"),BLEND::ZERO);
    }

}
