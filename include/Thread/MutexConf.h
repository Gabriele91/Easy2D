#ifndef MUTEXCONF_H
#define MUTEXCONF_H

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) || defined(WIN32)
	#define __DEF_MUTEX_WIN32
    #include <windows.h>
    typedef HANDLE mxrd_t;
#else
    #define __DEF_MUTEX_POSIX
    #include <pthread.h>
	typedef pthread_mutex_t mxrd_t;
#endif


#endif
