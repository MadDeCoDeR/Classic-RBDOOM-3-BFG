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
#include "f_finale.h"

void parsetext(char* text);
std::vector<std::string> getlines(char* text);
int checkstate(char* text);
void setThing(int pos, char* varname, int varval);
void setFrame(int pos, char* varname, int varval);
void setWeapon(int pos, char* varname, int varval);
void setPointer(int pos, char* varname, int varval);
void setCptr( char* varname, char* varfunc);
void setAmmo(int pos, char* varname, int varval);
void setText(std::vector<std::string>lines,int i,int il, int nl);
//More Headache than it's worth
//void setSound(int pos, char* varname, int varval);

typedef struct {
	char** var;
	char* name;
}dehstr;

dehstr strval[] = {
	{&GOTARMOR,"GOTARMOR"},
	{ &GOTMEGA,"GOTMEGA" },
	{ &GOTHTHBONUS,"GOTHTHBONUS" },
	{ &GOTARMBONUS,"GOTARMBONUS" },
	{ &GOTSTIM,"GOTSTIM" },
	{ &GOTMEDINEED,"GOTMEDINEED" },
	{ &GOTMEDIKIT,"GOTMEDIKIT" },
	{ &GOTSUPER,"GOTSUPER" },
	{ &GOTBLUECARD,"GOTBLUECARD" },
	{ &GOTYELWCARD,"GOTYELWCARD" },
	{ &GOTREDCARD,"GOTREDCARD" },
	{ &GOTBLUESKUL,"GOTBLUESKUL" },
	{ &GOTYELWSKUL,"GOTYELWSKUL" },
	{ &GOTREDSKULL,"GOTREDSKULL" },
	{ &GOTINVUL,"GOTINVUL" },
	{ &GOTBERSERK,"GOTBERSERK" },
	{ &GOTINVIS,"GOTINVIS" },
	{ &GOTSUIT,"GOTSUIT" },
	{ &GOTMAP,"GOTMAP" },
	{ &GOTVISOR,"GOTVISOR" },
	{ &GOTMSPHERE,"GOTMSPHERE" },
	{ &GOTCLIP,"GOTCLIP" },
	{ &GOTCLIPBOX,"GOTCLIPBOX" },
	{ &GOTROCKET,"GOTROCKET" },
	{ &GOTROCKBOX,"GOTROCKBOX" },
	{ &GOTCELL,"GOTCELL" },
	{ &GOTCELLBOX,"GOTCELLBOX" },
	{ &GOTSHELLS,"GOTSHELLS" },
	{ &GOTSHELLBOX,"GOTSHELLBOX" },
	{ &GOTBACKPACK,"GOTBACKPACK" },
	{ &GOTBFG9000,"GOTBFG9000" },
	{ &GOTCHAINGUN,"GOTCHAINGUN" },
	{ &GOTCHAINSAW,"GOTCHAINSAW" },
	{ &GOTLAUNCHER,"GOTLAUNCHER" },
	{ &GOTPLASMA,"GOTPLASMA" },
	{ &GOTSHOTGUN,"GOTSHOTGUN" },
	{ &GOTSHOTGUN2,"GOTSHOTGUN2" },
	{ &mapnames[0],"HUSTR_E1M1" },
	{ &mapnames[1],"HUSTR_E1M2" },
	{ &mapnames[2],"HUSTR_E1M3" },
	{ &mapnames[3],"HUSTR_E1M4" },
	{ &mapnames[4],"HUSTR_E1M5" },
	{ &mapnames[5],"HUSTR_E1M6" },
	{ &mapnames[6],"HUSTR_E1M7" },
	{ &mapnames[7],"HUSTR_E1M8" },
	{ &mapnames[8],"HUSTR_E1M9" },
	{ &mapnames[9],"HUSTR_E2M1" },
	{ &mapnames[10],"HUSTR_E2M2" },
	{ &mapnames[11],"HUSTR_E2M3" },
	{ &mapnames[12],"HUSTR_E2M4" },
	{ &mapnames[13],"HUSTR_E2M5" },
	{ &mapnames[14],"HUSTR_E2M6" },
	{ &mapnames[15],"HUSTR_E2M7" },
	{ &mapnames[16],"HUSTR_E2M8" },
	{ &mapnames[17],"HUSTR_E2M9" },
	{ &mapnames[18],"HUSTR_E3M1" },
	{ &mapnames[19],"HUSTR_E3M2" },
	{ &mapnames[20],"HUSTR_E3M3" },
	{ &mapnames[21],"HUSTR_E3M4" },
	{ &mapnames[22],"HUSTR_E3M5" },
	{ &mapnames[23],"HUSTR_E3M6" },
	{ &mapnames[24],"HUSTR_E3M7" },
	{ &mapnames[25],"HUSTR_E3M8" },
	{ &mapnames[26],"HUSTR_E3M9" },
	{ &mapnames[27],"HUSTR_E4M1" },
	{ &mapnames[28],"HUSTR_E4M2" },
	{ &mapnames[29],"HUSTR_E4M3" },
	{ &mapnames[30],"HUSTR_E4M4" },
	{ &mapnames[31],"HUSTR_E4M5" },
	{ &mapnames[32],"HUSTR_E4M6" },
	{ &mapnames[33],"HUSTR_E4M7" },
	{ &mapnames[34],"HUSTR_E4M8" },
	{ &mapnames[35],"HUSTR_E4M9" },
    { &mapnames2[0],"HUSTR_1" },
	{ &mapnames2[1],"HUSTR_2" },
	{ &mapnames2[2],"HUSTR_3" },
	{ &mapnames2[3],"HUSTR_4" },
	{ &mapnames2[4],"HUSTR_5" },
	{ &mapnames2[5],"HUSTR_6" },
	{ &mapnames2[6],"HUSTR_7" },
	{ &mapnames2[7],"HUSTR_8" },
	{ &mapnames2[8],"HUSTR_9" },
	{ &mapnames2[9],"HUSTR_10" },
	{ &mapnames2[10],"HUSTR_11" },
	{ &mapnames2[11],"HUSTR_12" },
	{ &mapnames2[12],"HUSTR_13" },
	{ &mapnames2[13],"HUSTR_14" },
	{ &mapnames2[14],"HUSTR_15" },
	{ &mapnames2[15],"HUSTR_16" },
	{ &mapnames2[16],"HUSTR_17" },
	{ &mapnames2[17],"HUSTR_18" },
	{ &mapnames2[18],"HUSTR_19" },
	{ &mapnames2[19],"HUSTR_20" },
	{ &mapnames2[20],"HUSTR_21" },
	{ &mapnames2[21],"HUSTR_22" },
	{ &mapnames2[22],"HUSTR_23" },
	{ &mapnames2[23],"HUSTR_24" },
	{ &mapnames2[24],"HUSTR_25" },
	{ &mapnames2[25],"HUSTR_26" },
	{ &mapnames2[26],"HUSTR_27" },
	{ &mapnames2[27],"HUSTR_28" },
	{ &mapnames2[28],"HUSTR_29" },
	{ &mapnames2[29],"HUSTR_30" },
	{ &mapnames2[30],"HUSTR_31" },
	{ &mapnames2[31],"HUSTR_32" },
	{ &mapnames2[32],"HUSTR_33" },
	{&e1text,"E1TEXT"},
	{ &e2text,"E2TEXT" },
	{ &e3text,"E3TEXT" },
	{ &e4text,"E4TEXT" },
	{&c1text,"C1TEXT"},
	{&c2text,"C2TEXT"},
	{ &c3text,"C3TEXT" },
	{ &c4text,"C4TEXT" },
	{ &c5text,"C5TEXT" },
	{ &c6text,"C6TEXT" },
	{ &c7text,"C7TEXT" },
	{ &c8Text,"C8TEXT" },
	{ &CC_ZOMBIE,"CC_ZOMBIE" },
	{ &CC_SHOTGUN,"CC_SHOTGUN" },
	{ &CC_HEAVY,"CC_HEAVY" }, 
	{ &CC_IMP,"CC_IMP" },
	{ &CC_DEMON,"CC_DEMON" },
	{ &CC_LOST,"CC_LOST" },
	{ &CC_CACO,"CC_CACO" },
	{ &CC_HELL,"CC_HELL" },
	{ &CC_BARON,"CC_BARON" },
	{ &CC_ARACH,"CC_ARACH" },
	{ &CC_ARACH,"CC_ARACH" },
	{ &CC_PAIN,"CC_PAIN" },
	{ &CC_REVEN,"CC_REVEN" },
	{ &CC_MANCU,"CC_MANCU" },
	{ &CC_ARCH,"CC_ARCH" },
	{ &CC_SPIDER,"CC_SPIDER" },
	{ &CC_CYBER,"CC_CYBER" },
	{ &CC_HERO,"CC_HERO" }
};

void loaddeh(int lump) {
	char* text = (char*)malloc(W_LumpLength(lump)+2);
	idLib::Printf("Applying DeHackeD patch ...\n");
	W_ReadLump(lump, text);
	
	//idLib::Printf("%s", text);
	parsetext(text);
	free(text);
	idLib::Printf("DeHackeD patch succesfully applied\n");
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
			std::string tv3 = strtok(NULL, "=");
			if (!tv3.empty()) {
				if (state != 6) {
					varval = atoi(tv3.c_str());
				}
				else {
					varfunc =strdup( tv3.c_str());
				}
			}
			//idLib::Printf("%s = %i\n", varname, varval);
			if (state == 1) {
				setThing(statepos-1, varname, varval);
			}
			if (state == 2) {
				setFrame(statepos, varname, varval);
				memcpy(::g->states, tempStates, sizeof(tempStates));
			}
			if (state == 4) {
				setWeapon(statepos , varname, varval);
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
				setAmmo(statepos, varname, varval);
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
				char* tst = strtok(strdup(linedtext[i].c_str()), " ");
				char* tval = strtok(NULL, " ");
					if (tst != NULL && tval != NULL) {

						statepos = atoi(tval);
						state = checkstate(tst);
						if (state == 1) {
							int tpos = statepos - 1;
							if (tpos >= NUMMOBJTYPES) {
								I_Error("No such Thing found");
							}
						}
						if (state == 2) {
							if (statepos >= NUMSTATES) {
								I_Error("No such Frame found");
							}
						}
						if (state == 3) {
							statepos = statepos + 1;
							varval2 = atoi(strtok(NULL, " "));
							setText(linedtext, i + 1, statepos, varval2);
						}
						if (state == 4) {
							if (statepos >= NUMWEAPONS) {
								I_Error("No such Weapon found");
							}
						}
						if (state == 5) {
							char* tv = strtok(NULL, " ");
							if (tv != NULL) {
								std::string tv2 = strtok(NULL, " ");
								if (!tv2.empty()) {
									statepos = atoi(tv2.substr(0,strlen(tv2.c_str())-1).c_str());
									
									if (statepos >= NUMSTATES) {
										idLib::Printf("%i\n", statepos);
										I_Error("No such codeptr found");
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
						if (tst != NULL){
						int tstate = checkstate(tst);
						if (tstate != 0) {
							state = tstate;
						}
					}
				}
			}
		}
	}
	}


std::vector<std::string> getlines(char* text) {
	std::vector<std::string> lines;
	int size = strlen(text);
	for (int i = 0; i < size /*- 7*/; i++ ) {
		std::string letter = "";
		qboolean ignore = false;
		while (text[i] != '\n') {
			if (text[i] == '#' && text[i-3]!='I' && text[i-2]!='D')
				ignore = true;

			if (!ignore) {
				if (text[i] != '\r') {
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
		lines.push_back(letter);

	}
	return lines;
}

int checkstate(char* text) {
	if (text != NULL) {
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
	else if (!idStr::Icmp(varname, "ID # ")) {
		mobjinfo[pos].doomednum = varval;
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
			weaponinfo[pos].downstate = varval;
		}
	}
	else if (!idStr::Icmp(varname, "Deselect frame ")) {
		if (varval < NUMSTATES) {
			weaponinfo[pos].upstate = varval;
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
			tempStates[pos].action = origStates[varval].action;
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

void setText(std::vector<std::string>lines, int i,int il,int nl) {
	int op = i;
	int size = 0;
	int psize = 0;
	int nsize = 0;
	std::string newline;
	char* tst = strtok(strdup(lines[i].c_str()), " ");
	//Dealing better with the memory
	char* ntxt=new char[nl];
	strcpy(ntxt, "");
	char* otxt = new char[il];
	while (checkstate(tst) == 0 && i<lines.size()) {
		newline = lines[i] + "\n";
		//if (size + strlen(newline.c_str()) <= il) {
		//idLib::Printf("%s\n", lines[i].c_str());
			size += strlen(newline.c_str());
			char* ltxt = strdup(newline.c_str());
			if (size <= il) {
				if (i == op) {
					strcpy(otxt, newline.c_str());
				}
				else {
					strcat(otxt, newline.c_str());
				}
			}
			else {
				if (strlen(ltxt) > il) {
					for (int o = 0; o < il - 1; o++) {
						otxt[o] = ltxt[o];
					}
					otxt[il - 1] = '\0';
					//strncpy(otxt, ltxt, il - 1);
					//strcat(otxt, "\0");
					strcpy(ntxt, ltxt + il - 1);
					/*int r = 0;
					for (int h = il-1; h < strlen(ltxt); h++,r++) {
						ntxt[r] = ltxt[h];
					}*/
					//ntxt[r - 1] = '\0';
					nsize += strlen(ltxt+il-1);
					//ntxt[strlen(ltxt)] = '\0';
				}
				else if (psize < il && psize + strlen(ltxt) > il) {
					int p = 0;
					for (int o = psize; o < il - 1; o++, p++) {
						otxt[o] = ltxt[p];
					}
					otxt[il - 1] = '\0';
					strcpy(ntxt, ltxt+p);
					nsize += strlen(ltxt + p);
				}
				else {
					strcat(ntxt, newline.c_str());
					nsize+= strlen(newline.c_str());
				}
			}
		i++;
		if (i < lines.size()) {
			tst = strtok(strdup(lines[i].c_str()), " ");
		}
		psize = size;
	}
	if (nsize < nl) {
		ntxt[nsize] = '\0';
	}
	else {
		ntxt[nl] = '\0';
	}
	//idLib::Printf("Replacing: %s with %s size %i\n", otxt, ntxt,nsize);
	int arrsz = sizeof(strval) / sizeof(*strval);
	if (otxt != nullptr) {
		bool replaced = false;
		for (int j = 0; j < arrsz; j++) {

			if (!idStr::Icmp(otxt, *strval[j].var)) {
				*strval[j].var=ntxt;
				replaced = true;
				//free(ntxt);
				break;
			}
		}
		if (!replaced) {
			for (int m = 0; m < NUMSPRITES; m++) {
				if (!idStr::Icmp(otxt, sprnames[m])) {
					sprnames[m] = ntxt;
					break;
				}
			}
		}
	}
	//free(otxt);
	
	//free(ntxt);
	//free(otxt);
	return;
}