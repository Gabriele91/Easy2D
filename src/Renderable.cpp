#include <stdafx.h>
#include <Renderable.h>
#include <Resource.h>
#include <ResourcesManager.h>
#include <ResourcesGroup.h>
#include <Object.h>
///////////////////////
using namespace Easy2D;
///////////////////////
REGISTERED_COMPONENT(Renderable,"Renderable")
///////////////////////
Renderable::Renderable(Mesh::ptr rmesh,
                       Texture::ptr rtex,
                       bool visible)
    :RenderState()
    ,visible(visible)
    ,canBatch(true)
{
    setTexture(rtex);
    setMesh(rmesh);
}
bool Renderable::canBatching(Renderable *oldstate)
{
    return  rtexture==oldstate->rtexture &&
            rshader==oldstate->rshader &&
            blending==oldstate->blending&&
            blendSrc==oldstate->blendSrc&&
            blendDst==oldstate->blendDst&&
            cullmode==oldstate->cullmode&&
            color==oldstate->color;
}
//get box
AABox2 Renderable::getBox()
{
    AABox2  box=getBaseBox();
    Object* obj=getObject();
    return canTransform() && obj ?
           box.applay(obj->getGlobalMatrix()) :
           box;
}
//get box
AABox2 Renderable::getBaseBox()
{
    if(getMesh()) return getMesh()->getAABox();
    return AABox2();
}
//serialize/deserialize
void Renderable::serialize(Table& table)
{
    Table& rtable=table.createTable(getComponentName());
    //serialize render state
	rsSerialize(rtable);
    //serialize renderable
    if(getShader())
		rtable.set("shader", getShader()->getName());
    if(getTexture())
		rtable.set("texture", getTexture()->getName());
    if(getMesh())
        rtable.set("mesh", getMesh()->getName());
    //visible
    rtable.set("visible", isVisible() ? "yes" : "no");
    //batch
    rtable.set("canBatch", getCanBatch() ? "yes" : "no");
}
void Renderable::deserialize(const Table& table)
{
    //deserialize rander state
    rsDeserialize(table);
    //deserialize renderable
    if (table.existsAsType("visible", Table::STRING))
    {
        if (table.getString("visible", isVisible() ? "yes" : "no") != "no") show();
        else hide();
    }
    //batch
    if (table.existsAsType("canBatch", Table::STRING))
    {
        setCanBatch(table.getString("canBatch", getCanBatch() ? "yes" : "no") != "no");
    }
    //material
    if(table.existsAsType("shader",Table::STRING))
    {
        auto rsmanager=table.getResourcesManager();
        DEBUG_ASSERT(rsmanager);
        auto rsgroup=rsmanager->getResourcesGroup();
        DEBUG_ASSERT(rsgroup);
        setShader(rsgroup->load<Shader>(table.getString("shader")));
    }
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