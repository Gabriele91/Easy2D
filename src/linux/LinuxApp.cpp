
#include "stdafx.h"
#include <Timer.h>
#include <LinuxApp.h>
#include <LinuxScreen.h>
#include <LinuxInput.h>
#include <Audio.h>
#include <AudioAL.h>
#include <Debug.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
///////////////////////
using namespace Easy2D;

LinuxApp::LinuxApp(const String& appdir)
    :Application()
{
    screen=(Screen*)new LinuxScreen();
    input=(Input*)new LinuxInput();
    audio=(Audio*)new AudioAL();//OpenAL
    //get home directory
    struct passwd *pw = getpwuid(getuid());
    //create path
    String localDir("/."+appdir);
    dataPath=pw->pw_dir+localDir;
    //create directory
    struct stat st = {0};
    if (stat(dataPath, &st) == -1) {
        mkdir(dataPath, 0700);
    }
    //not exit form loop
    doexit=false;
}

LinuxApp::~LinuxApp()
{
    //delete audio
    delete audio;
    audio=nullptr;
    //delete screen
    delete screen;
    screen=nullptr;
    //delete input
    delete input;
    input=nullptr;
}

bool LinuxApp::loadData(const String& path,void*& ptr,size_t &len)
{
    //open
    FILE *pfile=fopen(path,"rb");
    DEBUG_MESSAGE("load file: "<<path);
    DEBUG_ASSERT_MSG(pfile,"error load file: "<<path);
    //get size
    fseek(pfile,0,SEEK_END);
    len=ftell(pfile);
    fseek(pfile,0,SEEK_SET);
    //read
    ptr=malloc(len*sizeof(char)+1);
    fread(ptr,len,1,pfile);
    (*((char*)ptr+len))='\0';
    //close
    fclose(pfile);
    return pfile!=NULL;
}

String LinuxApp::appDataDirectory()
{
    return dataPath;
}

String LinuxApp::appWorkingDirectory()
{
    char cCurrentPath[PATH_MAX];
    if(!getcwd(cCurrentPath, sizeof(cCurrentPath)))
    {
        DEBUG_MESSAGE("Error get working directory: "<<errno);
    }
    return cCurrentPath;
}

String LinuxApp::appResourcesDirectory()
{
    return appWorkingDirectory();
}

void LinuxApp::exit()
{
    doexit=true;
}

void LinuxApp::loop()
{
    //
    Timer timer;
    double msToSleep=1000.0/(static_cast<double>(screen->getFrameRate()));
    double millipass=0;
    double dt=0;
    double sleepTime=0;
    //start timer
    timer.start();
    //set current context
    screen->acquireContext();
    //draw loop
    while( !input->getClose() && !doexit )
    {
        //get timer values
        millipass=timer.getGetCounter();
        //calc dt and sleep time
        sleepTime=msToSleep-millipass;
        while(sleepTime>0 && sleepTime<60000 )
        {
            //scheduler linux is faster then window
            usleep ((sleepTime>9?1:0)*1000);
            millipass=timer.getGetCounter();
            sleepTime=msToSleep-millipass;
        }
        //calc dt
        dt=millipass/1000.0;
        timer.reset();
        //save dt
        lastDeltaTime=dt;
        //update
        update((float)dt);
        //update audio
        audio->update((float)dt);
        //update opengl
        screen->swap();
    }
}

void LinuxApp::exec(Game *ptrGame)
{
    mainInstance=ptrGame;
    mainInstance->start();
    //setup input
    //to do fix
    ((LinuxInput*)input)->__setDisplay(((LinuxScreen*)screen)->display);
    //
    //
    loop();
    mainInstance->end();
}

void LinuxApp::update(float dt)
{
    input->update();
    mainInstance->run(dt);
}

bool LinuxApp::onlyPO2()
{
    return true;
}
