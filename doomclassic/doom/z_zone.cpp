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

#include "z_zone.h"
#include "i_system.h"
#include "doomdef.h"
#include "globaldata.h"


//
// ZONE MEMORY ALLOCATION
//
// There is never any space between memblocks,
//  and there will never be two contiguous free memblocks.
// The rover can be left pointing at a non-empty block.
//
// It is of no value to free a cachable block,
//  because it will get overwritten automatically if needed.
//

#define ZONEID	0x1d4a11


//
// Z_ClearZone
//
void Z_ClearZone (memzone_t* zone)
{
    memblock_t*		block;
	
    // set the entire zone to one free block
    zone->blocklist.next =
	zone->blocklist.prev =
	block = (memblock_t *)( (byte *)zone + sizeof(memzone_t) );
    
    zone->blocklist.user = (void **)zone;
    zone->blocklist.tag = PU_STATIC;
    zone->rover = block;
	
    block->prev = block->next = &zone->blocklist;
    
    // NULL indicates a free block.
    block->user = NULL;	

    block->size = zone->size - sizeof(memzone_t);
}

void *I_ZoneBase( int *size )
{
	//enum
	//{
	int HEAP_SIZE = ::g->zmem * 1024 * 1024;		// SMF - was 10 * 1024 * 1024
	//};
	*size = HEAP_SIZE;
	return malloc( HEAP_SIZE );
}

//
// Z_Init
//
void Z_Init (void)
{
    memblock_t*	block;
    //int		size;

    ::g->mainzone = (memzone_t *)I_ZoneBase (&::g->zonesize);

	memset( ::g->mainzone, 0, ::g->zonesize);
	::g->mainzone->size = ::g->zonesize;

    // set the entire zone to one free block
    ::g->mainzone->blocklist.next =
	::g->mainzone->blocklist.prev =
	block = (memblock_t *)( (byte *)::g->mainzone + sizeof(memzone_t) );

    ::g->mainzone->blocklist.user = (void **)::g->mainzone;
    ::g->mainzone->blocklist.tag = PU_STATIC;
    ::g->mainzone->rover = block;
	
    block->prev = block->next = &::g->mainzone->blocklist;

    // NULL indicates a free block.
    block->user = NULL;
    
    block->size = ::g->mainzone->size - sizeof(memzone_t);
}

//int NumAlloc = 0;

//
// Z_Free
//
void Z_Free (void* ptr)
{
    memblock_t*		block;
    memblock_t*		other;

	block = (memblock_t *) ( (byte *)ptr - sizeof(memblock_t));
#ifdef RANGECHECK
	if (block->size > ::g->NumAlloc)
		I_Error("Z_Free: Trying to free %i on tag %d", block->size, block->tag);
#endif

	::g->NumAlloc -= block->size;

	if (block->tag == PU_CACHE ){
		::g->CacheAlloc -= block->size;
	}

    if (block->id != ZONEID)
	I_Error ("Z_Free: freed a pointer without ZONEID");
		
    if (block->user > (void **)0x100)
    {
	// smaller values are not pointers
	// Note: OS-dependend?
	
	// clear the user's mark
	*block->user = 0;
    }
	
    // mark as free
    block->user = NULL;	
    block->tag = 0;
    block->id = 0;

    other = block->prev;

    if (!other->user)
    {
	// merge with previous free block
	other->size += block->size;
	other->next = block->next;
	other->next->prev = other;

	if (block == ::g->mainzone->rover)
	    ::g->mainzone->rover = other;

	block = other;
    }
	
    other = block->next;
    if (!other->user)
    {
	// merge the next free block onto the end
	block->size += other->size;
	block->next = other->next;
	block->next->prev = block;

	if (other == ::g->mainzone->rover)
	    ::g->mainzone->rover = block;
    }
}


//GK: Expirimental z-memory reallocation (dont worry it is never in use)
/*void
Z_Realloc
(int newsize) {
	memblock_t*	block;
	//memzone_t* newzone;
	memblock_t* oldblock= (memblock_t *)malloc(::g->zonesize-sizeof(memzone_t));
	memset(oldblock, 0, (::g->zonesize- sizeof(memzone_t)));
	int ns = ::g->zonesize + newsize;
	memblock_t* rover = ::g->mainzone->rover->next;
	memblock_t* start = rover;
	int offset = 0;
	int preoffs = 0;
	int cs = sizeof(memblock_t);
	int ps = cs;
	do
	{
		memcpy(oldblock+offset, rover, cs);
		if (rover->user && rover->user != (void**)2) {
			void* data = *rover->user;
			*(void **)(oldblock + offset)->user = (void *)((byte *)(oldblock + offset) + sizeof(memblock_t));
			//memcpy(*(oldblock + offset)->user, data, rover->size+sizeof(memblock_t));
		}
		offset += cs;
		preoffs = offset - (cs);
		(oldblock + preoffs)->next = &(oldblock + offset)[0];
		int prevoffs = (preoffs - (ps));
		if (preoffs > 0) {
			(oldblock + preoffs)->prev = &(oldblock + prevoffs)[0];
		}
		//oldblock = oldblock->next;
		rover = rover->next;
		ps = cs;
		if (!rover->user || rover->user == (void**)2) {
			cs = sizeof(memblock_t);
		}
		else {
			cs = rover->size+sizeof(memblock_t);
		}
	} while (rover != start);
	oldblock->prev = &(oldblock + preoffs)[0];
	(oldblock + preoffs)->next = &oldblock[0];
	free(::g->mainzone);
	::g->mainzone =(memzone_t*)malloc(ns );
	//newzone = (memzone_t*)&(::g->mainzone + ::g->zonesize)[0];
	memset(::g->mainzone, 0, ns);

	::g->mainzone->blocklist.next =
		::g->mainzone->blocklist.prev =
		block = (memblock_t *)((byte *)::g->mainzone + sizeof(memzone_t));

	::g->mainzone->blocklist.user = (void **)::g->mainzone;
	::g->mainzone->blocklist.tag = PU_STATIC;
	::g->mainzone->rover = block;

	block->prev = block->next = &::g->mainzone->blocklist;

	// NULL indicates a free block.
	block->user = NULL;

	block->size = ::g->mainzone->size - sizeof(memzone_t);

	start = oldblock;
	offset = 0;
	preoffs = 0;
	cs= sizeof(memblock_t);
	ps = cs;
	do
	{
		memcpy(::g->mainzone->rover + offset, oldblock, cs);
		offset += cs;
		preoffs = offset - (cs);
		(::g->mainzone->rover + preoffs)->next = &(::g->mainzone->rover + offset)[0];
		int prevoffs = (preoffs - (ps));
		if (preoffs > 0) {
			(::g->mainzone->rover + preoffs)->prev = &(::g->mainzone->rover + prevoffs)[0];
		}
		//oldblock = oldblock->next;
		oldblock = oldblock->next;
		ps = cs;
		if (!oldblock->user || oldblock->user == (void**)2) {
			cs = sizeof(memblock_t);
		}
		else {
			cs = oldblock->size + sizeof(memblock_t);
		}
	} while (oldblock != start);
	::g->mainzone->rover->prev = &(::g->mainzone->rover + preoffs)[0];
	(::g->mainzone->rover + preoffs)->next = &::g->mainzone->rover[0];
	::g->mainzone->size = ns;
	//::g->mainzone->rover = ::g->mainzone->rover->prev;
	//::g->mainzone->rover = oldblock;
	::g->mainzone->rover = ::g->mainzone->rover->prev;
	::g->mainzone->rover->size += newsize;
	// set the entire zone to one free block
	/*newzone->blocklist.next =
		newzone->blocklist.prev =
		block = (memblock_t *)((byte *)newzone + sizeof(memzone_t));

	newzone->blocklist.user = (void **)::g->mainzone;
	newzone->blocklist.tag = PU_STATIC;
	newzone->rover = block;

	block->prev = block->next = &newzone->blocklist;

	// NULL indicates a free block.
	block->user = NULL;

	block->size = newzone->size - sizeof(memzone_t);
	::g->zonesize = ns;
}*/

//
// Z_Malloc
// You can pass a NULL user if the tag is < PU_PURGELEVEL.
//
#define MINFRAGMENT		64

void*
Z_Malloc
( unsigned	size,
  int		tag,
  void*		user )
{
	
    int		extra;
    memblock_t*	start;
    memblock_t* rover;
    memblock_t* newblock;
    memblock_t*	base;
#ifdef RANGECHECK
	if (size < 0 || size > (unsigned)::g->zonesize)
		I_Error("Z_Malloc: Trying to allocate %i on tag %d", size, tag);
#endif
		::g->NumAlloc += size;
		if (tag == PU_CACHE) {
			::g->CacheAlloc += size;
		}
    	
	size = (size + 3) & ~3;
    
    // scan through the block list,
    // looking for the first free block
    // of sufficient size,
    // throwing out any purgable blocks along the way.

    // account for size of block header
    size += sizeof(memblock_t);
    
    // if there is a free block behind the rover,
    //  back up over them
    base = ::g->mainzone->rover;
    
    if (!base->prev->user)
		base = base->prev;
	
    rover = base;
    start = base->prev;
	
    do
    {
		if (rover == start)
		{
			// scanned all the way around the list
			I_Error ("Z_Malloc: failed on allocation of %i bytes", size);
			/*Z_Realloc(size); //TODO: Find out how to make z-memory dynamiclly resizeable
			base = ::g->mainzone->rover;
			rover = base;
			//rover = rover->prev;
			continue;*/
		}
	
		if (rover->user)
		{
			if (rover->tag < PU_PURGELEVEL)
			{
				// hit a block that can't be purged,
				//  so move base past it
				base = rover = rover->next;
			}
			else
			{
				// free the rover block (adding the size to base)

				// the rover can be the base block
				base = base->prev;
				Z_Free ((byte *)rover+sizeof(memblock_t));
				base = base->next;
				rover = base->next;
			}
		}
		else
			rover = rover->next;
    } while (base->user || base->size < size);

    
    // found a block big enough
    extra = base->size - size;
    
    if (extra >  MINFRAGMENT)
    {
		// there will be a free fragment after the allocated block
		newblock = (memblock_t *) ((byte *)base + size );
		newblock->size = extra;
		
		// NULL indicates free block.
		newblock->user = NULL;	
		newblock->tag = 0;
		newblock->prev = base;
		newblock->next = base->next;
		newblock->next->prev = newblock;

		base->next = newblock;
		base->size = size;
    }
	
    if (user)
    {
		// mark as an in use block
		base->user = (void**)user;			
		*(void **)user = (void *) ((byte *)base + sizeof(memblock_t));
    }
    else
    {
		if (tag >= PU_PURGELEVEL)
			I_Error ("Z_Malloc: an owner is required for purgable blocks");

		// mark as in use, but unowned	
		base->user = (void **)2;		
    }
    base->tag = tag;

    // next allocation will start looking here
    ::g->mainzone->rover = base->next;	
	
    base->id = ZONEID;
    
    return (void *) ((byte *)base + sizeof(memblock_t));
}



//
// Z_FreeTags
//
void
Z_FreeTags
( int		lowtag,
  int		hightag )
{
    memblock_t*	block;
    memblock_t*	next;
	
    for (block = ::g->mainzone->blocklist.next ;
	 block != &::g->mainzone->blocklist ;
	 block = next)
    {
	// get link before freeing
	next = block->next;

	// free block?
	if (!block->user)
	    continue;

	if (block->size > ::g->NumAlloc) //GK: Don't allow to free blocks with iregular sizes
		continue;

	if (block->tag >= lowtag && block->tag <= hightag)
	    Z_Free ( (byte *)block+sizeof(memblock_t));
    }
}



//
// Z_DumpHeap
// Note: TFileDumpHeap( stdout ) ?
//
void
Z_DumpHeap
( int		lowtag,
  int		hightag )
{
    memblock_t*	block;
	
    I_Printf ("zone size: %i  location: %p\n",
	    ::g->mainzone->size,::g->mainzone);
    
    I_Printf ("tag range: %i to %i\n",
	    lowtag, hightag);
	
    for (block = ::g->mainzone->blocklist.next ; ; block = block->next)
    {
	if (block->tag >= lowtag && block->tag <= hightag)
	    I_Printf ("block:%p    size:%7i    user:%p    tag:%3i\n",
		    block, block->size, block->user, block->tag);
		
	if (block->next == &::g->mainzone->blocklist)
	{
	    // all blocks have been hit
	    break;
	}
	
	if ( (byte *)block + block->size != (byte *)block->next)
	    I_Printf ("ERROR: block size does not touch the next block\n");

	if ( block->next->prev != block)
	    I_Printf ("ERROR: next block doesn't have proper back link\n");

	if (!block->user && !block->next->user)
	    I_Printf ("ERROR: two consecutive free blocks\n");
    }
}


//
// Z_FileDumpHeap
//
void Z_FileDumpHeap (FILE* f)
{
    memblock_t*	block;
	
    fprintf (f,"zone size: %i  location: %p\n",::g->mainzone->size,::g->mainzone);
	
    for (block = ::g->mainzone->blocklist.next ; ; block = block->next)
    {
	fprintf (f,"block:%p    size:%7i    user:%p    tag:%3i\n",
		 block, block->size, block->user, block->tag);
		
	if (block->next == &::g->mainzone->blocklist)
	{
	    // all blocks have been hit
	    break;
	}
	
	if ( (byte *)block + block->size != (byte *)block->next)
	    fprintf (f,"ERROR: block size does not touch the next block\n");

	if ( block->next->prev != block)
	    fprintf (f,"ERROR: next block doesn't have proper back link\n");

	if (!block->user && !block->next->user)
	    fprintf (f,"ERROR: two consecutive free blocks\n");
    }
}



//
// Z_CheckHeap
//
void Z_CheckHeap (void)
{
    memblock_t*	block;
	
    for (block = ::g->mainzone->blocklist.next ; ; block = block->next)
    {
	if (block->next == &::g->mainzone->blocklist)
	{
	    // all blocks have been hit
	    break;
	}
	
	if ( (byte *)block + block->size != (byte *)block->next)
	    I_Error ("Z_CheckHeap: block size does not touch the next block\n");

	if ( block->next->prev != block)
	    I_Error ("Z_CheckHeap: next block doesn't have proper back link\n");

	if (!block->user && !block->next->user)
	    I_Error ("Z_CheckHeap: two consecutive free blocks\n");
    }
}




//
// Z_ChangeTag
//
void
Z_ChangeTag2
( void*		ptr,
  int		tag )
{
    memblock_t*	block;
	
    block = (memblock_t *) ( (byte *)ptr - sizeof(memblock_t));

    if (block->id != ZONEID)
	I_Error ("Z_ChangeTag: freed a pointer without ZONEID");

    if (tag >= PU_PURGELEVEL && (uintptr_t)block->user < 0x100)
	I_Error ("Z_ChangeTag: an owner is required for purgable blocks");

    block->tag = tag;
}

void Z_ChangeTag2( void** pp, int tag ) { Z_ChangeTag2( *pp, tag ); }


//
// Z_FreeMemory
//
int Z_FreeMemory (void)
{
    memblock_t*		block;
    int			free;
	
    free = 0;
    
    for (block = ::g->mainzone->blocklist.next ;
	 block != &::g->mainzone->blocklist;
	 block = block->next)
    {
		if (!block) {
			continue;
		}
	if (!block->user || block->tag >= PU_PURGELEVEL)
	    free += block->size;
    }
    return free;
}

/*
bool MallocForLump( int lump, unsigned int size, void **data, int tag )
{
	*data = Z_Malloc( size, tag, 0 );

	return true;
}
*/
