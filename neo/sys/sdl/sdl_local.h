/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 dhewg (dhewm3)

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).

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
#ifndef __SDL_LOCAL_H__
#define __SDL_LOCAL_H__
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
// glimp.cpp
const int GRAB_ENABLE		= ( 1 << 0 );
const int GRAB_REENABLE		= ( 1 << 1 );
const int GRAB_HIDECURSOR	= ( 1 << 2 );
const int GRAB_SETSTATE		= ( 1 << 3 );

#ifdef USE_OPENXR
#define XR_USE_GRAPHICS_API_OPENGL
#define XR_USE_PLATFORM_WAYLAND
#ifndef XR_USE_PLATFORM_XLIB
#define XR_USE_PLATFORM_XLIB
#endif
#include <GL/glx.h>
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>
#endif

//GK: Keep that here in order to be used by more than one places
#if SDL_VERSION_ATLEAST(2, 0, 0)
static SDL_Window* window = NULL;
static SDL_SysWMinfo windowInfo; //GK: Keep System Info for the Window SDL has created
#endif

void GLimp_GrabInput( int flags );

char*	Sys_ConsoleInput();

#ifdef USE_OPENXR
static XrGraphicsBindingOpenGLXlibKHR x11Binding {};
static XrGraphicsBindingOpenGLWaylandKHR waylandBinding {};
void* GetOpenXRGraphicsBinding();
#endif

void Sys_QueEvent( sysEventType_t type, int value, int value2, int ptrLength, void *ptr, int inputDeviceNum );

#endif
