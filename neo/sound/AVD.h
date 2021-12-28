#pragma once
#include "precompiled.h"
#include <stdio.h>
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
#include <stdlib.h>
#include <math.h>
#if defined (USE_FFMPEG)
extern "C"
{
#define __STDC_CONSTANT_MACROS
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libswresample/swresample.h>
#include <libavcodec/version.h>
}
#endif

#include "snd_local.h"
void parseAVError(int error);
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
#include <xaudio2.h>
#include <x3daudio.h>
bool DecodeXAudio(byte** audio, int* len, idWaveFile::waveFmt_t* format,bool ext);
#endif // !USE_OPENAL
bool DecodeALAudio(byte** audio, int* len, int *rate ,ALenum *sample );
const char* GetSampleName(ALenum sample);

