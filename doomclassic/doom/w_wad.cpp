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
#include "d_exp.h"

#include "libs/zlib/minizip/unzip.h" //GK: Linux doesn't like the use of \ for file paths
#include "f_finale.h"
#include "g_game.h"
#include "p_setup.h"

#ifndef _WIN32
#include <dirent.h>
#include <unistd.h>
#endif

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
//GK: Keep information retrieved from .zip and .pk3 files
//inside those variables
std::vector<filelump_t> zipfileinfo (1);
std::vector<unz_file_pos> zippos(1);
int zipind = 0;
std::vector<std::string>wadsinzip;
//GK: End
bool OpenCompFile(const char* filename);
void W_RemoveLump(int lump);



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
	lumpinfo_t*		tlump;
	lumpinfo_t*		temlump;
	lumpinfo_t*		tl;
	lumpinfo_t*		lar;
	lumpinfo_t*		all_lump;
    int		i;
    idFile *		handle;
    int			length;
    int			startlump;
    std::vector<filelump_t>	fileinfo( 1 );
	char tn[9];

	::g->isbfg = true;
    
    // open the file and add to directory
    if ( (handle = fileSystem->OpenFileRead(filename)) == 0)
    {
		if (!idStr::Icmp(filename, "wads/DOOM.wad") || !idStr::Icmp(filename, "wads/DOOM2.wad") || !idStr::Icmp(filename, "wads/NERVE.wad") || !idStr::Icmp(filename, "wads/newopt.wad") || !idStr::Icmp(filename, "wads/ua.wad") || !idStr::Icmp(filename, "wads/mlbls.wad")) {
			common->FatalError("Doom Classic Error : Unable to load %s", filename);
		}
		I_Printf (" couldn't open %s\n",filename);
		return;
    }
    I_Printf (" adding %s\n",filename);
	
    startlump = numlumps;

	
    if ( idStr::Icmp( filename+strlen(filename)-3 , "wad" ) )
    {
		//GK: when loading archives return instandly don't add it to the lump list
#ifdef _WINDOWS
			if (OpenCompFile(filename)) {
#else //GK: Use the absolute path on linux because minizip might refuse to open it otherwise
			if (OpenCompFile(handle->GetFullPath())) {
#endif
				//handle=nullptr;
				//fileSystem->CloseFile(handle);
				//return;
				//GK: Give the list of files that has been found inside the .zip,.pk3 file
				//to the lumpinfo list
				fileinfo = zipfileinfo;
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
			//	idLib::Printf("Added %s succesfully!\n", fname);
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
		//GK:Init some values and don't let null value in chance
		all_lump = &lumpinfo[startlump];
		for (int i = startlump; i < numlumps;i++ , all_lump++) {
			strcpy(all_lump->name, "");
			all_lump->null = true;
		}
	} else {
		if (iwad) { //GK: Who knows better safe than sorry
			free(&lumpinfo);
			lumpinfo = NULL;
			lumpinfo = (lumpinfo_t*)malloc(numlumps*sizeof(lumpinfo_t));
			all_lump = &lumpinfo[startlump];
			for (int i = startlump; i < numlumps; i++, all_lump++) {
				strcpy(all_lump->name, "");
				all_lump->null = true;
			}
		}
		else {
			lumpinfo = (lumpinfo_t*)realloc(lumpinfo, numlumps * sizeof(lumpinfo_t));
			all_lump = &lumpinfo[startlump];
			for (int i = startlump; i < numlumps; i++, all_lump++) {
				strcpy(all_lump->name, "");
				all_lump->null = true;
			}
		}
	}

	if (!lumpinfo)
		I_Error ("Couldn't realloc lumpinfo");

	lump_p = &lumpinfo[startlump];

	::g->wadFileHandles[ ::g->numWadFiles++ ] = handle;
	int epos = 0;
	int reppos = 0;
	int op = 0;
	//GK: Know where the "_END" marker is on iwad list in order to reduce the loop
	int ep = 0;
	filelump_t * filelumpPointer = &fileinfo[0];
	rep = false;
	bool sprite = false;
	bool markfordelete = false; //GK:Mass murd-deletion flag
	for (i=startlump ; i<numlumps ; i++,lump_p++, filelumpPointer++)
	{
		//GK: replace lumps between "_START" and "_END" markers instead of append
		if (!iwad) {	
			 char marker[9];
			 char end [7];
			if (!idStr::Icmpn(filelumpPointer->name+2,"_START",6) || !idStr::Icmpn(filelumpPointer->name + 1, "_START", 6)) {
				switch (filelumpPointer->name[1]) {
				case '_':
					strncpy(marker, filelumpPointer->name, 7);
					break;
				default:
					if ( filelumpPointer->name[0] == filelumpPointer->name[1]) { //GK:In case of SS,FF or PP
						strncpy(marker, filelumpPointer->name + 1, 7);
					}
					else {
						continue;
					}
					break;
				}
					marker[7] = '\0';
					strncpy(end, filelumpPointer->name, 1);
					end[1] = '\0';
					strcat(end, "_END");
					end[5] = '\0';
				//}
			//	else {
					/*strncpy(marker, filelumpPointer->name,8); //GK:Ignore F1,P1 like markers
					marker[8] = '\0';
					strncpy(end, filelumpPointer->name, 2);
					end[2] = '\0';
					strcat(end, "_END");*/
				//	continue;
			//	}
				op = W_CheckNumForName(marker);
				if (op >= 0) {
					ep = W_CheckNumForName(end);
					if (!idStr::Icmp(marker, "S_START")) {
						sprite = true;
						
					}
						rep = true;
						reppos = op;
						continue;
					
				}
				else { //GK: New list?
					rep = false;
				}
			}
			//GK: TODO find a better way to handle sprite names
		/*	else if (!idStr::Icmpn(filelumpPointer->name + 1, "_START", 6)) {
				strncpy(marker, filelumpPointer->name , 7);
				marker[7] = '\0';
				strncpy(end, filelumpPointer->name, 1);
				end[1] = '\0';
				strcat(end, "_END");
				end[5] = '\0';
				op = W_CheckNumForName(marker);
				if (op >= 0) {
					ep = W_CheckNumForName(end);
					if (!idStr::Icmp(marker, "S_START")) {
						sprite = true;
					}
						rep = true;
						reppos = op;
						continue;
					
				}
				else { //GK: New list?
					rep = false;
				}
			}*/
			if (!idStr::Icmpn(filelumpPointer->name + 2, "_END", 4) || !idStr::Icmpn(filelumpPointer->name + 1, "_END", 4) && rep) {
				switch (filelumpPointer->name[1]) {
				case '_':
					rep = false;
					sprite = false;
					break;
				default:
					if (filelumpPointer->name[0] == filelumpPointer->name[1]) {
						rep = false;
						sprite = false;
					}
					break;
				}
				int n = i + 1; //GK: just in case stop the loop if the _END lump is the final
				if (n == numlumps) {
					break;
				}
				else {
					continue;
				}
			}
			/*else if (!idStr::Icmpn(filelumpPointer->name + 1, "_END", 4) && rep) {
				rep = false;
				sprite = false;
				int n = i + 1; //GK: just in case stop the loop if the _END lump is the final
				if (n == numlumps) {
					break;
				}
				else {
					continue;
				}
			}*/

			if (rep) {
					bool replaced = false;
					tlump = lumpinfo + ep;	
					for (int j = ep; j > op; j--, tlump--) {
						//GK: Lookup sprite animation frames in case of the modded one having scrambled frame name and rotation
						bool ok = false;
						if (!sprite) {
							if (!idStr::Icmpn(filelumpPointer->name, tlump->name, 8)) {
								//GK: find animation flats and delete their between frames
								for (int p = 0; !::g->animdefs[p].istexture ; p++) {
									if (!idStr::Icmpn(::g->animdefs[p].startname, tlump->name, 8)) {
										markfordelete = true;
										break;
									}
								}
								int k = 1;
								lumpinfo_t* ttlump = &lumpinfo[j + k];
									while (markfordelete) {
										I_Printf("Delete lump %s\n", ttlump->name);
										W_RemoveLump(j + (k+1));
										for (int p = 0; !::g->animdefs[p].istexture; p++) {
											if (!idStr::Icmpn(::g->animdefs[p].endname, ttlump->name, 8)) {
												markfordelete = false;
												break;
											}
										}
										
									}
								ok = true;
							}
						}
						else {
						/*	strcpy(tn, filelumpPointer->name);
							tn[8] = '\0';
							int diff = strlen(tlump->name) - strlen(tn);
							int score = 0;
							int len;
							int len2;
							bool zero = false;
							switch (diff) {
							case 0:
								len = strlen(tlump->name);
								len2 = strlen(tn);
								break;
							default:
								len = diff > 0 ? strlen(tn) : strlen(tlump->name);
								len2 = diff > 0 ? strlen(tlump->name) : strlen(tn);
							} 
							for (int i = 0; i < len; i++) {
								if (filelumpPointer->name[i]== tlump->name[i]) {
									score++;
								}
								else {
									break;
								}
							}
							if (score < len2) {
								if (score >= 4) {
									strcpy(tn, tlump->name);
									tn[5] = '0';
									tn[8] = '\0';
									if (!idStr::Icmpn(tn + 4, filelumpPointer->name + 4, 2)) {
										zero = true;
									}
									else {
										switch (diff) {
										case 0:
											switch (score) {
											case 5:
												if (len2 > 6 && tlump->name[5] != '0') {
													break;
												}
											case 6:
												strcpy(tlump->name, filelumpPointer->name);
												ok = true;
												break;
											default:
												if (len2 > 6) {
													if (!idStr::Icmpn(filelumpPointer->name + 4, tlump->name + 6, 2) && !idStr::Icmpn(filelumpPointer->name + 6, tlump->name + 4, 2)) {
														strcpy(tlump->name, filelumpPointer->name);
														ok = true;
													}
												}
												break;
											}
											break;
										default:
												switch (score) {
												case 6:
													strcpy(tlump->name, filelumpPointer->name);
													ok = true;
													break;
												}
											break;
										}
									}
									if (zero) {
										strcpy(tlump->name, filelumpPointer->name);
										ok = true;
										int k = 1;
										//GK: Since the replacement loop starts from the end
										//then look previous entries for frame rotations
										lumpinfo_t* ttlump = &lumpinfo[j - k];
										while (!idStr::Icmpn(ttlump->name, filelumpPointer->name, 5)) {
											W_RemoveLump(j - (k - 1));
											k++;
											ttlump = &lumpinfo[j - k];
											//tpos++;
										}
									}
								}
							}
							else {
								ok = true;
							}*/

							if (!idStr::Icmpn(filelumpPointer->name, tlump->name, 4)) {
								if (!idStr::Icmpn(filelumpPointer->name + 4, tlump->name + 4, 2)) {
									if (strlen(filelumpPointer->name) > 6 && strlen(tlump->name) > 6) {
										if (!idStr::Icmpn(filelumpPointer->name + 6, tlump->name + 6, 2)) {
											ok = true;
										}
										else {//GK:Silly me forget some if case senarios
											strcpy(tlump->name, filelumpPointer->name);
											ok = true;
										}
									}
									else {
										if (strlen(tlump->name) > 6) {
											strcpy(tlump->name, filelumpPointer->name);
										}
										ok = true;
									}
								}
								else {
										if (strlen(filelumpPointer->name) > 6 && strlen(tlump->name) > 6) {
											if (!idStr::Icmpn(filelumpPointer->name + 4, tlump->name + 6, 2)) {
												if (!idStr::Icmpn(filelumpPointer->name + 6, tlump->name + 4, 2)) {
													ok = true;
													strcpy(tlump->name, filelumpPointer->name);
												}
											}
										}
										else if (strlen(filelumpPointer->name) <= 6 && strlen(tlump->name) <= 6) {//GK:Silly me forget some if case senarios
											int rot = atoi(filelumpPointer->name + 5);
											if (rot < 1) {
												
													strcpy(tn, tlump->name);
													tn[5] = '0';
													if (!idStr::Icmpn(tn + 4, filelumpPointer->name + 4, 2)) {//GK:Silly me forget some if case senarios
														ok = true;
														strcpy(tlump->name, filelumpPointer->name);
														//GK: Since the replacement loop starts from the end
														//then look previous entries for frame rotations
														int k = 1;
														lumpinfo_t* ttlump = &lumpinfo[j - k];
														while (!idStr::Icmpn(ttlump->name, filelumpPointer->name, 5)) {
															W_RemoveLump(j- (k-1));
															k++;
															ttlump = &lumpinfo[j - k];
															//tpos++;
														}
													}
												
											}
											else {
										//		char tn[8];
												strcpy(tn, filelumpPointer->name);
												tn[5] = '0';
												if (!idStr::Icmpn(tn + 4, tlump->name + 4, 2)) {
													strcpy(tlump->name, filelumpPointer->name);
													ok = true;
												}
											}
										}
									else {//GK:Silly me forget some if case senarios
									//	char tn[9];
										strcpy(tn, tlump->name);
										tn[5] = '0';
										tn[8] = '\0';
										if (!idStr::Icmpn(tn + 4, filelumpPointer->name + 4, 2)) {
											strcpy(tlump->name, filelumpPointer->name);
											ok = true;
											int k = 1;
											//GK: Since the replacement loop starts from the end
											//then look previous entries for frame rotations
											lumpinfo_t* ttlump = &lumpinfo[j-k];
											while (!idStr::Icmpn(ttlump->name, filelumpPointer->name, 5)) {
												W_RemoveLump(j-(k-1));
												k++;
												ttlump = &lumpinfo[j - k];
												//tpos++;
											}
										}
									}
								}
							}
								
						}
						if (ok) {
							//idLib::Printf("Replacing lump %s\n", filelumpPointer->name); //for debug purposes
							tlump->handle = handle;
							tlump->position = LONG(filelumpPointer->filepos);
							tlump->size = LONG(filelumpPointer->size);
							replaced = true;
							//if (j > reppos) { //GK: What was the purpose of this?
								reppos = j;
							//}
							break;
						}
					}
					if (!replaced) {
						//GK:add aditional content in between the markers
						temlump = &lumpinfo[reppos + 1];
						epos = 0;
						for (int k = reppos + 1; k < numlumps; k++, temlump++) {
							if (temlump->null) {
								epos = k;
								break;
							}
						}
						if (epos > 0) {
							tl = &lumpinfo[epos - 1];
							//GK:Actually make right shift of the lumpinfo array
							for (int k = epos; k > reppos + 1; --k, --temlump,--tl) {
								temlump->handle = tl->handle;
								temlump->position = tl->position;
								temlump->size = tl->size;
								strncpy(temlump->name, tl->name, 8);
								temlump->null = tl->null;
							}
							//idLib::Printf("adding lump between markers %s\n", filelumpPointer->name); //for debug purposes
							lar = &lumpinfo[reppos + 1];
							lar->handle = handle;
							lar->position = LONG(filelumpPointer->filepos);
							lar->size = LONG(filelumpPointer->size);
							strncpy(lar->name, filelumpPointer->name, 8);
							lar->name[8] = '\0';
							lar->null = false;
							reppos++;
							ep++;
						}
					}
			}
			else {
					//idLib::Printf("adding lump %s\n", filelumpPointer->name); //for debug purposes
					lump_p->handle = handle;
					lump_p->position = LONG(filelumpPointer->filepos);
					lump_p->size = LONG(filelumpPointer->size);
					strncpy(lump_p->name, filelumpPointer->name, 8);
					lump_p->name[8] = '\0';
					lump_p->null = false;
					//GK: Check for REVERBD lump and activate reverb check ups
#ifdef USE_OPENAL
					if (!idStr::Cmpn(filelumpPointer->name, "REVERBD", 7)) {
						::g->hasreverb = true;
					}
#endif
					//GK: Check if it is a .deh file
					if ((!idStr::Cmpn(filelumpPointer->name, "DEHACKED", 8)) || (!idStr::Icmp(filename + strlen(filename) - 3, "deh")) || (!idStr::Icmp(filename + strlen(filename) - 3, "bex"))) {
						//idLib::Printf("Adding DeHackeD file %s\n",filelumpPointer->name);
						loaddeh(i);
						
					}
					//if (::g->gamemode == commercial) {
						//GK: if you find either MAPINFO lump of EXPINFO lump change to custom expansion and set it's data (from these two lumps)
						if (!idStr::Cmpn(filelumpPointer->name, "EXPINFO", 7) || !idStr::Cmpn(filelumpPointer->name, "MAPINFO", 7) || (!idStr::Icmp(filename + strlen(filename) - 3, "dlc"))) {
							::g->gamemission = pack_custom;
							EX_add(i);
						}
					//}
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
	//GK: Clear the file position array that we retrive from multiple 
	//.zip,.pk3 files
	if (zippos.size() > 1) {
		zipind = 0;
		zippos.clear();
		zippos.resize(1);
	}
	//GK: End
	//GK: Game crashing bugfix (still need work)
	::g->cpind = 0;
	if (::g->gamemode == commercial) {
		I_Printf("Reseting Dehacked Patches...\n");
		for (int i = 0; i < ::g->cpatch.size(); i++) {
			free(::g->cpatch[i]);
			::g->cpatch[i] = NULL;
		}
		resetValues();
		resetWeapons();
		ResetAmmo();
		resetMapNames();
		resetEndings();
		resetTexts();
		resetSprnames();
		ResetPars();
		ResetFinalflat();
		P_ResetAct();
		I_Printf("Reset Completed!!\n");
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
			//GK: It doesn't like doing recursive calls
			// and then adding the file that make them.
			//So we load the founded .wad files later
			for (int i = 0; i < wadsinzip.size(); i++) {
				inzip = true;
				W_AddFile(wadsinzip[i].c_str());
				iwad = false;
			}
			if (wadsinzip.size() > 0) {
				wadsinzip.clear();
			}
			inzip = false;
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
	if (!name) //GK: SANITY CHECK
		return -1;
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
	if (i == -1 && idStr::Icmp("TITLEPIC", name) && idStr::Icmp("HELP2", name) && idStr::Icmp("HELP01", name) && idStr::Icmp("HELP02", name) && idStr::Icmp("MAP33",name) && idStr::Icmp("CWILV32", name) && idStr::Icmp("SWITCHES", name)) //TITLEPIC might not exist
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
	//GK: Check if the "wild" file is a zip file
	unzFile zip = NULL;
	if (idStr::Icmp(handle->GetName() + strlen(handle->GetName()) - 3, "wad")) {
		zip = unzOpen(handle->GetFullPath());
	}
	//GK: End
	if (zip == NULL) {
		int r = handle->Seek(l->position, FS_SEEK_SET);
		//GK: Additional checkups has never been bad
		if (r == -1) {
			common->FatalError("W_ReadLump: Failed to find %s on %s", l->name, l->handle->GetName());
		}
		c = handle->Read(dest, l->size);
	}
	//GK: Open and load to the returning buffer the desired file
	//from the zip file
	else {
		unzGoToFilePos(zip, &zippos[l->position]); //GK: This is where zippos is saving the day
		if (unzOpenCurrentFile(zip) == UNZ_OK) {
			c = unzReadCurrentFile(zip, dest, l->size);
			unzCloseCurrentFile(zip);
		}
		else {
			c = 0;
		}
		unzClose(zip);
	}
	//GK: End

		if (c < l->size)
			I_Error("W_ReadLump: only read %i of %i on lump %i", c, l->size, lump);
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

	if (!idStr::Icmp("SWITCHES", name) && point == -1) { //Handle no SWITCHES lump
		return NULL;
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
	zipfileinfo.clear();
	zipfileinfo.resize(1);
	if (inzip) {
			sprintf(fdir, "%s%s", "base", filename);
	}
	else {
		strcpy(fdir, filename);
	}
	//idLib::Printf("Checking %s for compressed file\n",fdir);
	unzFile zip = unzOpen(fdir);
	if (zip != NULL) {
		//idLib::Printf("found compressed file\n");
#ifdef _WIN32
		CreateDirectory("base/pwads",NULL);
#else
		mkdir("base/pwads/", S_IRWXU);
#endif
		unz_global_info gi;
		if (unzGetGlobalInfo(zip, &gi) == UNZ_OK) {
			char rb[READ_SIZE];
			qboolean usesprites = false;
			qboolean usegraphic = false; //GK: TODO Find out how to translate GRAPHICS folder to FLAT or PLANE flag
			int indoffset = zipind;
			zipind += gi.number_entry;
			zipfileinfo.resize(gi.number_entry+10); //GK:Give a little bit more for flags
			int k = 0; //GK: Use internal counter for stability
			zippos.resize(zipind+10);
			numlumps += gi.number_entry;
			for (int i = 0; i < gi.number_entry; i++) {
				unz_file_info fi;
				char* name = new char[MAX_FILENAME];
				if (unzGetCurrentFileInfo(zip, &fi, name, MAX_FILENAME, NULL, 0, NULL, 0) == UNZ_OK) {
					//idLib::Printf("%s\n", name);
					const size_t filename_length = strlen(name);
					if (name[filename_length - 1] != '/') {
						if (!idStr::Icmp(name+filename_length - 3, "wad")) {
							if (unzOpenCurrentFile(zip) == UNZ_OK) {
								zipfileinfo.resize(zipfileinfo.size() - 1);
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
									fname.emplace_back(path);
									char* pname = new char[MAX_FILENAME];
									sprintf(pname, "%s%s", senddir, name);
									if (idStr::Icmp(name + strlen(name) - 3, "wad")) {
										relp = true;
									}
									inzip = true;
									//W_AddFile(pname);
									wadsinzip.emplace_back(pname);//GK: Just store the file path for now

								}
								if (i + 1 < gi.number_entry) {
									unzGoToNextFile(zip);
								}
							}
						}
						//GK: No wad file DONT extract
						else {
							//GK: Trim the filename from the path
							char* t = strtok(name, "/");
							char* tn = new char[512];
							qboolean insprite = false;
							while (t != NULL) {
								if (!usesprites && !idStr::Icmp(t, "SPRITES")) { //GK: Set S_START Flag if we entered a SPRITES folder
									strcpy(zipfileinfo[k].name, "S_START\0");
									zipfileinfo[k].filepos = k;
									insprite = true;
									usesprites = true;
									k++;
								}
								if (!insprite && !idStr::Icmp(t, "SPRITES")) {
									insprite = true;
								}
								tn = t;
								t = strtok(NULL, "/");
							}
							if (usesprites && !insprite) { //GK: And set a S_END flag once we exit the folder
								strcpy(zipfileinfo[k].name, "S_END\0");
								zipfileinfo[k].filepos = k;
								usesprites = false;
								k++;
							}
							//GK: And also keep the file extension
							char* fex = new char[5];
							t = strtok(tn, ".");
							while (t != NULL) {
								fex = t;
								t = strtok(NULL, ".");
								
							}
							//GK: Give fake name based on the file extension
							if (!idStr::Icmp(fex, "deh")) {
								strcpy(zipfileinfo[k].name, "DEHACKED");
							}
							else if (!idStr::Icmp(fex, "dlc")) {
								strcpy(zipfileinfo[k].name, "EXPINFO\0");
							}
							else {
								strcpy(zipfileinfo[k].name, tn);
								for (int j = 7; j >= 0; j--) {
									if (zipfileinfo[k].name[j] == '.') {
										zipfileinfo[k].name[j] = '\0';
										break;
									}
								}
								//GK: Make sure the name is always upper case
								for (int j = 0; j < 8; j++) {
									zipfileinfo[k].name[j] = toupper(zipfileinfo[k].name[j]);
								}
							}
							int j = indoffset + k;
							unzGetFilePos(zip, &zippos[j]);//GK: this is important DONT MESS WITH IT
							zipfileinfo[k].filepos = j;//GK: Keep the zippos position here
							zipfileinfo[k].size = fi.uncompressed_size;
							k++;
							if (i + 1 < gi.number_entry) {
								unzGoToNextFile(zip);
							}
						}
					}
					//GK: End
					else {
						//GK: If it found directory inside the archieve create it
						strcpy(maindir, "base/pwads/");
						//idLib::Printf("found directory\n");
						strcat(maindir, name);
						if (!idStr::Icmp(name, "SPRITES/")) {
							strcpy(zipfileinfo[k].name, "S_START\0");
							zipfileinfo[k].filepos = k;
							usesprites = true;
							k++;
						}
						//idLib::Printf("%s\n", maindir);
							#ifdef _WIN32
													CreateDirectory(maindir, NULL);
							#else
													mkdir(maindir, S_IRWXU);
							#endif
						foldername.emplace_back(maindir);
						if (i + 1 < gi.number_entry) {
							unzGoToNextFile(zip);
						}
					}

				}
			}
			if (usesprites) {
				strcpy(zipfileinfo[k].name, "S_END\0");
				zipfileinfo[k].filepos = k;
				usesprites = false;
			}
			while(1) { //GK: Remove unused entries
				if (zipfileinfo.size() == 1) {
					break;
				}
				int y = zipfileinfo.size() - 1;
				if (!idStr::Icmp(zipfileinfo[y].name, "")) {
					zipfileinfo.pop_back();
				}
				else {
					break;
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
		//idLib::Printf("Deleting File %s\n", fname[i].c_str());
		do {

		} while (remove(fname[i].c_str()) == 0);

	}
	for (int i = 0; i < foldername.size(); i++) {
		//idLib::Printf("Deleting Directory %s\n", foldername[i].c_str());
#ifdef _WIN32
		RemoveDirectory(foldername[i].c_str());
#else
		rmdir(foldername[i].c_str());
#endif

}
#ifdef _WIN32
	RemoveDirectory("base/pwads");
#else
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
		DoomLib::hexp[2] = true;
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
		DoomLib::hexp[2] = true;
		MasterList();
		
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
#else
		mkdir(ddir, S_IRWXU);
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
								fname.emplace_back(path);
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
	lumpinfo_t*		lump;
	lumpinfo_t*		lumpnfo = NULL;
	idFile *		handle;
	int			length;
	int			startlump;
	int nlps = 0;
	int cl = 0;
	int remlmp = 0;
	bool pushit = true;
	std::vector<filelump_t>	fileinfo(1);
	int count = 1;
	int count2 = 1;
	char* filename = new char[MAX_FILENAME];
#ifdef _WIN32
	char* dir = "base\\wads\\master\\*.wad";
#else
	char* dir = "base/wads/master/"; //GK: Otherwise it will crash once it will try to open them
#endif
#ifdef _WIN32
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
#else
	DIR           *dirp;
	struct dirent *directory;
#endif
	std::vector <int> fofs;
	int offs = 12;
	char* buffer = new char[512];
	std::ofstream of("base//wads//MASTERLEVELS.wad", std::ios::binary);
	of.write("PWAD", 4);
#ifdef _WIN32
	hFind = FindFirstFile(dir, &ffd);
#else
	dirp = opendir(dir);
#endif
	do {
		sprintf(filename, "wads/master/%s", masterlist[cl]);
		idLib::Printf("%s\n", filename);
		// open the file and add to directory
		if ((handle = fileSystem->OpenFileRead(filename)) == 0)
		{
			idLib::Printf("Doom Classic Error : Master Levels generation failed\n");
			of.close();
			DoomLib::hexp[2] = false;
			remove("base//wads//MASTERLEVELS.wad");
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
					fofs.emplace_back(offs);

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
						sprintf(tm, "MAP0%i", count);
						tm[5] = '\0';
						strcpy(lump->name, tm);
					}
					else {
						sprintf(tm, "MAP%i", count);
						tm[5] = '\0';
						strcpy(lump->name, tm);
					}
					count++;
				}
				if (!idStr::Cmpn(lump->name, "RSKY", 4)) {
					if (count2 < 4) {
						char * tm = new char[6];
						sprintf(tm, "STAR%i", count2);
						tm[5] = '\0';
						strcpy(lump->name, tm);
						count2++;
					}
				}
				if (!idStr::Cmp(lump->name, "STARS")) {
					if (count2 == 2) {
						char * tm = new char[6];
						sprintf(tm, "STAR%i", count2);
						tm[5] = '\0';
						strcpy(lump->name, tm);
						count2++;
					}
				}


			}
		}
		cl++;
#ifdef _WIN32
	} while (FindNextFile(hFind, &ffd) != 0);
#else
	}while ((directory = readdir(dirp)) != NULL && cl<20); //GK: Make sure there is no leak
	closedir(dirp);
#endif
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
	catch (int e) {

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

//GK:Open all the wads and copy and rename their contents into one WAD file
void MasterExport() {

	wadinfo_t		header;
	lumpinfo_t*		lump;
	lumpinfo_t*		lumpnfo = NULL;
	idFile *		handle;
	int			length;
	int			startlump;
	int nlps = 0;
	int cl = 0;
	int remlmp = 0;
	bool pushit = true;
	std::vector<filelump_t>	fileinfo(1);
	int count = 1;
	int count2 = 1;
	char* filename = new char[MAX_FILENAME];
	char* file[3];
	file[0] = "wads/MASTERLEVELS.wad"; //GK: Otherwise it will crash once it will try to open them
	file[1] = "wads/mlbls.wad";
	file[2] = "wads/ua.wad";
#ifdef _WIN32
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
#else
	DIR           *dirp;
	struct dirent *directory;
#endif
	std::vector <int> fofs;
	int offs = 12;
	char* buffer = new char[512];
	std::ofstream of("base//wads//MASTERLEVELZ.wad", std::ios::binary);
	of.write("PWAD", 4);
	for (int i = 0; i < 3; i++) {
		sprintf(filename, "%s", file[i]);
		idLib::Printf("%s\n", filename);
		// open the file and add to directory
		if ((handle = fileSystem->OpenFileRead(filename)) == 0)
		{
			idLib::Printf("Doom Classic Error : Master Levels generation failed\n");
			of.close();
			//DoomLib::hexp[2] = false;
			remove("base//wads//MASTERLEVELZ.wad");
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

			for (int i = startlump; i < nlps; i++, filelumpPointer++)
			{
				if (!idStr::Icmpn(filelumpPointer->name, "M_EPISOD",8) || !idStr::Icmp(filelumpPointer->name, "M_EPI1") || !idStr::Icmp(filelumpPointer->name, "M_EPI2") || !idStr::Icmp(filelumpPointer->name, "M_EPI3") || !idStr::Icmp(filelumpPointer->name, "M_EPI4")) {
					remlmp++;
					continue;
				}
				lump->null = false;
				lump->handle = handle;
				lump->position = LONG(filelumpPointer->filepos);
				lump->size = LONG(filelumpPointer->size);
				strncpy(lump->name, filelumpPointer->name, 8);
				lump->name[8] = '\0';
				fofs.emplace_back(offs);
				if (!idStr::Cmpn(lump->name, "MAP", 3)) {
					lump->name[1] = 'S';
					lump->name[2] = 'L';
				}
				if (!idStr::Cmpn(lump->name, "CWILV", 5)) {
					lump->name[0] = 'M';
				}
				if (!idStr::Cmpn(lump->name, "INTERPIC", 8)) {
					lump->name[0] = 'M';
					lump->name[1] = 'S';
				}
				if (!idStr::Cmpn(lump->name, "DNL", 3)) {
#ifdef WIN32
					sprintf(lump->name, "MAPINFO",7*sizeof(char));
#else
					sprintf(lump->name,"MAPINFO");
#endif
				}
				offs += lump->size;
				lump++;

			}
		}
	}
lump = &lumpnfo[0];
nlps = nlps - remlmp;
of.write(reinterpret_cast<char*>(&nlps), 4);
of.write(reinterpret_cast<char*>(&offs), 4);
//GK:The more the better
buffer = new char[offs];
try {
	for (int i = 0; i < nlps ; i++, lump++) {

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
catch (int e) {

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
	if (FILE *file = fopen("base/wads/MASTERLEVELS.wad", "r")) {
		fclose(file);
		DoomLib::hexp[2] = true;
	}
}
//GK:Do here the modded save check
bool W_CheckMods(int sc, std::vector<std::string> filelist) {
	bool ok = false;
	int ac = 0;
	bool movetonext;
	if (sc > 0) {
		//GK: No more trash talking
		for (int mf = 0; mf < filelist.size(); mf++) {
			int f = 1;
			while (wadfiles[f] != NULL) {
				movetonext = false;

				char* fname = strtok(strdup(wadfiles[f]), "\\");
				if (DoomLib::idealExpansion == DoomLib::expansionSelected) { //GK:No longer using ::g->gamemission here since the custom expansion addition might cause issues
					if (idStr::Icmpn(fname, "wads", 4)) {
						while (fname) {
							char* tname = strtok(NULL, "\\");
							if (tname) {
								fname = tname;
							}
							else {
								break;
							}
						}


						if (!idStr::Icmp(filelist[mf].c_str(), fname)) {
							ac++;
							if (ac == sc) {
								ok = true;
							}
							break;
						}

					}
					else {
						f++;
						continue;
					}
				}
				else {
					int o = 0;
					int n = 0;
					while (DoomLib::generalfiles[n] != NULL) { //GK:Check for global mods
						if (!idStr::Icmp(filelist[mf].c_str(), DoomLib::generalfiles[n])) {
							ac++;
							movetonext = true;
							if (ac == sc)
								ok = true;

							break;
						}
						n++;
					}
					while (DoomLib::otherfiles[DoomLib::idealExpansion - 1][o] != NULL) {
						if (!idStr::Icmp(filelist[mf].c_str(), DoomLib::otherfiles[DoomLib::idealExpansion - 1][o])) {
							ac++;
							movetonext = true;
							if (ac == sc)
								ok = true;

							break;
						}
						o++;
					}
					if (movetonext)
						break;
				}
				f++;
			}
			if (ok)
				break;
		}
	}
	else {
		ok = true;
	}
	return ok;
}

//GK:delete unwanted content in between the markers
void W_RemoveLump(int lump) {
	lumpinfo_t* temlump ,*tl;
	int epos;

	
	temlump = &lumpinfo[lump + 1];
	epos = 0;
	for (int k = lump + 1; k < numlumps; k++, temlump++) {
		if (temlump->null) {
			epos = k;
			break;
		}
	}
	if (epos > 0) {
		//GK: Start from the exact previous file
		temlump = &lumpinfo[lump - 1];
		tl = &lumpinfo[lump];
		//GK:Actually make left shift of the lumpinfo array
		for (int k = lump - 1; k <= epos; k++, temlump++, tl++) {
			temlump->handle = tl->handle;
			temlump->position = tl->position;
			temlump->size = tl->size;
			strncpy(temlump->name, tl->name, 8);
			temlump->null = tl->null;
		}
	}
}

char* W_GetNameForNum(int lump) {
	lumpinfo_t* l = lumpinfo + lump;
	return l->name;
}
