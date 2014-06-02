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

void *operator new( size_t size );
void *operator new [] ( size_t size );
void operator delete ( void *p );
void operator delete [] ( void *p );

#endif

#endif
