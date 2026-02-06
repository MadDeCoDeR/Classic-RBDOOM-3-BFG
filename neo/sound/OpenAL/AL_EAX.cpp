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

LPALGENEFFECTS			alGenEffectsRef;
LPALEFFECTI				alEffectiRef;
LPALEFFECTF				alEffectfRef;
LPALEFFECTFV			alEffectfvRef;
LPALISEFFECT			alIsEffectRef;
LPALISAUXILIARYEFFECTSLOT	alIsAuxiliaryEffectSlotRef;
LPALDELETEAUXILIARYEFFECTSLOTS	alDeleteAuxiliaryEffectSlotsRef;
LPALDELETEEFFECTS	alDeleteEffectsRef;
LPALAUXILIARYEFFECTSLOTI	alAuxiliaryEffectSlotiRef;
LPALGENAUXILIARYEFFECTSLOTS	alGenAuxiliaryEffectSlotsRef;
LPALGENFILTERS			alGenFiltersRef;
LPALFILTERF				alFilterfRef;
LPALFILTERFV			alFilterfvRef;
LPALFILTERI				alFilteriRef;
LPALDELETEFILTERS		alDeleteFiltersRef;
LPALISFILTER			alIsFilterRef;

void RegisterEFXFuncs() {
    alGenEffectsRef = (LPALGENEFFECTS)alGetProcAddress("alGenEffects");
    alEffectiRef = (LPALEFFECTI)alGetProcAddress("alEffecti");
    alEffectfRef = (LPALEFFECTF)alGetProcAddress("alEffectf");
    alEffectfvRef = (LPALEFFECTFV)alGetProcAddress("alEffectfv");
    alIsEffectRef = (LPALISEFFECT)alGetProcAddress("alIsEffect");
    alIsAuxiliaryEffectSlotRef = (LPALISAUXILIARYEFFECTSLOT)alGetProcAddress("alIsAuxiliaryEffectSlot");
    alDeleteAuxiliaryEffectSlotsRef = (LPALDELETEAUXILIARYEFFECTSLOTS)alGetProcAddress("alDeleteAuxiliaryEffectSlots");
    alDeleteEffectsRef = (LPALDELETEEFFECTS)alGetProcAddress("alDeleteEffects");
    alAuxiliaryEffectSlotiRef = (LPALAUXILIARYEFFECTSLOTI)alGetProcAddress("alAuxiliaryEffectSloti");
    alGenAuxiliaryEffectSlotsRef = (LPALGENAUXILIARYEFFECTSLOTS)alGetProcAddress("alGenAuxiliaryEffectSlots");
    alGenFiltersRef = (LPALGENFILTERS)alGetProcAddress("alGenFilters");
    alFilterfRef = (LPALFILTERF)alGetProcAddress("alFilterf");
    alFilterfvRef = (LPALFILTERFV)alGetProcAddress("alFilterfv");
    alFilteriRef = (LPALFILTERI)alGetProcAddress("alFilteri");
    alDeleteFiltersRef = (LPALDELETEFILTERS)alGetProcAddress("alDeleteFilters");
    alIsFilterRef = (LPALISFILTER)alGetProcAddress("alIsFilter");
}