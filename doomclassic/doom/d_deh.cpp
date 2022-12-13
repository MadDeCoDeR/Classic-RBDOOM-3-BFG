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
#include "idlib/Str.h"

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
#include "g_game.h"

typedef struct {
	char** var;
	const char* name;
}dehstr;
//TODO: Use this more
typedef struct{
	const char* name;
	long limit;
	char** sval;
	int* ival;
	long* lval;
	float* fval;
	spritenum_t* spval;
	statenum_t* stval;
	ammotype_t* amval;
}dehobj;

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
	{ &CC_HERO,"CC_HERO" },
	{&PD_BLUEO ,"PD_BLUEO"},
	{&PD_REDO,"PD_REDO"},
	{ &PD_YELLOWO,"PD_YELLOWO" },
	{ &PD_BLUEK,"PD_BLUEK" },
	{ &PD_REDK,"PD_REDK" },
	{ &PD_YELLOWK,"PD_YELLOWK" },
	{ &mapnamesp[0],"PHUSTR_1" },
	{ &mapnamesp[1],"PHUSTR_2" },
	{ &mapnamesp[2],"PHUSTR_3" },
	{ &mapnamesp[3],"PHUSTR_4" },
	{ &mapnamesp[4],"PHUSTR_5" },
	{ &mapnamesp[5],"PHUSTR_6" },
	{ &mapnamesp[6],"PHUSTR_7" },
	{ &mapnamesp[7],"PHUSTR_8" },
	{ &mapnamesp[8],"PHUSTR_9" },
	{ &mapnamesp[9],"PHUSTR_10" },
	{ &mapnamesp[10],"PHUSTR_11" },
	{ &mapnamesp[11],"PHUSTR_12" },
	{ &mapnamesp[12],"PHUSTR_13" },
	{ &mapnamesp[13],"PHUSTR_14" },
	{ &mapnamesp[14],"PHUSTR_15" },
	{ &mapnamesp[15],"PHUSTR_16" },
	{ &mapnamesp[16],"PHUSTR_17" },
	{ &mapnamesp[17],"PHUSTR_18" },
	{ &mapnamesp[18],"PHUSTR_19" },
	{ &mapnamesp[19],"PHUSTR_20" },
	{ &mapnamesp[20],"PHUSTR_21" },
	{ &mapnamesp[21],"PHUSTR_22" },
	{ &mapnamesp[22],"PHUSTR_23" },
	{ &mapnamesp[23],"PHUSTR_24" },
	{ &mapnamesp[24],"PHUSTR_25" },
	{ &mapnamesp[25],"PHUSTR_26" },
	{ &mapnamesp[26],"PHUSTR_27" },
	{ &mapnamesp[27],"PHUSTR_28" },
	{ &mapnamesp[28],"PHUSTR_29" },
	{ &mapnamesp[29],"PHUSTR_30" },
	{ &mapnamesp[30],"PHUSTR_31" },
	{ &mapnamesp[31],"PHUSTR_32" },
	{ &mapnamest[0],"THUSTR_1" },
	{ &mapnamest[1],"THUSTR_2" },
	{ &mapnamest[2],"THUSTR_3" },
	{ &mapnamest[3],"THUSTR_4" },
	{ &mapnamest[4],"THUSTR_5" },
	{ &mapnamest[5],"THUSTR_6" },
	{ &mapnamest[6],"THUSTR_7" },
	{ &mapnamest[7],"THUSTR_8" },
	{ &mapnamest[8],"THUSTR_9" },
	{ &mapnamest[9],"THUSTR_10" },
	{ &mapnamest[10],"THUSTR_11" },
	{ &mapnamest[11],"THUSTR_12" },
	{ &mapnamest[12],"THUSTR_13" },
	{ &mapnamest[13],"THUSTR_14" },
	{ &mapnamest[14],"THUSTR_15" },
	{ &mapnamest[15],"THUSTR_16" },
	{ &mapnamest[16],"THUSTR_17" },
	{ &mapnamest[17],"THUSTR_18" },
	{ &mapnamest[18],"THUSTR_19" },
	{ &mapnamest[19],"THUSTR_20" },
	{ &mapnamest[20],"THUSTR_21" },
	{ &mapnamest[21],"THUSTR_22" },
	{ &mapnamest[22],"THUSTR_23" },
	{ &mapnamest[23],"THUSTR_24" },
	{ &mapnamest[24],"THUSTR_25" },
	{ &mapnamest[25],"THUSTR_26" },
	{ &mapnamest[26],"THUSTR_27" },
	{ &mapnamest[27],"THUSTR_28" },
	{ &mapnamest[28],"THUSTR_29" },
	{ &mapnamest[29],"THUSTR_30" },
	{ &mapnamest[30],"THUSTR_31" },
	{ &mapnamest[31],"THUSTR_32" },
	{ &p1text,"P1TEXT" },
	{ &p2text,"P2TEXT" },
	{ &p3text,"P3TEXT" },
	{ &p4text,"P4TEXT" },
	{ &p5text,"P5TEXT" },
	{ &p6text,"P6TEXT" },
	{ &t1text,"T1TEXT" },
	{ &t2text,"T2TEXT" },
	{ &t3text,"T3TEXT" },
	{ &t4text,"T4TEXT" },
	{ &t5text,"T5TEXT" },
	{ &t6text,"T6TEXT" },
	{&finaleflat[0],"BGFLATE1" },
	{ &finaleflat[1],"BGFLATE2" },
	{ &finaleflat[2],"BGFLATE3" },
	{ &finaleflat[3],"BGFLATE4" },
	{ &finaleflat[4],"BGFLAT06" },
	{ &finaleflat[5],"BGFLAT11" },
	{ &finaleflat[6],"BGFLAT20" },
	{ &finaleflat[7],"BGFLAT30" },
	{ &finaleflat[8],"BGFLAT15" },
	{ &finaleflat[9],"BGFLAT31" },
	{ &finaleflat[11],"BGCASTCALL" },

};

dehbits mobfl[] = {
	{ "SPECIAL",MF_SPECIAL },
	// Blocks.
{ "SOLID",MF_SOLID },
// Can be hit.
{ "SHOOTABLE",MF_SHOOTABLE },
// Don't use the sector links (invisible but touchable).
{ "NOSECTOR",MF_NOSECTOR },
// Don't use the blocklinks (inert but displayable)
{ "NOBLOCKMAP",MF_NOBLOCKMAP },

// Not to be activated by sound, deaf monster.
{ "AMBUSH",MF_AMBUSH },
// Will try to attack right back.
{ "JUSTHIT",MF_JUSTHIT },
// Will take at least one step before attacking.
{ "JUSTATTACKED",MF_JUSTATTACKED },
// On level spawning (initial position),
//  hang from ceiling instead of stand on floor.
{ "SPAWNCEILING",MF_SPAWNCEILING },
// Don't apply gravity (every tic),
//  that is, object will float, keeping current height
//  or changing it actively.
{ "NOGRAVITY",MF_NOGRAVITY },

// Movement flags.
// This allows jumps from high places.
{ "DROPOFF",MF_DROPOFF },
// For players, will pick up items.
{ "PICKUP",MF_PICKUP },
// Player cheat. ???
{ "NOCLIP",MF_NOCLIP },
// Player: keep info about sliding along walls.
{ "SLIDE",MF_SLIDE },
// Allow moves to any height, no gravity.
// For active floaters, e.g. cacodemons, pain elementals.
{ "FLOAT",MF_FLOAT },
// Don't cross lines
//   ??? or look at heights on teleport.
{ "TELEPORT",MF_TELEPORT },
// Don't hit same species, explode on block.
// Player missiles as well as fireballs of various kinds.
{ "MISSILE",MF_MISSILE },
// Dropped by a demon, not level spawned.
// E.g. ammo clips dropped by dying former humans.
{ "DROPPED",MF_DROPPED },
// Use fuzzy draw (shadow demons or spectres),
//  temporary player invisibility powerup.
{ "SHADOW",MF_SHADOW },
// Flag: don't bleed when shot (use puff),
//  barrels and shootable furniture shall not bleed.
{ "NOBLOOD",MF_NOBLOOD },
// Don't stop moving halfway off a step,
//  that is, have dead bodies slide down all the way.
{ "CORPSE",MF_CORPSE },
// Floating to a height for a move, ???
//  don't auto float to target's height.
{ "INFLOAT",MF_INFLOAT },

// On kill, count this enemy object
//  towards intermission kill total.
// Happy gathering.
{ "COUNTKILL",MF_COUNTKILL },

// On picking up, count this item object
//  towards intermission item total.
{ "COUNTITEM",MF_COUNTITEM },

// Special handling: skull in flight.
// Neither a cacodemon nor a missile.
{ "SKULLFLY",MF_SKULLFLY },

// Don't spawn this object
//  in death match mode (e.g. key cards).
{ "NOTDMATCH",MF_NOTDMATCH },

// Player sprites in multiplayer modes are modified
//  using an internal color lookup table for re-indexing.
// If 0x4 0x8 or 0xc,
//  use a translation table for player colormaps
{ "TRANSLATION",MF_TRANSLATION },
// Hmm ???.
{ "TRANSSHIFT",MF_TRANSSHIFT }
};

int checkstate(char* text) {
	const char* stable[11] = {
		"Thing",
		"Frame",
		"Text",
		"Weapon",
		"Pointer",
		"[CODEPTR]",
		"Ammo",
		"par",
		"[STRINGS]",
		"Sound",
		"Misc"
	};
	if (text != NULL) {
		for (int i = 0; i < 11; i++) {
			if (!idStr::Icmp(text, stable[i])) {
				return i + 1;
			}
		}
	}

	return 0;
}

void setThing(int pos, char* varname, int varval) {
	//GK: This works (suprisingly)
	dehobj tvars[23] = {
		{"Initial frame ",MAXINT,NULL,&mobjinfo[pos].spawnstate},
		{"Hit points ",MAXINT,NULL,&mobjinfo[pos].spawnhealth},
		{"First moving frame ",MAXINT,NULL,&mobjinfo[pos].seestate},
		{"Alert sound ",NUMSFX,NULL,&mobjinfo[pos].seesound},
		{"Reaction time ",MAXINT,NULL,&mobjinfo[pos].reactiontime},
		{"Attack sound ",NUMSFX,NULL,&mobjinfo[pos].attacksound},
		{"Injury frame ",MAXINT,NULL,&mobjinfo[pos].painstate},
		{"Pain chance ",MAXINT,NULL,&mobjinfo[pos].painchance},
		{"Pain sound ",NUMSFX,NULL,&mobjinfo[pos].painsound},
		{"Close attack frame ",MAXINT,NULL,&mobjinfo[pos].meleestate},
		{"Far attack frame ",MAXINT,NULL,&mobjinfo[pos].missilestate},
		{"Death frame ",MAXINT,NULL,&mobjinfo[pos].deathstate,},
		{"Exploding frame ",MAXINT,NULL,&mobjinfo[pos].xdeathstate},
		{"Death sound ",NUMSFX,NULL,&mobjinfo[pos].deathsound},
		{"Speed ",MAXINT,NULL,&mobjinfo[pos].speed},
		{"Width ",MAXINT,NULL,&mobjinfo[pos].radius},
		{"Height ",MAXINT,NULL,&mobjinfo[pos].height},
		{"Mass ",MAXINT,NULL,&mobjinfo[pos].mass},
		{"Missle damage ",MAXINT,NULL,&mobjinfo[pos].damage},
		{"Action sound ",NUMSFX,NULL,&mobjinfo[pos].activesound},
		{"Bits ",MAXINT,NULL,&mobjinfo[pos].flags},
		{"Respawn frame ", MAXINT,NULL,&mobjinfo[pos].raisestate},
		{"ID # ",MAXINT,NULL,&mobjinfo[pos].doomednum},
	};
	for (int i = 0; i < 23; i++) {
		if (!idStr::Icmp(varname, tvars[i].name)) {
			if (varval < tvars[i].limit) {
				*tvars[i].ival = varval;
				return;
			}
		}
	}
}

void extendSpriteNames(int newSize) {
	if (newSize >= (int)sprnames.size() - 1) {
		int oldSize = sprnames.size();
		sprnames.resize(newSize + 2);
		for (int i = oldSize - NUMSPRITES; i <= (newSize + 1) - NUMSPRITES; i++) {
			sprnames[i + (NUMSPRITES - 1)] = (char*)malloc(5 * sizeof(char));
			sprintf(sprnames[i + (NUMSPRITES - 1)], "SP%02d", i);
		}
	}
}

void setFrame(int pos, char* varname, int varval) {
	dehobj fvars[6] = {
		{"Sprite subnumber ",MAXINT,NULL,NULL,&tempStates[pos].frame},
		{"Duration ",MAXINT,NULL,NULL,&tempStates[pos].tics},
		{"Unknown 1 ",MAXINT,NULL,NULL,&tempStates[pos].misc1},
		{"Unknown 2 ",MAXINT,NULL,NULL,&tempStates[pos].misc2},
		{"Sprite number ",MAXINT,NULL, &tempStates[pos].sprite},
		{"Next frame ",MAXINT,NULL,&tempStates[pos].nextstate}
	};
	for (int i = 0; i < 6; i++) {
		if (!idStr::Icmp(varname, fvars[i].name)) {
			if (varval < fvars[i].limit) {
				switch (i) {
				case 4:
					extendSpriteNames(varval);
					*fvars[i].ival = varval;
					break;
				case 5:
					//*fvars[i].stval = static_cast<statenum_t>(varval);
					*fvars[i].ival = varval;
					break;
				default:
					*fvars[i].lval = varval;
				}
				
				return;
			}
		}
	}
}

void setWeapon(int pos, char* varname, int varval) {
	dehobj wvars[6] = {
		{"Select frame ",MAXINT,NULL,&weaponinfo[pos].downstate},
		{"Deselect frame ",MAXINT,NULL,&weaponinfo[pos].upstate},
		{"Bobbing frame ",MAXINT,NULL,&weaponinfo[pos].readystate},
		{"Shooting frame ",MAXINT,NULL,&weaponinfo[pos].atkstate},
		{"Firing frame ",MAXINT,NULL,&weaponinfo[pos].flashstate},
		{"Ammo type ",NUMAMMO,NULL,NULL,NULL,NULL,NULL,NULL,&weaponinfo[pos].ammo}
	};
	for (int i = 0; i < 6; i++) {
		if (!idStr::Icmp(varname, wvars[i].name)) {
			if (varval < wvars[i].limit) {
				switch (i) {
				case 5:
					*wvars[i].amval = static_cast<ammotype_t>(varval);
					break;
				default:
					*wvars[i].ival = varval;
				}
				
				return;
			}
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
	varfunc++;
	if (tp != NULL) {
		int pos = atoi(tp);
		if (strlen(name) > 1 && strlen(varfunc) > 1) {
			if (!idStr::Icmp(name, "FRAME") && pos < (int)tempStates.size()) {
				tempStates[pos].action = getFunc(varfunc);
			}
		}
	}
}

void setAmmo(int pos, char* varname, int varval) {
	if (!idStr::Icmp(varname, "Max Ammo ")) {
		maxammo[pos] = varval;
		return;
	}
	else if (!idStr::Icmp(varname, "Per Ammo ")) {
		clipammo[pos] = varval;
		return;
	}
}
//More Headache than it's worth
const char* Soundtable[6] = { //Textoffset, Zero 1 and Zero 4 are wildcards you don't wanna play with
	"Zero/One ",
	"Value ",
	"Neg. One 1 ",
	"Neg. One 2 ",
	"Zero 2 ",
	"Zero 3 "
};
void setSound(int pos, char* varname, int varval) {
	int* svars[6] = {
		&S_sfx[pos].singularity,
		&S_sfx[pos].priority,
		&S_sfx[pos].usefulness,
		&S_sfx[pos].lumpnum,
		&S_sfx[pos].pitch,
		&S_sfx[pos].volume
	};
	/*if (!idStr::Icmp(varname, "Zero 1 ")) {
		S_sfx[pos].link = (sfxinfo_t*)varval;
		return;
	}*/
	for (int i = 0; i < 6; i++) {
		if (!idStr::Icmp(varname, Soundtable[i])) {
			*svars[i] = varval;
			return;
		}
	}
	
}

void setText(std::vector<std::string>lines, int i,int il,int nl) {
	int op = i;
	//int size = 0;
	//int psize = 0;
	//int nsize = 0;
	std::string newline;
	char* tst = strtok(strdup(lines[i].c_str()), " ");
	//Dealing better with the memory
	char* ntxt=new char[nl];
	strcpy(ntxt, "");
	char* otxt = new char[il];
	char* ltxt = new char[il + nl];
	while (checkstate(tst) == 0 && i < (int)lines.size()) {
		newline = lines[i] + "\n";
		if (i== op) {
			strcpy(ltxt, newline.c_str());
		}
		else {
			strcat(ltxt, newline.c_str());
		}
		i++;
		if (i < (int)lines.size()) {
			tst = strtok(strdup(lines[i].c_str()), " ");
		}
	}

	strncpy(otxt, ltxt, il-1);
	otxt[il - 1] = '\0';
	strncpy(ntxt, ltxt + il-1, nl-1);
	ntxt[nl - 1] = '\0';
	//idLib::Printf("Replacing: %s with %s size %i\n", otxt, ntxt,nsize);
	int arrsz = sizeof(strval) / sizeof(*strval);
	if (otxt != nullptr) {
		for (int j = 0; j < arrsz; j++) {

			if (!idStr::Icmp(otxt, *strval[j].var)) {
				if (::g->gamemission == pack_nerve || ::g->gamemission == pack_master) {
					if (!idStr::Icmpn(strval[j].name, "HU", 2) || (!idStr::Icmpn(strval[j].name, "E", 1) && idStr::Icmpn(strval[j].name + 1, "E", 1)) || (!idStr::Icmpn(strval[j].name, "C", 1) && idStr::Icmpn(strval[j].name + 1, "C", 1))) {
						::g->modifiedtext = true;
						if (::g->gamemode == commercial && (!idStr::Icmpn(strval[j].name, "E", 1) && idStr::Icmpn(strval[j].name + 1, "E", 1))) {
							::g->modind =strval[j].name[1] - '0';
						}
						if (::g->gamemode == commercial && (!idStr::Icmpn(strval[j].name, "C", 1) && idStr::Icmpn(strval[j].name + 1, "C", 1))) {
							::g->modftext = true;
						}
					}
					
				}
				*strval[j].var=ntxt;
				return;
			}
		}
		std::string otxtfmt = otxt;
		if (otxtfmt.find("%i") != std::string::npos) {
			sprintf(otxt, otxtfmt.c_str(),
				VERSION / 100, VERSION % 100);
		}
		char* prettyTitle = new char[100];
		sprintf(prettyTitle, "                         %s                           ", ::g->title);
		if (!idStr::Icmp(otxt, prettyTitle)) {
			char* parsedTitle = ntxt + 25;
			parsedTitle[strlen(parsedTitle) - 30] = '\0';
			strcpy(::g->title, parsedTitle);
			return;
		}
			for (int m = 0; m < NUMSPRITES - 1; m++) {
				if (!idStr::Icmp(otxt, sprnames[m])) {
					sprnames[m] = ntxt;
					return;
				}
			}
			for (int m = 0; m < 12; m++) {
				if (!idStr::Icmp(otxt, finaleflat[m])) {
					finaleflat[m] = ntxt;
					return;
				}
			}
			for (int m = 1; m < NUMMUSIC; m++) {
				if (!idStr::Icmp(otxt, ::g->S_music[m].name)) {
					::g->S_music[m].name = ntxt;
					return;
				}
			}
			for (int m = 1; m < NUMSFX; m++) {
				if (!idStr::Icmp(otxt, S_sfx[m].name)) {
					S_sfx[m].name = ntxt;
					return;
				}
			}
	}
	//free(otxt);
	
	//free(ntxt);
	//free(otxt);
	return;
}

void setPars(int pos, int val, int val2=-1) {
	if (val2 == -1) {
		cpars[pos-1] = val;
		return;
	}
	else {
		pars[pos][val] = val2;
		return;
	}
}

void setBText(char* varname, char* text) {
	int arrsz = sizeof(strval) / sizeof(*strval);
	text++;
	std::string ttext =text;
	for (uint i = 0; i < ttext.size(); i++) {
		if (ttext[i] == '\\' && ttext[i+1] == 'n') {
			ttext[i] = '\n';
			ttext[i + 1] = '\b';
			uint j = 3;
			if ( i + 2 < ttext.size() && ttext[i + 2] == '\\' ) {
				if (i + j < ttext.size() && ttext[i + j] == 'n') {
					ttext[i + 2] = '\n';
					ttext[i + j] = '\b';
					j++;
					if (i + j < ttext.size() && ttext[i + j] == '\\') {
						ttext[i + j] = '\b';
						j++;
					}
				}
				else {
					ttext[i + 2] = '\b';
				}
			}
			if (i + j < ttext.size()) {
				while (ttext[i + j] == ' ') {
					ttext[i + j] = '\b';
					j++;
					if (i + j > ttext.size()) {
						break;
					}
				}
			}
		}
	}
	varname[strlen(varname) - 1] = '\0';
	for (int i = 0; i < arrsz; i++) {
		if (!idStr::Icmp(varname, strval[i].name)) {
			*strval[i].var = strdup(ttext.c_str());
			return;
		}
	}
}

int Getflag(char* text) {
	int size = sizeof(mobfl) / sizeof(*mobfl);
	for (int i = 0; i < size; i++) {
		if (!idStr::Icmp(text, mobfl[i].name)) {
			return mobfl[i].bit;
		}
	}
	return 0;
}

int Generateflags(char* text) {
	char*ttext = text + 1;
	char* tst = strtok(strdup(ttext), "+");
	int flags= Getflag(tst);
	tst = strtok(NULL, "+");
	while (tst != NULL) {
		flags=flags|Getflag(tst);
		tst = strtok(NULL, "+");
	}
	return flags;
}

const char* misctable[15] = { //It's not more than the Things editor but still so many repetive values
	"Initial Health ",
	"Max Health ",
	"Initial Bullets ",
	"Max Armor ",
	"Green Armor Class ",
	"Blue Armor Class ",
	"Max Soulsphere ",
	"Soulsphere Health ",
	"Megasphere Health ",
	"God Mode Health ",
	"IDFA Armor ",
	"IDFA Armor Class ",
	"IDKFA Armor ",
	"IDKFA Armor Class ",
	"BFG Cells/Shot "
};

void setMisc(char* varname, int varval) {
	int *miscvars[15] = {
	&::g->ihealth,
	&::g->mhealth,
	&::g->iammo,
	&::g->marmor,
	&::g->gart,
	&::g->bart,
	&::g->msoul,
	&::g->psoul,
	&::g->pmega,
	&::g->ghealth,
	&::g->farmor,
	&::g->fart,
	&::g->kfarmor,
	&::g->kfart,
	&::g->BFGCELL
	};
	for (int i = 0; i < 15; i++) {
		if (!idStr::Icmp(varname, misctable[i])) {
			*miscvars[i] = varval;
			return;
		}
	}
}

std::vector<std::string> getlines(char* text) {
	std::vector<std::string> lines;
	int size = strlen(text);
	for (int i = 0; i < size /*- 7*/; i++) {
		std::string letter = "";
		qboolean ignore = false;
		while (text[i] != '\n') {
			if (text[i] == '#' && text[i - 3] != 'I' && text[i - 2] != 'D')
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

void parsetext(char* text) {
	std::vector<std::string> linedtext = getlines(text);
	int state = 0;
	char* varname;
	char* varfunc = NULL;
	int statepos = 0;
	int varval = 0;
	int varval2 = -1;
	char eq = '=';
	std::string vartext = "";
	for (uint i = 0; i < linedtext.size(); i++) {
		varval2 = -1;
		//I_Printf("%s\n", linedtext[i].c_str());
		if ((linedtext[i].find(eq) != std::string::npos) && state != 3 && state != 0) {
			varname = strtok(strdup(linedtext[i].c_str()), "=");
			std::string tv3 = strtok(NULL, "=");
			if (!tv3.empty()) {
				if (state != 6 && state != 9) {
					varval = atoi(tv3.c_str());
					if (state == 1 && !idStr::Icmp(varname, "Bits ") && varval == 0) {
						varval = Generateflags(strdup(tv3.c_str()));
					}
				}
				else if (state == 6) {
					varfunc = strdup(tv3.c_str());
				}
				else {
					vartext += strdup(tv3.c_str());
					i++;
					if (i < linedtext.size()) {
						while (linedtext[i].find(eq) == std::string::npos && linedtext[i] != "") {
							vartext += strdup(linedtext[i].c_str());
							if (i == linedtext.size() - 1) {
								break;
							}
							i++;
						}
						i = i - 1;
					}
				}
			}
			//idLib::Printf("%s = %i\n", varname, varval);
			switch (state) {
			case 1:
				setThing(statepos - 1, varname, varval);
				break;
			case 2:
				setFrame(statepos, varname, varval);
				//memcpy(::g->states, tempStates, sizeof(tempStates));
				::g->states = tempStates;
				break;
			case 4:
				setWeapon(statepos, varname, varval);
				break;
			case 5:
				setPointer(statepos, varname, varval);
				//memcpy(::g->states, tempStates, sizeof(tempStates));
				::g->states = tempStates;
				break;
			case 6:
				setCptr(varname, varfunc);
				//memcpy(::g->states, tempStates, sizeof(tempStates));
				::g->states = tempStates;
				break;
			case 7:
				setAmmo(statepos, varname, varval);
				break;
			case 9:
				setBText(varname, strdup(vartext.c_str()));
				vartext = "";
				break;
				//More Headache than it's worth
			case 10:
				setSound(statepos, varname, varval);
				break;
			case 11:
				setMisc(varname, varval);
				break;
			}


		}
		else {
			if (linedtext[i].length() > 1) {
				char* tst = strtok(strdup(linedtext[i].c_str()), " ");
				char* tval = strtok(NULL, " ");
				if (tst != NULL && tval != NULL) {

					statepos = atoi(tval);
					state = checkstate(tst);
					int tpos = -1;
					char* tv;
					switch (state) {
					case 1:
						tpos = statepos - 1;
						if (tpos >= NUMMOBJTYPES) {
							//I_Error("No such Thing found");
							mobjinfo.resize(tpos + 1);
						}
						break;
					case 2:
						if (statepos >= (int)tempStates.size()) {
							//I_Error("No such Frame found");
							//int oldsize = tempStates.size();
							tempStates.resize(statepos + 1);
							/*for (int i = oldsize; i < tempStates.size(); i++) {
								tempStates[i].nextstate = i + 1;
							}*/
						}
						break;
					case 3:
						statepos = statepos + 1;
						varval2 = atoi(strtok(NULL, " "))+1;
						setText(linedtext, i + 1, statepos, varval2);
						break;
					case 4:
						if (statepos >= NUMWEAPONS) {
							I_Error("No such Weapon found");
						}
						break;
					case 5:
						tv = strtok(NULL, " ");
						if (tv != NULL) {
							char* tv2 = strtok(NULL, " ");
							if (tv2 != NULL) {
								tv2[strlen(tv2) - 1] = '\0';
								statepos = atoi(tv2);

								if (statepos >= NUMSTATES) {
									idLib::Printf("%i\n", statepos);
									I_Error("No such codeptr found");
								}
							}
						}
						break;
					case 7:
						if (statepos >= NUMAMMO) {
							I_Error("No such Ammo found");
						}
						break;
					case 8:
						tval = strtok(NULL, " ");
						if (tval != NULL) {
							varval = atoi(tval);
						}
						tval = strtok(NULL, " ");
						if (tval != NULL) {
							varval2 = atoi(tval);

						}
						if (varval2 == -1) {
							if (::g->gamemode == commercial) {
								if (::g->gamemission == pack_custom) {
									if (statepos > 0 && statepos <= (int)::g->maps.size()) {
										::g->maps[statepos - 1].par = varval;
									}
									else {
										I_Error("No map found");
									}
								}
								else {
									if (statepos > 0 && statepos <= 33) {
										setPars(statepos, varval);
									}
									else {
										I_Error("No map found");
									}
								}
							}
						}
						else {
							
							if (::g->gamemode == retail) {
								if (::g->gamemission == pack_custom) {
									if (statepos > 0 && statepos <= (int)::g->clusters.size() && ::g->clusters[statepos].startmap > 0) {
										if (varval > 0 && varval <= (int)::g->maps.size()) {
											::g->maps[((::g->clusters[statepos - 1].startmap - 1) + varval) - 1].par = varval2;
										}
										else {
											I_Error("No level found");
										}
									}
									else if (statepos > 0 && statepos <= (int)::g->clusters.size()) {
										setPars(statepos, varval, varval2);
									}
									else {
										I_Error("No episode found");
									}
								}
								else {
									if (statepos > 0 && statepos <= 4) {
										if (varval > 0 && varval <= 9) {
											setPars(statepos, varval, varval2);
										}
										else {
											I_Error("No level found");
										}
									}
									else {
										I_Error("No episode found");
									}
								}
							}
						}
						break;
						//More Headache than it's worth
					case 10:
						if (statepos >= NUMSFX) {
							I_Error("No such Sound found");
						}
						break;
					}

				}
				else {
					if (tst != NULL) {
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

void loaddeh(int lump) {
	char* text = (char*)malloc(W_LumpLength(lump) + 1);
	text[W_LumpLength(lump)] = '\0';
	I_Printf("Applying DeHackeD patch ...\n");
	W_ReadLump(lump, text);

	//idLib::Printf("%s", text);
	parsetext(text);
	free(text);
	I_Printf("DeHackeD patch succesfully applied\n");
}