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
#pragma hdrstop
#include "precompiled.h"
#include "../snd_local.h"

extern idCVar s_skipHardwareSets;
extern idCVar s_debugHardware;

// The whole system runs at this sample rate
static int SYSTEM_SAMPLE_RATE = 44100;
static float ONE_OVER_SYSTEM_SAMPLE_RATE = 1.0f / SYSTEM_SAMPLE_RATE;



/*
========================
idSoundVoice_OpenAL::idSoundVoice_OpenAL
========================
*/
idSoundVoice_OpenAL::idSoundVoice_OpenAL()
	:
	triggered( false ),
	openalSource( 0 ), idSoundVoice()
{

}

/*
========================
idSoundVoice_OpenAL::~idSoundVoice_OpenAL
========================
*/
idSoundVoice_OpenAL::~idSoundVoice_OpenAL()
{
	DestroyInternal();
}

/*
========================
idSoundVoice_OpenAL::CompatibleFormat
========================
*/
bool idSoundVoice_OpenAL::CompatibleFormat( idSoundSample* s )
{
	if( alIsSource( openalSource ) )
	{
		// If this voice has never been allocated, then it's compatible with everything
		return true;
	}
	
	return false;
}

/*
========================
idSoundVoice_OpenAL::Create
========================
*/
void idSoundVoice_OpenAL::Create( const idSoundSample* leadinSample_, const idSoundSample* loopingSample_,const int channel_ )
{
	if( IsPlaying() )
	{
		// This should never hit
		Stop();
		return;
	}
	
	triggered = true;
	
	leadinSample = ( idSoundSample_OpenAL* )leadinSample_;
	loopingSample = ( idSoundSample_OpenAL* )loopingSample_;
	channel = channel_;
	
	if( alIsSource( openalSource ) && CompatibleFormat( (idSoundSample_OpenAL*)leadinSample ) )
	{
		sampleRate = leadinSample->GetFormat().basic.samplesPerSec;
	}
	else
	{
		DestroyInternal();
		formatTag = leadinSample->GetFormat().basic.formatTag;
		numChannels = leadinSample->GetFormat().basic.numChannels;
		sampleRate = leadinSample->GetFormat().basic.samplesPerSec;
		
		//soundSystemLocal.hardware.pXAudio2->CreateSourceVoice( &pSourceVoice, ( const WAVEFORMATEX* )&leadinSample->format, XAUDIO2_VOICE_USEFILTER, 4.0f, &streamContext );
		
		CheckALErrors();
		
		alGenSources( 1, &openalSource );
		if( CheckALErrors() != AL_NO_ERROR )
			//if( pSourceVoice == NULL )
		{
			// If this hits, then we are most likely passing an invalid sample format, which should have been caught by the loader (and the sample defaulted)
			return;
		}
		
		alSourcef( openalSource, AL_ROLLOFF_FACTOR, 0.0f );
		
		//if( ( loopingSample == NULL && leadinSample->openalBuffer != 0 ) || ( loopingSample != NULL && soundShader->entries[0]->hardwareBuffer ) )
		if( ((idSoundSample_OpenAL*)leadinSample)->openalBuffer != 0 )
		{
			alSourcei( openalSource, AL_BUFFER, 0 );
			
			// handle uncompressed (non streaming) single shot and looping sounds
			/*
			if( triggered )
			{
				alSourcei( openalSource, AL_BUFFER, looping ? chan->soundShader->entries[0]->openalBuffer : leadinSample->openalBuffer );
			}
			*/
		}
		else
		{
			//if( triggered )
			
			// handle streaming sounds (decode on the fly) both single shot AND looping
			
			alSourcei( openalSource, AL_BUFFER, 0 );
			alDeleteBuffers( 3, &lastopenalStreamingBuffer[0] );
			lastopenalStreamingBuffer[0] = openalStreamingBuffer[0];
			lastopenalStreamingBuffer[1] = openalStreamingBuffer[1];
			lastopenalStreamingBuffer[2] = openalStreamingBuffer[2];
			
			alGenBuffers( 3, &openalStreamingBuffer[0] );
			/*
			if( soundSystemLocal.alEAXSetBufferMode )
			{
				soundSystemLocal.alEAXSetBufferMode( 3, &chan->openalStreamingBuffer[0], alGetEnumValue( ID_ALCHAR "AL_STORAGE_ACCESSIBLE" ) );
			}
			*/
			openalStreamingBuffer[0];
			openalStreamingBuffer[1];
			openalStreamingBuffer[2];
		}
		
		if( s_debugHardware.GetBool() )
		{
			if( loopingSample == NULL || loopingSample == leadinSample )
			{
				idLib::Printf( "%dms: %i created for %s\n", Sys_Milliseconds(), openalSource, leadinSample ? leadinSample->GetName() : "<null>" );
			}
			else
			{
				idLib::Printf( "%dms: %i created for %s and %s\n", Sys_Milliseconds(), openalSource, leadinSample ? leadinSample->GetName() : "<null>", loopingSample ? loopingSample->GetName() : "<null>" );
			}
		}
	}
	
	sourceVoiceRate = sampleRate;
	//pSourceVoice->SetSourceSampleRate( sampleRate );
	//pSourceVoice->SetVolume( 0.0f );
	
	alSourcei( openalSource, AL_SOURCE_RELATIVE, AL_TRUE );
	alSource3f( openalSource, AL_POSITION, 0.0f, 0.0f, 0.0f );
	float orientation[6];
	orientation[0] = 0.0f;
	orientation[1] = 0.0f;
	orientation[2] = -1.0f;
	orientation[3] = 0.0f;
	orientation[4] = 1.0f;
	orientation[5] = 0.0f;
	alSourcefv(openalSource, AL_ORIENTATION, orientation);
	
		// RB: FIXME 0.0f ? GK: Not needed anymore, the issue was with the buffers and is fixed long time ago
		//alSourcef( openalSource, AL_GAIN, 1.0f );
	
	//OnBufferStart( leadinSample, 0 );
}

/*
========================
idSoundVoice_OpenAL::DestroyInternal
========================
*/
void idSoundVoice_OpenAL::DestroyInternal()
{
	if( alIsSource( openalSource ) )
	{
		alSourcei(openalSource, AL_BUFFER, 0);

		alDeleteSources(1, &openalSource);

		if (CheckALErrors() == AL_NO_ERROR) {
			openalSource = 0;
		}
		if( s_debugHardware.GetBool() )
		{
			idLib::Printf( "%dms: %i destroyed\n", Sys_Milliseconds(), openalSource );
		}
		for (int i = 0; i < 3; i++) {
			if (alIsBuffer(openalStreamingBuffer[i]))
			{

				alDeleteBuffers(1, &openalStreamingBuffer[i]);

				if (CheckALErrors() == AL_NO_ERROR)
				{
					openalStreamingBuffer[i] = 0;
				}
			}

			if (alIsBuffer(lastopenalStreamingBuffer[i]))
			{

				alDeleteBuffers(1, &lastopenalStreamingBuffer[i]);
				if (CheckALErrors() == AL_NO_ERROR)
				{
					lastopenalStreamingBuffer[i] = 0;
				}
			}
		}
		
		
		
		
		openalStreamingOffset = 0;
		
		hasVUMeter = false;
	}
}

/*
========================
idSoundVoice_OpenAL::Start
========================
*/
void idSoundVoice_OpenAL::Start( int offsetMS, int ssFlags )
{
	if( s_debugHardware.GetBool() )
	{
		idLib::Printf( "%dms: %i starting %s @ %dms\n", Sys_Milliseconds(), openalSource, leadinSample ? leadinSample->GetName() : "<null>", offsetMS );
	}
	
	if( !leadinSample )
	{
		return;
	}
	
	if( !alIsSource( openalSource ) )
	{
		return;
	}
	
	if( leadinSample->IsDefault() && !leadinSample->useavi) //GK: I kinda have no idea how to get the timestamp using FFMPEG and so I'm using the useavi in order to actually play the audio
	{
		idLib::Warning( "Starting defaulted sound sample %s", leadinSample->GetName() );
	}
	
	bool flicker = ( ssFlags & SSF_NO_FLICKER ) == 0;
	
	if( flicker != hasVUMeter )
	{
		hasVUMeter = flicker;
	}
	
	assert( offsetMS >= 0 );
	int offsetSamples = MsecToSamples( offsetMS, leadinSample->SampleRate() );
	if( loopingSample == NULL && offsetSamples >= leadinSample->GetPlayLength() )
	{
		return;
	}
	
	RestartAt( offsetSamples );
	Update();
	UnPause();
}

/*
========================
idSoundVoice_OpenAL::RestartAt
========================
*/
int idSoundVoice_OpenAL::RestartAt( int offsetSamples )
{
	offsetSamples &= ~127;
	
	idSoundSample_OpenAL* sample = (idSoundSample_OpenAL*)leadinSample;
	if( offsetSamples >= leadinSample->GetPlayLength() )
	{
		if( loopingSample != NULL )
		{
			offsetSamples %= loopingSample->GetPlayLength();
			sample = (idSoundSample_OpenAL*)loopingSample;
		}
		else
		{
			return 0;
		}
	}
	
	int previousNumSamples = 0;
	for( int i = 0; i < sample->buffers.Num(); i++ )
	{
		if( sample->buffers[i].numSamples > sample->playBegin + offsetSamples )
		{
			return SubmitBuffer( sample, i, sample->playBegin + offsetSamples - previousNumSamples );
		}
		previousNumSamples = sample->buffers[i].numSamples;
	}
	
	return 0;
}

/*
========================
idSoundVoice_OpenAL::SubmitBuffer
========================
*/
int idSoundVoice_OpenAL::SubmitBuffer( idSoundSample_OpenAL* sample, int bufferNumber, int offset )
{
	if( sample == NULL || ( bufferNumber < 0 ) || ( bufferNumber >= sample->buffers.Num() ) )
	{
		return 0;
	}
	
#if 0
	idSoundSystemLocal::bufferContext_t* bufferContext = soundSystemLocal.ObtainStreamBufferContext();
	if( bufferContext == NULL )
	{
		idLib::Warning( "No free buffer contexts!" );
		return 0;
	}
	
	bufferContext->voice = this;
	bufferContext->sample = sample;
	bufferContext->bufferNumber = bufferNumber;
#endif

	if( sample->openalBuffer > 0 )
	{
		
		alSourcei( openalSource, AL_BUFFER, sample->openalBuffer );
		alSourcei( openalSource, AL_LOOPING, ( sample == loopingSample && loopingSample != NULL ? AL_TRUE : AL_FALSE ) );
		//if (offset > 0)
		//{
		//	//alSourceRewind(openalSource);
		//	alSourcei(openalSource, AL_BYTE_OFFSET, offset);
		//}
		return sample->totalBufferSize;
	}
	else
	{
		//GK: Check also here for looping samples, since music samples fail to check it otherwise
		alSourcei(openalSource, AL_LOOPING, (sample == loopingSample && loopingSample != NULL ? AL_TRUE : AL_FALSE));
		ALint finishedbuffers;
		
		if( !triggered )
		{
			alGetSourcei( openalSource, AL_BUFFERS_PROCESSED, &finishedbuffers );
			alSourceUnqueueBuffers( openalSource, finishedbuffers, &openalStreamingBuffer[0] );
			if( finishedbuffers == 3 )
			{
				triggered = true;
			}
		}
		else
		{
			finishedbuffers = 3;
		}
		//GK: Just make sure we don't get 0 buffers because it's result on silent audio
		if (openalStreamingBuffer[0] == 0u && openalStreamingBuffer[1] == 0u && openalStreamingBuffer[2] == 0u) {
			alGenBuffers(3, &openalStreamingBuffer[0]);
		}
		ALenum format;
		
		if( sample->format.basic.formatTag == idWaveFile::FORMAT_PCM || sample->format.basic.formatTag == idWaveFile::FORMAT_FLOAT)
		{
			format = sample->NumChannels() == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
			if (sample->useavi) { //GK: Decode FFMPEG audio sample
				int format_byte = sample->format.basic.bitsPerSample;
				switch (format_byte) {
				case 8:
					format = sample->NumChannels() == 1 ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
					break;
				case 16:
					format = sample->NumChannels() == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
					break;
				case 32:
					format = sample->NumChannels() == 1 ? AL_FORMAT_MONO_FLOAT32 : AL_FORMAT_STEREO_FLOAT32;
					break;
				}
			}
		}
		else if( sample->format.basic.formatTag == idWaveFile::FORMAT_ADPCM )
		{
			format = sample->NumChannels() == 1 ? AL_FORMAT_MONO_MSADPCM_SOFT : AL_FORMAT_STEREO_MSADPCM_SOFT;
		}
		else if( sample->format.basic.formatTag == idWaveFile::FORMAT_XMA2 )
		{
			format = sample->NumChannels() == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		}
		else
		{
			format = sample->NumChannels() == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		}
		
		int rate = sample->SampleRate(); /*44100*/
		if (sample->useavi) {
			rate = sample->format.basic.samplesPerSec;
		}
		
		for( int j = 0; j < finishedbuffers && j < 1; j++ )
		{
			/*
			chan->GatherChannelSamples( chan->openalStreamingOffset * sample->objectInfo.nChannels, MIXBUFFER_SAMPLES * sample->objectInfo.nChannels, alignedInputSamples );
			for( int i = 0; i < ( MIXBUFFER_SAMPLES * sample->objectInfo.nChannels ); i++ )
			{
				if( alignedInputSamples[i] < -32768.0f )
					( ( short* )alignedInputSamples )[i] = -32768;
				else if( alignedInputSamples[i] > 32767.0f )
					( ( short* )alignedInputSamples )[i] = 32767;
				else
					( ( short* )alignedInputSamples )[i] = idMath::FtoiFast( alignedInputSamples[i] );
			}
			*/
			
			//alBufferData( buffers[0], sample->NumChannels() == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, sample->buffers[bufferNumber].buffer, sample->buffers[bufferNumber].bufferSize, sample->SampleRate() /*44100*/ );
			
			
			
			if (sample->format.basic.formatTag == idWaveFile::FORMAT_ADPCM) {
				alBufferi(openalStreamingBuffer[j], AL_UNPACK_BLOCK_ALIGNMENT_SOFT, sample->format.extra.adpcm.samplesPerBlock);
			}
			alBufferData( openalStreamingBuffer[j], format, sample->buffers[bufferNumber].buffer, sample->buffers[bufferNumber].bufferSize, rate );
			//openalStreamingOffset += MIXBUFFER_SAMPLES;
		}

		//if (offset > 0)
		//{
		//	//alSourceRewind(openalSource);
		//	alSourcei(openalSource, AL_BYTE_OFFSET, offset);
		//}
		
		if( finishedbuffers > 0 )
		{
			//alSourceQueueBuffers( openalSource, finishedbuffers, &buffers[0] );
			alSourceQueueBuffers( openalSource, 1, &openalStreamingBuffer[0] );
			
			if( bufferNumber == 0 )
			{
				//alSourcePlay( openalSource );
				triggered = false;
			}
			
			return sample->buffers[bufferNumber].bufferSize;
		}
	}
	
	// should never happen
	return 0;
	
}

/*
========================
idSoundVoice_OpenAL::Update
========================
*/
bool idSoundVoice_OpenAL::Update()
{
	if (!alIsSource(openalSource)) {
		return false;
	}
	//GK: Set the EFX in the last moment
	alSource3i(openalSource, AL_AUXILIARY_SEND_FILTER, AL_EFFECTSLOT_NULL, 0, AL_FILTER_NULL);
	alSource3i(openalSource, AL_AUXILIARY_SEND_FILTER, AL_EFFECTSLOT_NULL, 1, AL_FILTER_NULL);
	if (alIsEffectRef(((idSoundHardware_OpenAL*)soundSystemLocal.hardware)->EAX) && ((idSoundHardware_OpenAL*)soundSystemLocal.hardware)->EAX > 0) { //GK: OpenAL thinks that 0 is valid effect
		if (GetOcclusion() > 0.0f) {
			alSourcei(openalSource, AL_DIRECT_FILTER, ((idSoundHardware_OpenAL*)soundSystemLocal.hardware)->voicefilter);
		}
		//GK: Audio Logs, PDA Videos and Radio Comms are supposed to be produced by the suit. 
		//And they should not blend with Room's reverb (Plus some of these reverbs are making the voices harder to understand)
		if (channel == 9 || channel == 10 || channel == 12) {
			alSource3i(openalSource, AL_AUXILIARY_SEND_FILTER, ((idSoundHardware_OpenAL*)soundSystemLocal.hardware)->voiceslot, 1, AL_FILTER_NULL);
		}
		else {
			alSource3i(openalSource, AL_AUXILIARY_SEND_FILTER, ((idSoundHardware_OpenAL*)soundSystemLocal.hardware)->slot, 0, GetOcclusion() > 0.0f ? ((idSoundHardware_OpenAL*)soundSystemLocal.hardware)->voicefilter : AL_FILTER_NULL);
		}
	}
	return true;
}

/*
========================
idSoundVoice_OpenAL::IsPlaying
========================
*/
bool idSoundVoice_OpenAL::IsPlaying()
{
	if( !alIsSource( openalSource ) )
	{
		return false;
	}
	
	ALint state = AL_INITIAL;
	
	alGetSourcei( openalSource, AL_SOURCE_STATE, &state );
	
	return ( state == AL_PLAYING );
	
	//XAUDIO2_VOICE_STATE state;
	//pSourceVoice->GetState( &state );
	
	//return ( state.BuffersQueued != 0 );
}

/*
========================
idSoundVoice_OpenAL::FlushSourceBuffers
========================
*/
void idSoundVoice_OpenAL::FlushSourceBuffers()
{
	if( alIsSource( openalSource ) )
	{
		
		//alSourcei(openalSource, AL_BUFFER, 0);
		/*alDeleteBuffers(3, openalStreamingBuffer);
		openalStreamingBuffer[0] = openalStreamingBuffer[1] = openalStreamingBuffer[2] = 0;*/
	}
}

/*
========================
idSoundVoice_OpenAL::Pause
========================
*/
void idSoundVoice_OpenAL::Pause()
{
	if( !alIsSource( openalSource ) || paused )
	{
		return;
	}
	
	if( s_debugHardware.GetBool() )
	{
		idLib::Printf( "%dms: %i pausing %s\n", Sys_Milliseconds(), openalSource, leadinSample ? leadinSample->GetName() : "<null>" );
	}
	
	alSourcePause( openalSource );
	//pSourceVoice->Stop( 0, OPERATION_SET );
	paused = true;
}

/*
========================
idSoundVoice_OpenAL::UnPause
========================
*/
void idSoundVoice_OpenAL::UnPause()
{
	if( !alIsSource( openalSource ) || !paused )
	{
		return;
	}
	
	if( s_debugHardware.GetBool() )
	{
		idLib::Printf( "%dms: %i unpausing %s\n", Sys_Milliseconds(), openalSource, leadinSample ? leadinSample->GetName() : "<null>" );
	}
	alSourcePlay( openalSource );
	//pSourceVoice->Start( 0, OPERATION_SET );
	paused = false;
}

/*
========================
idSoundVoice_OpenAL::Stop
========================
*/
void idSoundVoice_OpenAL::Stop()
{
	if( !alIsSource( openalSource ) )
	{
		return;
	}
	
	if( !paused )
	{
		if( s_debugHardware.GetBool() )
		{
			idLib::Printf( "%dms: %i stopping %s\n", Sys_Milliseconds(), openalSource, leadinSample ? leadinSample->GetName() : "<null>" );
		}
		
		alSourceStop( openalSource );
		alSourcei(openalSource, AL_BUFFER, 0);
		
		//pSourceVoice->Stop( 0, OPERATION_SET );
		paused = true;
	}
}

/*
========================
idSoundVoice_OpenAL::GetAmplitude
========================
*/
float idSoundVoice_OpenAL::GetAmplitude()
{
	// TODO
	return 1.0f;
	
	/*
	if( !hasVUMeter )
	{
		return 1.0f;
	}
	
	float peakLevels[ MAX_CHANNELS_PER_VOICE ];
	float rmsLevels[ MAX_CHANNELS_PER_VOICE ];
	
	XAUDIO2FX_VOLUMEMETER_LEVELS levels;
	levels.ChannelCount = leadinSample->NumChannels();
	levels.pPeakLevels = peakLevels;
	levels.pRMSLevels = rmsLevels;
	
	if( levels.ChannelCount > MAX_CHANNELS_PER_VOICE )
	{
		levels.ChannelCount = MAX_CHANNELS_PER_VOICE;
	}
	
	if( pSourceVoice->GetEffectParameters( 0, &levels, sizeof( levels ) ) != S_OK )
	{
		return 0.0f;
	}
	
	if( levels.ChannelCount == 1 )
	{
		return rmsLevels[0];
	}
	
	float rms = 0.0f;
	for( uint32 i = 0; i < levels.ChannelCount; i++ )
	{
		rms += rmsLevels[i];
	}
	
	return rms / ( float )levels.ChannelCount;
	*/
}

/*
========================
idSoundVoice_OpenAL::ResetSampleRate
========================
*/
void idSoundVoice_OpenAL::SetSampleRate( uint32 newSampleRate, uint32 operationSet )
{

}

/*
========================
idSoundVoice_OpenAL::OnBufferStart
========================
*/
void idSoundVoice_OpenAL::OnBufferStart( idSoundSample* sample, int bufferNumber )
{
	//SetSampleRate( sample->SampleRate(), XAUDIO2_COMMIT_NOW );
	
	idSoundSample_OpenAL* nextSample = (idSoundSample_OpenAL*)sample;
	int nextBuffer = bufferNumber + 1;
	if( nextBuffer == sample->GetBuffers().Num() )
	{
		if( sample == leadinSample )
		{
			if( loopingSample == NULL )
			{
				return;
			}
			nextSample = (idSoundSample_OpenAL*)loopingSample;
		}
		nextBuffer = 0;
	}
	
	SubmitBuffer( nextSample, nextBuffer, 0 );
}
