
Thread::Thread(actionOnDelete cad)
		   :thisThFun(NULL)
		   ,thisThFunArgs(NULL)
		   ,cad(cad)
		   ,thr(NULL)
		   ,th_error(SUCCESSFUL){}
Thread::Thread(threadFunction fn,
			   void* args,
			   actionOnDelete cad)
			   :thisThFun(fn)
			   ,thisThFunArgs(args)
			   ,cad(cad)
			   ,thr(NULL)
			   ,th_error(SUCCESSFUL){}

Thread::~Thread(){
		if(thr){
			switch(cad){
				case TERMINATE_JOIN: join(); break;
				case TERMINATE_DELETE: destroy(); break;
				default: break;
			}
		}
	}
bool Thread::start(){
		thr =  (HANDLE)_beginthreadex(NULL,0,
									  _thrd_wrapper_function,
									  (void*)this,
									   0,NULL);

		if(!thr)
			th_error=ERROR_CREATE;

		th_error=SUCCESSFUL;

		return thr!=0;
	}
void Thread::yield(){
		  Sleep(0);
	}
void Thread::sleepThread(unsigned int msec){
		Sleep(msec);
	}
void Thread::destroy(){
		TerminateThread((HANDLE)thr, 0);		
		thr=NULL;
	}
int Thread::join(){
	  if (WaitForSingleObject(thr, INFINITE) == WAIT_FAILED){
		this->th_error=ERROR_JOIN;
		return -1;
	  }
	  DWORD dwRes;
	  GetExitCodeThread(thr, &dwRes);
	  thr=NULL;
	  return (int)dwRes;
	}
int Thread::getError(){
		return th_error;
	}
int Thread::run(){
		if(thisThFun)
			return thisThFun(thisThFunArgs);
		else
			return 0;
	}
unsigned WINAPI Thread::_thrd_wrapper_function(void * aArg){
	  Thread *_this = (Thread *) aArg;
	  return _this->run();
	}