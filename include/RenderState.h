#ifndef RENDERSTATE_H
#define RENDERSTATE_H

#include <Config.h>
#include <Mesh.h>
#include <Texture.h>
#include <Shader.h>
#include <Blend.h>
#include <Color.h>
#include <Table.h>
#include <RenderContext.h>

namespace Easy2D
{

class RenderState
{

protected:

    Texture::ptr rtexture;
    Mesh::ptr rmesh;
    Shader::ptr rshader;
    bool blending;
    uint blendSrc,blendDst;
    CullFace cullmode;
    Color color;
    //serialize/deserialize (utility)
    virtual void rsSerialize(Table& table);
    virtual void rsDeserialize(const Table& table);

public:

    RenderState()
        :rtexture(nullptr)
        ,rmesh(nullptr)
        ,rshader(nullptr)
        ,blending(false)
        ,blendSrc(BLEND::ONE)
        ,blendDst(BLEND::ZERO)
        ,cullmode(BACK)
    {}
    //draw
    virtual void draw();
    //enable setting (no draw)
    void enableStates();
    //
    virtual void setColor(const Color& rcolor)
    {
        color=rcolor;
    }
    virtual Color getColor() const
    {
        return color;
    }
    void setTexture(Texture::ptr rtex)
    {
        rtexture=rtex;
    }
    Texture::ptr getTexture() const 
    {
        return rtexture;
    }
    void setMesh(Mesh::ptr rm)
    {
        rmesh=rm;
    }
    Mesh::ptr getMesh() const
    {
        return rmesh;
    }
    void setShader(Shader::ptr rs)
    {
        rshader=rs;
    }
    Shader::ptr getShader() const
    {
        return rshader;
    }
    //blend state
    void enableBlend()
    {
        blending=true;
    }
    void disbaleBlend()
    {
        blending=false;
    }
    void setBlend(uint src,uint dst)
    {
        blendSrc=src;
        blendDst=dst;
    }
    uint getBlendSrc() const
    {
        return blendSrc;
    }
    uint getBlendDst() const
    {
        return blendDst;
    }
    bool isEnableBlend() const
    {
        return blending;
    }
    //cullmode
    void setCull(CullFace mode)
    {
        cullmode=mode;
    }
    CullFace getCullFace() const
    {
        return cullmode;
    }
    //operators
    bool operator==(const RenderState& rs) const
    {
        return getTexture()      ==rs.getTexture() &&
               getMesh()         ==rs.getMesh() &&
               getShader()       ==rs.getShader() &&
               isEnableBlend()   ==rs.isEnableBlend()&&
               getBlendSrc()     ==rs.getBlendSrc()&&
               getBlendDst()     ==rs.getBlendDst()&&
               getCullFace()     ==rs.getCullFace()&&
               getColor()        ==rs.getColor() ;
    }
    bool operator!=(const RenderState& rs) const
    {
        return !((*this)==rs);
    }

};

};

#endif