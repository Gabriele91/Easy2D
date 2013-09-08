#ifndef TEXTURE_H
#define TEXTURE_H

#include <Config.h>
#include <Math3D.h>
#include <EString.h>
#include <Resource.h>
#include <Mesh.h>
#include <iostream>
#include <Types.h>

namespace Easy2D {

	class Texture : public Resource<Texture>{
		//
		bool bBilinear,chBlr,bMipmaps,chMps;
		uint width,height;
		uint realWidth,realHeight;
        uint gpuid;
		Vec2 offsetUV;
		//sprite pow of tow  
		Mesh::ptr po2Srpite;
		void __build();
		//
	public:
		//
		Texture(ResourcesGroup *rsmr=NULL,
				const String& pathfile="");	
		//destructor
		virtual ~Texture();
		//
		void bind(uint ntexture=0);
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
		DFORCEINLINE uint getHeight(){ return height; }    
		DFORCEINLINE uint getRealHeight(){ return realHeight; }
		//overload
		bool operator ==(const Texture&) const;
		bool operator !=(const Texture&) const;
	};

};

#endif