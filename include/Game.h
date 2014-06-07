#ifndef GAME_H
#define GAME_H

#include <Config.h>
#include <Types.h>
#include <EString.h>
#include <Scene.h>
#include <ResourcesGroup.h>

namespace Easy2D
{


/**
* Abstract class, represent the game instance
* @class Game <Game.h>
*/
class Game : public ResourcesGroup, public Scene
{
public:

    Game(const String& name,
         uint width,
         uint height,
         uint bites=32,
         uint FreamPerSecond=60,
         bool fullscreen=false,
         int defaultNoAA=0);

    virtual ~Game() {};


private:

};

};

#endif