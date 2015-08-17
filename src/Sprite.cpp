#include <stdafx.h>
#include <Sprite.h>
#include <Resource.h>
#include <ResourcesManager.h>
#include <ResourcesGroup.h>

///////////////////////
using namespace Easy2D;
///////////////////////
REGISTERED_COMPONENT(Sprite,"Sprite")
///////////////////////
Sprite::Sprite(Texture::ptr texture):Renderable(nullptr,texture,true)
{
    //blend mode
    enableBlend();
    setBlend(BLEND::SRC::ALPHA,
             BLEND::ONE::MINUS::SRC::ALPHA);
    //if texture exist
    if(rtexture)
    {
        //get sprite mesh
        setMesh(texture->getPO2Sprite());
    }
}

//protected set mesh
void Sprite::setMesh(Mesh::ptr mesh)
{
    Renderable::setMesh(mesh);
}

//set texture
void Sprite::setTexture(Texture::ptr texture)
{
    //texture load
    if(!texture->isLoad())
        texture->load();
    //set texture
    Renderable::setTexture(texture);
    //get sprite mesh
    setMesh(texture->getPO2Sprite());
}

Vec2 Sprite::getPixelScale()
{
    if(getTexture() && getObject())
    {
        return getTexture()->getSpriteSize()*getObject()->getScale();
    }
    else if(getTexture())
    {
        return getTexture()->getSpriteSize();
    }
    return Vec2::ZERO;
}

void Sprite::setPixelScale(const Vec2& scale,bool global)
{
    if(getTexture() && getObject())
    {
        getObject()->setScale(scale/getTexture()->getSpriteSize(),global);
    }
    else if(getObject())
    {
        getObject()->setScale(scale,global);
    }
}
//serialize/deserialize
void Sprite::serialize(Table& table)
{
    Table& rsprite=table;
    //serialize render state
	rsSerialize(rsprite);
    //serialize shader
    if(getShader())
        rsprite.set("shader",getShader()->getName());
    //serialize texture
    if(getTexture())
		rsprite.set("texture", getTexture()->getName());
}
void Sprite::deserialize(const Table& table)
{
    //deserialize rander state
	rsDeserialize(table);
    //get shader
    if(table.existsAsType("shader",Table::STRING))
    {
        auto rsmanager=table.getResourcesManager();
        DEBUG_ASSERT(rsmanager);
        auto rsgroup=rsmanager->getResourcesGroup();
        DEBUG_ASSERT(rsgroup);
        setShader(rsgroup->load<Shader>(table.getString("shader")));
    }
    //get texture
    if(table.existsAsType("texture",Table::STRING))
    {
        auto rsmanager=table.getResourcesManager();
        DEBUG_ASSERT(rsmanager);
        auto rsgroup=rsmanager->getResourcesGroup();
        DEBUG_ASSERT(rsgroup);
        setTexture(rsgroup->load<Texture>(table.getString("texture")));
    }
}
