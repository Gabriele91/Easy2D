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
extern JavaVM* getJavaVM(){
	return app_state->activity->vm;
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

static void (*keyDown)(void* data,int key)=NULL;
static void (*keyUp)(void* data,int key)=NULL;

extern void onKeyDown(void(*function)(void* data,int key))
{
	keyDown=function;
}
extern void onKeyUp(void(*function)(void* data,int key))
{
	keyUp=function;
}

//callback
extern int32_t __android_handle_input(struct android_app* app, AInputEvent* event){

		/*
		Input source
		*/
		int nSourceId = AInputEvent_getSource( event );
		//not supported device?
		if (!(nSourceId == AINPUT_SOURCE_TOUCHPAD  || 
			  nSourceId == AINPUT_SOURCE_KEYBOARD || 
			  nSourceId == AINPUT_SOURCE_TOUCHSCREEN))
	    	return 0;  
	    //keyboard
	    if(nSourceId == AINPUT_SOURCE_KEYBOARD)
	    {
		    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY) 
		    { //keybord down
				 //get key code
				 unsigned int keyAction =  AKeyEvent_getAction(event);
				 //
				 if( keyDown && keyAction == AKEY_EVENT_ACTION_DOWN )
				 {
				 	keyDown(app->userData,AKeyEvent_getKeyCode(event));
				 }
				 else if( keyUp && keyAction == AKEY_EVENT_ACTION_UP )
				 {
				 	keyUp(app->userData,AKeyEvent_getKeyCode(event));
				 }
				 else 
				 {
				 	return 0;
				 }
				 return 1;
			}
			return 0;
		}
	    //action
	    unsigned int unmaskAction = AMotionEvent_getAction(event);
	    //base action
		unsigned int action = unmaskAction & AMOTION_EVENT_ACTION_MASK; 
		//pointer action
        unsigned int actionPointerIndex = (unmaskAction & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK);
        			 actionPointerIndex >>= AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
        
		if (action == AMOTION_EVENT_ACTION_DOWN) { //all down
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
		} else if (action == AMOTION_EVENT_ACTION_POINTER_DOWN ) { //one down
			if(fingerDown){
				int count = AMotionEvent_getPointerCount(event);
				int i = 0;
				for(;i < count ; i++) {
					unsigned int pointerFingerId = AMotionEvent_getPointerId(event, i);
					if( pointerFingerId==actionPointerIndex ) {
						fingerDown(app->userData,pointerFingerId,
												 AMotionEvent_getX(event, i),
												 AMotionEvent_getY(event, i),
												 AMotionEvent_getPressure(event, i));
					}
				}
			}
		} else if (action == AMOTION_EVENT_ACTION_UP) { //up all			
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
		} else if (action == AMOTION_EVENT_ACTION_POINTER_UP) { //up one			
			if(fingerUp){
				int count = AMotionEvent_getPointerCount(event);
				int i = 0;
				for(;i < count ; i++) {
					unsigned int pointerFingerId = AMotionEvent_getPointerId(event, i);
					if( pointerFingerId==actionPointerIndex ) {
						fingerUp(app->userData,pointerFingerId,
										       AMotionEvent_getX(event, i),
											   AMotionEvent_getY(event, i),
											   AMotionEvent_getPressure(event, i));
					}
				}
			}
		} else if (action == AMOTION_EVENT_ACTION_MOVE) { //move
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
	return 1;
}

//events cmd
static void (*initAndroid)(void* data)=NULL;
static void (*resumeAndroid)(void* data)=NULL;
static void (*termAndroid)(void* data)=NULL;
static void (*saveStateAndroid)(void* data)=NULL;
static void (*pauseAndroid)(void* data)=NULL;
static void (*configChange)(void* data)=NULL;
static void (*windowResized)(void* data,int width,int height)=NULL;
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
void onSaveStateAndroid(void (*function)(void* data)){
	saveStateAndroid=function;
}
extern void onPauseAndroid(void (*function)(void* data)){
	pauseAndroid=function;
}
extern void onConfigChange(void (*function)(void* data)){
	configChange=function;
}
extern void onWindowResized(void (*function)(void* data,int width,int height)){
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
			if(windowResized){ 
				int32_t width = ANativeWindow_getWidth(getAndroidApp()->window);
				int32_t height = ANativeWindow_getHeight(getAndroidApp()->window);
				windowResized(app->userData,(int)width,(int)height);
			}
		break;
		case APP_CMD_RESUME: 
			if(resumeAndroid) 
				resumeAndroid(app->userData);
		break;  
		case APP_CMD_CONFIG_CHANGED:
			if(configChange) 
				configChange(app->userData);
        break;
		case APP_CMD_SAVE_STATE:			
			if(saveStateAndroid) 
				saveStateAndroid(app->userData);
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
extern void callatexitANDROID();
/*********************************
Java Thread
**********************************/
static JNIEnv* javaEnvMainThread;
static void attachToAndroidThreadJava(){
    (*(getJavaVM()))->AttachCurrentThread(getJavaVM(), &javaEnvMainThread, NULL);
}
static void deattachToAndroidThreadJava(){
    (*(getJavaVM()))->DetachCurrentThread(getJavaVM());
}
extern JNIEnv* getEnv(){
	return javaEnvMainThread;
}
/*********************************
Android JNI
**********************************/
struct Easy2DActivity{
	jobject self;
	jclass selfClass;
	jmethodID setScreenOrientation,
			  getScreenOrientation,
		      setScreenTitle,
			  setFullScreenMode,
              displayKeyboard;
}easy2DActivity;

static void callEasy2DActivityVoidMethod(jmethodID field,...){
	va_list arguments;
	(*(getEnv()))->CallVoidMethodV(getEnv(),easy2DActivity.self,field, arguments);
}
static void easy2DActivitySetScreenOrientation(JNIEnv *env,int orientation){
	(*env)->CallVoidMethod(env,easy2DActivity.self,
						       easy2DActivity.setScreenOrientation, 
							   orientation);	
}
static int easy2DActivityGetScreenOrientation(JNIEnv *env){
	return (*env)->CallIntMethod(env,easy2DActivity.self,
						             easy2DActivity.getScreenOrientation);	
}
static void easy2DActivitySetScreenTitle(JNIEnv *env,const char *str){
	jstring jstr = (*env)->NewStringUTF(env,str);
	(*env)->CallVoidMethod(env,easy2DActivity.self,easy2DActivity.setScreenTitle, jstr);
}
static void easy2DActivitySetFullScreenMode(JNIEnv *env){
    (*env)->CallVoidMethod(env,easy2DActivity.self,easy2DActivity.setFullScreenMode);
}
static bool easy2DActivityDisplayKeyboard(JNIEnv *env,jboolean show){
    return (*env)->CallBooleanMethod(env,easy2DActivity.self,
                                         easy2DActivity.displayKeyboard,
                                         show);
}

JNIEXPORT void JNICALL Java_com_easy2d_Easy2DActivity_onCreateInterface(JNIEnv *env, jobject self){
//init easy2DActivity 
  easy2DActivity.self= (*env)->NewGlobalRef(env,self);
  easy2DActivity.selfClass = (*env)->GetObjectClass(env, self);
  easy2DActivity.setScreenOrientation= (*env)->GetMethodID(env, easy2DActivity.selfClass,"setScreenOrientation","(I)V");
  easy2DActivity.getScreenOrientation= (*env)->GetMethodID(env, easy2DActivity.selfClass,"getScreenOrientation","()I");
  easy2DActivity.setScreenTitle= (*env)->GetMethodID(env, easy2DActivity.selfClass,"setScreenTitle","(Ljava/lang/String;)V");
  easy2DActivity.setFullScreenMode= (*env)->GetMethodID(env, easy2DActivity.selfClass,"setFullScreenMode","()V");
  easy2DActivity.displayKeyboard= (*env)->GetMethodID(env, easy2DActivity.selfClass,"displayKeyboard","(Z)Z");
  easy2DActivitySetFullScreenMode(env);
}
//wrapper
extern void setAndroidScreenOrientation(int orientation){
	easy2DActivitySetScreenOrientation(getEnv(),orientation);
}
extern int getAndroidScreenOrientation(){
	return easy2DActivityGetScreenOrientation(getEnv());
}
extern void setAndroidScreenTitle(const char *str){
	easy2DActivitySetScreenTitle(getEnv(),str);
}
extern void setAndroidFullScreenMode(const char *str){
	easy2DActivitySetFullScreenMode(getEnv());
}
extern bool displayAndroidKeyboard(bool show) {
    return easy2DActivityDisplayKeyboard(getEnv(),show);
}
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
	//attach java thread
	attachToAndroidThreadJava();	
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
    //GET APK (ZIP FILE)
    ANativeActivity* activity = state->activity;
    jclass clazz = (*getEnv())->GetObjectClass(getEnv(), activity->clazz);
    jmethodID methodID = (*getEnv())->GetMethodID(getEnv(), clazz, "getPackageCodePath", "()Ljava/lang/String;");
	jobject result = (*getEnv())->CallObjectMethod(getEnv(), activity->clazz, methodID);
    jboolean isCopy;
    path_apk= (const char*) (*getEnv())->GetStringUTFChars(getEnv(), (jstring)result, &isCopy);
    ////////////////////////////
    //INIT openAL/backend
    JNI_OnLoad(activity->vm,0);
    //no in OpenSL ES backend
    ////////////////////////
	//call main
    char *argv[2];
    argv[0] = strdup("Easy2D");
    argv[1] = NULL;
    int out=main(1, argv);
    //call atexit
    callatexitANDROID();
	//deattach java thread
	deattachToAndroidThreadJava();
	//disable COUT 
	unoverloadSTDCOUT();
}