#ifndef APPLICATION_H
#define APPLICATION_H

#include <Config.h>
#include <Game.h>
#include <EString.h>
#include <Screen.h>
#include <Input.h>


namespace Easy2D {
	
	class Screen;
	class Game;
	class Audio;
	class Input;

	/**
    * Abstract class, represent the application instance
	* @class Application <Application.h>
    */
	class Application{

	public:
		/**
		* create singleton (if already created raises an exception)
		*/
		static Application *create();
		/**
		* return singleton istance
		*/
		static Application *instance();
		/**
		* destroy an application
		*/
		virtual ~Application();
		/**
		* load a binary file
		* @return succes
		*/
		virtual bool loadData(const String& path,void*& ptr,size_t &len)=0;
		/**
		* where you can save files data
		* @return path
		*/
		virtual String appDataDirectory()=0;
		/**
		* application root (read only)
		* @return path
		*/
		virtual String appWorkingDirectory()=0;
		/**
		* resources directory (read only)
		* @return path
		*/
		virtual String appResourcesDirectory()=0;			
		/**
		* application exit method
		*/
		virtual void exit()=0;
		/**
		* application loop
		*/
		virtual void loop()=0;		
		/**
		* execute a game application
		*/
		virtual void exec(Game *game)=0;
		/**
		* application update
		*/
		virtual void update(float dt)=0;
		/**
		* return true if device supports only power of two texture
		*/
		virtual bool onlyPO2()=0;		
		/**
		* return game istance
		*/
		Game* getGame(){
			return game;
		}
		/**
		* return screen device
		*/
		Screen* getScreen(){
			return screen;
		}
		/**
		* return audio device
		*/
		Audio* getAudio(){
			return audio;
		}
		/**
		* return input device
		*/
		Input* getInput(){
			return input;
		}

	protected:

		Game *game;
		Screen *screen;
		Input *input;
		Audio *audio;
		Application();
		static Application* appSingleton;

	};

};

#endif
