#ifndef SPRITE_H
#define SPRITE_H

#include <Config.h>
#include <Renderable.h>


namespace Easy2D {

	class Sprite : public Renderable {

	protected:
		//protected set mesh
		DFORCEINLINE void setMesh(Mesh::ptr mesh){
			Renderable::setMesh(mesh);
		}
		//build sprite mesh
		void _buildMesh();

	public:

		//costructor
		Sprite(Texture::ptr image=NULL,Layer *layer=NULL);
		//overload setTexture 
		void setTexture(Texture::ptr texture);

	};

};

#endif