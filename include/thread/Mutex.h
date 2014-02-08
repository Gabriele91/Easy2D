#ifndef MUTEX_H
#define MUTEX_H

#include <Config.h>
#include <MutexConf.h>

namespace Easy2D{
    
class Mutex{

public:

	Mutex();
	virtual ~Mutex();
	bool lock(bool block=true);
	void unlock();

private:

	mxrd_t mutex;

};
    
}

#endif