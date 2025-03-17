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
#include "z_zone.h"
#include "m_random.h"

#include "doomdef.h"
#include "p_local.h"
#include "sounds.h"

#include "st_stuff.h"
#include "hu_stuff.h"

#include "s_sound.h"

#include "doomstat.h"

#include <variant>

extern bool globalNetworking;

extern idCVar cl_freelook;

void G_PlayerReborn (int player);
void P_SpawnMapThing (mapthing_t*	mthing);


//
// P_SetMobjState
// Returns true if the mobj is still present.
//

qboolean
P_SetMobjState
( mobj_t*	mobj,
 int	state )
{
	state_t*	st;

	do
	{
		if (state == S_NULL)
		{
			mobj->state = (state_t *) S_NULL;
			P_RemoveMobj (mobj);
			return false;
		}
		st = &::g->states[state];
		mobj->state = st;
		mobj->tics = st->tics;
		mobj->sprite = st->sprite;
		mobj->frame = st->frame;

		// Modified handling.
		// Call action functions when the state is set
		if (const actionf_p2* action_p2 = std::get_if<actionf_p2>(&st->action))
			(*action_p2)(mobj, NULL);

		if (const actionf_p1* action_p1 = std::get_if<actionf_p1>(&st->action))
			(*action_p1)(mobj);

		state = st->nextstate;
	} while (!mobj->tics);

	return true;
}


//
// P_ExplodeMissile  
//
void P_ExplodeMissile (mobj_t* mo)
{
	mo->momx = mo->momy = mo->momz = 0;

	P_SetMobjState (mo, mobjinfo[mo->type].deathstate);

	mo->tics -= P_Random()&3;

	if (mo->tics < 1)
		mo->tics = 1;

	mo->flags &= ~MF_MISSILE;

	if (mo->info->deathsound)
		S_StartSound (mo, mo->info->deathsound);
}


//
// P_XYMovement  
//

void P_XYMovement (mobj_t* mo) 
{ 	
	fixed_t 	ptryx;
	fixed_t	ptryy;
	player_t*	player;
	fixed_t	xmove;
	fixed_t	ymove;
	fixed_t   oldx,oldy; // phares 9/10/98: reducing bobbing/momentum on ice
                       // when up against walls

	if (!mo->momx && !mo->momy)
	{
		if (mo->flags & MF_SKULLFLY)
		{
			// the skull slammed into something
			mo->flags &= ~MF_SKULLFLY;
			mo->momx = mo->momy = mo->momz = 0;

			P_SetMobjState (mo, mo->info->spawnstate);
		}
		return;
	}

	player = mo->player;

	if (mo->momx > MAXMOVE)
		mo->momx = MAXMOVE;
	else if (mo->momx < -MAXMOVE)
		mo->momx = -MAXMOVE;

	if (mo->momy > MAXMOVE)
		mo->momy = MAXMOVE;
	else if (mo->momy < -MAXMOVE)
		mo->momy = -MAXMOVE;

	xmove = mo->momx;
	ymove = mo->momy;

	oldx = mo->x; // phares 9/10/98: new code to reduce bobbing/momentum
  	oldy = mo->y; // when on ice & up against wall. These will be compared
                // to your x,y values later to see if you were able to move

	do
	{
		if (xmove > MAXMOVE/2 || ymove > MAXMOVE/2)
		{
			ptryx = mo->x + xmove/2;
			ptryy = mo->y + ymove/2;
			xmove >>= 1;
			ymove >>= 1;
		}
		else
		{
			ptryx = mo->x + xmove;
			ptryy = mo->y + ymove;
			xmove = ymove = 0;
		}

		if (!P_TryMove (mo, ptryx, ptryy))
		{
			// blocked move
			if (mo->player)
			{	// try to slide along it
				P_SlideMove (mo);
			}
			else if (mo->flags & MF_MISSILE)
			{
				// explode a missile
				if (::g->ceilingline &&
					::g->ceilingline->backsector &&
					::g->ceilingline->backsector->ceilingpic == ::g->skyflatnum)
				{
					// Hack to prevent missiles exploding
					// against the sky.
					// Does not handle sky floors.
					P_RemoveMobj (mo);
					return;
				}
				P_ExplodeMissile (mo);
			}
			else
				mo->momx = mo->momy = 0;
		}
	} while (xmove || ymove);

	// slow down
	if (player && player->cheats & CF_NOMOMENTUM)
	{
		// debug option for no sliding at all
		mo->momx = mo->momy = 0;
		return;
	}

	if (mo->flags & (MF_MISSILE | MF_SKULLFLY) )
		return; 	// no friction for missiles ever

	if (mo->z > mo->floorz)
		return;		// no friction when airborne

	if (mo->flags & MF_CORPSE)
	{
		// do not stop sliding
		//  if halfway off a step with some momentum
		if (mo->momx > FRACUNIT/4
			|| mo->momx < -FRACUNIT/4
			|| mo->momy > FRACUNIT/4
			|| mo->momy < -FRACUNIT/4)
		{
			if (mo->floorz != mo->subsector->sector->floorheight)
				return;
		}
	}

	if (mo->momx > -STOPSPEED
		&& mo->momx < STOPSPEED
		&& mo->momy > -STOPSPEED
		&& mo->momy < STOPSPEED
		&& (!player
		|| (player->cmd.forwardmove== 0
		&& player->cmd.sidemove == 0 ) ) )
	{
		// if in a walking frame, stop moving
		if ( player&&(unsigned)((player->mo->state - ::g->states.data())- S_PLAY_RUN1) < 4)
			P_SetMobjState (player->mo, S_PLAY);

		mo->momx = 0;
		mo->momy = 0;
	}
	else
	{
		// phares 3/17/98
    // Friction will have been adjusted by friction thinkers for icy
    // or muddy floors. Otherwise it was never touched and
    // remained set at ORIG_FRICTION

    // phares 9/10/98: reduce bobbing/momentum when on ice & up against wall

    if ((oldx == mo->x) && (oldy == mo->y)) // Did you go anywhere?
      { // No. Use original friction. This allows you to not bob so much
        // if you're on ice, but keeps enough momentum around to break free
        // when you're mildly stuck in a wall.
      mo->momx = FixedMul(mo->momx,ORIG_FRICTION);
      mo->momy = FixedMul(mo->momy,ORIG_FRICTION);
      }
    else
      { // Yes. Use stored friction.
      mo->momx = FixedMul(mo->momx,mo->friction);
      mo->momy = FixedMul(mo->momy,mo->friction);
      }
    mo->friction = ORIG_FRICTION; // reset to normal for next tic
	}
}

//
// P_ZMovement
//
void P_ZMovement (mobj_t* mo)
{
	fixed_t	dist;
	fixed_t	delta;

	// check for smooth step up
	if (mo->player && mo->z < mo->floorz)
	{
		mo->player->viewheight -= mo->floorz-mo->z;

		mo->player->deltaviewheight
			= (VIEWHEIGHT - mo->player->viewheight)>>3;
	}

	// adjust height
	mo->z += mo->momz;

	if ( mo->flags & MF_FLOAT
		&& mo->target)
	{
		// float down towards target if too close
		if ( !(mo->flags & MF_SKULLFLY)
			&& !(mo->flags & MF_INFLOAT) )
		{
			dist = P_AproxDistance (mo->x - mo->target->x,
				mo->y - mo->target->y);

			delta =(mo->target->z + (mo->height>>1)) - mo->z;

			if (delta<0 && dist < -(delta*3) )
				mo->z -= FLOATSPEED;
			else if (delta>0 && dist < (delta*3) )
				mo->z += FLOATSPEED;			
		}

	}

	// clip movement
	if (mo->z <= mo->floorz)
	{
		// hit the floor

		// Note (id):
		//  somebody left this after the setting momz to 0,
		//  kinda useless there.
		if (mo->flags & MF_SKULLFLY)
		{
			// the skull slammed into something
			mo->momz = -mo->momz;
		}

		if (mo->momz < 0)
		{
			if (mo->player
				&& mo->momz < -GRAVITY*8)	
			{
				// Squat down.
				// Decrease ::g->viewheight for a moment
				// after hitting the ground (hard),
				// and utter appropriate sound.
				mo->player->deltaviewheight = mo->momz>>3;
				if (globalNetworking || (mo->player == &::g->players[::g->consoleplayer]))
					S_StartSound (mo, sfx_oof);
			}
			mo->momz = 0;
		}
		mo->z = mo->floorz;

		if ( (mo->flags & MF_MISSILE)
			&& !(mo->flags & MF_NOCLIP) )
		{
			P_ExplodeMissile (mo);
			return;
		}
	}
	else if (! (mo->flags & MF_NOGRAVITY) )
	{
		fixed_t tGravity = GRAVITY;
		if (mo->flags2 & MF2_LOGRAV) {
			tGravity = GRAVITY/8;
		}

		if (mo->momz == 0)
			mo->momz = -tGravity*2;
		else
			mo->momz -= tGravity;
	}

	if (mo->z + mo->height > mo->ceilingz)
	{
		// hit the ceiling
		if (mo->momz > 0)
			mo->momz = 0;
		{
			mo->z = mo->ceilingz - mo->height;
		}

		if (mo->flags & MF_SKULLFLY)
		{	// the skull slammed into something
			mo->momz = -mo->momz;
		}

		if ( (mo->flags & MF_MISSILE)
			&& !(mo->flags & MF_NOCLIP) )
		{
			P_ExplodeMissile (mo);
			return;
		}
	}
} 



//
// P_NightmareRespawn
//
void
P_NightmareRespawn (mobj_t* mobj)
{
	fixed_t		x;
	fixed_t		y;
	fixed_t		z; 
	subsector_t*	ss; 
	mobj_t*		mo;
	mapthing_t*		mthing;

	x = mobj->spawnpoint.x << FRACBITS; 
	y = mobj->spawnpoint.y << FRACBITS; 

	// somthing is occupying it's position?
	if (!P_CheckPosition (mobj, x, y) ) 
		return;	// no respwan

	// spawn a teleport fog at old spot
	// because of removal of the body?
	mo = P_SpawnMobj (mobj->x,
		mobj->y,
		mobj->subsector->sector->floorheight , MT_TFOG); 
	// initiate teleport sound
	S_StartSound (mo, sfx_telept);

	// spawn a teleport fog at the new spot
	ss = R_PointInSubsector (x,y); 

	mo = P_SpawnMobj (x, y, ss->sector->floorheight , MT_TFOG); 

	S_StartSound (mo, sfx_telept);

	// spawn the new monster
	mthing = &mobj->spawnpoint;

	// spawn it
	if (mobj->info->flags & MF_SPAWNCEILING)
		z = ONCEILINGZ;
	else
		z = ONFLOORZ;

	// inherit attributes from deceased one
	mo = P_SpawnMobj (x,y,z, mobj->type);
	mo->spawnpoint = mobj->spawnpoint;	
	mo->angle = ANG45 * (mthing->angle/45);

	if (mthing->options & MTF_AMBUSH)
		mo->flags |= MF_AMBUSH;

	mo->reactiontime = 18;

	// remove the old monster,
	P_RemoveMobj (mobj);
}


//
// P_MobjThinker
//
void P_MobjThinker (mobj_t* mobj)
{
	// momentum movement
	if (mobj->momx
		|| mobj->momy
		|| (mobj->flags&MF_SKULLFLY) )
	{
		P_XYMovement (mobj);

		// FIXME: decent NOP/NULL/Nil function pointer please.
		bool skip = false;
		if (const actionf_v* currentAction = std::get_if<actionf_v>(&mobj->thinker.function)) {
			skip = (*currentAction) == (actionf_v)(-1);
		}
		if (skip)
			return;		// mobj was removed
	}
	if ( (mobj->z != mobj->floorz)
		|| mobj->momz )
	{
		P_ZMovement (mobj);
		bool skip = false;
		if (const actionf_v* currentAction = std::get_if<actionf_v>(&mobj->thinker.function)) {
			skip = (*currentAction) == (actionf_v)(-1);
		}
		// FIXME: decent NOP/NULL/Nil function pointer please.
		if (skip)
			return;		// mobj was removed
	}

	{
    sector_t* sector = mobj->subsector->sector;

    if (
      sector->special & KILL_MONSTERS_MASK &&
      mobj->z == mobj->floorz &&
      mobj->player == NULL &&
      mobj->flags & MF_SHOOTABLE &&
      !(mobj->flags & MF_FLOAT)
    )
    {
      P_DamageMobj(mobj, NULL, NULL, 10000);

      // must have been removed
	  bool skip = true;
	  if (const actionf_p1* thAction = std::get_if<actionf_p1>(&mobj->thinker.function)) {
		  skip = (*thAction) != (actionf_p1)P_MobjThinker;
	  }
	  if (skip)
        return;
    }
  }


	// cycle through states,
	// calling action functions at transitions
	if (mobj->tics != -1)
	{
		mobj->tics--;

		// you can cycle through multiple states in a tic
		if (!mobj->tics)
			if (!P_SetMobjState (mobj, mobj->state->nextstate) )
				return;		// freed itself
	}
	else
	{
		// check for nightmare respawn
		if (! (mobj->flags & MF_COUNTKILL) )
			return;

		if (!::g->respawnmonsters)
			return;

		mobj->movecount++;

		if (mobj->movecount < 12*TICRATE)
			return;

		if ( ::g->leveltime&31 )
			return;

		if (P_Random () > 4)
			return;

		P_NightmareRespawn (mobj);
	}

}


//
// P_SpawnMobj
//
mobj_t*
P_SpawnMobj
( fixed_t	x,
 fixed_t	y,
 fixed_t	z,
 int	type )
{
	mobj_t*	mobj;
	state_t*	st;
	const mobjinfo_t*	info;

	mobj = (mobj_t*)DoomLib::Z_Malloc(sizeof(*mobj), PU_MOBJ, NULL);
	memset (mobj, 0, sizeof (*mobj));
	info = &mobjinfo[type];

	mobj->type = type;
	mobj->info = info;
	mobj->x = x;
	mobj->y = y;
	mobj->radius = info->radius;
	mobj->height = info->height;
	mobj->flags = info->flags;
	mobj->flags2 = info->flags2;
	mobj->health = info->spawnhealth;
	//mobj->touching_sectorlist = NULL; // NULL head of sector list // phares 3/13/98

	if (::g->gameskill != sk_nightmare)
		mobj->reactiontime = info->reactiontime;

	mobj->lastlook = P_Random () % MAXPLAYERS;
	// do not set the state with P_SetMobjState,
	// because action routines can not be called yet
	st = &::g->states[info->spawnstate];

	mobj->state = st;
	mobj->tics = st->tics;
	mobj->sprite = st->sprite;
	mobj->frame = st->frame;

	// set subsector and/or block links
	P_SetThingPosition (mobj);
	if (mobj->subsector->sector) {
		mobj->floorz = mobj->subsector->sector->floorheight;
		mobj->ceilingz = mobj->subsector->sector->ceilingheight;

		if (z == ONFLOORZ)
			mobj->z = mobj->floorz;
		else if (z == ONCEILINGZ)
			mobj->z = mobj->ceilingz - mobj->info->height;
		else
			mobj->z = z;

	}

	mobj->friction = ORIG_FRICTION;
	mobj->thinker.function = (actionf_p1)P_MobjThinker;

	P_AddThinker (&mobj->thinker);

	return mobj;
}


//
// P_RemoveMobj
//


void P_RemoveMobj (mobj_t* mobj)
{
	if ((mobj->flags & MF_SPECIAL)
		&& !(mobj->flags & MF_DROPPED)
		&& (mobj->type != MT_INV)
		&& (mobj->type != MT_INS))
	{
		::g->itemrespawnque[::g->iquehead] = mobj->spawnpoint;
		::g->itemrespawntime[::g->iquehead] = ::g->leveltime;
		::g->iquehead = (::g->iquehead+1)&(ITEMQUESIZE-1);

		// lose one off the end?
		if (::g->iquehead == ::g->iquetail)
			::g->iquetail = (::g->iquetail+1)&(ITEMQUESIZE-1);
	}

	// unlink from sector and block lists
	P_UnsetThingPosition (mobj);

	// stop any playing sound
	//S_StopSound (mobj);

	// free block
	P_RemoveThinker ((thinker_t*)mobj);
}




//
// P_RespawnSpecials
//
void P_RespawnSpecials (void)
{
	fixed_t		x;
	fixed_t		y;
	fixed_t		z;

	subsector_t*	ss; 
	mobj_t*		mo;
	mapthing_t*		mthing;

	int			i;

	// only respawn items in ::g->deathmatch
	if (::g->deathmatch != 2)
		return;	// 

	// nothing left to respawn?
	if (::g->iquehead == ::g->iquetail)
		return;		

	// wait at least 30 seconds
	if (::g->leveltime - ::g->itemrespawntime[::g->iquetail] < 30*TICRATE)
		return;			

	mthing = &::g->itemrespawnque[::g->iquetail];

	x = mthing->x << FRACBITS; 
	y = mthing->y << FRACBITS; 

	// spawn a teleport fog at the new spot
	ss = R_PointInSubsector (x,y); 
	mo = P_SpawnMobj (x, y, ss->sector->floorheight , MT_IFOG); 
	S_StartSound (mo, sfx_itmbk);

	// find which type to spawn
	for (i=0 ; i< (int)mobjinfo.size(); i++)
	{
		if (mthing->type == mobjinfo[i].doomednum)
			break;
	}

	// spawn it
	if (mobjinfo[i].flags & MF_SPAWNCEILING)
		z = ONCEILINGZ;
	else
		z = ONFLOORZ;

	mo = (mobj_t*)P_SpawnMobj (x,y,z, (mobjtype_t)i);
	mo->spawnpoint = *mthing;	
	mo->angle = ANG45 * (mthing->angle/45);

	// pull it from the que
	::g->iquetail = (::g->iquetail+1)&(ITEMQUESIZE-1);
}




//
// P_SpawnPlayer
// Called when a player is spawned on the level.
// Most of the player structure stays unchanged
//  between levels.
//
void P_SpawnPlayer (mapthing_t* mthing)
{
	player_t*		p;
	fixed_t		x;
	fixed_t		y;
	fixed_t		z;

	mobj_t*		mobj;

	int			i;

	// not playing?
	if (!::g->playeringame[mthing->type-1])
		return;					

	p = &::g->players[mthing->type-1];

	if (p->playerstate == PST_REBORN)
		G_PlayerReborn (mthing->type-1);

	x 		= mthing->x << FRACBITS;
	y 		= mthing->y << FRACBITS;
	z		= ONFLOORZ;
	mobj	= P_SpawnMobj (x,y,z, MT_PLAYER);

	// set color translations for player ::g->sprites
	if (mthing->type > 1)		
		mobj->flags |= (mthing->type-1)<<MF_TRANSSHIFT;

	mobj->angle	= ANG45 * (mthing->angle/45);
	mobj->player = p;
	mobj->health = p->health;

	p->mo = mobj;
	p->playerstate = PST_LIVE;	
	p->refire = 0;
	p->message = NULL;
	p->damagecount = 0;
	p->bonuscount = 0;
	p->extralight = 0;
	p->fixedcolormap = 0;
	p->viewheight = VIEWHEIGHT;

	// setup gun psprite
	P_SetupPsprites (p);

	// give all cards in death match mode
	if (::g->deathmatch)
		for (i=0 ; i<NUMCARDS ; i++)
			p->cards[i] = true;

	if (mthing->type-1 == ::g->consoleplayer)
	{
		// wake up the status bar
		ST_Start ();
		// wake up the heads up text
		HU_Start ();		
	}

	// Give him everything is Give All is on.
	if( p->cheats & CF_GIVEALL ) {
		 p->armorpoints = ::g->marmor;
		 p->armortype = ::g->bart;

		int i_;
		for (i_ = 0; i_ < NUMWEAPONS; i_++) {
			p->weaponowned[i_] = true;
			if (::g->weaponcond[i_] != 2) { //GK: Everytime you get a weapon record that
				::g->weaponcond[i_] = 1;
			}
		}
		for (i_=0;i_<NUMAMMO;i_++)
			 p->ammo[i_] =  p->maxammo[i_];

		for (i_=0;i_<NUMCARDS;i_++)
			 p->cards[i_] = true;
	}

}


//
// P_SpawnMapThing
// The fields of the mapthing should
// already be in host byte order.
//
void P_SpawnMapThing (mapthing_t* mthing)
{
	int			i;
	int			bit;
	mobj_t*		mobj;
	fixed_t		x;
	fixed_t		y;
	fixed_t		z;

	// count ::g->deathmatch start positions
	if (mthing->type == 11)
	{
		if (::g->deathmatch_p < &::g->deathmatchstarts[10])
		{
			memcpy (::g->deathmatch_p, mthing, sizeof(*mthing));
			::g->deathmatch_p++;
		}
		return;
	}

	// check for ::g->players specially
	if (mthing->type <= 4)
	{
		// save spots for respawning in network games
		::g->playerstarts[mthing->type-1] = *mthing;
		if (!::g->deathmatch)
			P_SpawnPlayer (mthing);

		return;
	}

	// check for apropriate skill level
	if (!::g->netgame && (mthing->options & 16) )
		return;

	if (::g->gameskill == sk_baby)
		bit = 1;
	else if (::g->gameskill == sk_nightmare || ::g->gameskill == sk_masochism)
		bit = 4;
	else
		bit = 1<<(::g->gameskill-1);

	if (!(mthing->options & bit) )
		return;

	// find which type to spawn
	for (i=0 ; i< (int)mobjinfo.size() ; i++)
		if (mthing->type == mobjinfo[i].doomednum)
			break;

	//if ( i==NUMMOBJTYPES ) {
		//printf( "P_SpawnMapThing: Unknown type %i at (%i, %i)", mthing->type, mthing->x, mthing->y);
	//	return;
		//I_Error ("P_SpawnMapThing: Unknown type %i at (%i, %i)",
		//mthing->type,
		//mthing->x, mthing->y);
//	}

	// don't spawn keycards and ::g->players in ::g->deathmatch
	if (::g->deathmatch && mobjinfo[i].flags & MF_NOTDMATCH)
		return;

	// don't spawn any monsters if -::g->nomonsters
	if (::g->nomonsters
		&& ( i == MT_SKULL
		|| (mobjinfo[i].flags & MF_COUNTKILL)) )
	{
		return;
	}

	// spawn it
	x = mthing->x << FRACBITS;
	y = mthing->y << FRACBITS;

	if (mobjinfo[i].flags & MF_SPAWNCEILING)
		z = ONCEILINGZ;
	else
		z = ONFLOORZ;

	mobj = (mobj_t*)P_SpawnMobj (x,y,z, i);
	mobj->spawnpoint = *mthing;

	if (mobj->tics > 0)
		mobj->tics = 1 + (P_Random () % mobj->tics);
	if (mobj->flags & MF_COUNTKILL)
		::g->totalkills++;
	if (mobj->flags & MF_COUNTITEM)
		::g->totalitems++;

	mobj->angle = ANG45 * (mthing->angle/45);
	if (mthing->options & MTF_AMBUSH)
		mobj->flags |= MF_AMBUSH;
}



//
// GAME SPAWN FUNCTIONS
//


//
// P_SpawnPuff
//

void
P_SpawnPuff
( fixed_t	x,
 fixed_t	y,
 fixed_t	z )
{
	mobj_t*	th;

	z += ((P_Random()-P_Random())<<10);

	th = P_SpawnMobj (x,y,z, MT_PUFF);
	th->momz = FRACUNIT;
	th->tics -= P_Random()&3;

	if (th->tics < 1)
		th->tics = 1;

	// don't make punches spark on the wall
	if (::g->attackrange == MELEERANGE) {

		P_SetMobjState (th, S_PUFF3);
		
	}
}



//
// P_SpawnBlood
// 
void
P_SpawnBlood
( fixed_t	x,
 fixed_t	y,
 fixed_t	z,
 int		damage )
{
	mobj_t*	th;

	z += ((P_Random()-P_Random())<<10);
	th = P_SpawnMobj (x,y,z, MT_BLOOD);
	th->momz = FRACUNIT*2;
	th->tics -= P_Random()&3;

	if (th->tics < 1)
		th->tics = 1;

	if (damage <= 12 && damage >= 9)
		P_SetMobjState (th,S_BLOOD2);
	else if (damage < 9)
		P_SetMobjState (th,S_BLOOD3);
}



//
// P_CheckMissileSpawn
// Moves the missile forward a bit
//  and possibly explodes it right there.
//
void P_CheckMissileSpawn (mobj_t* th)
{
	th->tics -= P_Random()&3;
	if (th->tics < 1)
		th->tics = 1;

	// move a little forward so an angle can
	// be computed if it immediately explodes
	th->x += (th->momx>>1);
	th->y += (th->momy>>1);
	th->z += (th->momz>>1);

	if (!P_TryMove (th, th->x, th->y))
		P_ExplodeMissile (th);
}


//
// P_SpawnMissile
//
mobj_t*
P_SpawnMissile
( mobj_t*	source,
 mobj_t*	dest,
 int	type )
{
	mobj_t*	th;
	angle_t	an;
	int		dist;

	th = P_SpawnMobj (source->x,
		source->y,
		source->z + 4*8*FRACUNIT, type);

	if (th->info->seesound)
		S_StartSound (th, th->info->seesound);

	th->target = source;	// where it came from
	an = R_PointToAngle2 (source->x, source->y, dest->x, dest->y);	

	// fuzzy player
	if (dest->flags & MF_SHADOW)
		an += (P_Random()-P_Random())<<20;	

	th->angle = an;
	an >>= ANGLETOFINESHIFT;
	th->momx = FixedMul (th->info->speed, finecosine[an]);
	th->momy = FixedMul (th->info->speed, finesine[an]);

	dist = P_AproxDistance (dest->x - source->x, dest->y - source->y);
	dist = dist / th->info->speed;

	if (dist < 1)
		dist = 1;

	th->momz = (dest->z - source->z) / dist;
	P_CheckMissileSpawn (th);

	return th;
}


//
// P_SpawnPlayerMissile
// Tries to aim at a nearby monster
//
mobj_t*
P_SpawnPlayerMissile
( mobj_t*	source,
 int	type )
{
	mobj_t*	th;
	angle_t	an;

	fixed_t	x;
	fixed_t	y;
	fixed_t	z;
	fixed_t	slope;

	// see which target is to be aimed at
	an = source->angle;
	slope = P_AimLineAttack (source, an, 16*64*FRACUNIT);

	if (!::g->linetarget)
	{
		an += 1<<26;
		slope = P_AimLineAttack (source, an, 16*64*FRACUNIT);

		if (!::g->linetarget)
		{
			an -= 2<<26;
			slope = P_AimLineAttack (source, an, 16*64*FRACUNIT);
		}
	}
	if (cl_freelook.GetBool() && !::g->demorecording && ::g->gamestate != GS_DEMOLEVEL)
	{
		if ((game->GetCVarBool("aa_targetAimAssistEnable") && !::g->linetarget) || !game->GetCVarBool("aa_targetAimAssistEnable")) {
			an = source->angle;
			slope = -(((::g->mouseposy) << FRACBITS) / 473); //GK: Taken from Heretic source and altered in order to work with idTech 5 input
		}
	}

	x = source->x;
	y = source->y;
	z = source->z + 4*8*FRACUNIT - (((::g->mouseposy) << FRACBITS) / 473);

	th = P_SpawnMobj (x,y,z, type);

	if (th->info->seesound && (source->player == &::g->players[::g->consoleplayer]) ) {
		S_StartSound (th, th->info->seesound);
	}

	th->target = source;
	th->angle = an;
	th->momx = FixedMul( th->info->speed,
		finecosine[an>>ANGLETOFINESHIFT]);
	th->momy = FixedMul( th->info->speed,
		finesine[an>>ANGLETOFINESHIFT]);
	th->momz = FixedMul( th->info->speed, slope);

	P_CheckMissileSpawn (th);
	return th;
}

extern "C" {
void A_SpawnObject(mobj_t* mo) {
	if (!mo->state->args[0])
		return;

	mobjtype_t type = (mobjtype_t)(mo->state->args[0] - 1);
	angle_t angle = mo->angle + (uint)(((int64)mo->state->args[1] << 16) / 360);
	int shiftedAngle = angle >> ANGLETOFINESHIFT;
	int xOffset = FixedMul(mo->state->args[2], finecosine[shiftedAngle]) - FixedMul(mo->state->args[3], finesine[shiftedAngle]);
	int yOffset = FixedMul(mo->state->args[2], finesine[shiftedAngle]) + FixedMul(mo->state->args[3], finecosine[shiftedAngle]);
	int zOffset = mo->state->args[4];
	int xVelocity = FixedMul(mo->state->args[5], finecosine[shiftedAngle]) - FixedMul(mo->state->args[6], finesine[shiftedAngle]);
	int yVelocity = FixedMul(mo->state->args[5], finesine[shiftedAngle]) + FixedMul(mo->state->args[6], finecosine[shiftedAngle]);
	int zVelocity = mo->state->args[7];

	mobj_t* child = P_SpawnMobj(mo->x + xOffset, mo->y + yOffset, mo->z + zOffset, type);
	if (!child)
		return;

	child->angle = angle;
	child->momx = xVelocity;
	child->momy = yVelocity;
	child->momz = zVelocity;

	if (child->flags & MF_MISSILE) {
		if (mo->flags & MF_MISSILE) {
			child->target = mo->target;
			child->tracer = mo->tracer;
		} else {
			child->target = mo;
			child->tracer = mo->target;
		}
	}

}
}
