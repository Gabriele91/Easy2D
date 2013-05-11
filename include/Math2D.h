#ifndef MATH2D_H
#define MATH2D_H

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

		float x,y;
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
		DFORCEINLINE Vector2D getAbs(){ return Vector2D(fabs(x),fabs(y)); }
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
		String toString(const String& start="(",const String& sep=" ",const String& end=")\n") const;

	};
	Vector2D operator+(float v,const Vector2D& vt);
	Vector2D operator-(float v,const Vector2D& vt);
	Vector2D operator*(float v,const Vector2D& vt);
	Vector2D operator/(float v,const Vector2D& vt);
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
		float x,y,z;
		///////////////////////////////////////////////////////////////////////////
		Vector3D():x(0),y(0),z(0){};
		Vector3D(Vector2D v,float z):x(v.x),y(v.y),z(z){};
		Vector3D(float x,float y,float z):x(x),y(y),z(z){};
		~Vector3D(){};
		///////////////////////////////////////////////////////////////////////////
		DFORCEINLINE Vec2 xy() const{
			return Vec2(x,y);
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
		DFORCEINLINE Vector3D getAbs(){ return Vector3D(fabs(x),fabs(y), fabs(z)); }
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
	Vector3D operator+(float v,const Vector3D& vt);
	Vector3D operator-(float v,const Vector3D& vt);
	Vector3D operator*(float v,const Vector3D& vt);
	Vector3D operator/(float v,const Vector3D& vt);
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
		float x,y,z,w;
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
		DFORCEINLINE Vec3 xyz() const{
			return Vec3(x,y,z);
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
		DFORCEINLINE Vector4D getAbs(){ return Vector4D(fabs(x),fabs(y), fabs(z), fabs(w)); }
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
	Vector4D operator+(float v,const Vector4D& vt);
	Vector4D operator-(float v,const Vector4D& vt);
	Vector4D operator*(float v,const Vector4D& vt);
	Vector4D operator/(float v,const Vector4D& vt);
	///////////////////////////////////////////////////////////////////////////
	//plane  ORIGIN + NORMAL(direction)
	class Plane{
	public:

		//Ax + By + Cz + D
		Vector3D normal; //a b c
		float d;  //d

		DFORCEINLINE float& a(){ return normal.x; }
		DFORCEINLINE float& b(){ return normal.y; }
		DFORCEINLINE float& c(){ return normal.z; }
		DFORCEINLINE float a() const { return normal.x; }
		DFORCEINLINE float b() const { return normal.y; }
		DFORCEINLINE float c() const { return normal.z; }

		Plane();
		Plane(const Vector3D& normal,const Vector3D& origin);
		Plane(const Vector3D& v1,const Vector3D& v2,const Vector3D& v3);
		Plane(float a, float b, float c, float d);
		//calc from point
		void set3Points(const Vector3D& v1,const Vector3D& v2,const Vector3D& v3);
		//Linear rapresetation
		void setCoefficients(float a, float b, float c, float d);
		//Parametric rapresetation
		void setNormalAndOrigin(const Vector3D& normal,const Vector3D& origin);
		//distance from point
		float distance(const Vector3D& point);
		//normalize
		void normalize();
		//
		String	toString(const String& start="(",const String& sep=" ",const String& end=")") const;
	};
	///////////////////////////////////////////////////////////////////////////
	class Quaternion{
	public:
		float w,x,y,z;

		Quaternion();
		Quaternion(float w,float x,float y,float z);

		///identity
		void identity();
		///compute W coordinate
		void computeW();
		///normalise
		void safe_normalise();
		void normalise();
		///inverse
		Quaternion getInverse() const;
		///Quaternion multiplication
		Quaternion mul(const Quaternion &qt) const;
		///Quaternion*vector
		Quaternion mulVec(const Vector3D &v) const;
		///set pitch, yaw and roll
		void setFromEulero(float pitch, float yaw, float roll);
		void setFromEulero(const Vec3& pyr){
			setFromEulero(pyr.x,pyr.y,pyr.z);
		}
		void setLookRotation(const Vector3D &lookAt,Vector3D up);
		///return pitch, yaw and roll
		void getEulero(float &pitch, float &yaw, float &roll) const;
		void getEulero(Vec3& pyr) const{
			getEulero(pyr.x,pyr.y,pyr.z);
		}
		///set quaternion from axis angle
		void setFromAxisAngle(Vector3D &vt,float angle);
		///return axis angle from quaternion
		void getAxisAngle(Vector3D &vt,float &angle) const;
		///return rotate point
		Vector3D getRotatePoint(Vector3D & v) const;
		///linear quaternion interpolation
		Quaternion lerp(const Quaternion &q, float t) {
			return ((*this)*(1.0f-t) + q*t).getNormalize();
		}
		Quaternion slerp(const Quaternion &q, float t){
			Quaternion q3;
			float dot = this->dot(q);

			/*	dot = cos(theta)
				if (dot < 0), q1 and q2 are more than 90 degrees apart,
				so we can invert one to reduce spinning	*/
			if (dot < 0){
				dot = -dot;
				q3 = -q;
			}
			else
				q3 = q;
			if (dot < 0.95f){
				float angle = acosf(dot);
				return ((*this)*sinf(angle*(1-t)) + q3*sinf(angle*t))/sinf(angle);
			} else // if the angle is small, use linear interpolation
				return this->lerp(q3,t);
		}
		//standard op
		float length() const;
		float dot(const Quaternion& vec) const;
		Quaternion getNormalize() const;
		///return matrix from quaternion
		Matrix4x4 getMatrix() const;
		//overload op
		DFORCEINLINE const Quaternion operator *(float f) const{
			return Quaternion(x*f, y*f, z*f,w*f);
		}
		DFORCEINLINE const Quaternion operator /(float f) const{
			return Quaternion(x/f, y/f, z/f,w/f);
		}
		DFORCEINLINE const Quaternion operator +(const Quaternion &q) const{
			return Quaternion(x+q.x, y+q.y, z+q.z,w+q.w);
		}
		DFORCEINLINE const Quaternion operator -(const Quaternion &q) const{
			return Quaternion(x-q.x, y-q.y, z-q.z,w-q.w);
		}
		DFORCEINLINE const Quaternion operator -(void) const{
			return Quaternion(-x, -y, -z,-w);
		}
		///////////////////////////////////////////////////////////////////////////
		operator float* ()  {return &this->x;}
		operator const float* () const {return &this->x;}
		///////////////////////////////////////////////////////////////////////////
		String toString(const String& start="(",const String& sep=" ",const String& end=")\n") const;

	};
	class Matrix4x4{
	public:

		float entries[16];

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
		void setScale(const Vector3D *v3);
		void addScale(const Vector3D *v3);
		void addScale(const Vector2D *v2);
		///set scale
		void setScale(const Vector2D &v2);
		///return scale
		Vector3D getScale3D() const;
		///return scale
		Vector2D getScale2D() const;
		///set translation
		void setTranslation(const Vector3D &v3);
		///set translation
		void setTranslation(const Vector3D *v3);
		///set translation
		void setTranslation(const Vector2D &v2);
		///set concatenate trasformation:
		void concatenateXTranslation( float distance ){
			entries[0] = entries[0] + distance * entries[3];
			entries[4] = entries[4] + distance * entries[7];
			entries[8] = entries[8] + distance * entries[11];
			entries[12] = entries[12] + distance * entries[15];
		}
		void concatenateYTranslation( float distance ){
			entries[1] = entries[1] + distance * entries[3];
			entries[5] = entries[5] + distance * entries[7];
			entries[9] = entries[9] + distance * entries[11];
			entries[13] = entries[13] + distance * entries[15];
		}
		void concatenateZTranslation( float distance ){
			entries[2] = entries[2] + distance * entries[3];
			entries[6] = entries[6] + distance * entries[7];
			entries[10] = entries[10] + distance * entries[11];
			entries[14] = entries[14] + distance * entries[15];
		}
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
		void setFastTransform2DTRS(float* list);
		///fast setting:  x,y | alpha
		void setFastTransform2DTR(float* list);
		///fast setting:  sx,sy
		void setFastTransform2DS(float* list);
		///set quaternion transformation
		void setQuaternion(Quaternion &qt);
		///get quaternion transformation
		Quaternion getQuaternion();
		Quaternion getQuaternionFast();
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
	class Math{
	public:
		//const values
		static const float PI;
		static const float PI2;
		static const float PIOVER180;
		static const float G180OVERPI;
		//radians and degrees
		static DFORCEINLINE float torad(float deg) { return deg*PIOVER180; }
		static DFORCEINLINE float todeg(float rad) { return rad*G180OVERPI; }
		//fast swap
		template<typename T>
		static DFORCEINLINE void swap(T& x,T& y){
			 register T temp = std::move(x);
			 x = std::move(y);
			 y = std::move(temp);
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
		//lerp
		template <class T>
		static DFORCEINLINE T lerp( const T& left, const T& right, float t ){
			return (T)(left * (1-t) + right * t);
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
