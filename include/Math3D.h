#ifndef MATH3D_H
#define MATH3D_H

#include <Config.h>
#include <EString.h>

namespace Easy2D{

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

	class Vector2D{

	public:

		union
		{
			struct {float x,y;};
			struct {float u,v;};
			struct {float r,g;};
		};

		///////////////////////////////////////////////////////////////////////////
		static Vector2D ZERO;
		static Vector2D ONE;
		static Vector2D NEGATIVE_ONE;
		static Vector2D MIN;
		static Vector2D MAX;
		///////////////////////////////////////////////////////////////////////////
		Vector2D():x(0),y(0){};
		Vector2D(float x,float y):x(x),y(y){};
		~Vector2D(){};
		///////////////////////////////////////////////////////////////////////////
		template <char A>
		DFORCEINLINE float to() const{
			switch (A)
			{
				case 'x': case 'r': case 'u':  return x;
				case 'y': case 'g': case 'v': return y;

				case 1://Math::nx:
				    return -x;
				case 2://Math::ny:
				    return -y;

				default: return 0.0; break;
			}
		}
		template <uchar X,uchar Y>
		DFORCEINLINE Vec2 to() const{
			return Vec2(to<X>(),to<Y>());
		}
		///////////////////////////////////////////////////////////////////////////
		void normalize();
		///////////////////////////////////////////////////////////////////////////
		float length() const;
		float direction() const;
		float cross(const Vector2D& vec) const;
		float dot(const Vector2D& vec) const;
		float distance(const Vector2D& vec) const;
		float distancePow2(const Vector2D& vec) const;
		Vector2D axis(const Vector2D& vec) const;
		Vector2D getNormalize() const;
		Vector2D projected(const Vector2D& axis) const;
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE float squaredLength(){ return x*x+y*y; };
		DFORCEINLINE void abs(){ x=fabs(x); y=fabs(y); }
		DFORCEINLINE Vector2D getAbs() const { return Vector2D(fabs(x),fabs(y)); }
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE float& operator [] (unsigned int i) { return (i%2 == 0) ? x: y; }
		DFORCEINLINE bool operator==(const Vector2D &v) const { return (x==v.x && y==v.y);	}
		DFORCEINLINE bool operator!=(const Vector2D &v) const { return (x!=v.x || y!=v.y);	}
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE Vector2D operator+(const Vector2D& v) const {return Vector2D(x+v.x,y+v.y);}
		DFORCEINLINE Vector2D operator+(float v) const {return Vector2D(x+v,y+v);}

		DFORCEINLINE Vector2D operator-(const Vector2D& v) const {return Vector2D(x-v.x,y-v.y);}
		DFORCEINLINE Vector2D operator-(float v) const {return Vector2D(x-v,y-v);}
		DFORCEINLINE Vector2D operator-(void) const {return Vector2D(-x,-y);}

		DFORCEINLINE Vector2D operator*(const Vector2D& v) const {return Vector2D(x*v.x,y*v.y);}
		DFORCEINLINE Vector2D operator*(float v) const {return Vector2D(x*v,y*v);}

		DFORCEINLINE Vector2D operator/(const Vector2D& v) const {return Vector2D(x/v.x,y/v.y);}
		DFORCEINLINE Vector2D operator/(float v) const {return Vector2D(x/v,y/v);}
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE const Vector2D &operator+=(const Vector2D &v){ x+=v.x;
									y+=v.y;
									return *this; }
		DFORCEINLINE const Vector2D &operator+=(const float  &v){ x+=v;
                            		 y+=v;
                            		 return *this; }
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE const Vector2D &operator-=(const Vector2D &v){ x-=v.x;
									y-=v.y;
									return *this; }
		DFORCEINLINE const Vector2D &operator-=(const float  &v){ x-=v;
                            		 y-=v;
                            		return *this; }
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE const Vector2D &operator*=(const Vector2D &v){ x*=v.x;
									y*=v.y;
									return *this; }
		DFORCEINLINE const Vector2D &operator*=(const float  &v){ x*=v;
                            		 y*=v;
                            		 return *this; }
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE const Vector2D &operator/=(const Vector2D &v){ x/=v.x;
									y/=v.y;
									return *this; }
		DFORCEINLINE const Vector2D &operator/=(const float  &v){ x/=v;
                            		 y/=v;
                            		 return *this; }
		///////////////////////////////////////////////////////////////////////////
		//cast to pointer to float
		operator float* ()  {return &this->x;}
		operator const float* () const {return &this->x;}
		///////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////
		String toString(const String& start="(",const String& sep=" ",const String& end=")\n") const;

	};
    
    template <typename T>
    DFORCEINLINE Vector2D operator+(T v,const Vector2D& vt){
        return Vector2D(v+vt.x,v+vt.y);
    }
    template <typename T>
    DFORCEINLINE Vector2D operator-(T v,const Vector2D& vt){
        return Vector2D(v-vt.x,v-vt.y);
    }
    template <typename T>
    DFORCEINLINE Vector2D operator*(T v,const Vector2D& vt){
        return Vector2D(v*vt.x,v*vt.y);
    }
    template <typename T>
    DFORCEINLINE Vector2D operator/(T v,const Vector2D& vt){
        return Vector2D(v/vt.x,v/vt.y);
    }
	///////////////////////////////////////////////////////////////////////////////
    
    
	///////////////////////////////////////////////////////////////////////////////
	class Vector3D{

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
			struct {float x,y,z;};
			struct {float r,g,b;};
		};
		///////////////////////////////////////////////////////////////////////////
		Vector3D():x(0),y(0),z(0){};
		Vector3D(Vector2D v,float z):x(v.x),y(v.y),z(z){};
		Vector3D(float x,float y,float z):x(x),y(y),z(z){};
		~Vector3D(){};
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE Vec2 xy() const{
			return Vec2(x,y);
		}
		DFORCEINLINE Vec2 rg() const{
			return Vec2(x,y);
		}
		///////////////////////////////////////////////////////////////////////////
		template <char A>
		DFORCEINLINE float to() const{
			switch (A)
			{
				case 'x': case 'r': return x;
				case 'y': case 'g': return y;
				case 'z': case 'b': return z;

				case 1://Math::nx:
                    return -x;
				case 2://Math::ny:
				    return -y;
				case 3://Math::nz:
				    return -z;

				default: return 0.0; break;
			}
		}
		template <uchar X,uchar Y>
		DFORCEINLINE Vec2 to() const{
			return Vec2(to<X>(),to<Y>());
		}
		template <uchar X,uchar Y,uchar Z>
		DFORCEINLINE Vec3 to() const{
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
		void orthoNormalize(Vector3D& b){
			 this->normalize();
			 b -= b.projectToNormal(*this);
			 b.normalize();
		}
		Vector3D projectToNormal(const Vector3D& direction){
			return direction * dot(direction);
		}
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE float squaredLength(){ return x*x+y*y+z*z; };
		DFORCEINLINE void abs(){ x=fabs(x); y=fabs(y); z=fabs(z); }
		DFORCEINLINE Vector3D getAbs() const { return Vector3D(fabs(x),fabs(y), fabs(z)); }
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE float& operator [] (unsigned int i) { return i==0 ? x: i==1 ? y : z; }
		DFORCEINLINE bool operator==(const Vector3D &v) const { return (x==v.x && y==v.y && z==v.z);	}
		DFORCEINLINE bool operator!=(const Vector3D &v) const { return (x!=v.x || y!=v.y || z!=v.z);	}
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE Vector3D operator+(const Vector3D& v) const {return Vector3D(x+v.x,y+v.y,z+v.z);}
		DFORCEINLINE Vector3D operator+(float v) const {return Vector3D(x+v,y+v,z+v);}

		DFORCEINLINE Vector3D operator-(const Vector3D& v) const {return Vector3D(x-v.x,y-v.y,z-v.z);}
		DFORCEINLINE Vector3D operator-(float v) const {return Vector3D(x-v,y-v,z-v);}
		DFORCEINLINE Vector3D operator-(void) const {return Vector3D(-x,-y,-z);}

		DFORCEINLINE Vector3D operator*(const Vector3D& v) const {return Vector3D(x*v.x,y*v.y,z*v.z);}
		DFORCEINLINE Vector3D operator*(float v) const {return Vector3D(x*v,y*v,z*v);}

		DFORCEINLINE Vector3D operator/(const Vector3D& v) const {return Vector3D(x/v.x,y/v.y,z/v.z);}
		DFORCEINLINE Vector3D operator/(float v) const {return Vector3D(x/v,y/v,z/v);}
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE const Vector3D &operator+=(const Vector3D &v){
			x+=v.x;
			y+=v.y;
			z+=v.z;
			return *this;
		}
		DFORCEINLINE const Vector3D &operator+=(const float  &v){
			x+=v;
			y+=v;
			z+=v;
			return *this;
		}
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE const Vector3D &operator-=(const Vector3D &v){
			x-=v.x;
			y-=v.y;
			z-=v.z;
			return *this;
		}
		DFORCEINLINE const Vector3D &operator-=(const float  &v){
			x-=v;
			y-=v;
			z-=v;
			return *this;
		}
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE const Vector3D &operator*=(const Vector3D &v){
			x*=v.x;
			y*=v.y;
			z*=v.z;
			return *this;
		}
		DFORCEINLINE const Vector3D &operator*=(const float  &v){
			x*=v;
			y*=v;
			z*=v;
			return *this;
		}
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE const Vector3D &operator/=(const Vector3D &v){
			x/=v.x;
			y/=v.y;
			z/=v.z;
			return *this;
		}
		DFORCEINLINE const Vector3D &operator/=(const float  &v){
			x/=v;
			y/=v;
			z/=v;
			return *this;
		}
		///////////////////////////////////////////////////////////////////////////
		operator float* ()  {return &this->x;}
		operator const float* () const {return &this->x;}
		///////////////////////////////////////////////////////////////////////////
		String toString(const String& start="(",const String& sep=" ",const String& end=")\n") const;

	};
    
    template <typename T>
    DFORCEINLINE Vector3D operator+(T v,const Vector3D& vt){
        return Vector3D(v+vt.x,v+vt.y,v+vt.z);
    }
    template <typename T>
    DFORCEINLINE Vector3D operator-(T v,const Vector3D& vt){
        return Vector3D(v-vt.x,v-vt.y,v-vt.z);
    }
    template <typename T>
    DFORCEINLINE Vector3D operator*(T v,const Vector3D& vt){
        return Vector3D(v*vt.x,v*vt.y,v*vt.z);
    }
    template <typename T>
    DFORCEINLINE Vector3D operator/(T v,const Vector3D& vt){
        return Vector3D(v/vt.x,v/vt.y,v/vt.z);
    }
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	class Vector4D{

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
			struct {float x,y,z,w;};
			struct {float r,g,b,a;};
            __m128 row;
		}
        GCCALIGNED(16) ;
        
        DFORCEINLINE Vector4D& operator = (const Vector4D& v){
            row=v.row;
            return *this;
        }
#else
		union
		{
			struct {float x,y,z,w;};
			struct {float r,g,b,a;};
		};
#endif
		///////////////////////////////////////////////////////////////////////////
		Vector4D():x(0),y(0),z(0),w(0){};
		Vector4D(float x,float y,float z,float w):x(x),y(y),z(z),w(w){};
		Vector4D(Vector2D v,float z,float w):x(v.x),y(v.y),z(z),w(w){};
		Vector4D(const Vector3D& v,float w):x(v.x),y(v.y),z(v.z),w(w){};
		~Vector4D(){};
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE Vec2 xy() const{
			return Vec2(x,y);
		}
		DFORCEINLINE Vec2 rg() const{
			return Vec2(x,y);
		}
		DFORCEINLINE Vec3 xyz() const{
			return Vec3(x,y,z);
		}
		DFORCEINLINE Vec3 rgb() const{
			return Vec3(r,g,b);
		}

		///////////////////////////////////////////////////////////////////////////
		template <char A>
		DFORCEINLINE float to() const{
			switch (A)
			{
				case 'x': case 'r': return x;
				case 'y': case 'g': return y;
				case 'z': case 'b': return z;
				case 'w': case 'a': return w;

				case 1://Math::nx:
				    return -x;
				case 2://Math::ny:
				    return -y;
				case 3://Math::nz:
				    return -z;
				case 4://Math::nw:
				    return -w;
				default: return 0.0; break;
			}
		}
		template <uchar X,uchar Y>
		DFORCEINLINE Vec2 to() const{
			return Vec2(to<X>(),to<Y>());
		}
		template <uchar X,uchar Y,uchar Z>
		DFORCEINLINE Vec3 to() const{
			return Vec3(to<X>(),to<Y>(),to<Z>());
		}
		template <uchar X,uchar Y,uchar Z,uchar W>
		DFORCEINLINE Vec4 to() const{
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
		DFORCEINLINE float squaredLength(){ return x*x+y*y+z*z+w*w; };
		DFORCEINLINE void abs(){ x=fabs(x); y=fabs(y); z=fabs(z); w=fabs(w); }
		DFORCEINLINE Vector4D getAbs()const{ return Vector4D(fabs(x),fabs(y), fabs(z), fabs(w)); }
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE float& operator [] (unsigned int i) { return i==0 ? x: i==1 ? y : i==2 ? z : w; }
		DFORCEINLINE bool operator==(const Vector4D &v) const { return (x==v.x && y==v.y && z==v.z && w==v.w);	}
		DFORCEINLINE bool operator!=(const Vector4D &v) const { return (x!=v.x || y!=v.y || z!=v.z || w!=v.w);	}
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE Vector4D operator+(const Vector4D& v) const {return Vector4D(x+v.x,y+v.y,z+v.z,w+v.w);}
		DFORCEINLINE Vector4D operator+(float v) const {return Vector4D(x+v,y+v,z+v,w+v);}

		DFORCEINLINE Vector4D operator-(const Vector4D& v) const {return Vector4D(x-v.x,y-v.y,z-v.z,w-v.w);}
		DFORCEINLINE Vector4D operator-(float v) const {return Vector4D(x-v,y-v,z-v,w-v);}
		DFORCEINLINE Vector4D operator-(void) const {return Vector4D(-x,-y,-z,-w);}

		DFORCEINLINE Vector4D operator*(const Vector4D& v) const {return Vector4D(x*v.x,y*v.y,z*v.z,w*v.w);}
		DFORCEINLINE Vector4D operator*(float v) const {return Vector4D(x*v,y*v,z*v,w*v);}

		DFORCEINLINE Vector4D operator/(const Vector4D& v) const {return Vector4D(x/v.x,y/v.y,z/v.z,w/v.w);}
		DFORCEINLINE Vector4D operator/(float v) const {return Vector4D(x/v,y/v,z/v,w/v);}
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE const Vector4D &operator+=(const Vector4D &v){
			x+=v.x;
			y+=v.y;
			z+=v.z;
			w+=v.w;
			return *this;
		}
		DFORCEINLINE const Vector4D &operator+=(const float  &v){
			x+=v;
			y+=v;
			z+=v;
			w+=v;
			return *this;
		}
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE const Vector4D &operator-=(const Vector4D &v){
			x-=v.x;
			y-=v.y;
			z-=v.z;
			w-=v.w;
			return *this;
		}
		DFORCEINLINE const Vector4D &operator-=(const float  &v){
			x-=v;
			y-=v;
			z-=v;
			w-=v;
			return *this;
		}
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE const Vector4D &operator*=(const Vector4D &v){
			x*=v.x;
			y*=v.y;
			z*=v.z;
			w*=v.w;
			return *this;
		}
		DFORCEINLINE const Vector4D &operator*=(const float  &v){
			x*=v;
			y*=v;
			z*=v;
			w*=v;
			return *this;
		}
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE const Vector4D &operator/=(const Vector4D &v){
			x/=v.x;
			y/=v.y;
			z/=v.z;
			w/=v.w;
			return *this;
		}
		DFORCEINLINE const Vector4D &operator/=(const float  &v){
			x/=v;
			y/=v;
			z/=v;
			w/=v;
			return *this;
		}
		///////////////////////////////////////////////////////////////////////////
		operator float* ()  {return &this->x;}
		operator const float* () const {return &this->x;}
		///////////////////////////////////////////////////////////////////////////
		String toString(const String& start="(",const String& sep=" ",const String& end=")\n") const;

	};
    
    template <typename T>
    DFORCEINLINE Vector4D operator+(T v,const Vector4D& vt){
        return Vector4D(v+vt.x,v+vt.y,v+vt.z,v+vt.w);
    }
    template <typename T>
    DFORCEINLINE Vector4D operator-(T v,const Vector4D& vt){
        return Vector4D(v-vt.x,v-vt.y,v-vt.z,v-vt.w);
    }
    template <typename T>
    DFORCEINLINE Vector4D operator*(T v,const Vector4D& vt){
        return Vector4D(v*vt.x,v*vt.y,v*vt.z,v*vt.w);
    }
    template <typename T>
    DFORCEINLINE Vector4D operator/(T v,const Vector4D& vt){
        return Vector4D(v/vt.x,v/vt.y,v/vt.z,v/vt.w);
    }
	///////////////////////////////////////////////////////////////////////////
    class AABox2{
        
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
		void addPoint(const Vec2& point);
        //getter
        Vec2  getCenter() const{
            return (min+max)*0.5;
        }
        Vec2  getSize() const{
            return (max-min)*0.5;
        }
		const Vec2&  getMax() const{
			return max;
		}
		const Vec2&  getMin() const{
			return min;
		}
		Vec2&  getMax(){
			return max;
		}
		Vec2&  getMin(){
			return min;
		}
		bool  isIntersection(const Vec2& point);
		bool  isIntersection(const AABox2& aabb2);

    };
	///////////////////////////////////////////////////////////////////////////
	struct Transform2D{
		//
		Transform2D():alpha(0),scale(1.0f,1.0f){}
		//values
		Vector2D position;
		float alpha;
		Vector2D scale;
		//cast
		operator float*() { return &position.x; }
		operator const float*() const { return &position.x; }
	};
	///////////////////////////////////////////////////////////////////////////
	class Matrix4x4 {
	public:
	
#ifdef SIMD_SSE2
		VSALIGNED(16)
        union {
            
            struct {
                __m128 row0,row1,row2,row3;
            };
            
            float entries[16];
            
            struct {
                float m00, m01, m02, m03;
                float m10, m11, m12, m13;
                float m20, m21, m22, m23;
                float m30, m31, m32, m33;
            };
            
		}
        GCCALIGNED(16) ;
        DFORCEINLINE Matrix4x4& operator = (const Matrix4x4& m){
            row0=m.row0;
            row1=m.row1,
            row2=m.row2;
            row3=m.row3;
            return *this;
        }
#else
		union {
            
            float entries[16];
            struct {
                float m00, m01, m02, m03;
                float m10, m11, m12, m13;
                float m20, m21, m22, m23;
                float m30, m31, m32, m33;
            };
            
		};
#endif

		//constructors
		Matrix4x4();
		Matrix4x4(const Matrix4x4 &m4x4);
		Matrix4x4(float* m4x4);
		Matrix4x4(float e0,float e1,float e2,float e3,
				  float e4,float e5,float e6,float e7,
				  float e8,float e9,float e10,float e11,
				  float e12,float e13,float e14,float e15);
		//destructor
		virtual ~Matrix4x4(){};
		///identity
		void identity();
		///set all values to 0
		void zero();
		///matrix  inverse
		void inverse();
		///matrix inverse (only 2D transformation)
		void inverse2D();
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
		void addScale(const Vector3D &v3);
		void addScale(const Vector2D &v2);
		///set scale
		void setScale(const Vector2D &v2);
		///return scale
		Vector3D getScale3D() const;
		///return scale
		Vector2D getScale2D() const;
		///set translation
		void setTranslation(const Vector3D &v3);
		///set translation
		void setTranslation(const Vector2D &v2);
		///set concatenate trasformation:
		void addTranslationOnX( float distance );
		void addTranslationOnY( float distance );
		void addTranslationOnZ( float distance );
		///return translation
		Vector3D getTranslation3D() const;
		///return translation
		Vector2D getTranslation2D() const;
		///add a euler rotarion
		void addEulerRotation(const Vec3& euler);
		///set pitch
		void setRotX(float x);
		///set yaw
		void setRotY(float y);
		///set roll
		void setRotZ(float z);
		///return pitch
		float getRotX() const;
		///return yaw
		float getRotY() const;
		///return roll
		float getRotZ() const;
		///fast setting:  x,y | alpha | sx,sy
		void setTransform2D(const Transform2D& t2d);
		///set orthogonal transformation (projection matrix)
		void setOrtho(float left, float right, float bottom,float top, float n, float f);
		///set projection transformation (projection matrix)
		void setPerspective(float left, float right, float bottom,float top, float n, float f);
		void setPerspective(float fovy, float aspect, float n, float f);
		//operators:
		DFORCEINLINE float& operator[](int i) { return entries[i]; }
		DFORCEINLINE float& operator()(int x,int y) { return entries[x+(y*4)]; }
		DFORCEINLINE float operator[](int i) const{ return entries[i]; }
		DFORCEINLINE float operator()(int x,int y) const{ return entries[x+(y*4)]; }
		//
		operator float* ()  {return (float*)entries;}
		operator const float* () const {return (const float*)entries;}
		///////////////////////////////////////////////////////////////////////////
		String toString(const String& start="(",const String& sep=" ",const String& sepline=" ",const String& end=")\n") const;

    };
	///////////////////////////////////////////////////////////////////////////
	class Math{
	public:
		//enum attribute
		enum VecAttribute{
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
		static DFORCEINLINE float torad(float deg) { return deg*PIOVER180; }
		static DFORCEINLINE float todeg(float rad) { return rad*G180OVERPI; }
        static DFORCEINLINE float normaliseOrientation(float rot){
                rot=std::fmod(rot,(float)Math::PI2);
                return rot<0 ? rot+=Math::PI2 : rot;
        }
		//fast swap
		template<typename T>
		static DFORCEINLINE void swap(T& x,T& y){
			 register T temp = std::move(x);
			 x = std::move(y);
			 y = std::move(temp);
		}
		//infinite
        template<typename T>
        static DFORCEINLINE bool isinf(T x){
                #ifdef COMPILER_VISUAL_STUDIO
                        return _finite(x)==0;
                #else
                        return std::isinf(x);
                #endif
        }
        //nan
        template<typename T>
        static DFORCEINLINE bool isnan(T x){
                #ifdef COMPILER_VISUAL_STUDIO
                        return _isnan(x);
                #else
                        return std::isnan(x);
                #endif
        }
		//min
		template<typename T>
		static DFORCEINLINE T min(T x,T y){
			return x>y?y:x;
		}
		static DFORCEINLINE Vector2D min(Vector2D v1,Vector2D v2){
			return Vector2D(min(v1.x,v2.x),min(v1.y,v2.y));
		}
		static DFORCEINLINE Vector2D min(const Vector2D& v1,const Vector2D& v2){
			return Vector2D(min(v1.x,v2.x),min(v1.y,v2.y));
		}
		static DFORCEINLINE Vector3D min(Vector3D v1,Vector3D v2){
			return Vector3D(min(v1.x,v2.x),min(v1.y,v2.y),min(v1.z,v2.z));
		}
		static DFORCEINLINE Vector3D min(const Vector3D& v1,const Vector3D& v2){
			return Vector3D(min(v1.x,v2.x),min(v1.y,v2.y),min(v1.z,v2.z));
		}
		//max
		template<class T>
		static DFORCEINLINE T max(T x,T y){
			return x>y?x:y;
		}
		static DFORCEINLINE Vector2D max(Vector2D v1,Vector2D v2) {
			return Vector2D(max(v1.x,v2.x),max(v1.y,v2.y));
		}
		static DFORCEINLINE Vector2D max(const Vector2D& v1,const Vector2D& v2) {
			return Vector2D(max(v1.x,v2.x),max(v1.y,v2.y));
		}
		static DFORCEINLINE Vector3D max(Vector3D v1,Vector3D v2) {
			return Vector3D(max(v1.x,v2.x),max(v1.y,v2.y),max(v1.z,v2.z));
		}
		static DFORCEINLINE Vector3D max(const Vector3D& v1,const Vector3D& v2) {
			return Vector3D(max(v1.x,v2.x),max(v1.y,v2.y),max(v1.z,v2.z));
		}
		//min max list
		#ifdef COMPILER_VISUAL_STUDIO
			#include "MinMaxList.h"
		#else
			template<typename T,typename ...A>
			static DFORCEINLINE T min(T x,A... a){
				return min(x,min(a...));
			}
			template<typename T,typename ...A>
			static DFORCEINLINE T max(T x,A... a){
				return max(x,max(a...));
			}
		#endif
		//lerp==linear
		template <class T>
		static DFORCEINLINE T lerp( const T& left, const T& right, float t ){
			return (T)(left + ( right - left ) * t);
		}
		template <class T>
		static DFORCEINLINE T linear( const T& left, const T& right, float t ){
			return (T)(left + ( right - left ) * t);
		}
		template <class T>
		static DFORCEINLINE T quadratic( const T& left, const T& right, float t ){
			return (T)( linear( linear(left,right,t), linear(left,right,t), t) );
		}	
		template <class T>
		static DFORCEINLINE T cubic( const T& left, const T& right, float t ){
			return (T)( linear( quadratic(left,right,t), quadratic(left,right,t), t) );
		}
		//clamp
		template <class T>
		static DFORCEINLINE T clamp( const T& n, const T& max, const T& min ) {
		return n>max ? max : ( n<min ? min : n );
		}
		//saturate
		template <class T>
		static DFORCEINLINE T saturate( const T& value ){
			return clamp(value,1,0);
		}
		//power of 2 test
		template <class T>
		static DFORCEINLINE bool isPowerOfTwo(T x){
			return (x != 0) && ((x & (x - 1)) == 0);
		}
		static DFORCEINLINE uint nextPowerOfTwo(uint x){
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
		static DFORCEINLINE bool prime(unsigned int m){
		   unsigned int i,j;
		   if (m < 2) return false;
		   if (m == 2) return true;
		   if (!(m & 1)) return false;
		   if (m % 3 == 0) return (m == 3);
		   for (i=5; (j=i*i), j <= m && j > i; i += 6) {
		   if (m %   i   == 0) return false;
		   if (m % (i+2) == 0) return false;
		   }
		   return true;
		}
		//random values
		static void seedRandom(unsigned int seed=0) ;
		static float random();
		static float randomRange(float min,float max);
		///////////////////////////////////////////////////
		//fast factorial
		template <int n>
		struct factorial {
		  //static
		  enum { value = n * factorial<n - 1>::value };
		  //dynamic
		  int get(){ return fac(n); }

		private:
		  int fac(int x){ return x<1?1:x*fac(x-1); }

		};
		///////////////////////////////////////////////////
		//fast fibonacci
		template <int n>
		struct fibonacci {
		  //static
		  enum { value = fibonacci<n - 2>::value + fibonacci<n - 1>::value };
		  //dynamic
		  int get(){ return fib(n); }

		private:
		  int fib(int x){ return x<2?1:fib(x-2)+fib(x-1); }

		};
	};

    template <>
    struct Math::factorial<0>
    {
        enum { value = 1 };
    };
    template<>
    struct Math::fibonacci<0>{
        enum { value = 1 };
    };
    template<>
    struct Math::fibonacci<1>{
        enum { value = 1 };
    };
}

#endif
