#include <AndroidInput.h>
#include <AndroidMain.h>
#include <Input.h>
///////////////////////
using namespace Easy2D;
///////////////////////

AndroidInput::AndroidInput()
{
    //input window
    ewindow.__init();
    //input fingers
    efingers.__init();
    //set callback
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
    //update hit
    efingers.__clearHit();
    //update input
    updateInputAndroid();
    //update down fingers //callback
    efingers.__update(this);
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