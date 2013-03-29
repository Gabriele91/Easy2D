#ifndef WINDOWSAPP_H
#define WINDOWSAPP_H

#include <Config.h>
#include <Application.h>
#include <Game.h>

namespace Easy2D {
	
	/**
    * Abstract class, represent the application instance
	* @class Application <Application.h>
    */
	class WindowsApp : Application{
	public:
		/**
		* destroy an window application
		*/
		virtual ~WindowsApp();
		/**
		* where you can seve files data
		* @return path
		*/
		virtual String appDataDirectory();
		/**
		* application root (read only)
		* @return path
		*/
		virtual String appWorkingDirectory();
		/**
		* resources directory (read only)
		* @return path
		*/
		virtual String appResourcesDirectory();	
		/**
		* application loop
		*/
		virtual void loop();		
		/**
		* execute a game application
		*/
		virtual void exec(Game *game);
		/**
		* application update
		*/
		virtual void update(float dt);
		/**
		* return true if device supports only power of two texture
		*/
		virtual bool onlyPO2();

	protected:

		WindowsApp();
		friend class Application;

	};


};

#endif