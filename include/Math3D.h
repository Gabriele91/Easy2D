#ifndef MATH3D_H
#define MATH3D_H

#include <Config.h>
#include <EString.h>

namespace Easy2D
{
    
class Angle;
class Radian;
class Degree;
class Vector2D;
class Vector3D;
class Vector4D;
class Quaternion;
class Matrix4x4;
class Math;
typedef Vector2D Vec2;
typedef Vector3D Vec3;
typedef Vector4D Vec4;
typedef Matrix4x4 Mat4;
    
class Radian
{
    
    float rad;
    
public:
    ///////////////////////////////////////////////////////////////////////////
    explicit Radian ( float r=0 ) : rad(r) {}
    Radian ( const Degree& d );
    ///////////////////////////////////////////////////////////////////////////
    Radian& operator = ( const float& f ) { rad = f; return *this; }
    Radian& operator = ( const Radian& r ) { rad = r.rad; return *this; }
    Radian& operator = ( const Degree& d );
    ///////////////////////////////////////////////////////////////////////////
    float valueDegrees() const;
    float valueRadians() const { return rad; }
    ///////////////////////////////////////////////////////////////////////////
    const Radian& operator + () const { return *this; }
    Radian operator + ( const Radian& r ) const { return Radian ( rad + r.rad ); }
    Radian operator + ( const Degree& d ) const;
    Radian& operator += ( const Radian& r ) { rad += r.rad; return *this; }
    Radian& operator += ( const Degree& d );
    Radian operator - () const { return Radian(-rad); }
    Radian operator - ( const Radian& r ) const { return Radian ( rad - r.rad ); }
    Radian operator - ( const Degree& d ) const;
    Radian& operator -= ( const Radian& r ) { rad -= r.rad; return *this; }
    Radian& operator -= ( const Degree& d );
    Radian operator * ( float f ) const { return Radian ( rad * f ); }
    Radian operator * ( const Radian& f ) const { return Radian ( rad * f.rad ); }
    Radian& operator *= ( float f ) { rad *= f; return *this; }
    Radian operator / ( float f ) const { return Radian ( rad / f ); }
    Radian& operator /= ( float f ) { rad /= f; return *this; }

    bool operator <  ( const Radian& r ) const { return rad <  r.rad; }
    bool operator <= ( const Radian& r ) const { return rad <= r.rad; }
    bool operator == ( const Radian& r ) const { return rad == r.rad; }
    bool operator != ( const Radian& r ) const { return rad != r.rad; }
    bool operator >= ( const Radian& r ) const { return rad >= r.rad; }
    bool operator >  ( const Radian& r ) const { return rad >  r.rad; }
    ///////////////////////////////////////////////////////////////////////////
    String toString(const String& start="Radian(", const String& end=")\n") const
    {
        return start+rad+end;
    }
};

class Degree
{
    float deg;
    
public:
    ///////////////////////////////////////////////////////////////////////////
    explicit Degree ( float d=0 ) : deg(d) {}
    Degree ( const Radian& r ) : deg(r.valueDegrees()) {}
    ///////////////////////////////////////////////////////////////////////////
    Degree& operator = ( const float& f ) { deg = f; return *this; }
    Degree& operator = ( const Degree& d ) { deg = d.deg; return *this; }
    Degree& operator = ( const Radian& r ) { deg = r.valueDegrees(); return *this; }
    ///////////////////////////////////////////////////////////////////////////
    float valueDegrees() const { return deg; }
    float valueRadians() const;
    ///////////////////////////////////////////////////////////////////////////
    const Degree& operator + () const { return *this; }
    Degree operator + ( const Degree& d ) const { return Degree ( deg + d.deg ); }
    Degree operator + ( const Radian& r ) const { return Degree ( deg + r.valueDegrees() ); }
    Degree& operator += ( const Degree& d ) { deg += d.deg; return *this; }
    Degree& operator += ( const Radian& r ) { deg += r.valueDegrees(); return *this; }
    Degree operator - () const { return Degree(-deg); }
    Degree operator - ( const Degree& d ) const { return Degree ( deg - d.deg ); }
    Degree operator - ( const Radian& r ) const { return Degree ( deg - r.valueDegrees() ); }
    Degree& operator -= ( const Degree& d ) { deg -= d.deg; return *this; }
    Degree& operator -= ( const Radian& r ) { deg -= r.valueDegrees(); return *this; }
    Degree operator * ( float f ) const { return Degree ( deg * f ); }
    Degree operator * ( const Degree& f ) const { return Degree ( deg * f.deg ); }
    Degree& operator *= ( float f ) { deg *= f; return *this; }
    Degree operator / ( float f ) const { return Degree ( deg / f ); }
    Degree& operator /= ( float f ) { deg /= f; return *this; }
    
    bool operator <  ( const Degree& d ) const { return deg <  d.deg; }
    bool operator <= ( const Degree& d ) const { return deg <= d.deg; }
    bool operator == ( const Degree& d ) const { return deg == d.deg; }
    bool operator != ( const Degree& d ) const { return deg != d.deg; }
    bool operator >= ( const Degree& d ) const { return deg >= d.deg; }
    bool operator >  ( const Degree& d ) const { return deg >  d.deg; }
    ///////////////////////////////////////////////////////////////////////////
    String toString(const String& start="Degree(", const String& end=")\n") const
    {
        return start+deg+end;
    }
};
    
class Angle
{
    Radian angle;
    
public:
    
    enum InitType
    {
        RADIAN,
        DEGREE
    };
    
    Angle (){}
    Angle ( float value,InitType type)
    {
        switch (type)
        {
            case RADIAN: angle=Radian(value); break;
            case DEGREE: angle=Degree(value); break;
          //default: DEBUG_ASSERT_MSG(0,"Type of angle not valid"); break;
        }
    }
    Angle ( const Angle&  a ) : angle(a.angle) {}
    Angle ( const Radian& r ) : angle(r) {}
    Angle ( const Degree& r ) : angle(r.valueRadians()) {}
    ///////////////////////////////////////////////////////////////////////////
    static Angle radian(float r)
    {
        return Angle( Radian(r) );
    }
    static Angle degree(float d)
    {
        return Angle( Degree(d) );
    }
    ///////////////////////////////////////////////////////////////////////////
    operator Radian() const
    {
        return angle;
    }
    operator Degree() const
    {
        return Degree(angle);
    }
    ///////////////////////////////////////////////////////////////////////////
    float valueDegrees() const
    {
        return angle.valueDegrees();
    }
    float valueRadians() const
    {
        return angle.valueRadians();
    }
    ///////////////////////////////////////////////////////////////////////////
    const Angle& operator + () const { return *this; }
    Angle operator   +  ( const Angle& r ) const { return Angle ( angle + r.angle ); }
    Angle& operator +=  ( const Angle& r ) { angle += r.angle; return *this; }
    Angle operator   -  () const { return Angle(-angle); }
    Angle operator   -  ( const Angle& r ) const { return Angle ( angle - r.angle ); }
    Angle& operator -=  ( const Angle& r ) { angle -= r.angle; return *this; }
    Angle operator   *  ( float f ) const { return Angle ( angle * f ); }
    Angle operator   *  ( const Angle& f ) const { return Angle ( Radian( angle * f.angle ) ); }
    Angle& operator *=  ( float f ) { angle *= f; return *this; }
    Angle operator   /  ( float f ) const { return Angle ( Radian( angle / f ) ); }
    Angle& operator  /= ( float f ) { angle /= f; return *this; }
    
    bool operator <  ( const Angle& r ) const { return angle <  r.angle; }
    bool operator <= ( const Angle& r ) const { return angle <= r.angle; }
    bool operator == ( const Angle& r ) const { return angle == r.angle; }
    bool operator != ( const Angle& r ) const { return angle != r.angle; }
    bool operator >= ( const Angle& r ) const { return angle >= r.angle; }
    bool operator >  ( const Angle& r ) const { return angle >  r.angle; }
    ///////////////////////////////////////////////////////////////////////////
};
    
///////////////////////////////////////////////////////////////////////////
inline Angle operator * ( float a, const Angle& b )
{
    return Angle ( Radian( a * b.valueRadians() ) );
}
inline Angle operator / ( float a, const Angle& b )
{
    return Angle ( Radian( a / b.valueRadians() ) );
}
    
inline Radian operator * ( float a, const Radian& b )
{
    return Radian ( a * b.valueRadians() );
}
inline Radian operator / ( float a, const Radian& b )
{
    return Radian ( a / b.valueRadians() );
}
    
inline Degree operator * ( float a, const Degree& b )
{
    return Degree ( a * b.valueDegrees() );
}
inline Radian operator / ( float a, const Degree& b )
{
    return Degree ( a / b.valueDegrees() );
}
///////////////////////////////////////////////////////////////////////////

    
class Vector2D
{

public:

    union
    {
        struct
        {
            float x,y;
        };
        struct
        {
            float u,v;
        };
        struct
        {
            float r,g;
        };
    };

    ///////////////////////////////////////////////////////////////////////////
    static Vector2D ZERO;
    static Vector2D ONE;
    static Vector2D NEGATIVE_ONE;
    static Vector2D MIN;
    static Vector2D MAX;
    ///////////////////////////////////////////////////////////////////////////
    Vector2D():x(0),y(0) {};
    Vector2D(float x,float y):x(x),y(y) {};
    ~Vector2D() {};
    ///////////////////////////////////////////////////////////////////////////
    template <char A>
    DFORCEINLINE float to() const
    {
        switch (A)
        {
        case 'x':
        case 'r':
        case 'u':
            return x;
        case 'y':
        case 'g':
        case 'v':
            return y;

        case 1://Math::nx:
            return -x;
        case 2://Math::ny:
            return -y;

        default:
            return 0.0;
            break;
        }
    }
    template <uchar X,uchar Y>
    DFORCEINLINE Vec2 to() const
    {
        return Vec2(to<X>(),to<Y>());
    }
    ///////////////////////////////////////////////////////////////////////////
    void normalize();
    ///////////////////////////////////////////////////////////////////////////
    float  length() const;
    Radian direction() const;
    float  cross(const Vector2D& vec) const;
    float  dot(const Vector2D& vec) const;
    float  distance(const Vector2D& vec) const;
    float  distancePow2(const Vector2D& vec) const;
    Vector2D axis(const Vector2D& vec) const;
    Vector2D getNormalize() const;
    Vector2D projected(const Vector2D& axis) const;
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE float squaredLength() const
    {
        return x*x+y*y;
    };
    DFORCEINLINE void abs()
    {
        x=fabs(x);
        y=fabs(y);
    }
    DFORCEINLINE Vector2D getAbs() const
    {
        return Vector2D(fabs(x),fabs(y));
    }
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE float& operator [] (unsigned int i)
    {
        return (i%2 == 0) ? x: y;
    }
    DFORCEINLINE bool operator==(const Vector2D &v) const
    {
        return (x==v.x && y==v.y);
    }
    DFORCEINLINE bool operator!=(const Vector2D &v) const
    {
        return (x!=v.x || y!=v.y);
    }
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE Vector2D operator+(const Vector2D& v) const
    {
        return Vector2D(x+v.x,y+v.y);
    }
    DFORCEINLINE Vector2D operator+(float v) const
    {
        return Vector2D(x+v,y+v);
    }

    DFORCEINLINE Vector2D operator-(const Vector2D& v) const
    {
        return Vector2D(x-v.x,y-v.y);
    }
    DFORCEINLINE Vector2D operator-(float v) const
    {
        return Vector2D(x-v,y-v);
    }
    DFORCEINLINE Vector2D operator-(void) const
    {
        return Vector2D(-x,-y);
    }

    DFORCEINLINE Vector2D operator*(const Vector2D& v) const
    {
        return Vector2D(x*v.x,y*v.y);
    }
    DFORCEINLINE Vector2D operator*(float v) const
    {
        return Vector2D(x*v,y*v);
    }

    DFORCEINLINE Vector2D operator/(const Vector2D& v) const
    {
        return Vector2D(x/v.x,y/v.y);
    }
    DFORCEINLINE Vector2D operator/(float v) const
    {
        return Vector2D(x/v,y/v);
    }
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE const Vector2D &operator+=(const Vector2D &v)
    {
        x+=v.x;
        y+=v.y;
        return *this;
    }
    DFORCEINLINE const Vector2D &operator+=(const float  &v)
    {
        x+=v;
        y+=v;
        return *this;
    }
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE const Vector2D &operator-=(const Vector2D &v)
    {
        x-=v.x;
        y-=v.y;
        return *this;
    }
    DFORCEINLINE const Vector2D &operator-=(const float  &v)
    {
        x-=v;
        y-=v;
        return *this;
    }
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE const Vector2D &operator*=(const Vector2D &v)
    {
        x*=v.x;
        y*=v.y;
        return *this;
    }
    DFORCEINLINE const Vector2D &operator*=(const float  &v)
    {
        x*=v;
        y*=v;
        return *this;
    }
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE const Vector2D &operator/=(const Vector2D &v)
    {
        x/=v.x;
        y/=v.y;
        return *this;
    }
    DFORCEINLINE const Vector2D &operator/=(const float  &v)
    {
        x/=v;
        y/=v;
        return *this;
    }
    ///////////////////////////////////////////////////////////////////////////
    //cast to pointer to float
    operator float* ()
    {
        return &this->x;
    }
    operator const float* () const
    {
        return &this->x;
    }
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    String toString(const String& start="(",const String& sep=" ",const String& end=")\n") const;

};

template <typename T>
DFORCEINLINE Vector2D operator+(T v,const Vector2D& vt)
{
    return Vector2D(v+vt.x,v+vt.y);
}
template <typename T>
DFORCEINLINE Vector2D operator-(T v,const Vector2D& vt)
{
    return Vector2D(v-vt.x,v-vt.y);
}
template <typename T>
DFORCEINLINE Vector2D operator*(T v,const Vector2D& vt)
{
    return Vector2D(v*vt.x,v*vt.y);
}
template <typename T>
DFORCEINLINE Vector2D operator/(T v,const Vector2D& vt)
{
    return Vector2D(v/vt.x,v/vt.y);
}
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
class Vector3D
{

public:
    ///////////////////////////////////////////////////////////////////////////
    static Vector3D ZERO;
    static Vector3D ONE;
    static Vector3D NEGATIVE_ONE;
    static Vector3D MIN;
    static Vector3D MAX;
    ///////////////////////////////////////////////////////////////////////////
    union
    {
        struct
        {
            float x,y,z;
        };
        struct
        {
            float r,g,b;
        };
    };
    ///////////////////////////////////////////////////////////////////////////
    Vector3D():x(0),y(0),z(0) {};
	//Vector3D(Vector2D v, float z) :x(v.x), y(v.y), z(z) {}; 
	//Vector3D(float x, Vector2D v) :x(x), y(v.x), z(v.y) {};
	Vector3D(const Vector2D& v, float z) :x(v.x), y(v.y), z(z) {};
	Vector3D(float x, const Vector2D& v) :x(x), y(v.x), z(v.y) {};
    Vector3D(float x,float y,float z):x(x),y(y),z(z) {};
    ~Vector3D() {};
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE Vec2 xy() const
    {
        return Vec2(x,y);
	}   
	DFORCEINLINE Vec2& xy()
	{
		return *((Vec2*)&x);
	}
    DFORCEINLINE Vec2 rg() const
    {
        return Vec2(x,y);
	}
	DFORCEINLINE Vec2& rg()
	{
		return *((Vec2*)&x);
	}
    ///////////////////////////////////////////////////////////////////////////
    template <char A>
    DFORCEINLINE float to() const
    {
        switch (A)
        {
        case 'x':
        case 'r':
            return x;
        case 'y':
        case 'g':
            return y;
        case 'z':
        case 'b':
            return z;

        case 1://Math::nx:
            return -x;
        case 2://Math::ny:
            return -y;
        case 3://Math::nz:
            return -z;

        default:
            return 0.0;
            break;
        }
    }
    template <uchar X,uchar Y>
    DFORCEINLINE Vec2 to() const
    {
        return Vec2(to<X>(),to<Y>());
    }
    template <uchar X,uchar Y,uchar Z>
    DFORCEINLINE Vec3 to() const
    {
        return Vec3(to<X>(),to<Y>(),to<Z>());
    }
    ///////////////////////////////////////////////////////////////////////////
    void normalize();
    ///////////////////////////////////////////////////////////////////////////
    float length() const;
    float dot(const Vector3D& vec) const;
    float distance(const Vector3D& vec) const;
    float distancePow2(const Vector3D& vec) const;
    Vector3D cross(const Vector3D& vec) const;
    Vector3D getNormalize() const;
    void orthoNormalize(Vector3D& b)
    {
        this->normalize();
        b -= b.projectToNormal(*this);
        b.normalize();
    }
    Vector3D projectToNormal(const Vector3D& direction)
    {
        return direction * dot(direction);
    }
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE float squaredLength() const
    {
        return x*x+y*y+z*z;
    };
    DFORCEINLINE void abs()
    {
        x=fabs(x);
        y=fabs(y);
        z=fabs(z);
    }
    DFORCEINLINE Vector3D getAbs() const
    {
        return Vector3D(fabs(x),fabs(y), fabs(z));
    }
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE float& operator [] (unsigned int i)
    {
        return i==0 ? x: i==1 ? y : z;
    }
    DFORCEINLINE bool operator==(const Vector3D &v) const
    {
        return (x==v.x && y==v.y && z==v.z);
    }
    DFORCEINLINE bool operator!=(const Vector3D &v) const
    {
        return (x!=v.x || y!=v.y || z!=v.z);
    }
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE Vector3D operator+(const Vector3D& v) const
    {
        return Vector3D(x+v.x,y+v.y,z+v.z);
    }
    DFORCEINLINE Vector3D operator+(float v) const
    {
        return Vector3D(x+v,y+v,z+v);
    }

    DFORCEINLINE Vector3D operator-(const Vector3D& v) const
    {
        return Vector3D(x-v.x,y-v.y,z-v.z);
    }
    DFORCEINLINE Vector3D operator-(float v) const
    {
        return Vector3D(x-v,y-v,z-v);
    }
    DFORCEINLINE Vector3D operator-(void) const
    {
        return Vector3D(-x,-y,-z);
    }

    DFORCEINLINE Vector3D operator*(const Vector3D& v) const
    {
        return Vector3D(x*v.x,y*v.y,z*v.z);
    }
    DFORCEINLINE Vector3D operator*(float v) const
    {
        return Vector3D(x*v,y*v,z*v);
    }

    DFORCEINLINE Vector3D operator/(const Vector3D& v) const
    {
        return Vector3D(x/v.x,y/v.y,z/v.z);
    }
    DFORCEINLINE Vector3D operator/(float v) const
    {
        return Vector3D(x/v,y/v,z/v);
    }
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE const Vector3D &operator+=(const Vector3D &v)
    {
        x+=v.x;
        y+=v.y;
        z+=v.z;
        return *this;
    }
    DFORCEINLINE const Vector3D &operator+=(const float  &v)
    {
        x+=v;
        y+=v;
        z+=v;
        return *this;
    }
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE const Vector3D &operator-=(const Vector3D &v)
    {
        x-=v.x;
        y-=v.y;
        z-=v.z;
        return *this;
    }
    DFORCEINLINE const Vector3D &operator-=(const float  &v)
    {
        x-=v;
        y-=v;
        z-=v;
        return *this;
    }
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE const Vector3D &operator*=(const Vector3D &v)
    {
        x*=v.x;
        y*=v.y;
        z*=v.z;
        return *this;
    }
    DFORCEINLINE const Vector3D &operator*=(const float  &v)
    {
        x*=v;
        y*=v;
        z*=v;
        return *this;
    }
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE const Vector3D &operator/=(const Vector3D &v)
    {
        x/=v.x;
        y/=v.y;
        z/=v.z;
        return *this;
    }
    DFORCEINLINE const Vector3D &operator/=(const float  &v)
    {
        x/=v;
        y/=v;
        z/=v;
        return *this;
    }
    ///////////////////////////////////////////////////////////////////////////
    operator float* ()
    {
        return &this->x;
    }
    operator const float* () const
    {
        return &this->x;
    }
    ///////////////////////////////////////////////////////////////////////////
    String toString(const String& start="(",const String& sep=" ",const String& end=")\n") const;

};

template <typename T>
DFORCEINLINE Vector3D operator+(T v,const Vector3D& vt)
{
    return Vector3D(v+vt.x,v+vt.y,v+vt.z);
}
template <typename T>
DFORCEINLINE Vector3D operator-(T v,const Vector3D& vt)
{
    return Vector3D(v-vt.x,v-vt.y,v-vt.z);
}
template <typename T>
DFORCEINLINE Vector3D operator*(T v,const Vector3D& vt)
{
    return Vector3D(v*vt.x,v*vt.y,v*vt.z);
}
template <typename T>
DFORCEINLINE Vector3D operator/(T v,const Vector3D& vt)
{
    return Vector3D(v/vt.x,v/vt.y,v/vt.z);
}
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
class Vector4D
{

public:
    ///////////////////////////////////////////////////////////////////////////
    static Vector4D ZERO;
    static Vector4D ONE;
    static Vector4D NEGATIVE_ONE;
    static Vector4D MIN;
    static Vector4D MAX;
    ///////////////////////////////////////////////////////////////////////////

#ifdef SIMD_SSE2

    VSALIGNED(16)
    union
    {
        struct
        {
            float x,y,z,w;
        };
        struct
        {
            float r,g,b,a;
        };
        __m128 row;
    }
    GCCALIGNED(16) ;

    DFORCEINLINE Vector4D& operator = (const Vector4D& v)
    {
        row=v.row;
        return *this;
    }
#else
    union
    {
        struct
        {
            float x,y,z,w;
        };
        struct
        {
            float r,g,b,a;
        };
    };
#endif
    ///////////////////////////////////////////////////////////////////////////
    Vector4D():x(0),y(0),z(0),w(0) {};
    Vector4D(float x,float y,float z,float w):x(x),y(y),z(z),w(w) {};
	/*
	Vector4D(Vector2D v, float z, float w) :x(v.x), y(v.y), z(z), w(w) {};
	Vector4D(float x, Vector2D v, float w) :x(x), y(v.x), z(v.y), w(w) {};
	Vector4D(float x, float y, Vector2D v) :x(x), y(y), z(v.x), w(v.y) {};
    Vector4D(Vector2D v1,Vector2D v2):x(v1.x),y(v1.y),z(v2.x),w(v2.y) {};
	*/
	Vector4D(const Vector2D& v, float z, float w) :x(v.x), y(v.y), z(z), w(w) {};
	Vector4D(float x, const Vector2D& v, float w) :x(x), y(v.x), z(v.y), w(w) {};
	Vector4D(float x, float y, const Vector2D& v) :x(x), y(y), z(v.x), w(v.y) {};
	Vector4D(const Vector2D& v1, const Vector2D& v2) :x(v1.x), y(v1.y), z(v2.x), w(v2.y) {};

	Vector4D(const Vector3D& v, float w) :x(v.x), y(v.y), z(v.z), w(w) {};
    ~Vector4D() {};
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE Vec2 xy() const
    {
        return Vec2(x,y);
	}
	DFORCEINLINE Vec2& xy()
	{
		return *((Vec2*)&x);
	}
    DFORCEINLINE Vec2 rg() const
    {
        return Vec2(x,y);
	}
	DFORCEINLINE Vec2& rg()
	{
		return *((Vec2*)&x);
	}
    DFORCEINLINE Vec3 xyz() const
    {
        return Vec3(x,y,z);
	}
	DFORCEINLINE Vec3& xyz()
	{
		return *((Vec3*)&x);
	}
    DFORCEINLINE Vec3 rgb() const
    {
        return Vec3(r,g,b);
	}
	DFORCEINLINE Vec3& rgb()
	{
		return *((Vec3*)&x);
	}
    ///////////////////////////////////////////////////////////////////////////
    template <char A>
    DFORCEINLINE float to() const
    {
        switch (A)
        {
        case 'x':
        case 'r':
            return x;
        case 'y':
        case 'g':
            return y;
        case 'z':
        case 'b':
            return z;
        case 'w':
        case 'a':
            return w;

        case 1://Math::nx:
            return -x;
        case 2://Math::ny:
            return -y;
        case 3://Math::nz:
            return -z;
        case 4://Math::nw:
            return -w;
        default:
            return 0.0;
            break;
        }
    }
    template <uchar X,uchar Y>
    DFORCEINLINE Vec2 to() const
    {
        return Vec2(to<X>(),to<Y>());
    }
    template <uchar X,uchar Y,uchar Z>
    DFORCEINLINE Vec3 to() const
    {
        return Vec3(to<X>(),to<Y>(),to<Z>());
    }
    template <uchar X,uchar Y,uchar Z,uchar W>
    DFORCEINLINE Vec4 to() const
    {
        return Vec4(to<X>(),to<Y>(),to<Z>(),to<W>());
    }
    ///////////////////////////////////////////////////////////////////////////
    void normalize();
    ///////////////////////////////////////////////////////////////////////////
    float length() const;
    float dot(const Vector4D& vec) const;
    float distance(const Vector4D& vec) const;
    float distancePow2(const Vector4D& vec) const;
    Vector4D getNormalize() const;
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE float squaredLength() const
    {
        return x*x+y*y+z*z+w*w;
    };
    DFORCEINLINE void abs()
    {
        x=fabs(x);
        y=fabs(y);
        z=fabs(z);
        w=fabs(w);
    }
    DFORCEINLINE Vector4D getAbs()const
    {
        return Vector4D(fabs(x),fabs(y), fabs(z), fabs(w));
    }
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE float& operator [] (unsigned int i)
    {
        return i==0 ? x: i==1 ? y : i==2 ? z : w;
    }
    DFORCEINLINE bool operator==(const Vector4D &v) const
    {
        return (x==v.x && y==v.y && z==v.z && w==v.w);
    }
    DFORCEINLINE bool operator!=(const Vector4D &v) const
    {
        return (x!=v.x || y!=v.y || z!=v.z || w!=v.w);
    }
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE Vector4D operator+(const Vector4D& v) const
    {
        return Vector4D(x+v.x,y+v.y,z+v.z,w+v.w);
    }
    DFORCEINLINE Vector4D operator+(float v) const
    {
        return Vector4D(x+v,y+v,z+v,w+v);
    }

    DFORCEINLINE Vector4D operator-(const Vector4D& v) const
    {
        return Vector4D(x-v.x,y-v.y,z-v.z,w-v.w);
    }
    DFORCEINLINE Vector4D operator-(float v) const
    {
        return Vector4D(x-v,y-v,z-v,w-v);
    }
    DFORCEINLINE Vector4D operator-(void) const
    {
        return Vector4D(-x,-y,-z,-w);
    }

    DFORCEINLINE Vector4D operator*(const Vector4D& v) const
    {
        return Vector4D(x*v.x,y*v.y,z*v.z,w*v.w);
    }
    DFORCEINLINE Vector4D operator*(float v) const
    {
        return Vector4D(x*v,y*v,z*v,w*v);
    }

    DFORCEINLINE Vector4D operator/(const Vector4D& v) const
    {
        return Vector4D(x/v.x,y/v.y,z/v.z,w/v.w);
    }
    DFORCEINLINE Vector4D operator/(float v) const
    {
        return Vector4D(x/v,y/v,z/v,w/v);
    }
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE const Vector4D &operator+=(const Vector4D &v)
    {
        x+=v.x;
        y+=v.y;
        z+=v.z;
        w+=v.w;
        return *this;
    }
    DFORCEINLINE const Vector4D &operator+=(const float  &v)
    {
        x+=v;
        y+=v;
        z+=v;
        w+=v;
        return *this;
    }
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE const Vector4D &operator-=(const Vector4D &v)
    {
        x-=v.x;
        y-=v.y;
        z-=v.z;
        w-=v.w;
        return *this;
    }
    DFORCEINLINE const Vector4D &operator-=(const float  &v)
    {
        x-=v;
        y-=v;
        z-=v;
        w-=v;
        return *this;
    }
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE const Vector4D &operator*=(const Vector4D &v)
    {
        x*=v.x;
        y*=v.y;
        z*=v.z;
        w*=v.w;
        return *this;
    }
    DFORCEINLINE const Vector4D &operator*=(const float  &v)
    {
        x*=v;
        y*=v;
        z*=v;
        w*=v;
        return *this;
    }
    ///////////////////////////////////////////////////////////////////////////
    DFORCEINLINE const Vector4D &operator/=(const Vector4D &v)
    {
        x/=v.x;
        y/=v.y;
        z/=v.z;
        w/=v.w;
        return *this;
    }
    DFORCEINLINE const Vector4D &operator/=(const float  &v)
    {
        x/=v;
        y/=v;
        z/=v;
        w/=v;
        return *this;
    }
    ///////////////////////////////////////////////////////////////////////////
    operator float* ()
    {
        return &this->x;
    }
    operator const float* () const
    {
        return &this->x;
    }
    ///////////////////////////////////////////////////////////////////////////
    String toString(const String& start="(",const String& sep=" ",const String& end=")\n") const;

};

template <typename T>
DFORCEINLINE Vector4D operator+(T v,const Vector4D& vt)
{
    return Vector4D(v+vt.x,v+vt.y,v+vt.z,v+vt.w);
}
template <typename T>
DFORCEINLINE Vector4D operator-(T v,const Vector4D& vt)
{
    return Vector4D(v-vt.x,v-vt.y,v-vt.z,v-vt.w);
}
template <typename T>
DFORCEINLINE Vector4D operator*(T v,const Vector4D& vt)
{
    return Vector4D(v*vt.x,v*vt.y,v*vt.z,v*vt.w);
}
template <typename T>
DFORCEINLINE Vector4D operator/(T v,const Vector4D& vt)
{
    return Vector4D(v/vt.x,v/vt.y,v/vt.z,v/vt.w);
}
///////////////////////////////////////////////////////////////////////////
class AABox2
{

    //box structure
    Vec2 min;
    Vec2 max;

public:
    //costructor
    AABox2(const Vec2& center,Vec2 size);
    AABox2();
    //destructor
    ~AABox2();
    //setting
    void setBox(const Vec2& center,Vec2 size);
    void setRegion(const AABox2& aabox);
    void addPoint(const Vec2& point);
    void addBox(const AABox2& aabox);
    //getter
    Vec2  getCenter() const
    {
        return (min+max)*0.5;
    }
    Vec2  getSize() const
    {
        return (max-min)*0.5;
    }
    const Vec2&  getMax() const
    {
        return max;
    }
    const Vec2&  getMin() const
    {
        return min;
    }
    void setMax(const Vec2& m) 
    {
        max=m;
    }
    void setMin(const Vec2& m) 
    {
        min=m;
    }
    Vec2&  getMax()
    {
        return max;
    }
    Vec2&  getMin()
    {
        return min;
    }
    bool  isIntersection(const Vec2& point) const;
    bool  isIntersection(const AABox2& aabb2) const;
    AABox2 applay(const Matrix4x4& m4) const;

};
///////////////////////////////////////////////////////////////////////////
struct Transform2D
{
    //
    Transform2D()
    :alpha(Radian(0))
    ,scale(1.0f,1.0f)
    {
    }
    
    Transform2D(const Vector2D& position,
                Angle alpha,
                const Vector2D& scale)
                :position(position),
                 alpha(alpha),
                 scale(scale)
    {
    }
    
    //values
    Vector2D position;
    Angle    alpha;
    Vector2D scale;
    //operator
    bool operator==(const Transform2D& t2d) const
    {
        return alpha==t2d.alpha &&
               position==t2d.position &&
               scale==t2d.scale;
    }
    bool operator!=(const Transform2D& t2d) const
    {
        return alpha!=t2d.alpha ||
               position!=t2d.position ||
               scale!=t2d.scale;
    }
    //cast
    operator float*()
    {
        return &position.x;
    }
    operator const float*() const
    {
        return &position.x;
    }
};
///////////////////////////////////////////////////////////////////////////
class Matrix4x4
{
public:

#ifdef SIMD_SSE2
    VSALIGNED(16)
    union
    {

        struct
        {
            __m128 row0,row1,row2,row3;
        };

        float entries[16];

        struct
        {
            float m00, m01, m02, m03;
            float m10, m11, m12, m13;
            float m20, m21, m22, m23;
            float m30, m31, m32, m33;
        };

    }
    GCCALIGNED(16) ;
    DFORCEINLINE Matrix4x4& operator = (const Matrix4x4& m)
    {
        row0=m.row0;
        row1=m.row1,
        row2=m.row2;
        row3=m.row3;
        return *this;
    }
#else
    union
    {

        float entries[16];
        struct
        {
            float m00, m01, m02, m03;
            float m10, m11, m12, m13;
            float m20, m21, m22, m23;
            float m30, m31, m32, m33;
        };

    };
#endif
    //statics
    static Matrix4x4 IDENTITY;
    static Matrix4x4 ZERO;
    //constructors
    Matrix4x4();
    Matrix4x4(const Matrix4x4 &m4x4);
    Matrix4x4(float* m4x4);
    Matrix4x4(float e0,float e1,float e2,float e3,
              float e4,float e5,float e6,float e7,
              float e8,float e9,float e10,float e11,
              float e12,float e13,float e14,float e15);
    //destructor
    virtual ~Matrix4x4() {};
    ///identity
    void identity();
    ///set all values to 0
    void zero();
    ///matrix  inverse
    void inverse();
    ///matrix inverse (only 2D transformation)
    void inverse2D();
    //get matrix determinant
    float getDeterminant() const;
    ///matrix multiplication
    Matrix4x4 mul(const Matrix4x4 &m4x4) const;
    ///matrix multiplication (only 2D transformation)
    Matrix4x4 mul2D(const Matrix4x4 &m4x4) const;
    ///matrix * vector
    Vector4D mul(const Vector4D &v4) const;
    ///matrix * vector (only 2D transformation)
    Vector2D mul2D(const Vector2D &v2) const;
    ///return matrix inverse
    Matrix4x4 getInverse() const;
    ///return matrix inverse (only 2D transformation)
    Matrix4x4 getInverse2D() const;
    ///return matrix transposition
    Matrix4x4 getTranspose() const;
    ///set scale
    void setScale(const Vector3D &v3);
    void setScale(const Vector2D &v2);
    ///concatenate trasformation
    void addScale(const Vector3D &v3);
    void addScale(const Vector2D &v2);
    ///force scale trasformation
    void unsafeScale(const Vector3D &v3);
    void unsafeScale(const Vector2D &v2);
    ///return scale
    Vector3D getScale3D() const;
    Vector2D getScale2D() const;
    ///set translation
    void setTranslation(const Vector3D &v3);
    void setTranslation(const Vector2D &v2);
    ///concatenate trasformation
    void addTranslation( const Vector3D &v3 );
    void addTranslation( const Vector2D &v2 );
    void addTranslationOnX( float distance );
    void addTranslationOnY( float distance );
    void addTranslationOnZ( float distance );
    ///force translation translation
    void unsafeTranslation(const Vector3D &v3);
    void unsafeTranslation(const Vector2D &v2);
    ///return translation
    Vector3D getTranslation3D() const;
    Vector2D getTranslation2D() const;
    ///add a euler rotarion
    void addEulerRotation(const Vec3& euler);
    ///set pitch
    void setRotX(Angle x);
    ///set yaw
    void setRotY(Angle y);
    ///set roll
    void setRotZ(Angle z);
    ///return pitch
    Angle getRotX() const;
    ///return yaw
    Angle getRotY() const;
    ///return roll
    Angle getRotZ() const;
    ///rotation
    Vec3  getRotation() const;
    ///add a tranform
    void setTransform2D(const Transform2D& t2d);
    ///set orthogonal transformation (projection matrix)
    void setOrtho(float left, float right, float bottom,float top, float n, float f);
    ///set projection transformation (projection matrix)
    void setPerspective(float left, float right, float bottom,float top, float n, float f);
    void setPerspective(float fovy, float aspect, float n, float f);
    //operators:
    DFORCEINLINE float& operator[](int i)
    {
        return entries[i];
    }
    DFORCEINLINE float& operator()(int x,int y)
    {
        return entries[x+(y*4)];
    }
    DFORCEINLINE float operator[](int i) const
    {
        return entries[i];
    }
    DFORCEINLINE float operator()(int x,int y) const
    {
        return entries[x+(y*4)];
    }
    //
    operator float* ()
    {
        return (float*)entries;
    }
    operator const float* () const
    {
        return (const float*)entries;
    }
    ///////////////////////////////////////////////////////////////////////////
    String toString(const String& start="(",const String& sep=" ",const String& sepline=" ",const String& end=")\n") const;

};
///////////////////////////////////////////////////////////////////////////
class Math
{
public:
    //enum attribute
    enum VecAttribute
    {
        x='x',y='y',z='z',w='w',
        r='r',g='g',b='b',a='a',
        u='u',v='v',

        nx=1,ny=2,nz=3,nw=4,
        nr=1,ng=2,nb=3,na=4,
        nu=1,nv=2
    };
    //const values
    static const float PI;
    static const float PI2;
    static const float PIOVER180;
    static const float G180OVERPI;
    //radians and degrees
    static DFORCEINLINE float torad(float deg)
    {
        return deg*PIOVER180;
    }
    static DFORCEINLINE float todeg(float rad)
    {
        return rad*G180OVERPI;
    }
    //angles
    static DFORCEINLINE Angle normaliseOrientation(Angle angle)
    {
        float  rotation = std::fmod(angle.valueRadians(),(float)Math::PI2);
        return Angle(Radian(rotation < 0 ? rotation+=Math::PI2 : rotation));
    }
    
    static DFORCEINLINE float sin(Angle angle)
    {
        return std::sin(angle.valueRadians());
    }
    static DFORCEINLINE float cos(Angle angle)
    {
        return std::cos(angle.valueRadians());
    }
    
    static DFORCEINLINE float sin(Radian angle)
    {
        return std::sin(angle.valueRadians());
    }
    static DFORCEINLINE float cos(Radian angle)
    {
        return std::cos(angle.valueRadians());
    }
    
    static DFORCEINLINE float sin(Degree angle)
    {
        return std::sin(angle.valueRadians());
    }
    static DFORCEINLINE float cos(Degree angle)
    {
        return std::cos(angle.valueRadians());
    }
    
    //vector swap
    static DFORCEINLINE void memswap( byte *a, byte *b, size_t sizeBytes ){
        size_t sizeTrunc = sizeBytes & ~(sizeof(size_t) - 1);
        byte  *aTruncEnd = a + sizeTrunc;
        byte  *aEnd = a + sizeBytes;
        size_t tmpT;
        byte   tmp1;
        
        while ( a != aTruncEnd ){
            tmpT = *((size_t *)b);
            *((size_t *)b) = *((size_t *)a);
            *((size_t *)a) = tmpT;
            a += sizeof(size_t);
            b += sizeof(size_t);
        }
        
        while(a!=aEnd){
            tmp1 = *b;
            *b = *a;
            *a = tmp1;
            ++a;
            ++b;
        }
    }
    static DFORCEINLINE void memcpy( byte *destination, const byte *source, size_t sizeBytes ){
        size_t sizeTrunc = sizeBytes & ~(sizeof(size_t) - 1);
        byte  *destinationTruncEnd = destination + sizeTrunc;
        byte  *destinationEnd = destination + sizeBytes;
        
        while ( destination != destinationTruncEnd ){
            *((size_t *)destination) = *((size_t *)source);
            destination += sizeof(size_t);
            source += sizeof(size_t);
        }
        while(destination!=destinationEnd){
            *destination++ = *source++;
        }
    }
    //fast swap
    template<typename T>
    static DFORCEINLINE void swap(T& x,T& y)
    {
        T temp = std::move(x);
        x = std::move(y);
        y = std::move(temp);
    }
    //infinite
    template<typename T>
    static DFORCEINLINE bool isinf(T x)
    {
#ifdef COMPILER_VISUAL_STUDIO
        return _finite(x)==0;
#else
        return std::isinf(x);
#endif
    }
    //nan
    template<typename T>
    static DFORCEINLINE bool isnan(T x)
    {
#ifdef COMPILER_VISUAL_STUDIO
        return _isnan(x);
#else
        return std::isnan(x);
#endif
    }
    //min
    template<typename T>
    static DFORCEINLINE T min(T x,T y)
    {
        return x>y?y:x;
    }
    static DFORCEINLINE Vector2D min(Vector2D v1,Vector2D v2)
    {
        return Vector2D(min(v1.x,v2.x),min(v1.y,v2.y));
    }
    static DFORCEINLINE Vector2D min(const Vector2D& v1,const Vector2D& v2)
    {
        return Vector2D(min(v1.x,v2.x),min(v1.y,v2.y));
    }
    static DFORCEINLINE Vector3D min(Vector3D v1,Vector3D v2)
    {
        return Vector3D(min(v1.x,v2.x),min(v1.y,v2.y),min(v1.z,v2.z));
    }
    static DFORCEINLINE Vector3D min(const Vector3D& v1,const Vector3D& v2)
    {
        return Vector3D(min(v1.x,v2.x),min(v1.y,v2.y),min(v1.z,v2.z));
    }
    //max
    template<class T>
    static DFORCEINLINE T max(T x,T y)
    {
        return x>y?x:y;
    }
    static DFORCEINLINE Vector2D max(Vector2D v1,Vector2D v2)
    {
        return Vector2D(max(v1.x,v2.x),max(v1.y,v2.y));
    }
    static DFORCEINLINE Vector2D max(const Vector2D& v1,const Vector2D& v2)
    {
        return Vector2D(max(v1.x,v2.x),max(v1.y,v2.y));
    }
    static DFORCEINLINE Vector3D max(Vector3D v1,Vector3D v2)
    {
        return Vector3D(max(v1.x,v2.x),max(v1.y,v2.y),max(v1.z,v2.z));
    }
    static DFORCEINLINE Vector3D max(const Vector3D& v1,const Vector3D& v2)
    {
        return Vector3D(max(v1.x,v2.x),max(v1.y,v2.y),max(v1.z,v2.z));
    }
    //min max list
#ifdef COMPILER_VISUAL_STUDIO
#include "MinMaxList.h"
#else
    template<typename T,typename ...A>
    static DFORCEINLINE T min(T x,A... a)
    {
        return min(x,min(a...));
    }
    template<typename T,typename ...A>
    static DFORCEINLINE T max(T x,A... a)
    {
        return max(x,max(a...));
    }
#endif
    //lerp==linear
    template <class T>
    static DFORCEINLINE T lerp( const T& left, const T& right, float t )
    {
        return (T)(left + ( right - left ) * t);
    }
    template <class T>
    static DFORCEINLINE T linear( const T& left, const T& right, float t )
    {
        return (T)(left + ( right - left ) * t);
    }
    template <class T>
    static DFORCEINLINE T quadratic( const T& left, const T& right, float t )
    {
        return (T)( linear( linear(left,right,t), linear(left,right,t), t) );
    }
    template <class T>
    static DFORCEINLINE T cubic( const T& left, const T& right, float t )
    {
        return (T)( linear( quadratic(left,right,t), quadratic(left,right,t), t) );
    }
    //clamp
    template <class T>
    static DFORCEINLINE T clamp( const T& n, const T& max, const T& min )
    {
        return n>max ? max : ( n<min ? min : n );
    }
    //saturate
    template <class T>
    static DFORCEINLINE T saturate( const T& value )
    {
        return clamp(value,1,0);
    }
    //power of 2 test
    template <class T>
    static DFORCEINLINE bool isPowerOfTwo(T x)
    {
        return (x != 0) && ((x & (x - 1)) == 0);
    }
    static DFORCEINLINE uint nextPowerOfTwo(uint x)
    {
        x--;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        x++;
        return x;
    }
    //prime
    static DFORCEINLINE bool prime(unsigned int m)
    {
        unsigned int i,j;
        if (m < 2) return false;
        if (m == 2) return true;
        if (!(m & 1)) return false;
        if (m % 3 == 0) return (m == 3);
        for (i=5; (j=i*i), j <= m && j > i; i += 6)
        {
            if (m %   i   == 0) return false;
            if (m % (i+2) == 0) return false;
        }
        return true;
    }
    //sup multiple Of X
    static DFORCEINLINE size_t multipleOfX(size_t size, size_t x) //constexpr
    {
        return ((size / x) + ((size % x) != 0)) * x;
    }
    //random values
    static void seedRandom(unsigned int seed=0) ;
    static float random();
    static float randomRange(float min,float max);
    ///////////////////////////////////////////////////
    //fast factorial
    template <int n>
    struct factorial
    {
        //static
        enum { value = n * factorial<n - 1>::value };
        //dynamic
        int get()
        {
            return fac(n);
        }

    private:
        int fac(int x)
        {
            return x<1?1:x*fac(x-1);
        }

    };
    ///////////////////////////////////////////////////
    //fast fibonacci
    template <int n>
    struct fibonacci
    {
        //static
        enum { value = fibonacci<n - 2>::value + fibonacci<n - 1>::value };
        //dynamic
        int get()
        {
            return fib(n);
        }

    private:
        int fib(int x)
        {
            return x<2?1:fib(x-2)+fib(x-1);
        }

    };
};

template <>
struct Math::factorial<0>
{
    enum { value = 1 };
};
template<>
struct Math::fibonacci<0>
{
    enum { value = 1 };
};
template<>
struct Math::fibonacci<1>
{
    enum { value = 1 };
};
}

#endif
