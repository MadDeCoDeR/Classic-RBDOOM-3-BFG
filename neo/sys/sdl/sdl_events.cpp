/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 dhewg (dhewm3)
Copyright (C) 2012 Robert Beckebans
Copyright (C) 2013 Daniel Gibson
Copyright (C) 2018 George Kalampokis

This file is part of the Doom 3 GPL Source Code ("Doom 3 Source Code").

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.	If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#include "../../idlib/precompiled.h"

// DG: SDL.h somehow needs the following functions, so #undef those silly
//     "don't use" #defines from Str.h
#undef strncmp
#undef strcasecmp
#undef vsnprintf
// DG end

#include <SDL.h>
#if !SDL_VERSION_ATLEAST(2, 0, 0)
#include "SDL_thread.h"
#endif

#include "renderer/RenderCommon.h"
#include "sdl_local.h"
#include "../posix/posix_public.h"
#include "../common/localuser.h"
#include "../../framework/Common.h"

#if !SDL_VERSION_ATLEAST(2, 0, 0)
#define SDL_Keycode SDLKey
#define SDLK_APPLICATION SDLK_COMPOSE
#define SDLK_SCROLLLOCK SDLK_SCROLLOCK
#define SDLK_LGUI SDLK_LSUPER
#define SDLK_RGUI SDLK_RSUPER
#define SDLK_KP_0 SDLK_KP0
#define SDLK_KP_1 SDLK_KP1
#define SDLK_KP_2 SDLK_KP2
#define SDLK_KP_3 SDLK_KP3
#define SDLK_KP_4 SDLK_KP4
#define SDLK_KP_5 SDLK_KP5
#define SDLK_KP_6 SDLK_KP6
#define SDLK_KP_7 SDLK_KP7
#define SDLK_KP_8 SDLK_KP8
#define SDLK_KP_9 SDLK_KP9
#define SDLK_NUMLOCKCLEAR SDLK_NUMLOCK
#define SDLK_PRINTSCREEN SDLK_PRINT
// DG: SDL1 doesn't seem to have defines for scancodes.. add the (only) one we need
#define SDL_SCANCODE_GRAVE 49 // in SDL2 this is 53.. but according to two different systems and keyboards this works for SDL1
// DG end
#endif

static const int MAX_JOYSTICKS = 4; //GK: This thing still works only on PC right? Apparently no

// DG: those are needed for moving/resizing windows
extern idCVar r_windowX;
extern idCVar r_windowY;
extern idCVar r_windowWidth;
extern idCVar r_windowHeight;
extern idCVar com_emergencyexit;
// DG end
SDL_Thread* joyThread = nullptr;
//GK: This function will run as a thread in order to capture when a joystick is connected or disconnected
#if SDL_VERSION_ATLEAST(2, 0, 0)
void JoystickSamplingThread( void* data );
#else
int JoystickSamplingThread( void* data ); //GK: SDL 1.2 require the thread to have an int as return value
#endif
//GK End

const char* kbdNames[] =
{
	"english", "french", "german", "italian", "spanish", "turkish", "norwegian", NULL
};

idCVar in_keyboard( "in_keyboard", "english", CVAR_SYSTEM | CVAR_ARCHIVE | CVAR_NOCHEAT, "keyboard layout", kbdNames, idCmdSystem::ArgCompletion_String<kbdNames> );

struct kbd_poll_t
{
	int key;
	bool state;
	
	kbd_poll_t()
	{
	}
	
	kbd_poll_t( int k, bool s )
	{
		key = k;
		state = s;
	}
};

struct mouse_poll_t
{
	int action;
	int value;
	
	mouse_poll_t()
	{
	}
	
	mouse_poll_t( int a, int v )
	{
		action = a;
		value = v;
	}
};

static idList<kbd_poll_t> kbd_polls;
static idList<mouse_poll_t> mouse_polls;

struct joystick_poll_t
{
	int action;
	int value;
	
	// joystick_poll_t()
	// {
	// }
	
	// joystick_poll_t( int a, int v )
	// {
	// 	action = a;
	// 	value = v;
	// }
};

struct joyState {
	int buttons[15];
	int LTrigger;
	int RTrigger;
	int LXThumb;
	int LYThumb;
	int RXThumb;
	int RYThumb; 
};

static joyState current[4];
static joyState old[4];
static joystick_poll_t joystick_polls[42];
static int numEvents = 0;
int joyAxis[MAX_JOYSTICKS][6];
SDL_Joystick* joy = NULL;
#if SDL_VERSION_ATLEAST(2, 0, 0)
static SDL_GameController* gcontroller[MAX_JOYSTICKS] = {NULL}; //GK: Keep the SDL_Controller global in order to free the SDL_Controller when it's get disconnected
static SDL_Haptic *haptic[MAX_JOYSTICKS] = {NULL}; //GK: Joystick rumble support
static int registeredControllers = 0;
#endif
static bool joyThreadKill = false;
int SDL_joystick_has_hat = 0;
bool buttonStates[MAX_JOYSTICKS][K_LAST_KEY];	// For keeping track of button up/down events

#define	MAX_QUED_EVENTS		256
#define	MASK_QUED_EVENTS	( MAX_QUED_EVENTS - 1 )

sysEvent_t	eventQue[MAX_QUED_EVENTS];
int			eventHead = 0;
int			eventTail = 0;

#if SDL_VERSION_ATLEAST(2, 0, 0)

#include "sdl2_scancode_mappings.h"

static int SDLScanCodeToKeyNum( SDL_Scancode sc )
{
	int idx = int( sc );
	assert( idx >= 0 && idx < SDL_NUM_SCANCODES );
	
	return scanCodeToKeyNum[idx];
}

static SDL_Scancode KeyNumToSDLScanCode( int keyNum )
{
	if( keyNum < K_JOY1 )
	{
		for( int i = 0; i < SDL_NUM_SCANCODES; ++i )
		{
			if( scanCodeToKeyNum[i] == keyNum )
			{
				return SDL_Scancode( i );
			}
		}
	}
	return SDL_SCANCODE_UNKNOWN;
}

// both strings are expected to have at most SDL_TEXTINPUTEVENT_TEXT_SIZE chars/ints (including terminating null)
static void ConvertUTF8toUTF32( const char* utf8str, int32* utf32buf )
{
	static SDL_iconv_t cd = SDL_iconv_t( -1 );
	
	if( cd == SDL_iconv_t( -1 ) )
	{
		const char* toFormat = "UTF-32LE"; // TODO: what does d3bfg expect on big endian machines?
		cd = SDL_iconv_open( toFormat, "UTF-8" );
		if( cd == SDL_iconv_t( -1 ) )
		{
			common->Warning( "Couldn't initialize SDL_iconv for UTF-8 to UTF-32!" ); // TODO: or error?
			return;
		}
	}
	
	size_t len = strlen( utf8str );
	
	size_t inbytesleft = len;
	size_t outbytesleft = 4 * SDL_TEXTINPUTEVENT_TEXT_SIZE; // *4 because utf-32 needs 4x as much space as utf-8
	char* outbuf = ( char* )utf32buf;
	size_t n = SDL_iconv( cd, &utf8str, &inbytesleft, &outbuf, &outbytesleft );
	
	if( n == size_t( -1 ) ) // some error occured during iconv
	{
		common->Warning( "Converting UTF-8 string \"%s\" from SDL_TEXTINPUT to UTF-32 failed!", utf8str );
		
		// clear utf32-buffer, just to be sure there's no garbage..
		memset( utf32buf, 0, SDL_TEXTINPUTEVENT_TEXT_SIZE * sizeof( int32 ) );
	}
	
	// reset cd so it can be used again
	SDL_iconv( cd, NULL, &inbytesleft, NULL, &outbytesleft );
	
}

#else // SDL1.2
static int SDL_KeyToDoom3Key( SDL_Keycode key, bool& isChar )
{
	isChar = false;
	
	if( key >= SDLK_SPACE && key < SDLK_DELETE )
	{
		isChar = true;
		//return key;// & 0xff;
	}
	
	switch( key )
	{
		case SDLK_ESCAPE:
			return K_ESCAPE;
			
		case SDLK_SPACE:
			return K_SPACE;
			
		//case SDLK_EXCLAIM:
		/*
		SDLK_QUOTEDBL:
		SDLK_HASH:
		SDLK_DOLLAR:
		SDLK_AMPERSAND:
		SDLK_QUOTE		= 39,
		SDLK_LEFTPAREN		= 40,
		SDLK_RIGHTPAREN		= 41,
		SDLK_ASTERISK		= 42,
		SDLK_PLUS		= 43,
		SDLK_COMMA		= 44,
		SDLK_MINUS		= 45,
		SDLK_PERIOD		= 46,
		SDLK_SLASH		= 47,
		*/
		case SDLK_0:
			return K_0;
			
		case SDLK_1:
			return K_1;
			
		case SDLK_2:
			return K_2;
			
		case SDLK_3:
			return K_3;
			
		case SDLK_4:
			return K_4;
			
		case SDLK_5:
			return K_5;
			
		case SDLK_6:
			return K_6;
			
		case SDLK_7:
			return K_7;
			
		case SDLK_8:
			return K_8;
			
		case SDLK_9:
			return K_9;
			
		// DG: add some missing keys..
		case SDLK_UNDERSCORE:
			return K_UNDERLINE;
			
		case SDLK_MINUS:
			return K_MINUS;
			
		case SDLK_COMMA:
			return K_COMMA;
			
		case SDLK_COLON:
			return K_COLON;
			
		case SDLK_SEMICOLON:
			return K_SEMICOLON;
			
		case SDLK_PERIOD:
			return K_PERIOD;
			
		case SDLK_AT:
			return K_AT;
			
		case SDLK_EQUALS:
			return K_EQUALS;
		// DG end
		
		/*
		SDLK_COLON		= 58,
		SDLK_SEMICOLON		= 59,
		SDLK_LESS		= 60,
		SDLK_EQUALS		= 61,
		SDLK_GREATER		= 62,
		SDLK_QUESTION		= 63,
		SDLK_AT			= 64,
		*/
		/*
		   Skip uppercase letters
		 */
		/*
		SDLK_LEFTBRACKET	= 91,
		SDLK_BACKSLASH		= 92,
		SDLK_RIGHTBRACKET	= 93,
		SDLK_CARET		= 94,
		SDLK_UNDERSCORE		= 95,
		SDLK_BACKQUOTE		= 96,
		*/
		
		case SDLK_a:
			return K_A;
			
		case SDLK_b:
			return K_B;
			
		case SDLK_c:
			return K_C;
			
		case SDLK_d:
			return K_D;
			
		case SDLK_e:
			return K_E;
			
		case SDLK_f:
			return K_F;
			
		case SDLK_g:
			return K_G;
			
		case SDLK_h:
			return K_H;
			
		case SDLK_i:
			return K_I;
			
		case SDLK_j:
			return K_J;
			
		case SDLK_k:
			return K_K;
			
		case SDLK_l:
			return K_L;
			
		case SDLK_m:
			return K_M;
			
		case SDLK_n:
			return K_N;
			
		case SDLK_o:
			return K_O;
			
		case SDLK_p:
			return K_P;
			
		case SDLK_q:
			return K_Q;
			
		case SDLK_r:
			return K_R;
			
		case SDLK_s:
			return K_S;
			
		case SDLK_t:
			return K_T;
			
		case SDLK_u:
			return K_U;
			
		case SDLK_v:
			return K_V;
			
		case SDLK_w:
			return K_W;
			
		case SDLK_x:
			return K_X;
			
		case SDLK_y:
			return K_Y;
			
		case SDLK_z:
			return K_Z;
			
		case SDLK_RETURN:
			return K_ENTER;
			
		case SDLK_BACKSPACE:
			return K_BACKSPACE;
			
		case SDLK_PAUSE:
			return K_PAUSE;
			
		// DG: add tab key support
		case SDLK_TAB:
			return K_TAB;
		// DG end
		
		//case SDLK_APPLICATION:
		//	return K_COMMAND;
		case SDLK_CAPSLOCK:
			return K_CAPSLOCK;
			
		case SDLK_SCROLLLOCK:
			return K_SCROLL;
			
		case SDLK_POWER:
			return K_POWER;
			
		case SDLK_UP:
			return K_UPARROW;
			
		case SDLK_DOWN:
			return K_DOWNARROW;
			
		case SDLK_LEFT:
			return K_LEFTARROW;
			
		case SDLK_RIGHT:
			return K_RIGHTARROW;
			
		case SDLK_LGUI:
			return K_LWIN;
			
		case SDLK_RGUI:
			return K_RWIN;
		//case SDLK_MENU:
		//	return K_MENU;
		
		case SDLK_LALT:
			return K_LALT;
			
		case SDLK_RALT:
			return K_RALT;
			
		case SDLK_RCTRL:
			return K_RCTRL;
			
		case SDLK_LCTRL:
			return K_LCTRL;
			
		case SDLK_RSHIFT:
			return K_RSHIFT;
			
		case SDLK_LSHIFT:
			return K_LSHIFT;
			
		case SDLK_INSERT:
			return K_INS;
			
		case SDLK_DELETE:
			return K_DEL;
			
		case SDLK_PAGEDOWN:
			return K_PGDN;
			
		case SDLK_PAGEUP:
			return K_PGUP;
			
		case SDLK_HOME:
			return K_HOME;
			
		case SDLK_END:
			return K_END;
			
		case SDLK_F1:
			return K_F1;
			
		case SDLK_F2:
			return K_F2;
			
		case SDLK_F3:
			return K_F3;
			
		case SDLK_F4:
			return K_F4;
			
		case SDLK_F5:
			return K_F5;
			
		case SDLK_F6:
			return K_F6;
			
		case SDLK_F7:
			return K_F7;
			
		case SDLK_F8:
			return K_F8;
			
		case SDLK_F9:
			return K_F9;
			
		case SDLK_F10:
			return K_F10;
			
		case SDLK_F11:
			return K_F11;
			
		case SDLK_F12:
			return K_F12;
		// K_INVERTED_EXCLAMATION;
		
		case SDLK_F13:
			return K_F13;
			
		case SDLK_F14:
			return K_F14;
			
		case SDLK_F15:
			return K_F15;
			
		case SDLK_KP_7:
			return K_KP_7;
			
		case SDLK_KP_8:
			return K_KP_8;
			
		case SDLK_KP_9:
			return K_KP_9;
			
		case SDLK_KP_4:
			return K_KP_4;
			
		case SDLK_KP_5:
			return K_KP_5;
			
		case SDLK_KP_6:
			return K_KP_6;
			
		case SDLK_KP_1:
			return K_KP_1;
			
		case SDLK_KP_2:
			return K_KP_2;
			
		case SDLK_KP_3:
			return K_KP_3;
			
		case SDLK_KP_ENTER:
			return K_KP_ENTER;
			
		case SDLK_KP_0:
			return K_KP_0;
			
		case SDLK_KP_PERIOD:
			return K_KP_DOT;
			
		case SDLK_KP_DIVIDE:
			return K_KP_SLASH;
		// K_SUPERSCRIPT_TWO;
		
		case SDLK_KP_MINUS:
			return K_KP_MINUS;
		// K_ACUTE_ACCENT;
		
		case SDLK_KP_PLUS:
			return K_KP_PLUS;
			
		case SDLK_NUMLOCKCLEAR:
			return K_NUMLOCK;
			
		case SDLK_KP_MULTIPLY:
			return K_KP_STAR;
			
		case SDLK_KP_EQUALS:
			return K_KP_EQUALS;
			
		// K_MASCULINE_ORDINATOR;
		// K_GRAVE_A;
		// K_AUX1;
		// K_CEDILLA_C;
		// K_GRAVE_E;
		// K_AUX2;
		// K_AUX3;
		// K_AUX4;
		// K_GRAVE_I;
		// K_AUX5;
		// K_AUX6;
		// K_AUX7;
		// K_AUX8;
		// K_TILDE_N;
		// K_GRAVE_O;
		// K_AUX9;
		// K_AUX10;
		// K_AUX11;
		// K_AUX12;
		// K_AUX13;
		// K_AUX14;
		// K_GRAVE_U;
		// K_AUX15;
		// K_AUX16;
		
		case SDLK_PRINTSCREEN:
			return K_PRINTSCREEN;
			
		case SDLK_MODE:
			return K_RALT;
	}
	
	return 0;
}
#endif // SDL2

static void PushConsoleEvent( const char* s )
{
	char* b;
	size_t len;
	
	len = strlen( s ) + 1;
	b = ( char* )Mem_Alloc( len, TAG_EVENTS );
	strcpy( b, s );
	
	SDL_Event event;
	
	event.type = SDL_USEREVENT;
	event.user.code = SE_CONSOLE;
	event.user.data1 = ( void* )len;
	event.user.data2 = b;
	
	SDL_PushEvent( &event );
}



/*
=================
Sys_InitInput
=================
*/
void Sys_InitInput()
{
	int numJoysticks, i;
	
	kbd_polls.SetGranularity( 256 );
	mouse_polls.SetGranularity( 256 );
	
	for (int i = 0; i < MAX_JOYSTICKS; i++) {
		memset( &buttonStates[i], 0, sizeof( buttonStates[i] ) );
		memset( &joyAxis[i], 0, sizeof( joyAxis[i] ) );
	}
	memset( &current, 0, sizeof(joyState) );
	memset( &old, 0, sizeof(joyState) );
	memset( &joystick_polls, 0, sizeof(joystick_polls) );
	
#if !SDL_VERSION_ATLEAST(2, 0, 0)
	SDL_EnableUNICODE( 1 );
	SDL_EnableKeyRepeat( SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL );
#endif
	in_keyboard.SetModified();
	//GK: Insted of initializing only once the joystick run a thread that will allow the dynamic connection/disconnection of it
	#if SDL_VERSION_ATLEAST(2, 0, 0)
	joyThread = SDL_CreateThread((SDL_ThreadFunction)JoystickSamplingThread,"Joystic",NULL);
	#else
	joyThread = SDL_CreateThread(JoystickSamplingThread,NULL);
	#endif
	//GK:End
	while(eventHead - eventTail < MAX_QUED_EVENTS) {
		Sys_GenerateEvents();
	}
	Sys_ClearEvents();
}

/*
=================
Sys_ShutdownInput
=================
*/
void Sys_ShutdownInput()
{
	kbd_polls.Clear();
	mouse_polls.Clear();
	joyThreadKill = true;
	SDL_WaitThread(joyThread, NULL);
	
	for (int i = 0; i < MAX_JOYSTICKS; i++) {
		memset( &buttonStates[i], 0, sizeof( buttonStates[i] ) );
		memset( &joyAxis[i], 0, sizeof( joyAxis[i] ) );
	}
	memset( &current, 0, sizeof(joyState) );
	memset( &old, 0, sizeof(joyState) );
	memset( &joystick_polls, 0, sizeof(joystick_polls) );
}

/*
===========
Sys_InitScanTable
===========
*/
// Windows has its own version due to the tools
#ifndef _WIN32
void Sys_InitScanTable()
{
}
#endif

/*
===============
Sys_GetConsoleKey
===============
*/
unsigned char Sys_GetConsoleKey( bool shifted )
{
	static unsigned char keys[2] = { '`', '~' };
	
	if( in_keyboard.IsModified() )
	{
		idStr lang = in_keyboard.GetString();
		
		if( lang.Length() )
		{
			if( !lang.Icmp( "french" ) )
			{
				keys[0] = '<';
				keys[1] = '>';
			}
			else if( !lang.Icmp( "german" ) )
			{
				keys[0] = '^';
				keys[1] = 176; // °
			}
			else if( !lang.Icmp( "italian" ) )
			{
				keys[0] = '\\';
				keys[1] = '|';
			}
			else if( !lang.Icmp( "spanish" ) )
			{
				keys[0] = 186; // º
				keys[1] = 170; // ª
			}
			else if( !lang.Icmp( "turkish" ) )
			{
				keys[0] = '"';
				keys[1] = 233; // é
			}
			else if( !lang.Icmp( "norwegian" ) )
			{
				keys[0] = 124; // |
				keys[1] = 167; // §
			}
		}
		
		in_keyboard.ClearModified();
	}
	
	return shifted ? keys[1] : keys[0];
}

/*
===============
Sys_MapCharForKey
===============
*/
unsigned char Sys_MapCharForKey( int key )
{
	return key & 0xff;
}

/*
===============
Sys_GrabMouseCursor
===============
*/
void Sys_GrabMouseCursor( bool grabIt )
{
	int flags;
	
	if( grabIt )
	{
		// DG: disabling the cursor is now done once in GLimp_Init() because it should always be disabled
		flags = GRAB_ENABLE | GRAB_SETSTATE;
		// DG end
	}
	else
	{
		flags = GRAB_SETSTATE;
	}
	
	GLimp_GrabInput( flags );
}

/*
================
Sys_QueEvent

Ptr should either be null, or point to a block of data that can
be freed by the game later.
================
*/
void Sys_QueEvent( sysEventType_t type, int value, int value2, int ptrLength, void *ptr, int inputDeviceNum ) {
	sysEvent_t * ev = &eventQue[ eventHead & MASK_QUED_EVENTS ];

	if ( eventHead - eventTail >= MAX_QUED_EVENTS ) {
		common->Printf("Sys_QueEvent: overflow\n");
		// we are discarding an event, but don't leak memory
		if ( ev->evPtr ) {
			Mem_Free( ev->evPtr );
		}
		eventTail++;
	}

	eventHead++;

	ev->evType = type;
	ev->evValue = value;
	ev->evValue2 = value2;
	ev->evPtrLength = ptrLength;
	ev->evPtr = ptr;
	ev->inputDevice = inputDeviceNum;
}

#if SDL_VERSION_ATLEAST(2, 0, 0)
// utf-32 version of the textinput event
static int32 uniStr[SDL_TEXTINPUTEVENT_TEXT_SIZE] = {0};
static size_t uniStrPos = 0;
// DG: fake a "mousewheel not pressed anymore" event for SDL2
// so scrolling in menus stops after one step
static int mwheelRel = 0;
#endif
static int32 uniChar = 0;

// void PushJoyButton( int key, bool value )
// {
// 	// So we don't keep sending the same SE_KEY message over and over again
// 	if( buttonStates[key] != value )
// 	{
// 		buttonStates[key] = value;
// 		Sys_QueEvent( SE_KEY, key, value, 0, NULL, 0 );
// 	}
// }

void SDL_Poll()
{
	sysEvent_t res = { };
	
	SDL_Event ev;
	int key;
	// int range = 16384;
	// int axis = 0;
	// int percent;
	sys_jEvents joyEvent;
	
	// when this is returned, it's assumed that there are no more events!
	static const sysEvent_t no_more_events = { SE_NONE, 0, 0, 0, NULL };
	
	// WM0110: previous state of joystick hat
	static int previous_hat_state = SDL_HAT_CENTERED;

#if SDL_VERSION_ATLEAST(2, 0, 0)
	if( uniStr[0] != 0 )
	{
		Sys_QueEvent(SE_CHAR, uniStr[uniStrPos], 1, 0, NULL, 0);
		
		++uniStrPos;
		
		if( !uniStr[uniStrPos] || uniStrPos == SDL_TEXTINPUTEVENT_TEXT_SIZE )
		{
			memset( uniStr, 0, sizeof( uniStr ) );
			uniStrPos = 0;
		}
	}
#endif
	if( uniChar )
	{
		Sys_QueEvent(SE_CHAR, uniChar, 1, 0, NULL, 0);
		
		uniChar = 0;
	}

#if SDL_VERSION_ATLEAST(2, 0, 0)
	if( mwheelRel )
	{
		Sys_QueEvent(SE_KEY, mwheelRel, 0, 0, NULL, 0);
		mwheelRel = 0;
	}
#endif

	if (console->Active()) {
		if (!SDL_IsTextInputActive()) {
			SDL_StartTextInput();
		}
	}
	else {
		if (SDL_IsTextInputActive()) {
			SDL_StopTextInput();
		}
	}
	// loop until there is an event we care about (will return then) or no more events
	while ( SDL_PollEvent( &ev ) )
	{
		switch (ev.type)
		{
#if SDL_VERSION_ATLEAST(2, 0, 0)
		case SDL_WINDOWEVENT:
			switch (ev.window.event)
			{
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			{
				// unset modifier, in case alt-tab was used to leave window and ALT is still set
				// as that can cause fullscreen-toggling when pressing enter...
				SDL_Keymod currentmod = SDL_GetModState();
				int newmod = KMOD_NONE;
				if (currentmod & KMOD_CAPS) // preserve capslock
					newmod |= KMOD_CAPS;

				SDL_SetModState((SDL_Keymod)newmod);

				// DG: un-pause the game when focus is gained, that also re-grabs the input
				//     disabling the cursor is now done once in GLimp_Init() because it should always be disabled
				soundSystem->SetMute(false);
				cvarSystem->SetCVarBool("com_pausePlatform", false);
				cvarSystem->SetCVarBool("com_pause", false);
				// DG end
				break;
			}

			case SDL_WINDOWEVENT_FOCUS_LOST:
				SDL_MinimizeWindow(window);
				// DG: pause the game when focus is lost, that also un-grabs the input
				soundSystem->SetMute(true);
				cvarSystem->SetCVarBool("com_pausePlatform", true);
				cvarSystem->SetCVarBool("com_pause", true);
				// DG end
				break;

			case SDL_WINDOWEVENT_LEAVE:
				// mouse has left the window
				Sys_QueEvent(SE_MOUSE_LEAVE, 0, 0, 0, NULL, 0);

				// DG: handle resizing and moving of window
			case SDL_WINDOWEVENT_RESIZED:
			{
				int w = ev.window.data1;
				int h = ev.window.data2;
				r_windowWidth.SetInteger(w);
				r_windowHeight.SetInteger(h);

				glConfig.nativeScreenWidth = w;
				glConfig.nativeScreenHeight = h;
				break;
			}

			case SDL_WINDOWEVENT_MOVED:
			{
				int x = ev.window.data1;
				int y = ev.window.data2;
				r_windowX.SetInteger(x);
				r_windowY.SetInteger(y);
				break;
			}
			case SDL_WINDOWEVENT_CLOSE:
			{
				com_emergencyexit.SetBool(true);
				soundSystem->SetMute(true);
				cmdSystem->BufferCommandText(CMD_EXEC_APPEND, "quit\n");
				break;
			}
			}

			continue; // handle next event
#else // SDL 1.2
		case SDL_ACTIVEEVENT:
		{
			// DG: (un-)pause the game when focus is gained, that also (un-)grabs the input
			bool pause = true;

			if (ev.active.gain)
			{

				pause = false;

				// unset modifier, in case alt-tab was used to leave window and ALT is still set
				// as that can cause fullscreen-toggling when pressing enter...
				SDLMod currentmod = SDL_GetModState();
				int newmod = KMOD_NONE;
				if (currentmod & KMOD_CAPS) // preserve capslock
					newmod |= KMOD_CAPS;

				SDL_SetModState((SDLMod)newmod);
			}

			cvarSystem->SetCVarBool("com_pause", pause);

			if (ev.active.state == SDL_APPMOUSEFOCUS && !ev.active.gain)
			{
				// the mouse has left the window.
				Sys_QueEvent(SE_MOUSE_LEAVE, 0, 0, 0, NULL, 0);
			}

		}

		continue; // handle next event

		case SDL_VIDEOEXPOSE:
			continue; // handle next event

		// DG: handle resizing and moving of window
		case SDL_VIDEORESIZE:
		{
			int w = ev.resize.w;
			int h = ev.resize.h;
			r_windowWidth.SetInteger(w);
			r_windowHeight.SetInteger(h);

			glConfig.nativeScreenWidth = w;
			glConfig.nativeScreenHeight = h;

			// for some reason this needs a vid_restart in SDL1 but not SDL2 so GLimp_SetScreenParms() is called
			cmdSystem->BufferCommandText(CMD_EXEC_APPEND, "vid_restart\n");
			continue; // handle next event
		}
		// DG end
#endif // SDL1.2

		case SDL_KEYDOWN:
			if (ev.key.keysym.sym == SDLK_RETURN && (ev.key.keysym.mod & KMOD_ALT) > 0)
			{
				// DG: go to fullscreen on current display, instead of always first display
				int fullscreen = 0;
				if (!renderSystem->IsFullScreen())
				{
					// this will be handled as "fullscreen on current window"
					// r_fullscreen 1 means "fullscreen on first window" in d3 bfg
					SDL_Rect* windowRect = new SDL_Rect();
					windowRect->x = r_windowX.GetInteger();
					windowRect->y = r_windowY.GetInteger();
					windowRect->w = r_windowWidth.GetInteger();
					windowRect->h = r_windowHeight.GetInteger();
					fullscreen = SDL_GetRectDisplayIndex(windowRect) + 1;
					delete(windowRect);
					if (fullscreen <= 0) {
						common->Printf("SDL2: Failed to detect Display index from Window with error message: %s\n", SDL_GetError());
					}
					
				}
				cvarSystem->SetCVarInteger("r_fullscreen", fullscreen);
				// DG end
				cmdSystem->BufferCommandText(CMD_EXEC_APPEND, "vid_restart\n");
				continue; // handle next event
			}

			// DG: ctrl-g to un-grab mouse - yeah, left ctrl shoots, then just use right ctrl :)
			if (ev.key.keysym.sym == SDLK_g && (ev.key.keysym.mod & KMOD_CTRL) > 0)
			{
				bool grab = cvarSystem->GetCVarBool("in_nograb");
				grab = !grab;
				cvarSystem->SetCVarBool("in_nograb", grab);
				continue; // handle next event
			}
			// DG end

#if ! SDL_VERSION_ATLEAST(2, 0, 0)
				// DG: only do this for key-down, don't care about isChar from SDL_KeyToDoom3Key.
				//     if unicode is not 0  it should work..
			if (ev.key.state == SDL_PRESSED)
			{
				uniChar = ev.key.keysym.sym; // for SE_CHAR
			}
			// DG end
#endif // SDL 1.2

			// fall through
		case SDL_KEYUP:
		{
			bool isChar;

			// DG: special case for SDL_SCANCODE_GRAVE - the console key under Esc
			if (ev.key.keysym.scancode == SDL_SCANCODE_GRAVE)
			{
				key = K_GRAVE;
				uniChar = K_BACKSPACE; // bad hack to get empty console inputline..
			} // DG end, the original code is in the else case
			else
			{
#if SDL_VERSION_ATLEAST(2, 0, 0)
				key = SDLScanCodeToKeyNum(ev.key.keysym.scancode);

				if (key == 0)
				{
					// SDL2 has no ev.key.keysym.unicode anymore.. but the scancode should work well enough for console
					if (ev.type == SDL_KEYDOWN) // FIXME: don't complain if this was an ASCII char and the console is open?
						common->Warning("unmapped SDL key %d scancode %d", ev.key.keysym.sym, ev.key.keysym.scancode);

					continue; // just handle next event
				}
#else // SDL1.2
				key = SDL_KeyToDoom3Key(ev.key.keysym.sym, isChar);

				if (key == 0)
				{
					unsigned char uc = ev.key.keysym.unicode & 0xff;
					// check if its an unmapped console key
					if (uc == Sys_GetConsoleKey(false) || uc == Sys_GetConsoleKey(true))
					{
						key = K_GRAVE;
						uniChar = K_BACKSPACE; // bad hack to get empty console inputline..
					}
					else
					{
						if (uniChar)
						{
							res.evType = SE_CHAR;
							res.evValue = uniChar;

							uniChar = 0;

							return res;
						}

						if (ev.type == SDL_KEYDOWN) // FIXME: don't complain if this was an ASCII char and the console is open?
							common->Warning("unmapped SDL key %d (0x%x) scancode %d", ev.key.keysym.sym, ev.key.keysym.unicode, ev.key.keysym.scancode);

						continue; // just handle next event
					}
				}
#endif // SDL 1.2
			}

			//res.evType = SE_KEY;
			//res.evValue = key;
			//res.evValue2 = ev.key.state == SDL_PRESSED ? 1 : 0;

			kbd_polls.Append(kbd_poll_t(key, ev.key.state == SDL_PRESSED));

			if (key == K_BACKSPACE && ev.key.state == SDL_PRESSED)
				uniChar = key;

			Sys_QueEvent(SE_KEY, key, (ev.key.state == SDL_PRESSED ? 1 : 0), 0, NULL, 0);
			//return res;
		}
		break;
#if SDL_VERSION_ATLEAST(2, 0, 0)
		case SDL_TEXTINPUT:
			if (ev.text.text[0] != '\0')
			{
				// fill uniStr array for SE_CHAR events
				ConvertUTF8toUTF32(ev.text.text, uniStr);

				// return an event with the first/only char
				//res.evType = SE_CHAR;
				//res.evValue = uniStr[0];
				Sys_QueEvent(SE_CHAR, uniStr[0], 1, 0, NULL, 0);
				uniStrPos = 1;

				if (uniStr[1] == 0)
				{
					// it's just this one character, clear uniStr
					uniStr[0] = 0;
					uniStrPos = 0;
				}
				
				//return res;
				break;
			}

			continue; // just handle next event
#endif // SDL2

		case SDL_MOUSEMOTION:
			// DG: return event with absolute mouse-coordinates when in menu
			// to fix cursor problems in windowed mode
			if (game && game->Shell_IsActive())
			{
				// res.evType = SE_MOUSE_ABSOLUTE;
				// res.evValue = ev.motion.x;
				// res.evValue2 = ev.motion.y;
				Sys_QueEvent(SE_MOUSE_ABSOLUTE, ev.motion.x, ev.motion.y, 0, NULL, 0);
			}
			else     // this is the old, default behavior
			{
				// res.evType = SE_MOUSE;
				// res.evValue = ev.motion.xrel;
				// res.evValue2 = ev.motion.yrel;
				Sys_QueEvent(SE_MOUSE, ev.motion.xrel, ev.motion.yrel, 0, NULL, 0);
			}
			// DG end

			mouse_polls.Append(mouse_poll_t(M_DELTAX, ev.motion.xrel));
			mouse_polls.Append(mouse_poll_t(M_DELTAY, ev.motion.yrel));

			//return res;
			break;
#if SDL_VERSION_ATLEAST(2, 0, 0)
		case SDL_FINGERDOWN:
		case SDL_FINGERUP:
		case SDL_FINGERMOTION:
			continue; // Avoid 'unknown event' spam when testing with touchpad by skipping this

		case SDL_MOUSEWHEEL:
			//res.evType = SE_KEY;

			//res.evValue = (ev.wheel.y > 0) ? K_MWHEELUP : K_MWHEELDOWN;
			mouse_polls.Append(mouse_poll_t(M_DELTAZ, ev.wheel.y));

			//res.evValue2 = 1; // for "pressed"
			Sys_QueEvent(SE_KEY, (ev.wheel.y > 0) ? K_MWHEELUP : K_MWHEELDOWN, 1, 0, NULL, 0);

			// remember mousewheel direction to issue a "not pressed anymore" event
			mwheelRel = (ev.wheel.y > 0) ? K_MWHEELUP : K_MWHEELDOWN;

			//return res;
			break;
#endif // SDL2

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			res.evType = SE_KEY;

			switch (ev.button.button)
			{
			case SDL_BUTTON_LEFT:
				res.evValue = K_MOUSE1;
				mouse_polls.Append(mouse_poll_t(M_ACTION1, ev.button.state == SDL_PRESSED ? 1 : 0));
				break;
			case SDL_BUTTON_MIDDLE:
				res.evValue = K_MOUSE3;
				mouse_polls.Append(mouse_poll_t(M_ACTION3, ev.button.state == SDL_PRESSED ? 1 : 0));
				break;
			case SDL_BUTTON_RIGHT:
				res.evValue = K_MOUSE2;
				mouse_polls.Append(mouse_poll_t(M_ACTION2, ev.button.state == SDL_PRESSED ? 1 : 0));
				break;

#if !SDL_VERSION_ATLEAST(2, 0, 0)
			case SDL_BUTTON_WHEELUP:
				res.evValue = K_MWHEELUP;
				if (ev.button.state == SDL_PRESSED)
					mouse_polls.Append(mouse_poll_t(M_DELTAZ, 1));
				break;
			case SDL_BUTTON_WHEELDOWN:
				res.evValue = K_MWHEELDOWN;
				if (ev.button.state == SDL_PRESSED)
					mouse_polls.Append(mouse_poll_t(M_DELTAZ, -1));
				break;
#endif // SDL1.2

			default:
				// handle X1 button and above
				if (ev.button.button <= 16) // d3bfg doesn't support more than 16 mouse buttons
				{
					int buttonIndex = ev.button.button - SDL_BUTTON_LEFT;
					res.evValue = K_MOUSE1 + buttonIndex;
					mouse_polls.Append(mouse_poll_t(M_ACTION1 + buttonIndex, ev.button.state == SDL_PRESSED ? 1 : 0));
				}
				else // unsupported mouse button
				{
					continue; // just ignore
				}
			}

			res.evValue2 = ev.button.state == SDL_PRESSED ? 1 : 0;

			Sys_QueEvent(res.evType, res.evValue, res.evValue2, 0, NULL, 0);
			break;
#if SDL_VERSION_ATLEAST(2, 0, 0)
			// GameController
		case SDL_JOYAXISMOTION:
		case SDL_JOYHATMOTION:
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
		case SDL_JOYDEVICEADDED:
		case SDL_JOYDEVICEREMOVED:
			// Avoid 'unknown event' spam
			continue;
		case SDL_CONTROLLERAXISMOTION:
			joyEvent = (sys_jEvents)(J_AXIS_LEFT_X + ev.caxis.axis);
			switch(joyEvent) {
				case J_AXIS_LEFT_X:
					current[ev.caxis.which].LXThumb = ev.caxis.value;
					break;
				case J_AXIS_LEFT_Y:
					current[ev.caxis.which].LYThumb = ev.caxis.value;
					break;
				case J_AXIS_RIGHT_X:
					current[ev.caxis.which].RXThumb = ev.caxis.value;
					break;
				case J_AXIS_RIGHT_Y:
					current[ev.caxis.which].RYThumb = ev.caxis.value;
					break;
				case J_AXIS_LEFT_TRIG:
					current[ev.caxis.which].LTrigger = ev.caxis.value;
					break;
				case J_AXIS_RIGHT_TRIG:
					current[ev.caxis.which].RTrigger = ev.caxis.value;
					break;

			}
			break;
		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP:
			current[ev.cbutton.which].buttons[ev.cbutton.button] = (ev.cbutton.state == SDL_PRESSED ? 1 : 0);
			break;
		//GK: Steam Deck Hack: For some reason Steam Deck spams these two events
		case SDL_CONTROLLERDEVICEADDED:
		case SDL_CONTROLLERDEVICEREMAPPED:
			continue;
#else
			// WM0110
			// NOTE: it seems that the key bindings for the GUI and for the game are
			// totally independant. I think the event returned by this function seems to work
			// on the GUI and the event returned by Sys_ReturnJoystickInputEvent() works on
			// the game.
			// Also, remember that joystick keys must be binded to actions in order to work!
			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP:
				// sys_public.h: evValue is an axis number and evValue2 is the current state (-127 to 127)
				// WM0110: joystick ranges must be between (-32769, 32768)!
				res.evType = SE_KEY;
				switch( ev.jbutton.button )
				{
					case 0:
						res.evValue = K_JOY1;
						joystick_polls.Append( joystick_poll_t( J_ACTION1, ev.jbutton.state == SDL_PRESSED ? 1 : 0 ) );
						break;
				
					case 1:
						res.evValue = K_JOY2;
						joystick_polls.Append( joystick_poll_t( J_ACTION2, ev.jbutton.state == SDL_PRESSED ? 1 : 0 ) );
						break;
				
					case 2:
						res.evValue = K_JOY3;
						joystick_polls.Append( joystick_poll_t( J_ACTION3, ev.jbutton.state == SDL_PRESSED ? 1 : 0 ) );
						break;
				
					case 3:
						res.evValue = K_JOY4;
						joystick_polls.Append( joystick_poll_t( J_ACTION4, ev.jbutton.state == SDL_PRESSED ? 1 : 0 ) );
						break;
				
					case 4:
						res.evValue = K_JOY5;
						joystick_polls.Append( joystick_poll_t( J_ACTION5, ev.jbutton.state == SDL_PRESSED ? 1 : 0 ) );
						break;
				
					case 5:
						res.evValue = K_JOY6;
						joystick_polls.Append( joystick_poll_t( J_ACTION6, ev.jbutton.state == SDL_PRESSED ? 1 : 0 ) );
						break;
				
					case 6:
						res.evValue = K_JOY7;
						joystick_polls.Append( joystick_poll_t( J_ACTION7, ev.jbutton.state == SDL_PRESSED ? 1 : 0 ) );
						break;
				
					case 7:
						res.evValue = K_JOY8;
						joystick_polls.Append( joystick_poll_t( J_ACTION8, ev.jbutton.state == SDL_PRESSED ? 1 : 0 ) );
						break;
				
					case 8:
						res.evValue = K_JOY9;
						joystick_polls.Append( joystick_poll_t( J_ACTION9, ev.jbutton.state == SDL_PRESSED ? 1 : 0 ) );
						break;
				
					case 9:
						res.evValue = K_JOY10;
						joystick_polls.Append( joystick_poll_t( J_ACTION10, ev.jbutton.state == SDL_PRESSED ? 1 : 0 ) );
						break;
				
					case 10:
						res.evValue = K_JOY11;
						joystick_polls.Append( joystick_poll_t( J_ACTION11, ev.jbutton.state == SDL_PRESSED ? 1 : 0 ) );
						break;
				
					// D-PAD left (XBox 360 wireless)
					case 11:
						// If joystick has a hat, then use the hat as D-PAD. If not, D-PAD is mapped
						// to buttons.
						if( SDL_joystick_has_hat )
						{
							res.evValue = K_JOY12;
							joystick_polls.Append( joystick_poll_t( J_ACTION12, ev.jbutton.state == SDL_PRESSED ? 1 : 0 ) );
						}
						else
						{
							res.evValue = K_JOY_DPAD_LEFT;
							joystick_polls.Append( joystick_poll_t( J_DPAD_LEFT, ev.jbutton.state == SDL_PRESSED ? 1 : 0 ) );
						}
						break;
				
					// D-PAD right
					case 12:
						if( SDL_joystick_has_hat )
						{
							res.evValue = K_JOY13;
							joystick_polls.Append( joystick_poll_t( J_ACTION13, ev.jbutton.state == SDL_PRESSED ? 1 : 0 ) );
						}
						else
						{
							res.evValue = K_JOY_DPAD_RIGHT;
							joystick_polls.Append( joystick_poll_t( J_DPAD_RIGHT, ev.jbutton.state == SDL_PRESSED ? 1 : 0 ) );
						}
						break;
				
					// D-PAD up
					case 13:
						if( SDL_joystick_has_hat )
						{
							res.evValue = K_JOY14;
							joystick_polls.Append( joystick_poll_t( J_ACTION14, ev.jbutton.state == SDL_PRESSED ? 1 : 0 ) );
						}
						else
						{
							res.evValue = K_JOY_DPAD_UP;
							joystick_polls.Append( joystick_poll_t( J_DPAD_UP, ev.jbutton.state == SDL_PRESSED ? 1 : 0 ) );
						}
						break;
				
					// D-PAD down
					case 14:
						if( SDL_joystick_has_hat )
						{
							res.evValue = K_JOY15;
							joystick_polls.Append( joystick_poll_t( J_ACTION15, ev.jbutton.state == SDL_PRESSED ? 1 : 0 ) );
						}
						else
						{
							res.evValue = K_JOY_DPAD_DOWN;
							joystick_polls.Append( joystick_poll_t( J_DPAD_DOWN, ev.jbutton.state == SDL_PRESSED ? 1 : 0 ) );
						}
						break;
				
					default:
						common->Warning( "Sys_GetEvent(): Unknown joystick button number %i\n", ev.jbutton.button );
						continue; // just try next event
				}
				res.evValue2 = ev.jbutton.state == SDL_PRESSED ? 1 : 0;
				
				return res;
				
			case SDL_JOYHATMOTION:
				// If this is not the first hat, ignore this event.
				if( ev.jhat.which != 0 )
					continue; // just try next event
				
				res.evType = SE_KEY;
				if( ev.jhat.value & SDL_HAT_UP )
				{
					res.evValue = K_JOY_DPAD_UP;
					joystick_polls.Append( joystick_poll_t( J_DPAD_UP, 1 ) );
					res.evValue2 = 1;
					previous_hat_state = J_DPAD_UP;
				}
				else if( ev.jhat.value & SDL_HAT_DOWN )
				{
					res.evValue = K_JOY_DPAD_DOWN;
					joystick_polls.Append( joystick_poll_t( J_DPAD_DOWN, 1 ) );
					res.evValue2 = 1;
					previous_hat_state = J_DPAD_DOWN;
				}
				else if( ev.jhat.value & SDL_HAT_RIGHT )
				{
					res.evValue = K_JOY_DPAD_RIGHT;
					joystick_polls.Append( joystick_poll_t( J_DPAD_RIGHT, 1 ) );
					res.evValue2 = 1;
					previous_hat_state = J_DPAD_RIGHT;
				}
				else if( ev.jhat.value & SDL_HAT_LEFT )
				{
					res.evValue = K_JOY_DPAD_LEFT;
					joystick_polls.Append( joystick_poll_t( J_DPAD_LEFT, 1 ) );
					res.evValue2 = 1;
					previous_hat_state = J_DPAD_LEFT;
				}
				// SDL_HAT_CENTERED is defined as 0
				else if( ev.jhat.value == SDL_HAT_CENTERED )
				{
					// We need to know the previous state to know which event to send.
					if( previous_hat_state == J_DPAD_UP )
					{
						res.evValue = K_JOY_DPAD_UP;
						joystick_polls.Append( joystick_poll_t( J_DPAD_UP, 0 ) );
						res.evValue2 = 0;
					}
					else if( previous_hat_state == J_DPAD_DOWN )
					{
						res.evValue = K_JOY_DPAD_DOWN;
						joystick_polls.Append( joystick_poll_t( J_DPAD_DOWN, 0 ) );
						res.evValue2 = 0;
					}
					else if( previous_hat_state == J_DPAD_RIGHT )
					{
						res.evValue = K_JOY_DPAD_RIGHT;
						joystick_polls.Append( joystick_poll_t( J_DPAD_RIGHT, 0 ) );
						res.evValue2 = 0;
					}
					else if( previous_hat_state == J_DPAD_LEFT )
					{
						res.evValue = K_JOY_DPAD_LEFT;
						joystick_polls.Append( joystick_poll_t( J_DPAD_LEFT, 0 ) );
						res.evValue2 = 0;
					}
					else if( previous_hat_state == SDL_HAT_CENTERED )
					{
						common->Warning( "Sys_GetEvent(): SDL_JOYHATMOTION: previous state SDL_HAT_CENTERED repeated!\n" );
						continue; // just try next event
					}
					else
					{
						common->Warning( "Sys_GetEvent(): SDL_JOYHATMOTION: unknown previous hat state %i\n", previous_hat_state );
						continue; // just try next event
					}
				
					previous_hat_state = SDL_HAT_CENTERED;
				}
				else
				{
					common->Warning( "Sys_GetEvent(): Unknown SDL_JOYHATMOTION value %i\n", ev.jhat.value );
					continue; // just try next event
				}
				
				return res;
				
			case SDL_JOYAXISMOTION:
				res.evType = SE_JOYSTICK;
				// SDL joystick range is: -32768 to 32767, which is what is expected
				// in void idUsercmdGenLocal::Joystick( int deviceNum ).
				switch( ev.jaxis.axis )
				{
						//const int range = 16384;
						int triggerValue;
						bool triggered;
						int percent;
						int axis;
				
					// LEFT trigger
					case 2:
						// Convert TRIGGER value from space (-32768, 32767) to (0, 32767)
						triggerValue = ( ev.jaxis.value + 32768 ) / 2;
						// common->Printf("Sys_GetEvent: LEFT trigger value = %i / converted value = %i\n", ev.jaxis.value, trigger_value);
						res.evValue = J_AXIS_LEFT_TRIG;
				
						joystick_polls.Append( joystick_poll_t( J_AXIS_LEFT_TRIG, triggerValue ) );
						break;
				
					// Right trigger
					case 5:
						triggerValue = ( ev.jaxis.value + 32768 ) / 2;
						// common->Printf("Sys_GetEvent: RIGHT trigger value = %i / converted value = %i\n", ev.jaxis.value, trigger_value);
						res.evValue = J_AXIS_RIGHT_TRIG;
				
						joystick_polls.Append( joystick_poll_t( J_AXIS_RIGHT_TRIG, triggerValue ) );
						break;
				
					// LEFT X
					case 0:
						res.evValue = J_AXIS_LEFT_X;
						joystick_polls.Append( joystick_poll_t( J_AXIS_LEFT_X, ev.jaxis.value ) );
				
						triggered = ( ev.jaxis.value > 16384 );
						if( buttonStates[K_JOY_STICK1_RIGHT] != triggered )
						{
							buttonStates[K_JOY_STICK1_RIGHT] = triggered;
				
							res.evType = SE_KEY;
							res.evValue = K_JOY_STICK1_RIGHT;
							res.evValue2 = triggered;
						}
				
						triggered = ( ev.jaxis.value < -16384 );
						if( buttonStates[K_JOY_STICK1_LEFT] != triggered )
						{
							buttonStates[K_JOY_STICK1_LEFT] = triggered;
				
							res.evType = SE_KEY;
							res.evValue = K_JOY_STICK1_LEFT;
							res.evValue2 = triggered;
						}
						break;
				
					// LEFT Y
					case 1:
						res.evValue = J_AXIS_LEFT_Y;
						joystick_polls.Append( joystick_poll_t( J_AXIS_LEFT_Y, ev.jaxis.value ) );
				
						triggered = ( ev.jaxis.value > 16384 );
						if( buttonStates[K_JOY_STICK1_DOWN] != triggered )
						{
							buttonStates[K_JOY_STICK1_DOWN] = triggered;
				
							res.evType = SE_KEY;
							res.evValue = K_JOY_STICK1_DOWN;
							res.evValue2 = triggered;
						}
				
						triggered = ( ev.jaxis.value < -16384 );
						if( buttonStates[K_JOY_STICK1_UP] != triggered )
						{
							buttonStates[K_JOY_STICK1_UP] = triggered;
				
							res.evType = SE_KEY;
							res.evValue = K_JOY_STICK1_UP;
							res.evValue2 = triggered;
						}
				
						break;
				
					// RIGHT X
					case 3:
						res.evValue = J_AXIS_RIGHT_X;
						joystick_polls.Append( joystick_poll_t( J_AXIS_RIGHT_X, ev.jaxis.value ) );
				
						triggered = ( ev.jaxis.value > 16384 );
						if( buttonStates[K_JOY_STICK2_RIGHT] != triggered )
						{
							buttonStates[K_JOY_STICK2_RIGHT] = triggered;
				
							res.evType = SE_KEY;
							res.evValue = K_JOY_STICK2_RIGHT;
							res.evValue2 = triggered;
						}
				
						triggered = ( ev.jaxis.value < -16384 );
						if( buttonStates[K_JOY_STICK2_LEFT] != triggered )
						{
							buttonStates[K_JOY_STICK2_LEFT] = triggered;
				
							res.evType = SE_KEY;
							res.evValue = K_JOY_STICK2_LEFT;
							res.evValue2 = triggered;
						}
						break;
				
					// RIGHT Y
					case 4:
						res.evValue = J_AXIS_RIGHT_Y;
						joystick_polls.Append( joystick_poll_t( J_AXIS_RIGHT_Y, ev.jaxis.value ) );
				
						triggered = ( ev.jaxis.value > 16384 );
						if( buttonStates[K_JOY_STICK2_DOWN] != triggered )
						{
							buttonStates[K_JOY_STICK2_DOWN] = triggered;
				
							res.evType = SE_KEY;
							res.evValue = K_JOY_STICK2_DOWN;
							res.evValue2 = triggered;
						}
				
						triggered = ( ev.jaxis.value < -16384 );
						if( buttonStates[K_JOY_STICK2_UP] != triggered )
						{
							buttonStates[K_JOY_STICK2_UP] = triggered;
				
							res.evType = SE_KEY;
							res.evValue = K_JOY_STICK2_UP;
							res.evValue2 = triggered;
						}
						break;
				
					default:
						common->Warning( "Sys_GetEvent(): Unknown joystick axis number %i\n", ev.jaxis.axis );
						continue; // just try next event
				}
				
				return res;
				// WM0110
#endif
				
			case SDL_QUIT:
				PushConsoleEvent( "quit" );
				Sys_QueEvent(no_more_events.evType, no_more_events.evValue, no_more_events.evValue2, no_more_events.evPtrLength, no_more_events.evPtr, 0); // don't handle next event, just quit.
				break;
			case SDL_USEREVENT:
				switch( ev.user.code )
				{
					case SE_CONSOLE:
						Sys_QueEvent(SE_CONSOLE, 0, 0, ( intptr_t )ev.user.data1, ev.user.data2, 0);
						break;
					default:
						common->Warning( "unknown user event %u", ev.user.code );
				}
				continue; // just handle next event
			default:
				common->Warning( "unknown event %u", ev.type );
				continue; // just handle next event
		}
	}
	Sys_QueEvent(no_more_events.evType, no_more_events.evValue, no_more_events.evValue2, no_more_events.evPtrLength, no_more_events.evPtr, 0);
}

/*
================
Sys_GetEvent
================
*/

sysEvent_t Sys_GetEvent() {
	sysEvent_t	ev;

	// return if we have data
	if ( eventHead > eventTail ) {
		eventTail++;
		return eventQue[ ( eventTail - 1 ) & MASK_QUED_EVENTS ];
	}

	// return the empty event 
	memset( &ev, 0, sizeof( ev ) );

	return ev;
}

/*
================
Sys_ClearEvents
================
*/
void Sys_ClearEvents()
{
	SDL_Event ev;
	eventHead = eventTail = 0;
	
	SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
	// while( SDL_PollEvent( &ev ) )
	// 	;
		
	kbd_polls.SetNum( 0 );
	mouse_polls.SetNum( 0 );
	memset( &joystick_polls, 0, sizeof(joystick_polls) );
}

/*
================
Sys_GenerateEvents
================
*/
void Sys_GenerateEvents()
{
	char* s = Posix_ConsoleInput();
	
	if( s )
		PushConsoleEvent( s );
		
	//SDL_PumpEvents();
	SDL_Poll();
	//SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT); //GK: Nuke the rest (what could possible go wrong?)
}

/*
================
Sys_PollKeyboardInputEvents
================
*/
int Sys_PollKeyboardInputEvents()
{
	return kbd_polls.Num();
}

/*
================
Sys_ReturnKeyboardInputEvent
================
*/
int Sys_ReturnKeyboardInputEvent( const int n, int& key, bool& state )
{
	if( n >= kbd_polls.Num() )
		return 0;

	key = kbd_polls[n].key;
	state = kbd_polls[n].state;
	return 1;
}

/*
================
Sys_EndKeyboardInputEvents
================
*/
void Sys_EndKeyboardInputEvents()
{
	kbd_polls.SetNum( 0 );
}

/*
================
Sys_PollMouseInputEvents
================
*/
int Sys_PollMouseInputEvents( int mouseEvents[MAX_MOUSE_EVENTS][2] )
{
	int numEvents = mouse_polls.Num();
	
	if( numEvents > MAX_MOUSE_EVENTS )
	{
		numEvents = MAX_MOUSE_EVENTS;
	}
	
	for( int i = 0; i < numEvents; i++ )
	{
		const mouse_poll_t& mp = mouse_polls[i];
		
		mouseEvents[i][0] = mp.action;
		mouseEvents[i][1] = mp.value;
	}
	
	mouse_polls.SetNum( 0 );
	return numEvents;
}

const char* Sys_GetKeyName( keyNum_t keynum )
{
	// unfortunately, in SDL1.2 there is no way to get the keycode for a scancode, so this doesn't work there.
	// so this is SDL2-only.
#if SDL_VERSION_ATLEAST(2, 0, 0)
	
	SDL_Scancode scancode = KeyNumToSDLScanCode( ( int )keynum );
	SDL_Keycode keycode = SDL_GetKeyFromScancode( scancode );
	
	const char* ret = SDL_GetKeyName( keycode );
	if( ret != NULL && ret[0] != '\0' )
	{
		return ret;
	}
#endif
	return NULL;
}

char* Sys_GetClipboardData()
{
#if SDL_VERSION_ATLEAST(2, 0, 0)
	char* txt = SDL_GetClipboardText();
	
	if( txt == NULL )
	{
		return NULL;
	}
	else if( txt[0] == '\0' )
	{
		SDL_free( txt );
		return NULL;
	}
	
	char* ret = Mem_CopyString( txt );
	SDL_free( txt );
	return ret;
#else
	return NULL; // SDL1.2 doesn't support clipboard
#endif
}

void Sys_SetClipboardData( const char* string )
{
#if SDL_VERSION_ATLEAST(2, 0, 0)
	SDL_SetClipboardText( string );
#endif
}


//=====================================================================================
//	Joystick Input Handling
//=====================================================================================

void Sys_SetRumble( int device, int low, int hi )
{
	//GK: This is the code for the rumble effect by using SDL_Haptic.
	//It doesn't affect the game's performance (Remember NO SDL_Delay)

	#if SDL_VERSION_ATLEAST(2, 0, 0)
	if(haptic[device]){ //GK: Make sure the rumble code will run ONLY if the SDL_Haptic device is already initialized
	
	SDL_HapticEffect effect;
 	int effect_id;
	
	SDL_memset( &effect, 0, sizeof(SDL_HapticEffect) );
	//GK: SDL2 has support for left-right motor rumble
	effect.type=SDL_HAPTIC_LEFTRIGHT;
	effect.leftright.small_magnitude = low;
	effect.leftright.large_magnitude = hi;
	effect.leftright.length = 1000;
	//GK:Ps3 controller fix
	if(SDL_HapticNumEffectsPlaying(haptic[device]) >= SDL_HapticNumEffects(haptic[device])){
		SDL_HapticDestroyEffect(haptic[device],0);
	}
	//GK:End of fix
	effect_id = SDL_HapticNewEffect( haptic[device], &effect );
	if(effect_id < 0){
		common->Printf("%s\n",SDL_GetError());
		return;
	}

	 if(SDL_HapticRunEffect( haptic[device], effect_id, 1 )<0){
		 common->Printf("%s\n",SDL_GetError());
	 }
	}
	   

	#endif
}

//GK: Direct copy from Windows Implementation
void PushButton( int inputDeviceNum, int key, bool value )
{
	// So we don't keep sending the same SE_KEY message over and over again
	if( buttonStates[inputDeviceNum][key] != value )
	{
		buttonStates[inputDeviceNum][key] = value;
		Sys_QueEvent( SE_KEY, key, value, 0, NULL, inputDeviceNum );
	}
}
//GK: Direct copy from Windows Implementation
void PostInputEvent( int inputDeviceNum, int event, int value, int range = 16384 )
{
	// These events are used for GUI button presses
	if( ( event >= J_ACTION1 ) && ( event <= J_ACTION_MAX ) )
	{
		PushButton( inputDeviceNum, K_JOY1 + ( event - J_ACTION1 ), value != 0 );
	}
	else if( event == J_AXIS_LEFT_X )
	{
		PushButton( inputDeviceNum, K_JOY_STICK1_LEFT, ( value < -range ) );
		PushButton( inputDeviceNum, K_JOY_STICK1_RIGHT, ( value > range ) );
	}
	else if( event == J_AXIS_LEFT_Y )
	{
		PushButton( inputDeviceNum, K_JOY_STICK1_UP, ( value < -range ) );
		PushButton( inputDeviceNum, K_JOY_STICK1_DOWN, ( value > range ) );
	}
	else if( event == J_AXIS_RIGHT_X )
	{
		PushButton( inputDeviceNum, K_JOY_STICK2_LEFT, ( value < -range ) );
		PushButton( inputDeviceNum, K_JOY_STICK2_RIGHT, ( value > range ) );
	}
	else if( event == J_AXIS_RIGHT_Y )
	{
		PushButton( inputDeviceNum, K_JOY_STICK2_UP, ( value < -range ) );
		PushButton( inputDeviceNum, K_JOY_STICK2_DOWN, ( value > range ) );
	}
	else if( ( event >= J_DPAD_UP ) && ( event <= J_DPAD_RIGHT ) )
	{
		PushButton( inputDeviceNum, K_JOY_DPAD_UP + ( event - J_DPAD_UP ), value != 0 );
	}
	else if( event == J_AXIS_LEFT_TRIG )
	{
		PushButton( inputDeviceNum, K_JOY_TRIGGER1, ( value > range ) );
	}
	else if( event == J_AXIS_RIGHT_TRIG )
	{
		PushButton( inputDeviceNum, K_JOY_TRIGGER2, ( value > range ) );
	}
	if( event >= J_AXIS_MIN && event <= J_AXIS_MAX )
	{
		int axis = event - J_AXIS_MIN;
		int percent = ( value * 16 ) / range;
		if( joyAxis[inputDeviceNum][axis] != percent )
		{
			joyAxis[inputDeviceNum][axis] = percent;
			Sys_QueEvent( SE_JOYSTICK, axis, percent, 0, NULL, inputDeviceNum );
		}
	}
	
	// These events are used for actual game input
	joystick_polls[numEvents].action = event;
	joystick_polls[numEvents].value = value;
	numEvents++;
}

int Sys_PollJoystickInputEvents( int deviceNum )
{
	numEvents = 0;
	int controllerButtonRemap[15] =
			{
				{J_ACTION1}, //SDL_CONTROLLER_BUTTON_A
				{J_ACTION2}, //SDL_CONTROLLER_BUTTON_B
				{J_ACTION3}, //SDL_CONTROLLER_BUTTON_X
				{J_ACTION4}, //SDL_CONTROLLER_BUTTON_Y
				{J_ACTION10}, //SDL_CONTROLLER_BUTTON_BACK
				{J_ACTION11}, //SDL_CONTROLLER_BUTTON_GUIDE
				{J_ACTION9}, //SDL_CONTROLLER_BUTTON_START
				{J_ACTION7}, //SDL_CONTROLLER_BUTTON_LEFTSTICK
				{J_ACTION8}, //SDL_CONTROLLER_BUTTON_RIGHTSTICK
				{J_ACTION5}, //SDL_CONTROLLER_BUTTON_LEFTSHOULDER
				{J_ACTION6}, //SDL_CONTROLLER_BUTTON_RIGHTSHOULDER

				{J_DPAD_UP},
				{J_DPAD_DOWN},
				{J_DPAD_LEFT},
				{J_DPAD_RIGHT},
			};

	for (int i = 0; i < 15; i++) {
		if (current[deviceNum].buttons[i] != old[deviceNum].buttons[i]) {
			PostInputEvent(deviceNum, controllerButtonRemap[i], current[deviceNum].buttons[i]);
		}
	}
	if (current[deviceNum].LXThumb != old[deviceNum].LXThumb) {
		PostInputEvent(deviceNum, J_AXIS_LEFT_X, current[deviceNum].LXThumb);
	}
	if (current[deviceNum].LYThumb != old[deviceNum].LYThumb) {
		PostInputEvent(deviceNum, J_AXIS_LEFT_Y, current[deviceNum].LYThumb);
	}
	if (current[deviceNum].RXThumb != old[deviceNum].RXThumb) {
		PostInputEvent(deviceNum, J_AXIS_RIGHT_X, current[deviceNum].RXThumb);
	}
	if (current[deviceNum].RYThumb != old[deviceNum].RYThumb) {
		PostInputEvent(deviceNum, J_AXIS_RIGHT_Y, current[deviceNum].RYThumb);
	}
	if (current[deviceNum].LTrigger != old[deviceNum].LTrigger) {
		PostInputEvent(deviceNum, J_AXIS_LEFT_TRIG, current[deviceNum].LTrigger);
	}
	if (current[deviceNum].RTrigger != old[deviceNum].RTrigger) {
		PostInputEvent(deviceNum, J_AXIS_RIGHT_TRIG, current[deviceNum].RTrigger);
	}

	old[deviceNum] = current[deviceNum];


	return numEvents;
}

// This funcion called by void idUsercmdGenLocal::Joystick( int deviceNum ) in
// file UsercmdGen.cpp
// action - must have values belonging to enum sys_jEvents (sys_public.h)
// value - must be 1/0 for button or DPAD pressed/released
//         for joystick axes must be in the range (-32769, 32768)
//         for joystick trigger must be in the range (0, 32768)
int Sys_ReturnJoystickInputEvent( const int n, int& action, int& value )
{
	// Get last element of the list and copy into argument references
	if( ( n < 0 ) || ( n >= MAX_JOY_EVENT ) )
	{
		return 0;
	}

	const joystick_poll_t& mp = joystick_polls[n];
	action = mp.action;
	value = mp.value;
	
	return 1;
}

// This funcion called by void idUsercmdGenLocal::Joystick( int deviceNum ) in
// file UsercmdGen.cpp
void Sys_EndJoystickInputEvents()
{
	// Empty the joystick event container. This is called after
	// all joystick events have been read using Sys_ReturnJoystickInputEvent()
	//joystick_polls.Clear();
}

bool Sys_hasConnectedController() {
	bool hasConnected = false;
#if SDL_VERSION_ATLEAST(2, 0, 0)
	for (int i = 0; i < MAX_JOYSTICKS; i++) {
		if (gcontroller[i]) {
			hasConnected = true;
			break;
		}
	}
#else
	hasConnected = joy != NULL;
#endif
	return hasConnected;
}

//GK: This is the controller state detection thread. It can check whenever a controller is connected or not.
//Most of the console outputs have been disabled since this thread runs from the begining of the game and never stops.
static int	threadTimeDeltas[256];
static int	threadPacket[256];
static int	threadCount;
static int	defaultAvailable;
#if SDL_VERSION_ATLEAST(2, 0, 0)
void JoystickSamplingThread(void* data){
#else
int JoystickSamplingThread(void* data){
#endif

	static int prevTime = 0;
	static uint64 nextCheck[MAX_JOYSTICKS] = { 0 };
	const uint64 waitTime = 5000;// 000; // poll every 5 seconds to see if a controller was connected
	while(1){
		if (joyThreadKill) {
#if SDL_VERSION_ATLEAST(2, 0, 0)
			for(int i = 0; i < MAX_JOYSTICKS; i++) {
				if(haptic[i]){
					SDL_HapticClose(haptic[i]);
					haptic[i] = NULL;
				}
				if(gcontroller[i]){
					SDL_GameControllerClose(gcontroller[i]);
				}
				gcontroller[i]=NULL;
			}
#else
			if (joy)
			{
				SDL_JoystickClose(joy);
			}
			joy = NULL;
#endif
			break;
		}
		int	now = Sys_Microseconds();
		int	delta;
		if( prevTime == 0 )
		{
			delta = 4000;
		}
		else
		{
			delta = now - prevTime;
		}
		prevTime = now;
		threadTimeDeltas[threadCount & 255] = delta;
		threadCount++;
		if(now>=nextCheck[0]){ //GK: Similar to the windows thread
#if SDL_VERSION_ATLEAST(2, 0, 0)
	SDL_GameController* controller = NULL;
	int inactive = 0;
	int available = -1;
	bool alreadyConnected = false;
	for( int i = 0; i < MAX_JOYSTICKS; ++i )
	{
		if( SDL_IsGameController( i ) )
		{
			if (!gcontroller[i]) {
				controller = SDL_GameControllerOpen( i );
				if( controller )
				{
					if (available < 0) {
						available = i;
					}
					nextCheck[0]=0; //GK: Like the Windows thread constantly checking for the controller state once it's connected
					idLib::Printf("	Controller Connected: %s\n", SDL_GameControllerName(controller));
					gcontroller[i]=controller;
					if (!haptic[i]){ //GK: Initialize Haptic Device ONLY ONCE after the controller is connected
					haptic[i] = SDL_HapticOpenFromJoystick(SDL_GameControllerGetJoystick(gcontroller[i])); //GK: Make sure it mounted to the right controller
					if(haptic[i]){
						if(SDL_HapticRumbleInit( haptic[i] ) < 0){
							//common->Printf("Failed to rumble\n");
						}
					if ((SDL_HapticQuery(haptic[i]) & SDL_HAPTIC_LEFTRIGHT)==0){ //GK: Also make sure it has support for left-right motor rumble
						SDL_HapticClose(haptic[i]);
						haptic[i] = NULL;
						// common->Printf("Failed to find rumble effect\n");
					}
					idLib::Printf("Found haptic Device %d\n",SDL_HapticNumEffects(haptic[i]));
					}
					}
				}
			} else {
				alreadyConnected = true;
				continue;
				//common->Printf( "GameController %i name: %s\n", i, SDL_GameControllerName( controller ) );
				//common->Printf( "GameController %i is mapped as \"%s\".\n", i, SDL_GameControllerMapping( controller ) );
			}
		}else{
			inactive++;
					if(haptic[i]){
						SDL_HapticClose(haptic[i]);
						haptic[i] = NULL;
					}
					if(gcontroller[i]){
						SDL_GameControllerClose(gcontroller[i]);
					}
					gcontroller[i]=NULL;
					nextCheck[0] = now + waitTime;
					continue;
		}
	}
	if (!alreadyConnected) {
		//GK: Enable controller layout if there is one controller connected
		registeredControllers = 4-inactive;
		if (registeredControllers > 0) {
			if (session->GetSignInManager().GetMasterLocalUser() != NULL) {
				idLocalUserWin* user = dynamic_cast<idLocalUserWin*>(session->GetSignInManager().GetMasterLocalUser());
				user->SetInputDevice(available);
			}
			else {
				defaultAvailable = available;
			}
		}
	}
	/*idLib::joystick = inactive >= 4 ? false : true;*/
#else
	
	int numJoysticks = SDL_NumJoysticks();
	//common->Printf( "Sys_InitInput: Joystic - Found %i joysticks\n", numJoysticks );
//#if !SDL_VERSION_ATLEAST(2, 0, 0)
	int inactive = 0;
	int available = -1;
	for (i = 0; i < numJoysticks; i++) {
		//common->Printf( " Joystick %i name '%s'\n", i, SDL_JoystickName( i ) );
//#endif

	// Open first available joystick and use it
		if (SDL_NumJoysticks() > 0)
		{
			joy = SDL_JoystickOpen(i);

			if (joy)
			{
				if (available < 0) {
					available = i;
				}
				nextCheck[0] = 0;
				int num_hats;

				num_hats = SDL_JoystickNumHats(joy);
				//common->Printf( "Opened Joystick number 0\n" );
			//common->Printf( "Number of Axes: %d\n", SDL_JoystickNumAxes( joy ) );
			//common->Printf( "Number of Buttons: %d\n", SDL_JoystickNumButtons( joy ) );
			//common->Printf( "Number of Hats: %d\n", num_hats );
			//common->Printf( "Number of Balls: %d\n", SDL_JoystickNumBalls( joy ) );

				SDL_joystick_has_hat = 0;
				if (num_hats)
				{
					SDL_joystick_has_hat = 1;
				}
				break;
			}
			else
			{
				inactive++;
				nextCheck[0] = now + waitTime;
				if (joy)
				{
					//common->Printf( "Sys_ShutdownInput: closing SDL joystick.\n" );
					SDL_JoystickClose(joy);
				}
				joy = NULL;
				//common->Printf( "Couldn't open Joystick 0\n" );
			}
		}
		else
		{
			inactive = 4;
			nextCheck[0] = now + waitTime;
			if (joy)
			{
				//common->Printf( "Sys_ShutdownInput: closing SDL joystick.\n" );
				SDL_JoystickClose(joy);
			}
			joy = NULL;
		}
	}
	//GK: Enable controller layout if there is one controller connected
	if (inactive < MAX_JOYSTICKS) {
		if (session->GetSignInManager().GetMasterLocalUser() != NULL) {
			((idLocalUserWin*)session->GetSignInManager().GetMasterLocalUser())->SetInputDevice(available);
		}
		else {
			defaultAvailable = available;
		}
	}
	//idLib::joystick = inactive >= 4 ? false : true;
	// WM0110
#endif
		}else{
			continue;
		}
#if SDL_VERSION_ATLEAST(2, 0, 0)
		SDL_Delay(4);
#endif
	}
	#if !SDL_VERSION_ATLEAST(2, 0, 0)
	return 0; //GK: Don't forget SDL 1.2 require to return int value despite the fact that never does
	#endif
}