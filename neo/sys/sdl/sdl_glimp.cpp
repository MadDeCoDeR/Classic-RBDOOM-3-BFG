/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 dhewg (dhewm3)
Copyright (C) 2012-2014 Robert Beckebans
Copyright (C) 2013 Daniel Gibson

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
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

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
#include "res/doom_ico.cpp"

#include "renderer/RenderCommon.h"
#include "sdl_local.h"

idCVar in_nograb( "in_nograb", "0", CVAR_SYSTEM | CVAR_NOCHEAT, "prevents input grabbing" );

// RB: FIXME this shit. We need the OpenGL alpha channel for advanced rendering effects
idCVar r_waylandcompat( "r_waylandcompat", "0", CVAR_SYSTEM | CVAR_NOCHEAT | CVAR_ARCHIVE, "wayland compatible framebuffer" );

// RB: only relevant if using SDL 2.0
#if defined(__APPLE__)
// only core profile is supported on OS X
idCVar r_useOpenGL32( "r_useOpenGL32", "2", CVAR_INTEGER, "0 = OpenGL 3.x, 1 = OpenGL 3.2 compatibility profile, 2 = OpenGL 3.2 core profile", 0, 2 );
#elif defined(__linux__)
// Linux open source drivers suck //GK: So as it seems some mesa drivers might not support one of the two texture compression extensions that are required, in that case keep it to compatibility profile
idCVar r_useOpenGL32( "r_useOpenGL32", "1", CVAR_INTEGER, "0 = OpenGL 3.x, 1 = OpenGL 3.2 compatibility profile, 2 = OpenGL 3.2 core profile", 0, 2 );
#else
idCVar r_useOpenGL32( "r_useOpenGL32", "1", CVAR_INTEGER, "0 = OpenGL 3.x, 1 = OpenGL 3.2 compatibility profile, 2 = OpenGL 3.2 core profile", 0, 2 );
#endif
// RB end

extern idCVar r_fullscreen;

static bool grabbed = false;

#if SDL_VERSION_ATLEAST(2, 0, 0)

static SDL_GLContext context = NULL;
#else
static SDL_Surface* window = NULL;
#define SDL_WINDOW_OPENGL SDL_OPENGL
#define SDL_WINDOW_FULLSCREEN SDL_FULLSCREEN
#define SDL_WINDOW_RESIZABLE SDL_RESIZABLE
#endif

/*
===================
GLimp_PreInit

 R_GetModeListForDisplay is called before GLimp_Init(), but SDL needs SDL_Init() first.
 So do that in GLimp_PreInit()
 Calling that function more than once doesn't make a difference
===================
*/

void GLimp_PreInit() // DG: added this function for SDL compatibility
{
#ifdef __unix__
	if (SDL_GetCurrentVideoDriver() != "wayland") { //GK: On Linux avoid x11 as much as possible. Since SDL2 is statically linked with Wayland extension enforce it
		bool hasWayland = false;
		int driversNum = SDL_GetNumVideoDrivers();
		for (int wi = 0; wi < driversNum; wi++) {
			if (SDL_GetVideoDriver(wi) == "wayland") {
				hasWayland = true;
				break;
			}
		}
		if (hasWayland) {
			if (SDL_VideoInit("wayland") < 0) {
				common->Warning("Error when trying to use Wayland: %s\nReverting to the original Video Driver\n", SDL_GetError());
				SDL_VideoInit(SDL_GetVideoDriver(0));
			}
		}
	}
#endif
}

/*
===================
GLimp_GetSupportedVersion
===================
*/
/*
 GK: Supported Version retrieval. First we need a context without the version attributes set,
 in order to retrieve it from the system and use it acordingly later with other flags and attributes
 */
void GLimp_GetSupportedVersion(int* major, int* minor) {
	
	window = SDL_CreateWindow(ENGINE_NAME,
							0,
							0,
							0, 0, SDL_WINDOW_OPENGL );
	// DG end
	if (window != NULL) {
		context = SDL_GL_CreateContext( window );
		if (context != NULL) {
			idStr version_string = idStr(( const char* )glGetString( GL_VERSION )).SubStr(0, 3);
			idList<idStr> version_array = version_string.Split(".");
			*major = atoi(version_array[0].c_str());
			*minor = atoi(version_array[1].c_str());
			SDL_GL_DeleteContext(context);
			context = NULL;
		}
		SDL_DestroyWindow(window);
		window = NULL;
	}
}

/*
===================
GLimp_Init
===================
*/
bool GLimp_Init( glimpParms_t parms )
{
	common->Printf( "Initializing OpenGL subsystem\n" );
	
	GLimp_PreInit(); // DG: make sure SDL is initialized
	// DG: make window resizable
	Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_GRABBED;
	// DG end
	
	if( parms.fullScreen )
		flags |= SDL_WINDOW_FULLSCREEN;
	else if (parms.fullScreen < 0)
		flags |= SDL_WINDOW_BORDERLESS;
		
	int colorbits = 24;
	int depthbits = 24;
	int stencilbits = 8;
	
	for( int i = 0; i < 16; i++ )
	{
		// 0 - default
		// 1 - minus colorbits
		// 2 - minus depthbits
		// 3 - minus stencil
		if( ( i % 4 ) == 0 && i )
		{
			// one pass, reduce
			switch( i / 4 )
			{
				case 2 :
					if( colorbits == 24 )
						colorbits = 16;
					break;
				case 1 :
					if( depthbits == 24 )
						depthbits = 16;
					else if( depthbits == 16 )
						depthbits = 8;
				case 3 :
					if( stencilbits == 24 )
						stencilbits = 16;
					else if( stencilbits == 16 )
						stencilbits = 8;
			}
		}
		
		int tcolorbits = colorbits;
		int tdepthbits = depthbits;
		int tstencilbits = stencilbits;
		
		if( ( i % 4 ) == 3 )
		{
			// reduce colorbits
			if( tcolorbits == 24 )
				tcolorbits = 16;
		}
		
		if( ( i % 4 ) == 2 )
		{
			// reduce depthbits
			if( tdepthbits == 24 )
				tdepthbits = 16;
			else if( tdepthbits == 16 )
				tdepthbits = 8;
		}
		
		if( ( i % 4 ) == 1 )
		{
			// reduce stencilbits
			if( tstencilbits == 24 )
				tstencilbits = 16;
			else if( tstencilbits == 16 )
				tstencilbits = 8;
			else
				tstencilbits = 0;
		}
		
		int channelcolorbits = 4;
		if( tcolorbits == 24 )
			channelcolorbits = 8;
			
		SDL_GL_SetAttribute( SDL_GL_RED_SIZE, channelcolorbits );
		SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, channelcolorbits );
		SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, channelcolorbits );
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
		SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, tdepthbits );
		SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, tstencilbits );
		
		if( r_waylandcompat.GetBool() )
			SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 0 );
		else
			SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, channelcolorbits );
			
		SDL_GL_SetAttribute( SDL_GL_STEREO, parms.stereo ? 1 : 0 );
		
		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, parms.multiSamples ? 1 : 0 );
		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, parms.multiSamples );
		
#if SDL_VERSION_ATLEAST(2, 0, 0)
		
		// RB begin
		if( r_useOpenGL32.GetInteger() > 0 )
		{
			int major = 3;
			int minor = 3;
			if (r_useOpenGL32.GetInteger() == 1) {
				SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY );
			} else {
				SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
			}
			GLimp_GetSupportedVersion(&major, &minor);
			if (major < 3 || major == 3 && minor < 3) {
				common->FatalError("GLimp_Init: OpenGL version is not supported!\n");
			}
			glConfig.driverType = GLDRV_OPENGL32_COMPATIBILITY_PROFILE;
			
			SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, major );
			SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, minor );
			
			
			if( r_debugContext.GetBool() )
			{
				SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG );
			}
		}
		
		if( r_useOpenGL32.GetInteger() > 1 )
		{
			glConfig.driverType = GLDRV_OPENGL32_CORE_PROFILE;
		}
		// RB end
		
		// DG: set display num for fullscreen
		int windowPos = SDL_WINDOWPOS_UNDEFINED;
		if( parms.fullScreen > 0 )
		{
			if( parms.fullScreen > SDL_GetNumVideoDisplays() )
			{
				common->Warning( "Couldn't set display to num %i because we only have %i displays",
								 parms.fullScreen, SDL_GetNumVideoDisplays() );
			}
			else
			{
				// -1 because SDL starts counting displays at 0, while parms.fullScreen starts at 1
				windowPos = SDL_WINDOWPOS_UNDEFINED_DISPLAY( ( parms.fullScreen - 1 ) );
			}
		}
		// TODO: if parms.fullScreen == -1 there should be a borderless window spanning multiple displays
		/*
		 * NOTE that this implicitly handles parms.fullScreen == -2 (from r_fullscreen -2) meaning
		 * "do fullscreen, but I don't care on what monitor", at least on my box it's the monitor with
		 * the mouse cursor.
		 */
		
		
			window = SDL_CreateWindow(ENGINE_NAME,
								   windowPos,
								   windowPos,
								   parms.width, parms.height, flags );
		// DG end
		
		context = SDL_GL_CreateContext( window );
		
		if( !window )
		{
			common->DPrintf( "Couldn't set GL mode %d/%d/%d: %s",
							 channelcolorbits, tdepthbits, tstencilbits, SDL_GetError() );
			continue;
		}

		SDL_VERSION(&windowInfo.version);
		SDL_GetWindowWMInfo(window, &windowInfo);

		Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    	int shift = (doom_icon.bytes_per_pixel == 3) ? 8 : 0;
    	rmask = 0xff000000 >> shift;
    	gmask = 0x00ff0000 >> shift;
    	bmask = 0x0000ff00 >> shift;
    	amask = 0x000000ff >> shift;
#else // little endian, like x86
    	rmask = 0x000000ff;
    	gmask = 0x0000ff00;
    	bmask = 0x00ff0000;
    	amask = (doom_icon.bytes_per_pixel == 3) ? 0 : 0xff000000;
#endif
		
		SDL_Surface* surf = SDL_CreateRGBSurfaceFrom((void*)doom_icon.pixel_data, doom_icon.width, doom_icon.height,
		 doom_icon.bytes_per_pixel * 8, doom_icon.bytes_per_pixel * doom_icon.width, rmask, gmask, bmask, amask);

		 SDL_SetWindowIcon(window, surf);

		if( SDL_GL_SetSwapInterval( r_swapInterval.GetInteger() ) < 0 )
			common->Warning( "SDL_GL_SWAP_CONTROL not supported" );
			
		// RB begin
		SDL_GL_GetDrawableSize( window, &glConfig.nativeScreenWidth, &glConfig.nativeScreenHeight );
		// RB end
		
		glConfig.isFullscreen = ( SDL_GetWindowFlags( window ) & SDL_WINDOW_FULLSCREEN ) == SDL_WINDOW_FULLSCREEN;
#else
		glConfig.driverType = GLDRV_OPENGL3X;
		
		SDL_WM_SetCaption(ENGINE_NAME, ENGINE_NAME);
		
		if( SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, r_swapInterval.GetInteger() ) < 0 )
			common->Warning( "SDL_GL_SWAP_CONTROL not supported" );
		
		window = SDL_SetVideoMode( parms.width, parms.height, colorbits, flags );
		if( !window )
		{
			common->DPrintf( "Couldn't set GL mode %d/%d/%d: %s",
							 channelcolorbits, tdepthbits, tstencilbits, SDL_GetError() );
			continue;
		}
		
		glConfig.nativeScreenWidth = window->w > 0 ? window->w : 1280;
		glConfig.nativeScreenHeight = window->h > 0 ? window->h: 720;
		
		glConfig.isFullscreen = ( window->flags & SDL_FULLSCREEN ) == SDL_FULLSCREEN;
#endif
		
		common->Printf( "Using %d color bits, %d depth, %d stencil display\n",
						channelcolorbits, tdepthbits, tstencilbits );
						
		glConfig.colorBits = tcolorbits;
		glConfig.depthBits = tdepthbits;
		glConfig.stencilBits = tstencilbits;
		
		// RB begin
		glConfig.displayFrequency = 60;
		glConfig.isStereoPixelFormat = parms.stereo;
		glConfig.multisamples = parms.multiSamples;
		
		glConfig.pixelAspect = 1.0f;	// FIXME: some monitor modes may be distorted
		// should side-by-side stereo modes be consider aspect 0.5?
		
		// RB end
		
		break;
	}
	
	if( !window )
	{
		common->Printf( "No usable GL mode found: %s", SDL_GetError() );
		return false;
	}
	
#ifdef __APPLE__
	glewExperimental = GL_TRUE;
#endif
	
	GLenum glewResult = glewInit();
	if( GLEW_OK != glewResult )
	{
		// glewInit failed, something is seriously wrong
		common->Printf( "^3GLimp_Init() - GLEW could not load OpenGL subsystem: %s", glewGetErrorString( glewResult ) );
	}
	else
	{
		common->Printf( "Using GLEW %s\n", glewGetString( GLEW_VERSION ) );
	}
	
	// DG: disable cursor, we have two cursors in menu (because mouse isn't grabbed in menu)
	SDL_ShowCursor( SDL_DISABLE );
	// DG end
	SDL_SetWindowGrab(window, SDL_FALSE);
	return true;
}
/*
===================
 Helper functions for GLimp_SetScreenParms()
===================
*/

#if SDL_VERSION_ATLEAST(2, 0, 0)
// SDL1 doesn't support multiple displays, so the source is much shorter and doesn't need separate functions
// makes sure the window will be full-screened on the right display and returns the SDL display index
static int ScreenParmsHandleDisplayIndex( glimpParms_t parms )
{
	int displayIdx;
	if( parms.fullScreen > 0 )
	{
		displayIdx = parms.fullScreen - 1; // first display for SDL is 0, in parms it's 1
	}
	else // -2 == use current display
	{
		displayIdx = SDL_GetWindowDisplayIndex( window );
		if( displayIdx < 0 ) // for some reason the display for the window couldn't be detected
			displayIdx = 0;
	}
	
	if( parms.fullScreen > SDL_GetNumVideoDisplays() )
	{
		common->Warning( "Can't set fullscreen mode to display number %i, because SDL2 only knows about %i displays!",
						 parms.fullScreen, SDL_GetNumVideoDisplays() );
		return -1;
	}
	
	if( parms.fullScreen != glConfig.isFullscreen )
	{
		// we have to switch to another display
		if( glConfig.isFullscreen )
		{
			// if we're already in fullscreen mode but want to switch to another monitor
			// we have to go to windowed mode first to move the window.. SDL-oddity.
			SDL_SetWindowFullscreen( window, SDL_FALSE );
		}
		// select display ; SDL_WINDOWPOS_UNDEFINED_DISPLAY() doesn't work.
		int x = SDL_WINDOWPOS_CENTERED_DISPLAY( displayIdx );
		// move window to the center of selected display
		SDL_SetWindowPosition( window, x, x );
	}
	return displayIdx;
}

static bool SetScreenParmsFullscreen( glimpParms_t parms )
{
	SDL_DisplayMode m = {0};
	int displayIdx = ScreenParmsHandleDisplayIndex( parms );
	if( displayIdx < 0 )
		return false;
		
	// get current mode of display the window should be full-screened on
	SDL_GetCurrentDisplayMode( displayIdx, &m );
	
	// change settings in that display mode according to parms
	// FIXME: check if refreshrate, width and height are supported?
	// m.refresh_rate = parms.displayHz;
	m.w = parms.width;
	m.h = parms.height;
	
	// set that displaymode
	if( SDL_SetWindowDisplayMode( window, &m ) < 0 )
	{
		common->Warning( "Couldn't set window mode for fullscreen, reason: %s", SDL_GetError() );
		return false;
	}
	
	// if we're currently not in fullscreen mode, we need to switch to fullscreen
	if( !( SDL_GetWindowFlags( window ) & SDL_WINDOW_FULLSCREEN ) )
	{
		if( SDL_SetWindowFullscreen( window, SDL_TRUE ) < 0 )
		{
			common->Warning( "Couldn't switch to fullscreen mode, reason: %s!", SDL_GetError() );
			return false;
		}
	}
	return true;
}

static bool SetScreenParmsWindowed( glimpParms_t parms )
{
	SDL_SetWindowSize( window, parms.width, parms.height );
	SDL_SetWindowPosition( window, parms.x, parms.y );
	
	// if we're currently in fullscreen mode, we need to disable that
	if( SDL_GetWindowFlags( window ) & SDL_WINDOW_FULLSCREEN )
	{
		if( SDL_SetWindowFullscreen( window, SDL_FALSE ) < 0 )
		{
			common->Warning( "Couldn't switch to windowed mode, reason: %s!", SDL_GetError() );
			return false;
		}
	}
	return true;
}
#endif // SDL_VERSION_ATLEAST(2, 0, 0)

/*
===================
GLimp_SetScreenParms
===================
*/
bool GLimp_SetScreenParms( glimpParms_t parms )
{
#if SDL_VERSION_ATLEAST(2, 0, 0)
	if( parms.fullScreen > 0 || parms.fullScreen < 0 )
	{
		if( !SetScreenParmsFullscreen( parms ) )
			return false;
	}
	else if( parms.fullScreen == 0 ) // windowed mode
	{
		if( !SetScreenParmsWindowed( parms ) )
			return false;
	}
	else
	{
		common->Warning( "GLimp_SetScreenParms: fullScreen -1 (borderless window for multiple displays) currently unsupported!" );
		return false;
	}
#else // SDL 1.2 - so much shorter, but doesn't handle multiple displays
	SDL_Surface* s = SDL_GetVideoSurface();
	if( s == NULL )
	{
		common->Warning( "GLimp_SetScreenParms: Couldn't get video information, reason: %s", SDL_GetError() );
		return false;
	}
	
	
	int bitsperpixel = 24;
	if( s->format )
		bitsperpixel = s->format->BitsPerPixel;
	
	Uint32 flags = s->flags;
	
	if( parms.fullScreen )
		flags |= SDL_FULLSCREEN;
	else
		flags &= ~SDL_FULLSCREEN;
	
	s = SDL_SetVideoMode( parms.width, parms.height, bitsperpixel, flags );
	if( s == NULL )
	{
		common->Warning( "GLimp_SetScreenParms: Couldn't set video information, reason: %s", SDL_GetError() );
		return false;
	}
#endif // SDL_VERSION_ATLEAST(2, 0, 0)
	
	// Note: the following stuff would also work with SDL1.2
	SDL_GL_SetAttribute( SDL_GL_STEREO, parms.stereo ? 1 : 0 );
	
	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, parms.multiSamples ? 1 : 0 );
	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, parms.multiSamples );
	
	glConfig.isFullscreen = parms.fullScreen;
	glConfig.isStereoPixelFormat = parms.stereo;
	glConfig.nativeScreenWidth = parms.width > 0 ? parms.width : 1280;
	glConfig.nativeScreenHeight = parms.height > 0 ? parms.height : 720;
	glConfig.displayFrequency = parms.displayHz;
	glConfig.multisamples = parms.multiSamples;
	
	return true;
}

/*
===================
GLimp_Shutdown
===================
*/
void GLimp_Shutdown()
{
	common->Printf( "Shutting down OpenGL subsystem\n" );
	
#if SDL_VERSION_ATLEAST(2, 0, 0)
	//GK: This wasn't called anywhere else, and is needed in order to properly destroy SDL context
	Sys_ShutdownInput();
	if( context )
	{
		SDL_GL_DeleteContext( context );
		context = NULL;
	}
	
	if( window )
	{
		SDL_DestroyWindow( window );
		window = NULL;
	}
	atexit(SDL_Quit);
#endif
}

/*
===================
GLimp_SwapBuffers
===================
*/
void GLimp_SwapBuffers()
{
#if SDL_VERSION_ATLEAST(2, 0, 0)
	SDL_GL_SwapWindow( window );
#else
	SDL_GL_SwapBuffers();
#endif
}

/*
=================
GLimp_SetGamma
=================
*/
void GLimp_SetGamma( unsigned short red[256], unsigned short green[256], unsigned short blue[256] )
{
	if( !window )
	{
		common->Warning( "GLimp_SetGamma called without window" );
		return;
	}
	
#if SDL_VERSION_ATLEAST(2, 0, 0)
	if( SDL_SetWindowGammaRamp( window, red, green, blue ) )
#else
	if( SDL_SetGammaRamp( red, green, blue ) )
#endif
		common->Warning( "Couldn't set gamma ramp: %s", SDL_GetError() );
}

/*
===================
GLimp_ExtensionPointer
===================
*/
/*
GLExtension_t GLimp_ExtensionPointer(const char *name) {
	assert(SDL_WasInit(SDL_INIT_VIDEO));

	return (GLExtension_t)SDL_GL_GetProcAddress(name);
}
*/

void GLimp_GrabInput( int flags )
{
	bool grab = flags & GRAB_ENABLE;
	
	if( grab && ( flags & GRAB_REENABLE ) )
		grab = false;
		
	if( flags & GRAB_SETSTATE )
		grabbed = grab;
		
	if( in_nograb.GetBool() )
		grab = false;
		
	if( !window )
	{
		common->Warning( "GLimp_GrabInput called without window" );
		return;
	}
	
#if SDL_VERSION_ATLEAST(2, 0, 0)
	// DG: disabling the cursor is now done once in GLimp_Init() because it should always be disabled
	
	// DG: check for GRAB_ENABLE instead of GRAB_HIDECURSOR because we always wanna hide it
	SDL_SetRelativeMouseMode( flags & GRAB_ENABLE ? SDL_TRUE : SDL_FALSE );
	SDL_SetWindowGrab( window, grab ? SDL_TRUE : SDL_FALSE );
#else
	// DG end
	SDL_WM_GrabInput( grab ? SDL_GRAB_ON : SDL_GRAB_OFF );
#endif
}

/*
====================
DumpAllDisplayDevices
====================
*/
void DumpAllDisplayDevices()
{
	common->DPrintf( "TODO: DumpAllDisplayDevices\n" );
}



class idSort_VidMode : public idSort_Quick< vidMode_t, idSort_VidMode >
{
public:
	int Compare( const vidMode_t& a, const vidMode_t& b ) const
	{
		int wd = a.width - b.width;
		int hd = a.height - b.height;
		int fd = a.displayHz - b.displayHz;
		return ( hd != 0 ) ? hd : ( wd != 0 ) ? wd : fd;
	}
};

// RB: resolutions supported by XreaL
static void FillStaticVidModes( idList<vidMode_t>& modeList )
{
	modeList.AddUnique( vidMode_t( 320,   240, 60 ) );
	modeList.AddUnique( vidMode_t( 400,   300, 60 ) );
	modeList.AddUnique( vidMode_t( 512,   384, 60 ) );
	modeList.AddUnique( vidMode_t( 640,   480, 60 ) );
	modeList.AddUnique( vidMode_t( 800,   600, 60 ) );
	modeList.AddUnique( vidMode_t( 960,   720, 60 ) );
	modeList.AddUnique( vidMode_t( 1024,  768, 60 ) );
	modeList.AddUnique( vidMode_t( 1152,  864, 60 ) );
	modeList.AddUnique( vidMode_t( 1280,  720, 60 ) );
	modeList.AddUnique( vidMode_t( 1280,  768, 60 ) );
	modeList.AddUnique( vidMode_t( 1280,  800, 60 ) );
	modeList.AddUnique( vidMode_t( 1280, 1024, 60 ) );
	modeList.AddUnique( vidMode_t( 1366,  768, 60 ) );
	modeList.AddUnique( vidMode_t( 1440,  900, 60 ) );
	modeList.AddUnique( vidMode_t( 1680, 1050, 60 ) );
	modeList.AddUnique( vidMode_t( 1600, 1200, 60 ) );
	modeList.AddUnique( vidMode_t( 1920, 1080, 60 ) );
	modeList.AddUnique( vidMode_t( 1920, 1200, 60 ) );
	modeList.AddUnique( vidMode_t( 2048, 1536, 60 ) );
	modeList.AddUnique( vidMode_t( 2560, 1600, 60 ) );
	
	modeList.SortWithTemplate( idSort_VidMode() );
}

/*
====================
R_GetModeListForDisplay
====================
*/
bool R_GetModeListForDisplay( const unsigned requestedDisplayNum, idList<vidMode_t>& modeList )
{
	assert( requestedDisplayNum >= 0 );
	
	modeList.Clear();
#if SDL_VERSION_ATLEAST(2, 0, 0)
	// DG: SDL2 implementation
	if (requestedDisplayNum >= SDL_GetNumVideoDisplays())
	{
		// requested invalid displaynum
		return false;
	}
	int numModes = SDL_GetNumDisplayModes(requestedDisplayNum);
	if (numModes > 0)
	{
		for (int i = 0; i < numModes; i++)
		{
			SDL_DisplayMode m;
			int ret = SDL_GetDisplayMode(requestedDisplayNum, i, &m);
			if (ret != 0)
			{
				common->Warning("Can't get video mode no %i, because of %s\n", i, SDL_GetError());
				continue;
			}
			if (m.h < 720) {
				continue;
			}

			vidMode_t mode;
			mode.width = m.w;
			mode.height = m.h;
			mode.displayHz = m.refresh_rate ? m.refresh_rate : 60; // default to 60 if unknown (0)
			modeList.AddUnique(mode);
		}
	}
		
	return modeList.Num() > 0;
	// DG end
	
#else // SDL 1
	
	// DG: SDL1 only knows of one display - some functions rely on
	// R_GetModeListForDisplay() returning false for invalid displaynum to iterate all displays
	if( requestedDisplayNum >= 1 )
	{
		return false;
	}
	// DG end
	
	const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
	if( videoInfo == NULL )
	{
		// DG: yes, this can actually fail, e.g. if SDL_Init( SDL_INIT_VIDEO ) wasn't called
		common->Warning( "Can't get Video Info, using default modes...\n" );
		FillStaticVidModes( modeList );
		return true;
	}
	
	SDL_Rect** modes = SDL_ListModes( videoInfo->vfmt, SDL_OPENGL | SDL_FULLSCREEN );
	
	if( !modes )
	{
		common->Warning( "Can't get list of available modes, using default ones...\n" );
		FillStaticVidModes( modeList );
		return true;
	}
	
	if( modes == ( SDL_Rect** ) - 1 )
	{
		common->Printf( "Display supports any resolution\n" );
		FillStaticVidModes( modeList );
		return true;
	}
	
	int numModes;
	for( numModes = 0; modes[numModes]; numModes++ );
	
	if( numModes > 1 )
	{
		for( int i = 0; i < numModes; i++ )
		{
			vidMode_t mode;
			mode.width =  modes[i]->w;
			mode.height =  modes[i]->h;
			mode.displayHz = 60; // FIXME;
			modeList.AddUnique( mode );
		}
	
		// sort with lowest resolution first
		modeList.SortWithTemplate( idSort_VidMode() );
	
		return true;
	}
	
	return false;
#endif
}

class idSort_int : public idSort_Quick< int, idSort_int >
{
public:
	int Compare(const int a, const int b) const
	{
		int diff = a - b;
		return diff;
	}
};

/*
====================
R_GetModeListForDisplay
====================
*/
bool R_GetRefreshListForDisplay(const unsigned requestedDisplayNum, idList<int>& refreshList)
{
	assert(requestedDisplayNum >= 0);

	refreshList.Clear();
#if SDL_VERSION_ATLEAST(2, 0, 0)
	// DG: SDL2 implementation
	if (requestedDisplayNum >= SDL_GetNumVideoDisplays())
	{
		// requested invalid displaynum
		return false;
	}

	int numModes = SDL_GetNumDisplayModes(requestedDisplayNum);
	if (numModes > 0)
	{
		for (int i = 0; i < numModes; i++)
		{
			SDL_DisplayMode m;
			int ret = SDL_GetDisplayMode(requestedDisplayNum, i, &m);
			if (ret != 0)
			{
				common->Warning("Can't get video mode no %i, because of %s\n", i, SDL_GetError());
				continue;
			}
			if (refreshList.Find(m.refresh_rate) != NULL) {
				continue;
			}
			if (m.h < 720) {
				continue;
			}

			refreshList.AddUnique(m.refresh_rate);
		}

		if (refreshList.Num() < 1)
		{
			common->Warning("Couldn't get a single video mode for display %i, using default ones..!\n", requestedDisplayNum);
			return false;
		}

		// sort with lowest resolution first
		refreshList.SortWithTemplate(idSort_int());
	}
	else
	{
		common->Warning("Can't get Video Info, using default modes...\n");
		if (numModes < 0)
		{
			common->Warning("Reason was: %s\n", SDL_GetError());
		}
		return false;
	}

	return true;
	// DG end

#else // SDL 1

	// DG: SDL1 only knows of one display - some functions rely on
	// R_GetModeListForDisplay() returning false for invalid displaynum to iterate all displays
	//if (requestedDisplayNum >= 1)
	//{
	//	return false;
	//}
	//// DG end

	//const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
	//if (videoInfo == NULL)
	//{
	//	// DG: yes, this can actually fail, e.g. if SDL_Init( SDL_INIT_VIDEO ) wasn't called
	//	common->Warning("Can't get Video Info, using default modes...\n");
	//	//FillStaticVidModes(modeList);
	//	return false;
	//}

	//SDL_Rect** modes = SDL_ListModes(videoInfo->vfmt, SDL_OPENGL | SDL_FULLSCREEN);

	//if (!modes)
	//{
	//	common->Warning("Can't get list of available modes, using default ones...\n");
	//	//FillStaticVidModes(modeList);
	//	return false;
	//}

	//if (modes == (SDL_Rect**)-1)
	//{
	//	common->Printf("Display supports any resolution\n");
	//	//FillStaticVidModes(modeList);
	//	return false;
	//}

	//int numModes;
	//for (numModes = 0; modes[numModes]; numModes++);

	//if (numModes > 1)
	//{
	//	for (int i = 0; i < numModes; i++)
	//	{
	//		vidMode_t mode;
	//		mode.width = modes[i]->w;
	//		mode.height = modes[i]->h;
	//		mode.displayHz = 60; // FIXME;
	//		modeList.AddUnique(mode);
	//	}

	//	// sort with lowest resolution first
	//	modeList.SortWithTemplate(idSort_VidMode());

	//	return true;
	//}

	return false;
#endif
}

bool R_GetScreenResolution(const unsigned displayNum, int& w, int& h, int& hz) {
#if SDL_VERSION_ATLEAST(2, 0, 0)
	SDL_DisplayMode current;
	if (SDL_GetCurrentDisplayMode(displayNum, &current) == 0) {
		w = current.w;
		h = current.h;
		hz = current.refresh_rate;
		return true;
	}
	return false;
#else
	return false;
#endif
}

/*
====================
Sys_ChangeTitle
====================
 */
 void Sys_ChangeTitle( const char* string )
 {
	 SDL_SetWindowTitle(window, string);
 }

#ifdef USE_OPENXR
void* GetOpenXRGraphicsBinding()
{
	switch(windowInfo.subsystem) {
		case SDL_SYSWM_X11: {
			x11Binding = {XR_TYPE_GRAPHICS_BINDING_OPENGL_XLIB_KHR};
			x11Binding.xDisplay = windowInfo.info.x11.display;
			x11Binding.visualid = windowInfo.info.x11.window;
			return &x11Binding;
		}
		case SDL_SYSWM_WAYLAND: {
			waylandBinding = {XR_TYPE_GRAPHICS_BINDING_OPENGL_WAYLAND_KHR};
			waylandBinding.display = windowInfo.info.wl.display;
			return &waylandBinding;
		}

	}
	return NULL;
}
#endif