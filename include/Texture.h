#ifndef TEXTURE_H
#define TEXTURE_H

#include <Config.h>
#include <Math3D.h>
#include <EString.h>
#include <Mesh.h>
#include <iostream>
#include <Types.h>
#include <Resource.h>

namespace Easy2D {

	class Texture : public Resource<Texture>{
		//
		bool bBilinear,chBlr,bMipmaps,chMps,bFlipVertical;
		uint width,height;
		uint realWidth,realHeight;
		uint spriteWidth,spriteHeight;
        uint gpuid;
		Vec2 offsetUV;
		//sprite pow of tow  
		Mesh::ptr po2Srpite;
		void __build();
		//
	public:
		//
		Texture(ResourcesManager<Texture> *rsmr=NULL,
				const String& pathfile="");	
		//destructor
		virtual ~Texture();
		//
		void bind(uint ntexture=0);
		//flip vertical
		bool flipVertical();
		bool flipVertical(bool value);
		//load methods
		virtual bool load();
		virtual bool unload();
		bool loadFromBinaryData(std::vector<uchar>& bytes,
								uint width,
								uint height,
								uint format,
								uint type);
		//settings
		bool bilinear();
		bool bilinear(bool value);
		bool mipmaps();
		bool mipmaps(bool value);
		//offset UV (npow)
		DFORCEINLINE Vec2& getOffestUV(){ return offsetUV; }
		//mesh size this..
		Mesh::ptr getPO2Sprite();
		//query
		DFORCEINLINE uint getWidth(){ return width; }    
		DFORCEINLINE uint getRealWidth(){ return realWidth; }
		DFORCEINLINE uint getSpriteWidth(){ return spriteWidth; }
		DFORCEINLINE uint getHeight(){ return height; }    
		DFORCEINLINE uint getRealHeight(){ return realHeight; }
		DFORCEINLINE uint getSpriteHeight(){ return spriteHeight; }
		
		DFORCEINLINE Vec2 getSize(){ 
			return Vec2((float)width,
						(float)height); 
		}
		DFORCEINLINE Vec2 getRealSize(){ 
			return Vec2((float)realWidth,
						(float)realHeight); 
		}
		DFORCEINLINE Vec2 getSpriteSize(){ 
			return Vec2((float)spriteWidth,
						(float)spriteHeight); 
		}
		//overload
		bool operator ==(const Texture&) const;
		bool operator !=(const Texture&) const;
	};

};

#endif