#ifndef LABEL_H
#define LABEL_H

#include <Gui.h>

namespace Easy2D{
namespace Gui{

	class Label : public GuiObject
    {

	protected:
        //objects
        Font::ptr    font;
        Texture::ptr texture;
        Mesh::ptr mesh;
        //values
		String text;	
        Vec2 scale;
        Vec2 position;
        Vec2 textSize;

		Color  tColor;

        void updateBox();

	public:


		Label(Panel* guim);
		Label(GuiManager* guim);

        void setText(const String& name);

		void setScale(const Vec2& scale);		
        
        void setPosition(const Vec2& pos);

		virtual void draw();
	};

};
};

#endif