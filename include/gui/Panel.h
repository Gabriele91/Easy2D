#ifndef EPANEL_H
#define EPANEL_H

#include <Gui.h>

namespace Easy2D{
namespace Gui{

    class Panel : public GuiObject, 
                  protected GuiManager 
    {
        
        //friend class
        friend class GuiManager;
        friend class Button;
        friend class Label;
        friend class Popup;

        enum State
        {
            STAY,
            MOVE
        };

        State state;
        Vec2 pickpos;
        Vec2 scale;
        Vec2 position;
        Texture::ptr texture[2];
        Mesh::ptr mesh;
        
        void updateBox();
        virtual AABox2 getPickBox();
        //calc viewport
        Vec4 calcViewport();
        //recursive lock/unlock
        virtual void lockFocus();
        virtual void unlockFocus();
        //events
        virtual void onMousePress(Vec2 pos,Key::Mouse b);
        virtual void onMouseDown(Vec2 pos,Key::Mouse bt);
        virtual void onMouseRelease(Vec2 pos,Key::Mouse b);
        
    public:

		Panel(Panel* panel);
        Panel(GuiManager* guim);
        virtual void draw();
        void setScale(const Vec2& scale);
        void setPosition(const Vec2& pos);
        operator GuiManager*();
    };
    
};
};
#endif