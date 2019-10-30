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


#include "i_system.h"
#include "doomdef.h"
#include "p_local.h"

#include "g_game.h"

#include "s_sound.h"

// Data.
#include "sounds.h"

// State.
#include "doomstat.h"
#include "r_state.h"


//
// CHANGE THE TEXTURE OF A WALL SWITCH TO ITS OPPOSITE
//
//GK:From now on this will be a temp vanilla list just in case there is no SWITCHES lump loaded
switchlist_t tSwitchList[] =
{
    // Doom shareware episode 1 switches
    {"SW1BRCOM",	"SW2BRCOM",	1},
    {"SW1BRN1",	"SW2BRN1",	1},
    {"SW1BRN2",	"SW2BRN2",	1},
    {"SW1BRNGN",	"SW2BRNGN",	1},
    {"SW1BROWN",	"SW2BROWN",	1},
    {"SW1COMM",	"SW2COMM",	1},
    {"SW1COMP",	"SW2COMP",	1},
    {"SW1DIRT",	"SW2DIRT",	1},
    {"SW1EXIT",	"SW2EXIT",	1},
    {"SW1GRAY",	"SW2GRAY",	1},
    {"SW1GRAY1",	"SW2GRAY1",	1},
    {"SW1METAL",	"SW2METAL",	1},
    {"SW1PIPE",	"SW2PIPE",	1},
    {"SW1SLAD",	"SW2SLAD",	1},
    {"SW1STARG",	"SW2STARG",	1},
    {"SW1STON1",	"SW2STON1",	1},
    {"SW1STON2",	"SW2STON2",	1},
    {"SW1STONE",	"SW2STONE",	1},
    {"SW1STRTN",	"SW2STRTN",	1},
    
    // Doom registered episodes 2&3 switches
    {"SW1BLUE",	"SW2BLUE",	2},
    {"SW1CMT",		"SW2CMT",	2},
    {"SW1GARG",	"SW2GARG",	2},
    {"SW1GSTON",	"SW2GSTON",	2},
    {"SW1HOT",		"SW2HOT",	2},
    {"SW1LION",	"SW2LION",	2},
    {"SW1SATYR",	"SW2SATYR",	2},
    {"SW1SKIN",	"SW2SKIN",	2},
    {"SW1VINE",	"SW2VINE",	2},
    {"SW1WOOD",	"SW2WOOD",	2},
    
    // Doom II switches
    {"SW1PANEL",	"SW2PANEL",	3},
    {"SW1ROCK",	"SW2ROCK",	3},
    {"SW1MET2",	"SW2MET2",	3},
    {"SW1WDMET",	"SW2WDMET",	3},
    {"SW1BRIK",	"SW2BRIK",	3},
    {"SW1MOD1",	"SW2MOD1",	3},
    {"SW1ZIM",		"SW2ZIM",	3},
    {"SW1STON6",	"SW2STON6",	3},
    {"SW1TEK",		"SW2TEK",	3},
    {"SW1MARB",	"SW2MARB",	3},
    {"SW1SKULL",	"SW2SKULL",	3},
	
    {"\0",		"\0",		0}
};


//
// P_InitSwitchList
// Only called at game initialization.
//
void P_InitSwitchList(void)
{
    int		i;
    int		index;
    int		episode;
	switchlist_t* alphSwitchList;
	alphSwitchList = (switchlist_t *)W_CacheLumpName("SWITCHES", PU_STATIC); 
	if (!alphSwitchList) {//GK:Check if SWITCHES lump exist. If not then load the vanilla list
		alphSwitchList = tSwitchList;
	}
	
    episode = 1;

	if (::g->gamemode == registered || ::g->gamemode == retail)
		episode = 2;
    else if ( ::g->gamemode == commercial )
	    episode = 3;
    for (index = 0,i = 0;;i++)
    {
		if (!alphSwitchList[i].episode)
		{
			::g->numswitches = index/2;
			::g->switchlist[index] = -1;
			break;
		}
		
		if (alphSwitchList[i].episode <= episode)
		{
	#if 0	// UNUSED - debug?
			int		value;
				
			if (R_CheckTextureNumForName(alphSwitchList[i].name1) < 0)
			{
			I_Error("Can't find switch texture '%s'!",
				alphSwitchList[i].name1);
			continue;
			}
		    
			value = R_TextureNumForName(alphSwitchList[i].name1);
	#endif
			::g->switchlist[index++] = R_TextureNumForName(alphSwitchList[i].name1);
			::g->switchlist[index++] = R_TextureNumForName(alphSwitchList[i].name2);
		}
    }
}


//
// Start a button counting down till it turns off.
//
void
P_StartButton
( line_t*	line,
  bwhere_e	w,
  int		texture,
  int		time )
{
    int		i;
    
    // See if button is already pressed
    for (i = 0;i < MAXBUTTONS;i++)
    {
	if (::g->buttonlist[i].btimer
	    && ::g->buttonlist[i].line == line)
	{
	    
	    return;
	}
    }
    

    
    for (i = 0;i < MAXBUTTONS;i++)
    {
	if (!::g->buttonlist[i].btimer)
	{
	    ::g->buttonlist[i].line = line;
	    ::g->buttonlist[i].where = w;
	    ::g->buttonlist[i].btexture = texture;
	    ::g->buttonlist[i].btimer = time;
	    ::g->buttonlist[i].degensoundorg = &line->frontsector->soundorg;
	    return;
	}
    }
    
    I_Error("P_StartButton: no button slots left!");
}





//
// Function that changes wall texture.
// Tell it if switch is ok to use again (1=yes, it's a button).
//
void
P_ChangeSwitchTexture
( line_t*	line,
  int 		useAgain )
{
    int     texTop;
    int     texMid;
    int     texBot;
    int     i;
    int     sound;
	
    if (!useAgain)
	line->special = 0;

    texTop = ::g->sides[line->sidenum[0]].toptexture;
    texMid = ::g->sides[line->sidenum[0]].midtexture;
    texBot = ::g->sides[line->sidenum[0]].bottomtexture;
	
    sound = sfx_swtchn;

    // EXIT SWITCH?
    if (line->special == 11)                
	sound = sfx_swtchx;

	bool skipSound = false;
	bool front = true;
	bool back = true;
	if (line->frontsector) {
		front = line->frontsector->ceilingheight != line->frontsector->floorheight;
	}
	if (line->backsector) {
		back = line->backsector->ceilingheight != line->backsector->floorheight;
	}
	if (line->special == 46 && front && back) {
		skipSound = true;
	}
    for (i = 0;i < ::g->numswitches*2;i++)
    {
	if (::g->switchlist[i] == texTop)
	{
		if (!skipSound)
			S_StartSound(::g->buttonlist->soundorg,sound);

	    ::g->sides[line->sidenum[0]].toptexture = ::g->switchlist[i^1];

	    if (useAgain)
		P_StartButton(line,top,::g->switchlist[i],BUTTONTIME);

	    return;
	}
	else
	{
	    if (::g->switchlist[i] == texMid)
	    {
			if (!skipSound)
				S_StartSound(::g->buttonlist->soundorg,sound);

		::g->sides[line->sidenum[0]].midtexture = ::g->switchlist[i^1];

		if (useAgain)
		    P_StartButton(line, middle,::g->switchlist[i],BUTTONTIME);

		return;
	    }
	    else
	    {
		if (::g->switchlist[i] == texBot)
		{
			if (!skipSound)
				S_StartSound(::g->buttonlist->soundorg,sound);
		    ::g->sides[line->sidenum[0]].bottomtexture = ::g->switchlist[i^1];

		    if (useAgain)
			P_StartButton(line, bottom,::g->switchlist[i],BUTTONTIME);

		    return;
		}
	    }
	}
    }
}






//
// P_UseSpecialLine
// Called when a thing uses a special line.
// Only the front ::g->sides of ::g->lines are usable.
//
qboolean
P_UseSpecialLine
( mobj_t*	thing,
  line_t*	line,
  int		side )
{               

    // Err...
    // Use the back ::g->sides of VERY SPECIAL ::g->lines...
    if (side)
    {
	switch(line->special)
	{
	  case 124:
	    // Sliding door open&close
	    // UNUSED?
	    break;

	  default:
	    return false;
	    break;
	}
    }

	
    // Switches that other things can activate.
    if (!thing->player)
    {
	// never open secret doors
	if (line->flags & ML_SECRET)
	    return false;
	
	switch(line->special)
	{
	  case 1: 	// MANUAL DOOR RAISE
	  case 32:	// MANUAL BLUE
	  case 33:	// MANUAL RED
	  case 34:	// MANUAL YELLOW
				//jff 3/5/98 add ability to use teleporters for monsters
	  case 195:       // switch teleporters
	  case 174:
	  case 210:       // silent switch teleporters
	    break;
	    
	  default:
	    return false;
	    break;
	}
    }

	//jff 02/04/98 add check here for generalized floor/ceil mover
	{
		// pointer to line function is NULL by default, set non-null if
		// line special is push or switch generalized linedef type
		int(*linefunc)(line_t *line) = NULL;

		// check each range of generalized linedefs
		if ((unsigned)line->special >= GenFloorBase)
		{
			if (!thing->player)
				if ((line->special & FloorChange) || !(line->special & FloorModel))
					return false; // FloorModel is "Allow Monsters" if FloorChange is 0
			if (!line->tag && ((line->special & 6) != 6)) //jff 2/27/98 all non-manual
				return false;                         // generalized types require tag
			linefunc = EV_DoGenFloor;
		}
		else if ((unsigned)line->special >= GenCeilingBase)
		{
			if (!thing->player)
				if ((line->special & CeilingChange) || !(line->special & CeilingModel))
					return false;   // CeilingModel is "Allow Monsters" if CeilingChange is 0
			if (!line->tag && ((line->special & 6) != 6)) //jff 2/27/98 all non-manual
				return false;                         // generalized types require tag
			linefunc = EV_DoGenCeiling;
		}
		else if ((unsigned)line->special >= GenDoorBase)
		{
			if (!thing->player)
			{
				if (!(line->special & DoorMonster))
					return false;   // monsters disallowed from this door
				if (line->flags & ML_SECRET) // they can't open secret doors either
					return false;
			}
			if (!line->tag && ((line->special & 6) != 6)) //jff 3/2/98 all non-manual
				return false;                         // generalized types require tag
			linefunc = EV_DoGenDoor;
		}
		else if ((unsigned)line->special >= GenLockedBase)
		{
			if (!thing->player)
				return false;   // monsters disallowed from unlocking doors
			if (!P_CanUnlockGenDoor(line, thing->player))
				return false;
			if (!line->tag && ((line->special & 6) != 6)) //jff 2/27/98 all non-manual
				return false;                         // generalized types require tag

			linefunc = EV_DoGenLockedDoor;
		}
		else if ((unsigned)line->special >= GenLiftBase)
		{
			if (!thing->player)
				if (!(line->special & LiftMonster))
					return false; // monsters disallowed
			if (!line->tag && ((line->special & 6) != 6)) //jff 2/27/98 all non-manual
				return false;                         // generalized types require tag
			linefunc = EV_DoGenLift;
		}
		else if ((unsigned)line->special >= GenStairsBase)
		{
			if (!thing->player)
				if (!(line->special & StairMonster))
					return false; // monsters disallowed
			if (!line->tag && ((line->special & 6) != 6)) //jff 2/27/98 all non-manual
				return false;                         // generalized types require tag
			linefunc = EV_DoGenStairs;
		}
		else if ((unsigned)line->special >= GenCrusherBase)
		{
			if (!thing->player)
				if (!(line->special & CrusherMonster))
					return false; // monsters disallowed
			if (!line->tag && ((line->special & 6) != 6)) //jff 2/27/98 all non-manual
				return false;                         // generalized types require tag
			linefunc = EV_DoGenCrusher;
		}

		if (linefunc)
			switch ((line->special & TriggerType) >> TriggerTypeShift)
			{
			case PushOnce:
				if (!side)
					if (linefunc(line))
						line->special = 0;
				return true;
			case PushMany:
				if (!side)
					linefunc(line);
				return true;
			case SwitchOnce:
				if (linefunc(line))
					P_ChangeSwitchTexture(line, 0);
				return true;
			case SwitchMany:
				if (linefunc(line))
					P_ChangeSwitchTexture(line, 1);
				return true;
			default:  // if not a switch/push type, do nothing here
				return false;
			}
	}

    
    // do something  
    switch (line->special)
    {
	// MANUALS
      case 1:		// Vertical Door
      case 26:		// Blue Door/Locked
      case 27:		// Yellow Door /Locked
      case 28:		// Red Door /Locked

      case 31:		// Manual door open
      case 32:		// Blue locked door open
      case 33:		// Red locked door open
      case 34:		// Yellow locked door open

      case 117:		// Blazing door raise
      case 118:		// Blazing door open
	EV_VerticalDoor (line, thing);
	break;
	
	//UNUSED - Door Slide Open&Close
	// case 124:
	// EV_SlidingDoor (line, thing);
	// break;

	// SWITCHES
      case 7:
	// Build Stairs
	if (EV_BuildStairs(line,build8))
	    P_ChangeSwitchTexture(line,0);
	break;

      case 9:
	// Change Donut
	if (EV_DoDonut(line))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 11:
	// Exit level
	// DHM - Nerve :: Not in deathmatch, stay in level until timelimit or fraglimit
	if ( !::g->deathmatch && ::g->gameaction != ga_completed ) {
		P_ChangeSwitchTexture(line,0);
		G_ExitLevel ();
	}
	break;
	
      case 14:
	// Raise Floor 32 and change texture
	if (EV_DoPlat(line,raiseAndChange,32))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 15:
	// Raise Floor 24 and change texture
	if (EV_DoPlat(line,raiseAndChange,24))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 18:
	// Raise Floor to next highest floor
	if (EV_DoFloor(line, raiseFloorToNearest))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 20:
	// Raise Plat next highest floor and change texture
	if (EV_DoPlat(line,raiseToNearestAndChange,0))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 21:
	// PlatDownWaitUpStay
	if (EV_DoPlat(line,downWaitUpStay,0))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 23:
	// Lower Floor to Lowest
	if (EV_DoFloor(line,lowerFloorToLowest))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 29:
	// Raise Door
	if (EV_DoDoor(line,normal))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 41:
	// Lower Ceiling to Floor
	if (EV_DoCeiling(line,lowerToFloor))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 71:
	// Turbo Lower Floor
	if (EV_DoFloor(line,turboLower))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 49:
	// Ceiling Crush And Raise
	if (EV_DoCeiling(line,crushAndRaise))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 50:
	// Close Door
	if (EV_DoDoor(line,closed))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 51:
	// Secret EXIT
	if ( !::g->deathmatch && ::g->gameaction != ga_completed ) {
		P_ChangeSwitchTexture(line,0);
		G_SecretExitLevel ();
	}
	break;
	
      case 55:
	// Raise Floor Crush
	if (EV_DoFloor(line,raiseFloorCrush))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 101:
	// Raise Floor
	if (EV_DoFloor(line,raiseFloor))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 102:
	// Lower Floor to Surrounding floor height
	if (EV_DoFloor(line,lowerFloor))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 103:
	// Open Door
	if (EV_DoDoor(line,opened))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 111:
	// Blazing Door Raise (faster than TURBO!)
	if (EV_DoDoor (line,blazeRaise))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 112:
	// Blazing Door Open (faster than TURBO!)
	if (EV_DoDoor (line,blazeOpen))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 113:
	// Blazing Door Close (faster than TURBO!)
	if (EV_DoDoor (line,blazeClose))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 122:
	// Blazing PlatDownWaitUpStay
	if (EV_DoPlat(line,blazeDWUS,0))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 127:
	// Build Stairs Turbo 16
	if (EV_BuildStairs(line,turbo16))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 131:
	// Raise Floor Turbo
	if (EV_DoFloor(line,raiseFloorTurbo))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 133:
	// BlzOpenDoor BLUE
      case 135:
	// BlzOpenDoor RED
      case 137:
	// BlzOpenDoor YELLOW
	if (EV_DoLockedDoor (line,blazeOpen,thing))
	    P_ChangeSwitchTexture(line,0);
	break;
	
      case 140:
	// Raise Floor 512
	if (EV_DoFloor(line,raiseFloor512))
	    P_ChangeSwitchTexture(line,0);
	break;

	// killough 1/31/98: factored out compatibility check;
	// added inner switch, relaxed check to demo_compatibility

	  default:
			  switch (line->special)
			  {
				  //jff 1/29/98 added linedef types to fill all functions out so that
				  // all possess SR, S1, WR, W1 types

			  case 158:
				  // Raise Floor to shortest lower texture
				  // 158 S1  EV_DoFloor(raiseToTexture), CSW(0)
				  if (EV_DoFloor(line, raiseToTexture))
					  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 159:
				  // Raise Floor to shortest lower texture
				  // 159 S1  EV_DoFloor(lowerAndChange)
				  if (EV_DoFloor(line, lowerAndChange))
					  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 160:
				  // Raise Floor 24 and change
				  // 160 S1  EV_DoFloor(raiseFloor24AndChange)
				  if (EV_DoFloor(line, raiseFloor24AndChange))
					  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 161:
				  // Raise Floor 24
				  // 161 S1  EV_DoFloor(raiseFloor24)
				  if (EV_DoFloor(line, raiseFloor24))
					  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 162:
				  // Moving floor min n to max n
				  // 162 S1  EV_DoPlat(perpetualRaise,0)
				  if (EV_DoPlat(line, perpetualRaise, 0))
					  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 163:
				  // Stop Moving floor
				  // 163 S1  EV_DoPlat(perpetualRaise,0)
				  EV_StopPlat(line);
				  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 164:
				  // Start fast crusher
				  // 164 S1  EV_DoCeiling(fastCrushAndRaise)
				  if (EV_DoCeiling(line, fastCrushAndRaise))
					  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 165:
				  // Start slow silent crusher
				  // 165 S1  EV_DoCeiling(silentCrushAndRaise)
				  if (EV_DoCeiling(line, silentCrushAndRaise))
					  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 166:
				  // Raise ceiling, Lower floor
				  // 166 S1 EV_DoCeiling(raiseToHighest), EV_DoFloor(lowerFloortoLowest)
				  if (EV_DoCeiling(line, raiseToHighest) ||
					  EV_DoFloor(line, lowerFloorToLowest))
					  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 167:
				  // Lower floor and Crush
				  // 167 S1 EV_DoCeiling(lowerAndCrush)
				  if (EV_DoCeiling(line, lowerAndCrush))
					  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 168:
				  // Stop crusher
				  // 168 S1 EV_CeilingCrushStop()
				  if (EV_CeilingCrushStop(line))
					  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 169:
				  // Lights to brightest neighbor sector
				  // 169 S1  EV_LightTurnOn(0)
				  EV_LightTurnOn(line, 0);
				  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 170:
				  // Lights to near dark
				  // 170 S1  EV_LightTurnOn(35)
				  EV_LightTurnOn(line, 35);
				  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 171:
				  // Lights on full
				  // 171 S1  EV_LightTurnOn(255)
				  EV_LightTurnOn(line, 255);
				  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 172:
				  // Start Lights Strobing
				  // 172 S1  EV_StartLightStrobing()
				  EV_StartLightStrobing(line);
				  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 173:
				  // Lights to Dimmest Near
				  // 173 S1  EV_TurnTagLightsOff()
				  EV_TurnTagLightsOff(line);
				  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 174:
				  // Teleport
				  // 174 S1  EV_Teleport(side,thing)
				  if (EV_Teleport(line, side, thing))
					  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 175:
				  // Close Door, Open in 30 secs
				  // 175 S1  EV_DoDoor(close30ThenOpen)
				  if (EV_DoDoor(line, close30ThenOpen))
					  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 189: //jff 3/15/98 create texture change no motion type
						// Texture Change Only (Trigger)
						// 189 S1 Change Texture/Type Only
				  if (EV_DoChange(line, trigChangeOnly))
					  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 203:
				  // Lower ceiling to lowest surrounding ceiling
				  // 203 S1 EV_DoCeiling(lowerToLowest)
				  if (EV_DoCeiling(line, lowerToLowest))
					  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 204:
				  // Lower ceiling to highest surrounding floor
				  // 204 S1 EV_DoCeiling(lowerToMaxFloor)
				  if (EV_DoCeiling(line, lowerToMaxFloor))
					  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 209:
				  // killough 1/31/98: silent teleporter
				  //jff 209 S1 SilentTeleport 
				  if (EV_SilentTeleport(line, side, thing))
					  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 241: //jff 3/15/98 create texture change no motion type
						// Texture Change Only (Numeric)
						// 241 S1 Change Texture/Type Only
				  if (EV_DoChange(line, numChangeOnly))
					  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 221:
				  // Lower floor to next lowest floor
				  // 221 S1 Lower Floor To Nearest Floor
				  if (EV_DoFloor(line, lowerFloorToNearest))
					  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 229:
				  // Raise elevator next floor
				  // 229 S1 Raise Elevator next floor
				  if (EV_DoElevator(line, elevateUp))
					  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 233:
				  // Lower elevator next floor
				  // 233 S1 Lower Elevator next floor
				  if (EV_DoElevator(line, elevateDown))
					  P_ChangeSwitchTexture(line, 0);
				  break;

			  case 237:
				  // Elevator to current floor
				  // 237 S1 Elevator to current floor
				  if (EV_DoElevator(line, elevateCurrent))
					  P_ChangeSwitchTexture(line, 0);
				  break;


				  // jff 1/29/98 end of added S1 linedef types

				  //jff 1/29/98 added linedef types to fill all functions out so that
				  // all possess SR, S1, WR, W1 types

			  case 78: //jff 3/15/98 create texture change no motion type
					   // Texture Change Only (Numeric)
					   // 78 SR Change Texture/Type Only
				  if (EV_DoChange(line, numChangeOnly))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 176:
				  // Raise Floor to shortest lower texture
				  // 176 SR  EV_DoFloor(raiseToTexture), CSW(1)
				  if (EV_DoFloor(line, raiseToTexture))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 177:
				  // Raise Floor to shortest lower texture
				  // 177 SR  EV_DoFloor(lowerAndChange)
				  if (EV_DoFloor(line, lowerAndChange))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 178:
				  // Raise Floor 512
				  // 178 SR  EV_DoFloor(raiseFloor512)
				  if (EV_DoFloor(line, raiseFloor512))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 179:
				  // Raise Floor 24 and change
				  // 179 SR  EV_DoFloor(raiseFloor24AndChange)
				  if (EV_DoFloor(line, raiseFloor24AndChange))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 180:
				  // Raise Floor 24
				  // 180 SR  EV_DoFloor(raiseFloor24)
				  if (EV_DoFloor(line, raiseFloor24))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 181:
				  // Moving floor min n to max n
				  // 181 SR  EV_DoPlat(perpetualRaise,0)

				  EV_DoPlat(line, perpetualRaise, 0);
				  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 182:
				  // Stop Moving floor
				  // 182 SR  EV_DoPlat(perpetualRaise,0)
				  EV_StopPlat(line);
				  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 183:
				  // Start fast crusher
				  // 183 SR  EV_DoCeiling(fastCrushAndRaise)
				  if (EV_DoCeiling(line, fastCrushAndRaise))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 184:
				  // Start slow crusher
				  // 184 SR  EV_DoCeiling(crushAndRaise)
				  if (EV_DoCeiling(line, crushAndRaise))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 185:
				  // Start slow silent crusher
				  // 185 SR  EV_DoCeiling(silentCrushAndRaise)
				  if (EV_DoCeiling(line, silentCrushAndRaise))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 186:
				  // Raise ceiling, Lower floor
				  // 186 SR EV_DoCeiling(raiseToHighest), EV_DoFloor(lowerFloortoLowest)
				  if (EV_DoCeiling(line, raiseToHighest) ||
					  EV_DoFloor(line, lowerFloorToLowest))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 187:
				  // Lower floor and Crush
				  // 187 SR EV_DoCeiling(lowerAndCrush)
				  if (EV_DoCeiling(line, lowerAndCrush))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 188:
				  // Stop crusher
				  // 188 SR EV_CeilingCrushStop()
				  if (EV_CeilingCrushStop(line))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 190: //jff 3/15/98 create texture change no motion type
						// Texture Change Only (Trigger)
						// 190 SR Change Texture/Type Only
				  if (EV_DoChange(line, trigChangeOnly))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 191:
				  // Lower Pillar, Raise Donut
				  // 191 SR  EV_DoDonut()
				  if (EV_DoDonut(line))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 192:
				  // Lights to brightest neighbor sector
				  // 192 SR  EV_LightTurnOn(0)
				  EV_LightTurnOn(line, 0);
				  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 193:
				  // Start Lights Strobing
				  // 193 SR  EV_StartLightStrobing()
				  EV_StartLightStrobing(line);
				  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 194:
				  // Lights to Dimmest Near
				  // 194 SR  EV_TurnTagLightsOff()
				  EV_TurnTagLightsOff(line);
				  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 195:
				  // Teleport
				  // 195 SR  EV_Teleport(side,thing)
				  if (EV_Teleport(line, side, thing))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 196:
				  // Close Door, Open in 30 secs
				  // 196 SR  EV_DoDoor(close30ThenOpen)
				  if (EV_DoDoor(line, close30ThenOpen))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 205:
				  // Lower ceiling to lowest surrounding ceiling
				  // 205 SR EV_DoCeiling(lowerToLowest)
				  if (EV_DoCeiling(line, lowerToLowest))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 206:
				  // Lower ceiling to highest surrounding floor
				  // 206 SR EV_DoCeiling(lowerToMaxFloor)
				  if (EV_DoCeiling(line, lowerToMaxFloor))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 210:
				  // killough 1/31/98: silent teleporter
				  //jff 210 SR SilentTeleport 
				  if (EV_SilentTeleport(line, side, thing))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 211: //jff 3/14/98 create instant toggle floor type
						// Toggle Floor Between C and F Instantly
						// 211 SR Toggle Floor Instant
				  if (EV_DoPlat(line, toggleUpDn, 0))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 222:
				  // Lower floor to next lowest floor
				  // 222 SR Lower Floor To Nearest Floor
				  if (EV_DoFloor(line, lowerFloorToNearest))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 230:
				  // Raise elevator next floor
				  // 230 SR Raise Elevator next floor
				  if (EV_DoElevator(line, elevateUp))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 234:
				  // Lower elevator next floor
				  // 234 SR Lower Elevator next floor
				  if (EV_DoElevator(line, elevateDown))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 238:
				  // Elevator to current floor
				  // 238 SR Elevator to current floor
				  if (EV_DoElevator(line, elevateCurrent))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 258:
				  // Build stairs, step 8
				  // 258 SR EV_BuildStairs(build8)
				  if (EV_BuildStairs(line, build8))
					  P_ChangeSwitchTexture(line, 1);
				  break;

			  case 259:
				  // Build stairs, step 16
				  // 259 SR EV_BuildStairs(turbo16)
				  if (EV_BuildStairs(line, turbo16))
					  P_ChangeSwitchTexture(line, 1);
				  break;

				  // 1/29/98 jff end of added SR linedef types

			  }
		  break;
	
	// BUTTONS
      case 42:
	// Close Door
	if (EV_DoDoor(line,closed))
	    P_ChangeSwitchTexture(line,1);
	break;
	
      case 43:
	// Lower Ceiling to Floor
	if (EV_DoCeiling(line,lowerToFloor))
	    P_ChangeSwitchTexture(line,1);
	break;
	
      case 45:
	// Lower Floor to Surrounding floor height
	if (EV_DoFloor(line,lowerFloor))
	    P_ChangeSwitchTexture(line,1);
	break;
	
      case 60:
	// Lower Floor to Lowest
	if (EV_DoFloor(line,lowerFloorToLowest))
	    P_ChangeSwitchTexture(line,1);
	break;
	
      case 61:
	// Open Door
	if (EV_DoDoor(line,opened))
	    P_ChangeSwitchTexture(line,1);
	break;
	
      case 62:
	// PlatDownWaitUpStay
	if (EV_DoPlat(line,downWaitUpStay,1))
	    P_ChangeSwitchTexture(line,1);
	break;
	
      case 63:
	// Raise Door
	if (EV_DoDoor(line,normal))
	    P_ChangeSwitchTexture(line,1);
	break;
	
      case 64:
	// Raise Floor to ceiling
	if (EV_DoFloor(line,raiseFloor))
	    P_ChangeSwitchTexture(line,1);
	break;
	
      case 66:
	// Raise Floor 24 and change texture
	if (EV_DoPlat(line,raiseAndChange,24))
	    P_ChangeSwitchTexture(line,1);
	break;
	
      case 67:
	// Raise Floor 32 and change texture
	if (EV_DoPlat(line,raiseAndChange,32))
	    P_ChangeSwitchTexture(line,1);
	break;
	
      case 65:
	// Raise Floor Crush
	if (EV_DoFloor(line,raiseFloorCrush))
	    P_ChangeSwitchTexture(line,1);
	break;
	
      case 68:
	// Raise Plat to next highest floor and change texture
	if (EV_DoPlat(line,raiseToNearestAndChange,0))
	    P_ChangeSwitchTexture(line,1);
	break;
	
      case 69:
	// Raise Floor to next highest floor
	if (EV_DoFloor(line, raiseFloorToNearest))
	    P_ChangeSwitchTexture(line,1);
	break;
	
      case 70:
	// Turbo Lower Floor
	if (EV_DoFloor(line,turboLower))
	    P_ChangeSwitchTexture(line,1);
	break;
	
      case 114:
	// Blazing Door Raise (faster than TURBO!)
	if (EV_DoDoor (line,blazeRaise))
	    P_ChangeSwitchTexture(line,1);

	//GK: A small hack in order to get the secret count on E4M7
	if (((::g->gamemission == doom && ::g->gameepisode == 4 && ::g->gamemap == 7)) && line->backsector->special == 9) {
		thing->player->secretcount++;
		//GK send message when secret found
		S_StartSound(thing->player->mo, sfx_getpow);
		::g->plyr->message = GOTSECRET;
		line->backsector->special = 0;
	}
	//GK: End
	break;
	
      case 115:
	// Blazing Door Open (faster than TURBO!)
	if (EV_DoDoor (line,blazeOpen))
	    P_ChangeSwitchTexture(line,1);
	break;
	
      case 116:
	// Blazing Door Close (faster than TURBO!)
	if (EV_DoDoor (line,blazeClose))
	    P_ChangeSwitchTexture(line,1);
	break;
	
      case 123:
	// Blazing PlatDownWaitUpStay
	if (EV_DoPlat(line,blazeDWUS,0))
	    P_ChangeSwitchTexture(line,1);
	break;
	
      case 132:
	// Raise Floor Turbo
	if (EV_DoFloor(line,raiseFloorTurbo))
	    P_ChangeSwitchTexture(line,1);
	break;
	
      case 99:
	// BlzOpenDoor BLUE
      case 134:
	// BlzOpenDoor RED
      case 136:
	// BlzOpenDoor YELLOW
	if (EV_DoLockedDoor (line,blazeOpen,thing))
	    P_ChangeSwitchTexture(line,1);
	break;
	
      case 138:
	// Light Turn On
	EV_LightTurnOn(line,255);
	P_ChangeSwitchTexture(line,1);
	break;
	
      case 139:
	// Light Turn Off
	EV_LightTurnOn(line,35);
	P_ChangeSwitchTexture(line,1);
	break;
			
    }
	
    return true;
}


