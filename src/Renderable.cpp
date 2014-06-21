#include <stdafx.h>
#include <Renderable.h>
#include <Layer.h>
#include <Resource.h>
#include <ResourcesManager.h>
#include <ResourcesGroup.h>
///////////////////////
using namespace Easy2D;
///////////////////////
REGISTERED_COMPONENT(Renderable,"Renderable")
///////////////////////
Renderable::Renderable(Mesh::ptr rmesh, Texture::ptr rtex, Layer *rlayer, bool visible)
    :RenderState()
    ,rlayer(rlayer)
    ,visible(visible)
    ,zvalue(0.0)
{
    setTexture(rtex);
    setMesh(rmesh);
}
void  Renderable::setZ(float z)
{
    zvalue=z;
    if(rlayer)
    {
        rlayer->change();
    }
};
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
    rsprite.set("texture",getTexture()->getName());
    rsprite.set("mesh",getMesh()->getName());
}
void Renderable::deserialize(const Table& table)
{
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