#include <stdafx.h>
#include <Panel.h>
#include <GuiManager.h>
#include <RenderContext.h>

using namespace Easy2D;
using namespace Easy2D::Gui;

//reg an object
void GuiManager::addObject(GuiObject* obj)
{
    objects.push_back(obj);
}
//reg an object
void GuiManager::addPanel(Panel* obj)
{
    panels.push_back(obj);
}
//utility
Texture::ptr GuiManager::loadTexture(const String& name)
{
    return rsgroup->load<Texture>(name);
}
Mesh::ptr GuiManager::loadMesh(const String& name)
{
    return rsgroup->load<Mesh>(name);
}
Font::ptr GuiManager::loadFont(const String& name)
{
    return rsgroup->load<Font>(name);
}
        
//utility widgets
Vec4 GuiManager::getFlipViewport(const Vec4& xyzw)
{
    Vec2 size=Application::instance()->getScreen()->getSize();
    return Vec4(xyzw.x,size.y-(xyzw.y+xyzw.w),xyzw.z,xyzw.w);
}
Vec2 GuiManager::getFlipY(const Vec2& xy,const Vec2& scale)
{
    Vec2 size=getSize();
    return Vec2(xy.x,size.y-(xy.y+scale.y));
}
void GuiManager::setMat4(const Vec2& position,const Vec2& scale)
{
    Mat4 mat4;
    mat4.setTranslation(getFlipY(position,scale));
    mat4.addScale(scale);
    RenderContext::setView(Mat4::IDENTITY);
    RenderContext::setModel(mat4);
}
Vec2 GuiManager::getOrigin()
{
    return viewport.xy();
}
Vec2 GuiManager::getSize()
{
    return Vec2(viewport.z,viewport.w);
}

//draw utility
void GuiManager::setProjection()
{
    //flip y
    Vec4 viewPFY=getFlipViewport(viewport);
    //set view port
    glViewport(  (GLsizei)viewPFY.x,
                 (GLsizei)viewPFY.y,
                 (GLsizei)viewPFY.z, 
                 (GLsizei)viewPFY.w );
    //set view port
	RenderContext::setViewport(viewPFY);
	//set projection matrix
	RenderContext::setProjection(projection);
}
void GuiManager::setProjection(const Vec4& argviewport)
{
    //flip y
    Vec4 viewPFY=getFlipViewport(argviewport);
    //set view port
	RenderContext::setViewport(viewPFY);
	//set projection matrix
	RenderContext::setProjection(projection);
}
void GuiManager::drawItems()
{
    if(isFocused())
	{
        //draw panels
        for(auto obj:panels)
            obj->draw();
        //draw widgets
        for(auto obj:objects)
            obj->draw();
    }
    else
    {
        //draw widgets
        for(auto obj:objects)
            obj->draw();
        //draw panels
        for(auto obj:panels)
            obj->draw();
    }
}
//focus event:
void GuiManager::onMousePress(Vec2 pos,Key::Mouse b)
{
   bool childHasFocus=childFocus(pos,b);
   onFocus(!childHasFocus);
}
bool GuiManager::childFocus(Vec2 pos,Key::Mouse b){
    ///////////////////////LOCK
    if(focusLoked) return !focus;
    ///////////////////////LOCK
    size_t selected_i=0;
    Panel* selected=nullptr;

    if(b==Key::BUTTON_LEFT)
    {
        for(size_t i=panels.size();i!=0;--i)
        {
            Panel* panel=panels[i-1];

            if(!selected && panel->box.isIntersection(pos))
            {
                selected_i=i-1;
                selected=panel;
            }
            else
            {
                panel->onFocus(false);
            }

        }
    }

    if(selected)
    {
        //push to end
        panels.erase(panels.begin()+selected_i);
        panels.push_back(selected);
        //send messages
        selected->onFocus(true);
        return true;
    }
    else
    {
        return false;
    }
}
void GuiManager::onFocus(bool onfocus)
{
    focus=onfocus;
}
void GuiManager::lockFocus()
{
    focusLoked=true;
}
void GuiManager::unlockFocus()
{
    focusLoked=false;
}
//constructors
GuiManager::GuiManager():type(GUI_MANAGER)
                        ,style(nullptr)
                        ,rsgroup(nullptr)
                        ,focus(true)
                        ,focusLoked(false)
{
    //event
    Application::instance()->getInput()->addHandler(this);
}
GuiManager::~GuiManager()
{            
    //remove event
    Application::instance()->getInput()->removeHandler(this);
}
//init
void GuiManager::init(ResourcesGroup* rsgroup,Table::ptr style)
{
    //save
    this->rsgroup=rsgroup;
    this->style=style;
    //projection
    updateProjection(Application::instance()->getScreen()->getSize());
}
//draw
void GuiManager::draw()
{
    //set projection
    setProjection();
    //models
    drawItems();
}
//info
bool GuiManager::isFocused()
{
    return focus;
}
//projection
void GuiManager::updateProjection(const Vec2& argViewport)
{
    updateViewport(argViewport);
    updateProjection();
}
void GuiManager::updateProjection(const Vec4& argViewport)
{
    updateViewport(argViewport);
    updateProjection();
}
void GuiManager::updateProjection()
{
    projection.setOrtho(0.0, viewport.z, 
                        0.0, viewport.w, 
                        1.0f,-1.0f);
}
void GuiManager::updateViewport(const Vec2& argViewport)
{
    //set viewport
    viewport=Vec4(0,0,argViewport.x,argViewport.y);
}
void GuiManager::updateViewport(const Vec4& argViewport)
{
    //set viewport
    viewport=argViewport;
}
