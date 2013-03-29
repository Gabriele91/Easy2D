#ifndef DEBUG_H
#define DEBUG_H

#include <Config.h>
#include <ostream>

namespace Easy2D {

	class Debug{	
	public:
		
		static void breakpoint();
		static std::ostream& message();
		static void doassert(int v,const char* value,const char* fileSource,int line);

	};

#ifdef _DEBUG

	#define DEBUG_MESSAGE( x ) Debug::message()<<x<<"\n"
	#define DEBUG_ASSERT( x ) Debug::doassert((int)(x),#x,__FILE__,__LINE__)
	#define DEBUG_ASSERT_MGS( x,y ) Debug::message()<<y<<"\n"; Debug::doassert((int)(x),#x,__FILE__,__LINE__)
	#define DEBUG_ASSERT_REPLACE( x ) DEBUG_ASSERT(x)
	#define DEBUG_ASSERT_MGS_REPLACE( x,y ) DEBUG_ASSERT_MGS(x,y)
	#define DEBUG_BREAKPOINT() Debug::breakpoint()

#else
	
	#define DEBUG_MESSAGE( x )
	#define DEBUG_ASSERT( x )
	#define DEBUG_ASSERT_MGS( x,y )
	#define DEBUG_ASSERT_MGS_REPLACE( x,y ) x
	#define DEBUG_ASSERT_REPLACE( x ) x
	#define DEBUG_BREAKPOINT()

#endif


};

#endif