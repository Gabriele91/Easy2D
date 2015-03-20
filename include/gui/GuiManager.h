#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <Gui.h>

namespace Easy2D{
namespace Gui{

    class Popup 
    {
        class Item
        {
        public:
            String text;
            DFUNCTION<void()> selected;
            Item(const String& text,DFUNCTION<void()> callback):text(text)
                                                               ,selected(callback)
            {
            
            }
        };

        std::list<Item> items;

    public:

        Popup& addItem(const String& text,DFUNCTION<void()> callback)
        {
            items.push_back(Item(text,callback));
            return *this;
        }

    };

    class GuiManager : public Input::MouseHandler 
    {
        //friend class
        friend class Panel;
        friend class Button;
        friend class Label;

        enum Type
        {
            GUI_MANAGER,
            PANEL
        };
        Type type;
        //vector widgets
        std::vector<GuiObject*> objects;
        //reg an object
        void addObject(GuiObject* obj);
        //vector panels
        std::vector<Panel*> panels;
        //reg an object
        void addPanel(Panel* obj);

        //save rsgroup
        ResourcesGroup *rsgroup;
        Table::ptr      style;
        Vec4            viewport;
        Mat4            projection;
        bool            focus;
        bool            focusLoked;

        //utility
        Texture::ptr loadTexture(const String& name);
        Mesh::ptr loadMesh(const String& name);
        Font::ptr loadFont(const String& name);
        
        //utility widgets
        Vec4 getFlipViewport(const Vec4& xyzw);
        Vec2 getFlipY(const Vec2& xy,const Vec2& scale);
        void setMat4(const Vec2& position,const Vec2& scale);
        virtual Vec2 getOrigin();
        virtual Vec2 getSize();

        //draw utility
        void setProjection();
        void setProjection(const Vec4& viewport);
        void drawItems();

        //focus event:
        virtual void onMousePress(Vec2 pos,Key::Mouse b);
        virtual bool childFocus(Vec2 pos,Key::Mouse b);
        virtual void onFocus(bool onfocus);
        virtual void lockFocus();
        virtual void unlockFocus();
        virtual AABox2 getPickBox()
        {
           Vec4 view=getFlipViewport(viewport);
           Vec2 pos(view.x,view.y);
           Vec2 size(view.z,view.w);
           AABox2 box;
           box.setMin(pos);
           box.setMax(pos+size);
           return box;
        }

        //popup
        Popup popup;

    public:
        
        GuiManager();
        virtual ~GuiManager();
        
        //init
        void init(ResourcesGroup* rsgroup,Table::ptr style);
       
        //draw
        virtual void draw();
        
        //info
        virtual bool isFocused();

        //projection
        void updateProjection(const Vec2& argViewport);
        void updateProjection(const Vec4& argViewport);
        void updateProjection();
        void updateViewport(const Vec2& argViewport);
        void updateViewport(const Vec4& argViewport);
        void setSubViewport(const Vec4& argViewport);


    };

   
    
};
};

#endif