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
	GOTARMOR = "Picked up the armor.";
	GOTMEGA = "Picked up the MegaArmor!";
	GOTHTHBONUS = "Picked up a health bonus.";
	GOTARMBONUS = "Picked up an armor bonus.";
	GOTSTIM = "Picked up a stimpack.";
	GOTMEDINEED = "Picked up a medikit that you REALLY need!";
	GOTMEDIKIT = "Picked up a medikit.";
	GOTSUPER = "Supercharge!";

	GOTBLUECARD = "Picked up a blue keycard.";
	GOTYELWCARD = "Picked up a yellow keycard.";
	GOTREDCARD = "Picked up a red keycard.";
	GOTBLUESKUL = "Picked up a blue skull key.";
	GOTYELWSKUL = "Picked up a yellow skull key.";
	GOTREDSKULL = "Picked up a red skull key.";

	GOTINVUL = "Invulnerability!";
	GOTBERSERK = "Berserk!";
	GOTINVIS = "Partial Invisibility";
	GOTSUIT = "Radiation Shielding Suit";
	GOTMAP = "Computer Area Map";
	GOTVISOR = "Light Amplification Visor";
	GOTMSPHERE = "MegaSphere!";

	GOTCLIP = "Picked up a clip.";
	GOTCLIPBOX = "Picked up a box of bullets.";
	GOTROCKET = "Picked up a rocket.";
	GOTROCKBOX = "Picked up a box of rockets.";
	GOTCELL = "Picked up an energy cell.";
	GOTCELLBOX = "Picked up an energy cell pack.";
	GOTSHELLS = "Picked up 4 shotgun shells.";
	GOTSHELLBOX = "Picked up a box of shotgun shells.";
	GOTBACKPACK = "Picked up a backpack full of ammo!";

	GOTBFG9000 = "You got the BFG9000!  Oh, yes.";
	GOTCHAINGUN = "You got the chaingun!";
	GOTCHAINSAW = "A chainsaw!  Find some meat!";
	GOTLAUNCHER = "You got the rocket launcher!";
	GOTPLASMA = "You got the plasma gun!";
	GOTSHOTGUN = "You got the shotgun!";
	GOTSHOTGUN2 = "You got the super shotgun!";

	CC_ZOMBIE = "ZOMBIEMAN";
	CC_SHOTGUN = "SHOTGUN GUY";
	CC_HEAVY = "HEAVY WEAPON DUDE";
	CC_IMP = "IMP";
	CC_DEMON = "DEMON";
	CC_LOST = "LOST SOUL";
	CC_CACO = "CACODEMON";
	CC_HELL = "HELL KNIGHT";
	CC_BARON = "BARON OF HELL";
	CC_ARACH = "ARACHNOTRON";
	CC_PAIN = "PAIN ELEMENTAL";
	CC_REVEN = "REVENANT";
	CC_MANCU = "MANCUBUS";
	CC_ARCH = "ARCH-VILE";
	CC_SPIDER = "THE SPIDER MASTERMIND";
	CC_CYBER = "THE CYBERDEMON";
	CC_HERO = "OUR HERO";
	PD_BLUEO = "You need a blue key to activate this object";
	PD_REDO = "You need a red key to activate this object";
	PD_YELLOWO = "You need a yellow key to activate this object";
	PD_BLUEK = "You need a blue key to open this door";
	PD_REDK = "You need a red key to open this door";
	PD_YELLOWK = "You need a yellow key to open this door";
	//GK:Add these text from BOOM for generalized switches
	PD_BLUEC = "You need a blue card to open this door";
	PD_REDC = "You need a red card to open this door";
	PD_YELLOWC = "You need a yellow card to open this door";
	PD_BLUES = "You need a blue skull to open this door";
	PD_REDS = "You need a red skull to open this door";
	PD_YELLOWS = "You need a yellow skull to open this door";
	PD_ANY = "You need any key to open this door";
	PD_ALL3 = "You need all 3 keys to open this door";
	PD_ALL6 = "You need all 6 keys to open this door";

}

char* MASTER[] = { "All Maps","Select Map" };
char* masterlist[] = { "ATTACK.WAD",
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

char* M2[] = { "1-10","11-20" };