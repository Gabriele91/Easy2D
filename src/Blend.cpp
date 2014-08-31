#include <stdafx.h>
//
//  Blend.cpp
//  Easy2D
//
//  Created by Gabriele Di Bari on 30/08/14.
//
//
#include <EString.h>
#include <Blend.h>


//utilities
Easy2D::String Easy2D::BLEND::toString(uint blend)
{
    switch (blend)
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
uint Easy2D::BLEND::fromString(Easy2D::String blend,int vlDefault)
{
    //normalize
    blend.replaceAll(" ","");
    blend=blend.toUpper();
    //
    if(blend=="ONE") return GL_ONE;
    if(blend=="ZERO") return GL_ZERO;
    //
    if(blend=="ONE::MINUS::DST::COLOR") return GL_ONE_MINUS_DST_COLOR;
    if(blend=="ONE::MINUS::DST::ALPHA") return GL_ONE_MINUS_DST_ALPHA;
    if(blend=="ONE::MINUS::SRC::COLOR") return GL_ONE_MINUS_SRC_COLOR;
    if(blend=="ONE::MINUS::SRC::ALPHA") return GL_ONE_MINUS_SRC_ALPHA;
    //
    if(blend=="DST::COLOR") return GL_DST_COLOR;
    if(blend=="DST::ALPHA") return GL_DST_ALPHA;
    //
    if(blend=="SRC::COLOR") return GL_SRC_COLOR;
    if(blend=="SRC::ALPHA") return GL_SRC_ALPHA;
    if(blend=="SRC::ALPHA::SATURATE") return GL_SRC_ALPHA_SATURATE;
    
    return vlDefault;
}