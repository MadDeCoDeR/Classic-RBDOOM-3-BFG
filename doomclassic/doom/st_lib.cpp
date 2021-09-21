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

#include <ctype.h>

#include "doomdef.h"

#include "z_zone.h"
#include "v_video.h"

#include "m_swap.h"

#include "i_system.h"

#include "w_wad.h"

#include "st_stuff.h"
#include "st_lib.h"
#include "r_local.h"


// in AM_map.c

extern idCVar cl_HUD;


//
// Hack display negative frags.
//  Loads and store the stminus lump.
//

void STlib_init(void)
{
    ::g->sttminus = /*(patch_t *)*/ img2lmp(W_CacheLumpName("STTMINUS", PU_STATUS_FRONT), W_GetNumForName("STTMINUS"));
}


// ?
void
STlib_initNum
( st_number_t*		n,
  int			x,
  int			y,
  patch_t**		pl,
  int*			num,
  qboolean*		on,
  int			width )
{
    n->x	= x + ::g->ASPECT_POS_OFFSET;
    n->y	= y;
    n->oldnum	= 0;
    n->width	= width;
    n->num	= num;
    n->on	= on;
    n->p	= pl;
}

// ?
void
STlib_initAspectNum
(st_number_t* n,
    int			x,
    int			y,
    patch_t** pl,
    int* num,
    qboolean* on,
    int			width)
{
    n->x = x;
    n->y = y;
    n->oldnum = 0;
    n->width = width;
    n->num = num;
    n->on = on;
    n->p = pl;
}

// 
// A fairly efficient way to draw a number
//  based on differences from the old number.
// Note: worth the trouble?
//
void
STlib_drawNum
( st_number_t*	n,
  qboolean	refresh )
{

    int		numdigits = n->width;
    int		num = *n->num;
    
    int		w = SHORT(n->p[0]->width);
    int		h = SHORT(n->p[0]->height);
    int		x = n->x;
    
    int		neg;

    n->oldnum = *n->num;

    neg = num < 0;

    if (neg)
    {
	if (numdigits == 2 && num < -9)
	    num = -9;
	else if (numdigits == 3 && num < -99)
	    num = -99;
	
	num = -num;
    }

    // clear the area
    x = n->x > 0 ? n->x - numdigits*w : 0;

    int calcY = ::g->st_statusbaron ? n->y - ST_Y : n->y;
    if (calcY < 0)
	I_Error("drawNum: n->y - ST_Y < 0");

    if (::g->st_statusbaron) {
        V_CopyRect(x, calcY, BG, w * numdigits, h, x, n->y, FG, true);
    }

    // if non-number, do not draw it
    if (num == 1994)
	return;

    x = n->x;

    // in the special case of 0, you draw 0
    if (!num)
	V_DrawPatch(x > 0 ? x - w : 0, n->y, FG, n->p[ 0 ], true);

    // draw the new number
    while (num && numdigits--)
    {
	x -= w;
	V_DrawPatch(x > 0 ? x : 0, n->y, FG, n->p[ num % 10 ], true);
	num /= 10;
    }

    // draw a minus sign if necessary
    if (neg && ::g->st_statusbaron) {
        V_DrawPatch(x > 8 ? x - 8 : 0, n->y, FG, ::g->sttminus, true);
    }
    else if (neg && !::g->st_statusbaron && cl_HUD.GetBool()) {
        V_DrawPatch(x > 8 ? x - 8 : 0, n->y, FG, ::g->fullminus, true);
    }
}


//
void
STlib_updateNum
( st_number_t*		n,
  qboolean		refresh )
{
    if (*n->on) STlib_drawNum(n, refresh);
}


//
void
STlib_initPercent
( st_percent_t*		p,
  int			x,
  int			y,
  patch_t**		pl,
  int*			num,
  qboolean*		on,
  patch_t*		percent )
{
    STlib_initNum(&p->n, x, y, pl, num, on, 3);
    p->p = percent;
}




void
STlib_updatePercent
( st_percent_t*		per,
  int			refresh )
{
    if (refresh && *per->n.on)
	V_DrawPatch(per->n.x, per->n.y, FG, per->p, true);
    
    STlib_updateNum(&per->n, refresh);
}



void
STlib_initMultIcon
( st_multicon_t*	i,
  int			x,
  int			y,
  patch_t**		il,
  int*			inum,
  qboolean*		on )
{
    i->x	= x + ::g->ASPECT_POS_OFFSET;
    i->y	= y;
    i->oldinum 	= -1;
    i->inum	= inum;
    i->on	= on;
    i->p	= il;
}

void
STlib_initAspectMultIcon
(st_multicon_t* i,
    int			x,
    int			y,
    patch_t** il,
    int* inum,
    qboolean* on)
{
    i->x = x;
    i->y = y;
    i->oldinum = -1;
    i->inum = inum;
    i->on = on;
    i->p = il;
}



void
STlib_updateMultIcon
( st_multicon_t*	mi,
  qboolean		refresh )
{
    int			w;
    int			h;
    int			x;
    int			y;

    if (*mi->on
	&& (mi->oldinum != *mi->inum || refresh)
	&& (*mi->inum!=-1))
    {
	if (mi->oldinum != -1)
	{
	    x = mi->x - SHORT(mi->p[mi->oldinum]->leftoffset);
	    y = mi->y - SHORT(mi->p[mi->oldinum]->topoffset);
	    w = SHORT(mi->p[mi->oldinum]->width);
	    h = SHORT(mi->p[mi->oldinum]->height);

        if (::g->st_statusbaron) {
	        if (y - ST_Y < 0)
			    I_Error("updateMultIcon: y - ST_Y < 0");
       
            V_CopyRect(x, y - ST_Y, BG, w, h, x, y, FG, true);
        }
	}
	V_DrawPatch(mi->x, mi->y, FG, mi->p[*mi->inum], true);
	mi->oldinum = *mi->inum;
    }
}



void
STlib_initBinIcon
( st_binicon_t*		b,
  int			x,
  int			y,
  patch_t*		i,
  qboolean*		val,
  qboolean*		on )
{
    b->x	= x + ::g->ASPECT_POS_OFFSET;
    b->y	= y;
    b->oldval	= 0;
    b->val	= val;
    b->on	= on;
    b->p	= i;
}



void
STlib_updateBinIcon
( st_binicon_t*		bi,
  qboolean		refresh )
{
    int			x;
    int			y;
    int			w;
    int			h;

    if (*bi->on
	&& (bi->oldval != *bi->val || refresh))
    {
	x = bi->x - SHORT(bi->p->leftoffset);
	y = bi->y - SHORT(bi->p->topoffset);
	w = SHORT(bi->p->width);
	h = SHORT(bi->p->height);

	if (y - ST_Y < 0)
	    I_Error("updateBinIcon: y - ST_Y < 0");

	if (*bi->val)
	    V_DrawPatch(bi->x, bi->y, FG, bi->p, true);
	else
	    V_CopyRect(x, y-ST_Y, BG, w, h, x, y, FG, true);

	bi->oldval = *bi->val;
    }

}


