#ifndef GUI_H
#define GUI_H

#include <Easy2D.h>

namespace Easy2D{
namespace Gui{
    
    class GuiObject;
    class GuiManager;
    class Panel;
    class Button;
    class Label;
    class Popup;
    
    class GuiObject : protected RenderState 
    {
    
    protected:
        
        //friend class
        friend class Panel;
        friend class GuiManager;
        //var declaretion
        AABox2 box;
        GuiManager* guim;
        virtual void updateBox()=0;

    public:

        GuiObject(GuiManager* guim):RenderState()
                                   ,guim(guim)
        {
        }
        virtual void draw()=0;
		virtual const AABox2& getBox() const
		{
			return box;
		}
    };

};
};

#include <GuiManager.h>
#include <Panel.h>
#include <Button.h>
#include <Label.h>

#endif