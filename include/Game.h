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
         uint freamPerSecond= 60,
         bool fullscreen    = false,
         int  typeBuffers   = 8 /* COLOR32DEPTH24STENCIL8 */,
         int  defaultNoAA   = 0 /* NOAA */);

    virtual ~Game() {};


private:

};

};

#endif