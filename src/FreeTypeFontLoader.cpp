#include <stdafx.h>
//include dependences
#include <ft2build.h>
#include FT_FREETYPE_H
#include <FreeTypeFontLoader.h>
#include <Debug.h>
#include <Application.h>
#include <ResourcesManager.h>
#include <ResourcesGroup.h>
///////////////////////
using namespace Easy2D;
///////////////////////
static inline bool notCollision(const Vec2& min1,
								const Vec2& max1,
								const Vec2& min2,
								const Vec2& max2){
	return 	( max1.x < min2.x || max1.y < min2.y ||
			  min1.x > max2.x || min1.y > max2.y );
}
/* single thread */
bool FreeTypeFontLoader::pageFindPos(
	std::vector<Font::Character *>& characters,
	const Vec2& sizeBitmap,
	const Vec2& sizeCharacter,
	Vec2& pos
	){
		if(characters.size()==0) return true;
	int yend=sizeBitmap.y-sizeCharacter.y;
	int xend=sizeBitmap.x-sizeCharacter.x;

	for(int y=0; y<yend;++y){
		for(int x=0; x<xend;++x){
			bool collision=true;
			pos.x=x;
			pos.y=y;
			for(auto& chr:characters)
			{
				collision=collision&&(notCollision(pos,
												   pos+sizeCharacter,
												   Vec2(chr->srcX,chr->srcY),
												   Vec2(chr->srcX+chr->srcW,chr->srcY+chr->srcH)));
				if(!collision){
					x=chr->srcX+chr->srcW;
					break;
				}
			}
			if(collision)
				return true;
		}
	}

	return false;
}

void FreeTypeFontLoader::addCharImageInBuffer(
			std::vector<uchar>& imageBytes,
			const int imgWidth,
			const Vec2& imgPos,
			const unsigned char *imgCharBuffer,
			const int charRow,
			const int charWidth,
			const int charPitch){
	for(int y=0;y<charRow;++y){
		for(int x=0;x<charWidth;++x){
			imageBytes[((int)((y+imgPos.y)*imgWidth+imgPos.x+x))*2]=255;
			imageBytes[((int)((y+imgPos.y)*imgWidth+imgPos.x+x))*2+1]=imgCharBuffer[x+y*charPitch];
		}
	}
}

bool FreeTypeFontLoader::loadPage(     Font& font,
								 const Vec2 pageSize,
									   int count,
								       uint& startChar,
								 const uint& endChar,
								       void* _face){
	FT_Face face=(FT_Face)_face;
	//image vector
	std::vector<byte> bytes(pageSize.x*pageSize.y*2);
	std::vector<Font::Character *> listChars;
	//cursor
	Vec2 cursor;
	//char
	int c=0;
	//load images
	for(c=startChar;c<endChar;++c){//utf8
		//load image
		if(FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;
		FT_Bitmap bitmap=face->glyph->bitmap;
		//gen image
		Vec2 characterSize(bitmap.width,bitmap.rows);
		//find a pos
		if(!pageFindPos(listChars,pageSize,characterSize,cursor))
			break;

		addCharImageInBuffer(bytes,
							 pageSize.x,
							 cursor,
							 bitmap.buffer,
							 bitmap.rows,
							 bitmap.width,
							 bitmap.pitch);
		//add char:
		Font::Character * character=new  Font::Character();
		listChars.push_back(character);
		font.addCharacter(c,character);
		character->page=count;
		character->xAdv=(face->glyph->advance.x >> 6);
		character->srcX=cursor.x;
		character->srcY=cursor.y;
		character->srcW=bitmap.width;
		character->srcH=bitmap.rows;
		character->xOff=face->glyph->bitmap_left;
		character->yOff=face->glyph->bitmap_top;
	}
	//set last char
	startChar=c;
	//create texture
	Texture *texture=new Texture(&font.getResourcesManager()
								  ->getResourcesGroup()
								  ->getManager<Texture>());
	texture->bilinear(false);
	texture->mipmaps(false);
	texture->loadFromBinaryData(bytes,
								pageSize.x,
								pageSize.y,
								//deprecate
								//openGL 3, GL_RED + swizzle mask {GL_ZERO, GL_ZERO, GL_ZERO, GL_RED}
								GL_LUMINANCE_ALPHA ,
								GL_LUMINANCE_ALPHA);
	font.addPage(Texture::ptr(texture));

	return true;
}

bool FreeTypeFontLoader::fastLoadPage(     Font& font,
									 const Vec2 pageSize,
										   int count,
										   uint& startChar,
									 const uint& endChar,
								           void* _face){
	FT_Face face=(FT_Face)_face;
	//image vector
	std::vector<byte> bytes(pageSize.x*pageSize.y*2);
	std::vector<Font::Character *> listChars;
	//cursor
	Vec2 cursor;
	//last size
	int lineMaxY=0;
	Vec2 lastCursor;
	//char
	int c=0;
	//load images
	for(c=startChar;c<endChar;++c){//utf8
		//load image
		if(FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;
		FT_Bitmap bitmap=face->glyph->bitmap;
		//save max pos
		lineMaxY=Math::max(lineMaxY,(int)(cursor.y+bitmap.rows));
		//next char pos
		if(cursor.x+bitmap.width>pageSize.x){
			cursor.x=0;
			cursor.y=lineMaxY;
		}
		//do char go out by page the page?
		if(cursor.y+bitmap.rows>pageSize.y) break;
		//add image
		addCharImageInBuffer(bytes,
							 pageSize.x,
							 cursor,
							 bitmap.buffer,
							 bitmap.rows,
							 bitmap.width,
							 bitmap.pitch);
		//add char:
		Font::Character * character=new  Font::Character();
		listChars.push_back(character);
		font.addCharacter(c,character);
		character->page=count;
		character->xAdv=(face->glyph->advance.x >> 6);
		character->srcX=cursor.x;
		character->srcY=cursor.y;
		character->srcW=bitmap.width;
		character->srcH=bitmap.rows;
		character->xOff=face->glyph->bitmap_left;
		character->yOff=face->glyph->bitmap_top;

		//next line
		cursor.x+=bitmap.width;

	}
	//set last char
	startChar=c;
	//create texture
	Texture *texture=new Texture(&font.getResourcesManager()
								  ->getResourcesGroup()
								  ->getManager<Texture>());
	texture->bilinear(false);
	texture->mipmaps(false);
	texture->loadFromBinaryData(bytes,
								pageSize.x,
								pageSize.y,
								//deprecate
								//openGL 3, GL_RED + swizzle mask {GL_ZERO, GL_ZERO, GL_ZERO, GL_RED}
								GL_LUMINANCE_ALPHA ,
								GL_LUMINANCE_ALPHA);
	font.addPage(Texture::ptr(texture));

	return true;
}

bool FreeTypeFontLoader::load(Font& font,Table& fontInfo,const Utility::Path& fontPath){
	//init lib
    FT_Library freeType;
	DEBUG_ASSERT_MGS_REPLACE(FT_Init_FreeType(&freeType)==0, "Could not init freetype library");
    //load font
    FT_Face face;
	void *facebuffer=NULL; size_t facelen=0;
	Application::instance()->loadData(fontPath,facebuffer,facelen);
	DEBUG_ASSERT_MGS_REPLACE( FT_New_Memory_Face( freeType,
												   (FT_Byte*)facebuffer,
												   facelen,
												   0,
												   &face )==0,"Could not open font");
	//set font size
	uint fontSize=fontInfo.getFloat("fontSize",12);
	FT_Set_Pixel_Sizes(face, fontSize, 0);
	font.setSize(fontSize);
	//set name
	font.setName(face->family_name);

	//get page image size
	Vec2 pageSize=fontInfo.getVector2D("pageSize",Vec2(256,256));
	int page=0;
	uint charStartPage=33;
	const uint charEndPage=255;
	if(fontInfo.getString("loadFast","true"))
		while(charStartPage<charEndPage){
			//add a page
			fastLoadPage(font,
						 pageSize,
						 page,
						 charStartPage,
						 charEndPage,
						 face);
			++page;
		}
	else
		while(charStartPage<charEndPage){
			//add a page
			loadPage(font,
					 pageSize,
					 page,
					 charStartPage,
					 charEndPage,
					 face);
			++page;
		}

	//dealloc
	FT_Done_Face( face );
	free(facebuffer);
	FT_Done_FreeType( freeType );
	return true;
}
