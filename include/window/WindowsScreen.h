#ifndef WINDOWSSCREEN_H
#define WINDOWSSCREEN_H

#include <Config.h>
#include <WindowsApp.h>
#include <String.h>
#include <Screen.h>

namespace Easy2D {

	class WindowsScreen : Screen{

	public:

		WindowsScreen();
		virtual ~WindowsScreen();

		/**
		* return screen width
		*/
		virtual unsigned int getWidth();
		/**
		* return screen height
		*/
		virtual unsigned int getHeight();		
		/**
		* return screen native width
		*/
		virtual unsigned int getNativeWidth();
		/**
		* return screen  native height
		*/
		virtual unsigned int getNativeHeight();
		/**
		* return screen orientation
		*/
		virtual SceenOrientation getOrientation();
		/**
		* show or hide mouse cursor
		* @param show, set true if you want show cursor otherwise false
		*/
		virtual void setCursor(bool show=true);
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
								  unsigned int width,
								  unsigned int height,
								  unsigned int bites,
								  bool fullscreen);
		/**
		* close window
		*/
		virtual void closeWindow();

	private:
		//window methods
		void __initWindow(const char* appname,unsigned int bites);
		void __destroyWindow();

		///window application instance
		HINSTANCE hInstance;
		/// handle window
		HWND hWind; 
		/// handle device context
		HDC hDevCxt;
		/// handle OpenGL rendering context
		HGLRC hGLCxt; 

		//datas:
		unsigned int nativeWidth;
		unsigned int nativeHeight;
		unsigned int screenWidth;
		unsigned int screenHeight;
		bool fullscreen;
		bool showmouse;
		//friends class
		friend class WindowsApp;
	};


};

#endif