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

#include "precompiled.h"
#include "AL_EAX.h"

LPALGENEFFECTS			alGenEffectsRef = (LPALGENEFFECTS)alGetProcAddress("alGenEffects");
LPALEFFECTI				alEffectiRef = (LPALEFFECTI)alGetProcAddress("alEffecti");
LPALEFFECTF				alEffectfRef = (LPALEFFECTF)alGetProcAddress("alEffectf");
LPALEFFECTFV			alEffectfvRef = (LPALEFFECTFV)alGetProcAddress("alEffectfv");
LPALISEFFECT			alIsEffectRef = (LPALISEFFECT)alGetProcAddress("alIsEffect");
LPALISAUXILIARYEFFECTSLOT	alIsAuxiliaryEffectSlotRef = (LPALISAUXILIARYEFFECTSLOT)alGetProcAddress("alIsAuxiliaryEffectSlot");
LPALDELETEAUXILIARYEFFECTSLOTS	alDeleteAuxiliaryEffectSlotsRef = (LPALDELETEAUXILIARYEFFECTSLOTS)alGetProcAddress("alDeleteAuxiliaryEffectSlots");
LPALDELETEEFFECTS	alDeleteEffectsRef = (LPALDELETEEFFECTS)alGetProcAddress("alDeleteEffects");
LPALAUXILIARYEFFECTSLOTI	alAuxiliaryEffectSlotiRef = (LPALAUXILIARYEFFECTSLOTI)alGetProcAddress("alAuxiliaryEffectSloti");
LPALGENAUXILIARYEFFECTSLOTS	alGenAuxiliaryEffectSlotsRef = (LPALGENAUXILIARYEFFECTSLOTS)alGetProcAddress("alGenAuxiliaryEffectSlots");
LPALGENFILTERS			alGenFiltersRef = (LPALGENFILTERS)alGetProcAddress("alGenFilters");
LPALFILTERF				alFilterfRef = (LPALFILTERF)alGetProcAddress("alFilterf");
LPALFILTERFV			alFilterfvRef = (LPALFILTERFV)alGetProcAddress("alFilterfv");
LPALFILTERI				alFilteriRef = (LPALFILTERI)alGetProcAddress("alFilteri");
LPALDELETEFILTERS		alDeleteFiltersRef = (LPALDELETEFILTERS)alGetProcAddress("alDeleteFilters");
LPALISFILTER			alIsFilterRef = (LPALISFILTER)alGetProcAddress("alIsFilter");