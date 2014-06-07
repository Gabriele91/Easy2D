#include <stdafx.h>
#include <Layer.h>
#include <Renderable.h>
///////////////////////
using namespace Easy2D;
///////////////////////
//sort
bool LayerOrder::operator_lt(const Renderable* lrs,const Renderable* rrs)
{
    return lrs->getZ() < rrs->getZ();

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