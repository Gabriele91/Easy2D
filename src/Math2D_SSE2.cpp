#include <stdafx.h>
#include "Math2D_SSE2.h"

#if defined( SIMD_SSE2 )
namespace Easy2D{

ALIGNED(16,
union SSE2Struct {
	__m128 smid;
	ALIGNED(16, float v[4]);

	DFORCEINLINE SSE2Struct(){
		smid=_mm_setzero_ps();
	}
	DFORCEINLINE SSE2Struct(const float *a){
		v[0]=a[0];
		v[1]=a[1];
		v[2]=a[2];
		v[3]=a[3];
	}
	DFORCEINLINE void toVec(float *a){
		a[0]=v[0];
		a[1]=v[1];
		a[2]=v[2];
		a[3]=v[3];
	}

});
extern void SSE2_Matrix4Mul(Matrix4x4 &out,
                            const Matrix4x4 &A,
                            const Matrix4x4 &B)
{

	SSE2Struct a(A.entries);
	SSE2Struct b(A.entries+4);
	SSE2Struct c(A.entries+8);
	SSE2Struct d(A.entries+12);
	SSE2Struct t,t2;

	t.smid = _mm_set1_ps(B.entries[0]);
	t2.smid = _mm_mul_ps(a.smid,t.smid);
	t.smid=_mm_set1_ps(B.entries[1]);
	t2.smid = _mm_add_ps(_mm_mul_ps(b.smid,t.smid),t2.smid);
	t.smid=_mm_set1_ps(B.entries[2]);
	t2.smid = _mm_add_ps(_mm_mul_ps(c.smid,t.smid),t2.smid);
	t.smid=_mm_set1_ps(B.entries[3]);
	t2.smid = _mm_add_ps(_mm_mul_ps(d.smid,t.smid),t2.smid);

	t2.toVec(out.entries);

	t.smid = _mm_set1_ps(B.entries[4]);
	t2.smid = _mm_mul_ps(a.smid,t.smid);
	t.smid=_mm_set1_ps(B.entries[5]);
	t2.smid = _mm_add_ps(_mm_mul_ps(b.smid,t.smid),t2.smid);
	t.smid=_mm_set1_ps(B.entries[6]);
	t2.smid = _mm_add_ps(_mm_mul_ps(c.smid,t.smid),t2.smid);
	t.smid=_mm_set1_ps(B.entries[7]);
	t2.smid = _mm_add_ps(_mm_mul_ps(d.smid,t.smid),t2.smid);

	t2.toVec(out.entries+4);

	t.smid = _mm_set1_ps(B.entries[8]);
	t2.smid = _mm_mul_ps(a.smid,t.smid);
	t.smid=_mm_set1_ps(B.entries[9]);
	t2.smid = _mm_add_ps(_mm_mul_ps(b.smid,t.smid),t2.smid);
	t.smid=_mm_set1_ps(B.entries[10]);
	t2.smid = _mm_add_ps(_mm_mul_ps(c.smid,t.smid),t2.smid);
	t.smid=_mm_set1_ps(B.entries[11]);
	t2.smid = _mm_add_ps(_mm_mul_ps(d.smid,t.smid),t2.smid);

	t2.toVec(out.entries+8);

	t.smid = _mm_set1_ps(B.entries[12]);
	t2.smid = _mm_mul_ps(a.smid,t.smid);
	t.smid=_mm_set1_ps(B.entries[13]);
	t2.smid = _mm_add_ps(_mm_mul_ps(b.smid,t.smid),t2.smid);
	t.smid=_mm_set1_ps(B.entries[14]);
	t2.smid = _mm_add_ps(_mm_mul_ps(c.smid,t.smid),t2.smid);
	t.smid=_mm_set1_ps(B.entries[15]);
	t2.smid = _mm_add_ps(_mm_mul_ps(d.smid,t.smid),t2.smid);

	t2.toVec(out.entries+12);
}

extern void SSE2_Matrix4Inv(Matrix4x4& self){
__m128 self_row0 = _mm_load_ps(self.entries);
__m128 self_row1 = _mm_load_ps(self.entries+4);
__m128 self_row2 = _mm_load_ps(self.entries+8);
__m128 self_row3 = _mm_load_ps(self.entries+12);
 __m128 minor0, minor1, minor2, minor3;
 __m128 det, tmp1;
// -----------------------------------------------
 tmp1 = _mm_mul_ps(self_row2, self_row3);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
 minor0 = _mm_mul_ps(self_row1, tmp1);
 minor1 = _mm_mul_ps(self_row0, tmp1);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
 minor0 = _mm_sub_ps(_mm_mul_ps(self_row1, tmp1), minor0);
 minor1 = _mm_sub_ps(_mm_mul_ps(self_row0, tmp1), minor1);
 minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);
// -----------------------------------------------
 tmp1 = _mm_mul_ps(self_row1, self_row2);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
 minor0 = _mm_add_ps(_mm_mul_ps(self_row3, tmp1), minor0);
 minor3 = _mm_mul_ps(self_row0, tmp1);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
 minor0 = _mm_sub_ps(minor0, _mm_mul_ps(self_row3, tmp1));
 minor3 = _mm_sub_ps(_mm_mul_ps(self_row0, tmp1), minor3);
 minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);
// -----------------------------------------------
 tmp1 = _mm_mul_ps(_mm_shuffle_ps(self_row1, self_row1, 0x4E), self_row3);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
 self_row2 = _mm_shuffle_ps(self_row2, self_row2, 0x4E);
 minor0 = _mm_add_ps(_mm_mul_ps(self_row2, tmp1), minor0);
 minor2 = _mm_mul_ps(self_row0, tmp1);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
 minor0 = _mm_sub_ps(minor0, _mm_mul_ps(self_row2, tmp1));
 minor2 = _mm_sub_ps(_mm_mul_ps(self_row0, tmp1), minor2);
 minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);
// -----------------------------------------------
 tmp1 = _mm_mul_ps(self_row0, self_row1);//Streaming SIMD Extensions - Inverse of 4x4 Matrix
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
 minor2 = _mm_add_ps(_mm_mul_ps(self_row3, tmp1), minor2);
 minor3 = _mm_sub_ps(_mm_mul_ps(self_row2, tmp1), minor3);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
 minor2 = _mm_sub_ps(_mm_mul_ps(self_row3, tmp1), minor2);
 minor3 = _mm_sub_ps(minor3, _mm_mul_ps(self_row2, tmp1));
// -----------------------------------------------
 tmp1 = _mm_mul_ps(self_row0, self_row3);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
 minor1 = _mm_sub_ps(minor1, _mm_mul_ps(self_row2, tmp1));
 minor2 = _mm_add_ps(_mm_mul_ps(self_row1, tmp1), minor2);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
 minor1 = _mm_add_ps(_mm_mul_ps(self_row2, tmp1), minor1);
 minor2 = _mm_sub_ps(minor2, _mm_mul_ps(self_row1, tmp1));
// -----------------------------------------------
 tmp1 = _mm_mul_ps(self_row0, self_row2);
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
 minor1 = _mm_add_ps(_mm_mul_ps(self_row3, tmp1), minor1);
 minor3 = _mm_sub_ps(minor3, _mm_mul_ps(self_row1, tmp1));
 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
 minor1 = _mm_sub_ps(minor1, _mm_mul_ps(self_row3, tmp1));
 minor3 = _mm_add_ps(_mm_mul_ps(self_row1, tmp1), minor3);
// -----------------------------------------------
 det = _mm_mul_ps(self_row0, minor0);
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

};

#endif
