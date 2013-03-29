#ifndef THREAD_H
#define THREAD_H

#include "ThreadConf.h"

class Thread{

	public:

	enum threadState{
		SUCCESSFUL=0,
		ERROR_CREATE,
		ERROR_JOIN
	};
	enum actionOnDelete{
		TERMINATE_NONE=0,
		TERMINATE_JOIN,
		TERMINATE_DELETE
	};
    typedef int (*threadFunction)(void*);

	Thread(actionOnDelete cad=TERMINATE_NONE);
	Thread(threadFunction fn,void* args=NULL,actionOnDelete cad=TERMINATE_NONE);
	virtual ~Thread();
	void yield();
	void sleepThread(unsigned int msec);
	bool start();
	int join();
	void destroy();
	int getError();
	virtual int run();

    protected:

    threadFunction thisThFun;
    void * thisThFunArgs;

	private:

	actionOnDelete cad;
	thrd_t thr;
	int th_error;
	THREAD_STATIC_METHOS

};

#if defined(__DEF_THREAD_WIN32)
	#include "ThreadWin32.inl"
#else
	#include "ThreadPOSIX.inl"
#endif


#endif
