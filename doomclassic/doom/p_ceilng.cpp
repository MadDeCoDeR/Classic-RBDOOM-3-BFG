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


#include "z_zone.h"
#include "doomdef.h"
#include "p_local.h"

#include "s_sound.h"

// State.
#include "doomstat.h"
#include "r_state.h"

// Data.
#include "sounds.h"

//
// CEILINGS
//




//
// T_MoveCeiling
//

void T_MoveCeiling (ceiling_t* ceiling)
{
    result_e	res;
	
    switch(ceiling->direction)
    {
      case 0:
	// IN STASIS
	break;
      case 1:
	// UP
	res = T_MovePlane(ceiling->sector,
			  ceiling->speed,
			  ceiling->topheight,
			  false,1,ceiling->direction);
	
	if (!(::g->leveltime&7))
	{
	    switch(ceiling->type)
	    {
	      case silentCrushAndRaise:
		  case genSilentCrusher:
		break;
	      default:
		S_StartSound( &ceiling->sector->soundorg,
			     sfx_stnmov);
		// ?
		break;
	    }
	}
	
	if (res == pastdest)
	{
	    switch(ceiling->type)
	    {
	      case raiseToHighest:
		  case genCeiling:
		P_RemoveActiveCeiling(ceiling);
		break;

		// movers with texture change, change the texture then get removed
		  case genCeilingChgT:
		  case genCeilingChg0:
			  ceiling->sector->special = ceiling->newspecial;
			  //jff 3/14/98 transfer old special field as well
			  ceiling->sector->oldspecial = ceiling->oldspecial;
		  case genCeilingChg:
			  ceiling->sector->ceilingpic = ceiling->texture;
			  P_RemoveActiveCeiling(ceiling);
			  break;
		
	      case silentCrushAndRaise:
		S_StartSound( &ceiling->sector->soundorg,
			     sfx_pstop);
		  case genSilentCrusher:
		  case genCrusher:
	      case fastCrushAndRaise:
	      case crushAndRaise:
		ceiling->direction = -1;
		break;
		
	      default:
		break;
	    }
	    
	}
	break;
	
      case -1:
	// DOWN
	res = T_MovePlane(ceiling->sector,
			  ceiling->speed,
			  ceiling->bottomheight,
			  ceiling->crush,1,ceiling->direction);
	
	if (!(::g->leveltime&7))
	{
	    switch(ceiling->type)
	    {
	      case silentCrushAndRaise:
		  case genSilentCrusher:
			  break;
	      default:
		S_StartSound( &ceiling->sector->soundorg,
			     sfx_stnmov);
	    }
	}
	
	if (res == pastdest)
	{
	    switch(ceiling->type)
	    {
	      case silentCrushAndRaise:
		S_StartSound( &ceiling->sector->soundorg,
			     sfx_pstop);
	      case crushAndRaise:
		ceiling->speed = CEILSPEED;
	      case fastCrushAndRaise:
		ceiling->direction = 1;
		break;

		// 02/09/98 jff change slow crushers' speed back to normal
		// start back up
		  case genSilentCrusher:
		  case genCrusher:
			  if (ceiling->oldspeed<CEILSPEED * 3)
				  ceiling->speed = ceiling->oldspeed;
			  ceiling->direction = 1; //jff 2/22/98 make it go back up!
			  break;

			  // in the case of ceiling mover/changer, change the texture
			  // then remove the active ceiling
		  case genCeilingChgT:
		  case genCeilingChg0:
			  ceiling->sector->special = ceiling->newspecial;
			  //jff add to fix bug in special transfers from changes
			  ceiling->sector->oldspecial = ceiling->oldspecial;
		  case genCeilingChg:
			  ceiling->sector->ceilingpic = ceiling->texture;
			  P_RemoveActiveCeiling(ceiling);
			  break;
	      case lowerAndCrush:
	      case lowerToFloor:
		  case genCeiling:
			P_RemoveActiveCeiling(ceiling);
		break;

	      default:
		break;
	    }
	}
	else // ( res != pastdest )
	{
	    if (res == crushed)
	    {
		switch(ceiling->type)
		{
			//jff 02/08/98 slow down slow crushers on obstacle
		case genCrusher:
		case genSilentCrusher:
			if (ceiling->oldspeed < CEILSPEED * 3)
				ceiling->speed = CEILSPEED / 8;
			break;
		  case silentCrushAndRaise:
		  case crushAndRaise:
		  case lowerAndCrush:
		    ceiling->speed = CEILSPEED / 8;
		    break;

		  default:
		    break;
		}
	    }
	}
	break;
    }
}


//
// EV_DoCeiling
// Move a ceiling up/down and all around!
//
int
EV_DoCeiling
( line_t*	line,
  ceiling_e	type )
{
    int		secnum;
    int		rtn;
    sector_t*	sec;
    ceiling_t*	ceiling;
	
    secnum = -1;
    rtn = 0;
    
    //	Reactivate in-stasis ceilings...for certain types.
    switch(type)
    {
      case fastCrushAndRaise:
      case silentCrushAndRaise:
      case crushAndRaise:
	P_ActivateInStasisCeiling(line);
      default:
	break;
    }
	
    while ((secnum = P_FindSectorFromLineTag(line,secnum)) >= 0)
    {
	sec = &::g->sectors[secnum];
	if (P_SectorActive(ceiling_special, sec))
	    continue;
	
	// new door thinker
	rtn = 1;
	ceiling = (ceiling_t*)DoomLib::Z_Malloc(sizeof(*ceiling), PU_CEILING, 0);
	P_AddThinker (&ceiling->thinker);
	sec->ceilingdata = ceiling;
	ceiling->thinker.function = (actionf_p1)T_MoveCeiling;
	ceiling->sector = sec;
	ceiling->crush = false;
	
	switch(type)
	{
	  case fastCrushAndRaise:
	    ceiling->crush = true;
	    ceiling->topheight = sec->ceilingheight;
	    ceiling->bottomheight = sec->floorheight + (8*FRACUNIT);
	    ceiling->direction = -1;
	    ceiling->speed = CEILSPEED * 2;
	    break;

	  case silentCrushAndRaise:
	  case crushAndRaise:
	    ceiling->crush = true;
	    ceiling->topheight = sec->ceilingheight;
	  case lowerAndCrush:
	  case lowerToFloor:
	    ceiling->bottomheight = sec->floorheight;
	    if (type != lowerToFloor)
		ceiling->bottomheight += 8*FRACUNIT;
	    ceiling->direction = -1;
	    ceiling->speed = CEILSPEED;
	    break;

	  case raiseToHighest:
	    ceiling->topheight = P_FindHighestCeilingSurrounding(sec);
	    ceiling->direction = 1;
	    ceiling->speed = CEILSPEED;
	    break;
	}
		
	ceiling->tag = sec->tag;
	ceiling->type = type;
	P_AddActiveCeiling(ceiling);
    }
    return rtn;
}


//
// Add an active ceiling
//
void P_AddActiveCeiling(ceiling_t* c)
{
   //GK:From now on it uses indexed vectors (for now until and if I found something better)
	if (::g->cellind >= ::g->activeceilings.size()) {
		if (::g->activeceilings.size() == ::g->activeceilings.capacity()) {
			::g->activeceilings.reserve(::g->activeceilings.size() + MAXCEILINGS);
		}
		::g->activeceilings.emplace_back(c);
	}
	else {
		::g->activeceilings[::g->cellind] = c;
	}
	::g->cellind++;
}



//
// Remove a ceiling's thinker
//
void P_RemoveActiveCeiling(ceiling_t* c)
{
    uint		i;
	
    for (i = 0;i < ::g->cellind;i++)
    {
	if (::g->activeceilings[i] == c)
	{
	    ::g->activeceilings[i]->sector->ceilingdata = NULL;
	    P_RemoveThinker (&::g->activeceilings[i]->thinker);
	    ::g->activeceilings[i] = NULL;
		//::g->cellind = ::g->cellind - 1;
	    break;
	}
    }
}



//
// Restart a ceiling that's in-stasis
//
int P_ActivateInStasisCeiling(line_t* line)
{
    uint		i;
	int j = 0;
	
    for (i = 0;i < ::g->cellind;i++)
    {
	if (::g->activeceilings[i]
	    && (::g->activeceilings[i]->tag == line->tag)
	    && (::g->activeceilings[i]->direction == 0))
	{
	    ::g->activeceilings[i]->direction = ::g->activeceilings[i]->olddirection;
	    ::g->activeceilings[i]->thinker.function
	      = (actionf_p1)T_MoveCeiling;
		j = 1;
	}
    }
	return j;
}



//
// EV_CeilingCrushStop
// Stop a ceiling from crushing!
//
int	EV_CeilingCrushStop(line_t	*line)
{
    uint		i;
    int		rtn;
	
    rtn = 0;
    for (i = 0;i < ::g->cellind;i++)
    {
	if (::g->activeceilings[i]
	    && (::g->activeceilings[i]->tag == line->tag)
	    && (::g->activeceilings[i]->direction != 0))
	{
	    ::g->activeceilings[i]->olddirection = ::g->activeceilings[i]->direction;
	    ::g->activeceilings[i]->thinker.function = (actionf_v)NULL;
	    ::g->activeceilings[i]->direction = 0;		// in-stasis
	    rtn = 1;
	}
    }
    

    return rtn;
}

