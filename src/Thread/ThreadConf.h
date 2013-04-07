#ifndef THREADCONF_H
#define THREADCONF_H

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) || defined(WIN32)
    
	#define __DEF_THREAD_WIN32
    #include <windows.h>
	#include <process.h>
    typedef HANDLE thrd_t;

	#define THREAD_STATIC_METHOS\
	static unsigned WINAPI _thrd_wrapper_function(void * aArg);

#else

    #define __DEF_THREAD_POSIX
    #include <pthread.h>
    typedef pthread_t thrd_t;

	#define THREAD_STATIC_METHOS\	
	static void thread_exit_handler(int sig);\
	static void * _thrd_wrapper_function(void * aArg);

#endif

#endif