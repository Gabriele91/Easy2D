#include "Math_extends_vfp.h"
#define NO_THUMB
/*MACRO ASM*/
#ifndef NO_THUMB
#warning "Compiling in Thumb Mode. Mode switches activated."
#else
#warning "Compiling in ARM mode. Mode switches deactivated."
#endif

// Switches to from THUMB to ARM mode.
#ifndef NO_THUMB
  #define VFP_SWITCH_TO_ARM ".align 4               \n\t" \
                            "mov     r0, pc         \n\t" \
                            "bx      r0             \n\t" \
                            ".arm                   \n\t"
#else
  #define VFP_SWITCH_TO_ARM
#endif

// Switches from ARM to THUMB mode.
#ifndef NO_THUMB
  #define VFP_SWITCH_TO_THUMB "add     r0, pc, #1     \n\t" \
                              "bx      r0             \n\t" \
                              ".thumb                 \n\t"
#else
  #define VFP_SWITCH_TO_THUMB
#endif

// Sets length and stride to 0.
#define VFP_VECTOR_LENGTH_ZERO "fmrx    r0, fpscr            \n\t" \
                               "bic     r0, r0, #0x00370000  \n\t" \
                               "fmxr    fpscr, r0            \n\t"

// Set vector length. VEC_LENGTH has to be bitween 0 for length 1 and 3 for length 4.
#define VFP_VECTOR_LENGTH(VEC_LENGTH) "fmrx    r0, fpscr                         \n\t" \
                                      "bic     r0, r0, #0x00370000               \n\t" \
                                      "orr     r0, r0, #0x000" #VEC_LENGTH "0000 \n\t" \
                                      "fmxr    fpscr, r0                         \n\t"


// Multiplies two 4x4 matrices.
inline void Matrix4Mul(const float* src_mat_1, const float* src_mat_2, float* dst_mat) {
  asm volatile (VFP_SWITCH_TO_ARM
                VFP_VECTOR_LENGTH(3)

                // Interleaving loads and adds/muls for faster calculation.
                // Let A:=src_ptr_1, B:=src_ptr_2, then
                // function computes A*B as (B^T * A^T)^T.

                // Load the whole matrix into memory.
                "fldmias  %1, {s8-s23}    \n\t"
                // Load first column to scalar bank.
                "fldmias  %2!, {s0-s3}    \n\t"
                // First column times matrix.
                "fmuls s24, s8, s0        \n\t"
                "fmacs s24, s12, s1       \n\t"

                // Load second column to scalar bank.
                "fldmias %2!,  {s4-s7}    \n\t"

                "fmacs s24, s16, s2       \n\t"
                "fmacs s24, s20, s3       \n\t"
                // Save first column.
                "fstmias  %0!, {s24-s27}  \n\t"

                 // Second column times matrix.
                "fmuls s28, s8, s4        \n\t"
                "fmacs s28, s12, s5       \n\t"

                // Load third column to scalar bank.
                "fldmias  %2!, {s0-s3}    \n\t"

                "fmacs s28, s16, s6       \n\t"
                "fmacs s28, s20, s7       \n\t"
                // Save second column.
                "fstmias  %0!, {s28-s31}  \n\t"

                // Third column times matrix.
                "fmuls s24, s8, s0        \n\t"
                "fmacs s24, s12, s1       \n\t"

                // Load fourth column to scalar bank.
                "fldmias %2,  {s4-s7}    \n\t"

                "fmacs s24, s16, s2       \n\t"
                "fmacs s24, s20, s3       \n\t"
                 // Save third column.
                "fstmias  %0!, {s24-s27}  \n\t"

                // Fourth column times matrix.
                "fmuls s28, s8, s4        \n\t"
                "fmacs s28, s12, s5       \n\t"
                "fmacs s28, s16, s6       \n\t"
                "fmacs s28, s20, s7       \n\t"
                // Save fourth column.
                "fstmias  %0!, {s28-s31}  \n\t"

                VFP_VECTOR_LENGTH_ZERO
                VFP_SWITCH_TO_THUMB
                : "=r" (dst_mat), "=r" (src_mat_2)
                : "r" (src_mat_1), "0" (dst_mat), "1" (src_mat_2)
                : "r0", "cc", "memory", VFP_CLOBBER_S0_S31
                );
}
// Multiplies a 4x4 matrix with a 4-dim. vector.
inline void Matrix4Vector4Mul(const float* src_mat, const float* src_vec, float* dst_vec) {
  asm volatile (VFP_SWITCH_TO_ARM

                // Load the whole matrix.
                "fldmias  %0, {s8-s23}     \n\t"
                // Load vector to scalar bank.
                "fldmias  %1, {s0-s3}      \n\t"

                VFP_VECTOR_LENGTH(3)

                // First column times matrix.
                "fmuls s24, s8, s0        \n\t"
                "fmacs s24, s12, s1       \n\t"
                "fmacs s24, s16, s2       \n\t"
                "fmacs s24, s20, s3       \n\t"

                // Save vector.
                "fstmias  %2, {s24-s27}  \n\t"

                VFP_VECTOR_LENGTH_ZERO
                VFP_SWITCH_TO_THUMB
                :
                : "r" (src_mat), "r" (src_vec), "r" (dst_vec)
                : "r0", "cc", "memory", VFP_CLOBBER_S0_S3, VFP_CLOBBER_S8_S27
                );
}

// s0  s1  s2  s3
// s4  s5  s6  s7
// s8  s9  s10 s11
// s12 s13 s14 s15

// Load the matrix transposed into memory.
// Uses registers 24 - 31 as scratch.
// Increments sources address in register %0.
#define VFP_LOAD_MATRIX0_T_TO_S0_S15 "fldmias  %0!, {s0-s3}     \n\t" \
                                     "fcpys s4, s1              \n\t" \
                                     "fcpys s8, s2              \n\t" \
                                     "fldmias  %0!, {s24-s27}   \n\t" \
                                     "fcpys s12, s3             \n\t" \
                                     \
                                     "fcpys s1,  s24            \n\t" \
                                     "fcpys s5,  s25            \n\t" \
                                     "fldmias  %0!, {s28-s31}   \n\t" \
                                     "fcpys s9, s26             \n\t" \
                                     "fcpys s13, s27            \n\t" \
                                     \
                                     "fcpys s2,  s28            \n\t" \
                                     "fcpys s6, s29             \n\t" \
                                     "fldmias  %0!, {s24-s27}   \n\t" \
                                     "fcpys s10, s30            \n\t" \
                                     "fcpys s14, s31            \n\t" \
                                     \
                                     "fcpys s3, s24             \n\t" \
                                     "fcpys s7, s25             \n\t" \
                                     "fcpys s11, s26            \n\t" \
                                     "fcpys s15, s27            \n\t"
// Computes the inverse of a 4x4 matrix by using the Laplace Extension Theorem
// in 94 multiplications and 1 division.
// Comments in the code refer to David Eberly's definitions
// http://www.geometrictools.com/Documentation/LaplaceExpansionTheorem.pdf
// page 9 (retrieved Feb. 20, 2009).
inline void Matrix4Invert(const float* src_mat, float* dst_mat) {

  const float one = 1.0;

  asm volatile (VFP_SWITCH_TO_ARM
                // Load the whole matrix transposed.
                // Transposing is necessary to be able to execute inversion
                // with only 32 fpregs.
                VFP_LOAD_MATRIX0_T_TO_S0_S15

                // Destination matrix will be {s8 - s23}.

                // Mapping of matrix elements is
                // s0  s1  s2  s3      a00 a01 a02 a03
                // s4  s5  s6  s7  <=  a10 a11 a12 a13
                // s8  s9  s10 s11     a20 a21 a22 a23
                // s12 s13 s14 s15     a30 a31 a32 a33

                // Compute s_0 to s_5 (in s25 - s30).
                "fmuls  s25, s0, s5        \n\t"
                "fnmacs s25, s4, s1        \n\t"

                "fmuls  s26, s0, s6        \n\t"
                "fnmacs s26, s4, s2        \n\t"

                "fmuls  s27, s0, s7        \n\t"
                "fnmacs s27, s4, s3        \n\t"

                "fmuls  s28, s1, s6        \n\t"
                "fnmacs s28, s5, s2        \n\t"

                "fmuls  s29, s1, s7        \n\t"
                "fnmacs s29, s5, s3        \n\t"

                "fmuls  s30, s2, s7        \n\t"
                "fnmacs s30, s6, s3        \n\t"

                // Process last two columns -> s16-s23.
                // Last column.
                "fnmuls s20, s9, s30       \n\t"
                "fmacs  s20, s10, s29      \n\t"
                "fnmacs s20, s11, s28      \n\t"

                "fmuls s21, s8, s30        \n\t"
                "fnmacs s21, s10, s27      \n\t"
                "fmacs s21, s11, s26       \n\t"

                "fnmuls s22, s8, s29       \n\t"
                "fmacs  s22, s9, s27       \n\t"
                "fnmacs s22, s11, s25      \n\t"

                "fmuls s23, s8, s28        \n\t"
                "fnmacs s23, s9, s26       \n\t"
                "fmacs s23, s10, s25       \n\t"

                // Third column.
                "fmuls s16, s13, s30       \n\t"
                "fnmacs s16, s14, s29      \n\t"
                "fmacs s16, s15, s28       \n\t"

                "fnmuls s17, s12, s30      \n\t"
                "fmacs  s17, s14, s27      \n\t"
                "fnmacs s17, s15, s26      \n\t"

                "fmuls s18, s12, s29       \n\t"
                "fnmacs s18, s13, s27      \n\t"
                "fmacs s18, s15, s25       \n\t"

                "fnmuls s19, s12, s28      \n\t"
                "fmacs  s19, s13, s26      \n\t"
                "fnmacs s19, s14, s25      \n\t"

                // Compute c_5 to c_0 (in s24 - s29).
                // Determinant will be in s31.
                "fmuls  s24, s10, s15       \n\t"
                "fnmacs s24, s14, s11       \n\t"
                "fmuls  s31, s24, s25       \n\t"

                "fmuls  s25, s9, s15        \n\t"
                "fnmacs s25, s13, s11       \n\t"
                "fnmacs s31, s25, s26       \n\t"

                "fmuls  s26, s9, s14        \n\t"
                "fnmacs s26, s13, s10       \n\t"
                "fmacs s31, s26, s27        \n\t"

                "fmuls  s27, s8, s15        \n\t"
                "fnmacs s27, s12, s11       \n\t"
                "fmacs  s31, s27, s28       \n\t"

                "fmuls  s28, s8, s14        \n\t"
                "fnmacs s28, s12, s10       \n\t"
                "fnmacs s31, s28, s29       \n\t"

                "fmuls  s29, s8, s13        \n\t"
                "fnmacs s29, s12, s9        \n\t"
                "fmacs  s31, s29, s30       \n\t"

                // Second column.
                "fnmuls s12, s1, s24       \n\t"
                "fmacs  s12, s2, s25       \n\t"
                "fnmacs s12, s3, s26       \n\t"

                "fmuls  s13, s0, s24       \n\t"
                "fnmacs s13, s2, s27       \n\t"
                "fmacs  s13, s3, s28       \n\t"

                "fnmuls s14, s0, s25       \n\t"
                "fmacs  s14, s1, s27       \n\t"
                "fnmacs s14, s3, s29       \n\t"

                "fmuls  s15, s0, s26       \n\t"
                "fnmacs s15, s1, s28       \n\t"
                "fmacs  s15, s2, s29       \n\t"

                // First column.
                "fmuls  s8, s5, s24        \n\t"
                "fnmacs s8, s6, s25        \n\t"
                "fmacs  s8, s7, s26        \n\t"

                "fnmuls s9, s4, s24        \n\t"
                "fmacs  s9, s6, s27        \n\t"
                "fnmacs s9, s7, s28        \n\t"

                "fmuls  s10, s4, s25       \n\t"
                "fnmacs s10, s5, s27       \n\t"
                "fmacs  s10, s7, s29       \n\t"

                "fnmuls s11, s4, s26       \n\t"
                "fmacs  s11, s5, s28       \n\t"
                "fnmacs s11, s6, s29       \n\t"

                // Multiply with inverse of determinant.
                "fmsr s1, %2               \n\t"
                "fdivs s0, s1, s31         \n\t"

                VFP_VECTOR_LENGTH(7)

                // Remember: destination matrix is in {s8 - s23}
                "fmuls s24, s8, s0        \n\t"
                "fstmias  %1!, {s24-s31}  \n\t"
                "fmuls s8, s16, s0        \n\t"
                "fstmias  %1, {s8-s15}    \n\t"

                VFP_VECTOR_LENGTH(0)

                VFP_SWITCH_TO_THUMB
                : "=r" (src_mat), "=r" (dst_mat)
                : "r" (one), "0" (src_mat), "1" (dst_mat)
                : "r0", "cc", "memory", VFP_CLOBBER_S0_S31
                );

}

