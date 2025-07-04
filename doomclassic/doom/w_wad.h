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

#ifndef __W_WAD__
#define __W_WAD__


#ifdef __GNUG__
#pragma interface
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <string>
//
//
// TYPES
//
typedef struct
{
    // Should be "IWAD" or "PWAD".
    char		identification[4];		
    int			numlumps;
    int			infotableofs;
    
} wadinfo_t;


typedef struct
{
    int			filepos;
    int			size;
    char		name[8];
    
} filelump_t;

//
// WADFILE I/O related stuff.
//
typedef struct
{
	//GK: Fix for "trash" chararcters 
	//p.s. : Don't try this on filelump_t
    char	name[9];
    idFile *	handle;
    int		position;
    long		size;
	//GK: Know when an item is null (malloc does not let nulls even if the entry has nothing)
	bool null;
} lumpinfo_t;

extern	void**		lumpcache;
extern	std::vector<void*>      directlumpcache;
extern	std::vector<lumpinfo_t>	lumpinfo;
extern	int		numlumps;

void    W_InitMultipleFiles (const char** filenames);
void    W_Reload (void);
void	W_FreeLumps();
void	W_FreeWadFiles();

int	W_CheckNumForName (const char* name, int last = -1);
int	W_GetNumForName (const char* name);
idList<int> W_GetNumsForName(const char* name);

int	W_LumpLength (int lump);
void    W_ReadLump (int lump, void *dest);

void*	W_CacheLumpNum (int lump, int tag);
void*	W_CacheLumpName (const char* name, int tag);
void* W_LoadLumpNum(int lump);
void* W_LoadLumpName(const char* name);

void	W_Shutdown();

void CleanUncompFiles(bool unalloc);

void MakeMaster_Wad();
void MasterExport();
void uncompressMaster();
void MasterList();
void W_CheckExp();
bool W_CheckMods(int sc,std::vector<std::string> filelist);
char* W_GetNameForNum(int lump);
int W_GetLumpCount();
bool W_IsGameWAD(const char* filename);
void W_LoadLumpFromFile(const char* filename, const char* lumpName);
#endif

