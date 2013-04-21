#ifndef WINDOWSSCREEN_H
#define WINDOWSSCREEN_H
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/extensions/xf86vmode.h>
#include <Config.h>
#include <LinuxApp.h>
#include <EString.h>
#include <Screen.h>

namespace Easy2D {

	class LinuxScreen : Screen{

	public:

		LinuxScreen();
		virtual ~LinuxScreen();

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
								  bool fullscreen);
		/**
		* close window
		*/
		virtual void closeWindow();

	private:
        ///////////////////////////////////
        //X11
        //our fullscreen instance
        Display               * display;
        int                     screen;
        //our window instance
        Window                  window;
        GLXContext              context;
        XSetWindowAttributes    winAttr;
        uchar           doubleBuffered;
        uint            bitesOpenGL;
        String          appname;
        //original desktopMode
        XF86VidModeModeInfo     desktopMode;
		///////////////////////////////////
		XVisualInfo *  visual;
		void  __createGLXContext(uint bites);
		void __deleteGLXContext();
		void __createFullScreenWindow();
		void __createWindow();
		void __deleteWindow();
        ///////////////////////////////////
		//datas:
		uint nativeWidth;
		uint nativeHeight;
		uint screenWidth;
		uint screenHeight;
		uint freamPerSecond;
		bool fullscreen;
		bool showmouse;
		//friends class
		friend class LinuxApp;
	};


};

#endif
