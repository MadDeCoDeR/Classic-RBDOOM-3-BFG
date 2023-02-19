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
#include "precompiled.h"
#include "AL_CinematicAudio.h"
#include <sound/snd_local.h>
#include <sound/AVD.h>

extern idCVar s_noSound;
extern idCVar s_volume_dB;

CinematicAudio_OpenAL::CinematicAudio_OpenAL():
	av_rate_cin(0),
	av_sample_cin(0),
	offset(0),
	trigger(false)
{
	alGenSources(1, &alMusicSourceVoicecin);

	alSource3i(alMusicSourceVoicecin, AL_POSITION, 0, 0, 0);
	alSourcei(alMusicSourceVoicecin, AL_SOURCE_RELATIVE, AL_TRUE);
	alSourcei(alMusicSourceVoicecin, AL_ROLLOFF_FACTOR, 0);
	alListenerf(AL_GAIN, s_noSound.GetBool() ? 0.0f : DBtoLinear(s_volume_dB.GetFloat())); //GK: Set the sound volume the same that is used in DOOM 3
	alGenBuffers(NUM_BUFFERS, &alMusicBuffercin[0]);
}

void CinematicAudio_OpenAL::InitAudio(void* audioContext)
{
	AVCodecContext* dec_ctx2 = (AVCodecContext*)audioContext;
	av_rate_cin = dec_ctx2->sample_rate;
	int format_byte = 0;
	bool use_ext = false;
	switch (dec_ctx2->sample_fmt) {
	case AV_SAMPLE_FMT_U8:
	case AV_SAMPLE_FMT_U8P:
		format_byte = 1;
		break;
	case AV_SAMPLE_FMT_S16:
	case AV_SAMPLE_FMT_S16P:
		format_byte = 2;
		break;
	case AV_SAMPLE_FMT_S32:
	case AV_SAMPLE_FMT_S32P:
		format_byte = 4;
		break;
	default:
		format_byte = 4;
		use_ext = true;
	}
	switch (format_byte) {
	case 1:
		av_sample_cin = dec_ctx2->ch_layout.nb_channels == 2 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
		break;
	case 2:
		av_sample_cin = dec_ctx2->ch_layout.nb_channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
		break;
	case 4:
		av_sample_cin = dec_ctx2->ch_layout.nb_channels == 2 ? AL_FORMAT_STEREO_FLOAT32 : AL_FORMAT_MONO_FLOAT32;
		break;
	}
	common->Printf("Video audio stream found:\n\tSample Rate: %dHz\n\tSample Format: %s\n", av_rate_cin, GetSampleName(av_sample_cin));
	alSourceRewind(alMusicSourceVoicecin);
	alSourcei(alMusicSourceVoicecin, AL_BUFFER, 0);
	offset = 0;
	trigger = false;
}

void CinematicAudio_OpenAL::PlayAudio(uint8_t* data, int size)
{
	ALint processed, state;

	alGetSourcei(alMusicSourceVoicecin, AL_SOURCE_STATE, &state);
	alGetSourcei(alMusicSourceVoicecin, AL_BUFFERS_PROCESSED, &processed);

	if (trigger) {
		tBuffer->push(data);
		sizes->push(size);
		while (processed > 0) {
			ALuint bufid;

			alSourceUnqueueBuffers(alMusicSourceVoicecin, 1, &bufid);
			processed--;
			if (!tBuffer->empty()) {
				int tempSize = sizes->front();
				sizes->pop();
				uint8_t* tempdata = tBuffer->front();
				tBuffer->pop();
				if (tempSize > 0) {
					alBufferData(bufid, av_sample_cin, tempdata, tempSize, av_rate_cin);
					alSourceQueueBuffers(alMusicSourceVoicecin, 1, &bufid);
					ALenum error = alGetError();
					if (error != AL_NO_ERROR) {
						common->Warning("OpenAL Cinematic: %s\n", alGetString(error));
						return;
					}
				}
				offset++;
				if (offset == NUM_BUFFERS) {
					offset = 0;
				}
			}
		}
	}
	else {
		alBufferData(alMusicBuffercin[offset], av_sample_cin, data, size, av_rate_cin);
		offset++;
		if (offset == NUM_BUFFERS) {
			alSourceQueueBuffers(alMusicSourceVoicecin, offset, alMusicBuffercin);
			ALenum error = alGetError();
			if (error != AL_NO_ERROR) {
				common->Warning("OpenAL Cinematic: %s\n", alGetString(error));
				return;
			}
			trigger = true;
			offset = 0;
		}
	}

	if (trigger) {
		if (state != AL_PLAYING) {
			ALint queued;
			alGetSourcei(alMusicSourceVoicecin, AL_BUFFERS_QUEUED, &queued);
			if (queued == 0) {
				return;
			}
			alSourcePlay(alMusicSourceVoicecin);
			ALenum error = alGetError();
			if (error != AL_NO_ERROR) {
				common->Warning("OpenAL Cinematic: %s\n", alGetString(error));
				return;
			}
		}
	}
}

void CinematicAudio_OpenAL::ShutdownAudio()
{
	if (alIsSource(alMusicSourceVoicecin)) {
		alSourceStop(alMusicSourceVoicecin);
		ALint queued;
		alGetSourcei(alMusicSourceVoicecin, AL_BUFFERS_QUEUED, &queued);
		if (queued > 0) {
			alSourceUnqueueBuffers(alMusicSourceVoicecin, NUM_BUFFERS, alMusicBuffercin);
		}
		alSourcei(alMusicSourceVoicecin, AL_BUFFER, 0);
		alDeleteSources(1, &alMusicSourceVoicecin);
		if (CheckALErrors() == AL_NO_ERROR) {
			alMusicSourceVoicecin = 0;
		}
	}

	if (alIsBuffer(alMusicBuffercin[0])) {
		alDeleteBuffers(NUM_BUFFERS, alMusicBuffercin);
	}
	if (!tBuffer->empty()) {
		int buffersize = tBuffer->size();
		while (buffersize > 0) {
			tBuffer->pop();
			buffersize--;
		}
	}
	if (!sizes->empty()) {
		int buffersize = sizes->size();
		while (buffersize > 0) {
			sizes->pop();
			buffersize--;
		}
	}
}
