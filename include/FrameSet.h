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
		void addFrame(const Vec4& frame);
		std::vector<Vec2> sizeFrames;

	public:
		//create a frame set
		FrameSet(ResourcesGroup *rsgr,const String& path);
		virtual ~FrameSet();
		//resource
		virtual bool load();
		virtual bool unload();
		//return numbers of frames 
		DFORCEINLINE uint size(){ return frames.size(); }
		DFORCEINLINE Vec2 sizeFrame(uint i){ return sizeFrames[i]; }
		//return a frame
		Mesh::ptr getFrame(uint i);
		//return texture
		DFORCEINLINE Texture::ptr getTexture(){
			return texture;
		}
		//

	};

};

#endif