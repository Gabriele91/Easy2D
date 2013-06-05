#include <stdafx.h>
#include <ETime.h>
#ifdef PLATFORM_UNIX
    #include <sys/time.h>
#endif
//cpu Ticks
extern Easy2D::bit64 Easy2D::GetTimeTicks(){
	Easy2D::bit64 val;
	#ifdef PLATFORM_UNIX
		timeval timeVal;

		gettimeofday( &timeVal, NULL );

		val = (Easy2D::bit64)timeVal.tv_sec * (1000*1000) + (Easy2D::bit64)timeVal.tv_usec;
	#else
		QueryPerformanceCounter( (LARGE_INTEGER *)&val );
	#endif
	return val;
}
//time
extern double Easy2D::GetTime(){
	static double	coe;
#ifdef PLATFORM_UNIX
	coe = 1.0 / 1000.0;
#else
	static Easy2D::bit64 freq;

	if ( freq == 0 )
	{
		QueryPerformanceFrequency( (LARGE_INTEGER *)&freq );
		coe = 1000.0 / freq;
	}
#endif

	return GetTimeTicks() * coe;
}
