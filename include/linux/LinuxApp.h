#ifndef LINUXAPP_H
#define LINUXAPP_H

#include <Config.h>
#include <Application.h>
#include <Game.h>

namespace Easy2D
{

/**
* Abstract class, represent the application instance
* @class Application <Application.h>
*/
class LinuxApp : Application
{
public:
    /**
    * destroy an window application
    */
    virtual ~LinuxApp();
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
    //
    LinuxApp();
    friend class Application;

};


};

#endif
