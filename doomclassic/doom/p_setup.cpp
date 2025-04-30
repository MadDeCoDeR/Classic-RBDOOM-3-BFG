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


#include <math.h>

#include "z_zone.h"

#include "m_swap.h"
#include "m_bbox.h"

#include "g_game.h"

#include "i_system.h"
#include "w_wad.h"

#include "doomdef.h"
#include "p_local.h"

#include "s_sound.h"

#include "doomstat.h"

#include "d_udmf.h"
#include "d_act.h"
#include "nano_bsp.h"
//#ifdef USE_OPENAL
#include "s_efx.h"

#include "sound/OpenAL/AL_EAX.h"
//#endif
void	P_SpawnMapThing (mapthing_t*	mthing);

//
// MAP related Lookup tables.
// Store VERTEXES, LINEDEFS, SIDEDEFS, etc.
//





// BLOCKMAP
// Created from axis aligned bounding box
// of the map, a rectangular array of
// blocks of size ...
// Used to speed up collision detection
// by spatial subdivision in 2D.
//
// Blockmap size.
// offsets in ::g->blockmap are from here
// origin of block map
// for thing chains


// REJECT
// For fast sight rejection.
// Speeds up enemy AI by skipping detailed
//  LineOf Sight calculation.
// Without special effect, this could be
//  used as a PVS lookup as well.
//


// Maintain single and multi player starting spots.






//
// P_LoadVertexes
//
void P_LoadVertexes (int lump)
{
	byte*		data;
	int			i;
	mapvertex_t*	ml;
	vertex_t*		li;

	// Determine number of lumps:
	//  total lump length / vertex record length.
	::g->numvertexes = W_LumpLength (lump) / sizeof(mapvertex_t);

	// Allocate zone memory for buffer.
//	::g->vertexes = (vertex_t*)Z_Malloc (::g->numvertexes*sizeof(vertex_t),PU_LEVEL,0);	
	if (MallocForLump( lump, ::g->numvertexes*sizeof(vertex_t ), ::g->vertexes, PU_VERTEX ))
	{
		// Load data into cache.
		data = (byte*)W_CacheLumpNum (lump,PU_CACHE_SHARED); // ALAN: LOADTIME

		ml = (mapvertex_t *)data;
		li = ::g->vertexes;

		// Copy and convert vertex coordinates,
		// internal representation as fixed.
		for (i=0 ; i < ::g->numvertexes ; i++, li++, ml++)
		{
			li->x = SHORT(ml->x)<<FRACBITS;
			li->y = SHORT(ml->y)<<FRACBITS;
		}

		// Free buffer memory.
		Z_Free(data);
	}
}



//
// P_LoadSegs
//
void P_LoadSegs (int lump)
{
	byte*		data;
	int			i;
	mapseg_t*		ml;
	seg_t*		li;
	line_t*		ldef;
	int			psetup_linedef;
	int			side;

	::g->numsegs = W_LumpLength (lump) / sizeof(mapseg_t);
//	::g->segs = (seg_t*)Z_Malloc (::g->numsegs*sizeof(seg_t),PU_LEVEL,0);	

	if (MallocForLump( lump, ::g->numsegs*sizeof(seg_t), ::g->segs, PU_SEGS ))
	{
		memset (::g->segs, 0, ::g->numsegs*sizeof(seg_t));
		data = (byte*)W_CacheLumpNum (lump,PU_CACHE_SHARED); // ALAN: LOADTIME

		ml = (mapseg_t *)data;
		li = ::g->segs;
		for (i=0 ; i < ::g->numsegs ; i++, li++, ml++)
		{
			li->v1 = &::g->vertexes[SHORT(ml->v1) & 0xffff];
			li->v2 = &::g->vertexes[SHORT(ml->v2) & 0xffff];

			li->angle = (SHORT(ml->angle))<<16;
			li->offset = (SHORT(ml->offset))<<16;
			psetup_linedef = SHORT(ml->linedef) & 0xffff;
			ldef = &::g->lines[psetup_linedef];
			li->linedef = ldef;
			side = SHORT(ml->side) & 0xffff;
			li->sidedef = &::g->sides[ldef->sidenum[side]];
			li->frontsector = ::g->sides[ldef->sidenum[side]].sector;
			if (ldef-> flags & ML_TWOSIDED)
				li->backsector = ::g->sides[ldef->sidenum[side^1]].sector;
			else
				li->backsector = 0;
		}

		Z_Free(data);
	}
}


//
// P_LoadSubsectors
//
void P_LoadSubsectors (int lump)
{
	byte*		data;
	int			i;
	mapsubsector_t*	ms;
	subsector_t*	ss;

	::g->numsubsectors = W_LumpLength (lump) / sizeof(mapsubsector_t);

	if (MallocForLump( lump, ::g->numsubsectors*sizeof(subsector_t), ::g->subsectors, PU_SECTORS ))
	{
		data = (byte*)W_CacheLumpNum (lump,PU_CACHE_SHARED); // ALAN: LOADTIME

		ms = (mapsubsector_t *)data;
		memset (::g->subsectors,0, ::g->numsubsectors*sizeof(subsector_t));
		ss = ::g->subsectors;

		for (i=0 ; i < ::g->numsubsectors ; i++, ss++, ms++)
		{
			ss->numlines = (int)ms->numsegs & 0xffff;
			ss->firstline = (int)ms->firstseg & 0xffff;
		}

		Z_Free(data);
	}
}



//
// P_LoadSectors
//
void P_LoadSectors (int lump)
{
	byte*		data;
	int			i;
	mapsector_t*	ms;
	sector_t*		ss;

	::g->numsectors = W_LumpLength (lump) / sizeof(mapsector_t);
	
	::g->sectors = (sector_t*)Z_Malloc( ::g->numsectors*sizeof(sector_t), PU_SECTORS, NULL );
	memset (::g->sectors, 0, ::g->numsectors*sizeof(sector_t));
	data = (byte*)W_CacheLumpNum (lump,PU_CACHE_SHARED); // ALAN: LOADTIME

	ms = (mapsector_t *)data;
	ss = ::g->sectors;
	for (i=0 ; i < ::g->numsectors ; i++, ss++, ms++)
	{
		ss->floorheight = SHORT(ms->floorheight)<<FRACBITS;
		ss->ceilingheight = SHORT(ms->ceilingheight)<<FRACBITS;
		ss->floorpic = R_FlatNumForName(ms->floorpic);
		ss->ceilingpic = R_FlatNumForName(ms->ceilingpic);
		ss->lightlevel = SHORT(ms->lightlevel);
		ss->special = SHORT(ms->special);
		ss->tag = SHORT(ms->tag);
		ss->thinglist = NULL;
		//ss->touching_thinglist = NULL;            // phares 3/14/98
		//GK: Load the reverbs based on sector's index
		ss->counter = i;
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
		if (!common->UseAlternativeAudioAPI()) {
#endif
			if (::g->hasreverb) {
				if ((int)::g->reverbs.size() < i + 1) {
					::g->reverbs.push_back(GetReverb(strdup(::g->mapname.c_str()), i));
				}
				else {
					::g->reverbs[i] = GetReverb(strdup(::g->mapname.c_str()), i);
				}
			}
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
		}
#endif
	}

	Z_Free(data);

/*
	if (MallocForLump( lump, ::g->numsectors*sizeof(sector_t), (void**)&::g->sectors, PU_LEVEL_SHARED ))
	{
		memset (::g->sectors, 0, ::g->numsectors*sizeof(sector_t));
		data = (byte*)W_CacheLumpNum (lump,PU_CACHE_SHARED); // ALAN: LOADTIME

		ms = (mapsector_t *)data;
		ss = ::g->sectors;
		for (i=0 ; i < ::g->numsectors ; i++, ss++, ms++)
		{
			ss->floorheight = SHORT(ms->floorheight)<<FRACBITS;
			ss->ceilingheight = SHORT(ms->ceilingheight)<<FRACBITS;
			ss->floorpic = R_FlatNumForName(ms->floorpic);
			ss->ceilingpic = R_FlatNumForName(ms->ceilingpic);
			ss->lightlevel = SHORT(ms->lightlevel);
			ss->special = SHORT(ms->special);
			ss->tag = SHORT(ms->tag);
			ss->thinglist = NULL;
		}

		DoomLib::Z_Free(data);
	}
*/	
}


//
// P_LoadNodes
//
void P_LoadNodes (int lump)
{
	byte*	data;
	int		i;
	int		j;
	int		k;
	mapnode_t*	mn;
	node_t*	no;

	::g->numnodes = W_LumpLength (lump) / sizeof(mapnode_t);
	if (MallocForLump( lump, ::g->numnodes*sizeof(node_t), ::g->nodes, PU_NODE ))
	{
		data = (byte*)W_CacheLumpNum (lump,PU_CACHE_SHARED); // ALAN: LOADTIME

		mn = (mapnode_t *)data;
		no = ::g->nodes;

		for (i=0 ; i < ::g->numnodes ; i++, no++, mn++)
		{
			no->x = SHORT(mn->x)<<FRACBITS;
			no->y = SHORT(mn->y)<<FRACBITS;
			no->dx = SHORT(mn->dx)<<FRACBITS;
			no->dy = SHORT(mn->dy)<<FRACBITS;
			for (j=0 ; j<2 ; j++)
			{
				no->children[j] = SHORT(mn->children[j]);
				for (k=0 ; k<4 ; k++)
					no->bbox[j][k] = SHORT(mn->bbox[j][k])<<FRACBITS;
			}
		}

		Z_Free(data);
	}
}


//
// P_LoadThings
//
void P_LoadThings (int lump)
{
	byte*		data;
	int			i;
	mapthing_t*		mt;
	int			numthings;
	qboolean		spawn;

	data = (byte*)W_CacheLumpNum (lump,PU_CACHE_SHARED); // ALAN: LOADTIME
	numthings = (W_LumpLength (lump) / sizeof(mapthing_t));

	mt = (mapthing_t *)data;
	for (i=0 ; i<numthings ; i++, mt++)
	{
		spawn = true;

		// Do not spawn cool, new monsters if !commercial
		if ( ::g->gamemode != commercial)
		{
			if ((::g->gamemission == pack_custom && ::g->clusters[::g->gameepisode - 1].allowall))
			{
				//do nothing
			}
			else {
				switch (mt->type)
				{
				case 68:	// Arachnotron
				case 64:	// Archvile
				case 88:	// Boss Brain
				case 89:	// Boss Shooter
				case 69:	// Hell Knight
				case 67:	// Mancubus
				case 71:	// Pain Elemental
				case 65:	// Former Human Commando
				case 66:	// Revenant
				case 84:	// Wolf SS
					spawn = false;
					break;
				}
			}
		}
		if (spawn == false)
			break;

		// Do spawn all other stuff. 
		mt->x = SHORT(mt->x);
		mt->y = SHORT(mt->y);
		mt->angle = SHORT(mt->angle);
		mt->type = SHORT(mt->type);
		mt->options = SHORT(mt->options);

		if (::g->gameskill == sk_masochism && (mt->type == 2011 || mt->type == 2012)) {
			continue;
		}

		P_SpawnMapThing (mt);
	}

	Z_Free(data);
}

#if 0
//
// P_LoadLineDefs
// Also counts secret ::g->lines for intermissions.
//
void P_LoadLineDefs (int lump)
{
	byte*		data;
	int			i;
	maplinedef_t*	mld;
	line_t*		ld;
	vertex_t*		v1;
	vertex_t*		v2;

	::g->numlines = W_LumpLength (lump) / sizeof(maplinedef_t);
	if (MallocForLump( lump, ::g->numlines*sizeof(line_t), ::g->lines, PU_LINES ))
	{
		memset (::g->lines, 0, ::g->numlines*sizeof(line_t));
		data = (byte*)W_CacheLumpNum (lump,PU_CACHE_SHARED); // ALAN: LOADTIME

		mld = (maplinedef_t *)data;
		ld = ::g->lines;
		for (i=0 ; i < ::g->numlines ; i++, mld++, ld++)
		{
			ld->flags = SHORT(mld->flags);
			ld->special = SHORT(mld->special);
			ld->tag = SHORT(mld->tag);
			v1 = ld->v1 = &::g->vertexes[SHORT(mld->v1) & 0xffff];
			v2 = ld->v2 = &::g->vertexes[SHORT(mld->v2) & 0xffff];
			ld->dx = v2->x - v1->x;
			ld->dy = v2->y - v1->y;

			if (!ld->dx)
				ld->slopetype = ST_VERTICAL;
			else if (!ld->dy)
				ld->slopetype = ST_HORIZONTAL;
			else
			{
				if (FixedDiv (ld->dy , ld->dx) > 0)
					ld->slopetype = ST_POSITIVE;
				else
					ld->slopetype = ST_NEGATIVE;
			}

			if (v1->x < v2->x)
			{
				ld->bbox[BOXLEFT] = v1->x;
				ld->bbox[BOXRIGHT] = v2->x;
			}
			else
			{
				ld->bbox[BOXLEFT] = v2->x;
				ld->bbox[BOXRIGHT] = v1->x;
			}

			if (v1->y < v2->y)
			{
				ld->bbox[BOXBOTTOM] = v1->y;
				ld->bbox[BOXTOP] = v2->y;
			}
			else
			{
				ld->bbox[BOXBOTTOM] = v2->y;
				ld->bbox[BOXTOP] = v1->y;
			}

			ld->sidenum[0] = (int)mld->sidenum[0] == -1 ? -1l : (int)mld->sidenum[0] & 0xffff;
			ld->sidenum[1] = (int)mld->sidenum[1] == -1 ? -1l : (int)mld->sidenum[1] & 0xffff;
		//	if (ld->sidenum[0] != -1)
				ld->frontsector = ::g->sides[ld->sidenum[0]].sector;
		//	else
		//		ld->frontsector = 0;

			if (ld->sidenum[1] != -1)
				ld->backsector = ::g->sides[ld->sidenum[1]].sector;
			else
				ld->backsector = 0;
		}

		Z_Free(data);
	}
}


//
// P_LoadSideDefs
//
void P_LoadSideDefs (int lump)
{
	byte*		data;
	int			i;
	mapsidedef_t*	msd;
	side_t*		sd;

	::g->numsides = W_LumpLength (lump) / sizeof(mapsidedef_t);
	if (MallocForLump( lump, ::g->numsides*sizeof(side_t), ::g->sides, PU_SIDES))
	{
		memset (::g->sides, 0, ::g->numsides*sizeof(side_t));
		data = (byte*)W_CacheLumpNum (lump,PU_CACHE_SHARED); // ALAN: LOADTIME

		msd = (mapsidedef_t *)data;
		sd = ::g->sides;
		for (i=0 ; i < ::g->numsides ; i++, msd++, sd++)
		{
			sd->textureoffset = SHORT(msd->textureoffset)<<FRACBITS;
			sd->rowoffset = SHORT(msd->rowoffset)<<FRACBITS;
			sd->toptexture = R_TextureNumForName(msd->toptexture);
			sd->bottomtexture = R_TextureNumForName(msd->bottomtexture);
			sd->midtexture = R_TextureNumForName(msd->midtexture);
			sd->sector = &::g->sectors[SHORT(msd->sector) & 0xffff];
		}

		Z_Free(data);
	}
}
#endif

//
// P_LoadLineDefs
// Also counts secret lines for intermissions.
//        ^^^
// ??? killough ???
// Does this mean secrets used to be linedef-based, rather than sector-based?
//
// killough 4/4/98: split into two functions, to allow sidedef overloading
//
// killough 5/3/98: reformatted, cleaned up

void P_LoadLineDefs (int lump)
{
  byte *data;
  int  i;

  ::g->numlines = W_LumpLength (lump) / sizeof(maplinedef_t);
  ::g->lines = (line_t*)Z_Malloc (::g->numlines*sizeof(line_t),PU_LINES,0);
  memset (::g->lines, 0, ::g->numlines*sizeof(line_t));
  data = (byte*)W_CacheLumpNum (lump,PU_LINES);

  for (i=0; i<::g->numlines; i++)
    {
      maplinedef_t *mld = (maplinedef_t *) data + i;
      line_t *ld = ::g->lines+i;
      vertex_t *v1, *v2;

      ld->flags = SHORT(mld->flags);
      ld->special = SHORT(mld->special);
      ld->tag = SHORT(mld->tag);
      v1 = ld->v1 = &::g->vertexes[SHORT(mld->v1)];
      v2 = ld->v2 = &::g->vertexes[SHORT(mld->v2)];
      ld->dx = v2->x - v1->x;
      ld->dy = v2->y - v1->y;

      ld->tranlump = -1;   // killough 4/11/98: no translucency by default

      ld->slopetype = !ld->dx ? ST_VERTICAL : !ld->dy ? ST_HORIZONTAL :
        FixedDiv(ld->dy, ld->dx) > 0 ? ST_POSITIVE : ST_NEGATIVE;

      if (v1->x < v2->x)
        {
          ld->bbox[BOXLEFT] = v1->x;
          ld->bbox[BOXRIGHT] = v2->x;
        }
      else
        {
          ld->bbox[BOXLEFT] = v2->x;
          ld->bbox[BOXRIGHT] = v1->x;
        }

      if (v1->y < v2->y)
        {
          ld->bbox[BOXBOTTOM] = v1->y;
          ld->bbox[BOXTOP] = v2->y;
        }
      else
        {
          ld->bbox[BOXBOTTOM] = v2->y;
          ld->bbox[BOXTOP] = v1->y;
        }

      ld->sidenum[0] = SHORT(mld->sidenum[0]);
      ld->sidenum[1] = SHORT(mld->sidenum[1]);

      // killough 4/4/98: support special sidedef interpretation below
      if (ld->sidenum[0] != -1 && ld->special)
	  ::g->sides[*ld->sidenum].special = ld->special;
    }
  Z_Free (data);
}

// killough 4/4/98: delay using sidedefs until they are loaded
// killough 5/3/98: reformatted, cleaned up

void P_LoadLineDefs2(int lump)
{
  int i = ::g->numlines;
  line_t *ld = ::g->lines;
  for (;i--;ld++)
    {
      // killough 11/98: fix common wad errors (missing sidedefs):

      if (ld->sidenum[0] == -1)
	ld->sidenum[0] = 0;  // Substitute dummy sidedef for missing right side

      if (ld->sidenum[1] == -1)
	ld->flags &= ~ML_TWOSIDED;  // Clear 2s flag for missing left side

      ld->frontsector = ld->sidenum[0]!=-1 ? ::g->sides[ld->sidenum[0]].sector : NULL;
      ld->backsector  = ld->sidenum[1]!=-1 ? ::g->sides[ld->sidenum[1]].sector : NULL;
      switch (ld->special)
        {                       // killough 4/11/98: handle special types
          int lump, j;

        case 260:               // killough 4/11/98: translucent 2s textures
            lump = ::g->sides[*ld->sidenum].special; // translucency from sidedef
            if (!ld->tag)                       // if tag==0,
              ld->tranlump = lump;              // affect this linedef only
            else
              for (j=0;j<::g->numlines;j++)          // if tag!=0,
                if (::g->lines[j].tag == ld->tag)    // affect all matching linedefs
				::g->lines[j].tranlump = lump;
            break;
        }
    }
}

//
// P_LoadSideDefs
//
// killough 4/4/98: split into two functions

void P_LoadSideDefs (int lump)
{
	::g->numsides = W_LumpLength(lump) / sizeof(mapsidedef_t);
	::g->sides = (side_t*)Z_Malloc(::g->numsides*sizeof(side_t),PU_SIDES,0);
  	memset(::g->sides, 0, ::g->numsides*sizeof(side_t));
}

// killough 4/4/98: delay using texture names until
// after linedefs are loaded, to allow overloading.
// killough 5/3/98: reformatted, cleaned up

void P_LoadSideDefs2(int lump)
{
  byte *data = (byte*)W_CacheLumpNum(lump,PU_SIDES);
  int  i;

  for (i=0; i<::g->numsides; i++)
    {
      mapsidedef_t *msd = (mapsidedef_t *) data + i;
      side_t *sd = ::g->sides + i;
      sector_t *sec;

      sd->textureoffset = SHORT(msd->textureoffset)<<FRACBITS;
      sd->rowoffset = SHORT(msd->rowoffset)<<FRACBITS;

      // killough 4/4/98: allow sidedef texture names to be overloaded
      // killough 4/11/98: refined to allow colormaps to work as wall
      // textures if invalid as colormaps but valid as textures.

      sd->sector = sec = &::g->sectors[SHORT(msd->sector)];
      switch (sd->special)
        {
        case 242:                       // variable colormap via 242 linedef
          sd->bottomtexture =
            (sec->bottommap =   R_ColormapNumForName(msd->bottomtexture)) < 0 ?
            sec->bottommap = 0, R_TextureNumForName(msd->bottomtexture): 0 ;
          sd->midtexture =
            (sec->midmap =   R_ColormapNumForName(msd->midtexture)) < 0 ?
            sec->midmap = 0, R_TextureNumForName(msd->midtexture)  : 0 ;
          sd->toptexture =
            (sec->topmap =   R_ColormapNumForName(msd->toptexture)) < 0 ?
            sec->topmap = 0, R_TextureNumForName(msd->toptexture)  : 0 ;
          break;

        case 260: // killough 4/11/98: apply translucency to 2s normal texture
          sd->midtexture = idStr::Cmpn("TRANMAP", msd->midtexture, 8) ?
            (sd->special = W_CheckNumForName(msd->midtexture)) < 0 ||
            W_LumpLength(sd->special) != 65536 ?
            sd->special=0, R_TextureNumForName(msd->midtexture) :
              (sd->special++, 0) : (sd->special=0);
          sd->toptexture = R_TextureNumForName(msd->toptexture);
          sd->bottomtexture = R_TextureNumForName(msd->bottomtexture);
          break;

        default:                        // normal cases
          sd->midtexture = R_TextureNumForName(msd->midtexture);
          sd->toptexture = R_TextureNumForName(msd->toptexture);
          sd->bottomtexture = R_TextureNumForName(msd->bottomtexture);
          break;
        }
    }
  Z_Free (data);
}

#if 0

// jff 10/6/98
// New code added to speed up calculation of internal blockmap
// Algorithm is order of nlines*(ncols+nrows) not nlines*ncols*nrows
//

#define blkshift 7               /* places to shift rel position for cell num */
#define blkmask ((1<<blkshift)-1)/* mask for rel position within cell */
#define blkmargin 0              /* size guardband around map used */
                                 // jff 10/8/98 use guardband>0
                                 // jff 10/12/98 0 ok with + 1 in rows,cols

typedef struct linelist_t        // type used to list lines in each block
{
  long num;
  struct linelist_t *next;
} linelist_t;

//
// Subroutine to add a line number to a block list
// It simply returns if the line is already in the block
//

static void AddBlockLine
(
  linelist_t **lists,
  int *count,
  int *done,
  int blockno,
  long lineno
)
{
  linelist_t *l;

  if (done[blockno])
    return;

  l = (linelist_t*)malloc(sizeof(linelist_t));
  l->num = lineno;
  l->next = lists[blockno];
  lists[blockno] = l;
  count[blockno]++;
  done[blockno] = 1;
}

static void P_CreateBlockMap(void)
{
  int xorg,yorg;                 // blockmap origin (lower left)
  int nrows,ncols;               // blockmap dimensions
  linelist_t **blocklists=NULL;  // array of pointers to lists of lines
  int *blockcount=NULL;          // array of counters of line lists
  int *blockdone=NULL;           // array keeping track of blocks/line
  int NBlocks;                   // number of cells = nrows*ncols
  long linetotal=0;              // total length of all blocklists
  int i,j;
  int map_minx=INT_MAX;          // init for map limits search
  int map_miny=INT_MAX;
  int map_maxx=INT_MIN;
  int map_maxy=INT_MIN;

  // scan for map limits, which the blockmap must enclose

  // This fixes MBF's code, which has a bug where maxx/maxy
  // are wrong if the 0th node has the largest x or y
  if (::g->numvertexes)
  {
    map_minx = map_maxx = ::g->vertexes[0].x;
    map_miny = map_maxy = ::g->vertexes[0].y;
  }

  for (i=0;i<::g->numvertexes;i++)
  {
    fixed_t t;

    if ((t=::g->vertexes[i].x) < map_minx)
      map_minx = t;
    else if (t > map_maxx)
      map_maxx = t;
    if ((t=::g->vertexes[i].y) < map_miny)
      map_miny = t;
    else if (t > map_maxy)
      map_maxy = t;
  }
  map_minx >>= FRACBITS;    // work in map coords, not fixed_t
  map_maxx >>= FRACBITS;
  map_miny >>= FRACBITS;
  map_maxy >>= FRACBITS;

  // set up blockmap area to enclose level plus margin

  xorg = map_minx-blkmargin;
  yorg = map_miny-blkmargin;
  ncols = (map_maxx+blkmargin-xorg+1+blkmask)>>blkshift;  //jff 10/12/98
  nrows = (map_maxy+blkmargin-yorg+1+blkmask)>>blkshift;  //+1 needed for
  NBlocks = ncols*nrows;                                  //map exactly 1 cell

  // create the array of pointers on NBlocks to blocklists
  // also create an array of linelist counts on NBlocks
  // finally make an array in which we can mark blocks done per line

  // CPhipps - calloc's
  blocklists = (linelist_t **)calloc(NBlocks,sizeof(linelist_t *));
  blockcount = (int*)calloc(NBlocks,sizeof(int));
  blockdone = (int*)malloc(NBlocks*sizeof(int));

  // initialize each blocklist, and enter the trailing -1 in all blocklists
  // note the linked list of lines grows backwards

  for (i=0;i<NBlocks;i++)
  {
    blocklists[i] = (linelist_t*)malloc(sizeof(linelist_t));
    blocklists[i]->num = -1;
    blocklists[i]->next = NULL;
    blockcount[i]++;
  }

  // For each linedef in the wad, determine all blockmap blocks it touches,
  // and add the linedef number to the blocklists for those blocks

  for (i=0;i<::g->numlines;i++)
  {
    int x1 = ::g->lines[i].v1->x>>FRACBITS;         // lines[i] map coords
    int y1 = ::g->lines[i].v1->y>>FRACBITS;
    int x2 = ::g->lines[i].v2->x>>FRACBITS;
    int y2 = ::g->lines[i].v2->y>>FRACBITS;
    int dx = x2-x1;
    int dy = y2-y1;
    int vert = !dx;                            // lines[i] slopetype
    int horiz = !dy;
    int spos = (dx^dy) > 0;
    int sneg = (dx^dy) < 0;
    int bx,by;                                 // block cell coords
    int minx = x1>x2? x2 : x1;                 // extremal lines[i] coords
    int maxx = x1>x2? x1 : x2;
    int miny = y1>y2? y2 : y1;
    int maxy = y1>y2? y1 : y2;

    // no blocks done for this linedef yet

    memset(blockdone,0,NBlocks*sizeof(int));

    // The line always belongs to the blocks containing its endpoints

    bx = (x1-xorg)>>blkshift;
    by = (y1-yorg)>>blkshift;
    AddBlockLine(blocklists,blockcount,blockdone,by*ncols+bx,i);
    bx = (x2-xorg)>>blkshift;
    by = (y2-yorg)>>blkshift;
    AddBlockLine(blocklists,blockcount,blockdone,by*ncols+bx,i);


    // For each column, see where the line along its left edge, which
    // it contains, intersects the Linedef i. Add i to each corresponding
    // blocklist.

    if (!vert)    // don't interesect vertical lines with columns
    {
      for (j=0;j<ncols;j++)
      {
        // intersection of Linedef with x=xorg+(j<<blkshift)
        // (y-y1)*dx = dy*(x-x1)
        // y = dy*(x-x1)+y1*dx;

        int x = xorg+(j<<blkshift);       // (x,y) is intersection
        int y = (dy*(x-x1))/dx+y1;
        int yb = (y-yorg)>>blkshift;      // block row number
        int yp = (y-yorg)&blkmask;        // y position within block

        if (yb<0 || yb>nrows-1)     // outside blockmap, continue
          continue;

        if (x<minx || x>maxx)       // line doesn't touch column
          continue;

        // The cell that contains the intersection point is always added

        AddBlockLine(blocklists,blockcount,blockdone,ncols*yb+j,i);

        // if the intersection is at a corner it depends on the slope
        // (and whether the line extends past the intersection) which
        // blocks are hit

        if (yp==0)        // intersection at a corner
        {
          if (sneg)       //   \ - blocks x,y-, x-,y
          {
            if (yb>0 && miny<y)
              AddBlockLine(blocklists,blockcount,blockdone,ncols*(yb-1)+j,i);
            if (j>0 && minx<x)
              AddBlockLine(blocklists,blockcount,blockdone,ncols*yb+j-1,i);
          }
          else if (spos)  //   / - block x-,y-
          {
            if (yb>0 && j>0 && minx<x)
              AddBlockLine(blocklists,blockcount,blockdone,ncols*(yb-1)+j-1,i);
          }
          else if (horiz) //   - - block x-,y
          {
            if (j>0 && minx<x)
              AddBlockLine(blocklists,blockcount,blockdone,ncols*yb+j-1,i);
          }
        }
        else if (j>0 && minx<x) // else not at corner: x-,y
          AddBlockLine(blocklists,blockcount,blockdone,ncols*yb+j-1,i);
      }
    }

    // For each row, see where the line along its bottom edge, which
    // it contains, intersects the Linedef i. Add i to all the corresponding
    // blocklists.

    if (!horiz)
    {
      for (j=0;j<nrows;j++)
      {
        // intersection of Linedef with y=yorg+(j<<blkshift)
        // (x,y) on Linedef i satisfies: (y-y1)*dx = dy*(x-x1)
        // x = dx*(y-y1)/dy+x1;

        int y = yorg+(j<<blkshift);       // (x,y) is intersection
        int x = (dx*(y-y1))/dy+x1;
        int xb = (x-xorg)>>blkshift;      // block column number
        int xp = (x-xorg)&blkmask;        // x position within block

        if (xb<0 || xb>ncols-1)   // outside blockmap, continue
          continue;

        if (y<miny || y>maxy)     // line doesn't touch row
          continue;

        // The cell that contains the intersection point is always added

        AddBlockLine(blocklists,blockcount,blockdone,ncols*j+xb,i);

        // if the intersection is at a corner it depends on the slope
        // (and whether the line extends past the intersection) which
        // blocks are hit

        if (xp==0)        // intersection at a corner
        {
          if (sneg)       //   \ - blocks x,y-, x-,y
          {
            if (j>0 && miny<y)
              AddBlockLine(blocklists,blockcount,blockdone,ncols*(j-1)+xb,i);
            if (xb>0 && minx<x)
              AddBlockLine(blocklists,blockcount,blockdone,ncols*j+xb-1,i);
          }
          else if (vert)  //   | - block x,y-
          {
            if (j>0 && miny<y)
              AddBlockLine(blocklists,blockcount,blockdone,ncols*(j-1)+xb,i);
          }
          else if (spos)  //   / - block x-,y-
          {
            if (xb>0 && j>0 && miny<y)
              AddBlockLine(blocklists,blockcount,blockdone,ncols*(j-1)+xb-1,i);
          }
        }
        else if (j>0 && miny<y) // else not on a corner: x,y-
          AddBlockLine(blocklists,blockcount,blockdone,ncols*(j-1)+xb,i);
      }
    }
  }

  // Add initial 0 to all blocklists
  // count the total number of lines (and 0's and -1's)

  memset(blockdone,0,NBlocks*sizeof(int));
  for (i=0,linetotal=0;i<NBlocks;i++)
  {
    AddBlockLine(blocklists,blockcount,blockdone,i,0);
    linetotal += blockcount[i];
  }

  // Create the blockmap lump

  //blockmaplump = malloc_IfSameLevel(blockmaplump, sizeof(*blockmaplump) * (4 + NBlocks + linetotal));
  ::g->blockmaplump = (long*)Z_Malloc(sizeof(*::g->blockmaplump) * (4 + NBlocks + linetotal), PU_BLOCKMAP, 0);

  // blockmap header

  ::g->blockmaplump[0] = ::g->bmaporgx = xorg << FRACBITS;
  ::g->blockmaplump[1] = ::g->bmaporgy = yorg << FRACBITS;
  ::g->blockmaplump[2] = ::g->bmapwidth  = ncols;
  ::g->blockmaplump[3] = ::g->bmapheight = nrows;

  // offsets to lists and block lists

  for (i=0;i<NBlocks;i++)
  {
    linelist_t *bl = blocklists[i];
    long offs = ::g->blockmaplump[4+i] =   // set offset to block's list
      (i? ::g->blockmaplump[4+i-1] : 4+NBlocks) + (i? blockcount[i-1] : 0);

    // add the lines in each block's list to the blockmaplump
    // delete each list node as we go

    while (bl)
    {
      linelist_t *tmp = bl->next;
      ::g->blockmaplump[offs++] = bl->num;
      free(bl);
      bl = tmp;
    }
  }

  // free all temporary storage

  free(blocklists);
  free(blockcount);
  free(blockdone);
}
#endif

//
// killough 10/98:
//
// Rewritten to use faster algorithm.
//
// New procedure uses Bresenham-like algorithm on the linedefs, adding the
// linedef to each block visited from the beginning to the end of the linedef.
//
// The algorithm's complexity is on the order of nlines*total_linedef_length.
//
// Please note: This section of code is not interchangable with TeamTNT's
// code which attempts to fix the same problem.

static void P_CreateBlockMap(void)
{
  int i;
  fixed_t minx = INT_MAX, miny = INT_MAX, maxx = INT_MIN, maxy = INT_MIN;

  // First find limits of map

  for (i=0; i<::g->numvertexes; i++)
    {
      if (::g->vertexes[i].x >> FRACBITS < minx)
	minx = ::g->vertexes[i].x >> FRACBITS;
      else
	if (::g->vertexes[i].x >> FRACBITS > maxx)
	  maxx = ::g->vertexes[i].x >> FRACBITS;
      if (::g->vertexes[i].y >> FRACBITS < miny)
	miny = ::g->vertexes[i].y >> FRACBITS;
      else
	if (::g->vertexes[i].y >> FRACBITS > maxy)
	  maxy = ::g->vertexes[i].y >> FRACBITS;
    }

  // Save blockmap parameters

  ::g->bmaporgx = minx << FRACBITS;
  ::g->bmaporgy = miny << FRACBITS;
  ::g->bmapwidth  = ((maxx-minx) >> MAPBTOFRAC) + 1;
  ::g->bmapheight = ((maxy-miny) >> MAPBTOFRAC) + 1;

  // Compute blockmap, which is stored as a 2d array of variable-sized lists.
  //
  // Pseudocode:
  //
  // For each linedef:
  //
  //   Map the starting and ending vertices to blocks.
  //
  //   Starting in the starting vertex's block, do:
  //
  //     Add linedef to current block's list, dynamically resizing it.
  //
  //     If current block is the same as the ending vertex's block, exit loop.
  //
  //     Move to an adjacent block by moving towards the ending block in 
  //     either the x or y direction, to the block which contains the linedef.

  {
    typedef struct { int n, nalloc, *list; } bmap_t;  // blocklist structure
    unsigned tot = ::g->bmapwidth * ::g->bmapheight;            // size of blockmap
    bmap_t *bmap = (bmap_t*)calloc(sizeof *bmap, tot); // array of blocklists

    for (i=0; i < ::g->numlines; i++)
      {
	// starting coordinates
	int x = (::g->lines[i].v1->x >> FRACBITS) - minx;
	int y = (::g->lines[i].v1->y >> FRACBITS) - miny;
	
	// x-y deltas
	int adx = ::g->lines[i].dx >> FRACBITS, dx = adx < 0 ? -1 : 1;
	int ady = ::g->lines[i].dy >> FRACBITS, dy = ady < 0 ? -1 : 1; 

	// difference in preferring to move across y (>0) instead of x (<0)
	int diff = !adx ? 1 : !ady ? -1 :
	  (((x >> MAPBTOFRAC) << MAPBTOFRAC) + 
	   (dx > 0 ? MAPBLOCKUNITS-1 : 0) - x) * (ady = abs(ady)) * dx -
	  (((y >> MAPBTOFRAC) << MAPBTOFRAC) + 
	   (dy > 0 ? MAPBLOCKUNITS-1 : 0) - y) * (adx = abs(adx)) * dy;

	// starting block, and pointer to its blocklist structure
	int b = (y >> MAPBTOFRAC)*::g->bmapwidth + (x >> MAPBTOFRAC);

	// ending block
	int bend = (((::g->lines[i].v2->y >> FRACBITS) - miny) >> MAPBTOFRAC) *
	::g->bmapwidth + (((::g->lines[i].v2->x >> FRACBITS) - minx) >> MAPBTOFRAC);

	// delta for pointer when moving across y
	dy *= ::g->bmapwidth;

	// deltas for diff inside the loop
	adx <<= MAPBTOFRAC;
	ady <<= MAPBTOFRAC;

	// Now we simply iterate block-by-block until we reach the end block.
	while ((unsigned) b < tot)    // failsafe -- should ALWAYS be true
	  {
	    // Increase size of allocated list if necessary
	    if (bmap[b].n >= bmap[b].nalloc)
	      bmap[b].list = (int*) realloc(bmap[b].list, 
				     (bmap[b].nalloc = bmap[b].nalloc ? 
				      bmap[b].nalloc*2 : 8)*sizeof*bmap->list);

	    // Add linedef to end of list
	    bmap[b].list[bmap[b].n++] = i;

	    // If we have reached the last block, exit
	    if (b == bend)
	      break;

	    // Move in either the x or y direction to the next block
	    if (diff < 0) 
	      diff += ady, b += dx;
	    else
	      diff -= adx, b += dy;
	  }
      }

    // Compute the total size of the blockmap.
    //
    // Compression of empty blocks is performed by reserving two offset words
    // at tot and tot+1.
    //
    // 4 words, unused if this routine is called, are reserved at the start.

    {
      int count = tot+6;  // we need at least 1 word per block, plus reserved's

      for (i = 0; i < tot; i++)
	if (bmap[i].n)
	  count += bmap[i].n + 2; // 1 header word + 1 trailer word + blocklist

      // Allocate blockmap lump with computed count
      ::g->blockmaplump =(long*) Z_Malloc(sizeof(*::g->blockmaplump) * count, PU_BLOCKMAP, NULL);
    }									 

    // Now compress the blockmap.
    {
      int ndx = tot += 4;         // Advance index to start of linedef lists
      bmap_t *bp = bmap;          // Start of uncompressed blockmap

      ::g->blockmaplump[ndx++] = 0;    // Store an empty blockmap list at start
      ::g->blockmaplump[ndx++] = -1;   // (Used for compression)

      for (i = 4; i < tot; i++, bp++)
	if (bp->n)                                      // Non-empty blocklist
	  {
	    ::g->blockmaplump[::g->blockmaplump[i] = ndx++] = 0;  // Store index & header
	    do
		::g->blockmaplump[ndx++] = bp->list[--bp->n];  // Copy linedef list
	    while (bp->n);
	    ::g->blockmaplump[ndx++] = -1;                   // Store trailer
	    free(bp->list);                           // Free linedef list
	  }
	else            // Empty blocklist: point to reserved empty blocklist
	::g->blockmaplump[i] = tot;

      free(bmap);    // Free uncompressed blockmap
    }
  }
}

//
// P_LoadBlockMap
//
void P_LoadBlockMap (int lump)
{
	if (::g->needsNodeRebuild) {
		P_CreateBlockMap();
	} else {
		int		i;
		int		count;

		bool firstTime = false;
		if (!lumpcache[lump]) {			// SMF - solution for double endian conversion issue
			firstTime = true;
		}
		//GK:Remove blockmap limit
		short* bl;
		bl = (short*)W_CacheLumpNum (lump,PU_BLOCKMAP); // ALAN: This is initialized somewhere else as shared...

		
		count = W_LumpLength (lump)/2;

		if ( firstTime ) {				// SMF
			::g->blockmaplump = (long*)Z_Malloc(sizeof(*::g->blockmaplump)*count, PU_BLOCKMAP, 0);
			::g->blockmaplump[0] = SHORT(bl[0]);
			::g->blockmaplump[1] = SHORT(bl[1]);
			::g->blockmaplump[2] = (long)(SHORT(bl[2])) & FRACMASK;
			::g->blockmaplump[3] = (long)(SHORT(bl[3])) & FRACMASK;
			for (i = 0; i < count; i++) {
				short t = SHORT(bl[i]);
				::g->blockmaplump[i] = t == -1 ? -1l : (long)t & FRACMASK;
			}
			Z_Free(bl);
		}

		::g->bmaporgx = ( ::g->blockmaplump[0] )<<FRACBITS;
		::g->bmaporgy = ( ::g->blockmaplump[1] )<<FRACBITS;
		::g->bmapwidth = ( ::g->blockmaplump[2] );
		::g->bmapheight = ( ::g->blockmaplump[3] );

		// clear out mobj chains
		count = sizeof(*::g->blocklinks)* ::g->bmapwidth*::g->bmapheight;
		::g->blocklinks = (mobj_t**)Z_Malloc (count,PU_BLOCKMAP, 0);
		memset (::g->blocklinks, 0, count);
		::g->blockmap = ::g->blockmaplump + 4;
	}
}

//P_ResetAct
//Make sure that the next map,game,expansion
//will not gonna carry the changes ActMap did
//in any sector
void P_ResetAct() {
	for (uint i = 0; i < ::g->acts.size(); i++) {
		if (!::g->acts[i].empty()) {
			for (actdef_t* act : ::g->acts[i]) {
				if (act->cvar) {
					cvarSystem->SetCVarString(act->cvar, act->oldValue);
				}
			}
		}
	}
	::g->hasacts = false;
	::g->oldsec = 0;
}

//P_ActMap
//Load the act map for later use
void P_ActMap(int lump) {
	loadacts(lump);
}

//
// P_GroupLines
// Builds sector line lists and subsector sector numbers.
// Finds block bounding boxes for ::g->sectors.
//
void P_GroupLines (void)
{
	line_t**		linebuffer;
	int			i;
	int			j;
	int			total;
	line_t*		li;
	sector_t*		sector;
	subsector_t*	ss;
	seg_t*		seg;
	fixed_t		bbox[4];
	int			block;

	
	// look up sector number for each subsector
	ss = ::g->subsectors;
	for (i=0 ; i < ::g->numsubsectors ; i++, ss++)
	{
		if(ss->firstline<0)
			break;
		seg = &::g->segs[ss->firstline];
		ss->sector = seg->sidedef->sector;
	}

	// count number of ::g->lines in each sector
	li = ::g->lines;
	total = 0;
	for (i=0 ; i < ::g->numlines ; i++, li++)
	{
		total++;
		li->frontsector->linecount++;

		if (li->backsector && li->backsector != li->frontsector)
		{
			li->backsector->linecount++;
			total++;
		}
	}

	// build line tables for each sector	
	linebuffer = (line_t**)Z_Malloc (total*sizeof(line_t*), PU_LINES, 0);
	sector = ::g->sectors;
	for (i=0 ; i < ::g->numsectors ; i++, sector++)
	{
		M_ClearBox (bbox);
		sector->lines = linebuffer;
		li = ::g->lines;
		for (j=0 ; j < ::g->numlines ; j++, li++)
		{
			if (li->frontsector == sector || li->backsector == sector)
			{
				*linebuffer++ = li;
				M_AddToBox (bbox, li->v1->x, li->v1->y);
				M_AddToBox (bbox, li->v2->x, li->v2->y);
			}
		}
		if (linebuffer - sector->lines != sector->linecount)
			I_Error ("P_GroupLines: miscounted");

		// set the degenmobj_t to the middle of the bounding box
		sector->soundorg.x = (bbox[BOXRIGHT]+bbox[BOXLEFT])/2;
		sector->soundorg.y = (bbox[BOXTOP]+bbox[BOXBOTTOM])/2;

		// adjust bounding box to map blocks
		block = (bbox[BOXTOP]-::g->bmaporgy+MAXRADIUS)>>MAPBLOCKSHIFT;
		block = block >= ::g->bmapheight ? ::g->bmapheight-1 : block;
		sector->blockbox[BOXTOP]=block;

		block = (bbox[BOXBOTTOM]-::g->bmaporgy-MAXRADIUS)>>MAPBLOCKSHIFT;
		block = block < 0 ? 0 : block;
		sector->blockbox[BOXBOTTOM]=block;

		block = (bbox[BOXRIGHT]-::g->bmaporgx+MAXRADIUS)>>MAPBLOCKSHIFT;
		block = block >= ::g->bmapwidth ? ::g->bmapwidth-1 : block;
		sector->blockbox[BOXRIGHT]=block;

		block = (bbox[BOXLEFT]-::g->bmaporgx-MAXRADIUS)>>MAPBLOCKSHIFT;
		block = block < 0 ? 0 : block;
		sector->blockbox[BOXLEFT]=block;
	}

}

//
// killough 10/98
//
// Remove slime trails.
//
// Slime trails are inherent to Doom's coordinate system -- i.e. there is
// nothing that a node builder can do to prevent slime trails ALL of the time,
// because it's a product of the integer coordinate system, and just because
// two lines pass through exact integer coordinates, doesn't necessarily mean
// that they will intersect at integer coordinates. Thus we must allow for
// fractional coordinates if we are to be able to split segs with node lines,
// as a node builder must do when creating a BSP tree.
//
// A wad file does not allow fractional coordinates, so node builders are out
// of luck except that they can try to limit the number of splits (they might
// also be able to detect the degree of roundoff error and try to avoid splits
// with a high degree of roundoff error). But we can use fractional coordinates
// here, inside the engine. It's like the difference between square inches and
// square miles, in terms of granularity.
//
// For each vertex of every seg, check to see whether it's also a vertex of
// the linedef associated with the seg (i.e, it's an endpoint). If it's not
// an endpoint, and it wasn't already moved, move the vertex towards the
// linedef by projecting it using the law of cosines. Formula:
//
//      2        2                         2        2
//    dx  x0 + dy  x1 + dx dy (y0 - y1)  dy  y0 + dx  y1 + dx dy (x0 - x1)
//   {---------------------------------, ---------------------------------}
//                  2     2                            2     2
//                dx  + dy                           dx  + dy
//
// (x0,y0) is the vertex being moved, and (x1,y1)-(x1+dx,y1+dy) is the
// reference linedef.
//
// Segs corresponding to orthogonal linedefs (exactly vertical or horizontal
// linedefs), which comprise at least half of all linedefs in most wads, don't
// need to be considered, because they almost never contribute to slime trails
// (because then any roundoff error is parallel to the linedef, which doesn't
// cause slime). Skipping simple orthogonal lines lets the code finish quicker.
//
// Please note: This section of code is not interchangable with TeamTNT's
// code which attempts to fix the same problem.
//
// Firelines (TM) is a Rezistered Trademark of MBF Productions
//

void P_RemoveSlimeTrails(void)                // killough 10/98
{
	byte *hit =(byte*) calloc(1, ::g->numvertexes);         // Hitlist for vertices
	int i;
	for (i = 0; i<::g->numsegs; i++)                   // Go through each seg
	{
		const line_t *l = ::g->segs[i].linedef;      // The parent linedef
		if (l->dx && l->dy)                     // We can ignore orthogonal lines
		{
			vertex_t *v = ::g->segs[i].v1;
			do
				if (!hit[v - ::g->vertexes])           // If we haven't processed vertex
				{
					hit[v - ::g->vertexes] = 1;        // Mark this vertex as processed
					if (v != l->v1 && v != l->v2) // Exclude endpoints of linedefs
					{ // Project the vertex back onto the parent linedef
						int64_t dx2 = (l->dx >> FRACBITS) * (l->dx >> FRACBITS);
						int64_t dy2 = (l->dy >> FRACBITS) * (l->dy >> FRACBITS);
						int64_t dxy = (l->dx >> FRACBITS) * (l->dy >> FRACBITS);
						int64_t s = dx2 + dy2;
						int x0 = v->x, y0 = v->y, x1 = l->v1->x, y1 = l->v1->y;
						v->x = (fixed_t)((dx2 * x0 + dy2 * x1 + dxy * (y0 - y1)) / s);
						v->y = (fixed_t)((dy2 * y0 + dx2 * y1 + dxy * (x0 - x1)) / s);
					}
				}  // Obfuscated C contest entry:   :)
			while ((v != ::g->segs[i].v2) && ((v = ::g->segs[i].v2) != 0));
		}
	}
	free(hit);
}


//
// P_SetupLevel
//
void
P_SetupLevel
( int		episode,
 int		map,
 int		playermask,
 skill_t	skill)
{
	int		i;
	char	lumpname[17];
	int		lumpnum;

	::g->totalkills = ::g->totalitems = ::g->totalsecret = ::g->wminfo.maxfrags = 0;
	::g->wminfo.partime = 3600 * TICRATE; //GK: Time Really SUCKS
	for (i=0 ; i<MAXPLAYERS ; i++)
	{
		::g->players[i].killcount = ::g->players[i].secretcount 
			= ::g->players[i].itemcount = 0;

		::g->players[i].berserkKills = 0;
		::g->players[i].barrelKills = 0;
		::g->players[i].lastHitBarrel = mapthing_t();
		::g->players[i].bfgTargets = 0;
		::g->players[i].inBFGStates = false;
		::g->players[i].CGunShoots = 0;
		::g->players[i].fistKills = 0;
		::g->players[i].shotgunKills = 0;
		::g->players[i].doubleShotgunKills = 0;
		::g->players[i].lastShotgunKillTime = 0;
		::g->players[i].gotHit = false;
		::g->players[i].missleGibs = 0;
		::g->players[i].missleGibTime = 0;
		::g->players[i].plasmaKills = 0;
		::g->players[i].lastPlasmaKillTime = 0;
		::g->players[i].calamityKills = 0;
		::g->players[i].lastCalamityKillTime = 0;
	}

	// Initial height of PointOfView
	// will be set by player think.
	::g->players[::g->consoleplayer].viewz = 1; 
	P_ResetAct();
	// Make sure all sounds are stopped before Z_FreeTags.
	S_Start ();			

	Z_FreeTags( PU_MUSIC, PU_PURGELEVEL-1 );
	//GK: New level new flips
	::g->flip = false;
	::g->isfliped = false;
	//GK: Make sure previous reverb effects are freed
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (!common->UseAlternativeAudioAPI()) {
#endif
		if (::g->hasreverb) {
			for (int i1 = ::g->reverbs.size() - 1; i1 >= 0; i1--) {
				Mem_Free(::g->reverbs[i1]);
				::g->reverbs.pop_back();
			}
			alAuxiliaryEffectSlotiRef((ALuint)::g->clslot, AL_EFFECTSLOT_EFFECT, AL_EFFECTSLOT_NULL);
		}
		::g->mapindex = 0;
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	}
#endif
	// UNUSED W_Profile ();
	P_InitThinkers ();
	
	// if working with a devlopment map, reload it
	// W_Reload ();

	// DHM - NERVE :: Update the cached asset pointers in case the wad files were reloaded
	{
		void ST_loadData(void);
		ST_loadData();
		
		void HU_Init(void);
		HU_Init();
	}

	// find map name
	if ( ::g->gamemode == commercial)
	{
		sprintf (lumpname,"MAP%02i", map);

		if (::g->gamemission == pack_custom ) { //GK:Custom expansion related stuff
			sprintf(lumpname, "%s", ::g->maps[map-1].lumpname);
		}
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
		if (!common->UseAlternativeAudioAPI()) {
#endif
			//GK: Get Map's name in order to check for reverbs
			switch (::g->gamemission) {
			case doom2:
				::g->mapname = mapnames2[map - 1];
				break;
			case pack_tnt:
				::g->mapname = mapnamest[map - 1];
				break;
			case pack_plut:
				::g->mapname = mapnamesp[map - 1];
				break;
			default:
				::g->mapname = DoomLib::GetCurrentExpansion()->mapNames[map - 1];
				break;
			}
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
		}
#endif
	}
	else
	{
		
			lumpname[0] = 'E';
			lumpname[1] = '0' + episode;
			lumpname[2] = 'M';
			lumpname[3] = '0' + map;
			lumpname[4] = 0;

			if (episode < 5 && map < 10) {
				idLib::Printf("%s\n", mapnames[(episode - 1) * 9 + (map - 1)]);
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
				if (!common->UseAlternativeAudioAPI())
#endif
					::g->mapname = mapnames[(episode - 1) * 9 + (map - 1)];
				
			}
			if (::g->gamemission == pack_custom) { //GK:Custom expansion related stuff
				if ((int)::g->clusters.size() >= episode) {
					if (!::g->clusters[episode - 1].startmap && ::g->clusters[episode - 1].mapname != NULL) {
						for (int i2 = 0; i2 < (int)::g->maps.size(); i2++) {
							if (!idStr::Icmp(::g->clusters[episode - 1].mapname, ::g->maps[i2].lumpname)) {
								::g->clusters[episode - 1].startmap = i2;
							}
						}
					}
				if (::g->clusters[episode - 1].startmap) {
					int tempmap = map >= ::g->clusters[episode - 1].startmap ? (map - ::g->clusters[episode - 1].startmap) : (map - 1);
					int newmap = ::g->clusters[episode - 1].startmap + tempmap;
					::g->endmap = ::g->clusters[episode - 1].endmap;
					if (::g->maps[newmap - 1].lumpname != NULL) {
						sprintf(lumpname, "%s", ::g->maps[newmap - 1].lumpname);
						if (::g->maps[newmap - 1].realname != NULL) {
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
							if (!common->UseAlternativeAudioAPI())
#endif
								::g->mapname = ::g->maps[newmap - 1].realname;
							
						}
					}
				}
				}
			}
	}

	lumpnum = W_GetNumForName (lumpname);

	::g->leveltime = 0;
	bool buildNodes = false;
	for (int i3 = 0; i3 < 6; i3++)
		::g->normalpowers[i3] = 0;

	bool isudmf = false;
	//GK: Check if the map is udmf
	if (!idStr::Icmp(W_GetNameForNum(lumpnum + 1), "TEXTMAP")) {
		isudmf = true;
		LoadUdmf(lumpnum + 1);
		typedef void(*loadaction)(int);
		typedef struct {
			const char* name;
			loadaction func;
		}loaddata;
		//GK: Check if other map lumps are present between the TEXTMAP and the ENDMAP lumps
		loaddata ld[9] = {
			{"THINGS",P_LoadThings},
			{"LINEDEFS",P_LoadLineDefs},
			{"SIDEDEFS",P_LoadSideDefs},
			{"VERTEXES",P_LoadVertexes},
			{"SEGS",P_LoadSegs},
			{"SSECTORS",P_LoadSubsectors},
			{"NODES",P_LoadNodes},
			{"SECTORS",P_LoadSectors},
			{"BLOCKMAP",P_LoadBlockMap}
		};
		int i4 = 2;
		int j = 1;
		while (idStr::Icmp(W_GetNameForNum(lumpnum + i4), "ENDMAP")) {
			i4++;
		}
		//GK: Get them from the end to the start for better stability
		while (idStr::Icmp(W_GetNameForNum(lumpnum+i4 - j), "TEXTMAP")) {
			bool cont = false;
			for (int k = 0; k < 9; k++) {
				if (!idStr::Icmp(W_GetNameForNum(lumpnum+i4 - j), ld[k].name)) {
					ld[k].func(lumpnum+i4-j);
					j++;
					cont = true;
					break;
				}
			}
			if (cont)
				continue;

			if (!idStr::Icmp(W_GetNameForNum(lumpnum+i4 - j), "REJECT"))
				::g->rejectmatrix = (byte*)W_CacheLumpNum(lumpnum + ML_REJECT, PU_REJECT);

			j++;
		}
	}
	else {

		::g->needsNodeRebuild = W_LumpLength(lumpnum + ML_NODES) == 0 || W_LumpLength(lumpnum + ML_SSECTORS) == 0;
		// note: most of this ordering is important	
		P_ActMap(lumpnum + ML_ACTMAP);
		
		P_LoadVertexes(lumpnum + ML_VERTEXES);
		P_LoadSectors(lumpnum + ML_SECTORS);
		P_LoadSideDefs  (lumpnum+ML_SIDEDEFS);             // killough 4/4/98
		P_LoadLineDefs  (lumpnum+ML_LINEDEFS);             //       |
		P_LoadSideDefs2 (lumpnum+ML_SIDEDEFS);             //       |
		P_LoadLineDefs2 (lumpnum+ML_LINEDEFS);             // killough 4/4/98
		P_LoadBlockMap(lumpnum + ML_BLOCKMAP);
		
		if (::g->needsNodeRebuild) {
			buildNodes = true;
			BSP_BuildNodes();

		} else {
			P_LoadSubsectors(lumpnum + ML_SSECTORS);
			P_LoadNodes(lumpnum + ML_NODES);
			P_LoadSegs(lumpnum + ML_SEGS);
		}

		::g->rejectmatrix = (byte*)W_CacheLumpNum(lumpnum + ML_REJECT, PU_REJECT);
	}

	P_GroupLines ();

	if (!buildNodes) {
		P_RemoveSlimeTrails();    // killough 10/98: remove slime trails from wad
	}

	::g->bodyqueslot = 0;
	::g->deathmatch_p = ::g->deathmatchstarts;
	if (!isudmf) {
		P_LoadThings(lumpnum + ML_THINGS);
	}
	else {
		//GK: In udmf we store the mapthings in memory and spawning them later
		for (int o = 0; o < ::g->nummapthings; o++) {
			P_SpawnMapThing(&::g->mapthings[o]);
		}
	}

	// if ::g->deathmatch, randomly spawn the active ::g->players
	if (::g->deathmatch)
	{
		for (i=0 ; i<MAXPLAYERS ; i++)
			if (::g->playeringame[i])
			{
				// DHM - Nerve :: In deathmatch, reset every player at match start
				::g->players[i].playerstate = PST_REBORN;

				::g->players[i].mo = NULL;
				G_DeathMatchSpawnPlayer (i);
			}

	}

	// clear special respawning que
	::g->iquehead = ::g->iquetail = 0;		

	// set up world state
	P_SpawnSpecials ();

	// build subsector connect matrix
	//	UNUSED P_ConnectSubsectors ();

	// preload graphics
	if (::g->precache)
		R_PrecacheLevel ();

	if (::g->demorecording) {
		G_BeginRecording();
	}
}



//
// P_Init
//
void P_Init (void)
{
	P_InitSwitchList ();
	P_InitPicAnims ();
	R_InitSprites (sprnames);
}




