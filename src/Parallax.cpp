#include <stdafx.h>
#include <Parallax.h>

///////////////////////
using namespace Easy2D;
///////////////////////

void Parallax::onFixedRun(float dt)
{
    auto scene=getObject()->getScene();
    if(scene)
    {
        auto camera=scene->getCamera();
        if(camera)
        {
            //pos
            Vec2 pos=getObject()->getPosition()-moved;
            //pos cam
            Vec2 posCam=camera->getPosition();
            //move
            moved=posCam*parallax-posCam;
            getObject()->setPosition(pos+moved);
        }
    }
}


void Parallax::serialize(Table& table)
{
    table.set("scale",parallax);
}
void Parallax::deserialize(const Table& table)
{
    parallax=table.getVector2D("scale",Vec2::ONE);
}