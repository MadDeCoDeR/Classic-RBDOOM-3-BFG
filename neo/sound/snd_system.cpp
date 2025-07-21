/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013 Robert Beckebans

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
#include "precompiled.h"
#pragma hdrstop

#include "snd_local.h"

idCVar s_noSound( "s_noSound", "0", CVAR_BOOL, "returns NULL for all sounds loaded and does not update the sound rendering" );

#ifdef ID_RETAIL
idCVar s_useCompression( "s_useCompression", "1", CVAR_BOOL, "Use compressed sound files (mp3/xma)" );
idCVar s_playDefaultSound( "s_playDefaultSound", "0", CVAR_BOOL, "play a beep for missing sounds" );
idCVar s_maxSamples( "s_maxSamples", "5", CVAR_INTEGER, "max samples to load per shader" );
#else
idCVar s_useCompression( "s_useCompression", "1", CVAR_BOOL, "Use compressed sound files (mp3/xma)" );
idCVar s_playDefaultSound( "s_playDefaultSound", "1", CVAR_BOOL, "play a beep for missing sounds" );
idCVar s_maxSamples( "s_maxSamples", "5", CVAR_INTEGER, "max samples to load per shader" );
#endif

#if defined(_MSC_VER) && defined(USE_XAUDIO2)
idCVar s_useXAudio2("s_useXAudio2", "0", CVAR_BOOL | CVAR_SYSTEM | CVAR_ARCHIVE, "set in order to use XAudio 2");
#endif

idCVar preLoad_Samples( "preLoad_Samples", "1", CVAR_SYSTEM | CVAR_BOOL, "preload samples during beginlevelload" );

extern idCVar s_device;
extern idCVar com_pause;
extern idCVar sys_lang;

idSoundSystemLocal soundSystemLocal;
idSoundSystem* soundSystem = &soundSystemLocal;
/*
================================================================================================

idSoundSystemLocal

================================================================================================
*/

/*
========================
TestSound_f

This is called from the main thread.
========================
*/
void TestSound_f( const idCmdArgs& args )
{
	if( args.Argc() != 2 )
	{
		idLib::Printf( "Usage: testSound <file>\n" );
		return;
	}
	if( soundSystemLocal.currentSoundWorld )
	{
		soundSystemLocal.currentSoundWorld->PlayShaderDirectly( args.Argv( 1 ) );
	}
}

/*
========================
RestartSound_f
========================
*/
void RestartSound_f( const idCmdArgs& args )
{
	soundSystemLocal.Restart();
}

/*
========================
ListSamples_f

========================
*/
void ListSamples_f( const idCmdArgs& args )
{
	idLib::Printf( "Sound samples\n-------------\n" );
	int totSize = 0;
	for( int i = 0; i < soundSystemLocal.samples.Num(); i++ )
	{
		idLib::Printf( "%05dkb\t%s\n", soundSystemLocal.samples[ i ]->BufferSize() / 1024, soundSystemLocal.samples[ i ]->GetName() );
		totSize += soundSystemLocal.samples[ i ]->BufferSize();
	}
	idLib::Printf( "--------------------------\n" );
	idLib::Printf( "%05dkb total size\n", totSize / 1024 );
}

/*
========================
idSoundSystemLocal::Restart
========================
*/
void idSoundSystemLocal::Restart()
{

	// Mute all channels in all worlds
	for( int i = 0; i < soundWorlds.Num(); i++ )
	{
		idSoundWorldLocal* sw = soundWorlds[i];
		for( int e = 0; e < sw->emitters.Num(); e++ )
		{
			idSoundEmitterLocal* emitter = sw->emitters[e];
			for( int c = 0; c < emitter->channels.Num(); c++ )
			{
				if (emitter->channels[c]->CanMute()) {
					emitter->channels[c]->Mute();
				}
				else if (emitter->channels[c]->hardwareVoice != NULL){
					emitter->channels[c]->hardwareVoice->Pause();
				}
			}
		}
	}
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (useXAudio) {
		// Shutdown sound hardware
		hardware->Shutdown();

		// Reinitialize sound hardware
		if (!s_noSound.GetBool())
		{
			hardware->Init();
		}
	}
	else
#endif
	if (alcIsExtensionPresent((ALCdevice*)this->GetInternal(), "ALC_SOFT_reopen_device")) {
			((idSoundHardware_OpenAL*)hardware)->RestartHardware();
	}
	InitStreamBuffers();
}

void DefaultDeviceChangeThread(void* data) {

	static uint64 nextCheck = 0;
	const uint64 waitTime = 5000;
	while (1) {
		int	now = Sys_Milliseconds();
		if (now >= nextCheck) {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
			if (common->UseAlternativeAudioAPI()) {
				AudioDevice defaultDevice;
				idSoundHardware_XAudio2::EnumerateAudioDevices(&defaultDevice);
				if (defaultDevice.id != ((AudioDevice*)data)->id) {
					idScopedCriticalSection cs(soundSystemLocal.mutex);
					soundSystemLocal.SetNeedsRestart();
				}
			} else
#endif
			{
				//ALCdevice* defaultALCDevice = alcOpenDevice(NULL); //GK: Otherwise outside of an attached debugger it goes bananas with audio reset
				const ALCchar* defaultDevice =  alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
				const ALCchar* selectedDevice = alcGetString((ALCdevice*)data, ALC_ALL_DEVICES_SPECIFIER);
				/*char* mbdefdev = strdup(defaultDevice);
				idSoundHardware_OpenAL::parseDeviceName(defaultDevice, mbdefdev);
				char* mbseldev = strdup(selectedDevice);
				idSoundHardware_OpenAL::parseDeviceName(selectedDevice, mbseldev);*/
				//idLib::Printf("Default Device: %s\nSelected Device: %s\n", mbdefdev, mbseldev); //GK: Otherwise outside of an attached debugger it only works once???
				if (idStr::Icmp(defaultDevice, selectedDevice)) {
					idScopedCriticalSection cs(soundSystemLocal.mutex);
					soundSystemLocal.SetNeedsRestart();
				}
			}
			nextCheck = now + waitTime;
		}
		Sys_ThreadSleep(4);
	}

}

/*
========================
idSoundSystemLocal::Init

Initialize the SoundSystem.
========================
*/
void idSoundSystemLocal::Init()
{

	idLib::Printf( "----- Initializing Sound System ------\n" );
	
	soundTime = Sys_Milliseconds();
	random.SetSeed( soundTime );

#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (common->UseAlternativeAudioAPI()) {
		hardware = new(TAG_AUDIO) idSoundHardware_XAudio2;
		useXAudio = true;
	} else 
#endif
		hardware = new(TAG_AUDIO) idSoundHardware_OpenAL;
	
	if( !s_noSound.GetBool() )
	{
		hardware->Init();
		InitStreamBuffers();
	}
	cmdSystem->AddCommand( "testSound", TestSound_f, 0, "tests a sound", idCmdSystem::ArgCompletion_SoundName );
	cmdSystem->AddCommand( "s_restart", RestartSound_f, 0, "restart sound system" );
	cmdSystem->AddCommand( "listSamples", ListSamples_f, 0, "lists all loaded sound samples" );
	
	idLib::Printf( "sound system initialized.\n" );
	idLib::Printf( "--------------------------------------\n" );
	if (!initOnce && s_device.GetInteger() < 0) {
		idLib::Printf("Creating Default Device Detection Thread\n");
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
		if (useXAudio) {
			Sys_CreateThread((xthread_t)DefaultDeviceChangeThread, ((idSoundHardware_XAudio2*)hardware)->GetSelectedDevice(), THREAD_LOWEST, "Default Audio Device Change Listener", CORE_ANY);
		}
		else
#endif
		{
			Sys_CreateThread((xthread_t)DefaultDeviceChangeThread, this->GetInternal(), THREAD_LOWEST, "Default Audio Device Change Listener", CORE_ANY);
		}
		initOnce = true;
	}
}

/*
========================
idSoundSystemLocal::InitStreamBuffers
========================
*/
void idSoundSystemLocal::InitStreamBuffers()
{
	streamBufferMutex.Lock();
	const bool empty = ( bufferContexts.Num() == 0 );
	if( empty )
	{
		bufferContexts.SetNum( MAX_SOUND_BUFFERS );
		for( int i = 0; i < MAX_SOUND_BUFFERS; i++ )
		{
			freeStreamBufferContexts.Append( &( bufferContexts[ i ] ) );
		}
	}
	else
	{
		for( int i = 0; i < activeStreamBufferContexts.Num(); i++ )
		{
			freeStreamBufferContexts.Append( activeStreamBufferContexts[ i ] );
		}
		activeStreamBufferContexts.Clear();
	}
	assert( bufferContexts.Num() == MAX_SOUND_BUFFERS );
	assert( freeStreamBufferContexts.Num() == MAX_SOUND_BUFFERS );
	assert( activeStreamBufferContexts.Num() == 0 );
	streamBufferMutex.Unlock();
}

/*
========================
idSoundSystemLocal::FreeStreamBuffers
========================
*/
void idSoundSystemLocal::FreeStreamBuffers()
{
	streamBufferMutex.Lock();
	bufferContexts.Clear();
	freeStreamBufferContexts.Clear();
	activeStreamBufferContexts.Clear();
	streamBufferMutex.Unlock();
}

/*
========================
idSoundSystemLocal::Shutdown
========================
*/
void idSoundSystemLocal::Shutdown()
{
	hardware->Shutdown();
	// EAX or not, the list needs to be cleared
	EFXDatabase.Clear();
	ccdecl.Clear();
	efxloaded = false;
	FreeStreamBuffers();
	samples.DeleteContents( true );
	sampleHash.Free();
}

/*
========================
idSoundSystemLocal::ObtainStreamBuffer

Get a stream buffer from the free pool, returns NULL if none are available
========================
*/
idSoundSystemLocal::bufferContext_t* idSoundSystemLocal::ObtainStreamBufferContext()
{
	bufferContext_t* bufferContext = NULL;
	streamBufferMutex.Lock();
	if( freeStreamBufferContexts.Num() != 0 )
	{
		bufferContext = freeStreamBufferContexts[ freeStreamBufferContexts.Num() - 1 ];
		freeStreamBufferContexts.SetNum( freeStreamBufferContexts.Num() - 1 );
		activeStreamBufferContexts.Append( bufferContext );
	}
	streamBufferMutex.Unlock();
	return bufferContext;
}

/*
========================
idSoundSystemLocal::ReleaseStreamBuffer

Releases a stream buffer back to the free pool
========================
*/
void idSoundSystemLocal::ReleaseStreamBufferContext( bufferContext_t* bufferContext )
{
	streamBufferMutex.Lock();
	if( activeStreamBufferContexts.Remove( bufferContext ) )
	{
		freeStreamBufferContexts.Append( bufferContext );
	}
	streamBufferMutex.Unlock();
}

/*
========================
idSoundSystemLocal::AllocSoundWorld
========================
*/
idSoundWorld* idSoundSystemLocal::AllocSoundWorld( idRenderWorld* rw )
{
	idSoundWorldLocal* local = new( TAG_AUDIO ) idSoundWorldLocal;
	local->renderWorld = rw;
	soundWorlds.Append( local );
	return local;
}

/*
========================
idSoundSystemLocal::FreeSoundWorld
========================
*/
void idSoundSystemLocal::FreeSoundWorld( idSoundWorld* sw )
{
	idSoundWorldLocal* local = static_cast<idSoundWorldLocal*>( sw );
	soundWorlds.Remove( local );
	delete local;
}

/*
========================
idSoundSystemLocal::SetPlayingSoundWorld

Specifying NULL will cause silence to be played.
========================
*/
void idSoundSystemLocal::SetPlayingSoundWorld( idSoundWorld* soundWorld )
{
	if( currentSoundWorld == soundWorld )
	{
		return;
	}
	idSoundWorldLocal* oldSoundWorld = currentSoundWorld;
	
	currentSoundWorld = static_cast<idSoundWorldLocal*>( soundWorld );
	
	if( oldSoundWorld != NULL )
	{
		oldSoundWorld->Update();
	}
}

/*
========================
idSoundSystemLocal::GetPlayingSoundWorld
========================
*/
idSoundWorld* idSoundSystemLocal::GetPlayingSoundWorld()
{
	return currentSoundWorld;
}

/*
========================
idSoundSystemLocal::Render
========================
*/
void idSoundSystemLocal::Render()
{

	if( s_noSound.GetBool() )
	{
		return;
	}
	
	if( needsRestart )
	{
		idScopedCriticalSection cs(mutex);
		Restart();
		needsRestart = false; //GK: That way OpenAL can properly keep all it's existing audio buffer data
	}
	
	SCOPED_PROFILE_EVENT( "SoundSystem::Render" );
	
	if( currentSoundWorld != NULL )
	{
		currentSoundWorld->Update();
	}
	
	hardware->Update();
	
	// The sound system doesn't use game time or anything like that because the sounds are decoded in real time.
	soundTime = Sys_Milliseconds();
}

/*
========================
idSoundSystemLocal::OnReloadSound
========================
*/
void idSoundSystemLocal::OnReloadSound( const idDecl* sound )
{
	for( int i = 0; i < soundWorlds.Num(); i++ )
	{
		soundWorlds[i]->OnReloadSound( sound );
	}
}

/*
========================
idSoundSystemLocal::StopAllSounds
========================
*/
void idSoundSystemLocal::StopAllSounds()
{
	for( int i = 0; i < soundWorlds.Num(); i++ )
	{
		idSoundWorld* sw = soundWorlds[i];
		if( sw )
		{
			sw->StopAllSounds();
		}
	}
	if (hardware != NULL) {
		hardware->Update();
	}
}

/*
========================
idSoundSystemLocal::GetInternal
========================
*/
void* idSoundSystemLocal::GetInternal() const
{
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (useXAudio) {
		return (void*)((idSoundHardware_XAudio2*)hardware)->GetIXAudio2();
	} else
#endif
		return (void*)((idSoundHardware_OpenAL*)hardware)->GetOpenALDevice();
}

/*
========================
idSoundSystemLocal::SoundTime
========================
*/
int idSoundSystemLocal::SoundTime() const
{
	return soundTime;
}

/*
========================
idSoundSystemLocal::AllocateVoice
========================
*/
idSoundVoice* idSoundSystemLocal::AllocateVoice( const idSoundSample* leadinSample, const idSoundSample* loopingSample, const int channel)
{
	return hardware->AllocateVoice(leadinSample, loopingSample, channel);
}

/*
========================
idSoundSystemLocal::FreeVoice
========================
*/
void idSoundSystemLocal::FreeVoice( idSoundVoice* voice )
{
	hardware->FreeVoice( voice );
}

/*
========================
idSoundSystemLocal::LoadSample
========================
*/
idSoundSample* idSoundSystemLocal::LoadSample( const char* name )
{
	idStrStatic< MAX_OSPATH > canonical = name;
	canonical.ToLower();
	canonical.BackSlashesToSlashes();
	canonical.StripFileExtension();
	int hashKey = idStr::Hash( canonical );
	for( int i = sampleHash.First( hashKey ); i != -1; i = sampleHash.Next( i ) )
	{
		if( idStr::Cmp( samples[i]->GetName(), canonical ) == 0 )
		{
			samples[i]->SetLevelLoadReferenced();
			return samples[i];
		}
	}
	idSoundSample* sample;
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (useXAudio) {
		sample = new(TAG_AUDIO) idSoundSample_XAudio2;
	} else
#endif
		sample = new(TAG_AUDIO) idSoundSample_OpenAL;
	sample->SetName( canonical );
	sampleHash.Add( hashKey, samples.Append( sample ) );
	if( !insideLevelLoad )
	{
		// Sound sample referenced before any map is loaded
		sample->SetNeverPurge();
		sample->LoadResource();
	}
	else
	{
		sample->SetLevelLoadReferenced();
	}
	
	if( cvarSystem->GetCVarBool( "fs_buildgame" ) )
	{
		fileSystem->AddSamplePreload( canonical );
	}
	
	return sample;
}

/*
========================
idSoundSystemLocal::StopVoicesWithSample

A sample is about to be freed, make sure the hardware isn't mixing from it.
========================
*/
void idSoundSystemLocal::StopVoicesWithSample( const idSoundSample* const sample )
{
	for( int w = 0; w < soundWorlds.Num(); w++ )
	{
		idSoundWorldLocal* sw = soundWorlds[w];
		if( sw == NULL )
		{
			continue;
		}
		for( int e = 0; e < sw->emitters.Num(); e++ )
		{
			idSoundEmitterLocal* emitter = sw->emitters[e];
			if( emitter == NULL )
			{
				continue;
			}
			for( int i = 0; i < emitter->channels.Num(); i++ )
			{
				if( emitter->channels[i]->leadinSample == sample || emitter->channels[i]->loopingSample == sample )
				{
					emitter->channels[i]->Mute();
				}
			}
		}
	}
}

/*
========================
idSoundSystemLocal::FreeVoice
========================
*/
cinData_t idSoundSystemLocal::ImageForTime( const int milliseconds, const bool waveform )
{
	cinData_t cd;
	cd.imageY = NULL;
	cd.imageCr = NULL;
	cd.imageCb = NULL;
	cd.imageWidth = 0;
	cd.imageHeight = 0;
	cd.status = FMV_IDLE;
	return cd;
}

/*
========================
idSoundSystemLocal::BeginLevelLoad
========================
*/
void idSoundSystemLocal::BeginLevelLoad(const char* mapstring)
{
	insideLevelLoad = true;
	for( int i = 0; i < samples.Num(); i++ )
	{
		if( samples[i]->GetNeverPurge() )
		{
			continue;
		}
		samples[i]->FreeData();
		samples[i]->ResetLevelLoadReferenced();
	}
	//GK: Like the OG Doom 3 make sure there are no efx data remained
	if (efxloaded) {
		EFXDatabase.UnloadFile();
		efxloaded = false;
		hardware->ShutdownReverbSystem();
	}
	if (ccloaded) {
		ccdecl.Clear();
		ccloaded = false;
	}
	//GK: Moved this here since some audio entries are loaded and setup during the level load and not while the level is playing
	idStr ccname("cc/");
	idStr ccmapname(mapstring);
	idStr ccgenericname("generic");
	idStr ccFileExtension(".ccscript");

	ccgenericname.SetFileExtension(ccFileExtension);
	ccmapname.SetFileExtension(ccFileExtension);
	ccmapname.StripPath();
	ccname += sys_lang.GetString();
	ccname += "/";
	ccname += ccgenericname;
	ccloaded = ccdecl.LoadFile(ccname);
	ccname = ccname.SubStr(0, ccname.Last('/')) + "/" + ccmapname;
	bool ccmaploaded = ccdecl.LoadFile(ccname);
	ccloaded = ccloaded || ccmaploaded;
}



/*
========================
idSoundSystemLocal::Preload
========================
*/
void idSoundSystemLocal::Preload( idPreloadManifest& manifest )
{

	idStrStatic< MAX_OSPATH > filename;
	
	int	start = Sys_Milliseconds();
	int numLoaded = 0;
	
	idList< preloadSort_t > preloadSort;
	preloadSort.Resize( manifest.NumResources() );
	for( int i = 0; i < manifest.NumResources(); i++ )
	{
		const preloadEntry_s& p = manifest.GetPreloadByIndex( i );
		idResourceCacheEntry rc;
		// FIXME: write these out sorted
		if( p.resType == PRELOAD_SAMPLE )
		{
			if( p.resourceName.Find( "/vo/", false ) >= 0 )
			{
				continue;
			}
			filename  = "generated/";
			filename += p.resourceName;
			filename.SetFileExtension( "idwav" );
			if( fileSystem->GetResourceCacheEntry( filename, rc ) )
			{
				preloadSort_t ps = {};
				ps.idx = i;
				ps.ofs = rc.offset;
				preloadSort.Append( ps );
			}
		}
	}
	
	preloadSort.SortWithTemplate( idSort_Preload() );
	
	for( int i = 0; i < preloadSort.Num(); i++ )
	{
		const preloadSort_t& ps = preloadSort[ i ];
		const preloadEntry_s& p = manifest.GetPreloadByIndex( ps.idx );
		filename = p.resourceName;
		filename.Replace( "generated/", "" );
		numLoaded++;
		idSoundSample* sample = LoadSample( filename );
		if( sample != NULL && !sample->IsLoaded() )
		{
			sample->LoadResource();
			sample->SetLevelLoadReferenced();
		}
	}
	
	int	end = Sys_Milliseconds();
	common->Printf( "%05d sounds preloaded in %5.1f seconds\n", numLoaded, ( end - start ) * 0.001 );
	common->Printf( "----------------------------------------\n" );
}

/*
========================
idSoundSystemLocal::EndLevelLoad
========================
*/
void idSoundSystemLocal::EndLevelLoad(const char* mapstring)
{

	insideLevelLoad = false;
	
	common->Printf( "----- idSoundSystemLocal::EndLevelLoad -----\n" );
	int		start = Sys_Milliseconds();
	int		keepCount = 0;
	int		loadCount = 0;
	
	idList< preloadSort_t > preloadSort;
	preloadSort.Resize( samples.Num() );
	
	for( int i = 0; i < samples.Num(); i++ )
	{
		common->UpdateLevelLoadPacifier();
		
		
		if( samples[i]->GetNeverPurge() )
		{
			continue;
		}
		if( samples[i]->IsLoaded() )
		{
			keepCount++;
			continue;
		}
		if( samples[i]->GetLevelLoadReferenced() )
		{
			idStrStatic< MAX_OSPATH > filename  = "generated/";
			filename += samples[ i ]->GetName();
			filename.SetFileExtension( "idwav" );
			preloadSort_t ps = {};
			ps.idx = i;
			idResourceCacheEntry rc;
			if( fileSystem->GetResourceCacheEntry( filename, rc ) )
			{
				ps.ofs = rc.offset;
			}
			else
			{
				ps.ofs = 0;
			}
			preloadSort.Append( ps );
			loadCount++;
		}
	}
	preloadSort.SortWithTemplate( idSort_Preload() );
	for( int i = 0; i < preloadSort.Num(); i++ )
	{
		common->UpdateLevelLoadPacifier();
		
		
		samples[ preloadSort[ i ].idx ]->LoadResource();
	}

	//GK: Just like the OG Doom 3
	idStr efxname("efxs/");
	idStr mapname(mapstring);

	mapname.SetFileExtension(".efx");
	mapname.StripPath();
	efxname += mapname;

	efxloaded = EFXDatabase.LoadFile(efxname);
	if (efxloaded) {
		common->Printf("sound: found %s\n", efxname.c_str());
	}
	else {
		common->Printf("sound: missing %s\n", efxname.c_str());
		hardware->ShutdownReverbSystem();
	}


	
	int	end = Sys_Milliseconds();
	
	common->Printf( "%5i sounds loaded in %5.1f seconds\n", loadCount, ( end - start ) * 0.001 );
	common->Printf( "----------------------------------------\n" );
}



/*
========================
idSoundSystemLocal::FreeVoice
========================
*/
void idSoundSystemLocal::PrintMemInfo( MemInfo_t* mi )
{
}