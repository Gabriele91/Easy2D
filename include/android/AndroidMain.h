#ifndef ANDROIDMAIN_H
#define ANDROIDMAIN_H

#include <android/native_activity.h>

#ifdef __cplusplus
extern "C"{
#endif

	//query application
	const char* getAndroidApk();
	struct android_app* getAndroidApp();
	bool getIsAndroidValidDevice();
	//set value
	void setIsAndroidValidDevice(bool validDevice);
	void setAndroidUserData(void* userData);
	//accelerometer input
	void onAccelerometerEvent(void (*function)(void* data,float x,float y,float z,float azimuth,float pitch,float roll));
	//input handle
	void onFingerDown(void(*function)(void* data,int i,float x,float y,float p));
	void onFingerUp(void(*function)(void* data,int i,float x,float y,float p));
	void onFingerMove(void(*function)(void* data,int i,float x,float y,float p));
	//cmd handle
	void onInitAndroid(void (*function)(void* data));
	void onResumeAndroid(void (*function)(void* data));
	void onTermAndroid(void (*function)(void* data));
    void onConfigChange(void (*function)(void* data));
	void onPauseAndroid(void (*function)(void* data));
	void onWindowResized(void (*function)(void* data));
	void onGetFocusAndroid(void (*function)(void* data));
	void onLostFocusAndroid(void (*function)(void* data));
	//input update (in loop)
	void updateInputAndroid();
	//extern main
	int main(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif