#include <stdafx.h>
#include <Math3D.h>
#include "Math3D_SSE2.h"
#include "Math3D_neon.h"
#include "Math3D_vfp.h"

/* Easy2D */
using namespace Easy2D;
/* VECTOR2D */
Vector2D Vector2D::ZERO;
Vector2D Vector2D::ONE(1,1);
Vector2D Vector2D::NEGATIVE_ONE(-1,-1);
Vector2D Vector2D::MIN(FLT_MIN,FLT_MIN);
Vector2D Vector2D::MAX(FLT_MAX,FLT_MAX);
void Vector2D::normalize()
{
    float d=sqrt(x*x+y*y);
    x/=d;
    y/=d;
}
float Vector2D::length() const
{
    return sqrt(x*x+y*y);
}
float Vector2D::direction() const
{
    return atan2f(y,x);//atan(y/x)
}
float Vector2D::cross(const Vector2D& vec) const
{
    return x * vec.y - y * vec.x;
}
float Vector2D::dot(const Vector2D& vec) const
{
    return x * vec.x + y * vec.y;
}
float Vector2D::distance(const Vector2D& vec) const
{
    float dx=(x-vec.x);
    float dy=(y-vec.y);
    return  sqrt(dx*dx+dy*dy);
}
float Vector2D::distancePow2(const Vector2D& vec) const
{
    float dx=(x-vec.x);
    float dy=(y-vec.y);
    return  dx*dx+dy*dy;
}
Vector2D Vector2D::axis(const Vector2D& vec) const
{
    return Vector2D(x - vec.x,-y + vec.y);
}
Vector2D Vector2D::getNormalize() const
{
    float d=sqrt(x*x+y*y);
    return Vector2D(x/d,y/d);
}
Vector2D Vector2D::projected(const Vector2D& axis) const
{
    float p=((x*axis.x+y*axis.y)/(axis.x*axis.x+axis.y*axis.y));
    return Vector2D(p*axis.x,p*axis.y);
}
String Vector2D::toString(const String& start,const String& sep,const String& end) const
{
    return start+String::toString(x)+sep+String::toString(y)+end;
}
/* VECTOR3D */
Vector3D Vector3D::ZERO;
Vector3D Vector3D::ONE(1,1,1);
Vector3D Vector3D::NEGATIVE_ONE(-1,-1,-1);
Vector3D Vector3D::MIN(FLT_MIN,FLT_MIN,FLT_MIN);
Vector3D Vector3D::MAX(FLT_MAX,FLT_MAX,FLT_MAX);
void Vector3D::normalize()
{
    float d=sqrt(x*x+y*y+z*z);
    x/=d;
    y/=d;
    z/=d;
}
float Vector3D::length() const
{
    return sqrt(x*x+y*y+z*z);
}
float Vector3D::dot(const Vector3D& vec) const
{
    return x*vec.x+y*vec.y+z*vec.z;
}
float Vector3D::distance(const Vector3D& vec) const
{
    float dx=x-vec.x,dy=y-vec.y,dz=z-vec.z;
    return sqrt(dx*dx+dy*dy+dz*dz);
}
float Vector3D::distancePow2(const Vector3D& vec) const
{
    float dx=x-vec.x,dy=y-vec.y,dz=z-vec.z;
    return dx*dx+dy*dy+dz*dz;
}
Vector3D Vector3D::cross(const Vector3D& vec) const
{
    return Vector3D(y * vec.z - z * vec.y,
                    z * vec.x - x * vec.z,
                    x * vec.y - y * vec.x);
}
Vector3D Vector3D::getNormalize() const
{
    float d=sqrt(x*x+y*y+z*z);
    return Vector3D(x/d,y/d,z/d);
}
String Vector3D::toString(const String& start,const String& sep,const String& end) const
{
    return start+String::toString(x)+sep+String::toString(y)+sep+String::toString(z)+end;
}

/* VECTOR4D */
Vector4D Vector4D::ZERO;
Vector4D Vector4D::ONE(1,1,1,1);
Vector4D Vector4D::NEGATIVE_ONE(-1,-1,-1,-1);
Vector4D Vector4D::MIN(FLT_MIN,FLT_MIN,FLT_MIN,FLT_MIN);
Vector4D Vector4D::MAX(FLT_MAX,FLT_MAX,FLT_MAX,FLT_MAX);
void Vector4D::normalize()
{
    float d=sqrt(x*x+y*y+z*z+w*w);
    x/=d;
    y/=d;
    z/=d;
    w/=d;
}
float Vector4D::length() const
{
    return sqrt(x*x+y*y+z*z+w*w);
}
float Vector4D::dot(const Vector4D& vec) const
{
    return x*vec.x+y*vec.y+z*vec.z+w*vec.w;
}
float Vector4D::distance(const Vector4D& vec) const
{
    float dx=x-vec.x,dy=y-vec.y,dz=z-vec.z,dw=w-vec.w;
    return sqrt(dx*dx+dy*dy+dz*dz+dw*dw);
}
float Vector4D::distancePow2(const Vector4D& vec) const
{
    float dx=x-vec.x,dy=y-vec.y,dz=z-vec.z,dw=w-vec.w;
    return dx*dx+dy*dy+dz*dz+dw*dw;
}
Vector4D Vector4D::getNormalize() const
{
    float d=sqrt(x*x+y*y+z*z+w*w);
    return Vector4D(x/d,y/d,z/d,w/d);
}
String Vector4D::toString(const String& start,const String& sep,const String& end) const
{
    return start+String::toString(x)+sep+String::toString(y)+sep+String::toString(z)+sep+String::toString(w)+end;
}

/* AABBox */
AABox2::AABox2(const Vec2& center,Vec2 size)
{
    setBox(center,size);
}
AABox2::AABox2() {}
AABox2::~AABox2() {}

void AABox2::setBox(const Vec2& center,Vec2 size)
{
    size.abs();
    min=center-size*0.5;
    max=center+size*0.5;
}
void AABox2::addPoint(const Vec2& p)
{
    //found box                      m______
    min.x=Math::min(min.x,p.x);  //  |\    |
    min.y=Math::min(min.y,p.y);  //  | \.c |
    max.x=Math::max(max.x,p.x);  //  |   \ |
    max.y=Math::max(max.y,p.y);  //  |____\|M
}
bool AABox2::isIntersection(const AABox2& aabb2)
{
    auto cdiff=(getCenter()-aabb2.getCenter()).getAbs();
    auto ssum=getSize()+aabb2.getSize();
    return  cdiff.x <= ssum.x && cdiff.y <= ssum.y;
}
bool AABox2::isIntersection(const Vec2& point)
{
    auto cdiff=(getCenter()-point).getAbs();
    auto size=getSize();
    return  cdiff.x <= size.x && cdiff.y <= size.y;
}

/* MATRIX4x4*/

static float Matrix4x4Identity[]=
{
    1.0,0.0,0.0,0.0,
    0.0,1.0,0.0,0.0,
    0.0,0.0,1.0,0.0,
    0.0,0.0,0.0,1.0
};
static float Matrix4x4Zero[]=
{
    0.0,0.0,0.0,0.0,
    0.0,0.0,0.0,0.0,
    0.0,0.0,0.0,0.0,
    0.0,0.0,0.0,0.0
};

Matrix4x4 Matrix4x4::IDENTITY;
Matrix4x4 Matrix4x4::ZERO(Matrix4x4Zero);


static DFORCEINLINE bool gluInvertMatrix(const float m[16],float invOut[16])
{
    float inv[16], det;
    int i;

    inv[0] =   m[5]*m[10]*m[15] - m[5]*m[11]*m[14] - m[9]*m[6]*m[15]
               + m[9]*m[7]*m[14] + m[13]*m[6]*m[11] - m[13]*m[7]*m[10];
    inv[4] =  -m[4]*m[10]*m[15] + m[4]*m[11]*m[14] + m[8]*m[6]*m[15]
              - m[8]*m[7]*m[14] - m[12]*m[6]*m[11] + m[12]*m[7]*m[10];
    inv[8] =   m[4]*m[9]*m[15] - m[4]*m[11]*m[13] - m[8]*m[5]*m[15]
               + m[8]*m[7]*m[13] + m[12]*m[5]*m[11] - m[12]*m[7]*m[9];
    inv[12] = -m[4]*m[9]*m[14] + m[4]*m[10]*m[13] + m[8]*m[5]*m[14]
              - m[8]*m[6]*m[13] - m[12]*m[5]*m[10] + m[12]*m[6]*m[9];
    inv[1] =  -m[1]*m[10]*m[15] + m[1]*m[11]*m[14] + m[9]*m[2]*m[15]
              - m[9]*m[3]*m[14] - m[13]*m[2]*m[11] + m[13]*m[3]*m[10];
    inv[5] =   m[0]*m[10]*m[15] - m[0]*m[11]*m[14] - m[8]*m[2]*m[15]
               + m[8]*m[3]*m[14] + m[12]*m[2]*m[11] - m[12]*m[3]*m[10];
    inv[9] =  -m[0]*m[9]*m[15] + m[0]*m[11]*m[13] + m[8]*m[1]*m[15]
              - m[8]*m[3]*m[13] - m[12]*m[1]*m[11] + m[12]*m[3]*m[9];
    inv[13] =  m[0]*m[9]*m[14] - m[0]*m[10]*m[13] - m[8]*m[1]*m[14]
               + m[8]*m[2]*m[13] + m[12]*m[1]*m[10] - m[12]*m[2]*m[9];
    inv[2] =   m[1]*m[6]*m[15] - m[1]*m[7]*m[14] - m[5]*m[2]*m[15]
               + m[5]*m[3]*m[14] + m[13]*m[2]*m[7] - m[13]*m[3]*m[6];
    inv[6] =  -m[0]*m[6]*m[15] + m[0]*m[7]*m[14] + m[4]*m[2]*m[15]
              - m[4]*m[3]*m[14] - m[12]*m[2]*m[7] + m[12]*m[3]*m[6];
    inv[10] =  m[0]*m[5]*m[15] - m[0]*m[7]*m[13] - m[4]*m[1]*m[15]
               + m[4]*m[3]*m[13] + m[12]*m[1]*m[7] - m[12]*m[3]*m[5];
    inv[14] = -m[0]*m[5]*m[14] + m[0]*m[6]*m[13] + m[4]*m[1]*m[14]
              - m[4]*m[2]*m[13] - m[12]*m[1]*m[6] + m[12]*m[2]*m[5];
    inv[3] =  -m[1]*m[6]*m[11] + m[1]*m[7]*m[10] + m[5]*m[2]*m[11]
              - m[5]*m[3]*m[10] - m[9]*m[2]*m[7] + m[9]*m[3]*m[6];
    inv[7] =   m[0]*m[6]*m[11] - m[0]*m[7]*m[10] - m[4]*m[2]*m[11]
               + m[4]*m[3]*m[10] + m[8]*m[2]*m[7] - m[8]*m[3]*m[6];
    inv[11] = -m[0]*m[5]*m[11] + m[0]*m[7]*m[9] + m[4]*m[1]*m[11]
              - m[4]*m[3]*m[9] - m[8]*m[1]*m[7] + m[8]*m[3]*m[5];
    inv[15] =  m[0]*m[5]*m[10] - m[0]*m[6]*m[9] - m[4]*m[1]*m[10]
               + m[4]*m[2]*m[9] + m[8]*m[1]*m[6] - m[8]*m[2]*m[5];

    det = m[0]*inv[0] + m[1]*inv[4] + m[2]*inv[8] + m[3]*inv[12];
    if (det == 0)
        return false;

    det = 1.0f / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

    return true;
}

Matrix4x4::Matrix4x4()
{
    identity();
};

Matrix4x4::Matrix4x4(const Matrix4x4 &m4x4)
{
    (*this)=m4x4;
}
Matrix4x4::Matrix4x4(float* m4x4)
{
    memcpy(entries, m4x4, 16*sizeof(float));
}
Matrix4x4::Matrix4x4(float e0,float e1,float e2,float e3,
                     float e4,float e5,float e6,float e7,
                     float e8,float e9,float e10,float e11,
                     float e12,float e13,float e14,float e15)
{
    entries[0]=e0;
    entries[1]=e1;
    entries[2]=e2;
    entries[3]=e3;
    entries[4]=e4;
    entries[5]=e5;
    entries[6]=e6;
    entries[7]=e7;
    entries[8]=e8;
    entries[9]=e9;
    entries[10]=e10;
    entries[11]=e11;
    entries[12]=e12;
    entries[13]=e13;
    entries[14]=e14;
    entries[15]=e15;
}

void Matrix4x4::identity()
{
    memcpy(entries, Matrix4x4Identity, 16*sizeof(float));
}
void Matrix4x4::zero()
{
    memset(entries,0,sizeof(float)*16);
}


Matrix4x4 Matrix4x4::mul(const Matrix4x4 &m4x4) const
{

#if (TARGET_IPHONE_SIMULATOR == 0) && (TARGET_OS_IPHONE == 1)
    Matrix4x4 out_m4x4;
#ifdef _ARM_ARCH_7
    NEON_Matrix4Mul( this->entries ,m4x4.entries, out_m4x4.entries);
#else
    Matrix4Mul( this->entries ,m4x4.entries, out_m4x4.entries);
#endif
    return out_m4x4;
#elif defined( SIMD_SSE2 )
    Matrix4x4 out_m4x4;
    SSE2_Matrix4Mul(out_m4x4,m4x4,*this);
    return out_m4x4;
#else
    return Matrix4x4(entries[0]*m4x4.entries[0]+entries[4]*m4x4.entries[1]+entries[8]*m4x4.entries[2]+entries[12]*m4x4.entries[3],
                     entries[1]*m4x4.entries[0]+entries[5]*m4x4.entries[1]+entries[9]*m4x4.entries[2]+entries[13]*m4x4.entries[3],
                     entries[2]*m4x4.entries[0]+entries[6]*m4x4.entries[1]+entries[10]*m4x4.entries[2]+entries[14]*m4x4.entries[3],
                     entries[3]*m4x4.entries[0]+entries[7]*m4x4.entries[1]+entries[11]*m4x4.entries[2]+entries[15]*m4x4.entries[3],
                     entries[0]*m4x4.entries[4]+entries[4]*m4x4.entries[5]+entries[8]*m4x4.entries[6]+entries[12]*m4x4.entries[7],
                     entries[1]*m4x4.entries[4]+entries[5]*m4x4.entries[5]+entries[9]*m4x4.entries[6]+entries[13]*m4x4.entries[7],
                     entries[2]*m4x4.entries[4]+entries[6]*m4x4.entries[5]+entries[10]*m4x4.entries[6]+entries[14]*m4x4.entries[7],
                     entries[3]*m4x4.entries[4]+entries[7]*m4x4.entries[5]+entries[11]*m4x4.entries[6]+entries[15]*m4x4.entries[7],
                     entries[0]*m4x4.entries[8]+entries[4]*m4x4.entries[9]+entries[8]*m4x4.entries[10]+entries[12]*m4x4.entries[11],
                     entries[1]*m4x4.entries[8]+entries[5]*m4x4.entries[9]+entries[9]*m4x4.entries[10]+entries[13]*m4x4.entries[11],
                     entries[2]*m4x4.entries[8]+entries[6]*m4x4.entries[9]+entries[10]*m4x4.entries[10]+entries[14]*m4x4.entries[11],
                     entries[3]*m4x4.entries[8]+entries[7]*m4x4.entries[9]+entries[11]*m4x4.entries[10]+entries[15]*m4x4.entries[11],
                     entries[0]*m4x4.entries[12]+entries[4]*m4x4.entries[13]+entries[8]*m4x4.entries[14]+entries[12]*m4x4.entries[15],
                     entries[1]*m4x4.entries[12]+entries[5]*m4x4.entries[13]+entries[9]*m4x4.entries[14]+entries[13]*m4x4.entries[15],
                     entries[2]*m4x4.entries[12]+entries[6]*m4x4.entries[13]+entries[10]*m4x4.entries[14]+entries[14]*m4x4.entries[15],
                     entries[3]*m4x4.entries[12]+entries[7]*m4x4.entries[13]+entries[11]*m4x4.entries[14]+entries[15]*m4x4.entries[15]);

#endif

}
Matrix4x4 Matrix4x4::mul2D(const Matrix4x4 &m4x4) const
{


#if (TARGET_IPHONE_SIMULATOR == 0) && (TARGET_OS_IPHONE == 1)
    Matrix4x4 out_m4x4;
#ifdef _ARM_ARCH_7
    NEON_Matrix4Mul( this->entries ,m4x4.entries, out_m4x4.entries);
#else
    Matrix4Mul( this->entries ,m4x4.entries, out_m4x4.entries);
#endif
    return out_m4x4;
#elif defined( SIMD_SSE2 )
    Matrix4x4 out_m4x4;
    SSE2_Matrix4Mul(out_m4x4,m4x4,*this);
    return out_m4x4;
#else
    /*

    | (0)  (1)  0 |   | (0)  (1)  0 |
    | (4)  (5)  0 | x | (4)  (5)  0 |
    | (12) (13) 1 |   | (12) (13) 1 |

    (0)  = (0)*(0)+(1)*(4)           (+0*(12))
    (1)  = (0)*(1)+(1)*(5)           (+0*(13))
     0   = 0
    (4)  = (4)*(0)+(5)*(4)           (+0*(12))
    (5)  = (4)*(1)+(5)*(5)           (+0*(13))
     0   = 0
    (12) = (12)*(0)+(13)*(4)+1*(12)
    (13) = (12)*(1)+(13)*(5)+1*(13)
     1   = 1
    */


    return Matrix4x4(
               entries[0]*m4x4.entries[0]+entries[4]*m4x4.entries[1],
               entries[1]*m4x4.entries[0]+entries[5]*m4x4.entries[1],
               0,
               0,
               //
               entries[0]*m4x4.entries[4]+entries[4]*m4x4.entries[5],
               entries[1]*m4x4.entries[4]+entries[5]*m4x4.entries[5],
               0,
               0,
               //
               0,
               0,
               1,
               0,
               //
               entries[0]*m4x4.entries[12]+entries[4]*m4x4.entries[13]+entries[12],
               entries[1]*m4x4.entries[12]+entries[5]*m4x4.entries[13]+entries[13],
               0,
               1);
#endif

}
Vector4D Matrix4x4::mul(const Vector4D &v4) const
{
#if (TARGET_IPHONE_SIMULATOR == 0) && (TARGET_OS_IPHONE == 1)
    Vector4D out;
#ifdef _ARM_ARCH_7
    NEON_Matrix4Vector4Mul( this->entries, &v4.x, &out.x );
#else
    Matrix4Vector4Mul(this->entries,&v4.x,&out.x);
#endif
#elif defined( SIMD_SSE2 )
    Vector4D out;
    out.row=SSE2_lincomb(v4.row,*this);
    return out;
#else
    Vector4D out;

    out.x=entries[0] * v4.x + entries[4] * v4.y + entries[8]  * v4.z + entries[12] * v4.w;
    out.y=entries[1] * v4.x + entries[5] * v4.y + entries[9]  * v4.z + entries[13] * v4.w;
    out.z=entries[2] * v4.x + entries[6] * v4.y + entries[10] * v4.z + entries[14] * v4.w;
    out.w=entries[3] * v4.x + entries[7] * v4.y + entries[11] * v4.z + entries[15] * v4.w;

    return out;
#endif
}
Vector2D Matrix4x4::mul2D(const Vector2D &v2) const
{
    Vector2D out;
    out.x=entries[0] * v2.x + entries[4] * v2.y + entries[8] + entries[12];
    out.y=entries[1] * v2.x + entries[5] * v2.y + entries[9] + entries[13];
    return out;
}

void Matrix4x4::inverse()
{
#if (TARGET_IPHONE_SIMULATOR == 0) && (TARGET_OS_IPHONE == 1)
    Matrix4Invert(&(this->entries[0]),&(this->entries[0]));
#elif defined( SIMD_SSE2 )
    SSE2_Matrix4Inv(*this);
#else
    gluInvertMatrix(&(this->entries[0]),&(this->entries[0]));
#endif
}
void Matrix4x4::inverse2D()
{
#if (TARGET_IPHONE_SIMULATOR == 0) && (TARGET_OS_IPHONE == 1)
    Matrix4Invert(&(this->entries[0]),&(this->entries[0]));
#else
    float det=entries[0]*entries[5]-entries[1]*entries[4];
    float tmp_entries_0=entries[0];

    entries[0]= entries[5]/det;
    entries[1]=-entries[1]/det;
    entries[4]=-entries[4]/det;
    entries[5]= tmp_entries_0/det;

    entries[12]= (-(entries[0]*entries[12]))-entries[4]*entries[13];
    entries[13]= (-(entries[1]*entries[12]))-entries[5]*entries[13];
#endif
}
Matrix4x4 Matrix4x4::getInverse() const
{
#if (TARGET_IPHONE_SIMULATOR == 0) && (TARGET_OS_IPHONE == 1)
    Matrix4x4 out;
    Matrix4Invert(&(entries[0]),&(out.entries[0]));
#elif defined( SIMD_SSE2 )
    Matrix4x4 out(*this);
    SSE2_Matrix4Inv(out);
#else
    Matrix4x4 out;
    gluInvertMatrix(&(entries[0]),&(out.entries[0]));
#endif
    return out;
}
Matrix4x4 Matrix4x4::getInverse2D() const
{
    ///@code
    /// INVERSE ROTATION / SCALE
    ///      | a b |                   | a b | T (T does not mean transposed matrix)
    ///  A=  | c d |   A^-1 = 1/det(A) | c d |
    ///
    ///                   |  d -b |
    ///  A^-1 = 1/det(A)  | -c  a |
    ///  det(A) = a*d-b*c
    ///  invdet= 1/ (a*d-b*c)
    ///
    /// a=(0), b=(1)
    /// c=(4), d=(5)
    ///
    ///  det(A) = (0)*(5)-(1)*(4)
    ///  (0)=   (5) / det(A)
    ///  (1)=  -(1) / det(A)
    ///  (4)=  -(4) / det(A)
    ///  (5)=   (0) / det(A)
    ///
    ///@endcode
    Matrix4x4 out;
    float det=entries[0]*entries[5]-entries[1]*entries[4];

    out.entries[0]= entries[5]/det;
    out.entries[1]=-entries[1]/det;
    out.entries[4]=-entries[4]/det;
    out.entries[5]= entries[0]/det;
    ///@code
    ///INVERSE TRANSLATION
    ///
    /// MULL -t X R^1
    /// |  I    0    |    | R^-1     0 |
    /// | -t    I    |  x |   0      I | =
    ///
    /// | I*R^-1     0 |   |  R^-1      0 |
    /// | (-t)*R^-1  I | = | (-t)*R^-1  I |
    ///
    /// (-t)*R^-1
    ///
    ///  t=[ x y ] -> -t =[ -x -y ]
    ///
    ///  [ -x -y ] x | a b |
    ///              | c d | =
    ///
    ///
    /// (-t)*R^-1 = [ (-(a*x))-c*y  (-(b*x))-d*y ]
    ///
    /// x=entries[12];
    /// y=entries[13];
    ///
    ///  out : = R^-1
    ///
    /// a=out.entries[0];
    /// b=out.entries[1];
    /// c=out.entries[4];
    /// d=out.entries[5];
    ///
    /// out.entries[12]= (-(a*x))-c*y
    /// out.entries[13]= (-(b*x))-d*y
    ///
    /// then....
    /// out.entries[12]= (-(out.entries[0]*entries[12]))-out.entries[4]*entries[13];
    /// out.entries[13]= (-(out.entries[1]*entries[12]))-out.entries[5]*entries[13];
    ///@endcode
    ////////////////////////////////////////////
    out.entries[12]= (-(out.entries[0]*entries[12]))-out.entries[4]*entries[13];
    out.entries[13]= (-(out.entries[1]*entries[12]))-out.entries[5]*entries[13];

    return out;
}
Matrix4x4 Matrix4x4::getTranspose() const
{

#if defined( SIMD_SSE2 )

    Matrix4x4 newMat(*this);
    SSE2_Matrix4Transpose(newMat);
    return newMat;

#else
    Matrix4x4 newMat(*this);
    float tmp;

#define _xy_m4x4_(x,y) newMat.entries[(y*4)+x]

    tmp = _xy_m4x4_(1,0);
    _xy_m4x4_(1,0) = _xy_m4x4_(0,1);
    _xy_m4x4_(0,1) = tmp;

    tmp = _xy_m4x4_(2,0);
    _xy_m4x4_(2,0) = _xy_m4x4_(0,2);
    _xy_m4x4_(0,2) = tmp;

    tmp = _xy_m4x4_(2,1);
    _xy_m4x4_(2,1) = _xy_m4x4_(1,2);
    _xy_m4x4_(1,2) = tmp;

    tmp = _xy_m4x4_(3,0);
    _xy_m4x4_(3,0) = _xy_m4x4_(0,3);
    _xy_m4x4_(0,3) = tmp;

    tmp = _xy_m4x4_(3,1);
    _xy_m4x4_(3,1) = _xy_m4x4_(1,3);
    _xy_m4x4_(1,3) = tmp;

    tmp = _xy_m4x4_(3,2);
    _xy_m4x4_(3,2) = _xy_m4x4_(2,3);
    _xy_m4x4_(2,3) = tmp;

#undef _xy_m4x4_

    return newMat;
#endif
}

void Matrix4x4::setScale(const Vector3D &v3)
{
    identity();
    entries[0]=v3.x;
    entries[5]=v3.y;
    entries[10]=v3.z;
}
void Matrix4x4::addScale(const Vector3D &v3)
{
    entries[0]*=v3.x;
    entries[1]*=v3.y;
    entries[2]*=v3.z;
    entries[4]*=v3.x;
    entries[5]*=v3.y;
    entries[6]*=v3.z;
    entries[8]*=v3.x;
    entries[9]*=v3.y;
    entries[10]*=v3.z;
}
void Matrix4x4::addScale(const Vector2D &v2)
{
    entries[0]*=v2.x;
    entries[1]*=v2.y;
    entries[4]*=v2.x;
    entries[5]*=v2.y;
    entries[8]*=v2.x;
    entries[9]*=v2.y;
}
void Matrix4x4::setScale(const Vector2D &v2)
{
    identity();
    entries[0]=v2.x;
    entries[5]=v2.y;
}
Vector3D Matrix4x4::getScale3D() const
{
    return Vector3D( entries[0], entries[5], entries[10]);
}
Vector2D Matrix4x4::getScale2D() const
{
    return Vector2D( entries[0], entries[5]);
}

void Matrix4x4::setTranslation(const Vector3D &v3)
{
    identity();
    entries[12]=v3.x;
    entries[13]=v3.y;
    entries[14]=v3.z;
}
void Matrix4x4::setTranslation(const Vector2D &v2)
{
    identity();
    entries[12]=v2.x;
    entries[13]=v2.y;
}
Vector3D Matrix4x4::getTranslation3D() const
{
    return Vector3D( entries[12], entries[13], entries[14]);
}
Vector2D Matrix4x4::getTranslation2D() const
{
    return Vector2D( entries[12], entries[13]);
}

///set concatenate trasformation:
void Matrix4x4::addTranslationOnX( float distance )
{
    entries[0] = entries[0] + distance * entries[3];
    entries[4] = entries[4] + distance * entries[7];
    entries[8] = entries[8] + distance * entries[11];
    entries[12] = entries[12] + distance * entries[15];
}
void Matrix4x4::addTranslationOnY( float distance )
{
    entries[1] = entries[1] + distance * entries[3];
    entries[5] = entries[5] + distance * entries[7];
    entries[9] = entries[9] + distance * entries[11];
    entries[13] = entries[13] + distance * entries[15];
}
void Matrix4x4::addTranslationOnZ( float distance )
{
    entries[2] = entries[2] + distance * entries[3];
    entries[6] = entries[6] + distance * entries[7];
    entries[10] = entries[10] + distance * entries[11];
    entries[14] = entries[14] + distance * entries[15];
}
///add a euler rotarion
void Matrix4x4::addEulerRotation(const Vec3& euler)
{
    //var dec
    float cos_ang,sin_ang;

    // yaw
    cos_ang=std::cos(euler.y);
    sin_ang=std::sin(euler.y);

    float m00 = entries[0]*cos_ang + entries[2]*-sin_ang;
    float m01 = entries[4]*cos_ang + entries[6]*-sin_ang;
    float m02 = entries[8]*cos_ang + entries[10]*-sin_ang;

    entries[2] = entries[0]*sin_ang + entries[2]*cos_ang;
    entries[6] = entries[4]*sin_ang + entries[6]*cos_ang;
    entries[10]= entries[8]*sin_ang + entries[10]*cos_ang;

    entries[0]=m00;
    entries[4]=m01;
    entries[8]=m02;

    // pitch
    cos_ang=std::cos(euler.x);
    sin_ang=std::sin(euler.x);

    float m10 = entries[1]*cos_ang + entries[2]*sin_ang;
    float m11 = entries[5]*cos_ang + entries[6]*sin_ang;
    float m12 = entries[9]*cos_ang + entries[10]*sin_ang;

    entries[2] = entries[1]*-sin_ang + entries[2]*cos_ang;
    entries[6] = entries[5]*-sin_ang + entries[6]*cos_ang;
    entries[10] = entries[9]*-sin_ang + entries[10]*cos_ang;

    entries[1]=m10;
    entries[5]=m11;
    entries[9]=m12;

    // roll
    cos_ang=std::cos(euler.z);
    sin_ang=std::sin(euler.z);

    m00 = entries[0]*cos_ang + entries[1]*sin_ang;
    m01 = entries[4]*cos_ang + entries[5]*sin_ang;
    m02 = entries[8]*cos_ang + entries[9]*sin_ang;

    entries[1] = entries[0]*-sin_ang + entries[1]*cos_ang;
    entries[5] = entries[4]*-sin_ang + entries[5]*cos_ang;
    entries[9] = entries[8]*-sin_ang + entries[9]*cos_ang;

    entries[0]=m00;
    entries[4]=m01;
    entries[8]=m02;
}

void Matrix4x4::setRotY(float angle)
{
    identity();

    entries[0]=(float)std::cos(angle);
    entries[2]=-(float)std::sin(angle);

    entries[8]=-entries[2];
    entries[10]=entries[0];
}
void Matrix4x4::setRotX(float angle)
{
    identity();

    entries[5]=(float)std::cos(angle);
    entries[6]=(float)std::sin(angle);

    entries[9]=-entries[6];
    entries[10]=entries[5];
}
void Matrix4x4::setRotZ(float angle)
{
    identity();

    entries[0]=(float)std::cos(angle);
    entries[1]=(float)std::sin(angle);

    entries[4]=-entries[1];
    entries[5]=entries[0];
}

float Matrix4x4::getRotY() const
{
    //****
    //****
    //@*@*
    //****
    float a=entries[8];
    float b=entries[10];
    if(a<=0.0001 && a>=-0.0001) a=0;
    if(b<=0.0001 && b>=-0.0001) b=0;
    return atan2(a,b);
}
float Matrix4x4::getRotX() const
{

    //****
    //****
    //@@@*
    //****
    float ang=std::atan2( entries[9],std::sqrt( entries[8]*entries[8]+entries[10]*entries[10] ) );

    if(ang<=0.0001 && ang>=-0.0001) ang=0;

    return ang;

}
float Matrix4x4::getRotZ() const
{
    //*@**
    //*@**
    //****
    //****
    //return z rot
    float a=entries[1];
    float b=entries[5];
    if(a<=0.0001 && a>=-0.0001) a=0.0;
    if(b<=0.0001 && b>=-0.0001) b=0.0;
    return atan2(a,b);
}
// x,y | alpha | sx,sy
void Matrix4x4::setTransform2D(const Transform2D& t2d)
{
    //(scale z=0 and Translation z=0)

    //Translation
    entries[12]=t2d.position.x;
    entries[13]=t2d.position.y;
    //RotZ
    entries[0]=(float)std::cos(Math::torad(t2d.alpha));
    entries[1]=(float)std::sin(Math::torad(t2d.alpha));
    entries[4]=-entries[1];
    entries[5]=entries[0];

    //mT x mRZ x scale
    // scale.x  // row0 * x
    entries[0]*=t2d.scale.x;
    entries[1]*=t2d.scale.x;
    // scale.y  // row1 * y
    entries[4]*=t2d.scale.y;
    entries[5]*=t2d.scale.y;
    // scale.z //
    entries[10]=1.0f;
}
void Matrix4x4::setOrtho(float left, float right, float bottom,float top, float n, float f)
{
    identity();

    entries[0]=2.0f/(right-left);

    entries[5]=2.0f/(top-bottom);

    entries[10]=-2.0f/(f-n);

    entries[12]=-(right+left)/(right-left);
    entries[13]=-(top+bottom)/(top-bottom);
    entries[14]=-(f+n)/(f-n);
}
void Matrix4x4::setPerspective(float l, float r,
                               float b,float t,
                               float n, float f)
{

    //http://www.manpagez.com/man/3/glFrustum/
    identity();
    //row1
    entries[0]  = 2 * n / (r - l);
    entries[2]  = (r + l) / (r - l);
    //row2
    entries[5]  = 2 * n / (t - b);
    entries[6]  = (t + b) / (t - b);
    //row3
    entries[10] = -(f + n) / (f - n);
    entries[11] = -(2 * f * n) / (f - n);
    //row4
    entries[14] = -1;
    entries[15] =  0;

}
void Matrix4x4::setPerspective(float fov, float fRealAspect, float fNear, float fFar)
{

    //from:https://code.google.com/p/oolongengine/source/browse/trunk/Oolong%20Engine2/Math/Matrix.cpp
    float f, n;

    f = 1.0f / (float)std::tan(fov * 0.5f);
    n = 1.0f / (fNear - fFar);

    entries[ 0] = f / fRealAspect;
    entries[ 1] = 0;
    entries[ 2] = 0;
    entries[ 3] = 0;

    entries[ 4] = 0;
    entries[ 5] = f;
    entries[ 6] = 0;
    entries[ 7] = 0;

    entries[ 8] = 0;
    entries[ 9] = 0;
    entries[10] = (fFar + fNear) * n;
    entries[11] = -1;

    entries[12] = 0;
    entries[13] = 0;
    entries[14] = (2 * fFar * fNear) * n;
    entries[15] = 0;


}

String Matrix4x4::toString(const String& start,const String& sep,const String& sepline,const String& end) const
{
    return 	start+String::toString(entries[0])+sep +String::toString(entries[1]) +sep+String::toString(entries[2]) +sep+String::toString(entries[3]) +sepline
            +String::toString(entries[4])+sep +String::toString(entries[5]) +sep+String::toString(entries[6]) +sep+String::toString(entries[7]) +sepline
            +String::toString(entries[8])+sep +String::toString(entries[9]) +sep+String::toString(entries[10])+sep+String::toString(entries[11])+sepline
            +String::toString(entries[12])+sep+String::toString(entries[13])+sep+String::toString(entries[14])+sep+String::toString(entries[15])+end;
}
/* MATH */
const float Math::PI=3.1415926535f;
const float Math::PI2=6.2831853071796f;
const float Math::PIOVER180=0.0174532925f;
const float Math::G180OVERPI=57.2957795130f;
void  Math::seedRandom(unsigned int seed)
{

    if( !seed )
        seed = (unsigned int)std::time( NULL );

    srand( seed );

}
float Math::random()
{
    return (float)rand()/(float)RAND_MAX;
}
float Math::randomRange(float min,float max)
{
    return ((float)rand()/(float)RAND_MAX)*(max-min) + min;
}
