#ifndef BUTTON_H
#define BUTTON_H

#include <Gui.h>

namespace Easy2D{
namespace Gui{

    class Button : public GuiObject, 
                   public Input::MouseHandler 
    {

    protected:

        enum State
        {
            UP,
            OVER,
            DOWN
        };

        State state;
        //objects
        Texture::ptr texture[3];
        Mesh::ptr    mesh[3];
        Font::ptr    font;
        //vars
        Vec2 scale;
        Vec2 position;
        Vec2 textSize;
        Color  tColor;
        String text;

        //box
        void updateBox();

        //event
        Function<void()> eventClick;
        void onMouseMove(Vec2 pos);
        void onMouseDown(Vec2 pos,Key::Mouse b);    
        void onMouseRelease(Vec2 pos,Key::Mouse b);

    public:

		Button(Panel* guim);
		Button(GuiManager* guim);
        virtual ~Button();
		//
        void setText(const String& name);
		//
        void setScale(const Vec2& scale);
        //
        void setPosition(const Vec2& pos);
		//
        void onClick(Function<void()> event);
		//
        virtual void draw();

    };

};
};

#endif
