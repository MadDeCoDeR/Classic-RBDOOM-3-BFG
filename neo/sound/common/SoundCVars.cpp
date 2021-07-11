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


idCVar s_skipHardwareSets("s_skipHardwareSets", "0", CVAR_BOOL, "Do all calculation, but skip XA2 calls");
idCVar s_debugHardware("s_debugHardware", "0", CVAR_BOOL, "Print a message any time a hardware voice changes");
idCVar s_showLevelMeter("s_showLevelMeter", "0", CVAR_BOOL | CVAR_ARCHIVE, "Show VU meter");
idCVar s_meterTopTime("s_meterTopTime", "1000", CVAR_INTEGER | CVAR_ARCHIVE, "How long (in milliseconds) peaks are displayed on the VU meter");
idCVar s_meterPosition("s_meterPosition", "100 100 20 200", CVAR_ARCHIVE, "VU meter location (x y w h)");
idCVar s_device("s_device", "-1", CVAR_INTEGER | CVAR_ARCHIVE, "Which audio device to use (listDevices to list, -1 for default)");
idCVar s_showPerfData("s_showPerfData", "0", CVAR_BOOL, "Show XAudio2 Performance data");