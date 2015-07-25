#include <stdafx.h>
#include <Config.h>
//ANDROID
#ifdef PLATFORM_ANDROID
typedef void (*AtExitFnPtr)(void);
static AtExitFnPtr atExitFnPtr =  nullptr;
extern "C" void callatexitANDROID()
{
    if(atExitFnPtr) atExitFnPtr();
}
#endif
//function
int Easy2D::atexit(void (*function)(void))
{
#ifdef PLATFORM_ANDROID
    atExitFnPtr = function;
    return 1;
#else
    return atexit(function);
#endif
}