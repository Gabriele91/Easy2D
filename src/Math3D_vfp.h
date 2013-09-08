#ifndef MATH_2D_VFP
#define MATH_2D_VFP

#if (TARGET_IPHONE_SIMULATOR == 0) && (TARGET_OS_IPHONE == 1)
	// Multiplies two 4x4 matrices.
	extern inline void Matrix4Mul(const float* src_mat_1, const float* src_mat_2, float* dst_ptr);
	// Multiplies a 4x4 matrix with a 4-dim. vector.
	extern inline void Matrix4Vector4Mul(const float* src_mat, const float* src_vec, float* dst_vec);

	// Inverts a 4x4 Matrix with 94 multiplications and one division.
	// This is not the fastest possible implementation (60 mult + 2 divisions) but it is
	// not dependent on the determinants of submatrices.
	// Furthermore on iPhone, division has IPC of 15 vs. 1 IPC for multiplication.
	extern inline void Matrix4Invert(const float* src_mat, float* dst_mat);
#endif

#endif

