/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013 Felix Rueegg

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

#include "Precompiled.h"
#include "globaldata.h"

//
// DESCRIPTION:
//	System interface for sound.
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <sys/types.h>
#include <fcntl.h>
// Timer stuff. Experimental.
#include <time.h>
#include <signal.h>
#include "z_zone.h"
#include "i_system.h"
#include "i_sound.h"
#include "i_sound_openal.h"
#include "m_argv.h"
#include "m_misc.h"
#include "w_wad.h"
#include "d_main.h"
#include "doomdef.h"
#include "libs/timidity/timidity.h"
#include "libs/timidity/controls.h"
#include "sound/snd_local.h"
#include "sound/AVD.h"

//#pragma warning ( disable : 4244 ) //GK: No longer thrown ???

#define SFX_RATE		11050
#define SFX_SAMPLETYPE		AL_FORMAT_MONO8

#define MIDI_CHANNELS		2
#define MIDI_RATE		22050
#define MIDI_SAMPLETYPE		AL_FORMAT_STEREO8
#define MIDI_FORMAT		AUDIO_U8
#define MIDI_FORMAT_BYTES	1

ALuint		alMusicSourceVoice;
ALuint		alMusicBuffer;

extern MidiSong* doomMusic;
extern byte* musicBuffer;
extern int		totalBufferSize;

ALenum av_sample;
int av_rate;
bool use_avi;

extern bool		waitingForMusic;
extern bool		musicReady;
ALuint clslot=0;
ALuint clmusslot = 0;
typedef struct {
	float x;
	float y;
	float z;
} vec3_t;

typedef struct {
	vec3_t OrientTop;
	vec3_t OrientFront;
	vec3_t Position;
} doomListener_t;

typedef struct tagActiveSoundAL_t {
	ALuint alSourceVoice;
	int id;
	int valid;
	int start;
	int player;
	bool localSound;
	mobj_t *originator;
	int rate; //GK: Keep record of rate and sample for link sfx
	ALenum sample;
} activeSoundAL_t;

// array of all the possible sounds
// in split screen we only process the loudest sound of each type per frame
extern soundEvent_t soundEvents[128];
extern int PLAYERCOUNT;

// Source voice settings for all sound effects
const ALfloat		SFX_MAX_DISTANCE = 1200.f;
const ALfloat		SFX_REFERENCE_DISTANCE = 100.f;
const ALfloat		SFX_ROLLOFF_FACTOR = 0.2f;

extern const float		GLOBAL_VOLUME_MULTIPLIER;

extern float			x_SoundVolume;
extern float			x_MusicVolume;

// The actual lengths of all sound effects.
static int 		lengths[NUMSFX];
ALuint			alBuffers[NUMSFX];
activeSoundAL_t		activeSounds[NUM_SOUNDBUFFERS] = {0};

extern int			S_initialized;
extern bool		Music_initialized;
static bool		soundHardwareInitialized = false;
static int		numOutputChannels = 0;
static int		channelMask = 0;

doomListener_t		doom_Listener;

extern idCVar S_museax;

void			I_InitSoundChannelAL( int channel, int numOutputChannels_ );

/*
======================
getsfx
======================
*/
// This function loads the sound data from the WAD lump,
// for single sound.
//
void* getsfx ( const char* sfxname, int* len )
{
	unsigned char*      sfx;
	unsigned char*	    sfxmem;
	int                 size;
	char                name[20];
	int                 sfxlump;
	//float               scale = 1.0f;

	// Get the sound data from the WAD
	sprintf( name, "ds%s", sfxname );

	// Scale down the plasma gun, it clips
	//if ( strcmp( sfxname, "plasma" ) == 0 ) {
	//	scale = 0.75f;
	//}
	//if ( strcmp( sfxname, "itemup" ) == 0 ) {
	//	scale = 1.333f;
	//}

	// If sound requested is not found in current WAD, use pistol as default
	if ( W_CheckNumForName( name ) == -1 )
		sfxlump = W_GetNumForName( "dspistol" );
	else
		sfxlump = W_GetNumForName( name );

	// Sound lump headers are 8 bytes.
	const int SOUND_LUMP_HEADER_SIZE_IN_BYTES = 8;

	size = W_LumpLength( sfxlump ) - SOUND_LUMP_HEADER_SIZE_IN_BYTES;

	sfx = (unsigned char*)W_CacheLumpNum( sfxlump, PU_CACHE_SHARED );
	byte h = sfx[0];
	/*const*/ unsigned char * sfxSampleStart = sfx + SOUND_LUMP_HEADER_SIZE_IN_BYTES;
	if (h != 3) { //GK: Remember the magic number
		sfxSampleStart = sfx;
		DecodeALAudio(&sfxSampleStart, &size, &av_rate, &av_sample);
	}
	else {
		memcpy(&av_rate,sfx+2,sizeof(short));
		av_sample = SFX_SAMPLETYPE;
	}
	// Allocate from zone memory.
	//sfxmem = (float*)DoomLib::Z_Malloc( size*(sizeof(float)), PU_SOUND_SHARED, 0 );
	sfxmem = (unsigned char*)malloc( size * sizeof(unsigned char) );
	
	// Now copy, and convert to Xbox360 native float samples, do initial volume ramp, and scale
	for ( int i = 0; i < size; i++ ) {
		sfxmem[i] = sfxSampleStart[i];// * scale;
	}
	
	// Remove the cached lump.
	Z_Free( sfx );
	
	// Set length.
	*len = size;
	
	// Return allocated padded data.
	return (void *) (sfxmem);
}

/*
======================
I_SetChannels
======================
*/
void I_SetChannelsAL()
{
	// Original Doom set up lookup tables here
}

/*
======================
I_SetSfxVolume
======================
*/
void I_SetSfxVolumeAL( int volume )
{
	x_SoundVolume = ((float)volume / 15.f) * GLOBAL_VOLUME_MULTIPLIER;
}

/*
======================
I_GetSfxLumpNum
======================
*/
//
// Retrieve the raw data lump index
//  for a given SFX name.
//
int I_GetSfxLumpNumAL( sfxinfo_t* sfx )
{
	char namebuf[9];
	sprintf( namebuf, "ds%s", sfx->name );
	return W_GetNumForName( namebuf );
}

/*
======================
I_StartSound2
======================
*/
// Starting a sound means adding it
//  to the current list of active sounds
//  in the internal channels.
// As the SFX info struct contains
//  e.g. a pointer to the raw data,
//  it is ignored.
// As our sound handling does not handle
//  priority, it is ignored.
// Pitching (that is, increased speed of playback) is set
//
int I_StartSound2 ( int id, int player, mobj_t *origin, mobj_t *listener_origin, int pitch, int priority )
{
	if ( !soundHardwareInitialized || id == 0 ) {
		return id;
	}
	
	int i;
	activeSoundAL_t* sound = 0;
	int oldest = 0, oldestnum = -1;
	
	// these id's should not overlap
	if ( id == sfx_sawup || id == sfx_sawidl || id == sfx_sawful || id == sfx_sawhit || id == sfx_stnmov ) {
		// Loop all channels, check.
		for ( i = 0; i < NUM_SOUNDBUFFERS; i++ )
		{
			sound = &activeSounds[i];
			
			if ( sound->valid && ( sound->id == id && sound->player == player ) ) {
				I_StopSoundAL( sound->id, player );
				break;
			}
		}
	}
	
	// find a valid channel, or one that has finished playing
	for ( i = 0; i < NUM_SOUNDBUFFERS; i++ ) {
		sound = &activeSounds[i];
		
		if ( !sound->valid )
			break;
		
		if ( !oldest || oldest > sound->start ) {
			oldestnum = i;
			oldest = sound->start;
		}
		
		ALint sourceState;
		alGetSourcei( sound->alSourceVoice, AL_SOURCE_STATE, &sourceState );
		if ( sourceState == AL_STOPPED ) {
			break;
		}
	}
	
	// none found, so use the oldest one
	if ( i == NUM_SOUNDBUFFERS ) {
		i = oldestnum;
		sound = &activeSounds[i];
	}
	sound = &activeSounds[id]; //GK: use the coresponding channel of this sfx
	alSourceStop( sound->alSourceVoice );
	
	// Attach the source voice to the correct buffer
	//if ( sound->id != id ) { //GK: Why ???
		alSourcei( sound->alSourceVoice, AL_BUFFER, 0 );
		alSourcei( sound->alSourceVoice, AL_BUFFER, alBuffers[id] );
	//}
	
	// Set the source voice volume
	alSourcef( sound->alSourceVoice, AL_GAIN, x_SoundVolume );
	
	// Set the source voice pitch
	alSourcef( sound->alSourceVoice, AL_PITCH, 1 + ((float)pitch-128.f)/95.f );
	
	// Set the source voice position
	ALfloat x = 0.f;
	ALfloat y = 0.f;
	ALfloat z = 0.f;
	if ( origin ) {
		if ( origin == listener_origin ) {
			sound->localSound = true;
		} else {
			sound->localSound = false;
			x = (ALfloat)(origin->x >> FRACBITS);
			z = (ALfloat)(origin->y >> FRACBITS);
			y = (ALfloat)(origin->z >> FRACBITS);
		}
	} else {
		sound->localSound = true;
	}
	if ( sound->localSound ) {
		x = doom_Listener.Position.x;
		z = doom_Listener.Position.z;
		y = doom_Listener.Position.y;
	}
	alSource3f( sound->alSourceVoice, AL_POSITION, x, y, z );
	//GK: Set the EFX in the last moment
	alSource3i(sound->alSourceVoice, AL_AUXILIARY_SEND_FILTER, AL_EFFECTSLOT_NULL, 2, AL_FILTER_NULL);
	if (alIsEffectRef((ALuint)::g->clEAX) && ::g->clEAX > 0) {
		alSource3i(sound->alSourceVoice, AL_AUXILIARY_SEND_FILTER, clslot, 2, AL_FILTER_NULL);
	}
	alSourcePlay( sound->alSourceVoice );
	
	// Set id, and start time
	sound->id = id;
	sound->start = ::g->gametic;
	sound->valid = 1;
	sound->player = player;
	sound->originator = origin;
	
	return id;
}

/*
======================
I_ProcessSoundEvents
======================
*/
void I_ProcessSoundEventsAL( void )
{
	for( int i = 0; i < 128; i++ ) {
		if( soundEvents[i].pitch ) {
			I_StartSound2( i, soundEvents[i].player, soundEvents[i].originator, soundEvents[i].listener,
					soundEvents[i].pitch, soundEvents[i].priority );
		}
	}
	memset( soundEvents, 0, sizeof( soundEvents ) );
}

/*
======================
I_StartSound
======================
*/
int I_StartSoundAL ( int id, mobj_t *origin, mobj_t *listener_origin, int vol, int pitch, int priority )
{
	// only allow player 0s sounds in intermission and finale screens
	if( (::g->gamestate != GS_LEVEL && ::g->gamestate != GS_DEMOLEVEL) && DoomLib::GetPlayer() != 0 ) {
		return 0;
	}
	
	// if we're only one player or we're trying to play the chainsaw sound, do it normal
	// otherwise only allow one sound of each type per frame
	if( PLAYERCOUNT == 1 || id == sfx_sawup || id == sfx_sawidl || id == sfx_sawful || id == sfx_sawhit ) {
		return I_StartSound2( id, ::g->consoleplayer, origin, listener_origin, pitch, priority );
	} else {
		if( soundEvents[ id ].vol < vol ) {
			soundEvents[ id ].player = DoomLib::GetPlayer();
			soundEvents[ id ].pitch = pitch;
			soundEvents[ id ].priority = priority;
			soundEvents[ id ].vol = vol;
			soundEvents[ id ].originator = origin;
			soundEvents[ id ].listener = listener_origin;
		}
		return id;
	}
}

/*
======================
I_StopSound
======================
*/
void I_StopSoundAL ( int handle, int player )
{
	// You need the handle returned by StartSound.
	// Would be looping all channels,
	// tracking down the handle,
	// and setting the channel to zero.
	int i;
	activeSoundAL_t* sound = 0;
	
	for ( i = 0; i < NUM_SOUNDBUFFERS; ++i ) {
		sound = &activeSounds[i];
		if ( !sound->valid || sound->id != handle || (player >= 0 && sound->player != player) )
			continue;
		break;
	}
	
	if ( i == NUM_SOUNDBUFFERS )
		return;
	
	// Stop the sound
	alSourceStop( sound->alSourceVoice );
	
	sound->valid = 0;
	sound->player = -1;
}

/*
======================
I_SoundIsPlaying
======================
*/
int I_SoundIsPlayingAL( int handle )
{
	if ( !soundHardwareInitialized ) {
		return 0;
	}
	
	int i;
	activeSoundAL_t* sound;
	
	for ( i = 0; i < NUM_SOUNDBUFFERS; ++i ) {
		sound = &activeSounds[i];
		if ( !sound->valid || sound->id != handle )
			continue;
		
		ALint sourceState;
		alGetSourcei( sound->alSourceVoice, AL_SOURCE_STATE, &sourceState );
		if ( sourceState == AL_PLAYING ) {
			return 1;
		}
	}
	
	return 0;
}

/*
======================
I_UpdateSound
======================
*/
// Update listener position and go through all the
// channels and update sound positions.
void I_UpdateSoundAL( void )
{
	if ( !soundHardwareInitialized ) {
		return;
	}
	
	// Update listener orientation and position
	mobj_t *playerObj = ::g->players[0].mo;
	if ( playerObj ) {
		angle_t	pAngle = playerObj->angle;
		fixed_t fx, fz;
		
		pAngle >>= ANGLETOFINESHIFT;
		
		fx = finecosine[pAngle];
		fz = finesine[pAngle];
		
		doom_Listener.OrientFront.x = (float)(fx) / 65535.f;
		doom_Listener.OrientFront.y = 0.f;
		doom_Listener.OrientFront.z = (float)(fz) / 65535.f;
		doom_Listener.Position.x = (float)(playerObj->x >> FRACBITS);
		doom_Listener.Position.y = 0.f;
		doom_Listener.Position.z = (float)(playerObj->y >> FRACBITS);
	} else {
		doom_Listener.OrientFront.x = 0.f;
		doom_Listener.OrientFront.y = 0.f;
		doom_Listener.OrientFront.z = 1.f;
		
		doom_Listener.Position.x = 0.f;
		doom_Listener.Position.y = 0.f;
		doom_Listener.Position.z = 0.f;
	}
	
	ALfloat listenerOrientation[] = { doom_Listener.OrientFront.x, doom_Listener.OrientFront.y,
		doom_Listener.OrientFront.z, doom_Listener.OrientTop.x, doom_Listener.OrientTop.y,
		doom_Listener.OrientTop.z };
	alListenerfv( AL_ORIENTATION, listenerOrientation );
	alListener3f( AL_POSITION, doom_Listener.Position.x, doom_Listener.Position.y, doom_Listener.Position.z );
	
	// Update playing source voice positions
	int i;
	activeSoundAL_t* sound;
	for ( i=0; i < NUM_SOUNDBUFFERS; i++ ) {
		sound = &activeSounds[i];
		
		if ( !sound->valid ) {
			continue;
		}
		
		ALint sourceState;
		alGetSourcei( sound->alSourceVoice, AL_SOURCE_STATE, &sourceState );
		if ( sourceState == AL_PLAYING ) {
			if ( sound->localSound ) {
				alSource3f( sound->alSourceVoice, AL_POSITION, doom_Listener.Position.x,
						doom_Listener.Position.y, doom_Listener.Position.z );
			} else {
				ALfloat x = (ALfloat)(sound->originator->x >> FRACBITS);
				ALfloat y = (ALfloat)(sound->originator->z >> FRACBITS);
				ALfloat z = (ALfloat)(sound->originator->y >> FRACBITS);
				
				alSource3f( sound->alSourceVoice, AL_POSITION, x, y, z );
			}
		}
	}
}

/*
======================
I_UpdateSoundParams
======================
*/
void I_UpdateSoundParamsAL( int handle, int vol, int sep, int pitch )
{
}

/*
======================
I_ShutdownSound
======================
*/
void I_ShutdownSoundAL( void )
{
	//int done = 0;
	int i;
	
	if ( S_initialized ) {
		// Stop all sounds
		for ( i = 0; i < NUM_SOUNDBUFFERS; i++ ) {
			activeSoundAL_t * sound = &activeSounds[i];
			
			if ( !sound ) {
				continue;
			}
			
			I_StopSoundAL( sound->id, 0 );
		}
		
		// Free allocated sound memory
		for ( i = 1; i < NUMSFX; i++ ) {
			alDeleteBuffers(1, &alBuffers[i]); //GK: Restart the AL buffers in order to clear them from previous sfx
			alGenBuffers(1, &alBuffers[i]);
			if ( S_sfx[i].data && !(S_sfx[i].link) ) {
				free( S_sfx[i].data );
				lengths[i] = 0;
			}
		}
	}
	
	I_StopSongAL( 0 );
	ResetSfx(); //GK: At last I found where I can reset the dehacked sound editor without screwing over the game
	S_initialized = 0;
}

/*
======================
I_InitSoundHardware

Called from the tech4x initialization code. Sets up Doom classic's
sound channels.
======================
*/
void I_InitSoundHardwareAL( int numOutputChannels_, int channelMask_ )
{
	::numOutputChannels = numOutputChannels_;
	::channelMask = channelMask_;
	I_InitMusicAL(); //GK: Just to be sure that music will play
	// Initialize source voices
	for ( int i = 0; i < NUM_SOUNDBUFFERS; i++ ) {
		I_InitSoundChannelAL( i, numOutputChannels );
	}

	// Create OpenAL buffers for all sounds
	for ( int i = 1; i < NUMSFX; i++ ) {
		alGenBuffers( (ALuint)1, &alBuffers[i] );
		if (S_sfx[i].data) {
			alBufferData(alBuffers[i], activeSounds[i].sample, (byte*)S_sfx[i].data, lengths[i], activeSounds[i].rate);
		}
	}
	
	alGenAuxiliaryEffectSlotsRef(1, &clslot);
	soundHardwareInitialized = true;
}

/*
======================
I_ShutdownSoundHardware

Called from the tech4x shutdown code. Tears down Doom classic's
sound channels.
======================
*/
void I_ShutdownSoundHardwareAL()
{
	soundHardwareInitialized = false;
	
	I_ShutdownMusicAL();
	
	// Delete all source voices
	for ( int i = 0; i < NUM_SOUNDBUFFERS; ++i ) {
		activeSoundAL_t * sound = &activeSounds[i];
		
		if ( !sound ) {
			continue;
		}
		
		if ( sound->alSourceVoice ) {
			alSourceStop( sound->alSourceVoice );
			alSourcei( sound->alSourceVoice, AL_BUFFER, 0 );
			alDeleteSources( 1, &sound->alSourceVoice );
			if (CheckALErrors() == AL_NO_ERROR) {
				sound->alSourceVoice = 0;
			}
		}
	}

	// Delete OpenAL buffers for all sounds
	alDeleteBuffers(NUMSFX, alBuffers );
	
	if (alIsAuxiliaryEffectSlotRef(clslot)) {
		alDeleteAuxiliaryEffectSlotsRef(1,&clslot);
		clslot = 0;
	}
	if (alIsAuxiliaryEffectSlotRef(clmusslot)) {
		alDeleteAuxiliaryEffectSlotsRef(1, &clmusslot);
		clmusslot = 0;
	}
}

/*
======================
I_InitSoundChannel
======================
*/
void I_InitSoundChannelAL( int channel, int numOutputChannels_ )
{
	activeSoundAL_t *soundchannel = &activeSounds[ channel ];
	
	alGenSources( (ALuint)1, &soundchannel->alSourceVoice );
	
	alSource3f( soundchannel->alSourceVoice, AL_VELOCITY, 0.f, 0.f, 0.f );
	alSourcef( soundchannel->alSourceVoice, AL_LOOPING, AL_FALSE );
	alSourcef( soundchannel->alSourceVoice, AL_MAX_DISTANCE, SFX_MAX_DISTANCE );
	alSourcef( soundchannel->alSourceVoice, AL_REFERENCE_DISTANCE, SFX_REFERENCE_DISTANCE );
	alSourcef( soundchannel->alSourceVoice, AL_ROLLOFF_FACTOR, SFX_ROLLOFF_FACTOR );
}

/*
======================
I_InitSound
======================
*/
void I_InitSoundAL()
{
	if ( S_initialized == 0 ) {
		// Set up listener parameters
		doom_Listener.OrientFront.x	= 0.f;
		doom_Listener.OrientFront.y	= 0.f;
		doom_Listener.OrientFront.z	= 1.f;
		
		doom_Listener.OrientTop.x	= 0.f;
		doom_Listener.OrientTop.y	= -1.f;
		doom_Listener.OrientTop.z	= 0.f;
		
		doom_Listener.Position.x	= 0.f;
		doom_Listener.Position.y	= 0.f;
		doom_Listener.Position.z	= 0.f;
		
		for ( int i = 1; i < NUMSFX; i++ ) {
			// Alias? Example is the chaingun sound linked to pistol.
			if ( !S_sfx[i].link ) {
				// Load data from WAD file.
				S_sfx[i].data = getsfx( S_sfx[i].name, &lengths[i] );
				activeSounds[i].rate = av_rate;
				activeSounds[i].sample = av_sample;
			} else {
				// Previously loaded already?
				S_sfx[i].data = S_sfx[i].link->data;
				int pointer = (S_sfx[i].link-S_sfx) / sizeof(sfxinfo_t);
				lengths[i] = lengths[pointer]; //GK: Get Linked sfx rate, sample and length
				av_rate = activeSounds[pointer].rate;
				av_sample = activeSounds[pointer].sample;
			}
			if ( S_sfx[i].data ) {
				alBufferData( alBuffers[i], av_sample, (byte*)S_sfx[i].data, lengths[i], av_rate );
			}
		}
		::g->clslot = clslot;
		S_initialized = 1;
	}
}

/*
======================
I_SubmitSound
======================
*/
void I_SubmitSoundAL( void )
{
	// Only do this for player 0, it will still handle positioning
	//		for other players, but it can't be outside the game
	//		frame like the soundEvents are.
	if ( DoomLib::GetPlayer() == 0 ) {
		// Do 3D positioning of sounds
		I_UpdateSoundAL();
		
		// Change music if required
		I_UpdateMusicAL();
	}
}


// =========================================================
// =========================================================
// Background Music
// =========================================================
// =========================================================

/*
======================
I_SetMusicVolume
======================
*/
void I_SetMusicVolumeAL( int volume )
{
	x_MusicVolume = (float)volume / 15.f;
}

/*
======================
I_InitMusic
======================
*/
void I_InitMusicAL( void )
{
	if ( !Music_initialized ) {
		// Initialize Timidity
		Timidity_Init( MIDI_RATE, MIDI_FORMAT, MIDI_CHANNELS, MIDI_RATE, "classicmusic/gravis.cfg" );
		
		if (!soundSystemLocal.needsRestart) {
			musicBuffer = NULL;
			totalBufferSize = 0;
			waitingForMusic = false;
			musicReady = false;
		}
		else {
			waitingForMusic = true;
			musicReady = true;
		}
		
		
		alGenSources( (ALuint)1, &alMusicSourceVoice );
		
		alSourcef( alMusicSourceVoice, AL_PITCH, 1.f );
		alSourcef( alMusicSourceVoice, AL_LOOPING, AL_TRUE );
		
		alGenBuffers( (ALuint)1, &alMusicBuffer );
		//GK: Set default preset for music in order to level it's volume to the levels of the reverbed sfxes
		alGenAuxiliaryEffectSlotsRef(1, &clmusslot);
		EFXEAXREVERBPROPERTIES voicereverb = EFX_REVERB_PRESET_MOOD_HELL;
		EFXEAXREVERBPROPERTIES* voicereverb2 = &voicereverb;
		ALuint EFX;
		alGenEffectsRef(1, &EFX);
		alEffectiRef(EFX, AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB);
		alEffectfRef(EFX, AL_EAXREVERB_DENSITY, voicereverb2->flDensity);
		alEffectfRef(EFX, AL_EAXREVERB_DIFFUSION, voicereverb2->flDiffusion);
		alEffectfRef(EFX, AL_EAXREVERB_GAIN, voicereverb2->flGain);
		alEffectfRef(EFX, AL_EAXREVERB_GAINHF, voicereverb2->flGainHF);
		alEffectfRef(EFX, AL_EAXREVERB_GAINLF, voicereverb2->flGainLF);
		alEffectfRef(EFX, AL_EAXREVERB_DECAY_TIME, voicereverb2->flDecayTime);
		alEffectfRef(EFX, AL_EAXREVERB_DECAY_HFRATIO, voicereverb2->flDecayHFRatio);
		alEffectfRef(EFX, AL_EAXREVERB_DECAY_LFRATIO, voicereverb2->flDecayLFRatio);
		alEffectfRef(EFX, AL_EAXREVERB_REFLECTIONS_GAIN, voicereverb2->flReflectionsGain);
		alEffectfRef(EFX, AL_EAXREVERB_REFLECTIONS_DELAY, voicereverb2->flReflectionsDelay);
		alEffectfvRef(EFX, AL_EAXREVERB_REFLECTIONS_PAN, voicereverb2->flReflectionsPan);
		alEffectfRef(EFX, AL_EAXREVERB_LATE_REVERB_GAIN, voicereverb2->flLateReverbGain);
		alEffectfRef(EFX, AL_EAXREVERB_LATE_REVERB_DELAY, voicereverb2->flLateReverbDelay);
		alEffectfvRef(EFX, AL_EAXREVERB_LATE_REVERB_PAN, voicereverb2->flLateReverbPan);
		alEffectfRef(EFX, AL_EAXREVERB_ECHO_TIME, voicereverb2->flEchoTime);
		alEffectfRef(EFX, AL_EAXREVERB_ECHO_DEPTH, voicereverb2->flEchoDepth);
		alEffectfRef(EFX, AL_EAXREVERB_MODULATION_TIME, voicereverb2->flModulationTime);
		alEffectfRef(EFX, AL_EAXREVERB_MODULATION_DEPTH, voicereverb2->flModulationDepth);
		alEffectfRef(EFX, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, voicereverb2->flAirAbsorptionGainHF);
		alEffectfRef(EFX, AL_EAXREVERB_HFREFERENCE, voicereverb2->flHFReference);
		alEffectfRef(EFX, AL_EAXREVERB_LFREFERENCE, voicereverb2->flLFReference);
		alEffectfRef(EFX, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, voicereverb2->flRoomRolloffFactor);
		alEffectiRef(EFX, AL_EAXREVERB_DECAY_HFLIMIT, voicereverb2->iDecayHFLimit);
		alAuxiliaryEffectSlotiRef(clmusslot, AL_EFFECTSLOT_EFFECT, EFX);
		alDeleteEffectsRef(1, &EFX);
		
		Music_initialized = true;
	}
}

/*
======================
I_ShutdownMusic
======================
*/
void I_ShutdownMusicAL( void )
{
	if ( Music_initialized ) {
		if ( alIsSource(alMusicSourceVoice) ) {
			I_StopSongAL( 0 );
			alSourcei( alMusicSourceVoice, AL_BUFFER, 0 );
			alDeleteSources( 1, &alMusicSourceVoice );
			if (CheckALErrors() == AL_NO_ERROR) {
				alMusicSourceVoice = 0;
			}
		}
		
		if ( alMusicBuffer ) {
			alDeleteBuffers( 1, &alMusicBuffer );
		}
		
		if ( musicBuffer && !soundSystemLocal.needsRestart) {
			free( musicBuffer );
			musicBuffer = NULL;
		}
		
		Timidity_Shutdown();
	}

	if (alIsAuxiliaryEffectSlotRef(clmusslot)) {
		alDeleteAuxiliaryEffectSlotsRef(1, &clmusslot);
	}
	
	if (!soundSystemLocal.needsRestart) {
		totalBufferSize = 0;
		waitingForMusic = false;
		musicReady = false;
	}
	
	Music_initialized = false;
}

int Mus2Midi(unsigned char* bytes, unsigned char* out, int* len);

namespace {
	const int MaxMidiConversionSize = 1024 * 1024;
	unsigned char midiConversionBuffer[MaxMidiConversionSize];
}

/*
======================
I_LoadSong
======================
*/
bool I_LoadSong( const char * songname )
{
	idStr lumpName = "d_";
	lumpName += static_cast< const char * >( songname );
	if (alMusicBuffer) {
		alDeleteBuffers(1, &alMusicBuffer);
	}
	alGenBuffers((ALuint)1, &alMusicBuffer);
	
	unsigned char * musFile = static_cast< unsigned char * >( W_LoadLumpName( lumpName.c_str()/*, PU_STATIC_SHARED*/ ) );
	/*Z_Free(lumpcache[W_GetNumForName(lumpName.c_str())]);
	Z_FreeMemory();*/
	
	int length = 0;
	//GK: Capture it's return value and use it to determine if the file is mus or not
	int res = Mus2Midi(musFile, midiConversionBuffer, &length);
	int mus_size = W_LumpLength(W_CheckNumForName(lumpName.c_str()));
	if (res == 0) {
		//GK:if not mus file load it raw
		doomMusic = Timidity_LoadSongMem(musFile, mus_size);
	}
	else {
		doomMusic = Timidity_LoadSongMem(midiConversionBuffer, length);
	}
	
	if ( doomMusic ) {
		musicBuffer = (byte *)malloc( MIDI_CHANNELS * MIDI_FORMAT_BYTES * doomMusic->samples );
		totalBufferSize = doomMusic->samples * MIDI_CHANNELS * MIDI_FORMAT_BYTES;
		Timidity_Start( doomMusic );
		
		int rc = RC_NO_RETURN_VALUE;
		int num_bytes = 0;
		int offset = 0;
		
		do {
			rc = Timidity_PlaySome( musicBuffer + offset, MIDI_RATE, &num_bytes );
			offset += num_bytes;
		} while ( rc != RC_TUNE_END );
		
		Timidity_Stop();
		Timidity_FreeSong( doomMusic );
		free(lumpcache[W_GetNumForName(lumpName.c_str())]);
		lumpcache[W_GetNumForName(lumpName.c_str())] = NULL;
		use_avi = false;
	}
	else {
			use_avi = DecodeALAudio(&musFile,&mus_size,&av_rate,&av_sample); //GK: Simplified
			if (use_avi) {
				free(lumpcache[W_GetNumForName(lumpName.c_str())]);
				lumpcache[W_GetNumForName(lumpName.c_str())] = NULL;
				totalBufferSize = mus_size;
				musicBuffer = musFile;
			}
		}
	
	musicReady = true;
	return true;
}

/*
======================
I_PlaySong
======================
*/
void I_PlaySongAL( const char *songname, int looping )
{
	if ( !Music_initialized ) {
		return;
	}
	
	I_StopSongAL( 0 );
	
	// Clear old state
	if ( musicBuffer ) {
		free( musicBuffer );
		musicBuffer = NULL;
	}
	
	musicReady = false;
	I_LoadSong( songname );
	waitingForMusic = true;
	
	if ( DoomLib::GetPlayer() >= 0 ) {
		//GK: Did they even know how to use openAL ??
		alSourcei(alMusicSourceVoice, AL_LOOPING, looping);
	}
}

/*
======================
I_UpdateMusic
======================
*/
void I_UpdateMusicAL( void )
{
	if ( !Music_initialized ) {
		return;
	}

	alSource3i(alMusicSourceVoice, AL_AUXILIARY_SEND_FILTER, AL_EFFECTSLOT_NULL, 3, AL_FILTER_NULL);
	if ((alIsEffectRef((ALuint)::g->clEAX) && ::g->clEAX > 0) || S_museax.GetBool()) {
		alSource3i(alMusicSourceVoice, AL_AUXILIARY_SEND_FILTER, clmusslot, 3, AL_FILTER_NULL);
	}

	if ( alMusicSourceVoice ) {
		// Set the volume GK: and the music Reverb
		alSourcef( alMusicSourceVoice, AL_GAIN, x_MusicVolume * GLOBAL_VOLUME_MULTIPLIER );
		
	}
	
	if ( waitingForMusic ) {
		if ( musicReady && alMusicSourceVoice ) {
			if ( musicBuffer ) {

				alSourcei( alMusicSourceVoice, AL_BUFFER, 0 );
				
				if (!use_avi) {
					alBufferData(alMusicBuffer, MIDI_SAMPLETYPE, musicBuffer, totalBufferSize, MIDI_RATE);
				}
				else {
					alBufferData(alMusicBuffer, av_sample, musicBuffer, totalBufferSize, av_rate);
				}
				alSourcei( alMusicSourceVoice, AL_BUFFER, alMusicBuffer );
				/*free(musicBuffer);
				musicBuffer = NULL;*/
				alSourcePlay( alMusicSourceVoice );
			}
			
			waitingForMusic = false;
		}
	}
}

/*
======================
I_PauseSong
======================
*/
void I_PauseSongAL ( int handle )
{
	if ( !Music_initialized || !alMusicSourceVoice ) {
		return;
	}
	
	alSourcePause( alMusicSourceVoice );
}

/*
======================
I_ResumeSong
======================
*/
void I_ResumeSongAL ( int handle )
{
	if ( !Music_initialized || !alMusicSourceVoice ) {
		return;
	}
	
	alSourcePlay( alMusicSourceVoice );
}

/*
======================
I_StopSong
======================
*/
void I_StopSongAL( int handle )
{
	if ( !Music_initialized || !alMusicSourceVoice ) {
		return;
	}
	alSourcei(alMusicSourceVoice, AL_LOOPING, 0);
	alSourceStop( alMusicSourceVoice );
	alSourcei(alMusicSourceVoice, AL_BUFFER, 0);
	alDeleteBuffers(1, &alMusicBuffer);
	alGenBuffers(1, &alMusicBuffer);
	alDeleteSources(1, &alMusicSourceVoice);
	alGenSources(1, &alMusicSourceVoice);
}

/*
======================
I_UnRegisterSong
======================
*/
void I_UnRegisterSongAL( int handle )
{
	// does nothing
}

/*
======================
I_RegisterSong
======================
*/
int I_RegisterSongAL( void* data, int length )
{
	// does nothing
	return 0;
}
