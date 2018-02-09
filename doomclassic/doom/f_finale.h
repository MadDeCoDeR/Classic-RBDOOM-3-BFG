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

#ifndef __F_FINALE__
#define __F_FINALE__


#include "doomtype.h"
#include "d_event.h"
//
// FINALE
//
//GK: Use this for editing Endings
extern char*	e1text;
extern char*	e2text;
extern char*	e3text;
extern char*	e4text;

extern char*	c1text;
extern char*	c2text;
extern char*	c3text;
extern char*	c4text;
extern char*	c5text;
extern char*	c6text;
extern char*	c7text;
extern char*	c8Text;
extern char*	c9Text;

extern char*	p1text;
extern char*	p2text;
extern char*	p3text;
extern char*	p4text;
extern char*	p5text;
extern char*	p6text;

extern char*	t1text;
extern char*	t2text;
extern char*	t3text;
extern char*	t4text;
extern char*	t5text;
extern char*	t6text;

extern char* finaleflat[];

void resetEndings();
void ResetFinalflat();
// Called by main loop.
qboolean F_Responder (event_t* ev);

// Called by main loop.
void F_Ticker (void);

// Called by main loop.
void F_Drawer (void);


void F_StartFinale (void);




#endif

