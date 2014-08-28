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
    //set texture
    rtexture->bind();
}

void RenderState::draw()
{
    ///////////////////////////////////
    //enable state settings
    enableStates();
    ///////////////////////////////////
    //draw mesh
    rmesh->draw();
}


static inline String funcBlendToString(int fun)
{
    switch (fun)
    {
        case GL_ONE: return "ONE";
        case GL_ZERO: return "ZERO";

        case GL_ONE_MINUS_DST_COLOR: return "ONE::MINUS::DST::COLOR";
        case GL_ONE_MINUS_DST_ALPHA: return "ONE::MINUS::DST::ALPHA";
        case GL_ONE_MINUS_SRC_COLOR: return "ONE::MINUS::SRC::COLOR";
        case GL_ONE_MINUS_SRC_ALPHA: return "ONE::MINUS::SRC::ALPHA";


        case GL_DST_COLOR: return "DST::COLOR";
        case GL_DST_ALPHA: return "DST::ALPHA";

        case GL_SRC_COLOR: return "SRC::COLOR";
        case GL_SRC_ALPHA: return "SRC::ALPHA";
        case GL_SRC_ALPHA_SATURATE: return "SRC::ALPHA::SATURATE";
        default: return "";
    }
}
static inline int stringToFuncBlend(String fun,int vlDefault)
{
    //normalize
    fun.replaceAll(" ","");
    fun=fun.toUpper();
    //
    if(fun=="ONE") return GL_ONE;
    if(fun=="ZERO") return GL_ZERO;
    //
    if(fun=="ONE::MINUS::DST::COLOR") return GL_ONE_MINUS_DST_COLOR;
    if(fun=="ONE::MINUS::DST::ALPHA") return GL_ONE_MINUS_DST_ALPHA;
    if(fun=="ONE::MINUS::SRC::COLOR") return GL_ONE_MINUS_SRC_COLOR;
    if(fun=="ONE::MINUS::SRC::ALPHA") return GL_ONE_MINUS_SRC_ALPHA;
    //
    if(fun=="DST::COLOR") return GL_DST_COLOR;
    if(fun=="DST::ALPHA") return GL_DST_ALPHA;
    //
    if(fun=="SRC::COLOR") return GL_SRC_COLOR;
    if(fun=="SRC::ALPHA") return GL_SRC_ALPHA;
    if(fun=="SRC::ALPHA::SATURATE") return GL_SRC_ALPHA_SATURATE;

    return vlDefault;
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
        blend.set("src",funcBlendToString(blendSrc));
        blend.set("dst",funcBlendToString(blendDst));
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
        blendSrc=stringToFuncBlend(blend.getString("src","ONE"),BLEND::ONE);
        blendDst=stringToFuncBlend(blend.getString("dst","ZERO"),BLEND::ZERO);
    }

}
