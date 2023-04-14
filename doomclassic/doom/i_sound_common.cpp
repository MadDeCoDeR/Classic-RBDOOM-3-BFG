/**
* Copyright (C) 2021 George Kalmpokis
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
* of the Software, and to permit persons to whom the Software is furnished to
* do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software. As clarification, there
* is no requirement that the copyright notice and permission be included in
* binary distributions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include "Precompiled.h"
#include <idlib/precompiled.h>

#include "i_sound.h"


#if defined(_MSC_VER) && defined(USE_XAUDIO2)
#include "i_sound_win32.h"
#endif
#include "i_sound_openal.h"

idCVar S_museax("S_museax", "0", CVAR_BOOL | CVAR_SOUND | CVAR_ARCHIVE, "Set music EAX for Classic Doom");
#include "w_wad.h"

soundEvent_t soundEvents[128];
MidiSong* doomMusic;
byte* musicBuffer;
int totalBufferSize;
bool	waitingForMusic;
bool	musicReady;
// Real volumes
const float		GLOBAL_VOLUME_MULTIPLIER = 0.5f;
float			x_SoundVolume = GLOBAL_VOLUME_MULTIPLIER;
float			x_MusicVolume = GLOBAL_VOLUME_MULTIPLIER;
int				S_initialized = 0;
bool			Music_initialized = false;

void I_InitSound() {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
if (common->UseAlternativeAudioAPI()) {
		I_InitSoundXA2();
	}
 else
#endif
 I_InitSoundAL();
}
void I_InitSoundHardware(int numOutputChannels_, int channelMask) {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (common->UseAlternativeAudioAPI()) {
		I_InitSoundHardwareXA2(numOutputChannels_, channelMask);
	}
	else
#endif
		I_InitSoundHardwareAL(numOutputChannels_, channelMask);
}

// ... update sound buffer and audio device at runtime...
void I_UpdateSound(void) {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (common->UseAlternativeAudioAPI()) {
		I_UpdateSoundXA2();
	}
	else
#endif
		I_UpdateSoundAL();
}
void I_SubmitSound(void) {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (common->UseAlternativeAudioAPI()) {
		I_SubmitSoundXA2();
	}
	else
#endif
		I_SubmitSoundAL();
}

// ... shut down and relase at program termination.
void I_ShutdownSound(void) {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (common->UseAlternativeAudioAPI()) {
		I_ShutdownSoundXA2();
	}
	else
#endif
		I_ShutdownSoundAL();
}
void I_ShutdownSoundHardware() {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (common->UseAlternativeAudioAPI()) {
		I_ShutdownSoundHardwareXA2();
	}
	else
#endif
		I_ShutdownSoundHardwareAL();
}

//
//  SFX I/O
//

// Initialize channels?
void I_SetChannels() {
	// Original Doom set up lookup tables here
}

// Get raw data lump index for sound descriptor.
int I_GetSfxLumpNum(sfxinfo_t* sfxinfo) {
	char namebuf[9];
	sprintf(namebuf, "ds%s", sfxinfo->name);
	return W_GetNumForName(namebuf);
}

// Starts a sound in a particular sound channel.
int I_StartSound(int id, mobj_t* origin, mobj_t* listener_origin, int vol, int pitch, int priority) {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (common->UseAlternativeAudioAPI()) {
		return I_StartSoundXA2(id, origin, listener_origin, vol, pitch, priority);
	}
	else
#endif
		return I_StartSoundAL(id, origin, listener_origin, vol, pitch, priority);
}


// Stops a sound channel.
void I_StopSound(int handle, int player) {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (common->UseAlternativeAudioAPI()) {
		I_StopSoundXA2(handle, player);
	}
	else
#endif
		I_StopSoundAL(handle, player);
}

// Called by S_*() functions
//  to see if a channel is still playing.
// Returns 0 if no longer playing, 1 if playing.
int I_SoundIsPlaying(int handle) {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (common->UseAlternativeAudioAPI()) {
		return I_SoundIsPlayingXA2(handle);
	}
	else
#endif
		return I_SoundIsPlayingAL(handle);
}

// Updates the volume, separation,
//  and pitch of a sound channel.
void I_UpdateSoundParams(int handle, int vol, int sep, int pitch) {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (common->UseAlternativeAudioAPI()) {
		I_UpdateSoundParamsXA2(handle, vol, pitch, pitch);
	}
	else
#endif
		I_UpdateSoundParamsAL(handle, vol, pitch, pitch);
}

void I_SetSfxVolume(int volume) {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (common->UseAlternativeAudioAPI()) {
		I_SetSfxVolumeXA2(volume);
	}
	else
#endif
		I_SetSfxVolumeAL(volume);
}
//
//  MUSIC I/O
//
void I_InitMusic(void) {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (common->UseAlternativeAudioAPI()) {
		I_InitMusicXA2();
	}
	else
#endif
		I_InitMusicAL();
}
void I_ShutdownMusic(void) {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (common->UseAlternativeAudioAPI()) {
		I_ShutdownMusicXA2();
	}
	else
#endif
		I_ShutdownMusicAL();
}
// Volume.
void I_SetMusicVolume(int volume) {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (common->UseAlternativeAudioAPI()) {
		I_SetMusicVolumeXA2(volume);
	}
	else
#endif
		I_SetMusicVolumeAL(volume);
}
// PAUSE game handling.
void I_PauseSong(int handle) {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (common->UseAlternativeAudioAPI()) {
		I_PauseSongXA2(handle);
	}
	else
#endif
		I_PauseSongAL(handle);
}
void I_ResumeSong(int handle) {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (common->UseAlternativeAudioAPI()) {
		I_ResumeSongXA2(handle);
	}
	else
#endif
		I_ResumeSongAL(handle);
}
// Registers a song handle to song data.
int I_RegisterSong(void* data, int length) {
	// does nothing
	return 0;
}
// Called by anything that wishes to start music.
//  plays a song, and when the song is done,
//  starts playing it again in an endless loop.
// Horrible thing to do, considering.
void I_PlaySong(const char* songname, int looping) {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (common->UseAlternativeAudioAPI()) {
		I_PlaySongXA2(songname, looping);
	}
	else
#endif
		I_PlaySongAL(songname, looping);
}
// Stops a song over 3 seconds.
void I_StopSong(int handle) {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (common->UseAlternativeAudioAPI()) {
		I_StopSongXA2(handle);
	}
	else
#endif
		I_StopSongAL(handle);
}
// See above (register), then think backwards
void I_UnRegisterSong(int handle) {
	// does nothing
}
// Update Music (XMP), check for notifications
void I_UpdateMusic(void) {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (common->UseAlternativeAudioAPI()) {
		I_UpdateMusicXA2();
	}
	else
#endif
		I_UpdateMusicAL();
}

void I_ProcessSoundEvents() {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (common->UseAlternativeAudioAPI()) {
		I_ProcessSoundEventsXA2();
	}
	else
#endif
		I_ProcessSoundEventsAL();
}