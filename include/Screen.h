#ifndef SCREEN_H
#define SCREEN_H

#include <Config.h>
#include <Application.h>

namespace Easy2D {
	////////////////////
	class Game;
	////////////////////
	class Screen{
	
	public:				
		/**
		* Screen Anti Aliasing enum
		* @enum AntiAliasing
		*/
		enum AntiAliasing{
			NOAA=0,
			MSAAx2=2,
			MSAAx4=4,
			MSAAx8=8,
			MSAAx16=16,
			MSAAx32=32,
			MSAAx64=64,
			CSAA=128,
			CSAAQ=256,
			BESTAA=512
		};
		/**
		* Screen orientation enum
		* @enum SceenOrientation
		*/
		enum Orientation{
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
		virtual Orientation getOrientation()=0;
		/**
		* return frame rate
		*/
		virtual uint getFrameRate()=0;
		/**
		* set the specified thread's current rendering context
		*/
		virtual void acquireContext()=0;
		/**
		* swap the buffers
		*/
		virtual void swap()=0;
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
								  uint width,
								  uint height,
								  uint bites,
								  uint freamPerSecond,
								  bool fullscreen,
								  Screen::AntiAliasing dfAA)=0;
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