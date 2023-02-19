/**
* Copyright (C) 2018 George Kalmpokis
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
#pragma hdrstop
#include "precompiled.h"
#include <vector>

#include <sound/AVD.h>
#include <queue>

#if defined(_MSC_VER) && defined(USE_XAUDIO2)
bool DecodeXAudio(byte** audio,int* len, idWaveFile::waveFmt_t* format,bool ext) {
	if ( *len <= 0) {
		return false;
	}
	int ret = 0;
	int avindx = 0;
	AVFormatContext*		fmt_ctx = avformat_alloc_context();
#if LIBAVCODEC_VERSION_MAJOR > 58
	const AVCodec* dec;
#else
	AVCodec* dec;
#endif
	AVCodecContext*			dec_ctx;
	AVPacket packet;
	SwrContext* swr_ctx = NULL;
	unsigned char *avio_ctx_buffer = NULL;
	avio_ctx_buffer = static_cast<unsigned char *>(av_malloc((size_t)*len));
	memcpy(avio_ctx_buffer, *audio, *len);
	AVIOContext *avio_ctx = avio_alloc_context(avio_ctx_buffer, *len, 0, NULL, NULL, NULL, NULL);
	fmt_ctx->pb = avio_ctx;
	if ((ret = avformat_open_input(&fmt_ctx, "", NULL, NULL)) < 0) {
		parseAVError(ret);
		return false;
	}

	if ((ret = avformat_find_stream_info(fmt_ctx, NULL)) < 0)
	{
		parseAVError(ret);
		return false;
	}
	ret = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, &dec, 0);
	avindx = ret;
	dec_ctx = avcodec_alloc_context3(dec);
	if ((ret = avcodec_parameters_to_context(dec_ctx, fmt_ctx->streams[avindx]->codecpar)) < 0) {
		char* error = new char[256];
		av_strerror(ret, error, 256);
		common->Warning("AVD: Failed to create codec context from codec parameters with error: %s\n", error);
	}
	dec_ctx->time_base = fmt_ctx->streams[avindx]->time_base;
	dec_ctx->framerate = fmt_ctx->streams[avindx]->avg_frame_rate;
	dec_ctx->pkt_timebase = fmt_ctx->streams[avindx]->time_base;
	dec = avcodec_find_decoder(dec_ctx->codec_id);
	if ((ret = avcodec_open2(dec_ctx, dec, NULL)) < 0)
	{
		parseAVError(ret);
		return false;
	}
	bool hasplanar = false;
	AVSampleFormat dst_smp = AV_SAMPLE_FMT_NONE;
	if (dec_ctx->sample_fmt >= 5) {
		dst_smp = static_cast<AVSampleFormat> (dec_ctx->sample_fmt - 5);
		if (!swr_alloc_set_opts2(&swr_ctx, &dec_ctx->ch_layout, dst_smp, dec_ctx->sample_rate, &dec_ctx->ch_layout, dec_ctx->sample_fmt, dec_ctx->sample_rate, 0, NULL)) {
			int res = swr_init(swr_ctx);
			if (res >= 0) {
				hasplanar = true;
			}
		}
	}
	int format_byte = 0;
	bool use_ext = false;
	switch (dec_ctx->sample_fmt) {
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
	*&format->basic.samplesPerSec = dec_ctx->sample_rate;
	*&format->basic.numChannels = dec_ctx->ch_layout.nb_channels;
	*&format->basic.avgBytesPerSec = *&format->basic.samplesPerSec * format_byte * *&format->basic.numChannels;
	*&format->basic.blockSize = format_byte * *&format->basic.numChannels;
	*&format->basic.bitsPerSample = format_byte * 8;
	if (ext) {
		*&format->basic.formatTag = WAVE_FORMAT_EXTENSIBLE;
		*&format->extraSize = 22;
		switch (*&format->basic.numChannels) {
		case 1:
			*&format->extra.extensible.channelMask = SPEAKER_MONO;
			break;
		case 2:
			*&format->extra.extensible.channelMask = SPEAKER_STEREO;
			break;
		case 4:
			*&format->extra.extensible.channelMask = SPEAKER_QUAD;
			break;
		case 5:
			*&format->extra.extensible.channelMask = SPEAKER_5POINT1_SURROUND;
			break;
		case 7:
			*&format->extra.extensible.channelMask = SPEAKER_7POINT1_SURROUND;
			break;
		default:
			*&format->extra.extensible.channelMask = SPEAKER_MONO;
			break;
		}
		*&format->extra.extensible.validBitsPerSample = *&format->basic.bitsPerSample;
		if (!use_ext) {
			*&format->extra.extensible.subFormat.data1 = KSDATAFORMAT_SUBTYPE_PCM.Data1;
			*&format->extra.extensible.subFormat.data2 = KSDATAFORMAT_SUBTYPE_PCM.Data2;
			*&format->extra.extensible.subFormat.data3 = KSDATAFORMAT_SUBTYPE_PCM.Data3;
			for (int i = 0; i < 8; i++) {
				*&format->extra.extensible.subFormat.data4[i] = KSDATAFORMAT_SUBTYPE_PCM.Data4[i];
			}
		}
		else {
			*&format->extra.extensible.subFormat.data1 = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT.Data1;
			*&format->extra.extensible.subFormat.data2 = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT.Data2;
			*&format->extra.extensible.subFormat.data3 = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT.Data3;
			for (int i = 0; i < 8; i++) {
				*&format->extra.extensible.subFormat.data4[i] = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT.Data4[i];
			}
		}
	}
	else {
		if (!use_ext) {
			*&format->basic.formatTag = WAVE_FORMAT_PCM;
		}
		else {
			*&format->basic.formatTag = WAVE_FORMAT_IEEE_FLOAT;
		}
		*&format->extraSize = 0;
	}
	if (av_new_packet(&packet, 1) == 0) {
		AVFrame* frame = av_frame_alloc();
			int offset = 0;
			int num_bytes = 0;
			//int bufferoffset = format_byte * 10;
			//unsigned long long length = *len;
			std::vector<byte*> tBuffer;
			std::vector<int> buffSizes;
			uint8_t** tBuffer2 = NULL;
			int  bufflinesize;
			std::queue<AVPacket> packetQueue[4];
			while (av_read_frame(fmt_ctx, &packet) >= 0) {
				if (packet.stream_index == avindx) {
					packetQueue->push(packet);
					ret = avcodec_send_packet(dec_ctx, &packetQueue->front());
					if (ret != 0 && ret != AVERROR(EAGAIN)) {
						char* error = new char[256];
						av_strerror(ret, error, 256);
						common->Warning("AVD: Failed to send packet for decoding with message: %s\n", error);
					}
					else {
						packet = packetQueue->front();
						packetQueue->pop();
						ret = avcodec_receive_frame(dec_ctx, frame);
						if (ret != 0) {
							char* error = new char[256];
							av_strerror(ret, error, 256);
							common->Warning("AVD: Failed to receive frame from decoding with message: %s\n", error);
						}
						else {

							if (hasplanar) {
								av_samples_alloc_array_and_samples(&tBuffer2,
									&bufflinesize,
									*&format->basic.numChannels,
									av_rescale_rnd(frame->nb_samples, frame->sample_rate, frame->sample_rate, AV_ROUND_UP),
									dst_smp,
									0);

								int res = swr_convert(swr_ctx, tBuffer2, bufflinesize, (const uint8_t**)frame->extended_data, frame->nb_samples);
								num_bytes = av_samples_get_buffer_size(&bufflinesize, frame->ch_layout.nb_channels,
									res, dst_smp, 1);
								tBuffer.push_back((byte*)malloc(num_bytes));
								buffSizes.push_back(num_bytes);
								memcpy(tBuffer.back(), tBuffer2[0], num_bytes);

								offset += num_bytes;
								av_freep(&tBuffer2[0]);

							}
							else {
								num_bytes = frame->linesize[0];
								tBuffer.push_back((byte*)malloc(num_bytes));
								buffSizes.push_back(num_bytes);
								memcpy(tBuffer.back(), frame->extended_data[0], num_bytes);
								offset += num_bytes;
							}



						}
					}

				}

				av_packet_unref(&packet);
			}
		av_frame_free(&frame);
		free(frame);
		*len = offset;
		*audio = (byte*)malloc(offset);
		offset = 0;
		for (uint i = 0; i < tBuffer.size(); i++) {
			memcpy(*audio + offset, tBuffer[i], buffSizes[i]);
			offset += buffSizes[i];
			byte* temp = tBuffer[i];
			free(temp);
			temp = NULL;
		}
		tBuffer.clear();
		buffSizes.clear();
		if (swr_ctx != NULL) {
			swr_free(&swr_ctx);
		}

		avcodec_close(dec_ctx);

		av_free(fmt_ctx->pb);
		avformat_close_input(&fmt_ctx);


		av_free(avio_ctx->buffer);
		av_freep(avio_ctx);
	}

	return true;
}
#endif
bool DecodeALAudio(byte** audio, int* len, int *rate, ALenum *sample) {
	if ( *len <= 0) {
		return false;
	}
	int ret = 0;
	int avindx = 0;
	AVFormatContext*		fmt_ctx = avformat_alloc_context();
#if LIBAVCODEC_VERSION_MAJOR > 58
	const AVCodec* dec;
#else
	AVCodec* dec;
#endif
	AVCodecContext*			dec_ctx;
	AVPacket packet;
	SwrContext* swr_ctx = NULL;
	unsigned char *avio_ctx_buffer = NULL;
	avio_ctx_buffer = static_cast<unsigned char *>(av_malloc((size_t)*len));
	memcpy(avio_ctx_buffer, *audio, *len);
	AVIOContext *avio_ctx = avio_alloc_context(avio_ctx_buffer, *len, 0, NULL, NULL, NULL, NULL);
	fmt_ctx->pb = avio_ctx;
	if ((ret = avformat_open_input(&fmt_ctx, "", NULL, NULL)) < 0) {
		parseAVError(ret);
		return false;
	}

	if ((ret = avformat_find_stream_info(fmt_ctx, NULL)) < 0)
	{
		parseAVError(ret);
		return false;
	}
	ret = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, &dec, 0);
	avindx = ret;
	dec_ctx = avcodec_alloc_context3(dec);
	if ((ret = avcodec_parameters_to_context(dec_ctx, fmt_ctx->streams[avindx]->codecpar)) < 0) {
		char* error = new char[256];
		av_strerror(ret, error, 256);
		common->Warning("AVD: Failed to create codec context from codec parameters with error: %s\n", error);
	}
	dec_ctx->time_base = fmt_ctx->streams[avindx]->time_base;
	dec_ctx->framerate = fmt_ctx->streams[avindx]->avg_frame_rate;
	dec_ctx->pkt_timebase = fmt_ctx->streams[avindx]->time_base;
	dec = avcodec_find_decoder(dec_ctx->codec_id);
	if ((ret = avcodec_open2(dec_ctx, dec, NULL)) < 0)
	{
		parseAVError(ret);
		return false;
	}
	bool hasplanar = false;
	AVSampleFormat dst_smp = AV_SAMPLE_FMT_NONE;
	if (dec_ctx->sample_fmt >= 5) {
		dst_smp = static_cast<AVSampleFormat> (dec_ctx->sample_fmt - 5);
		if (!swr_alloc_set_opts2(&swr_ctx, &dec_ctx->ch_layout, dst_smp, dec_ctx->sample_rate, &dec_ctx->ch_layout, dec_ctx->sample_fmt, dec_ctx->sample_rate, 0, NULL)) {
			int res = swr_init(swr_ctx);
			if (res >= 0) {
				hasplanar = true;
			}
		}
	}
	int format_byte = 0;
	bool use_ext = false;
	switch (dec_ctx->sample_fmt) {
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
	*rate = dec_ctx->sample_rate;
	switch (format_byte) {
	case 1:
		*sample = dec_ctx->ch_layout.nb_channels == 2 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
		break;
	case 2:
		*sample = dec_ctx->ch_layout.nb_channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
		break;
	case 4:
		*sample = dec_ctx->ch_layout.nb_channels == 2 ? AL_FORMAT_STEREO_FLOAT32 : AL_FORMAT_MONO_FLOAT32;
		break;
	}
	if (av_new_packet(&packet, 1) == 0) {
		AVFrame* frame = av_frame_alloc();
		int offset = 0;
		int num_bytes = 0;
		//int bufferoffset = format_byte * 10;
		//unsigned long long length = *len;
		std::vector<byte*> tBuffer;
		std::vector<int> buffSizes;
		uint8_t** tBuffer2 = NULL;
		int  bufflinesize;
		std::queue<AVPacket> packetQueue[4];
		while (av_read_frame(fmt_ctx, &packet) >= 0) {
			if (packet.stream_index == avindx) {
				packetQueue->push(packet);
				ret = avcodec_send_packet(dec_ctx, &packetQueue->front());
				if (ret != 0 && ret != AVERROR(EAGAIN)) {
					char* error = new char[256];
					av_strerror(ret, error, 256);
					common->Warning("AVD: Failed to send packet for decoding with message: %s\n", error);
				}
				else {
					packet = packetQueue->front();
					packetQueue->pop();
					ret = avcodec_receive_frame(dec_ctx, frame);
					if (ret != 0) {
						char* error = new char[256];
						av_strerror(ret, error, 256);
						common->Warning("AVD: Failed to receive frame from decoding with message: %s\n", error);
					}
					else {

						if (hasplanar) {
							av_samples_alloc_array_and_samples(&tBuffer2,
								&bufflinesize,
								frame->ch_layout.nb_channels,
								av_rescale_rnd(frame->nb_samples, frame->sample_rate, frame->sample_rate, AV_ROUND_UP),
								dst_smp,
								0);
							int res = swr_convert(swr_ctx, tBuffer2, bufflinesize, (const uint8_t**)frame->extended_data, frame->nb_samples);
							num_bytes = av_samples_get_buffer_size(&bufflinesize, frame->ch_layout.nb_channels,
								res, dst_smp, 1);
							tBuffer.push_back((byte*)malloc(num_bytes));
							buffSizes.push_back(num_bytes);
							memcpy(tBuffer.back(), tBuffer2[0], num_bytes);

							offset += num_bytes;
							av_freep(&tBuffer2[0]);

						}
						else {
							num_bytes = frame->linesize[0];
							tBuffer.push_back((byte*)malloc(num_bytes));
							buffSizes.push_back(num_bytes);
							memcpy(tBuffer.back(), frame->extended_data[0], num_bytes);
							offset += num_bytes;
						}



					}
				}
			}

			av_packet_unref(&packet);
		}
		av_frame_free(&frame);
		free(frame);
		*len = offset;
		*audio = (byte*)malloc(offset);
		offset = 0;
		for (uint i = 0; i < tBuffer.size(); i++) {
			memcpy(*audio + offset, tBuffer[i], buffSizes[i]);
			offset += buffSizes[i];
			byte* temp = tBuffer[i];
			free(temp);
			temp = NULL;
		}
		tBuffer.clear();
		buffSizes.clear();
		if (swr_ctx != NULL) {
			swr_free(&swr_ctx);
		}

		avcodec_close(dec_ctx);
		avformat_close_input(&fmt_ctx);
		avformat_free_context(fmt_ctx);


		av_free(avio_ctx->buffer);
		av_freep(avio_ctx);
	}
	return true;
}

const char* GetSampleName(ALenum sample) {
	switch (sample) {
	case AL_FORMAT_MONO8:
		return "Mono 8-bit";
	case AL_FORMAT_MONO16:
		return "Mono 16-bit";
	case AL_FORMAT_STEREO8:
		return "Stereo 8-bit";
	case AL_FORMAT_STEREO16:
		return "Stereo 16-bit";
	case AL_FORMAT_STEREO_FLOAT32:
		return "Stereo Float Point";
	case AL_FORMAT_MONO_FLOAT32:
		return "Mono Float Point";
	}
	return "";
}

void parseAVError(int error) {
	char* errorbuff = new char[256];
	av_make_error_string(errorbuff, 256, error);
	common->Printf("FFMPEG Error: %s\n", errorbuff);
}