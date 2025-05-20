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

#include "Precompiled.h"
#include "globaldata.h"
#include "Main.h"

#include <stdlib.h>

#include "doomdef.h"
#include "doomstat.h"

#include "i_system.h"
#include "z_zone.h"
#include "m_argv.h"
#include "m_random.h"
#include "w_wad.h"

#include "r_local.h"
#include "p_local.h"

#include "g_game.h"

#include "s_sound.h"

// State.
#include "r_state.h"

// Data.
#include "sounds.h"

#include "../../neo/d3xp/Game_local.h"

#include "d_exp.h"

#include "m_bbox.h"
//
// Animating textures and planes
// There is another anim_t used in wi_stuff, unrelated. BLAH!
// we now use anim_t2
//

//
//      source animation definition
//





//
// P_InitPicAnims
//

// Floor/ceiling animation sequences,
//  defined by first and last frame,
//  i.e. the flat (64x64 tile) name to
//  be used.
// The full animation sequence is given
//  using all the flats between the start
//  and end entry, in the order found in
//  the WAD file.
//
/*const animdef_t		animdefs[] =
{
	{false,	"NUKAGE3",	"NUKAGE1",	8},
	{false,	"FWATER4",	"FWATER1",	8},
	{false,	"SWATER4",	"SWATER1", 	8},
	{false,	"LAVA4",	"LAVA1",	8},
	{false,	"BLOOD3",	"BLOOD1",	8},

	// DOOM II flat animations.
	{false,	"RROCK08",	"RROCK05",	8},		
	{false,	"SLIME04",	"SLIME01",	8},
	{false,	"SLIME08",	"SLIME05",	8},
	{false,	"SLIME12",	"SLIME09",	8},

	{true,	"BLODGR4",	"BLODGR1",	8},
	{true,	"SLADRIP3",	"SLADRIP1",	8},

	{true,	"BLODRIP4",	"BLODRIP1",	8},
	{true,	"FIREWALL",	"FIREWALA",	8},
	{true,	"GSTFONT3",	"GSTFONT1",	8},
	{true,	"FIRELAVA",	"FIRELAV3",	8},
	{true,	"FIREMAG3",	"FIREMAG1",	8},
	{true,	"FIREBLU2",	"FIREBLU1",	8},
	{true,	"ROCKRED3",	"ROCKRED1",	8},

	{true,	"BFALL4",	"BFALL1",	8},
	{true,	"SFALL4",	"SFALL1",	8},
	{true,	"WFALL4",	"WFALL1",	8},
	{true,	"DBRAIN4",	"DBRAIN1",	8},

	{-1}
};*/

// killough 3/7/98: Initialize generalized scrolling
static void P_InitTagLists(void);
static void P_SpawnScrollers(void);
static void P_SpawnFriction(void);
static void P_SpawnPushers(void);

//
//      Animating line specials
//




void P_InitPicAnims (void)
{
	int		i;
	size_t index = 0;
	animdef_t* animList = NULL;
	animList = (animdef_t*)W_CacheLumpName("ANIMATED", PU_CACHE);
	int animListSize = W_LumpLength(W_GetNumForName("ANIMATED"));
	if (!animList) {
		animList = ::g->animdefs;
		animListSize = sizeof(::g->animdefs);
	}



	//	Init animation
	//::g->lastanim = ::g->anims;
	int animSize = animListSize / sizeof(animdef_t);
	::g->anims.resize(animSize * 2);
	for (i=0 ; animList[i].istexture != -1 ; i++)
	{
		if (animList[i].istexture)
		{
			// different episode ?
			if (R_CheckTextureNumForName(animList[i].startname) == -1)
				continue;	

			::g->anims[index].picnum = R_TextureNumForName (animList[i].endname);
			::g->anims[index].basepic = R_TextureNumForName (animList[i].startname);
		}
		else
		{
			if (W_CheckNumForName(animList[i].startname) == -1)
				continue;

			::g->anims[index].picnum = R_FlatNumForName (animList[i].endname);
			::g->anims[index].basepic = R_FlatNumForName (animList[i].startname);
		}

		::g->anims[index].istexture = animList[i].istexture;
		::g->anims[index].numpics = ::g->anims[index].picnum - ::g->anims[index].basepic + 1;

		if (::g->anims[index].numpics < 2)
			I_Error ("P_InitPicAnims: bad cycle from %s to %s",
				animList[i].startname,
				animList[i].endname);

		memcpy(&::g->anims[index].speed, animList[i].speed, 4);
		index++;
	}

}



//
// UTILITIES
//



//
// getSide()
// Will return a side_t*
//  given the number of the current sector,
//  the line number, and the side (0/1) that you want.
//
side_t*
getSide
( int		currentSector,
 int		line,
 int		side )
{
	return &::g->sides[ (::g->sectors[currentSector].lines[line])->sidenum[side] ];
}


//
// getSector()
// Will return a sector_t*
//  given the number of the current sector,
//  the line number and the side (0/1) that you want.
//
sector_t*
getSector
( int		currentSector,
 int		line,
 int		side )
{
	return ::g->sides[ (::g->sectors[currentSector].lines[line])->sidenum[side] ].sector;
}


//
// twoSided()
// Given the sector number and the line number,
//  it will tell you whether the line is two-sided or not.
//
int
twoSided
( int	sector,
 int	line )
{
	return (::g->sectors[sector].lines[line])->flags & ML_TWOSIDED;
}




//
// getNextSector()
// Return sector_t * of sector next to current.
// NULL if not two-sided line
//
sector_t*
getNextSector
( line_t*	line,
 sector_t*	sec )
{
	if (!(line->flags & ML_TWOSIDED))
		return NULL;

	if (line->frontsector == sec)
		return line->backsector;

	return line->frontsector;
}



//
// P_FindLowestFloorSurrounding()
// FIND LOWEST FLOOR HEIGHT IN SURROUNDING SECTORS
//
fixed_t	P_FindLowestFloorSurrounding(sector_t* sec)
{
	int			i;
	line_t*		check;
	sector_t*		other;
	fixed_t		floor = sec->floorheight;

	for (i=0 ;i < sec->linecount ; i++)
	{
		check = sec->lines[i];
		other = getNextSector(check,sec);

		if (!other)
			continue;

		if (other->floorheight < floor)
			floor = other->floorheight;
	}
	return floor;
}



//
// P_FindHighestFloorSurrounding()
// FIND HIGHEST FLOOR HEIGHT IN SURROUNDING SECTORS
//
fixed_t	P_FindHighestFloorSurrounding(sector_t *sec)
{
	int			i;
	line_t*		check;
	sector_t*		other;
	fixed_t		floor = -500*FRACUNIT;

	for (i=0 ;i < sec->linecount ; i++)
	{
		check = sec->lines[i];
		other = getNextSector(check,sec);

		if (!other)
			continue;

		if (other->floorheight > floor)
			floor = other->floorheight;
	}
	return floor;
}



//
// P_FindNextHighestFloor
// FIND NEXT HIGHEST FLOOR IN SURROUNDING SECTORS
// Note: this should be doable w/o a fixed array.

// 20 adjoining ::g->sectors max!

fixed_t
P_FindNextHighestFloor
( sector_t*	sec,
 int		currentheight )
{
	int			i;
	int			h;
	int			min;
	line_t*		check;
	sector_t*		other;
	fixed_t		height = currentheight;


	std::vector<fixed_t>		heightlist;	

	for (i=0, h=0 ;i < sec->linecount ; i++)
	{
		check = sec->lines[i];
		other = getNextSector(check,sec);

		if (!other)
			continue;

		if (other->floorheight > height) {
			if (h >= (int)heightlist.size()) {
				heightlist.emplace_back(other->floorheight);
				h++;
			}
			else {
				heightlist[h++] = other->floorheight;
			}
		}

	}

	// Find lowest height in list
	if (!h)
		return currentheight;

	min = heightlist[0];

	// Range checking? 
	for (i = 1;i < h;i++)
		if (heightlist[i] < min)
			min = heightlist[i];

	return min;
}


//
// FIND LOWEST CEILING IN THE SURROUNDING SECTORS
//
fixed_t
P_FindLowestCeilingSurrounding(sector_t* sec)
{
	int			i;
	line_t*		check;
	sector_t*		other;
	fixed_t		height = MAXINT;

	for (i=0 ;i < sec->linecount ; i++)
	{
		check = sec->lines[i];
		other = getNextSector(check,sec);

		if (!other)
			continue;

		if (other->ceilingheight < height)
			height = other->ceilingheight;
	}
	return height;
}


//
// FIND HIGHEST CEILING IN THE SURROUNDING SECTORS
//
fixed_t	P_FindHighestCeilingSurrounding(sector_t* sec)
{
	int		i;
	line_t*	check;
	sector_t*	other;
	fixed_t	height = 0;

	for (i=0 ;i < sec->linecount ; i++)
	{
		check = sec->lines[i];
		other = getNextSector(check,sec);

		if (!other)
			continue;

		if (other->ceilingheight > height)
			height = other->ceilingheight;
	}
	return height;
}



//
// RETURN NEXT SECTOR # THAT LINE TAG REFERS TO
//
int
P_FindSectorFromLineTag
( line_t*	line,
 int		start )
{
	start = start >= 0 ? ::g->sectors[start].nexttag :
		::g->sectors[(unsigned)line->tag % (unsigned)::g->numsectors].firsttag;
	while (start >= 0 && ::g->sectors[start].tag != line->tag)
		start = ::g->sectors[start].nexttag;
	return start;
	// int	i;

	// for (i = start+1; i < ::g->numsectors; i++)
	// 	if (::g->sectors[i].tag == line->tag)
	// 		return i;

	// return -1;
}




//
// Find minimum light from an adjacent sector
//
int
P_FindMinSurroundingLight
( sector_t*	sector,
 int		max )
{
	int		i;
	int		min;
	line_t*	line;
	sector_t*	check;

	min = max;
	for (i=0 ; i < sector->linecount ; i++)
	{
		line = sector->lines[i];
		check = getNextSector(line,sector);

		if (!check)
			continue;

		if (check->lightlevel < min)
			min = check->lightlevel;
	}
	return min;
}



//
// EVENTS
// Events are operations triggered by using, crossing,
// or shooting special ::g->lines, or by timed thinkers.
//

//
// P_CrossSpecialLine - TRIGGER
// Called every time a thing origin is about
//  to cross a line with a non 0 special.
//
void
P_CrossSpecialLine
( int		linenum,
 int		side,
 mobj_t*	thing )
{
	line_t*	line;
	int		ok;
	bool ok2 = false;

	line = &::g->lines[linenum];
	//GK: Bug fix for Doom 2 Map 15 Just in case the sector is inaccessible check if the player passes through any of it's lines (if they are special)
	if (thing->player) {
		bool hs = false;
		switch (::g->gamemission){
		case pack_custom:
			if (::g->map) {
				ok2 = ::g->maps[::g->map - 1].cspecls;
			}
			if (ok2) {
				switch (line->frontsector->special) {
				case 9:
					hs = true;
					break;
				}
				switch (line->backsector->special) {
				case 9:
					hs = true;
					break;
				}
			}
				break;
		case doom2:
			switch (::g->gamemap) {
			case 15:
				switch (line->frontsector->special) {
				case 9:
					hs = true;
					break;
				}
				break;
			case 27: //GK: Similar to MAP15 on Doom II but this time is the back sector
				switch (line->backsector->special) {
				case 9:
					hs = true;
					break;
				}
				break;
			}
			break;
		}
		if (hs) {
			thing->player->secretcount++;
			//GK send message when secret found
			S_StartSound(thing->player->mo, sfx_getpow);
			::g->plyr->message = GOTSECRET; 
			if (line->frontsector->special == 9) {
				line->frontsector->special = 0;
			}
			if (line->backsector->special == 9) {
				line->backsector->special = 0;
			}
		}
	}

	//	Triggers that other things can activate
	if (!thing->player)
	{
		// Things that should NOT trigger specials...
		switch (thing->type)
		{
		case MT_ROCKET:
		case MT_PLASMA:
		case MT_BFG:
		case MT_TROOPSHOT:
		case MT_HEADSHOT:
		case MT_BRUISERSHOT:
			return;
			break;

		default: break;
		}

		ok = 0;
		switch (line->special)
		{
		case 39:	// TELEPORT TRIGGER
		case 97:	// TELEPORT RETRIGGER
		case 125:	// TELEPORT MONSTERONLY TRIGGER
		case 126:	// TELEPORT MONSTERONLY RETRIGGER
		case 4:	// RAISE DOOR
		case 10:	// PLAT DOWN-WAIT-UP-STAY TRIGGER
		case 88:	// PLAT DOWN-WAIT-UP-STAY RETRIGGER
					//jff 3/5/98 add ability of monsters etc. to use teleporters
		case 208:     //silent thing teleporters
		case 207:
		case 243:     //silent line-line teleporter
		case 244:     //jff 3/6/98 make fit within DCK's 256 linedef types
		case 262:     //jff 4/14/98 add monster only
		case 263:     //jff 4/14/98 silent thing,line,line rev types
		case 264:     //jff 4/14/98 plus player/monster silent line
		case 265:     //            reversed types
		case 266:
		case 267:
		case 268:
		case 269:
			ok = 1;
			break;
		}
		if (!ok)
			return;
	}
		//jff 02/04/98 add check here for generalized lindef types
 // generalized types not recognized if old demo
	{
		// pointer to line function is NULL by default, set non-null if
		// line special is walkover generalized linedef type
		int(*linefunc)(line_t *line) = NULL;

		// check each range of generalized linedefs
		if ((unsigned)line->special >= GenFloorBase)
		{
			if (!thing->player)
				if ((line->special & FloorChange) || !(line->special & FloorModel))
					return;     // FloorModel is "Allow Monsters" if FloorChange is 0
			if (!line->tag) //jff 2/27/98 all walk generalized types require tag
				return;
			linefunc = EV_DoGenFloor;
		}
		else if ((unsigned)line->special >= GenCeilingBase)
		{
			if (!thing->player)
				if ((line->special & CeilingChange) || !(line->special & CeilingModel))
					return;     // CeilingModel is "Allow Monsters" if CeilingChange is 0
			if (!line->tag) //jff 2/27/98 all walk generalized types require tag
				return;
			linefunc = EV_DoGenCeiling;
		}
		else if ((unsigned)line->special >= GenDoorBase)
		{
			if (!thing->player)
			{
				if (!(line->special & DoorMonster))
					return;                    // monsters disallowed from this door
				if (line->flags & ML_SECRET) // they can't open secret doors either
					return;
			}
			if (!line->tag) //3/2/98 move outside the monster check
				return;
			linefunc = EV_DoGenDoor;
		}
		else if ((unsigned)line->special >= GenLockedBase)
		{
			if (!thing->player)
				return;                     // monsters disallowed from unlocking doors
			if (((line->special&TriggerType) == WalkOnce) || ((line->special&TriggerType) == WalkMany))
			{ //jff 4/1/98 check for being a walk type before reporting door type
				if (!P_CanUnlockGenDoor(line, thing->player))
					return;
			}
			else
				return;
			linefunc = EV_DoGenLockedDoor;
		}
		else if ((unsigned)line->special >= GenLiftBase)
		{
			if (!thing->player)
				if (!(line->special & LiftMonster))
					return; // monsters disallowed
			if (!line->tag) //jff 2/27/98 all walk generalized types require tag
				return;
			linefunc = EV_DoGenLift;
		}
		else if ((unsigned)line->special >= GenStairsBase)
		{
			if (!thing->player)
				if (!(line->special & StairMonster))
					return; // monsters disallowed
			if (!line->tag) //jff 2/27/98 all walk generalized types require tag
				return;
			linefunc = EV_DoGenStairs;
		}

		if (linefunc) // if it was a valid generalized type
			switch ((line->special & TriggerType) >> TriggerTypeShift)
			{
			case WalkOnce:
				if (linefunc(line))
					line->special = 0;    // clear special if a walk once type
				return;
			case WalkMany:
				linefunc(line);
				return;
			default:                  // if not a walk type, do nothing here
				return;
			}
	}

		


	// Note: could use some const's here.
	switch (line->special)
	{
		// TRIGGERS.
		// All from here to RETRIGGERS.
	case 2:
		// Open Door
		EV_DoDoor(line,opened);
		line->special = 0;
		break;

	case 3:
		// Close Door
		EV_DoDoor(line,closed);
		line->special = 0;
		break;

	case 4:
		// Raise Door
		EV_DoDoor(line,normal);
		line->special = 0;
		break;

	case 5:
		// Raise Floor
		EV_DoFloor(line,raiseFloor);
		line->special = 0;
		break;

	case 6:
		// Fast Ceiling Crush & Raise
		EV_DoCeiling(line,fastCrushAndRaise);
		line->special = 0;
		break;

	case 8:
		// Build Stairs
		EV_BuildStairs(line,build8);
		line->special = 0;
		break;

	case 10:
		// PlatDownWaitUp
		EV_DoPlat(line,downWaitUpStay,0);
		line->special = 0;
		break;

	case 12:
		// Light Turn On - brightest near
		EV_LightTurnOn(line,0);
		line->special = 0;
		break;

	case 13:
		// Light Turn On 255
		EV_LightTurnOn(line,255);
		line->special = 0;
		break;

	case 16:
		// Close Door 30
		EV_DoDoor(line,close30ThenOpen);
		line->special = 0;
		break;

	case 17:
		// Start Light Strobing
		EV_StartLightStrobing(line);
		line->special = 0;
		break;

	case 19:
		// Lower Floor
		EV_DoFloor(line,lowerFloor);
		line->special = 0;
		break;

	case 22:
		// Raise floor to nearest height and change texture
		EV_DoPlat(line,raiseToNearestAndChange,0);
		line->special = 0;
		break;

	case 25:
		// Ceiling Crush and Raise
		EV_DoCeiling(line,crushAndRaise);
		line->special = 0;
		break;

	case 30:
		// Raise floor to shortest texture height
		//  on either side of ::g->lines.
		EV_DoFloor(line,raiseToTexture);
		line->special = 0;
		break;

	case 35:
		// Lights Very Dark
		EV_LightTurnOn(line,35);
		line->special = 0;
		break;

	case 36:
		// Lower Floor (TURBO)
		EV_DoFloor(line,turboLower);
		line->special = 0;
		break;

	case 37:
		// LowerAndChange
		EV_DoFloor(line,lowerAndChange);
		line->special = 0;
		break;

	case 38:
		// Lower Floor To Lowest
		EV_DoFloor( line, lowerFloorToLowest );
		line->special = 0;
		break;

	case 39:
		// TELEPORT!
		if (EV_Teleport( line, side, thing ))
			line->special = 0;
		break;

	case 40:
		// RaiseCeilingLowerFloor
		EV_DoCeiling( line, raiseToHighest );
		EV_DoFloor( line, lowerFloorToLowest );
		line->special = 0;
		break;

	case 44:
		// Ceiling Crush
		EV_DoCeiling( line, lowerAndCrush );
		line->special = 0;
		break;

	case 52:
		// EXIT!
		// DHM - Nerve :: Don't exit level in death match, timelimit and fraglimit only
		if ( !::g->deathmatch && ::g->gameaction != ga_completed ) {
			G_ExitLevel();
		}
		break;

	case 53:
		// Perpetual Platform Raise
		EV_DoPlat(line,perpetualRaise,0);
		line->special = 0;
		break;

	case 54:
		// Platform Stop
		EV_StopPlat(line);
		line->special = 0;
		break;

	case 56:
		// Raise Floor Crush
		EV_DoFloor(line,raiseFloorCrush);
		line->special = 0;
		break;

	case 57:
		// Ceiling Crush Stop
		EV_CeilingCrushStop(line);
		line->special = 0;
		break;

	case 58:
		// Raise Floor 24
		EV_DoFloor(line,raiseFloor24);
		line->special = 0;
		break;

	case 59:
		// Raise Floor 24 And Change
		EV_DoFloor(line,raiseFloor24AndChange);
		line->special = 0;
		break;

	case 104:
		// Turn lights off in sector(tag)
		EV_TurnTagLightsOff(line);
		line->special = 0;
		break;

	case 108:
		// Blazing Door Raise (faster than TURBO!)
		EV_DoDoor (line,blazeRaise);
		line->special = 0;
		break;

	case 109:
		// Blazing Door Open (faster than TURBO!)
		EV_DoDoor (line,blazeOpen);
		line->special = 0;
		break;

	case 100:
		// Build Stairs Turbo 16
		EV_BuildStairs(line,turbo16);
		line->special = 0;
		break;

	case 110:
		// Blazing Door Close (faster than TURBO!)
		EV_DoDoor (line,blazeClose);
		line->special = 0;
		break;

	case 119:
		// Raise floor to nearest surr. floor
		EV_DoFloor(line,raiseFloorToNearest);
		line->special = 0;
		break;

	case 121:
		// Blazing PlatDownWaitUpStay
		EV_DoPlat(line,blazeDWUS,0);
		line->special = 0;
		break;

	case 124:
		// Secret EXIT
		if ( !::g->deathmatch && ::g->gameaction != ga_completed ) {
			G_SecretExitLevel ();
		}
		break;

	case 125:
		// TELEPORT MonsterONLY
		if (!thing->player)
		{
			if (EV_Teleport( line, side, thing ))
				line->special = 0;
		}
		break;

	case 130:
		// Raise Floor Turbo
		EV_DoFloor(line,raiseFloorTurbo);
		line->special = 0;
		break;

	case 141:
		// Silent Ceiling Crush & Raise
		EV_DoCeiling(line,silentCrushAndRaise);
		line->special = 0;
		break;

		// RETRIGGERS.  All from here till end.
	case 72:
		// Ceiling Crush
		EV_DoCeiling( line, lowerAndCrush );
		break;

	case 73:
		// Ceiling Crush and Raise
		EV_DoCeiling(line,crushAndRaise);
		break;

	case 74:
		// Ceiling Crush Stop
		EV_CeilingCrushStop(line);
		break;

	case 75:
		// Close Door
		EV_DoDoor(line,closed);
		break;

	case 76:
		// Close Door 30
		EV_DoDoor(line,close30ThenOpen);
		break;

	case 77:
		// Fast Ceiling Crush & Raise
		EV_DoCeiling(line,fastCrushAndRaise);
		break;

	case 79:
		// Lights Very Dark
		EV_LightTurnOn(line,35);
		break;

	case 80:
		// Light Turn On - brightest near
		EV_LightTurnOn(line,0);
		break;

	case 81:
		// Light Turn On 255
		EV_LightTurnOn(line,255);
		break;

	case 82:
		// Lower Floor To Lowest
		EV_DoFloor( line, lowerFloorToLowest );
		break;

	case 83:
		// Lower Floor
		EV_DoFloor(line,lowerFloor);
		break;

	case 84:
		// LowerAndChange
		EV_DoFloor(line,lowerAndChange);
		break;

	case 86:
		// Open Door
		EV_DoDoor(line,opened);
		break;

	case 87:
		// Perpetual Platform Raise
		EV_DoPlat(line,perpetualRaise,0);
		break;

	case 88:
		// PlatDownWaitUp
		EV_DoPlat(line,downWaitUpStay,0);
		break;

	case 89:
		// Platform Stop
		EV_StopPlat(line);
		break;

	case 90:
		// Raise Door
		EV_DoDoor(line,normal);
		break;

	case 91:
		// Raise Floor
		EV_DoFloor(line,raiseFloor);
		break;

	case 92:
		// Raise Floor 24
		EV_DoFloor(line,raiseFloor24);
		break;

	case 93:
		// Raise Floor 24 And Change
		EV_DoFloor(line,raiseFloor24AndChange);
		break;

	case 94:
		// Raise Floor Crush
		EV_DoFloor(line,raiseFloorCrush);
		break;

	case 95:
		// Raise floor to nearest height
		// and change texture.
		EV_DoPlat(line,raiseToNearestAndChange,0);
		break;

	case 96:
		// Raise floor to shortest texture height
		// on either side of ::g->lines.
		EV_DoFloor(line,raiseToTexture);
		break;

	case 97:
		// TELEPORT!
		EV_Teleport( line, side, thing );
		break;

	case 98:
		// Lower Floor (TURBO)
		EV_DoFloor(line,turboLower);
		break;

	case 105:
		// Blazing Door Raise (faster than TURBO!)
		EV_DoDoor (line,blazeRaise);
		break;

	case 106:
		// Blazing Door Open (faster than TURBO!)
		EV_DoDoor (line,blazeOpen);
		break;

	case 107:
		// Blazing Door Close (faster than TURBO!)
		EV_DoDoor (line,blazeClose);
		break;

	case 120:
		// Blazing PlatDownWaitUpStay.
		EV_DoPlat(line,blazeDWUS,0);
		break;

	case 126:
		// TELEPORT MonsterONLY.
		if (!thing->player)
			EV_Teleport( line, side, thing );
		break;

	case 128:
		// Raise To Nearest Floor
		EV_DoFloor(line,raiseFloorToNearest);
		break;

	case 129:
		// Raise Floor Turbo
		EV_DoFloor(line,raiseFloorTurbo);
		break;
	default:
		switch (line->special)
		{
			// Extended walk once triggers

		case 142:
			// Raise Floor 512
			// 142 W1  EV_DoFloor(raiseFloor512)
			if (EV_DoFloor(line, raiseFloor512))
				line->special = 0;
			break;

		case 143:
			// Raise Floor 24 and change
			// 143 W1  EV_DoPlat(raiseAndChange,24)
			if (EV_DoPlat(line, raiseAndChange, 24))
				line->special = 0;
			break;

		case 144:
			// Raise Floor 32 and change
			// 144 W1  EV_DoPlat(raiseAndChange,32)
			if (EV_DoPlat(line, raiseAndChange, 32))
				line->special = 0;
			break;

		case 145:
			// Lower Ceiling to Floor
			// 145 W1  EV_DoCeiling(lowerToFloor)
			if (EV_DoCeiling(line, lowerToFloor))
				line->special = 0;
			break;

		case 146:
			// Lower Pillar, Raise Donut
			// 146 W1  EV_DoDonut()
			if (EV_DoDonut(line))
				line->special = 0;
			break;

		case 199:
			// Lower ceiling to lowest surrounding ceiling
			// 199 W1 EV_DoCeiling(lowerToLowest)
			if (EV_DoCeiling(line, lowerToLowest))
				line->special = 0;
			break;

		case 200:
			// Lower ceiling to highest surrounding floor
			// 200 W1 EV_DoCeiling(lowerToMaxFloor)
			if (EV_DoCeiling(line, lowerToMaxFloor))
				line->special = 0;
			break;

		case 207:
			// killough 2/16/98: W1 silent teleporter (normal kind)
			if (EV_SilentTeleport(line, side, thing))
				line->special = 0;
			break;

			//jff 3/16/98 renumber 215->153
		case 153: //jff 3/15/98 create texture change no motion type
				  // Texture/Type Change Only (Trig)
				  // 153 W1 Change Texture/Type Only
			if (EV_DoChange(line, trigChangeOnly))
				line->special = 0;
			break;

		case 239: //jff 3/15/98 create texture change no motion type
				  // Texture/Type Change Only (Numeric)
				  // 239 W1 Change Texture/Type Only
			if (EV_DoChange(line, numChangeOnly))
				line->special = 0;
			break;

		case 219:
			// Lower floor to next lower neighbor
			// 219 W1 Lower Floor Next Lower Neighbor
			if (EV_DoFloor(line, lowerFloorToNearest))
				line->special = 0;
			break;

		case 227:
			// Raise elevator next floor
			// 227 W1 Raise Elevator next floor
			if (EV_DoElevator(line, elevateUp))
				line->special = 0;
			break;

		case 231:
			// Lower elevator next floor
			// 231 W1 Lower Elevator next floor
			if (EV_DoElevator(line, elevateDown))
				line->special = 0;
			break;

		case 235:
			// Elevator to current floor
			// 235 W1 Elevator to current floor
			if (EV_DoElevator(line, elevateCurrent))
				line->special = 0;
			break;

		case 243: //jff 3/6/98 make fit within DCK's 256 linedef types
				  // killough 2/16/98: W1 silent teleporter (linedef-linedef kind)
			if (EV_SilentLineTeleport(line, side, thing, false))
				line->special = 0;
			break;

		case 262: //jff 4/14/98 add silent line-line reversed
			if (EV_SilentLineTeleport(line, side, thing, true))
				line->special = 0;
			break;

		case 264: //jff 4/14/98 add monster-only silent line-line reversed
			if (!thing->player &&
				EV_SilentLineTeleport(line, side, thing, true))
				line->special = 0;
			break;

		case 266: //jff 4/14/98 add monster-only silent line-line
			if (!thing->player &&
				EV_SilentLineTeleport(line, side, thing, false))
				line->special = 0;
			break;

		case 268: //jff 4/14/98 add monster-only silent
			if (!thing->player && EV_SilentTeleport(line, side, thing))
				line->special = 0;
			break;

			//jff 1/29/98 end of added W1 linedef types

			// Extended walk many retriggerable

			//jff 1/29/98 added new linedef types to fill all functions
			//out so that all have varieties SR, S1, WR, W1

		case 147:
			// Raise Floor 512
			// 147 WR  EV_DoFloor(raiseFloor512)
			EV_DoFloor(line, raiseFloor512);
			break;

		case 148:
			// Raise Floor 24 and Change
			// 148 WR  EV_DoPlat(raiseAndChange,24)
			EV_DoPlat(line, raiseAndChange, 24);
			break;

		case 149:
			// Raise Floor 32 and Change
			// 149 WR  EV_DoPlat(raiseAndChange,32)
			EV_DoPlat(line, raiseAndChange, 32);
			break;

		case 150:
			// Start slow silent crusher
			// 150 WR  EV_DoCeiling(silentCrushAndRaise)
			EV_DoCeiling(line, silentCrushAndRaise);
			break;

		case 151:
			// RaiseCeilingLowerFloor
			// 151 WR  EV_DoCeiling(raiseToHighest),
			//         EV_DoFloor(lowerFloortoLowest)
			EV_DoCeiling(line, raiseToHighest);
			EV_DoFloor(line, lowerFloorToLowest);
			break;

		case 152:
			// Lower Ceiling to Floor
			// 152 WR  EV_DoCeiling(lowerToFloor)
			EV_DoCeiling(line, lowerToFloor);
			break;

			//jff 3/16/98 renumber 153->256
		case 256:
			// Build stairs, step 8
			// 256 WR EV_BuildStairs(build8)
			EV_BuildStairs(line, build8);
			break;

			//jff 3/16/98 renumber 154->257
		case 257:
			// Build stairs, step 16
			// 257 WR EV_BuildStairs(turbo16)
			EV_BuildStairs(line, turbo16);
			break;

		case 155:
			// Lower Pillar, Raise Donut
			// 155 WR  EV_DoDonut()
			EV_DoDonut(line);
			break;

		case 156:
			// Start lights strobing
			// 156 WR Lights EV_StartLightStrobing()
			EV_StartLightStrobing(line);
			break;

		case 157:
			// Lights to dimmest near
			// 157 WR Lights EV_TurnTagLightsOff()
			EV_TurnTagLightsOff(line);
			break;

		case 201:
			// Lower ceiling to lowest surrounding ceiling
			// 201 WR EV_DoCeiling(lowerToLowest)
			EV_DoCeiling(line, lowerToLowest);
			break;

		case 202:
			// Lower ceiling to highest surrounding floor
			// 202 WR EV_DoCeiling(lowerToMaxFloor)
			EV_DoCeiling(line, lowerToMaxFloor);
			break;

		case 208:
			// killough 2/16/98: WR silent teleporter (normal kind)
			EV_SilentTeleport(line, side, thing);
			break;

		case 212: //jff 3/14/98 create instant toggle floor type
				  // Toggle floor between C and F instantly
				  // 212 WR Instant Toggle Floor
			EV_DoPlat(line, toggleUpDn, 0);
			break;

			//jff 3/16/98 renumber 216->154
		case 154: //jff 3/15/98 create texture change no motion type
				  // Texture/Type Change Only (Trigger)
				  // 154 WR Change Texture/Type Only
			EV_DoChange(line, trigChangeOnly);
			break;

		case 240: //jff 3/15/98 create texture change no motion type
				  // Texture/Type Change Only (Numeric)
				  // 240 WR Change Texture/Type Only
			EV_DoChange(line, numChangeOnly);
			break;

		case 220:
			// Lower floor to next lower neighbor
			// 220 WR Lower Floor Next Lower Neighbor
			EV_DoFloor(line, lowerFloorToNearest);
			break;

		case 228:
			// Raise elevator next floor
			// 228 WR Raise Elevator next floor
			EV_DoElevator(line, elevateUp);
			break;

		case 232:
			// Lower elevator next floor
			// 232 WR Lower Elevator next floor
			EV_DoElevator(line, elevateDown);
			break;

		case 236:
			// Elevator to current floor
			// 236 WR Elevator to current floor
			EV_DoElevator(line, elevateCurrent);
			break;

		case 244: //jff 3/6/98 make fit within DCK's 256 linedef types
				  // killough 2/16/98: WR silent teleporter (linedef-linedef kind)
			EV_SilentLineTeleport(line, side, thing, false);
			break;

		case 263: //jff 4/14/98 add silent line-line reversed
			EV_SilentLineTeleport(line, side, thing, true);
			break;

		case 265: //jff 4/14/98 add monster-only silent line-line reversed
			if (!thing->player)
				EV_SilentLineTeleport(line, side, thing, true);
			break;

		case 267: //jff 4/14/98 add monster-only silent line-line
			if (!thing->player)
				EV_SilentLineTeleport(line, side, thing, false);
			break;

		case 269: //jff 4/14/98 add monster-only silent
			if (!thing->player)
				EV_SilentTeleport(line, side, thing);
			break;

			//jff 1/29/98 end of added WR linedef types
		}
		break;
	}
}



//
// P_ShootSpecialLine - IMPACT SPECIALS
// Called when a thing shoots a special line.
//
void
P_ShootSpecialLine
( mobj_t*	thing,
 line_t*	line )
{
	int		ok;
	//jff 02/04/98 add check here for generalized linedef
	{
		// pointer to line function is NULL by default, set non-null if
		// line special is gun triggered generalized linedef type
		int(*linefunc)(line_t *line) = NULL;

		// check each range of generalized linedefs
		if ((unsigned)line->special >= GenFloorBase)
		{
			if (!thing->player)
				if ((line->special & FloorChange) || !(line->special & FloorModel))
					return;   // FloorModel is "Allow Monsters" if FloorChange is 0
			if (!line->tag) //jff 2/27/98 all gun generalized types require tag
				return;

			linefunc = EV_DoGenFloor;
		}
		else if ((unsigned)line->special >= GenCeilingBase)
		{
			if (!thing->player)
				if ((line->special & CeilingChange) || !(line->special & CeilingModel))
					return;   // CeilingModel is "Allow Monsters" if CeilingChange is 0
			if (!line->tag) //jff 2/27/98 all gun generalized types require tag
				return;
			linefunc = EV_DoGenCeiling;
		}
		else if ((unsigned)line->special >= GenDoorBase)
		{
			if (!thing->player)
			{
				if (!(line->special & DoorMonster))
					return;   // monsters disallowed from this door
				if (line->flags & ML_SECRET) // they can't open secret doors either
					return;
			}
			if (!line->tag) //jff 3/2/98 all gun generalized types require tag
				return;
			linefunc = EV_DoGenDoor;
		}
		else if ((unsigned)line->special >= GenLockedBase)
		{
			if (!thing->player)
				return;   // monsters disallowed from unlocking doors
			if (((line->special&TriggerType) == GunOnce) || ((line->special&TriggerType) == GunMany))
			{ //jff 4/1/98 check for being a gun type before reporting door type
				if (!P_CanUnlockGenDoor(line, thing->player))
					return;
			}
			else
				return;
			if (!line->tag) //jff 2/27/98 all gun generalized types require tag
				return;

			linefunc = EV_DoGenLockedDoor;
		}
		else if ((unsigned)line->special >= GenLiftBase)
		{
			if (!thing->player)
				if (!(line->special & LiftMonster))
					return; // monsters disallowed
			linefunc = EV_DoGenLift;
		}
		else if ((unsigned)line->special >= GenStairsBase)
		{
			if (!thing->player)
				if (!(line->special & StairMonster))
					return; // monsters disallowed
			if (!line->tag) //jff 2/27/98 all gun generalized types require tag
				return;
			linefunc = EV_DoGenStairs;
		}
		else if ((unsigned)line->special >= GenCrusherBase)
		{
			if (!thing->player)
				if (!(line->special & StairMonster))
					return; // monsters disallowed
			if (!line->tag) //jff 2/27/98 all gun generalized types require tag
				return;
			linefunc = EV_DoGenCrusher;
		}

		if (linefunc)
			switch ((line->special & TriggerType) >> TriggerTypeShift)
			{
			case GunOnce:
				if (linefunc(line))
					P_ChangeSwitchTexture(line, 0);
				return;
			case GunMany:
				if (linefunc(line))
					P_ChangeSwitchTexture(line, 1);
				return;
			default:  // if not a gun type, do nothing here
				return;
			}
	}
	//	Impacts that other things can activate.
	if (!thing->player)
	{
		ok = 0;
		switch(line->special)
		{
		case 46:
			// OPEN DOOR IMPACT
			ok = 1;
			break;
		}
		if (!ok)
			return;
	}

	switch(line->special)
	{
	case 24:
		// RAISE FLOOR
		EV_DoFloor(line,raiseFloor);
		P_ChangeSwitchTexture(line,0);
		break;

	case 46:
		// OPEN DOOR
		EV_DoDoor(line,opened);
		P_ChangeSwitchTexture(line,1);
		break;

	case 47:
		// RAISE FLOOR NEAR AND CHANGE
		EV_DoPlat(line,raiseToNearestAndChange,0);
		P_ChangeSwitchTexture(line,0);
		break;
	}
}


void P_GiveSecret(player_t* player) {
	player->secretcount++;
	//GK send message when secret found
	::g->plyr->message = GOTSECRET;
	S_StartSound(player->mo, sfx_getpow);


	if (!::g->demoplayback && (::g->usergame && !::g->netgame)) {
		// DHM - Nerve :: Let's give achievements in real time in Doom 2
		if (!common->IsMultiplayer()) {
			if (idAchievementManager::isClassicDoomOnly()) {
				idAchievementManager::LocalUser_CompleteAchievement(CLASSIC_ACHIEVEMENT_SECRET_AREA);
			}
			else {
				switch (DoomLib::GetGameSKU()) {
				case GAME_SKU_DOOM1_BFG: {
					// Removing trophies for DOOM and DOOM II BFG due to point limit.
					//gameLocal->UnlockAchievement( Doom1BFG_Trophies::SCOUT_FIND_ANY_SECRET );
					break;
				}
				case GAME_SKU_DOOM2_BFG: {
#ifdef __MONOLITH__
					//gameLocal->UnlockAchievement( Doom2BFG_Trophies::IMPORTANT_LOOKING_DOOR_FIND_ANY_SECRET );
					idAchievementManager::LocalUser_CompleteAchievement(ACHIEVEMENT_DOOM2_IMPORTANT_LOOKING_DOOR_FIND_ANY_SECRET);
#endif
					break;
				}
				case GAME_SKU_DCC: {
					// Not on PC.
					//gameLocal->UnlockAchievement( DOOM_ACHIEVEMENT_FIND_SECRET );
					break;
				}
				default: {
					// No unlocks for other SKUs.
					break;
				}
				}
			}
		}
	}
}


//
// P_PlayerInSpecialSector
// Called every tic frame
//  that the player origin is in a special sector
//
void P_PlayerInSpecialSector (player_t* player)
{
	sector_t*	sector;

	sector = player->mo->subsector->sector;

	// Falling, not all the way down yet?
	if (player->mo->z != sector->floorheight)
		return;	

	// Has hitten ground.
	if (sector->special < 32) {
		switch (sector->special)
		{
		case 5:
			// HELLSLIME DAMAGE
			if (!player->powers[pw_ironfeet])
				if (!(::g->leveltime&0x1f))
					P_DamageMobj (player->mo, NULL, NULL, 10);
			break;

		case 7:
			// NUKAGE DAMAGE
			if (!player->powers[pw_ironfeet])
				if (!(::g->leveltime&0x1f))
					P_DamageMobj (player->mo, NULL, NULL, 5);
			break;

		case 16:
			// SUPER HELLSLIME DAMAGE
		case 4:
			// STROBE HURT
			if (!player->powers[pw_ironfeet]
			|| (P_Random()<5) )
			{
				if (!(::g->leveltime&0x1f))
					P_DamageMobj (player->mo, NULL, NULL, 20);
			}
			break;

		case 9:
			// SECRET SECTOR
			sector->special = 0;
			P_GiveSecret(player);
			break;

		case 11:
			// EXIT SUPER DAMAGE! (for E1M8 finale)
			player->cheats &= ~CF_GODMODE;

			if (!(::g->leveltime&0x1f))
				P_DamageMobj (player->mo, NULL, NULL, 20);

			if (player->health <= 10)
				G_ExitLevel();
			break;
		default:
			I_Error ("P_PlayerInSpecialSector: "
				"unknown special %i",
				sector->special);
			break;
		};
	} else if(sector->special & DEATH_MASK) {
		switch(sector->special & DAMAGE_MASK >> DAMAGE_SHIFT) {
			case 0: {
				if (!player->powers[pw_invulnerability] && !player->powers[pw_ironfeet])
					P_DamageMobj(player->mo, NULL, NULL, 10000);
				break;
			}
			case 1:
				P_DamageMobj(player->mo, NULL, NULL, 10000);
				break;
			case 2:
				for (int i = 0; i < ::g->doomcom.numnodes; i++) {
					if (::g->playeringame[i]) {
						P_DamageMobj(::g->players[i].mo, NULL, NULL, 10000);
					}
				}
				G_ExitLevel();
				break;
			case 3: 
				for (int i = 0; i < ::g->doomcom.numnodes; i++) {
					if (::g->playeringame[i]) {
						P_DamageMobj(::g->players[i].mo, NULL, NULL, 10000);
					}
				}
				G_SecretExitLevel();
				break;
		}
	}
	else {
		switch ((sector->special & DAMAGE_MASK) >> DAMAGE_SHIFT)
		{
		case 0: // no damage
			break;
		case 1: // 2/5 damage per 31 ticks
			if (!player->powers[pw_ironfeet])
				if (!(::g->leveltime & 0x1f))
					P_DamageMobj(player->mo, NULL, NULL, 5);
			break;
		case 2: // 5/10 damage per 31 ticks
			if (!player->powers[pw_ironfeet])
				if (!(::g->leveltime & 0x1f))
					P_DamageMobj(player->mo, NULL, NULL, 10);
			break;
		case 3: // 10/20 damage per 31 ticks
			if (!player->powers[pw_ironfeet]
				|| (P_Random() < 5))  // take damage even with suit
			{
				if (!(::g->leveltime & 0x1f))
					P_DamageMobj(player->mo, NULL, NULL, 20);
			}
			break;
		}

		if (sector->special & SECRET_MASK)
		{
			sector->special &= ~SECRET_MASK;
			if (sector->special < 32) // if all extended bits clear,
				sector->special = 0;    // sector is not special anymore
			P_GiveSecret(player);
		}

		// phares 3/19/98:
		//
		// If FRICTION_MASK or PUSH_MASK is set, we don't care at this
		// point, since the code to deal with those situations is
		// handled by Thinkers.
	}
}




//
// P_UpdateSpecials
// Animate planes, scroll walls, etc.
//
int PlayerFrags( int playernum ) {
	int	frags = 0;

	for( int i=0 ; i<MAXPLAYERS ; i++) {
		if ( i != playernum ) {
			frags += ::g->players[playernum].frags[i];
		}
	}

	frags -= ::g->players[playernum].frags[playernum];

	return frags;
}

void P_UpdateSpecials (void)
{
	int		pic;
	int		i;


	//	LEVEL TIMER
	if (::g->levelTimer == true)
	{
		::g->levelTimeCount--;
		if (!::g->levelTimeCount)
			G_ExitLevel();
	}

	// DHM - Nerve :: FRAG COUNT
	if ( ::g->deathmatch && ::g->levelFragCount > 0 ) {
		bool fragCountHit = false;

		for ( int i_=0; i_<MAXPLAYERS; i_++ ) {
			if ( ::g->playeringame[i_] ) {
				if ( PlayerFrags(i_) >= ::g->levelFragCount ) {
					fragCountHit = true;
				}
			}
		}

		if ( fragCountHit ) {
			G_ExitLevel();
		}
	}

	//	ANIMATE FLATS AND TEXTURES GLOBALLY
	for (anim_t2 &anim : ::g->anims)
	{
		for (i=anim.basepic ; i<anim.basepic+anim.numpics ; i++)
		{
			pic = anim.basepic + ( (::g->leveltime/anim.speed + i)%anim.numpics );
			if (anim.istexture)
				::g->texturetranslation[i] = pic;
			else
				::g->flattranslation[i] = pic;
		}
	}


	//	ANIMATE LINE SPECIALS
	size_t ai;
	for (ai = 0; ai < ::g->numlinespecials; ai++)
	{
	//	line = ::g->linespeciallist[i];
		switch(::g->linespeciallist[ai].special)
		{
		case 48:
			// EFFECT FIRSTCOL SCROLL +
			::g->sides[::g->linespeciallist[ai].sidenum[0]].textureoffset += FRACUNIT;
			break;
		}
	}


	//	DO BUTTONS
	for (i = 0; i < MAXBUTTONS; i++)
		if (::g->buttonlist[i].btimer)
		{
			::g->buttonlist[i].btimer--;
			if (!::g->buttonlist[i].btimer)
			{
				switch(::g->buttonlist[i].where)
				{
				case top:
					::g->sides[::g->buttonlist[i].line->sidenum[0]].toptexture =
						::g->buttonlist[i].btexture;
					break;

				case middle:
					::g->sides[::g->buttonlist[i].line->sidenum[0]].midtexture =
						::g->buttonlist[i].btexture;
					break;

				case bottom:
					::g->sides[::g->buttonlist[i].line->sidenum[0]].bottomtexture =
						::g->buttonlist[i].btexture;
					break;
				}
				S_StartSound((mobj_t *)&::g->buttonlist[i].soundorg,sfx_swtchn);
				memset(&::g->buttonlist[i],0,sizeof(button_t));
			}
		}

}



//
// Special Stuff that can not be categorized
//
int EV_DoDonut(line_t*	line)
{
	sector_t*		s1;
	sector_t*		s2;
	sector_t*		s3;
	int			secnum;
	int			rtn;
	int			i;
	floormove_t*	floor;

	secnum = -1;
	rtn = 0;
	while ((secnum = P_FindSectorFromLineTag(line,secnum)) >= 0)
	{
		s1 = &::g->sectors[secnum];

		// ALREADY MOVING?  IF SO, KEEP GOING...
		if (P_SectorActive(floor_special, s1))
			continue;

		rtn = 1;
		s2 = getNextSector(s1->lines[0],s1);
		for (i = 0;i < s2->linecount;i++)
		{
			if ((!(s2->lines[i]->flags & ML_TWOSIDED)) ||
				(s2->lines[i]->backsector == s1))
				continue;
			s3 = s2->lines[i]->backsector;

			//	Spawn rising slime
			floor = (floormove_t*)DoomLib::Z_Malloc (sizeof(*floor), PU_FLOOR, 0);
			P_AddThinker (&floor->thinker);
			s2->floordata = floor;
			floor->thinker.function = (actionf_p1) T_MoveFloor;
			floor->type = donutRaise;
			floor->crush = false;
			floor->direction = 1;
			floor->sector = s2;
			floor->speed = FLOORSPEED / 2;
			floor->texture = s3->floorpic;
			floor->newspecial = 0;
			floor->floordestheight = s3->floorheight;

			//	Spawn lowering donut-hole
			floor = (floormove_t*)DoomLib::Z_Malloc (sizeof(*floor), PU_FLOOR, 0);
			P_AddThinker (&floor->thinker);
			s1->floordata = floor;
			floor->thinker.function = (actionf_p1) T_MoveFloor;
			floor->type = lowerFloor;
			floor->crush = false;
			floor->direction = -1;
			floor->sector = s1;
			floor->speed = FLOORSPEED / 2;
			floor->floordestheight = s3->floorheight;
			break;
		}
	}
	return rtn;
}



//
// SPECIAL SPAWNING
//

//
// P_SpawnSpecials
// After the map has been loaded, scan for specials
//  that spawn thinkers
//


// Parses command line parameters.
void P_SpawnSpecials (void)
{
	sector_t*	sector;
	int		i;
	int		episode;

	episode = 1;
	if (W_CheckNumForName("texture2") >= 0)
		episode = 2;


	// See if -TIMER needs to be used.
	::g->levelTimer = false;

	i = M_CheckParm("-avg");
	if (i && ::g->deathmatch)
	{
		::g->levelTimer = true;
		::g->levelTimeCount = 20 * 60 * TICRATE;
	}
	//GK:Revive network related stuff
	i = M_CheckParm("-timer");
	if (i && ::g->deathmatch) {
		::g->dmtime = atoi(::g->myargv[i + 1]) * 60 * TICRATE;
		::g->levelTimer = true;
		::g->levelTimeCount = ::g->dmtime;
	}
	else {
#ifdef ID_ENABLE_DOOM_CLASSIC_NETWORKING
		const int timeLimit = 60;
#else
		const int timeLimit = 0;
#endif
		if (timeLimit != 0 && g->deathmatch)
		{
			::g->dmtime = timeLimit * 60 * TICRATE;
			::g->levelTimer = true;
			::g->levelTimeCount = ::g->dmtime;
		}
	}
	i = M_CheckParm("-fraglimit");
	if (i && ::g->deathmatch) {
		::g->levelFragCount = atoi(::g->myargv[i + 1]);
	}
	else {
#ifdef ID_ENABLE_DOOM_CLASSIC_NETWORKING
		const int fragLimit = 10;
#else
		const int fragLimit = 0;
#endif
		if (fragLimit != 0 && ::g->deathmatch)
		{
			//::g->levelFragCount = atoi(::g->myargv[i+1]);
			::g->levelFragCount = fragLimit;
		}
		else {
			::g->levelFragCount = 0;
		}
	}
	//	Init special SECTORs.
	sector = ::g->sectors;
	for (i=0 ; i < ::g->numsectors ; i++, sector++)
	{
		if (!sector->special)
			continue;

		switch (sector->special)
		{
		case 1:
			// FLICKERING LIGHTS
			P_SpawnLightFlash (sector);
			break;

		case 2:
			// STROBE FAST
			P_SpawnStrobeFlash(sector,FASTDARK,0);
			break;

		case 3:
			// STROBE SLOW
			P_SpawnStrobeFlash(sector,SLOWDARK,0);
			break;

		case 4:
			// STROBE FAST/DEATH SLIME
			P_SpawnStrobeFlash(sector,FASTDARK,0);
			sector->special = 4;
			break;

		case 8:
			// GLOWING LIGHT
			P_SpawnGlowingLight(sector);
			break;
		case 9:
			// SECRET SECTOR
			::g->totalsecret++;
			break;

		case 10:
			// DOOR CLOSE IN 30 SECONDS
			P_SpawnDoorCloseIn30 (sector);
			break;

		case 12:
			// SYNC STROBE SLOW
			P_SpawnStrobeFlash (sector, SLOWDARK, 1);
			break;

		case 13:
			// SYNC STROBE FAST
			P_SpawnStrobeFlash (sector, FASTDARK, 1);
			break;

		case 14:
			// DOOR RAISE IN 5 MINUTES
			P_SpawnDoorRaiseIn5Mins (sector, i);
			break;

		case 17:
			P_SpawnFireFlicker(sector);
			break;
		}
	}


	//	Init line EFFECTs
	::g->numlinespecials = 0;
	for (i = 0;i < ::g->numlines; i++)
	{
		switch(::g->lines[i].special)
		{
		case 48:
			// EFFECT FIRSTCOL SCROLL+
			if (::g->numlinespecials >= ::g->linespeciallist.size()) {
				if (::g->linespeciallist.size() == ::g->linespeciallist.capacity()) {
					::g->linespeciallist.resize(::g->linespeciallist.size() + MAXLINEANIMS);
				}
				::g->linespeciallist[::g->numlinespecials] = ::g->lines[i];
			}
			else {
				::g->linespeciallist[::g->numlinespecials] = ::g->lines[i];
			}
			::g->numlinespecials++;
			break;
		}
	}

	P_InitTagLists();
	P_SpawnScrollers(); // killough 3/7/98: Add generalized scrollers
	P_SpawnFriction();
	P_SpawnPushers();

	for (i = 0; i< ::g->numlines; i++)
		switch (::g->lines[i].special)
		{
			int s, sec;

			// killough 3/7/98:
			// support for drawn heights coming from different sector
		case 242:
			sec = ::g->sides[*::g->lines[i].sidenum].sector - ::g->sectors;
			for (s = -1; (s = P_FindSectorFromLineTag(::g->lines + i, s)) >= 0;)
				::g->sectors[s].heightsec = sec;
			break;

			// killough 3/16/98: Add support for setting
			// floor lighting independently (e.g. lava)
		case 213:
			sec = ::g->sides[*::g->lines[i].sidenum].sector - ::g->sectors;
			for (s = -1; (s = P_FindSectorFromLineTag(::g->lines + i, s)) >= 0;)
				::g->sectors[s].floorlightsec = sec;
			break;

			// killough 4/11/98: Add support for setting
			// ceiling lighting independently
		case 261:
			sec = ::g->sides[*::g->lines[i].sidenum].sector - ::g->sectors;
			for (s = -1; (s = P_FindSectorFromLineTag(::g->lines + i, s)) >= 0;)
				::g->sectors[s].ceilinglightsec = sec;
			break;
			// killough 10/98:
			//
			// Support for sky textures being transferred from sidedefs.
			// Allows scrolling and other effects (but if scrolling is
			// used, then the same sector tag needs to be used for the
			// sky sector, the sky-transfer linedef, and the scroll-effect
			// linedef). Still requires user to use F_SKY1 for the floor
			// or ceiling texture, to distinguish floor and ceiling sky.

		case 271:   // Regular sky
		case 272:   // Same, only flipped
			for (s = -1; (s = P_FindSectorFromLineTag(::g->lines+i,s)) >= 0;)
			::g->sectors[s].sky = i | PL_SKYFLAT;
			break;
		}
	//	Init other misc stuff
	::g->cellind = 0;
	if (::g->cellind >= ::g->activeceilings.size()) {
#if _ITERATOR_DEBUG_LEVEL < 2
		if (::g->activeceilings.size() == ::g->activeceilings.capacity()) {
			::g->activeceilings.reserve(::g->activeceilings.size() + MAXCEILINGS);
		}
		::g->activeceilings.emplace_back((ceiling_t*)DoomLib::Z_Malloc(sizeof(ceiling_t), PU_CEILING, 0));
#else
		if (::g->activeceilings.size() == ::g->activeceilings.capacity()) {
			::g->activeceilings.resize(::g->activeceilings.size() + MAXCEILINGS);
		}
		for (int ci = ::g->cellind; ci < ::g->activeceilings.size(); ci++) {
			::g->activeceilings[ci] = (ceiling_t*)DoomLib::Z_Malloc(sizeof(ceiling_t), PU_CEILING, 0);
		}
#endif
	}
	::g->cellind++;
	::g->platind = 0;
	if (::g->platind >= ::g->activeplats.size()) {
#if _ITERATOR_DEBUG_LEVEL < 2
		if (::g->activeplats.size() == ::g->activeplats.capacity()) {
			::g->activeplats.reserve(::g->activeplats.size() + MAXPLATS);
		}
		::g->activeplats.emplace_back((plat_t*)DoomLib::Z_Malloc(sizeof(plat_t), PU_PLATS, 0));
#else
		if (::g->activeplats.size() == ::g->activeplats.capacity()) {
			::g->activeplats.resize(::g->activeplats.size() + MAXPLATS);
		}
		for (int pi = ::g->platind; pi < ::g->activeplats.size(); pi++) {
			::g->activeplats[pi] = (plat_t*)DoomLib::Z_Malloc(sizeof(plat_t), PU_PLATS, 0);
		}
#endif
	}
	::g->platind++;

	for (i = 0;i < MAXBUTTONS;i++)
		memset(&::g->buttonlist[i],0,sizeof(button_t));

	// UNUSED: no horizonal sliders.
	//	P_InitSlidingDoorFrames();
}

// killough 2/28/98:
//
// This function, with the help of r_plane.c and r_bsp.c, supports generalized
// scrolling floors and walls, with optional mobj-carrying properties, e.g.
// conveyor belts, rivers, etc. A linedef with a special type affects all
// tagged sectors the same way, by creating scrolling and/or object-carrying
// properties. Multiple linedefs may be used on the same sector and are
// cumulative, although the special case of scrolling a floor and carrying
// things on it, requires only one linedef. The linedef's direction determines
// the scrolling direction, and the linedef's length determines the scrolling
// speed. This was designed so that an edge around the sector could be used to
// control the direction of the sector's scrolling, which is usually what is
// desired.
//
// Process the active scrollers.
//
// This is the main scrolling code
// killough 3/7/98

void T_Scroll(scroll_t *s)
{
	fixed_t dx = s->dx, dy = s->dy;

	if (s->control != -1)
	{   // compute scroll amounts based on a sector's height changes
		fixed_t height = ::g->sectors[s->control].floorheight +
			::g->sectors[s->control].ceilingheight;
		fixed_t delta = height - s->last_height;
		s->last_height = height;
		dx = FixedMul(dx, delta);
		dy = FixedMul(dy, delta);
	}

	// killough 3/14/98: Add acceleration
	if (s->accel)
	{
		s->vdx = dx += s->vdx;
		s->vdy = dy += s->vdy;
	}

	if (!(dx | dy))                   // no-op if both (x,y) offsets 0
		return;

	switch (s->type)
	{
		side_t *side;
		sector_t *sec;
		fixed_t height, waterheight;  // killough 4/4/98: add waterheight
		mobj_t *thing;

	case sc_side:                   // killough 3/7/98: Scroll wall texture
		side = ::g->sides + s->affectee;
		side->textureoffset += dx;
		side->rowoffset += dy;
		break;

	case sc_floor:                  // killough 3/7/98: Scroll floor texture
		sec = ::g->sectors + s->affectee;
		sec->floor_xoffs += dx;
		sec->floor_yoffs += dy;
		break;

	case sc_ceiling:               // killough 3/7/98: Scroll ceiling texture
		sec = ::g->sectors + s->affectee;
		sec->ceiling_xoffs += dx;
		sec->ceiling_yoffs += dy;
		break;

	case sc_carry:

		// killough 3/7/98: Carry things on floor
		// killough 3/20/98: use new sector list which reflects true members
		// killough 3/27/98: fix carrier bug
		// killough 4/4/98: Underwater, carry things even w/o gravity

		sec = ::g->sectors + s->affectee;
		height = sec->floorheight;
		waterheight = sec->heightsec != -1 &&
			::g->sectors[sec->heightsec].floorheight > height ?
			::g->sectors[sec->heightsec].floorheight : MININT;

		for (size_t i = 0; i < ::g->sector_list.size(); i++) {
			std::shared_ptr<msecnode_t> node = ::g->sector_list[i];
			if (node->m_sector == sec) {
				if (!((thing = node->m_thing)->flags & MF_NOCLIP) &&
					(!(thing->z > height) || thing->z < waterheight))
				{
					// Move objects only if on floor or underwater,
					// non-floating, and clipped.
					thing->momx += dx;
					thing->momy += dy;
				}
			}
		}
		break;

	case sc_carry_ceiling:       // to be added later
		break;
	}
}

//
// Add_Scroller()
//
// Add a generalized scroller to the thinker list.
//
// type: the enumerated type of scrolling: floor, ceiling, floor carrier,
//   wall, floor carrier & scroller
//
// (dx,dy): the direction and speed of the scrolling or its acceleration
//
// control: the sector whose heights control this scroller's effect
//   remotely, or -1 if no control sector
//
// affectee: the index of the affected object (sector or sidedef)
//
// accel: non-zero if this is an accelerative effect
//

static void Add_Scroller(int type, fixed_t dx, fixed_t dy,
	int control, int affectee, int accel)
{
	scroll_t *s =(scroll_t*) DoomLib::Z_Malloc(sizeof *s, PU_LEVSPEC, 0);
	s->thinker.function = (actionf_p1)T_Scroll;
	s->type =static_cast<scrollers>( type);
	s->dx = dx;
	s->dy = dy;
	s->accel = accel;
	s->vdx = s->vdy = 0;
	if ((s->control = control) != -1)
		s->last_height =
		::g->sectors[control].floorheight + ::g->sectors[control].ceilingheight;
	s->affectee = affectee;
	P_AddThinker(&s->thinker);
}

// Adds wall scroller. Scroll amount is rotated with respect to wall's
// linedef first, so that scrolling towards the wall in a perpendicular
// direction is translated into vertical motion, while scrolling along
// the wall in a parallel direction is translated into horizontal motion.
//
// killough 5/25/98: cleaned up arithmetic to avoid drift due to roundoff

static void Add_WallScroller(fixed_t dx, fixed_t dy, const line_t *l,
	int control, int accel)
{
	fixed_t x = abs(l->dx), y = abs(l->dy), d;
	if (y > x)
		d = x, x = y, y = d;
	d = FixedDiv(x, finesine[(tantoangle[FixedDiv(y, x) >> DBITS] + ANG90)
		>> ANGLETOFINESHIFT]);
	x = -FixedDiv(FixedMul(dy, l->dy) + FixedMul(dx, l->dx), d);
	y = -FixedDiv(FixedMul(dx, l->dy) - FixedMul(dy, l->dx), d);
	Add_Scroller(sc_side, x, y, control, *l->sidenum, accel);
}

// Amount (dx,dy) vector linedef is shifted right to get scroll amount
#define SCROLL_SHIFT 5

// Factor to scale scrolling effect into mobj-carrying properties = 3/32.
// (This is so scrolling floors and objects on them can move at same speed.)
//GK: reduced it because for some reason with it's orignal value it was moving the object so fast that it tears apart the space time continium
#define CARRYFACTOR ((fixed_t)(FRACUNIT*.09375))

// Initialize the scrollers
static void P_SpawnScrollers(void)
{
	int i;
	line_t *l = ::g->lines;

	for (i = 0; i< ::g->numlines; i++, l++)
	{
		fixed_t dx = l->dx >> SCROLL_SHIFT;  // direction and speed of scrolling
		fixed_t dy = l->dy >> SCROLL_SHIFT;
		int control = -1, accel = 0;         // no control sector or acceleration
		int special = l->special;

		// killough 3/7/98: Types 245-249 are same as 250-254 except that the
		// first side's sector's heights cause scrolling when they change, and
		// this linedef controls the direction and speed of the scrolling. The
		// most complicated linedef since donuts, but powerful :)
		//
		// killough 3/15/98: Add acceleration. Types 214-218 are the same but
		// are accelerative.

		if (special >= 245 && special <= 249)         // displacement scrollers
		{
			special += 250 - 245;
			control = ::g->sides[*l->sidenum].sector - ::g->sectors;
		}
		else
			if (special >= 214 && special <= 218)       // accelerative scrollers
			{
				accel = 1;
				special += 250 - 214;
				control = ::g->sides[*l->sidenum].sector - ::g->sectors;
			}

		switch (special)
		{
			/*register*/ int s;

		case 250:   // scroll effect ceiling
			for (s = -1; (s = P_FindSectorFromLineTag(l, s)) >= 0;)
				Add_Scroller(sc_ceiling, -dx, dy, control, s, accel);
			break;

		case 251:   // scroll effect floor
		case 253:   // scroll and carry objects on floor
			for (s = -1; (s = P_FindSectorFromLineTag(l, s)) >= 0;)
				Add_Scroller(sc_floor, -dx, dy, control, s, accel);
			if (special != 253)
				break;

		case 252: // carry objects on floor
			dx = FixedMul(dx, CARRYFACTOR);
			dy = FixedMul(dy, CARRYFACTOR);
			for (s = -1; (s = P_FindSectorFromLineTag(l, s)) >= 0;)
				Add_Scroller(sc_carry, dx, dy, control, s, accel);
			break;

			// killough 3/1/98: scroll wall according to linedef
			// (same direction and speed as scrolling floors)
		case 254:
			for (s = -1; (s = P_FindLineFromLineTag(l, s)) >= 0;)
				if (s != i)
					Add_WallScroller(dx, dy, ::g->lines + s, control, accel);
			break;

		case 255:    // killough 3/2/98: scroll according to sidedef offsets
			s = ::g->lines[i].sidenum[0];
			Add_Scroller(sc_side, -::g->sides[s].textureoffset,
				::g->sides[s].rowoffset, -1, s, accel);
			break;

		case 48:                  // scroll first side
			Add_Scroller(sc_side, FRACUNIT, 0, -1, ::g->lines[i].sidenum[0], accel);
			break;

		case 85:                  // jff 1/30/98 2-way scroll
			Add_Scroller(sc_side, -FRACUNIT, 0, -1, ::g->lines[i].sidenum[0], accel);
			break;
		case 1024: // special 255 with tag control
        case 1025:
        case 1026:
          if (l->tag == 0)
            I_Error("Line %d is missing a tag!", i);

          if (special > 1024)
            control = ::g->sides[*l->sidenum].sector->counter; //GK: it's the same as iSectorID

          if (special == 1026)
            accel = 1;

          s = ::g->lines[i].sidenum[0];
          dx = -::g->sides[s].textureoffset;
          dy = ::g->sides[s].rowoffset;
          for (s = -1; (s = P_FindLineFromLineTag(l, s)) >= 0;)
            if (s != i)
              Add_Scroller(sc_side, dx, dy, control, ::g->lines[s].sidenum[0], accel);

          break;
		}
	}
}

// killough 3/7/98 -- end generalized scroll effects

//
// P_SectorActive()
//
// Passed a linedef special class (floor, ceiling, lighting) and a sector
// returns whether the sector is already busy with a linedef special of the
// same class. If old demo compatibility true, all linedef special classes
// are the same.
//
// jff 2/23/98 added to prevent old demos from
//  succeeding in starting multiple specials on one sector
//
int P_SectorActive(special_e t, sector_t *sec)
{
		switch (t)             // return whether thinker of same type is active
		{
		case floor_special:
			return *((int*)&sec->floordata); //GK: Dealing with linux idiosicracies
		case ceiling_special:
			return *((int*)&sec->ceilingdata);
		case lighting_special:
			return *((int*)&sec->lightingdata);
		}
	return 1; // don't know which special, must be active, shouldn't be here
}

//
// P_FindNextLowestFloor()
//
// Passed a sector and a floor height, returns the fixed point value
// of the largest floor height in a surrounding sector smaller than
// the floor height passed. If no such height exists the floorheight
// passed is returned.
//
// jff 02/03/98 Twiddled Lee's P_FindNextHighestFloor to make this
//
fixed_t P_FindNextLowestFloor(sector_t *sec, int currentheight)
{
	sector_t *other;
	int i;

	for (i = 0; i < sec->linecount; i++)
		if (((other = getNextSector(sec->lines[i], sec)) != 0) &&
			other->floorheight < currentheight)
		{
			int height = other->floorheight;
			while (++i < sec->linecount)
				if (((other = getNextSector(sec->lines[i], sec)) != 0) &&
					other->floorheight > height &&
					other->floorheight < currentheight)
					height = other->floorheight;
			return height;
		}
	return currentheight;
}

//
// P_FindShortestTextureAround()
//
// Passed a sector number, returns the shortest lower texture on a
// linedef bounding the sector.
//
// Note: If no lower texture exists 32000*FRACUNIT is returned.
//       but if compatibility then MAXINT is returned
//
// jff 02/03/98 Add routine to find shortest lower texture
//
fixed_t P_FindShortestTextureAround(int secnum)
{
	int minsize = MAXINT;
	side_t*     side;
	int i;
	sector_t *sec = &::g->sectors[secnum];

	/*if (!compatibility)*/
		minsize = 32000 << FRACBITS; //jff 3/13/98 prevent overflow in height calcs

	for (i = 0; i < sec->linecount; i++)
	{
		if (twoSided(secnum, i))
		{
			side = getSide(secnum, i, 0);
			if (side->bottomtexture > 0)  //jff 8/14/98 texture 0 is a placeholder
				if (::g->s_textureheight[side->bottomtexture] < minsize)
					minsize = ::g->s_textureheight[side->bottomtexture];
			side = getSide(secnum, i, 1);
			if (side->bottomtexture > 0)  //jff 8/14/98 texture 0 is a placeholder
				if (::g->s_textureheight[side->bottomtexture] < minsize)
					minsize = ::g->s_textureheight[side->bottomtexture];
		}
	}
	return minsize;
}

//
// P_FindModelCeilingSector()
//
// Passed a ceiling height and a sector number, return a pointer to a
// a sector with that ceiling height across the lowest numbered two sided
// line surrounding the sector.
//
// Note: If no sector at that height bounds the sector passed, return NULL
//
// jff 02/03/98 Add routine to find numeric model ceiling
//  around a sector specified by sector number
//  used only from generalized ceiling types
// jff 3/14/98 change first parameter to plain height to allow call
//  from routine not using ceiling_t
//
sector_t *P_FindModelCeilingSector(fixed_t ceildestheight, int secnum)
{
	//int i;
	sector_t *sec = NULL;
	int linecount;

	sec = &::g->sectors[secnum]; //jff 3/2/98 woops! better do this
							//jff 5/23/98 don't disturb sec->linecount while searching
							// but allow early exit in old demos
	linecount = sec->linecount;
	/*for (i = 0; i < (/*demo_compatibility && sec->linecount<linecount ?
		sec->linecount : linecount); i++)
	{
		if (twoSided(secnum, i))
		{
			if (getSide(secnum, i, 0)->sector - ::g->sectors == secnum)
				sec = getSector(secnum, i, 1);
			else
				sec = getSector(secnum, i, 0);

			if (sec->ceilingheight == ceildestheight)
				return sec;
		}
	}*/
	return NULL;
}

//
// P_FindModelFloorSector()
//
// Passed a floor height and a sector number, return a pointer to a
// a sector with that floor height across the lowest numbered two sided
// line surrounding the sector.
//
// Note: If no sector at that height bounds the sector passed, return NULL
//
// jff 02/03/98 Add routine to find numeric model floor
//  around a sector specified by sector number
// jff 3/14/98 change first parameter to plain height to allow call
//  from routine not using floormove_t
//
sector_t *P_FindModelFloorSector(fixed_t floordestheight, int secnum)
{
	//int i;
	sector_t *sec = NULL;
	int linecount;

	sec = &::g->sectors[secnum]; //jff 3/2/98 woops! better do this
							//jff 5/23/98 don't disturb sec->linecount while searching
							// but allow early exit in old demos
	linecount = sec->linecount;
	/*for (i = 0; i < (/*demo_compatibility && sec->linecount<linecount ?
		sec->linecount : linecount); i++)
	{
		if (twoSided(secnum, i))
		{
			if (getSide(secnum, i, 0)->sector - ::g->sectors == secnum)
				sec = getSector(secnum, i, 1);
			else
				sec = getSector(secnum, i, 0);

			if (sec->floorheight == floordestheight)
				return sec;
		}
	}*/
	return NULL;
}

//
// P_FindNextHighestCeiling()
//
// Passed a sector and a ceiling height, returns the fixed point value
// of the smallest ceiling height in a surrounding sector larger than
// the ceiling height passed. If no such height exists the ceiling height
// passed is returned.
//
// jff 02/03/98 Twiddled Lee's P_FindNextHighestFloor to make this
//
fixed_t P_FindNextHighestCeiling(sector_t *sec, int currentheight)
{
	sector_t *other;
	int i;

	for (i = 0; i < sec->linecount; i++)
		if (((other = getNextSector(sec->lines[i], sec)) != 0) &&
			other->ceilingheight > currentheight)
		{
			int height = other->ceilingheight;
			while (++i < sec->linecount)
				if (((other = getNextSector(sec->lines[i], sec)) != 0) &&
					other->ceilingheight < height &&
					other->ceilingheight > currentheight)
					height = other->ceilingheight;
			return height;
		}
	return currentheight;
}

//
// P_FindNextLowestCeiling()
//
// Passed a sector and a ceiling height, returns the fixed point value
// of the largest ceiling height in a surrounding sector smaller than
// the ceiling height passed. If no such height exists the ceiling height
// passed is returned.
//
// jff 02/03/98 Twiddled Lee's P_FindNextHighestFloor to make this
//
fixed_t P_FindNextLowestCeiling(sector_t *sec, int currentheight)
{
	sector_t *other;
	int i;

	for (i = 0; i < sec->linecount; i++)
		if (((other = getNextSector(sec->lines[i], sec)) != 0) &&
			other->ceilingheight < currentheight)
		{
			int height = other->ceilingheight;
			while (++i < sec->linecount)
				if (((other = getNextSector(sec->lines[i], sec)) != 0) &&
					other->ceilingheight > height &&
					other->ceilingheight < currentheight)
					height = other->ceilingheight;
			return height;
		}
	return currentheight;
}


//
// P_FindShortestUpperAround()
//
// Passed a sector number, returns the shortest upper texture on a
// linedef bounding the sector.
//
// Note: If no upper texture exists 32000*FRACUNIT is returned.
//       but if compatibility then MAXINT is returned
//
// jff 03/20/98 Add routine to find shortest upper texture
//
fixed_t P_FindShortestUpperAround(int secnum)
{
	int minsize = MAXINT;
	side_t*     side;
	int i;
	sector_t *sec = &::g->sectors[secnum];

	/*if (!compatibility)*/
		minsize = 32000 << FRACBITS; //jff 3/13/98 prevent overflow
									 // in height calcs
	for (i = 0; i < sec->linecount; i++)
	{
		if (twoSided(secnum, i))
		{
			side = getSide(secnum, i, 0);
			if (side->toptexture > 0)     //jff 8/14/98 texture 0 is a placeholder
				if (::g->s_textureheight[side->toptexture] < minsize)
					minsize = ::g->s_textureheight[side->toptexture];
			side = getSide(secnum, i, 1);
			if (side->toptexture > 0)     //jff 8/14/98 texture 0 is a placeholder
				if (::g->s_textureheight[side->toptexture] < minsize)
					minsize = ::g->s_textureheight[side->toptexture];
		}
	}
	return minsize;
}

//
// P_CanUnlockGenDoor()
//
// Passed a generalized locked door linedef and a player, returns whether
// the player has the keys necessary to unlock that door.
//
// Note: The linedef passed MUST be a generalized locked door type
//       or results are undefined.
//
// jff 02/05/98 routine added to test for unlockability of
//  generalized locked doors
//
qboolean P_CanUnlockGenDoor
(line_t* line,
	player_t* player)
{
	// does this line special distinguish between skulls and keys?
	int skulliscard = (line->special & LockedNKeys) >> LockedNKeysShift;

	// determine for each case of lock type if player's keys are adequate
	switch ((line->special & LockedKey) >> LockedKeyShift)
	{
	case AnyKey:
		if
			(
				!player->cards[it_redcard] &&
				!player->cards[it_redskull] &&
				!player->cards[it_bluecard] &&
				!player->cards[it_blueskull] &&
				!player->cards[it_yellowcard] &&
				!player->cards[it_yellowskull]
				)
		{
			player->message = PD_ANY; // Ty 03/27/98 - externalized
			S_StartSound(player->mo, sfx_oof);             // killough 3/20/98
			return false;
		}
		break;
	case RCard:
		if
			(
				!player->cards[it_redcard] &&
				(!skulliscard || !player->cards[it_redskull])
				)
		{
			player->message = skulliscard ? PD_REDK : PD_REDC; // Ty 03/27/98 - externalized
			S_StartSound(player->mo, sfx_oof);             // killough 3/20/98
			return false;
		}
		break;
	case BCard:
		if
			(
				!player->cards[it_bluecard] &&
				(!skulliscard || !player->cards[it_blueskull])
				)
		{
			player->message = skulliscard ? PD_BLUEK : PD_BLUEC; // Ty 03/27/98 - externalized
			S_StartSound(player->mo, sfx_oof);             // killough 3/20/98
			return false;
		}
		break;
	case YCard:
		if
			(
				!player->cards[it_yellowcard] &&
				(!skulliscard || !player->cards[it_yellowskull])
				)
		{
			player->message = skulliscard ? PD_YELLOWK : PD_YELLOWC; // Ty 03/27/98 - externalized
			S_StartSound(player->mo, sfx_oof);             // killough 3/20/98
			return false;
		}
		break;
	case RSkull:
		if
			(
				!player->cards[it_redskull] &&
				(!skulliscard || !player->cards[it_redcard])
				)
		{
			player->message = skulliscard ? PD_REDK : PD_REDS; // Ty 03/27/98 - externalized
			S_StartSound(player->mo, sfx_oof);             // killough 3/20/98
			return false;
		}
		break;
	case BSkull:
		if
			(
				!player->cards[it_blueskull] &&
				(!skulliscard || !player->cards[it_bluecard])
				)
		{
			//player->message = skulliscard ? s_PD_BLUEK : s_PD_BLUES; // Ty 03/27/98 - externalized
			S_StartSound(player->mo, sfx_oof);             // killough 3/20/98
			return false;
		}
		break;
	case YSkull:
		if
			(
				!player->cards[it_yellowskull] &&
				(!skulliscard || !player->cards[it_yellowcard])
				)
		{
			player->message = skulliscard ? PD_YELLOWK : PD_YELLOWS; // Ty 03/27/98 - externalized
			S_StartSound(player->mo, sfx_oof);             // killough 3/20/98
			return false;
		}
		break;
	case AllKeys:
		if
			(
				!skulliscard &&
				(
					!player->cards[it_redcard] ||
					!player->cards[it_redskull] ||
					!player->cards[it_bluecard] ||
					!player->cards[it_blueskull] ||
					!player->cards[it_yellowcard] ||
					!player->cards[it_yellowskull]
					)
				)
		{
			player->message = PD_ALL6; // Ty 03/27/98 - externalized
			S_StartSound(player->mo, sfx_oof);             // killough 3/20/98
			return false;
		}
		if
			(
				skulliscard &&
				(
				(!player->cards[it_redcard] &&
					!player->cards[it_redskull]) ||
					(!player->cards[it_bluecard] &&
						!player->cards[it_blueskull]) ||
						(!player->cards[it_yellowcard] &&
							!player->cards[it_yellowskull])
					)
				)
		{
			player->message = PD_ALL3; // Ty 03/27/98 - externalized
			S_StartSound(player->mo, sfx_oof);             // killough 3/20/98
			return false;
		}
		break;
	}
	return true;
}
// killough 4/16/98: Same thing, only for linedefs

int P_FindLineFromLineTag(const line_t *line, int start)
{
	start = start >= 0 ? ::g->lines[start].nexttag :
		::g->lines[(unsigned)line->tag % (unsigned)::g->numlines].firsttag;
	while (start >= 0 && ::g->lines[start].tag != line->tag)
		start = ::g->lines[start].nexttag;
	return start;
}

// Hash the sector tags across the sectors and linedefs.
static void P_InitTagLists(void)
{
  int i;

  for (i=::g->numsectors; --i>=0; )        // Initially make all slots empty.
    ::g->sectors[i].firsttag = -1;
  for (i=::g->numsectors; --i>=0; )        // Proceed from last to first sector
    {                                 // so that lower sectors appear first
      int j = (unsigned) ::g->sectors[i].tag % (unsigned) ::g->numsectors; // Hash func
      ::g->sectors[i].nexttag = ::g->sectors[j].firsttag;   // Prepend sector to chain
      ::g->sectors[j].firsttag = i;
    }

  // killough 4/17/98: same thing, only for linedefs

  for (i=::g->numlines; --i>=0; )        // Initially make all slots empty.
  ::g->lines[i].firsttag = -1;
  for (i=::g->numlines; --i>=0; )        // Proceed from last to first linedef
    {                               // so that lower linedefs appear first
      int j = (unsigned) ::g->lines[i].tag % (unsigned) ::g->numlines; // Hash func
      ::g->lines[i].nexttag = ::g->lines[j].firsttag;   // Prepend linedef to chain
      ::g->lines[j].firsttag = i;
    }
}

////////////////////////////////////////////////////////////////////////////
//
// FRICTION EFFECTS
//
// phares 3/12/98: Start of friction effects

// As the player moves, friction is applied by decreasing the x and y
// momentum values on each tic. By varying the percentage of decrease,
// we can simulate muddy or icy conditions. In mud, the player slows
// down faster. In ice, the player slows down more slowly.
//
// The amount of friction change is controlled by the length of a linedef
// with type 223. A length < 100 gives you mud. A length > 100 gives you ice.
//
// Also, each sector where these effects are to take place is given a
// new special type _______. Changing the type value at runtime allows
// these effects to be turned on or off.
//
// Sector boundaries present problems. The player should experience these
// friction changes only when his feet are touching the sector floor. At
// sector boundaries where floor height changes, the player can find
// himself still 'in' one sector, but with his feet at the floor level
// of the next sector (steps up or down). To handle this, Thinkers are used
// in icy/muddy sectors. These thinkers examine each object that is touching
// their sectors, looking for players whose feet are at the same level as
// their floors. Players satisfying this condition are given new friction
// values that are applied by the player movement code later.

/////////////////////////////
//
// Add a friction thinker to the thinker list
//
// Add_Friction adds a new friction thinker to the list of active thinkers.
//

static void Add_Friction(int friction, int movefactor, int affectee)
    {
    friction_t *f = (friction_t*)DoomLib::Z_Malloc(sizeof *f, PU_LEVSPEC, 0);

    f->thinker.function = (actionf_p1) T_Friction;
    f->friction = friction;
    f->movefactor = movefactor;
    f->affectee = affectee;
    P_AddThinker(&f->thinker);
    }

/////////////////////////////
//
// This is where abnormal friction is applied to objects in the sectors.
// A friction thinker has been spawned for each sector where less or
// more friction should be applied. The amount applied is proportional to
// the length of the controlling linedef.

void T_Friction(friction_t *f)
    {
    sector_t *sec;
    mobj_t   *thing;
    std::shared_ptr<msecnode_t> node;

    if (::g->demoplayback)
        return;

    sec = ::g->sectors + f->affectee;

    // Be sure the special sector type is still turned on. If so, proceed.
    // Else, bail out; the sector type has been changed on us.

    if (!(sec->special & FRICTION_MASK))
        return;

    // Assign the friction value to players on the floor, non-floating,
    // and clipped. Normally the object's friction value is kept at
    // ORIG_FRICTION and this thinker changes it for icy or muddy floors.

    // In Phase II, you can apply friction to Things other than players.

    // When the object is straddling sectors with the same
    // floorheight that have different frictions, use the lowest
    // friction value (muddy has precedence over icy).

	for (size_t i = 0; i < ::g->sector_list.size(); i++)
	{
		if (::g->sector_list[i]->m_sector != sec) {
			continue;
		}
		node = ::g->sector_list[i];
		thing = node->m_thing;
		if (thing->player &&
			!(thing->flags & (MF_NOGRAVITY | MF_NOCLIP)) &&
			thing->z <= sec->floorheight)
		{
			if ((thing->friction == ORIG_FRICTION) || // normal friction?
				(f->friction < thing->friction))
			{
				thing->friction = f->friction;
				thing->movefactor = f->movefactor;
			}
		}
	}
	}

/////////////////////////////
//
// Initialize the sectors where friction is increased or decreased

static void P_SpawnFriction(void)
    {
    int i;
    line_t *l = ::g->lines;
    int s;
    int length;     // line length controls magnitude
    int friction;   // friction value to be applied during movement
    int movefactor; // applied to each player move to simulate inertia

    for (i = 0 ; i < ::g->numlines ; i++,l++)
        if (l->special == 223)
            {
            length = P_AproxDistance(l->dx,l->dy)>>FRACBITS;
            friction = (0x1EB8*length)/0x80 + 0xD000;

            // The following check might seem odd. At the time of movement,
            // the move distance is multiplied by 'friction/0x10000', so a
            // higher friction value actually means 'less friction'.

            if (friction > ORIG_FRICTION)       // ice
                movefactor = ((0x10092 - friction)*(0x70))/0x158;
            else
                movefactor = ((friction - 0xDB34)*(0xA))/0x80;
            for (s = -1; (s = P_FindSectorFromLineTag(l,s)) >= 0 ; )
                Add_Friction(friction,movefactor,s);
            }
    }

//
// phares 3/12/98: End of friction effects
//
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//
// PUSH/PULL EFFECT
//
// phares 3/20/98: Start of push/pull effects
//
// This is where push/pull effects are applied to objects in the sectors.
//
// There are four kinds of push effects
//
// 1) Pushing Away
//
//    Pushes you away from a point source defined by the location of an
//    MT_PUSH Thing. The force decreases linearly with distance from the
//    source. This force crosses sector boundaries and is felt w/in a circle
//    whose center is at the MT_PUSH. The force is felt only if the point
//    MT_PUSH can see the target object.
//
// 2) Pulling toward
//
//    Same as Pushing Away except you're pulled toward an MT_PULL point
//    source. This force crosses sector boundaries and is felt w/in a circle
//    whose center is at the MT_PULL. The force is felt only if the point
//    MT_PULL can see the target object.
//
// 3) Wind
//
//    Pushes you in a constant direction. Full force above ground, half
//    force on the ground, nothing if you're below it (water).
//
// 4) Current
//
//    Pushes you in a constant direction. No force above ground, full
//    force if on the ground or below it (water).
//
// The magnitude of the force is controlled by the length of a controlling
// linedef. The force vector for types 3 & 4 is determined by the angle
// of the linedef, and is constant.
//
// For each sector where these effects occur, the sector special type has
// to have the PUSH_MASK bit set. If this bit is turned off by a switch
// at run-time, the effect will not occur. The controlling sector for
// types 1 & 2 is the sector containing the MT_PUSH/MT_PULL Thing.


#define PUSH_FACTOR 7

/////////////////////////////
//
// Add a push thinker to the thinker list

static void Add_Pusher(int type, int x_mag, int y_mag, mobj_t* source, int affectee)
    {
    pusher_t *p = (pusher_t*)DoomLib::Z_Malloc(sizeof *p, PU_LEVSPEC, 0);

    p->thinker.function = (actionf_p1) T_Pusher;
    p->source = source;
    p->type = (pusherType_t)type;
    p->x_mag = x_mag>>FRACBITS;
    p->y_mag = y_mag>>FRACBITS;
    p->magnitude = P_AproxDistance(p->x_mag,p->y_mag);
    if (source) // point source exist?
        {
        p->radius = (p->magnitude)<<(FRACBITS+1); // where force goes to zero
        p->x = p->source->x;
        p->y = p->source->y;
        }
    p->affectee = affectee;
    P_AddThinker(&p->thinker);
    }

/////////////////////////////
//
// PIT_PushThing determines the angle and magnitude of the effect.
// The object's x and y momentum values are changed.
//
// tmpusher belongs to the point source (MT_PUSH/MT_PULL).
//

extern "C" {

qboolean PIT_PushThing(mobj_t* thing)
    {
    if (thing->player &&
        !(thing->flags & (MF_NOGRAVITY | MF_NOCLIP)))
        {
        angle_t pushangle;
        int dist;
        int speed;
        int sx,sy;

        sx = ::g->tmpusher->x;
        sy = ::g->tmpusher->y;
        dist = P_AproxDistance(thing->x - sx,thing->y - sy);
        speed = (::g->tmpusher->magnitude -
                 ((dist>>FRACBITS)>>1))<<(FRACBITS-PUSH_FACTOR-1);

        // If speed <= 0, you're outside the effective radius. You also have
        // to be able to see the push/pull source point.

        if ((speed > 0) && (P_CheckSight(thing,::g->tmpusher->source)))
            {
            pushangle = R_PointToAngle2(thing->x,thing->y,sx,sy);
            if (::g->tmpusher->source->type == MT_PUSH)
                pushangle += ANG180;    // away
            pushangle >>= ANGLETOFINESHIFT;
            thing->momx += FixedMul(speed,finecosine[pushangle]);
            thing->momy += FixedMul(speed,finesine[pushangle]);
            }
        }
    return true;
    }
}
/////////////////////////////
//
// T_Pusher looks for all objects that are inside the radius of
// the effect.
//

void T_Pusher(pusher_t *p)
    {
    sector_t *sec;
    mobj_t   *thing;
    std::shared_ptr<msecnode_t> node;
    int xspeed,yspeed;
    int xl,xh,yl,yh,bx,by;
    int radius;
    int ht = 0;

    if (::g->demoplayback)
        return;

    sec = ::g->sectors + p->affectee;

    // Be sure the special sector type is still turned on. If so, proceed.
    // Else, bail out; the sector type has been changed on us.

    if (!(sec->special & PUSH_MASK))
        return;

    // For constant pushers (wind/current) there are 3 situations:
    //
    // 1) Affected Thing is above the floor.
    //
    //    Apply the full force if wind, no force if current.
    //
    // 2) Affected Thing is on the ground.
    //
    //    Apply half force if wind, full force if current.
    //
    // 3) Affected Thing is below the ground (underwater effect).
    //
    //    Apply no force if wind, full force if current.
    //
    // Apply the effect to clipped players only for now.
    //
    // In Phase II, you can apply these effects to Things other than players.

    if (p->type == p_push)
        {

        // Seek out all pushable things within the force radius of this
        // point pusher. Crosses sectors, so use blockmap.

        ::g->tmpusher = p; // MT_PUSH/MT_PULL point source
        radius = p->radius; // where force goes to zero
        ::g->tmbbox[BOXTOP]    = p->y + radius;
        ::g->tmbbox[BOXBOTTOM] = p->y - radius;
        ::g->tmbbox[BOXRIGHT]  = p->x + radius;
        ::g->tmbbox[BOXLEFT]   = p->x - radius;

        xl = (::g->tmbbox[BOXLEFT] - ::g->bmaporgx - MAXRADIUS)>>MAPBLOCKSHIFT;
        xh = (::g->tmbbox[BOXRIGHT] - ::g->bmaporgx + MAXRADIUS)>>MAPBLOCKSHIFT;
        yl = (::g->tmbbox[BOXBOTTOM] - ::g->bmaporgy - MAXRADIUS)>>MAPBLOCKSHIFT;
        yh = (::g->tmbbox[BOXTOP] - ::g->bmaporgy + MAXRADIUS)>>MAPBLOCKSHIFT;
        for (bx=xl ; bx<=xh ; bx++)
            for (by=yl ; by<=yh ; by++)
                P_BlockThingsIterator(bx,by,PIT_PushThing);
        return;
        }

    // constant pushers p_wind and p_current

    if (sec->heightsec != -1) // special water sector?
        ht = ::g->sectors[sec->heightsec].floorheight;
    for (size_t j = 0; j < ::g->sector_list.size(); j++)
        {
			if (::g->sector_list[j]->m_sector != sec) {
				continue;
			}
			node = ::g->sector_list[j];
        thing = node->m_thing;
        if (!thing->player || (thing->flags & (MF_NOGRAVITY | MF_NOCLIP)))
            continue;
        if (p->type == p_wind)
            {
            if (sec->heightsec == -1) // NOT special water sector
                if (thing->z > thing->floorz) // above ground
                    {
                    xspeed = p->x_mag; // full force
                    yspeed = p->y_mag;
                    }
                else // on ground
                    {
                    xspeed = (p->x_mag)>>1; // half force
                    yspeed = (p->y_mag)>>1;
                    }
            else // special water sector
                {
                if (thing->z > ht) // above ground
                    {
                    xspeed = p->x_mag; // full force
                    yspeed = p->y_mag;
                    }
                else if (thing->player->viewz < ht) // underwater
                    xspeed = yspeed = 0; // no force
                else // wading in water
                    {
                    xspeed = (p->x_mag)>>1; // half force
                    yspeed = (p->y_mag)>>1;
                    }
                }
            }
        else // p_current
            {
            if (sec->heightsec == -1) // NOT special water sector
                if (thing->z > sec->floorheight) // above ground
                    xspeed = yspeed = 0; // no force
                else // on ground
                    {
                    xspeed = p->x_mag; // full force
                    yspeed = p->y_mag;
                    }
            else // special water sector
                if (thing->z > ht) // above ground
                    xspeed = yspeed = 0; // no force
                else // underwater
                    {
                    xspeed = p->x_mag; // full force
                    yspeed = p->y_mag;
                    }
            }
        thing->momx += xspeed<<(FRACBITS-PUSH_FACTOR);
        thing->momy += yspeed<<(FRACBITS-PUSH_FACTOR);
        }
    }

/////////////////////////////
//
// P_GetPushThing() returns a pointer to an MT_PUSH or MT_PULL thing,
// NULL otherwise.

mobj_t* P_GetPushThing(int s)
    {
    mobj_t* thing;
    sector_t* sec;

    sec = ::g->sectors + s;
    thing = sec->thinglist;
    while (thing)
        {
        switch(thing->type)
            {
          case MT_PUSH:
          case MT_PULL:
            return thing;
          default:
            break;
            }
        thing = thing->snext;
        }
    return NULL;
    }

/////////////////////////////
//
// Initialize the sectors where pushers are present
//

static void P_SpawnPushers(void)
    {
    int i;
    line_t *l = ::g->lines;
    int s;
    mobj_t* thing;

    for (i = 0 ; i < ::g->numlines ; i++,l++)
        switch(l->special)
            {
          case 224: // wind
            for (s = -1; (s = P_FindSectorFromLineTag(l,s)) >= 0 ; )
                Add_Pusher(p_wind,l->dx,l->dy,NULL,s);
            break;
          case 225: // current
            for (s = -1; (s = P_FindSectorFromLineTag(l,s)) >= 0 ; )
                Add_Pusher(p_current,l->dx,l->dy,NULL,s);
            break;
          case 226: // push/pull
            for (s = -1; (s = P_FindSectorFromLineTag(l,s)) >= 0 ; )
                {
                thing = P_GetPushThing(s);
                if (thing) // No MT_P* means no effect
                    Add_Pusher(p_push,l->dx,l->dy,thing,s);
                }
            break;
            }
    }

//
// phares 3/20/98: End of Pusher effects
//
////////////////////////////////////////////////////////////////////////////