/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013 Robert Beckebans
Copyright (c) 2010 by Chris Robinson <chris.kcat@gmail.com> (OpenAL Info Utility)

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
#pragma hdrstop
#include "precompiled.h"
#include "../snd_local.h"
#include "../../../doomclassic/doom/i_sound_openal.h"

extern idCVar s_showLevelMeter;
extern idCVar s_meterTopTime;
extern idCVar s_meterPosition;
extern idCVar s_device;
extern idCVar s_showPerfData;
extern idCVar s_volume_dB;

/*
========================
idSoundHardware_OpenAL::idSoundHardware_OpenAL
========================
*/
idSoundHardware_OpenAL::idSoundHardware_OpenAL(): idSoundHardware()
{
	openalDevice = NULL;
	openalContext = NULL;
	
	//vuMeterRMS = NULL;
	//vuMeterPeak = NULL;
	
	//outputChannels = 0;
	//channelMask = 0;
	
	voices.SetNum( 0 );
	zombieVoices.SetNum( 0 );
	freeVoices.SetNum( 0 );
	
	//lastResetTime = 0;
}

void idSoundHardware_OpenAL::PrintDeviceList( const char* list )
{
	if( !list || *list == '\0' )
	{
		idLib::Printf( "    !!! none !!!\n" );
	}
	else
	{
		do
		{
			idLib::Printf( "    %s\n", list );
			list += strlen( list ) + 1;
		}
		while( *list != '\0' );
	}
}

void idSoundHardware_OpenAL::PrintALCInfo( ALCdevice* device )
{
	ALCint major, minor;
	
	if( device )
	{
		const ALCchar* devname = NULL;
		idLib::Printf( "\n" );
		if( alcIsExtensionPresent( device, "ALC_ENUMERATE_ALL_EXT" ) != AL_FALSE )
		{
			devname = alcGetString( device, ALC_ALL_DEVICES_SPECIFIER );
		}
		
		if( CheckALCErrors( device ) != ALC_NO_ERROR || !devname )
		{
			devname = alcGetString( device, ALC_DEVICE_SPECIFIER );
		}
		
		idLib::Printf( "** Info for device \"%s\" **\n", devname );
	}
	alcGetIntegerv( device, ALC_MAJOR_VERSION, 1, &major );
	alcGetIntegerv( device, ALC_MINOR_VERSION, 1, &minor );
	
	if( CheckALCErrors( device ) == ALC_NO_ERROR )
		idLib::Printf( "ALC version: %d.%d\n", major, minor );
		
	if( device )
	{
		idLib::Printf( "OpenAL extensions: %s", alGetString( AL_EXTENSIONS ) );
		
		//idLib::Printf("ALC extensions:");
		//printList(alcGetString(device, ALC_EXTENSIONS), ' ');
		CheckALCErrors( device );
	}
}

void idSoundHardware_OpenAL::PrintALInfo()
{
	idLib::Printf( "OpenAL vendor string: %s\n", alGetString( AL_VENDOR ) );
	idLib::Printf( "OpenAL renderer string: %s\n", alGetString( AL_RENDERER ) );
	idLib::Printf( "OpenAL version string: %s\n", alGetString( AL_VERSION ) );
	idLib::Printf( "OpenAL extensions: %s", alGetString( AL_EXTENSIONS ) );
	//PrintList(alGetString(AL_EXTENSIONS), ' ');
	CheckALErrors();
}

void listDevices_f_AL( const idCmdArgs& args )
{
	idLib::Printf( "Available playback devices:\n" );
	if( alcIsExtensionPresent( NULL, "ALC_ENUMERATE_ALL_EXT" ) != AL_FALSE )
	{
		idSoundHardware_OpenAL::PrintDeviceList( alcGetString( NULL, ALC_ALL_DEVICES_SPECIFIER ) );
	}
	else
	{
		idSoundHardware_OpenAL::PrintDeviceList( alcGetString( NULL, ALC_DEVICE_SPECIFIER ) );
	}
	
	//idLib::Printf("Available capture devices:\n");
	//printDeviceList(alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER));
	
	if( alcIsExtensionPresent( NULL, "ALC_ENUMERATE_ALL_EXT" ) != AL_FALSE )
	{
		idLib::Printf( "Default playback device: %s\n", alcGetString( NULL, ALC_DEFAULT_ALL_DEVICES_SPECIFIER ) );
	}
	else
	{
		idLib::Printf( "Default playback device: %s\n",  alcGetString( NULL, ALC_DEFAULT_DEVICE_SPECIFIER ) );
	}
	
	//idLib::Printf("Default capture device: %s\n", alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER));
	
	idSoundHardware_OpenAL::PrintALCInfo( NULL );
	
	idSoundHardware_OpenAL::PrintALCInfo( ( ALCdevice* )soundSystem->GetInternal() );
}

static void parseDeviceName(const ALCchar* wcDevice, char* mbDevice) {
#ifdef WIN32
	int wdev_size = MultiByteToWideChar(CP_UTF8, NULL, wcDevice, -1, NULL, 0);
	//GK: just convert the name from UTF-8 char to wide char and then to ANSI char
	wchar_t* wdevs = new wchar_t[wdev_size];
	MultiByteToWideChar(CP_UTF8, NULL, wcDevice, -1, wdevs, wdev_size);
	WideCharToMultiByte(CP_ACP, NULL, wdevs, wdev_size, mbDevice, wdev_size, NULL, 0);
	delete[] wdevs;
#else
	wchar_t* wdevs = new wchar_t[512];
	int wdev_size = mbstowcs(wdevs, wcDevice, strlen(wcDevice));
	wdevs[wdev_size] = '\0';
	int mb_size = wcstombs(mbDevice, wdevs, wdev_size);
	mbDevice[mb_size] = '\0';
	delete[] wdevs;
#endif
}

static void list_audio_devices(const ALCchar *devices, const ALCchar *selectedDevice)  //GK: Why not ?
{
	const ALCchar *device = devices, *next = devices + 1;	
	size_t len = 0;
	int index = 0;
	char* mbseldev = strdup(selectedDevice);
	parseDeviceName(selectedDevice, mbseldev);

	common->Printf( "Devices list:\n");
	common->Printf( "-------------\n");
	while (device && *device != '\0' && next && *next != '\0') {
		index++;
		char *mbdevs=strdup(device);
		parseDeviceName(device, mbdevs);
		common->Printf( "%s	%3d: %s\n", !idStr::Icmp(mbdevs, mbseldev)? "*" : "", index,  mbdevs);
		len = strlen(device);
		device += (len + 1);
		next += (len + 2);
	}
	common->Printf("-------------\n");
}



/*
========================
idSoundHardware_OpenAL::Init
========================
*/
void idSoundHardware_OpenAL::Init()
{
	cmdSystem->AddCommand( "listDevices", listDevices_f_AL, 0, "Lists the connected sound devices", NULL );
	ALboolean enumeration;
	enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	
	common->Printf( "Setup OpenAL device and context... " );
	
	openalDevice = alcOpenDevice( NULL );
	if( openalDevice == NULL )
	{
		common->Warning( "idSoundHardware_OpenAL::Init: alcOpenDevice() failed\n" );
		return;
	}
	//GK: Set this for EFX support
	ALCint att[4] = {0};
	att[0] = ALC_MAX_AUXILIARY_SENDS;
	att[1] = 4;
	openalContext = alcCreateContext( openalDevice, att );
	if( alcMakeContextCurrent( openalContext ) == 0 )
	{
		common->FatalError( "idSoundHardware_OpenAL::Init: alcMakeContextCurrent( %p) failed\n", openalContext );
		return;
	}
	//GK: And check if it works
	hasEFX = true;
	ALCint size = 0;
	alcGetIntegerv(openalDevice, ALC_MAX_AUXILIARY_SENDS, 1, &size);
	if (!alcIsExtensionPresent(openalDevice, "ALC_EXT_EFX") || size == 0) {
		hasEFX = false;
		common->Printf("No EAX support");
	} else {
	ALCint num_sends = 0;
	alcGetIntegerv(openalDevice, ALC_MAX_AUXILIARY_SENDS, 1, &num_sends);
	common->Printf("idSoundHardware_OpenAL::Init: Number of EAX sends: %d\n", num_sends);
	alGenAuxiliaryEffectSlots(1, &slot); //GK: This will remain static during the whole execution
	if (!alIsAuxiliaryEffectSlot(slot)) {
		common->Warning("idSoundHardware_OpenAL::Init: alGenAuxiliaryEffectSlots() failed\n");
	}
	else {
		//GK: Set default preset for Audio Logs, PDA Videos and Radio Communications
		alGenAuxiliaryEffectSlots(1, &voiceslot);
		if (!alIsAuxiliaryEffectSlot(voiceslot)) {
			common->Warning("idSoundHardware_OpenAL::Init: EFX voice Effect slot failed to initialize\n");
		}
		else {
			EFXEAXREVERBPROPERTIES voicereverb = EFX_REVERB_PRESET_AUDITORIUM;
			EFXEAXREVERBPROPERTIES* voicereverb2 = &voicereverb;
			ALuint EFX;
			alGenEffects(1, &EFX);
			alEffecti(EFX, AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB);
			alEffectf(EFX, AL_EAXREVERB_DENSITY, voicereverb2->flDensity);
			alEffectf(EFX, AL_EAXREVERB_DIFFUSION, voicereverb2->flDiffusion);
			alEffectf(EFX, AL_EAXREVERB_GAIN, voicereverb2->flGain);
			alEffectf(EFX, AL_EAXREVERB_GAINHF, voicereverb2->flGainHF);
			alEffectf(EFX, AL_EAXREVERB_GAINLF, voicereverb2->flGainLF);
			alEffectf(EFX, AL_EAXREVERB_DECAY_TIME, voicereverb2->flDecayTime);
			alEffectf(EFX, AL_EAXREVERB_DECAY_HFRATIO, voicereverb2->flDecayHFRatio);
			alEffectf(EFX, AL_EAXREVERB_DECAY_LFRATIO, voicereverb2->flDecayLFRatio);
			alEffectf(EFX, AL_EAXREVERB_REFLECTIONS_GAIN, voicereverb2->flReflectionsGain);
			alEffectf(EFX, AL_EAXREVERB_REFLECTIONS_DELAY, voicereverb2->flReflectionsDelay);
			alEffectfv(EFX, AL_EAXREVERB_REFLECTIONS_PAN, voicereverb2->flReflectionsPan);
			alEffectf(EFX, AL_EAXREVERB_LATE_REVERB_GAIN, voicereverb2->flLateReverbGain);
			alEffectf(EFX, AL_EAXREVERB_LATE_REVERB_DELAY, voicereverb2->flLateReverbDelay);
			alEffectfv(EFX, AL_EAXREVERB_LATE_REVERB_PAN, voicereverb2->flLateReverbPan);
			alEffectf(EFX, AL_EAXREVERB_ECHO_TIME, voicereverb2->flEchoTime);
			alEffectf(EFX, AL_EAXREVERB_ECHO_DEPTH, voicereverb2->flEchoDepth);
			alEffectf(EFX, AL_EAXREVERB_MODULATION_TIME, voicereverb2->flModulationTime);
			alEffectf(EFX, AL_EAXREVERB_MODULATION_DEPTH, voicereverb2->flModulationDepth);
			alEffectf(EFX, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, voicereverb2->flAirAbsorptionGainHF);
			alEffectf(EFX, AL_EAXREVERB_HFREFERENCE, voicereverb2->flHFReference);
			alEffectf(EFX, AL_EAXREVERB_LFREFERENCE, voicereverb2->flLFReference);
			alEffectf(EFX, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, voicereverb2->flRoomRolloffFactor);
			alEffecti(EFX, AL_EAXREVERB_DECAY_HFLIMIT, voicereverb2->iDecayHFLimit);
			alAuxiliaryEffectSloti(((idSoundHardware_OpenAL*)soundSystemLocal.hardware)->voiceslot, AL_EFFECTSLOT_EFFECT, EFX);
			alDeleteEffects(1, &EFX);
		}

		alGenFilters(1, &voicefilter);
		if (!alIsFilter(voicefilter)) {
			common->Warning("idSoundHardware_OpenAL::Init: alGenFilters() failed\n");
		}
		else {
			//GK: Direct Copy paste from Dhewm 3
			alFilteri(voicefilter, AL_FILTER_TYPE, AL_FILTER_LOWPASS);
			// original EAX occusion value was -1150
			// default OCCLUSIONLFRATIO is 0.25

			// pow(10.0, (-1150*0.25)/2000.0)
			alFilterf(voicefilter, AL_LOWPASS_GAIN, 0.718208f);
			// pow(10.0, -1150/2000.0)
			alFilterf(voicefilter, AL_LOWPASS_GAINHF, 0.266073f);
		}
	}
	}
	common->Printf( "Done.\n" );
	
	common->Printf( "OpenAL vendor: %s\n", alGetString( AL_VENDOR ) );
	common->Printf( "OpenAL renderer: %s\n", alGetString( AL_RENDERER ) );
	common->Printf( "OpenAL version: %s\n", alGetString( AL_VERSION ) );
	common->Printf( "OpenAL extensions: %s\n", alGetString( AL_EXTENSIONS ) );

	if (enumeration == AL_TRUE) {
		list_audio_devices(alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER), alcGetString(openalDevice, ALC_ALL_DEVICES_SPECIFIER));
	}
	
	//pMasterVoice->SetVolume( DBtoLinear( s_volume_dB.GetFloat() ) );
	
	//outputChannels = deviceDetails.OutputFormat.Format.nChannels;
	//channelMask = deviceDetails.OutputFormat.dwChannelMask;
	
	//idSoundVoice::InitSurround( outputChannels, channelMask );
	
	// ---------------------
	// Initialize the Doom classic sound system.
	// ---------------------
	
	I_InitSoundHardwareAL( voices.Max(), 0 );

	// ---------------------
	// Create VU Meter Effect
	// ---------------------
	/*
	IUnknown* vuMeter = NULL;
	XAudio2CreateVolumeMeter( &vuMeter, 0 );
	
	XAUDIO2_EFFECT_DESCRIPTOR descriptor;
	descriptor.InitialState = true;
	descriptor.OutputChannels = outputChannels;
	descriptor.pEffect = vuMeter;
	
	XAUDIO2_EFFECT_CHAIN chain;
	chain.EffectCount = 1;
	chain.pEffectDescriptors = &descriptor;
	
	pMasterVoice->SetEffectChain( &chain );
	
	vuMeter->Release();
	*/
	
	// ---------------------
	// Create VU Meter Graph
	// ---------------------
	
	/*
	vuMeterRMS = console->CreateGraph( outputChannels );
	vuMeterPeak = console->CreateGraph( outputChannels );
	vuMeterRMS->Enable( false );
	vuMeterPeak->Enable( false );
	
	memset( vuMeterPeakTimes, 0, sizeof( vuMeterPeakTimes ) );
	
	vuMeterPeak->SetFillMode( idDebugGraph::GRAPH_LINE );
	vuMeterPeak->SetBackgroundColor( idVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
	
	vuMeterRMS->AddGridLine( 0.500f, idVec4( 0.5f, 0.5f, 0.5f, 1.0f ) );
	vuMeterRMS->AddGridLine( 0.250f, idVec4( 0.5f, 0.5f, 0.5f, 1.0f ) );
	vuMeterRMS->AddGridLine( 0.125f, idVec4( 0.5f, 0.5f, 0.5f, 1.0f ) );
	
	const char* channelNames[] = { "L", "R", "C", "S", "Lb", "Rb", "Lf", "Rf", "Cb", "Ls", "Rs" };
	for( int i = 0, ci = 0; ci < sizeof( channelNames ) / sizeof( channelNames[0] ); ci++ )
	{
		if( ( channelMask & BIT( ci ) ) == 0 )
		{
			continue;
		}
		vuMeterRMS->SetLabel( i, channelNames[ ci ] );
		i++;
	}
	*/
	
	// OpenAL doesn't really impose a maximum number of sources
	voices.SetNum( voices.Max() );
	for (int i = 0; i < voices.Max(); i++) {
		voices[i] = *(new idSoundVoice_OpenAL());
	}
	freeVoices.SetNum( voices.Max() );
	zombieVoices.SetNum( 0 );
	for( int i = 0; i < voices.Num(); i++ )
	{
		freeVoices[i] = &voices[i];
	}
}

/*
========================
idSoundHardware_OpenAL::Shutdown
========================
*/
void idSoundHardware_OpenAL::Shutdown()
{
	// ---------------------
	// Shutdown the Doom classic sound system.
	// ---------------------
	I_ShutdownSoundHardwareAL();

	for( int i = 0; i < voices.Num(); i++ )
	{
		idSoundVoice_OpenAL* voice = (idSoundVoice_OpenAL*)&voices[i];
		voice->DestroyInternal();
	}
	voices.Clear();
	freeVoices.Clear();
	zombieVoices.Clear();

	if (alIsFilter(voicefilter)) {
		alDeleteFilters(1, &voicefilter);
	}

	ShutdownReverbSystem();
	
	alcMakeContextCurrent( NULL );
	
	alcDestroyContext( openalContext );
	openalContext = NULL;
	
	alcCloseDevice( openalDevice );
	openalDevice = NULL;

	
	
	/*
	if( vuMeterRMS != NULL )
	{
		console->DestroyGraph( vuMeterRMS );
		vuMeterRMS = NULL;
	}
	if( vuMeterPeak != NULL )
	{
		console->DestroyGraph( vuMeterPeak );
		vuMeterPeak = NULL;
	}
	*/
}

void idSoundHardware_OpenAL::ShutdownReverbSystem()
{
	alAuxiliaryEffectSloti(slot, AL_EFFECTSLOT_EFFECT, AL_EFFECT_NULL);
	if (alIsEffect(EAX)) {
		alDeleteEffects(1, &EAX);
		EAX = 0;
	}
}

/*
========================
idSoundHardware_OpenAL::AllocateVoice
========================
*/
idSoundVoice* idSoundHardware_OpenAL::AllocateVoice( const idSoundSample* leadinSample, const idSoundSample* loopingSample, const int channel )
{
	if( leadinSample == NULL )
	{
		return NULL;
	}
	if( loopingSample != NULL )
	{
		if( ( leadinSample->GetFormat().basic.formatTag != loopingSample->GetFormat().basic.formatTag ) || ( leadinSample->GetFormat().basic.numChannels != loopingSample->GetFormat().basic.numChannels ) )
		{
			idLib::Warning( "Leadin/looping format mismatch: %s & %s", leadinSample->GetName(), loopingSample->GetName() );
			loopingSample = NULL;
		}
	}
	
	// Try to find a free voice that matches the format
	// But fallback to the last free voice if none match the format
	idSoundVoice_OpenAL* voice = NULL;
	for( int i = 0; i < freeVoices.Num(); i++ )
	{
		voice = (idSoundVoice_OpenAL*)freeVoices[i];
		if( voice->IsPlaying() )
		{
			continue;
		}
		
		if( voice->CompatibleFormat( ( idSoundSample_OpenAL* )leadinSample ) )
		{
			break;
		}
	}
	if( voice != NULL )
	{
		voice->Create( leadinSample, loopingSample, channel );
		freeVoices.Remove( voice );
		return voice;
	}
	
	return NULL;
}

/*
========================
idSoundHardware_OpenAL::FreeVoice
========================
*/
void idSoundHardware_OpenAL::FreeVoice( idSoundVoice* voice )
{
	voice->Stop();
	
	// Stop() is asyncronous, so we won't flush bufferes until the
	// voice on the zombie channel actually returns !IsPlaying()
	zombieVoices.Append( (idSoundVoice_OpenAL*)voice );
}

/*
========================
idSoundHardware_OpenAL::Update
========================
*/
void idSoundHardware_OpenAL::Update()
{
	if (openalDevice == NULL)
	{
		int nowTime = Sys_Milliseconds();
		if (lastResetTime + 1000 < nowTime)
		{
			lastResetTime = nowTime;
			Init();
		}
		return;
	}

	ALfloat listenerPosition[3];
	ALfloat listenerOrientation[6];
	if (soundSystem->GetPlayingSoundWorld() != NULL) {
		listenerPosition[0] = -((idSoundWorldLocal*)soundSystem->GetPlayingSoundWorld())->listener.pos.y;
		listenerPosition[1] = ((idSoundWorldLocal*)soundSystem->GetPlayingSoundWorld())->listener.pos.z;
		listenerPosition[2] = -((idSoundWorldLocal*)soundSystem->GetPlayingSoundWorld())->listener.pos.x;

		listenerOrientation[0] = -((idSoundWorldLocal*)soundSystem->GetPlayingSoundWorld())->listener.axis[0].y;
		listenerOrientation[1] = ((idSoundWorldLocal*)soundSystem->GetPlayingSoundWorld())->listener.axis[0].z;
		listenerOrientation[2] = -((idSoundWorldLocal*)soundSystem->GetPlayingSoundWorld())->listener.axis[0].x;

		listenerOrientation[3] = -((idSoundWorldLocal*)soundSystem->GetPlayingSoundWorld())->listener.axis[2].y;
		listenerOrientation[4] = ((idSoundWorldLocal*)soundSystem->GetPlayingSoundWorld())->listener.axis[2].z;
		listenerOrientation[5] = -((idSoundWorldLocal*)soundSystem->GetPlayingSoundWorld())->listener.axis[2].x;
	}
	
	if( soundSystem->IsMuted() )
	{
		alListenerf( AL_GAIN, 0.0f );
	}
	else
	{
		alListenerf( AL_GAIN, common->GetCurrentGame() == DOOM3_BFG ? DBtoLinear( s_volume_dB.GetFloat() ) : 1.0f );
	}
	if (common->GetCurrentGame() == DOOM3_BFG) {
		alListenerfv(AL_POSITION, listenerPosition);
		alListenerfv(AL_ORIENTATION, listenerOrientation);
	}
	
	
	// IXAudio2SourceVoice::Stop() has been called for every sound on the
	// zombie list, but it is documented as asyncronous, so we have to wait
	// until it actually reports that it is no longer playing.
	for( int i = 0; i < zombieVoices.Num(); i++ )
	{
		idSoundVoice_OpenAL* alZombieVoice = (idSoundVoice_OpenAL*)zombieVoices[i];
		alZombieVoice->FlushSourceBuffers();
		if( !alZombieVoice->IsPlaying() )
		{
			freeVoices.Append( zombieVoices[i] );
			zombieVoices.RemoveIndexFast( i );
			i--;
		}
		else
		{
			static int playingZombies;
			playingZombies++;
		}
	}
	
	/*
	if( s_showPerfData.GetBool() )
	{
		XAUDIO2_PERFORMANCE_DATA perfData;
		pXAudio2->GetPerformanceData( &perfData );
		idLib::Printf( "Voices: %d/%d CPU: %.2f%% Mem: %dkb\n", perfData.ActiveSourceVoiceCount, perfData.TotalSourceVoiceCount, perfData.AudioCyclesSinceLastQuery / ( float )perfData.TotalCyclesSinceLastQuery, perfData.MemoryUsageInBytes / 1024 );
	}
	*/
	
	/*
	if( vuMeterRMS == NULL )
	{
		// Init probably hasn't been called yet
		return;
	}
	
	vuMeterRMS->Enable( s_showLevelMeter.GetBool() );
	vuMeterPeak->Enable( s_showLevelMeter.GetBool() );
	
	if( !s_showLevelMeter.GetBool() )
	{
		pMasterVoice->DisableEffect( 0 );
		return;
	}
	else
	{
		pMasterVoice->EnableEffect( 0 );
	}
	
	float peakLevels[ 8 ];
	float rmsLevels[ 8 ];
	
	XAUDIO2FX_VOLUMEMETER_LEVELS levels;
	levels.ChannelCount = outputChannels;
	levels.pPeakLevels = peakLevels;
	levels.pRMSLevels = rmsLevels;
	
	if( levels.ChannelCount > 8 )
	{
		levels.ChannelCount = 8;
	}
	
	pMasterVoice->GetEffectParameters( 0, &levels, sizeof( levels ) );
	
	int currentTime = Sys_Milliseconds();
	for( int i = 0; i < outputChannels; i++ )
	{
		if( vuMeterPeakTimes[i] < currentTime )
		{
			vuMeterPeak->SetValue( i, vuMeterPeak->GetValue( i ) * 0.9f, colorRed );
		}
	}
	
	float width = 20.0f;
	float height = 200.0f;
	float left = 100.0f;
	float top = 100.0f;
	
	sscanf( s_meterPosition.GetString(), "%f %f %f %f", &left, &top, &width, &height );
	
	vuMeterRMS->SetPosition( left, top, width * levels.ChannelCount, height );
	vuMeterPeak->SetPosition( left, top, width * levels.ChannelCount, height );
	
	for( uint32 i = 0; i < levels.ChannelCount; i++ )
	{
		vuMeterRMS->SetValue( i, rmsLevels[ i ], idVec4( 0.5f, 1.0f, 0.0f, 1.00f ) );
		if( peakLevels[ i ] >= vuMeterPeak->GetValue( i ) )
		{
			vuMeterPeak->SetValue( i, peakLevels[ i ], colorRed );
			vuMeterPeakTimes[i] = currentTime + s_meterTopTime.GetInteger();
		}
	}
	*/
}

void idSoundHardware_OpenAL::UpdateEAXEffect(idSoundEffect* effect)
{
	EFXEAXREVERBPROPERTIES EnvironmentParameters;
	if (alIsEffect(EAX)) {
		alDeleteEffects(1, &EAX);

	}
	EAX = 0;
	// get area reverb setting from EAX Manager
	if ((effect) && (effect->data)) {
		memcpy(&EnvironmentParameters, effect->data, effect->datasize);
		/*if (soundSystemLocal.s_muteEAXReverb.GetBool()) {
			EnvironmentParameters.flGain = -10000;
			EnvironmentID = -2;
		}*/
		if (hasEFX) {
			SetEFX(&EnvironmentParameters);
			alAuxiliaryEffectSloti(slot, AL_EFFECTSLOT_EFFECT, EAX);
		}
	}
}

//#ifdef USE_OPENAL
//GK: In openAL-soft the EFX is quite different from the standard openAL
/*
========================
idSoundSystemLocal::SetEFX
========================
*/
void idSoundHardware_OpenAL::SetEFX(EFXEAXREVERBPROPERTIES* rev)
{
	alGenEffects(1, &EAX);
	if (alGetEnumValue("AL_EFFECT_EAXREVERB") != 0)

	{

		//common->Printf("Using EAX Reverb\n");



		/* EAX Reverb is available. Set the EAX effect type then load the

		 * reverb properties. */

		alEffecti(EAX, AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB);



		alEffectf(EAX, AL_EAXREVERB_DENSITY, rev->flDensity);

		alEffectf(EAX, AL_EAXREVERB_DIFFUSION, rev->flDiffusion);

		alEffectf(EAX, AL_EAXREVERB_GAIN, rev->flGain);

		alEffectf(EAX, AL_EAXREVERB_GAINHF, rev->flGainHF);

		alEffectf(EAX, AL_EAXREVERB_GAINLF, rev->flGainLF);

		alEffectf(EAX, AL_EAXREVERB_DECAY_TIME, rev->flDecayTime);

		alEffectf(EAX, AL_EAXREVERB_DECAY_HFRATIO, rev->flDecayHFRatio);

		alEffectf(EAX, AL_EAXREVERB_DECAY_LFRATIO, rev->flDecayLFRatio);

		alEffectf(EAX, AL_EAXREVERB_REFLECTIONS_GAIN, rev->flReflectionsGain);

		alEffectf(EAX, AL_EAXREVERB_REFLECTIONS_DELAY, rev->flReflectionsDelay);

		alEffectfv(EAX, AL_EAXREVERB_REFLECTIONS_PAN, rev->flReflectionsPan);

		alEffectf(EAX, AL_EAXREVERB_LATE_REVERB_GAIN, rev->flLateReverbGain);

		alEffectf(EAX, AL_EAXREVERB_LATE_REVERB_DELAY, rev->flLateReverbDelay);

		alEffectfv(EAX, AL_EAXREVERB_LATE_REVERB_PAN, rev->flLateReverbPan);

		alEffectf(EAX, AL_EAXREVERB_ECHO_TIME, rev->flEchoTime);

		alEffectf(EAX, AL_EAXREVERB_ECHO_DEPTH, rev->flEchoDepth);

		alEffectf(EAX, AL_EAXREVERB_MODULATION_TIME, rev->flModulationTime);

		alEffectf(EAX, AL_EAXREVERB_MODULATION_DEPTH, rev->flModulationDepth);

		alEffectf(EAX, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, rev->flAirAbsorptionGainHF);

		alEffectf(EAX, AL_EAXREVERB_HFREFERENCE, rev->flHFReference);

		alEffectf(EAX, AL_EAXREVERB_LFREFERENCE, rev->flLFReference);

		alEffectf(EAX, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, rev->flRoomRolloffFactor);

		alEffecti(EAX, AL_EAXREVERB_DECAY_HFLIMIT, rev->iDecayHFLimit);

	}

	else

	{

		//common->Printf("Using Standard Reverb\n");



		/* No EAX Reverb. Set the standard reverb effect type then load the

		 * available reverb properties. */

		alEffecti(EAX, AL_EFFECT_TYPE, AL_EFFECT_REVERB);



		alEffectf(EAX, AL_REVERB_DENSITY, rev->flDensity);

		alEffectf(EAX, AL_REVERB_DIFFUSION, rev->flDiffusion);

		alEffectf(EAX, AL_REVERB_GAIN, rev->flGain);

		alEffectf(EAX, AL_REVERB_GAINHF, rev->flGainHF);

		alEffectf(EAX, AL_REVERB_DECAY_TIME, rev->flDecayTime);

		alEffectf(EAX, AL_REVERB_DECAY_HFRATIO, rev->flDecayHFRatio);

		alEffectf(EAX, AL_REVERB_REFLECTIONS_GAIN, rev->flReflectionsGain);

		alEffectf(EAX, AL_REVERB_REFLECTIONS_DELAY, rev->flReflectionsDelay);

		alEffectf(EAX, AL_REVERB_LATE_REVERB_GAIN, rev->flLateReverbGain);

		alEffectf(EAX, AL_REVERB_LATE_REVERB_DELAY, rev->flLateReverbDelay);

		alEffectf(EAX, AL_REVERB_AIR_ABSORPTION_GAINHF, rev->flAirAbsorptionGainHF);

		alEffectf(EAX, AL_REVERB_ROOM_ROLLOFF_FACTOR, rev->flRoomRolloffFactor);

		alEffecti(EAX, AL_REVERB_DECAY_HFLIMIT, rev->iDecayHFLimit);

	}
}
//#endif




