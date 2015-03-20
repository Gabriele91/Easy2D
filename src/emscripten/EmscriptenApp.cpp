
#include "stdafx.h"
#include <Timer.h>
#include <SDL\SDL.h>
#include <emscripten.h>
#include <EmscriptenApp.h>
#include <EmscriptenScreen.h>
#include <EmscriptenInput.h>
#include <AudioAL.h>
#include <Debug.h>
#include <sys/time.h>
#include <unistd.h>
///////////////////////
using namespace Easy2D;

EmscriptenApp::EmscriptenApp()
    :Application()
{
    //init SDL
    SDL_Init(SDL_INIT_VIDEO);
    //init C++ class
    screen=(Screen*)new EmscriptenScreen();
    input=(Input*)new EmscriptenInput();
    audio=(Audio*)new AudioAL();
    //not exit form loop
    doexit=false;
}

EmscriptenApp::~EmscriptenApp()
{
    //delete audio
    delete audio;
    audio=NULL;
    //delete screen
    delete screen;
    screen=NULL;
    //delete input
    delete input;
    input=NULL;
    //close SDL
    SDL_Quit();
}

bool EmscriptenApp::loadData(const String& path,void*& ptr,size_t &len)
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

String EmscriptenApp::appDataDirectory()
{
    return "";
}

String EmscriptenApp::appWorkingDirectory()
{
    char cCurrentPath[PATH_MAX];
    if(!getcwd(cCurrentPath, sizeof(cCurrentPath)))
    {
        DEBUG_MESSAGE("Error get working directory: "<<errno);
    }
    return cCurrentPath;
}

String EmscriptenApp::appResourcesDirectory()
{
    return appWorkingDirectory();
}

void EmscriptenApp::exit()
{
    doexit=true;
}

void EmscriptenApp::loop()
{
    emscripten_set_main_loop([]()
    {
        //cast
        EmscriptenApp* _self=(EmscriptenApp*)Application::instance();
        EmscriptenScreen* _self_screen=(EmscriptenScreen*)Application::instance()->getScreen();
        EmscriptenInput* _self_input=(EmscriptenInput*)Application::instance()->getInput();
        AudioAL* _self_audio=(AudioAL*)Application::instance()->getAudio();
        //save dt
        _self->lastDeltaTime=1.0/(double)_self_screen->getFrameRate();
        //update
        _self->update(_self->lastDeltaTime);
        //update audio
        _self_audio->update((float)dt);
        //update opengl
        _self_screen->swap();
        //exit (!doexit)
        if(_self_input->getClose()||_self->doexit)
            emscripten_cancel_main_loop();
    }
    ,screen->getFrameRate()
    ,1);
}

void EmscriptenApp::exec(Game *ptrGame)
{
    mainInstance=ptrGame;
    mainInstance->start();
    //
    loop();
    mainInstance->end();
}

void EmscriptenApp::update(float dt)
{
    input->update();
    mainInstance->run(dt);
}

bool EmscriptenApp::onlyPO2()
{
    return true;
}
