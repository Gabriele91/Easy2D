#include <stdafx.h>
#include "Math3D_SSE2.h"

#if defined( SIMD_SSE2 ) && !defined( __arm__ )
namespace Easy2D
{

ALIGNED(16,
        union SSE2Struct
{
    __m128 smid;
    ALIGNED(16, float v[4]);

    DFORCEINLINE SSE2Struct()
    {
        smid=_mm_setzero_ps();
    }
    DFORCEINLINE SSE2Struct(const float *a)
    {
        v[0]=a[0];
        v[1]=a[1];
        v[2]=a[2];
        v[3]=a[3];
    }
    DFORCEINLINE void toVec(float *a)
    {
        a[0]=v[0];
        a[1]=v[1];
        a[2]=v[2];
        a[3]=v[3];
    }

});

// linear combination:
// a[0] * B.row[0] + a[1] * B.row[1] + a[2] * B.row[2] + a[3] * B.row[3]
extern __m128 SSE2_lincomb(const __m128 &a, const Matrix4x4 &B)
{
    __m128 result;
    result = _mm_mul_ps(_mm_shuffle_ps(a, a, 0x00), B.row0);
    result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a, a, 0x55), B.row1));
    result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a, a, 0xaa), B.row2));
    result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a, a, 0xff), B.row3));
    return result;
}

extern void SSE2_Matrix4Mul(Matrix4x4 &out,
                            const Matrix4x4 &A,
                            const Matrix4x4 &B)
{

    __m128 out0x = SSE2_lincomb(A.row0, B);
    __m128 out1x = SSE2_lincomb(A.row1, B);
    __m128 out2x = SSE2_lincomb(A.row2, B);
    __m128 out3x = SSE2_lincomb(A.row3, B);

    out.row0 = out0x;
    out.row1 = out1x;
    out.row2 = out2x;
    out.row3 = out3x;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FROM BULLET
#define _mm_ror_ps(vec,i)	\
(((i)%4) ? \
(_mm_shuffle_ps(vec,vec, _MM_SHUFFLE((unsigned char)(i+3)%4,(unsigned char)(i+2)%4,(unsigned char)(i+1)%4,(unsigned char)(i+0)%4))) : (vec))

static VSALIGNED(16) const unsigned int _vmathPNPN[4] GCCALIGNED(16) = {0x00000000, 0x80000000, 0x00000000, 0x80000000};
static VSALIGNED(16) const unsigned int _vmathNPNP[4] GCCALIGNED(16) = {0x80000000, 0x00000000, 0x80000000, 0x00000000};
static VSALIGNED(16) const float _vmathZERONE[4]      GCCALIGNED(16) = {1.0f, 0.0f, 0.0f, 1.0f};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern void SSE2_Matrix4Inv(Matrix4x4& self)
{

    __m128 Va,Vb,Vc;
    __m128 r1,r2,r3,tt,tt2;
    __m128 sum,Det,RDet;
    __m128 trns0,trns1,trns2,trns3;

    __m128 _L1 = self.row0;
    __m128 _L2 = self.row1;
    __m128 _L3 = self.row2;
    __m128 _L4 = self.row3;
    // Calculating the minterms for the first line.

    // _mm_ror_ps is just a macro using _mm_shuffle_ps().
    tt = _L4;
    tt2 = _mm_ror_ps(_L3,1);
    Vc = _mm_mul_ps(tt2,_mm_ror_ps(tt,0));					// V3'dot V4
    Va = _mm_mul_ps(tt2,_mm_ror_ps(tt,2));					// V3'dot V4"
    Vb = _mm_mul_ps(tt2,_mm_ror_ps(tt,3));					// V3' dot V4^

    r1 = _mm_sub_ps(_mm_ror_ps(Va,1),_mm_ror_ps(Vc,2));		// V3" dot V4^ - V3^ dot V4"
    r2 = _mm_sub_ps(_mm_ror_ps(Vb,2),_mm_ror_ps(Vb,0));		// V3^ dot V4' - V3' dot V4^
    r3 = _mm_sub_ps(_mm_ror_ps(Va,0),_mm_ror_ps(Vc,1));		// V3' dot V4" - V3" dot V4'

    tt = _L2;
    Va = _mm_ror_ps(tt,1);
    sum = _mm_mul_ps(Va,r1);
    Vb = _mm_ror_ps(tt,2);
    sum = _mm_add_ps(sum,_mm_mul_ps(Vb,r2));
    Vc = _mm_ror_ps(tt,3);
    sum = _mm_add_ps(sum,_mm_mul_ps(Vc,r3));

    // Calculating the determinant.
    Det = _mm_mul_ps(sum,_L1);
    Det = _mm_add_ps(Det,_mm_movehl_ps(Det,Det));

    const __m128 Sign_PNPN = _mm_load_ps((float *)_vmathPNPN);
    const __m128 Sign_NPNP = _mm_load_ps((float *)_vmathNPNP);

    __m128 mtL1 = _mm_xor_ps(sum,Sign_PNPN);

    // Calculating the minterms of the second line (using previous results).
    tt = _mm_ror_ps(_L1,1);
    sum = _mm_mul_ps(tt,r1);
    tt = _mm_ror_ps(tt,1);
    sum = _mm_add_ps(sum,_mm_mul_ps(tt,r2));
    tt = _mm_ror_ps(tt,1);
    sum = _mm_add_ps(sum,_mm_mul_ps(tt,r3));
    __m128 mtL2 = _mm_xor_ps(sum,Sign_NPNP);

    // Testing the determinant.
    Det = _mm_sub_ss(Det,_mm_shuffle_ps(Det,Det,1));

    // Calculating the minterms of the third line.
    tt = _mm_ror_ps(_L1,1);
    Va = _mm_mul_ps(tt,Vb);									// V1' dot V2"
    Vb = _mm_mul_ps(tt,Vc);									// V1' dot V2^
    Vc = _mm_mul_ps(tt,_L2);								// V1' dot V2

    r1 = _mm_sub_ps(_mm_ror_ps(Va,1),_mm_ror_ps(Vc,2));		// V1" dot V2^ - V1^ dot V2"
    r2 = _mm_sub_ps(_mm_ror_ps(Vb,2),_mm_ror_ps(Vb,0));		// V1^ dot V2' - V1' dot V2^
    r3 = _mm_sub_ps(_mm_ror_ps(Va,0),_mm_ror_ps(Vc,1));		// V1' dot V2" - V1" dot V2'

    tt = _mm_ror_ps(_L4,1);
    sum = _mm_mul_ps(tt,r1);
    tt = _mm_ror_ps(tt,1);
    sum = _mm_add_ps(sum,_mm_mul_ps(tt,r2));
    tt = _mm_ror_ps(tt,1);
    sum = _mm_add_ps(sum,_mm_mul_ps(tt,r3));
    __m128 mtL3 = _mm_xor_ps(sum,Sign_PNPN);

    // Dividing is FASTER than rcp_nr! (Because rcp_nr causes many register-memory RWs).
    RDet = _mm_div_ss(_mm_load_ss((float *)&_vmathZERONE), Det); // TODO: just 1.0f?
    RDet = _mm_shuffle_ps(RDet,RDet,0x00);

    // Devide the first 12 minterms with the determinant.
    mtL1 = _mm_mul_ps(mtL1, RDet);
    mtL2 = _mm_mul_ps(mtL2, RDet);
    mtL3 = _mm_mul_ps(mtL3, RDet);

    // Calculate the minterms of the forth line and devide by the determinant.
    tt = _mm_ror_ps(_L3,1);
    sum = _mm_mul_ps(tt,r1);
    tt = _mm_ror_ps(tt,1);
    sum = _mm_add_ps(sum,_mm_mul_ps(tt,r2));
    tt = _mm_ror_ps(tt,1);
    sum = _mm_add_ps(sum,_mm_mul_ps(tt,r3));
    __m128 mtL4 = _mm_xor_ps(sum,Sign_NPNP);
    mtL4 = _mm_mul_ps(mtL4, RDet);

    // Now we just have to transpose the minterms matrix.
    trns0 = _mm_unpacklo_ps(mtL1,mtL2);
    trns1 = _mm_unpacklo_ps(mtL3,mtL4);
    trns2 = _mm_unpackhi_ps(mtL1,mtL2);
    trns3 = _mm_unpackhi_ps(mtL3,mtL4);

    self.row0 = _mm_movelh_ps(trns0,trns1);
    self.row1 = _mm_movehl_ps(trns1,trns0);
    self.row2 = _mm_movelh_ps(trns2,trns3);
    self.row3 = _mm_movehl_ps(trns3,trns2);
}

extern void SSE2_Matrix4Transpose(Matrix4x4& self)
{

    __m128 T0 = _mm_unpacklo_ps(self.row0, self.row1);
    __m128 T1 = _mm_unpacklo_ps(self.row2, self.row3);
    __m128 T2 = _mm_unpackhi_ps(self.row0, self.row1);
    __m128 T3 = _mm_unpackhi_ps(self.row2, self.row3);

    self.row0 = _mm_movelh_ps(T0, T1);
    self.row1 = _mm_movelh_ps(T1, T0);
    self.row2 = _mm_movelh_ps(T2, T3);
    self.row3 = _mm_movelh_ps(T3, T2);

}

};

#endif
