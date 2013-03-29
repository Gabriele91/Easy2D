#ifndef GAME_H
#define GAME_H

#include <Config.h>
#include <Screen.h>
#include <EString.h>

namespace Easy2D {

    /** 
    * Abstract class, represent the game instance
	* @class Game <Game.h>
    */
	class Game {
	public:

		Game(const String& name,unsigned int width, unsigned int height, unsigned int bites,bool fullscreen);
		virtual ~Game(){};
		virtual void start()=0;
		virtual void run(float dt)=0;
		virtual void end()=0;

	private:

	};

};

#endif