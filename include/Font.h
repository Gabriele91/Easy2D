#ifndef FONT_H
#define FONT_H

#include <Config.h>
#include <Utility.h>
#include <Resource.h>
#include <Texture.h>
#include <Color.h>

namespace Easy2D
{

class FreeTypeFontLoader;
class BMFontLoader;

class Font : public Resource<Font>
{

public:

    struct Character
    {
        Character() :
            srcX(0),
            srcY(0),
            srcW(0),
            srcH(0),
            xOff(0),
            yOff(0),
            xAdv(0),
            page(0) {}

        short srcX;
        short srcY;
        short srcW;
        short srcH;
        short xOff;
        short yOff;
        short xAdv;
        short page;
        unsigned int chnl;
        //std::vector<int> kerningPairs;
    };

    struct AssociativeChars
    {
        /* values declaretion */
        uint first,second;
        /* constructors operators */
        AssociativeChars():first(0),second(0) {}
        AssociativeChars(uint c1,uint c2):first(c1),second(c2) {}
        /* operators */
        uint operator[](uint i) const
        {
            return (i%2 ? second : first);
        }
        uint& operator[](uint i)
        {
            return (i%2 ? second : first);
        }
        /* hash map operators */
        bool operator==(const AssociativeChars& v) const
        {
            return first==v.first && second==v.second;
        }
        struct hash
        {
            std::size_t operator()(const AssociativeChars& k) const
            {
                return std::hash<int>()(k.first);
            }
        };
    };

private:
    //map characters
    typedef DUNORDERED_MAP< AssociativeChars, int, AssociativeChars::hash > MAPKerningPairs;
    typedef DUNORDERED_MAP< int , Character* > MAPCharacters;
    MAPCharacters characters;
    MAPKerningPairs kerningPairs;
    //page list
    std::vector<Texture::ptr> pages;
    //font size
    bit16 fontSize;
    //font size
    String fontName;
    //load from fnt
    bool isBMFont;
    //private methods
    void pushAKerningPairs(uint k1,uint k2,int amount)
    {
        kerningPairs[AssociativeChars(k1,k2)]=amount;
    }
    int getKerningPairs(uint k1,uint k2)
    {
        AssociativeChars pairs(k1,k2);
        MAPKerningPairs::const_iterator it=kerningPairs.find(pairs);
        if(it!=kerningPairs.cend())
            return it->second;
        return 0;
    }
    void setSize(bit16 size)
    {
        fontSize=size;
    }
    void setName(const String& name)
    {
        fontName=name;
    }
    void addPage(Texture::ptr texturePage)
    {
        pages.push_back(texturePage);
    }
    void addCharacter(int charName,Character* character)
    {
        characters[charName]=character;
    }
    Character* getCharacter(int c)
    {
        auto it=characters.find(c);
        if(it!=characters.end())
            return it->second;
        return NULL;
    }
    void drawListCharArray(int page,float *xyUV0,float *xyUV2,int size);
    //friend class
    friend class FreeTypeFontLoader;
    friend class BMFontLoader;

public:

    //costructor
    Font(ResourcesManager<Font> *rsmr,const String& path);
    //load methods
    virtual bool load();
    virtual bool unload();
    //getters
    int size();
    const String& getFontName();
    //other methods
    void text(const Vec2& pos,
              const String& textDraw,
              const Color& color=Color(255,255,255,255),
              bool kerning=true);
    //text size
    Vec2 textSize( const String& textDraw,bool kerning=true);

};

};

#endif