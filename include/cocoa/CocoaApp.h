//
//  CocoaApp.h
//  Easy2D
//
//  Created by Gabriele Di Bari on 14/06/13.
//
//

#ifndef COCOAAPP_H
#define COCOAAPP_H


#include <Config.h>
#include <Application.h>
#include <Game.h>

namespace Easy2D
{

/**
 * Abstract class, represent the application instance
 * @class Application <Application.h>
 */
class CocoaApp : Application
{
public:
    /**
     * create a window application
     */
    CocoaApp(const String& name);
    /**
     * destroy a window application
     */
    virtual ~CocoaApp();
    /**
     * load a binary file
     * @return succes
     */
    virtual bool loadData(const String& path,void*& ptr,size_t &len);
    /**
     * where you can save files data
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
     * application exit method
     */
    virtual void exit();
    /**
     * application loop
     */
    virtual void loop();
    /**
     * execute a instance application
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
    //exit loop?
    bool doexit;
    //cocoa app pointer
    void *cocoaApp;
    //path data files
    String dataPath;
    //
    CocoaApp();
    friend class Application;

};


};

#endif

