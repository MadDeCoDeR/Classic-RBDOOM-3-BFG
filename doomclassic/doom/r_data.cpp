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

#include "m_swap.h"

#include "w_wad.h"

#include "doomdef.h"
#include "r_local.h"
#include "p_local.h"

#include "doomstat.h"
#include "r_sky.h"

#ifdef LINUX
#include  <alloca.h>
#endif


#include "r_data.h"
#include "m_random.h"

#include <vector>

//GK:Added some parts of boom's source code created by Lee Killough to fix Medusa effect
//since it was causing the game to crash

//
// Graphics.
// DOOM graphics for walls and ::g->sprites
// is stored in vertical runs of opaque pixels (posts).
// A column is composed of zero or more posts,
// a patch or sprite is composed of zero or more columns.
// 



//
// Texture definition.
// Each texture is composed of one or more patches,
// with patches being lumps stored in the WAD.
// The lumps are referenced by number, and patched
// into the rectangular texture space using origin
// and possibly other attributes.
//


//
// Texture definition.
// A DOOM wall texture is a list of patches
// which are to be combined in a predefined order.
//


// A single patch from a texture definition,
//  basically a rectangular area within
//  the texture rectangle.


// A maptexturedef_t describes a rectangular texture,
//  which is composed of one or more mappatch_t structures
//  that arrange graphic patches.






// for global animation

// needed for pre rendering



//
// MAPTEXTURE_T CACHING
// When a texture is first needed,
//  it counts the number of composite columns
//  required in the texture and allocates space
//  for a column directory and any new columns.
// The directory will simply point inside other patches
//  if there is only one patch in a given column,
//  but any columns with multiple patches
//  will have new postColumn_ts generated.
//



//
// R_DrawColumnInCache
// Clip and draw a column
//  from a patch into a cached post.
//
// Rewritten by Lee Killough for performance and to fix Medusa bug
//
void
R_DrawColumnInCache
( postColumn_t*	patch,
  byte*			cache,
  int			originy,
  int			cacheheight,
	byte* marks)
{
    int		count;
    int		position;
    byte*	source;
    byte*	dest;
	
    dest = (byte *)cache + 3;
	
    while (patch->topdelta != 0xff)
    {
	source = (byte *)patch + 3;
	count = patch->length;
	position = originy + patch->topdelta;

	if (position < 0)
	{
	    count += position;
	    position = 0;
	}

	if (position + count > cacheheight)
	    count = cacheheight - position;

	if (count > 0) {
		memcpy(cache + position, source, count);
		// killough 4/9/98: remember which cells in column have been drawn,
		// so that column can later be converted into a series of posts, to
		// fix the Medusa bug.

		memset(marks + position, 0xff, count);
	}
		
	patch = (postColumn_t *)(  (byte *)patch + patch->length + 4); 
    }
}



//
// R_GenerateComposite
// Using the texture definition,
//  the composite texture is created from the patches,
//  and each column is cached.
//
// Rewritten by Lee Killough for performance and to fix Medusa bug
// And Ported by George Kalampokis by rewriting some lines to fit c++ and restructures in this version
void R_GenerateComposite (int texnum)
{
    byte*			block;
    texture_t*		texture;
    texpatch_t*		patch;	
    patch_t*		realpatch;
//    int				x; //Not needed 
    int				x1;
    int				x2;
    int				i;
    postColumn_t*	patchcol;
    short*			collump;
    unsigned *	colofs;// killough 4/9/98: make 32-bit
    texture = ::g->s_textures[texnum];

    block = (byte*)DoomLib::Z_Malloc (::g->s_texturecompositesize[texnum],
		      PU_CACHE_SHARED, 
		      &::g->s_texturecomposite[texnum]);	

    collump = ::g->s_texturecolumnlump[texnum];
    colofs = ::g->s_texturecolumnofs[texnum];
	// killough 4/9/98: marks to identify transparent regions in merged textures
	byte *marks =(byte*) calloc(texture->width, texture->height), *source;
    // Composite the columns together.
    patch = texture->patches;
		
	for (i = texture->patchcount-1, patch = texture->patches;
		i >=0;
		i--, patch++)
	{
		realpatch = /*(patch_t*)*/img2lmp(W_CacheLumpNum(patch->patch, PU_CACHE_SHARED),patch->patch);
		x1 = patch->originx;
		x2 = x1 + SHORT(realpatch->width);
		const int *cofs = realpatch->columnofs - x1;
		if (x1 < 0)
			x1 = 0;
		//else
		//	x = x1;

		if (x2 > texture->width)
			x2 = texture->width;

		for (; x1 < x2; x1++)
		{
			// Column does not have multiple patches?
		   // if (collump[x] >= 0)
			//continue;
			if (collump[x1] == -1) {      // Column has multiple patches?
										// killough 1/25/98, 4/9/98: Fix medusa bug.
				//GK: This version uses postColumn_t instead of column_t
				patchcol = (postColumn_t *)((byte *)realpatch
						+ LONG(cofs[x1]));
				R_DrawColumnInCache(patchcol,
					block + colofs[x1],
					patch->originy,
					texture->height,
					marks + x1*texture->height);
			}
		}
	}
	// killough 4/9/98: Next, convert multipatched columns into true columns,
	// to fix Medusa bug while still allowing for transparent regions.

	source =(byte*) malloc(texture->height);       // temporary column
	for (i = 0; i < texture->width; i++)
		if (collump[i] == -1)                 // process only multipatched columns
		{
			postColumn_t *col = (postColumn_t *)(block + colofs[i] - 3);  // cached column
			const byte *mark = marks + i * texture->height;
			int j = 0;

			// save column in temporary so we can shuffle it around
			memcpy(source, (byte *)col + 3, texture->height);

			for (;;)  // reconstruct the column by scanning transparency marks
			{
				while (j < texture->height && !mark[j]) // skip transparent cells
					j++;
				if (j >= texture->height)           // if at end of column
				{
					col->topdelta = (byte)-1;             // end-of-column marker
					break;
				}
				col->topdelta = j;                  // starting offset of post
				for (col->length = 0; j < texture->height && mark[j]; j++)
					col->length++;                    // count opaque cells
													  // copy opaque cells from the temporary back into the column
				memcpy((byte *)col + 3, source + col->topdelta, col->length);
				col = (postColumn_t *)((byte *)col + col->length + 4); // next post
				}
			}
	free(source);         // free temporary column
	free(marks);          // free transparency marks						
    
}



//
// R_GenerateLookup
//
// Rewritten by Lee Killough for performance and to fix Medusa bug
//
// And Ported by George Kalampokis by rewriting some lines to fit c++ and restructures in this version
void R_GenerateLookup (int texnum)
{
    texture_t*		texture;
    texpatch_t*		patch;	
    patch_t*		realpatch;
    int			x;
    int			x1;
    int			x2;
    int			i;
    short*		collump;
    unsigned*	colofs;// killough 4/9/98: make 32-bit
	
    texture = ::g->s_textures[texnum];

    // Composited texture not created yet.
    ::g->s_texturecomposite[texnum] = 0;
    
    ::g->s_texturecompositesize[texnum] = 0;
    collump = ::g->s_texturecolumnlump[texnum];
    colofs = ::g->s_texturecolumnofs[texnum];
    
    // Now count the number of columns
    //  that are covered by more than one patch.
    // Fill in the lump / offset, so columns
    //  with only a single patch are all done.
   // std::vector<byte> patchcount(texture->width, 0);
    patch = texture->patches;
	//std::vector<unsigned short> posts(texture->width,0);
	// killough 4/9/98: keep count of posts in addition to patches.
	// Part of fix for medusa bug for multipatched 2s normals.
	//GK: Give struct a name in c++ calloc must be cast in order to work and using (struct*) is not an option
	struct ct{
		unsigned int patches, posts;
	};
	std::vector<ct> count;
	{
		//GK: Either keep the for loops or make them while loops both has the same result
		for (i = texture->patchcount-1;
			i >=0;
			i--)
		{
			int pat = patch->patch;
			realpatch = /*(patch_t*)*/img2lmp(W_CacheLumpNum(pat, PU_CACHE_SHARED), pat);
			x1 = patch++->originx;
			x2 = x1 + SHORT(realpatch->width);
			const int *cofs = realpatch->columnofs - x1;
			x = x1;
			if (x1 < 0)
				x = 0;


			if (x2 > texture->width)
				x2 = texture->width;

			for (int xi = 0; xi < x2; xi++) {
				count.push_back({0, 0});
			}

			for (; x < x2; x++)
			{
				// killough 4/9/98: keep a count of the number of posts in column,
				// to fix Medusa bug while allowing for transparent multipatches.

				if (LONG(cofs[x]) < W_LumpLength(pat) && LONG(cofs[x]) >= 0)
				{
					postColumn_t *col = (postColumn_t *)((byte *)realpatch + LONG(cofs[x]));

					if (col->length < W_LumpLength(pat) && x < texture->width)
					{
						for (; col->topdelta != 0xff; count[x].posts++)
							col = (postColumn_t *)((byte *)col + col->length + 4);
						count[x].patches++;
						collump[x] = pat;
						colofs[x] = LONG(cofs[x]) + 3;
					}
				}
			}
		}
	}
	{
		int csize = 0;
		int height = texture->height;
		
		for (x = texture->width-1; x >=0; x--)
		{
			if (!count[x].patches)
			{
				I_Error("R_GenerateLookup: column without a patch (%s:%d)\n",
					texture->name, x);
				return;
			}
			// I_Error ("R_GenerateLookup: column without a patch");

			if (count[x].patches > 1 && x<texture->width)
			{
				// killough 1/25/98, 4/9/98:
				//
				// Fix Medusa bug, by adding room for column header
				// and trailer bytes for each post in merged column.
				// For now, just allocate conservatively 4 bytes
				// per post per patch per column, since we don't
				// yet know how many posts the merged column will
				// require, and it's bounded above by this limit.

				// Use the cached block.
					collump[x] = -1; // mark lump as multipatched
					colofs[x] = csize + 3; // three header bytes in a column
					csize += 4* count[x].posts +1;  // 1 stop byte plus 4 bytes per post
				

			/*	if (csize > 0x10000 - texture->height)
				{
					I_Error("R_GenerateLookup: texture %i is >64k",
						texnum);
				}*/


			}
			csize += height; // height bytes of texture data
		}
		::g->s_texturecompositesize[texnum] = csize;
	}
	//free(count);  // killough 4/9/98
}




//
// R_GetColumn
//
byte*
R_GetColumn
( int		tex,
  int		col )
{
    int		lump;
    int		ofs;
	
    col &= ::g->s_texturewidthmask[tex];
    lump = ::g->s_texturecolumnlump[tex][col];
    ofs = ::g->s_texturecolumnofs[tex][col];
    
    if (lump > 0)
	return (byte *)W_CacheLumpNum(lump,PU_CACHE_SHARED)+ofs;

    if (!::g->s_texturecomposite[tex])
	R_GenerateComposite (tex);

    return ::g->s_texturecomposite[tex] + ofs;
}
//GK: Begin
//
// R_GetColumn
//
//This function esentialy is taking a random
// pixel from the first row of a flat and it
// is storing it in a buffer.
//This allow us to fill rendering spaces
// (aka places where the renderer isn't 
// suppose to render) with a monochromatic
// pixel
void R_GenerateSkyHead(int lump) {
	postColumn_t * column;
	byte*			source;
	byte src = 0;
	int pos = 0;
	//int col = 0;
	patch_t* patch = (patch_t *)W_CacheLumpNum(lump, PU_CACHE_SHARED);
	::g->skybuffer = (byte*)malloc( 3 * sizeof(byte));
	// SMF - rewritten for scaling
		column = (postColumn_t *)((byte *)patch + LONG(patch->columnofs[rand() % 8]));
			source = (byte *)column + 3;
			src = *source++;
				::g->skybuffer[pos] = src;
				pos++;
			::g->skybuffer[pos] = src;
			pos++;
		::g->skybuffer[pos] = 0xff;
		pos++;
	
}

//
// R_GetSkyColumn
//
//With that we can get the monochromatic
// pixel.
byte*
R_GetSkyColumn
(int		tex,
	int		col)
{
	int		lump;
	int		ofs;
	

	col &= ::g->s_texturewidthmask[tex];
	lump = ::g->s_texturecolumnlump[tex][col];
	ofs = ::g->s_texturecolumnofs[tex][col];

	if (lump > 0) {
		if (!::g->skybuffer) {
			R_GenerateSkyHead(lump);
		}
		return ::g->skybuffer;
	}
		

	if (!::g->s_texturecomposite[tex])
		R_GenerateComposite(tex);

	return ::g->s_texturecomposite[tex] + ofs;
}
//GK: End



//
// R_InitTextures
// Initializes the texture list
//  with the s_textures from the world map.
//
void R_InitTextures (void)
{
    maptexture_t*	mtexture;
    texture_t*		texture;
    mappatch_t*		mpatch;
    texpatch_t*		patch;

    int			i;
    int			j;

    int*		maptex;
    int*		maptex2;
    int*		maptex1;
    
    char		name[9];
    char*		names;
    char*		name_p;
    
    int			totalwidth;
    int			nummappatches;
    int			offset;
    int			maxoff;
    int			maxoff2;
    int			numtextures1;
    int			numtextures2;

    int*		directory;
    
    int			temp1;
    int			temp2;
    int			temp3;

    
    // Load the patch names from pnames.lmp.
    name[8] = 0;	
    names = (char*)W_CacheLumpName ("PNAMES", PU_CACHE_SHARED);
    nummappatches = LONG ( *((int *)names) );
    name_p = names+4;
    
	std::vector<int> patchlookup(nummappatches);

    for (i=0 ; i<nummappatches ; i++)
    {
		strncpy (name,name_p+i*8, 8);
		patchlookup[i] = W_CheckNumForName (name, W_CheckNumForName("P_END"));
		if (patchlookup[i] == -1) {
			patchlookup[i] = W_CheckNumForName (name);
		}
    }
    Z_Free(names);
    
	if (::g->s_numtextures == 0)
	{

		// Load the map texture definitions from textures.lmp.
		// The data is contained in one or two lumps,
		//  TEXTURE1 for shareware, plus TEXTURE2 for commercial.
		maptex = maptex1 = (int*)W_CacheLumpName ("TEXTURE1", PU_CACHE_SHARED); // ALAN:  LOADTIME
		numtextures1 = LONG(*maptex);
		maxoff = W_LumpLength (W_GetNumForName ("TEXTURE1"));
		directory = maptex+1;

		if (W_CheckNumForName ("TEXTURE2") != -1)
		{
			maptex2 = (int*)W_CacheLumpName ("TEXTURE2", PU_CACHE_SHARED); // ALAN:  LOADTIME
			numtextures2 = LONG(*maptex2);
			maxoff2 = W_LumpLength (W_GetNumForName ("TEXTURE2"));
		}
		else
		{
			maptex2 = NULL;
			numtextures2 = 0;
			maxoff2 = 0;
		}


		::g->s_numtextures = numtextures1 + numtextures2;

		::g->s_textures = (texture_t**)DoomLib::Z_Malloc (::g->s_numtextures*sizeof(texture_t*), PU_STATIC_SHARED, 0);
		::g->s_texturecolumnlump = (short**)DoomLib::Z_Malloc (::g->s_numtextures*sizeof(short*), PU_STATIC_SHARED, 0);
		::g->s_texturecolumnofs = (unsigned**)DoomLib::Z_Malloc (::g->s_numtextures*sizeof(unsigned*), PU_STATIC_SHARED, 0);
		::g->s_texturewidthmask = (int*)DoomLib::Z_Malloc (::g->s_numtextures*4, PU_STATIC_SHARED, 0);
		::g->s_textureheight = (fixed_t*)DoomLib::Z_Malloc (::g->s_numtextures*4, PU_STATIC_SHARED, 0);
		::g->s_texturecomposite = (byte**)DoomLib::Z_Malloc (::g->s_numtextures*sizeof(byte*), PU_STATIC_SHARED, 0);
		::g->s_texturecompositesize = (int*)DoomLib::Z_Malloc (::g->s_numtextures*4, PU_STATIC_SHARED, 0);

		totalwidth = 0;

		//	Really complex printing shit...
		temp1 = W_GetNumForName ("S_START");  // P_???????
		temp2 = W_GetNumForName ("S_END") - 1;
		temp3 = ((temp2-temp1+63)/64) + ((::g->s_numtextures+63)/64);
		I_Printf("[");
		/*for (i = 0; i < temp3; i++)
			I_Printf(" ");
		I_Printf("         ]");*/
		/*for (i = 0; i < temp3; i++)
			I_Printf("\x8");
		I_Printf("\x8\x8\x8\x8\x8\x8\x8\x8\x8\x8");	*/

		for (i=0 ; i < ::g->s_numtextures ; i++, directory++)
		{
			if (!(i & 63))
				I_Printf(".");

			if (i == numtextures1)
			{
				// Start looking in second texture file.
				maptex = maptex2;
				maxoff = maxoff2;
				directory = maptex+1;
			}
		
			offset = LONG(*directory);

			if (offset > maxoff)
				I_Error ("R_InitTextures: bad texture directory");
		
			mtexture = (maptexture_t *) ( (byte *)maptex + offset);
			texture = ::g->s_textures[i] = (texture_t*)DoomLib::Z_Malloc (sizeof(texture_t)
				+ sizeof(texpatch_t)*(SHORT(mtexture->patchcount)-1), PU_STATIC_SHARED, 0);

			texture->width = SHORT(mtexture->width);
			texture->height = SHORT(mtexture->height);
			texture->patchcount = SHORT(mtexture->patchcount);

			memcpy (texture->name, mtexture->name, sizeof(texture->name));
			mpatch = &mtexture->patches[0];
			patch = &texture->patches[0];

			for (j=0 ; j<texture->patchcount ; j++, mpatch++, patch++)
			{
				patch->originx = SHORT(mpatch->originx);
				patch->originy = SHORT(mpatch->originy);
				patch->patch = patchlookup[SHORT(mpatch->patch)];
				if (patch->patch == -1)
				{
					I_Error ("R_InitTextures: Missing patch in texture %s",
					texture->name);
				}
			}		
			::g->s_texturecolumnlump[i] = (short*)DoomLib::Z_Malloc (texture->width*2, PU_STATIC_SHARED,0);
			::g->s_texturecolumnofs[i] = (unsigned*)DoomLib::Z_Malloc (texture->width*sizeof(unsigned), PU_STATIC_SHARED,0);//GK:Use sizeof(unsigned) for better stability

			j = 1;
			while (j*2 <= texture->width)
				j<<=1;

			::g->s_texturewidthmask[i] = j-1;
			::g->s_textureheight[i] = texture->height<<FRACBITS;

			totalwidth += texture->width;
		}
		I_Printf("]");

		Z_Free(maptex1);
		if (maptex2)
			Z_Free(maptex2);


		// Precalculate whatever possible.	
		for (i=0 ; i < ::g->s_numtextures ; i++)
			R_GenerateLookup (i);
	}

	// ALAN:  These animations are done globally -- can it be shared?
	// Create translation table for global animation.
	::g->texturetranslation = (int*)DoomLib::Z_Malloc ((::g->s_numtextures+1)*4, PU_STATIC, 0);

	for (i=0 ; i < ::g->s_numtextures ; i++)
		::g->texturetranslation[i] = i;	
}

//
// R_ClearTextures
// Clear the texture list
//  with the s_textures from the world map.
//
void R_ClearTextures(void) {
	
	for (int i = 0; i < ::g->s_numtextures; i++) {
		Z_Free(::g->s_texturecolumnlump[i]);
		Z_Free(::g->s_texturecolumnofs[i]);
		Z_Free(::g->s_textures[i]);
	}
	Z_Free(::g->s_textures);
	Z_Free(::g->s_texturecolumnlump);
	Z_Free(::g->s_texturecolumnofs);
	Z_Free(::g->s_texturewidthmask);
	Z_Free(::g->s_textureheight);
	Z_Free(::g->s_texturecomposite);
	Z_Free(::g->s_texturecompositesize);
	Z_Free(::g->texturetranslation);
	if (::g->skybuffer) {
		free(::g->skybuffer);
		::g->skybuffer = NULL;
	}
	::g->s_numtextures = 0;
}

//
// R_InitFlats
//
void R_InitFlats (void)
{
    int		i;
	
    ::g->firstflat = W_GetNumForName ("F_START") + 1;
    ::g->lastflat = W_GetNumForName ("F_END") - 1;
    ::g->numflats = ::g->lastflat - ::g->firstflat + 1;
	
    // Create translation table for global animation.
    ::g->flattranslation = (int*)DoomLib::Z_Malloc ((::g->numflats+1)*4, PU_STATIC, 0);
    
    for (i=0 ; i < ::g->numflats ; i++)
	::g->flattranslation[i] = i;
}

//
// R_ClearFlats
//
void R_ClearFlats(void)
{
	::g->firstflat = 0;
	::g->lastflat = 0;
	::g->numflats = 0;

	Z_Free(::g->flattranslation);
}


//
// R_InitSpriteLumps
// Finds the width and hoffset of all ::g->sprites in the wad,
//  so the sprite does not need to be cached completely
//  just for having the header info ready during rendering.
//
void R_InitSpriteLumps (void)
{
    int		i;
    patch_t	*patch;
	
    ::g->firstspritelump = W_GetNumForName ("S_START") + 1;
    ::g->lastspritelump = W_GetNumForName ("S_END") - 1;
    
    ::g->numspritelumps = ::g->lastspritelump - ::g->firstspritelump + 1;
    ::g->spritewidth = (fixed_t*)DoomLib::Z_Malloc (::g->numspritelumps*4, PU_STATIC, 0);
	::g->spriteheight = (int*)DoomLib::Z_Malloc(::g->numspritelumps * 4, PU_STATIC, 0);
    ::g->spriteoffset = (fixed_t*)DoomLib::Z_Malloc (::g->numspritelumps*4, PU_STATIC, 0);
    ::g->spritetopoffset = (fixed_t*)DoomLib::Z_Malloc (::g->numspritelumps*4, PU_STATIC, 0);
	
    for (i=0 ; i< ::g->numspritelumps ; i++)
    {
	if (!(i&63))
	    I_Printf (".");
	patch = /*(patch_t*)*/img2lmp(W_CacheLumpNum (::g->firstspritelump+i, PU_CACHE_SHARED),::g->firstspritelump + i);
	::g->spriteheight[i] = SHORT(patch->height);//GK:Get the sprite height
	::g->spritewidth[i] = SHORT(patch->width)<<FRACBITS;
	::g->spriteoffset[i] = SHORT(patch->leftoffset)<<FRACBITS;
	::g->spritetopoffset[i] = SHORT(patch->topoffset)<<FRACBITS;
    }
}

//
// R_ClearSpriteLumps
//
void R_ClearSpriteLumps(void)
{
	::g->firstspritelump = 0;
	::g->lastspritelump = 0;

	::g->numspritelumps = 0;
	Z_Free(::g->spritewidth);
	Z_Free(::g->spriteheight);
	Z_Free(::g->spriteoffset);
	Z_Free(::g->spritetopoffset);
}


//
// R_InitColormaps
//
// killough 3/20/98: rewritten to allow dynamic colormaps
// and to remove unnecessary 256-byte alignment
//
// killough 4/4/98: Add support for C_START/C_END markers
//

void R_InitColormaps(void)
{
  int i;
  ::g->firstcolormaplump = W_GetNumForName("C_START");
  ::g->lastcolormaplump  = W_GetNumForName("C_END");
  ::g->numcolormaps = ::g->lastcolormaplump - ::g->firstcolormaplump;
  ::g->colormaps = (lighttable_t**) Z_Malloc(sizeof(*::g->colormaps) * ::g->numcolormaps, PU_STATIC, 0);

  ::g->colormaps[0] = (lighttable_t*)W_CacheLumpNum(W_GetNumForName("COLORMAP"), PU_STATIC);

  for (i=1; i<::g->numcolormaps; i++)
  ::g->colormaps[i] = (lighttable_t*)W_CacheLumpNum(i+::g->firstcolormaplump, PU_STATIC);
}

// killough 4/4/98: get colormap number from name
// killough 4/11/98: changed to return -1 for illegal names
// killough 4/17/98: changed to use ns_colormaps tag

int R_ColormapNumForName(const char *name)
{
  int i = 0;
  if (idStr::Cmpn(name,"COLORMAP",8))     // COLORMAP predefined to return 0
    if ((i = (W_CheckNumForName)(name, ns_colormaps)) != -1)
      i -= ::g->firstcolormaplump;
  return i;
}

//
// R_InitTranMap
//
// Initialize translucency filter map
//
// By Lee Killough 2/21/98
//

int tran_filter_pct = 66;       // filter percent

#define TSC 12        /* number of fixed point digits in filter percent */

//GK: No Point to keep it in file. Generate it everytime
void R_InitTranMap(int progress)
{
	// Compose a default transparent filter map based on PLAYPAL.
	unsigned char *playpal = (unsigned char *)W_CacheLumpName("PLAYPAL", PU_STATIC);

	::g->main_tranmap = (byte *)Z_Malloc(256 * 256, PU_STATIC, 0); // killough 4/11/98

	long pal[3][256], tot[256], pal_w1[3][256];
	long w1 = ((unsigned long)tran_filter_pct << TSC) / 100;
	long w2 = (1l << TSC) - w1;

	// First, convert playpal into long int type, and transpose array,
	// for fast inner-loop calculations. Precompute tot array.

	int i = 255;
	const unsigned char *p = playpal + 255 * 3;
	do
	{
		long t, d;
		pal_w1[0][i] = (pal[0][i] = t = p[0]) * w1;
		d = t * t;
		pal_w1[1][i] = (pal[1][i] = t = p[1]) * w1;
		d += t * t;
		pal_w1[2][i] = (pal[2][i] = t = p[2]) * w1;
		d += t * t;
		p -= 3;
		tot[i] = d << (TSC - 1);
	} while (--i >= 0);

	// Next, compute all entries using minimum arithmetic.

	int k, j;
	byte *tp = ::g->main_tranmap;
	for (k = 0; k < 256; k++)
	{
		long r1 = pal[0][k] * w2;
		long g1 = pal[1][k] * w2;
		long b1 = pal[2][k] * w2;

		for (j = 0; j < 256; j++, tp++)
		{
			int color = 255;
			long err;
			long r = pal_w1[0][j] + r1;
			long g = pal_w1[1][j] + g1;
			long b = pal_w1[2][j] + b1;
			long best = LONG_MAX;
			do
				if ((err = tot[color] - pal[0][color] * r - pal[1][color] * g - pal[2][color] * b) < best)
					best = err, *tp = color;
			while (--color >= 0);
		}
	}

	Z_ChangeTag(playpal, PU_CACHE);
}

//
// R_InitData
// Locates all the lumps
//  that will be used by all views
// Must be called after W_Init.
//
void R_InitData (void)
{
    R_InitTextures ();
    I_Printf ("\nInitTextures");
    R_InitFlats ();
    I_Printf ("\nInitFlats");
    R_InitSpriteLumps ();
    I_Printf ("\nInitSprites");
	R_InitTranMap(1);     
	I_Printf ("\nInitTranMap");              // killough 2/21/98, 3/6/98
    R_InitColormaps ();
    I_Printf ("\nInitColormaps");
}

//
// R_ClearData
// Clear all listed data 
//  that has been loaded.
// Must be called when changing the aspect ratio in menu.
//
void R_ClearData(void)
{
	R_ClearTextures();
	R_ClearFlats();
	R_ClearSpriteLumps();
}



//
// R_FlatNumForName
// Retrieval, get a flat number for a flat name.
//
int R_FlatNumForName (const char* name)
{
    int		i;
    char	namet[9];
    i = W_CheckNumForName (name, ::g->lastflat + 1);

    if (i == -1)
    {
		namet[8] = 0;
		memcpy (namet, name,8);
		I_Error ("R_FlatNumForName: %s not found",namet);
    }
    return i - ::g->firstflat;
}




//
// R_CheckTextureNumForName
// Check whether texture is available.
// Filter out NoTexture indicator.
//
int	R_CheckTextureNumForName (const char *name)
{
    int		i;

    // "NoTexture" marker.
    if (name[0] == '-')		
	return 0;
		
    for (i=0 ; i < ::g->s_numtextures ; i++)
	if ( !idStr::Icmpn( ::g->s_textures[i]->name, name, 8 ) )
	    return i;
		
    return -1;
}



//
// R_TextureNumForName
// Calls R_CheckTextureNumForName,
//  aborts with error message.
//
int	R_TextureNumForName (const char* name)
{
    int		i;
    i = R_CheckTextureNumForName (name);

    if (i==-1)
    {
	I_Error ("R_TextureNumForName: %s not found",
		 name);
    }
    return i;
}




//
// R_PrecacheLevel
// Preloads all relevant graphics for the level.
//

void R_PrecacheLevel (void)
{
    int			i;
    int			j;
    int			k;
    int			lump;
    
    texture_t*		texture;
    thinker_t*		th;
    spriteframe_t*	sf;

    if (::g->demoplayback)
	return;
    
    // Precache flats.
	std::vector<char> flatpresent(::g->numflats, 0);
    
    for (i=0 ; i < ::g->numsectors ; i++)
    {
	flatpresent[::g->sectors[i].floorpic] = 1;
	flatpresent[::g->sectors[i].ceilingpic] = 1;
    }
	
    ::g->flatmemory = 0;

    for (i=0 ; i < ::g->numflats ; i++)
    {
	if (flatpresent[i])
	{
	    lump = ::g->firstflat + i;
	    ::g->flatmemory += lumpinfo[lump].size;
	    W_CacheLumpNum(lump, PU_CACHE_SHARED);
	}
    }
    
    // Precache textures.
    std::vector<char> texturepresent(::g->s_numtextures, 0);
	
    for (i=0 ; i < ::g->numsides ; i++)
    {
	texturepresent[::g->sides[i].toptexture] = 1;
	texturepresent[::g->sides[i].midtexture] = 1;
	texturepresent[::g->sides[i].bottomtexture] = 1;
    }

    // Sky texture is always present.
    // Note that F_SKY1 is the name used to
    //  indicate a sky floor/ceiling as a flat,
    //  while the sky texture is stored like
    //  a wall texture, with an episode dependend
    //  name.
    texturepresent[::g->skytexture] = 1;
	
    ::g->texturememory = 0;
    for (i=0 ; i < ::g->s_numtextures ; i++)
    {
	if (!texturepresent[i])
	    continue;

	texture = ::g->s_textures[i];
	
	for (j=0 ; j<texture->patchcount ; j++)
	{
	    lump = texture->patches[j].patch;
	    ::g->texturememory += lumpinfo[lump].size;
	    W_CacheLumpNum(lump , PU_CACHE_SHARED);
	}
    }
    
    // Precache ::g->sprites.
    std::vector<char> spritepresent(::g->numsprites, 0);
	
    for (th = ::g->thinkercap.next ; th != &::g->thinkercap ; th=th->next)
    {
		bool countIn = false;
		if (const actionf_p1* currentAction = std::get_if<actionf_p1>(&th->function)) {
			countIn = (*currentAction) == (actionf_p1)P_MobjThinker;
		}
		if (countIn)
	    spritepresent[((mobj_t *)th)->sprite] = 1;
    }
	
    ::g->spritememory = 0;
    for (i=0 ; i < ::g->numsprites ; i++)
    {
	if (!spritepresent[i])
	    continue;

	for (j=0 ; j < ::g->sprites[i]->numframes ; j++)
	{
	    sf = &::g->sprites[i]->spriteframes[j];
	    for (k=0 ; k<8 ; k++)
	    {
		lump = ::g->firstspritelump + sf->lump[k];
		::g->spritememory += lumpinfo[lump].size;
		W_CacheLumpNum(lump , PU_CACHE_SHARED);
	    }
	}
    }
}





