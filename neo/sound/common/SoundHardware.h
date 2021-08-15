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
#include "../sound/sound.h"
#include "../sound/common/CommonSoundVoice.h"
#include "../sound/snd_local.h"
#ifndef __SOUNDHARDWARE_H__
#define __SOUNDHARDWARE_H__

class idSoundVoice;
class idSoundSample;

class idSoundHardware
{
public:

	idSoundHardware() {
		vuMeterRMS = NULL;
		vuMeterPeak = NULL;

		outputChannels = 0;
		channelMask = 0;

		lastResetTime = 0;
	}

	virtual void			Init() = 0;
	virtual void			Shutdown() = 0;

	virtual void			ShutdownReverbSystem() = 0;

	virtual void 			Update() = 0;

	virtual void			UpdateEAXEffect(idSoundEffect* effect) = 0;

	virtual idSoundVoice* AllocateVoice(const idSoundSample* leadinSample, const idSoundSample* loopingSample, const int channel) = 0;
	virtual void			FreeVoice(idSoundVoice* voice) = 0;

	virtual int				GetNumZombieVoices() const = 0;
	virtual int				GetNumFreeVoices() const = 0;

	virtual bool			IsReverbSupported() = 0;

protected:
	friend class idSoundSample;
	friend class idSoundVoice;
	int					lastResetTime;

	int					outputChannels;
	int					channelMask;

	idDebugGraph* vuMeterRMS;
	idDebugGraph* vuMeterPeak;
	int					vuMeterPeakTimes[8];

	
};
#endif
