//
//  Memory.h
//  RiemannSphere
//
//  Created by Gabriele Di Bari on 14/08/13.
//  Copyright (c) 2013 Gabriele Di Bari. All rights reserved.
//

#ifndef MEMORY_H
#define MEMORY_H

#include <Config.h>

#if defined(DOVERRIDE_NEW_DEL)
#define throw_alloc
#define throw_dealloc throw()

void *operator new( size_t size ) throw_alloc ;
void *operator new [] ( size_t size ) throw_alloc ;
void operator delete ( void *p ) throw_dealloc ;
void operator delete [] ( void *p ) throw_dealloc ;

#endif

#endif
