//
//  CocoaScreen.h
//  Easy2D
//
//  Created by Gabriele Di Bari on 14/06/13.
//
//

#ifndef COCOASCREEN_H
#define COCOASCREEN_H


#include <Config.h>
#include <CocoaApp.h>
#include <EString.h>
#include <Screen.h>

namespace Easy2D {
    
	class CocoaScreen : Screen{
        
	public:
        
		CocoaScreen();
		virtual ~CocoaScreen();
        
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
		virtual void setOrientation(Orientation orientation){
			//void
		}
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
        virtual void setPositionCursor(const Vec2& pos);
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
        //cocoa pointers
        void* cocoaWindow;
        void* cocoaGLContext;
        void* cocoaInfo;
        void (*onCocoaWindowCreated)(void*);
        void (*onCocoaWindowClose)(void*);
        void __openWindow(int w,int h,const char *title,bool fullscreen);
        void __closeWindow();
        void __createContext(int msaa);
        void __deleteContext();
        void __initOpenGL();
        ///////////////////////////////////
		//datas:
		uint nativeWidth;
		uint nativeHeight;
		uint screenWidth;
		uint screenHeight;
        uint wantWidth;
        uint wantHeight;
        uint wantPixel;
		uint freamPerSecond;
		bool fullscreen;
		bool showmouse;
		//friends class
		friend class CocoaApp;
	};
    
    
};

#endif
