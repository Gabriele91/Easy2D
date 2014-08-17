#ifndef COLOR_H
#define COLOR_H

#include <Config.h>
#include <Math3D.h>

namespace Easy2D
{
/* pixel structure */
struct Color
{
    /*
    black	Solid black. RGBA is (0, 0, 0, 1).
    blue	Solid blue. RGBA is (0, 0, 1, 1).
    clear	Completely transparent. RGBA is (0, 0, 0, 0).
    cyan	Cyan. RGBA is (0, 1, 1, 1).
    gray	Gray. RGBA is (0.5, 0.5, 0.5, 1).
    green	Solid green. RGBA is (0, 1, 0, 1).
    grey	English spelling for gray. RGBA is the same (0.5, 0.5, 0.5, 1).
    magenta	Magenta. RGBA is (1, 0, 1, 1).
    red	Solid red. RGBA is (1, 0, 0, 1).
    white	Solid white. RGBA is (1, 1, 1, 1).
    yellow  Yellow. RGBA is (1, 0.92, 0.016, 1), but the color is nice to look at!
    */
    //static colors
    static Color BLACK,
           BLUE,
           CLEAR,
           CYAN,
           GRAY,
           GREEN,
           GREY,
           MAGENTA,
           RED,
           WHITE,
           YELLOW;
    //
    uchar r,g,b,a;
    Color()
    {
        r=g=b=a=255;
    }
    Color(uchar r, uchar g, uchar b, uchar a):r(r),g(g),b(b),a(a) {}
    void setColor(uchar r, uchar g, uchar b, uchar a)
    {
        this->r=r;
        this->g=g;
        this->b=b;
        this->a=a;
    }

    Color operator + (const Color& v) const
    {
        Color out;
        out.r=(uchar)Math::clamp(r+v.r,255,0);
        out.g=(uchar)Math::clamp(g+v.g,255,0);
        out.b=(uchar)Math::clamp(b+v.b,255,0);
        out.a=(uchar)Math::clamp(a+v.a,255,0);
        return out;
    }
    Color operator - (const Color& v) const
    {
        Color out;
        out.r=(uchar)Math::clamp(r-v.r,255,0);
        out.g=(uchar)Math::clamp(g-v.g,255,0);
        out.b=(uchar)Math::clamp(b-v.b,255,0);
        out.a=(uchar)Math::clamp(a-v.a,255,0);
        return out;
    }
    Color operator * (const Color& v) const
    {
        Color out;
        out.r=(uchar)Math::clamp((r*v.r)/255,255,0);
        out.g=(uchar)Math::clamp((g*v.g)/255,255,0);
        out.b=(uchar)Math::clamp((b*v.b)/255,255,0);
        out.a=(uchar)Math::clamp((a*v.a)/255,255,0);
        return out;
    }
    bool operator == (const Color& v) const
    {
        return r==v.r && g==v.g && b==v.b && a==v.a;
    }
    bool operator != (const Color& v) const
    {
        return r!=v.r || g!=v.g || b!=v.b || a!=v.a;
    }
    Color operator * (const float factor) const
    {
        Color out;
        out.r=(uchar)Math::clamp(r*factor,255.0f,0.0f);
        out.g=(uchar)Math::clamp(g*factor,255.0f,0.0f);
        out.b=(uchar)Math::clamp(b*factor,255.0f,0.0f);
        out.a=(uchar)Math::clamp(a*factor,255.0f,0.0f);
        return out;
    }

    operator const uchar* () const
    {
        return (const uchar*) this;
    }
    operator uchar* () const
    {
        return (uchar*) this;
    }

    float rNormalize() const
    {
        return (float)r/255;
    }
    float gNormalize() const
    {
        return (float)g/255;
    }
    float bNormalize() const
    {
        return (float)b/255;
    };
    float aNormalize() const
    {
        return (float)a/255;
    }
    Vec4 toNormalize() const
    {
        return Vec4(r,g,b,a)/255.0;
    }
    const Color& fromNormalize(const Vec4& v)
    {
        r=uchar(v.r*255.0f);
        g=uchar(v.g*255.0f);
        b=uchar(v.b*255.0f);
        a=uchar(v.a*255.0f);
        return *this;
    }
    Vec4 toVec4() const
    {
        return Vec4(r,g,b,a);
    }
    const Color& fromVec4(const Vec4& v)
    {
        r=uchar(v.r);
        g=uchar(v.g);
        b=uchar(v.b);
        a=uchar(v.a);
        return *this;
    }
};
};

#endif