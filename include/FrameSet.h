#ifndef FRAMESET_H
#define FRAMESET_H

#include <Config.h>
#include <Math2D.h>
#include <EString.h>
#include <Resource.h>
#include <Texture.h>
#include <Mesh.h>

namespace Easy2D {

	class FrameSet : public Resource<FrameSet>{

		Texture::ptr texture;
		std::vector<Mesh::ptr> frames;

	public:
		//create a frame set
		FrameSet(ResourcesGroup *rsgr,const String& path);
		//resource
		virtual bool load();
		virtual bool unload();
		//return numbers of frames 
		uint size();
		//return a frame
		Mesh::ptr getFrame(uint i);
		//return texture
		Texture::ptr getTexture();
		//

	};

};

#endif