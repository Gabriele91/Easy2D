#include <AndroidInput.h>
#include <AndroidMain.h>
#include <Input.h>
///////////////////////
using namespace Easy2D;
///////////////////////

AndroidInput::AndroidInput(){
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
		[](void* data,float x,float y,float z,float azimuth,float pitch,float roll){
			//save value
			AccelerometerValues accelerometerVs(x,y,z,azimuth,pitch,roll);
			((AndroidInput*)Application::instance()->getInput())->accelerometerVs=accelerometerVs;
			//callback
			((AndroidInput*)Application::instance()->getInput())
				->__callOnAccelerometerEvent(accelerometerVs);
	});
}

void AndroidInput::update(){
	//reset input window
	ewindow.resize=false;
	//update hit
	efingers.__clearHit();
	//update input
	updateInputAndroid();
	//update down fingers //callback
	efingers.__update(this);
}

//mouse
void AndroidInput::__callOnFingerMove(const Vec3& position, Key::Finger fid) {
	for(auto ih : vfingersh )
		ih->onFingerMove(position,fid);
}
void AndroidInput::__callOnFingerDown(const Vec3& position, Key::Finger fid) {
	for(auto ih : vfingersh )
		ih->onFingerDown(position,fid);
}
void AndroidInput::__callOnFingerPress(const Vec3& position, Key::Finger fid) {
	for(auto ih : vfingersh )
		ih->onFingerPress(position,fid);
}
void AndroidInput::__callOnFingerRelease(const Vec3& position, Key::Finger fid) {
	for(auto ih : vfingersh )
		ih->onFingerRelease(position,fid);
}
void AndroidInput::__callOnAccelerometerEvent(const AccelerometerValues& acVs){
	for(auto ih : vaccelerometerh )
		ih->onAcceleration(acVs);
}