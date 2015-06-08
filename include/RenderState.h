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
    void setColor(const Color& rcolor)
    {
        color=rcolor;
    }
    const Color& getColor() const 
    {
        return color;
    }
    void setTexture(Texture::ptr rtex)
    {
        rtexture=rtex;
    }
    Texture::ptr getTexture()
    {
        return rtexture;
    }
    void setMesh(Mesh::ptr rm)
    {
        rmesh=rm;
    }
    Mesh::ptr getMesh()
    {
        return rmesh;
    }
    void setShader(Shader::ptr rs)
    {
        rshader=rs;
    }
    Shader::ptr getShader()
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
    bool getIsEnableBlend() const
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
        return rtexture==rs.rtexture &&
               rmesh==rs.rmesh &&
               rshader==rs.rshader &&
               blending==rs.blending&&
               blendSrc==rs.blendSrc&&
               blendDst==rs.blendDst&&
               cullmode==rs.cullmode&&
               color==rs.color ;
    }
    bool operator!=(const RenderState& rs) const
    {
        return !((*this)==rs);
    }

};

};

#endif