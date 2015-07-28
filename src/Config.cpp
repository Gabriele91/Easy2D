#include <stdafx.h>
#include <Config.h>
//ANDROID
#ifdef PLATFORM_ANDROID
typedef void (*AtExitFnPtr)(void);
static std::vector < AtExitFnPtr > atExitFnPtrs;
extern "C" void callatexitANDROID()
{
    for(auto funAtExit:atExitFnPtrs)
    {
        funAtExit();
    }
}
#endif
//function
int Easy2D::atexit(void (*function)(void))
{
#ifdef PLATFORM_ANDROID
    atExitFnPtrs.push_back(function);
    return 0;
#else
    return ::atexit(function);
#endif
}