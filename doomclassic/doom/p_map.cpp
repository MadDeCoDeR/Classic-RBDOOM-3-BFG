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

#include "m_bbox.h"
#include "m_random.h"
#include "i_system.h"

#include "doomdef.h"
#include "p_local.h"

#include "s_sound.h"

// State.
#include "doomstat.h"
#include "r_state.h"
// Data.
#include "sounds.h"

#include "Main.h"
#include "d_exp.h"

// If "floatok" true, move would be ok
// if within "tmfloorz - tmceilingz".


// keep track of the line that lowers the ceiling,
// so missiles don't explode against sky hack walls

// keep track of special ::g->lines as they are hit,
// but don't process them until the move is proven valid


extern idCVar cl_freelook;

//
// TELEPORT MOVE
// 

//
// PIT_StompThing
//
qboolean PIT_StompThing (mobj_t* thing)
{
    fixed_t	blockdist;
		
    if (!(thing->flags & MF_SHOOTABLE) )
	return true;
		
    blockdist = thing->radius + ::g->tmthing->radius;
    
    if ( abs(thing->x - ::g->tmx) >= blockdist
	 || abs(thing->y - ::g->tmy) >= blockdist )
    {
	// didn't hit it
	return true;
    }
    
    // don't clip against self
    if (thing == ::g->tmthing)
	return true;
    
	bool ok = false;
	if (::g->gamemission == pack_custom)
		if (::g->maps[::g->gamemap - 1].monstertelefrag)
			ok = true;
    // monsters don't stomp things except on boss level
    if ( !::g->tmthing->player && (::g->gamemap != 30 && !ok) )
	return false;	
		
    P_DamageMobj (thing, ::g->tmthing, ::g->tmthing, 10000);
	
    return true;
}

// P_GetMoveFactor() returns the value by which the x,y     // phares 3/19/98
// movements are multiplied to add to player movement.      //     |
//     V
int P_GetMoveFactor(mobj_t *mo)
{
	int movefactor = ORIG_FRICTION_FACTOR;

	// If the floor is icy or muddy, it's harder to get moving. This is where
	// the different friction factors are applied to 'trying to move'. In
	// p_mobj.c, the friction factors are applied as you coast and slow down.

	int momentum, friction;

	if (!::g->demoplayback &&
		!(mo->flags & (MF_NOGRAVITY | MF_NOCLIP)))
	{
		friction = mo->friction;
		if (friction == ORIG_FRICTION) // normal floor
			;
		else if (friction > ORIG_FRICTION) // ice
		{
			movefactor = mo->movefactor;
			mo->movefactor = ORIG_FRICTION_FACTOR; // reset
		}
		else // sludge
		{

			// phares 3/11/98: you start off slowly, then increase as
			// you get better footing

			momentum = (P_AproxDistance(mo->momx, mo->momy));
			movefactor = mo->movefactor;
			if (momentum > MORE_FRICTION_MOMENTUM << 2)
				movefactor <<= 3;

			else if (momentum > MORE_FRICTION_MOMENTUM << 1)
				movefactor <<= 2;

			else if (momentum > MORE_FRICTION_MOMENTUM)
				movefactor <<= 1;

			mo->movefactor = ORIG_FRICTION_FACTOR; // reset
		}
	} //     ^
	return (movefactor); //     |
} // phares 3/19/98

//
// P_TeleportMove
//
qboolean
P_TeleportMove
( mobj_t*	thing,
  fixed_t	x,
  fixed_t	y )
{
    int			xl;
    int			xh;
    int			yl;
    int			yh;
    int			bx;
    int			by;
    
    subsector_t*	newsubsec;
    
    // kill anything occupying the position
    ::g->tmthing = thing;
    ::g->tmflags = thing->flags;
	
    ::g->tmx = x;
    ::g->tmy = y;
	
    ::g->tmbbox[BOXTOP] = y + ::g->tmthing->radius;
    ::g->tmbbox[BOXBOTTOM] = y - ::g->tmthing->radius;
    ::g->tmbbox[BOXRIGHT] = x + ::g->tmthing->radius;
    ::g->tmbbox[BOXLEFT] = x - ::g->tmthing->radius;

    newsubsec = R_PointInSubsector (x,y);
    ::g->ceilingline = NULL;
    
    // The base floor/ceiling is from the subsector
    // that contains the point.
    // Any contacted ::g->lines the step closer together
    // will adjust them.
    ::g->tmfloorz = ::g->tmdropoffz = newsubsec->sector->floorheight;
    ::g->tmceilingz = newsubsec->sector->ceilingheight;
			
    ::g->validcount++;
    ::g->numspechit = 0;
    
    // stomp on any things contacted
    xl = (::g->tmbbox[BOXLEFT] - ::g->bmaporgx - MAXRADIUS)>>MAPBLOCKSHIFT;
    xh = (::g->tmbbox[BOXRIGHT] - ::g->bmaporgx + MAXRADIUS)>>MAPBLOCKSHIFT;
    yl = (::g->tmbbox[BOXBOTTOM] - ::g->bmaporgy - MAXRADIUS)>>MAPBLOCKSHIFT;
    yh = (::g->tmbbox[BOXTOP] - ::g->bmaporgy + MAXRADIUS)>>MAPBLOCKSHIFT;

    for (bx=xl ; bx<=xh ; bx++)
	for (by=yl ; by<=yh ; by++)
	    if (!P_BlockThingsIterator(bx,by,PIT_StompThing))
		return false;
    
    // the move is ok,
    // so link the thing into its new position
    P_UnsetThingPosition (thing);

    thing->floorz = ::g->tmfloorz;
    thing->ceilingz = ::g->tmceilingz;	
    thing->x = x;
    thing->y = y;

    P_SetThingPosition (thing);
	
    return true;
}


//
// MOVEMENT ITERATOR FUNCTIONS
//


//
// PIT_CheckLine
// Adjusts ::g->tmfloorz and ::g->tmceilingz as ::g->lines are contacted
//
qboolean PIT_CheckLine (line_t* ld)
{
    if (::g->tmbbox[BOXRIGHT] <= ld->bbox[BOXLEFT]
	|| ::g->tmbbox[BOXLEFT] >= ld->bbox[BOXRIGHT]
	|| ::g->tmbbox[BOXTOP] <= ld->bbox[BOXBOTTOM]
	|| ::g->tmbbox[BOXBOTTOM] >= ld->bbox[BOXTOP] )
	return true;

    if (P_BoxOnLineSide (::g->tmbbox, ld) != -1)
	return true;
		
    // A line has been hit
    
    // The moving thing's destination position will cross
    // the given line.
    // If this should not be allowed, return false.
    // If the line is special, keep track of it
    // to process later if the move is proven ok.
    // NOTE: specials are NOT sorted by order,
    // so two special ::g->lines that are only 8 pixels apart
    // could be crossed in either order.
    
    if (!ld->backsector)
	return false;		// one sided line
		
    if (!(::g->tmthing->flags & MF_MISSILE) )
    {
	if ( ld->flags & ML_BLOCKING || (::g->tmthing->player && ld->flags & ML_BLOCKPLAYERS))
	    return false;	// explicitly blocking everything

	if ( !::g->tmthing->player && ld->flags & ML_BLOCKMONSTERS || 
	(ld->flags & ML_BLOCKLANDMONSTERS && !(::g->tmthing->flags & MF_FLOAT)) )
	    return false;	// block monsters only
    }

    // set ::g->openrange, ::g->opentop, ::g->openbottom
    P_LineOpening (ld);	
	
    // adjust floor / ceiling heights
    if (::g->opentop < ::g->tmceilingz)
    {
	::g->tmceilingz = ::g->opentop;
	::g->ceilingline = ld;
    }

    if (::g->openbottom > ::g->tmfloorz)
	::g->tmfloorz = ::g->openbottom;	

    if (::g->lowfloor < ::g->tmdropoffz)
	::g->tmdropoffz = ::g->lowfloor;

    // if contacted a special line, add it to the list
    if (ld->special /*&& ::g->numspechit < MAXSPECIALCROSS*/ )
    {
		//GK:From now on it uses indexed vectors (for now until and if I found something better)
		if (::g->numspechit >= ::g->spechit.size()) {
#if _ITERATOR_DEBUG_LEVEL < 2
			if (::g->spechit.size() == ::g->spechit.capacity()) {
				::g->spechit.reserve(::g->spechit.size() + MAXSPECIALCROSS);
			}
			//::g->specind = 0;
			::g->spechit.emplace_back(ld);
#else
			if (::g->spechit.size() == ::g->spechit.capacity()) {
				::g->spechit.resize(::g->spechit.size() + MAXSPECIALCROSS);
			}
			::g->spechit[::g->numspechit] = ld;
#endif
		}
		else {
			::g->spechit[::g->numspechit] = ld;
		}
	::g->numspechit++;
    }

    return true;
}

qboolean P_ProjectileImmune(mobj_t *target, mobj_t *source)
{
  return
    ( // PG_GROUPLESS means no immunity, even to own species
      mobjinfo[target->type].projectileGroup != PG_GROUPLESS ||
      target == source
    ) &&
    (
      ( // target type has default behaviour, and things are the same type
        mobjinfo[target->type].projectileGroup == PG_DEFAULT &&
        source->type == target->type
      ) ||
      ( // target type has special behaviour, and things have the same group
        mobjinfo[target->type].projectileGroup != PG_DEFAULT &&
        mobjinfo[target->type].projectileGroup == mobjinfo[source->type].projectileGroup
      )
    );
}


//
// PIT_CheckThing
//
qboolean PIT_CheckThing (mobj_t* thing)
{
    fixed_t		blockdist;
    qboolean		solid;
    int			damage;
		
    if (!(thing->flags & (MF_SOLID|MF_SPECIAL|MF_SHOOTABLE) ))
	return true;
    
    blockdist = thing->radius + ::g->tmthing->radius;

    if ( abs(thing->x - ::g->tmx) >= blockdist
	 || abs(thing->y - ::g->tmy) >= blockdist )
    {
	// didn't hit it
	return true;	
    }
	if (::g->tmthing->player) {//GK: trigger secret by touching an object inside the secret sector (map exclusive)
		bool ok = false;
		if (::g->map) {
			ok = ::g->maps[::g->map - 1].tsecret;
		}
		if (((::g->gamemission == pack_custom && ok) || (::g->gamemission == doom && ::g->gameepisode == 4 && (::g->gamemap == 3 || ::g->gamemap == 7 ))) && thing->subsector->sector->special == 9) {
			::g->tmthing->player->secretcount++;
			//GK send message when secret found
			S_StartSound(::g->tmthing->player->mo, sfx_getpow);
			::g->plyr->message = GOTSECRET;
			thing->subsector->sector->special = 0;
		}
	}
    // don't clip against self
    if (thing == ::g->tmthing)
	return true;
    
    // check for skulls slamming into things
    if (::g->tmthing->flags & MF_SKULLFLY)
    {
	damage = ((P_Random()%8)+1)*::g->tmthing->info->damage;
	
	P_DamageMobj (thing, ::g->tmthing, ::g->tmthing, damage);
	
	::g->tmthing->flags &= ~MF_SKULLFLY;
	::g->tmthing->momx = ::g->tmthing->momy = ::g->tmthing->momz = 0;
	
	P_SetMobjState (::g->tmthing, ::g->tmthing->info->spawnstate);
	
	return false;		// stop moving
    }

    
    // missiles can hit other things
    if (::g->tmthing->flags & MF_MISSILE)
    {
	// see if it went over / under
	if (::g->tmthing->z > thing->z + thing->height)
	    return true;		// overhead
	if (::g->tmthing->z+::g->tmthing->height < thing->z)
	    return true;		// underneath
		
	if (::g->tmthing->target && P_ProjectileImmune(thing, ::g->tmthing->target))
	// (
	//     ::g->tmthing->target->type == thing->type || 
	//     (::g->tmthing->target->type == MT_KNIGHT && thing->type == MT_BRUISER)||
	//     (::g->tmthing->target->type == MT_BRUISER && thing->type == MT_KNIGHT) ) )
	{
	    // Don't hit same species as originator.
	    if (thing == ::g->tmthing->target)
		return true;

	    if (thing->type != MT_PLAYER)
	    {
		// Explode, but do no damage.
		// Let ::g->players missile other ::g->players.
		return false;
	    }
	}
	
	if (! (thing->flags & MF_SHOOTABLE) )
	{
	    // didn't do any damage
	    return !(thing->flags & MF_SOLID);	
	}
	
	// damage / explode
	damage = ((P_Random()%8)+1)*::g->tmthing->info->damage;
	P_DamageMobj (thing, ::g->tmthing, ::g->tmthing->target, damage);

	// don't traverse any more
	return false;				
    }
    
    // check for special pickup
    if (thing->flags & MF_SPECIAL)
    {
	solid = thing->flags&MF_SOLID;
	if (::g->tmflags&MF_PICKUP)
	{
	    // can remove thing
	    P_TouchSpecialThing (thing, ::g->tmthing);
	}
	return !solid;
    }
	
    return !(thing->flags & MF_SOLID);
}


//
// MOVEMENT CLIPPING
//

//
// P_CheckPosition
// This is purely informative, nothing is modified
// (except things picked up).
// 
// in:
//  a mobj_t (can be valid or invalid)
//  a position to be checked
//   (doesn't need to be related to the mobj_t->x,y)
//
// during:
//  special things are touched if MF_PICKUP
//  early out on solid lines?
//
// out:
//  newsubsec
//  floorz
//  ceilingz
//  ::g->tmdropoffz
//   the lowest point contacted
//   (monsters won't move to a dropoff)
//  speciallines[]
//  numspeciallines
//
qboolean
P_CheckPosition
( mobj_t*	thing,
  fixed_t	x,
  fixed_t	y )
{
    int			xl;
    int			xh;
    int			yl;
    int			yh;
    int			bx;
    int			by;
    subsector_t*	newsubsec;

    ::g->tmthing = thing;
    ::g->tmflags = thing->flags;
	
    ::g->tmx = x;
    ::g->tmy = y;
	
    ::g->tmbbox[BOXTOP] = y + ::g->tmthing->radius;
    ::g->tmbbox[BOXBOTTOM] = y - ::g->tmthing->radius;
    ::g->tmbbox[BOXRIGHT] = x + ::g->tmthing->radius;
    ::g->tmbbox[BOXLEFT] = x - ::g->tmthing->radius;

    newsubsec = R_PointInSubsector (x,y);
    ::g->ceilingline = NULL;
    
    // The base floor / ceiling is from the subsector
    // that contains the point.
    // Any contacted ::g->lines the step closer together
    // will adjust them.
    ::g->tmfloorz = ::g->tmdropoffz = newsubsec->sector->floorheight;
    ::g->tmceilingz = newsubsec->sector->ceilingheight;
			
    ::g->validcount++;
    ::g->numspechit = 0;

    if ( ::g->tmflags & MF_NOCLIP )
	return true;
    
    // Check things first, possibly picking things up.
    // The bounding box is extended by MAXRADIUS
    // because mobj_ts are grouped into mapblocks
    // based on their origin point, and can overlap
    // into adjacent blocks by up to MAXRADIUS units.
    xl = (::g->tmbbox[BOXLEFT] - ::g->bmaporgx - MAXRADIUS)>>MAPBLOCKSHIFT;
    xh = (::g->tmbbox[BOXRIGHT] - ::g->bmaporgx + MAXRADIUS)>>MAPBLOCKSHIFT;
    yl = (::g->tmbbox[BOXBOTTOM] - ::g->bmaporgy - MAXRADIUS)>>MAPBLOCKSHIFT;
    yh = (::g->tmbbox[BOXTOP] - ::g->bmaporgy + MAXRADIUS)>>MAPBLOCKSHIFT;

    for (bx=xl ; bx<=xh ; bx++)
	for (by=yl ; by<=yh ; by++)
	    if (!P_BlockThingsIterator(bx,by,PIT_CheckThing))
		return false;
    
    // check ::g->lines
    xl = (::g->tmbbox[BOXLEFT] - ::g->bmaporgx)>>MAPBLOCKSHIFT;
    xh = (::g->tmbbox[BOXRIGHT] - ::g->bmaporgx)>>MAPBLOCKSHIFT;
    yl = (::g->tmbbox[BOXBOTTOM] - ::g->bmaporgy)>>MAPBLOCKSHIFT;
    yh = (::g->tmbbox[BOXTOP] - ::g->bmaporgy)>>MAPBLOCKSHIFT;

    for (bx=xl ; bx<=xh ; bx++)
	for (by=yl ; by<=yh ; by++)
	    if (!P_BlockLinesIterator (bx,by,PIT_CheckLine))
		return false;

    return true;
}


//
// P_TryMove
// Attempt to move to a new position,
// crossing special ::g->lines unless MF_TELEPORT is set.
//
qboolean
P_TryMove
( mobj_t*	thing,
  fixed_t	x,
  fixed_t	y )
{
    fixed_t	oldx;
    fixed_t	oldy;
    int		side;
    int		oldside;
   // line_t*	ld;

    ::g->floatok = false;
    if (!P_CheckPosition (thing, x, y))
	return false;		// solid wall or thing
    
    if ( !(thing->flags & MF_NOCLIP) )
    {
	if (::g->tmceilingz - ::g->tmfloorz < thing->height)
	    return false;	// doesn't fit

	::g->floatok = true;
	
	if ( !(thing->flags&MF_TELEPORT) 
	     &&::g->tmceilingz - thing->z < thing->height)
	    return false;	// mobj must lower itself to fit

	if ( !(thing->flags&MF_TELEPORT)
	     && ::g->tmfloorz - thing->z > 24*FRACUNIT )
	    return false;	// too big a step up

	if ( !(thing->flags&(MF_DROPOFF|MF_FLOAT))
	     && ::g->tmfloorz - ::g->tmdropoffz > 24*FRACUNIT )
	    return false;	// don't stand over a dropoff
    }
    
    // the move is ok,
    // so link the thing into its new position
    P_UnsetThingPosition (thing);

    oldx = thing->x;
    oldy = thing->y;
    thing->floorz = ::g->tmfloorz;
    thing->ceilingz = ::g->tmceilingz;	
    thing->x = x;
    thing->y = y;
	//GK: Higher Level ?
	if (thing->player) {
		if (game->GetCVarBool("pm_thirdPerson") && abs(game->GetCVarFloat("pm_thirdPersonAngle")) == 90.0f) {
			if (::g->flip && !::g->isfliped) {
				thing->angle = thing->angle - ANG180;
				::g->isfliped = true;
			}
			else if (!::g->flip && ::g->isfliped) {
				thing->angle = thing->angle + ANG180;
				::g->isfliped = false;
			}
		}
	}

    P_SetThingPosition (thing);
    
    // if any special ::g->lines were hit, do the effect
    if (! (thing->flags&(MF_TELEPORT|MF_NOCLIP)) )
    {
		while (::g->numspechit--)
		{
			// see if the line was crossed
			//ld = ::g->spechit[::g->numspechit];
			side = P_PointOnLineSide (thing->x, thing->y, ::g->spechit[::g->numspechit]);
			oldside = P_PointOnLineSide (oldx, oldy, ::g->spechit[::g->numspechit]);
			if (side != oldside)
			{
			if (::g->spechit[::g->numspechit]->special)
				P_CrossSpecialLine (::g->spechit[::g->numspechit] -::g->lines, oldside, thing);
			}
		}
    }

    return true;
}

// phares 3/16/98
//
// P_AddSecnode() searches the current list to see if this sector is
// already there. If not, it adds a sector node at the head of the list of
// sectors this object appears in. This is called when creating a list of
// nodes that will get linked in later. Returns a pointer to the new node.

void P_AddSecnode(sector_t* s, mobj_t* thing)
{
	// for (size_t i = 0; i < ::g->sector_list.size(); i++) {
	// 	if (::g->sector_list[i]->m_sector == s)   // Already have a node for this sector?
	// 	{
	// 		::g->sector_list[i]->m_thing = thing; // Yes. Setting m_thing says 'keep it'.
	// 		return;
	// 	}
		
	// }

	for (size_t i = 0; i < ::g->sector_list.size(); i++) {
		if (::g->sector_list[i]->m_sector == s && ::g->sector_list[i]->m_thing == thing)   // Already have a node for this sector?
			{
		 		return;
		 	}

	 }
	
#if _ITERATOR_DEBUG_LEVEL < 2
	if (::g->sector_list.size() == ::g->sector_list.capacity()) {
		::g->sector_list.reserve(::g->sector_list.size() + 1);
}
	//::g->specind = 0;
	std::shared_ptr<msecnode_t> tnode = ::g->sector_list.emplace_back(std::make_shared<msecnode_t>());
#else
	if (::g->sector_list.size() == ::g->sector_list.capacity()) {
		::g->sector_list.resize(::g->sector_list.size() + 1);
		for (size_t i = 0; i < ::g->sector_list.size(); i++) {
			if (::g->sector_list[i] == NULL) {
				::g->sector_list[i] = std::make_shared<msecnode_t>();
			}
		}
	}
	std::shared_ptr<msecnode_t> tnode = ::g->sector_list[::g->headsecind];
#endif
	::g->headsecind++;
	tnode->visited = 0;

	tnode->m_sector = s;       // sector
	tnode->m_thing = thing;     // mobj
	//s->touching_thinglist = tnode;

	//msecnode_t* node;

	//node = nextnode;
	//while (node)
	//{
	//	if (node->m_sector == s)   // Already have a node for this sector?
	//	{
	//		node->m_thing = thing; // Yes. Setting m_thing says 'keep it'.
	//		return(nextnode);
	//	}
	//	node = node->m_tnext;
	//}

	//// Couldn't find an existing node for this sector. Add one at the head
	//// of the list.

	//node = P_GetSecnode();

	//// killough 4/4/98, 4/7/98: mark new nodes unvisited.
	//node->visited = 0;

	//node->m_sector = s;       // sector
	//node->m_thing = thing;     // mobj
	//node->m_tprev = NULL;    // prev node on Thing thread
	//node->m_tnext = nextnode;  // next node on Thing thread
	//if (nextnode)
	//	nextnode->m_tprev = node; // set back link on Thing

	//							  // Add new node at head of sector thread starting at s->touching_thinglist

	//node->m_sprev = NULL;    // prev node on sector thread
	//node->m_snext = s->touching_thinglist; // next node on sector thread
	//if (s->touching_thinglist)
	//	node->m_snext->m_sprev = node;
	//s->touching_thinglist = node;
	//return(node);
}

/**
 --------------------------------------------
	P_DeleteSecNodesFromThing
 --------------------------------------------
 Removes all secnodes from the sector_list that are assosiated with
 the given thing
*/
void P_DeleteSecNodesFromThing(mobj_t* th) {
	std::vector<std::shared_ptr<msecnode_t>> toBeDeleted;

	for (size_t i = 0; i < ::g->sector_list.size(); i++)
	{
		if (::g->sector_list[i]->m_thing == th)
		{
			toBeDeleted.push_back(::g->sector_list[i]);
		}
	}
	for (size_t j = 0; j < toBeDeleted.size(); j++) {
		::g->sector_list.erase(std::find(::g->sector_list.begin(), ::g->sector_list.end(), toBeDeleted[j]));
		::g->headsecind -= 1;
	}
}

// P_DelSecnode() deletes a sector node from the list of
// sectors this object appears in. Returns a pointer to the next node
// on the linked list, or NULL.

void P_DelSecnode(msecnode_t* node)
{
	/*if (node)
	{
		node->m_thing = NULL;
		node->m_sector = NULL;
		delete node;
	}*/
}                             // phares 3/13/98

							  // Delete an entire sector list

void P_DelSeclist()

{
	while(!::g->sector_list.empty()) {
		msecnode_t* node = ::g->sector_list[::g->headsecind - 1].get();
		::g->sector_list[::g->headsecind - 1] = NULL;
		::g->headsecind--;
		if (::g->headsecind < 0) {
			::g->headsecind = 0;
		}
		P_DelSecnode(node);
	}
}

// phares 3/14/98
//
// PIT_GetSectors
// Locates all the sectors the object is in by looking at the lines that
// cross through it. You have already decided that the object is allowed
// at this location, so don't bother with checking impassable or
// blocking lines.

qboolean PIT_GetSectors(line_t* ld)
{
	if (::g->tmbbox[BOXRIGHT] <= ld->bbox[BOXLEFT] ||
		::g->tmbbox[BOXLEFT] >= ld->bbox[BOXRIGHT] ||
		::g->tmbbox[BOXTOP] <= ld->bbox[BOXBOTTOM] ||
		::g->tmbbox[BOXBOTTOM] >= ld->bbox[BOXTOP])
		return true;

	if (P_BoxOnLineSide(::g->tmbbox, ld) != -1)
		return true;

	// This line crosses through the object.

	// Collect the sector(s) from the line and add to the
	// sector_list you're examining. If the Thing ends up being
	// allowed to move to this position, then the sector_list
	// will be attached to the Thing's mobj_t at touching_sectorlist.
	if (ld->frontsector) 
		P_AddSecnode(ld->frontsector, ::g->tmthing);

	// Don't assume all lines are 2-sided, since some Things
	// like MT_TFOG are allowed regardless of whether their radius takes
	// them beyond an impassable linedef.

	// killough 3/27/98, 4/4/98:
	// Use sidedefs instead of 2s flag to determine two-sidedness.

	if (ld->backsector)
		P_AddSecnode(ld->backsector, ::g->tmthing);

	return true;
}

// phares 3/14/98
//
// P_CreateSecNodeList alters/creates the sector_list that shows what sectors
// the object resides in.

void P_CreateSecNodeList(mobj_t* thing, fixed_t x, fixed_t y)
{
	int xl;
	int xh;
	int yl;
	int yh;
	int bx;
	int by;

	// First, clear out the existing m_thing fields. As each node is
	// added or verified as needed, m_thing will be set properly. When
	// finished, delete all nodes where m_thing is still NULL. These
	// represent the sectors the Thing has vacated.

	for (size_t i = 0; i < ::g->sector_list.size(); i++)
	{
		std::shared_ptr<msecnode_t> node = ::g->sector_list[i];
		if (node != NULL && ::g->sector_list[i]->m_thing == thing) {
			node->m_thing = NULL;
		}
	}

	::g->tmthing = thing;
	::g->tmflags = thing->flags;

	::g->tmx = x;
	::g->tmy = y;

	::g->tmbbox[BOXTOP] = y + ::g->tmthing->radius;
	::g->tmbbox[BOXBOTTOM] = y - ::g->tmthing->radius;
	::g->tmbbox[BOXRIGHT] = x + ::g->tmthing->radius;
	::g->tmbbox[BOXLEFT] = x - ::g->tmthing->radius;

	::g->validcount++; // used to make sure we only process a line once

	xl = (::g->tmbbox[BOXLEFT] - ::g->bmaporgx) >> MAPBLOCKSHIFT;
	xh = (::g->tmbbox[BOXRIGHT] - ::g->bmaporgx) >> MAPBLOCKSHIFT;
	yl = (::g->tmbbox[BOXBOTTOM] - ::g->bmaporgy) >> MAPBLOCKSHIFT;
	yh = (::g->tmbbox[BOXTOP] - ::g->bmaporgy) >> MAPBLOCKSHIFT;

	for (bx = xl; bx <= xh; bx++)
		for (by = yl; by <= yh; by++)
			P_BlockLinesIterator(bx, by, PIT_GetSectors);

	// Add the sector of the (x,y) point to sector_list.

	P_AddSecnode(thing->subsector->sector, thing);

	// Now delete any nodes that won't be used. These are the ones where
	// m_thing is still NULL.
	std::vector<std::shared_ptr<msecnode_t>> toBeDeleted;

	 for (size_t i = 0; i < ::g->sector_list.size(); i++)
	 {
		if (::g->sector_list[i]->m_thing == NULL)
		{
			toBeDeleted.push_back(::g->sector_list[i]);
		}
	 }
	 for (size_t j = 0; j < toBeDeleted.size(); j++) {
		::g->sector_list.erase(std::find(::g->sector_list.begin(), ::g->sector_list.end(), toBeDeleted[j]));
		::g->headsecind -= 1;
	 }
}


//
// P_ThingHeightClip
// Takes a valid thing and adjusts the thing->floorz,
// thing->ceilingz, and possibly thing->z.
// This is called for all nearby monsters
// whenever a sector changes height.
// If the thing doesn't fit,
// the z will be set to the lowest value
// and false will be returned.
//
qboolean P_ThingHeightClip (mobj_t* thing)
{
    qboolean		onfloor;
	
    onfloor = (thing->z == thing->floorz);
	
    P_CheckPosition (thing, thing->x, thing->y);	
    // what about stranding a monster partially off an edge?
	
    thing->floorz = ::g->tmfloorz;
    thing->ceilingz = ::g->tmceilingz;
	
    if (onfloor)
    {
	// walking monsters rise and fall with the floor
	thing->z = thing->floorz;
    }
    else
    {
	// don't adjust a floating monster unless forced to
	if (thing->z+thing->height > thing->ceilingz)
	    thing->z = thing->ceilingz - thing->height;
    }
	
    if (thing->ceilingz - thing->floorz < thing->height)
	return false;
		
    return true;
}



//
// SLIDE MOVE
// Allows the player to slide along any angled walls.
//






//
// P_HitSlideLine
// Adjusts the xmove / ymove
// so that the next move will slide along the wall.
//
void P_HitSlideLine (line_t* ld)
{
    int			side;

    angle_t		lineangle;
    angle_t		moveangle;
    angle_t		deltaangle;
    
    fixed_t		movelen;
    fixed_t		newlen;
	bool icyfloor = ::g->slidemo->player && (::g->onground && (::g->slidemo->friction > ORIG_FRICTION));

	if (ld->slopetype == ST_HORIZONTAL)
	{
		if (icyfloor && (abs(::g->tmymove) > abs(::g->tmxmove)))
		{
			::g->tmxmove /= 2; // absorb half the momentum
			::g->tmymove = -::g->tmymove/2;
      		S_StartSound(::g->slidemo,sfx_oof); // oooff!
		}
		else
		{
			::g->tmymove = 0;
		}
		return;
	}

	if (ld->slopetype == ST_VERTICAL)
    {
		if (icyfloor && (abs(::g->tmxmove) > abs(::g->tmymove)))
		{
			::g->tmxmove = -::g->tmxmove / 2; // absorb half the momentum
			::g->tmymove /= 2;
			S_StartSound(::g->slidemo, sfx_oof); // oooff!
		}
		else
		{
			::g->tmxmove = 0;
		}
	return;
    }
	
    side = P_PointOnLineSide (::g->slidemo->x, ::g->slidemo->y, ld);
	
    lineangle = R_PointToAngle2 (0,0, ld->dx, ld->dy);

    if (side == 1)
	lineangle += ANG180;

    moveangle = R_PointToAngle2 (0,0, ::g->tmxmove, ::g->tmymove);
    deltaangle = moveangle-lineangle;

	// killough 3/2/98:
  // The moveangle+=10 breaks v1.9 demo compatibility in
  // some demos, so it needs demo_compatibility switch.

	if (!::g->demoplayback)
		moveangle += 10;				// prevents sudden path reversal due to        // phares
										// rounding error                              //   |
	deltaangle = moveangle - lineangle; //   V
	movelen = P_AproxDistance(::g->tmxmove, ::g->tmymove);
	if (icyfloor && (deltaangle > ANG45) && (deltaangle < ANG90 + ANG45))
	{
		moveangle = lineangle - deltaangle;
		movelen /= 2;					// absorb
		S_StartSound(::g->slidemo, sfx_oof); // oooff!
		moveangle >>= ANGLETOFINESHIFT;
		::g->tmxmove = FixedMul(movelen, finecosine[moveangle]);
		::g->tmymove = FixedMul(movelen, finesine[moveangle]);
	} //   ^
	else //   |
	{	 // phares
		if (deltaangle > ANG180)
			deltaangle += ANG180;
		//	I_Error ("SlideLine: ang>ANG180");

		lineangle >>= ANGLETOFINESHIFT;
		deltaangle >>= ANGLETOFINESHIFT;

		movelen = P_AproxDistance(::g->tmxmove, ::g->tmymove);
		newlen = FixedMul(movelen, finecosine[deltaangle]);

		::g->tmxmove = FixedMul(newlen, finecosine[lineangle]);
		::g->tmymove = FixedMul(newlen, finesine[lineangle]);
	}
}


//
// PTR_SlideTraverse
//
qboolean PTR_SlideTraverse (intercept_t* in)
{
    line_t*	li;
	
    if (!in->isaline)
	I_Error ("PTR_SlideTraverse: not a line?");
		
    li = in->d.line;
    
    if ( ! (li->flags & ML_TWOSIDED) )
    {
	if (P_PointOnLineSide (::g->slidemo->x, ::g->slidemo->y, li))
	{
	    // don't hit the back side
	    return true;		
	}
	goto isblocking;
    }

    // set ::g->openrange, ::g->opentop, ::g->openbottom
    P_LineOpening (li);
    
    if (::g->openrange < ::g->slidemo->height)
	goto isblocking;		// doesn't fit
		
    if (::g->opentop - ::g->slidemo->z < ::g->slidemo->height)
	goto isblocking;		// mobj is too high

    if (::g->openbottom - ::g->slidemo->z > 24*FRACUNIT )
	goto isblocking;		// too big a step up

    // this line doesn't block movement
    return true;		
	
    // the line does block movement,
    // see if it is closer than best so far
  isblocking:		
    if (in->frac < ::g->bestslidefrac)
    {
	::g->secondslidefrac = ::g->bestslidefrac;
	::g->secondslideline = ::g->bestslideline;
	::g->bestslidefrac = in->frac;
	::g->bestslideline = li;
    }
	
    return false;	// stop
}



//
// P_SlideMove
// The momx / momy move is bad, so try to slide
// along a wall.
// Find the first line hit, move flush to it,
// and slide along it
//
// This is a kludgy mess.
//
void P_SlideMove (mobj_t* mo)
{
    fixed_t		leadx;
    fixed_t		leady;
    fixed_t		trailx;
    fixed_t		traily;
    fixed_t		newx;
    fixed_t		newy;
    int			hitcount;
		
    ::g->slidemo = mo;
    hitcount = 0;
    
  retry:
    if (++hitcount == 3)
	goto stairstep;		// don't loop forever

    
    // ::g->trace along the three leading corners
    if (mo->momx > 0)
    {
	leadx = mo->x + mo->radius;
	trailx = mo->x - mo->radius;
    }
    else
    {
	leadx = mo->x - mo->radius;
	trailx = mo->x + mo->radius;
    }
	
    if (mo->momy > 0)
    {
	leady = mo->y + mo->radius;
	traily = mo->y - mo->radius;
    }
    else
    {
	leady = mo->y - mo->radius;
	traily = mo->y + mo->radius;
    }
		
    ::g->bestslidefrac = FRACUNIT+1;
	
    P_PathTraverse ( leadx, leady, leadx+mo->momx, leady+mo->momy,
		     PT_ADDLINES, PTR_SlideTraverse );
    P_PathTraverse ( trailx, leady, trailx+mo->momx, leady+mo->momy,
		     PT_ADDLINES, PTR_SlideTraverse );
    P_PathTraverse ( leadx, traily, leadx+mo->momx, traily+mo->momy,
		     PT_ADDLINES, PTR_SlideTraverse );
    
    // move up to the wall
    if (::g->bestslidefrac == FRACUNIT+1)
    {
	// the move most have hit the middle, so stairstep
      stairstep:
	if (!P_TryMove (mo, mo->x, mo->y + mo->momy))
	    P_TryMove (mo, mo->x + mo->momx, mo->y);
	return;
    }

    // fudge a bit to make sure it doesn't hit
    ::g->bestslidefrac -= 0x800;	
    if (::g->bestslidefrac > 0)
    {
	newx = FixedMul (mo->momx, ::g->bestslidefrac);
	newy = FixedMul (mo->momy, ::g->bestslidefrac);
	
	if (!P_TryMove (mo, mo->x+newx, mo->y+newy))
	    goto stairstep;
    }
    
    // Now continue along the wall.
    // First calculate remainder.
    ::g->bestslidefrac = FRACUNIT-(::g->bestslidefrac+0x800);
    
    if (::g->bestslidefrac > FRACUNIT)
	::g->bestslidefrac = FRACUNIT;
    
    if (::g->bestslidefrac <= 0)
	return;
    
    ::g->tmxmove = FixedMul (mo->momx, ::g->bestslidefrac);
    ::g->tmymove = FixedMul (mo->momy, ::g->bestslidefrac);

    P_HitSlideLine (::g->bestslideline);	// clip the moves

    mo->momx = ::g->tmxmove;
    mo->momy = ::g->tmymove;
		
    if (!P_TryMove (mo, mo->x+::g->tmxmove, mo->y+::g->tmymove))
    {
	goto retry;
    }
}


//
// P_LineAttack
//

// Height if not aiming up or down
// ???: use slope for monsters?



// slopes to top and bottom of target


//
// PTR_AimTraverse
// Sets linetaget and ::g->aimslope when a target is aimed at.
//
qboolean
PTR_AimTraverse (intercept_t* in)
{
    line_t*		li;
    mobj_t*		th;
    fixed_t		slope;
    fixed_t		thingtopslope;
    fixed_t		thingbottomslope;
    fixed_t		dist;
		
    if (in->isaline)
    {
	li = in->d.line;
	
	if ( !(li->flags & ML_TWOSIDED) )
	    return false;		// stop
	
	// Crosses a two sided line.
	// A two sided line will restrict
	// the possible target ranges.
	P_LineOpening (li);
	
	if (::g->openbottom >= ::g->opentop)
	    return false;		// stop
	
	dist = FixedMul (::g->attackrange, in->frac);

	if (li->frontsector->floorheight != li->backsector->floorheight)
	{
	    slope = FixedDiv (::g->openbottom - ::g->shootz , dist);
	    if (slope > ::g->bottomslope)
		::g->bottomslope = slope;
	}
		
	if (li->frontsector->ceilingheight != li->backsector->ceilingheight)
	{
	    slope = FixedDiv (::g->opentop - ::g->shootz , dist);
	    if (slope < ::g->topslope)
		::g->topslope = slope;
	}
		
	if (::g->topslope <= ::g->bottomslope)
	    return false;		// stop
			
	return true;			// shot continues
    }
    
    // shoot a thing
    th = in->d.thing;
    if (th == ::g->shootthing)
	return true;			// can't shoot self
    
    if (!(th->flags&MF_SHOOTABLE))
	return true;			// corpse or something

    // check angles to see if the thing can be aimed at
    dist = FixedMul (::g->attackrange, in->frac);
    thingtopslope = FixedDiv (th->z+th->height - ::g->shootz , dist);

    if (thingtopslope < ::g->bottomslope)
	return true;			// shot over the thing

    thingbottomslope = FixedDiv (th->z - ::g->shootz, dist);

    if (thingbottomslope > ::g->topslope)
	return true;			// shot under the thing
    
    // this thing can be hit!
    if (thingtopslope > ::g->topslope)
	thingtopslope = ::g->topslope;
    
    if (thingbottomslope < ::g->bottomslope)
	thingbottomslope = ::g->bottomslope;

	if (!cl_freelook.GetBool() || game->GetCVarBool("aa_targetAimAssistEnable") || ::g->demorecording || ::g->demoplayback || ::g->shootthing != ::g->viewplayer->mo) {
		::g->aimslope = (thingtopslope + thingbottomslope) / 2;
	}
	/*else {
		if (game->GetCVarBool("aa_targetAimAssistEnable")) {
			::g->assistslope = ((((thingtopslope + thingbottomslope) / 2) << ANGLETOFINESHIFT) >> FRACBITS) * 673;
		}
		else {
			::g->assistslope = 0;
		}
	}*/
    ::g->linetarget = th;

    return false;			// don't go any farther
}


//
// PTR_ShootTraverse
//
qboolean PTR_ShootTraverse (intercept_t* in)
{
    fixed_t		x;
    fixed_t		y;
    fixed_t		z;
    fixed_t		frac;
    
    line_t*		li;
    
    mobj_t*		th;

    fixed_t		slope;
    fixed_t		dist;
    fixed_t		thingtopslope;
    fixed_t		thingbottomslope;
	//GK: TODO: Make bullet puffs to spawn on the floor/ceiling
	/*if (::g->mouseposy && !(in->isaline)) {
		frac = in->frac - FixedDiv(10 * FRACUNIT, ::g->attackrange);
		x = ::g->trace.x + FixedMul(::g->trace.dx, frac);
		y = ::g->trace.y + FixedMul(::g->trace.dy, frac);
		z = ::g->shootz + FixedMul(::g->aimslope, FixedMul(frac, ::g->attackrange)) - (((::g->mouseposy) << FRACBITS) / 673);
		P_SpawnPuff(x, y, z);
		return false;
	}*/
		
    if (in->isaline)
    {
	li = in->d.line;
	
	if (li->special)
	    P_ShootSpecialLine (::g->shootthing, li);

	if ( !(li->flags & ML_TWOSIDED) )
	    goto hitline;
	
	// crosses a two sided line
	P_LineOpening (li);
		
	dist = FixedMul (::g->attackrange, in->frac);

	if (li->frontsector->floorheight != li->backsector->floorheight)
	{
	    slope = FixedDiv (::g->openbottom - ::g->shootz , dist);
	    if (slope > ::g->aimslope)
		goto hitline;
	}
		
	if (li->frontsector->ceilingheight != li->backsector->ceilingheight)
	{
	    slope = FixedDiv (::g->opentop - ::g->shootz , dist);
	    if (slope < ::g->aimslope)
		goto hitline;
	}

	// shot continues
	return true;
	
	
	// hit line
      hitline:
	// position a bit closer
	frac = in->frac - FixedDiv (4*FRACUNIT,::g->attackrange);
	x = ::g->trace.x + FixedMul (::g->trace.dx, frac);
	y = ::g->trace.y + FixedMul (::g->trace.dy, frac);
	z = ::g->shootz + FixedMul (::g->aimslope, FixedMul(frac, ::g->attackrange)) -(((::g->mouseposy) << FRACBITS) / 473);

	if (li->frontsector->ceilingpic == ::g->skyflatnum)
	{
	    // don't shoot the sky!
	    if (z > li->frontsector->ceilingheight)
		return false;
	    
	    // it's a sky hack wall
	    if	(li->backsector && li->backsector->ceilingpic == ::g->skyflatnum)
		return false;		
	}

	mobj_t * sourceObject = ::g->shootthing;
	if( sourceObject ) {

		if( ( sourceObject->player) == &(::g->players[DoomLib::GetPlayer()]) ) {
			
			// Fist Punch.
			if( ::g->attackrange == MELEERANGE ) {
			}
		}
	}

	// Spawn bullet puffs.
	P_SpawnPuff (x,y,z);
	
	// don't go any farther
	return false;	
    }
    
    // shoot a thing
    th = in->d.thing;
    if (th == ::g->shootthing)
	return true;		// can't shoot self
    
    if (!(th->flags&MF_SHOOTABLE))
	return true;		// corpse or something
		
    // check angles to see if the thing can be aimed at
    dist = FixedMul (::g->attackrange, in->frac);
    thingtopslope = FixedDiv (th->z+th->height - ::g->shootz , dist);

    if (thingtopslope < ::g->aimslope)
	return true;		// shot over the thing

    thingbottomslope = FixedDiv (th->z - ::g->shootz, dist);

    if (thingbottomslope > ::g->aimslope)
	return true;		// shot under the thing

    
    // hit thing
    // position a bit closer
    frac = in->frac - FixedDiv (10*FRACUNIT,::g->attackrange);

    x = ::g->trace.x + FixedMul (::g->trace.dx, frac);
    y = ::g->trace.y + FixedMul (::g->trace.dy, frac);
    z = ::g->shootz + FixedMul (::g->aimslope, FixedMul(frac, ::g->attackrange)) - (((::g->mouseposy) << FRACBITS) / 473);

	// check for friendly fire.
#ifdef ID_ENABLE_DOOM_CLASSIC_NETWORKING
	if( th  && !::g->deathmatch ) {
		player_t * hitPlayer = th->player;

		if( hitPlayer ) {

			mobj_t * sourceObject = ::g->shootthing;

			if( sourceObject ) {
				player_t* sourcePlayer = sourceObject->player;

				if( sourcePlayer != NULL && sourcePlayer != hitPlayer ) {
					return true;
				}
			}
		}
	}
#endif

	mobj_t * sourceObject = ::g->shootthing;
	if( sourceObject ) {

		if( ( sourceObject->player) == &(::g->players[DoomLib::GetPlayer()]) ) {

			// Fist Punch.
			if( ::g->attackrange == MELEERANGE ) {
			}
		}
	}


    // Spawn bullet puffs or blod spots,
    // depending on target type.
    if (in->d.thing->flags & MF_NOBLOOD)
	P_SpawnPuff (x,y,z);
    else
	P_SpawnBlood (x,y,z, ::g->la_damage);

    if (::g->la_damage)
	P_DamageMobj (th, ::g->shootthing, ::g->shootthing, ::g->la_damage);

    // don't go any farther
    return false;
	
}


//
// P_AimLineAttack
//
fixed_t
P_AimLineAttack
( mobj_t*	t1,
  angle_t	angle,
  fixed_t	distance )
{
    fixed_t	x2;
    fixed_t	y2;
	
    angle >>= ANGLETOFINESHIFT;
    ::g->shootthing = t1;
    
    x2 = t1->x + (distance>>FRACBITS)*finecosine[angle];
    y2 = t1->y + (distance>>FRACBITS)*finesine[angle];
    ::g->shootz = t1->z + (t1->height>>1) + 8*FRACUNIT;

    // can't shoot outside view angles
    ::g->topslope = 100*FRACUNIT/160;	
    ::g->bottomslope = -100*FRACUNIT/160;
    
    ::g->attackrange = distance;
    ::g->linetarget = NULL;
	
    P_PathTraverse ( t1->x, t1->y,
		     x2, y2,
		     PT_ADDLINES|PT_ADDTHINGS,
		     PTR_AimTraverse );
		
    if (::g->linetarget)
	return ::g->aimslope;

    return 0;
}
 

//
// P_LineAttack
// If damage == 0, it is just a test ::g->trace
// that will leave ::g->linetarget set.
//
void
P_LineAttack
( mobj_t*	t1,
  angle_t	angle,
  fixed_t	distance,
  fixed_t	slope,
  int		damage )
{
    fixed_t	x2;
    fixed_t	y2;
	
    angle >>= ANGLETOFINESHIFT;
    ::g->shootthing = t1;
    ::g->la_damage = damage;
    x2 = t1->x + (distance>>FRACBITS)*finecosine[angle];
    y2 = t1->y + (distance>>FRACBITS)*finesine[angle];
    ::g->shootz = t1->z + (t1->height>>1) + 8*FRACUNIT;
    ::g->attackrange = distance;
    ::g->aimslope = slope;
		
    P_PathTraverse ( t1->x, t1->y,
		     x2, y2,
		     PT_ADDLINES|PT_ADDTHINGS,
		     PTR_ShootTraverse );
}
 


//
// USE LINES
//

qboolean	PTR_UseTraverse (intercept_t* in)
{
    int		side;
	
    if (!in->d.line->special)
    {
	P_LineOpening (in->d.line);
	if (::g->openrange <= 0)
	{
	    S_StartSound (::g->usething, sfx_noway);
	    
	    // can't use through a wall
	    return false;	
	}
	// not a special line, but keep checking
	return true ;		
    }
	
    side = 0;
    if (P_PointOnLineSide (::g->usething->x, ::g->usething->y, in->d.line) == 1)
	side = 1;
    
    //	return false;		// don't use back side
	
    P_UseSpecialLine (::g->usething, in->d.line, side);

    // can't use for than one special line in a row
    return false;
}


//
// P_UseLines
// Looks for special ::g->lines in front of the player to activate.
//
void P_UseLines (player_t*	player) 
{
    int		angle;
    fixed_t	x1;
    fixed_t	y1;
    fixed_t	x2;
    fixed_t	y2;
	
    ::g->usething = player->mo;
		
    angle = player->mo->angle >> ANGLETOFINESHIFT;

    x1 = player->mo->x;
    y1 = player->mo->y;
    x2 = x1 + (USERANGE>>FRACBITS)*finecosine[angle];
    y2 = y1 + (USERANGE>>FRACBITS)*finesine[angle];
	
    P_PathTraverse ( x1, y1, x2, y2, PT_ADDLINES, PTR_UseTraverse );
}


//
// RADIUS ATTACK
//

qboolean P_SplashImmune(mobj_t *target, mobj_t *source, mobj_t *spot)
{
  return // not neutral, not default behaviour, and same group
    !(spot->flags2 & MF2_NEUTRAL_SPLASH) &&
    mobjinfo[target->type].splashGroup != SG_DEFAULT &&
    mobjinfo[target->type].splashGroup == mobjinfo[source->type].splashGroup;
}

//
// PIT_RadiusAttack
// "bombsource" is the creature
// that caused the explosion at "bombspot".
//
qboolean PIT_RadiusAttack (mobj_t* thing)
{
    fixed_t	dx;
    fixed_t	dy;
    fixed_t	dist;
	
    if (!(thing->flags & MF_SHOOTABLE) )
	return true;

	if (::g->bombsource && P_SplashImmune(thing, ::g->bombsource, ::g->bombspot))
    return true;

    // Boss spider and cyborg
    // take no damage from concussion.
    if (((thing->flags2 & MF2_NORADIUSDMG) || (thing->flags2 & MF2_BOSS)) && !(thing->flags2 & MF2_FORCERADIUSDMG))
	return true;	
		
    dx = abs(thing->x - ::g->bombspot->x);
    dy = abs(thing->y - ::g->bombspot->y);
    
    dist = dx>dy ? dx : dy;
    dist = (dist - thing->radius) >> FRACBITS;

    if (dist < 0)
	dist = 0;

    if (dist >= ::g->bombdistance)
	return true;	// out of range

    if ( P_CheckSight (thing, ::g->bombspot) )
    {
		int damage = ::g->bombdamage == ::g->bombdistance ? (::g->bombdamage - dist) : ((::g->bombdamage * (::g->bombdistance - dist) / ::g->bombdistance) + 1);
	// must be in direct path
	P_DamageMobj (thing, ::g->bombspot, ::g->bombsource, damage);
    }
    
    return true;
}

void P_RadiusAttack(mobj_t* spot,
	mobj_t* source,
	int		damage) {
	P_RadiusAttack(spot, source, damage, damage);
}
//
// P_RadiusAttack
// Source is the creature that caused the explosion at spot.
//
void
P_RadiusAttack
( mobj_t*	spot,
  mobj_t*	source,
  int		damage,
  int		distance)
{
    int		x;
    int		y;
    
    int		xl;
    int		xh;
    int		yl;
    int		yh;
    
    fixed_t	dist;
	
    dist = (distance+MAXRADIUS)<<FRACBITS;
    yh = (spot->y + dist - ::g->bmaporgy)>>MAPBLOCKSHIFT;
    yl = (spot->y - dist - ::g->bmaporgy)>>MAPBLOCKSHIFT;
    xh = (spot->x + dist - ::g->bmaporgx)>>MAPBLOCKSHIFT;
    xl = (spot->x - dist - ::g->bmaporgx)>>MAPBLOCKSHIFT;
    ::g->bombspot = spot;
    ::g->bombsource = source;
    ::g->bombdamage = damage;
	::g->bombdistance = distance;
	
    for (y=yl ; y<=yh ; y++)
	for (x=xl ; x<=xh ; x++)
	    P_BlockThingsIterator (x, y, PIT_RadiusAttack );
}



//
// SECTOR HEIGHT CHANGING
// After modifying a ::g->sectors floor or ceiling height,
// call this routine to adjust the positions
// of all things that touch the sector.
//
// If anything doesn't fit anymore, true will be returned.
// If crunch is true, they will take damage
//  as they are being crushed.
// If Crunch is false, you should set the sector height back
//  the way it was and call P_ChangeSector again
//  to undo the changes.
//


//
// PIT_ChangeSector
//
qboolean PIT_ChangeSector (mobj_t*	thing)
{
    mobj_t*	mo;
	
    if (P_ThingHeightClip (thing))
    {
	// keep checking
	return true;
    }
    

    // crunch bodies to giblets
    if (thing->health <= 0)
    {
	P_SetMobjState (thing, S_GIBS);

	thing->flags &= ~MF_SOLID;
	thing->height = 0;
	thing->radius = 0;

	// keep checking
	return true;		
    }

    // crunch dropped items
    if (thing->flags & MF_DROPPED)
    {
	P_RemoveMobj (thing);
	
	// keep checking
	return true;		
    }

    if (! (thing->flags & MF_SHOOTABLE) )
    {
	// assume it is bloody gibs or something
	return true;			
    }
    
    ::g->nofit = true;

    if (::g->crushchange && !(::g->leveltime&3) )
    {
	P_DamageMobj(thing,NULL,NULL,10);

	// spray blood in a random direction
	mo = P_SpawnMobj (thing->x,
			  thing->y,
			  thing->z + thing->height/2, MT_BLOOD);
	
	mo->momx = (P_Random() - P_Random ())<<12;
	mo->momy = (P_Random() - P_Random ())<<12;
    }

    // keep checking (crush other things)	
    return true;	
}



//
// P_ChangeSector
//
qboolean
P_ChangeSector
( sector_t*	sector,
  qboolean	crunch )
{
    int		x;
    int		y;
	
    ::g->nofit = false;
    ::g->crushchange = crunch;
	
    // re-check heights for all things near the moving sector
    for (x=sector->blockbox[BOXLEFT] ; x<= sector->blockbox[BOXRIGHT] ; x++)
	for (y=sector->blockbox[BOXBOTTOM];y<= sector->blockbox[BOXTOP] ; y++)
	    P_BlockThingsIterator (x, y, PIT_ChangeSector);
	
	
    return ::g->nofit;
}


