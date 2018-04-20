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
char* GOTARMOR	= "Picked up the armor.";
char* GOTMEGA = "Picked up the MegaArmor!";
char* GOTHTHBONUS=	"Picked up a health bonus.";
char* GOTARMBONUS	="Picked up an armor bonus.";
char* GOTSTIM	="Picked up a stimpack.";
char* GOTMEDINEED=	"Picked up a medikit that you REALLY need!";
char* GOTMEDIKIT=	"Picked up a medikit.";
char* GOTSUPER=	"Supercharge!";

char* GOTBLUECARD = "Picked up a blue keycard.";
char* GOTYELWCARD = "Picked up a yellow keycard.";
char* GOTREDCARD = "Picked up a red keycard.";
char* GOTBLUESKUL = "Picked up a blue skull key.";
char* GOTYELWSKUL = "Picked up a yellow skull key.";
char* GOTREDSKULL = "Picked up a red skull key.";

char* GOTINVUL = "Invulnerability!";
char* GOTBERSERK = "Berserk!";
char* GOTINVIS = "Partial Invisibility";
char* GOTSUIT = "Radiation Shielding Suit";
char* GOTMAP = "Computer Area Map";
char* GOTVISOR = "Light Amplification Visor";
char* GOTMSPHERE = "MegaSphere!";

char* GOTCLIP = "Picked up a clip.";
char* GOTCLIPBOX = "Picked up a box of bullets.";
char* GOTROCKET = "Picked up a rocket.";
char* GOTROCKBOX = "Picked up a box of rockets.";
char* GOTCELL = "Picked up an energy cell.";
char* GOTCELLBOX = "Picked up an energy cell pack.";
char* GOTSHELLS = "Picked up 4 shotgun shells.";
char* GOTSHELLBOX = "Picked up a box of shotgun shells.";
char* GOTBACKPACK = "Picked up a backpack full of ammo!";

char* GOTBFG9000 = "You got the BFG9000!  Oh, yes.";
char* GOTCHAINGUN = "You got the chaingun!";
char* GOTCHAINSAW = "A chainsaw!  Find some meat!";
char* GOTLAUNCHER = "You got the rocket launcher!";
char* GOTPLASMA = "You got the plasma gun!";
char* GOTSHOTGUN = "You got the shotgun!";
char* GOTSHOTGUN2 = "You got the super shotgun!";


char* C2TEXT =
"YOU HAVE WON! YOUR VICTORY HAS ENABLED\n"
"HUMANKIND TO EVACUATE EARTH AND ESCAPE\n"
"THE NIGHTMARE.  NOW YOU ARE THE ONLY\n"
"HUMAN LEFT ON THE FACE OF THE PLANET.\n"
"CANNIBAL MUTATIONS, CARNIVOROUS ALIENS,\n"
"AND EVIL SPIRITS ARE YOUR ONLY NEIGHBORS.\n"
"YOU SIT BACK AND WAIT FOR DEATH, CONTENT\n"
"THAT YOU HAVE SAVED YOUR SPECIES.\n"
"\n"
"BUT THEN, EARTH CONTROL BEAMS DOWN A\n"
"MESSAGE FROM SPACE: \"SENSORS HAVE LOCATED\n"
"THE SOURCE OF THE ALIEN INVASION. IF YOU\n"
"GO THERE, YOU MAY BE ABLE TO BLOCK THEIR\n"
"ENTRY.  THE ALIEN BASE IS IN THE HEART OF\n"
"YOUR OWN HOME CITY, NOT FAR FROM THE\n"
"STARPORT.\" SLOWLY AND PAINFULLY YOU GET\n"
"UP AND RETURN TO THE FRAY.";

char* CC_ZOMBIE = "ZOMBIEMAN";
char* CC_SHOTGUN = "SHOTGUN GUY";
char* CC_HEAVY = "HEAVY WEAPON DUDE";
char* CC_IMP = "IMP";
char* CC_DEMON = "DEMON";
char* CC_LOST = "LOST SOUL";
char* CC_CACO = "CACODEMON";
char* CC_HELL = "HELL KNIGHT";
char* CC_BARON = "BARON OF HELL";
char* CC_ARACH = "ARACHNOTRON";
char* CC_PAIN = "PAIN ELEMENTAL";
char* CC_REVEN = "REVENANT";
char* CC_MANCU = "MANCUBUS";
char* CC_ARCH = "ARCH-VILE";
char* CC_SPIDER = "THE SPIDER MASTERMIND";
char* CC_CYBER = "THE CYBERDEMON";
char* CC_HERO = "OUR HERO";
char* PD_BLUEO = "You need a blue key to activate this object";
char* PD_REDO = "You need a red key to activate this object";
char* PD_YELLOWO = "You need a yellow key to activate this object";
char* PD_BLUEK = "You need a blue key to open this door";
char* PD_REDK = "You need a red key to open this door";
char* PD_YELLOWK = "You need a yellow key to open this door";
//GK:Add these text from BOOM for generalized switches
char* PD_BLUEC = "You need a blue card to open this door";
char* PD_REDC = "You need a red card to open this door";
char* PD_YELLOWC = "You need a yellow card to open this door";
char* PD_BLUES = "You need a blue skull to open this door";
char* PD_REDS = "You need a red skull to open this door";
char* PD_YELLOWS = "You need a yellow skull to open this door";
char* PD_ANY = "You need any key to open this door";
char* PD_ALL3 = "You need all 3 keys to open this door";
char* PD_ALL6 = "You need all 6 keys to open this door";

void resetTexts() {
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