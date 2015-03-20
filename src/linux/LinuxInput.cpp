#include <LinuxInput.h>
#include <Input.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
///////////////////////
using namespace Easy2D;
///////////////////////

Key::Keyboard KeyMapUnix[Key::KEYBOARDMAX];

LinuxInput::LinuxInput()
{
    //set base value
    memset(KeyMapUnix,Key::KEY_NULL,sizeof(Key::Keyboard)*Key::KEYBOARDMAX);
    //
    KeyMapUnix[(XK_Up)&0x00FF]=Key::UP;
    KeyMapUnix[(XK_Down)&0x00FF]=Key::DOWN;
    KeyMapUnix[(XK_Left)&0x00FF]=Key::LEFT;
    KeyMapUnix[(XK_Right)&0x00FF]=Key::RIGHT;
    KeyMapUnix[(XK_BackSpace)&0x00FF]=Key::BACKSPACE;
    KeyMapUnix[(XK_Tab)&0x00FF]=Key::TAB;
    KeyMapUnix[(XK_Clear)&0x00FF]=Key::CLEAR;
    KeyMapUnix[(XK_Return)&0x00FF]=Key::RETURN;
    KeyMapUnix[(XK_Pause)&0x00FF]=Key::PAUSE;
    KeyMapUnix[(XK_Escape)&0x00FF]=Key::ESCAPE;
    KeyMapUnix[XK_space]=Key::SPACE;
    KeyMapUnix[XK_quotedbl]=Key::QUOTE;
    KeyMapUnix[XK_apostrophe]=Key::APOSTROPHE;
    KeyMapUnix[XK_comma]=Key::COMMA;
    KeyMapUnix[XK_minus]=Key::MINUS;
    KeyMapUnix[XK_period]=Key::PERIOD;
    KeyMapUnix[XK_slash]=Key::SLASH;

    KeyMapUnix[XK_0]=Key::N0;
    KeyMapUnix[XK_1]=Key::N1;
    KeyMapUnix[XK_2]=Key::N2;
    KeyMapUnix[XK_3]=Key::N3;
    KeyMapUnix[XK_4]=Key::N4;
    KeyMapUnix[XK_5]=Key::N5;
    KeyMapUnix[XK_6]=Key::N6;
    KeyMapUnix[XK_7]=Key::N7;
    KeyMapUnix[XK_8]=Key::N8;
    KeyMapUnix[XK_9]=Key::N9;

    KeyMapUnix[XK_a]=Key::A;
    KeyMapUnix[XK_b]=Key::B;
    KeyMapUnix[XK_c]=Key::C;
    KeyMapUnix[XK_d]=Key::D;
    KeyMapUnix[XK_e]=Key::E;
    KeyMapUnix[XK_f]=Key::F;
    KeyMapUnix[XK_g]=Key::G;
    KeyMapUnix[XK_h]=Key::H;
    KeyMapUnix[XK_i]=Key::I;
    KeyMapUnix[XK_j]=Key::J;
    KeyMapUnix[XK_k]=Key::K;
    KeyMapUnix[XK_l]=Key::L;
    KeyMapUnix[XK_m]=Key::M;
    KeyMapUnix[XK_n]=Key::N;
    KeyMapUnix[XK_o]=Key::O;
    KeyMapUnix[XK_p]=Key::P;
    KeyMapUnix[XK_q]=Key::Q;
    KeyMapUnix[XK_r]=Key::R;
    KeyMapUnix[XK_s]=Key::S;
    KeyMapUnix[XK_t]=Key::T;
    KeyMapUnix[XK_u]=Key::U;
    KeyMapUnix[XK_v]=Key::V;
    KeyMapUnix[XK_w]=Key::W;
    KeyMapUnix[XK_x]=Key::X;
    KeyMapUnix[XK_y]=Key::Y;
    KeyMapUnix[XK_z]=Key::Z;

    KeyMapUnix[XK_semicolon]=Key::SEMICOLON;
    KeyMapUnix[XK_equal]=Key::EQUALS;
    KeyMapUnix[XK_bracketleft]=Key::LEFTBRACKET;
    KeyMapUnix[XK_backslash]=Key::BACKSLASH;
    KeyMapUnix[XK_less]=Key::LESS;
    KeyMapUnix[XK_bracketright]=Key::RIGHTBRACKET;
    KeyMapUnix[XK_grave]=Key::BACKQUOTE;
    KeyMapUnix[(XK_Delete)&0x00FF]=Key::KDELETE;

    KeyMapUnix[(XK_KP_0)&0x00FF]=Key::KP0;
    KeyMapUnix[(XK_KP_1)&0x00FF]=Key::KP1;
    KeyMapUnix[(XK_KP_2)&0x00FF]=Key::KP2;
    KeyMapUnix[(XK_KP_3)&0x00FF]=Key::KP3;
    KeyMapUnix[(XK_KP_4)&0x00FF]=Key::KP4;
    KeyMapUnix[(XK_KP_5)&0x00FF]=Key::KP5;
    KeyMapUnix[(XK_KP_6)&0x00FF]=Key::KP6;
    KeyMapUnix[(XK_KP_7)&0x00FF]=Key::KP7;
    KeyMapUnix[(XK_KP_8)&0x00FF]=Key::KP8;
    KeyMapUnix[(XK_KP_9)&0x00FF]=Key::KP9;
    KeyMapUnix[(XK_KP_Decimal)&0x00FF]=Key::KP_PERIOD;
    KeyMapUnix[(XK_KP_Divide)&0x00FF]=Key::KP_DIVIDE;
    KeyMapUnix[(XK_KP_Multiply)&0x00FF]=Key::KP_MULTIPLY;

    KeyMapUnix[(XK_KP_Subtract)&0x00FF]=Key::KP_MINUS;
    KeyMapUnix[(XK_KP_Add)&0x00FF]=Key::KP_PLUS;

    KeyMapUnix[(XK_Home)&0x00FF]=Key::HOME;
    KeyMapUnix[(XK_End)&0x00FF]=Key::END;
    KeyMapUnix[(XK_Page_Up)&0x00FF]=Key::PAGEUP;
    KeyMapUnix[(XK_Page_Down)&0x00FF]=Key::PAGEDOWN;

    KeyMapUnix[(XK_Num_Lock)&0x00FF]=Key::NUMLOCK;
    KeyMapUnix[(XK_Caps_Lock)&0x00FF]=Key::CAPSLOCK;
    KeyMapUnix[(XK_Scroll_Lock)&0x00FF]=Key::SCROLLOCK;

    KeyMapUnix[(XK_Shift_R)&0x00FF]=Key::RSHIFT;
    KeyMapUnix[(XK_Shift_L)&0x00FF]=Key::LSHIFT;

    KeyMapUnix[(XK_Control_R)&0x00FF]=Key::RCTRL;
    KeyMapUnix[(XK_Control_L)&0x00FF]=Key::LCTRL;

    KeyMapUnix[(XK_Alt_R)&0x00FF]=Key::RALT;
    KeyMapUnix[(XK_Alt_L)&0x00FF]=Key::LALT;

    KeyMapUnix[(XK_Meta_R)&0x00FF]=Key::RMETA;
    KeyMapUnix[(XK_Meta_L)&0x00FF]=Key::LMETA;

    //KeyMapUnix[(XK_Help)&0x00FF]=Key::HELP;//olredy usate
    //KeyMapUnix[(XK_Print)&0x00FF]=Key::PRINT;//olredy usate
    //KeyMapUnix[(XK_Break)&0x00FF]=Key::BREAK;//olredy usate
    //KeyMapUnix[(XK_Menu)&0x00FF]=Key::MENU;//olredy usate

    KeyMapUnix[(XK_F1)&0x00FF]=Key::F1;
    KeyMapUnix[(XK_F2)&0x00FF]=Key::F2;
    KeyMapUnix[(XK_F3)&0x00FF]=Key::F3;
    KeyMapUnix[(XK_F4)&0x00FF]=Key::F4;
    KeyMapUnix[(XK_F5)&0x00FF]=Key::F5;
    KeyMapUnix[(XK_F6)&0x00FF]=Key::F6;
    KeyMapUnix[(XK_F7)&0x00FF]=Key::F7;
    KeyMapUnix[(XK_F8)&0x00FF]=Key::F8;
    KeyMapUnix[(XK_F9)&0x00FF]=Key::F9;
    KeyMapUnix[(XK_F10)&0x00FF]=Key::F10;
    KeyMapUnix[(XK_F11)&0x00FF]=Key::F11;
    KeyMapUnix[(XK_F12)&0x00FF]=Key::F12;
    KeyMapUnix[(XK_F13)&0x00FF]=Key::F13;
    KeyMapUnix[(XK_F14)&0x00FF]=Key::F14;
    KeyMapUnix[(XK_F15)&0x00FF]=Key::F15;
    //input window
    ewindow.__init();
    //input keyboard hit
    ekeyboard.__init();
    //input mouse hit
    emouse.__init();

}

void LinuxInput::__setDisplay(Display *display)
{
    this->display=display;
}

bool LinuxInput::_isKeyRepeat(XEvent &event)
{

    //When a key is repeated, there will be two events: released, followed by another immediate pressed. So check to see if another pressed is present
    if(!XPending(display))
        return false;

    XEvent e;
    XPeekEvent(display, &e);
    if(e.type == KeyPress &&
            e.xkey.keycode == event.xkey.keycode &&
            (e.xkey.time - event.xkey.time) < 2)
    {
        XNextEvent(display, &e);
        return true;
    }

    return false;

}

void LinuxInput::__update(XEvent &event)
{
    //events
    switch(event.type)
    {
    //Maximized
    /* if (wnck_window_is_maximized (win))
       input->window.state=IW_MAXIMIZE;
    */
    case MapNotify://show
        break;
    case UnmapNotify: //hide
        ewindow.maximized=true;
        break;
    //focus event
    case FocusIn:
        //
        ewindow.focus=true;
        //
        break;
    case FocusOut:
        //reset windows input
        if(ewindow.focus)
        {
            //reset window
            ewindow.__init();
            //reset keyboard hit
            ekeyboard.__init();
            //reset mouse hit
            emouse.__init();
        }
        //
        ewindow.focus=false;
        //
        break;
    //close event
    case ClientMessage:
        if (strcmp(XGetAtomName(display, event.xclient.message_type), "WM_PROTOCOLS") == 0)
            ewindow.close=true;
        break;
    //resize
    case Expose:
        ewindow.resize=true;
        ewindow.windowResize.x=event.xexpose.width;
        ewindow.windowResize.y=event.xexpose.height;
        break;
    //keyboard
    case KeyPress:
#define keyDownEvent(key) \
                { \
                    if(ekeyboard.status[KeyMapUnix[(key)]]==0){\
                        ekeyboard.__keyboardDown(KeyMapUnix[(key)]);\
                        __callOnKeyPress(KeyMapUnix[(key)]);\
                        }\
                }
    {
        //stack
        KeySym keysym = XLookupKeysym(&event.xkey, 0);
        if((XK_Help)&0x0FFF==(keysym)&0x0FFF) keyDownEvent(Key::HELP)
            else if((XK_Print)&0x0FFF==(keysym)&0x0FFF) keyDownEvent(Key::PRINT)
                else if((XK_Break)&0x0FFF==(keysym)&0x0FFF) keyDownEvent(Key::BREAK)
                    //else if((IK_MENU)&0x0FFF==(keysym)&0x0FFF) keyDownEvent(Key::MENU)
                    else if((XK_Insert)&0x0FFF==(keysym)&0x0FFF)keyDownEvent(Key::INSERT)
                        else if((keysym)&0x00FF) keyDownEvent((keysym)&0x00FF)
                        }
    event.xkey.state &= ~ShiftMask;
    event.xkey.state &= ~LockMask;
#undef keyDownEvent
    break;
    case KeyRelease:
#define keyReleaseEvent(key) \
                { \
                ekeyboard.__keyboardUp(KeyMapUnix[(key)]);\
                __callOnKeyRelease(KeyMapUnix[(key)]);\
                }
        if(!_isKeyRepeat(event))
        {
            event.xkey.state &= ~ShiftMask;
            event.xkey.state &= ~LockMask;
            {
                //stack
                KeySym keysym = XLookupKeysym(&event.xkey, 0);
                if((XK_Help)&0x0FFF==(keysym)&0x0FFF) keyReleaseEvent(Key::HELP)
                    else if((XK_Print)&0x0FFF==(keysym)&0x0FFF) keyReleaseEvent(Key::PRINT)
                        else if((XK_Break)&0x0FFF==(keysym)&0x0FFF) keyReleaseEvent(Key::BREAK)
                            //else if((IK_MENU)&0x0FFF==(keysym)&0x0FFF) keyReleaseEvent(Key::MENU)
                            else if((XK_Insert)&0x0FFF==(keysym)&0x0FFF)keyReleaseEvent(Key::INSERT)
                                else if((keysym)&0x00FF) keyReleaseEvent((keysym)&0x00FF)
                                }
        }
#undef keyReleaseEvent
        break;
    case ButtonPress:
    case ButtonRelease:
        //location of mause
        emouse.pos.x= event.xmotion.x;
        emouse.pos.y= event.xmotion.y;
#define isUpOrDownButtom(key)\
                        if(event.type==ButtonRelease){\
                            emouse.__mouseUp(key);\
                            __callOnMouseRelease(emouse.pos,key);\
                        }\
                        else{\
                            emouse.__mouseDown(key);\
                            __callOnMousePress(emouse.pos,key);\
                        }
        //buttom events
        switch(event.xbutton.button)
        {
        case Button1: // left
            isUpOrDownButtom(Key::BUTTON_LEFT) break;
        case Button2: // middle
            isUpOrDownButtom(Key::BUTTON_MIDDLE) break;
        case Button3: // right
            isUpOrDownButtom(Key::BUTTON_RIGHT) break;
        // 120 like windows value
        case Button4:
            emouse.scroll=120;
            __callOnMouseScroll(emouse.scroll);
            break;
        case Button5:
            emouse.scroll=-120;
            __callOnMouseScroll(emouse.scroll);
            break;
        default:
            break;
        }
#undef isUpOrDownButtom
        break;
    case MotionNotify:
        emouse.pos.x= event.xmotion.x;
        emouse.pos.y= event.xmotion.y;
        __callOnMouseMove(emouse.pos);
        break;
    }

}
void LinuxInput::update()
{
    //update input
    //reset input window
    ewindow.resize=false;
    //update hit
    ekeyboard.__clearHit();
    emouse.__clearHit();
    //update x11 events
    XEvent event;
    while (XPending(display) > 0)
    {
        XNextEvent(display, &event);
        __update(event);
    }
    ///////////////////////////LOOP EVENT
    //update down keys
    ekeyboard.__update(this);
    emouse.__update(this);

}

//calls
void LinuxInput::__callOnKeyPress(Key::Keyboard key)
{
    for(size_t i=0; i!=vkeyboardh.size(); ++i)
        vkeyboardh[i]->onKeyPress(key);
}
void LinuxInput::__callOnKeyRelease(Key::Keyboard key)
{
    for(size_t i=0; i!=vkeyboardh.size(); ++i)
        vkeyboardh[i]->onKeyRelease(key);
}
void LinuxInput::__callOnKeyDown(Key::Keyboard key)
{
    for(size_t i=0; i!=vkeyboardh.size(); ++i)
        vkeyboardh[i]->onKeyDown(key);
}
//mouse
void LinuxInput::__callOnMouseMove(Vec2 mousePosition)
{
    for(size_t i=0; i!=vmouseh.size(); ++i)
        vmouseh[i]->onMouseMove(mousePosition);
}
void LinuxInput::__callOnMousePress(Vec2 mousePosition, Key::Mouse button)
{
    for(size_t i=0; i!=vmouseh.size(); ++i)
        vmouseh[i]->onMousePress(mousePosition,button);
}
void LinuxInput::__callOnMouseDown(Vec2 mousePosition, Key::Mouse button)
{
    for(size_t i=0; i!=vmouseh.size(); ++i)
        vmouseh[i]->onMouseDown(mousePosition,button);
}
void LinuxInput::__callOnMouseRelease(Vec2 mousePosition, Key::Mouse button)
{
    for(size_t i=0; i!=vmouseh.size(); ++i)
        vmouseh[i]->onMouseRelease(mousePosition,button);
}
void LinuxInput::__callOnMouseScroll(short scrollDelta)
{
    for(size_t i=0; i!=vmouseh.size(); ++i)
        vmouseh[i]->onMouseScroll(scrollDelta);
}

