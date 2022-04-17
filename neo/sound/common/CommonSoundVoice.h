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
#include "../SoundVoice.h"
#include "../sound/common/SoundSample.h"
#ifndef __COMMON_SOUNDVOICE_H__
#define __COMMON_SOUNDVOICE_H__

static const int MAX_QUEUED_BUFFERS = 3;

/*
================================================
idSoundVoice_XAudio2
================================================
*/
class idSoundVoice : public idSoundVoice_Base
{
public:

	idSoundVoice(): leadinSample(NULL),
		loopingSample(NULL),
		formatTag(0),
		numChannels(0),
		sampleRate(0),
		paused(true),
		hasVUMeter(false),
		chains(1) {}

	virtual void					Create(const idSoundSample* _leadinSample, const idSoundSample* _loopingSample, const int _channel) {}

	// Start playing at a particular point in the buffer.  Does an Update() too
	virtual void					Start(int offsetMS, int ssFlags){}

	// Stop playing.
	virtual void					Stop() {}

	// Stop consuming buffers
	virtual void					Pause() {}
	// Start consuming buffers again
	virtual void					UnPause() {}

	// Sends new position/volume/pitch information to the hardware
	virtual bool					Update() {
		return false;
	}

	// returns the RMS levels of the most recently processed block of audio, SSF_FLICKER must have been passed to Start
	virtual float					GetAmplitude() {
		return -1.0f;
	}

	// returns true if we can re-use this voice
	virtual bool					CompatibleFormat(idSoundSample* s) {
		return false;
	}

	uint32					GetSampleRate() const
	{
		return sampleRate;
	}

protected:
	friend class idSoundhardware;
	friend class idSoundSample;

	idSoundSample* leadinSample;
	idSoundSample* loopingSample;

	// These are the fields from the sample format that matter to us for voice reuse
	uint16					formatTag;
	uint16					numChannels;

	uint32					sourceVoiceRate;
	uint32					sampleRate;

	bool					hasVUMeter;
	bool					paused;
	int						channel;
	int						chains;
};

#endif

