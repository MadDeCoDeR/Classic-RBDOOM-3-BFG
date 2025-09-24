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
#include <SDL3/SDL.h>

#include "renderer/RenderCommon.h"
#include "sdl_local.h"
#include "../posix/posix_public.h"
#include "../common/localuser.h"
#include "../../framework/Common.h"

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
void JoystickSamplingThread( void* data );
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
static SDL_Gamepad* gcontroller[MAX_JOYSTICKS] = {NULL}; //GK: Keep the SDL_Controller global in order to free the SDL_Controller when it's get disconnected
static SDL_Haptic *haptic[MAX_JOYSTICKS] = {NULL}; //GK: Joystick rumble support
static int registeredControllers = 0;
static bool joyThreadKill = false;
int SDL_joystick_has_hat = 0;
bool buttonStates[MAX_JOYSTICKS][K_LAST_KEY];	// For keeping track of button up/down events
extern SDL_Window* window;


#define	MAX_QUED_EVENTS		256
#define	MASK_QUED_EVENTS	( MAX_QUED_EVENTS - 1 )

sysEvent_t	eventQue[MAX_QUED_EVENTS];
int			eventHead = 0;
int			eventTail = 0;

#include "sdl2_scancode_mappings.h"
#include <map>

static int SDLScanCodeToKeyNum( SDL_Scancode sc )
{
	int idx = int( sc );
	assert( idx >= 0 && idx < SDL_SCANCODE_COUNT );
	
	return scanCodeToKeyNum[idx];
}

static SDL_Scancode KeyNumToSDLScanCode( int keyNum )
{
	if( keyNum < K_JOY1 )
	{
		for( int i = 0; i < SDL_SCANCODE_COUNT; ++i )
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
	size_t outbytesleft = 4 * len; // *4 because utf-32 needs 4x as much space as utf-8
	char* outbuf = ( char* )utf32buf;
	size_t n = SDL_iconv( cd, &utf8str, &inbytesleft, &outbuf, &outbytesleft );
	
	if( n == size_t( -1 ) ) // some error occured during iconv
	{
		common->Warning( "Converting UTF-8 string \"%s\" from SDL_TEXTINPUT to UTF-32 failed!", utf8str );
		
		// clear utf32-buffer, just to be sure there's no garbage..
		memset( utf32buf, 0, len * sizeof( int32 ) );
	}
	
	// reset cd so it can be used again
	SDL_iconv( cd, NULL, &inbytesleft, NULL, &outbytesleft );
	
}

static void PushConsoleEvent( const char* s )
{
	char* b;
	size_t len;
	
	len = strlen( s ) + 1;
	b = ( char* )Mem_Alloc( len, TAG_EVENTS );
	strcpy( b, s );
	
	SDL_Event event;
	
	event.type = SDL_EVENT_USER;
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
	
	in_keyboard.SetModified();
	//GK: Insted of initializing only once the joystick run a thread that will allow the dynamic connection/disconnection of it
	joyThread = SDL_CreateThread((SDL_ThreadFunction)JoystickSamplingThread,"Joystic",NULL);
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

static std::map<SDL_JoystickID, int> reverseControllerMap;
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

// utf-32 version of the textinput event
static int32 uniStr[512] = {0};
static size_t uniStrPos = 0;
// DG: fake a "mousewheel not pressed anymore" event for SDL2
// so scrolling in menus stops after one step
static int mwheelRel = 0;
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

	if( uniStr[0] != 0 )
	{
		Sys_QueEvent(SE_CHAR, uniStr[uniStrPos], 1, 0, NULL, 0);
		
		++uniStrPos;
		
		if( !uniStr[uniStrPos] || uniStrPos == 512 )
		{
			memset( uniStr, 0, sizeof( uniStr ) );
			uniStrPos = 0;
		}
	}
	if( uniChar )
	{
		Sys_QueEvent(SE_CHAR, uniChar, 1, 0, NULL, 0);
		
		uniChar = 0;
	}

	if( mwheelRel )
	{
		Sys_QueEvent(SE_KEY, mwheelRel, 0, 0, NULL, 0);
		mwheelRel = 0;
	}

	if (console->Active()) {
		if (!SDL_TextInputActive(window)) {
			if (!SDL_StartTextInput(window)) {
				common->Warning("Error initializing text input: %s\n", SDL_GetError());
			}
		}
	}
	else {
		if (SDL_TextInputActive(window)) {
			SDL_StopTextInput(window);
		}
	}
	// loop until there is an event we care about (will return then) or no more events
	while ( SDL_PollEvent( &ev ) )
	{
		switch (ev.type)
		{
			case SDL_EVENT_WINDOW_FOCUS_GAINED:
			{
				// unset modifier, in case alt-tab was used to leave window and ALT is still set
				// as that can cause fullscreen-toggling when pressing enter...
				SDL_Keymod currentmod = SDL_GetModState();
				int newmod = SDL_KMOD_NONE;
				if (currentmod & SDL_KMOD_CAPS) // preserve capslock
					newmod |= SDL_KMOD_CAPS;

				SDL_SetModState((SDL_Keymod)newmod);

				// DG: un-pause the game when focus is gained, that also re-grabs the input
				//     disabling the cursor is now done once in GLimp_Init() because it should always be disabled
				soundSystem->SetMute(false);
				cvarSystem->SetCVarBool("com_pausePlatform", false);
				cvarSystem->SetCVarBool("com_pause", false);
				// DG end
				break;
			}

			case SDL_EVENT_WINDOW_FOCUS_LOST:
				SDL_MinimizeWindow(window);
				// DG: pause the game when focus is lost, that also un-grabs the input
				soundSystem->SetMute(true);
				cvarSystem->SetCVarBool("com_pausePlatform", true);
				cvarSystem->SetCVarBool("com_pause", true);
				// DG end
				break;

			case SDL_EVENT_WINDOW_MOUSE_LEAVE:
				// mouse has left the window
				Sys_QueEvent(SE_MOUSE_LEAVE, 0, 0, 0, NULL, 0);
				break;

				// DG: handle resizing and moving of window
			case SDL_EVENT_WINDOW_RESIZED:
			{
				int w = ev.window.data1;
				int h = ev.window.data2;
				r_windowWidth.SetInteger(w);
				r_windowHeight.SetInteger(h);

				glConfig.nativeScreenWidth = w;
				glConfig.nativeScreenHeight = h;
				cmdSystem->BufferCommandText(CMD_EXEC_APPEND, "vid_restart\n");
				break;
			}

			case SDL_EVENT_WINDOW_MOVED:
			{
				int x = ev.window.data1;
				int y = ev.window.data2;
				r_windowX.SetInteger(x);
				r_windowY.SetInteger(y);
				cmdSystem->BufferCommandText(CMD_EXEC_APPEND, "vid_restart\n");
				break;
			}
			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			{
				com_emergencyexit.SetBool(true);
				soundSystem->SetMute(true);
				cmdSystem->BufferCommandText(CMD_EXEC_APPEND, "quit\n");
				break;
			}
			continue;
		case SDL_EVENT_KEY_DOWN:
			if (ev.key.key == SDLK_RETURN && (ev.key.mod & SDL_KMOD_ALT) > 0)
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
					fullscreen = SDL_GetDisplayForRect(windowRect);
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
			if (ev.key.key == SDLK_G && (ev.key.mod & SDL_KMOD_CTRL) > 0)
			{
				bool grab = cvarSystem->GetCVarBool("in_nograb");
				grab = !grab;
				cvarSystem->SetCVarBool("in_nograb", grab);
				continue; // handle next event
			}
			// DG end

			// fall through
		case SDL_EVENT_KEY_UP:
		{
			bool isChar;

			// DG: special case for SDL_SCANCODE_GRAVE - the console key under Esc
			if (ev.key.scancode == SDL_SCANCODE_GRAVE)
			{
				key = K_GRAVE;
				uniChar = K_BACKSPACE; // bad hack to get empty console inputline..
			} // DG end, the original code is in the else case
			else
			{
				key = SDLScanCodeToKeyNum(ev.key.scancode);

				if (key == 0)
				{
					// SDL2 has no ev.key.keysym.unicode anymore.. but the scancode should work well enough for console
					if (ev.type == SDL_EVENT_KEY_DOWN) // FIXME: don't complain if this was an ASCII char and the console is open?
						common->Warning("unmapped SDL key %d scancode %d", ev.key.key, ev.key.scancode);

					continue; // just handle next event
				}
			}

			//res.evType = SE_KEY;
			//res.evValue = key;
			//res.evValue2 = ev.key.state == SDL_PRESSED ? 1 : 0;

			kbd_polls.Append(kbd_poll_t(key, ev.key.down == true));

			if (key == K_BACKSPACE && ev.key.down == true)
				uniChar = key;

			Sys_QueEvent(SE_KEY, key, (ev.key.down == true ? 1 : 0), 0, NULL, 0);
			//return res;
		}
		break;
		case SDL_EVENT_TEXT_INPUT:
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

		case SDL_EVENT_MOUSE_MOTION:
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

		case SDL_EVENT_FINGER_DOWN:
		case SDL_EVENT_FINGER_UP:
		case SDL_EVENT_FINGER_MOTION:
			continue; // Avoid 'unknown event' spam when testing with touchpad by skipping this

		case SDL_EVENT_MOUSE_WHEEL:
			//res.evType = SE_KEY;

			//res.evValue = (ev.wheel.y > 0) ? K_MWHEELUP : K_MWHEELDOWN;
			mouse_polls.Append(mouse_poll_t(M_DELTAZ, ev.wheel.y));

			//res.evValue2 = 1; // for "pressed"
			Sys_QueEvent(SE_KEY, (ev.wheel.y > 0) ? K_MWHEELUP : K_MWHEELDOWN, 1, 0, NULL, 0);

			// remember mousewheel direction to issue a "not pressed anymore" event
			mwheelRel = (ev.wheel.y > 0) ? K_MWHEELUP : K_MWHEELDOWN;

			//return res;
			break;

		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		case SDL_EVENT_MOUSE_BUTTON_UP:
			res.evType = SE_KEY;

			switch (ev.button.button)
			{
			case SDL_BUTTON_LEFT:
				res.evValue = K_MOUSE1;
				mouse_polls.Append(mouse_poll_t(M_ACTION1, ev.button.down == true ? 1 : 0));
				break;
			case SDL_BUTTON_MIDDLE:
				res.evValue = K_MOUSE3;
				mouse_polls.Append(mouse_poll_t(M_ACTION3, ev.button.down == true ? 1 : 0));
				break;
			case SDL_BUTTON_RIGHT:
				res.evValue = K_MOUSE2;
				mouse_polls.Append(mouse_poll_t(M_ACTION2, ev.button.down == true ? 1 : 0));
				break;

			default:
				// handle X1 button and above
				if (ev.button.button <= 16) // d3bfg doesn't support more than 16 mouse buttons
				{
					int buttonIndex = ev.button.button - SDL_BUTTON_LEFT;
					res.evValue = K_MOUSE1 + buttonIndex;
					mouse_polls.Append(mouse_poll_t(M_ACTION1 + buttonIndex, ev.button.down == true ? 1 : 0));
					break;
				}
				else // unsupported mouse button
				{
					continue; // just ignore
				}
			}

			res.evValue2 = ev.button.down == true ? 1 : 0;

			Sys_QueEvent(res.evType, res.evValue, res.evValue2, 0, NULL, 0);
			break;
			// GameController
		case SDL_EVENT_JOYSTICK_AXIS_MOTION:
		case SDL_EVENT_JOYSTICK_HAT_MOTION:
		case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
		case SDL_EVENT_JOYSTICK_BUTTON_UP:
		case SDL_EVENT_JOYSTICK_ADDED:
		case SDL_EVENT_JOYSTICK_REMOVED:
		case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE:
		case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE:
			// Avoid 'unknown event' spam
			continue;
		case SDL_EVENT_GAMEPAD_AXIS_MOTION:
			joyEvent = (sys_jEvents)(J_AXIS_LEFT_X + ev.gaxis.axis);
			switch(joyEvent) {
				case J_AXIS_LEFT_X:
					current[reverseControllerMap[ev.gaxis.which]].LXThumb = ev.gaxis.value;
					break;
				case J_AXIS_LEFT_Y:
					current[reverseControllerMap[ev.gaxis.which]].LYThumb = ev.gaxis.value;
					break;
				case J_AXIS_RIGHT_X:
					current[reverseControllerMap[ev.gaxis.which]].RXThumb = ev.gaxis.value;
					break;
				case J_AXIS_RIGHT_Y:
					current[reverseControllerMap[ev.gaxis.which]].RYThumb = ev.gaxis.value;
					break;
				case J_AXIS_LEFT_TRIG:
					current[reverseControllerMap[ev.gaxis.which]].LTrigger = ev.gaxis.value;
					break;
				case J_AXIS_RIGHT_TRIG:
					current[reverseControllerMap[ev.gaxis.which]].RTrigger = ev.gaxis.value;
					break;

			}
			break;
		case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
		case SDL_EVENT_GAMEPAD_BUTTON_UP:
			current[reverseControllerMap[ev.gbutton.which]].buttons[ev.gbutton.button] = (ev.gbutton.down == true ? 1 : 0);
			break;
		//GK: Steam Deck Hack: For some reason Steam Deck spams these two events
		case SDL_EVENT_GAMEPAD_ADDED:
		case SDL_EVENT_GAMEPAD_REMAPPED:
		case SDL_EVENT_KEYMAP_CHANGED:
			continue;
				
			case SDL_EVENT_QUIT:
				PushConsoleEvent( "quit" );
				Sys_QueEvent(no_more_events.evType, no_more_events.evValue, no_more_events.evValue2, no_more_events.evPtrLength, no_more_events.evPtr, 0); // don't handle next event, just quit.
				break;
			case SDL_EVENT_USER:
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
				//common->Warning( "unknown event %u", ev.type ); //GK: We don't have to log everything
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
	
	SDL_FlushEvents(SDL_EVENT_FIRST, SDL_EVENT_LAST);
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
	SDL_Scancode scancode = KeyNumToSDLScanCode( ( int )keynum );
	SDL_Keycode keycode = SDL_GetKeyFromScancode( scancode, SDL_KMOD_NONE,  true);
	
	const char* ret = SDL_GetKeyName( keycode );
	if( ret != NULL && ret[0] != '\0' )
	{
		return ret;
	}
	return NULL;
}

char* Sys_GetClipboardData()
{
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
}

void Sys_SetClipboardData( const char* string )
{
	SDL_SetClipboardText( string );
}


//=====================================================================================
//	Joystick Input Handling
//=====================================================================================

void Sys_SetRumble( int device, int low, int hi )
{
	//GK: This is the code for the rumble effect by using SDL_Haptic.
	//It doesn't affect the game's performance (Remember NO SDL_Delay)

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
	if(SDL_GetMaxHapticEffectsPlaying(haptic[device]) >= SDL_GetMaxHapticEffects(haptic[device])){
		SDL_DestroyHapticEffect(haptic[device],0);
	}
	//GK:End of fix
	effect_id = SDL_CreateHapticEffect( haptic[device], &effect );
	if(effect_id < 0){
		common->Printf("%s\n",SDL_GetError());
		return;
	}

	 if(SDL_RunHapticEffect( haptic[device], effect_id, 1 )<0){
		 common->Printf("%s\n",SDL_GetError());
	 }
	}
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
	for (int i = 0; i < MAX_JOYSTICKS; i++) {
		if (gcontroller[i]) {
			hasConnected = true;
			break;
		}
	}
	return hasConnected;
}

//GK: This is the controller state detection thread. It can check whenever a controller is connected or not.
//Most of the console outputs have been disabled since this thread runs from the begining of the game and never stops.
static int	threadTimeDeltas[256];
static int	threadPacket[256];
static int	threadCount;
static int	defaultAvailable;


void JoystickSamplingThread(void* data){

	static int prevTime = 0;
	static uint64 nextCheck[MAX_JOYSTICKS] = { 0 };
	const uint64 waitTime = 5000;// 000; // poll every 5 seconds to see if a controller was connected
	while(1){
		if (joyThreadKill) {
			for(int i = 0; i < MAX_JOYSTICKS; i++) {
				if(haptic[i]){
					SDL_CloseHaptic(haptic[i]);
					haptic[i] = NULL;
				}
				if(gcontroller[i]){
					SDL_CloseGamepad(gcontroller[i]);
				}
				gcontroller[i]=NULL;
			}
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
	SDL_Gamepad* controller = NULL;
	int inactive = 0;
	int available = -1;
	bool alreadyConnected = false;
	int count = 0;
	SDL_JoystickID* controllers = SDL_GetGamepads(&count);
	if (count == 0) {
		reverseControllerMap.clear();
		count = 4; //GK: Clean time
	}
	for( uint32 i = 0; i < count; i++ )
	{
		if( SDL_IsGamepad( controllers[i] ) )
		{
			if (!gcontroller[i]) {
				controller = SDL_OpenGamepad( controllers[i] );
				if( controller )
				{
					if (available < 0) {
						available = i;
					}
					reverseControllerMap.insert(std::make_pair(controllers[i], i));
					nextCheck[0]=0; //GK: Like the Windows thread constantly checking for the controller state once it's connected
					idLib::Printf("Controller Connected: %s\n", SDL_GetGamepadName(controller));
					gcontroller[i]=controller;
					int hcount = 0;
					SDL_HapticID* haptics = SDL_GetHaptics(&hcount);
					if (!haptic[i]){ //GK: Initialize Haptic Device ONLY ONCE after the controller is connected
						int selected = -1;
						const char* controllerName = SDL_GetGamepadName(gcontroller[i]);
						common->Printf("Found Haptic Devices:\n");
						for (int j = 0; j < hcount; j++) {
							const char* hapticName = SDL_GetHapticNameForID(haptics[j]);
							common->Printf("\t%s\n", hapticName);
							if (!idStr::Cmp(controllerName, hapticName)) {
								selected = j;
							}
						}
						haptic[i] = SDL_OpenHaptic(haptics[selected]); //GK: Make sure it mounted to the right controller
						if(haptic[i]){
							if(SDL_InitHapticRumble( haptic[i] ) < 0){
								common->Printf("Failed to initialize rumble support with Error: %s\n", SDL_GetError());
							}
						if ((SDL_GetHapticFeatures(haptic[i]) & SDL_HAPTIC_LEFTRIGHT)==0){ //GK: Also make sure it has support for left-right motor rumble
							SDL_CloseHaptic(haptic[i]);
							haptic[i] = NULL;
							// common->Printf("Failed to find rumble effect\n");
						}
						idLib::Printf("Found haptic Device %d\n",SDL_GetMaxHapticEffects(haptic[i]));
						} else {
							common->Printf("Error Opening Haptic Device: %s\n", SDL_GetError());
						}
					
					}
					SDL_free(haptics);
				} else {
					common->Warning("Error Initializing controller: %s\n", SDL_GetError());
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
						SDL_CloseHaptic(haptic[i]);
						haptic[i] = NULL;
					}
					if(gcontroller[i]){
						SDL_CloseGamepad(gcontroller[i]);
					}
					gcontroller[i]=NULL;
					nextCheck[0] = now + waitTime;
					continue;
		}
	}
	SDL_free(controllers);
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
		}else{
			continue;
		}
		SDL_Delay(4);
	}
}