/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/
#include "precompiled.h"
#include "d_englsh.h"
//GK:Use string values as varibles for DeHackeD string Editor
char* GOTARMOR;
char* GOTMEGA;
char* GOTHTHBONUS;
char* GOTARMBONUS;
char* GOTSTIM;
char* GOTMEDINEED;
char* GOTMEDIKIT;
char* GOTSUPER;

char* GOTBLUECARD;
char* GOTYELWCARD;
char* GOTREDCARD;
char* GOTBLUESKUL;
char* GOTYELWSKUL;
char* GOTREDSKULL;

char* GOTINVUL;
char* GOTBERSERK;
char* GOTINVIS;
char* GOTSUIT;
char* GOTMAP;
char* GOTVISOR;
char* GOTMSPHERE;

char* GOTCLIP;
char* GOTCLIPBOX;
char* GOTROCKET;
char* GOTROCKBOX;
char* GOTCELL;
char* GOTCELLBOX;
char* GOTSHELLS;
char* GOTSHELLBOX;
char* GOTBACKPACK;

char* GOTBFG9000;
char* GOTCHAINGUN;
char* GOTCHAINSAW;
char* GOTLAUNCHER;
char* GOTPLASMA;
char* GOTSHOTGUN;
char* GOTSHOTGUN2;

char* CC_ZOMBIE;
char* CC_SHOTGUN;
char* CC_HEAVY;
char* CC_IMP;
char* CC_DEMON;
char* CC_LOST;
char* CC_CACO;
char* CC_HELL;
char* CC_BARON;
char* CC_ARACH;
char* CC_PAIN;
char* CC_REVEN;
char* CC_MANCU;
char* CC_ARCH;
char* CC_SPIDER;
char* CC_CYBER;
char* CC_HERO;
char* PD_BLUEO;
char* PD_REDO;
char* PD_YELLOWO;
char* PD_BLUEK;
char* PD_REDK;
char* PD_YELLOWK;
//GK:Add these text from BOOM for generalized switches
char* PD_BLUEC;
char* PD_REDC;
char* PD_YELLOWC;
char* PD_BLUES;
char* PD_REDS;
char* PD_YELLOWS;
char* PD_ANY;
char* PD_ALL3;
char* PD_ALL6;

void initModdableTexts() {
	//GK:Use string values as varibles for DeHackeD string Editor
	GOTARMOR = (char*)"Picked up the armor.";
	GOTMEGA = (char*)"Picked up the MegaArmor!";
	GOTHTHBONUS = (char*)"Picked up a health bonus.";
	GOTARMBONUS = (char*)"Picked up an armor bonus.";
	GOTSTIM = (char*)"Picked up a stimpack.";
	GOTMEDINEED = (char*)"Picked up a medikit that you REALLY need!";
	GOTMEDIKIT = (char*)"Picked up a medikit.";
	GOTSUPER = (char*)"Supercharge!";

	GOTBLUECARD = (char*)"Picked up a blue keycard.";
	GOTYELWCARD = (char*)"Picked up a yellow keycard.";
	GOTREDCARD = (char*)"Picked up a red keycard.";
	GOTBLUESKUL = (char*)"Picked up a blue skull key.";
	GOTYELWSKUL = (char*)"Picked up a yellow skull key.";
	GOTREDSKULL = (char*)"Picked up a red skull key.";

	GOTINVUL = (char*)"Invulnerability!";
	GOTBERSERK = (char*)"Berserk!";
	GOTINVIS = (char*)"Partial Invisibility";
	GOTSUIT = (char*)"Radiation Shielding Suit";
	GOTMAP = (char*)"Computer Area Map";
	GOTVISOR = (char*)"Light Amplification Visor";
	GOTMSPHERE = (char*)"MegaSphere!";

	GOTCLIP = (char*)"Picked up a clip.";
	GOTCLIPBOX = (char*)"Picked up a box of bullets.";
	GOTROCKET = (char*)"Picked up a rocket.";
	GOTROCKBOX = (char*)"Picked up a box of rockets.";
	GOTCELL = (char*)"Picked up an energy cell.";
	GOTCELLBOX = (char*)"Picked up an energy cell pack.";
	GOTSHELLS = (char*)"Picked up 4 shotgun shells.";
	GOTSHELLBOX = (char*)"Picked up a box of shotgun shells.";
	GOTBACKPACK = (char*)"Picked up a backpack full of ammo!";

	GOTBFG9000 = (char*)"You got the BFG9000!  Oh, yes.";
	GOTCHAINGUN = (char*)"You got the chaingun!";
	GOTCHAINSAW = (char*)"A chainsaw!  Find some meat!";
	GOTLAUNCHER = (char*)"You got the rocket launcher!";
	GOTPLASMA = (char*)"You got the plasma gun!";
	GOTSHOTGUN = (char*)"You got the shotgun!";
	GOTSHOTGUN2 = (char*)"You got the super shotgun!";

	CC_ZOMBIE = (char*)"ZOMBIEMAN";
	CC_SHOTGUN = (char*)"SHOTGUN GUY";
	CC_HEAVY = (char*)"HEAVY WEAPON DUDE";
	CC_IMP = (char*)"IMP";
	CC_DEMON = (char*)"DEMON";
	CC_LOST = (char*)"LOST SOUL";
	CC_CACO = (char*)"CACODEMON";
	CC_HELL = (char*)"HELL KNIGHT";
	CC_BARON = (char*)"BARON OF HELL";
	CC_ARACH = (char*)"ARACHNOTRON";
	CC_PAIN = (char*)"PAIN ELEMENTAL";
	CC_REVEN = (char*)"REVENANT";
	CC_MANCU = (char*)"MANCUBUS";
	CC_ARCH = (char*)"ARCH-VILE";
	CC_SPIDER = (char*)"THE SPIDER MASTERMIND";
	CC_CYBER = (char*)"THE CYBERDEMON";
	CC_HERO = (char*)"OUR HERO";
	PD_BLUEO = (char*)"You need a blue key to activate this object";
	PD_REDO = (char*)"You need a red key to activate this object";
	PD_YELLOWO = (char*)"You need a yellow key to activate this object";
	PD_BLUEK = (char*)"You need a blue key to open this door";
	PD_REDK = (char*)"You need a red key to open this door";
	PD_YELLOWK = (char*)"You need a yellow key to open this door";
	//GK:Add these text from BOOM for generalized switches
	PD_BLUEC = (char*)"You need a blue card to open this door";
	PD_REDC = (char*)"You need a red card to open this door";
	PD_YELLOWC = (char*)"You need a yellow card to open this door";
	PD_BLUES = (char*)"You need a blue skull to open this door";
	PD_REDS = (char*)"You need a red skull to open this door";
	PD_YELLOWS = (char*)"You need a yellow skull to open this door";
	PD_ANY = (char*)"You need any key to open this door";
	PD_ALL3 = (char*)"You need all 3 keys to open this door";
	PD_ALL6 = (char*)"You need all 6 keys to open this door";

}

const char* MASTER[] = { "All Maps","Select Map" };
const char* masterlist[] = { "ATTACK.WAD",
"CANYON.WAD",
"CATWALK.WAD",
"COMBINE.WAD",
"FISTULA.WAD",
"GARRISON.WAD",
"MANOR.WAD",
"PARADOX.WAD",
"SUBSPACE.WAD",
"SUBTERRA.WAD",
"TTRAP.WAD",
"VIRGIL.WAD",
"MINOS.WAD",
"BLOODSEA.WAD",
"MEPHISTO.WAD",
"NESSUS.WAD",
"GERYON.WAD",
"VESPERAS.WAD",
"BLACKTWR.WAD",
"TEETH.WAD" };

const char* M2[] = { "1-10","11-20" };