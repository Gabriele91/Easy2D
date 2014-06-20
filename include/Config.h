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
#include <cstdlib>
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

#if defined(__ANDROID__)
#define PLATFORM_ANDROID
#define PLATFORM_UNIX
#define DCPP_11
#define DISABLE_VAOS
#define OPENGL_ES
#include <stdint.h>
//#include <AL/al.h>
//#include <AL/alc.h>
#include <android/log.h>
#include <android/native_activity.h>
#include <android_native_app_glue.h>
#include <android/OpenGLAndroid.h>
#include <AL/al.h>
#include <AL/alc.h>
namespace Easy2D
{
int atexit(void (*function)(void));
};
#elif defined(_WIN32)
#define PLATFORM_WINDOW
#define DCPP_11
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <window/OpenGLWindow.h>
#include <AL/al.h>
#include <AL/alc.h>
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#define PLATFORM_UNIX

#if TARGET_OS_IPHONE
#define PLATFORM_IPHONE
#define PLATFORM_IOS
#elif TARGET_IPHONE_SIMULATOR
#define PLATFORM_IPHONE_SIMULATOR
#define PLATFORM_IOS
#elif TARGET_OS_MAC
#define PLATFORM_MAC_OS_X
#define PLATFORM_OSX
#define PLATFORM_UNIX
#define DCPP_11
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

#elif defined(__linux__)
#define PLATFORM_LINUX
#define PLATFORM_UNIX
#define DCPP_11
#include <signal.h>
#include <stdint.h>
#include <GL/gl.h>
#include <linux/OpenGLLinux.h>
#include <AL/al.h>
#include <AL/alc.h>

#elif defined(EMSCRIPTEN)

#define PLATFORM_EMSCRIPTEN
#define PLATFORM_UNIX
#define DCPP_11
#define OPENGL_ES
#define DISABLE_MIDMAP
#include <signal.h>
#include <emscripten/OpenGLEmscripten.h>
#include <stdint.h>
#include <AL/al.h>
#include <AL/alc.h>


#else
#error "platform not supported"
#endif


//default settings
#if !defined(ENABLE_CPU_BATCHING_MESH) &&  !defined(DISABLE_CPU_BATCHING_MESH)
#define ENABLE_CPU_BATCHING_MESH
//#define DISABLE_CPU_BATCHING_MESH
#endif

//default settings
#if !defined(ENABLE_STREAM_BUFFER) &&  !defined(DISABLE_STREAM_BUFFER)
#define ENABLE_STREAM_BUFFER
//#define DISABLE_STREAM_BUFFER
#endif

//default settings
#if !defined(ENABLE_VAOS) &&  !defined(DISABLE_VAOS)
//#define ENABLE_VAOS
#define DISABLE_VAOS
#endif


#if !defined(ENABLE_VAOS) &&  !defined(DISABLE_VAOS)
#error "must to be define ENABLE_VAOS or DISABLE_VAOS"
#elif defined(ENABLE_VAOS) &&  defined(DISABLE_VAOS)
#error "must to be defined only ENABLE_VAOS or only DISABLE_VAOS"
#endif

#if !defined(ENABLE_STREAM_BUFFER) &&  !defined(DISABLE_STREAM_BUFFER)
#error "must to be define ENABLE_STREAM_BUFFER or DISABLE_VAOS"
#elif defined(ENABLE_STREAM_BUFFER) &&  defined(DISABLE_STREAM_BUFFER)
#error "must to be defined only ENABLE_STREAM_BUFFER or only DISABLE_VAOS"
#endif

#if !defined(ENABLE_CPU_BATCHING_MESH) &&  !defined(DISABLE_CPU_BATCHING_MESH)
#error "must to be define ENABLE_CPU_BATCHING_MESH or DISABLE_CPU_BATCHING_MESH"
#elif defined(ENABLE_CPU_BATCHING_MESH) &&  defined(DISABLE_CPU_BATCHING_MESH)
#error "must to be defined only ENABLE_CPU_BATCHING_MESH or only DISABLE_CPU_BATCHING_MESH"
#endif

#if ( (__GNUC__>=4) && (__GNUC_MINOR__ >=6) || defined(__ANDROID__) ) || defined(__llvm__)
//types
#define cppTypeInfo std::type_info

#ifndef DISABLE_FORCE_INLINE
#define DFORCEINLINE __attribute__ ((always_inline))
#else
#define DFORCEINLINE inline
#endif
#define DINLINE inline
#define COMPILER_GCC
#define ASPACKED( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#define ALIGNED( size, __Declaration__ )  __Declaration__ __attribute__ ((aligned(size)))

#if !defined( PFD_SUPPORT_COMPOSITION ) && defined(PLATFORM_WINDOW)
#define PFD_SUPPORT_COMPOSITION 0x00008000
#endif

#define GCCALIGNED(size) __attribute__ ((aligned(size)))
#define VSALIGNED(size)

#if defined(ENABLE_SIMD)
#if defined(__SSE__)
#define SIMD_SSE
#endif
#if defined(__SSE2__)
#define SIMD_SSE2
#endif
#endif


#elif defined( _MSC_VER )
//types
#define cppTypeInfo type_info

#ifndef DISABLE_FORCE_INLINE
#define DFORCEINLINE __forceinline
#else
#define DFORCEINLINE inline
#endif
#define DINLINE __inline
#define ASPACKED( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
#define ALIGNED( size, __Declaration__ ) __declspec(align(size)) __Declaration__
#define COMPILER_VISUAL_STUDIO

#if _MSC_VER == 1600
#include <utility>
#define decltype(...) \
		  std::identity<decltype(__VA_ARGS__)>::type
#endif

#define GCCALIGNED(size)
#define VSALIGNED(size)  __declspec(align(size))

#if defined(ENABLE_SIMD)
#if  _M_IX86_FP>=1
#define SIMD_SSE
#endif
#if _M_IX86_FP>=2
#define SIMD_SSE2
#endif
#endif

#else
#error compiler not supported
#endif





#if defined(DCPP_11)
#include <unordered_map>
#include <functional>
#include <memory>
#define DFUNCTION	std::function
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
#define DFUNCTION	std::tr1::function
#define DUNORDERED_MAP    std::tr1::unordered_map
#define DUNORDERED_MULTIMAP    std::tr1::unordered_multimap
#define DHASH std::tr1::hash
#define DNOSTDHASH tr1::hash
#define DS_PTR std::tr1::shared_ptr
#define DW_PTR std::tr1::weak_ptr
#endif

#if defined( SIMD_SSE2 ) || defined( SIMD_SSE )
#include <emmintrin.h>
#include <xmmintrin.h>
#include <mmintrin.h>
#define DOVERRIDE_NEW_DEL
#include <Memory.h>
#endif

#endif
