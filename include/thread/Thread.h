#ifndef THREAD_H
#define THREAD_H

#include <Config.h>
#include <ThreadConf.h>

namespace Easy2D{
    
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
	
	typedef DFUNCTION<int(void*)> threadFunction;

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
	
	void setArgs(void* args){
		 thisThFunArgs=args;
	}
	void setFunction(threadFunction fn){
		 thisThFun=fn;
	}

    protected:
		
    void * thisThFunArgs;
    threadFunction thisThFun;

	private:

	actionOnDelete cad;
	thrd_t thr;
	int th_error;
	THREAD_STATIC_METHOS

};

}

#endif
