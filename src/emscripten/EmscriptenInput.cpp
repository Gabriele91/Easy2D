#include <EmscriptenInput.h>
#include <Input.h>
#include <SDL/SDL.h>
///////////////////////
using namespace Easy2D;
///////////////////////

Key::Keyboard KeyMapSDL[1256];

EmscriptenInput::EmscriptenInput()
{
    //set base value
    memset(KeyMapSDL,Key::KEY_NULL,sizeof(Key::Keyboard)*1256);
    //
    KeyMapSDL[SDLK_RETURN]= Key::RETURN;
    KeyMapSDL[SDLK_ESCAPE]= Key::ESCAPE;
    KeyMapSDL[SDLK_BACKSPACE]= Key::BACKSPACE;
    KeyMapSDL[SDLK_TAB]= Key::TAB;
    KeyMapSDL[SDLK_SPACE]= Key::SPACE;
    KeyMapSDL[SDLK_LEFTBRACKET]= Key::LEFTBRACKET;
    KeyMapSDL[SDLK_BACKSLASH]= Key::BACKSLASH;
    KeyMapSDL[SDLK_RIGHTBRACKET]= Key::RIGHTBRACKET;
    //KeyMapSDL[SDLK_EXCLAIM]='!';
    KeyMapSDL[SDLK_QUOTEDBL]= Key::QUOTE;
    KeyMapSDL[SDLK_QUOTE]= Key::APOSTROPHE;
    //KeyMapSDL[SDLK_CARET]= '^',
    //KeyMapSDL[SDLK_UNDERSCORE]= '_',
    KeyMapSDL[SDLK_BACKQUOTE]= Key::BACKQUOTE;
    //KeyMapSDL[SDLK_PLUS]= '+',
    KeyMapSDL[SDLK_COMMA]= Key::COMMA;
    KeyMapSDL[SDLK_MINUS]= Key::MINUS;
    KeyMapSDL[SDLK_PERIOD]= Key::PERIOD;
    KeyMapSDL[SDLK_SLASH]= Key::SLASH;

    //KeyMapSDL[SDLK_COLON]= ':'
    KeyMapSDL[SDLK_SEMICOLON]= Key::SEMICOLON;
    KeyMapSDL[SDLK_LESS]= Key::LESS;
    KeyMapSDL[SDLK_EQUALS]= Key::EQUALS;
    //KeyMapSDL[SDLK_GREATER]= '>',
    //KeyMapSDL[SDLK_QUESTION]= '?',
    //KeyMapSDL[SDLK_AT]= '@',

    KeyMapSDL[SDLK_0]= Key::N0;
    KeyMapSDL[SDLK_1]= Key::N1;
    KeyMapSDL[SDLK_2]= Key::N2;
    KeyMapSDL[SDLK_3]= Key::N3;
    KeyMapSDL[SDLK_4]= Key::N4;
    KeyMapSDL[SDLK_5]= Key::N5;
    KeyMapSDL[SDLK_6]= Key::N6;
    KeyMapSDL[SDLK_7]= Key::N7;
    KeyMapSDL[SDLK_8]= Key::N8;
    KeyMapSDL[SDLK_9]= Key::N9;

    KeyMapSDL[SDLK_a]= Key::A;
    KeyMapSDL[SDLK_b]= Key::B;
    KeyMapSDL[SDLK_c]= Key::C;
    KeyMapSDL[SDLK_d]= Key::D;
    KeyMapSDL[SDLK_e]= Key::E;
    KeyMapSDL[SDLK_f]= Key::F;
    KeyMapSDL[SDLK_g]= Key::G;
    KeyMapSDL[SDLK_h]= Key::H;
    KeyMapSDL[SDLK_i]= Key::I;
    KeyMapSDL[SDLK_j]= Key::J;
    KeyMapSDL[SDLK_k]= Key::K;
    KeyMapSDL[SDLK_l]= Key::L;
    KeyMapSDL[SDLK_m]= Key::M;
    KeyMapSDL[SDLK_n]= Key::N;
    KeyMapSDL[SDLK_o]= Key::O;
    KeyMapSDL[SDLK_p]= Key::P;
    KeyMapSDL[SDLK_q]= Key::Q;
    KeyMapSDL[SDLK_r]= Key::R;
    KeyMapSDL[SDLK_s]= Key::S;
    KeyMapSDL[SDLK_t]= Key::T;
    KeyMapSDL[SDLK_u]= Key::U;
    KeyMapSDL[SDLK_v]= Key::V;
    KeyMapSDL[SDLK_w]= Key::W;
    KeyMapSDL[SDLK_x]= Key::X;
    KeyMapSDL[SDLK_y]= Key::Y;
    KeyMapSDL[SDLK_z]= Key::Z;

    KeyMapSDL[SDLK_CAPSLOCK]= Key::CAPSLOCK;

    KeyMapSDL[SDLK_F1]= Key::F1;
    KeyMapSDL[SDLK_F2]= Key::F2;
    KeyMapSDL[SDLK_F3]= Key::F3;
    KeyMapSDL[SDLK_F4]= Key::F4;
    KeyMapSDL[SDLK_F5]= Key::F5;
    KeyMapSDL[SDLK_F6]= Key::F6;
    KeyMapSDL[SDLK_F7]= Key::F7;
    KeyMapSDL[SDLK_F8]= Key::F8;
    KeyMapSDL[SDLK_F9]= Key::F9;
    KeyMapSDL[SDLK_F10]= Key::F10;
    KeyMapSDL[SDLK_F11]= Key::F11;
    KeyMapSDL[SDLK_F12]= Key::F12;

    KeyMapSDL[SDLK_PRINTSCREEN]= Key::PRINT;
    KeyMapSDL[SDLK_SCROLLLOCK]= Key::SCROLLOCK;
    KeyMapSDL[SDLK_PAUSE]= Key::PAUSE;
    KeyMapSDL[SDLK_INSERT]= Key::INSERT;
    KeyMapSDL[SDLK_HOME]= Key::HOME;
    KeyMapSDL[SDLK_PAGEUP]= Key::PAGEUP;
    KeyMapSDL[SDLK_DELETE]= Key::KDELETE;
    KeyMapSDL[SDLK_END]= Key::END;
    KeyMapSDL[SDLK_PAGEDOWN]= Key::PAGEDOWN;
    KeyMapSDL[SDLK_RIGHT]= Key::RIGHT;
    KeyMapSDL[SDLK_LEFT]= Key::LEFT;
    KeyMapSDL[SDLK_DOWN]= Key::DOWN;
    KeyMapSDL[SDLK_UP]= Key::UP;

    KeyMapSDL[SDLK_NUMLOCKCLEAR]= Key::NUMLOCK;
    KeyMapSDL[SDLK_KP_DIVIDE]= Key::KP_DIVIDE;
    KeyMapSDL[SDLK_KP_MULTIPLY]= Key::KP_MULTIPLY;
    KeyMapSDL[SDLK_KP_MINUS]= Key::KP_MINUS;
    KeyMapSDL[SDLK_KP_PLUS]= Key::KP_PLUS;
    //KeyMapSDL[SDLK_KP_ENTER]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_ENTER),
    KeyMapSDL[SDLK_KP_1]= Key::KP1;
    KeyMapSDL[SDLK_KP_2]= Key::KP2;
    KeyMapSDL[SDLK_KP_3]= Key::KP3;
    KeyMapSDL[SDLK_KP_4]= Key::KP4;
    KeyMapSDL[SDLK_KP_5]= Key::KP5;
    KeyMapSDL[SDLK_KP_6]= Key::KP6;
    KeyMapSDL[SDLK_KP_7]= Key::KP7;
    KeyMapSDL[SDLK_KP_8]= Key::KP8;
    KeyMapSDL[SDLK_KP_9]= Key::KP9;
    KeyMapSDL[SDLK_KP_0]= Key::KP0;
    KeyMapSDL[SDLK_KP_PERIOD]= Key::KP_PERIOD;

    //KeyMapSDL[SDLK_APPLICATION]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_APPLICATION),
    KeyMapSDL[SDLK_POWER]= Key::POWER;
    //KeyMapSDL[SDLK_KP_EQUALS]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALS),
    KeyMapSDL[SDLK_F13]= Key::F13;
    KeyMapSDL[SDLK_F14]= Key::F14;
    KeyMapSDL[SDLK_F15]= Key::F15;
    //KeyMapSDL[SDLK_F16]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F16),
    //KeyMapSDL[SDLK_F17]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F17),
    //KeyMapSDL[SDLK_F18]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F18),
    //KeyMapSDL[SDLK_F19]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F19),
    //KeyMapSDL[SDLK_F20]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F20),
    //KeyMapSDL[SDLK_F21]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F21),
    //KeyMapSDL[SDLK_F22]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F22),
    //KeyMapSDL[SDLK_F23]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F23),
    //KeyMapSDL[SDLK_F24]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F24),
    //KeyMapSDL[SDLK_EXECUTE]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXECUTE),
    KeyMapSDL[SDLK_HELP]= Key::HELP;
    KeyMapSDL[SDLK_MENU]= Key::MENU;
    //KeyMapSDL[SDLK_SELECT]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SELECT),
    //KeyMapSDL[SDLK_STOP]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_STOP),
    //KeyMapSDL[SDLK_AGAIN]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AGAIN),
    //KeyMapSDL[SDLK_UNDO]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UNDO),
    //KeyMapSDL[SDLK_CUT]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CUT),
    //KeyMapSDL[SDLK_COPY]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_COPY),
    //KeyMapSDL[SDLK_PASTE]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PASTE),
    //KeyMapSDL[SDLK_FIND]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_FIND),
    //KeyMapSDL[SDLK_MUTE]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MUTE),
    //KeyMapSDL[SDLK_VOLUMEUP]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEUP),
    //KeyMapSDL[SDLK_VOLUMEDOWN]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEDOWN),
    //KeyMapSDL[SDLK_KP_COMMA]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COMMA),
    //KeyMapSDL[SDLK_KP_EQUALSAS400]=
    //KeyMapSDL[KeyMapSDL[SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALSAS400),


    //KeyMapSDL[SDLK_ALTERASE]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_ALTERASE),
    //KeyMapSDL[SDLK_SYSREQ]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SYSREQ),
    //KeyMapSDL[SDLK_CANCEL]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CANCEL),
    KeyMapSDL[SDLK_CLEAR]=Key::CLEAR;
    //KeyMapSDL[SDLK_PRIOR]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRIOR),
    //KeyMapSDL[SDLK_RETURN2]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RETURN2),
    //KeyMapSDL[SDLK_SEPARATOR]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SEPARATOR),
    //KeyMapSDL[SDLK_OUT]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OUT),
    //KeyMapSDL[SDLK_OPER]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OPER),
    //KeyMapSDL[SDLK_CLEARAGAIN]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEARAGAIN),
    //KeyMapSDL[SDLK_CRSEL]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CRSEL),
    //KeyMapSDL[SDLK_EXSEL]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXSEL),

    //KeyMapSDL[SDLK_KP_00]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_00),
    //KeyMapSDL[SDLK_KP_000]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_000),
    //KeyMapSDL[SDLK_THOUSANDSSEPARATOR]=
    //KeyMapSDL[KeyMapSDL[SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_THOUSANDSSEPARATOR),
    //KeyMapSDL[SDLK_DECIMALSEPARATOR]=
    //KeyMapSDL[KeyMapSDL[SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DECIMALSEPARATOR),
    //KeyMapSDL[SDLK_CURRENCYUNIT]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYUNIT),
    //KeyMapSDL[SDLK_CURRENCYSUBUNIT]=
    //KeyMapSDL[KeyMapSDL[SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYSUBUNIT),
    //KeyMapSDL[SDLK_KP_LEFTPAREN]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTPAREN),
    //KeyMapSDL[SDLK_KP_RIGHTPAREN]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTPAREN),
    //KeyMapSDL[SDLK_KP_LEFTBRACE]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTBRACE),
    //KeyMapSDL[SDLK_KP_RIGHTBRACE]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTBRACE),
    //KeyMapSDL[SDLK_KP_TAB]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_TAB),
    //KeyMapSDL[SDLK_KP_BACKSPACE]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BACKSPACE),
    //KeyMapSDL[SDLK_KP_A]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_A),
    //KeyMapSDL[SDLK_KP_B]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_B),
    //KeyMapSDL[SDLK_KP_C]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_C),
    //KeyMapSDL[SDLK_KP_D]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_D),
    //KeyMapSDL[SDLK_KP_E]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_E),
    //KeyMapSDL[SDLK_KP_F]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_F),
    //KeyMapSDL[SDLK_KP_XOR]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_XOR),
    //KeyMapSDL[SDLK_KP_POWER]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_POWER),
    //KeyMapSDL[SDLK_KP_PERCENT]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERCENT),
    //KeyMapSDL[SDLK_KP_LESS]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LESS),
    //KeyMapSDL[SDLK_KP_GREATER]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_GREATER),
    //KeyMapSDL[SDLK_KP_AMPERSAND]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AMPERSAND),
    //KeyMapSDL[SDLK_KP_DBLAMPERSAND]=
    //KeyMapSDL[KeyMapSDL[SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLAMPERSAND),
    //KeyMapSDL[SDLK_KP_VERTICALBAR]=
    //KeyMapSDL[KeyMapSDL[SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_VERTICALBAR),
    //KeyMapSDL[SDLK_KP_DBLVERTICALBAR]=
    //KeyMapSDL[KeyMapSDL[SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLVERTICALBAR),
    //KeyMapSDL[SDLK_KP_COLON]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COLON),
    //KeyMapSDL[SDLK_KP_HASH]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HASH),
    //KeyMapSDL[SDLK_KP_SPACE]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_SPACE),
    //KeyMapSDL[SDLK_KP_AT]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AT),
    //KeyMapSDL[SDLK_KP_EXCLAM]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EXCLAM),
    //KeyMapSDL[SDLK_KP_MEMSTORE]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSTORE),
    //KeyMapSDL[SDLK_KP_MEMRECALL]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMRECALL),
    //KeyMapSDL[SDLK_KP_MEMCLEAR]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMCLEAR),
    //KeyMapSDL[SDLK_KP_MEMADD]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMADD),
    //KeyMapSDL[SDLK_KP_MEMSUBTRACT]=
    //KeyMapSDL[KeyMapSDL[SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSUBTRACT),
    //KeyMapSDL[SDLK_KP_MEMMULTIPLY]=
    //KeyMapSDL[KeyMapSDL[SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMMULTIPLY),
    //KeyMapSDL[SDLK_KP_MEMDIVIDE]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMDIVIDE),
    ////KeyMapSDL[SDLK_KP_PLUSMINUS]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUSMINUS),
    //KeyMapSDL[SDLK_KP_CLEAR]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEAR),
    //KeyMapSDL[SDLK_KP_CLEARENTRY]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEARENTRY),
    //KeyMapSDL[SDLK_KP_BINARY]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BINARY),
    //KeyMapSDL[SDLK_KP_OCTAL]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_OCTAL),
    //KeyMapSDL[SDLK_KP_DECIMAL]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DECIMAL),
    //KeyMapSDL[SDLK_KP_HEXADECIMAL]=
    //KeyMapSDL[KeyMapSDL[SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HEXADECIMAL),

    KeyMapSDL[SDLK_LCTRL]= Key::LCTRL;
    KeyMapSDL[SDLK_LSHIFT]= Key::LSHIFT;
    KeyMapSDL[SDLK_LALT]= Key::LALT;
    KeyMapSDL[SDLK_LGUI]= Key::LMETA;
    KeyMapSDL[SDLK_RCTRL]= Key::RCTRL;
    KeyMapSDL[SDLK_RSHIFT]= Key::RSHIFT;
    KeyMapSDL[SDLK_RALT]= Key::RALT;
    KeyMapSDL[SDLK_RGUI]= Key::RMETA;

    //KeyMapSDL[SDLK_MODE]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MODE),

    //KeyMapSDL[SDLK_AUDIONEXT]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIONEXT),
    //KeyMapSDL[SDLK_AUDIOPREV]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOPREV),
    //KeyMapSDL[SDLK_AUDIOSTOP]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOSTOP),
    //KeyMapSDL[SDLK_AUDIOPLAY]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOPLAY),
    //KeyMapSDL[SDLK_AUDIOMUTE]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOMUTE),
    //KeyMapSDL[SDLK_MEDIASELECT]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIASELECT),
    //KeyMapSDL[SDLK_WWW]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_WWW),
    //KeyMapSDL[SDLK_MAIL]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MAIL),
    //KeyMapSDL[SDLK_CALCULATOR]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CALCULATOR),
    //KeyMapSDL[SDLK_COMPUTER]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_COMPUTER),
    //KeyMapSDL[SDLK_AC_SEARCH]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_SEARCH),
    //KeyMapSDL[SDLK_AC_HOME]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_HOME),
    //KeyMapSDL[SDLK_AC_BACK]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BACK),
    //KeyMapSDL[SDLK_AC_FORWARD]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_FORWARD),
    //KeyMapSDL[SDLK_AC_STOP]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_STOP),
    //KeyMapSDL[SDLK_AC_REFRESH]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_REFRESH),
    //KeyMapSDL[SDLK_AC_BOOKMARKS]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BOOKMARKS),

    //KeyMapSDL[SDLK_BRIGHTNESSDOWN]=SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_BRIGHTNESSDOWN),
    //KeyMapSDL[SDLK_BRIGHTNESSUP]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_BRIGHTNESSUP),
    //KeyMapSDL[SDLK_DISPLAYSWITCH]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DISPLAYSWITCH),
    //KeyMapSDL[SDLK_KBDILLUMTOGGLE]=SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMTOGGLE),
    //KeyMapSDL[SDLK_KBDILLUMDOWN]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMDOWN),
    //KeyMapSDL[SDLK_KBDILLUMUP]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMUP),
    //KeyMapSDL[SDLK_EJECT]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EJECT),
    //KeyMapSDL[SDLK_SLEEP]= SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SLEEP),
    //KeyMapSDL[SDLK_LAST ]= SDL_SCANCODE_TO_KEYCODE(SDL_NUM_SCANCODES)

    //input window
    ewindow.__init();
    //input keyboard hit
    ekeyboard.__init();
    //input mouse hit
    emouse.__init();

}


void EmscriptenInput::__update(SDL_Event& event)
{
    //events
    switch(event.type)
    {
    //Maximized
    /*
    case MapNotify://show
    break;
    case UnmapNotify: //hide
        ewindow.maximized=true;
    break;
    //focus event
    case FocusIn:
        //
        ewindow.focus=true;
        //
    break;
    case FocusOut:
        //reset windows input
        if(ewindow.focus){
            //reset window
            ewindow.__init();
            //reset keyboard hit
            ekeyboard.__init();
            //reset mouse hit
            emouse.__init();
        }
        //
        ewindow.focus=false;
        //
    break;
    //close event
    case ClientMessage:
        if (strcmp(XGetAtomName(display, event.xclient.message_type), "WM_PROTOCOLS") == 0)
        ewindow.close=true;
    break;
    //resize
    case Expose:
        ewindow.resize=true;
    	ewindow.windowResize.x=event.xexpose.width;
    	ewindow.windowResize.y=event.xexpose.height;
    break;
    */
    //keyboard
    case SDL_KEYDOWN:
#define keyDownEvent(key) \
                { \
                    if(ekeyboard.status[KeyMapSDL[(key)]]==0){\
                        ekeyboard.__keyboardDown(KeyMapSDL[(key)]);\
                        __callOnKeyPress(KeyMapSDL[(key)]);\
                        }\
                }

        keyDownEvent(event.key.keysym.sym)

#undef keyDownEvent
        break;
    case SDL_KEYUP:
#define keyReleaseEvent(key) \
                { \
                ekeyboard.__keyboardUp(KeyMapSDL[(key)]);\
                __callOnKeyRelease(KeyMapSDL[(key)]);\
                }

        keyReleaseEvent(event.key.keysym.sym)

#undef keyReleaseEvent
        break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        //location of mause
    {
        SDL_MouseButtonEvent *m = (SDL_MouseButtonEvent*)&event;
        emouse.pos.x=(float)m->x;
        emouse.pos.y=(float)m->y;

#define isUpOrDownButtom(key)\
			if(event.type==SDL_MOUSEBUTTONUP){\
				emouse.__mouseUp(key);\
				__callOnMouseRelease(emouse.pos,key);\
			}\
			else{\
				emouse.__mouseDown(key);\
				__callOnMousePress(emouse.pos,key);\
			}
        //buttom events
        switch(((SDL_MouseButtonEvent*)&event)->button)
        {
        case 1: // left
            isUpOrDownButtom(Key::BUTTON_LEFT) break;
        case 2: // middle
            isUpOrDownButtom(Key::BUTTON_MIDDLE) break;
        case 3: // right
            isUpOrDownButtom(Key::BUTTON_RIGHT) break;
        // 120 like windows value
        case 4:
            emouse.scroll=120;
            __callOnMouseScroll(emouse.scroll);
            break;
        case 5:
            emouse.scroll=-120;
            __callOnMouseScroll(emouse.scroll);
            break;
        default:
            break;
        }
    }
#undef isUpOrDownButtom
    break;
    case SDL_MOUSEMOTION:
    {
        SDL_MouseButtonEvent *m = (SDL_MouseButtonEvent*)&event;
        emouse.pos.x=(float)m->x;
        emouse.pos.y=(float)m->y;
    }
    __callOnMouseMove(emouse.pos);
    break;
    }

}
void EmscriptenInput::update()
{
    //update input
    //reset input window
    ewindow.resize=false;
    //update hit
    ekeyboard.__clearHit();
    emouse.__clearHit();
    //update SDL events
    SDL_Event event;
    while (SDL_PollEvent(&event)) __update(event);
    ///////////////////////////LOOP EVENT
    //update down keys
    ekeyboard.__update(this);
    emouse.__update(this);

}


//calls
void EmscriptenInput::__callOnKeyPress(Key::Keyboard key)
{
    for(size_t i=0; i!=vkeyboardh.size(); ++i)
        vkeyboardh[i]->onKeyPress(key);
}
void EmscriptenInput::__callOnKeyRelease(Key::Keyboard key)
{
    for(size_t i=0; i!=vkeyboardh.size(); ++i)
        vkeyboardh[i]->onKeyRelease(key);
}
void EmscriptenInput::__callOnKeyDown(Key::Keyboard key)
{
    for(size_t i=0; i!=vkeyboardh.size(); ++i)
        vkeyboardh[i]->onKeyDown(key);
}
//mouse
void EmscriptenInput::__callOnMouseMove(Vec2 mousePosition)
{
    for(size_t i=0; i!=vmouseh.size(); ++i)
        vmouseh[i]->onMouseMove(mousePosition);
}
void EmscriptenInput::__callOnMousePress(Vec2 mousePosition, Key::Mouse button)
{
    for(size_t i=0; i!=vmouseh.size(); ++i)
        vmouseh[i]->onMousePress(mousePosition,button);
}
void EmscriptenInput::__callOnMouseDown(Vec2 mousePosition, Key::Mouse button)
{
    for(size_t i=0; i!=vmouseh.size(); ++i)
        vmouseh[i]->onMouseDown(mousePosition,button);
}
void EmscriptenInput::__callOnMouseRelease(Vec2 mousePosition, Key::Mouse button)
{
    for(size_t i=0; i!=vmouseh.size(); ++i)
        vmouseh[i]->onMouseRelease(mousePosition,button);
}
void EmscriptenInput::__callOnMouseScroll(short scrollDelta)
{
    for(size_t i=0; i!=vmouseh.size(); ++i)
        vmouseh[i]->onMouseScroll(scrollDelta);
}
