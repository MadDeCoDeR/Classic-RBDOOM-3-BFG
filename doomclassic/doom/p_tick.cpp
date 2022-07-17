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
#include "p_local.h"

#include "doomstat.h"
#include "m_menu.h"


extern idCVar in_photomode;

//
// THINKERS
// All thinkers should be allocated by Z_Malloc
// so they can be operated on uniformly.
// The actual structures will vary in size,
// but the first element must be thinker_t.
//



// Both the head and tail of the thinker list.


//
// P_InitThinkers
//
void P_InitThinkers (void)
{
    ::g->thinkercap.prev = ::g->thinkercap.next  = &::g->thinkercap;
}




//
// P_AddThinker
// Adds a new thinker at the end of the list.
//
void P_AddThinker (thinker_t* thinker)
{
    ::g->thinkercap.prev->next = thinker;
    thinker->next = &::g->thinkercap;
    thinker->prev = ::g->thinkercap.prev;
    ::g->thinkercap.prev = thinker;
}



//
// P_RemoveThinker
// Deallocation is lazy -- it will not actually be freed
// until its thinking turn comes up.
//
void P_RemoveThinker (thinker_t* thinker)
{
  // FIXED?:Now the unallocation will be instant.
	thinker->function.acv = (actionf_v)(-1);
	thinker->next->prev = thinker->prev;
	thinker->prev->next = thinker->next;
	Z_Free(thinker);
}



//
// P_AllocateThinker
// Allocates memory and adds a new thinker at the end of the list.
//
void P_AllocateThinker (thinker_t*	thinker)
{
}



//
// P_RunThinkers
//
void P_RunThinkers (void)
{
    thinker_t*	currentthinker;

    currentthinker = ::g->thinkercap.next;
    while (currentthinker != &::g->thinkercap)
    {
	/*	 if ( currentthinker->function.acv == (actionf_v)(-1) )
		 {
			 // time to remove it
			 currentthinker->next->prev = currentthinker->prev;
			 currentthinker->prev->next = currentthinker->next;
			 Z_Free(currentthinker);
		 }
		 else*/
		 {
			 if (currentthinker->function.acp1)
				 currentthinker->function.acp1 ((mobj_t*)currentthinker);
		 }
	currentthinker = currentthinker->next;
    }
}



//
// P_Ticker
//
extern byte demoversion;

void P_Ticker (void)
{
    int		i;
    
    // run the tic
    if (::g->paused)
		return;

	// don't think during wipe
	if ( !::g->netgame && (!::g->demoplayback || demoversion == VERSION ) && ::g->wipe ) {
		return;
	}

    // pause if in menu and at least one tic has been run
    if ( !::g->netgame
	 && ::g->menuactive
	 && !::g->demoplayback
	 && ::g->players[::g->consoleplayer].viewz != 1)
    {
	return;
    }

	if (in_photomode.GetBool()) {
		return;
	}

	//GK: Pause game if the controller unexpectedly disconnects
	if (idLib::joystick && !Sys_hasConnectedController()) {
		M_StartControlPanel();
		return;
	}


	for (i=0 ; i<MAXPLAYERS ; i++) {
		if (::g->playeringame[i]) {
		    P_PlayerThink (&::g->players[i]);
		}
	}
	//GK: In case the Z-Memory is about to get full crear the cache
	int tenprec = ((::g->zonesize/1024)/1024) *0.9;
	int inuse = ((::g->NumAlloc)/1024)/1024;
	if (inuse > tenprec) {
		Z_FreeTags(PU_CACHE,PU_CACHE);
	}

    P_RunThinkers ();
    P_UpdateSpecials ();
    P_RespawnSpecials ();

    // for par times
    ::g->leveltime++;	
	::g->normaltime = ::g->leveltime / TICRATE;
}

