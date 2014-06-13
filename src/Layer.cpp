#include <stdafx.h>
#include <Layer.h>
#include <Renderable.h>
///////////////////////
using namespace Easy2D;
///////////////////////
//sort
bool LayerOrder::operator_lt(const Object* lrs,const Object* rrs)
{
    return lrs->getComponent<Renderable>()->getZ() < rrs->getComponent<Renderable>()->getZ();

}
void LayerOrder::dosort()
{
    if(reorder)
    {
        std::sort (renderables.begin(),
                   renderables.end(),
                   operator_lt);

        reorder=false;
    }
}