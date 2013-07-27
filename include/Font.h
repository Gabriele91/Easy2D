#ifndef FONT_H
#define FONT_H

#include <Config.h>
#include <Utility.h>
#include <Resource.h>
#include <Texture.h>
#include <Color.h>

namespace Easy2D {
	
	class FreeTypeFontLoader;
	class BMFontLoader;

	class Font : public Resource<Font>{

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

	private:
		//map characters
		DUNORDERED_MAP< int , Character* > characters;
		//page list
		std::vector<Texture::ptr> pages;
		//font size
		bit16 fontSize;
		//font size
		String fontName;
		//load from fnt
		bool isBMFont;
		//private methods
		void setSize(bit16 size){
			fontSize=size;
		}
		void setName(const String& name){
			fontName=name;
		}
		void addPage(Texture::ptr texturePage){
			pages.push_back(texturePage);
		}
		void addCharacter(int charName,Character* character){
			characters[charName]=character;
		}
		Character* getCharacter(int c){
			auto it=characters.find(c);
			if(it!=characters.end()) 
				return it->second;
			return NULL;
		}
		void drawListCharArray(int page,float *xyUV0,float *xyUV2,int size){
				//bind texture
				pages[page]->bind();
				//set vertex
				glVertexPointer(  2, GL_FLOAT, sizeof(float)*4,  xyUV0);
				glTexCoordPointer(2, GL_FLOAT, sizeof(float)*4,  xyUV2);
				//draw array
				glDrawArrays(GL_TRIANGLES, 0, size);
		}
		//friend class		
		friend class FreeTypeFontLoader;
		friend class BMFontLoader;

	public:

		//costructor
		Font(ResourcesGroup *rsgr,const String& path);
		//load methods
		virtual bool load();
		virtual bool unload();
		//getters
		int size();
		const String& getFontName();
		//other methods
		void text(const Vec2& pos,
				  const String& textDraw,
			      const Color& color=Color(255,255,255,255));


	};

};

#endif