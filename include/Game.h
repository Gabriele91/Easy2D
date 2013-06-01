#ifndef GAME_H
#define GAME_H

#include <Config.h>
#include <Types.h>
#include <Screen.h>
#include <Input.h>
#include <EString.h>

namespace Easy2D {
	
	class Screen;
	class Audio;
	class Input;

    /** 
    * Abstract class, represent the game instance
	* @class Game <Game.h>
    */
	class Game {
	public:

		Game(const String& name,
			 uint width, 
			 uint height, 
			 uint bites=32,
			 uint FreamPerSecond=60,
			 bool fullscreen=false,
			 int defaultNoAA=0);
		virtual ~Game(){};

		virtual void start()=0;
		virtual void run(float dt)=0;
		virtual void end()=0;
		
		Screen* getScreen();
		Audio* getAudio();
		Input* getInput();

	private:

	};

};

#endif