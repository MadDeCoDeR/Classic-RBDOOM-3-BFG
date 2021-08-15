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
#include <precompiled.h>
#include "../snd_local.h"

#ifndef __SOUNDSAMPLE_H__
#define __SOUNDSAMPLE_H__

/*
================================================
idSoundSample
================================================
*/
class idSoundSample
{
public:
	struct sampleBuffer_t
	{
		void* buffer;
		int bufferSize;
		int numSamples;
	};
	bool useavi = false; //GK:Keep track on whenever we are about to load non wav audio files
	// Loads and initializes the resource based on the name.
	virtual void	 LoadResource() = 0;

	void			SetName(const char* n)
	{
		name = n;
	}
	const char* GetName() const
	{
		return name;
	}
	ID_TIME_T		GetTimestamp() const
	{
		return timestamp;
	}


	// turns it into a beep
	virtual void			MakeDefault() = 0;

	// frees all data
	virtual void			FreeData() = 0;

	int				LengthInMsec() const
	{
		return SamplesToMsec(NumSamples(), SampleRate());
	}
	int				SampleRate() const
	{
		return format.basic.samplesPerSec;
	}
	int				NumSamples() const
	{
		return playLength;
	}
	int				NumChannels() const
	{
		return format.basic.numChannels;
	}
	int				BufferSize() const
	{
		return totalBufferSize;
	}

	bool			IsCompressed() const
	{
		return (format.basic.formatTag != idWaveFile::FORMAT_PCM);
	}

	bool			IsDefault() const
	{
		return timestamp == FILE_NOT_FOUND_TIMESTAMP;
	}
	bool			IsLoaded() const
	{
		return loaded;
	}

	void			SetNeverPurge()
	{
		neverPurge = true;
	}
	bool			GetNeverPurge() const
	{
		return neverPurge;
	}

	void			SetLevelLoadReferenced()
	{
		levelLoadReferenced = true;
	}
	void			ResetLevelLoadReferenced()
	{
		levelLoadReferenced = false;
	}
	bool			GetLevelLoadReferenced() const
	{
		return levelLoadReferenced;
	}

	int				GetLastPlayedTime() const
	{
		return lastPlayedTime;
	}
	void			SetLastPlayedTime(int t)
	{
		lastPlayedTime = t;
	}

	idWaveFile::waveFmt_t GetFormat() const{
		return format;
	}

	idList<sampleBuffer_t, TAG_AUDIO> GetBuffers() {
		return buffers;
	}

	int GetPlayLength() {
		return playLength;
	}


	virtual float			GetAmplitude(int timeMS) const = 0;
protected:
	friend class idSoundHardware;
	friend class idSoundVoice;

	virtual bool			LoadWav(const idStr& name) = 0;
	virtual bool			LoadAll(const idStr& name) = 0;
	virtual bool			LoadAmplitude(const idStr& name) = 0;
	virtual void			WriteAllSamples(const idStr& sampleName) = 0;
	virtual bool			LoadGeneratedSample(const idStr& name) = 0;
	virtual void			WriteGeneratedSample(idFile* fileOut) = 0;

	

	idStr			name;

	ID_TIME_T		timestamp;
	bool			loaded;

	bool			neverPurge;
	bool			levelLoadReferenced;
	bool			usesMapHeap;

	uint32			lastPlayedTime;

	int				totalBufferSize;	// total size of all the buffers
	idList<sampleBuffer_t, TAG_AUDIO> buffers;

	int				playBegin;
	int				playLength;

	idWaveFile::waveFmt_t	format;

	idList<byte, TAG_AMPLITUDE> amplitude;
};

#endif
