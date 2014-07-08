#ifndef APPLICATION_H
#define APPLICATION_H

#include <Config.h>
#include <EString.h>


namespace Easy2D
{

class Game;
class Screen;
class Audio;
class Input;
class ResourcesGroup;

/**
* Abstract class, represent the application instance
* @class Application <Application.h>
*/
class Application
{

public:

    enum Seek
    {
        SET=0,
        CUR=1,
        END=2
    };

    class ResouceStream
    {

    public:

        virtual ~ResouceStream() {}
        ///close file
        virtual void close()=0;
        ///read from file
        virtual size_t read(void * ptr, size_t size, size_t count)=0;
        ///seek from file
        virtual size_t seek (size_t offset, Seek origin )=0;
        ///returns the current value of the position indicator of the stream
        virtual size_t tell()=0;
        ///get file size
        virtual size_t size()=0;
        ///return a uchar cast in int
        virtual int getc()=0;
        ///rewind from file
        virtual void rewind ()=0;

    };
    /**
    * create singleton (if already created raises an exception)
    */
    static Application *create(const String& name,Application* implementation=nullptr);
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
     * stream resource
     * @return ResouceStream object
     */
    virtual ResouceStream* getResouceStream(const String& path);
    /**
    * where you can seve files data
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
    * execute a instance application
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
    * return last delta time
    */
    virtual float getLastDeltaTime()
    {
        return lastDeltaTime;
    }
    /**
    * return main instance
    */
    Game* getGame()
    {
        return mainInstance;
    }
    /**
    * return screen device
    */
    Screen* getScreen()
    {
        return screen;
    }
    /**
    * return audio device
    */
    Audio* getAudio()
    {
        return audio;
    }
    /**
    * return input device
    */
    Input* getInput()
    {
        return input;
    }
    /**
    * save a resourcesGroup
    */
    virtual void subscriptionResourcesGroup(const String& name,ResourcesGroup *rsGr);
    /**
    * get a resourcesGroup
    */
    virtual ResourcesGroup* getResourcesGroup(const String& name);
    /**
    * erase a resourcesGroup
    */
    virtual void unsubscriptionResourcesGroup(const String& name);

protected:

    //resources group map
    DUNORDERED_MAP< String, ResourcesGroup* > groups;
    //
    String appname;
    float lastDeltaTime;
    Game *mainInstance;
    Screen *screen;
    Input *input;
    Audio *audio;
    //
    Application();
    static Application* appSingleton;
};

};

#endif
