#include <AndroidInput.h>
#include <AndroidMain.h>
#include <Input.h>
///////////////////////
using namespace Easy2D;
///////////////////////
/* ANDROID KEY MAP */
Key::Keyboard KeyAndroidMap[210+1]=
{
    Key::KEY_NULL,
    Key::KP4,
    Key::KP6,
    Key::HOME,
    Key::ESCAPE,
    Key::TAB,
    Key::LSHIFT,
    Key::N0,
    Key::N1,
    Key::N2,
    Key::N3,
    Key::N4,
    Key::N5,
    Key::N6,
    Key::N7,
    Key::N8,
    Key::N9,
    Key::KP_DIVIDE,
    Key::KP_MULTIPLY,
    Key::UP,
    Key::DOWN,
    Key::LEFT,
    Key::RIGHT,
    Key::END,
    Key::PAGEUP,
    Key::PAGEDOWN,
    Key::RALT,
    Key::RSHIFT,
    Key::BACKSPACE,
    Key::A,
    Key::B,
    Key::C,
    Key::D,
    Key::E,
    Key::F,
    Key::G,
    Key::H,
    Key::I,
    Key::J,
    Key::K,
    Key::L,
    Key::M,
    Key::N,
    Key::O,
    Key::P,
    Key::Q,
    Key::R,
    Key::S,
    Key::T,
    Key::U,
    Key::V,
    Key::W,
    Key::X,
    Key::Y,
    Key::Z,
    Key::COMMA,
    Key::PERIOD,
    Key::KP7,
    Key::KP9,
    Key::F1,
    Key::F2,
    Key::TAB,
    Key::SPACE,
    Key::LMETA,
    Key::F3,
    Key::F4,
    Key::RETURN,
    Key::KDELETE,
    Key::KEY_NULL,
    Key::KP_MINUS,
    Key::EQUALS,
    Key::LEFTBRACKET,
    Key::RIGHTBRACKET,
    Key::BACKSLASH,
    Key::SEMICOLON,
    Key::APOSTROPHE,
    Key::SLASH,
    Key::KP_PERIOD,
    Key::NUMLOCK,
    Key::F5,
    Key::RSHIFT,
    Key::KP_PLUS,
    Key::LCTRL,
    Key::F6,
    Key::LSHIFT,
    Key::KP2,
    Key::HELP,
    Key::KP8,
    Key::KP5,
    Key::KP1,
    Key::KP3,
    Key::KP0,
    Key::PAGEUP,
    Key::PAGEDOWN,
    Key::LSHIFT,
    Key::LSHIFT,
    Key::A,
    Key::B,
    Key::C,
    Key::RETURN,
    Key::END,
    Key::Z,
    Key::PAGEUP,
    Key::PAGEDOWN,
    Key::LCTRL,
    Key::RCTRL,
    Key::LALT,
    Key::RALT,
    Key::RETURN,
    Key::ESCAPE,
    Key::SPACE,
    Key::ESCAPE,
    Key::KDELETE,
    Key::LEFT,
    Key::RIGHT,
    Key::CAPSLOCK,
    Key::SCROLLOCK,
    Key::LEFT,
    Key::RIGHT,
    Key::RMETA,
    Key::KEY_NULL,
    Key::PAUSE,
    Key::HOME,
    Key::END,
    Key::INSERT,
    Key::RIGHT,
    Key::F1,
    Key::F2,
    Key::F3,
    Key::F4,
    Key::F5,
    Key::F1,
    Key::F2,
    Key::F3,
    Key::F4,
    Key::F5,
    Key::F6,
    Key::F7,
    Key::F8,
    Key::F9,
    Key::F10,
    Key::F11,
    Key::F12,
    Key::NUMLOCK,
    Key::KP0,
    Key::KP1,
    Key::KP2,
    Key::KP3,
    Key::KP4,
    Key::KP5,
    Key::KP6,
    Key::KP7,
    Key::KP8,
    Key::KP9,
    Key::KP_DIVIDE,
    Key::KP_MULTIPLY,
    Key::KP_MINUS,
    Key::KP_PLUS,
    Key::KP_PERIOD,
    Key::KP_PERIOD,
    Key::KEY_NULL,
    Key::KEY_NULL,
    Key::KP_DIVIDE,
    Key::KP_MULTIPLY,
    Key::F13,
    Key::F14,
    Key::UP,
    Key::DOWN,
    Key::PAGEUP,
    Key::PAGEDOWN,
    Key::F15,
    Key::F1,
    Key::F2,
    Key::F3,
    Key::F4,
    Key::F5,
    Key::F6,
    Key::F7,
    Key::F8,
    Key::F9,
    Key::F10,
    Key::F11,
    Key::F12,
    Key::F13,
    Key::F14,
    Key::F15,
    Key::F1,
    Key::F2,
    Key::A,
    Key::B,
    Key::C,
    Key::D,
    Key::E,
    Key::F,
    Key::G,
    Key::H,
    Key::I,
    Key::J,
    Key::K,
    Key::L,
    Key::M,
    Key::N,
    Key::O,
    Key::P,
    Key::KEY_NULL,
    Key::KEY_NULL,
    Key::KEY_NULL,
    Key::KEY_NULL,
    Key::KEY_NULL,
    Key::KEY_NULL,
    Key::KEY_NULL,
};



AndroidInput::AndroidInput()
{
    //input window
    ewindow.__init();
    //input fingers
    efingers.__init();
    //input keyboard
    ekeyboard.__init();
    //set callback
    onKeyDown(
        [](void* data,int code)
    {   
        //save input
        auto input=((AndroidInput*)Application::instance()->getInput());
        //get if olready pressed
        bool dopress=!input->getKeyDown(KeyAndroidMap[code]);
        //update input
        input->ekeyboard.__keyboardDown(KeyAndroidMap[code]);
        //callback only first down
        if(dopress)
        {
            input->__callOnKeyPress(KeyAndroidMap[code]);
        }
    });
    onKeyUp(
        [](void* data,int code)
    {
        ((AndroidInput*)Application::instance()->getInput())
        ->ekeyboard.__keyboardUp(KeyAndroidMap[code]);
        //callback
        ((AndroidInput*)Application::instance()->getInput())
        ->__callOnKeyRelease(KeyAndroidMap[code]);
    });
    onFingerDown(
        [](void* data,int i,float x,float y,float p)
    {
        ((AndroidInput*)Application::instance()->getInput())
        ->efingers.__fingerDown((Key::Finger)(i+1),Vec3(x,y,p));
        //callback
        ((AndroidInput*)Application::instance()->getInput())
        ->__callOnFingerPress(Vec3(x,y,p),(Key::Finger)(i+1));
    }
    );
    onFingerUp(
        [](void* data,int i,float x,float y,float p)
    {
        ((AndroidInput*)Application::instance()->getInput())
        ->efingers.__fingerUp((Key::Finger)(i+1),Vec3(x,y,p));
        //callback
        ((AndroidInput*)Application::instance()->getInput())
        ->__callOnFingerRelease(Vec3(x,y,p),(Key::Finger)(i+1));
    }
    );
    onFingerMove(
        [](void* data,int i,float x,float y,float p)
    {
        ((AndroidInput*)Application::instance()->getInput())
        ->efingers.__fingerDown((Key::Finger)(i+1),Vec3(x,y,p));
        //callback
        ((AndroidInput*)Application::instance()->getInput())
        ->__callOnFingerMove(Vec3(x,y,p),(Key::Finger)(i+1));
    }
    );
    onAccelerometerEvent(
        [](void* data,float x,float y,float z,float azimuth,float pitch,float roll)
    {
        //save value
        AccelerometerValues accelerometerVs(x,y,z,azimuth,pitch,roll);
        ((AndroidInput*)Application::instance()->getInput())->accelerometerVs=accelerometerVs;
        //callback
        ((AndroidInput*)Application::instance()->getInput())
        ->__callOnAccelerometerEvent(accelerometerVs);
    });
    //window events
    onGetFocusAndroid(
        [](void* data)
    {
        ((AndroidInput*)Application::instance()->getInput())->ewindow.focus=true;
        ((AndroidInput*)Application::instance()->getInput())->__callOnFocus(true);
    });
    onLostFocusAndroid(
        [](void* data)
    {
        ((AndroidInput*)Application::instance()->getInput())->ewindow.focus=false;
        ((AndroidInput*)Application::instance()->getInput())->__callOnFocus(false);
    });
    onTermAndroid(
        [](void* data)
    {
        /*
        ((AndroidInput*)Application::instance()->getInput())->ewindow.close=true;
        ((AndroidInput*)Application::instance()->getInput())->__callOnClose();
        */
    });
    onWindowResized(
        [](void* data,int w,int h)
    {
        ((AndroidInput*)Application::instance()->getInput())->ewindow.resize=true;
        ((AndroidInput*)Application::instance()->getInput())->ewindow.windowResize=Vec2((float)w,(float)h);
        ((AndroidInput*)Application::instance()->getInput())->__callOnResize(Vec2((float)w,(float)h));
    });
}

void AndroidInput::update()
{
    //reset input window
    ewindow.resize=false;
    //reset hit keyboard
    ekeyboard.__clearHit();
    //update hit
    efingers.__clearHit();
    //update input
    updateInputAndroid();
    //update keyboard
    ekeyboard.__update(this);
    //update down fingers 
    efingers.__update(this);
}

//keyboards
void AndroidInput::__callOnKeyPress(Key::Keyboard key)
{
    for(size_t i=0; i!=vkeyboardh.size(); ++i)
        vkeyboardh[i]->onKeyPress(key);
}
void AndroidInput::__callOnKeyRelease(Key::Keyboard key)
{
    for(size_t i=0; i!=vkeyboardh.size(); ++i)
        vkeyboardh[i]->onKeyRelease(key);
}
void AndroidInput::__callOnKeyDown(Key::Keyboard key)
{
    for(size_t i=0; i!=vkeyboardh.size(); ++i)
        vkeyboardh[i]->onKeyDown(key);
}
//window
void AndroidInput::__callOnFocus(bool focus)
{
    for(size_t i=0; i!=vwindowh.size(); ++i)
        vwindowh[i]->onFocus(focus);
}
void AndroidInput::__callOnClose()
{
    for(size_t i=0; i!=vwindowh.size(); ++i)
        vwindowh[i]->onClose();
}
void AndroidInput::__callOnResize(Vec2 size)
{
    for(size_t i=0; i!=vwindowh.size(); ++i)
        vwindowh[i]->onResize(size);
}
//Finger
void AndroidInput::__callOnFingerMove(const Vec3& position, Key::Finger fid)
{
    for(size_t i=0; i!=vfingersh.size(); ++i)
        vfingersh[i]->onFingerMove(position,fid);
}
void AndroidInput::__callOnFingerDown(const Vec3& position, Key::Finger fid)
{
    for(size_t i=0; i!=vfingersh.size(); ++i)
        vfingersh[i]->onFingerDown(position,fid);
}
void AndroidInput::__callOnFingerPress(const Vec3& position, Key::Finger fid)
{
    for(size_t i=0; i!=vfingersh.size(); ++i)
        vfingersh[i]->onFingerPress(position,fid);
}
void AndroidInput::__callOnFingerRelease(const Vec3& position, Key::Finger fid)
{
    for(size_t i=0; i!=vfingersh.size(); ++i)
        vfingersh[i]->onFingerRelease(position,fid);
}
void AndroidInput::__callOnAccelerometerEvent(const AccelerometerValues& acVs)
{
    for(size_t i=0; i!=vaccelerometerh.size(); ++i)
        vaccelerometerh[i]->onAcceleration(acVs);
}