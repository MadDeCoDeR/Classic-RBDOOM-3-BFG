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

#include <stdlib.h>

#include "m_random.h"
#include "i_system.h"

#include "doomdef.h"
#include "p_local.h"

#include "s_sound.h"

#include "g_game.h"

// State.
#include "doomstat.h"
#include "r_state.h"

// Data.
#include "sounds.h"
#include "d_exp.h"

extern bool globalNetworking;



//
// P_NewChaseDir related LUT.
//
const dirtype_t opposite[] =
{
  DI_WEST, DI_SOUTHWEST, DI_SOUTH, DI_SOUTHEAST,
  DI_EAST, DI_NORTHEAST, DI_NORTH, DI_NORTHWEST, DI_NODIR_CL
};

const dirtype_t diags[] =
{
    DI_NORTHWEST, DI_NORTHEAST, DI_SOUTHWEST, DI_SOUTHEAST
};


extern "C" void A_Fall (mobj_t *actor);


//
// ENEMY THINKING
// Enemies are allways spawned
// with targetplayer = -1, threshold = 0
// Most monsters are spawned unaware of all ::g->players,
// but some can be made preaware
//


//
// Called by P_NoiseAlert.
// Recursively traverse adjacent ::g->sectors,
// sound blocking ::g->lines cut off traversal.
//


void
P_RecursiveSound
( sector_t*	sec,
  int		soundblocks )
{
    int		i;
    line_t*	check;
    sector_t*	other;
	
    // wake up all monsters in this sector
    if (sec->validcount == ::g->validcount
	&& sec->soundtraversed <= soundblocks+1)
    {
	return;		// already flooded
    }
    
    sec->validcount = ::g->validcount;
    sec->soundtraversed = soundblocks+1;
    sec->soundtarget = ::g->soundtarget;
	
    for (i=0 ;i<sec->linecount ; i++)
    {
	check = sec->lines[i];
	if (! (check->flags & ML_TWOSIDED) )
	    continue;
	
	P_LineOpening (check);

	if (::g->openrange <= 0)
	    continue;	// closed door
	
	if ( ::g->sides[ check->sidenum[0] ].sector == sec)
	    other = ::g->sides[ check->sidenum[1] ] .sector;
	else
	    other = ::g->sides[ check->sidenum[0] ].sector;
	
	if (check->flags & ML_SOUNDBLOCK)
	{
	    if (!soundblocks)
		P_RecursiveSound (other, 1);
	}
	else
	    P_RecursiveSound (other, soundblocks);
    }
}



//
// P_NoiseAlert
// If a monster yells at a player,
// it will alert other monsters to the player.
//
void
P_NoiseAlert
( mobj_t*	target,
  mobj_t*	emmiter )
{
    ::g->soundtarget = target;
    ::g->validcount++;
    P_RecursiveSound (emmiter->subsector->sector, 0);
}




//
// P_CheckMeleeRange
//
qboolean P_CheckMeleeRange (mobj_t*	actor, int meleerange = -1)
{
    mobj_t*	pl;
    fixed_t	dist;
	if (meleerange < 0) {
		meleerange = actor->info->meleeRange;
	}
	
    if (!actor->target)
	return false;
		
    pl = actor->target;
    dist = P_AproxDistance (pl->x-actor->x, pl->y-actor->y);

    if (dist >= meleerange-20*FRACUNIT+pl->info->radius)
	return false;
	
    if (! P_CheckSight (actor, actor->target) )
	return false;
							
    return true;		
}

//
// P_CheckMissileRange
//
qboolean P_CheckMissileRange (mobj_t* actor)
{
    fixed_t	dist;
	
    if (! P_CheckSight (actor, actor->target) )
	return false;
	
    if ( actor->flags & MF_JUSTHIT )
    {
	// the target just hit the enemy,
	// so fight back!
	actor->flags &= ~MF_JUSTHIT;
	return true;
    }
	
    if (actor->reactiontime)
	return false;	// do not attack yet
		
    // OPTIMIZE: get this from a global checksight
    dist = P_AproxDistance ( actor->x-actor->target->x,
			     actor->y-actor->target->y) - 64*FRACUNIT;
    
    if (!actor->info->meleestate)
	dist -= 128*FRACUNIT;	// no melee attack, so fire more

    dist >>= 16;

    if (actor->flags2 & MF2_SHORTMRANGE)
    {
	if (dist > 14*64)	
	    return false;	// too far away
    }
	

    if (actor->flags2 & MF2_LONGMELEE)
    {
	if (dist < 196)	
	    return false;	// close for fist attack
    }
	

    if (actor->flags2 & MF2_RANGEHALF)
    {
	dist >>= 1;
    }
    
    if (dist > 200)
	dist = 200;
		
    if (actor->flags2 & MF2_HIGHERMPROB && dist > 160)
	dist = 160;
		
    if (P_Random () < dist)
	return false;
		
    return true;
}


//
// P_Move
// Move in the current direction,
// returns false if the move is blocked.
//
const fixed_t	xspeed[8] = {FRACUNIT,47000,0,-47000,-FRACUNIT,-47000,0,47000};
const fixed_t yspeed[8] = {0,47000,FRACUNIT,47000,0,-47000,-FRACUNIT,-47000};



qboolean P_Move (mobj_t*	actor)
{
    fixed_t	tryx;
    fixed_t	tryy;
    
   // line_t*	ld;
    
    // warning: 'catch', 'throw', and 'try'
    // are all C++ reserved words
    qboolean	try_ok;
    qboolean	good;
		
    if (actor->movedir == DI_NODIR_CL)
	return false;
		
    if ((unsigned)actor->movedir >= 8)
	I_Error ("Weird actor->movedir!");
		
    tryx = actor->x + actor->info->speed*xspeed[actor->movedir];
    tryy = actor->y + actor->info->speed*yspeed[actor->movedir];

    try_ok = P_TryMove (actor, tryx, tryy);

    if (!try_ok)
    {
	// open any specials
	if (actor->flags & MF_FLOAT && ::g->floatok)
	{
	    // must adjust height
	    if (actor->z < ::g->tmfloorz)
		actor->z += FLOATSPEED;
	    else
		actor->z -= FLOATSPEED;

	    actor->flags |= MF_INFLOAT;
	    return true;
	}
		
	if (!::g->numspechit)
	    return false;
			
	actor->movedir = DI_NODIR_CL;
	good = false;
	while (::g->numspechit--)
	{
	    // if the special is not a door
	    // that can be opened,
	    // return false
	    if (P_UseSpecialLine (actor, ::g->spechit[::g->numspechit],0))
		good = true;
	}
	return good;
    }
    else
    {
	actor->flags &= ~MF_INFLOAT;
    }
	
	
    if (! (actor->flags & MF_FLOAT) )	
	actor->z = actor->floorz;
    return true; 
}


//
// TryWalk
// Attempts to move actor on
// in its current (ob->moveangle) direction.
// If blocked by either a wall or an actor
// returns FALSE
// If move is either clear or blocked only by a door,
// returns TRUE and sets...
// If a door is in the way,
// an OpenDoor call is made to start it opening.
//
qboolean P_TryWalk (mobj_t* actor)
{	
    if (!P_Move (actor))
    {
	return false;
    }

    actor->movecount = P_Random()&15;
    return true;
}




void P_NewChaseDir (mobj_t*	actor)
{
    fixed_t	deltax;
    fixed_t	deltay;
    
    dirtype_t	d[3];
    
    int		tdir;
    dirtype_t	olddir;
    
    dirtype_t	turnaround;

    if (!actor->target)
	I_Error ("P_NewChaseDir: called with no target");
		
    olddir = (dirtype_t)actor->movedir;
    turnaround=opposite[olddir];

    deltax = actor->target->x - actor->x;
    deltay = actor->target->y - actor->y;

    if (deltax>10*FRACUNIT)
	d[1]= DI_EAST;
    else if (deltax<-10*FRACUNIT)
	d[1]= DI_WEST;
    else
	d[1]=DI_NODIR_CL;

    if (deltay<-10*FRACUNIT)
	d[2]= DI_SOUTH;
    else if (deltay>10*FRACUNIT)
	d[2]= DI_NORTH;
    else
	d[2]=DI_NODIR_CL;

    // try direct route
    if (d[1] != DI_NODIR_CL
	&& d[2] != DI_NODIR_CL)
    {
	actor->movedir = diags[((deltay<0)<<1)+(deltax>0)];
	if (actor->movedir != turnaround && P_TryWalk(actor))
	    return;
    }

    // try other directions
    if (P_Random() > 200
	||  abs(deltay)>abs(deltax))
    {
	tdir=d[1];
	d[1]=d[2];
	d[2]=(dirtype_t)tdir;
    }

    if (d[1]==turnaround)
	d[1]=DI_NODIR_CL;
    if (d[2]==turnaround)
	d[2]=DI_NODIR_CL;
	
    if (d[1]!=DI_NODIR_CL)
    {
	actor->movedir = d[1];
	if (P_TryWalk(actor))
	{
	    // either moved forward or attacked
	    return;
	}
    }

    if (d[2]!=DI_NODIR_CL)
    {
	actor->movedir =d[2];

	if (P_TryWalk(actor))
	    return;
    }

    // there is no direct path to the player,
    // so pick another direction.
    if (olddir!=DI_NODIR_CL)
    {
	actor->movedir =olddir;

	if (P_TryWalk(actor))
	    return;
    }

    // randomly determine direction of search
    if (P_Random()&1) 	
    {
	for ( tdir=DI_EAST;
	      tdir<=DI_SOUTHEAST;
	      tdir++ )
	{
	    if (tdir!=turnaround)
	    {
		actor->movedir =tdir;
		
		if ( P_TryWalk(actor) )
		    return;
	    }
	}
    }
    else
    {
	for ( tdir=DI_SOUTHEAST;
	      tdir != (DI_EAST-1);
	      tdir-- )
	{
	    if (tdir!=turnaround)
	    {
		actor->movedir =tdir;
		
		if ( P_TryWalk(actor) )
		    return;
	    }
	}
    }

    if (turnaround !=  DI_NODIR_CL)
    {
	actor->movedir =turnaround;
	if ( P_TryWalk(actor) )
	    return;
    }

    actor->movedir = DI_NODIR_CL;	// can not move
}



//
// P_LookForPlayers
// If allaround is false, only look 180 degrees in front.
// Returns true if a player is targeted.
//
qboolean
P_LookForPlayers
( mobj_t*	actor,
  qboolean	allaround )
{
    int		c;
    int		stop;
    player_t*	player;
    sector_t*	sector;
    angle_t	an;
    fixed_t	dist;
		
    sector = actor->subsector->sector;
	
    c = 0;
    stop = (actor->lastlook-1)&3;
	
    for ( ; ; actor->lastlook = (actor->lastlook+1)&3 )
    {
	if (!::g->playeringame[actor->lastlook])
	    continue;
			
	if (c++ == 2
	    || actor->lastlook == stop)
	{
	    // done looking
	    return false;	
	}
	
	player = &::g->players[actor->lastlook];

	if (player->health <= 0)
	    continue;		// dead

	if (!P_CheckSight (actor, player->mo))
	    continue;		// out of sight
			
	if (!allaround)
	{
	    an = R_PointToAngle2 (actor->x,
				  actor->y, 
				  player->mo->x,
				  player->mo->y)
		- actor->angle;
	    
	    if (an > ANG90 && an < ANG270)
	    {
		dist = P_AproxDistance (player->mo->x - actor->x,
					player->mo->y - actor->y);
		// if real close, react anyway
		if (dist > MELEERANGE)
		    continue;	// behind back
	    }
	}
	if (actor->target == NULL) {
		actor->originalTarget = player->mo;
	}
	actor->target = player->mo;
	return true;
    }
#ifndef _MSC_VER
    return false;
#endif
}


//----------------------------------------------------------------------------
//
// FUNC P_FaceMobj
//
// Returns 1 if 'source' needs to turn clockwise, or 0 if 'source' needs
// to turn counter clockwise.  'delta' is set to the amount 'source'
// needs to turn.
//
//----------------------------------------------------------------------------

int P_FaceMobj(mobj_t *source, mobj_t *target, angle_t *delta)
{
	angle_t diff;
	angle_t angle1;
	angle_t angle2;

	angle1 = source->angle;
	angle2 = R_PointToAngle2(source->x, source->y, target->x, target->y);
	if(angle2 > angle1)
	{
		diff = angle2-angle1;
		if(diff > ANG180)
		{
			*delta = ANGMAX-diff;
			return(0);
		}
		else
		{
			*delta = diff;
			return(1);
		}
	}
	else
	{
		diff = angle1-angle2;
		if(diff > ANG180)
		{
			*delta = ANGMAX-diff;
			return(1);
		}
		else
		{
			*delta = diff;
			return(0);
		}
	}
}

//
// P_CheckFov
// Returns true if t2 is within t1's field of view.
// Not directly related to P_CheckSight, but often
// used in tandem.
//
// Adapted from Eternity, so big thanks to Quasar
//
qboolean P_CheckFov(mobj_t *t1, mobj_t *t2, angle_t fov)
{
  angle_t angle, minang, maxang;

  angle = R_PointToAngle2(t1->x, t1->y, t2->x, t2->y);
  minang = t1->angle - fov / 2;
  maxang = t1->angle + fov / 2;

  return((minang > maxang) ? angle >= minang || angle <= maxang
                           : angle >= minang && angle <= maxang);
}

static mobj_t *RoughBlockCheck(mobj_t *mo, int index, angle_t fov)
{
  mobj_t *link;

  link = ::g->blocklinks[index];
  while (link)
  {
    // skip non-shootable actors
    if (!(link->flags & MF_SHOOTABLE))
    {
      link = link->bnext;
      continue;
    }

    // // skip dormant actors
    // if (link->flags2 & MF2_DORMANT)
    // {
    //     link = link->bnext;
    //     continue;
    // }

    // skip the projectile's owner
    if (link == mo->target)
    {
      link = link->bnext;
      continue;
    }

    // skip actors on the same "team", unless infighting or deathmatching
    if (mo->target &&
      !((link->flags ^ mo->target->flags) & MF_FRIEND) &&
      mo->target->target != link &&
      !(::g->deathmatch && link->player && mo->target->player))
    {
      link = link->bnext;
      continue;
    }

    // skip actors outside of specified FOV
    if (fov > 0 && !P_CheckFov(mo, link, fov))
    {
      link = link->bnext;
      continue;
    }

    // skip actors not in line of sight
    if (!P_CheckSight(mo, link))
    {
      link = link->bnext;
      continue;
    }

    // all good! return it.
    return link;
  }

  // couldn't find a valid target
  return NULL;
}

//
// P_RoughTargetSearch
// Searches though the surrounding mapblocks for monsters/players
// based on Hexen's P_RoughMonsterSearch
//
// distance is in MAPBLOCKUNITS

mobj_t *P_RoughTargetSearch(mobj_t *mo, angle_t fov, int distance)
{
  int blockX;
  int blockY;
  int startX, startY;
  int blockIndex;
  int firstStop;
  int secondStop;
  int thirdStop;
  int finalStop;
  int count;
  mobj_t *target;

  startX = (mo->x - ::g->bmaporgx) >> MAPBLOCKSHIFT;
  startY = (mo->y - ::g->bmaporgy) >> MAPBLOCKSHIFT;

  if (startX >= 0 && startX < ::g->bmapwidth && startY >= 0 && startY < ::g->bmapheight)
  {
    if ((target = RoughBlockCheck(mo, startY*::g->bmapwidth + startX, fov)))
    { // found a target right away
      return target;
    }
  }
  for (count = 1; count <= distance; count++)
  {
    blockX = startX - count;
    blockY = startY - count;

    if (blockY < 0)
    {
      blockY = 0;
    }
    else if (blockY >= ::g->bmapheight)
    {
      blockY = ::g->bmapheight - 1;
    }
    if (blockX < 0)
    {
      blockX = 0;
    }
    else if (blockX >= ::g->bmapwidth)
    {
      blockX = ::g->bmapwidth - 1;
    }
    blockIndex = blockY * ::g->bmapwidth + blockX;
    firstStop = startX + count;
    if (firstStop < 0)
    {
      continue;
    }
    if (firstStop >= ::g->bmapwidth)
    {
      firstStop = ::g->bmapwidth - 1;
    }
    secondStop = startY + count;
    if (secondStop < 0)
    {
      continue;
    }
    if (secondStop >= ::g->bmapheight)
    {
      secondStop = ::g->bmapheight - 1;
    }
    thirdStop = secondStop * ::g->bmapwidth + blockX;
    secondStop = secondStop * ::g->bmapwidth + firstStop;
    firstStop += blockY * ::g->bmapwidth;
    finalStop = blockIndex;

    // Trace the first block section (along the top)
    for (; blockIndex <= firstStop; blockIndex++)
    {
      if ((target = RoughBlockCheck(mo, blockIndex, fov)))
      {
        return target;
      }
    }
    // Trace the second block section (right edge)
    for (blockIndex--; blockIndex <= secondStop; blockIndex += ::g->bmapwidth)
    {
      if ((target = RoughBlockCheck(mo, blockIndex, fov)))
      {
        return target;
      }
    }
    // Trace the third block section (bottom edge)
    for (blockIndex -= ::g->bmapwidth; blockIndex >= thirdStop; blockIndex--)
    {
      if ((target = RoughBlockCheck(mo, blockIndex, fov)))
      {
        return target;
      }
    }
    // Trace the final block section (left edge)
    for (blockIndex++; blockIndex > finalStop; blockIndex -= ::g->bmapwidth)
    {
      if ((target = RoughBlockCheck(mo, blockIndex, fov)))
      {
        return target;
      }
    }
  }
  return NULL;
}

extern "C" {
//
// A_KeenDie
// DOOM II special, map 32.
// Uses special tag 666.
//
void A_KeenDie (mobj_t* mo)
{
    thinker_t*	th;
    mobj_t*	mo2;
    line_t	junk;

    A_Fall (mo);
    
    // scan the remaining thinkers
    // to see if all Keens are dead
    for (th = ::g->thinkercap.next ; th != &::g->thinkercap ; th=th->next)
    {
	bool skip = true;
	if (const actionf_p1* thAction = std::get_if<actionf_p1>(&th->function)) {
		skip = (*thAction) != (actionf_p1)P_MobjThinker;
	}
	if (skip)
	    continue;

	mo2 = (mobj_t *)th;
	if (mo2 != mo
	    && mo2->type == mo->type
	    && mo2->health > 0)
	{
	    // other Keen not dead
	    return;		
	}
    }

    junk.tag = 666;
    EV_DoDoor(&junk,opened);
}


//
// ACTION ROUTINES
//

//
// A_Look
// Stay in state until a player is sighted.
//
void A_Look (mobj_t* actor)
{
    mobj_t*	targ;
	
    actor->threshold = 0;	// any shot will wake up
    targ = actor->subsector->sector->soundtarget;

    if (targ
	&& (targ->flags & MF_SHOOTABLE) )
    {
		if (actor->originalTarget == NULL) {
			actor->originalTarget = targ;
		}
	actor->target = targ;

	if ( actor->flags & MF_AMBUSH )
	{
	    if (P_CheckSight (actor, actor->target))
		goto seeyou;
	}
	else
	    goto seeyou;
    }
	
	
    if (!P_LookForPlayers (actor, false) )
	return;
		
    // go into chase state
  seeyou:
    if (actor->info->seesound)
    {
	int		sound;
		
	switch (actor->info->seesound)
	{
	  case sfx_posit1:
	  case sfx_posit2:
	  case sfx_posit3:
	    sound = sfx_posit1+P_Random()%3;
	    break;

	  case sfx_bgsit1:
	  case sfx_bgsit2:
	    sound = sfx_bgsit1+P_Random()%2;
	    break;

	  default:
	    sound = actor->info->seesound;
	    break;
	}

	if ((actor->flags2 & MF2_BOSS) || (actor->flags2 & MF2_FULLVOLSOUNDS))
	{
	    // full volume
	    S_StartSound (NULL, sound);
	}
	else
	    S_StartSound (actor, sound);
    }

    P_SetMobjState (actor, actor->info->seestate);
}


//
// A_Chase
// Actor has a melee attack,
// so it tries to close as fast as possible
//
void A_Chase (mobj_t*	actor)
{
    int		delta;

    if (actor->reactiontime)
	actor->reactiontime--;
				

    // modify target threshold
    if  (actor->threshold)
    {
	if (!actor->target
	    || actor->target->health <= 0)
	{
	    actor->threshold = 0;
	}
	else
	    actor->threshold--;
    }
    
    // turn towards movement direction if not there yet
    if (actor->movedir < 8)
    {
	actor->angle &= (7<<29);
	delta = actor->angle - (actor->movedir << 29);
	
	if (delta > 0)
	    actor->angle -= ANG90/2;
	else if (delta < 0)
	    actor->angle += ANG90/2;
    }

    if (!actor->target
	|| !(actor->target->flags&MF_SHOOTABLE))
    {
	// look for a new target
	if (P_LookForPlayers(actor,true))
	    return; 	// got a new target
	
	P_SetMobjState (actor, actor->info->spawnstate);
	return;
    }
    
    // do not attack twice in a row
    if (actor->flags & MF_JUSTATTACKED)
    {
	actor->flags &= ~MF_JUSTATTACKED;
	if (::g->gameskill != sk_nightmare && !::g->fastparm)
	    P_NewChaseDir (actor);
	return;
    }
    
    // check for melee attack
    if (actor->info->meleestate && P_CheckMeleeRange (actor))
    {
	if (actor->info->attacksound)
	    S_StartSound (actor, actor->info->attacksound);

	P_SetMobjState (actor, actor->info->meleestate);
	return;
    }
    
    // check for missile attack
    if (actor->info->missilestate)
    {
	if (::g->gameskill < sk_nightmare
	    && !::g->fastparm && actor->movecount)
	{
	    goto nomissile;
	}
	
	if (!P_CheckMissileRange (actor))
	    goto nomissile;
	
	P_SetMobjState (actor, actor->info->missilestate);
	actor->flags |= MF_JUSTATTACKED;
	return;
    }

    // ?
  nomissile:
    // possibly choose another target
    if (::g->netgame
	&& !actor->threshold
	&& !P_CheckSight (actor, actor->target) )
    {
	if (P_LookForPlayers(actor,true))
	    return;	// got a new target
    }
    
    // chase towards player
    if (--actor->movecount<0
	|| !P_Move (actor))
    {
	P_NewChaseDir (actor);
    }
    
    // make active sound
    if (actor->info->activesound && P_Random () < 3)
    {
		S_StartSound (actor, actor->info->activesound);
    }
}


//
// A_FaceTarget
//
void A_FaceTarget (mobj_t* actor)
{	
    if (!actor->target)
	return;
    
    actor->flags &= ~MF_AMBUSH;
	
    actor->angle = R_PointToAngle2 (actor->x,
				    actor->y,
				    actor->target->x,
				    actor->target->y);
    
    if (actor->target->flags & MF_SHADOW)
	actor->angle += (P_Random()-P_Random())<<21;
}


//
// A_PosAttack
//
void A_PosAttack (mobj_t* actor)
{
    int		angle;
    int		damage;
    int		slope;
	
    if (!actor->target)
	return;
		
    A_FaceTarget (actor);
    angle = actor->angle;
    slope = P_AimLineAttack (actor, angle, MISSILERANGE);

    S_StartSound (actor, sfx_pistol);
    angle += (P_Random()-P_Random())<<20;
    damage = ((P_Random()%5)+1)*3;
    P_LineAttack (actor, angle, MISSILERANGE, slope, damage);
}

void A_SPosAttack (mobj_t* actor)
{
    int		i;
    int		angle;
    int		bangle;
    int		damage;
    int		slope;
	
    if (!actor->target)
	return;

    S_StartSound (actor, sfx_shotgn);
    A_FaceTarget (actor);
    bangle = actor->angle;
    slope = P_AimLineAttack (actor, bangle, MISSILERANGE);

    for (i=0 ; i<3 ; i++)
    {
	angle = bangle + ((P_Random()-P_Random())<<20);
	damage = ((P_Random()%5)+1)*3;
	P_LineAttack (actor, angle, MISSILERANGE, slope, damage);
    }
}

void A_CPosAttack (mobj_t* actor)
{
    int		angle;
    int		bangle;
    int		damage;
    int		slope;
	
    if (!actor->target)
	return;

    S_StartSound (actor, sfx_shotgn);
    A_FaceTarget (actor);
    bangle = actor->angle;
    slope = P_AimLineAttack (actor, bangle, MISSILERANGE);

    angle = bangle + ((P_Random()-P_Random())<<20);
    damage = ((P_Random()%5)+1)*3;
    P_LineAttack (actor, angle, MISSILERANGE, slope, damage);
}

void A_CPosRefire (mobj_t* actor)
{	
    // keep firing unless target got out of sight
    A_FaceTarget (actor);

    if (P_Random () < 40)
	return;

    if (!actor->target
	|| actor->target->health <= 0
	|| !P_CheckSight (actor, actor->target) )
    {
	P_SetMobjState (actor, actor->info->seestate);
    }
}


void A_SpidRefire (mobj_t* actor)
{	
    // keep firing unless target got out of sight
    A_FaceTarget (actor);

    if (P_Random () < 10)
	return;

    if (!actor->target
	|| actor->target->health <= 0
	|| !P_CheckSight (actor, actor->target) )
    {
	P_SetMobjState (actor, actor->info->seestate);
    }
}

void A_BspiAttack (mobj_t *actor)
{	
    if (!actor->target)
	return;
		
    A_FaceTarget (actor);

    // launch a missile
    P_SpawnMissile (actor, actor->target, MT_ARACHPLAZ);
}


//
// A_TroopAttack
//
void A_TroopAttack (mobj_t* actor)
{
    int		damage;
	
    if (!actor->target)
	return;
		
    A_FaceTarget (actor);
    if (P_CheckMeleeRange (actor))
    {
	S_StartSound (actor, sfx_claw);
	damage = (P_Random()%8+1)*3;
	P_DamageMobj (actor->target, actor, actor, damage);
	return;
    }

    
    // launch a missile
    P_SpawnMissile (actor, actor->target, MT_TROOPSHOT);
}


void A_SargAttack (mobj_t* actor)
{
    int		damage;

    if (!actor->target)
	return;
		
    A_FaceTarget (actor);
    if (P_CheckMeleeRange (actor))
    {
	damage = ((P_Random()%10)+1)*4;
	P_DamageMobj (actor->target, actor, actor, damage);
    }
}

void A_HeadAttack (mobj_t* actor)
{
    int		damage;
	
    if (!actor->target)
	return;
		
    A_FaceTarget (actor);
    if (P_CheckMeleeRange (actor))
    {
	damage = (P_Random()%6+1)*10;
	P_DamageMobj (actor->target, actor, actor, damage);
	return;
    }
    
    // launch a missile
    P_SpawnMissile (actor, actor->target, MT_HEADSHOT);
}

void A_CyberAttack (mobj_t* actor)
{	
    if (!actor->target)
	return;
		
    A_FaceTarget (actor);
    P_SpawnMissile (actor, actor->target, MT_ROCKET);
}


void A_BruisAttack (mobj_t* actor)
{
    int		damage;
	
    if (!actor->target)
	return;
		
    if (P_CheckMeleeRange (actor))
    {
	S_StartSound (actor, sfx_claw);
	damage = (P_Random()%8+1)*10;
	P_DamageMobj (actor->target, actor, actor, damage);
	return;
    }
    
    // launch a missile
    P_SpawnMissile (actor, actor->target, MT_BRUISERSHOT);
}


//
// A_SkelMissile
//
void A_SkelMissile (mobj_t* actor)
{	
    mobj_t*	mo;
	
    if (!actor->target)
	return;
		
    A_FaceTarget (actor);
    actor->z += 16*FRACUNIT;	// so missile spawns higher
    mo = P_SpawnMissile (actor, actor->target, MT_TRACER);
    actor->z -= 16*FRACUNIT;	// back to normal

    mo->x += mo->momx;
    mo->y += mo->momy;
    mo->tracer = actor->target;
}


void A_Tracer (mobj_t* actor)
{
    angle_t	exact;
    fixed_t	dist;
    fixed_t	slope;
    mobj_t*	dest;
    mobj_t*	th;
		
    //if (::g->gametic & 3)
		//return;

	// DHM - Nerve :: Demo fix - Keep the game state deterministic!!!
	if ( ::g->leveltime & 3 ) {
		return;
	}

    // spawn a puff of smoke behind the rocket		
    P_SpawnPuff (actor->x, actor->y, actor->z);
	
    th = P_SpawnMobj (actor->x-actor->momx,
		      actor->y-actor->momy,
		      actor->z, MT_SMOKE);
    
    th->momz = FRACUNIT;
    th->tics -= P_Random()&3;
    if (th->tics < 1)
	th->tics = 1;
    
    // adjust direction
    dest = actor->tracer;
	
    if (!dest || dest->health <= 0)
	return;
    
    // change angle	
    exact = R_PointToAngle2 (actor->x,
			     actor->y,
			     dest->x,
			     dest->y);

    if (exact != actor->angle)
    {
	if (exact - actor->angle > 0x80000000)
	{
	    actor->angle -= ::g->TRACEANGLE;
	    if (exact - actor->angle < 0x80000000)
		actor->angle = exact;
	}
	else
	{
	    actor->angle += ::g->TRACEANGLE;
	    if (exact - actor->angle > 0x80000000)
		actor->angle = exact;
	}
    }
	
    exact = actor->angle>>ANGLETOFINESHIFT;
    actor->momx = FixedMul (actor->info->speed, finecosine[exact]);
    actor->momy = FixedMul (actor->info->speed, finesine[exact]);
    
    // change slope
    dist = P_AproxDistance (dest->x - actor->x,
			    dest->y - actor->y);
    
    dist = dist / actor->info->speed;

    if (dist < 1)
	dist = 1;
    slope = (dest->z+40*FRACUNIT - actor->z) / dist;

    if (slope < actor->momz)
	actor->momz -= FRACUNIT/8;
    else
	actor->momz += FRACUNIT/8;
}


void A_SkelWhoosh (mobj_t*	actor)
{
    if (!actor->target)
	return;
    A_FaceTarget (actor);
    S_StartSound (actor,sfx_skeswg);
}

void A_SkelFist (mobj_t*	actor)
{
    int		damage;

    if (!actor->target)
	return;
		
    A_FaceTarget (actor);
	
    if (P_CheckMeleeRange (actor))
    {
	damage = ((P_Random()%10)+1)*6;
	S_StartSound (actor, sfx_skepch);
	P_DamageMobj (actor->target, actor, actor, damage);
    }
}



//
// PIT_VileCheck
// Detect a corpse that could be raised.
//

qboolean PIT_VileCheck (mobj_t*	thing )
{
    int		maxdist;
    qboolean	check;
	
    if (!(thing->flags & MF_CORPSE) )
	return true;	// not a monster
    
    if (thing->tics != -1)
	return true;	// not lying still yet
    
    if (thing->info->raisestate == S_NULL)
	return true;	// monster doesn't have a raise state
    
    maxdist = thing->info->radius + mobjinfo[MT_VILE].radius;
	
    if ( abs(thing->x - ::g->viletryx) > maxdist
	 || abs(thing->y - ::g->viletryy) > maxdist )
	return true;		// not actually touching
		
    ::g->corpsehit = thing;
    ::g->corpsehit->momx = ::g->corpsehit->momy = 0;
    ::g->corpsehit->height <<= 2;
    check = P_CheckPosition (::g->corpsehit, ::g->corpsehit->x, ::g->corpsehit->y);
    ::g->corpsehit->height >>= 2;

    if (!check)
	return true;		// doesn't fit here
		
    return false;		// got one, so stop checking
}



//
// A_VileChase
// Check for ressurecting a body
//
void A_VileChase (mobj_t* actor)
{
    int			xl;
    int			xh;
    int			yl;
    int			yh;
    
    int			bx;
    int			by;

    const mobjinfo_t*	info;
    mobj_t*		temp;
	
    if (actor->movedir != DI_NODIR_CL)
    {
	// check for corpses to raise
	::g->viletryx =
	    actor->x + actor->info->speed*xspeed[actor->movedir];
	::g->viletryy =
	    actor->y + actor->info->speed*yspeed[actor->movedir];

	xl = (::g->viletryx - ::g->bmaporgx - MAXRADIUS*2)>>MAPBLOCKSHIFT;
	xh = (::g->viletryx - ::g->bmaporgx + MAXRADIUS*2)>>MAPBLOCKSHIFT;
	yl = (::g->viletryy - ::g->bmaporgy - MAXRADIUS*2)>>MAPBLOCKSHIFT;
	yh = (::g->viletryy - ::g->bmaporgy + MAXRADIUS*2)>>MAPBLOCKSHIFT;
	
	::g->vileobj = actor;
	for (bx=xl ; bx<=xh ; bx++)
	{
	    for (by=yl ; by<=yh ; by++)
	    {
		// Call PIT_VileCheck to check
		// whether object is a corpse
		// that canbe raised.
		if (!P_BlockThingsIterator(bx,by,PIT_VileCheck))
		{
		    // got one!
		    temp = actor->target;
		    actor->target = ::g->corpsehit;
		    A_FaceTarget (actor);
		    actor->target = temp;
					
		    P_SetMobjState (actor, S_VILE_HEAL1);
		    S_StartSound (::g->corpsehit, sfx_slop);
		    info = ::g->corpsehit->info;
		    
		    P_SetMobjState (::g->corpsehit,info->raisestate);
		    ::g->corpsehit->height <<= 2;
		    ::g->corpsehit->flags = info->flags;
		    ::g->corpsehit->health = info->spawnhealth;
		    ::g->corpsehit->target = NULL;

		    return;
		}
	    }
	}
    }

    // Return to normal attack.
    A_Chase (actor);
}


//
// A_VileStart
//
void A_VileStart (mobj_t* actor)
{
    S_StartSound (actor, sfx_vilatk);
}


//
// A_Fire
// Keep fire in front of player unless out of sight
//
void A_Fire (mobj_t* actor);

void A_StartFire (mobj_t* actor)
{
    S_StartSound(actor,sfx_flamst);
    A_Fire(actor);
}

void A_FireCrackle (mobj_t* actor)
{
    S_StartSound(actor,sfx_flame);
    A_Fire(actor);
}

void A_Fire (mobj_t* actor)
{
    mobj_t*	dest;
    unsigned	an;
		
    dest = actor->tracer;
    if (!dest)
	return;
		
    // don't move it if the vile lost sight
    if (!P_CheckSight (actor->target, dest) )
	return;

    an = dest->angle >> ANGLETOFINESHIFT;

    P_UnsetThingPosition (actor);
    actor->x = dest->x + FixedMul (24*FRACUNIT, finecosine[an]);
    actor->y = dest->y + FixedMul (24*FRACUNIT, finesine[an]);
    actor->z = dest->z;
    P_SetThingPosition (actor);
}



//
// A_VileTarget
// Spawn the hellfire
//
void A_VileTarget (mobj_t*	actor)
{
    mobj_t*	fog;
	
    if (!actor->target)
	return;

    A_FaceTarget (actor);

    fog = P_SpawnMobj (actor->target->x,
		       actor->target->y,
		       actor->target->z, MT_FIRE);
    
    actor->tracer = fog;
    fog->target = actor;
    fog->tracer = actor->target;
    A_Fire (fog);
}




//
// A_VileAttack
//
void A_VileAttack (mobj_t* actor)
{	
    mobj_t*	fire;
    int		an;
	
    if (!actor->target)
	return;
    
    A_FaceTarget (actor);

    if (!P_CheckSight (actor, actor->target) )
	return;

    S_StartSound (actor, sfx_barexp);
    P_DamageMobj (actor->target, actor, actor, 20);
    actor->target->momz = 1000*FRACUNIT/actor->target->info->mass;
	
    an = actor->angle >> ANGLETOFINESHIFT;

    fire = actor->tracer;

    if (!fire)
	return;
		
    // move the fire between the vile and the player
    fire->x = actor->target->x - FixedMul (24*FRACUNIT, finecosine[an]);
    fire->y = actor->target->y - FixedMul (24*FRACUNIT, finesine[an]);	
    P_RadiusAttack (fire, actor, 70 );
}




//
// Mancubus attack,
// firing three missiles (bruisers)
// in three different directions?
// Doesn't look like it. 
//

void A_FatRaise (mobj_t *actor)
{
    A_FaceTarget (actor);
    S_StartSound (actor, sfx_manatk);
}


void A_FatAttack1 (mobj_t* actor)
{
    mobj_t*	mo;
    int		an;
	
    A_FaceTarget (actor);
    // Change direction  to ...
    actor->angle += FATSPREAD;
    P_SpawnMissile (actor, actor->target, MT_FATSHOT);

    mo = P_SpawnMissile (actor, actor->target, MT_FATSHOT);
    mo->angle += FATSPREAD;
    an = mo->angle >> ANGLETOFINESHIFT;
    mo->momx = FixedMul (mo->info->speed, finecosine[an]);
    mo->momy = FixedMul (mo->info->speed, finesine[an]);
}

void A_FatAttack2 (mobj_t* actor)
{
    mobj_t*	mo;
    int		an;

    A_FaceTarget (actor);
    // Now here choose opposite deviation.
    actor->angle -= FATSPREAD;
    P_SpawnMissile (actor, actor->target, MT_FATSHOT);

    mo = P_SpawnMissile (actor, actor->target, MT_FATSHOT);
    mo->angle -= FATSPREAD*2;
    an = mo->angle >> ANGLETOFINESHIFT;
    mo->momx = FixedMul (mo->info->speed, finecosine[an]);
    mo->momy = FixedMul (mo->info->speed, finesine[an]);
}

void A_FatAttack3 (mobj_t*	actor)
{
    mobj_t*	mo;
    int		an;

    A_FaceTarget (actor);
    
    mo = P_SpawnMissile (actor, actor->target, MT_FATSHOT);
    mo->angle -= FATSPREAD/2;
    an = mo->angle >> ANGLETOFINESHIFT;
    mo->momx = FixedMul (mo->info->speed, finecosine[an]);
    mo->momy = FixedMul (mo->info->speed, finesine[an]);

    mo = P_SpawnMissile (actor, actor->target, MT_FATSHOT);
    mo->angle += FATSPREAD/2;
    an = mo->angle >> ANGLETOFINESHIFT;
    mo->momx = FixedMul (mo->info->speed, finecosine[an]);
    mo->momy = FixedMul (mo->info->speed, finesine[an]);
}


//
// SkullAttack
// Fly at the player like a missile.
//

void A_SkullAttack (mobj_t* actor)
{
    mobj_t*		dest;
    angle_t		an;
    int			dist;

    if (!actor->target)
	return;
		
    dest = actor->target;	
    actor->flags |= MF_SKULLFLY;

    S_StartSound (actor, actor->info->attacksound);
    A_FaceTarget (actor);
    an = actor->angle >> ANGLETOFINESHIFT;
    actor->momx = FixedMul (SKULLSPEED, finecosine[an]);
    actor->momy = FixedMul (SKULLSPEED, finesine[an]);
    dist = P_AproxDistance (dest->x - actor->x, dest->y - actor->y);
    dist = dist / SKULLSPEED;
    
    if (dist < 1)
	dist = 1;
    actor->momz = (dest->z+(dest->height>>1) - actor->z) / dist;
}


//
// A_PainShootSkull
// Spawn a lost soul and launch it at the target
//
void
A_PainShootSkull
( mobj_t*	actor,
  angle_t	angle )
{
    fixed_t	x;
    fixed_t	y;
    fixed_t	z;
    
    mobj_t*	newmobj;
    angle_t	an;
    int		prestep;
    int		count;
    thinker_t*	currentthinker;

    // count total number of skull currently on the level
    count = 0;

    currentthinker = ::g->thinkercap.next;
    while (currentthinker != &::g->thinkercap)
    {
		bool countIn = false;
		if (const actionf_p1* currentAction = std::get_if<actionf_p1>(&currentthinker->function)) {
			countIn = (*currentAction) == (actionf_p1)P_MobjThinker;
		}
		if (countIn
	    && ((mobj_t *)currentthinker)->type == MT_SKULL)
	    count++;
	currentthinker = currentthinker->next;
    }

    // if there are allready 20 skulls on the level,
    // don't spit another one
   // if (count > 20)
	//return;


    // okay, there's playe for another one
    an = angle >> ANGLETOFINESHIFT;
    
    prestep =
	4*FRACUNIT
	+ 3*(actor->info->radius + mobjinfo[MT_SKULL].radius)/2;
    
    x = actor->x + FixedMul (prestep, finecosine[an]);
    y = actor->y + FixedMul (prestep, finesine[an]);
    z = actor->z + 8*FRACUNIT;
		
    newmobj = P_SpawnMobj (x , y, z, MT_SKULL);

    // Check for movements.
    if (!P_TryMove (newmobj, newmobj->x, newmobj->y))
    {
	// kill it immediately
	P_DamageMobj (newmobj,actor,actor,10000);	
	return;
    }
		
    newmobj->target = actor->target;
    A_SkullAttack (newmobj);
}


//
// A_PainAttack
// Spawn a lost soul and launch it at the target
// 
void A_PainAttack (mobj_t* actor)
{
    if (!actor->target)
	return;

    A_FaceTarget (actor);
    A_PainShootSkull (actor, actor->angle);
}


void A_PainDie (mobj_t* actor)
{
    A_Fall (actor);
    A_PainShootSkull (actor, actor->angle+ANG90);
    A_PainShootSkull (actor, actor->angle+ANG180);
    A_PainShootSkull (actor, actor->angle+ANG270);
}






void A_Scream (mobj_t* actor)
{
    int		sound;
	
    switch (actor->info->deathsound)
    {
      case 0:
	return;
		
      case sfx_podth1:
      case sfx_podth2:
      case sfx_podth3:
	sound = sfx_podth1 + P_Random ()%3;
	break;
		
      case sfx_bgdth1:
      case sfx_bgdth2:
	sound = sfx_bgdth1 + P_Random ()%2;
	break;
	
      default:
	sound = actor->info->deathsound;
	break;
    }

    // Check for bosses.
    if ((actor->flags2 & MF2_BOSS) || (actor->flags2 & MF2_FULLVOLSOUNDS))
    {
	// full volume
	S_StartSound (NULL, sound);
    }
    else
	S_StartSound (actor, sound);
}


void A_XScream (mobj_t* actor)
{
    S_StartSound (actor, sfx_slop);	
}

void A_Pain (mobj_t* actor)
{
	if (actor->info->painsound )
		S_StartSound (actor, actor->info->painsound);	
}



void A_Fall (mobj_t *actor)
{
    // actor is on ground, it can be walked over
    actor->flags &= ~MF_SOLID;

    // So change this if corpse objects
    // are meant to be obstacles.
}


//
// A_Explode
//
void A_Explode (mobj_t* thingy)
{
    P_RadiusAttack ( thingy, thingy->target, 128 );
}


//
// A_BossDeath
// Possibly trigger special effects
// if on first boss level
//
void A_BossDeath (mobj_t* mo)
{
    thinker_t*	th;
    mobj_t*	mo2;
    line_t	junk;
    int		i;
	size_t ok = false; //GK:Oversimplyfication for the if case on map 07's logic
		
	if (::g->gamemission == pack_custom && ::g->map) { //GK:Custom expansion related stuff
		for (size_t i1 = 0; i1 < ::g->maps[::g->map - 1].bossData.size(); i1++) {
			if (::g->maps[::g->map - 1].bossData[i1].name) {
				if (mo->type != ::g->maps[::g->map - 1].bossData[i1].name) {
					continue;
				}
				else {
					ok = i1 + 1;
					break;
				}
			}
		}

		if (!::g->maps[::g->map - 1].miniboss || (::g->maps[::g->map - 1].bossData.size() > 0 && !ok)) {
			return;
		}
		else {
			ok = !ok ? true: ok;
		}
	}else
    if ( ::g->gamemode == commercial)
    {

		if (::g->gamemission == pack_master) {
			if (::g->gamemap != 14 && ::g->gamemap != 15 && ::g->gamemap != 16) {// GK: Fix for Master Levels
				return;
			}
			else {
				ok = true;
			}
		}

		if (::g->gamemission != pack_master)
			if (::g->gamemap != 7) 
				return;

	idLib::Printf("%d %d\n", ::g->gamemission, ::g->gamemap);
	if (!(mo->flags2 & MF2_MAP07BOSS1)
	    && !(mo->flags2 & MF2_MAP07BOSS2))
	    return;
    }
    else
    {
	switch(::g->gameepisode)
	{
	  case 1:
	    if (::g->gamemap != 8)
		return;

	    if (!(mo->flags2 & MF2_E1M8BOSS))
		return;
	    break;
	    
	  case 2:
	    if (::g->gamemap != 8)
		return;

	    if (!(mo->flags2 & MF2_E2M8BOSS))
		return;
	    break;
	    
	  case 3:
	    if (::g->gamemap != 8)
		return;
	    
	    if (!(mo->flags2 & MF2_E3M8BOSS))
		return;
	    
	    break;
	    
	  case 4:
	    switch(::g->gamemap)
	    {
	      case 6:
		if (!(mo->flags2 & MF2_E4M6BOSS))
		    return;
		break;
		
	      case 8: 
		if (!(mo->flags2 & MF2_E4M8BOSS))
		    return;
		break;
		
	      default:
		return;
		break;
	    }
	    break;
	    
	  default:
	    if (::g->gamemap != 8)
		return;
	    break;
	}
		
    }
	
    
    // make sure there is a player alive for victory
    for (i=0 ; i<MAXPLAYERS ; i++)
	if (::g->playeringame[i] && ::g->players[i].health > 0)
	    break;
    
    if (i==MAXPLAYERS)
	return;	// no one left alive, so do not end game
    
    // scan the remaining thinkers to see
    // if all bosses are dead
    for (th = ::g->thinkercap.next ; th != &::g->thinkercap ; th=th->next)
    {
	bool skip = true;
	if (const actionf_p1* thAction = std::get_if<actionf_p1>(&th->function)) {
		skip = (*thAction) != (actionf_p1)P_MobjThinker;
	}
	if (skip)
	    continue;
	
	mo2 = (mobj_t *)th;
	if (mo2 != mo
	    && mo2->type == mo->type
	    && mo2->health > 0)
	{
	    // other boss not dead
	    return;
	}
    }
	
	if (::g->gamemission == pack_custom) {
		junk.tag = ::g->maps[::g->map - 1].bossData[ok - 1].tag ? ::g->maps[::g->map - 1].bossData[ok - 1].tag : 666;
		if (::g->maps[::g->map - 1].bossData[ok - 1].action) {
			if (!idStr::Icmp("openDoor", ::g->maps[::g->map - 1].bossData[ok - 1].action)) {
				EV_DoDoor(&junk, blazeOpen);
				return;
			}
			if (!idStr::Icmp("lowerFloor", ::g->maps[::g->map - 1].bossData[ok - 1].action)) {
				EV_DoFloor(&junk, lowerFloorToLowest);
				return;
			}
		}
	} else

    // victory!
    if ( ::g->gamemode == commercial)
    {
	if (::g->gamemap == 7 || ok) // GK: Take in account the 14th ,15th and 16th levels on master Levels Expansion since they are using MAP07 special logic
	{
	    if (mo->flags2 & MF2_MAP07BOSS1)
	    {
		junk.tag = 666;
		EV_DoFloor(&junk,lowerFloorToLowest);
		return;
	    }
	    
	    if (mo->flags2 & MF2_MAP07BOSS2)
	    {
		junk.tag = 667;
		EV_DoFloor(&junk,raiseToTexture);
		return;
	    }
	}
    }
    else
    {
		switch (::g->gameepisode)
		{
		case 1:
			junk.tag = 666;
			EV_DoFloor(&junk, lowerFloorToLowest);
			return;
			break;

		case 4:
			switch (::g->gamemap)
			{
			case 6:
				junk.tag = 666;
				EV_DoDoor(&junk, blazeOpen);
				return;
				break;

			case 8:
				junk.tag = 666;
				EV_DoFloor(&junk, lowerFloorToLowest);
				return;
				break;
			}
		}
    }
	
    G_ExitLevel ();
}


void A_Hoof (mobj_t* mo)
{
    S_StartSound (mo, sfx_hoof);
    A_Chase (mo);
}

void A_Metal (mobj_t* mo)
{
    S_StartSound (mo, sfx_metal);
    A_Chase (mo);
}

void A_BabyMetal (mobj_t* mo)
{
    S_StartSound (mo, sfx_bspwlk);
    A_Chase (mo);
}

void
A_OpenShotgun2
( player_t*	player,
  pspdef_t*	psp )
{
	if (globalNetworking || (player == &::g->players[::g->consoleplayer]))
		S_StartSound (player->mo, sfx_dbopn);
}

void
A_LoadShotgun2
( player_t*	player,
  pspdef_t*	psp )
{
	if (globalNetworking || (player == &::g->players[::g->consoleplayer]))
		S_StartSound (player->mo, sfx_dbload);
}

void
A_ReFire
( player_t*	player,
  pspdef_t*	psp );

void
A_CloseShotgun2
( player_t*	player,
  pspdef_t*	psp )
{
	if (globalNetworking || (player == &::g->players[::g->consoleplayer]))
		S_StartSound (player->mo, sfx_dbcls);
    A_ReFire(player,psp);
}




void A_BrainAwake (mobj_t* mo)
{
    thinker_t*	thinker;
    mobj_t*	m;
	
    // find all the target spots
	::g->easy = 0;
    ::g->numbraintargets = 0;
    ::g->braintargeton = 0;
	
    thinker = ::g->thinkercap.next;
    for (thinker = ::g->thinkercap.next ;
	 thinker != &::g->thinkercap ;
	 thinker = thinker->next)
    {
	bool skip = true;
	if (const actionf_p1* thAction = std::get_if<actionf_p1>(&thinker->function)) {
		skip = (*thAction) != (actionf_p1)P_MobjThinker;
	}
	if (skip)
	    continue;	// not a mobj

	m = (mobj_t *)thinker;

	if (m->type == MT_BOSSTARGET )
	{
	    ::g->braintargets[::g->numbraintargets] = m;
	    ::g->numbraintargets++;
	}
    }
	
    S_StartSound (NULL,sfx_bossit);
}


void A_BrainPain (mobj_t*	mo)
{
    S_StartSound (NULL,sfx_bospn);
}


void A_BrainScream (mobj_t*	mo)
{
    int		x;
    int		y;
    int		z;
    mobj_t*	th;
	
    for (x=mo->x - 196*FRACUNIT ; x< mo->x + 320*FRACUNIT ; x+= FRACUNIT*8)
    {
	y = mo->y - 320*FRACUNIT;
	z = 128 + P_Random()*2*FRACUNIT;
	th = P_SpawnMobj (x,y,z, MT_ROCKET);
	th->momz = P_Random()*512;

	P_SetMobjState (th, S_BRAINEXPLODE1);

	th->tics -= P_Random()&7;
	if (th->tics < 1)
	    th->tics = 1;
    }
	
    S_StartSound (NULL,sfx_bosdth);
}



void A_BrainExplode (mobj_t* mo)
{
    int		x;
    int		y;
    int		z;
    mobj_t*	th;
	
    x = mo->x + (P_Random () - P_Random ())*2048;
    y = mo->y;
    z = 128 + P_Random()*2*FRACUNIT;
    th = P_SpawnMobj (x,y,z, MT_ROCKET);
    th->momz = P_Random()*512;

    P_SetMobjState (th, S_BRAINEXPLODE1);

    th->tics -= P_Random()&7;
    if (th->tics < 1)
	th->tics = 1;
}


void A_BrainDie (mobj_t*	mo)
{
    G_ExitLevel ();
}

void A_BrainSpit (mobj_t*	mo)
{
	mobj_t*	targ;
	mobj_t*	newmobj;

	::g->easy ^= 1;
	if (::g->gameskill <= sk_easy && (!::g->easy))
		return;

	if ( 1 ) {
		// count number of thinkers
		int numCorpse = 0;
		int numEnemies = 0;

		for ( thinker_t* th = ::g->thinkercap.next; th != &::g->thinkercap; th = th->next ) {
			bool countIn = false;
			if (const actionf_p1* currentAction = std::get_if<actionf_p1>(&th->function)) {
				countIn = (*currentAction) == (actionf_p1)P_MobjThinker;
			}
			if (countIn ) {
				mobj_t* obj = (mobj_t*)th;

				if ( obj->flags & MF_CORPSE ) {
					numCorpse++;
				}
				else if ( obj->type > MT_PLAYER && obj->type < MT_KEEN ) {
					numEnemies++;
				}
			}
		}

		if ( numCorpse > 48 ) {
			for ( int i = 0; i < 12; i++ ) {
				for ( thinker_t* th = ::g->thinkercap.next; th != &::g->thinkercap; th = th->next ) {
					bool countIn = false;
					if (const actionf_p1* currentAction = std::get_if<actionf_p1>(&th->function)) {
						countIn = (*currentAction) == (actionf_p1)P_MobjThinker;
					}
					if (countIn ) {
						mobj_t* obj = (mobj_t*)th;

						if ( obj->flags & MF_CORPSE ) {
							P_RemoveMobj( obj );
							break;
						}
					}
				}
			}
		}

		if ( numEnemies > 32 ) {
			return;
		}
	}

	// shoot a cube at current target
	targ = ::g->braintargets[::g->braintargeton];
	::g->braintargeton = (::g->braintargeton+1) % ::g->numbraintargets;

	// spawn brain missile
	newmobj = P_SpawnMissile (mo, targ, MT_SPAWNSHOT);
	newmobj->target = targ;
	newmobj->reactiontime =
	((targ->y - mo->y)/newmobj->momy) / newmobj->state->tics;

	S_StartSound(NULL, sfx_bospit);
}



void A_SpawnFly (mobj_t* mo);

// travelling cube sound
void A_SpawnSound (mobj_t* mo)	
{
    S_StartSound (mo,sfx_boscub);
    A_SpawnFly(mo);
}

void A_SpawnFly (mobj_t* mo)
{
    mobj_t*	newmobj;
    mobj_t*	fog;
    mobj_t*	targ;
    int		r;
    mobjtype_t	type;
	
    if (--mo->reactiontime)
	return;	// still flying
	
    targ = mo->target;

    // First spawn teleport fog.
    fog = P_SpawnMobj (targ->x, targ->y, targ->z, MT_SPAWNFIRE);
    S_StartSound (fog, sfx_telept);

    // Randomly select monster to spawn.
    r = P_Random ();

    // Probability distribution (kind of :),
    // decreasing likelihood.
    if ( r<50 )
	type = MT_TROOP;
    else if (r<90)
	type = MT_SERGEANT;
    else if (r<120)
	type = MT_SHADOWS;
    else if (r<130)
	type = MT_PAIN;
    else if (r<160)
	type = MT_HEAD;
    else if (r<162)
	type = MT_VILE;
    else if (r<172)
	type = MT_UNDEAD;
    else if (r<192)
	type = MT_BABY;
    else if (r<222)
	type = MT_FATSO;
    else if (r<246)
	type = MT_KNIGHT;
    else
	type = MT_BRUISER;		

    newmobj	= P_SpawnMobj (targ->x, targ->y, targ->z, type);
    if (P_LookForPlayers (newmobj, true) )
	P_SetMobjState (newmobj, newmobj->info->seestate);
	
    // telefrag anything in this spot
    P_TeleportMove (newmobj, newmobj->x, newmobj->y);

    // remove self (i.e., cube).
    P_RemoveMobj (mo);
}



void A_PlayerScream (mobj_t* mo)
{
    // Default death sound.
    int		sound = sfx_pldeth;
	
    if ( (::g->gamemode == commercial)
	&& 	(mo->health < -50))
    {
	// IF THE PLAYER DIES
	// LESS THAN -50% WITHOUT GIBBING
	sound = sfx_pdiehi;
    }
    
	if ( ::g->demoplayback || globalNetworking || (mo == ::g->players[::g->consoleplayer].mo))
		S_StartSound (mo, sound);
}

void A_RandomJump(mobj_t* mo, pspdef_t* psp)

{
	// [BH] allow A_RandomJump() to work for weapon states as well
	if (psp)
	{
		if (M_Random() < psp->state->misc2)
			P_SetPsprite(::g->plyr, psp - &::g->plyr->psprites[ps_weapon], psp->state->misc1);
	}
	else
	{
		if (M_Random() < mo->state->misc2)
			P_SetMobjState(mo, mo->state->misc1);
	}
}

void A_Spawn(mobj_t* mo) {

	if (mo->state->misc1) {
		mobj_t* newmobj = P_SpawnMobj(mo->x, mo->y, (mo->state->misc2 << FRACBITS) + mo->z, (mobjtype_t)(mo->state->misc1 - 1));
		if (!newmobj) {}
	}
}

void A_PlaySound(mobj_t* mo) {
	if (mo->state->misc1 > -1) {
		S_StartSound(mo->state->misc2 > 0? NULL : mo, mo->state->misc1);
	}
}

void A_Detonate(mobj_t* thingy)
{
	P_RadiusAttack(thingy, thingy->target, thingy->info->damage);
}

//
// killough 9/98: a mushroom explosion effect, sorta :)
// Original idea: Linguica
//

void A_Mushroom(mobj_t* actor)
{
	int i, j, n = actor->info->damage;

	// Mushroom parameters are part of code pointer's state
	fixed_t misc1 = actor->state->misc1 ? actor->state->misc1 : FRACUNIT * 4;
	fixed_t misc2 = actor->state->misc2 ? actor->state->misc2 : FRACUNIT / 2;

	A_Explode(actor);               // make normal explosion

	for (i = -n; i <= n; i += 8)    // launch mushroom cloud
		for (j = -n; j <= n; j += 8)
		{
			mobj_t target = *actor, * mo;
			target.x += i << FRACBITS;    // Aim in many directions from source
			target.y += j << FRACBITS;
			target.z += P_AproxDistance(i, j) * misc1;           // Aim fairly high
			mo = P_SpawnMissile(actor, &target, MT_FATSHOT);    // Launch fireball
			mo->momx = FixedMul(mo->momx, misc2);
			mo->momy = FixedMul(mo->momy, misc2);               // Slow down a bit
			mo->momz = FixedMul(mo->momz, misc2);
			mo->flags &= ~MF_NOGRAVITY;   // Make debris fall under gravity
		}
}

void A_Turn(mobj_t* mo) {
	mo->angle += ((uint64)mo->state->misc1 << 32) / 360;
}

void A_Face(mobj_t* mo) {
	mo->angle = ((uint64)mo->state->misc1 << 32) / 360;
}

void A_Scratch(mobj_t* mo) {
	if (mo->target && (A_FaceTarget(mo), P_CheckMeleeRange(mo))) {
		if (mo->state->misc2) {
			S_StartSound(mo, mo->state->misc2);
		}
		if (mo->state->misc1) {
			P_DamageMobj(mo->target, mo, mo, mo->state->misc1);
		}
	}
}

void A_LineEffect(mobj_t *mo) {
	for (long i = 0; i < ::g->numlines; i++) {
		if (::g->lines[i].special == mo->state->misc1 && ::g->lines[i].tag == mo->state->misc2) {
			if (!P_UseSpecialLine(mo, &::g->lines[i], 0)) {
				P_CrossSpecialLine(i, 0, mo);
			}
			mo->state->misc1 = (long)::g->lines[i].special;
			break;
		}
	}
}

void A_Die(mobj_t* mo) {
	P_DamageMobj(mo, NULL, NULL, mo->health);
}

//MBF21
void A_RadiusDamage(mobj_t* thingy)
{
	P_RadiusAttack(thingy, thingy->target, thingy->state->args[0], thingy->state->args[1]);
}

void A_RemoveFlags(mobj_t* mo) {
	int flags = mo->state->args[0];
	int flags2 = mo->state->args[1];

	// unlink/relink the thing from the blockmap if
  // the NOBLOCKMAP or NOSECTOR flags are removed
	qboolean update_blockmap = ((flags & MF_NOBLOCKMAP) && (mo->flags & MF_NOBLOCKMAP))
		|| ((flags & MF_NOSECTOR) && (mo->flags & MF_NOSECTOR));

	if (update_blockmap)
		P_UnsetThingPosition(mo);

	mo->flags &= ~flags;
	mo->flags2 &= ~flags2;

	if (update_blockmap)
		P_SetThingPosition(mo);
}

void A_MonsterProjectile(mobj_t* mo) {
	int type, angle, pitch, spawnofs_xy, spawnofs_z;
	int an;

	if (!mo->target || !mo->state->args[0])
		return;

	type = mo->state->args[0] - 1;
	angle = mo->state->args[1];
	pitch = mo->state->args[2];
	spawnofs_xy = mo->state->args[3];
	spawnofs_z = mo->state->args[4];

	A_FaceTarget(mo);
	mobj_t* missle = P_SpawnMissile(mo, mo->target, type);
	if (!missle)
		return;

	// adjust angle
	missle->angle += (unsigned int)(((int64_t)angle << 16) / 360);
	an = missle->angle >> ANGLETOFINESHIFT;
	missle->momx = FixedMul(missle->info->speed, finecosine[an]);
	missle->momy = FixedMul(missle->info->speed, finesine[an]);

	// adjust pitch (approximated, using Doom's ye olde
	// finetangent table; same method as monster aim)
	missle->momz += FixedMul(missle->info->speed, DegToSlope(pitch));

	// adjust position
	an = (mo->angle - ANG90) >> ANGLETOFINESHIFT;
	missle->x += FixedMul(spawnofs_xy, finecosine[an]);
	missle->y += FixedMul(spawnofs_xy, finesine[an]);
	missle->z += spawnofs_z;

	// always set the 'tracer' field, so this pointer
	// can be used to fire seeker missiles at will.
	missle->tracer = mo->target;
}
void A_MonsterBulletAttack(mobj_t* mo) {
	fixed_t hspread, vspread;
	uint numbullets = 1, damagebase = 3, damagedice = 5;
	int bangle, slope, angle, damage;

	if (!mo->target || !mo->state->args[0])
		return;

	hspread = mo->state->args[0];
	vspread = mo->state->args[1];
	numbullets = mo->state->args[2];
	damagebase = mo->state->args[3];
	damagedice = mo->state->args[4];

	S_StartSound (mo, mo->info->attacksound);
	A_FaceTarget(mo);
	bangle = mo->angle;
    slope = P_AimLineAttack (mo, bangle, MISSILERANGE);

    for (uint i=0 ; i< numbullets ; i++)
    {
	angle = bangle + ((P_Random()-P_Random())<<hspread);
	damage = ((P_Random()% damagedice)+1)*damagebase;
	slope += ((P_Random()-P_Random())<<vspread);
	P_LineAttack (mo, angle, MISSILERANGE, slope, damage);
    }
}

void A_MonsterMeleeAttack(mobj_t* mo) {
	uint damagebase = 3, damagedice = 8, sound;
	fixed_t range = -1;
	int		damage;

    if (!mo->target || !mo->state->args[0])
	return;

	damagebase = mo->state->args[0];
	damagedice = mo->state->args[1];
	sound = mo->state->args[2];
	range = mo->state->args[3];
		
    A_FaceTarget (mo);
    if (P_CheckMeleeRange (mo, range))
    {
		S_StartSound (mo, sound);
		damage = ((P_Random()%damagedice)+1)*damagebase;
		P_DamageMobj (mo->target, mo, mo, damage);
    }
}

void A_NoiseAlert(mobj_t* mo) {
	P_NoiseAlert(mo, mo);
}

void A_HealChase(mobj_t* actor) {
	int			xl;
    int			xh;
    int			yl;
    int			yh;
    
    int			bx;
    int			by;
	uint		state;
	uint		sound;

	if (!actor->state->args[0]) 
		return;

	state = actor->state->args[0];
	sound = actor->state->args[1];

    const mobjinfo_t*	info;
    mobj_t*		temp;
	
    if (actor->movedir != DI_NODIR_CL)
    {
		// check for corpses to raise
		::g->viletryx =
			actor->x + actor->info->speed*xspeed[actor->movedir];
		::g->viletryy =
			actor->y + actor->info->speed*yspeed[actor->movedir];

		xl = (::g->viletryx - ::g->bmaporgx - MAXRADIUS*2)>>MAPBLOCKSHIFT;
		xh = (::g->viletryx - ::g->bmaporgx + MAXRADIUS*2)>>MAPBLOCKSHIFT;
		yl = (::g->viletryy - ::g->bmaporgy - MAXRADIUS*2)>>MAPBLOCKSHIFT;
		yh = (::g->viletryy - ::g->bmaporgy + MAXRADIUS*2)>>MAPBLOCKSHIFT;
		
		::g->vileobj = actor;
		for (bx=xl ; bx<=xh ; bx++)
		{
			for (by=yl ; by<=yh ; by++)
			{
				// Call PIT_VileCheck to check
				// whether object is a corpse
				// that canbe raised.
				if (!P_BlockThingsIterator(bx,by,PIT_VileCheck))
				{
					// got one!
					temp = actor->target;
					actor->target = ::g->corpsehit;
					A_FaceTarget (actor);
					actor->target = temp;
							
					P_SetMobjState (actor, state);
					S_StartSound (::g->corpsehit, sound);
					info = ::g->corpsehit->info;
					
					P_SetMobjState (::g->corpsehit,info->raisestate);
					::g->corpsehit->height <<= 2;
					::g->corpsehit->flags = info->flags;
					::g->corpsehit->health = info->spawnhealth;
					::g->corpsehit->target = NULL;

					return;
				}
			}
		}
    }

    // Return to normal attack.
    A_Chase (actor);
}

void A_SeekTracer(mobj_t *actor)
{
	int dir;
	int dist;
	angle_t delta;
	angle_t angle;
	mobj_t *target;
	angle_t thresh;
	angle_t turnMax;

	if (!actor->state->args[0]) 
		return;

	thresh = FixedToAngle(actor->state->args[0]);
	turnMax = FixedToAngle(actor->state->args[1]);

	target = (mobj_t *)actor->tracer;
	if(target == NULL)
	{
		return;
	}
	if(!(target->flags&MF_SHOOTABLE))
	{ // Target died
		actor->tracer = 0;
		return;
	}
	dir = P_FaceMobj(actor, target, &delta);
	if(delta > thresh)
	{
		delta >>= 1;
		if(delta > turnMax)
		{
			delta = turnMax;
		}
	}
	if(dir)
	{ // Turn clockwise
		actor->angle += delta;
	}
	else
	{ // Turn counter clockwise
		actor->angle -= delta;
	}
	angle = actor->angle>>ANGLETOFINESHIFT;
	actor->momx = FixedMul(actor->info->speed, finecosine[angle]);
	actor->momy = FixedMul(actor->info->speed, finesine[angle]);
	{ // Need to seek vertically
		dist = P_AproxDistance(target->x-actor->x, target->y-actor->y);
		dist = dist/actor->info->speed;
		if(dist < 1)
		{
			dist = 1;
		}
		actor->momz = (target->z + (target->height/2) - actor->z)/dist;
	}
}

void A_FindTracer(mobj_t* actor) {
	angle_t fov;
	uint rangeblocks;

	if (actor->tracer) {
		return;
	}

	if (!actor->state->args[0]) {
		return;
	}

	fov = FixedToAngle(actor->state->args[0]);
	rangeblocks = actor->state->args[1];

	actor->tracer = P_RoughTargetSearch(actor, fov, rangeblocks);
}

void A_ClearTracer(mobj_t* mo) {
	mo->tracer = NULL;
}


}; // extern "C"

