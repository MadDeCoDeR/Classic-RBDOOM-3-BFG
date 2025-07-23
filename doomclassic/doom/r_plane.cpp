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

#include "i_system.h"
#include "z_zone.h"
#include "w_wad.h"

#include "doomdef.h"
#include "doomstat.h"

#include "r_local.h"
#include "r_sky.h"


extern idCVar r_clight;
void AddNewVisplane();

//
// opening
//

// Here comes the obnoxious "visplane".



//
// Clip values are the solid pixel bounding the range.
//  ::g->floorclip starts out ::g->SCREENHEIGHT
//  ::g->ceilingclip starts out -1
//

//
// ::g->spanstart holds the start of a plane span
// initialized to 0 at start
//

//
// texture mapping
//





//
// R_InitPlanes
// Only at game startup.
//
void R_InitPlanes (void)
{
  // Doh!
}


//
// R_MapPlane
//
// Uses global vars:
//  ::g->planeheight
//  ::g->ds_source
//  ::g->basexscale
//  ::g->baseyscale
//  ::g->viewx
//  ::g->viewy
//
// BASIC PRIMITIVE
//
void
R_MapPlane
( int		y,
  int		x1,
  int		x2 )
{
    angle_t	angle;
    fixed_t	distance;
    fixed_t	length;
    unsigned	index;
	
//#ifdef RANGECHECK
    if ( x2 < x1 || x1<0 || x2>=::g->viewwidth || y>=::g->viewheight ) //GK:Sanity check
    {
		//I_Error ("R_MapPlane: %i, %i at %i",x1,x2,y);
		return;
    }
//#endif

    if (::g->planeheight != ::g->cachedheight[y])
    {
	::g->cachedheight[y] = ::g->planeheight;
	distance = ::g->cacheddistance[y] = FixedMul (::g->planeheight, ::g->yslope[y]);
	::g->ds_xstep = ::g->cachedxstep[y] = FixedMul (distance,::g->basexscale);
	::g->ds_ystep = ::g->cachedystep[y] = FixedMul (distance,::g->baseyscale);
    }
    else
    {
	distance = ::g->cacheddistance[y];
	::g->ds_xstep = ::g->cachedxstep[y];
	::g->ds_ystep = ::g->cachedystep[y];
    }
	
	extern angle_t GetViewAngle();
    length = FixedMul (distance,::g->distscale[x1]);
    angle = (GetViewAngle() + ::g->xtoviewangle[x1])>>ANGLETOFINESHIFT;
	extern fixed_t GetViewX(); extern fixed_t GetViewY();
    ::g->ds_xfrac = GetViewX() + FixedMul(finecosine[angle], length) + ::g->planeXoffs;
    ::g->ds_yfrac = -GetViewY() - FixedMul(finesine[angle], length) + ::g->planeYoffs;

    if (::g->fixedcolormap)
	::g->ds_colormap = ::g->fixedcolormap;
    else
    {
		if (r_clight.GetBool()) {
			index = 0;
		}
		else {
			index = distance >> ::g->LIGHTZSHIFT;
		}
	
	if (index >= MAXLIGHTZ )
	    index = MAXLIGHTZ-1;
	//GK:Sanity check
	if (::g->planezlight >= ::g->reallightlevels)
		::g->planezlight = ::g->reallightlevels - 1;

	if (::g->planezlight < 0)
		::g->planezlight = 0;

	::g->ds_colormap = ::g->zlight[::g->planezlight][index];
    }
	
    ::g->ds_y = y;
    ::g->ds_x1 = x1;
    ::g->ds_x2 = x2;

    // high or low detail
    spanfunc (
		::g->ds_xfrac,
		::g->ds_yfrac,
		::g->ds_y,
		::g->ds_x1,
		::g->ds_x2,
		::g->ds_xstep,
		::g->ds_ystep,
		::g->ds_colormap,
		::g->ds_source );	
}


//
// R_ClearPlanes
// At begining of frame.
//
void R_ClearPlanes (void)
{
    int		i;
    angle_t	angle;
    
    // opening / clipping determination
    for (i=0 ; i < ::g->viewwidth ; i++)
    {
	::g->floorclip[i] = ::g->viewheight;
	::g->ceilingclip[i] = -1;
    }
	//GK:Reset indexed vector
	::g->planeind = 0;
	//visplane_t* tplane = new visplane_t();
	if (::g->visplanes.empty()) {
#if _ITERATOR_DEBUG_LEVEL < 2
		::g->visplanes.reserve(MAXVISPLANES);
		::g->visplanes.emplace_back(std::make_unique<visplane_t>());
		::g->visplanes[::g->planeind]->skyflatmapindex = -1;
#else
		::g->visplanes.resize(MAXVISPLANES);
		for (int vpi = 0; vpi < MAXVISPLANES; vpi++) {
			::g->visplanes[vpi] = std::make_unique<visplane_t>();
			::g->visplanes[vpi]->skyflatmapindex = -1;
		}
#endif
	}
	::g->planeind++;
	//::g->lastvisplane = ::g->visplanes;
    ::g->lastopening = ::g->openings;

    // texture calculation
    memset (::g->cachedheight, 0, sizeof(::g->cachedheight));

    // left to right mapping
	extern angle_t GetViewAngle();
    angle = (GetViewAngle()-ANG90)>>ANGLETOFINESHIFT;
	
    // scale will be unit scale at SCREENWIDTH/2 distance
    ::g->basexscale = FixedDiv (finecosine[angle],::g->centerxfrac_ow);
    ::g->baseyscale = -FixedDiv (finesine[angle],::g->centerxfrac_ow);
}

//GK: Begin
/*
		FindSkyFlatMapIndex
==================================

Checks the plane picnum if it belongs to the SKYDEFS flatmapping
and return the index of the array.
If it fails it return -1
*/
int FindSkyFlatMapIndex(int picnum) {
	int result = -1;
	if (!::g->skyFlatMaps.empty()) {
		for (size_t i = 0; i < ::g->skyFlatMaps.size(); i++) {
			if (picnum == R_FlatNumForName(::g->skyFlatMaps[i]->flat)) {
				result = i;
				break;
			}
		}
	}

	return result;
}
//GK: End

//
// R_FindPlane
//
visplane_t* R_FindPlane( fixed_t height, int picnum, int lightlevel,fixed_t xoffs,fixed_t yoffs ) {
	visplane_t*	check = NULL;
	int skyFlatMapIndex = FindSkyFlatMapIndex(picnum);
    if (picnum == ::g->skyflatnum || picnum & PL_SKYFLAT || skyFlatMapIndex > -1) {
		height = 0;			// all skys map together
		lightlevel = 0;
	}
		for (uint i = 0; i < ::g->planeind; i++) {
			check = ::g->visplanes[i].get();
			if (height == check->height && picnum == check->picnum && lightlevel == check->lightlevel && xoffs == check->xoffs && yoffs == check->yoffs) {
				break;
			}
		}

		if (check != ::g->visplanes[::g->planeind-1].get())
			return check;

    //if (::g->lastvisplane - ::g->visplanes == MAXVISPLANES)
		//I_Error ("R_FindPlane: no more visplanes");
	//if ( ) {
		//check = ::g->visplanes[0];
		//return check;
	//}
		
   // ::g->lastvisplane++;
		AddNewVisplane();

    check->height = height;
    check->picnum = picnum;
    check->lightlevel = lightlevel;
    check->minx = ::g->SCREENWIDTH;
    check->maxx = -1;
	check->xoffs = xoffs;
	check->yoffs = yoffs;
	check->skyflatmapindex = skyFlatMapIndex;

    memset(check->top,0xff,sizeof(check->top));

    return check;
}


//
// R_CheckPlane
//
visplane_t*
R_CheckPlane
( visplane_t*	pl,
  int		start,
  int		stop )
{
    int		intrl;
    int		intrh;
    int		unionl;
    int		unionh;
    int		x;
	
	if (start < pl->minx)
	{
		intrl = pl->minx;
		unionl = start;
	}
	else
	{
		unionl = pl->minx;
		intrl = start;
	}

	if (stop > pl->maxx)
	{
		intrh = pl->maxx;
		unionh = stop;
	}
	else
	{
		unionh = pl->maxx;
		intrh = stop;
	}

	for (x=intrl ; x<= intrh ; x++)
		if (pl->top[x] != 0xffff)
			break;

	if (x > intrh)
	{
		pl->minx = unionl;
		pl->maxx = unionh;

		// use the same one
		return pl;		
	}
	
	/*if ( ::g->lastvisplane - ::g->visplanes == MAXVISPLANES ) {
		return pl;
	}*/
	//visplane_t* tplane = new visplane_t();
    // make a new visplane
	::g->visplanes[::g->planeind-1]->height = pl->height;
	::g->visplanes[::g->planeind - 1]->picnum = pl->picnum;
	::g->visplanes[::g->planeind - 1]->lightlevel = pl->lightlevel;
	pl = ::g->visplanes[::g->planeind - 1].get();
	AddNewVisplane();
    pl->minx = start;
    pl->maxx = stop;

    memset(pl->top,0xff,sizeof(pl->top));
		
    return pl;
}


//
// R_MakeSpans
//
void
R_MakeSpans
( int		x,
  int		t1,
  int		b1,
  int		t2,
  int		b2 )
{
    while (t1 < t2 && t1<=b1)
    {	
	R_MapPlane (t1,::g->spanstart[t1],x-1);
	t1++;
    }
    while (b1 > b2 && b1>=t1)
    {
	R_MapPlane (b1,::g->spanstart[b1],x-1);
	b1--;
    }
	
    while (t2 < t1 && t2<=b2)
    {
	::g->spanstart[t2++] = x;
    }
    while (b2 > b1 && b2>=t2)
    {
	::g->spanstart[b2--] = x;
    }
}

int R_InitSkyPlane(int x, int texture) {
	extern angle_t GetViewAngle();
	int angle = ((GetViewAngle() + ::g->xtoviewangle[x]) ^ ::g->flipImg) >> ANGLETOSKYSHIFT;
	::g->dc_x = x;
	::g->texnum = texture; //GK:Get sky texture's height for use in R_DrawColumn
	::g->usesprite = false;
	return angle;
}

void R_DrawSkyPlane(int x, int texture) {
	int angle = R_InitSkyPlane(x, texture);
	::g->dc_source = R_GetColumn(texture, angle);
	::g->issky = false;
	colfunc(::g->dc_colormap, ::g->dc_source);
}

void R_DrawFakeSkyPlane(int x, int texture) {
	int angle = R_InitSkyPlane(x, texture);
	::g->dc_source = R_GetSkyColumn(texture, angle);
	::g->issky = true;
	colfunc(::g->dc_colormap, ::g->dc_source);
}

//GK: Begin
/*

		R_DrawSkyMappedPlane		
====================================

Similar to R_DrawSkyPlane but instead of using ::g->skytexture it uses a sky texture asigned from 
the SKYDEFS flatMapping.
The first argument is the same as it is in R_DrawSkyPlane
The second argument is an index for the flatmapping array
*/
void R_DrawSkyMappedPlane(int x, int index) {
	int mappedTexture = R_TextureNumForName(::g->skyFlatMaps[index]->sky);
	int angle = R_InitSkyPlane(x, mappedTexture);
	::g->dc_source = R_GetColumn(mappedTexture, angle);
	::g->issky = false;
	colfunc(::g->dc_colormap, ::g->dc_source);
}
//GK: End

//
// R_DrawPlanes
// At the end of each frame.
//
void R_DrawPlanes (void)
{
    //visplane_t*		pl;
    int			light;
    int			x;
    int			stop;
    //int			angle;
				
#ifdef RANGECHECK
  /*  if (::g->ds_p - ::g->drawsegs > MAXDRAWSEGS)
	I_Error ("R_DrawPlanes: ::g->drawsegs overflow (%i)",
		 ::g->ds_p - ::g->drawsegs);*/
    
   /* if (::g->lastvisplane - ::g->visplanes > MAXVISPLANES)
	I_Error ("R_DrawPlanes: visplane overflow (%i)",
		 ::g->lastvisplane - ::g->visplanes);*/
    
    if (::g->lastopening - ::g->openings > MAXOPENINGS)
	I_Error ("R_DrawPlanes: opening overflow (%i)",
		 ::g->lastopening - ::g->openings);
#endif

    for (uint i = 0; i < ::g->planeind-1; i++)
    {
		//pl = ::g->visplanes[i];
	if (::g->visplanes[i]->minx > ::g->visplanes[i]->maxx)
	    continue;

	::g->flipImg = 0;
	// sky flat
	if (::g->visplanes[i]->picnum == ::g->skyflatnum || ::g->visplanes[i]->picnum & PL_SKYFLAT || ::g->visplanes[i]->skyflatmapindex > -1)
	{
		int skyToRender = ::g->skytexture;
	    ::g->dc_iscale = ::g->pspriteiscale>>::g->detailshift;
	    
	    // Sky is allways drawn full bright,
	    //  i.e. ::g->colormaps[0] is used.
	    // Because of this hack, sky is not affected
	    //  by INVUL inverse mapping.
	    ::g->dc_colormap = ::g->colormaps[0];
		if (::g->visplanes[i]->picnum & PL_SKYFLAT) {
			// Sky Linedef
	    	const line_t *l = &::g->lines[::g->visplanes[i]->picnum & ~PL_SKYFLAT];

	    	// Sky transferred from first sidedef
	    	const side_t *s = *l->sidenum + ::g->sides;

			// Texture comes from upper texture of reference sidedef
        skyToRender = ::g->texturetranslation[s->toptexture];

	    	// Vertical offset allows careful sky positioning.

	    	::g->dc_texturemid = s->rowoffset - 28*FRACUNIT;

			::g->flipImg = l->special == 272 ? 0u : ~0u;
		} else {
	    	::g->dc_texturemid = ::g->skytexturemid;
		}
		int ttmid = 100 * FRACUNIT;
		if (::g->dc_texturemid > ttmid) { //GK:Tall skies support
			::g->dc_iscale = ::g->dc_iscale *2.0f;
		}
		unsigned short mintop = USHRT_MAX;
	    for (x= ::g->visplanes[i]->minx ; x <= ::g->visplanes[i]->maxx ; x++)
	    {
			//GK: Sky Freelook Hack
			// With this hack the sky flat remains static
			// onthe world and when player is looking above
			// it then a monochromatic pixel will be drawn
			// in order to avoid HOMs
			
		::g->dc_yl = ::g->visplanes[i]->top[x] - ::g->mouseposy;
		::g->dc_yh = ::g->visplanes[i]->bottom[x];
		int realheight = (::g->s_textureheight[::g->skytexture] >> FRACBITS) * 3;
		int viewheight = ::g->visplanes[i]->bottom[x] - ::g->visplanes[i]->top[x];
		if (::g->visplanes[i]->top[x] < mintop && ::g->visplanes[i]->top[x] <= realheight - ::g->visplanes[i]->top[x]) {
			mintop = ::g->visplanes[i]->top[x];
		}
		if (::g->dc_yl <= ::g->dc_yh)
		{
			if (::g->dc_yl > ::g->visplanes[i]->top[x]) {
				::g->dc_yh = ::g->dc_yl;
				::g->dc_yl = ::g->visplanes[i]->top[x];
			
				if (::g->dc_yl <= realheight - ::g->visplanes[i]->top[x]) {
					R_DrawFakeSkyPlane(x, skyToRender);
				}
				else {
					if (::g->visplanes[i]->skyflatmapindex > -1) {
					R_DrawSkyMappedPlane(x, ::g->visplanes[i]->skyflatmapindex);
					} else {
						R_DrawSkyPlane(x, skyToRender);
					}
				}
			}
			if (::g->dc_yl < ::g->visplanes[i]->top[x]) {
				::g->dc_yl = ::g->visplanes[i]->top[x];
			}
			else {
				::g->dc_yl = ::g->visplanes[i]->top[x] - ::g->mouseposy;
			}
			::g->dc_yh = ::g->visplanes[i]->bottom[x];
			if (::g->visplanes[i]->skyflatmapindex > -1) {
					R_DrawSkyMappedPlane(x, ::g->visplanes[i]->skyflatmapindex);
				} else {
					R_DrawSkyPlane(x, skyToRender);
				}
		}
		else {
			::g->dc_yl = ::g->visplanes[i]->top[x];
			if (abs(viewheight) < realheight && ::g->visplanes[i]->bottom[x] < mintop) {
				if (::g->visplanes[i]->skyflatmapindex > -1) {
					R_DrawSkyMappedPlane(x, ::g->visplanes[i]->skyflatmapindex);
				} else {
					R_DrawSkyPlane(x, skyToRender);
				}
			}
			else {
				R_DrawFakeSkyPlane(x, skyToRender);
			}
			
		}
	    }
	    continue;
	}
	
	// regular flat
	::g->ds_source = (byte*)W_CacheLumpNum(::g->firstflat +
				   ::g->flattranslation[::g->visplanes[i]->picnum],
				   PU_CACHE_SHARED);
	
	::g->planeXoffs = ::g->visplanes[i]->xoffs;
	::g->planeYoffs = ::g->visplanes[i]->yoffs;
	::g->planeheight = abs(::g->visplanes[i]->height-::g->viewz);
	light = (::g->visplanes[i]->lightlevel >> LIGHTSEGSHIFT)+::g->extralight;

	if (light >= ::g->reallightlevels)
	    light = ::g->reallightlevels -1;

	if (light < 0)
	    light = 0;

		::g->planezlight=light;

		::g->visplanes[i]->top[::g->visplanes[i]->maxx+1] = 0xffff;
		::g->visplanes[i]->top[::g->visplanes[i]->minx-1] = 0xffff;
		
	stop = ::g->visplanes[i]->maxx + 1;

	for (x= ::g->visplanes[i]->minx ; x<= stop ; x++)
	{
	    R_MakeSpans(x, ::g->visplanes[i]->top[x-1],
			::g->visplanes[i]->bottom[x-1],
			::g->visplanes[i]->top[x],
			::g->visplanes[i]->bottom[x]);
	}
    }
}

void AddNewVisplane() {
	if (::g->planeind >= ::g->visplanes.size()) {
#if _ITERATOR_DEBUG_LEVEL < 2
		if (::g->visplanes.size() == ::g->visplanes.capacity()) {
			::g->visplanes.reserve(::g->visplanes.size() + MAXVISPLANES);
		}
		::g->visplanes.emplace_back(std::make_unique<visplane_t>());
		::g->visplanes[::g->planeind]->skyflatmapindex = -1;
#else
		::g->visplanes.resize(::g->visplanes.size() + MAXVISPLANES);
		for (int vpi = ::g->planeind; vpi < ::g->visplanes.size(); vpi++) {
			::g->visplanes[vpi] = std::make_unique<visplane_t>();
			::g->visplanes[vpi]->skyflatmapindex = -1;
		}
#endif
	}
	else {
		::g->visplanes[::g->planeind]->height = 0;
		::g->visplanes[::g->planeind]->picnum = 0;
		::g->visplanes[::g->planeind]->lightlevel = 0;
		::g->visplanes[::g->planeind]->minx = 0;
		::g->visplanes[::g->planeind]->maxx = 0;
		std::fill(::g->visplanes[::g->planeind]->bottom, ::g->visplanes[::g->planeind]->bottom + ::g->SCREENWIDTH, 0);
		::g->visplanes[::g->planeind]->xoffs = ::g->visplanes[::g->planeind]->yoffs = 0;
		::g->visplanes[::g->planeind]->skyflatmapindex = -1;
		::g->visplanes[::g->planeind]->nervePad1 = 0;
		::g->visplanes[::g->planeind]->nervePad2 = 0;
		::g->visplanes[::g->planeind]->nervePad3 = 0;
		::g->visplanes[::g->planeind]->nervePad4 = 0;
		::g->visplanes[::g->planeind]->nervePad5 = 0;
		::g->visplanes[::g->planeind]->pad1 = 0;
		::g->visplanes[::g->planeind]->pad2 = 0;
		::g->visplanes[::g->planeind]->pad3 = 0;
		::g->visplanes[::g->planeind]->pad4 = 0;
		std::fill(::g->visplanes[::g->planeind]->top, ::g->visplanes[::g->planeind]->top + ::g->SCREENWIDTH, 0);
	}
	::g->planeind++;
}