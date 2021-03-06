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
#define DISABLE_MIDMAP //ALPHA COVERAGE BUGS

#include <stdint.h>
#include <android/log.h>
#include <android/native_activity.h>
#include <android_native_app_glue.h>
#include <android/OpenGLAndroid.h>
#include <AL/al.h>
#include <AL/alc.h>

#elif defined(_WIN32)
#define PLATFORM_WINDOW
#define DCPP_11
#ifndef NOMINMAX 
    #define NOMINMAX 
#endif
#define _WINSOCKAPI_
#include <windows.h>
#include <AL/al.h>
#include <AL/alc.h>
#ifdef ES2_ANGLEPROJECT
	#define DISABLE_MIDMAP
	#include <angleproject/OpenGLAngle.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <window/OpenGLWindow.h>
#endif
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#define PLATFORM_UNIX

#if TARGET_OS_IPHONE
    #define PLATFORM_IPHONE
    #define PLATFORM_IOS

    //#define DISABLE_STREAM_BUFFER
    //#define DISABLE_VAOS
    //#define OPENGL_ES
    #define DISABLE_MIDMAP //ALPHA COVERAGE BUGS

    #include <OpenAL/al.h>
    #include <OpenAL/alc.h>
    #define DCPP_11
    #define PLATFORM_UNIX
    #include <OpenGLiOS.h>

#elif TARGET_IPHONE_SIMULATOR
    #define PLATFORM_IPHONE_SIMULATOR
    #define PLATFORM_IOS

    //#define DISABLE_STREAM_BUFFER
    //#define DISABLE_VAOS
    //#define OPENGL_ES
    #define DISABLE_MIDMAP //ALPHA COVERAGE BUGS

    #include <OpenAL/al.h>
    #include <OpenAL/alc.h>
    #define DCPP_11
    #define PLATFORM_UNIX
    #include <OpenGLiOS.h>

    //no sse in emulator
    #ifdef ENABLE_SIMD
        #undef  ENABLE_SIMD
        #define DISABLE_SIMD
        #define DOVERRIDE_NEW_DEL
    #endif

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

//no sse in arm
#if defined( ENABLE_SIMD )  && defined( __arm__ )
    #undef ENABLE_SIMD
    #define DISABLE_SIMD
    #define DOVERRIDE_NEW_DEL
    //NEON OR VFP
    #ifdef __ARM_NEON__
        #define ENABLE_NEON
    #else
        #define ENABLE_VFP
    #endif
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

#if !defined(ENABLE_SIMD) &&  !defined(DISABLE_SIMD)
#error "must to be define ENABLE_SIMD or DISABLE_SIMD"
#elif defined(ENABLE_SIMD) &&  defined(DISABLE_SIMD)
#error "must to be defined only ENABLE_SIMD or only DISABLE_SIMD"
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

#if ( (__GNUC__==4) && (__GNUC_MINOR__ >=8) || (__GNUC__>=5)  || defined(__ANDROID__) ) || defined(__llvm__)
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

#if defined(EASY2DLIBRARY_EXPORT) // inside DLL
#   define E2DDLL  __declspec(dllexport)
#elif defined(EASY2DLIBRARY_IMPORT)  // outside DLL
#   define E2DDLL  __declspec(dllimport)
#else //static libary
#   define E2DDLL
#endif

#if _MSC_VER == 1600
#include <utility>
#define decltype(...) \
		  std::identity<decltype(__VA_ARGS__)>::type
#endif
#if _MSC_VER < 1800
namespace std
{
	inline bool signbit(double num) { return _copysign(1.0, num) < 0; }
}
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

#if defined(_WIN64)
	#define FORCE_UNDEFINED_SYMBOL(s) __pragma(comment (linker, "/include:"#s))
#elif defined(_WIN32)
	#define FORCE_UNDEFINED_SYMBOL(s) __pragma(comment (linker, "/include:_"#s))
#endif

#if defined(EASY2DLIBRARY_EXPORT) // inside DLL
#   define E2DDLL  __declspec(dllexport)
#elif defined(EASY2DLIBRARY_IMPORT)  // outside DLL
#   define E2DDLL  __declspec(dllimport)
#else //static libary
#   define E2DDLL
#endif

#else
#error compiler not supported
#endif





#if defined(DCPP_11)
#include <unordered_map>
#include <functional>
#include <memory>
namespace Easy2D
{
    //Function pointer alias
    template < typename F >
    using Function = std::function< F >;
    //Map Alias
    template < class Key,
               class T,
               class Hash = std::hash<Key>,
               class KeyEqual = std::equal_to<Key> >
    using UnorderedMap  = std::unordered_map< Key, T, Hash, KeyEqual >;
    //Multi Map alias
    template < class Key,
               class T,
               class Hash = std::hash<Key>,
               class KeyEqual = std::equal_to<Key> >
    using UnorderedMultiMap = std::unordered_multimap< Key, T, Hash, KeyEqual >;
    //Pointer alias
    template < typename T > using SPtr = std::shared_ptr< T >;
    template < typename T > using WPtr = std::weak_ptr< T >;
    template < typename T > using UPtr = std::unique_ptr< T >;
}

#elif defined(DCPP_X0)
#include <tr1/unordered_map>
#include <tr1/functional>
#include <tr1/memory>
namespace Easy2D
{
    //Function pointer alias
    template < typename F >
    using Function = std::tr1::function< F >;
    //Map Alias
    template < class Key,
    class T,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>,
    class Allocator = std::allocator< std::pair<const Key, T> >
    using UnorderedMap  = std::tr1::unordered_map< Key, T, Hash, KeyEqual, Allocator >;
    //Multi Map alias
    template < class Key,
    class T,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>,
    class Allocator = std::allocator< std::pair<const Key, T> >
    using UnorderedMultiMap = std::tr1::unordered_multimap< Key, T, Hash, KeyEqual, Allocator >;
    //Pointer alias
    template < typename T > using SPtr = std::tr1::shared_ptr< T >;
    template < typename T > using WPtr = std::tr1::weak_ptr< T >;
    template < typename T > using UPtr = std::tr1::unique_ptr< T >;
}
#endif

#if defined( SIMD_SSE2 ) || defined( SIMD_SSE )
#include <emmintrin.h>
#include <xmmintrin.h>
#include <mmintrin.h>
/* #define DOVERRIDE_NEW_DEL */
#endif
////////////////////////////////////////////////////////
#include <MemoryAllocator.h>
////////////////////////////////////////////////////////
namespace Easy2D
{
    int atexit(void (*function)(void));
};
////////////////////////////////////////////////////////
//enable lua jit or lua
#if !defined(USE_LUA) &&  !defined(USE_LUA_JIT)
    #error "must to be define USE_LUA or USE_LUA_JIT"
#elif defined(USE_LUA) &&  defined(USE_LUA_JIT)
    #error "must to be defined only USE_LUA or only USE_LUA_JIT"
#endif

#if !defined(USE_LUA) || !defined(USE_LUA_JIT)
    #include <Easy2DLua.h>
#endif

#endif
