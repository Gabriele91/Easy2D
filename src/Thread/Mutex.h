#ifndef MUTEX_H
#define MUTEX_H

#include "MutexConf.h"

class Mutex{

public:

	Mutex();
	virtual ~Mutex();
	bool lock(bool block=true);
	void unlock();

private:

	mxrd_t mutex;

};

#if defined(__DEF_MUTEX_WIN32)
	#include "MutexWin32.inl"
#else
	#include "MutexPOSIX.inl"
#endif
#endif