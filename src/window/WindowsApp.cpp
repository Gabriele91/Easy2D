#include "stdafx.h"
#include <Timer.h>
#include <WindowsApp.h>
#include <WindowsScreen.h>
#include <WindowsInput.h>
#include <Audio.h>
#include <AudioAL.h>
#include <Shlobj.h>
#include <Socket.h>
#include <Debug.h>
//window
#include <direct.h>
//
///////////////////////
using namespace Easy2D;

WindowsApp::WindowsApp(const String& name)
    :Application()
{
    m_screen=(Screen*)new WindowsScreen();
    m_input=(Input*)new WindowsInput();
    m_audio=(Audio*)new AudioAL();//OpenAL
    //savename
    m_appname=name;
    /////////////////////////////////////
    //create appdirectory
    //init appdata folder
    TCHAR szPath[MAX_PATH];
    SHGetFolderPathA(((WindowsScreen*)get_screen())->hWind,
                     CSIDL_APPDATA|CSIDL_FLAG_CREATE,
                     NULL,
                     0,
                     szPath);
    dataPath = String(szPath) + '/' + m_appname;
    //create directory
    CreateDirectory(dataPath.c_str(),0);
    //get errors
    DWORD error=GetLastError();
    DEBUG_ASSERT(error != ERROR_PATH_NOT_FOUND);
    //DEBUG_ASSERT(error != ERROR_ALREADY_EXISTS);
    //REALTIME APPLICATION
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	/////////////////////////////////////
	//init sockets
	Socket::initializeSockets();
    /////////////////////////////////////
    //not exit from loop
    doexit=false;
}

WindowsApp::~WindowsApp()
{
    //delete audio
    delete m_audio;
    m_audio=nullptr;
    //delete screen
    delete m_screen;
    m_screen=nullptr;
    //delete input
    delete m_input;
	m_input = nullptr;
	//close sockets
	Socket::shutdownSockets ();
}

bool WindowsApp::load_data(const String& path,void*& ptr,size_t &len)
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

String WindowsApp::app_data_directory()
{
    return dataPath;
}

String WindowsApp::app_working_directory()
{
    char cCurrentPath[MAX_PATH];
    if(!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
    {
        DEBUG_MESSAGE("Error get working directory: "<<errno);
    }
    return cCurrentPath;
}

String WindowsApp::app_resources_directory()
{
    return app_working_directory();
}

void WindowsApp::exit()
{
    PostQuitMessage(0);
    doexit=true;
}

void WindowsApp::loop()
{
    //
    Timer timer;
    double msToSleep=1000.0/(static_cast<double>(m_screen->get_frame_rate()));
    double millipass=0;
    double dt=0;
    double sleepTime=0;
    //start timer
    timer.start();
    //draw loop
    while( !m_input->get_close() && !doexit )
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
        m_last_delta_time=(float)dt;
        //update
        update((float)dt);
        //update audio
        m_audio->update((float)dt);
        //update opengl
        m_screen->swap();
    }
}

void WindowsApp::exec(Game *ptrMainInstance)
{
	m_main_instance = ptrMainInstance;
	//set current context
	m_screen->acquire_context();
	//start
    m_main_instance->start();
	//loop
    loop();
	//end
    m_main_instance->end();
}

void WindowsApp::update(float dt)
{
    m_input->update();
    m_main_instance->run(dt);
}

bool WindowsApp::onlyPO2()
{
    return true;
}
