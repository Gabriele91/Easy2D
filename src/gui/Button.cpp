#include <stdafx.h>
#include <Button.h>

using namespace Easy2D;
using namespace Easy2D::Gui;

void Button::updateBox(){
    box.setBox(guim->getOrigin()+position+scale/2.0, scale);
    box.setRegion(guim->getPickBox());
}
//event

void Button::onMouseMove(Vec2 pos)
{
    if(!guim->isFocused())
    { 
        state=UP; 
        return; 
    }

    if(state!=DOWN)
    {
        if(box.isIntersection(pos))
            state=OVER;
        else
            state=UP;
    }
}
void Button::onMouseDown(Vec2 pos,Key::Mouse b)
{
    if(!guim->isFocused())
    { 
        state=UP;
        return; 
    }

    if(state!=DOWN)
    {
        if(b==Key::BUTTON_LEFT)
            if(box.isIntersection(pos))
                state=DOWN;
    }
}    
void Button::onMouseRelease(Vec2 pos,Key::Mouse b)
{         
    if(!guim->isFocused())
    { 
        state=UP; 
        return; 
    }

    //click?
    if(state==DOWN)
    {
        if(b==Key::BUTTON_LEFT)
        {
            if(box.isIntersection(pos))
            {
                if(eventClick)
                    eventClick();
            }
        }
    }
    //reset
    if(box.isIntersection(pos))
        state=OVER;
    else
        state=UP;
}


Button::Button(Panel* panel) 
			  :Button((GuiManager*)panel)
{
}
Button::Button(GuiManager* guim):GuiObject(guim)
                                ,state(UP)
                                ,eventClick(nullptr)
{
    //event
    Application::instance()->getInput()->addHandler(this);
    //style
    const Table& style=guim->style->getConstTable("buttom");
    //style
    auto& textures=style.getConstTable("images");
    //draw info
    texture[UP]=guim->loadTexture(textures.getString("up"));
    texture[OVER]=guim->loadTexture(textures.getString("over"));
    texture[DOWN]=guim->loadTexture(textures.getString("down"));
    //style
    auto& meshs=style.getConstTable("meshs");
    //mesh
    mesh[UP]=guim->loadMesh(meshs.getString("up"));
    mesh[OVER]=guim->loadMesh(meshs.getString("over"));
    mesh[DOWN]=guim->loadMesh(meshs.getString("down"));
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
Button::~Button()
{
    Application::instance()->getInput()->removeHandler(this);
}

void Button::setText(const String& name)
{
    text=name;
    textSize=font->textSize(text);
}

void Button::setScale(const Vec2& scale)
{
    this->scale=scale;
    updateBox();
}        
        
void Button::setPosition(const Vec2& pos)
{
    position=pos;
    updateBox();
}

void Button::onClick(Function<void()> event)
{
    eventClick=event;
}

void Button::draw()
{
    //build matrix
    guim->setMat4(position,scale);
    //draw box
    setTexture(texture[state]);
    setMesh(mesh[state]);
    RenderState::draw();
    //draw text
    font->text(box.getCenter()-textSize*0.5,text,tColor);
}
