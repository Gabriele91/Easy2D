#ifndef CONFIG_H
#define CONFIG_H

#include <cmath>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cfloat>
#include <climits>
#include <list>
#include <string>
#include <sstream>

#ifdef ORDERED_TABLE
	#include <map>
	#define ENABLE_ORDERED_TABLE
#endif

#if defined(_WIN32)
	#define PLATFORM_WINDOW	
	#define DCPP_11
	#define NOMINMAX
	#include <windows.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
	//#include <AL/al.h>
	//#include <AL/alc.h>
#elif defined(__linux__)
	#define PLATFORM_LINUX
	#define DCPP_11
	#include <AL/al.h>
	#include <AL/alc.h>
	#include <GL/glew.h>
	#include <GL/glext.h>
	#include <GL/gl.h>
#elif defined(__ANDROID__)
	#define PLATFORM_ANDROID
	#define DCPP_X0
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
#elif __GNUC__>=4 && __GNUC_MINOR__ >=6
	#define DFORCEINLINE __attribute__ ((always_inline)) 
	#define DINLINE inline
#elif 
	#error "compiler not supported"
#endif


#if defined(DCPP_11)
	#include <unordered_map>
	#include <functional>
	#define DUNORDERED_MAP    std::unordered_map
	#define DUNORDERED_MULTIMAP    std::unordered_multimap
	#define DHASH    std::hash
	#define DNOSTDHASH hash
#elif defined(DCPP_X0)
	#include <tr1/unordered_map>
	#include <tr1/functional>
	#define DUNORDERED_MAP    std::tr1::unordered_map
	#define DUNORDERED_MULTIMAP    std::tr1::unordered_multimap
	#define DHASH std::tr1::hash
	#define DNOSTDHASH tr1::hash
#endif



#endif