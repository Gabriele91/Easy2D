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
void RenderState::enableStates(RenderState *oldstate)
{
    ///////////////////////////////////
    //enable blend
    if(blending)
    {
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

void RenderState::draw()
{
    ///////////////////////////////////
    //enable state settings
    enableStates();
    ///////////////////////////////////
    //draw mesh
    rmesh->draw();
}

void RenderState::draw(RenderState *oldstate)
{
    ///////////////////////////////////
    //enable state settings
    enableStates(oldstate);
    ///////////////////////////////////
    //draw mesh
    rmesh->draw();
}


inline String funcBlendToString(int fun)
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
inline int stringToFuncBlend(const String& fun,int vlDefault)
{
    if(fun.toUpper()=="ONE") return GL_ONE;
    if(fun.toUpper()=="ZERO") return GL_ZERO;

    if(fun.toUpper()=="ONE::MINUS::DST::COLOR") return GL_ONE_MINUS_DST_COLOR;
    if(fun.toUpper()=="ONE::MINUS::DST::ALPHA") return GL_ONE_MINUS_DST_ALPHA;
    if(fun.toUpper()=="ONE::MINUS::SRC::COLOR") return GL_ONE_MINUS_SRC_COLOR;
    if(fun.toUpper()=="ONE::MINUS::SRC::ALPHA") return GL_ONE_MINUS_SRC_ALPHA;
    
    if(fun.toUpper()=="DST::COLOR") return GL_DST_COLOR;
    if(fun.toUpper()=="DST::ALPHA") return GL_DST_ALPHA;

    if(fun.toUpper()=="SRC::COLOR") return GL_SRC_COLOR;
    if(fun.toUpper()=="SRC::ALPHA") return GL_SRC_ALPHA;
    if(fun.toUpper()=="SRC::ALPHA::SATURATE") return GL_SRC_ALPHA_SATURATE;

    return vlDefault;
}
inline String cullToString(int cull)
{
    switch (cull)
    {
        case GL_BACK: return "BACK";
        case GL_FRONT: return "FRONT";
        default: return "";
    }
}
inline int stringToCall(const String& cull)
{
    if(cull.toUpper()=="FRONT") return GL_FRONT;
    return GL_BACK;
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