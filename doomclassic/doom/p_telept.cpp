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



#include "doomdef.h"

#include "s_sound.h"

#include "p_local.h"


// Data.
#include "sounds.h"

// State.
#include "r_state.h"

#include "d_exp.h"

//
// TELEPORTATION
//
int
EV_Teleport
( line_t*	line,
  int		side,
  mobj_t*	thing )
{
    int		i;
    int		tag;
    mobj_t*	m;
    mobj_t*	fog;
    unsigned	an;
    thinker_t*	thinker;
    sector_t*	sector;
    fixed_t	oldx;
    fixed_t	oldy;
    fixed_t	oldz;

    // don't teleport missiles
    if (thing->flags & MF_MISSILE)
	return 0;		

    // Don't teleport if hit back of line,
    //  so you can get out of teleporter.
    if (side == 1)		
	return 0;	

    
    tag = line->tag;
	if (::g->gamemission == doom2 && ::g->gamemap == 33 ) {//GK: A small hack in order to fix doom 2 Map 33 bug.
		if (line->tag == 0) {
			tag = 41;
		}
	}
	int ok = 0;
	if (::g->map) {
		ok = ::g->maps[::g->map - 1].otel;
	}
	if (::g->gamemission == pack_custom && ok) {
		if (line->tag == 0) {
			tag = ok;
		}
	}
    for (i = 0; i < ::g->numsectors; i++)
    {
	if (::g->sectors[ i ].tag == tag )
	{
	    thinker = ::g->thinkercap.next;
	    for (thinker = ::g->thinkercap.next;
		 thinker != &::g->thinkercap;
		 thinker = thinker->next)
	    {
		// not a mobj
		if (thinker->function.acp1 != (actionf_p1)P_MobjThinker)
		    continue;	

		m = (mobj_t *)thinker;
		
		// not a teleportman
		if (m->type != MT_TELEPORTMAN )
		    continue;		

		sector = m->subsector->sector;
		// wrong sector
		if (sector-::g->sectors != i )
		    continue;	

		oldx = thing->x;
		oldy = thing->y;
		oldz = thing->z;
				
		if (!P_TeleportMove (thing, m->x, m->y))
		    return 0;
		
		thing->z = thing->floorz;  //fixme: not needed?
		if (thing->player)
		    thing->player->viewz = thing->z+thing->player->viewheight;
				
		// spawn teleport fog at source and destination
		fog = P_SpawnMobj (oldx, oldy, oldz, MT_TFOG);
		S_StartSound (fog, sfx_telept);
		an = m->angle >> ANGLETOFINESHIFT;
		fog = P_SpawnMobj (m->x+20*finecosine[an], m->y+20*finesine[an]
				   , thing->z, MT_TFOG);

		// emit sound, where?
		S_StartSound (fog, sfx_telept);
		
		// don't move for a bit
		if (thing->player)
		    thing->reactiontime = 18;	

		thing->angle = m->angle;
		thing->momx = thing->momy = thing->momz = 0;
		return 1;
	    }	
	}
    }
    return 0;
}


//
// Silent TELEPORTATION, by Lee Killough
// Primarily for rooms-over-rooms etc.
//

int EV_SilentTeleport(line_t *line, int side, mobj_t *thing)
{
	int       i;
	mobj_t    *m;
	thinker_t *th;

	// don't teleport missiles
	// Don't teleport if hit back of line,
	// so you can get out of teleporter.

	if (side || thing->flags & MF_MISSILE)
		return 0;

	for (i = -1; (i = P_FindSectorFromLineTag(line, i)) >= 0;)
		for (th = ::g->thinkercap.next; th != &::g->thinkercap; th = th->next)
			if (th->function.acp1 == (actionf_p1)P_MobjThinker &&
				(m = (mobj_t *)th)->type == MT_TELEPORTMAN &&
				m->subsector->sector - ::g->sectors == i)
			{
				// Height of thing above ground, in case of mid-air teleports:
				fixed_t z = thing->z - thing->floorz;

				// Get the angle between the exit thing and source linedef.
				// Rotate 90 degrees, so that walking perpendicularly across
				// teleporter linedef causes thing to exit in the direction
				// indicated by the exit thing.
				angle_t angle =
					R_PointToAngle2(0, 0, line->dx, line->dy) - m->angle + ANG90;

				// Sine, cosine of angle adjustment
				fixed_t s = finesine[angle >> ANGLETOFINESHIFT];
				fixed_t c = finecosine[angle >> ANGLETOFINESHIFT];

				// Momentum of thing crossing teleporter linedef
				fixed_t momx = thing->momx;
				fixed_t momy = thing->momy;

				// Whether this is a player, and if so, a pointer to its player_t
				player_t *player = thing->player;

				// Attempt to teleport, aborting if blocked
				if (!P_TeleportMove(thing, m->x, m->y))
					return 0;

				// Rotate thing according to difference in angles
				thing->angle += angle;

				// Adjust z position to be same height above ground as before
				thing->z = z + thing->floorz;

				// Rotate thing's momentum to come out of exit just like it entered
				thing->momx = FixedMul(momx, c) - FixedMul(momy, s);
				thing->momy = FixedMul(momy, c) + FixedMul(momx, s);

				// Adjust player's view, in case there has been a height change
				// Voodoo dolls are excluded by making sure player->mo == thing.
				if (player && player->mo == thing)
				{
					// Save the current deltaviewheight, used in stepping
					fixed_t deltaviewheight = player->deltaviewheight;

					// Clear deltaviewheight, since we don't want any changes
					player->deltaviewheight = 0;

					// Set player's view according to the newly set parameters
					//P_CalcHeight(player);

					// Reset the delta to have the same dynamics as before
					player->deltaviewheight = deltaviewheight;
				}
				return 1;
			}
	return 0;
}
//
// Silent linedef-based TELEPORTATION, by Lee Killough
// Primarily for rooms-over-rooms etc.
// This is the complete player-preserving kind of teleporter.
// It has advantages over the teleporter with thing exits.
//

// maximum fixed_t units to move object to avoid hiccups
#define FUDGEFACTOR 10

int EV_SilentLineTeleport(line_t *line, int side, mobj_t *thing,
	qboolean reverse)
{
	int i;
	line_t *l;

	if (side || thing->flags & MF_MISSILE)
		return 0;

	for (i = -1; (i = P_FindLineFromLineTag(line, i)) >= 0;)
		if ((l = ::g->lines + i) != line && l->backsector)
		{
			// Get the thing's position along the source linedef
			fixed_t pos = abs(line->dx) > abs(line->dy) ?
				FixedDiv(thing->x - line->v1->x, line->dx) :
				FixedDiv(thing->y - line->v1->y, line->dy);

			// Get the angle between the two linedefs, for rotating
			// orientation and momentum. Rotate 180 degrees, and flip
			// the position across the exit linedef, if reversed.
			angle_t angle = (reverse ? pos = FRACUNIT - pos, 0 : ANG180) +
				R_PointToAngle2(0, 0, l->dx, l->dy) -
				R_PointToAngle2(0, 0, line->dx, line->dy);

			// Interpolate position across the exit linedef
			fixed_t x = l->v2->x - FixedMul(pos, l->dx);
			fixed_t y = l->v2->y - FixedMul(pos, l->dy);

			// Sine, cosine of angle adjustment
			fixed_t s = finesine[angle >> ANGLETOFINESHIFT];
			fixed_t c = finecosine[angle >> ANGLETOFINESHIFT];

			// Maximum distance thing can be moved away from interpolated
			// exit, to ensure that it is on the correct side of exit linedef
			int fudge = FUDGEFACTOR;

			// Whether this is a player, and if so, a pointer to its player_t.
			// Voodoo dolls are excluded by making sure thing->player->mo==thing.
			player_t *player = thing->player && thing->player->mo == thing ?
				thing->player : NULL;

			// Whether walking towards first side of exit linedef steps down
			int stepdown =
				l->frontsector->floorheight < l->backsector->floorheight;

			// Height of thing above ground
			fixed_t z = thing->z - thing->floorz;

			// Side to exit the linedef on positionally.
			//
			// Notes:
			//
			// This flag concerns exit position, not momentum. Due to
			// roundoff error, the thing can land on either the left or
			// the right side of the exit linedef, and steps must be
			// taken to make sure it does not end up on the wrong side.
			//
			// Exit momentum is always towards side 1 in a reversed
			// teleporter, and always towards side 0 otherwise.
			//
			// Exiting positionally on side 1 is always safe, as far
			// as avoiding oscillations and stuck-in-wall problems,
			// but may not be optimum for non-reversed teleporters.
			//
			// Exiting on side 0 can cause oscillations if momentum
			// is towards side 1, as it is with reversed teleporters.
			//
			// Exiting on side 1 slightly improves player viewing
			// when going down a step on a non-reversed teleporter.

			int side_ = reverse || (player && stepdown);

			// Make sure we are on correct side of exit linedef.
			while (P_PointOnLineSide(x, y, l) != side_ && --fudge >= 0)
				if (abs(l->dx) > abs(l->dy))
					y -= l->dx < 0 != side_ ? -1 : 1;
				else
					x += l->dy < 0 != side_ ? -1 : 1;

			// Attempt to teleport, aborting if blocked
			if (!P_TeleportMove(thing, x, y))
				return 0;

			// Adjust z position to be same height above ground as before.
			// Ground level at the exit is measured as the higher of the
			// two floor heights at the exit linedef.
			thing->z = z + ::g->sides[l->sidenum[stepdown]].sector->floorheight;

			// Rotate thing's orientation according to difference in linedef angles
			thing->angle += angle;

			// Momentum of thing crossing teleporter linedef
			x = thing->momx;
			y = thing->momy;

			// Rotate thing's momentum to come out of exit just like it entered
			thing->momx = FixedMul(x, c) - FixedMul(y, s);
			thing->momy = FixedMul(y, c) + FixedMul(x, s);

			// Adjust a player's view, in case there has been a height change
			if (player)
			{
				// Save the current deltaviewheight, used in stepping
				fixed_t deltaviewheight = player->deltaviewheight;

				// Clear deltaviewheight, since we don't want any changes now
				player->deltaviewheight = 0;

				// Set player's view according to the newly set parameters
				//P_CalcHeight(player);

				// Reset the delta to have the same dynamics as before
				player->deltaviewheight = deltaviewheight;
			}

			return 1;
		}
	return 0;
}