#include "stdafx.h"
#include <WindowsInput.h>
#include <WindowsScreen.h>
#include <Debug.h>
#include <Application.h>
///////////////////////
using namespace Easy2D;

#ifndef VK_0

#define VK_0	'0'
#define VK_1	'1'
#define VK_2	'2'
#define VK_3	'3'
#define VK_4	'4'
#define VK_5	'5'
#define VK_6	'6'
#define VK_7	'7'
#define VK_8	'8'
#define VK_9	'9'
#define VK_A	'A'
#define VK_B	'B'
#define VK_C	'C'
#define VK_D	'D'
#define VK_E	'E'
#define VK_F	'F'
#define VK_G	'G'
#define VK_H	'H'
#define VK_I	'I'
#define VK_J	'J'
#define VK_K	'K'
#define VK_L	'L'
#define VK_M	'M'
#define VK_N	'N'
#define VK_O	'O'
#define VK_P	'P'
#define VK_Q	'Q'
#define VK_R	'R'
#define VK_S	'S'
#define VK_T	'T'
#define VK_U	'U'
#define VK_V	'V'
#define VK_W	'W'
#define VK_X	'X'
#define VK_Y	'Y'
#define VK_Z	'Z'

#endif

#define VK_SEMICOLON	0xBA
#define VK_EQUALS		0xBB
#define VK_COMMA		0xBC
#define VK_MINUS		0xBD
#define VK_PERIOD		0xBE
#define VK_SLASH		0xBF
#define VK_GRAVE		0xC0
#define VK_LBRACKET		0xDB
#define VK_BACKSLASH	0xDC
#define VK_RBRACKET		0xDD
#define VK_APOSTROPHE	0xDE
#define VK_BACKTICK		0xDF
#define VK_OEM_102		0xE2

Key::Keyboard keyMapWIN32[Key::KEYBOARDMAX];

WindowsInput::WindowsInput()
{
    //set base value
    memset(keyMapWIN32,Key::KEY_NULL,sizeof(Key::Keyboard)*Key::KEYBOARDMAX);
    //
    keyMapWIN32[ VK_BACK] = Key::BACKSPACE;
    keyMapWIN32[ VK_TAB]= Key::TAB;
    keyMapWIN32[ VK_CLEAR]= Key::CLEAR;
    keyMapWIN32[ VK_RETURN]= Key::RETURN;
    keyMapWIN32[ VK_PAUSE]= Key::PAUSE;
    keyMapWIN32[ VK_ESCAPE]= Key::ESCAPE;
    keyMapWIN32[ VK_SPACE]= Key::SPACE;
    keyMapWIN32[ VK_APOSTROPHE]= Key::QUOTE;
    keyMapWIN32[ VK_COMMA]= Key::COMMA;
    keyMapWIN32[ VK_MINUS]= Key::MINUS;
    keyMapWIN32[ VK_PERIOD]= Key::PERIOD;
    keyMapWIN32[ VK_SLASH]= Key::SLASH;
    keyMapWIN32[ VK_0]= Key::N0;
    keyMapWIN32[ VK_1]= Key::N1;
    keyMapWIN32[ VK_2]= Key::N2;
    keyMapWIN32[ VK_3]= Key::N3;
    keyMapWIN32[ VK_4]= Key::N4;
    keyMapWIN32[ VK_5]= Key::N5;
    keyMapWIN32[ VK_6]= Key::N6;
    keyMapWIN32[ VK_7]= Key::N7;
    keyMapWIN32[ VK_8]= Key::N8;
    keyMapWIN32[ VK_9]= Key::N9;
    keyMapWIN32[ VK_SEMICOLON]= Key::SEMICOLON;
    keyMapWIN32[ VK_EQUALS]= Key::EQUALS;
    keyMapWIN32[ VK_LBRACKET]= Key::LEFTBRACKET;
    keyMapWIN32[ VK_BACKSLASH]= Key::BACKSLASH;
    keyMapWIN32[ VK_OEM_102]= Key::LESS;
    keyMapWIN32[ VK_RBRACKET]= Key::RIGHTBRACKET;
    keyMapWIN32[ VK_GRAVE]= Key::BACKQUOTE;
    keyMapWIN32[ VK_BACKTICK]= Key::BACKQUOTE;
    keyMapWIN32[ VK_A]= Key::A;
    keyMapWIN32[ VK_B]= Key::B;
    keyMapWIN32[ VK_C]= Key::C;
    keyMapWIN32[ VK_D]= Key::D;
    keyMapWIN32[ VK_E]= Key::E;
    keyMapWIN32[ VK_F]= Key::F;
    keyMapWIN32[ VK_G]= Key::G;
    keyMapWIN32[ VK_H]= Key::H;
    keyMapWIN32[ VK_I]= Key::I;
    keyMapWIN32[ VK_J]= Key::J;
    keyMapWIN32[ VK_K]= Key::K;
    keyMapWIN32[ VK_L]= Key::L;
    keyMapWIN32[ VK_M]= Key::M;
    keyMapWIN32[ VK_N]= Key::N;
    keyMapWIN32[ VK_O]= Key::O;
    keyMapWIN32[ VK_P]= Key::P;
    keyMapWIN32[ VK_Q]= Key::Q;
    keyMapWIN32[ VK_R]= Key::R;
    keyMapWIN32[ VK_S]= Key::S;
    keyMapWIN32[ VK_T]= Key::T;
    keyMapWIN32[ VK_U]= Key::U;
    keyMapWIN32[ VK_V]= Key::V;
    keyMapWIN32[ VK_W]= Key::W;
    keyMapWIN32[ VK_X]= Key::X;
    keyMapWIN32[ VK_Y]= Key::Y;
    keyMapWIN32[ VK_Z]= Key::Z;
    keyMapWIN32[ VK_DELETE]= Key::KDELETE;

    //KEY KEYPAD
    keyMapWIN32[ VK_NUMPAD0]= Key::KP0;
    keyMapWIN32[ VK_NUMPAD1]= Key::KP1;
    keyMapWIN32[ VK_NUMPAD2]= Key::KP2;
    keyMapWIN32[ VK_NUMPAD3]= Key::KP3;
    keyMapWIN32[ VK_NUMPAD4]= Key::KP4;
    keyMapWIN32[ VK_NUMPAD5]= Key::KP5;
    keyMapWIN32[ VK_NUMPAD6]= Key::KP6;
    keyMapWIN32[ VK_NUMPAD7]= Key::KP7;
    keyMapWIN32[ VK_NUMPAD8]= Key::KP8;
    keyMapWIN32[ VK_NUMPAD9]= Key::KP9;
    keyMapWIN32[ VK_DECIMAL]= Key::KP_PERIOD;
    keyMapWIN32[ VK_DIVIDE]= Key::KP_DIVIDE;
    keyMapWIN32[ VK_MULTIPLY]= Key::KP_MULTIPLY;
    keyMapWIN32[ VK_SUBTRACT]= Key::KP_MINUS;
    keyMapWIN32[ VK_ADD]= Key::KP_PLUS;

    //KEY ARROWS AND SYSTEM COMAND
    keyMapWIN32[ VK_UP]= Key::UP;
    keyMapWIN32[ VK_DOWN]= Key::DOWN;
    keyMapWIN32[ VK_RIGHT]= Key::RIGHT;
    keyMapWIN32[ VK_LEFT]= Key::LEFT;
    keyMapWIN32[ VK_INSERT]= Key::INSERT;
    keyMapWIN32[ VK_HOME]= Key::HOME;
    keyMapWIN32[ VK_END]= Key::END;
    keyMapWIN32[ VK_PRIOR]= Key::PAGEUP;
    keyMapWIN32[ VK_NEXT]= Key::PAGEDOWN;
    //F1-15 KEY
    keyMapWIN32[ VK_F1]= Key::F1;
    keyMapWIN32[ VK_F2]= Key::F2;
    keyMapWIN32[ VK_F3]= Key::F3;
    keyMapWIN32[ VK_F4]= Key::F4;
    keyMapWIN32[ VK_F5]= Key::F5;
    keyMapWIN32[ VK_F6]= Key::F6;
    keyMapWIN32[ VK_F7]= Key::F7;
    keyMapWIN32[ VK_F8]= Key::F8;
    keyMapWIN32[ VK_F9]= Key::F9;
    keyMapWIN32[ VK_F10]= Key::F10;
    keyMapWIN32[ VK_F11]= Key::F11;
    keyMapWIN32[ VK_F12]= Key::F12;
    keyMapWIN32[ VK_F13]= Key::F13;
    keyMapWIN32[ VK_F14]= Key::F14;
    keyMapWIN32[ VK_F15]= Key::F15;
    keyMapWIN32[ VK_NUMLOCK]= Key::NUMLOCK;
    keyMapWIN32[ VK_CAPITAL]= Key::CAPSLOCK;
    keyMapWIN32[ VK_SCROLL]= Key::SCROLLOCK;
    keyMapWIN32[ VK_RSHIFT]= Key::RSHIFT;
    keyMapWIN32[ VK_LSHIFT]= Key::LSHIFT;
    keyMapWIN32[ VK_RCONTROL]= Key::RCTRL;
    keyMapWIN32[ VK_LCONTROL]= Key::LCTRL;
    keyMapWIN32[ VK_RMENU]= Key::RALT;
    keyMapWIN32[ VK_LMENU]= Key::LALT;
    keyMapWIN32[ VK_RWIN]= Key::RMETA;
    keyMapWIN32[ VK_LWIN]= Key::LMETA;
    //input window
    ewindow.__init();
    //input keyboard hit
    ekeyboard.__init();
    //input mouse hit
    emouse.__init();

}

void WindowsInput::update()
{

    //update input
    //reset input window
    bool oldFocus=ewindow.focus;
    ewindow.resize=false;
    //update hit
    ekeyboard.__clearHit();
    emouse.__clearHit();
    //update windows message
    MSG		msg;
    //take message and send it...
    while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    ///////////////////////////LOOP EVENT
    //get hendler windows
    HWND hWind=((WindowsScreen*)Application::instance()->getScreen())->hWind;
    //update maximize
    //get info
    bool maximized=IsZoomed(hWind)!=FALSE;
    //if is changed
    if(ewindow.maximized!=maximized)
    {
        //change value
        ewindow.maximized=maximized;
        //if is maximized
        if(maximized)
            __callOnChangeState(Window::MAXIMIZED);
    }
    //update minimixed
    //get info
    bool minimized=IsIconic(hWind)!=FALSE;
    //if is changed
    if(ewindow.minimized!=minimized)
    {
        //change value
        ewindow.minimized=minimized;
        //if is minimized
        if(minimized)
            __callOnChangeState(Window::MINIMIZED);
    }
    //reset input:
    if(!ewindow.focus && oldFocus) //lost focus
    {
        //reset window
        ewindow.__init();
        //reset keyboard hit
        ekeyboard.__init();
        //reset mouse hit
        emouse.__init();
    }
    ///////////////////////////LOOP EVENT
    //update event
    ekeyboard.__update(this);
    emouse.__update(this);

}


//calls
void WindowsInput::__callOnKeyPress(Key::Keyboard key)
{
    for(size_t i=0; i!=vkeyboardh.size(); ++i)
        vkeyboardh[i]->onKeyPress(key);
}
void WindowsInput::__callOnKeyRelease(Key::Keyboard key)
{
    for(size_t i=0; i!=vkeyboardh.size(); ++i)
        vkeyboardh[i]->onKeyRelease(key);
}
void WindowsInput::__callOnKeyDown(Key::Keyboard key)
{
    for(size_t i=0; i!=vkeyboardh.size(); ++i)
        vkeyboardh[i]->onKeyDown(key);
}
//mouse
void WindowsInput::__callOnMouseMove(Vec2 mousePosition)
{
    for(size_t i=0; i!=vmouseh.size(); ++i)
        vmouseh[i]->onMouseMove(mousePosition);
}
void WindowsInput::__callOnMousePress(Vec2 mousePosition, Key::Mouse button)
{
    for(size_t i=0; i!=vmouseh.size(); ++i)
        vmouseh[i]->onMousePress(mousePosition,button);
}
void WindowsInput::__callOnMouseDown(Vec2 mousePosition, Key::Mouse button)
{
    for(size_t i=0; i!=vmouseh.size(); ++i)
        vmouseh[i]->onMouseDown(mousePosition,button);
}
void WindowsInput::__callOnMouseRelease(Vec2 mousePosition, Key::Mouse button)
{
    for(size_t i=0; i!=vmouseh.size(); ++i)
        vmouseh[i]->onMouseRelease(mousePosition,button);
}
void WindowsInput::__callOnMouseScroll(short scrollDelta)
{
    for(size_t i=0; i!=vmouseh.size(); ++i)
        vmouseh[i]->onMouseScroll(scrollDelta);
}
//window
void WindowsInput::__callOnFocus(bool focus)
{
    for(size_t i=0; i!=vwindowh.size(); ++i)
        vwindowh[i]->onFocus(focus);
}
void WindowsInput::__callOnChangeState(Window::State windowState)
{
    for(size_t i=0; i!=vwindowh.size(); ++i)
        vwindowh[i]->onChangeState(windowState);
}
void WindowsInput::__callOnClose()
{
    for(size_t i=0; i!=vwindowh.size(); ++i)
        vwindowh[i]->onClose();
}
void WindowsInput::__callOnResize(Vec2 size)
{
    for(size_t i=0; i!=vwindowh.size(); ++i)
        vwindowh[i]->onResize(size);
}
//window events:
LRESULT CALLBACK WindowsInput::WndProc(   HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{

    if(Application::instance()!=NULL)
    {

        WindowsInput *winput=(WindowsInput*)Application::instance()->getInput();
        DEBUG_ASSERT(winput);

        switch (message)
        {

        case WM_SYSCOMMAND:

            switch (wparam)
            {
            case SC_SCREENSAVE:
                winput->ekeyboard.status[Key::PRINT]=true;
                break;

            case SC_MONITORPOWER:
                winput->ekeyboard.status[Key::POWER]=true;
                break;
            case SC_CLOSE:
                winput->ewindow.close=true;
                PostQuitMessage(0);
                break;
            default :
                // Pass All Unhandled Messages To DefWindowProc
                return DefWindowProc(hwnd,message,wparam,lparam);
                break;
            }

            break;

        // WINDOWS EVENT //
        //FOCUS
        case WM_ACTIVATE:
            winput->ewindow.focus=LOWORD(wparam) == WA_ACTIVE;
            winput->__callOnFocus(winput->ewindow.focus);
            break;
        case WM_SIZE:
            winput->ewindow.maximized=false;
            winput->ewindow.minimized=false;
            winput->ewindow.resize=true;
            winput->ewindow.windowResize.x=LOWORD(lparam);
            winput->ewindow.windowResize.y=HIWORD(lparam);
            winput->__callOnResize(winput->ewindow.windowResize);
            break;
        case WM_NCDESTROY:
        case WM_DESTROY:
        case WM_QUIT:
        case WM_CLOSE:
            winput->ewindow.close=true;
            winput->__callOnClose();
            PostQuitMessage(0);
            break;

        // KEYBOAR EVENT //
        case WM_KEYDOWN:
            if((HIWORD(lparam) & KF_REPEAT) == 0)
            {
                if(wparam==VK_MENU)  //alt
                {
                    winput->ekeyboard.__keyboardDown(lparam&(1<<24) ? Key::RALT : Key::LALT);
                    winput->__callOnKeyPress(lparam&(1<<24) ? Key::RALT : Key::LALT);
                }
                else if(wparam==VK_CONTROL)  //ctrl
                {
                    winput->ekeyboard.__keyboardDown(lparam&(1<<24) ? Key::RCTRL : Key::LCTRL);
                    winput->__callOnKeyPress(lparam&(1<<24) ? Key::RCTRL : Key::LCTRL);
                }
                else if(wparam==VK_SHIFT)
                {
                    winput->ekeyboard.__keyboardDown(GetKeyState(VK_RSHIFT) & 0x8000 ? Key::RSHIFT: Key::LSHIFT);
                    winput->__callOnKeyPress(GetKeyState(VK_RSHIFT) & 0x8000 ? Key::RSHIFT: Key::LSHIFT);
                }
                else
                {
                    winput->ekeyboard.__keyboardDown(keyMapWIN32[wparam]);
                    winput->__callOnKeyPress(keyMapWIN32[wparam]);
                }
            }
            break;

        case WM_KEYUP:
            if(wparam==VK_MENU)  //alt
            {
                winput->ekeyboard.__keyboardUp(lparam&(1<<24) ? Key::RALT : Key::LALT);
                winput->__callOnKeyRelease(lparam&(1<<24) ? Key::RALT : Key::LALT);
            }
            else if(wparam==VK_CONTROL)  //ctrl
            {
                winput->ekeyboard.__keyboardUp(lparam&(1<<24) ? Key::RCTRL : Key::LCTRL);
                winput->__callOnKeyRelease(lparam&(1<<24) ? Key::RCTRL : Key::LCTRL);
            }
            else if(wparam==VK_SHIFT)
            {
                winput->ekeyboard.__keyboardUp(GetKeyState(VK_RSHIFT) & 0x8000 ? Key::RSHIFT: Key::LSHIFT);
                winput->__callOnKeyRelease(GetKeyState(VK_RSHIFT) & 0x8000 ? Key::RSHIFT: Key::LSHIFT);
            }
            else
            {
                winput->ekeyboard.__keyboardUp(keyMapWIN32[wparam]);
                winput->__callOnKeyRelease(keyMapWIN32[wparam]);
            }
            break;

        // MOUSE EVENT //
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_MOUSEMOVE:


            winput->emouse.pos.x= LOWORD(lparam);
            winput->emouse.pos.y= HIWORD(lparam);

            if(message==WM_LBUTTONDOWN)
            {
                winput->emouse.__mouseDown(Key::BUTTON_LEFT);
                winput->__callOnMousePress(winput->emouse.pos,Key::BUTTON_LEFT);
            }
            else if(message==WM_MBUTTONDOWN)
            {
                winput->emouse.__mouseDown(Key::BUTTON_MIDDLE);
                winput->__callOnMousePress(winput->emouse.pos,Key::BUTTON_MIDDLE);
            }
            else if(message==WM_RBUTTONDOWN)
            {
                winput->emouse.__mouseDown(Key::BUTTON_RIGHT);
                winput->__callOnMousePress(winput->emouse.pos,Key::BUTTON_RIGHT);
            }

            else if(message==WM_LBUTTONUP)
            {
                winput->emouse.__mouseUp(Key::BUTTON_LEFT);
                winput->__callOnMouseRelease(winput->emouse.pos,Key::BUTTON_LEFT);
            }
            else if(message==WM_MBUTTONUP)
            {
                winput->emouse.__mouseUp(Key::BUTTON_MIDDLE);
                winput->__callOnMouseRelease(winput->emouse.pos,Key::BUTTON_MIDDLE);
            }
            else if(message==WM_RBUTTONUP)
            {
                winput->emouse.__mouseUp(Key::BUTTON_RIGHT);
                winput->__callOnMouseRelease(winput->emouse.pos,Key::BUTTON_RIGHT);
            }
            else
            {
                winput->__callOnMouseMove(winput->emouse.pos);
            }

            break;
        case WM_MOUSEWHEEL:
            winput->emouse.scroll=(short)GET_WHEEL_DELTA_WPARAM(wparam);
            winput->__callOnMouseScroll(winput->emouse.scroll);
            break;

        default:
            // Pass All Unhandled Messages To DefWindowProc
            return DefWindowProc(hwnd,message,wparam,lparam);
            break;
        }
        //not pass processed message
        return 0;


    }
    // Pass All Unhandled Messages To DefWindowProc
    return DefWindowProc(hwnd,message,wparam,lparam);
}