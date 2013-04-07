#ifndef SPRITE_H
#define SPRITE_H

#include <Config.h>
#include <Renderable.h>


namespace Easy2D {

	class Sprite : public Renderable {

	protected:
		//protected set mesh
		void setMesh(Mesh::ptr);
		//
	public:

		Sprite(Texture::ptr image,Layer *layer);

	};

};

#endif