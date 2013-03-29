
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

bool Thread::start(){
		if(pthread_create(&thr,NULL,
						  _thrd_wrapper_function,
						  (void *)this) != 0)
						  thr = NULL;
		if(!thr)
			th_error=ERROR_CREATE;

		th_error=SUCCESSFUL;

		return thr!=0;
	}

void Thread::yield(){
		  sched_yield();
	}

void Thread::sleepThread(unsigned int msec){
		usleep(msec * 1000);
	}

void Thread::destroy(){
		int status=0;
		if ( (status = pthread_kill(thr, SIGUSR1)) != 0){
			//error
			#ifndef __ANDROID__
				 pthread_cancel(thr);
			#endif
		}
		pthread_join(thr, 0);

		thr=NULL;
	}

int Thread::join(){
	  void *pres;
	  int ires = 0;
	  if (pthread_join(thr, &pres) != 0){
		this->th_error=ERROR_JOIN;
		return -1;
	  }
	  if (pres != NULL){
		ires = *(int*)pres;
		free(pres);
	  }
	  thr=NULL;
	  return (int)ires;
	}

int Thread::getError(){
		return th_error;
	}

virtual int Thread::run(){
		if(thisThFun)
			return thisThFun(thisThFunArgs);
		else
			return 0;
	}

static void Thread::thread_exit_handler(int sig){
		pthread_exit(0);
	}
static void * Thread::_thrd_wrapper_function(void * aArg){

	  int  res;
	  void *pres;
	  ///SET OUT SIGNAL
	  struct sigaction actions;
	  memset(&actions, 0, sizeof(actions));
	  sigemptyset(&actions.sa_mask);
	  actions.sa_flags = 0;
	  actions.sa_handler = thread_exit_handler;
	  int rc = sigaction(SIGUSR1,&actions,NULL);
	  ///
	  Thread *_this = (Thread *) aArg;
	  res=_this->run();

	  pres = malloc(sizeof(int));
	  if (pres != NULL)
	  {
		*(int*)pres = res;
	  }

	  return pres;

	}
