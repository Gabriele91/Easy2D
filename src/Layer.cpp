#include <stdafx.h>
#include <Layer.h>
#include <Renderable.h>
///////////////////////
using namespace Easy2D;
///////////////////////
//sort
bool LayerOrder::operator_lt(const Renderable* lrs,const Renderable* rrs){ 
	return (*lrs) < (*rrs);
}