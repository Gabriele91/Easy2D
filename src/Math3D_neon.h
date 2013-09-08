#ifndef MATH_2D_NEON
#define MATH_2D_NEON

#if (TARGET_IPHONE_SIMULATOR == 0) && (TARGET_OS_IPHONE == 1)
	#ifdef _ARM_ARCH_7
		// Multiplies two 4x4 matrices (a,b) outputing a 4x4 matrix (output)
		extern inline void NEON_Matrix4Mul(const float* a, const float* b, float* output );
		// Multiplies a 4x4 matrix (m) with a vector 4 (v), outputing a vector 4
		extern inline void NEON_Matrix4Vector4Mul(const float* m, const float* v, float* output);
	#endif
#endif

#endif

