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

#ifndef __R_DEFS__
#define __R_DEFS__

#include "Precompiled.h"

// Screenwidth.
#include "doomdef.h"

// Some more or less basic data types
// we depend on.
#include "m_fixed.h"

// We rely on the thinker data struct
// to handle sound origins in sectors.
#include "d_think.h"
// SECTORS do store MObjs anyway.
#include "p_mobj.h"


// Silhouette, needed for clipping Segs (mainly)
// and sprites representing things.
#define SIL_NONE		0
#define SIL_BOTTOM		1
#define SIL_TOP			2
#define SIL_BOTH		3

#define MAXDRAWSEGS		256 //GK:The more the less HOMs we get





//
// INTERNAL MAP TYPES
//  used by play and refresh
//

//
// Your plain vanilla vertex.
// Note: transformed values not buffered locally,
//  like some DOOM-alikes ("wt", "WebView") did.
//
typedef struct
{
    fixed_t	x;
    fixed_t	y;
    
} vertex_t;


// Forward of LineDefs, for Sectors.
struct line_s;

// Each sector has a degenmobj_t in its center
//  for sound origin purposes.
// I suppose this does not handle sound from
//  moving objects (doppler), because
//  position is prolly just buffered, not
//  updated.
typedef struct
{
    thinker_t		thinker;	// not used for anything
    fixed_t		x;
    fixed_t		y;
    fixed_t		z;

} degenmobj_t;

//
// The SECTORS record, at runtime.
// Stores things/mobjs.
//
typedef	struct
{
    fixed_t	floorheight;
    fixed_t	ceilingheight;
    short	floorpic;
    short	ceilingpic;
    short	lightlevel;
    short	special;
    short	tag;

    // 0 = untraversed, 1,2 = sndlines -1
    int		soundtraversed;

    // thing that made a sound (or null)
    mobj_t*	soundtarget;

    // mapblock bounding box for height changes
    int		blockbox[4];

    // origin for any sounds played by the sector
    degenmobj_t	soundorg;

    // if == validcount, already checked
    int		validcount;

    // list of mobjs in sector
    mobj_t*	thinglist;

      // thinker_t for reversable actions
  void *floordata;    // jff 2/22/98 make thinkers on
  void *ceilingdata;  // floors, ceilings, lighting,
  void *lightingdata; // independent of one another
  short oldspecial;      //jff 2/16/98 remembers if sector WAS secret (automap)
    int			linecount;
    struct line_s**	lines;	// [linecount] size

	// jff 2/26/98 lockout machinery for stairbuilding
	int stairlock;   // -2 on first locked -1 after thinker done 0 normally
	int prevsec;     // -1 or number of sector for previous step
	int nextsec;     // -1 or number of next step sector

	// killough 3/7/98: floor and ceiling texture offsets
	fixed_t   floor_xoffs, floor_yoffs;
	fixed_t ceiling_xoffs, ceiling_yoffs;

	// killough 3/7/98: support flat heights drawn at another sector's heights
	int heightsec;    // other sector, or -1 if no other sector

	// killough 4/11/98: support for lightlevels coming from another sector
	int floorlightsec, ceilinglightsec;

    int bottommap, midmap, topmap; // killough 4/4/98: dynamic colormaps

    // killough 10/98: support skies coming from sidedefs. Allows scrolling
    // skies and other effects. No "level info" kind of lump is needed, 
    // because you can use an arbitrary number of skies per level with this
    // method. This field only applies when skyflatnum is used for floorpic
    // or ceilingpic, because the rest of Doom needs to know which is sky
    // and which isn't, etc.

    int sky;

	// list of mobjs that are at least partially in the sector
    // thinglist is a subset of touching_thinglist
	//struct msecnode_s *touching_thinglist;               // phares 3/14/98 
    
	int counter; //GK: keep the sector's index for reverb
    int firsttag;
    int nexttag;
} sector_t;




//
// The SideDef.
//

typedef struct
{
    // add this to the calculated texture column
    fixed_t	textureoffset;
    
    // add this to the calculated texture top
    fixed_t	rowoffset;

    // Texture indices.
    // We do not maintain names here. 
    short	toptexture;
    short	bottomtexture;
    short	midtexture;

    // Sector the SideDef is facing.
    sector_t*	sector;

    // killough 4/4/98, 4/11/98: highest referencing special linedef's type,
  // or lump number of special effect. Allows texture names to be overloaded
  // for other functions.

  int special;
    
} side_t;



//
// Move clipping aid for LineDefs.
//
typedef enum
{
    ST_HORIZONTAL,
    ST_VERTICAL,
    ST_POSITIVE,
    ST_NEGATIVE

} slopetype_t;



typedef struct line_s
{
    // Vertices, from v1 to v2.
    vertex_t*	v1;
    vertex_t*	v2;

    // Precalculated v2 - v1 for side checking.
    fixed_t	dx;
    fixed_t	dy;

    // Animation related.
    unsigned short	flags;
    short	special;
    short	tag;

    // Visual appearance: SideDefs.
    //  sidenum[1] will be -1 if one sided
    long	sidenum[2];			

    // Neat. Another bounding box, for the extent
    //  of the LineDef.
    fixed_t	bbox[4];

    // To aid move clipping.
    slopetype_t	slopetype;

    // Front and back sector.
    // Note: redundant? Can be retrieved from SideDefs.
    sector_t*	frontsector;
    sector_t*	backsector;

    // if == validcount, already checked
    int		validcount;

    // thinker_t for reversable actions
    void*	specialdata;	
	int tranlump;          // killough 4/11/98: translucency filter, -1 == none

	int firsttag, nexttag;  // killough 4/17/98: improves searches for tags.
} line_t;

typedef struct msecnode_s
{
	sector_t          *m_sector; // a sector containing this object
	struct mobj_t     *m_thing;  // this object
	qboolean visited; // killough 4/4/98, 4/7/98: used in search algorithms
} msecnode_t;


//
// A SubSector.
// References a Sector.
// Basically, this is a list of LineSegs,
//  indicating the visible walls that define
//  (all or some) sides of a convex BSP leaf.
//
typedef struct subsector_s
{
    sector_t*	sector;
    long	numlines;
    long	firstline;
    
} subsector_t;



//
// The LineSeg.
//
typedef struct seg_s
{
    vertex_t*	v1;
    vertex_t*	v2;
    
    fixed_t	offset;

    angle_t	angle;

    side_t*	sidedef;
    line_t*	linedef;

    // Sector references.
    // Could be retrieved from linedef, too.
    // backsector is NULL for one sided lines
    sector_t*	frontsector;
    sector_t*	backsector;
    
    // NanoBSP
    struct seg_s *next;
} seg_t;



//
// BSP node.
//
typedef struct
{
    // Partition line.
    fixed_t	x;
    fixed_t	y;
    fixed_t	dx;
    fixed_t	dy;

    // Bounding box for each child.
    fixed_t	bbox[2][4];

    // If NF_SUBSECTOR its a subsector.
    int children[2];
    
} node_t;

typedef struct Nanode  nanode_t;

struct Nanode
{
	// when non-NULL, this is actually a leaf of the BSP tree
	seg_t * segs;

	// final index number of this node / leaf
	int  index;

	// partition line (start coord, delta to end)
	fixed_t  x, y, dx, dy;

	// right and left children
	struct Nanode * right;
	struct Nanode * left;
};


// posts are runs of non masked source pixels
typedef struct
{
    byte		topdelta;	// -1 is the last post in a column
    byte		length; 	// length data bytes follows
} post_t;

// postColumn_t is a list of 0 or more post_t, (byte)-1 terminated
typedef post_t	postColumn_t;



// PC direct to screen pointers
//B UNUSED - keep till detailshift in r_draw.c resolved
//extern byte*	destview;
//extern byte*	destscreen;





//
// OTHER TYPES
//

// This could be wider for >8 bit display.
// Indeed, true color support is posibble
//  precalculating 24bpp lightmap/colormap LUT.
//  from darkening PLAYPAL to all black.
// Could even us emore than 32 levels.
typedef byte	lighttable_t;	




//
// ?
//
typedef struct drawseg_s
{
    seg_t*		curline;
    int			x1;
    int			x2;

    fixed_t		scale1;
    fixed_t		scale2;
    fixed_t		scalestep;

    // 0=none, 1=bottom, 2=top, 3=both
    int			silhouette;

    // do not clip sprites above this
    fixed_t		bsilheight;

    // do not clip sprites below this
    fixed_t		tsilheight;
    
    // Pointers to lists for sprite clipping,
    //  all three adjusted so [x1] is first value.
    short*		sprtopclip;		
    short*		sprbottomclip;	
    short*		maskedtexturecol;
    
} drawseg_t;



// Patches.
// A patch holds one or more columns.
// Patches are used for sprites and all masked pictures,
// and we compose textures from the TEXTURE1/2 lists
// of patches.
struct patch_t
{ 
    short		width;		// bounding box size 
    short		height; 
    short		leftoffset;	// pixels to the left of origin 
    short		topoffset;	// pixels below the origin 
    int			columnofs[8];	// only [width] used
    // the [0] is &columnofs[width] 
};







// A vissprite_t is a thing
//  that will be drawn during a refresh.
// I.e. a sprite object that is partly visible.
typedef struct vissprite_s
{
	int suck; //Compiler opt fix...wtf

    // Doubly linked list.
    struct vissprite_s*	prev;
    struct vissprite_s*	next;
    
    int			x1;
    int			x2;

    // for line side calculation
    fixed_t		gx;
    fixed_t		gy;		

    // global bottom / top for silhouette clipping
    fixed_t		gz;
    fixed_t		gzt;

    // horizontal position of x1
    fixed_t		startfrac;
    
    fixed_t		scale;
    
    // negative if flipped
    fixed_t		xiscale;	

    fixed_t		texturemid;
    int			patch;

    // for color translation and shadow draw,
    //  maxbright frames as well
    lighttable_t*	colormap;
   
    // killough 3/27/98: height sector for underwater/fake ceiling support
    int heightsec;

    int			mobjflags;
	//GK: Heretic stuff for making the psprite to stuck with the view
	qboolean	psprite;		// true if psprite
	fixed_t		footclip;		// foot clipping
} vissprite_t;


//	
// Sprites are patches with a special naming convention
//  so they can be recognized by R_InitSprites.
// The base name is NNNNFx or NNNNFxFx, with
//  x indicating the rotation, x = 0, 1-7.
// The sprite and frame specified by a thing_t
//  is range checked at run time.
// A sprite is a patch_t that is assumed to represent
//  a three dimensional object and may have multiple
//  rotations pre drawn.
// Horizontal flipping is used to save space,
//  thus NNNNF2F5 defines a mirrored patch.
// Some sprites will only have one picture used
// for all views: NNNNF0
//
typedef struct
{
    // If false use 0 for any position.
    // Note: as eight entries are available,
    //  we might as well insert the same name eight times.
    qboolean	rotate;

    // Lump to use for view angles 0-7.
    short	lump[8];

    // Flip bit (1 = flip) to use for view angles 0-7.
    byte	flip[8];
    
} spriteframe_t;



//
// A sprite definition:
//  a number of animation frames.
//
typedef struct
{
    int			numframes;
    spriteframe_t*	spriteframes;

} spritedef_t;



//
// Now what is a visplane, anyway?
// 
typedef struct
{
  fixed_t		height;
  int			picnum;
  int			lightlevel;
  int			minx;
  int			maxx;
  fixed_t xoffs, yoffs;         // killough 2/28/98: Support scrolling flats
  int           skyflatmapindex; //GK: ID24 SKYDEFS FlatMapping
  // leave pads for [minx-1]/[maxx+1]
  int			nervePad1;
  int			nervePad2;
  byte			nervePad3;
  byte			nervePad4;
  byte			nervePad5;

  byte		pad1;
  // Here lies the rub for all
  //  dynamic resize/change of resolution.
  //unsigned short		top[65535];			// [SCREENWIDTH];
  unsigned short		top[MAXWIDTH];
  byte		pad2;
  byte		pad3;
  // See above.
  //unsigned short		bottom[65535];		// [SCREENWIDTH];
  unsigned short		bottom[MAXWIDTH];
  byte		pad4;

} visplane_t;




#endif

