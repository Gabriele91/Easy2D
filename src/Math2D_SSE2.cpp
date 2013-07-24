#include <stdafx.h>
#include "Math2D_SSE2.h"

#if defined( SIMD_SSE2 )
DFORCEINLINE void Easy2D::SSE2_Matrix4Mul(Easy2D::Matrix4x4& self,const Easy2D::Matrix4x4 &m4x4){
		 const __m128 a = m4x4.row0;
		 const __m128 b = m4x4.row1;
		 const __m128 c = m4x4.row2;
		 const __m128 d = m4x4.row3;
		 
		 __m128 t1, t2;

		 //1° colum
		 t1 = _mm_set1_ps(self.m11);
		 t2 = _mm_mul_ps(a, t1);
		 t1 =_mm_set1_ps(self.m12);
		 t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
		 t1 =_mm_set1_ps(self.m13);
		 t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
		 t1 =_mm_set1_ps(self.m14);
		 t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);
		 
		 _mm_store_ps(&self.entries[0], t2);

		 //2° colum
		 t1 = _mm_set1_ps(self.m21);
		 t2 = _mm_mul_ps(a, t1);
		 t1 =_mm_set1_ps(self.m22);
		 t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
		 t1 =_mm_set1_ps(self.m23);
		 t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
		 t1 =_mm_set1_ps(self.m24);
		 t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);

		 _mm_store_ps(&self.entries[4], t2);
		 
		 //3° colum
		 t1 = _mm_set1_ps(self.m31);
		 t2 = _mm_mul_ps(a, t1);
		 t1 =_mm_set1_ps(self.m32);
		 t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
		 t1 =_mm_set1_ps(self.m33);
		 t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
		 t1 =_mm_set1_ps(self.m34);
		 t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);

		 _mm_store_ps(&self.entries[8], t2);
		 
		 //4° colum
		 t1 = _mm_set1_ps(self.m41);
		 t2 = _mm_mul_ps(a, t1);
		 t1 =_mm_set1_ps(self.m42);
		 t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
		 t1 =_mm_set1_ps(self.m43);
		 t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
		 t1 =_mm_set1_ps(self.m44);
		 t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);

		 _mm_store_ps(&self.entries[12], t2);
	}
DFORCEINLINE void Easy2D::SSE2_Matrix4Inv(Matrix4x4& self)
{
 __m128 minor0, minor1, minor2, minor3;
 __m128 det, tmp1;
// -----------------------------------------------
 tmp1 = _mm_mul_ps(self.row2, self.row3);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
 minor0 = _mm_mul_ps(self.row1, tmp1);
 minor1 = _mm_mul_ps(self.row0, tmp1);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
 minor0 = _mm_sub_ps(_mm_mul_ps(self.row1, tmp1), minor0);
 minor1 = _mm_sub_ps(_mm_mul_ps(self.row0, tmp1), minor1);
 minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);
// -----------------------------------------------
 tmp1 = _mm_mul_ps(self.row1, self.row2);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
 minor0 = _mm_add_ps(_mm_mul_ps(self.row3, tmp1), minor0);
 minor3 = _mm_mul_ps(self.row0, tmp1);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
 minor0 = _mm_sub_ps(minor0, _mm_mul_ps(self.row3, tmp1));
 minor3 = _mm_sub_ps(_mm_mul_ps(self.row0, tmp1), minor3);
 minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);
// -----------------------------------------------
 tmp1 = _mm_mul_ps(_mm_shuffle_ps(self.row1, self.row1, 0x4E), self.row3);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
 self.row2 = _mm_shuffle_ps(self.row2, self.row2, 0x4E);
 minor0 = _mm_add_ps(_mm_mul_ps(self.row2, tmp1), minor0);
 minor2 = _mm_mul_ps(self.row0, tmp1);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
 minor0 = _mm_sub_ps(minor0, _mm_mul_ps(self.row2, tmp1));
 minor2 = _mm_sub_ps(_mm_mul_ps(self.row0, tmp1), minor2);
 minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);
// -----------------------------------------------
 tmp1 = _mm_mul_ps(self.row0, self.row1);//Streaming SIMD Extensions - Inverse of 4x4 Matrix
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
 minor2 = _mm_add_ps(_mm_mul_ps(self.row3, tmp1), minor2);
 minor3 = _mm_sub_ps(_mm_mul_ps(self.row2, tmp1), minor3);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
 minor2 = _mm_sub_ps(_mm_mul_ps(self.row3, tmp1), minor2);
 minor3 = _mm_sub_ps(minor3, _mm_mul_ps(self.row2, tmp1));
// -----------------------------------------------
 tmp1 = _mm_mul_ps(self.row0, self.row3);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
 minor1 = _mm_sub_ps(minor1, _mm_mul_ps(self.row2, tmp1));
 minor2 = _mm_add_ps(_mm_mul_ps(self.row1, tmp1), minor2);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
 minor1 = _mm_add_ps(_mm_mul_ps(self.row2, tmp1), minor1);
 minor2 = _mm_sub_ps(minor2, _mm_mul_ps(self.row1, tmp1));
// -----------------------------------------------
 tmp1 = _mm_mul_ps(self.row0, self.row2);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
 minor1 = _mm_add_ps(_mm_mul_ps(self.row3, tmp1), minor1);
 minor3 = _mm_sub_ps(minor3, _mm_mul_ps(self.row1, tmp1));
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
 minor1 = _mm_sub_ps(minor1, _mm_mul_ps(self.row3, tmp1));
 minor3 = _mm_add_ps(_mm_mul_ps(self.row1, tmp1), minor3);
// -----------------------------------------------
 det = _mm_mul_ps(self.row0, minor0);
 det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
 det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
 tmp1 = _mm_rcp_ss(det);
 det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
 det = _mm_shuffle_ps(det, det, 0x00);
 //
 minor0 = _mm_mul_ps(det, minor0);
 _mm_storel_pi((__m64*)(self.entries), minor0);
 _mm_storeh_pi((__m64*)(self.entries+2), minor0);
 minor1 = _mm_mul_ps(det, minor1);
 _mm_storel_pi((__m64*)(self.entries+4), minor1);
 _mm_storeh_pi((__m64*)(self.entries+6), minor1);
 minor2 = _mm_mul_ps(det, minor2);
 _mm_storel_pi((__m64*)(self.entries+ 8), minor2);
 _mm_storeh_pi((__m64*)(self.entries+10), minor2);
 minor3 = _mm_mul_ps(det, minor3);
 _mm_storel_pi((__m64*)(self.entries+12), minor3);
 _mm_storeh_pi((__m64*)(self.entries+14), minor3);
}
#endif
