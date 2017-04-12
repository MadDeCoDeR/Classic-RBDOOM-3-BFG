/**
* Copyright (C) 2017 George Kalmpokis
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


#include <ctype.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <vector>

#include "doomtype.h"

#include "info.h"

#include "d_deh.h"
#include "w_wad.h"

#include "i_system.h"

#include "d_items.h"
#include "sounds.h"

void parsetext(char* text);
std::vector<std::string> getlines(char* text);
int checkstate(char* text);
void setThing(int pos, char* varname, int varval);
void setFrame(int pos, char* varname, int varval);
void setWeapon(int pos, char* varname, int varval);
void setPointer(int pos, char* varname, int varval);
void setCptr( char* varname, char* varfunc);
void setAmmo(int pos, char* varname, int varval);
//More Headache than it's worth
//void setSound(int pos, char* varname, int varval);

void loaddeh(int lump) {
	char* text = (char*)malloc(W_LumpLength(lump)+2);

	W_ReadLump(lump, text);
	//idLib::Printf("%s", text);
	parsetext(text);
		
	free(text);
}

void parsetext(char* text) {
	std::vector<std::string> linedtext = getlines(text);
	int state = 0;
	char* varname;
	char* varfunc;
	int statepos;
	int varval;
	int varval2;
	char eq = '=';
	for (int i = 0; i < linedtext.size(); i++) {
		//idLib::Printf("%s\n", linedtext[i].c_str());
		if ((linedtext[i].find(eq) != std::string::npos) && state != 3 && state != 0) {
			varname = strtok(strdup(linedtext[i].c_str()), "=");
			char* tv3 = strtok(NULL, "=");
			if (tv3 != NULL) {
				if (state != 6) {
					varval = atoi(tv3);
				}
				else {
					varfunc = tv3;
				}
			}
			//idLib::Printf("%s = %i\n", varname, varval);
			if (state == 1) {
				setThing(statepos, varname, varval);
			}
			if (state == 2) {
				setFrame(statepos+1, varname, varval);
				memcpy(::g->states, tempStates, sizeof(tempStates));
			}
			if (state == 4) {
				setWeapon(statepos + 1, varname, varval);
				//memcpy(::g->states, tempStates, sizeof(tempStates));
			}
			if (state == 5) {
				setPointer(statepos, varname, varval);
				memcpy(::g->states, tempStates, sizeof(tempStates));
			}
			if (state == 6) {
				setCptr(varname, varfunc);
				memcpy(::g->states, tempStates, sizeof(tempStates));
			}
			if (state == 7) {
				setAmmo(statepos+1, varname, varval);
			}
			//More Headache than it's worth
			/*
			if (state == 8) {
				setSound(statepos + 1, varname, varval);
			}
			*/
		}
		else {
			if (linedtext[i].length() > 1) {
				varname = strtok(strdup(linedtext[i].c_str()), " ");
				char* tval = strtok(NULL, " ");
				//if (state != 3) {
					if (tval != NULL) {

						//if (idStr::Cmpn(varname, "[", 1)) {
						//	if ((idStr::Icmp(varname, "Patch") && idStr::Icmp(varname, "ID")) && state != 3 && state !=6) {
						statepos = atoi(tval) - 1;
						state = checkstate(varname);
						if (state == 1) {
							if (statepos >= NUMMOBJTYPES) {
								I_Error("No such Thing found");
							}
						}
						if (state == 2) {
							if (statepos >= NUMSTATES) {
								I_Error("No such Frame found");
							}
						}
						if (state == 3) {
							varval2 = atoi(strtok(NULL, " "));
						}
						if (state == 4) {
							if (statepos >= NUMWEAPONS) {
								I_Error("No such Weapon found");
							}
						}
						if (state == 5) {
							char* tv = strtok(NULL, " ");
							if (tv != NULL) {
								char* tv2 = strtok(NULL, " ");
								if (tv2 != NULL) {
									char* tv3 = (char*)malloc(strlen(tv2) + 1);
									for (int h = 0; h < strlen(tv2) - 1; h++) {
										tv3[h] = tv2[h];
									}
									statepos = atoi(tv3);
									if (statepos >= NUMSTATES) {
										I_Error("No such Frame found");
									}
								}
							}
						}
						if (state == 7) {
							if (statepos >= NUMAMMO) {
								I_Error("No such Ammo found");
							}
						}
						//More Headache than it's worth
						/*
						if (state == 8) {
							if (statepos >= NUMSFX) {
								I_Error("No such Sound found");
							}
						}
						*/

					}
					else {
						int tstate = checkstate(varname);
						if (tstate != 0) {
							state = tstate;
						}
					//}
				}
			}
			/*else {
				if (state == 3) {
					if (linedtext[i+1].length() > 1) {
						if(checkstate(strtok(strdup(linedtext[i + 1].c_str()), " ")) != 0)
						state = 0;
					}
					else {
						state = 3;
					}
				}
				else {
					state = 0;
				}
			}*/
		}
	}
	}


std::vector<std::string> getlines(char* text) {
	std::vector<std::string> lines;
	int size = strlen(text);
	for (int i = 0; i < size - 7; i++ ) {
		std::string letter = "";
		qboolean ignore = false;
		while (text[i] != '\n') {
			if (text[i] == '#')
				ignore = true;

			if (!ignore) {
				if (text[i] != '\r') {
					letter += text[i];
				}
			}
			if (i < size - 7) {
				i++;
			}
			else {
				break;
			}

		}
		//i++;
		lines.push_back(letter);

	}
	return lines;
}

int checkstate(char* text) {
	if (!idStr::Icmp(text, "Thing")) {
		return 1;
	}
	if (!idStr::Icmp(text, "Frame")) {
		return 2;
	}
	if (!idStr::Icmp(text, "Text")) {
		return 3;
	}
	if (!idStr::Icmp(text, "Weapon")) {
		return 4;
	}
	if (!idStr::Icmp(text, "Pointer")) {
		return 5;
	}
	if (!idStr::Icmp(text, "[CODEPTR]")) {
		return 6;
	}
	if (!idStr::Icmp(text, "Ammo")) {
		return 7;
	}
	//More Headache than it's worth
	/*
	if (!idStr::Icmp(text, "Sound")) {
		return 8;
	}
	*/
	return 0;
}

void setThing(int pos, char* varname, int varval) {
	if (!idStr::Icmp(varname, "Initial frame ")) {
		if (varval < NUMSTATES) {
			mobjinfo[pos].spawnstate = varval;
		}
	}
	else if (!idStr::Icmp(varname, "Hit points ")) {
		mobjinfo[pos].spawnhealth = varval;
	}
	else if (!idStr::Icmp(varname, "First moving frame ")) {
		if (varval < NUMSTATES) {
			mobjinfo[pos].seestate = varval;
		}
	}
	else if (!idStr::Icmp(varname, "Alert sound ")) {
		if (varval < NUMSFX) {
			mobjinfo[pos].seesound = varval;
		}
	}
	else if (!idStr::Icmp(varname, "Reaction time ")) {
		mobjinfo[pos].reactiontime = varval;
	}
	else if (!idStr::Icmp(varname, "Attack sound ")) {
		if (varval < NUMSFX) {
			mobjinfo[pos].attacksound = varval;
		}
	}
	else if (!idStr::Icmp(varname, "Injury frame ")) {
		if (varval < NUMSTATES) {
			mobjinfo[pos].painstate = varval;
		}
	}
	else if (!idStr::Icmp(varname, "Pain chance ")) {
		mobjinfo[pos].painchance = varval;
	}
	else if (!idStr::Icmp(varname, "Pain sound ")) {
		if (varval < NUMSFX) {
			mobjinfo[pos].painsound = varval;
		}
	}
	else if (!idStr::Icmp(varname, "Close attack frame ")) {
		if (varval < NUMSTATES) {
			mobjinfo[pos].meleestate = varval;
		}
	}
	else if (!idStr::Icmp(varname, "Far attack frame ")) {
		if (varval < NUMSTATES) {
			mobjinfo[pos].missilestate = varval;
		}
	}
	else if (!idStr::Icmp(varname, "Death frame ")) {
		if (varval < NUMSTATES) {
			mobjinfo[pos].deathstate = varval;
		}
	}
	else if (!idStr::Icmp(varname, "Exploding frame ")) {
		if (varval < NUMSTATES) {
			mobjinfo[pos].xdeathstate = varval;
		}
	}
	else if (!idStr::Icmp(varname, "Death sound ")) {
		if (varval < NUMSFX) {
			mobjinfo[pos].deathsound = varval;
		}
	}
	else if (!idStr::Icmp(varname, "Speed ")) {
		mobjinfo[pos].speed = varval;
	}
	else if (!idStr::Icmp(varname, "Width ")) {
		mobjinfo[pos].radius = varval;
	}
	else if (!idStr::Icmp(varname, "Height ")) {
		mobjinfo[pos].height = varval;
	}
	else if (!idStr::Icmp(varname, "Mass ")) {
		mobjinfo[pos].mass = varval;
	}
	else if (!idStr::Icmp(varname, "Missle damage ")) {
		mobjinfo[pos].damage = varval;
	}
	else if (!idStr::Icmp(varname, "Action sound ")) {
		if (varval < NUMSFX) {
			mobjinfo[pos].activesound = varval;
		}
	}
	else if (!idStr::Icmp(varname, "Bits ")) {
		mobjinfo[pos].flags = varval;
	}
	else if (!idStr::Icmp(varname, "Respawn frame ")) {
		if (varval < NUMSTATES) {
			mobjinfo[pos].raisestate = varval;
		}
	}
}

void setFrame(int pos, char* varname, int varval) {
	if (!idStr::Icmp(varname, "Sprite number ")) {
		if (varval < NUMSPRITES) {
			tempStates[pos].sprite = static_cast<spritenum_t> (varval);
		}
	}
	else if (!idStr::Icmp(varname, "Sprite subnumber ")) {
		tempStates[pos].frame = varval;
	}
	else if (!idStr::Icmp(varname, "Next frame ")) {
		if (varval < NUMSTATES) {
			tempStates[pos].nextstate = static_cast<statenum_t> (varval);
		}
	}
	else if (!idStr::Icmp(varname, "Duration ")) {
		tempStates[pos].tics = varval;
	}
	else if (!idStr::Icmp(varname, "Unknown 1 ")) {
		tempStates[pos].misc1 = varval;
	}
	else if (!idStr::Icmp(varname, "Unknown 2 ")) {
		tempStates[pos].misc2 = varval;
	}
}

void setWeapon(int pos, char* varname, int varval) {
	if (!idStr::Icmp(varname, "Ammo type ")) {
		if (varval < NUMAMMO) {
			weaponinfo[pos].ammo = static_cast<ammotype_t>(varval);
		}
	}
	else if (!idStr::Icmp(varname, "Select frame ")) {
		if (varval < NUMSTATES) {
			weaponinfo[pos].upstate = varval;
		}
	}
	else if (!idStr::Icmp(varname, "Deselect frame ")) {
		if (varval < NUMSTATES) {
			weaponinfo[pos].downstate = varval;
		}
	}
	else if (!idStr::Icmp(varname, "Bobbing frame ")) {
		if (varval < NUMSTATES) {
			weaponinfo[pos].readystate = varval;
		}
	}
	else if (!idStr::Icmp(varname, "Shooting frame ")) {
		if (varval < NUMSTATES) {
			weaponinfo[pos].atkstate = varval;
		}
	}
	else if (!idStr::Icmp(varname, "Firing frame ")) {
		if (varval < NUMSTATES) {
			weaponinfo[pos].flashstate = varval;
		}
	}
}

void setPointer(int pos, char* varname, int varval) {
	if (!idStr::Icmp(varname, "Codep Frame ")) {
		if (varval < NUMSTATES) {
			tempStates[pos].action = tempStates[varval].action;
		}
	}
}

void setCptr(char* varname, char* varfunc) {
	char* name = strtok(varname, " ");
	char* tp = strtok(NULL, " ");
	if (tp != NULL) {
		int pos = atoi(tp);
		if (strlen(name) > 1 && strlen(varfunc) > 1) {
			if (!idStr::Icmp(name, "FRAME") && pos < NUMSTATES) {
				if (idStr::Icmp(varfunc, " NULL")) {
					tempStates[pos].action = getFunc(varfunc);
				}
				else {
					tempStates[pos].action = NULL;
				}
			}
		}
	}
}

void setAmmo(int pos, char* varname, int varval) {
	if (!idStr::Icmp(varname, "Max Ammo ")) {
		maxammo[pos] = varval;
	}
	else if (!idStr::Icmp(varname, "Per Ammo ")) {
		clipammo[pos] = varval;
	}
}
//More Headache than it's worth
//void setSound(int pos, char* varname, int varval) {
//	if (!idStr::Icmp(varname, "Zero/One ")) {
//		S_sfx[pos].singularity = varval;
//	}
//	else if (!idStr::Icmp(varname, "Value ")) {
//		S_sfx[pos].priority = varval;
//	}
	/*else if (!idStr::Icmp(varname, "Zero 1 ")) {
		S_sfx[pos].link = &S_sfx[varval];
	}
	else if (!idStr::Icmp(varname, "Neg. One 1 ")) {
		S_sfx[pos].pitch = varval;
	}
	else if (!idStr::Icmp(varname, "Neg. One 2 ")) {
		S_sfx[pos].volume = varval;
	}
	else if (!idStr::Icmp(varname, "Zero 4 ")) {
		//S_sfx[pos].data = (void*)varval;
	}
	else if (!idStr::Icmp(varname, "Zero 2 ")) {
		S_sfx[pos].usefulness = varval;
	}
	else if (!idStr::Icmp(varname, "Zero 3 ")) {
		S_sfx[pos].lumpnum = varval;
	}*/
//}