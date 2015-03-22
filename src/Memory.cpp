//
//  Memory.cpp
//  RiemannSphere
//
//  Created by Gabriele Di Bari on 14/08/13.
//  Copyright (c) 2013 Gabriele Di Bari. All rights reserved.
//

#include <stdafx.h>
#include <Memory.h>
#include <Debug.h>
#define NOT(x) (!(x))
#define BYTESALING 16

#ifndef DOVERRIDE_NEW_DEL

void* Easy2D::malloc(size_t size)
{
	void *p = ::malloc( size );
	if NOT(p) Easy2D::Debug::message() << ("malloc failed !\n");
	return p;
}
void  Easy2D::free(void* p)
{
	::free( p );
}

#ifdef NEW_DELETE_OVERLOADABLE

#define  throw_alloc
#define  throw_dealloc

typedef void*(*costum_malloc)(size_t);
typedef void(*costum_free)(void*);
static bool not_costum_malloc = true;
static bool not_costum_free = true;
static costum_malloc __malloc = nullptr;
static costum_free __free = nullptr;

void Easy2D::setAllocatorFunctionPtr(costum_malloc cmalloc)
{
	::__malloc = cmalloc;
	not_costum_malloc = false;
}
void Easy2D::setDeallocatorFunctionPtr(costum_free cfree)
{
	::__free = cfree;
	not_costum_free = false;
}

void *operator new( size_t size ) throw_alloc
{
	void *p = nullptr;
	if (!not_costum_malloc && __malloc) p = __malloc(size);
	else p=::malloc(size);

	if NOT( p )
		Easy2D::Debug::message() << ("new failed !\n");

	return p;
}
 
//==================================================================
void *operator new [] ( size_t size ) throw_alloc
{
	void *p = nullptr;
	if (!not_costum_malloc && __malloc) p = __malloc(size);
	else p = ::malloc(size);

	if NOT( p )
		Easy2D::Debug::message()<<( "new failed !\n" );

	return p;
}

//==================================================================
void operator delete( void *p ) throw_dealloc
{
	if (!not_costum_free && __malloc){ if (p) __free(p); }
	else if (p) ::free(p);
	
}

//==================================================================
void operator delete [] ( void *p ) throw_dealloc
{
	if (!not_costum_free && __malloc){ if (p) __free(p); }
	else if (p) ::free(p);
}

#endif

#else

#ifdef _MSC_VER
    #include <malloc.h>
#elif defined(__MACH__)
    //#include <mm_malloc.h>
    #include <mm_malloc.h>
    #include <stdlib.h>
#else
    #include <mm_malloc.h>
    #include <stdlib.h>
#endif

void* Easy2D::malloc(size_t size)
{
	void *p = ::malloc(size);
	//void *p = _mm_malloc(size, BYTESALING);
	if NOT(p) Easy2D::Debug::message() << ("malloc failed !\n");
	return p;
}
void  Easy2D::free(void* p)
{
	::free(p);
	//_mm_free( p );
}

//==================================================================
void *operator new( size_t size ) throw_alloc
{
    void *p = _mm_malloc( size, BYTESALING );

    if NOT( p )
		Easy2D::Debug::message() << ("new failed !\n");

    return p;
}

//==================================================================
void *operator new [] ( size_t size ) throw_alloc
{
    void *p = _mm_malloc( size, BYTESALING );

    if NOT( p )
        Easy2D::Debug::message()<<( "new failed !\n" );

    return p;
}

//==================================================================
void operator delete( void *p ) throw_dealloc
{
    if ( p )
        _mm_free( p );
}

//==================================================================
void operator delete [] ( void *p ) throw_dealloc
{
    if ( p )
        _mm_free( p );
}

#endif
