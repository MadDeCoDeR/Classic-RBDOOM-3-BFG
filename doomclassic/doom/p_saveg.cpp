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
#include "p_local.h"

// State.
#include "doomstat.h"
#include "r_state.h"



// Pads ::g->save_p to a 4-byte boundary
//  so that the load/save works on SGI&Gecko.



//
// P_ArchivePlayers
//
void P_ArchivePlayers (void)
{
    int		i;
    int		j;
    player_t*	dest;
		
    for (i=0 ; i<MAXPLAYERS ; i++)
    {
	if (!::g->playeringame[i])
	    continue;
	
	PADSAVEP();

	dest = (player_t *)::g->save_p;
	memcpy (dest,&::g->players[i],sizeof(player_t));
	::g->save_p += sizeof(player_t);
	for (j=0 ; j<NUMPSPRITES ; j++)
	{
	    if (dest->psprites[j].state)
	    {
		dest->psprites[j].state 
			= (state_t *)(dest->psprites[j].state-::g->states.data());
	    }
	}
    }
}



//
// P_UnArchivePlayers
//
void P_UnArchivePlayers (void)
{
    int		i;
    int		j;
	
    for (i=0 ; i<MAXPLAYERS ; i++)
    {
	if (!::g->playeringame[i])
	    continue;
	
	PADSAVEP();

	memcpy (&::g->players[i],::g->save_p, sizeof(player_t));
	::g->save_p += sizeof(player_t);
	
	// will be set when unarc thinker
	::g->players[i].mo = NULL;	
	::g->players[i].message = NULL;
	::g->players[i].attacker = NULL;

	for (j=0 ; j<NUMPSPRITES ; j++)
	{
	    if (::g->players[i]. psprites[j].state)
	    {
		::g->players[i]. psprites[j].state 
		    = &::g->states[ (intptr_t)::g->players[i].psprites[j].state ];
	    }
	}
    }
}


//
// P_ArchiveWorld
//
void P_ArchiveWorld (void)
{
    int			i;
    int			j;
    sector_t*		sec;
    line_t*		li;
    side_t*		si;
    short*		put;
	
    put = (short *)::g->save_p;
    
    // do ::g->sectors
    for (i=0, sec = ::g->sectors ; i < ::g->numsectors ; i++,sec++)
    {
	*put++ = sec->floorheight >> FRACBITS;
	*put++ = sec->ceilingheight >> FRACBITS;
	*put++ = sec->floorpic;
	*put++ = sec->ceilingpic;
	*put++ = sec->lightlevel;
	*put++ = sec->special;		// needed?
	*put++ = sec->tag;		// needed?
    }

    
    // do ::g->lines
    for (i=0, li = ::g->lines ; i < ::g->numlines ; i++,li++)
    {
	*put++ = li->flags;
	*put++ = li->special;
	*put++ = li->tag;
	for (j=0 ; j<2 ; j++)
	{
	    if (li->sidenum[j] == -1)
		continue;
	    
	    si = &::g->sides[li->sidenum[j]];

	    *put++ = si->textureoffset >> FRACBITS;
	    *put++ = si->rowoffset >> FRACBITS;
	    *put++ = si->toptexture;
	    *put++ = si->bottomtexture;
	    *put++ = si->midtexture;	
	}
    }

	// Doom 2 level 30 requires some global pointers, wheee!
	*put++ = ::g->braintargeton;
	*put++ = ::g->easy;

    ::g->save_p = (byte *)put;
}



//
// P_UnArchiveWorld
//
void P_UnArchiveWorld (void)
{
    int			i;
    int			j;
    sector_t*	sec;
    line_t*		li;
    side_t*		si;
    short*		get;
	
    get = (short *)::g->save_p;
    
    // do ::g->sectors
    for (i=0, sec = ::g->sectors ; i < ::g->numsectors ; i++,sec++)
    {
	sec->floorheight = *get++ << FRACBITS;
	sec->ceilingheight = *get++ << FRACBITS;
	sec->floorpic = *get++;
	sec->ceilingpic = *get++;
	sec->lightlevel = *get++;
	sec->special = *get++;		// needed?
	sec->tag = *get++;		// needed?
	//GK: and thats why the old save files will no longer work
	sec->ceilingdata = 0; //jff 2/22/98 now three thinker fields, not two
	sec->floordata = 0;
	sec->lightingdata = 0;
	sec->soundtarget = 0;
    }
    
    // do ::g->lines
    for (i=0, li = ::g->lines ; i < ::g->numlines ; i++,li++)
    {
	li->flags = *get++;
	li->special = *get++;
	li->tag = *get++;
	for (j=0 ; j<2 ; j++)
	{
	    if (li->sidenum[j] == -1)
		continue;
	    si = &::g->sides[li->sidenum[j]];
	    si->textureoffset = *get++ << FRACBITS;
	    si->rowoffset = *get++ << FRACBITS;
	    si->toptexture = *get++;
	    si->bottomtexture = *get++;
	    si->midtexture = *get++;
	}
    }

	// Doom 2 level 30 requires some global pointers, wheee!
	::g->braintargeton = *get++;
	::g->easy = *get++;

    ::g->save_p = (byte *)get;	
}





//
// Thinkers
//

int GetMOIndex( mobj_t* findme ) {
	thinker_t*	th;
	mobj_t*		mobj;
	int			index = 0;

	for (th = ::g->thinkercap.next ; th != &::g->thinkercap ; th=th->next)
	{
		if (th->function.acp1 == (actionf_p1)P_MobjThinker) {
			index++;
			mobj = (mobj_t*)th;

			if ( mobj == findme ) {
				return index;
			}
		}
	}

	return 0;
}

mobj_t* GetMO( int index ) {
	thinker_t*	th;
	int			testindex = 0;

	if ( !index ) {
		return NULL;
	}

	for (th = ::g->thinkercap.next ; th != &::g->thinkercap ; th=th->next)
	{
		if (th->function.acp1 == (actionf_p1)P_MobjThinker) {
			testindex++;

			if ( testindex == index ) {
				return (mobj_t*)th;
			}
		}
	}

	return NULL;
}

//
// P_ArchiveThinkers
//
void P_ArchiveThinkers (void)
{
	thinker_t*		th;
	mobj_t*			mobj;
	ceiling_t*		ceiling;
	vldoor_t*		door;
	floormove_t*	floor;
	plat_t*			plat;
	fireflicker_t*	fire;
	lightflash_t*	flash;
	strobe_t*		strobe;
	glow_t*			glow;

	int i;
	
	// save off the current thinkers
	//I_Printf( "Savegame on leveltime %d\n====================\n", ::g->leveltime );

	for (th = ::g->thinkercap.next ; th != &::g->thinkercap ; th=th->next)
	{
		//mobj_t*	test = (mobj_t*)th;
		//I_Printf( "%3d: %x == function\n", index++, th->function.acp1 );

		if (th->function.acp1 == (actionf_p1)P_MobjThinker)
		{
			*::g->save_p++ = tc_mobj;
			PADSAVEP();

			mobj = (mobj_t *)::g->save_p;
			memcpy (mobj, th, sizeof(*mobj));
			::g->save_p += sizeof(*mobj);
			mobj->state = (state_t *)(mobj->state - ::g->states.data());

			if (mobj->player)
				mobj->player = (player_t *)((mobj->player-::g->players) + 1);

			// Save out 'target'
			int moIndex = GetMOIndex( mobj->target );
			*::g->save_p++ = moIndex >> 8;
			*::g->save_p++ = moIndex;

			// Save out 'tracer'
			moIndex = GetMOIndex( mobj->tracer );
			*::g->save_p++ = moIndex >> 8;
			*::g->save_p++ = moIndex;

			moIndex = GetMOIndex( mobj->snext );
			*::g->save_p++ = moIndex >> 8;
			*::g->save_p++ = moIndex;

			moIndex = GetMOIndex( mobj->sprev );
			*::g->save_p++ = moIndex >> 8;
			*::g->save_p++ = moIndex;

			// Is this the head of a sector list?
			if ( mobj->subsector->sector->thinglist == (mobj_t*)th ) {
				*::g->save_p++ = 1;
			}
			else {
				*::g->save_p++ = 0;
			}

			moIndex = GetMOIndex( mobj->bnext );
			*::g->save_p++ = moIndex >> 8;
			*::g->save_p++ = moIndex;

			moIndex = GetMOIndex( mobj->bprev );
			*::g->save_p++ = moIndex >> 8;
			*::g->save_p++ = moIndex;

			// Is this the head of a block list?
			int	blockx = (mobj->x - ::g->bmaporgx)>>MAPBLOCKSHIFT;
			int	blocky = (mobj->y - ::g->bmaporgy)>>MAPBLOCKSHIFT;
			if ( blockx >= 0 && blockx < ::g->bmapwidth && blocky >= 0 && blocky < ::g->bmapheight 
				&& (mobj_t*)th == ::g->blocklinks[blocky*::g->bmapwidth+blockx] ) {

					*::g->save_p++ = 1;
			}
			else {
				*::g->save_p++ = 0;
			}
			continue;
		}

		if (th->function.acv == (actionf_v)NULL)
		{
			size_t ci;
			for (ci = 0; ci < ::g->cellind;ci++)
				if (::g->activeceilings[ci] == (ceiling_t *)th)
					break;

			if (ci< ::g->cellind)
			{
				*::g->save_p++ = tc_ceiling;
				PADSAVEP();
				ceiling = (ceiling_t *)::g->save_p;
				memcpy (ceiling, th, sizeof(*ceiling));
				::g->save_p += sizeof(*ceiling);
				ceiling->sector = (sector_t *)(ceiling->sector - ::g->sectors);
			}
			continue;
		}

		if (th->function.acp1 == (actionf_p1)T_MoveCeiling)
		{
			*::g->save_p++ = tc_ceiling;
			PADSAVEP();
			ceiling = (ceiling_t *)::g->save_p;
			memcpy (ceiling, th, sizeof(*ceiling));
			::g->save_p += sizeof(*ceiling);
			ceiling->sector = (sector_t *)(ceiling->sector - ::g->sectors);
			continue;
		}

		if (th->function.acp1 == (actionf_p1)T_VerticalDoor)
		{
			*::g->save_p++ = tc_door;
			PADSAVEP();
			door = (vldoor_t *)::g->save_p;
			memcpy (door, th, sizeof(*door));
			::g->save_p += sizeof(*door);
			door->sector = (sector_t *)(door->sector - ::g->sectors);
			continue;
		}

		if (th->function.acp1 == (actionf_p1)T_MoveFloor)
		{
			*::g->save_p++ = tc_floor;
			PADSAVEP();
			floor = (floormove_t *)::g->save_p;
			memcpy (floor, th, sizeof(*floor));
			::g->save_p += sizeof(*floor);
			floor->sector = (sector_t *)(floor->sector - ::g->sectors);
			continue;
		}

		if (th->function.acp1 == (actionf_p1)T_PlatRaise)
		{
			*::g->save_p++ = tc_plat;
			PADSAVEP();
			plat = (plat_t *)::g->save_p;
			memcpy (plat, th, sizeof(*plat));
			::g->save_p += sizeof(*plat);
			plat->sector = (sector_t *)(plat->sector - ::g->sectors);
			continue;
		}

		if (th->function.acp1 == (actionf_p1)T_FireFlicker)
		{
			*::g->save_p++ = tc_fire;
			PADSAVEP();
			fire = (fireflicker_t *)::g->save_p;
			memcpy (fire, th, sizeof(*fire));
			::g->save_p += sizeof(*fire);
			fire->sector = (sector_t *)(fire->sector - ::g->sectors);
			continue;
		}

		if (th->function.acp1 == (actionf_p1)T_LightFlash)
		{
			*::g->save_p++ = tc_flash;
			PADSAVEP();
			flash = (lightflash_t *)::g->save_p;
			memcpy (flash, th, sizeof(*flash));
			::g->save_p += sizeof(*flash);
			flash->sector = (sector_t *)(flash->sector - ::g->sectors);
			continue;
		}

		if (th->function.acp1 == (actionf_p1)T_StrobeFlash)
		{
			*::g->save_p++ = tc_strobe;
			PADSAVEP();
			strobe = (strobe_t *)::g->save_p;
			memcpy (strobe, th, sizeof(*strobe));
			::g->save_p += sizeof(*strobe);
			strobe->sector = (sector_t *)(strobe->sector - ::g->sectors);
			continue;
		}

		if (th->function.acp1 == (actionf_p1)T_Glow)
		{
			*::g->save_p++ = tc_glow;
			PADSAVEP();
			glow = (glow_t *)::g->save_p;
			memcpy (glow, th, sizeof(*glow));
			::g->save_p += sizeof(*glow);
			glow->sector = (sector_t *)(glow->sector - ::g->sectors);
			continue;
		}
	}

	// add a terminating marker
	*::g->save_p++ = tc_end;

	sector_t* sec;
    short* put = (short *)::g->save_p;
	for (i=0, sec = ::g->sectors ; i < ::g->numsectors ; i++,sec++) {
		*put++ = (short)GetMOIndex( sec->soundtarget );
	}

	::g->save_p = (byte *)put;

	// add a terminating marker
	*::g->save_p++ = tc_end;
}



//
// P_UnArchiveThinkers
//
void P_UnArchiveThinkers (void)
{
	byte			tclass;
	thinker_t*		currentthinker;
	thinker_t*		next;
	mobj_t*			mobj;
	ceiling_t*		ceiling;
	vldoor_t*		door;
	floormove_t*	floor;
	plat_t*			plat;
	fireflicker_t*	fire;
	lightflash_t*	flash;
	strobe_t*		strobe;
	glow_t*			glow;

	thinker_t*	th;

	int count = 0;
	sector_t* ss = NULL;
	//GK: Why so many values were individual arrays ???
	typedef struct{
		int mo_targets;
		int mo_tracers;
		int mo_snext;
		int mo_sprev;
		bool mo_shead;
		int mo_bnext;
		int mo_bprev;
		bool mo_bhead;
	}mostuff;

	int			mo_index = 0;
	int			mo_max = 1024;
	//GK: Use vector instead of dynamic arrays for stability
	std::vector<mostuff> unmo;
	//GK: But also try to optimize it
	unmo.reserve(mo_max);
	mostuff mos;
	unmo.push_back(mos);
	// remove all the current thinkers
	currentthinker = ::g->thinkercap.next;
	while (currentthinker != &::g->thinkercap)
	{
		next = currentthinker->next;

		if (currentthinker->function.acp1 == (actionf_p1)P_MobjThinker)
			P_RemoveMobj ((mobj_t *)currentthinker);
		else
			Z_Free(currentthinker);

		currentthinker = next;
	}

	P_InitThinkers ();

	// read in saved thinkers
	while (1)
	{
		tclass = *::g->save_p++;
		switch (tclass)
		{
		case tc_end:

			// clear sector thing lists
			ss = ::g->sectors;
			for (int i=0 ; i < ::g->numsectors ; i++, ss++) {
				ss->thinglist = NULL;
			}

			// clear blockmap thing lists
			count = sizeof(*::g->blocklinks) * ::g->bmapwidth * ::g->bmapheight;
			memset (::g->blocklinks, 0, count);

			// Doom 2 level 30 requires some global pointers, wheee!
			::g->numbraintargets = 0;

			// fixup mobj_t pointers now that all thinkers have been restored
			mo_index = 0;
			for (th = ::g->thinkercap.next ; th != &::g->thinkercap ; th=th->next) {
				if (th->function.acp1 == (actionf_p1)P_MobjThinker) {
					mobj = (mobj_t*)th;

					mobj->target = GetMO( unmo[mo_index].mo_targets );
					mobj->tracer = GetMO(unmo[mo_index].mo_tracers );

					mobj->snext = GetMO(unmo[mo_index].mo_snext );
					mobj->sprev = GetMO(unmo[mo_index].mo_sprev );

					if (unmo[mo_index].mo_shead ) {
						mobj->subsector->sector->thinglist = mobj;
					}

					mobj->bnext = GetMO(unmo[mo_index].mo_bnext );
					mobj->bprev = GetMO(unmo[mo_index].mo_bprev );

					if (unmo[mo_index].mo_bhead ) {
						// Is this the head of a block list?
						int	blockx = (mobj->x - ::g->bmaporgx)>>MAPBLOCKSHIFT;
						int	blocky = (mobj->y - ::g->bmaporgy)>>MAPBLOCKSHIFT;
						if ( blockx >= 0 && blockx < ::g->bmapwidth && blocky >= 0 && blocky < ::g->bmapheight ) {
							::g->blocklinks[blocky*::g->bmapwidth+blockx] = mobj;
						}
					}

					// Doom 2 level 30 requires some global pointers, wheee!
					if ( mobj->type == MT_BOSSTARGET ) {
						::g->braintargets[::g->numbraintargets] = mobj;
						::g->numbraintargets++;
					}

					mo_index++;
					mostuff mos_;
					unmo.push_back(mos_);
				}
			}

			int i;
			sector_t*	sec;
		    short*	get;

			get = (short *)::g->save_p;
			for (i=0, sec = ::g->sectors ; i < ::g->numsectors ; i++,sec++)
			{
				sec->soundtarget = GetMO( *get++ );
			}
			::g->save_p = (byte *)get;

			tclass = *::g->save_p++;
			if ( tclass != tc_end ) {
				I_Error( "Savegame error after loading sector soundtargets." );
			}

			// print the current thinkers
			//I_Printf( "Loadgame on leveltime %d\n====================\n", ::g->leveltime );
			for (th = ::g->thinkercap.next ; th != &::g->thinkercap ; th=th->next)
			{
				//mobj_t*	test = (mobj_t*)th;
				//I_Printf( "%3d: %x == function\n", index++, th->function.acp1 );
			}

			return; 	// end of list

		case tc_mobj:
			PADSAVEP();
			mobj = (mobj_t*)DoomLib::Z_Malloc(sizeof(*mobj), PU_MOBJ, NULL);
			memcpy (mobj, ::g->save_p, sizeof(*mobj));
			::g->save_p += sizeof(*mobj);
			mobj->state = &::g->states[(intptr_t)mobj->state];

			mobj->target = NULL;
			mobj->tracer = NULL;

			if (mobj->player)
			{
				mobj->player = &::g->players[(intptr_t)mobj->player-1];
				mobj->player->mo = mobj;
			}

			P_SetThingPosition (mobj);

			mobj->info = &mobjinfo[mobj->type];
			mobj->floorz = mobj->subsector->sector->floorheight;
			mobj->ceilingz = mobj->subsector->sector->ceilingheight;
			mobj->thinker.function.acp1 = (actionf_p1)P_MobjThinker;

			// Read in 'target' and store for fixup
			int a, b, foundIndex;
			a = *::g->save_p++;
			b = *::g->save_p++;
			foundIndex = (a << 8) + b;
			unmo[mo_index].mo_targets = foundIndex;

			// Read in 'tracer' and store for fixup
			a = *::g->save_p++;
			b = *::g->save_p++;
			foundIndex = (a << 8) + b;
			unmo[mo_index].mo_tracers = foundIndex;

			// Read in 'snext' and store for fixup
			a = *::g->save_p++;
			b = *::g->save_p++;
			foundIndex = (a << 8) + b;
			unmo[mo_index].mo_snext = foundIndex;

			// Read in 'sprev' and store for fixup
			a = *::g->save_p++;
			b = *::g->save_p++;
			foundIndex = (a << 8) + b;
			unmo[mo_index].mo_sprev = foundIndex;

			foundIndex = *::g->save_p++;
			unmo[mo_index].mo_shead = foundIndex == 1;

			// Read in 'bnext' and store for fixup
			a = *::g->save_p++;
			b = *::g->save_p++;
			foundIndex = (a << 8) + b;
			unmo[mo_index].mo_bnext = foundIndex;

			// Read in 'bprev' and store for fixup
			a = *::g->save_p++;
			b = *::g->save_p++;
			foundIndex = (a << 8) + b;
			unmo[mo_index].mo_bprev = foundIndex;

			foundIndex = *::g->save_p++;
			unmo[mo_index].mo_bhead = foundIndex == 1;

			mo_index++;
			mostuff mos__;
			unmo.push_back(mos__);
			P_AddThinker (&mobj->thinker);
			break;

		case tc_ceiling:
			PADSAVEP();
			ceiling = (ceiling_t*)DoomLib::Z_Malloc(sizeof(*ceiling), PU_CEILING, NULL);
			memcpy (ceiling, ::g->save_p, sizeof(*ceiling));
			::g->save_p += sizeof(*ceiling);
			ceiling->sector = &::g->sectors[(intptr_t)ceiling->sector];
			ceiling->sector->ceilingdata = ceiling;

			if (ceiling->thinker.function.acp1)
				ceiling->thinker.function.acp1 = (actionf_p1)T_MoveCeiling;

			P_AddThinker (&ceiling->thinker);
			P_AddActiveCeiling(ceiling);
			break;

		case tc_door:
			PADSAVEP();
			door = (vldoor_t*)DoomLib::Z_Malloc(sizeof(*door), PU_DOOR, NULL);
			memcpy (door, ::g->save_p, sizeof(*door));
			::g->save_p += sizeof(*door);
			door->sector = &::g->sectors[(intptr_t)door->sector];
			door->sector->ceilingdata = door;
			door->thinker.function.acp1 = (actionf_p1)T_VerticalDoor;
			P_AddThinker (&door->thinker);
			break;

		case tc_floor:
			PADSAVEP();
			floor = (floormove_t*)DoomLib::Z_Malloc (sizeof(*floor), PU_FLOOR, NULL);
			memcpy (floor, ::g->save_p, sizeof(*floor));
			::g->save_p += sizeof(*floor);
			floor->sector = &::g->sectors[(intptr_t)floor->sector];
			floor->sector->floordata = floor;
			floor->thinker.function.acp1 = (actionf_p1)T_MoveFloor;
			P_AddThinker (&floor->thinker);
			break;

		case tc_plat:
			PADSAVEP();
			plat = (plat_t*)DoomLib::Z_Malloc (sizeof(*plat), PU_PLATS, NULL);
			memcpy (plat, ::g->save_p, sizeof(*plat));
			::g->save_p += sizeof(*plat);
			plat->sector = &::g->sectors[(intptr_t)plat->sector];
			plat->sector->floordata = plat;

			if (plat->thinker.function.acp1)
				plat->thinker.function.acp1 = (actionf_p1)T_PlatRaise;

			P_AddThinker (&plat->thinker);
			P_AddActivePlat(plat);
			break;

		case tc_fire:
			PADSAVEP();
			fire = (fireflicker_t*)DoomLib::Z_Malloc (sizeof(*fire), PU_LIGHTS, NULL);
			memcpy (fire, ::g->save_p, sizeof(*fire));
			::g->save_p += sizeof(*fire);
			fire->sector = &::g->sectors[(intptr_t)fire->sector];
			fire->thinker.function.acp1 = (actionf_p1)T_FireFlicker;
			P_AddThinker (&fire->thinker);
			break;

		case tc_flash:
			PADSAVEP();
			flash = (lightflash_t*)DoomLib::Z_Malloc (sizeof(*flash), PU_LIGHTS, NULL);
			memcpy (flash, ::g->save_p, sizeof(*flash));
			::g->save_p += sizeof(*flash);
			flash->sector = &::g->sectors[(intptr_t)flash->sector];
			flash->thinker.function.acp1 = (actionf_p1)T_LightFlash;
			P_AddThinker (&flash->thinker);
			break;

		case tc_strobe:
			PADSAVEP();
			strobe = (strobe_t*)DoomLib::Z_Malloc (sizeof(*strobe), PU_LIGHTS, NULL);
			memcpy (strobe, ::g->save_p, sizeof(*strobe));
			::g->save_p += sizeof(*strobe);
			strobe->sector = &::g->sectors[(intptr_t)strobe->sector];
			strobe->thinker.function.acp1 = (actionf_p1)T_StrobeFlash;
			P_AddThinker (&strobe->thinker);
			break;

		case tc_glow:
			PADSAVEP();
			glow = (glow_t*)DoomLib::Z_Malloc (sizeof(*glow), PU_LIGHTS, NULL);
			memcpy (glow, ::g->save_p, sizeof(*glow));
			::g->save_p += sizeof(*glow);
			glow->sector = &::g->sectors[(intptr_t)glow->sector];
			glow->thinker.function.acp1 = (actionf_p1)T_Glow;
			P_AddThinker (&glow->thinker);
			break;

		default:
			I_Error ("Unknown tclass %i in savegame",tclass);
		}
	}
}


//
// P_ArchiveSpecials
//



//
// Things to handle:
//
// T_MoveCeiling, (ceiling_t: sector_t * swizzle), - active list
// T_VerticalDoor, (vldoor_t: sector_t * swizzle),
// T_MoveFloor, (floormove_t: sector_t * swizzle),
// T_LightFlash, (lightflash_t: sector_t * swizzle),
// T_StrobeFlash, (strobe_t: sector_t *),
// T_Glow, (glow_t: sector_t *),
// T_PlatRaise, (plat_t: sector_t *), - active list
//
void P_ArchiveSpecials (void)
{
    thinker_t*		th;
    ceiling_t*		ceiling;
    vldoor_t*		door;
    floormove_t*	floor;
    plat_t*		plat;
    lightflash_t*	flash;
    strobe_t*		strobe;
    glow_t*		glow;
	elevator_t* elevator;
	scroll_t* scroll;
    //int			i;
	
    // save off the current thinkers
    for (th = ::g->thinkercap.next ; th != &::g->thinkercap ; th=th->next)
    {
	if (th->function.acv == (actionf_v)NULL)
	{
		size_t ci;
	    for (ci = 0; ci < ::g->cellind;ci++)
		if (::g->activeceilings[ci] == (ceiling_t *)th)
		    break;
	    
	    if (ci< ::g->cellind)
	    {
		*::g->save_p++ = tc_ceiling;
		PADSAVEP();
		ceiling = (ceiling_t *)::g->save_p;
		memcpy (ceiling, th, sizeof(*ceiling));
		::g->save_p += sizeof(*ceiling);
		ceiling->sector = (sector_t *)(ceiling->sector - ::g->sectors);
	    }
	    continue;
	}
			
	if (th->function.acp1 == (actionf_p1)T_MoveCeiling)
	{
	    *::g->save_p++ = tc_ceiling;
	    PADSAVEP();
	    ceiling = (ceiling_t *)::g->save_p;
	    memcpy (ceiling, th, sizeof(*ceiling));
	    ::g->save_p += sizeof(*ceiling);
	    ceiling->sector = (sector_t *)(ceiling->sector - ::g->sectors);
	    continue;
	}
			
	if (th->function.acp1 == (actionf_p1)T_VerticalDoor)
	{
	    *::g->save_p++ = tc_door;
	    PADSAVEP();
	    door = (vldoor_t *)::g->save_p;
	    memcpy (door, th, sizeof(*door));
	    ::g->save_p += sizeof(*door);
	    door->sector = (sector_t *)(door->sector - ::g->sectors);
	    continue;
	}
			
	if (th->function.acp1 == (actionf_p1)T_MoveFloor)
	{
	    *::g->save_p++ = tc_floor;
	    PADSAVEP();
	    floor = (floormove_t *)::g->save_p;
	    memcpy (floor, th, sizeof(*floor));
	    ::g->save_p += sizeof(*floor);
	    floor->sector = (sector_t *)(floor->sector - ::g->sectors);
	    continue;
	}
			
	if (th->function.acp1 == (actionf_p1)T_PlatRaise)
	{
	    *::g->save_p++ = tc_plat;
	    PADSAVEP();
	    plat = (plat_t *)::g->save_p;
	    memcpy (plat, th, sizeof(*plat));
	    ::g->save_p += sizeof(*plat);
	    plat->sector = (sector_t *)(plat->sector - ::g->sectors);
	    continue;
	}
			
	if (th->function.acp1 == (actionf_p1)T_LightFlash)
	{
	    *::g->save_p++ = tc_flash;
	    PADSAVEP();
	    flash = (lightflash_t *)::g->save_p;
	    memcpy (flash, th, sizeof(*flash));
	    ::g->save_p += sizeof(*flash);
	    flash->sector = (sector_t *)(flash->sector - ::g->sectors);
	    continue;
	}
			
	if (th->function.acp1 == (actionf_p1)T_StrobeFlash)
	{
	    *::g->save_p++ = tc_strobe;
	    PADSAVEP();
	    strobe = (strobe_t *)::g->save_p;
	    memcpy (strobe, th, sizeof(*strobe));
	    ::g->save_p += sizeof(*strobe);
	    strobe->sector = (sector_t *)(strobe->sector - ::g->sectors);
	    continue;
	}
			
	if (th->function.acp1 == (actionf_p1)T_Glow)
	{
	    *::g->save_p++ = tc_glow;
	    PADSAVEP();
	    glow = (glow_t *)::g->save_p;
	    memcpy (glow, th, sizeof(*glow));
	    ::g->save_p += sizeof(*glow);
	    glow->sector = (sector_t *)(glow->sector - ::g->sectors);
	    continue;
	}
	//GK:BOOM's elevator and scroller related stuff
	if (th->function.acp1 == (actionf_p1)T_MoveElevator)
	{
		*::g->save_p++ = tc_elevator;
		PADSAVEP();
		elevator = (elevator_t *)::g->save_p;
		memcpy(elevator, th, sizeof(*elevator));
		::g->save_p += sizeof(*elevator);
		elevator->sector = (sector_t *)(elevator->sector - ::g->sectors);
		continue;
	}

	if (th->function.acp1 == (actionf_p1)T_Scroll)
	{
		*::g->save_p++ = tc_scroll;
		PADSAVEP();
		scroll = (scroll_t *)::g->save_p;
		memcpy(scroll, th, sizeof(*scroll));
		::g->save_p += sizeof(*scroll);
		continue;
	}

    }
	
    // add a terminating marker
    *::g->save_p++ = tc_endspecials;	

}


//
// P_UnArchiveSpecials
//
void P_UnArchiveSpecials (void)
{
    byte		tclass;
    ceiling_t*		ceiling;
    vldoor_t*		door;
    floormove_t*	floor;
    plat_t*		plat;
    lightflash_t*	flash;
    strobe_t*		strobe;
    glow_t*		glow;
	elevator_t* elevator;
	scroll_t* scroll;

    // read in saved thinkers
    while (1)
    {
	tclass = *::g->save_p++;
	switch (tclass)
	{
	  case tc_endspecials:
	    return;	// end of list
			
	  case tc_ceiling:
	    PADSAVEP();
	    ceiling = (ceiling_t*)DoomLib::Z_Malloc(sizeof(*ceiling), PU_CEILING, NULL);
	    memcpy (ceiling, ::g->save_p, sizeof(*ceiling));
	    ::g->save_p += sizeof(*ceiling);
	    ceiling->sector = &::g->sectors[(intptr_t)ceiling->sector];
	    ceiling->sector->ceilingdata = ceiling;

	    if (ceiling->thinker.function.acp1)
		ceiling->thinker.function.acp1 = (actionf_p1)T_MoveCeiling;

	    P_AddThinker (&ceiling->thinker);
	    P_AddActiveCeiling(ceiling);
	    break;
				
	  case tc_door:
	    PADSAVEP();
	    door = (vldoor_t*)DoomLib::Z_Malloc(sizeof(*door), PU_DOOR, NULL);
	    memcpy (door, ::g->save_p, sizeof(*door));
	    ::g->save_p += sizeof(*door);
	    door->sector = &::g->sectors[(intptr_t)door->sector];
	    door->sector->ceilingdata = door;
	    door->thinker.function.acp1 = (actionf_p1)T_VerticalDoor;
	    P_AddThinker (&door->thinker);
	    break;
				
	  case tc_floor:
	    PADSAVEP();
	    floor = (floormove_t*)DoomLib::Z_Malloc (sizeof(*floor), PU_FLOOR, NULL);
	    memcpy (floor, ::g->save_p, sizeof(*floor));
	    ::g->save_p += sizeof(*floor);
	    floor->sector = &::g->sectors[(intptr_t)floor->sector];
	    floor->sector->floordata = floor;
	    floor->thinker.function.acp1 = (actionf_p1)T_MoveFloor;
	    P_AddThinker (&floor->thinker);
	    break;
				
	  case tc_plat:
	    PADSAVEP();
	    plat = (plat_t*)DoomLib::Z_Malloc (sizeof(*plat), PU_PLATS, NULL);
	    memcpy (plat, ::g->save_p, sizeof(*plat));
	    ::g->save_p += sizeof(*plat);
	    plat->sector = &::g->sectors[(intptr_t)plat->sector];
	    plat->sector->floordata = plat;

	    if (plat->thinker.function.acp1)
		plat->thinker.function.acp1 = (actionf_p1)T_PlatRaise;

	    P_AddThinker (&plat->thinker);
	    P_AddActivePlat(plat);
	    break;
				
	  case tc_flash:
	    PADSAVEP();
	    flash = (lightflash_t*)DoomLib::Z_Malloc (sizeof(*flash), PU_LIGHTS, NULL);
	    memcpy (flash, ::g->save_p, sizeof(*flash));
	    ::g->save_p += sizeof(*flash);
	    flash->sector = &::g->sectors[(intptr_t)flash->sector];
	    flash->thinker.function.acp1 = (actionf_p1)T_LightFlash;
	    P_AddThinker (&flash->thinker);
	    break;
				
	  case tc_strobe:
	    PADSAVEP();
	    strobe = (strobe_t*)DoomLib::Z_Malloc (sizeof(*strobe), PU_LIGHTS, NULL);
	    memcpy (strobe, ::g->save_p, sizeof(*strobe));
	    ::g->save_p += sizeof(*strobe);
	    strobe->sector = &::g->sectors[(intptr_t)strobe->sector];
	    strobe->thinker.function.acp1 = (actionf_p1)T_StrobeFlash;
	    P_AddThinker (&strobe->thinker);
	    break;
				
	  case tc_glow:
	    PADSAVEP();
	    glow = (glow_t*)DoomLib::Z_Malloc (sizeof(*glow), PU_LIGHTS, NULL);
	    memcpy (glow, ::g->save_p, sizeof(*glow));
	    ::g->save_p += sizeof(*glow);
	    glow->sector = &::g->sectors[(intptr_t)glow->sector];
	    glow->thinker.function.acp1 = (actionf_p1)T_Glow;
	    P_AddThinker (&glow->thinker);
	    break;
		//GK:BOOM's elevator and scroller related stuff
	  case tc_elevator:
		  PADSAVEP();
		  elevator = (elevator_t*)DoomLib::Z_Malloc(sizeof(*elevator), PU_DOOR, NULL);
		  memcpy(elevator, ::g->save_p, sizeof(*elevator));
		  ::g->save_p += sizeof(*elevator);
		  elevator->sector = &::g->sectors[(intptr_t)elevator->sector];
		  elevator->thinker.function.acp1 = (actionf_p1)T_MoveElevator;
		  P_AddThinker(&elevator->thinker);
		  break;

	  case tc_scroll:
		  PADSAVEP();
		  scroll = (scroll_t*)DoomLib::Z_Malloc(sizeof(*scroll), PU_DOOR, NULL);
		  memcpy(scroll, ::g->save_p, sizeof(*scroll));
		  ::g->save_p += sizeof(*scroll);
		  scroll->thinker.function.acp1 = (actionf_p1)T_Scroll;
		  P_AddThinker(&scroll->thinker);
		  break;
				
	  default:
	    I_Error ("P_UnarchiveSpecials:Unknown tclass %i "
		     "in savegame",tclass);
	}
	
    }

}


