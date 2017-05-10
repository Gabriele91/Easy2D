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

#define throw_alloc
#define throw_dealloc throw()

namespace Easy2D
{
    void* alignedAlloc(size_t size,size_t alignment);
    void  alignedFree(void*);
    
    void* malloc(size_t size);
    void  free(void*);
    
	template <class T>
	class Pointers //: protected std::enable_shared_from_this< T >
	{
	public:
		//define shared ptr
		typedef SPtr<T> ptr;
		//define weak ptr
		typedef WPtr<T> wptr;
		//define unique ptr
		typedef UPtr<T> uptr;
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

    template < const size_t Aligned = 16 >
    class AlignedAlloc
    {
    public:
        
        void* operator new( size_t size ) throw_alloc
        {
            return alignedAlloc(size,Aligned);
        }
        void* operator new( size_t size,  void* ptr ) throw_alloc
        {
            return ptr;
        }
        
        void* operator new [] ( size_t size ) throw_alloc
        {
            return alignedAlloc(size,Aligned);
        }
        void* operator new[]( size_t size, void* ptr ) throw_alloc
        {
            return ptr;
        }
        
        void  operator delete ( void *p ) throw_dealloc
        {
            alignedFree(p);
        }
        void  operator delete [] ( void *p ) throw_dealloc
        {
            alignedFree(p);
        }
    };
    
    
	#ifdef NEW_DELETE_OVERLOADABLE
	    void setAllocatorFunctionPtr(void*(*)(size_t));
	    void setDeallocatorFunctionPtr(void(*)(void*));
	#endif
};


#if defined(DOVERRIDE_NEW_DEL)

void *operator new( size_t size ) throw_alloc ;
void *operator new [] ( size_t size ) throw_alloc ;
void operator delete ( void *p ) throw_dealloc ;
void operator delete [] ( void *p ) throw_dealloc ;

#endif

#endif
