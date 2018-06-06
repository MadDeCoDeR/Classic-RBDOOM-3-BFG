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
#include "m_argv.h"
#include "m_misc.h"
#include "w_wad.h"
#include "d_main.h"
#include "doomdef.h"
#include "libs/timidity/timidity.h"
#include "libs/timidity/controls.h"
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libswresample/swresample.h>
}
#include "sound/snd_local.h"

#pragma warning ( disable : 4244 )

#define SFX_RATE		11050
#define SFX_SAMPLETYPE		AL_FORMAT_MONO8

#define MIDI_CHANNELS		2
#define MIDI_RATE		22050
#define MIDI_SAMPLETYPE		AL_FORMAT_STEREO8
#define MIDI_FORMAT		AUDIO_U8
#define MIDI_FORMAT_BYTES	1

ALuint		alMusicSourceVoice;
ALuint		alMusicBuffer;

MidiSong*	doomMusic;
byte*		musicBuffer;
int		totalBufferSize;

ALenum av_sample;
int av_rate;
bool use_avi;

bool		waitingForMusic;
bool		musicReady;

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

typedef struct tagActiveSound_t {
	ALuint alSourceVoice;
	int id;
	int valid;
	int start;
	int player;
	bool localSound;
	mobj_t *originator;
} activeSound_t;

// cheap little struct to hold a sound
typedef struct {
	int vol;
	int player;
	int pitch;
	int priority;
	mobj_t *originator;
	mobj_t *listener;
} soundEvent_t;

// array of all the possible sounds
// in split screen we only process the loudest sound of each type per frame
soundEvent_t soundEvents[128];
extern int PLAYERCOUNT;

// Source voice settings for all sound effects
const ALfloat		SFX_MAX_DISTANCE = 1200.f;
const ALfloat		SFX_REFERENCE_DISTANCE = 100.f;
const ALfloat		SFX_ROLLOFF_FACTOR = 0.2f;

// Real volumes
const float		GLOBAL_VOLUME_MULTIPLIER = 0.5f;

float			x_SoundVolume = GLOBAL_VOLUME_MULTIPLIER;
float			x_MusicVolume = GLOBAL_VOLUME_MULTIPLIER;

// The actual lengths of all sound effects.
static int 		lengths[NUMSFX];
ALuint			alBuffers[NUMSFX];
activeSound_t		activeSounds[NUM_SOUNDBUFFERS] = {0};

int			S_initialized = 0;
bool			Music_initialized = false;
static bool		soundHardwareInitialized = false;
static int		numOutputChannels = 0;

doomListener_t		doom_Listener;

void			I_InitSoundChannel( int channel, int numOutputChannels_ );

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
	const unsigned char * sfxSampleStart = sfx + SOUND_LUMP_HEADER_SIZE_IN_BYTES;
	
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
void I_SetChannels()
{
	// Original Doom set up lookup tables here
}

/*
======================
I_SetSfxVolume
======================
*/
void I_SetSfxVolume( int volume )
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
int I_GetSfxLumpNum( sfxinfo_t* sfx )
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
	activeSound_t* sound = 0;
	int oldest = 0, oldestnum = -1;
	
	// these id's should not overlap
	if ( id == sfx_sawup || id == sfx_sawidl || id == sfx_sawful || id == sfx_sawhit || id == sfx_stnmov ) {
		// Loop all channels, check.
		for ( i = 0; i < NUM_SOUNDBUFFERS; i++ )
		{
			sound = &activeSounds[i];
			
			if ( sound->valid && ( sound->id == id && sound->player == player ) ) {
				I_StopSound( sound->id, player );
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
	
	alSourceStop( sound->alSourceVoice );
	
	// Attach the source voice to the correct buffer
	if ( sound->id != id ) {
		alSourcei( sound->alSourceVoice, AL_BUFFER, 0 );
		alSourcei( sound->alSourceVoice, AL_BUFFER, alBuffers[id] );
	}
	
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
		}
	} else {
		sound->localSound = true;
	}
	if ( sound->localSound ) {
		x = doom_Listener.Position.x;
		z = doom_Listener.Position.z;
	}
	alSource3f( sound->alSourceVoice, AL_POSITION, x, y, z );
	
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
void I_ProcessSoundEvents( void )
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
int I_StartSound ( int id, mobj_t *origin, mobj_t *listener_origin, int vol, int pitch, int priority )
{
	// only allow player 0s sounds in intermission and finale screens
	if( ::g->gamestate != GS_LEVEL && DoomLib::GetPlayer() != 0 ) {
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
void I_StopSound ( int handle, int player )
{
	// You need the handle returned by StartSound.
	// Would be looping all channels,
	// tracking down the handle,
	// and setting the channel to zero.
	int i;
	activeSound_t* sound = 0;
	
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
int I_SoundIsPlaying( int handle )
{
	if ( !soundHardwareInitialized ) {
		return 0;
	}
	
	int i;
	activeSound_t* sound;
	
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
void I_UpdateSound( void )
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
	activeSound_t* sound;
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
				ALfloat y = 0.f;
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
void I_UpdateSoundParams( int handle, int vol, int sep, int pitch )
{
}

/*
======================
I_ShutdownSound
======================
*/
void I_ShutdownSound( void )
{
	int done = 0;
	int i;
	
	if ( S_initialized ) {
		// Stop all sounds
		for ( i = 0; i < NUM_SOUNDBUFFERS; i++ ) {
			activeSound_t * sound = &activeSounds[i];
			
			if ( !sound ) {
				continue;
			}
			
			I_StopSound( sound->id, 0 );
		}
		
		// Free allocated sound memory
		for ( i = 1; i < NUMSFX; i++ ) {
			if ( S_sfx[i].data && !(S_sfx[i].link) ) {
				free( S_sfx[i].data );
			}
		}
	}
	
	I_StopSong( 0 );
	
	S_initialized = 0;
}

/*
======================
I_InitSoundHardware

Called from the tech4x initialization code. Sets up Doom classic's
sound channels.
======================
*/
void I_InitSoundHardware( int numOutputChannels_, int channelMask )
{
	::numOutputChannels = numOutputChannels_;
	I_InitMusic(); //GK: Just to be sure that music will play
	// Initialize source voices
	for ( int i = 0; i < NUM_SOUNDBUFFERS; i++ ) {
		I_InitSoundChannel( i, numOutputChannels );
	}

	// Create OpenAL buffers for all sounds
	for ( int i = 1; i < NUMSFX; i++ ) {
		alGenBuffers( (ALuint)1, &alBuffers[i] );
	}
	
		
	soundHardwareInitialized = true;
}

/*
======================
I_ShutdownSoundHardware

Called from the tech4x shutdown code. Tears down Doom classic's
sound channels.
======================
*/
void I_ShutdownSoundHardware()
{
	soundHardwareInitialized = false;
	
	I_ShutdownMusic();
	
	// Delete all source voices
	for ( int i = 0; i < NUM_SOUNDBUFFERS; ++i ) {
		activeSound_t * sound = &activeSounds[i];
		
		if ( !sound ) {
			continue;
		}
		
		if ( sound->alSourceVoice ) {
			alSourceStop( sound->alSourceVoice );
			alSourcei( sound->alSourceVoice, AL_BUFFER, 0 );
			alDeleteSources( 1, &sound->alSourceVoice );
		}
	}

	// Delete OpenAL buffers for all sounds
	for ( int i = 0; i < NUMSFX; i++ ) {
		alDeleteBuffers( 1, &alBuffers[i] );
	}
}

/*
======================
I_InitSoundChannel
======================
*/
void I_InitSoundChannel( int channel, int numOutputChannels_ )
{
	activeSound_t *soundchannel = &activeSounds[ channel ];
	
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
void I_InitSound()
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
			} else {
				// Previously loaded already?
				S_sfx[i].data = S_sfx[i].link->data;
				lengths[i] = lengths[ (S_sfx[i].link-S_sfx) / sizeof(sfxinfo_t) ];
			}
			if ( S_sfx[i].data ) {
				alBufferData( alBuffers[i], SFX_SAMPLETYPE, (byte*)S_sfx[i].data, lengths[i], SFX_RATE );
			}
		}
		
		S_initialized = 1;
	}
}

/*
======================
I_SubmitSound
======================
*/
void I_SubmitSound( void )
{
	// Only do this for player 0, it will still handle positioning
	//		for other players, but it can't be outside the game
	//		frame like the soundEvents are.
	if ( DoomLib::GetPlayer() == 0 ) {
		// Do 3D positioning of sounds
		I_UpdateSound();
		
		// Change music if required
		I_UpdateMusic();
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
void I_SetMusicVolume( int volume )
{
	x_MusicVolume = (float)volume / 15.f;
}

/*
======================
I_InitMusic
======================
*/
void I_InitMusic( void )
{
	if ( !Music_initialized ) {
		// Initialize Timidity
		Timidity_Init( MIDI_RATE, MIDI_FORMAT, MIDI_CHANNELS, MIDI_RATE, "classicmusic/gravis.cfg" );
		
		musicBuffer = NULL;
		totalBufferSize = 0;
		waitingForMusic = false;
		musicReady = false;
		
		alGenSources( (ALuint)1, &alMusicSourceVoice );
		
		alSourcef( alMusicSourceVoice, AL_PITCH, 1.f );
		alSourcef( alMusicSourceVoice, AL_LOOPING, AL_TRUE );
		
		alGenBuffers( (ALuint)1, &alMusicBuffer );
		
		Music_initialized = true;
	}
}

/*
======================
I_ShutdownMusic
======================
*/
void I_ShutdownMusic( void )
{
	if ( Music_initialized ) {
		if ( alMusicSourceVoice ) {
			I_StopSong( 0 );
			alSourcei( alMusicSourceVoice, AL_BUFFER, 0 );
			alDeleteSources( 1, &alMusicSourceVoice );
		}
		
		if ( alMusicBuffer ) {
			alDeleteBuffers( 1, &alMusicBuffer );
		}
		
		if ( musicBuffer ) {
			free( musicBuffer );
			musicBuffer = NULL;
		}
		
		Timidity_Shutdown();
	}
	
	totalBufferSize = 0;
	waitingForMusic = false;
	musicReady = false;
	
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
	
	unsigned char * musFile = static_cast< unsigned char * >( W_CacheLumpName( lumpName.c_str(), PU_STATIC_SHARED ) );
	Z_Free(lumpcache[W_GetNumForName(lumpName.c_str())]);
	Z_FreeMemory();
	
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
		use_avi = false;
	}
	else {
			int ret = 0;
			int avindx = 0;
			AVFormatContext*		fmt_ctx = avformat_alloc_context();
			AVCodec*				dec;
			AVCodecContext*			dec_ctx;
			AVPacket packet;
			SwrContext* swr_ctx = NULL;
			unsigned char *avio_ctx_buffer = NULL;
			av_register_all();
			avio_ctx_buffer = static_cast<unsigned char *>(av_malloc((size_t)mus_size));
			memcpy(avio_ctx_buffer, musFile, mus_size);
			AVIOContext *avio_ctx = avio_alloc_context(avio_ctx_buffer, mus_size, 0, NULL, NULL, NULL, NULL);
			fmt_ctx->pb = avio_ctx;
			avformat_open_input(&fmt_ctx, "", NULL, NULL);

			if ((ret = avformat_find_stream_info(fmt_ctx, NULL)) < 0)
			{
				return false;
			}
			ret = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, &dec, 0);
			avindx = ret;
			dec_ctx = fmt_ctx->streams[avindx]->codec;
			dec = avcodec_find_decoder(dec_ctx->codec_id);
			if ((ret = avcodec_open2(dec_ctx, dec, NULL)) < 0)
			{

				return false;
			}
			bool hasplanar = false;
			AVSampleFormat dst_smp;
			if (dec_ctx->sample_fmt >= 5) {
				dst_smp = static_cast<AVSampleFormat> (dec_ctx->sample_fmt - 5);
				swr_ctx = swr_alloc_set_opts(NULL, dec_ctx->channel_layout, dst_smp, dec_ctx->sample_rate, dec_ctx->channel_layout, dec_ctx->sample_fmt, dec_ctx->sample_rate, 0, NULL);
				int res = swr_init(swr_ctx);
				hasplanar = true;
			}
			int format_byte = 0;
			bool use_ext = false;
			if (dec_ctx->sample_fmt == AV_SAMPLE_FMT_U8 || dec_ctx->sample_fmt == AV_SAMPLE_FMT_U8P) {
				format_byte = 1;
			}
			else if (dec_ctx->sample_fmt == AV_SAMPLE_FMT_S16 || dec_ctx->sample_fmt == AV_SAMPLE_FMT_S16P) {
				format_byte = 2;
			}
			else if (dec_ctx->sample_fmt == AV_SAMPLE_FMT_S32 || dec_ctx->sample_fmt == AV_SAMPLE_FMT_S32P) {
				format_byte = 4;
			}
			else {
				//return false;
				format_byte = 4;
				use_ext = true;
			}
			av_rate = dec_ctx->sample_rate;
			switch (format_byte) {
			case 1:
				if (dec_ctx->channels == 2) {
					av_sample = AL_FORMAT_STEREO8;
				}
				else {
					av_sample = AL_FORMAT_MONO8;
				}
				break;
			case 2:
				if (dec_ctx->channels == 2) {
					av_sample = AL_FORMAT_STEREO16;
				}
				else {
					av_sample = AL_FORMAT_MONO16;
				}
				break;
			case 4:
				if (dec_ctx->channels == 2) {
					av_sample = AL_FORMAT_STEREO_FLOAT32;
				}
				else {
					av_sample = AL_FORMAT_MONO_FLOAT32;
				}
				break;
			}
			av_init_packet(&packet);
			AVFrame *frame;
			int frameFinished = 0;
			int offset = 0;
			int num_bytes = 0;
			int bufferoffset = format_byte * 10;
			byte* tBuffer = (byte *)malloc(2 * mus_size*bufferoffset);
			uint8_t** tBuffer2 = NULL;
			int  bufflinesize;

			while (av_read_frame(fmt_ctx, &packet) >= 0) {
				if (packet.stream_index == avindx) {
					frame = av_frame_alloc();
					frameFinished = 0;
					avcodec_decode_audio4(dec_ctx, frame, &frameFinished, &packet);
					if (frameFinished) {

						if (hasplanar) {
							av_samples_alloc_array_and_samples(&tBuffer2,
								&bufflinesize,
								frame->channels,
								av_rescale_rnd(frame->nb_samples, frame->sample_rate, frame->sample_rate, AV_ROUND_UP),
								dst_smp,
								0);

							int res = swr_convert(swr_ctx, tBuffer2, bufflinesize, (const uint8_t **)frame->extended_data, frame->nb_samples);
							num_bytes = av_samples_get_buffer_size(&bufflinesize, frame->channels,
								res, dst_smp, 1);
							memcpy(tBuffer + offset, tBuffer2[0], num_bytes);

							offset += num_bytes;
							av_freep(&tBuffer2[0]);

						}
						else {
							num_bytes = frame->linesize[0];
							while (!tBuffer) {
								tBuffer = (byte *)calloc( mus_size,sizeof(byte*));
							}
							memcpy(tBuffer + offset, frame->extended_data[0], num_bytes);
							offset += num_bytes;
						}



					}
					av_frame_free(&frame);
					free(frame);
				}

				av_free_packet(&packet);
			}
			totalBufferSize = offset;
			musicBuffer = (byte *)malloc(offset);
			memcpy(musicBuffer, tBuffer, offset);
			free(tBuffer);
			tBuffer = NULL;
			if (swr_ctx != NULL) {
				swr_free(&swr_ctx);
			}

			avcodec_close(dec_ctx);
#ifdef _WINDOWS
			av_free(fmt_ctx->pb);
			avformat_close_input(&fmt_ctx);
#else
			//GK: Let it leak let it leak because linux thing that it freeing twiice
			avformat_close_input(&fmt_ctx);
			avformat_free_context(fmt_ctx);
#endif


			av_free(avio_ctx->buffer);
			av_freep(avio_ctx);
			use_avi = true;
		}
	
	musicReady = true;
	return true;
}

/*
======================
I_PlaySong
======================
*/
void I_PlaySong( const char *songname, int looping )
{
	if ( !Music_initialized ) {
		return;
	}
	
	I_StopSong( 0 );
	
	// Clear old state
	if ( musicBuffer ) {
		free( musicBuffer );
		musicBuffer = 0;
	}
	
	musicReady = false;
	I_LoadSong( songname );
	waitingForMusic = true;
	
	if ( DoomLib::GetPlayer() >= 0 ) {
		::g->mus_looping = looping;
	}
}

/*
======================
I_UpdateMusic
======================
*/
void I_UpdateMusic( void )
{
	if ( !Music_initialized ) {
		return;
	}
	
	if ( alMusicSourceVoice ) {
		// Set the volume
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
void I_PauseSong ( int handle )
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
void I_ResumeSong ( int handle )
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
void I_StopSong( int handle )
{
	if ( !Music_initialized || !alMusicSourceVoice ) {
		return;
	}
	
	alSourceStop( alMusicSourceVoice );
}

/*
======================
I_UnRegisterSong
======================
*/
void I_UnRegisterSong( int handle )
{
	// does nothing
}

/*
======================
I_RegisterSong
======================
*/
int I_RegisterSong( void* data, int length )
{
	// does nothing
	return 0;
}
