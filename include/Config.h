#ifndef CONFIG_H
#define CONFIG_H

//std include C
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cfloat>
#include <climits>
//std include  Cpp
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
//os include
#include <sys/types.h>
#include <sys/stat.h>
//engine include
#include <Types.h>

#ifdef ORDERED_TABLE
	#include <map>
	#define ENABLE_ORDERED_TABLE
#endif

#if defined(_WIN32)
	#define PLATFORM_WINDOW	
	#define DCPP_11
	#define NOMINMAX
	#define ENABLE_VAOS
	#include <windows.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <window/OpenGLWindow.h>
	//#include <AL/al.h>
	//#include <AL/alc.h>
#elif defined(__linux__)
	#define PLATFORM_LINUX
	#define PLATFORM_UNIX
	#define DCPP_11
	#define ENABLE_VAOS
	#include <AL/al.h>
	#include <AL/alc.h>
	#include <GL/glew.h>
	#include <GL/glext.h>
	#include <GL/gl.h>
#elif defined(__APPLE__)
    #include "TargetConditionals.h"
	#define PLATFORM_UNIX
	#define ENABLE_VAOS

    #if TARGET_OS_IPHONE    
		#define PLATFORM_IPHONE

    #elif TARGET_IPHONE_SIMULATOR
		#define PLATFORM_IPHONE_SIMULATOR

    #elif TARGET_OS_MAC
		#define PLATFORM_MAC_OS_X

    #endif
#elif defined(__ANDROID__)
	#define PLATFORM_ANDROID
	#define PLATFORM_UNIX
	#define DCPP_X0
	#define DISABLE_VAOS
	#include <stdint.h>
	#include <AL/al.h>
	#include <AL/alc.h>
	#include <android/log.h>
	#include <android/native_activity.h>
	#include <android_native_app_glue.h>
#elif
	#error "platform not supported"
#endif


#ifdef _MSC_VER
	#define DFORCEINLINE __forceinline
	#define DINLINE __inline

	#if _MSC_VER == 1600
		#include <utility>
		#define decltype(...) \
		  std::identity<decltype(__VA_ARGS__)>::type
	#endif

#elif __GNUC__>=4 && __GNUC_MINOR__ >=6
	#define DFORCEINLINE __attribute__ ((always_inline)) 
	#define DINLINE inline
#elif 
	#error "compiler not supported"
#endif

#if !defined(ENABLE_VAOS) &&  !defined(DISABLE_VAOS)
	#error "must to be define ENABLE_VAOS or DISABLE_VAOS"
#elif defined(ENABLE_VAOS) &&  defined(DISABLE_VAOS)
	#error "must to be defined only ENABLE_VAOS or only DISABLE_VAOS"
#endif

#if defined(DCPP_11)
	#include <unordered_map>
	#include <functional>
	#include <memory> 
	#define DUNORDERED_MAP    std::unordered_map
	#define DUNORDERED_MULTIMAP    std::unordered_multimap
	#define DHASH    std::hash
	#define DNOSTDHASH hash
	#define DS_PTR std::shared_ptr
	#define DW_PTR std::weak_ptr
#elif defined(DCPP_X0)
	#include <tr1/unordered_map>
	#include <tr1/functional>
	#include <tr1/memory> 
	#define DUNORDERED_MAP    std::tr1::unordered_map
	#define DUNORDERED_MULTIMAP    std::tr1::unordered_multimap
	#define DHASH std::tr1::hash
	#define DNOSTDHASH tr1::hash
	#define DS_PTR std::tr1::shared_ptr
	#define DW_PTR std::tr1::weak_ptr
#endif



#endif