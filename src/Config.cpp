#include <stdafx.h>
#include <Config.h>
#ifdef PLATFORM_ANDROID
//POSIX
extern "C" int __cxa_atexit(void (*func) (void *), void * arg, void * dso_handle);
//WAPPER
static void wrapperAtexit (void* arg){
		((void (*)(void))arg)();
}
//function 
int Easy2D::atexit(void (*function)(void)){
	__cxa_atexit(wrapperAtexit,(void*)function,NULL);
}
#endif