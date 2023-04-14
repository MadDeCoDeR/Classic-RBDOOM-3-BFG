/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company. 

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).  

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/
#include "precompiled.h"
#pragma hdrstop

#include "snd_local.h"

/*
===============
idEFXFile::idEFXFile
===============
*/
idEFXFile::idEFXFile( void ) { }

/*
===============
idEFXFile::Clear
===============
*/
void idEFXFile::Clear( void ) {
	effects.DeleteContents( true );
}

/*
===============
idEFXFile::~idEFXFile
===============
*/
idEFXFile::~idEFXFile( void ) {
	Clear();
}

/*
===============
idEFXFile::FindEffect
===============
*/
bool idEFXFile::FindEffect( idStr &name, idSoundEffect **effect, int *index ) {
	int i;

	for ( i = 0; i < effects.Num(); i++ ) {
		if ( ( effects[i] ) && ( effects[i]->name == name ) ) {
			*effect = effects[i];
			*index = i;
			return true;
		}
	}
	return false;
}

/*
===============
idEFXFile::ReadEffect
===============
*/
bool idEFXFile::ReadEffect( idLexer &src, idSoundEffect *effect ) {
	idToken name, token;
	
	if ( !src.ReadToken( &token ) )
		return false;

	// reverb effect
	if ( token == "reverb" ) {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
		if (!common->UseAlternativeAudioAPI()) {
#endif
			EFXEAXREVERBPROPERTIES* reverb = (EFXEAXREVERBPROPERTIES*)Mem_Alloc(sizeof(EFXEAXREVERBPROPERTIES), TAG_AUDIO);
			if (reverb) {
				memset(reverb, 0, sizeof(EFXEAXREVERBPROPERTIES));
				src.ReadTokenOnLine(&token);
				name = token;

				if (!src.ReadToken(&token)) {
					Mem_Free(reverb);
					return false;
				}

				if (token != "{") {
					src.Error("idEFXFile::ReadEffect: { not found, found %s", token.c_str());
					Mem_Free(reverb);
					return false;
				}

				do {
					if (!src.ReadToken(&token)) {
						src.Error("idEFXFile::ReadEffect: EOF without closing brace");
						Mem_Free(reverb);
						return false;
					}

					if (token == "}") {
						effect->name = name;
						effect->data = (void*)reverb;
						effect->datasize = sizeof(EFXEAXREVERBPROPERTIES);
						break;
					}
					//GK: Thanks to Dhewm 3 I found a better way to parse these files
					if (token == "environment") {
						src.ReadTokenOnLine(&token);
						//reverb->flDensity = token.GetUnsignedLongValue();
					}
					else if (token == "environment size") {
						float size = src.ParseFloat();
						reverb->flDensity = (size < 2.0f) ? (size - 1.0f) : 1.0f;
					}
					else if (token == "environment diffusion") {
						reverb->flDiffusion = src.ParseFloat();
					}
					else if (token == "room") {
						reverb->flGain = idMath::ClampFloat(AL_EAXREVERB_MIN_GAIN, AL_EAXREVERB_MAX_GAIN, idMath::Pow(10.0f, src.ParseInt() / 2000.0f));
					}
					else if (token == "room hf") {
						reverb->flGainHF = idMath::ClampFloat(AL_EAXREVERB_MIN_GAINHF, AL_EAXREVERB_MAX_GAINHF, idMath::Pow(10.0f, src.ParseInt() / 2000.0f));
					}
					else if (token == "room lf") {
						reverb->flGainLF = idMath::ClampFloat(AL_EAXREVERB_MIN_GAINLF, AL_EAXREVERB_MAX_GAINLF, idMath::Pow(10.0f, src.ParseInt() / 2000.0f));
					}
					else if (token == "decay time") {
						reverb->flDecayTime = src.ParseFloat();
					}
					else if (token == "decay hf ratio") {
						reverb->flDecayHFRatio = src.ParseFloat();
					}
					else if (token == "decay lf ratio") {
						reverb->flDecayLFRatio = src.ParseFloat();
					}
					else if (token == "reflections") {
						reverb->flReflectionsGain = idMath::ClampFloat(AL_EAXREVERB_MIN_REFLECTIONS_GAIN, AL_EAXREVERB_MAX_REFLECTIONS_GAIN, idMath::Pow(10.0f, src.ParseInt() / 2000.0f));
					}
					else if (token == "reflections delay") {
						reverb->flReflectionsDelay = src.ParseFloat();
					}
					else if (token == "reflections pan") {
						reverb->flLateReverbPan[0] = src.ParseFloat();
						reverb->flLateReverbPan[1] = src.ParseFloat();
						reverb->flLateReverbPan[2] = src.ParseFloat();
					}
					else if (token == "reverb") {
						reverb->flLateReverbGain = idMath::ClampFloat(AL_EAXREVERB_MIN_LATE_REVERB_GAIN, AL_EAXREVERB_MAX_LATE_REVERB_GAIN, idMath::Pow(10.0f, src.ParseInt() / 2000.0f));
					}
					else if (token == "reverb delay") {
						reverb->flLateReverbDelay = src.ParseFloat();
					}
					else if (token == "reverb pan") {
						reverb->flLateReverbPan[0] = src.ParseFloat();
						reverb->flLateReverbPan[1] = src.ParseFloat();
						reverb->flLateReverbPan[2] = src.ParseFloat();
					}
					else if (token == "echo time") {
						reverb->flEchoTime = src.ParseFloat();
					}
					else if (token == "echo depth") {
						reverb->flEchoDepth = src.ParseFloat();
					}
					else if (token == "modulation time") {
						reverb->flModulationTime = src.ParseFloat();
					}
					else if (token == "modulation depth") {
						reverb->flModulationDepth = src.ParseFloat();
					}
					else if (token == "air absorption hf") {
						//GK: This is wrong in the files so clamp it
						reverb->flAirAbsorptionGainHF = idMath::ClampFloat(AL_EAXREVERB_MIN_AIR_ABSORPTION_GAINHF, AL_EAXREVERB_MAX_AIR_ABSORPTION_GAINHF, idMath::Pow(10.0f, src.ParseFloat() / 2000.0f));
					}
					else if (token == "hf reference") {
						reverb->flHFReference = src.ParseFloat();
					}
					else if (token == "lf reference") {
						reverb->flLFReference = src.ParseFloat();
					}
					else if (token == "room rolloff factor") {
						reverb->flRoomRolloffFactor = src.ParseFloat();
					}
					else if (token == "flags") {
						src.ReadTokenOnLine(&token);
						unsigned int flags = token.GetUnsignedLongValue();
						reverb->iDecayHFLimit = (flags & 0x20) ? AL_TRUE : AL_FALSE;
					}
					else {
						src.ReadTokenOnLine(&token);
						src.Error("idEFXFile::ReadEffect: Invalid parameter in reverb definition");
						Mem_Free(reverb);
					}
				} while (1);
				if (reverb->flAirAbsorptionGainHF < AL_EAXREVERB_MIN_AIR_ABSORPTION_GAINHF) {
					reverb->flAirAbsorptionGainHF = AL_EAXREVERB_MIN_AIR_ABSORPTION_GAINHF;
				}
				return true;
			}
		
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
		}else {
			XAUDIO2FX_REVERB_PARAMETERS* reverb = (XAUDIO2FX_REVERB_PARAMETERS*)Mem_Alloc(sizeof(XAUDIO2FX_REVERB_PARAMETERS), TAG_AUDIO);
			memset(reverb, 0, sizeof(XAUDIO2FX_REVERB_PARAMETERS));
			src.ReadTokenOnLine(&token);
			name = token;
			if (!src.ReadToken(&token)) {
				return false;
			}

			if (token != "{") {
				src.Error("idEFXFile::ReadEffect: { not found, found %s", token.c_str());
				return false;
			}

			do {
				if (!src.ReadToken(&token)) {
					src.Error("idEFXFile::ReadEffect: EOF without closing brace");
					return false;
				}

				if (token == "}") {
					effect->name = name;
					effect->data = (void*)reverb;
					effect->datasize = sizeof(reverb);
					break;
				}
				reverb->DisableLateField = XAUDIO2FX_REVERB_DEFAULT_DISABLE_LATE_FIELD;
				reverb->PositionLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION;
				reverb->PositionMatrixLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
				reverb->PositionMatrixRight = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
				reverb->PositionRight = XAUDIO2FX_REVERB_DEFAULT_POSITION;
				reverb->RearDelay = XAUDIO2FX_REVERB_DEFAULT_7POINT1_REAR_DELAY;
				reverb->SideDelay = XAUDIO2FX_REVERB_DEFAULT_7POINT1_SIDE_DELAY;
				reverb->WetDryMix = 75.0F;
				//GK: Thanks to Dhewm 3 I found a better way to parse these files
				if (token == "environment") {
					src.ReadTokenOnLine(&token);
					//reverb->WetDryMix = src.ParseFloat();
				}
				else if (token == "environment size") {
					float size = src.ParseFloat();
					reverb->RoomSize = size;
					reverb->Density = size;
				}
				else if (token == "environment diffusion") {
					float diffusion = src.ParseFloat();
					reverb->EarlyDiffusion = diffusion * 15;
					reverb->LateDiffusion = diffusion * 15;
				}
				else if (token == "room") {
					reverb->RoomFilterMain = idMath::ClampFloat(XAUDIO2FX_REVERB_MIN_ROOM_FILTER_MAIN, XAUDIO2FX_REVERB_MAX_ROOM_FILTER_MAIN, src.ParseInt() / 100.0f);
				}
				else if (token == "room hf") {
					reverb->RoomFilterHF = idMath::ClampFloat(XAUDIO2FX_REVERB_MIN_ROOM_FILTER_HF, XAUDIO2FX_REVERB_MAX_ROOM_FILTER_HF, src.ParseInt() / 100.0f);
				}
				/*else if (token == "room lf") {
					reverb.LowEQGain = idMath::ClampFloat(XAUDIO2FX_REVERB_MIN_LOW_EQ_GAIN, XAUDIO2FX_REVERB_MAX_LOW_EQ_GAIN, idMath::Pow(10.0f, src.ParseInt() / 2000.0f));
				}*/
				else if (token == "decay time") {
					reverb->DecayTime = src.ParseFloat();
				}
				else if (token == "decay hf ratio") {
					float hfRatio = src.ParseFloat();
					reverb->HighEQGain = hfRatio * 4;
					reverb->HighEQCutoff = hfRatio * 7.5F;
				}
				else if (token == "decay lf ratio") {
					float lfRatio = src.ParseFloat();
					reverb->LowEQGain = lfRatio * 6;
					reverb->LowEQCutoff = lfRatio * 4.5F;
				}
				else if (token == "reflections") {
					reverb->ReflectionsGain = idMath::ClampFloat(XAUDIO2FX_REVERB_MIN_REFLECTIONS_GAIN, XAUDIO2FX_REVERB_MAX_REFLECTIONS_GAIN, src.ParseInt() / 100.0f);
				}
				else if (token == "reflections delay") {
					reverb->ReflectionsDelay = src.ParseFloat();
				}
				//else if (token == "reflections pan") {
				//	reverb->flLateReverbPan[0] = src.ParseFloat();
				//	reverb->flLateReverbPan[1] = src.ParseFloat();
				//	reverb->flLateReverbPan[2] = src.ParseFloat();
				//}
				else if (token == "reverb") {
					reverb->ReverbGain = idMath::ClampFloat(XAUDIO2FX_REVERB_MIN_REVERB_GAIN, XAUDIO2FX_REVERB_MAX_REVERB_GAIN, src.ParseInt() / 100.0f);
				}
				else if (token == "reverb delay") {
					reverb->ReverbDelay = src.ParseFloat();
				}
				//else if (token == "reverb pan") {
				//	reverb->flLateReverbPan[0] = src.ParseFloat();
				//	reverb->flLateReverbPan[1] = src.ParseFloat();
				//	reverb->flLateReverbPan[2] = src.ParseFloat();
				//}
				//else if (token == "echo time") {
				//	reverb->flEchoTime = src.ParseFloat();
				//}
				//else if (token == "echo depth") {
				//	reverb->flEchoDepth = src.ParseFloat();
				//}
				//else if (token == "modulation time") {
				//	reverb->flModulationTime = src.ParseFloat();
				//}
				//else if (token == "modulation depth") {
				//	reverb->flModulationDepth = src.ParseFloat();
				//}
				//else if (token == "air absorption hf") {
				//	//GK: This is wrong in the files so clamp it
				//	reverb->flAirAbsorptionGainHF = idMath::ClampFloat(AL_EAXREVERB_MIN_AIR_ABSORPTION_GAINHF, AL_EAXREVERB_MAX_AIR_ABSORPTION_GAINHF, idMath::Pow(10.0f, src.ParseFloat() / 2000.0f));
				//}
				else if (token == "hf reference") {
					reverb->RoomFilterFreq = src.ParseFloat();
				}
				//else if (token == "lf reference") {
				//	reverb->flLFReference = src.ParseFloat();
				//}
				//else if (token == "room rolloff factor") {
				//	reverb->flRoomRolloffFactor = src.ParseFloat();
				//}
				//else if (token == "flags") {
				//	src.ReadTokenOnLine(&token);
				//	unsigned int flags = token.GetUnsignedLongValue();
				//	reverb->iDecayHFLimit = (flags & 0x20) ? AL_TRUE : AL_FALSE;
				//}
				else {
					src.ReadTokenOnLine(&token);
					//src.Error("idEFXFile::ReadEffect: Invalid parameter in reverb definition");
					//reverb = {};
				}
			} while (1);

			return true;
		}
#endif
	} else {
		// other effect (not supported at the moment)
		src.Error( "idEFXFile::ReadEffect: Unknown effect definition" );
	}

	return false;
}


/*
===============
idEFXFile::LoadFile
===============
*/
bool idEFXFile::LoadFile( const char *filename, bool OSPath ) {
	idLexer src( LEXFL_NOSTRINGCONCAT );
	idToken token;

	src.LoadFile( filename, OSPath );
	if ( !src.IsLoaded() ) {
		return false;
	}

	if ( !src.ExpectTokenString( "Version" ) ) {
		return false;
	}

	if ( src.ParseInt() != 1 ) {
		src.Error( "idEFXFile::LoadFile: Unknown file version" );
		return false;
	}
	
	while ( !src.EndOfFile() ) {
		idSoundEffect *effect = new idSoundEffect;
		if ( ReadEffect( src, effect ) ) {
			effects.Append( effect );
		}
	};

	return true;
}


/*
===============
idEFXFile::UnloadFile
===============
*/
void idEFXFile::UnloadFile( void ) {
	Clear();
}
