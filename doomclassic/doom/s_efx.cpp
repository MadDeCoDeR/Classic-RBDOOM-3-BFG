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

#include "Precompiled.h"
#include "globaldata.h"
#include "doomtype.h"
#include "info.h"
#include "i_system.h"
#include "w_wad.h"
#include "s_efx.h"
#include "sound/OpenAL/AL_EAX.h"

typedef struct {
	const char* name;
	float min;
	float max;
	float* flvalue;
	int* ivalue;
}vertype;

void SetEFX(EFXEAXREVERBPROPERTIES* rev)
{
	alGenEffectsRef(1, &::g->clEAX);

		//common->Printf("Using EAX Reverb\n");



		/* EAX Reverb is available. Set the EAX effect type then load the

		 * reverb properties. */

		alEffectiRef((ALuint)::g->clEAX, AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB);



		alEffectfRef((ALuint)::g->clEAX, AL_EAXREVERB_DENSITY, rev->flDensity);

		alEffectfRef((ALuint)::g->clEAX, AL_EAXREVERB_DIFFUSION, rev->flDiffusion);

		alEffectfRef((ALuint)::g->clEAX, AL_EAXREVERB_GAIN, rev->flGain);

		alEffectfRef((ALuint)::g->clEAX, AL_EAXREVERB_GAINHF, rev->flGainHF);

		alEffectfRef((ALuint)::g->clEAX, AL_EAXREVERB_GAINLF, rev->flGainLF);

		alEffectfRef((ALuint)::g->clEAX, AL_EAXREVERB_DECAY_TIME, rev->flDecayTime);

		alEffectfRef((ALuint)::g->clEAX, AL_EAXREVERB_DECAY_HFRATIO, rev->flDecayHFRatio);

		alEffectfRef((ALuint)::g->clEAX, AL_EAXREVERB_DECAY_LFRATIO, rev->flDecayLFRatio);

		alEffectfRef((ALuint)::g->clEAX, AL_EAXREVERB_REFLECTIONS_GAIN, rev->flReflectionsGain);

		alEffectfRef((ALuint)::g->clEAX, AL_EAXREVERB_REFLECTIONS_DELAY, rev->flReflectionsDelay);

		alEffectfvRef((ALuint)::g->clEAX, AL_EAXREVERB_REFLECTIONS_PAN, rev->flReflectionsPan);

		alEffectfRef((ALuint)::g->clEAX, AL_EAXREVERB_LATE_REVERB_GAIN, rev->flLateReverbGain);

		alEffectfRef((ALuint)::g->clEAX, AL_EAXREVERB_LATE_REVERB_DELAY, rev->flLateReverbDelay);

		alEffectfvRef((ALuint)::g->clEAX, AL_EAXREVERB_LATE_REVERB_PAN, rev->flLateReverbPan);

		alEffectfRef((ALuint)::g->clEAX, AL_EAXREVERB_ECHO_TIME, rev->flEchoTime);

		alEffectfRef((ALuint)::g->clEAX, AL_EAXREVERB_ECHO_DEPTH, rev->flEchoDepth);

		alEffectfRef((ALuint)::g->clEAX, AL_EAXREVERB_MODULATION_TIME, rev->flModulationTime);

		alEffectfRef((ALuint)::g->clEAX, AL_EAXREVERB_MODULATION_DEPTH, rev->flModulationDepth);

		alEffectfRef((ALuint)::g->clEAX, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, rev->flAirAbsorptionGainHF);

		alEffectfRef((ALuint)::g->clEAX, AL_EAXREVERB_HFREFERENCE, rev->flHFReference);

		alEffectfRef((ALuint)::g->clEAX, AL_EAXREVERB_LFREFERENCE, rev->flLFReference);

		alEffectfRef((ALuint)::g->clEAX, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, rev->flRoomRolloffFactor);

		alEffectiRef((ALuint)::g->clEAX, AL_EAXREVERB_DECAY_HFLIMIT, rev->iDecayHFLimit);
}

std::vector<std::string> getReverblines(char* text) {
	bool ignoremult = false;
	std::vector<std::string> lines;
	int size = strlen(text);
	for (int i = 0; i < size /*- 7*/; i++) {
		std::string letter = "";
		bool ignore = false;
		while (text[i] != '\n') {
			if ((text[i] == '/' && text[i + 1] == '/'))
				ignore = true;
			if (text[i] == '/' && text[i + 1] == '*') {
				ignoremult = true;
			}
			else if (text[i] == '*' && text[i + 1] == '/') {
				ignoremult = false;
			}

			if (!ignore && !ignoremult) {
				if (text[i] != '\r' && text[i] != '\"') {
					letter += text[i];
				}
			}
			if (i < size /*- 7*/) {
				i++;
			}
			else {
				break;
			}

		}
		//i++;
		if (idStr::Icmp(letter.c_str(), "")) {
			lines.push_back(letter);
		}

	}
	return lines;
}

EFXEAXREVERBPROPERTIES *GetProperties(std::vector<std::string> lines, int index) {
	//if (idStr::Cmp(lines[index].c_str()+9, "{")) {
	//	return NULL;
	//}
	EFXEAXREVERBPROPERTIES *reverb = (EFXEAXREVERBPROPERTIES *)Mem_Alloc(sizeof(EFXEAXREVERBPROPERTIES), TAG_AUDIO);
	if (reverb) {
		memset(reverb, 0, sizeof(EFXEAXREVERBPROPERTIES));
		vertype revars[28] = {
		{ "environment size" ,0,0,&reverb->flDensity },
		{ "environment diffusion" ,0,0,&reverb->flDiffusion },
		{ "room" ,AL_EAXREVERB_MIN_GAIN,AL_EAXREVERB_MAX_GAIN,&reverb->flGain },
		{ "room hf" ,AL_EAXREVERB_MIN_GAINHF,AL_EAXREVERB_MAX_GAINHF,&reverb->flGainHF },
		{ "room lf" ,AL_EAXREVERB_MIN_GAINLF,AL_EAXREVERB_MAX_GAINLF,&reverb->flGainLF },
		{ "decay time" ,0,0,&reverb->flDecayTime },
		{ "decay hf ratio" ,0,0,&reverb->flDecayHFRatio },
		{ "decay lf ratio" ,0,0,&reverb->flDecayLFRatio },
		{ "reflections" ,AL_EAXREVERB_MIN_REFLECTIONS_GAIN,AL_EAXREVERB_MAX_REFLECTIONS_GAIN,&reverb->flReflectionsGain },
		{ "reflections delay" ,0,0,&reverb->flReflectionsDelay },
		{ "reflections pan" ,0,0,&reverb->flLateReverbPan[0] },
		{ "" ,0,0,&reverb->flLateReverbPan[1] },
		{ "" ,0,0,&reverb->flLateReverbPan[2] },
		{ "reverb" ,AL_EAXREVERB_MIN_LATE_REVERB_GAIN,AL_EAXREVERB_MAX_LATE_REVERB_GAIN,&reverb->flLateReverbGain },
		{ "reverb delay" ,0,0,&reverb->flLateReverbDelay },
		{ "reverb pan" ,0,0,&reverb->flLateReverbPan[0] },
		{ "" ,0,0,&reverb->flLateReverbPan[1] },
		{ "" ,0,0,&reverb->flLateReverbPan[2] },
		{ "echo time" ,0,0,&reverb->flEchoTime },
		{ "echo depth" ,0,0,&reverb->flEchoDepth },
		{ "modulation time" ,0,0,&reverb->flModulationTime },
		{ "modulation depth" ,0,0,&reverb->flModulationDepth },
		{ "air absorption hf" ,AL_EAXREVERB_MIN_AIR_ABSORPTION_GAINHF,AL_EAXREVERB_MAX_AIR_ABSORPTION_GAINHF,&reverb->flAirAbsorptionGainHF },
		{ "hf reference" ,0,0,&reverb->flHFReference },
		{ "lf reference" ,0,0,&reverb->flLFReference },
		{ "room rolloff factor" ,0,0,&reverb->flRoomRolloffFactor },
		{ "flags" ,0,0,NULL,&reverb->iDecayHFLimit },
		{"sector",0,0,NULL,NULL}
		};


		for (int i = index ; i < (int)lines.size(); i++) {
			if (!idStr::Cmp(lines[i].c_str(), "}")) {
				break;
			}
			for (int j = 0; j < 28; j++) {
				int nsize = strlen(revars[j].name);
				if (nsize == 0)
					continue;

				if (!idStr::Cmpn(lines[i].c_str(), revars[j].name, nsize)) {
					float val = atof(lines[i].c_str() + (nsize + 1));
					switch (j) {
					case 0:
						*revars[j].flvalue = (val < 2.0f) ? (val - 1.0f) : 1.0f;
						break;
					case 2:
					case 3:
					case 4:
					case 8:
					case 13:
					case 22:
						*revars[j].flvalue=idMath::ClampFloat(revars[j].min, revars[j].max, idMath::Pow(10.0f, val / 2000.0f));
						break;
					case 10:
					case 15:
					{
						char* tval = strtok(strdup(lines[i].c_str() + (nsize + 1)), " ");
						val = atof(tval);
						*revars[j].flvalue = val;
						int u = 1;
						while (tval != NULL) {
							*revars[j + u].flvalue = val;
							tval = strtok(NULL, " ");
							if (tval != NULL) {
								val = atof(tval);
							}
							u++;
						}
						break;
					}
					case 26:
					{
						unsigned int flags = atoi(lines[i].c_str() + (nsize + 1));
						*revars[j].ivalue = (flags & 0x20) ? AL_TRUE : AL_FALSE;
						break;
					}
					case 27:
						memcpy(reverb, ::g->reverbs[val], sizeof(EFXEAXREVERBPROPERTIES));
						return reverb;
					default:
						*revars[j].flvalue = val;
						break;
					}
				}
			}

		}
		return reverb;
	}
	return NULL;
}

EFXEAXREVERBPROPERTIES *GetReverb(char* name, int sector) {
	int lump = W_GetNumForName("REVERBD");
	char* text = (char*)malloc(W_LumpLength(lump) + 1);
	text[W_LumpLength(lump)] = '\0';
	W_ReadLump(lump, text);
	std::vector<std::string>lines = getReverblines(text);
	bool isok = false;
	int s = ::g->mapindex;
	for (int i = s; i < (int)lines.size(); i++) {
		if (!idStr::Cmpn(lines[i].c_str(), "map", 3)) {
			if (!idStr::Cmp(lines[i].c_str() + 4, name)) {
				isok = true;
				::g->mapindex = i;
				continue;
			}
			else {
				isok = false;
				continue;
			}
		}
		if (isok && !idStr::Cmpn(lines[i].c_str()+(lines[i].size()-1), "{", 1)) {
			if (sector == atoi(lines[i].c_str() + 7)) {
				return GetProperties(lines, i+1);
			}
		}
	}
	return NULL;
}