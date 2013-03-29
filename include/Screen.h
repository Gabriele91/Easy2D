#ifndef SCREEN_H
#define SCREEN_H

#include <Config.h>
#include <Game.h>
#include <Application.h>

namespace Easy2D {

	class Screen{
	
	public:		
		/**
		* Screen oretation enum
		* @enum SceenOrientation
		*/
		enum SceenOrientation{
			///@code
			/// like this:
			///    _____
			///   |     |
			///   |     |
			///   |     |
			///   |__o__|
			///
			///@endcode
			PORTRAIT,
			///@code
			/// like this:
			///    _____
			///   |  o  |
			///   |     |
			///   |     |
			///   |_____|
			///
			///@endcode
			PORTRAIT_REVERSE,
			///@code
			/// like this:
			/// ___________
			///|           |
			///|           |
			///|_____o_____|
			///
			///@endcode
			LANDSCAPE_LEFT,
			///@code
			/// like this:
			/// ___________
			///|     o     |
			///|           |
			///|___________|
			///
			///@endcode
			LANDSCAPE_RIGHT,
		};
		/**
		* return screen width
		*/
		virtual unsigned int getWidth()=0;
		/**
		* return screen height
		*/
		virtual unsigned int getHeight()=0;		
		/**
		* return screen native width
		*/
		virtual unsigned int getNativeWidth()=0;
		/**
		* return screen  native height
		*/
		virtual unsigned int getNativeHeight()=0;
		/**
		* return screen orientation
		*/
		virtual SceenOrientation getOrientation()=0;
		/**
		* show or hide mouse cursor
		* @param show, set true if you want show cursor otherwise false
		*/
		virtual void setCursor(bool show=true)=0;
		/**
		* return if cursor is shown or hidden
		*/
		virtual bool getCursor()=0;		
		/**
		* enable or disable full screen
		*/
		virtual void setFullscreen(bool fullscreen=true)=0;
		/**
		* return if fullscreen is enable return true
		*/
		virtual bool isFullscreen()=0;
		/**
		* virtual destructor
		*/
		virtual ~Screen(){};

	protected:
		/**
		* create window
		*/
		virtual void createWindow(const char* appname,
								  unsigned int width,
								  unsigned int height,
								  unsigned int bites,
								  bool fullscreen)=0;
		/**
		* close window
		*/
		virtual void closeWindow()=0;

		/**
		* Application is a friend class 
		*/
		friend class Application;
		friend class Game;
  
	};

};

#endif