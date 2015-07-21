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
    ,colorCascade(false)
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
            getColor()==oldstate->getColor();
}
//get color
Color Renderable::getParentColor() const
{
    //enable?
    if(!colorCascade) return Color::WHITE;
    //get object
    if(!getObject()) return Color::WHITE;
    //ptr to parent
    const Object* parent = getObject()->getParent();
    //exist parent?
    if(!parent) return Color::WHITE;
    //get component
    const Renderable* rparent= parent->getComponent<Renderable>();
    //exist component?
    if(!rparent) return Color::WHITE;
    //get parent
    return rparent->getColor();
}
//get box
AABox2 Renderable::getBox()
{
    AABox2  box=getBaseBox();
    return canTransform() ? box.applay(getModel()) : box;
}
//get box
AABox2 Renderable::getBaseBox()
{
    if(getMesh()) return getMesh()->getAABox();
    return AABox2();
}
//get model matrix
Mat4 Renderable::getModel()
{
    Object* obj = getObject();
    if (obj) return obj->getGlobalMatrix();
    else     return Mat4::IDENTITY;
}

//overload
void Renderable::rsSerialize(Table& table)
{
    RenderState::rsSerialize(table);
    //visible
    table.set("visible", isVisible() ? "yes" : "no");
    //batch
    table.set("canBatch", getCanBatch() ? "yes" : "no");
    //color cascate
    table.set("colorCascade", isEnableParentColor() ? "yes" : "no");
}
void Renderable::rsDeserialize(const Table& table)
{
    RenderState::rsDeserialize(table);
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
    //color cascate
    if (table.existsAsType("colorCascade", Table::STRING))
    {
        setEnableParentColor(table.getString("colorCascade", getCanBatch() ? "yes" : "no") != "no");
    }
}
//serialize/deserialize
void Renderable::serialize(Table& table)
{
    //serialize render state
	rsSerialize(table);
    //serialize renderable
    if(getShader())
		table.set("shader", getShader()->getName());
    if(getTexture())
		table.set("texture", getTexture()->getName());
    if(getMesh())
        table.set("mesh", getMesh()->getName());
}
void Renderable::deserialize(const Table& table)
{
    //deserialize rander state
    rsDeserialize(table);
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