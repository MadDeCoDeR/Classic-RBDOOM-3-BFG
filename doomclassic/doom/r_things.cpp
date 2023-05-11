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


#include <stdio.h>
#include <stdlib.h>


#include "doomdef.h"
#include "m_swap.h"

#include "i_system.h"
#include "z_zone.h"
#include "w_wad.h"

#include "r_local.h"

#include "doomstat.h"




//void R_DrawColumn (void);
//void R_DrawFuzzColumn (void);





//extern idCVar pm_thirdPerson;
//
// Sprite rotation 0 is facing the viewer,
//  rotation 1 is one angle turn CLOCKWISE around the axis.
// This is not the same as the angle,
//  which increases counter clockwise (protractor).
// There was a lot of stuff grabbed wrong, so I changed it...
//


// constant arrays
//  used for psprite clipping and initializing clipping


//
// INITIALIZATION FUNCTIONS
//

// variables used to look up
//  and range check thing_t ::g->sprites patches






//
// R_InstallSpriteLump
// Local function for R_InitSprites.
//
void
R_InstallSpriteLump
( int		lump,
  unsigned	frame,
  unsigned	rotation,
  qboolean	flipped )
{
    int		r;
	
    if (frame >= 29 || rotation > 8)
	I_Error("R_InstallSpriteLump: "
		"Bad frame characters in lump %i", lump);
	
    if ((int)frame > ::g->maxframe)
	::g->maxframe = frame;
		
    if (rotation == 0)
    {
	// the lump should be used for all rotations
	if (::g->sprtemp[frame].rotate == false)
	    I_Printf ("R_InitSprites: Sprite %s frame %c has "
		"multip rot=0 lump", ::g->spritename, 'A'+frame);

	if (::g->sprtemp[frame].rotate == true)
		I_Printf("R_InitSprites: Sprite %s frame %c has rotations "
		     "and a rot=0 lump\n", ::g->spritename, 'A'+frame);
			
	::g->sprtemp[frame].rotate = false;
	for (r=0 ; r<8 ; r++)
	{
	    ::g->sprtemp[frame].lump[r] = lump - ::g->firstspritelump;
	    ::g->sprtemp[frame].flip[r] = (byte)flipped;
	}
	return;
    }
	
    // the lump is only used for one rotation
    if (::g->sprtemp[frame].rotate == false)
		I_Printf("R_InitSprites: Sprite %s frame %c has rotations "
		 "and a rot=0 lump\n", ::g->spritename, 'A'+frame);
		
    ::g->sprtemp[frame].rotate = true;

    // make 0 based
    rotation--;		
    if (::g->sprtemp[frame].lump[rotation] != -1)
	I_Error ("R_InitSprites: Sprite %s : %c : %c "
		 "has two lumps mapped to it",
		 ::g->spritename, 'A'+frame, '1'+rotation);
		
    ::g->sprtemp[frame].lump[rotation] = lump - ::g->firstspritelump;
    ::g->sprtemp[frame].flip[rotation] = (byte)flipped;
}




//
// R_InitSpriteDefs
// Pass a null terminated list of sprite names
//  (4 chars exactly) to be used.
// Builds the sprite rotation matrixes to account
//  for horizontally flipped ::g->sprites.
// Will report an error if the lumps are inconsistant. 
// Only called at startup.
//
// Sprite lump names are 4 characters for the actor,
//  a letter for the frame, and a number for the rotation.
// A sprite that is flippable will have an additional
//  letter/number appended.
// The rotation character can be 0 to signify no rotations.
//
void R_InitSpriteDefs (const std::vector <char*> namelist) 
{ 
    //const char* const*	check;
    int		i;
    int		l;
    int		intname;
    int		frame;
    int		rotation;
    int		start;
    int		end;
    int		patched;
		
 //   // count the number of sprite names
 //   check = namelist;
 //   while (*check != NULL)
	//check++;

	::g->numsprites = namelist.size() - 1;//check-namelist;
	
    if (!::g->numsprites)
	return;
		
	//::g->sprites.clear();
	::g->sprind = 0;
	if (::g->sprites.empty()) {
		::g->sprites.resize(::g->numsprites);// = (spritedef_t*)DoomLib::Z_Malloc(::g->numsprites * sizeof(*::g->sprites), PU_STATIC, NULL);
		for (int si = 0; si < ::g->numsprites; si++) {
			::g->sprites[si] = new spritedef_t();
		}
	}
	::g->sprind++;
	
    start = ::g->firstspritelump-1;
    end = ::g->lastspritelump+1;
	
    // scan all the lump names for each of the names,
    //  noting the highest frame letter.
    // Just compare 4 characters as ints
    for (i=0 ; i < ::g->numsprites ; i++)
    {
	::g->spritename = namelist[i];
	memset (::g->sprtemp,-1, sizeof(::g->sprtemp));
		
	::g->maxframe = -1;
	intname = *(int *)namelist[i];
	
	// scan the lumps,
	//  filling in the frames for whatever is found
	for (l=start+1 ; l<end ; l++)
	{
	    if (*(int *)lumpinfo[l].name == intname)
	    {
		frame = lumpinfo[l].name[4] - 'A';
		rotation = lumpinfo[l].name[5] - '0';
		//GK: modified sprites are literally replacing vanilla ones here so this checkup is useless
		//if (::g->modifiedgame)
		//    patched = W_GetNumForName (lumpinfo[l].name);
		//else
		    patched = l;

		R_InstallSpriteLump (patched, frame, rotation, false);

		if (lumpinfo[l].name[6])
		{
		    frame = lumpinfo[l].name[6] - 'A';
		    rotation = lumpinfo[l].name[7] - '0';
		    R_InstallSpriteLump (l, frame, rotation, true);
		}
		}
	}
	
	// check the frames that were found for completeness
	if (::g->maxframe == -1)
	{
	    ::g->sprites[i]->numframes = 0;
		/*if (::g->sprind >= ::g->sprites.size()) {
			if (::g->sprites.size() == ::g->sprites.capacity()) {
				::g->sprites.reserve(::g->sprites.size() + MAXVISSPRITES);
			}
			::g->sprites.emplace_back(new spritedef_t());
		}*/
		::g->sprind++;
	    continue;
	}
		
	::g->maxframe++;
	
	for (frame = 0 ; frame < ::g->maxframe ; frame++)
	{
	    switch ((int)::g->sprtemp[frame].rotate)
	    {
	      case -1:
		// no rotations were found for that frame at all
		//GK: Ignore for now
		//I_Error ("R_InitSprites: No patches found "
		//	 "for %s frame %c", namelist[i], frame+'A');
		break;
		
	      case 0:
		// only the first rotation is needed
		break;
			
	      case 1:
		// must have all 8 frames
		for (rotation=0 ; rotation<8 ; rotation++)
		    if (::g->sprtemp[frame].lump[rotation] == -1)
			I_Error ("R_InitSprites: Sprite %s frame %c "
				 "is missing rotations",
				 namelist[i], frame+'A');
		break;
	    }
	}
	
	// allocate space for the frames present and copy ::g->sprtemp to it
		::g->sprites[i]->numframes = ::g->maxframe;
		::g->sprites[i]->spriteframes =
			(spriteframe_t*)DoomLib::Z_Malloc(::g->maxframe * sizeof(spriteframe_t), PU_SPRITE, NULL);
		memcpy(::g->sprites[i]->spriteframes, ::g->sprtemp, ::g->maxframe * sizeof(spriteframe_t));
	
	/*if (::g->sprind >= ::g->sprites.size()) {
		if (::g->sprites.size() == ::g->sprites.capacity()) {
			::g->sprites.reserve(::g->sprites.size() + MAXVISSPRITES);
		}
		::g->sprites.emplace_back(new spritedef_t());
	}*/
	::g->sprind++;
    }

}




//
// GAME FUNCTIONS
//



//
// R_InitSprites
// Called at program start.
//
void R_InitSprites (const std::vector <char*> namelist)
{
    int		i;
	
    for (i=0 ; i<MAXWIDTH ; i++)
    {
	::g->negonearray[i] = -1;
    }
	
    R_InitSpriteDefs (namelist);
}

//
// R_ZeroVisSprite
//

void R_ZeroVisSprite(int index) {
	::g->vissprites[index]->colormap = 0;
	::g->vissprites[index]->gx = 0;
	::g->vissprites[index]->gy = 0;
	::g->vissprites[index]->gz = 0;
	::g->vissprites[index]->gzt = 0;
	::g->vissprites[index]->mobjflags = 0;
	::g->vissprites[index]->next = ::g->vissprites[index];
	::g->vissprites[index]->prev = ::g->vissprites[index - 1];
	::g->vissprites[index]->patch = 0;
	::g->vissprites[index]->scale = 0;
	::g->vissprites[index]->startfrac = 0;
	::g->vissprites[index]->suck = 0;
	::g->vissprites[index]->texturemid = 0;
	::g->vissprites[index]->x1 = 0;
	::g->vissprites[index]->x2 = 0;
	::g->vissprites[index]->xiscale = 0;
}

//
// R_ClearSprites
// Called at frame start.
//
void R_ClearSprites (void)
{
	//::g->vissprites.clear();
	::g->visspriteind = 0;
	if (::g->vissprites.empty()) {
#if _ITERATOR_DEBUG_LEVEL < 2
		::g->vissprites.reserve(MAXVISSPRITES);
		::g->vissprites.emplace_back(new vissprite_t());
#else
		::g->vissprites.resize(MAXVISSPRITES);
		for (int vi = 0; vi < MAXVISSPRITES; vi++) {
			::g->vissprites[vi] = new vissprite_t();
		}
#endif
	}
/*	else {
		for (int i = 0; i < ::g->vissprites.size(); i++) {
			R_ZeroVisSprite(i);
		}
	}*/
	::g->visspriteind++;
    //::g->vissprite_p = ::g->vissprites;
}

//
// R_NewVisSprite
//

vissprite_t* R_NewVisSprite (void)
{
    //if (::g->vissprite_p == &::g->vissprites[MAXVISSPRITES])
	//return &::g->overflowsprite;
	
	if (::g->visspriteind >= ::g->vissprites.size()) {
		if (::g->vissprites.size() == ::g->vissprites.capacity()) {
			::g->vissprites.reserve(::g->vissprites.size() + MAXVISSPRITES);
		}
		::g->vissprites.emplace_back(new vissprite_t());
	}
	else {
		R_ZeroVisSprite(::g->visspriteind);
	}
	
    return ::g->vissprites[::g->visspriteind-1];
}



//
// R_DrawMaskedColumn
// Used for ::g->sprites and masked mid textures.
// Masked means: partly transparent, i.e. stored
//  in posts/runs of opaque pixels.
//


void R_DrawMaskedColumn (postColumn_t* column)
{
    int		topscreen;
    int 	bottomscreen;
    fixed_t	basetexturemid;
	
    basetexturemid = ::g->dc_texturemid;
	
    for ( ; column->topdelta != 0xff ; ) 
    {
	// calculate unclipped screen coordinates
	//  for post
	topscreen = ::g->sprtopscreen + ::g->spryscale*column->topdelta;
	bottomscreen = topscreen + ::g->spryscale*column->length;

	::g->dc_yl = (topscreen+FRACUNIT-1)>>FRACBITS;
	::g->dc_yh = (bottomscreen-1)>>FRACBITS;
		
	if (::g->dc_yh >= ::g->mfloorclip[::g->dc_x])
	    ::g->dc_yh = ::g->mfloorclip[::g->dc_x]-1;
	if (::g->dc_yl <= ::g->mceilingclip[::g->dc_x])
	    ::g->dc_yl = ::g->mceilingclip[::g->dc_x]+1;

	if (::g->dc_yl <= ::g->dc_yh && ::g->dc_yh < ::g->viewheight)
	{
	    ::g->dc_source = (byte *)column + 3;
	    ::g->dc_texturemid = basetexturemid - (column->topdelta<<FRACBITS);
	    // ::g->dc_source = (byte *)column + 3 - column->topdelta;

	    // Drawn by either R_DrawColumn
	    //  or (SHADOW) R_DrawFuzzColumn.
	    colfunc ( ::g->dc_colormap, ::g->dc_source );	
	}
	column = (postColumn_t *)(  (byte *)column + column->length + 4);
    }
	
    ::g->dc_texturemid = basetexturemid;
}



//
// R_DrawVisSprite
//  ::g->mfloorclip and ::g->mceilingclip should also be set.
//
void
R_DrawVisSprite
( vissprite_t*		vis,
  int			x1,
  int			x2 )
{
    postColumn_t*		column;
    int			texturecolumn;
    fixed_t		frac;
    patch_t*		patch;
	fixed_t		baseclip;
	
	
   // patch = /*(patch_t*)*/img2lmp(W_CacheLumpNum (vis->patch+::g->firstspritelump, PU_CACHE_SHARED));
	patch = img2lmp(W_CacheLumpNum(vis->patch + ::g->firstspritelump, PU_CACHE_SHARED),vis->patch + ::g->firstspritelump);
	::g->texnum = vis->patch;  //GK:Get the pointer to the sprite in order to get it's height
    ::g->dc_colormap = vis->colormap;
	::g->usesprite = true;//GK:SPRITE
	::g->issky = false;
    if (!::g->dc_colormap)
    {
	// NULL colormap = shadow draw
	colfunc = fuzzcolfunc;
    }
    else if (vis->mobjflags & MF_TRANSLATION)
    {
	colfunc = R_DrawTranslatedColumn;
	::g->dc_translation = ::g->translationtables - 256 +
	    ( (vis->mobjflags & MF_TRANSLATION) >> (MF_TRANSSHIFT-8) );
    }
	
    ::g->dc_iscale = abs(vis->xiscale)>>::g->detailshift;
    ::g->dc_texturemid = vis->texturemid;
    frac = vis->startfrac;
    ::g->spryscale = vis->scale;
    ::g->sprtopscreen = ::g->centeryfrac - FixedMul(::g->dc_texturemid,::g->spryscale);
	//GK: Begin
	//GK: Heretic stuff for making the psprite to stuck with the view
	// check to see if weapon is a vissprite
	if (vis->psprite)
	{
		::g->dc_texturemid += FixedMul(((::g->centery - ::g->viewheight / 2) << FRACBITS),
			vis->xiscale);
		::g->sprtopscreen += (::g->viewheight / 2 - ::g->centery) << FRACBITS;
	}

	if ( !vis->psprite)
	{
		::g->sprbotscreen = ::g->sprtopscreen + FixedMul(patch->height << FRACBITS,
			::g->spryscale);
		baseclip = (::g->sprbotscreen - FixedMul(vis->footclip << FRACBITS,
			::g->spryscale)) >> FRACBITS;
	}
	else
	{
		baseclip = -1;
	}
	//GK: End
    for (::g->dc_x=vis->x1 ; ::g->dc_x<=vis->x2 ; ::g->dc_x++, frac += vis->xiscale)
    {
	texturecolumn = frac>>FRACBITS;
#ifdef RANGECHECK
	if (texturecolumn < 0 || texturecolumn >= SHORT(patch->width))
	    I_Error ("R_DrawSpriteRange: bad texturecolumn");
#endif
		column = (postColumn_t *) ((byte *)patch +
			LONG(patch->columnofs[texturecolumn]));
	R_DrawMaskedColumn (column);
    }
	
    colfunc = basecolfunc;
}







//
// R_ProjectSprite
// Generates a vissprite for a thing
//  if it might be visible.
//
void R_ProjectSprite (mobj_t* thing)
{
    fixed_t		tr_x;
    fixed_t		tr_y;
    
    fixed_t		gxt;
    fixed_t		gyt;
    
    fixed_t		tx;
    fixed_t		tz;

    fixed_t		xscale;
    
    int			x1;
    int			x2;

    spritedef_t*	sprdef;
    spriteframe_t*	sprframe;
    int			lump;
    
    unsigned		rot;
    qboolean		flip;
    
    unsigned			index;

    vissprite_t*	vis;
    
    angle_t		ang;
    fixed_t		iscale;
    
    // transform the origin point
	extern fixed_t GetViewX(); extern fixed_t GetViewY();
    tr_x = thing->x - GetViewX();
    tr_y = thing->y - GetViewY();
	
    gxt = FixedMul(tr_x,::g->viewcos); 
    gyt = -FixedMul(tr_y,::g->viewsin);
    
    tz = gxt-gyt; 

    // thing is behind view plane?
    if (tz < MINZ)
	return;
    
    xscale = FixedDiv(::g->projection, tz);
	
    gxt = -FixedMul(tr_x,::g->viewsin); 
    gyt = FixedMul(tr_y,::g->viewcos); 
    tx = -(gyt+gxt); 

    // too far off the side?
    if (abs(tx)>(tz<<2))
	return;
    
    // decide which patch to use for sprite relative to player
#ifdef RANGECHECK
    if (thing->sprite >= ::g->sprind)
	I_Error ("R_ProjectSprite: invalid sprite number %i ",
		 thing->sprite);
#endif
	//GK:Sanity check
	int ind = 0;
	if (thing->sprite >= ::g->sprind) {
		ind = ::g->sprind-1;
	}
	else {
		ind = thing->sprite;
	}
    sprdef = ::g->sprites[ind];
#ifdef RANGECHECK
    if ( (thing->frame&FF_FRAMEMASK) >= sprdef->numframes )
	I_Error ("R_ProjectSprite: invalid sprite frame %i : %i \n",
		 thing->sprite, thing->frame);
#endif
    sprframe = &sprdef->spriteframes[ thing->frame & FF_FRAMEMASK];

    if (sprframe->rotate)
    {
	// choose a different rotation based on player view
	ang = R_PointToAngle (thing->x, thing->y);
	rot = (ang-thing->angle+(unsigned)(ANG45/2)*9)>>29;
	lump = sprframe->lump[rot];
	flip = (qboolean)sprframe->flip[rot];
    }
    else
    {
	// use single rotation for all views
	lump = sprframe->lump[0];
	flip = (qboolean)sprframe->flip[0];
    }
    
    // calculate edges of the shape
    tx -= ::g->spriteoffset[lump];	
    x1 = (::g->centerxfrac + FixedMul (tx,xscale) ) >>FRACBITS;

    // off the right side?
    if (x1 > ::g->viewwidth)
	return;
    
    tx +=  ::g->spritewidth[lump];
    x2 = ((::g->centerxfrac + FixedMul (tx,xscale) ) >>FRACBITS) - 1;

    // off the left side
    if (x2 < 0)
	return;
    
    // store information in a vissprite
    vis = R_NewVisSprite ();
	::g->visspriteind++;
    vis->mobjflags = thing->flags;
	vis->psprite = false;
    vis->scale = xscale << ::g->detailshift;
    vis->gx = thing->x;
    vis->gy = thing->y;
    vis->gz = thing->z;
    vis->gzt = thing->z + ::g->spritetopoffset[lump];
    vis->texturemid = vis->gzt - ::g->viewz;
    vis->x1 = x1 < 0 ? 0 : x1;
    vis->x2 = x2 >= ::g->viewwidth ? ::g->viewwidth-1 : x2;	
    iscale = FixedDiv (FRACUNIT, xscale);

    if (flip)
    {
	vis->startfrac = ::g->spritewidth[lump]-1;
	vis->xiscale = -iscale;
    }
    else
    {
	vis->startfrac = 0;
	vis->xiscale = iscale;
    }

    if (vis->x1 > x1)
	vis->startfrac += vis->xiscale*(vis->x1-x1);
    vis->patch = lump;
    
    // get light level
    if (thing->flags & MF_SHADOW)
    {
	// shadow draw
	vis->colormap = NULL;
    }
    else if (::g->fixedcolormap)
    {
	// fixed map
	vis->colormap = ::g->fixedcolormap;
    }
    else if (thing->frame & FF_FULLBRIGHT)
    {
	// full bright
	vis->colormap = ::g->colormaps;
    }
    
    else
    {
	// diminished light
	index = xscale>>(::g->reallightscaleshift-::g->detailshift);

	if (index >= ::g->reallightscale)
	    index = ::g->reallightscale -1;

	vis->colormap = ::g->spritelights[index];
    }	
}




//
// R_AddSprites
// During BSP traversal, this adds ::g->sprites by sector.
//
void R_AddSprites (sector_t* sec)
{
    mobj_t*		thing;
    int			lightnum;

    // BSP is traversed by subsector.
    // A sector might have been split into several
    //  ::g->subsectors during BSP building.
    // Thus we check whether its already added.
    if (sec->validcount == ::g->validcount)
	return;		

    // Well, now it will be done.
    sec->validcount = ::g->validcount;
	
    lightnum = (sec->lightlevel >> LIGHTSEGSHIFT)+::g->extralight;

    if (lightnum < 0)		
	::g->spritelights = ::g->scalelight[0];
    else if (lightnum >= ::g->reallightlevels)
	::g->spritelights = ::g->scalelight[::g->reallightlevels -1];
    else
	::g->spritelights = ::g->scalelight[lightnum];

    // Handle all things in sector.
    for (thing = sec->thinglist ; thing ; thing = thing->snext)
	R_ProjectSprite (thing);
}


//
// R_DrawPSprite
//
void R_DrawPSprite (pspdef_t* psp)
{
    fixed_t		tx;
    int			x1;
    int			x2;
    spritedef_t*	sprdef;
    spriteframe_t*	sprframe;
    int			lump;
    qboolean		flip;
    vissprite_t*	vis;
    vissprite_t		avis;
    
    // decide which patch to use
#ifdef RANGECHECK
    if ( psp->state->sprite >= ::g->sprind)
	I_Error ("R_ProjectSprite: invalid sprite number %i ",
		 psp->state->sprite);
#endif
	//GK:Sanity check
	int index = 0;
	if (psp->state->sprite >= ::g->sprind) {
		index = ::g->sprind-1;
	}
	else {
		index = psp->state->sprite;
	}
		sprdef = ::g->sprites[index];
#ifdef RANGECHECK
    if ( (psp->state->frame & FF_FRAMEMASK)  >= sprdef->numframes)
	I_Error ("R_ProjectSprite: invalid sprite frame %i : %i ",
		 psp->state->sprite, psp->state->frame);
#endif
    sprframe = &sprdef->spriteframes[ psp->state->frame & FF_FRAMEMASK ];

    lump = sprframe->lump[0];
    flip = (qboolean)sprframe->flip[0];
    
    // calculate edges of the shape
    tx = psp->sx-160*FRACUNIT;
	
    tx -= ::g->spriteoffset[lump];	
    x1 = (::g->centerxfrac + FixedMul (tx,::g->pspritescale) ) >>FRACBITS;

    // off the right side
    if (x1 > ::g->viewwidth)
	return;		

    tx +=  ::g->spritewidth[lump];
    x2 = ((::g->centerxfrac + FixedMul (tx, ::g->pspritescale) ) >>FRACBITS) - 1;

    // off the left side
    if (x2 < 0)
	return;
    
    // store information in a vissprite
    vis = &avis;
    vis->mobjflags = 0;
	vis->psprite = true;
    vis->texturemid = (BASEYCENTER<<FRACBITS)+FRACUNIT/2-(psp->sy-::g->spritetopoffset[lump]);
    vis->x1 = x1 < 0 ? 0 : x1;
    vis->x2 = x2 >= ::g->viewwidth ? ::g->viewwidth-1 : x2;	
    vis->scale = ::g->pspritescale << ::g->detailshift;
    
    if (flip)
    {
	vis->xiscale = -::g->pspriteiscale;
	vis->startfrac = ::g->spritewidth[lump]-1;
    }
    else
    {
	vis->xiscale = ::g->pspriteiscale;
	vis->startfrac = 0;
    }
    
    if (vis->x1 > x1)
	vis->startfrac += vis->xiscale*(vis->x1-x1);

    vis->patch = lump;

    if (::g->viewplayer->powers[pw_invisibility] > 4*32
	|| ::g->viewplayer->powers[pw_invisibility] & 8)
    {
	// shadow draw
	vis->colormap = NULL;
    }
    else if (::g->fixedcolormap)
    {
	// fixed color
	vis->colormap = ::g->fixedcolormap;
    }
    else if (psp->state->frame & FF_FULLBRIGHT)
    {
	// full bright
	vis->colormap = ::g->colormaps;
    }
    else
    {
	// local light
	vis->colormap = ::g->spritelights[::g->reallightscale -1];
    }
	
    R_DrawVisSprite (vis, vis->x1, vis->x2);
}



//
// R_DrawPlayerSprites
//
void R_DrawPlayerSprites (void)
{
    int		i;
    int		lightnum;
    pspdef_t*	psp;
    
    // get light level
    lightnum =
	(::g->viewplayer->mo->subsector->sector->lightlevel >> LIGHTSEGSHIFT) 
	+::g->extralight;

    if (lightnum < 0)		
	::g->spritelights = ::g->scalelight[0];
    else if (lightnum >= ::g->reallightlevels)
	::g->spritelights = ::g->scalelight[::g->reallightlevels -1];
    else
	::g->spritelights = ::g->scalelight[lightnum];
    
    // clip to screen bounds
    ::g->mfloorclip = ::g->screenheightarray;
    ::g->mceilingclip = ::g->negonearray;
    
    // add all active psprites
    for (i=0, psp=::g->viewplayer->psprites;
	 i<NUMPSPRITES;
	 i++,psp++)
    {
	if (psp->state)
	    R_DrawPSprite (psp);
    }
}




//
// R_SortVisSprites
//


void R_SortVisSprites (void)
{
    int			i;
    int			count;
    vissprite_t*	ds = NULL;
    vissprite_t*	best = NULL;
    vissprite_t		unsorted;
    fixed_t		bestscale;

    count = ::g->visspriteind;
	
    unsorted.next = unsorted.prev = &unsorted;

    if (!count)
	return;
		
    for (uint i_=0 ; i_ < ::g->visspriteind ; i_++)
    {
		//GK:use the actual vector instead of a temp array
		if (i_ < ::g->visspriteind -1) {
			::g->vissprites[i_]->next= ::g->vissprites[i_ + 1];
		}
		else {
			::g->vissprites[i_]->next = ::g->vissprites[i_];
		}
	if (i_ > 0) {
		::g->vissprites[i_]->prev = ::g->vissprites[i_ - 1];
	}
	else {
		::g->vissprites[i_]->prev = ::g->vissprites[i_];
	}
    }
    
    ::g->vissprites[0]->prev = &unsorted;
    unsorted.next = &*::g->vissprites[0];
	::g->vissprites[::g->visspriteind-1]->next = &unsorted;
    unsorted.prev = ::g->vissprites[::g->visspriteind-1];
    
    // pull the ::g->vissprites out by scale
    //best = 0;		// shut up the compiler warning
    ::g->vsprsortedhead.next = ::g->vsprsortedhead.prev = &::g->vsprsortedhead;
    for (i=0 ; i<count ; i++)
    {
	bestscale = MAXINT;
	for (ds=unsorted.next ; ds!= &unsorted ; ds=ds->next)
	{
	    if (ds->scale < bestscale)
	    {
		bestscale = ds->scale;
		best = ds;
	    }
	}
	best->next->prev = best->prev;
	best->prev->next = best->next;
	best->next = &::g->vsprsortedhead;
	best->prev = ::g->vsprsortedhead.prev;
	::g->vsprsortedhead.prev->next = best;
	::g->vsprsortedhead.prev = best;
    }
}



//
// R_DrawSprite
//
void R_DrawSprite (vissprite_t* spr)
{
    drawseg_t*		ds;
    short		clipbot[MAXWIDTH];
    short		cliptop[MAXWIDTH];
    int			x;
    int			r1;
    int			r2;
    fixed_t		scale;
    fixed_t		lowscale;
    int			silhouette;
		
    for (x = spr->x1 ; x<=spr->x2 ; x++)
	clipbot[x] = cliptop[x] = -2;
    
    // Scan ::g->drawsegs from end to start for obscuring ::g->segs.
    // The first drawseg that has a greater scale
    //  is the clip seg.
    for (int i = ::g->drawsegind - 2; i >= 0; i--)
    {
		ds = ::g->drawsegs[i];
	// determine if the drawseg obscures the sprite
	if (ds->x1 > spr->x2
	    || ds->x2 < spr->x1
	    || (!ds->silhouette
		&& !ds->maskedtexturecol) )
	{
	    // does not cover sprite
	    continue;
	}
			
	r1 = ds->x1 < spr->x1 ? spr->x1 : ds->x1;
	r2 = ds->x2 > spr->x2 ? spr->x2 : ds->x2;

	if (ds->scale1 > ds->scale2)
	{
	    lowscale = ds->scale2;
	    scale = ds->scale1;
	}
	else
	{
	    lowscale = ds->scale1;
	    scale = ds->scale2;
	}
		
	if (scale < spr->scale
	    || ( lowscale < spr->scale
		 && !R_PointOnSegSide (spr->gx, spr->gy, ds->curline) ) )
	{
	    // masked mid texture?
	    if (ds->maskedtexturecol)	
		R_RenderMaskedSegRange (ds, r1, r2);
	    // seg is behind sprite
	    continue;			
	}

	
	// clip this piece of the sprite
	silhouette = ds->silhouette;
	
	if (spr->gz >= ds->bsilheight)
	    silhouette &= ~SIL_BOTTOM;

	if (spr->gzt <= ds->tsilheight)
	    silhouette &= ~SIL_TOP;
			
	if (silhouette == 1)
	{
	    // bottom sil
	    for (x=r1 ; x<=r2 ; x++)
		if (clipbot[x] == -2)
		    clipbot[x] = ds->sprbottomclip[x];
	}
	else if (silhouette == 2)
	{
	    // top sil
	    for (x=r1 ; x<=r2 ; x++)
		if (cliptop[x] == -2)
		    cliptop[x] = ds->sprtopclip[x];
	}
	else if (silhouette == 3)
	{
	    // both
	    for (x=r1 ; x<=r2 ; x++)
	    {
		if (clipbot[x] == -2)
		    clipbot[x] = ds->sprbottomclip[x];
		if (cliptop[x] == -2)
		    cliptop[x] = ds->sprtopclip[x];
	    }
	}
		
    }
    
    // all clipping has been performed, so draw the sprite

    // check for unclipped columns
    for (x = spr->x1 ; x<=spr->x2 ; x++)
    {
	if (clipbot[x] == -2)		
	    clipbot[x] = ::g->viewheight;

	if (cliptop[x] == -2)
	    cliptop[x] = -1;
    }
		
    ::g->mfloorclip = clipbot;
    ::g->mceilingclip = cliptop;
    R_DrawVisSprite (spr, spr->x1, spr->x2);
}




//
// R_DrawMasked
//
void R_DrawMasked (void)
{
    vissprite_t*	spr;
    drawseg_t*		ds;
	
    R_SortVisSprites ();

    if (::g->visspriteind>1) //GK:Does it add a new *Feature* ?
    {
	// draw all ::g->vissprites back to front
	for (spr = ::g->vsprsortedhead.next ;
	     spr != &::g->vsprsortedhead ;
	     spr=spr->next)
	{
	    
	    R_DrawSprite (spr);
	}
    }
    
    // render any remaining masked mid textures
	for (int i = ::g->drawsegind - 2; i >= 0; i--) {
		ds = ::g->drawsegs[i];
		if (ds->maskedtexturecol)
			R_RenderMaskedSegRange(ds, ds->x1, ds->x2);
	}
    
    // draw the psprites on top of everything
    //  but does not draw on side views
	//GK: DONT POINT THAT THING ON ME
	if (!game->GetCVarBool("pm_thirdPerson")) {
		if (!::g->viewangleoffset)
			R_DrawPlayerSprites();

	}
}




