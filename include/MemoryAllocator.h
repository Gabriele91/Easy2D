//
//  Memory.h
//  RiemannSphere
//
//  Created by Gabriele Di Bari on 14/08/13.
//  Copyright (c) 2013 Gabriele Di Bari. All rights reserved.
//

#ifndef MEMORYALLOCATOR_H
#define MEMORYALLOCATOR_H

#include <Config.h>

namespace Easy2D
{
	template <class T>
	class Pointers //: protected std::enable_shared_from_this< T >
	{
	public:
		//define shared ptr
		typedef DS_PTR<T> ptr;
		//define weak ptr
		typedef DW_PTR<T> wptr;
		//define unique ptr
		typedef DU_PTR<T> uptr;
		//return shared 
		template< class... Args >
		static inline ptr snew(Args ...args)
		{
			return std::make_shared<T>(args...);
		}
		//return unique 
		template< class... Args >
		static inline uptr unew(Args ...args)
		{
			return std::make_unique<T>(args...);
		}
	};

	void* malloc(size_t size);
	void  free(void*); 
	#ifdef NEW_DELETE_OVERLOADABLE
	    void setAllocatorFunctionPtr(void*(*)(size_t));
	    void setDeallocatorFunctionPtr(void(*)(void*));
	#endif
};


#if defined(DOVERRIDE_NEW_DEL)
#define throw_alloc
#define throw_dealloc throw()

void *operator new( size_t size ) throw_alloc ;
void *operator new [] ( size_t size ) throw_alloc ;
void operator delete ( void *p ) throw_dealloc ;
void operator delete [] ( void *p ) throw_dealloc ;

#endif

#endif
