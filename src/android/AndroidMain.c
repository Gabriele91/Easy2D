#include <jni.h>
#include <android/sensor.h>
#include <android/log.h>
#include <android/input.h>
#include <android/native_activity.h>
#include <android_native_app_glue.h>

#ifndef bool
	#define bool char
	#define false 0
	#define true 1
#endif
//global variables
static struct android_app* app_state=NULL;
static const char* path_apk=NULL;
static bool isAValidDevice=false;

extern const char* getAndroidApk(){
        return path_apk;
}
extern struct android_app* getAndroidApp(){
        return app_state;
}
extern bool getIsAndroidValidDevice(){
	return isAValidDevice;
}
extern void setIsAndroidValidDevice(bool validDevice){
	isAValidDevice=validDevice;
}
extern void setAndroidUserData(void* userData){
	if(app_state)
		app_state->userData=userData;
}
//Sensors 
static struct Sensors{
	ASensorManager* sensorManager;
    ASensorEventQueue* sensorEventQueue;
    const ASensor* accelerometerSensor;
    //const ASensor* magneticSensor;
    //const ASensor* gyroscopeSensor;
    //const ASensor* lightSensor;
    //const ASensor* proximitySensor;
}sensors;
static void initSensors(){
	sensors.sensorManager=NULL;
	sensors.sensorEventQueue=NULL;
	sensors.accelerometerSensor=NULL;
}
static void createSensorsEventQueue(){
	//get sensor istance
    sensors.sensorManager = ASensorManager_getInstance();
	//create queue
    sensors.sensorEventQueue = 
		ASensorManager_createEventQueue(
		sensors.sensorManager,
		getAndroidApp()->looper, 
		LOOPER_ID_USER, NULL, NULL);	
}

//accelerometer
static void addAccelerometer(){
    sensors.accelerometerSensor =
		ASensorManager_getDefaultSensor(
			sensors.sensorManager,
            ASENSOR_TYPE_ACCELEROMETER);
}
static void setEnableAccelerometer(int32_t framerate){
	if(sensors.accelerometerSensor){
		ASensorEventQueue_enableSensor(sensors.sensorEventQueue,
									   sensors.accelerometerSensor);
		ASensorEventQueue_setEventRate(
							sensors.sensorEventQueue,
							sensors.accelerometerSensor, 
							framerate);
	}
}
static void setDisableAccelerometer(){
	if(sensors.accelerometerSensor){
                ASensorEventQueue_disableSensor(
						sensors.sensorEventQueue,
                        sensors.accelerometerSensor);
	}
}

static void (*accelerometerEvent)(void* data,float x,float y,float z,float azimuth,float pitch,float roll)=NULL;
extern void onAccelerometerEvent(void (*function)(void* data,float x,float y,float z,float azimuth,float pitch,float roll)){
	accelerometerEvent=function;
}

static void updateSensorsAccelerometer(void* data, int ident, int events){
	 if (ident == LOOPER_ID_USER) {
        if (sensors.accelerometerSensor) {
            ASensorEvent event;
            while (ASensorEventQueue_getEvents(sensors.sensorEventQueue,&event, 1) > 0) {
				if(accelerometerEvent)
					accelerometerEvent(data,
									   event.acceleration.x,
									   event.acceleration.y,
									   event.acceleration.z,
									   event.acceleration.azimuth,
									   event.acceleration.pitch,
									   event.acceleration.roll);
            }
        }
    }
}

//evets input
static void (*fingerDown)(void* data,int i,float x,float y,float p)=NULL;
static void (*fingerUp)(void* data,int i,float x,float y,float p)=NULL;
static void (*fingerMove)(void* data,int i,float x,float y,float p)=NULL;

extern void onFingerDown(void(*function)(void* data,int i,float x,float y,float p)){
	fingerDown=function;
}
extern void onFingerUp(void(*function)(void* data,int i,float x,float y,float p)){
	fingerUp=function;
}
extern void onFingerMove(void(*function)(void* data,int i,float x,float y,float p)){
	fingerMove=function;
}
//callback
extern int32_t __android_handle_input(struct android_app* app, AInputEvent* event){

		unsigned int action = AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
        unsigned int actionPointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

		if (action == AMOTION_EVENT_ACTION_DOWN || action == AMOTION_EVENT_ACTION_POINTER_DOWN) {
			if(fingerDown){
				int count = AMotionEvent_getPointerCount(event);
				int i = 0;
				for(;i < count ; i++) {
						unsigned int pointerFingerId = AMotionEvent_getPointerId(event, i);
						fingerDown(app->userData,pointerFingerId,
												 AMotionEvent_getX(event, i),
												 AMotionEvent_getY(event, i),
												 AMotionEvent_getPressure(event, i));
				}
			}
		} else if (action == AMOTION_EVENT_ACTION_UP || action == AMOTION_EVENT_ACTION_POINTER_UP) { //up			
			if(fingerUp){
				int count = AMotionEvent_getPointerCount(event);
				int i = 0;
				for(;i < count ; i++) {
						unsigned int pointerFingerId = AMotionEvent_getPointerId(event, i);
						fingerUp(app->userData,pointerFingerId,
										       AMotionEvent_getX(event, i),
											   AMotionEvent_getY(event, i),
											   AMotionEvent_getPressure(event, i));
				}
			}
		} else if (action == AMOTION_EVENT_ACTION_MOVE) {                                           //move
			if(fingerMove){
				int count = AMotionEvent_getPointerCount(event);
				int i = 0;
				for(;i < count ; i++) {
						unsigned int pointerFingerId = AMotionEvent_getPointerId(event, i);
						fingerMove(app->userData,pointerFingerId,
												 AMotionEvent_getX(event, i),
												 AMotionEvent_getY(event, i),
												 AMotionEvent_getPressure(event, i));
				}
			}
		} else if (action == AMOTION_EVENT_ACTION_CANCEL) {                                  
		} else {
			return 0;
		}
}

//events cmd
static void (*initAndroid)(void* data)=NULL;
static void (*resumeAndroid)(void* data)=NULL;
static void (*termAndroid)(void* data)=NULL;
static void (*pauseAndroid)(void* data)=NULL;
static void (*configChange)(void* data)=NULL;
static void (*windowResized)(void* data)=NULL;
static void (*getFocusAndroid)(void* data)=NULL;
static void (*lostFocusAndroid)(void* data)=NULL;

extern void onInitAndroid(void (*function)(void* data)){
	initAndroid=function;
}
extern void onResumeAndroid(void (*function)(void* data)){
	resumeAndroid=function;
}
extern void onTermAndroid(void (*function)(void* data)){
	termAndroid=function;
}
extern void onPauseAndroid(void (*function)(void* data)){
	pauseAndroid=function;
}
extern void onConfigChange(void (*function)(void* data)){
	configChange=function;
}
extern void onWindowResized(void (*function)(void* data)){
	windowResized=function;
}
extern void onGetFocusAndroid(void (*function)(void* data)){
	getFocusAndroid=function;
}
extern void onLostFocusAndroid(void (*function)(void* data)){
	lostFocusAndroid=function;
}
//callback
extern void __android_handle_cmd(struct android_app* app, int32_t cmd) {
	switch(cmd){
		case APP_CMD_INIT_WINDOW: 
			setIsAndroidValidDevice(true);
			if(initAndroid) 
				initAndroid(app->userData);
		break;
		case APP_CMD_TERM_WINDOW: 
			if(termAndroid) 
				termAndroid(app->userData);
		break;
		case APP_CMD_PAUSE: 
			setIsAndroidValidDevice(false);
			if(pauseAndroid) 
				pauseAndroid(app->userData);
		break;
		case APP_CMD_WINDOW_RESIZED:
			if(windowResized) 
				windowResized(app->userData);
		break;
		case APP_CMD_RESUME: 
			if(resumeAndroid) 
				resumeAndroid(app->userData);
		break;  
		case APP_CMD_CONFIG_CHANGED:
			if(configChange) 
				configChange(app->userData);
        break;
		case APP_CMD_GAINED_FOCUS: 
			setEnableAccelerometer((1000L/60)*1000);
			if(getFocusAndroid) 
				getFocusAndroid(app->userData);
		break;  
		case APP_CMD_LOST_FOCUS: 
			setDisableAccelerometer();
			if(lostFocusAndroid) 
				lostFocusAndroid(app->userData);
		break;
		default:break;
	}
	
}


//input update
extern void updateInputAndroid(){

	int ident,events;
	struct android_poll_source* source;

	while ((ident = ALooper_pollAll(0, NULL, &events,(void**)&source)) >= 0){
		//updata android event
		if (source != NULL) 
			source->process(app_state, source);
		//update accelerometer
		updateSensorsAccelerometer(getAndroidApp()!=NULL ? 
								   getAndroidApp()->userData : 
								   NULL ,ident,events);
	}

}

//externs
extern int main(int argc, char *argv[]);
extern void overloadSTDCOUT();
extern void unoverloadSTDCOUT();
//main
/*********************************
Android MAIN
**********************************/
extern void android_main(struct android_app* state) {
    /////////////////////////////////
    // Make sure glue isn't stripped.
    app_dummy();
	//enable COUT 
	overloadSTDCOUT();
    /////////////////////////////////
    //Init state
    state->onAppCmd = __android_handle_cmd;
    state->onInputEvent = __android_handle_input;
    //save app
    app_state=state;
	////////////////////////////
	//init sensors
	initSensors();
	createSensorsEventQueue();
	addAccelerometer();
	////////////////////////////
    int ident;
    int events;
    struct android_poll_source* source;
    //waiting to get window
    while (!getIsAndroidValidDevice()){
        while ((ident = ALooper_pollAll(0, NULL, &events,(void**)&source)) >= 0){
                if (source != NULL) source->process(state, source);
                if (state->destroyRequested != 0) return;
        }
        usleep( 16 );
    }
	////////////////////////////

    ////////////////////////////
    //GET APK (ZIP FILE)
    ANativeActivity* activity = state->activity;
    JNIEnv* env;
    (*(activity->vm))->AttachCurrentThread(activity->vm, &env, NULL);
    jclass clazz = (*env)->GetObjectClass(env, activity->clazz);
    jmethodID methodID = (*env)->GetMethodID(env, clazz, "getPackageCodePath", "()Ljava/lang/String;");
	jobject result = (*env)->CallObjectMethod(env, activity->clazz, methodID);
    jboolean isCopy;
    path_apk= (const char*) (*env)->GetStringUTFChars(env, (jstring)result, &isCopy);
    ////////////////////////////
    ////////////////////////////
    //INIT openAL/backend
    //JNI_OnLoad(activity->vm,0);
    ////////////////////////
	//call main
    char *argv[2];
    argv[0] = strdup("Easy2D");
    argv[1] = NULL;
    int out=main(1, argv);
	//disable COUT 
	unoverloadSTDCOUT();
}