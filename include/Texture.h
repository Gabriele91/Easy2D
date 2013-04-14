#ifndef TEXTURE_H
#define TEXTURE_H

#include <Config.h>
#include <Math2D.h>
#include <EString.h>
#include <Resource.h>
#include <iostream>

namespace Easy2D {

	class Texture : public Resource<Texture>{

		bool bBilinear,chBlr,bMipmaps,chMps;
		uint width,height;
		uint realWidth,realHeight;
        uint gpuid;

	public:
		Texture(ResourcesGroup *rsmr=NULL,
				const String& pathfile="");	
		//
		void bind(uint ntexture=0);
		//load methods
		virtual bool load();
		virtual bool unload();
		//settings
		bool bilinear();
		bool bilinear(bool value);
		bool mipmaps();
		bool mipmaps(bool value);
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