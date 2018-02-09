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

#ifdef __GNUG__
#pragma implementation "w_wad.h"
#endif
#include "w_wad.h"

#include "globaldata.h"

#include <stdio.h>
#include <string>
#include <ctype.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <vector>
#include <fstream>
#include <iostream>

#include "doomtype.h"
#include "m_swap.h"
#include "i_system.h"
#include "z_zone.h"

#include "idlib/precompiled.h"

#include "d_deh.h"

#include "libs\zlib\minizip\unzip.h"
#include "f_finale.h"
#include "g_game.h"

#define READ_SIZE 8192
#define MAX_FILENAME 512
//
// GLOBALS
//

lumpinfo_t*	lumpinfo = NULL;
int			numlumps;
void**		lumpcache;
std::vector<std::string> fname;
std::vector<std::string> foldername;
bool OpenCompFile(const char* filename);



int filelength (FILE* handle) 
{ 
	// DHM - not used :: development tool (loading single lump not in a WAD file)
	return 0;
}


void
ExtractFileBase
( const char*		path,
  char*		dest )
{
	const char*	src;
	int		length;

	src = path + strlen(path) - 1;

	// back up until a \ or the start
	while (src != path
		&& *(src-1) != '\\'
		&& *(src-1) != '/')
	{
		src--;
	}
    
	// copy up to eight characters
	memset (dest,0,8);
	length = 0;

	while (*src && *src != '.')
	{
		if (++length == 9)
			I_Error ("Filename base of %s >8 chars",path);

		*dest++ = toupper((int)*src++);
	}
}





//
// LUMP BASED ROUTINES.
//

//
// W_AddFile
// All files are optional, but at least one file must be
//  found (PWAD, if all required lumps are present).
// Files with a .wad extension are wadlink files
//  with multiple lumps.
// Other files are single lumps with the base filename
//  for the lump name.
//
// If filename starts with a tilde, the file is handled
//  specially to allow map reloads.
// But: the reload feature is a fragile hack...

const char*			reloadname;
//GK : is it an IWAD file? (first file to load)
bool iwad = false;
//Replace??
bool rep = false;
bool relp = false;
bool inzip = false;

void W_AddFile ( const char *filename)
{
    wadinfo_t		header;
    lumpinfo_t*		lump_p;
    int		i;
    idFile *		handle;
    int			length;
    int			startlump;
    std::vector<filelump_t>	fileinfo( 1 );

	::g->isbfg = true;
    
    // open the file and add to directory
    if ( (handle = fileSystem->OpenFileRead(filename)) == 0)
    {
		idLib::Printf(" couldn't open %s\n", filename);
		I_Printf (" couldn't open %s\n",filename);
		return;
    }
    I_Printf (" adding %s\n",filename);
	idLib::Printf(" adding %s\n", filename);
    startlump = numlumps;

	
    if ( idStr::Icmp( filename+strlen(filename)-3 , "wad" ) )
    {
		//GK: when loading archives return instandly don't add it to the lump list
			if (OpenCompFile(filename)) {
				//handle=nullptr;
				fileSystem->CloseFile(handle);
				return;
			}
			else {
				// single lump file
				fileinfo[0].filepos = 0;
				//GK: Allow to load "Wild" files
				char* fname = new char[256];
				if(relp){
					sprintf(fname, "./base%s", filename);
					
				}
				else {
					strcpy(fname, filename);
				}
				std::ifstream inf(fname, std::ifstream::ate | std::ifstream::binary);
				fileinfo[0].size = inf.tellg();
				ExtractFileBase(filename, fileinfo[0].name);
				numlumps++;
				relp = false;
				idLib::Printf("Added %s succesfully!\n", fname);
			}
    }
    else 
    {
		// WAD file
		handle->Read( &header, sizeof( header ) );
		if ( idStr::Cmpn( header.identification,"IWAD",4 ) )
		{
			// Homebrew levels?
			if ( idStr::Cmpn( header.identification, "PWAD", 4 ) )
			{
				I_Error ("Wad file %s doesn't have IWAD "
					"or PWAD id\n", filename);
			}
		    
			// ???modifiedgame = true;		
		}
		header.numlumps = LONG(header.numlumps);
		header.infotableofs = LONG(header.infotableofs);
		length = header.numlumps*sizeof(filelump_t);
		fileinfo.resize(header.numlumps);
		handle->Seek(  header.infotableofs, FS_SEEK_SET );
		handle->Read( &fileinfo[0], length );
		numlumps += header.numlumps;
    }

    
	// Fill in lumpinfo
	if (lumpinfo == NULL) {
		lumpinfo = (lumpinfo_t*)malloc( numlumps*sizeof(lumpinfo_t) );
	} else {
		lumpinfo = (lumpinfo_t*)realloc( lumpinfo, numlumps*sizeof(lumpinfo_t) );
	}

	if (!lumpinfo)
		I_Error ("Couldn't realloc lumpinfo");

	lump_p = &lumpinfo[startlump];

	::g->wadFileHandles[ ::g->numWadFiles++ ] = handle;
	int reppos = 0;
	int np = 0;
	int op = 0;
	//GK: Know where the "_END" marker is on iwad list in order to reduce the loop
	int ep = 0;
	filelump_t * filelumpPointer = &fileinfo[0];
	rep = false;
	for (i=startlump ; i<numlumps ; i++,lump_p++, filelumpPointer++)
	{
		//GK: replace lumps between "_START" and "_END" markers instead of append
		if (!iwad) {	
			 char check[6];
			 char marker[8];
			 char end [7];
			if (filelumpPointer->name[2] == '_') {
				strncpy(check, filelumpPointer->name+2, 6);
				if (filelumpPointer->name[1] != filelumpPointer->name[0]) {
					strncpy(marker, filelumpPointer->name, 7);
					strncpy(end,filelumpPointer->name,2);
					end[2] = '\0';
					strcat(end, "_END");
				}
				else {
					strncpy(marker, filelumpPointer->name + 1, 7);
					marker[7] = '\0';
					strncpy(end, filelumpPointer->name, 1);
					end[1] = '\0';
					strcat(end, "_END");
					end[5] = '\0';
				}
			}
			else if (filelumpPointer->name[1] == '_') {
				strncpy(check, filelumpPointer->name + 1, 6);
				strncpy(marker, filelumpPointer->name , 7);
				marker[7] = '\0';
				strncpy(end, filelumpPointer->name, 1);
				end[1] = '\0';
				strcat(end, "_END");
				end[5] = '\0';
			}
			
			//GK: Names with 8 characters might have "trash" use Cmpn instead of Icmp
			//ignore all the S,P and F markers (they causing troubles and making them doubles)
			if (!idStr::Cmpn(check, "_START",6)){
				np = i;
				op = W_CheckNumForName(marker);
				ep = W_CheckNumForName(end);
				rep = true;
				i++;
				if (i >= numlumps) {
					break;
				}
				filelumpPointer++;
				lump_p++;
				reppos = op;
				for (int g = 0; g < 6; g++) {
					check[g] = NULL;
				}
			}
			if (filelumpPointer->name[2] == '_') {
				strncpy(check, filelumpPointer->name + 2, 6);
			}
			else if (filelumpPointer->name[1] == '_') {
				strncpy(check, filelumpPointer->name + 1, 6);
			}
			if (!idStr::Cmpn(check, "_END",4)){
				rep = false;
				
			}
			
			bool smark = false;
			for (int u = 1; u < 4; u++) {
				char ps[10];
				sprintf(ps, "P%i_START", u);
				char pe[10];
				sprintf(pe, "P%i_END", u);
				char fs[10];
				sprintf(fs, "F%i_START", u);
				char fe[10];
				sprintf(fe, "F%i_END", u);
				if (!idStr::Cmpn(filelumpPointer->name, ps, 8) || !idStr::Cmpn(filelumpPointer->name, pe, 6) || !idStr::Cmpn(filelumpPointer->name, fs, 8) || !idStr::Cmpn(filelumpPointer->name, fe, 6)) {
					smark = true;
					for (int g = 0; g < 6; g++) {
						check[g] = NULL;
					}
				}
			}
			if (rep) {
				if (!smark) {


					bool replaced = false;
					lumpinfo_t* tlump = &lumpinfo[op];
					for (int j = op; j < ep; j++, tlump++) {
						if (!idStr::Cmpn(filelumpPointer->name, tlump->name, 8)) {
							//idLib::Printf("Replacing lump %s\n", filelumpPointer->name); //for debug purposes
							tlump->handle = handle;
							tlump->position = LONG(filelumpPointer->filepos);
							tlump->size = LONG(filelumpPointer->size);
							replaced = true;
							if (j > reppos) {
								reppos = j;
							}
							break;
							//lumpinfo_t* fl= (lumpinfo_t*)malloc(numlumps * sizeof(lumpinfo_t));
							//lumpinfo_t* sl= (lumpinfo_t*)malloc(numlumps * sizeof(lumpinfo_t));
						}
					}
					if (!replaced) {
						//GK:add aditional content in between the markers
						lumpinfo_t* temlump = &lumpinfo[reppos + 1];
						lumpinfo_t* tl = (lumpinfo_t*)malloc(numlumps * sizeof(lumpinfo_t));
						tl->handle = temlump->handle;
						strncpy(tl->name, temlump->name, 8);
						tl->position = temlump->position;
						tl->size = temlump->size;
						tl->null = temlump->null;
						lumpinfo_t* tl2 = (lumpinfo_t*)malloc(numlumps * sizeof(lumpinfo_t));
						temlump++;
						for (int k = reppos + 2; k < numlumps; k++, temlump++) {
							tl2->handle = temlump->handle;
							strncpy(tl2->name, temlump->name, 8);
							tl2->position = temlump->position;
							tl2->size = temlump->size;
							tl2->null = temlump->null;

							temlump->handle = tl->handle;
							strncpy(temlump->name, tl->name, 8);
							temlump->position = tl->position;
							temlump->size = tl->size;
							temlump->null = tl->null;
							if (tl2->null) {
								break;
							}

							tl->handle = tl2->handle;
							strncpy(tl->name, tl2->name, 8);
							tl->position = tl2->position;
							tl->size = tl2->size;
							tl->null = tl2->null;
						}
						free(tl);
						free(tl2);
						//idLib::Printf("adding lump between markers %s\n", filelumpPointer->name); //for debug purposes
						lumpinfo_t* lar = &lumpinfo[reppos + 1];
						/*lump_p*/lar->handle = handle;
						/*lump_p*/lar->position = LONG(filelumpPointer->filepos);
						/*lump_p*/lar->size = LONG(filelumpPointer->size);
						strncpy(/*lump_p*/lar->name, filelumpPointer->name, 8);
						lar->name[8] = '\0';
						lar->null = false;
						reppos++;
						ep++;
					}
				}
			}
			else {
				if (idStr::Cmpn(check, "_END",4) && !smark){
					//idLib::Printf("adding lump %s\n", filelumpPointer->name); //for debug purposes
					lump_p->handle = handle;
					lump_p->position = LONG(filelumpPointer->filepos);
					lump_p->size = LONG(filelumpPointer->size);
					strncpy(lump_p->name, filelumpPointer->name, 8);
					lump_p->name[8] = '\0';
					lump_p->null = false;
					//GK: Check if it is a .deh file
					if ((!idStr::Cmpn(filelumpPointer->name, "DEHACKED", 8)) || (!idStr::Icmp(filename + strlen(filename) - 3, "deh")) || (!idStr::Icmp(filename + strlen(filename) - 3, "bex"))) {
						//idLib::Printf("Adding DeHackeD file %s\n",filelumpPointer->name);
						loaddeh(i);
						
					}
				}
				else {
					for (int g = 0; g < 6; g++) {
						check[g] = NULL;
					}
				}
			}
			
		}
		else {
			lump_p->handle = handle;
			lump_p->position = LONG(filelumpPointer->filepos);
			lump_p->size = LONG(filelumpPointer->size);
			strncpy(lump_p->name, filelumpPointer->name, 8);
			lump_p->name[8] = '\0';
			lump_p->null = false;
		}
		//GK end
	}
	int point = W_GetNumForName("MAP33");
	if (point == -1) {
		if (::g->autostart) {
			if (::g->startmap > 32) {
				::g->startmap = 1;
			}
		}
		::g->isbfg = false;
	}
}




//
// W_Reload
// Flushes any of the reloadable lumps in memory
//  and reloads the directory.
//
void W_Reload (void)
{
	// DHM - unused development tool
}

//
// W_FreeLumps
// Frees all lump data
//
void W_FreeLumps() {
	if ( lumpcache != NULL ) {
		for ( int i = 0; i < numlumps; i++ ) {
			if ( lumpcache[i] ) {
				Z_Free( lumpcache[i] );
			}
		}

		Z_Free( lumpcache );
		lumpcache = NULL;
	}

	if ( lumpinfo != NULL ) {
		free( lumpinfo );
		lumpinfo = NULL;
		numlumps = 0;
	}
}

//
// W_FreeWadFiles
// Free this list of wad files so that a new list can be created
//
void W_FreeWadFiles() {
	for (int i = 0 ; i < MAXWADFILES ; i++) {
		wadfiles[i] = NULL;
		if ( ::g->wadFileHandles[i] ) {
			delete ::g->wadFileHandles[i];
		}
		::g->wadFileHandles[i] = NULL;
	}
	::g->numWadFiles = 0;
	extraWad = 0;
	//GK: Game crashing bugfix (still need work)
	if (::g->gamemode == commercial) {
		idLib::Printf("Reseting Dehacked Patches...\n");
		resetValues();
		resetWeapons();
		ResetAmmo();
		resetMapNames();
		resetEndings();
		resetTexts();
		resetSprnames();
		ResetPars();
		ResetFinalflat();
		idLib::Printf("Reset Completed!!\n");
	}
	//GK End
}



//
// W_InitMultipleFiles
// Pass a null terminated list of files to use.
// All files are optional, but at least one file
//  must be found.
// Files with a .wad extension are idlink files
//  with multiple lumps.
// Other files are single lumps with the base filename
//  for the lump name.
// Lump names can appear multiple times.
// The name searcher looks backwards, so a later file
//  does override all earlier ones.
//
void W_InitMultipleFiles (const char** filenames)
{
	int		size;

	if (lumpinfo == NULL)
	{
		// open all the files, load headers, and count lumps
		numlumps = 0;

		// will be realloced as lumps are added
		
		free(lumpinfo);
		lumpinfo = NULL;
		iwad = true;
		for ( ; *filenames ; filenames++)
		{
			W_AddFile (*filenames);
			iwad = false;
		}
		//iwad = true;
		if (!numlumps)
			I_Error ("W_InitMultipleFiles: no files found");

		// set up caching
		size = numlumps * sizeof(*lumpcache);
		lumpcache = (void**)DoomLib::Z_Malloc(size, PU_STATIC_SHARED, 0 );

		if (!lumpcache)
			I_Error ("Couldn't allocate lumpcache");

		memset (lumpcache,0, size);
	} else {
		// set up caching
		size = numlumps * sizeof(*lumpcache);
		lumpcache = (void**)DoomLib::Z_Malloc(size, PU_STATIC_SHARED, 0 );

		if (!lumpcache)
			I_Error ("Couldn't allocate lumpcache");

		memset (lumpcache,0, size);
	}
}


void W_Shutdown() {
/*
	for (int i = 0 ; i < MAXWADFILES ; i++) {
		if ( ::g->wadFileHandles[i] ) {
			doomFiles->FClose( ::g->wadFileHandles[i] );
		}
	}

	if ( lumpinfo != NULL ) {
		free( lumpinfo );
		lumpinfo = NULL;
	}
*/
	W_FreeLumps();
	W_FreeWadFiles();
}

//
// W_NumLumps
//
int W_NumLumps (void)
{
    return numlumps;
}



//
// W_CheckNumForName
// Returns -1 if name not found.
//

int W_CheckNumForName (const char* name)
{
	const int NameLength = 9;

    union {
		char	s[NameLength];
		int	x[2];
	
    } name8;
    
    int		v1;
    int		v2;
    lumpinfo_t*	lump_p;

    // make the name into two integers for easy compares
    strncpy (name8.s,name, NameLength - 1);

    // in case the name was a fill 8 chars
    name8.s[NameLength - 1] = 0;

    // case insensitive
	for ( int i = 0; i < NameLength; ++i ) {
		name8.s[i] = toupper( name8.s[i] );		
	}

    v1 = name8.x[0];
    v2 = name8.x[1];


    // scan backwards so patch lump files take precedence
    lump_p = lumpinfo + numlumps;

    while (lump_p-- != lumpinfo)
    {
		if ( *(int *)lump_p->name == v1
			&& *(int *)&lump_p->name[4] == v2)
		{
			return lump_p - lumpinfo;
		}
    }

    // TFB. Not found.
    return -1;
}




//
// W_GetNumForName
// Calls W_CheckNumForName, but bombs out if not found.
//
int W_GetNumForName ( const char* name)
{
    int	i;

    i = W_CheckNumForName ( name);
    //GK begin
	if (i == -1 && idStr::Icmp("TITLEPIC", name) && idStr::Icmp("HELP2", name) && idStr::Icmp("HELP01", name) && idStr::Icmp("HELP02", name) && idStr::Icmp("MAP33",name) && idStr::Icmp("CWILV32", name)) //TITLEPIC might not exist
      I_Error ("W_GetNumForName: %s not found!", name);
	//GK End
      
    return i;
}


//
// W_LumpLength
// Returns the buffer size needed to load the given lump.
//
int W_LumpLength (int lump)
{
    if (lump >= numlumps)
		I_Error ("W_LumpLength: %i >= numlumps",lump);

    return lumpinfo[lump].size;
}



//
// W_ReadLump
// Loads the lump into the given buffer,
//  which must be >= W_LumpLength().
//
void
W_ReadLump
( int		lump,
  void*		dest )
{
    int			c;
    lumpinfo_t*	l;
    idFile *		handle;
	
    if (lump >= numlumps)
		I_Error ("W_ReadLump: %i >= numlumps",lump);

    l = lumpinfo+lump;
	
	handle = l->handle;
	//idLib::Printf("Reading %s from %s\n", l->name, handle->GetName());
	//if (handle->GetName() != NULL && handle->GetName() != "" && handle->GetName() != " ") {
		handle->Seek(l->position, FS_SEEK_SET);
		
		c = handle->Read(dest, l->size);

		if (c < l->size)
			I_Error("W_ReadLump: only read %i of %i on lump %i", c, l->size, lump);
	//}
}




//
// W_CacheLumpNum
//
void*
W_CacheLumpNum
( int		lump,
  int		tag )
{
#ifdef RANGECHECK
	if (lump >= numlumps)
	I_Error ("W_CacheLumpNum: %i >= numlumps",lump);
#endif

	if (!lumpcache[lump])
	{
		byte*	ptr;
		// read the lump in
		//I_Printf ("cache miss on lump %i\n",lump);
		ptr = (byte*)DoomLib::Z_Malloc(W_LumpLength (lump), tag, &lumpcache[lump]);
		W_ReadLump (lump, lumpcache[lump]);
	}

	return lumpcache[lump];
}



//
// W_CacheLumpName
//
void*
W_CacheLumpName
( const char*		name,
  int		tag )
{
	//GK begin
	int point = W_GetNumForName(name);
	if (!idStr::Icmp("TITLEPIC", name) && point == -1) { //Handle no TITLEPIC lump from DOOM2.WAD
		point = W_GetNumForName("INTERPIC");
	}
	//GK end
    return W_CacheLumpNum (point, tag);
}


void W_Profile (void)
{
}

//GK: Open archive files extract it's content and load it as files for DOOM
bool OpenCompFile(const char* filename) {
	char* maindir = new char[MAX_FILENAME];
	char* senddir = new char[MAX_FILENAME];
	senddir = "/pwads/";
	char* fdir = new char[MAX_FILENAME];
	if (inzip) {
			sprintf(fdir, "%s%s", "base", filename);
	}
	else {
		strcpy(fdir, filename);
	}
	idLib::Printf("Checking %s for compressed file\n",fdir);
	unzFile zip = unzOpen(fdir);
	if (zip != NULL) {
		idLib::Printf("found compressed file\n");
#ifdef _WIN32
		CreateDirectory("base/pwads",NULL);
#elif
		mkdir("base/pwads");
#endif
		unz_global_info gi;
		if (unzGetGlobalInfo(zip, &gi) == UNZ_OK) {
			char rb[READ_SIZE];
			for (int i = 0; i < gi.number_entry; i++) {
				unz_file_info fi;
				char* name = new char[MAX_FILENAME];
				if (unzGetCurrentFileInfo(zip, &fi, name, MAX_FILENAME, NULL, 0, NULL, 0) == UNZ_OK) {
					//idLib::Printf("%s\n", name);
					const size_t filename_length = strlen(name);
					if (name[filename_length - 1] != '/') {
						if (unzOpenCurrentFile(zip) == UNZ_OK) {
							char* path = new char[MAX_FILENAME];
							sprintf(path, "%s%s", "base/pwads/", name);
							std::FILE *out = fopen(path, "wb");
							if (out != NULL) {
								int buff = UNZ_OK;
								do {
									buff = unzReadCurrentFile(zip, rb, READ_SIZE);
									if (buff > 0) {
										fwrite(rb, buff, 1, out);
									}
								} while (buff > 0);
								fclose(out);
								unzCloseCurrentFile(zip);
								fname.push_back(path);
								char* pname = new char[MAX_FILENAME];
								sprintf(pname, "%s%s",senddir, name);
								if (idStr::Icmp(name + strlen(name) - 3, "wad")) {
									relp = true;
								}
								inzip = true;
								W_AddFile(pname);

							}
							if (i + 1 < gi.number_entry) {
								unzGoToNextFile(zip);
							}
						}
					}
					else {
						//GK: If it found directory inside the archieve create it
						strcpy(maindir, "base/pwads/");
						//idLib::Printf("found directory\n");
						strcat(maindir, name);
						//idLib::Printf("%s\n", maindir);
							#ifdef _WIN32
													CreateDirectory(maindir, NULL);
							#elif
													mkdir(maindir);
							#endif
						foldername.push_back(maindir);
						if (i + 1 < gi.number_entry) {
							unzGoToNextFile(zip);
						}
					}

				}
			}

			unzClose(zip);
			inzip = false;
			return true;
		}
	}
		return false;
}
//GK: Delete uncompressed files.
//Problem: It must free them in order to delete them. And still might left some files back
void CleanUncompFiles(bool unalloc) {
	if (unalloc) {
		W_Shutdown();
	}
	for (int i = 0; i < fname.size(); i++) {
		idLib::Printf("Deleting File %s\n", fname[i].c_str());
		do {

		} while (remove(fname[i].c_str()) == 0);

	}
	for (int i = 0; i < foldername.size(); i++) {
		idLib::Printf("Deleting Directory %s\n", foldername[i].c_str());
#ifdef _WIN32
		RemoveDirectory(foldername[i].c_str());
#elif
		rmdir(foldername[i].c_str());
#endif

}
#ifdef _WIN32
	RemoveDirectory("base/pwads");
#elif
	rmdir("base/pwads");
#endif
		fname.clear();
		foldername.clear();
}
//GK: Check for either the wad a folder or a zip file that contains ALL the Master Levels
void MakeMaster_Wad() {
	struct stat info;
	if (FILE *f = fopen("base/wads/MASTERLEVELS.wad", "r")) {
		fclose(f);
		return;
	}
	if (FILE *f = fopen("base/wads/master.zip", "r")) {
		uncompressMaster();
		fclose(f);
	}
	if (stat("base/wads/master/", &info) != 0)
		return;
	else if (info.st_mode & S_IFDIR){
		idLib::Printf("Found Directry\n");
		MasterList();
		DoomLib::hexp[2] = true;
		return;
	}
}
//GK: In case of zip file
void uncompressMaster() {
	char* sdir = "base/wads/master.zip";
	char* ddir = "base/wads/master/";
	unzFile zip = unzOpen(sdir);
	if (zip != NULL) {
		idLib::Printf("found compressed file\n");
#ifdef _WIN32
		CreateDirectory(ddir, NULL);
#elif
		mkdir(ddir);
#endif
		unz_global_info gi;
		if (unzGetGlobalInfo(zip, &gi) == UNZ_OK) {
			char rb[READ_SIZE];
			for (int i = 0; i < gi.number_entry; i++) {
				unz_file_info fi;
				char* name = new char[MAX_FILENAME];
				if (unzGetCurrentFileInfo(zip, &fi, name, MAX_FILENAME, NULL, 0, NULL, 0) == UNZ_OK) {
					//idLib::Printf("%s\n", name);
					const size_t filename_length = strlen(name);
					if (name[filename_length - 1] != '/') {
						if (unzOpenCurrentFile(zip) == UNZ_OK) {
							char* path = new char[MAX_FILENAME];
							sprintf(path, "%s%s", ddir, name);
							std::FILE *out = fopen(path, "wb");
							if (out != NULL) {
								int buff = UNZ_OK;
								do {
									buff = unzReadCurrentFile(zip, rb, READ_SIZE);
									if (buff > 0) {
										fwrite(rb, buff, 1, out);
									}
								} while (buff > 0);
								fclose(out);
								unzCloseCurrentFile(zip);
								fname.push_back(path);
								char* pname = new char[MAX_FILENAME];
								sprintf(pname, "%s%s", ddir, name);

							}
							if (i + 1 < gi.number_entry) {
								unzGoToNextFile(zip);
							}
						}
					}

				}
			}

			unzClose(zip);
			inzip = false;
			//MasterList();
			return;
		}
	}
	return;
}
//GK:Open all the wads and copy and rename their contents into one WAD file
void MasterList() {
	
	wadinfo_t		header;
	lumpinfo_t*		lump ;
	lumpinfo_t*		lumpnfo = NULL;
	idFile *		handle;
	int			length;
	int			startlump;
	int nlps =0;
	int cl = 0;
	int remlmp = 0;
	bool pushit = true;
	std::vector<filelump_t>	fileinfo(1);
	int count = 1;
	int count2 = 1;
	char* filename = new char[MAX_FILENAME];
	char* dir = "base\\wads\\master\\*.wad";
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	std::vector <int> fofs;
	int offs=12;
	char* buffer =new char[512];
	std::ofstream of("base//wads//MASTERLEVELS.wad",std::ios::binary);
	of.write("PWAD",4);
	hFind = FindFirstFile(dir, &ffd);
	do {
		sprintf(filename, "wads/master/%s", masterlist[cl]);
		idLib::Printf("%s\n", filename);
		// open the file and add to directory
		if ((handle = fileSystem->OpenFileRead(filename)) == 0)
		{
			return;
		}
		startlump = nlps;


		if (idStr::Icmp(filename + strlen(filename) - 3, "wad"))
		{
		}
		else
		{
			// WAD file
			handle->Read(&header, sizeof(header));
			if (idStr::Cmpn(header.identification, "IWAD", 4))
			{
				// Homebrew levels?
				if (idStr::Cmpn(header.identification, "PWAD", 4))
				{
					I_Error("Wad file %s doesn't have IWAD "
						"or PWAD id\n", filename);
				}

				// ???modifiedgame = true;		
			}
			header.numlumps = LONG(header.numlumps);
			header.infotableofs = LONG(header.infotableofs);
			length = header.numlumps * sizeof(filelump_t);
			fileinfo.resize(header.numlumps);
			handle->Seek(header.infotableofs, FS_SEEK_SET);
			handle->Read(&fileinfo[0], length);
			nlps += header.numlumps;
			if (lumpnfo == NULL) {
				lumpnfo = (lumpinfo_t*)malloc(nlps * sizeof(lumpinfo_t));
			}
			else {
				lumpnfo = (lumpinfo_t*)realloc(lumpnfo, nlps * sizeof(lumpinfo_t));
			}

			if (!lumpnfo)
				I_Error("Couldn't realloc lumpinfo");

			lump = &lumpnfo[startlump];
			
			filelump_t * filelumpPointer = &fileinfo[0];
			rep = false;
			
			for (int i = startlump; i < nlps; i++, lump++, filelumpPointer++)
			{
				    lump->null = false;
					lump->handle = handle;
					lump->position = LONG(filelumpPointer->filepos);
					lump->size = LONG(filelumpPointer->size);
					if (pushit)
					    fofs.push_back(offs);

					if (idStr::Cmpn(filelumpPointer->name, "TEXTURE1", 8) && idStr::Cmpn(filelumpPointer->name, "PNAMES", 6) && idStr::Cmpn(filelumpPointer->name, "PP_START", 8) && idStr::Cmpn(filelumpPointer->name, "PP_END", 7))
					{
						if (count2 >= 4) 
						{
							if (idStr::Cmpn(filelumpPointer->name, "RSKY1", 5)) 
							{
								offs += lump->size;
								pushit = true;
							}
							else
							{
								lump->null = true;
								remlmp++;
								pushit = false;
							}
						}
						else if (count2 > 2) 
						{
							if (idStr::Cmpn(filelumpPointer->name, "STARS", 5)) 
							{
								offs += lump->size;
								pushit = true;
							}
							else
							{
								lump->null = true;
								remlmp++;
								pushit = false;
							}
						}
						else 
						{
							offs += lump->size;
							pushit = true;
						}
					}
					else
					{
						lump->null = true;
						remlmp++;
						pushit = false;
					}
					strncpy(lump->name, filelumpPointer->name, 8);
					lump->name[8] = '\0';
					if (!idStr::Cmpn(lump->name, "MAP", 3)) {
						char * tm = new char[6];
						if (count < 10) {
							sprintf(tm, "MAP0%i\0", count);
							strcpy(lump->name, tm);
						}
						else {
							sprintf(tm, "MAP%i\0", count);
							strcpy(lump->name, tm);
						}
						count++;
					}
					if (!idStr::Cmpn(lump->name, "RSKY", 4)) {
						if (count2 < 4) {
						char * tm = new char[6];
						sprintf(tm, "STAR%i\0", count2);
						strcpy(lump->name, tm);
						count2++;
					}
					}
					if (!idStr::Cmp(lump->name, "STARS")) {
						if (count2 == 2) {
							char * tm = new char[6];
							sprintf(tm, "STAR%i\0", count2);
							strcpy(lump->name, tm);
							count2++;
						}
					}
					
				
				}
			}
		cl++;
	} while (FindNextFile(hFind, &ffd) != 0);
	lump = &lumpnfo[0];
	nlps = nlps - remlmp;
	of.write(reinterpret_cast<char*>(&nlps), 4);
	of.write(reinterpret_cast<char*>(&offs), 4);
	//GK:The more the better
	buffer = new char[offs];
	try {
		for (int i = 0; i < nlps+remlmp; i++, lump++) {

			handle = lump->handle;
			handle->Seek(lump->position, FS_SEEK_SET);
			int c = handle->Read(buffer, lump->size);
			if (!lump->null) {
				if (idStr::Cmpn(buffer, "WARNING", 7)) {
					of.write(buffer, lump->size);
				}
			}
		}
	}
	catch (HANDLE e) {

	}

	lump = &lumpnfo[0];

	for (int i = 0; i < nlps; lump++) {
		if (!lump->null) {
			of.write(reinterpret_cast<char*>(&fofs[i]), 4);
			of.write(reinterpret_cast<char*>(&lump->size), 4);
			of.write(lump->name, 8);
			i++;
		}
	}
	lump = nullptr;
	lumpnfo = nullptr;
	delete[] buffer;
	of.flush();
	//GK: Nothing to see here (or close in that mater)
	return;
}

void W_CheckExp() {
	if (FILE *file = fopen("base/wads/TNT.WAD", "r")) {
		fclose(file);
		DoomLib::hexp[0] = true;
	}
	if (FILE *file = fopen("base/wads/PLUTONIA.WAD", "r")) {
		fclose(file);
		DoomLib::hexp[1] = true;
	}
	if (FILE *file = fopen("base/wads/MASTERLEVELS.WAD", "r")) {
		fclose(file);
		DoomLib::hexp[2] = true;
	}
}