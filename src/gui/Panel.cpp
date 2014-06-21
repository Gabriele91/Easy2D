#include <stdafx.h>
#include <Panel.h>

using namespace Easy2D;
using namespace Easy2D::Gui;

      
void Panel::updateBox()
{
    //update box
    box.setBox(guim->getOrigin()+position+scale/2.0, scale);
    box.setRegion(guim->getPickBox());
    //update pannels
    for(auto obj:panels)
    {
        obj->updateBox();
    }
    //update widgets
    for(auto obj:objects)
    {
        obj->updateBox();
    }
}
AABox2 Panel::getPickBox()
{
    return box;
}
//calc viewport
Vec4 Panel::calcViewport()
{
    if(guim->type==PANEL)
    {
        Vec4 viewport(this->viewport);
        Vec4 dad=((Panel*)guim)->calcViewport();
        //real pos
        viewport.x+=dad.x;
        viewport.y+=dad.y;
        //max x,y
        Vec2 vpmaxXY(viewport.z+viewport.x, viewport.w+viewport.y);
        Vec2 dadmaxXY(dad.z+dad.x, dad.w+dad.y);
        //sub pos
        viewport.x=Math::max(viewport.x,dad.x);
        viewport.y=Math::max(viewport.y,dad.y);
        //sub size
        viewport.z=Math::min(dadmaxXY.x,vpmaxXY.x)-viewport.x;
        viewport.w=Math::min(dadmaxXY.y,vpmaxXY.y)-viewport.y;
        //if max<min
        if(viewport.z<0 ||viewport.w<0)
        {
            viewport.z=0;
            viewport.w=0;
        }

        return viewport;
    }
    return viewport;
}
//recursive lock/unlock
void Panel::lockFocus()
{
    //lock this
    GuiManager::lockFocus();
    //lock parent
    guim->lockFocus();
}
void Panel::unlockFocus()
{
    //unlock this
    GuiManager::unlockFocus();
    //unlock parent
    guim->unlockFocus();
}
//events
void Panel::onMousePress(Vec2 pos,Key::Mouse b)
{
    //child focus
    bool childFocused=GuiManager::childFocus(pos,b);
    //also....
    if(!childFocused && isFocused() && b==Key::BUTTON_LEFT)
    {
        state=MOVE;
        lockFocus();
        pickpos=pos-position;
    }
}
void Panel::onMouseDown(Vec2 pos,Key::Mouse bt)
{
    //update pos
    if(state==MOVE)
        setPosition(pos-pickpos);
}
void Panel::onMouseRelease(Vec2 pos,Key::Mouse b)
{
    if(state==MOVE)
    {
        state=STAY;
        unlockFocus();
        pickpos=Vec2::ZERO;
    }
}


Panel::Panel(GuiManager* guim):GuiObject(guim)
                              ,GuiManager()
                              ,state(STAY)
{
    //type
    type=PANEL;
    //default no focus
    onFocus(false);
    //init GuiManager
    init(guim->rsgroup,guim->style);
    //save style info
    const Table& style=guim->style->getConstTable("panel");
    //style
    auto& textures=style.getConstTable("images");
    //draw info
    texture[0]=guim->loadTexture(textures.getString("unfocused"));
    texture[1]=guim->loadTexture(textures.getString("focused"));
    //get mesh
    mesh=guim->loadMesh(style.getString("mesh"));
    //this, append
    guim->addPanel(this);
    //setup render state
    enableBlend();
    setBlend(BLEND::SRC::ALPHA,BLEND::ONE::MINUS::SRC::ALPHA);
    setCull(GL_BACK);
}
void Panel::draw()
{
    setProjection(calcViewport());
    //draw box
    setMat4(Vec2::ZERO,scale);
    setTexture(texture[isFocused()]);
    setMesh(mesh);
    RenderState::draw();
    //draw all items
    drawItems();
    //reset projection
    guim->setProjection();
};

void Panel::setScale(const Vec2& scale)
{
    this->scale=scale;
    updateProjection(Vec4(position,scale));
    updateBox();
}        

void Panel::setPosition(const Vec2& pos)
{
    position=pos;
    updateProjection(Vec4(position,scale));
    updateBox();
}

GuiManager* Panel::toGuiManager()
{
    return this;
}