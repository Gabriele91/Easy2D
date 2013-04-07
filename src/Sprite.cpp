#include <stdafx.h>
#include <Sprite.h>

///////////////////////
using namespace Easy2D;
///////////////////////

Sprite::Sprite(Texture::ptr texture,Layer *layer)
			  :Renderable(NULL,texture,layer,true)
{
}