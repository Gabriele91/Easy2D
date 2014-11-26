#include "stdafx.h"
#include <Timer.h>
#include <AngleApp.h>
#include <AngleScreen.h>
#include <AngleInput.h>
#include <Audio.h>
#include <AudioAL.h>
#include <Shlobj.h>
#include <Debug.h>
//window
#include <direct.h>
//
///////////////////////
using namespace Easy2D;

AngleApp::AngleApp(const String& name)
    :Application()
{
    screen=(Screen*)new AngleScreen();
    input=(Input*)new AngleInput();
    audio=(Audio*)new AudioAL();//OpenAL
    //savename
    appname=name;
    /////////////////////////////////////
    //create appdirectory
    //init appdata folder
    TCHAR szPath[MAX_PATH];
    SHGetFolderPathA(((AngleScreen*)getScreen())->hWind,
                     CSIDL_APPDATA|CSIDL_FLAG_CREATE,
                     NULL,
                     0,
                     szPath);
    dataPath = String(szPath) + '/' + appname;
    //create directory
    CreateDirectory(dataPath.c_str(),0);
    //get errors
    DWORD error=GetLastError();
    DEBUG_ASSERT(error != ERROR_PATH_NOT_FOUND);
    //DEBUG_ASSERT(error != ERROR_ALREADY_EXISTS);
    //REALTIME APPLICATION
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    /////////////////////////////////////
    //not exit from loop
    doexit=false;
}

AngleApp::~AngleApp()
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

bool AngleApp::loadData(const String& path,void*& ptr,size_t &len)
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

String AngleApp::appDataDirectory()
{
    return dataPath;
}

String AngleApp::appWorkingDirectory()
{
    char cCurrentPath[MAX_PATH];
    if(!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
    {
        DEBUG_MESSAGE("Error get working directory: "<<errno);
    }
    return cCurrentPath;
}

String AngleApp::appResourcesDirectory()
{
    return appWorkingDirectory();
}

void AngleApp::exit()
{
    PostQuitMessage(0);
    doexit=true;
}

void AngleApp::loop()
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
        while(sleepTime>0 && sleepTime<60000.0 )
        {
            Sleep(sleepTime>10?1:0);
            millipass=timer.getGetCounter();
            sleepTime=msToSleep-millipass;
        }
        //calc dt
        dt=millipass/1000.0;
        timer.reset();
        //save dt
        lastDeltaTime=(float)dt;
        //update
        update((float)dt);
        //update opengl
        screen->swap();
    }
}

void AngleApp::exec(Game *ptrMainInstance)
{
    mainInstance=ptrMainInstance;
    mainInstance->start();
    loop();
    mainInstance->end();
}

void AngleApp::update(float dt)
{
    input->update();
    mainInstance->run(dt);
}

bool AngleApp::onlyPO2()
{
    return true;
}
