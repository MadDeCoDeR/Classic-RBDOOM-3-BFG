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
// FLOORS
//

//
// Move a plane (floor or ceiling) and check for crushing
//
result_e
T_MovePlane
( sector_t*	sector,
  fixed_t	speed,
  fixed_t	dest,
  qboolean	crush,
  int		floorOrCeiling,
  int		direction )
{
    qboolean	flag;
    fixed_t	lastpos;
	
    switch(floorOrCeiling)
    {
      case 0:
	// FLOOR
	switch(direction)
	{
	  case -1:
	    // DOWN
	    if (sector->floorheight - speed < dest)
	    {
		lastpos = sector->floorheight;
		sector->floorheight = dest;
		flag = P_ChangeSector(sector,crush);
		if (flag == true)
		{
		    sector->floorheight =lastpos;
		    P_ChangeSector(sector,crush);
		    //return crushed;
		}
		return pastdest;
	    }
	    else
	    {
		lastpos = sector->floorheight;
		sector->floorheight -= speed;
		flag = P_ChangeSector(sector,crush);
		if (flag == true)
		{
		    sector->floorheight = lastpos;
		    P_ChangeSector(sector,crush);
		    return crushed;
		}
	    }
	    break;
						
	  case 1:
	    // UP
	    if (sector->floorheight + speed > dest)
	    {
		lastpos = sector->floorheight;
		sector->floorheight = dest;
		flag = P_ChangeSector(sector,crush);
		if (flag == true)
		{
		    sector->floorheight = lastpos;
		    P_ChangeSector(sector,crush);
		    //return crushed;
		}
		return pastdest;
	    }
	    else
	    {
		// COULD GET CRUSHED
		lastpos = sector->floorheight;
		sector->floorheight += speed;
		flag = P_ChangeSector(sector,crush);
		if (flag == true)
		{
		    if (crush == true)
			return crushed;
		    sector->floorheight = lastpos;
		    P_ChangeSector(sector,crush);
		    return crushed;
		}
	    }
	    break;
	}
	break;
									
      case 1:
	// CEILING
	switch(direction)
	{
	  case -1:
	    // DOWN
	    if (sector->ceilingheight - speed < dest)
	    {
		lastpos = sector->ceilingheight;
		sector->ceilingheight = dest;
		flag = P_ChangeSector(sector,crush);

		if (flag == true)
		{
		    sector->ceilingheight = lastpos;
		    P_ChangeSector(sector,crush);
		    //return crushed;
		}
		return pastdest;
	    }
	    else
	    {
		// COULD GET CRUSHED
		lastpos = sector->ceilingheight;
		sector->ceilingheight -= speed;
		flag = P_ChangeSector(sector,crush);

		if (flag == true)
		{
		    if (crush == true)
			return crushed;
		    sector->ceilingheight = lastpos;
		    P_ChangeSector(sector,crush);
		    return crushed;
		}
	    }
	    break;
						
	  case 1:
	    // UP
	    if (sector->ceilingheight + speed > dest)
	    {
		lastpos = sector->ceilingheight;
		sector->ceilingheight = dest;
		flag = P_ChangeSector(sector,crush);
		if (flag == true)
		{
		    sector->ceilingheight = lastpos;
		    P_ChangeSector(sector,crush);
		    //return crushed;
		}
		return pastdest;
	    }
	    else
	    {
		lastpos = sector->ceilingheight;
		sector->ceilingheight += speed;
		flag = P_ChangeSector(sector,crush);
// UNUSED
#if 0
		if (flag == true)
		{
		    sector->ceilingheight = lastpos;
		    P_ChangeSector(sector,crush);
		    return crushed;
		}
#endif
	    }
	    break;
	}
	break;
		
    }
    return ok;
}


//
// MOVE A FLOOR TO IT'S DESTINATION (UP OR DOWN)
//
void T_MoveFloor(floormove_t* floor)
{
    result_e	res;
	
    res = T_MovePlane(floor->sector,
		      floor->speed,
		      floor->floordestheight,
		      floor->crush,0,floor->direction);
    
    if (!(::g->leveltime&7))
	S_StartSound( &floor->sector->soundorg,
		     sfx_stnmov);
    
    if (res == pastdest)
    {
	floor->sector->floordata = NULL;

	if (floor->direction == 1)
	{
	    switch(floor->type)
	    {
	      case donutRaise:
		floor->sector->special = floor->newspecial;
		floor->sector->floorpic = floor->texture;
		  case genFloorChgT:
		  case genFloorChg0:
			  floor->sector->special = floor->newspecial;
			  //jff add to fix bug in special transfers from changes
			  floor->sector->oldspecial = floor->oldspecial;
			  //fall thru
		  case genFloorChg:
			  floor->sector->floorpic = floor->texture;
			  break;
	      default:
		break;
	    }
	}
	else if (floor->direction == -1)
	{
	    switch(floor->type)
	    {
	      case lowerAndChange:
		floor->sector->special = floor->newspecial;
		//jff add to fix bug in special transfers from changes
		floor->sector->oldspecial = floor->oldspecial;
		floor->sector->floorpic = floor->texture;
		  case genFloorChgT:
		  case genFloorChg0:
			  floor->sector->special = floor->newspecial;
			  //jff add to fix bug in special transfers from changes
			  floor->sector->oldspecial = floor->oldspecial;
			  //fall thru
		  case genFloorChg:
			  floor->sector->floorpic = floor->texture;
			  break;
	      default:
		break;
	    }
	}
	P_RemoveThinker(&floor->thinker);

	S_StartSound( &floor->sector->soundorg,
		     sfx_pstop);
    }

}

//
// HANDLE FLOOR TYPES
//
int
EV_DoFloor
( line_t*	line,
  floor_e	floortype )
{
    int			secnum;
    int			rtn;
    int			i;
    sector_t*		sec;
    floormove_t*	floor;

    secnum = -1;
    rtn = 0;
    while ((secnum = P_FindSectorFromLineTag(line,secnum)) >= 0)
    {
	sec = &::g->sectors[secnum];
		
	// ALREADY MOVING?  IF SO, KEEP GOING...
	if (P_SectorActive(floor_special,sec))
	    continue;
	
	// new floor thinker
	rtn = 1;
	floor = (floormove_t*)DoomLib::Z_Malloc(sizeof(*floor), PU_FLOOR, 0);
	P_AddThinker (&floor->thinker);
	sec->floordata = floor;
	floor->thinker.function = (actionf_p1) T_MoveFloor;
	floor->type = floortype;
	floor->crush = false;

	switch(floortype)
	{
	  case lowerFloor:
	    floor->direction = -1;
	    floor->sector = sec;
	    floor->speed = FLOORSPEED;
	    floor->floordestheight = 
		P_FindHighestFloorSurrounding(sec);
	    break;

	  case lowerFloorToLowest:
	    floor->direction = -1;
	    floor->sector = sec;
	    floor->speed = FLOORSPEED;
	    floor->floordestheight = 
		P_FindLowestFloorSurrounding(sec);
	    break;

	  case turboLower:
	    floor->direction = -1;
	    floor->sector = sec;
	    floor->speed = FLOORSPEED * 4;
	    floor->floordestheight = 
		P_FindHighestFloorSurrounding(sec);
	    if (floor->floordestheight != sec->floorheight)
		floor->floordestheight += 8*FRACUNIT;
	    break;

	  case raiseFloorCrush:
	    floor->crush = true;
	  case raiseFloor:
	    floor->direction = 1;
	    floor->sector = sec;
	    floor->speed = FLOORSPEED;
	    floor->floordestheight = 
		P_FindLowestCeilingSurrounding(sec);
	    if (floor->floordestheight > sec->ceilingheight)
		floor->floordestheight = sec->ceilingheight;
	    floor->floordestheight -= (8*FRACUNIT)*
		(floortype == raiseFloorCrush);
	    break;

	  case raiseFloorTurbo:
	    floor->direction = 1;
	    floor->sector = sec;
	    floor->speed = FLOORSPEED*4;
	    floor->floordestheight = 
		P_FindNextHighestFloor(sec,sec->floorheight);
	    break;

	  case raiseFloorToNearest:
	    floor->direction = 1;
	    floor->sector = sec;
	    floor->speed = FLOORSPEED;
	    floor->floordestheight = 
		P_FindNextHighestFloor(sec,sec->floorheight);
	    break;

	 case lowerFloorToNearest:
		  floor->direction = -1;
		  floor->sector = sec;
		  floor->speed = FLOORSPEED;
		  floor->floordestheight =
			  P_FindNextLowestFloor(sec, sec->floorheight);
		  break;

	  case raiseFloor24:
	    floor->direction = 1;
	    floor->sector = sec;
	    floor->speed = FLOORSPEED;
	    floor->floordestheight = floor->sector->floorheight +
		24 * FRACUNIT;
	    break;
	  case raiseFloor512:
	    floor->direction = 1;
	    floor->sector = sec;
	    floor->speed = FLOORSPEED;
	    floor->floordestheight = floor->sector->floorheight +
		512 * FRACUNIT;
	    break;

	  case raiseFloor24AndChange:
	    floor->direction = 1;
	    floor->sector = sec;
	    floor->speed = FLOORSPEED;
	    floor->floordestheight = floor->sector->floorheight +
		24 * FRACUNIT;
	    sec->floorpic = line->frontsector->floorpic;
	    sec->special = line->frontsector->special;
	    break;

	  case raiseToTexture:
	  {
	      int	minsize = MAXINT;
	      side_t*	side;
				
	      floor->direction = 1;
	      floor->sector = sec;
	      floor->speed = FLOORSPEED;
	      for (i = 0; i < sec->linecount; i++)
	      {
		  if (twoSided (secnum, i) )
		  {
		      side = getSide(secnum,i,0);
		      if (side->bottomtexture >= 0)
			  if (::g->s_textureheight[side->bottomtexture] < 
			      minsize)
			      minsize = 
				  ::g->s_textureheight[side->bottomtexture];
		      side = getSide(secnum,i,1);
		      if (side->bottomtexture >= 0)
			  if (::g->s_textureheight[side->bottomtexture] < 
			      minsize)
			      minsize = 
				 ::g->s_textureheight[side->bottomtexture];
		  }
	      }
	      floor->floordestheight =
		  floor->sector->floorheight + minsize;
	  }
	  break;
	  
	  case lowerAndChange:
	    floor->direction = -1;
	    floor->sector = sec;
	    floor->speed = FLOORSPEED;
	    floor->floordestheight = 
		P_FindLowestFloorSurrounding(sec);
	    floor->texture = sec->floorpic;

	    for (i = 0; i < sec->linecount; i++)
	    {
		if ( twoSided(secnum, i) )
		{
		    if (getSide(secnum,i,0)->sector-::g->sectors == secnum)
		    {
			sec = getSector(secnum,i,1);

			if (sec->floorheight == floor->floordestheight)
			{
			    floor->texture = sec->floorpic;
			    floor->newspecial = sec->special;
			    break;
			}
		    }
		    else
		    {
			sec = getSector(secnum,i,0);

			if (sec->floorheight == floor->floordestheight)
			{
			    floor->texture = sec->floorpic;
			    floor->newspecial = sec->special;
			    break;
			}
		    }
		}
	    }
	  default:
	    break;
	}
    }
    return rtn;
}




//
// BUILD A STAIRCASE!
//
int
EV_BuildStairs
( line_t*	line,
  stair_e	type )
{
    int			secnum;
    int			height;
    int			i;
    int			newsecnum;
    int			texture;
    int			ok;
    int			rtn;
    
    sector_t*		sec;
    sector_t*		tsec;

    floormove_t*	floor;
    
    fixed_t		stairsize = 0;
    fixed_t		speed = 0;

    secnum = -1;
    rtn = 0;
    while ((secnum = P_FindSectorFromLineTag(line,secnum)) >= 0)
    {
	sec = &::g->sectors[secnum];
		
	// ALREADY MOVING?  IF SO, KEEP GOING...
	if (P_SectorActive(floor_special,sec))
	    continue;
	
	// new floor thinker
	rtn = 1;
	floor = (floormove_t*)DoomLib::Z_Malloc(sizeof(*floor), PU_FLOOR, 0);
	P_AddThinker (&floor->thinker);
	sec->floordata = floor;
	floor->thinker.function = (actionf_p1) T_MoveFloor;
	floor->direction = 1;
	floor->sector = sec;
	switch(type)
	{
	  case build8:
	    speed = FLOORSPEED/4;
	    stairsize = 8*FRACUNIT;
	    break;
	  case turbo16:
	    speed = FLOORSPEED*4;
	    stairsize = 16*FRACUNIT;
	    break;
	}
	floor->speed = speed;
	height = sec->floorheight + stairsize;
	floor->floordestheight = height;
		
	texture = sec->floorpic;
	
	// Find next sector to raise
	// 1.	Find 2-sided line with same sector side[0]
	// 2.	Other side is the next sector to raise
	do
	{
	    ok = 0;
	    for (i = 0;i < sec->linecount;i++)
	    {
		if ( !((sec->lines[i])->flags & ML_TWOSIDED) )
		    continue;
					
		tsec = (sec->lines[i])->frontsector;
		newsecnum = tsec-::g->sectors;
		
		if (secnum != newsecnum)
		    continue;

		tsec = (sec->lines[i])->backsector;
		newsecnum = tsec - ::g->sectors;

		if (tsec->floorpic != texture)
		    continue;
					
		height += stairsize;

		if (P_SectorActive(floor_special,tsec))
		    continue;
					
		sec = tsec;
		secnum = newsecnum;
		floor = (floormove_t*)DoomLib::Z_Malloc(sizeof(*floor), PU_FLOOR, 0);

		P_AddThinker (&floor->thinker);

		sec->floordata = floor;
		floor->thinker.function = (actionf_p1) T_MoveFloor;
		floor->direction = 1;
		floor->sector = sec;
		floor->speed = speed;
		floor->floordestheight = height;
		ok = 1;
		break;
	    }
	} while(ok);
    }
    return rtn;
}


//
// EV_DoChange()
//
// Handle pure change types. These change floor texture and sector type
// by trigger or numeric model without moving the floor.
//
// The linedef causing the change and the type of change is passed
// Returns true if any sector changes
//
// jff 3/15/98 added to better support generalized sector types
//
int EV_DoChange
(line_t*       line,
	change_e      changetype)
{
	int                   secnum;
	int                   rtn;
	sector_t*             sec;
	sector_t*             secm;

	secnum = -1;
	rtn = 0;
	// change all sectors with the same tag as the linedef
	while ((secnum = P_FindSectorFromLineTag(line, secnum)) >= 0)
	{
		sec = &::g->sectors[secnum];

		rtn = 1;

		// handle trigger or numeric change type
		switch (changetype)
		{
		case trigChangeOnly:
			sec->floorpic = line->frontsector->floorpic;
			sec->special = line->frontsector->special;
			sec->oldspecial = line->frontsector->oldspecial;
			break;
		case numChangeOnly:
			secm = P_FindModelFloorSector(sec->floorheight, secnum);
			if (secm) // if no model, no change
			{
				sec->floorpic = secm->floorpic;
				sec->special = secm->special;
				sec->oldspecial = secm->oldspecial;
			}
			break;
		default:
			break;
		}
	}
	return rtn;
}

//
// EV_DoElevator
//
// Handle elevator linedef types
//
// Passed the linedef that triggered the elevator and the elevator action
//
// jff 2/22/98 new type to move floor and ceiling in parallel
//
int EV_DoElevator
(line_t*       line,
	elevator_e    elevtype)
{
	int                   secnum;
	int                   rtn;
	sector_t*             sec;
	elevator_t*           elevator;

	secnum = -1;
	rtn = 0;
	// act on all sectors with the same tag as the triggering linedef
	while ((secnum = P_FindSectorFromLineTag(line, secnum)) >= 0)
	{
		sec = &::g->sectors[secnum];

		// If either floor or ceiling is already activated, skip it
		if (sec->floordata || sec->ceilingdata) //jff 2/22/98
			continue;

		// create and initialize new elevator thinker
		rtn = 1;
		elevator =(elevator_t*) DoomLib::Z_Malloc(sizeof(*elevator), PU_LEVSPEC, 0);
		P_AddThinker(&elevator->thinker);
		sec->floordata = elevator; //jff 2/22/98
		sec->ceilingdata = elevator; //jff 2/22/98
		elevator->thinker.function = (actionf_p1)T_MoveElevator;
		elevator->type = elevtype;

		// set up the fields according to the type of elevator action
		switch (elevtype)
		{
			// elevator down to next floor
		case elevateDown:
			elevator->direction = -1;
			elevator->sector = sec;
			elevator->speed = ELEVATORSPEED;
			elevator->floordestheight =
				P_FindNextLowestFloor(sec, sec->floorheight);
			elevator->ceilingdestheight =
				elevator->floordestheight + sec->ceilingheight - sec->floorheight;
			break;

			// elevator up to next floor
		case elevateUp:
			elevator->direction = 1;
			elevator->sector = sec;
			elevator->speed = ELEVATORSPEED;
			elevator->floordestheight =
				P_FindNextHighestFloor(sec, sec->floorheight);
			elevator->ceilingdestheight =
				elevator->floordestheight + sec->ceilingheight - sec->floorheight;
			break;

			// elevator to floor height of activating switch's front sector
		case elevateCurrent:
			elevator->sector = sec;
			elevator->speed = ELEVATORSPEED;
			elevator->floordestheight = line->frontsector->floorheight;
			elevator->ceilingdestheight =
				elevator->floordestheight + sec->ceilingheight - sec->floorheight;
			elevator->direction =
				elevator->floordestheight>sec->floorheight ? 1 : -1;
			break;

		default:
			break;
		}
	}
	return rtn;
}

//
// T_MoveElevator()
//
// Move an elevator to it's destination (up or down)
// Called once per tick for each moving floor.
//
// Passed an elevator_t structure that contains all pertinent info about the
// move. See P_SPEC.H for fields.
// No return.
//
// jff 02/22/98 added to support parallel floor/ceiling motion
//
void T_MoveElevator(elevator_t* elevator)
{
	result_e      res;

	if (elevator->direction<0)      // moving down
	{
		res = T_MovePlane             //jff 4/7/98 reverse order of ceiling/floor
		(
			elevator->sector,
			elevator->speed,
			elevator->ceilingdestheight,
			0,
			1,                          // move floor
			elevator->direction
		);
		if (res == ok || res == pastdest) // jff 4/7/98 don't move ceil if blocked
			T_MovePlane
			(
				elevator->sector,
				elevator->speed,
				elevator->floordestheight,
				0,
				0,                        // move ceiling
				elevator->direction
			);
	}
	else // up
	{
		res = T_MovePlane             //jff 4/7/98 reverse order of ceiling/floor
		(
			elevator->sector,
			elevator->speed,
			elevator->floordestheight,
			0,
			0,                          // move ceiling
			elevator->direction
		);
		if (res == ok || res == pastdest) // jff 4/7/98 don't move floor if blocked
			T_MovePlane
			(
				elevator->sector,
				elevator->speed,
				elevator->ceilingdestheight,
				0,
				1,                        // move floor
				elevator->direction
			);
	}

	// make floor move sound
	if (!(::g->leveltime & 7))
		S_StartSound((mobj_t *)&elevator->sector->soundorg, sfx_stnmov);

	if (res == pastdest)            // if destination height acheived
	{
		elevator->sector->floordata = NULL;     //jff 2/22/98
		elevator->sector->ceilingdata = NULL;   //jff 2/22/98
		P_RemoveThinker(&elevator->thinker);    // remove elevator from actives

												// make floor stop sound
		S_StartSound((mobj_t *)&elevator->sector->soundorg, sfx_pstop);
	}
}