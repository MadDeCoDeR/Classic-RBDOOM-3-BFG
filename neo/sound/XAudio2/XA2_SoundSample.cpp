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
#pragma hdrstop
#include "precompiled.h"
#include "../snd_local.h"
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libswresample/swresample.h>
}

extern idCVar s_useCompression;
extern idCVar s_noSound;

#define GPU_CONVERT_CPU_TO_CPU_CACHED_READONLY_ADDRESS( x ) x

const uint32 SOUND_MAGIC_IDMSA = 0x6D7A7274;

extern idCVar sys_lang;

/*
========================
AllocBuffer
========================
*/
static void* AllocBuffer( int size, const char* name )
{
	return Mem_Alloc( size, TAG_AUDIO );
}

/*
========================
FreeBuffer
========================
*/
static void FreeBuffer( void* p )
{
	return Mem_Free( p );
}

/*
========================
idSoundSample_XAudio2::idSoundSample_XAudio2
========================
*/
idSoundSample_XAudio2::idSoundSample_XAudio2()
{
	timestamp = FILE_NOT_FOUND_TIMESTAMP;
	loaded = false;
	neverPurge = false;
	levelLoadReferenced = false;
	
	memset( &format, 0, sizeof( format ) );
	
	totalBufferSize = 0;
	
	playBegin = 0;
	playLength = 0;
	
	lastPlayedTime = 0;
}

/*
========================
idSoundSample_XAudio2::~idSoundSample_XAudio2
========================
*/
idSoundSample_XAudio2::~idSoundSample_XAudio2()
{
	FreeData();
}

/*
========================
idSoundSample_XAudio2::WriteGeneratedSample
========================
*/
void idSoundSample_XAudio2::WriteGeneratedSample( idFile* fileOut )
{
	fileOut->WriteBig( SOUND_MAGIC_IDMSA );
	fileOut->WriteBig( timestamp );
	fileOut->WriteBig( loaded );
	fileOut->WriteBig( playBegin );
	fileOut->WriteBig( playLength );
	idWaveFile::WriteWaveFormatDirect( format, fileOut );
	fileOut->WriteBig( ( int )amplitude.Num() );
	fileOut->Write( amplitude.Ptr(), amplitude.Num() );
	fileOut->WriteBig( totalBufferSize );
	fileOut->WriteBig( ( int )buffers.Num() );
	for( int i = 0; i < buffers.Num(); i++ )
	{
		fileOut->WriteBig( buffers[ i ].numSamples );
		fileOut->WriteBig( buffers[ i ].bufferSize );
		fileOut->Write( buffers[ i ].buffer, buffers[ i ].bufferSize );
	};
}
/*
========================
idSoundSample_XAudio2::WriteAllSamples
========================
*/
void idSoundSample_XAudio2::WriteAllSamples( const idStr& sampleName )
{
	idSoundSample_XAudio2* samplePC = new idSoundSample_XAudio2();
	{
		idStrStatic< MAX_OSPATH > inName = sampleName;
		inName.Append( ".msadpcm" );
		idStrStatic< MAX_OSPATH > inName2 = sampleName;
		inName2.Append( ".wav" );
		
		idStrStatic< MAX_OSPATH > outName = "generated/";
		outName.Append( sampleName );
		outName.Append( ".idwav" );
		
		if( samplePC->LoadWav( inName ) || samplePC->LoadWav( inName2 ) )
		{
			idFile* fileOut = fileSystem->OpenFileWrite( outName, "fs_basepath" );
			samplePC->WriteGeneratedSample( fileOut );
			delete fileOut;
		}
	}
	delete samplePC;
}

/*
========================
idSoundSample_XAudio2::LoadGeneratedSound
========================
*/
bool idSoundSample_XAudio2::LoadGeneratedSample( const idStr& filename )
{
	idFileLocal fileIn( fileSystem->OpenFileReadMemory( filename ) );
	if( fileIn != NULL )
	{
		uint32 magic;
		fileIn->ReadBig( magic );
		fileIn->ReadBig( timestamp );
		fileIn->ReadBig( loaded );
		fileIn->ReadBig( playBegin );
		fileIn->ReadBig( playLength );
		idWaveFile::ReadWaveFormatDirect( format, fileIn );
		int num;
		fileIn->ReadBig( num );
		amplitude.Clear();
		amplitude.SetNum( num );
		fileIn->Read( amplitude.Ptr(), amplitude.Num() );
		fileIn->ReadBig( totalBufferSize );
		fileIn->ReadBig( num );
		buffers.SetNum( num );
		for( int i = 0; i < num; i++ )
		{
			fileIn->ReadBig( buffers[ i ].numSamples );
			fileIn->ReadBig( buffers[ i ].bufferSize );
			buffers[ i ].buffer = AllocBuffer( buffers[ i ].bufferSize, GetName() );
			fileIn->Read( buffers[ i ].buffer, buffers[ i ].bufferSize );
			buffers[ i ].buffer = GPU_CONVERT_CPU_TO_CPU_CACHED_READONLY_ADDRESS( buffers[ i ].buffer );
		}
		return true;
	}
	return false;
}
/*
========================
idSoundSample_XAudio2::Load
========================
*/
void idSoundSample_XAudio2::LoadResource()
{
	FreeData();
	
	if( idStr::Icmpn( GetName(), "_default", 8 ) == 0 )
	{
		MakeDefault();
		return;
	}
	
	if( s_noSound.GetBool() )
	{
		MakeDefault();
		return;
	}
	
	loaded = false;
	
	for( int i = 0; i < 2; i++ )
	{
		idStrStatic< MAX_OSPATH > sampleName = GetName();
		if( ( i == 0 ) && !sampleName.Replace( "/vo/", va( "/vo/%s/", sys_lang.GetString() ) ) )
		{
			i++;
		}
		idStrStatic< MAX_OSPATH > generatedName = "generated/";
		generatedName.Append( sampleName );
		
		{
			if( s_useCompression.GetBool() )
			{
				sampleName.Append( ".msadpcm" );
			}
			else
			{
				sampleName.Append( ".wav" );
			}
			generatedName.Append( ".idwav" );
		}
		//GK:First look for ogg,mp3 and flac and then for wav
		sampleName.SetFileExtension("ogg");
		loaded = LoadAll(sampleName);
		if (loaded) {
			useavi = true;
			return;
		}
		else {
			sampleName.SetFileExtension("mp3");
			loaded = LoadAll(sampleName);
			if (loaded) {
				useavi = true;
				return;
			}
			else {
				sampleName.SetFileExtension("flac");
				loaded = LoadAll(sampleName);
				if (loaded) {
					useavi = true;
					return;
				}
			}
		}
		if (s_useCompression.GetBool())
		{
			sampleName.SetFileExtension(".msadpcm");
		}
		else
		{
			sampleName.SetFileExtension(".wav");
		}
		loaded = LoadGeneratedSample( generatedName ) || LoadWav( sampleName );
		
		if( !loaded && s_useCompression.GetBool() )
		{
			sampleName.SetFileExtension( "wav" );
			loaded = LoadWav( sampleName );
		}
		
		if( loaded )
		{
			if( cvarSystem->GetCVarBool( "fs_buildresources" ) )
			{
				fileSystem->AddSamplePreload( GetName() );
				WriteAllSamples( GetName() );
				
				if( sampleName.Find( "/vo/" ) >= 0 )
				{
					for( int i = 0; i < Sys_NumLangs(); i++ )
					{
						const char* lang = Sys_Lang( i );
						if( idStr::Icmp( lang, ID_LANG_ENGLISH ) == 0 )
						{
							continue;
						}
						idStrStatic< MAX_OSPATH > locName = GetName();
						locName.Replace( "/vo/", va( "/vo/%s/", Sys_Lang( i ) ) );
						WriteAllSamples( locName );
					}
				}
			}
			useavi = false;
			return;
		}
	}
	
	if( !loaded )
	{
		// make it default if everything else fails
		MakeDefault();
	}
	return;
}
//GK:Begin
bool			idSoundSample_XAudio2::LoadAll(const idStr& name) { //GK:Decode and Demux pretty much any other audio file we are about to load
	idFileLocal f(fileSystem->OpenFileRead(name));
	if (f == NULL)
	{
		return false;
	}
	int mus_size = f->Length();
	unsigned char* musFile=new unsigned char[mus_size];
	f->Read(musFile, mus_size);
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
	if (!use_ext) {
		format.basic.formatTag = idWaveFile::FORMAT_PCM;
	}
	else {
		format.basic.formatTag = idWaveFile::FORMAT_FLOAT;
	}
	format.basic.samplesPerSec = dec_ctx->sample_rate;
	format.basic.numChannels = dec_ctx->channels;
	format.basic.avgBytesPerSec = format.basic.samplesPerSec * format_byte * format.basic.numChannels;
	format.basic.blockSize = format_byte * format.basic.numChannels;
	format.basic.bitsPerSample = format_byte * 8;
	/*voiceFormat.cbSize = 22;
	exvoice.Format = voiceFormat;
	switch (voiceFormat.nChannels) {
	case 1:
		exvoice.dwChannelMask = SPEAKER_MONO;
		break;
	case 2:
		exvoice.dwChannelMask = SPEAKER_STEREO;
		break;
	case 4:
		exvoice.dwChannelMask = SPEAKER_QUAD;
		break;
	case 5:
		exvoice.dwChannelMask = SPEAKER_5POINT1_SURROUND;
		break;
	case 7:
		exvoice.dwChannelMask = SPEAKER_7POINT1_SURROUND;
		break;
	default:
		exvoice.dwChannelMask = SPEAKER_MONO;
		break;
	}
	exvoice.Samples.wReserved = 0;
	exvoice.Samples.wSamplesPerBlock = voiceFormat.wBitsPerSample;
	exvoice.Samples.wValidBitsPerSample = voiceFormat.wBitsPerSample;
	if (!use_ext) {
		exvoice.SubFormat = KSDATAFORMAT_SUBTYPE_PCM;
	}
	else {
		exvoice.SubFormat = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;
	}
	soundSystemLocal.hardware.GetIXAudio2()->CreateSourceVoice(&pMusicSourceVoice, (WAVEFORMATEX *)&exvoice, XAUDIO2_VOICE_MUSIC);*/
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
						format.basic.numChannels,
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
	playLength = (totalBufferSize) / format.basic.blockSize;

	buffers.SetNum(1);
	buffers[0].bufferSize = totalBufferSize;
	buffers[0].numSamples = playLength;
	buffers[0].buffer = AllocBuffer(totalBufferSize, GetName());
	memcpy(buffers[0].buffer, tBuffer, offset);
	free(tBuffer);
	if (format.basic.bitsPerSample == 16)
	{
		idSwap::LittleArray((short*)buffers[0].buffer, totalBufferSize / sizeof(short));
	}
	buffers[0].buffer = GPU_CONVERT_CPU_TO_CPU_CACHED_READONLY_ADDRESS(buffers[0].buffer);
	tBuffer = NULL;
	if (swr_ctx != NULL) {
		swr_free(&swr_ctx);
	}

	avcodec_close(dec_ctx);

	av_free(avio_ctx->buffer);
	av_freep(avio_ctx);
	av_free(fmt_ctx->pb);
	avformat_close_input(&fmt_ctx);

	
	
	return true;

}
//GK:End
/*
========================
idSoundSample_XAudio2::LoadWav
========================
*/
bool idSoundSample_XAudio2::LoadWav( const idStr& filename )
{

	// load the wave
	idWaveFile wave;
	if( !wave.Open( filename ) )
	{
		return false;
	}
	
	idStrStatic< MAX_OSPATH > sampleName = filename;
	sampleName.SetFileExtension( "amp" );
	LoadAmplitude( sampleName );
	
	const char* formatError = wave.ReadWaveFormat( format );
	if( formatError != NULL )
	{
		idLib::Warning( "LoadWav( %s ) : %s", filename.c_str(), formatError );
		MakeDefault();
		return false;
	}
	timestamp = wave.Timestamp();
	
	totalBufferSize = wave.SeekToChunk( 'data' );
	
	if( format.basic.formatTag == idWaveFile::FORMAT_PCM || format.basic.formatTag == idWaveFile::FORMAT_EXTENSIBLE )
	{
	
		if( format.basic.bitsPerSample != 16 )
		{
			idLib::Warning( "LoadWav( %s ) : %s", filename.c_str(), "Not a 16 bit PCM wav file" );
			MakeDefault();
			return false;
		}
		
		playBegin = 0;
		playLength = ( totalBufferSize ) / format.basic.blockSize;
		
		buffers.SetNum( 1 );
		buffers[0].bufferSize = totalBufferSize;
		buffers[0].numSamples = playLength;
		buffers[0].buffer = AllocBuffer( totalBufferSize, GetName() );
		
		
		wave.Read( buffers[0].buffer, totalBufferSize );
		
		if( format.basic.bitsPerSample == 16 )
		{
			idSwap::LittleArray( ( short* )buffers[0].buffer, totalBufferSize / sizeof( short ) );
		}
		
		buffers[0].buffer = GPU_CONVERT_CPU_TO_CPU_CACHED_READONLY_ADDRESS( buffers[0].buffer );
		
	}
	else if( format.basic.formatTag == idWaveFile::FORMAT_ADPCM )
	{
	
		playBegin = 0;
		playLength = ( ( totalBufferSize / format.basic.blockSize ) * format.extra.adpcm.samplesPerBlock );
		
		buffers.SetNum( 1 );
		buffers[0].bufferSize = totalBufferSize;
		buffers[0].numSamples = playLength;
		buffers[0].buffer  = AllocBuffer( totalBufferSize, GetName() );
		
		wave.Read( buffers[0].buffer, totalBufferSize );
		
		buffers[0].buffer = GPU_CONVERT_CPU_TO_CPU_CACHED_READONLY_ADDRESS( buffers[0].buffer );
		
	}
	else if( format.basic.formatTag == idWaveFile::FORMAT_XMA2 )
	{
	
		if( format.extra.xma2.blockCount == 0 )
		{
			idLib::Warning( "LoadWav( %s ) : %s", filename.c_str(), "No data blocks in file" );
			MakeDefault();
			return false;
		}
		
		int bytesPerBlock = format.extra.xma2.bytesPerBlock;
		assert( format.extra.xma2.blockCount == ALIGN( totalBufferSize, bytesPerBlock ) / bytesPerBlock );
		assert( format.extra.xma2.blockCount * bytesPerBlock >= totalBufferSize );
		assert( format.extra.xma2.blockCount * bytesPerBlock < totalBufferSize + bytesPerBlock );
		
		buffers.SetNum( format.extra.xma2.blockCount );
		for( int i = 0; i < buffers.Num(); i++ )
		{
			if( i == buffers.Num() - 1 )
			{
				buffers[i].bufferSize = totalBufferSize - ( i * bytesPerBlock );
			}
			else
			{
				buffers[i].bufferSize = bytesPerBlock;
			}
			
			buffers[i].buffer = AllocBuffer( buffers[i].bufferSize, GetName() );
			wave.Read( buffers[i].buffer, buffers[i].bufferSize );
			buffers[i].buffer = GPU_CONVERT_CPU_TO_CPU_CACHED_READONLY_ADDRESS( buffers[i].buffer );
		}
		
		int seekTableSize = wave.SeekToChunk( 'seek' );
		if( seekTableSize != 4 * buffers.Num() )
		{
			idLib::Warning( "LoadWav( %s ) : %s", filename.c_str(), "Wrong number of entries in seek table" );
			MakeDefault();
			return false;
		}
		
		for( int i = 0; i < buffers.Num(); i++ )
		{
			wave.Read( &buffers[i].numSamples, sizeof( buffers[i].numSamples ) );
			idSwap::Big( buffers[i].numSamples );
		}
		
		playBegin = format.extra.xma2.loopBegin;
		playLength = format.extra.xma2.loopLength;
		
		if( buffers[buffers.Num() - 1].numSamples < playBegin + playLength )
		{
			// This shouldn't happen, but it's not fatal if it does
			playLength = buffers[buffers.Num() - 1].numSamples - playBegin;
		}
		else
		{
			// Discard samples beyond playLength
			for( int i = 0; i < buffers.Num(); i++ )
			{
				if( buffers[i].numSamples > playBegin + playLength )
				{
					buffers[i].numSamples = playBegin + playLength;
					// Ideally, the following loop should always have 0 iterations because playBegin + playLength ends in the last block already
					// But there is no guarantee for that, so to be safe, discard all buffers beyond this one
					for( int j = i + 1; j < buffers.Num(); j++ )
					{
						FreeBuffer( buffers[j].buffer );
					}
					buffers.SetNum( i + 1 );
					break;
				}
			}
		}
		
	}
	else
	{
		idLib::Warning( "LoadWav( %s ) : Unsupported wave format %d", filename.c_str(), format.basic.formatTag );
		MakeDefault();
		return false;
	}
	
	wave.Close();
	
	if( format.basic.formatTag == idWaveFile::FORMAT_EXTENSIBLE )
	{
		// HACK: XAudio2 doesn't really support FORMAT_EXTENSIBLE so we convert it to a basic format after extracting the channel mask
		format.basic.formatTag = format.extra.extensible.subFormat.data1;
	}
	
	// sanity check...
	assert( buffers[buffers.Num() - 1].numSamples == playBegin + playLength );
	
	return true;
}


/*
========================
idSoundSample_XAudio2::MakeDefault
========================
*/
void idSoundSample_XAudio2::MakeDefault()
{
	FreeData();
	
	static const int DEFAULT_NUM_SAMPLES = 256;
	
	timestamp = FILE_NOT_FOUND_TIMESTAMP;
	loaded = true;
	
	memset( &format, 0, sizeof( format ) );
	format.basic.formatTag = idWaveFile::FORMAT_PCM;
	format.basic.numChannels = 1;
	format.basic.bitsPerSample = 16;
	format.basic.samplesPerSec = XAUDIO2_MIN_SAMPLE_RATE;
	format.basic.blockSize = format.basic.numChannels * format.basic.bitsPerSample / 8;
	format.basic.avgBytesPerSec = format.basic.samplesPerSec * format.basic.blockSize;
	
	assert( format.basic.blockSize == 2 );
	
	totalBufferSize = DEFAULT_NUM_SAMPLES * 2;
	
	short* defaultBuffer = ( short* )AllocBuffer( totalBufferSize, GetName() );
	for( int i = 0; i < DEFAULT_NUM_SAMPLES; i += 2 )
	{
		defaultBuffer[i + 0] = SHRT_MIN;
		defaultBuffer[i + 1] = SHRT_MAX;
	}
	
	buffers.SetNum( 1 );
	buffers[0].buffer = defaultBuffer;
	buffers[0].bufferSize = totalBufferSize;
	buffers[0].numSamples = DEFAULT_NUM_SAMPLES;
	buffers[0].buffer = GPU_CONVERT_CPU_TO_CPU_CACHED_READONLY_ADDRESS( buffers[0].buffer );
	
	playBegin = 0;
	playLength = DEFAULT_NUM_SAMPLES;
}

/*
========================
idSoundSample_XAudio2::FreeData

Called before deleting the object and at the start of LoadResource()
========================
*/
void idSoundSample_XAudio2::FreeData()
{
	if( buffers.Num() > 0 )
	{
		soundSystemLocal.StopVoicesWithSample( ( idSoundSample* )this );
		for( int i = 0; i < buffers.Num(); i++ )
		{
			FreeBuffer( buffers[i].buffer );
		}
		buffers.Clear();
	}
	amplitude.Clear();
	
	timestamp = FILE_NOT_FOUND_TIMESTAMP;
	memset( &format, 0, sizeof( format ) );
	loaded = false;
	totalBufferSize = 0;
	playBegin = 0;
	playLength = 0;
}

/*
========================
idSoundSample_XAudio2::LoadAmplitude
========================
*/
bool idSoundSample_XAudio2::LoadAmplitude( const idStr& name )
{
	amplitude.Clear();
	idFileLocal f( fileSystem->OpenFileRead( name ) );
	if( f == NULL )
	{
		return false;
	}
	amplitude.SetNum( f->Length() );
	f->Read( amplitude.Ptr(), amplitude.Num() );
	return true;
}

/*
========================
idSoundSample_XAudio2::GetAmplitude
========================
*/
float idSoundSample_XAudio2::GetAmplitude( int timeMS ) const
{
	if( timeMS < 0 || timeMS > LengthInMsec() )
	{
		return 0.0f;
	}
	if( IsDefault() )
	{
		return 1.0f;
	}
	int index = timeMS * 60 / 1000;
	if( index < 0 || index >= amplitude.Num() )
	{
		return 0.0f;
	}
	return ( float )amplitude[index] / 255.0f;
}
