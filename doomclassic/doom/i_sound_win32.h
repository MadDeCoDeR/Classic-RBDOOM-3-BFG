/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company. 

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").  

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#ifndef __I_SOUND_XA2__
#define __I_SOUND_XA2__

#include "doomdef.h"
//
//// UNIX hack, to be removed.
//#ifdef SNDSERV
//#include <stdio.h>
//extern FILE* sndserver;
//extern char* sndserver_filename;
//#endif
//
#include "doomstat.h"
#include "sounds.h"

// Init at program start...
void I_InitSoundXA2();
void I_InitSoundHardwareXA2( int numOutputChannels_, int channelMask_ );

// ... update sound buffer and audio device at runtime...
void I_UpdateSoundXA2(void);
void I_SubmitSoundXA2(void);

// ... shut down and relase at program termination.
void I_ShutdownSoundXA2(void);
void I_ShutdownSoundHardwareXA2();

//
//  SFX I/O
//

// Initialize channels?
void I_SetChannelsXA2();

// Get raw data lump index for sound descriptor.
int I_GetSfxLumpNumXA2 (sfxinfo_t* sfxinfo );


// Starts a sound in a particular sound channel.
int I_StartSoundXA2( int id, mobj_t *origin, mobj_t *listener_origin, int vol, int pitch, int priority );


// Stops a sound channel.
void I_StopSoundXA2(int handle, int player = -1);

// Called by S_*() functions
//  to see if a channel is still playing.
// Returns 0 if no longer playing, 1 if playing.
int I_SoundIsPlayingXA2(int handle);

// Updates the volume, separation,
//  and pitch of a sound channel.
void I_UpdateSoundParamsXA2( int handle, int vol, int sep, int pitch );

void I_SetSfxVolumeXA2( int );
//
//  MUSIC I/O
//
void I_InitMusicXA2(void);
void I_ShutdownMusicXA2(void);
// Volume.
void I_SetMusicVolumeXA2(int volume);
// PAUSE game handling.
void I_PauseSongXA2(int handle);
void I_ResumeSongXA2(int handle);
// Registers a song handle to song data.
int I_RegisterSongXA2(void *data, int length);
// Called by anything that wishes to start music.
//  plays a song, and when the song is done,
//  starts playing it again in an endless loop.
// Horrible thing to do, considering.
void I_PlaySongXA2( const char *songname, int looping );
// Stops a song over 3 seconds.
void I_StopSongXA2(int handle);
// See above (register), then think backwards
void I_UnRegisterSongXA2(int handle);
// Update Music (XMP), check for notifications
void I_UpdateMusicXA2(void);

void I_ProcessSoundEventsXA2();

#endif

