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

#if defined(DOVERRIDE_NEW_DEL)

#ifdef _MSC_VER
#include <malloc.h>
#elif defined(__MACH__)
//#include <mm_malloc.h>
#else
#include <mm_malloc.h>
#endif


using namespace Easy2D;

//==================================================================
void *operator new( size_t size ) throw_alloc
{
    void *p = _mm_malloc( size, BYTESALING );

    if NOT( p )
        Debug::message()<<( "new failed !\n" );

    return p;
}

//==================================================================
void *operator new [] ( size_t size ) throw_alloc
{
    void *p = _mm_malloc( size, BYTESALING );

    if NOT( p )
        Debug::message()<<( "new failed !\n" );

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
