#ifndef INPUT_H
#define INPUT_H

#include <Config.h>
#include <Debug.h>
#include <Math3D.h>
#include <Types.h>

namespace Easy2D
{

namespace Key
{
    extern E2D_DLL const char *stringKeyboard[];
    extern E2D_DLL const char *stringMouse[];
    extern E2D_DLL const char *stringFinger[];
    enum Keyboard
    {
        KEY_NULL=0,
        BACKSPACE=1,
        TAB,
        CLEAR,
        RETURN,
        PAUSE,
        ESCAPE,
        SPACE,
        QUOTE,
        APOSTROPHE,
        COMMA,
        MINUS,
        PERIOD,
        SLASH,
        N0,
        N1,
        N2,
        N3,
        N4,
        N5,
        N6,
        N7,
        N8,
        N9,
        SEMICOLON,
        EQUALS,
        LEFTBRACKET,
        BACKSLASH,
        LESS,
        RIGHTBRACKET,
        BACKQUOTE,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        KDELETE,
        KP0,
        KP1,
        KP2,
        KP3,
        KP4,
        KP5,
        KP6,
        KP7,
        KP8,
        KP9,
        KP_PERIOD,
        KP_DIVIDE,
        KP_MULTIPLY,
        KP_MINUS,
        KP_PLUS,
        UP,
        DOWN,
        RIGHT,
        LEFT,
        INSERT,
        HOME,
        END,
        PAGEUP,
        PAGEDOWN,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        NUMLOCK,
        CAPSLOCK,
        SCROLLOCK,
        RSHIFT,
        LSHIFT,
        RCTRL,
        LCTRL,
        RALT,
        LALT,
        RMETA,
        LMETA,
        HELP,
        PRINT,
        BREAK,
        MENU,
        POWER,
        KEYBOARDMAX = 323
    };
    enum Mouse
    {
        MOUSE_NULL=0,
        BUTTON_LEFT=1,
        BUTTON_MIDDLE,
        BUTTON_RIGHT,
        BUTTON4,
        BUTTON5,
        BUTTON6,
        MOUSEMAX
    };
    enum Finger
    {
        FINGER_NULL=0,
        FINGER1=1,
        FINGER2,
        FINGER3,
        FINGER4,
        FINGER5,
        FINGER6,
        FINGER7,
        FINGER8,
        FINGER9,
        FINGER10,
        FINGERMAX
    };
#ifdef COMPILER_VISUAL_STUDIO
    #include "InputList.h"
#else
    template< typename T,typename ...A >
    bool list(T input,T key,A ... a)
    {
        return input==key || list(input,a...);
    }
    template<typename T>
    bool list(T input,T key)
    {
        return input==key;
    }
#endif

};
namespace Window
{
    enum State
    {
        MINIMIZED,
        MAXIMIZED
    };
};

/**
* @class Input
*
* @brief abstract class input management
*
* @author Gabriele Di Bari
*
*/
class E2D_DLL Input
{

public:

    class E2D_DLL KeyboardHandler
    {
    public:
        //keyboard
        virtual void on_key_press(Key::Keyboard key) {}
        virtual void on_key_release(Key::Keyboard key) {}
        virtual void on_key_down(Key::Keyboard key) {}
        virtual void on_text_input(const String& text) {}
    };
    class E2D_DLL FingersHandler
    {
    public:
        //finger touch
        virtual void on_finger_move(Vec3 touchPosition,Key::Finger FingerID )  {}
        virtual void on_finger_down(Vec3 touchPosition,Key::Finger FingerID ) {}
        virtual void on_finger_press(Vec3 touchPosition,Key::Finger FingerID ) {}
        virtual void on_finger_release(Vec3 touchPosition,Key::Finger FingerID )   {}
    };

    struct E2D_DLL AccelerometerValues
    {
        Vec3 m_accel;
        float m_azimuth,m_pitch,m_roll;
        AccelerometerValues():m_azimuth(0),m_pitch(0),m_roll(0) {}
        AccelerometerValues(const Vec3& accel,float azimuth,float pitch,float roll)
            :m_accel(accel),m_azimuth(azimuth),m_pitch(pitch),m_roll(roll) {}
        AccelerometerValues(float x,float y, float z,float azimuth,float pitch,float roll)
            :m_accel(x,y,z),m_azimuth(azimuth),m_pitch(pitch),m_roll(roll) {}
    };

    class E2D_DLL AccelerometerHandler
    {
    public:
        //accelerometer
        virtual void on_acceleration(const AccelerometerValues& values)	{}
    };
    class E2D_DLL MouseHandler
    {
    public:
        //mouse
        virtual void on_mouse_move(Vec2 mousePosition) {}
        virtual void on_mouse_press(Vec2 mousePosition, Key::Mouse button) {}
        virtual void on_mouse_down(Vec2 mousePosition, Key::Mouse button) {}
        virtual void on_mouse_release(Vec2 mousePosition, Key::Mouse button) {}
        virtual void on_mouse_scroll(short scrollDelta) {}
    };
    class E2D_DLL WindowHandler
    {
    public:
        //window
        virtual void on_focus(bool focus) {}
        virtual void on_change_state(Window::State windowState) {}
        virtual void on_close() {}
        virtual void on_resize(Vec2 size) {}

    };

    /**
    * Destructor
    */
    virtual ~Input() {};
    /**
    * get mouse position
    * @return mouse position
    */
    virtual Vec2 get_mouse() const=0;
    /**
    * get finger position
    * @param finger number
    * @return finger position
    */
    virtual Vec2 get_finger(Key::Finger id) const=0;
    /**
    * get accelerometer values
    * @return (x,y,z, azimuth, pitch, roll) accelerometer values
    */
    virtual const AccelerometerValues& get_accelerometer() const=0;
    /**
    * update window event,
    * call this method in a loop
    */
    virtual void update() = 0;
    /**
    * Return the last input string got from keyboard
    * @return input string
    */
    virtual const String& get_input_string()
    {
        return String::NONE;
    }
    /**
    * Return true if keyboard button is down
    * @param key's id
    * @return status
    */
    virtual bool get_key_down(Key::Keyboard id) const=0;
    /**
    *  Return true if go up keyboard button
    * @param key's id
    * @return status
    */
    virtual bool get_key_up(Key::Keyboard id) const=0;
    /**
    *  Return true if hit keyboard button
    * @param key's id
    * @return status
    */
    virtual bool get_key_hit(Key::Keyboard id) const=0;
    /**
    * Return true if mouse button is down
    * @param mouse button
    * @return status
    */
    virtual bool get_mouse_down(Key::Mouse id) const=0;
    /**
    * Return true if mouse button is up
    * @param mouse button
    * @return status
    */
    virtual bool get_mouse_up(Key::Mouse id) const=0;
    /**
    * Return true if mouse button is hit
    * @param mouse button
    * @return status
    */
    virtual bool get_mouse_hit(Key::Mouse id) const=0;
    /**
    * Return scroll wheel delta
    * @return scroll delta
    */
    virtual float get_scroll() const=0;
    /**
    * Return true if finger is down
    * @param finger number
    * @return status
    */
    virtual bool get_finger_down(Key::Finger id) const=0;
    /**
    * Return true if finger is up
    * @param finger number
    * @return status
    */
    virtual bool get_finger_up(Key::Finger id) const=0;
    /**
    * Return true if finger hit the screen
    * @param  finger number
    * @return status
    */
    virtual bool get_finger_hit(Key::Finger id) const=0;
    /**
    * Return true if window has focus
    * @return focus status
    */
    virtual bool get_focus() const=0;
    /**
    * Return true if window minimized
    * @return window status
    */
    virtual bool get_minimized() const=0;
    /**
    * Return true if window maximized
    * @return window status
    */
    virtual bool get_maximized() const=0;
    /**
    * Return true if window is closed
    * @return window status
    */
    virtual bool get_close() const=0;
    /**
    * Return true if window is resized
    * @return window status
    */
    virtual bool get_resize() const=0;
    /**
    * window size (when it is resized)
    * @return window size
    */
    virtual Vec2 get_resize_values() const=0;
    /**
    * Adds an input window handler to the callback list
    */
    virtual void add_handler(WindowHandler *handler)
    {
        DEBUG_ASSERT(handler);
        m_vwindowh.push_back(handler);
    }
    /**
    * Adds an input keyboard handler to the callback list
    */
    virtual void add_handler(KeyboardHandler *handler)
    {
        DEBUG_ASSERT(handler);
        m_vkeyboardh.push_back(handler);
    }
    /**
    * Adds an input fingers handler to the callback list
    */
    virtual void add_handler(FingersHandler *handler)
    {
        DEBUG_ASSERT(handler);
        m_vfingersh.push_back(handler);
    }
    /**
    * Adds an input accelerometer handler to the callback list
    */
    virtual void add_handler(AccelerometerHandler *handler)
    {
        DEBUG_ASSERT(handler);
        m_vaccelerometerh.push_back(handler);
    }
    /**
    * Adds an input mouse handler to the callback list
    */
    virtual void add_handler(MouseHandler *handler)
    {
        DEBUG_ASSERT(handler);
        m_vmouseh.push_back(handler);
    }
    /**
    * Removes an input window handler from the callback list
    */
    virtual void remove_handler(WindowHandler *handler)
    {
        DEBUG_ASSERT(handler);
        for(auto it=m_vwindowh.begin(); it!=m_vwindowh.end(); ++it)
        {
            if(*it==handler)
            {
                m_vwindowh.erase(it);
                break;
            }
        }
    }
    /**
    * Removes an input keyboard handler from the callback list
    */
    virtual void remove_handler(KeyboardHandler *handler)
    {
        DEBUG_ASSERT(handler);
        for(auto it=m_vkeyboardh.begin(); it!=m_vkeyboardh.end(); ++it)
        {
            if(*it==handler)
            {
                m_vkeyboardh.erase(it);
                break;
            }
        }
    }
    /**
    * Removes an input fingers handler from the callback list
    */
    virtual void remove_handler(FingersHandler *handler)
    {
        DEBUG_ASSERT(handler);
        for(auto it=m_vfingersh.begin(); it!=m_vfingersh.end(); ++it)
        {
            if(*it==handler)
            {
                m_vfingersh.erase(it);
                break;
            }
        }
    }
    /**
    * Removes an input accelerometer handler from the callback list
    */
    virtual void remove_handler(AccelerometerHandler *handler)
    {
        DEBUG_ASSERT(handler);
        for(auto it=m_vaccelerometerh.begin(); it!=m_vaccelerometerh.end(); ++it)
        {
            if(*it==handler)
            {
                m_vaccelerometerh.erase(it);
                break;
            }
        }
    }
    /**
    * Removes an input mouse handler from the callback list
    */
    virtual void remove_handler(MouseHandler *handler)
    {
        DEBUG_ASSERT(handler);
        for(auto it=m_vmouseh.begin(); it!=m_vmouseh.end(); ++it)
        {
            if(*it==handler)
            {
                m_vmouseh.erase(it);
                break;
            }
        }
    }

protected:

    std::vector<KeyboardHandler*> m_vkeyboardh;
    std::vector<FingersHandler*> m_vfingersh;
    std::vector<AccelerometerHandler*> m_vaccelerometerh;
    std::vector<MouseHandler*> m_vmouseh;
    std::vector<WindowHandler*> m_vwindowh;

};

};

#endif
