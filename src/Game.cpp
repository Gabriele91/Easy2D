#include <stdafx.h>
#include <Screen.h>
#include <Game.h>
#include <Application.h>
///////////////////////
using namespace Easy2D;

Game::Game(const String& name,
           uint width,
           uint height,
           uint freamPerSecond,
           bool fullscreen,
           int  typeBuffers,
           int  typeAA)
:Scene( name,
        width,
        height,
        freamPerSecond,
        fullscreen,
        typeBuffers,
        typeAA)
{
}

