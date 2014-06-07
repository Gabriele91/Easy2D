#ifndef COLOR_H
#define COLOR_H

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
    unsigned char r,g,b,a;
    Color()
    {
        r=g=b=a=255;
    }
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a):r(r),g(g),b(b),a(a) {}
    void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        this->r=r;
        this->g=g;
        this->b=b;
        this->a=a;
    }

    Color operator + (const Color& v)
    {
        return (Color(r+v.r,g+v.g,b+v.b,a+v.a));
    }
    Color operator - (const Color& v)
    {
        return (Color(r-v.r,g-v.g,b-v.b,a-v.a));
    }
    bool operator == (const Color& v)
    {
        return r==v.r && g==v.g && b==v.b && a==v.a;
    }
    bool operator != (const Color& v)
    {
        return r!=v.r || g!=v.g || b!=v.b || a!=v.a;
    }

    operator const unsigned char* () const
    {
        return (const unsigned char*) this;
    }
    operator unsigned char* () const
    {
        return (unsigned char*) this;
    }

    float rNormalize()
    {
        return (float)r/255;
    }
    float gNormalize()
    {
        return (float)g/255;
    }
    float bNormalize()
    {
        return (float)b/255;
    };
    float aNormalize()
    {
        return (float)a/255;
    }
};
};

#endif