#ifndef FREETYPEFONTLOADER_H
#define FREETYPEFONTLOADER_H

#include <Config.h>
#include <Table.h>
#include <Font.h>

namespace Easy2D
{

class FreeTypeFontLoader
{

private:

    static bool pageFindPos(
        std::vector<Font::Character *>& characters,
        const Vec2& sizeBitmap,
        const Vec2& sizeCharacter,
        Vec2& pos
    );

    static void addCharImageInBuffer(
        std::vector<uchar>& imagebytes,
        const int imgWidth,
        const Vec2& imgPos,
        const unsigned char *imgCharBuffer,
        const int charRow,
        const int charWidth,
        const int charPitch);

    static bool loadPage(Font& font,
                         const Vec2 pageSize,
                         int count,
                         uint& startChar,
                         const uint& endChar,
                         void* face);


    static bool fastLoadPage(Font& font,
                             const Vec2 pageSize,
                             int count,
                             uint& startChar,
                             const uint& endChar,
                             void* face);

public:

    static bool load(Font& font,Table& fontInfo,const Utility::Path& file);

};

};

#endif