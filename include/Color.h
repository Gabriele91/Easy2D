#ifndef COLOR_H
#define COLOR_H

namespace Easy2D {
	/* pixel structure */
	struct Color{

		unsigned char r,g,b,a;
		Color(){r=g=b=a=255;}
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a):r(r),g(g),b(b),a(a){}
		void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a){
			this->r=r; this->g=g; this->b=b; this->a=a;
		}
		Color operator + (const Color& v){ return (Color(r+v.r,g+v.g,b+v.b,a+v.a)); }
		Color operator - (const Color& v){ return (Color(r-v.r,g-v.g,b-v.b,a-v.a)); }
		operator const unsigned char* () const {return (const unsigned char*) this;}
		operator unsigned char* () const {return (unsigned char*) this;}
		float rNormalize(){ return (float)r/255; }
		float gNormalize(){ return (float)g/255; }
		float bNormalize(){ return (float)b/255; };
		float aNormalize(){ return (float)a/255; }
	};
};

#endif