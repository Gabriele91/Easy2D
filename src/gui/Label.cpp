#include <stdafx.h>
#include <Label.h>

using namespace Easy2D;
using namespace Easy2D::Gui;

void Label::updateBox(){
    box.setBox(guim->getOrigin()+position+scale/2.0, scale);
    box.setRegion(guim->getPickBox());
}

Label::Label(GuiManager* guim):GuiObject(guim)
{
    //style
    const Table& style=guim->style->getConstTable("label");
    //get texture
    texture=guim->loadTexture(style.getString("image"));
    //get mesh
    mesh=guim->loadMesh(style.getString("mesh"));
    //load font
    font=guim->loadFont(style.getString("font"));
    //color font
    Vec4 color=style.getVector4D("color",Vec4(0,0,0,255));
    tColor=Color((uchar)color.r,(uchar)color.g,(uchar)color.b,(uchar)color.a);
    //this, append
    guim->addObject(this);
    //setup render state
    enableBlend();
    setBlend(BLEND::SRC::ALPHA,BLEND::ONE::MINUS::SRC::ALPHA);
    setCull(BACK);
}

void Label::setText(const String& name)
{
    text=name;
    textSize=font->textSize(text);
}

void Label::setScale(const Vec2& scale)
{
    this->scale=scale;
    updateBox();
}		
        
void Label::setPosition(const Vec2& pos)
{
    position=pos;
    updateBox();
}

void Label::draw()
{
    //build matrix
    guim->setMat4(position,scale);
    //draw box
    setTexture(texture);
    setMesh(mesh);
    RenderState::draw();
    //draw text
    font->text(box.getCenter()-textSize*0.5,text,tColor);
}