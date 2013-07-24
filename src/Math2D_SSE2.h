#ifndef MATH_2D_SSE2
#define MATH_2D_SSE2
#include <Config.h>
#include <Math2D.h>

#if defined( SIMD_SSE2 )
	namespace Easy2D {
		// Multiplies two 4x4 matrices (a,b) a*=b
		extern DFORCEINLINE void SSE2_Matrix4Mul(Matrix4x4& self,const Matrix4x4 &m4x4);
		extern DFORCEINLINE void SSE2_Matrix4Inv(Matrix4x4& self);
	}
#endif

#endif