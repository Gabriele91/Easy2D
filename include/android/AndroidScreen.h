#ifndef ANDROIDSCREEN_H
#define ANDROIDSCREEN_H
#include <Config.h>
#include <AndroidApp.h>
#include <EString.h>
#include <Screen.h>

namespace Easy2D {

	class AndroidScreen : Screen{

	public:

		AndroidScreen();
		virtual ~AndroidScreen();

		/**
		* return screen width
		*/
		virtual uint getWidth();
		/**
		* return screen height
		*/
		virtual uint getHeight();
		/**
		* return screen native width
		*/
		virtual uint getNativeWidth();
		/**
		* return screen  native height
		*/
		virtual uint getNativeHeight();
		/**
		* return screen orientation
		*/
		virtual Orientation getOrientation();		
		/**
		* set screen orientation
		*/
		virtual void setOrientation(Orientation orientation);
		/**
		* return frame rate
		*/
		virtual uint getFrameRate();
		/**
		* set the specified thread's current rendering context
		*/
		virtual void acquireContext();
		/**
		* swap the buffers
		*/
		virtual void swap();
		/**
		* show or hide mouse cursor
		* @param show, set true if you want show cursor otherwise false
		*/
		virtual void setCursor(bool show=true);
		/**
        * set position cursor
        */
        void setPositionCursor(const Vec2& pos);
		/**
		* return if cursor is shown or hidden
		*/
		virtual bool getCursor();
		/**
		* enable or disable full screen
		*/
		virtual void setFullscreen(bool fullscreen=true);
		/**
		* return if fullscreen is enable return true
		*/
		virtual bool isFullscreen();

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
								  AntiAliasing dfAA=NOAA);
        /*
		* close window
		*/
		virtual void closeWindow();

	private:
        ///////////////////////////////////
		//EGL
		EGLDisplay display;
		EGLSurface surface;
		EGLContext context;
		EGLint numConfigs;
		EGLConfig config;
        ///////////////////////////////////
		//function
		void __setupScreen();
		void __createSurface();
		void __createContext();
		void __isResized();

		void __disableContext();
		void __deleteContext();
		void __deleteSurface();

		bool __isAValidContext();
		void __initStateOpenGLES();
		//datas:
		uint nativeWidth;
		uint nativeHeight;
		uint screenWidth;
		uint screenHeight;
		uint freamPerSecond;
		bool fullscreen;
		bool showmouse;
		//friends class
		friend class AndroidApp;
	};


};

#endif
