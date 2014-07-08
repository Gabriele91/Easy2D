#include <stdafx.h>
#include <Renderable.h>
#include <Resource.h>
#include <ResourcesManager.h>
#include <ResourcesGroup.h>
///////////////////////
using namespace Easy2D;
///////////////////////
REGISTERED_COMPONENT(Renderable,"Renderable")
///////////////////////
Renderable::Renderable(Mesh::ptr rmesh, Texture::ptr rtex, bool visible)
    :RenderState()
    ,visible(visible)
{
    setTexture(rtex);
    setMesh(rmesh);
}
bool Renderable::canBatching(Renderable *oldstate)
{
    return  rtexture==oldstate->rtexture &&
            blending==oldstate->blending&&
            blendSrc==oldstate->blendSrc&&
            blendDst==oldstate->blendDst&&
            cullmode==oldstate->cullmode&&
            color==oldstate->color;
}

//serialize/deserialize
void Renderable::serialize(Table& table)
{
    Table& rsprite=table.createTable(getComponentName());
    //serialize render state
    rsSerialize(rsprite);
    //serialize renderable
    rsprite.set("texture",getTexture()->getName());
    rsprite.set("mesh",getMesh()->getName());
}
void Renderable::deserialize(const Table& table)
{
    //deserialize rander state
    rsDeserialize(table);
    //deserialize renderable
    if(table.existsAsType("texture",Table::STRING))
    {
        auto rsmanager=table.getResourcesManager();
        DEBUG_ASSERT(rsmanager);
        auto rsgroup=rsmanager->getResourcesGroup();
        DEBUG_ASSERT(rsgroup);
        setTexture(rsgroup->load<Texture>(table.getString("texture")));
    }    
    if(table.existsAsType("Mesh",Table::STRING))
    {
        auto rsmanager=table.getResourcesManager();
        DEBUG_ASSERT(rsmanager);
        auto rsgroup=rsmanager->getResourcesGroup();
        DEBUG_ASSERT(rsgroup);
        setMesh(rsgroup->load<Mesh>(table.getString("mesh")));
    }
}