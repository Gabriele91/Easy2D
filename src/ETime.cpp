#include <stdafx.h>
#include <ETime.h>
//cpu Ticks
extern Easy2D::bit64 Easy2D::GetTimeTicks(){
	Easy2D::bit64 val;
	#if defined(_MSC_VER)
		QueryPerformanceCounter( (LARGE_INTEGER *)&val );
	#else
		timeval timeVal;

		gettimeofday( &timeVal, NULL );

		val = (I64)timeVal.tv_sec * (1000*1000) + (I64)timeVal.tv_usec;
	#endif
	return val;
}
//time
extern double Easy2D::GetTime(){
	static double	coe;

#if defined(_MSC_VER)
	static Easy2D::bit64 freq;

	if ( freq == 0 )
	{
		QueryPerformanceFrequency( (LARGE_INTEGER *)&freq );
		coe = 1000.0 / freq;
	}

#else
	coe = 1.0 / 1000.0;

#endif

	return GetTimeTicks() * coe;
}