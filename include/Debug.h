#ifndef DEBUG_H
#define DEBUG_H

#include <Config.h>
#include <ostream>

namespace Easy2D {

	class Debug{
	public:

		static void breakpoint();
		static std::ostream& message();
		template<typename T>
		static inline void doassert(T v,
                             const char* value,
                             const char* fileSource,
                             int line){
            if(!v){
                message()<<"Assert : "<<value<<" : "<<line<<" : "<<fileSource<<"\n";
                breakpoint();
            }
        }
		static void gpucheckerrors(const char* fileSource,int line);

	};

#ifdef _DEBUG

	#define DEBUG_MESSAGE( x ) Debug::message()<<x<<"\n"
	#define DEBUG_ASSERT( x ) Debug::doassert((x),#x,__FILE__,__LINE__)
	#define DEBUG_ASSERT_MSG( x,y ) if(!(x)){Debug::message()<<y<<"\n";} Debug::doassert((x),#x,__FILE__,__LINE__)
	#define DEBUG_ASSERT_REPLACE( x ) DEBUG_ASSERT(x)
	#define DEBUG_ASSERT_MGS_REPLACE( x,y ) DEBUG_ASSERT_MSG(x,y)
	#define DEBUG_BREAKPOINT() Debug::breakpoint()
	#define CHECK_GPU_ERRORS() Debug::gpucheckerrors(__FILE__,__LINE__)

#else

	#define DEBUG_MESSAGE( x )
	#define DEBUG_ASSERT( x )
	#define DEBUG_ASSERT_MSG( x,y )
	#define DEBUG_ASSERT_MGS_REPLACE( x,y ) x
	#define DEBUG_ASSERT_REPLACE( x ) x
	#define DEBUG_BREAKPOINT()
	#define CHECK_GPU_ERRORS()

#endif


};

#endif
