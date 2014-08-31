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


//serialize/deserialize
void Sprite::serialize(Table& table)
{
    Table& rsprite=table;
    //serialize render state
    rsSerialize(rsprite);
    //serialize texture
    if(getTexture())
        rsprite.set("texture",getTexture()->getName());
}
void Sprite::deserialize(const Table& table)
{
    //deserialize rander state
    rsDeserialize(table);
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
