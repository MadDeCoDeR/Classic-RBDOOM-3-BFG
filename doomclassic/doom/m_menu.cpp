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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>


#include "doomdef.h"
#include "dstrings.h"

#include "d_main.h"

#include "i_system.h"
#include "i_video.h"
#include "z_zone.h"
#include "v_video.h"
#include "w_wad.h"
#include "m_misc.h"
#include "r_local.h"


#include "hu_stuff.h"

#include "g_game.h"

#include "m_argv.h"
#include "m_swap.h"

#include "s_sound.h"

#include "doomstat.h"

// Data.
#include "sounds.h"

#include "m_menu.h"

#include "m_cheat.h"

#include "Main.h"
//#include "../game/player/PlayerProfileDoom.h"
#include "sys/sys_session.h"
#include "sys/sys_signin.h"
#include "d3xp/Game_local.h"
#include "../framework/Common_local.h" //GK: Include it also here in order to pass through the aspect ratio parameter

#include "info.h"
#include "d_items.h"
#include "f_finale.h"
#include "g_game.h"
#include "p_setup.h"

#include "am_map.h"
//#ifdef USE_OPENAL
#include "sound/OpenAL/AL_EAX.h"
//#endif
extern idCVar S_museax;

extern idCVar cl_messages;
extern idCVar cl_randpitch;
extern idCVar in_joylayout;
extern idCVar in_joystickRumble;
extern idCVar in_alwaysRunCl;
extern idCVar cl_freelook;
extern idCVar cl_jump;
extern idCVar cl_showStats;

extern idCVar cl_cursor;
extern idCVar r_clblurry;
extern idCVar cl_engineHz_interp;
extern idCVar cl_engineHz;
extern idCVar in_joyjpn;
extern idCVar r_swapInterval;
extern idCVar cl_HUD;
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
extern idCVar s_useXAudio2;
#endif

extern idCVar in_photomode;
extern idCVar in_toggleRun;
//
// defaulted values
//

// Show messages has default, 0 = off, 1 = on


// Blocky mode, has default, 0 = high, 1 = normal

// temp for ::g->screenblocks (0-9)

// -1 = no quicksave slot picked!

// 1 = message to be printed
// ...and here is the message string!

// message x & y

// timed message = no input from user

int mh = 0;

const char gammamsg[5][26] =
{
	GAMMALVL0,
		GAMMALVL1,
		GAMMALVL2,
		GAMMALVL3,
		GAMMALVL4
};

// we are going to be entering a savegame string
// old save description before edit






//
// MENU TYPEDEFS
//




const unsigned char cheat_menu_seq[] = {
	'\x17',' ','2','\x12','1','\x16',0xff
};


static int optoffs = 0; //GK: use for dynamicly adding/removing the number of message lines thermo from the menu 

static bool reset = false;

static bool hardreset = false;

// graphic name of skulls
// warning: initializer-string for array of chars is too long
char    skullName[2][/*8*/9] = 
{
	"M_SKULL1","M_SKULL2"
};

// current menudef

//
// PROTOTYPES
//
void M_NewGame(int choice);
void M_Episode(int choice);
void M_Expansion(int choice);
void M_ChooseSkill(int choice);
void M_LoadGame(int choice);
void M_LoadExpansion(int choice);
void M_SaveGame(int choice);
void M_Options(int choice);
void M_EndGame(int choice);
void M_ReadThis(int choice);
void M_ReadThis2(int choice);
void M_QuitDOOM(int choice);
void M_ExitGame(int choice);
void M_GameSelection(int choice);
void M_CancelExit(int choice);
void M_Alwaysrun(int choice);
void M_ChangeMessages(int choice);
void M_ChangeGPad(int choice);
void M_FullScreen(int choice);
void M_Aspect(int choice);
void M_Light(int choice);
void M_Resolution(int choice);
void M_Sync(int choice);
void M_SetRes(int choice);
void M_Refresh(int choice);
void M_Framerate(int choice);
void M_Blurry(int choice);
void M_ChangeSensitivity(int choice);
void M_SfxVol(int choice);
void M_MusicVol(int choice);
void M_MusicRev(int choice);
void M_RandomPitch(int choice);
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
void M_SAPI(int choice);
#endif
void M_ChangeDetail(int choice);
void M_SizeDisplay(int choice);
void M_StartGame(int choice);
void M_Sound(int choice);
void M_Video(int choice);
void M_MasterSelect(int choice);
void M_Doom_IT(int choice);
void M_Gameplay(int choice);
void M_Freelook(int choice);
void M_Autoaim(int choice);
void M_Jump(int choice);
void M_Cross(int choice);
void M_Mapst(int choice);
void M_HUD(int choice);
void M_Ctl(int choice);
void M_Key(int choice);
void M_ChangeKeys(int choice);
void M_Rumble(int choice);
void M_Layout(int choice);
void M_ToggleRun(int choice);

void M_FinishReadThis(int choice);
void M_LoadSelect(int choice);
void M_SaveSelect(int choice);
void M_ReadSaveStrings(void);
bool M_CheckQuickSave(void);
void M_QuickSave(void);
void M_QuickLoad(void);

void M_DrawMainMenu(void);
void M_DrawQuit(void);
void M_DrawReadThis1(void);
void M_DrawReadThis2(void);
void M_DrawNewGame(void);
void M_DrawEpisode(void);
void M_DrawOptions(void);
void M_DrawSound(void);
void M_DrawVideo(void);
void M_DrawLoad(void);
void M_DrawSave(void);
void M_DrawMaster(void);
void M_DrawDoomIT(void);
void M_DrawGame(void);
void M_DrawRes(void);
void M_DrawCtl(void);
void M_DrawKey(void);

void M_DrawSaveLoadBorder(int x,int y);
void M_SetupNextMenu(menu_t *menudef);
void M_DrawThermo(int x,int y,int thermWidth,int thermDot);
void M_DrawEmptyCell(menu_t *menu,int item);
void M_DrawSelCell(menu_t *menu,int item);
void M_WriteText(int x, int y, const char *string, bool aspect);
int  M_StringWidth(const char *string);
int  M_StringHeight(const char *string);
void M_StartControlPanel(void);
//GK:Use it and in other places
//void M_StartMessage(const char *string,messageRoutine_t routine,qboolean input);
void M_StopMessage(void);
void M_ClearMenus (void);

void M_DrawExtraReadThis(int choice);
void M_DrawErt(void);
//GK: Support for additional HELP lumps
menuitem_t etrareadthis[1] = {
	{ 1,"", M_FinishReadThis,'0' }
};
menu_t extradef = {
	1,
	&::g->MainDef,
	etrareadthis,
	M_DrawErt,
	280,185,
	0
};
idCVar doomit("doomit","0",CVAR_GAME|CVAR_ARCHIVE|CVAR_NOCHEAT|CVAR_INTEGER|CVAR_ROM,"0 = no DOOM-IT, 1 = DOOM-IT");
int state = 0;
idList<vidMode_t>			modeList;
idList<int>			classicRefreshList;
int refreshIndex = 0;
int modeSize = 0;
int pageIndex = 0;
//
// DOOM MENU
//




//
// EPISODE SELECT
//



//
// NEW GAME
//





//
// OPTIONS MENU
//



//
// Read This! MENU 1 & 2
//






//
// SOUND VOLUME MENU
//



//
// LOAD GAME MENU
//



//
// SAVE GAME MENU
//

typedef struct
{
	const char* display;
	const char* bind;
} bindInfo_t;

static bindInfo_t keyboardBinds[] =
{
	{ "Forward",	"_forward"								},	// FORWARD
	{ "Backward",	"_back"									},	// BACKPEDAL
	{ "Move Left",	"_moveLeft"								},	// MOVE LEFT
	{ "Move Right",	"_moveRight"							},	// MOVE RIGHT
	{ "Jump",	"_moveUp"								},	// JUMP
	{ "Turn Left",	"_left"									},	// TURN LEFT
	{ "Turn Right",	"_right"								},	// TURN RIGHT
	{ "Sprint",	"_speed"								},	// SPRINT

	{ "Attack",	"_attack"								},	// ATTACK
	{ "Next Weap",	"_impulse14"							},	// NEXT WEAPON
	{ "Prev Weap",	"_impulse15"							},	// PREV. WEAPON
	{ "Use",	"_use"						},	// USE
	{ "Auto Map",	"_impulse19"							},	// PDA / SCOREBOARD

	{ "Weap 1",	"_impulse0"							},	// FISTS / GRABBER
	{ "Weap 2",	"_impulse2"							},	// PISTOL
	{ "Weap 3",	"_impulse3"							},	// SHOTGUN / DOUBLE
	{ "Weap 4",	"_impulse5"							},	// MACHINEGUN
	{ "Weap 5",	"_impulse6"							},	// CHAINGUN
	{ "Weap 6",	"_impulse7"							},	// GRENADES
	{ "Weap 7",	"_impulse8"							},	// PLASMA GUN

	{ "Help Screen",	"help"							},	// NEXT WEAPON
	{ "Save Game",	"savegame"						},	// USE
	{ "Load Game",	"loadgame"							},	// PDA / SCOREBOARD
	{ "Sound Settings",	"soundsetting"							},	// FISTS / GRABBER
	{ "Real Light",	"rlight"							},	// PISTOL
	{ "Quick Save",	"savegame quick"							},	// SHOTGUN / DOUBLE
	{ "End Game",	"endgame"							},	// MACHINEGUN
	{ "Messages",	"messages"							},	// CHAINGUN
	{ "Quick Load",	"loadgame quick"							},	// GRENADES
	{ "Exit Game",	"exitgame"							},	// PLASMA GUN
	{ "Gamma Correction",	"gamma"							},	// PLASMA GUN

	{ "Photo Mode",	"_thirdperson"							},	// Photo Mode
};

static const int numBinds = sizeof(keyboardBinds) / sizeof(keyboardBinds[0]);

typedef struct
{
	const char* display;
	const char* bind;
} layoutbindInfo_t;

static layoutbindInfo_t joyLayoutBinds[] =
{
	{ "#JOY01", "JOY1"								},	// A/X/B
	{ "#JOY02", "JOY2"									},	// B/O/A
	{ "#JOY03", "JOY3"								},	// X/SQUARE/Y
	{ "#JOY04", "JOY4"							},	// Y/TRIANGLE/X
	{ "#JOY06", "JOY5"								},	// LB/L1/L
	{ "#JOY05","JOY6"									},	// RB/R1/R
	{ "#JOY10", "JOY7"								},	// LS/L3
	{ "#JOY09", "JOY8"								},	// RS/R3
	{ "#JOY15", "JOY9"								},	// Start/+
	{ "#JOY16", "JOY10"							},	// Select/-
	{ "#JOY11", "JOY_DPAD_UP"								},	// Dpad up
	{ "#JOY12", "JOY_DPAD_RIGHT"									},	// Dpad right
	{ "#JOY13", "JOY_DPAD_DOWN"								},	// Dpad down
	{ "#JOY14", "JOY_DPAD_LEFT"							},	// Dpad left
	{ "#JOY08", "JOY_TRIGGER1"							},	// LT/L2/ZL
	{ "#JOY07", "JOY_TRIGGER2"						},	// RT/R2/ZR
};

static const int numLayBinds = sizeof(joyLayoutBinds) / sizeof(joyLayoutBinds[0]);

int newmap = -1;
//
// M_ReadSaveStrings
//  read the strings from the savegame files
//
void M_ReadSaveStrings(void)
{
	idFile*         handle;
	int             count;
	int             i;
	char    name[256];

	for (i = 0;i < load_end;i++)
	{
		if (DoomLib::idealExpansion == ::g->rexp && ::g->gamemission == pack_custom && ::g->savedir) { //GK: Custom expansion related stuff
			sprintf(name, "%s\\%s%d.dsg", ::g->savedir, SAVEGAMENAME, i);
		}
		else
		if( common->GetCurrentGame() == DOOM_CLASSIC ) {
			sprintf(name,"DOOM\\%s%d.dsg",  SAVEGAMENAME,i );
		} else {
			if( DoomLib::idealExpansion == doom2 ) {
				sprintf(name,"DOOM2\\%s%d.dsg",  SAVEGAMENAME,i );
			} else if ( DoomLib::idealExpansion == pack_nerve){
				sprintf(name,"DOOM2_NRFTL\\%s%d.dsg",  SAVEGAMENAME,i );
			}
			else if (DoomLib::idealExpansion == pack_tnt) {
				sprintf(name, "DOOM2_TNT\\%s%d.dsg", SAVEGAMENAME, i);
			}
			else if (DoomLib::idealExpansion == pack_plut) {
				sprintf(name, "DOOM2_PLUT\\%s%d.dsg", SAVEGAMENAME, i);
			}
			else if (DoomLib::idealExpansion == pack_master) {
				sprintf(name, "DOOM2_MASTER\\%s%d.dsg", SAVEGAMENAME, i);
			}
			
			
		}

		handle = fileSystem->OpenFileRead ( name, false );
		if (handle == NULL)
		{
			strcpy(&::g->savegamestrings[i][0],EMPTYSTRING);
			::g->LoadMenu[i].status = 0;
			continue;
		}
		count = handle->Read( &::g->savegamestrings[i], SAVESTRINGSIZE );
		fileSystem->CloseFile( handle );	
		strcpy( ::g->savegamepaths[i], name );
		::g->LoadMenu[i].status = 1;
	}
}
//GK: From now on quicksave-quickload will use a hidden saveslot
//
// M_CheckQuickSave
//  read the strings from the savegame files
//
bool M_CheckQuickSave(void)
{
	idFile*         handle;
	//int             count;
	//int             i;
	char    name[256];

	//for (i = 0; i < load_end; i++)
	{
		if (::g->gamemission == pack_custom && ::g->savedir) { //GK: Custom expansion related stuff
			sprintf(name, "%s\\%s%d.dsg", ::g->savedir, QUICKSAVENAME, 7);
		}
		else
		if (common->GetCurrentGame() == DOOM_CLASSIC) {
			sprintf(name, "DOOM\\%s%d.dsg", QUICKSAVENAME, 7);
		}
		else {
				if (DoomLib::idealExpansion == doom2) {
					sprintf(name, "DOOM2\\%s%d.dsg", QUICKSAVENAME, 7);
				}
				else if (DoomLib::idealExpansion == pack_nerve) {
					sprintf(name, "DOOM2_NRFTL\\%s%d.dsg", QUICKSAVENAME, 7);
				}
				else if (DoomLib::idealExpansion == pack_tnt) {
					sprintf(name, "DOOM2_TNT\\%s%d.dsg", QUICKSAVENAME, 7);
				}
				else if (DoomLib::idealExpansion == pack_plut) {
					sprintf(name, "DOOM2_PLUT\\%s%d.dsg", QUICKSAVENAME, 7);
				}
				else if (DoomLib::idealExpansion == pack_master) {
					sprintf(name, "DOOM2_MASTER\\%s%d.dsg", QUICKSAVENAME, 7);
				}


		}

		handle = fileSystem->OpenFileRead(name, false);
		if (handle == NULL)
		{
			return false;
		}
		handle->Read(&::g->savegamestrings[7], SAVESTRINGSIZE);
		strcpy(::g->savegamepaths[7], name);
		return true;
	}
}
//GK: End
//
// M_LoadGame & Cie.
//
void M_DrawLoad(void)
{
	int             i;

	V_DrawPatchDirect (72,28,0,/*(patch_t*)*/img2lmp(W_CacheLumpName("M_LOADG",PU_CACHE_SHARED), W_GetNumForName("M_LOADG")), false);
	for (i = 0;i < load_end; i++)
	{
		M_DrawSaveLoadBorder(::g->LoadDef.x,::g->LoadDef.y+LINEHEIGHT*i);
		M_WriteText(::g->LoadDef.x,::g->LoadDef.y+LINEHEIGHT*i,::g->savegamestrings[i], false);
	}
	if (!idLib::joystick) {
		M_WriteText(::g->LoadDef.x, ::g->LoadDef.y + LINEHEIGHT * load_end, XTODEL, false);
	}
	else {
		M_WriteText(::g->LoadDef.x, ::g->LoadDef.y + LINEHEIGHT * load_end, XTODELGP, false);
	}
}



//
// Draw border for the savegame description
//
void M_DrawSaveLoadBorder(int x,int y)
{
	int             i;

	V_DrawPatchDirect (x-8,y+7,0,/*(patch_t*)*/img2lmp(W_CacheLumpName("M_LSLEFT",PU_CACHE_SHARED), W_GetNumForName("M_LSLEFT")), false);

	for (i = 0;i < 28;i++)
	{
		V_DrawPatchDirect (x,y+7,0,/*(patch_t*)*/img2lmp(W_CacheLumpName("M_LSCNTR",PU_CACHE_SHARED), W_GetNumForName("M_LSCNTR")), false);
		x += 8;
	}

	V_DrawPatchDirect (x,y+7,0,/*(patch_t*)*/img2lmp(W_CacheLumpName("M_LSRGHT",PU_CACHE_SHARED), W_GetNumForName("M_LSRGHT")), false);
}

void M_DeleteSelected(int ch) {
	if (ch != KEY_ENTER)
		return;
	fileSystem->RemoveFile(::g->savegamepaths[::g->itemOn]);
	M_SetupNextMenu(&::g->LoadDef);
	M_ReadSaveStrings();
}


//
// User wants to load this game
//
void M_LoadSelect(int choice)
{
	if( ::g->gamemode != commercial ) {
		G_LoadGame ( ::g->savegamepaths[ choice ] );
	} else {
		DoomLib::use_doomit = false;
		strcpy( DoomLib::loadGamePath, ::g->savegamepaths[ choice ] );
		if (G_CheckSave(DoomLib::loadGamePath)) {
			DoomLib::SetCurrentExpansion(DoomLib::idealExpansion);
			DoomLib::skipToLoad = true;
		}
		else {
			M_SetupNextMenu(&::g->MainDef);
		}
	}
	M_ClearMenus ();
}


void M_LoadExpansion(int choice)
{
	::g->exp = choice;
	bool procced = true;
	switch( choice ) {
	case 0:
		DoomLib::SetIdealExpansion( doom2 );
		break;
	case 1:
		if (DoomLib::hexp[3]) {
			DoomLib::SetIdealExpansion(pack_nerve);
		}
		else {
			procced = false;
			M_StartMessage(EXPROMT, NULL, false);
		}
		break;
	//GK: Before the expansion loads check if it is exist. Otherwise drop back to main menu
	case 2:
		if (DoomLib::hexp[0]) {
			DoomLib::SetIdealExpansion(pack_tnt);
		}
		else {
			procced = false;
			M_StartMessage(EXPROMT, NULL, false);
		}
		break;
	case 3:
		if (DoomLib::hexp[1]) {
			DoomLib::SetIdealExpansion(pack_plut);
		}
		else {
			procced = false;
			M_StartMessage(EXPROMT, NULL, false);
		}
		break;
	case 4:
		if (DoomLib::hexp[2]) {
			DoomLib::SetIdealExpansion(pack_master);
		}
		else {
			procced = false;
			M_StartMessage(EXPROMT, NULL, false);
		}
		break;
	}
	if (procced) {
		M_SetupNextMenu(&::g->LoadDef);
		M_ReadSaveStrings();
	}
	else {
		M_SetupNextMenu(&::g->MainDef);
	}
}

//
// Selected from DOOM menu
//
void M_LoadGame (int choice)
{
	if (::g->netgame)
	{
		M_StartMessage(LOADNET,NULL,false);
		return;
	}

	if (::g->gamemode == commercial) {
		M_SetupNextMenu(&::g->LoadExpDef);
	} else{
		M_SetupNextMenu(&::g->LoadDef);
		M_ReadSaveStrings();
	}
	
}


//
//  M_SaveGame & Cie.
//
void M_DrawSave(void)
{
	int             i;

	V_DrawPatchDirect (72,28,0,/*(patch_t*)*/img2lmp(W_CacheLumpName("M_SAVEG",PU_CACHE_SHARED), W_GetNumForName("M_SAVEG")), false);
	for (i = 0;i < load_end; i++)
	{
		M_DrawSaveLoadBorder(::g->LoadDef.x,::g->LoadDef.y+LINEHEIGHT*i);
		M_WriteText(::g->LoadDef.x,::g->LoadDef.y+LINEHEIGHT*i,::g->savegamestrings[i], false);
	}

	if (::g->saveStringEnter)
	{
		i = M_StringWidth(::g->savegamestrings[::g->saveSlot]);
		M_WriteText(::g->LoadDef.x + i,::g->LoadDef.y+LINEHEIGHT*::g->saveSlot,"_", false);
	}
}

//
// M_Responder calls this when user is finished
//
void M_DoSave(int slot)
{
	G_SaveGame (slot,::g->savegamestrings[slot]);
	M_ClearMenus ();

	// PICK QUICKSAVE SLOT YET? GK: Already did
	//if (::g->quickSaveSlot == -2)
	//	::g->quickSaveSlot = slot;
}

//
// User wants to save. Start string input for M_Responder
//
//
// Locally used constants, shortcuts.
//
//GK:Remember no CONSTANTS
extern /*const*/ char* mapnames[];
extern /*const*/ char* mapnames2[];
extern char* mapnamesp[];
extern char* mapnamest[];
void M_SaveSelect(int choice)
{
	//GK: show modded level names on save files
	/*const*/ char* s=new char[256];
	char* mapname;
	const ExpansionData* exp = DoomLib::GetCurrentExpansion();

	switch ( ::g->gamemode )
	{
	case shareware:
	case registered:
	case retail:
		s = (/*exp->*/mapnames[(::g->gameepisode-1)*9+::g->gamemap-1]);
		break;
	case commercial:
		if (DoomLib::expansionSelected == doom2) {
			char* ts;
			mapname = new char[256];
			strcpy(mapname, mapnames2[::g->gamemap - 1]);
			ts = strtok(mapname, " ");
			if (!idStr::Icmp(ts, "level")) {
				//s = (/*exp->*/mapnames2[::g->gamemap-1]);
				ts = strtok(NULL, " ");
			}
			strcpy(s, ts);
			while (true) {
				ts = strtok(NULL, " ");
				if (ts != NULL) {
					strcat(s, " ");

					strcat(s, ts);
				}
				else {
					break;
				}

			}
		}
		else if (DoomLib::expansionSelected == pack_tnt) {
			char* ts;
			mapname = new char[256];
			strcpy(mapname, mapnamest[::g->gamemap - 1]);
			ts = strtok(mapname, " ");
			if (!idStr::Icmp(ts, "level")) {
				//s = (/*exp->*/mapnames2[::g->gamemap-1]);
				ts = strtok(NULL, " ");
			}
			strcpy(s, ts);
			while (true) {
				ts = strtok(NULL, " ");
				if (ts != NULL) {
					strcat(s, " ");

					strcat(s, ts);
				}
				else {
					break;
				}

			}
		}
		else if (DoomLib::expansionSelected == pack_plut) {
			char* ts;
			mapname = new char[256];
			strcpy(mapname, mapnamesp[::g->gamemap - 1]);
			ts = strtok(mapname, " ");
			if (!idStr::Icmp(ts, "level")) {
				//s = (/*exp->*/mapnames2[::g->gamemap-1]);
				ts = strtok(NULL, " ");
			}
			strcpy(s, ts);
			while (true) {
				ts = strtok(NULL, " ");
				if (ts != NULL) {
					strcat(s, " ");

					strcat(s, ts);
				}
				else {
					break;
				}

			}
		}
		else {
			//GK: Give the modified level name instead
			if (::g->modifiedtext) {
				char* ts;
				mapname = new char[256];
				strcpy(mapname, mapnames2[::g->gamemap - 1]);
				ts = strtok(mapname, " ");
				if (!idStr::Icmp(ts, "level")) {
					//s = (/*exp->*/mapnames2[::g->gamemap-1]);
					ts = strtok(NULL, " ");
				}
				strcpy(s, ts);
				while (true) {
					ts = strtok(NULL, " ");
					if (ts != NULL) {
						strcat(s, " ");

						strcat(s, ts);
					}
					else {
						break;
					}

				}
			}
			else {
				s = strdup(exp->mapNames[::g->gamemap - 1]);
			}
		}
		break;
	default:
		s = strdup(exp->mapNames[::g->gamemap-1]);
		break;
	}

	if (::g->gamemission == pack_custom) {
		int map = ::g->gamemap;
		if (::g->gamemode == retail && ::g->clusters[::g->gameepisode - 1].startmap) {
			map = ::g->clusters[::g->gameepisode - 1].startmap + (::g->gamemap - 1);
		}
		if (::g->maps[map - 1].realname) {
			s = strdup(::g->maps[map - 1].realname);
		}
	}

	::g->saveSlot = choice;	
	strcpy(::g->savegamestrings[::g->saveSlot], s);
	M_DoSave(::g->saveSlot);
}

//
// Selected from DOOM menu
//
void M_SaveGame (int choice)
{
	if (!::g->usergame)
	{
		M_StartMessage(SAVEDEAD,NULL,false);
		return;
	}
	else if( ::g->plyr && ::g->plyr->mo && ::g->plyr->mo->health <= 0 ) {
		M_StartMessage("you can't save if you're dead!\n\npress any button",NULL,false);
		return;
	}


	if (::g->gamestate != GS_LEVEL)
		return;

	// Reset back to what expansion we are currently playing.
	DoomLib::SetIdealExpansion( DoomLib::expansionSelected );

	M_SetupNextMenu(&::g->SaveDef);
	M_ReadSaveStrings();
}



//
//      M_QuickSave
//

void M_QuickSaveResponse(int ch)
{
	if (ch == KEY_ENTER)
	{
		M_SaveSelect(::g->quickSaveSlot); //GK: use Saveselect instead in order to change it's name
		S_StartSound(NULL,sfx_swtchx);
	}
}

void M_QuickSave(void)
{
	if (!::g->usergame)
	{
		S_StartSound(NULL,sfx_oof);
		return;
	}

	if (::g->gamestate != GS_LEVEL)
		return;

	if (::g->quickSaveSlot < 0)
	{
		//M_StartControlPanel();
		//M_ReadSaveStrings();
		//M_SetupNextMenu(&::g->SaveDef);
		//GK: Use a hidden saveslot for quick saves
		::g->quickSaveSlot = 7;	// means to pick a slot now
		M_SaveSelect(::g->quickSaveSlot);
		return;
	}
	if (!idLib::joystick) {
		sprintf(::g->tempstring, QSPROMPT, ::g->savegamestrings[::g->quickSaveSlot]);
	}
	else {
		sprintf(::g->tempstring, M_UseCircleForAccept() ? QSPROMPTGPJ : QSPROMPTGP, ::g->savegamestrings[::g->quickSaveSlot]);
	}
	M_StartMessage(::g->tempstring,M_QuickSaveResponse,true);
}



//
// M_QuickLoad
//
void M_QuickLoadResponse(int ch)
{
	if (ch == KEY_ENTER)
	{
		M_LoadSelect(::g->quickSaveSlot);
		S_StartSound(NULL,sfx_swtchx);
	}
}


void M_QuickLoad(void)
{
	if (::g->netgame)
	{
		M_StartMessage(QLOADNET,NULL,false);
		return;
	}

	//GK: No longer need to assign quicksave slot. Instead check for the hidden save
	if (!M_CheckQuickSave())
	{
		M_StartMessage(QSAVESPOT,NULL,false);
		return;
	}
	else {
		::g->quickSaveSlot = 7;
	}
	if (!idLib::joystick) {
		sprintf(::g->tempstring, QLPROMPT, ::g->savegamestrings[::g->quickSaveSlot]);
	}
	else {
		sprintf(::g->tempstring, M_UseCircleForAccept() ? QLPROMPTGPJ : QLPROMPTGP, ::g->savegamestrings[::g->quickSaveSlot]);
	}
	M_StartMessage(::g->tempstring,M_QuickLoadResponse,true);
}




//
// Read This Menus
// Had a "quick hack to fix romero bug"
//
void M_DrawReadThis1(void)
{
	::g->inhelpscreens = true;
	switch ( ::g->gamemode )
	{
	case commercial:
		V_DrawPatchDirect (0,0,0,/*(patch_t*)*/img2lmp(W_CacheLumpName("HELP",PU_CACHE_SHARED), W_GetNumForName("HELP")), false);
		break;
	case shareware:
	case registered:
	case retail:
			V_DrawPatchDirect(0, 0, 0, /*(patch_t*)*/img2lmp(W_CacheLumpName("HELP1", PU_CACHE_SHARED), W_GetNumForName("HELP1")), false);
			//GK: For ultimate DOOM check if we are using mod that add additional HELP lumps. If not then instantly close "Read This!" 
			if (W_GetNumForName("HELP2") <= -1 && W_GetNumForName("HELP01") <= -1) {
				mh = -1;
				::g->ReadMenu1[0].routine = M_FinishReadThis;
			}
		break;
	default:
		break;
	}
	return;
}
//GK:Read this for specific additional modded help pages
void M_DrawExtraReadThis(int choice) {
	choice = 0;
	M_SetupNextMenu(&extradef);
}

void M_DrawErt(void) {
	::g->inhelpscreens = true;
	if (W_GetNumForName("HELP02") > -1) {
		V_DrawPatchDirect(0, 0, 0, /*(patch_t*)*/img2lmp(W_CacheLumpName("HELP02", PU_CACHE_SHARED), W_GetNumForName("HELP02")), false);
	}
	mh = 0;
	::g->ReadMenu2[0].routine = M_FinishReadThis;
	return;
}
//GK end

//
// Read This Menus - optional second page.
//
void M_DrawReadThis2(void)
{
	::g->inhelpscreens = true;
	switch ( ::g->gamemode )
	{
	case retail:
		//GK: In case we use mod that uses additional HELP lumps
			if (W_GetNumForName("HELP2") > -1) {
				V_DrawPatchDirect(0, 0, 0, /*(patch_t*)*/img2lmp(W_CacheLumpName("HELP2", PU_CACHE_SHARED), W_GetNumForName("HELP2")), false);
			}
			else
			{
				if (W_GetNumForName("HELP01") > -1) {
					V_DrawPatchDirect(0, 0, 0, /*(patch_t*)*/img2lmp(W_CacheLumpName("HELP01", PU_CACHE_SHARED), W_GetNumForName("HELP01")), false);
					mh = 1;

				}
				//GK: No need
				/*else {
					//V_DrawPatchDirect(0, 0, 0, (patch_t*)img2lmp(W_CacheLumpName("CREDIT", PU_CACHE_SHARED));
				}*/
			}
		break;
	case commercial:
		// This hack keeps us from having to change menus.
		V_DrawPatchDirect (0,0,0,/*(patch_t*)*/img2lmp(W_CacheLumpName("CREDIT",PU_CACHE_SHARED), W_GetNumForName("CREDIT")), false);
		break;
	case shareware:
	case registered:
		V_DrawPatchDirect (0,0,0,/*(patch_t*)*/img2lmp(W_CacheLumpName("HELP2",PU_CACHE_SHARED), W_GetNumForName("HELP2")), false);
		break;
	default:
		break;
	}
	if (mh == 1) {
		::g->ReadMenu2[0].routine = M_DrawExtraReadThis;
	}
	return;
}
//GK: Put them here in order to be used by the sound settings
char	msgNames[2][9] =
{
"M_MSGOFF","M_MSGON"
};
char	sapiNames[2][9] =
{
"M_OPENAL", "M_XAUDIO"
};
//
// Change Sfx & Music volumes
//
void M_DrawSound(void)
{
	int randpitch = cl_randpitch.GetInteger() >= 1 ? 1 : 0;
	int musrev = S_museax.GetInteger() >= 1 ? 1 : 0;
	V_DrawPatchDirect (60,38,0,/*(patch_t*)*/img2lmp(W_CacheLumpName("M_SVOL",PU_CACHE_SHARED), W_GetNumForName("M_SVOL")), false);

	M_DrawThermo( ::g->SoundDef.x,::g->SoundDef.y+LINEHEIGHT*(sfx_vol+1),
		16, s_volume_sound.GetInteger() );

	M_DrawThermo(::g->SoundDef.x,::g->SoundDef.y+LINEHEIGHT*(music_vol+1),
		16, s_volume_midi.GetInteger() );
	V_DrawPatchDirect(::g->SoundDef.x + 200, ::g->SoundDef.y + LINEHEIGHT * sound_rp, 0,
		/*(patch_t*)*/img2lmp(W_CacheLumpName(msgNames[randpitch], PU_CACHE_SHARED), W_GetNumForName(msgNames[randpitch])), false);
	V_DrawPatchDirect(::g->SoundDef.x + 170, ::g->SoundDef.y + LINEHEIGHT * music_rev, 0,
		/*(patch_t*)*/img2lmp(W_CacheLumpName(msgNames[musrev], PU_CACHE_SHARED), W_GetNumForName(msgNames[musrev])), false);
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	int sapi = s_useXAudio2.GetInteger() >= 1 ? 1 : 0;
	V_DrawPatchDirect(::g->SoundDef.x + 130, ::g->SoundDef.y + LINEHEIGHT * s_api, 0,
		/*(patch_t*)*/img2lmp(W_CacheLumpName(sapiNames[sapi], PU_CACHE_SHARED), W_GetNumForName(sapiNames[sapi])), false);
#endif
}

char	fullNames[3][9] =
{
"M_MSGOFF","M_MSGON", "M_BORL"
};

char    detailNames[3][9] =
{
"M_GDLOW","M_DETAIL","M_DISOPT" //GK: Use unique values for aspect ratio
};

char	lightNames[3][9] =
{
	"M_MSGOFF", "M_DARK", "M_LIGHT"
};

char	syncNames[3][9] =
{
	"M_MSGOFF", "M_SMART", "M_MSGON"
};

//GK:Begin
//
// Change aspect Ratio & lighting
//
void M_DrawVideo(void)
{
	V_DrawPatchDirect(60, 38, 0,/*(patch_t*)*/img2lmp(W_CacheLumpName("M_VID", PU_CACHE_SHARED), W_GetNumForName("M_VID")), false);

	int aspect = r_aspect.GetInteger() >= 1 ? 1 : 0;
	int correct = r_aspectcorrect.GetInteger();
	int asoffset = 165 - (6 * correct); //GK: The word "correct" is larger than the others and therefor it requires different x offset
	int reallight = r_clight.GetInteger();
	int fullscreenOnOff = r_fullscreen.GetInteger() >= 1 ? 1 : r_fullscreen.GetInteger() < 0 ? 2 : 0;
	//int blurryeffect = r_clblurry.GetInteger();
	int syncValue = r_swapInterval.GetInteger();
	char* res = new char[11];
	sprintf(res, "%4i x %4i", r_customWidth.GetInteger(), r_customHeight.GetInteger());
	std::string fps = va("%d FPS", cl_engineHz.GetInteger());
	if (cl_engineHz_interp.GetBool()) {
		fps += "I";
	}
	std::string refreshString = va("%d Hz", com_engineHz.GetInteger());

	V_DrawPatchDirect(::g->VideoDef.x + 150, ::g->VideoDef.y + LINEHEIGHT * endgame, 0,
		/*(patch_t*)*/img2lmp(W_CacheLumpName(fullNames[fullscreenOnOff], PU_CACHE_SHARED), W_GetNumForName(fullNames[fullscreenOnOff])), false);
	V_DrawPatchDirect(::g->VideoDef.x + asoffset, ::g->VideoDef.y + LINEHEIGHT * (detail - (cl_engineHz_interp.GetBool() ? 0 : 1)), 0,
		/*(patch_t*)*/img2lmp(W_CacheLumpName(detailNames[aspect + correct], PU_CACHE_SHARED), W_GetNumForName(detailNames[aspect + correct])), false);
	V_DrawPatchDirect(::g->VideoDef.x + 135, ::g->VideoDef.y + LINEHEIGHT * (light - (cl_engineHz_interp.GetBool() ? 0 : 1)), 0,
		/*(patch_t*)*/img2lmp(W_CacheLumpName(lightNames[reallight], PU_CACHE_SHARED), W_GetNumForName(lightNames[reallight])), false);
	V_DrawPatchDirect(::g->VideoDef.x + 105, ::g->VideoDef.y + LINEHEIGHT * vsync, 0,
		/*(patch_t*)*/img2lmp(W_CacheLumpName(syncNames[syncValue], PU_CACHE_SHARED), W_GetNumForName(syncNames[syncValue])), false);
	//V_DrawPatchDirect(::g->VideoDef.x + 160, ::g->VideoDef.y + LINEHEIGHT * (blurry), 0,
	//	/*(patch_t*)*/img2lmp(W_CacheLumpName(msgNames[blurryeffect], PU_CACHE_SHARED), W_GetNumForName(msgNames[blurryeffect])));
	M_WriteText(::g->VideoDef.x + 150, ::g->VideoDef.y + LINEHEIGHT * (resolution) + 6, res, false);
	M_WriteText(::g->VideoDef.x + 133, ::g->VideoDef.y + LINEHEIGHT * (framerate - (cl_engineHz_interp.GetBool() ? 0 : 1)) + 6, fps.c_str(), false);
	if (cl_engineHz_interp.GetBool()) {
		M_WriteText(::g->VideoDef.x + 160, ::g->VideoDef.y + LINEHEIGHT * (refresh)+6, refreshString.c_str(), false);
	}
}

void M_Video(int choice)
{
	M_SetupNextMenu(&::g->VideoDef);
}
//
// Toogle always run & Freelook
//
void M_DrawGame(void)
{
	V_DrawPatchDirect(60, 38, 0,/*(patch_t*)*/img2lmp(W_CacheLumpName("M_GM", PU_CACHE_SHARED), W_GetNumForName("M_GM")), false);

	int alwayrun = in_alwaysRunCl.GetInteger();
	int freelook = cl_freelook.GetInteger();
	int jumping = cl_jump.GetBool();
	int crosshair = cl_cursor.GetBool();
	int mapstats = cl_showStats.GetBool();
	int autoaim = game->GetCVarBool("aa_targetAimAssistEnable");
	int hudValue = cl_HUD.GetBool();

	int aimOffset = cl_freelook.GetBool() ? 0 : 1;


	V_DrawPatchDirect(::g->GameDef.x + 120, ::g->GameDef.y + LINEHEIGHT * run, 0,
		/*(patch_t*)*/img2lmp(W_CacheLumpName(msgNames[alwayrun], PU_CACHE_SHARED), W_GetNumForName(msgNames[alwayrun])), false);
	V_DrawPatchDirect(::g->GameDef.x + 135, ::g->GameDef.y + LINEHEIGHT * (look), 0,
		/*(patch_t*)*/img2lmp(W_CacheLumpName(msgNames[freelook], PU_CACHE_SHARED), W_GetNumForName(msgNames[freelook])), false);
	if (cl_freelook.GetBool()) {
		V_DrawPatchDirect(::g->GameDef.x + 120, ::g->GameDef.y + LINEHEIGHT * (aim), 0,
			/*(patch_t*)*/img2lmp(W_CacheLumpName(msgNames[autoaim], PU_CACHE_SHARED), W_GetNumForName(msgNames[autoaim])), false);
	}
	V_DrawPatchDirect(::g->GameDef.x + 70, ::g->GameDef.y + LINEHEIGHT * (jump - aimOffset), 0,
		/*(patch_t*)*/img2lmp(W_CacheLumpName(msgNames[jumping], PU_CACHE_SHARED), W_GetNumForName(msgNames[jumping])), false);
	V_DrawPatchDirect(::g->GameDef.x + 140, ::g->GameDef.y + LINEHEIGHT * (cross - aimOffset), 0,
		/*(patch_t*)*/img2lmp(W_CacheLumpName(msgNames[crosshair], PU_CACHE_SHARED), W_GetNumForName(msgNames[crosshair])), false);
	V_DrawPatchDirect(::g->GameDef.x + 121, ::g->GameDef.y + LINEHEIGHT * (mapst - aimOffset), 0,
		/*(patch_t*)*/img2lmp(W_CacheLumpName(msgNames[mapstats], PU_CACHE_SHARED), W_GetNumForName(msgNames[mapstats])), false);
	V_DrawPatchDirect(::g->GameDef.x + 60, ::g->GameDef.y + LINEHEIGHT * (hud - aimOffset), 0,
		/*(patch_t*)*/img2lmp(W_CacheLumpName(msgNames[hudValue], PU_CACHE_SHARED), W_GetNumForName(msgNames[hudValue])), false);
}

void M_Gameplay(int choice)
{
	M_SetupNextMenu(&::g->GameDef);
}
//GK: End
void M_Sound(int choice)
{
	M_SetupNextMenu(&::g->SoundDef);
}

void M_SfxVol(int choice)
{
	switch(choice)
	{
	case 0:
		s_volume_sound.SetInteger( s_volume_sound.GetInteger() - 1 );
		break;
	case 1:
		s_volume_sound.SetInteger( s_volume_sound.GetInteger() + 1 );
		break;
	}

	S_SetSfxVolume( s_volume_sound.GetInteger() );
}

void M_MusicVol(int choice)
{
	switch(choice)
	{
	case 0:
		s_volume_midi.SetInteger( s_volume_midi.GetInteger() - 1 );
		break;
	case 1:
		s_volume_midi.SetInteger( s_volume_midi.GetInteger() + 1 );
		break;
	}

	S_SetMusicVolume( s_volume_midi.GetInteger() );
}

void M_RandomPitch(int choice) {
	cl_randpitch.SetBool(!cl_randpitch.GetBool());
}

void M_MusicRev(int choice) {
	S_museax.SetBool(!S_museax.GetBool());
}

#if defined(_MSC_VER) && defined(USE_XAUDIO2)
void M_SAPI(int choice) {
	s_useXAudio2.SetBool(!s_useXAudio2.GetBool());
	hardreset = true;
}
#endif


//
// M_DrawMainMenu
//
void M_DrawMainMenu(void)
{
	V_DrawPatchDirect (94,2,0,/*(patch_t*)*/img2lmp(W_CacheLumpName("M_DOOM",PU_CACHE_SHARED), W_GetNumForName("M_DOOM")), false);
}

//
// M_DrawQuit
//
void M_DrawQuit(void) {
	V_DrawPatchDirect (54,38,0,/*(patch_t*)*/img2lmp(W_CacheLumpName("M_EXITO",PU_CACHE_SHARED), W_GetNumForName("M_EXITO")), false);
}



//
// M_NewGame
//
void M_DrawNewGame(void)
{
	V_DrawPatchDirect (96,14,0,/*(patch_t*)*/img2lmp(W_CacheLumpName("M_NEWG",PU_CACHE_SHARED), W_GetNumForName("M_NEWG")), false);
	V_DrawPatchDirect (54,38,0,/*(patch_t*)*/img2lmp(W_CacheLumpName("M_SKILL",PU_CACHE_SHARED), W_GetNumForName("M_SKILL")), false);
}

void M_NewGame(int choice)
{
	if (::g->netgame && !::g->demoplayback)
	{
		M_StartMessage(NEWGAME,NULL,false);
		return;
	}

	if ( ::g->gamemode == commercial )
		M_SetupNextMenu(&::g->ExpDef); 
	else
		M_SetupNextMenu(&::g->EpiDef);
}


//
//      M_Episode
//

void M_DrawEpisode(void)
{
	V_DrawPatchDirect (54,38,0,/*(patch_t*)*/img2lmp(W_CacheLumpName("M_EPISOD",PU_CACHE_SHARED), W_GetNumForName("M_EPISOD")), false);
}

void M_VerifyNightmare(int ch)
{
	if (ch != KEY_ENTER)
		return;

	if (::g->gamemode != commercial) {
		static int startLevel = 1;
		G_DeferedInitNew((skill_t)nightmare, ::g->epi + 1, startLevel);
		
	}
	else {
		if (DoomLib::idealExpansion == pack_master && state == 0)
			DoomLib::use_doomit = false;
		if (DoomLib::idealExpansion != pack_master)
			DoomLib::use_doomit = false;
		DoomLib::SetCurrentExpansion(DoomLib::idealExpansion);
		DoomLib::skipToNew = true;
		DoomLib::chosenSkill = nightmare;
		DoomLib::chosenEpisode = ::g->epi + 1;
	}
	M_ClearMenus();
}

void M_ChooseSkill(int choice)
{
	
	if (choice == nightmare)
	{
		if (!idLib::joystick) {
			M_StartMessage(NIGHTMARE, M_VerifyNightmare, true);
		}
		else {
			M_StartMessage(M_UseCircleForAccept() ? NIGHTMAREGPJ : NIGHTMAREGP, M_VerifyNightmare, true);
		}
		return;
	}
	
	if ( ::g->gamemode != commercial ) {
		static int startLevel = 1;
		G_DeferedInitNew((skill_t)choice,::g->epi+1, startLevel);
		{ //GK: Set Endmap for the selected episode
			if ((int)::g->clusters.size() <= ::g->epi){
				::g->gamemission = doom;
			}
			else {
				if (!::g->clusters[::g->epi].startmap && ::g->clusters[::g->epi].mapname == NULL) {
					::g->gamemission = doom;
				}
				else {
					::g->gamemission = pack_custom;
					if (!::g->clusters[::g->epi].endmap) {
						::g->clusters[::g->epi].endmap = ::g->clusters[::g->epi].startmap + 7;
					}
					::g->endmap = ::g->clusters[::g->epi].endmap;
				}
			}
		}
		M_ClearMenus ();
	} else {
		if (DoomLib::idealExpansion == pack_master && state == 0)
			DoomLib::use_doomit = false;
		if (DoomLib::idealExpansion != pack_master)
			DoomLib::use_doomit = false;
		DoomLib::SetCurrentExpansion( DoomLib::idealExpansion );
		DoomLib::skipToNew = true;
		DoomLib::chosenSkill = choice;
		DoomLib::chosenEpisode = ::g->epi+1;
	}
}

void M_Episode(int choice)
{
	// Yet another hack...
	if ( (::g->gamemode == registered)
		&& (choice > 2))
	{
		I_PrintfE("M_Episode: 4th episode requires UltimateDOOM\n");
		choice = 0;
	}
	if (::g->gamemode == shareware && choice > 0) {
		M_StartMessage(SWSTRINGBFG, NULL, true);
		return;
	}

	::g->epi = choice;
	M_SetupNextMenu(&::g->NewDef);
}

void M_Expansion(int choice)
{
	::g->exp = choice;
	bool procced = true;
	if( choice == 0 ) {
		DoomLib::SetIdealExpansion( doom2 );
	}else if (choice == 1){
		if (DoomLib::hexp[3]) {
			DoomLib::SetIdealExpansion(pack_nerve);
		}
		else {
			procced = false;
			M_StartMessage(EXPROMT, NULL, false);
		}
	}
	else if (choice == 2) {
		if (DoomLib::hexp[0]) {
			DoomLib::SetIdealExpansion(pack_tnt);
		}
		else {
			procced = false;
			M_StartMessage(EXPROMT, NULL, false);
		}
	}
	else if (choice == 3) {
		if (DoomLib::hexp[1]) {
			DoomLib::SetIdealExpansion(pack_plut);
		}
		else {
			procced = false;
			M_StartMessage(EXPROMT, NULL, false);
		}
	}
	else if (choice == 4) {
		if (DoomLib::hexp[2]) {
			DoomLib::SetIdealExpansion(pack_master);
			if (doomit.GetInteger() == 1) {
				DoomLib::use_doomit = true;
				state = 0;
				M_SetupNextMenu(&::g->MasterDef);
			}
		}
		else {
			procced = false;
			M_StartMessage(EXPROMT, NULL, false);
		}
	}
		if (procced) {
			if (choice == 4 && !DoomLib::use_doomit) {
				M_SetupNextMenu(&::g->NewDef);
			}
			else if (choice != 4) {
				M_SetupNextMenu(&::g->NewDef);
			}
			else if (choice == 4 && DoomLib::use_doomit && doomit.GetInteger() == 0) {
				M_SetupNextMenu(&::g->NewDef);
			}
		}
		else {
			M_SetupNextMenu(&::g->MainDef);
		}
}

void M_MasterSelect(int choice) {
	if (choice == 0) {
		if (state == 0) {
			//DoomLib::use_doomit = false;
			M_SetupNextMenu(&::g->NewDef);
		}
		else {
			state=2;
			M_SetupNextMenu(&::g->DOOMITDef);
		}
	}
	else {
		if (state == 0) {
			state=3;
			M_SetupNextMenu(&::g->MasterDef);
		}
		else {
			state=1;
			M_SetupNextMenu(&::g->DOOMITDef);
		}
	}
}

void M_DrawMaster(void) {
	for (int i = 0; i < master_end; i++) {
		if (state == 0) {
			M_WriteText(::g->MasterDef.x, ::g->MasterDef.y + LINEHEIGHT * i, MASTER[i], false);
		}
		else {
			M_WriteText(::g->MasterDef.x, ::g->MasterDef.y + LINEHEIGHT * i, M2[i], false);
		}
	}
}

void M_Doom_IT(int choice) {
	DoomLib::use_doomit = true;
	if (state == 2) {
		DoomLib::selection = choice + 1;
	}
	else {
		DoomLib::selection = choice + 11;
	}
	//state = 0;
	M_SetupNextMenu(&::g->NewDef);
}

void M_DrawDoomIT(void) {
	for (int i = 0; i < doomit_end/2; i++) {
		if (state == 2) {
			M_WriteText(::g->DOOMITDef.x, ::g->DOOMITDef.y + LINEHEIGHT * i, masterlist[i], false);
		}
		else {
			M_WriteText(::g->DOOMITDef.x, ::g->DOOMITDef.y + LINEHEIGHT * i, masterlist[i+10], false);
		}
	}
}

//
// M_Options
//


int M_GetMouseSpeedForMenu( float cvarValue ) {
	const float shiftedMouseSpeed = cvarValue - 0.25f;
	const float normalizedMouseSpeed = shiftedMouseSpeed / ( 4.0f - 0.25 );
	const float scaledMouseSpeed = normalizedMouseSpeed * 15.0f;
	const int roundedMouseSpeed = static_cast< int >( scaledMouseSpeed + 0.5f );
	
	return roundedMouseSpeed;
}

void M_DrawOptions(void)
{
	V_DrawPatchDirect (108,15,0,/*(patch_t*)*/img2lmp(W_CacheLumpName("M_OPTTTL",PU_CACHE_SHARED), W_GetNumForName("M_OPTTTL")), false);

	//V_DrawPatchDirect (::g->OptionsDef.x + 175,::g->OptionsDef.y+LINEHEIGHT*detail,0,
	//	(patch_t*)W_CacheLumpName(detailNames[::g->detailLevel],PU_CACHE_SHARED));
		
	V_DrawPatchDirect (::g->OptionsDef.x + 120,::g->OptionsDef.y+LINEHEIGHT*messages,0,
		/*(patch_t*)*/img2lmp(W_CacheLumpName(msgNames[m_show_messages.GetInteger()],PU_CACHE_SHARED), W_GetNumForName(msgNames[m_show_messages.GetInteger()])), false);
	//GK:begin
	if (m_show_messages.GetInteger()) {
		optoffs = 1;
		const int clmessages=cl_messages.GetInteger()-1;
		M_DrawThermo(::g->OptionsDef.x, ::g->OptionsDef.y + LINEHEIGHT * (messages + optoffs), 4, clmessages);
	}
	else {
		optoffs = 0;
	}
	//GK: No longer needed here
	/*V_DrawPatchDirect(::g->OptionsDef.x + 120, ::g->OptionsDef.y + LINEHEIGHT * (scrnsize+optoffs), 0,
		(patch_t*)W_CacheLumpName(msgNames[in_alwaysRunCl.GetBool()], PU_CACHE_SHARED));
	*/
	//GK:End
	/*extern idCVar in_mouseSpeed;
	const int roundedMouseSpeed = M_GetMouseSpeedForMenu( in_mouseSpeed.GetFloat() );

	M_DrawThermo( ::g->OptionsDef.x, ::g->OptionsDef.y + LINEHEIGHT * ( mousesens + 1+optoffs ), 16, roundedMouseSpeed );*/

	//M_DrawThermo(::g->OptionsDef.x,::g->OptionsDef.y+LINEHEIGHT*(scrnsize+1),
	//	9,::g->screenSize);
}

void M_Options(int choice)
{
	M_SetupNextMenu(&::g->OptionsDef);
}

void M_DrawRes(void) {
	V_DrawPatchDirect(108, 10, 0,/*(patch_t*)*/img2lmp(W_CacheLumpName("M_RES", PU_CACHE_SHARED), W_GetNumForName("M_RES")), false);
	modeList.Clear();
	if (r_fullscreen.GetInteger() > 1) {
		R_GetModeListForDisplay(r_fullscreen.GetInteger() - 1, modeList);
	}
	else {
		R_GetModeListForDisplay(0, modeList);
	}
	modeSize = modeList.Num() < doomit_end ? modeList.Num() : doomit_end;
	for (int i = 0; i < modeSize; i++) {
		std::string res;
		res = va("%4i x %4i", modeList[i].width, modeList[i].height);
		if (modeList[i].displayHz != 60)
		{
			res += va(" @ %dhz", modeList[i].displayHz);
		}
		if (i < 10) {
			M_WriteText(::g->ResDef.x, ::g->ResDef.y + LINEHEIGHT * i, res.c_str(), false);
		}
		else {
			M_WriteText(::g->ResDef.x + 120, ::g->ResDef.y + LINEHEIGHT * (i - 10), res.c_str(), false);
		}
	}
}


//
//      Toggle messages on/off
//
void M_ChangeMessages(int choice)
{
	// warning: unused parameter `int choice'
	//choice = 0;
	bool sm = m_show_messages.GetBool();
	if (!sm) {
		m_show_messages.SetBool(!m_show_messages.GetBool());
		if (choice == 0) { //GK: Try to circle through the options
			cl_messages.SetInteger(4);
		}
	}
	else if ((cl_messages.GetInteger() == 1 && choice ==0)||(cl_messages.GetInteger()== 4 && choice == 1)){
		m_show_messages.SetBool(!m_show_messages.GetBool());
		cl_messages.SetInteger(1);
	}

	if (sm != m_show_messages.GetBool()) {

	if (!m_show_messages.GetBool())
		::g->players[::g->consoleplayer].message = MSGOFF;
	else
		::g->players[::g->consoleplayer].message = MSGON ;


	::g->message_dontfuckwithme = true;
	}
	if (m_show_messages.GetBool()) {
		int clmess = cl_messages.GetInteger();
		if (sm) {
			switch (choice) { //GK: Put on use the 'choice' parameter
			case 0:
				if (clmess > 1) {
					clmess--;
				}
				break;
			case 1:
				if (clmess < 4) {
					clmess++;
				}
				break;
			}
		}
		cl_messages.SetInteger(clmess);
		HUlib_initSText(&::g->w_message,
			HU_MSGX, HU_MSGY, cl_messages.GetInteger(),
			::g->hu_font,
			HU_FONTSTART, &::g->message_on);
	}
}

//
//      Toggle messages on/off
//
void M_ChangeGPad(int choice)
{
	// warning: unused parameter `int choice'
	choice = 0;
	idLib::joystick= !idLib::joystick ;

	::g->message_dontfuckwithme = true;
}
//GK:Begin
//
//      Toggle Always Run
//
void M_Alwaysrun(int choice) {

	in_alwaysRunCl.SetBool(in_alwaysRunCl.GetBool() ? 0 : 1);
	//cmdSystem->BufferCommandText(CMD_EXEC_APPEND, "vid_restart\n");
}

void M_Freelook(int choice) 
{
	cl_freelook.SetBool(cl_freelook.GetBool() ? 0 : 1);
}

void M_Autoaim(int choice)
{
	game->SetCVarBool("aa_targetAimAssistEnable", !game->GetCVarBool("aa_targetAimAssistEnable"));
}

void M_Jump(int choice)
{
	cl_jump.SetBool(cl_jump.GetBool() ? 0 : 1);
}

void M_Cross(int choice)
{
	cl_cursor.SetBool(cl_cursor.GetBool() ? 0 : 1);
}

void M_Mapst(int choice)
{
	cl_showStats.SetBool(cl_showStats.GetBool() ? 0 : 1);
}

void M_HUD(int choice)
{
	cl_HUD.SetBool(cl_HUD.GetBool() ? 0 : 1);
}
//GK:End
//
//      Toggle Fullscreen
//
void M_FullScreen( int choice ) {
	r_fullscreen.SetInteger( r_fullscreen.GetInteger() == 0 ? 1 : r_fullscreen.GetInteger() == 1 ? -1 : 0 );
	cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "vid_restart\n" );
}

void M_Aspect(int choice) {
	bool originalAspect = r_aspectcorrect.GetBool();

	//GK: Similar to fullscreen but with the aspect parameter
	if (r_aspect.GetInteger() && !r_aspectcorrect.GetBool()) {
		r_aspectcorrect.SetBool(true);
	}
	else {
		r_aspect.SetInteger(r_aspect.GetInteger() ? 0 : 1);
		r_aspectcorrect.SetBool(false);
	}
	if (originalAspect != r_aspectcorrect.GetBool() && !::g->netgame)
	{
		reset = true;
		::g->useDemo = false;
	}
	R_Initwidth(); //GK: Restart the classic Doom renderer
	::g->reset = true;
	R_Init();
	ST_Start();
	if (::g->automapactive) {
		AM_Start();
	}
	cmdSystem->BufferCommandText(CMD_EXEC_APPEND, "vid_restart\n");
}

void M_Light(int choice) {
	r_clight.SetInteger(r_clight.GetInteger() ? r_clight.GetInteger() == 1 ? 2 : 0 : 1);
	::g->reset = true;
	R_Init(); //GK: Re-init the renderer to apply the new light mode
}

void M_Resolution(int choice) {
	M_SetupNextMenu(&::g->ResDef);
}

void M_SetRes(int choice) {
	r_vidMode.SetInteger(choice);
	r_customWidth.SetInteger(modeList[choice].width);
	r_customHeight.SetInteger(modeList[choice].height);
	//if (modeList[choice].displayHz != 60) {
		r_displayRefresh.SetInteger(modeList[choice].displayHz);
	//}
	cmdSystem->BufferCommandText(CMD_EXEC_APPEND, "vid_restart\n");
	M_SetupNextMenu(::g->currentMenu->prevMenu);
}

void M_Sync(int choice) {
	r_swapInterval.SetInteger(r_swapInterval.GetInteger() == 2 ? 0 : r_swapInterval.GetInteger() + 1);
}

void M_Framerate(int choice) {
	int oldHz = cl_engineHz.GetInteger();
	cl_engineHz.SetInteger(oldHz == 40 ? 35 : 40);
	if (oldHz > 35) {
		cl_engineHz_interp.SetBool(!cl_engineHz_interp.GetBool());
	}

	hardreset = true;
}

void M_Refresh(int choice) {
	if (!R_GetRefreshListForDisplay(r_fullscreen.GetInteger() > 1 ? r_fullscreen.GetInteger() - 1 : 0, classicRefreshList)) {
		classicRefreshList.Clear();
	}
	idList<int> clasicFrameRate = classicRefreshList;
	clasicFrameRate.AddUnique(60);
	clasicFrameRate.AddUnique(120);
	if (refreshIndex == clasicFrameRate.Num() - 1) {
		refreshIndex = 0;
	}
	else {
		refreshIndex++;
	}
	com_engineHz.SetInteger(clasicFrameRate[refreshIndex]);
	if (classicRefreshList.Find(clasicFrameRate[refreshIndex]) != NULL) {
		r_displayRefresh.SetInteger(clasicFrameRate[refreshIndex]);
	}

	hardreset = true;
}

void M_Blurry(int choice) {
	r_clblurry.SetBool(!r_clblurry.GetBool());
}

char	layNames[2][9] =
{
"M_XBOX","M_PSX"
};

void M_DrawCtl(void)
{
	V_DrawPatchDirect(58, 15, 0,/*(patch_t*)*/img2lmp(W_CacheLumpName("M_CTL", PU_CACHE_SHARED), W_GetNumForName("M_CTL")), false);
	//GK:End
	extern idCVar in_mouseSpeed;
	const int roundedMouseSpeed = M_GetMouseSpeedForMenu(in_mouseSpeed.GetFloat());
	bool rumbleOnOff = in_joystickRumble.GetBool();
	bool layoutOnOff = in_joylayout.GetBool();
	bool autorunOnOff = in_toggleRun.GetBool();

	V_DrawPatchDirect(::g->CtlDef.x + 95, ::g->CtlDef.y + LINEHEIGHT * rumble, 0,
		/*(patch_t*)*/img2lmp(W_CacheLumpName(msgNames[rumbleOnOff], PU_CACHE_SHARED), W_GetNumForName(msgNames[rumbleOnOff])), false);
	V_DrawPatchDirect(::g->CtlDef.x + 99, ::g->CtlDef.y + LINEHEIGHT * layout, 0,
		/*(patch_t*)*/img2lmp(W_CacheLumpName(layNames[layoutOnOff], PU_CACHE_SHARED), W_GetNumForName(layNames[layoutOnOff])), false);
	M_DrawThermo(::g->CtlDef.x, ::g->CtlDef.y + LINEHEIGHT * ctl_empty3, 16, roundedMouseSpeed);
	V_DrawPatchDirect(::g->CtlDef.x + 148, ::g->CtlDef.y + LINEHEIGHT * togglerun, 0,
		/*(patch_t*)*/img2lmp(W_CacheLumpName(msgNames[autorunOnOff], PU_CACHE_SHARED), W_GetNumForName(msgNames[autorunOnOff])), false);

	//M_DrawThermo(::g->OptionsDef.x,::g->OptionsDef.y+LINEHEIGHT*(scrnsize+1),
	//	9,::g->screenSize);
}

const char* M_GetPageText(int i) {
	switch (pageIndex) {
	case 0:
		return "next page";
	case 1:
	case 2:
		return i == 10 ? "next page" : "prev page";
	case 3:
		return "prev page";
	}
	return "";
}

void M_DrawKey(void) {
	V_DrawPatchDirect(108, 10, 0,/*(patch_t*)*/img2lmp(W_CacheLumpName("M_KEY", PU_CACHE_SHARED), W_GetNumForName("M_KEY")), false);
	int bindStart = 0;
	int bindEnd = 0;
	int aspect = ::g->ASPECT_IMAGE_SCALER - GLOBAL_IMAGE_SCALER;
	bindStart = pageIndex * 10;
	int endOffs = bindStart == 0 ? 1 : 2;
	bindEnd = (bindStart + 10) + endOffs;
	for (int i = bindStart; i < bindEnd; i++) {
		std::string res;
		if (i < numBinds) {
			keyBindings_t bind = idKeyInput::KeyBindingsFromBinding(keyboardBinds[i].bind, false, true);
			
			idStr bindings;

			if (!bind.gamepad.IsEmpty() && idLib::joystick)
			{
				idStrList joyBinds;
				int start = 0;
				while (start < bind.gamepad.Length())
				{
					int end = bind.gamepad.Find(", ", true, start);
					if (end < 0)
					{
						end = bind.gamepad.Length();
					}
					joyBinds.Alloc().CopyRange(bind.gamepad, start, end);
					start = end + 2;
				}
				const char* buttonsWithImages[] =
				{
					"JOY1", "JOY2", "JOY3", "JOY4", "JOY5", "JOY6",
					"JOY_DPAD_UP", "JOY_DPAD_LEFT", "JOY_DPAD_DOWN", "JOY_DPAD_RIGHT"
					"JOY_TRIGGER1", "JOY_TRIGGER2", 0
				};
				for (int i_ = 0; i_ < joyBinds.Num(); i_++)
				{
					if (joyBinds[i_].Icmpn("JOY_STICK", 9) == 0)
					{
						continue; // Can't rebind the sticks, so don't even show them
					}
					bool hasImage = false;
					int imageIndex = -1;
					for (int b = 0; b < numLayBinds; b++)
					{
						if (joyBinds[i_].Icmp(joyLayoutBinds[b].bind) == 0)
						{
							hasImage = true;
							imageIndex = b;
							break;
						}
					}
					if (!bindings.IsEmpty())
					{
						bindings.Append(", ");
					}
					if (hasImage)
					{
						bindings.Append(joyLayoutBinds[imageIndex].display);
					}
					else
					{
						bindings.Append(joyBinds[i_]);
					}
				}
			}

			if (!bind.keyboard.IsEmpty())
			{
				if (!bindings.IsEmpty())
				{
					bindings.Append(", ");
				}
				bindings.Append(bind.keyboard);
			}

			if (!bind.mouse.IsEmpty())
			{
				if (!bindings.IsEmpty())
				{
					bindings.Append(", ");
				}
				bindings.Append(bind.mouse);
			}
			bindings.ToUpper();
			res = va("%s - %s", keyboardBinds[i].display, bindings.c_str());
		}
		/*if (aspect) {
			if (i < 10) {
				M_WriteAspectText(::g->KeyDef.x, ::g->KeyDef.y + LINEHEIGHT * i, res.c_str());
			}
			else {
				M_WriteAspectText(::g->KeyDef.x + 220, ::g->KeyDef.y + LINEHEIGHT * (i - 10), res.c_str());
			}
		}
		else*/
		{
			if ( (i < ((pageIndex + 1) * 10)) || (pageIndex == 3 && (i - bindStart) == 10)) {
				M_WriteText(::g->KeyDef.x + ((i - bindStart) < 10 ? 0 : (aspect ? 220 : 0)), ::g->KeyDef.y + LINEHEIGHT * ((i - bindStart) < 10 ? (i - bindStart) : (aspect ? ((i - bindStart) - 10) : (i - bindStart))), res.c_str(), true);
			}
			else {
				M_WriteText(::g->KeyDef.x + (aspect ? 220 : 200), ::g->KeyDef.y + LINEHEIGHT * (pageIndex == 3 && !aspect ? ((i - bindStart) - 11) : ((i - bindStart) - 10)), M_GetPageText((i- bindStart)), true);
			}
		}
	}
}
//
// M_EndGame
//
void M_EndGameResponse(int ch)
{
	if (ch != KEY_ENTER)
		return;

	::g->currentMenu->lastOn = ::g->itemOn;
	M_ClearMenus ();
	D_StartTitle ();
}

void M_EndGame(int choice)
{
	choice = 0;
	if (!::g->usergame)
	{
		S_StartSound(NULL,sfx_oof);
		return;
	}
	if (::op) {
		::op->SetAdditionalInfo("status", va("%s:Title Menu", ::g->acronymPrefix));
	}
	if (::g->netgame)
	{
		M_StartMessage(NETEND,NULL,false);
		return;
	}
	if (!idLib::joystick) {
		M_StartMessage(ENDGAME, M_EndGameResponse, true);
	}
	else {
		M_StartMessage(M_UseCircleForAccept() ? ENDGAMEGPJ : ENDGAMEGP, M_EndGameResponse, true);
	}
}



//GK:Re use the Read this! menu
//
// M_ReadThis
//
void M_ReadThis(int choice)
{
	choice = 0;
	M_SetupNextMenu(&::g->ReadDef1);
}

void M_ReadThis2(int choice)
{
	choice = 0;
	M_SetupNextMenu(&::g->ReadDef2);
}

void M_FinishReadThis(int choice)
{
	choice = 0;
	if (mh == -1) {
		::g->ReadMenu1[0].routine = M_ReadThis2;
	}
	M_SetupNextMenu(&::g->MainDef);
}




//
// M_QuitDOOM
//
void M_QuitResponse(int ch)
{
	// Exceptions disabled by default on PS3
	//throw "";
}

void M_QuitDOOM(int choice)
{
	M_SetupNextMenu(&::g->QuitDef);
	//M_StartMessage("are you sure?\npress A to quit, or B to cancel",M_QuitResponse,true);
	//common->SwitchToGame( DOOM3_BFG );
}

void M_ExitGame(int choice)
{
	//CleanUncompFiles(true);
	//GK:logout properly from the netgame
	if (::g->netgame) {
		DoomLib::Interface.QuitCurrentGame();
	}
	common->Quit();
}

void M_CancelExit(int choice) {
	M_SetupNextMenu(&::g->MainDef);
}

void M_CloseGame()
{
	I_Printf("Reseting Dehacked Patches...\n");
	::g->cpind = 0;
	for (uint i = 0; i < ::g->cpatch.size(); i++) {
		free(::g->cpatch[i]);
		::g->cpatch[i] = NULL;
	}
	//GK: Make sure the other game wont start with reverb
//#ifdef USE_OPENAL
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (!common->UseAlternativeAudioAPI())
#endif
		alAuxiliaryEffectSlotiRef((ALuint)::g->clslot, AL_EFFECTSLOT_EFFECT, AL_EFFECTSLOT_NULL);
	
//#endif
	resetValues();
	//resetWeapons();
	ResetAmmo();
	//resetMapNames();
	resetEndings();
	//resetTexts();
	resetSprnames();
	ResetPars();
	ResetFinalflat();
	P_ResetAct();
	I_Printf("Reset Completed!!\n");
	memset(DoomLib::otherfiles, 0, 5 * 20);//GK:Reset this for better checking

	//CleanUncompFiles(); //GK: A good practice would have been to delete the files after
	//we change the game but W_Shutdown which must be called to free the files causes bugs and crashes
	initonce = false;
	//GK:logout properly from the netgame
	if (::g->netgame) {
		DoomLib::Interface.QuitCurrentGame();
	}
	if (in_photomode.GetBool()) {
		in_photomode.SetBool(false);
		idVec3 viewangles;
		common->GetPhotoMode()->End(&viewangles);
	}
}

void M_GameSelection(int choice)
{
	M_CloseGame();
	common->SwitchToGame( DOOM3_BFG );
}

void M_ChangeSensitivity(int choice)
{
	extern idCVar in_mouseSpeed;

	int roundedMouseSpeed = M_GetMouseSpeedForMenu( in_mouseSpeed.GetFloat() );

	switch(choice)
	{
	case 0:
		if ( roundedMouseSpeed > 0 ) {
			roundedMouseSpeed--;
		}
		break;
	case 1:
		if ( roundedMouseSpeed < 15 ) {
			roundedMouseSpeed++;
		}
		break;
	}

	const float normalizedNewMouseSpeed = roundedMouseSpeed / 15.0f;
	const float rescaledNewMouseSpeed = 0.25f + ( ( 4.0f - 0.25 ) * normalizedNewMouseSpeed );
	in_mouseSpeed.SetFloat( rescaledNewMouseSpeed );
}

void M_Ctl(int choice)
{
	M_SetupNextMenu(&::g->CtlDef);
}

void M_Key(int choice)
{
	M_SetupNextMenu(&::g->KeyDef);
}

void M_ChangeKeys(int choice) {
	//int aspect = ::g->ASPECT_IMAGE_SCALER - GLOBAL_IMAGE_SCALER;
	if (choice == 10) {
		if (pageIndex < 3) {
			pageIndex++;
			::g->itemOn = 0;
			::g->currentMenu->lastOn = 0;
			M_SetupNextMenu(&::g->KeyDef);
			return;
		}
		
	}
	if (choice == 11) {
		if (pageIndex > 0) {
			pageIndex--;
			::g->itemOn = 0;
			::g->currentMenu->lastOn = 0;
			M_SetupNextMenu(&::g->KeyDef);
			return;
		}
	}
	::g->captureBind = true;
	//if (!aspect) {
		::g->bindIndex = choice + (pageIndex * 10);
	/*}
	else
	{
		::g->bindIndex = choice;
	}*/
	char* tempMsg = new char[128];
	sprintf(tempMsg, KEYMSG, keyboardBinds[::g->bindIndex].display);
	M_StartMessage(tempMsg, NULL, false);
	
}

void M_Rumble(int choice)
{
	in_joystickRumble.SetBool(!in_joystickRumble.GetBool());
}

void M_Layout(int choice)
{
	in_joylayout.SetBool(!in_joylayout.GetBool());
	HU_UpdateGlyphs();
	if (!common->IsNewDOOM3()) {
		hardreset = true;
	}
}

void M_ToggleRun(int choice)
{
	in_toggleRun.SetBool(!in_toggleRun.GetBool());
}


void M_ChangeDetail(int choice)
{
	choice = 0;
	::g->detailLevel = 1 - ::g->detailLevel;

	// FIXME - does not work. Remove anyway?
	I_PrintfE("M_ChangeDetail: low detail mode n.a.\n");

	return;

	/*R_SetViewSize (::g->screenblocks, ::g->detailLevel);

	if (!::g->detailLevel)
	::g->players[::g->consoleplayer].message = DETAILHI;
	else
	::g->players[::g->consoleplayer].message = DETAILLO;*/
}




void M_SizeDisplay(int choice)
{
	switch(choice)
	{
	case 0:
		if (::g->screenSize > 7)
		{
			::g->screenblocks--;
			::g->screenSize--;
		}
		break;
	case 1:
		if (::g->screenSize < 8)
		{
			::g->screenblocks++;
			::g->screenSize++;
		}
		break;
	}


	R_SetViewSize (::g->screenblocks, ::g->detailLevel);
}




//
//      Menu Functions
//
void
M_DrawThermo
( int	x,
 int	y,
 int	thermWidth,
 int	thermDot )
{
	int		xx;
	int		i;

	xx = x;
	V_DrawPatchDirect (xx,y,0,/*(patch_t*)*/img2lmp(W_CacheLumpName("M_THERML",PU_CACHE_SHARED), W_GetNumForName("M_THERML")), false);
	xx += 8;
	for (i=0;i<thermWidth;i++)
	{
		V_DrawPatchDirect (xx,y,0,/*(patch_t*)*/img2lmp(W_CacheLumpName("M_THERMM",PU_CACHE_SHARED), W_GetNumForName("M_THERMM")), false);
		xx += 8;
	}
	V_DrawPatchDirect (xx,y,0,/*(patch_t*)*/img2lmp(W_CacheLumpName("M_THERMR",PU_CACHE_SHARED), W_GetNumForName("M_THERMR")), false);

	V_DrawPatchDirect ((x+8) + thermDot*8,y,
		0,/*(patch_t*)*/img2lmp(W_CacheLumpName("M_THERMO",PU_CACHE_SHARED), W_GetNumForName("M_THERMO")), false);
}



void
M_DrawEmptyCell
( menu_t*	menu,
 int		item )
{
	V_DrawPatchDirect (menu->x - 10,        menu->y+item*LINEHEIGHT - 1, 0,
		/*(patch_t*)*/img2lmp(W_CacheLumpName("M_CELL1",PU_CACHE_SHARED), W_GetNumForName("M_CELL1")), false);
}

void
M_DrawSelCell
( menu_t*	menu,
 int		item )
{
	V_DrawPatchDirect (menu->x - 10,        menu->y+item*LINEHEIGHT - 1, 0,
		/*(patch_t*)*/img2lmp(W_CacheLumpName("M_CELL2",PU_CACHE_SHARED), W_GetNumForName("M_CELL2")), false);
}


void
M_StartMessage
( const char*	string,
 messageRoutine_t routine,
 qboolean	input )
{
	::g->messageLastMenuActive = ::g->menuactive;
	::g->messageToPrint = 1;
	::g->messageString = (char *)string;
	::g->messageRoutine = (messageRoutine_t)routine;
	::g->messageNeedsInput = input;
	::g->menuactive = true;
	return;
}



void M_StopMessage(void)
{
	::g->menuactive = ::g->messageLastMenuActive;
	::g->messageToPrint = 0;
}



//
// Find string width from ::g->hu_font chars
//
int M_StringWidth(const char* string)
{
	unsigned int             i;
	int             w = 0;
	int             c;

	for (i = 0;i < strlen(string);i++)
	{
		c = toupper(string[i]) - HU_FONTSTART;
		if (c < 0 || c >= HU_FONTSIZE)
			w += 4;
		else
			w += SHORT (::g->hu_font[c]->width);
	}

	return w;
}



//
//      Find string height from ::g->hu_font chars
//
int M_StringHeight(const char* string)
{
	unsigned int             i;
	int             h;
	int             height = SHORT(::g->hu_font[0]->height);

	h = height;
	for (i = 0;i < strlen(string);i++)
		if (string[i] == '\n')
			h += height;

	return h;
}


//
//      Write a string using the ::g->hu_font
//
void
M_WriteText
( int		x,
 int		y,
 const char*	string,
 bool			aspect)
{
	int		w;
	const char*	ch;
	int		c;
	int		cx;
	int		cy;


	ch = string;
	cx = x;
	cy = y;

	while(1)
	{
		c = *ch++;
		if (!c)
			break;
		if (c == '\n')
		{
			cx = x;
			cy += 12;
			continue;
		}
		
		
		if (c == '#') {
			char joybuffer[5];
			strncpy(joybuffer, ch, 5);
			if (!idStr::Icmpn(joybuffer, "JOY", 3)) {
				char buffer[2];
				strncpy(buffer, joybuffer + 3, 2);
				int index = atoi(buffer);
				V_DrawPatchDirect(cx, cy - (::g->hu_glyph[index -1]->height / 4), 0, ::g->hu_glyph[index - 1], aspect);
				for (int k = 0; k < 5; k++) {
					c = *ch++;
				}
				cx += ::g->hu_glyph[index - 1]->width;
				continue;
			}
		}

		c = toupper(c) - HU_FONTSTART;
		if (c < 0 || c>= HU_FONTSIZE)
		{
			cx += 4;
			continue;
		}

		w = SHORT (::g->hu_font[c]->width);
		if (cx+w > ::g->SCREENWIDTH)
			break;
		V_DrawPatchDirect(cx, cy, 0, ::g->hu_font[c], aspect);
		cx+=w;
	}
}



//
// CONTROL PANEL
//
void M_ResetGame(int ch)
{
	reset = false;
	if (ch != KEY_ENTER)
		return;
	M_CloseGame();
	currentGame_t current = (currentGame_t)DoomLib::expansionSelected;
	DoomLib::SetCurrentExpansion(current);
	

}

void M_HardResetGame(int ch)
{
	hardreset = false;
	if (ch != KEY_ENTER)
		return;
	
	M_CloseGame();
	common->SwitchToGame(DOOM3_BFG, true);

}

void M_CheckReset()
{
	if (reset) {
		if (!idLib::joystick) {
			M_StartMessage(RESETGAME, M_ResetGame, true);
		}
		else {
			M_StartMessage(M_UseCircleForAccept() ? RESETGAMEGPJ : RESETGAMEGP, M_ResetGame, true);
		}
	}

	if (hardreset) {
		if (!idLib::joystick) {
			M_StartMessage(HARDRESETGAME, M_HardResetGame, true);
		}
		else {
			M_StartMessage(M_UseCircleForAccept() ? HARDRESETGAMEGPJ : HARDRESETGAMEGP, M_HardResetGame, true);
		}
	}
}

//
// M_Responder
//
qboolean M_Responder (event_t* ev)
{
	int             ch;
	int             i;

	ch = -1;

	if (ev->type == ev_joystick && ::g->joywait < I_GetTime())
	{
		if (ev->data3 == -1)
		{
			ch = KEY_UPARROW;
			::g->joywait = I_GetTime() + 5;
		}
		else if (ev->data3 == 1)
		{
			ch = KEY_DOWNARROW;
			::g->joywait = I_GetTime() + 5;
		}

		if (ev->data2 == -1)
		{
			ch = KEY_LEFTARROW;
			::g->joywait = I_GetTime() + 2;
		}
		else if (ev->data2 == 1)
		{
			ch = KEY_RIGHTARROW;
			::g->joywait = I_GetTime() + 2;
		}

		if (ev->data1 & 1)
		{
			ch = KEY_ENTER;
			::g->joywait = I_GetTime() + 5;
		}
		if (ev->data1 & 2)
		{
			ch = KEY_BACKSPACE;
			::g->joywait = I_GetTime() + 5;
		}
		if (ev->data1 & 3)
		{
			ch = 45;
			::g->joywait = I_GetTime() + 5;
		}
	
	}
	else
	{
		if (ev->type == ev_mouse && ::g->mousewait < I_GetTime())
		{
			::g->mmenu_mousey += ev->data3;
			if (::g->mmenu_mousey < ::g->lasty-30)
			{
				ch = KEY_DOWNARROW;
				::g->mousewait = I_GetTime() + 5;
				::g->mmenu_mousey = ::g->lasty -= 30;
			}
			else if (::g->mmenu_mousey > ::g->lasty+30)
			{
				ch = KEY_UPARROW;
				::g->mousewait = I_GetTime() + 5;
				::g->mmenu_mousey = ::g->lasty += 30;
			}

			::g->mmenu_mousex += ev->data2;
			if (::g->mmenu_mousex < ::g->lastx-30)
			{
				ch = KEY_LEFTARROW;
				::g->mousewait = I_GetTime() + 5;
				::g->mmenu_mousex = ::g->lastx -= 30;
			}
			else if (::g->mmenu_mousex > ::g->lastx+30)
			{
				ch = KEY_RIGHTARROW;
				::g->mousewait = I_GetTime() + 5;
				::g->mmenu_mousex = ::g->lastx += 30;
			}

			if (ev->data1&1)
			{
				ch = KEY_ENTER;
				::g->mousewait = I_GetTime() + 15;
			}

			if (ev->data1&2)
			{
				ch = KEY_BACKSPACE;
				::g->mousewait = I_GetTime() + 15;
			}
		} else 
			if (ev->type == ev_keydown)
			{
				ch = ev->data1;
				if (idLib::joystick) {
					if (::g->currentMenu == &::g->LoadDef) {
						if (ch == KEY_TAB) {
							ch = KEY_X;
						}
					}
					if (M_UseCircleForAccept()) {
						if (ch == KEY_ENTER) {
							ch = KEY_BACKSPACE;
						}
						else if (ch == KEY_BACKSPACE) {
							ch = KEY_ENTER;
						}
					}
				}
				if (::g->gamemode == commercial && (::g->gamestate == GS_DEMOSCREEN || ::g->demoplayback)) { //GK: Make sure it works ONLY on the main menu
					//GK begin
					//GK: Using an improved cheat system that no longer relies on time limits
						::g->cheat[::g->cheatind] = ev->data1;
						::g->cheatind++;
						::g->markfordelete = 0;
						if (cht_CheckCheat(cheat_menu_seq, ::g->cheat, ::g->cheatind, ::g->markfordelete, true)) {
							::g->markfordelete = 0;
							for (int o = 0; o < 3; o++) {
								if (buf[o] == 11) { //0=11 so set it to 1
									buf[o] = 1;
								}
							}

							int expm = buf[1] - 1;
							M_ChangeMenuExp(expm);
						}
					if (!::g->markfordelete || ::g->cheatind>=14) {
						for (int u = 0; u < 14; u++) {
							::g->cheat[u] = '\0';
						}
						::g->cheatind = 0;
					}
				}
				
			}
	}

	if (ch == -1)
		return false;


	// Save Game string input
	if (::g->saveStringEnter)
	{
		switch(ch)
		{
		case KEY_BACKSPACE:
			if (::g->saveCharIndex > 0)
			{
				::g->saveCharIndex--;
				::g->savegamestrings[::g->saveSlot][::g->saveCharIndex] = 0;
			}
			break;

		case KEY_ESCAPE:
			::g->saveStringEnter = 0;
			strcpy(&::g->savegamestrings[::g->saveSlot][0],::g->saveOldString);
			break;

		case KEY_ENTER:
			::g->saveStringEnter = 0;
			if (::g->savegamestrings[::g->saveSlot][0])
				M_DoSave(::g->saveSlot);
			break;

		default:
			ch = toupper(ch);
			if (ch != 32)
				if (ch-HU_FONTSTART < 0 || ch-HU_FONTSTART >= HU_FONTSIZE)
					break;
			if (ch >= 32 && ch <= 127 &&
				::g->saveCharIndex < SAVESTRINGSIZE-1 &&
				M_StringWidth(::g->savegamestrings[::g->saveSlot]) <
				(SAVESTRINGSIZE-2)*8)
			{
				::g->savegamestrings[::g->saveSlot][::g->saveCharIndex++] = ch;
				::g->savegamestrings[::g->saveSlot][::g->saveCharIndex] = 0;
			}
			break;
		}
		return true;
	}

	// Take care of any messages that need input
	if (::g->messageToPrint)
	{
		if (::g->messageNeedsInput == true &&
			!(ch == KEY_ENTER || ch == KEY_BACKSPACE || ch == KEY_ESCAPE))
			return false;

		::g->menuactive = ::g->messageLastMenuActive;
		::g->messageToPrint = 0;
		if (::g->messageRoutine)
			::g->messageRoutine(ch);

		S_StartSound(NULL,sfx_swtchx);
		return true;
	}
/*
	if (::g->devparm && ch == KEY_F1)
	{
		G_ScreenShot ();
		return true;
	}*/
	//GK: Restored (It was quite easy)
	// F-Keys
	if (!::g->menuactive && !idLib::joystick) //GK: Apparently controllers hate the Function Keys
		switch(ch)
	{
		case KEY_MINUS:         // Screen size down
			if (::g->automapactive || ::g->chat_on || in_photomode.GetBool())
				return false;
			M_SizeDisplay(0);
			//M_Aspect(0); //GK: Screen size doesn't work
			S_StartSound(NULL,sfx_stnmov);
			return true;

		case KEY_EQUALS:        // Screen size up
			if (::g->automapactive || ::g->chat_on || in_photomode.GetBool())
				return false;
			M_SizeDisplay(1);
			//M_Aspect(1); //GK: It doesn't circle through
			S_StartSound(NULL,sfx_stnmov);
			return true;

		case KEY_F1:            // Help key
			M_StartControlPanel ();
			M_ReadThis(0); //GK: otherwise it doesn't work
			//if ( ::g->gamemode == retail )
				//::g->currentMenu = &::g->ReadDef2;

			//else
			//	::g->currentMenu = &::g->ReadDef1;

			//::g->itemOn = 0;
			S_StartSound(NULL,sfx_swtchn);
			return true;

		case KEY_F2:            // Save
			M_StartControlPanel();
			S_StartSound(NULL,sfx_swtchn);
			M_SaveGame(0);
			return true;

		case KEY_F3:            // Load
			M_StartControlPanel();
			S_StartSound(NULL,sfx_swtchn);
			M_LoadGame(0);
			return true;

		case KEY_F4:            // Sound Volume
			M_StartControlPanel ();
			::g->currentMenu = &::g->SoundDef;
			::g->itemOn = sfx_vol;
			S_StartSound(NULL,sfx_swtchn);
			return true;

		case KEY_F5:            // Detail toggle
			//M_ChangeDetail(0);
			M_Light(0); //GK: You don't want to mess with the detail (looking you with my evil eye)
			S_StartSound(NULL,sfx_swtchn);
			return true;

		case KEY_F6:            // Quicksave
			S_StartSound(NULL,sfx_swtchn);
			M_QuickSave();
			return true;

		case KEY_F7:            // End game
			S_StartSound(NULL,sfx_swtchn);
			M_EndGame(0);
			return true;

		case KEY_F8:            // Toggle messages
			M_ChangeMessages(1);
			S_StartSound(NULL,sfx_swtchn);
			return true;

		case KEY_F9:            // Quickload
			S_StartSound(NULL,sfx_swtchn);
			M_QuickLoad();
			return true;

		case KEY_F10:           // Quit DOOM
			M_StartControlPanel(); //GK: without it it doesn't show
			S_StartSound(NULL,sfx_swtchn);
			M_QuitDOOM(0);
			return true;

		case KEY_F11:           // gamma toggle
			::g->usegamma++;
			if (::g->usegamma > 4)
				::g->usegamma = 0;
			::g->players[::g->consoleplayer].message = gammamsg[::g->usegamma];
			I_SetPalette ((byte*)W_CacheLumpName ("PLAYPAL",PU_CACHE_SHARED),W_LumpLength(W_GetNumForName("PLAYPAL")));
			return true;
		//case KEY_F12: //GK: Why not
		//	M_Alwaysrun(0);
		//	S_StartSound(NULL, sfx_swtchn);
		//	return true;
	}


	// Pop-up menu?
	if (!::g->menuactive)
	{
		//GK : Re-enable pop up main menu
		if (ch == KEY_ESCAPE /*&& ( ::g->gamestate == GS_LEVEL || ::g->gamestate == GS_INTERMISSION || ::g->gamestate == GS_FINALE  )*/ )
		{
			M_StartControlPanel ();

			S_StartSound(NULL,sfx_swtchn);
			return true;
		}
		
		return false;
	}

	if (::g->currentMenu == &::g->LoadDef) {
		if (ch == 45) {
			idFile* handle = fileSystem->OpenFileRead(::g->savegamepaths[::g->itemOn], false);
			if (handle != NULL) {
				fileSystem->CloseFile(handle);
				if (!idLib::joystick) {
					M_StartMessage(DELSAV, M_DeleteSelected, true);
				}
				else {
					M_StartMessage(M_UseCircleForAccept() ? DELSAVGPJ : DELSAVGP, M_DeleteSelected, true);
				}
			}
		}
	}
	// Keys usable within menu
	//int aspect = ::g->ASPECT_IMAGE_SCALER - GLOBAL_IMAGE_SCALER;
	switch (ch)
	{
	case KEY_DOWNARROW:
		do
		{
			if (::g->itemOn + 1 > ::g->currentMenu->numitems - 1) {
				::g->itemOn = 0;
			} else
				
				if (::g->currentMenu->menuitems == ::g->ResDef.menuitems && ::g->itemOn + 1 >= modeSize) {
					::g->itemOn = 0;
				}
				else if (::g->currentMenu->menuitems == ::g->KeyDef.menuitems && ::g->itemOn + 1 >= (pageIndex == 0 ? 11 : 12)) {
					::g->itemOn = 0;
				}
				else if (::g->currentMenu->menuitems == ::g->KeyDef.menuitems &&  pageIndex == 3 && ::g->itemOn + 1 >= 2) {
					::g->itemOn += 10;
				}
				else if (::g->currentMenu == &::g->VideoDef && !cl_engineHz_interp.GetBool() && ::g->itemOn == vsync) {
					::g->itemOn = vsync + 2;
				}
				else if (::g->currentMenu == &::g->GameDef && !cl_freelook.GetBool() && ::g->itemOn == look) {
					::g->itemOn = jump;
				}
			else ::g->itemOn++;
			S_StartSound(NULL,sfx_pstop);
		} while(::g->currentMenu->menuitems[::g->itemOn].status==-1);
		return true;

	case KEY_UPARROW:
		do
		{
			if (!::g->itemOn)
				if (::g->currentMenu->menuitems == ::g->ResDef.menuitems)
					::g->itemOn = modeSize - 1;
				else if (::g->currentMenu->menuitems == ::g->KeyDef.menuitems)
					::g->itemOn = pageIndex == 0 ? 10 : 11;
				else ::g->itemOn = ::g->currentMenu->numitems-1;
			else if (::g->currentMenu == &::g->VideoDef && !cl_engineHz_interp.GetBool() && ::g->itemOn == framerate)
				::g->itemOn = framerate - 2;
			else if (::g->currentMenu == &::g->GameDef && !cl_freelook.GetBool() && ::g->itemOn == jump)
				::g->itemOn = look;
			else if (::g->currentMenu->menuitems == ::g->KeyDef.menuitems && pageIndex == 3 && ::g->itemOn == 11) {
				::g->itemOn -= 10;
			}
			else ::g->itemOn--;
			S_StartSound(NULL,sfx_pstop);
		} while(::g->currentMenu->menuitems[::g->itemOn].status==-1);
		return true;

	case KEY_LEFTARROW:
		if (::g->currentMenu->menuitems[::g->itemOn].routine &&
			::g->currentMenu->menuitems[::g->itemOn].status == 2)
		{
			S_StartSound(NULL,sfx_stnmov);
			::g->currentMenu->menuitems[::g->itemOn].routine(0);
		}
		return true;

	case KEY_RIGHTARROW:
		if (::g->currentMenu->menuitems[::g->itemOn].routine &&
			::g->currentMenu->menuitems[::g->itemOn].status == 2)
		{
			S_StartSound(NULL,sfx_stnmov);
			::g->currentMenu->menuitems[::g->itemOn].routine(1);
		}
		return true;

	case KEY_ENTER:
		if (::g->currentMenu->menuitems[::g->itemOn].routine &&
			::g->currentMenu->menuitems[::g->itemOn].status)
		{
			::g->currentMenu->lastOn = ::g->itemOn;
			if (::g->currentMenu->menuitems[::g->itemOn].status == 2)
			{
				::g->currentMenu->menuitems[::g->itemOn].routine(1);      // right arrow
				S_StartSound(NULL,sfx_stnmov);
			}
			else
			{
				::g->currentMenu->menuitems[::g->itemOn].routine(::g->itemOn);
				S_StartSound(NULL,sfx_pistol);
			}
		}
		return true;

	case KEY_ESCAPE:
		//GK : Re-enable pop up main menu
		::g->currentMenu->lastOn = ::g->itemOn;
		M_CheckReset();
		M_ClearMenus();
		S_StartSound(NULL, sfx_swtchn);
		return true;

	case KEY_BACKSPACE:
		::g->currentMenu->lastOn = ::g->itemOn;
		M_CheckReset();
		if (::g->currentMenu->prevMenu)
		{
			::g->currentMenu = ::g->currentMenu->prevMenu;
			::g->itemOn = ::g->currentMenu->lastOn;
			S_StartSound(NULL,sfx_swtchn);
		} else if ( ::g->currentMenu == &::g->MainDef && ( !::g->demoplayback && ::g->gamestate != GS_DEMOSCREEN ) ) {
			M_ClearMenus();
			::g->paused = false;
		}
		return true;

	default:
		for (i = ::g->itemOn+1;i < ::g->currentMenu->numitems;i++)
			if (::g->currentMenu->menuitems[i].alphaKey == ch)
			{
				::g->itemOn = i;
				S_StartSound(NULL,sfx_pstop);
				return true;
			}
			for (i = 0;i <= ::g->itemOn;i++)
				if (::g->currentMenu->menuitems[i].alphaKey == ch)
				{
					::g->itemOn = i;
					S_StartSound(NULL,sfx_pstop);
					return true;
				}
				break;

	}

	return false;
}



//
// M_StartControlPanel
//
void M_StartControlPanel (void)
{
	// intro might call this repeatedly
	if (::g->menuactive)
		return;

	::g->menuactive = 1;
	::g->currentMenu = &::g->MainDef;
	::g->itemOn = ::g->currentMenu->lastOn;
}


//
// M_Drawer
// Called after the view has been rendered,
// but before it has been blitted.
//
void M_Drawer (void)
{
	unsigned short		i;
	short		max;
	char		string[120];
	int			start;

	::g->inhelpscreens = false;


	// Horiz. & Vertically center string and print it.
	if (::g->messageToPrint)
	{
		start = 0;
		::g->md_y = 100 - M_StringHeight(::g->messageString)/2;
		while(*(::g->messageString+start))
		{
			for (i = 0;i < strlen(::g->messageString+start);i++)
				if (*(::g->messageString+start+i) == '\n')
				{
					memset(string,0,120);
					strncpy(string,::g->messageString+start,i);
					start += i+1;
					break;
				}

				if (i == strlen(::g->messageString+start))
				{
					strcpy(string,::g->messageString+start);
					start += i;
				}

				::g->md_x = 160 - M_StringWidth(string)/2;
				M_WriteText(::g->md_x,::g->md_y,string, false);
				::g->md_y += SHORT(::g->hu_font[0]->height);
		}
		return;
	}


	if (!::g->menuactive)
		return;

	if (::g->currentMenu->routine)
		::g->currentMenu->routine();         // call Draw routine
	int aspect = ::g->ASPECT_IMAGE_SCALER - GLOBAL_IMAGE_SCALER;
	// DRAW MENU
	::g->md_x = ::g->currentMenu->x;
	::g->md_y = ::g->currentMenu->y;
	if (::g->currentMenu->menuitems == ::g->QuitDef.menuitems && common->IsNewDOOM3()) {
		::g->currentMenu->numitems = 2;
	}
	if (::g->currentMenu->menuitems == ::g->ResDef.menuitems && ::g->itemOn >= 10) {
		::g->md_x = ::g->currentMenu->x + 120;
	}
	if (::g->currentMenu->menuitems == ::g->KeyDef.menuitems && ::g->itemOn >= 10 ) {
		if (!aspect) 
		{
			::g->md_x = ::g->currentMenu->x + 200;
		}
		else 
		{
			::g->md_x = ::g->currentMenu->x + 165;
		}
		if (pageIndex == 3 && !aspect && ::g->itemOn == 10) {
			::g->md_x = ::g->currentMenu->x;
		}
	}
	max = ::g->currentMenu->numitems;
	

	for (i=0;i<max;i++)
	{
		if (::g->currentMenu == &::g->VideoDef && !cl_engineHz_interp.GetBool() && i == refresh) {
			 continue;
		}
		if (::g->currentMenu == &::g->GameDef && !cl_freelook.GetBool() && i == aim) {
			continue;
		}
		if (::g->currentMenu->menuitems[i].name[0])
			V_DrawPatchDirect (::g->md_x,::g->md_y,0,
				/*(patch_t*)*/img2lmp(W_CacheLumpName(::g->currentMenu->menuitems[i].name ,PU_CACHE_SHARED), W_GetNumForName(::g->currentMenu->menuitems[i].name)), false);
		if (::g->currentMenu == &::g->OptionsDef && i == messages) { //GK: This is where the real dynamic menu rendering happens
			::g->md_y += LINEHEIGHT+(optoffs*LINEHEIGHT);
		}
		else {
			::g->md_y += LINEHEIGHT;
		}
	}
	//GK: Remove the skull while showing the Read This!
	if (!::g->inhelpscreens) {
		// DRAW SKULL
		int lineoffs = ::g->itemOn*LINEHEIGHT;
		if (::g->currentMenu == &::g->OptionsDef && ::g->itemOn > messages) {
			lineoffs += (optoffs*LINEHEIGHT);
		}
		if (::g->currentMenu == &::g->VideoDef && !cl_engineHz_interp.GetBool() && ::g->itemOn > refresh) {
			lineoffs = (::g->itemOn - 1) * LINEHEIGHT;
		}
		if (::g->currentMenu == &::g->GameDef && !cl_freelook.GetBool() && ::g->itemOn > aim) {
			lineoffs = (::g->itemOn - 1) * LINEHEIGHT;
		}
		if ((::g->currentMenu->menuitems == ::g->ResDef.menuitems || ::g->currentMenu->menuitems == ::g->KeyDef.menuitems) && ::g->itemOn >= 10) {
			lineoffs = LINEHEIGHT * (::g->itemOn - 10);
			if (pageIndex == 3 && !aspect ) {
				if (::g->itemOn == 10) {
					lineoffs = ::g->itemOn * LINEHEIGHT;
				}
				else if (::g->itemOn == 11) {
					lineoffs = LINEHEIGHT * (::g->itemOn - 11);
				}
			}
			
		}
		V_DrawPatchDirect(::g->md_x + SKULLXOFF, ::g->currentMenu->y - 5 + lineoffs, 0,
			/*(patch_t*)*/img2lmp(W_CacheLumpName(skullName[::g->whichSkull], PU_CACHE_SHARED), W_GetNumForName(skullName[::g->whichSkull])), false);

		if (idLib::joystick) {
			if (M_UseCircleForAccept()) {
				M_WriteText(ORIGINAL_WIDTH / 4 + 10, ORIGINAL_HEIGHT - 10, "#JOY02 - Select     #JOY01 - Back", false);
			}
			else {
				M_WriteText(ORIGINAL_WIDTH / 4 + 10, ORIGINAL_HEIGHT - 10, "#JOY01 - Select     #JOY02 - Back", false);
			}
		}

	}

	
}


//
// M_ClearMenus
//
void M_ClearMenus (void)
{
	::g->menuactive = 0;
	// if (!::g->netgame && ::g->usergame && ::g->paused)
	//       ::g->sendpause = true;
}






//
// M_SetupNextMenu
//
void M_SetupNextMenu(menu_t *menudef)
{
	::g->currentMenu = menudef;
	::g->itemOn = ::g->currentMenu->lastOn;
}


//
// M_Ticker
//
void M_Ticker (void)
{
	if (--::g->skullAnimCounter <= 0)
	{
		::g->whichSkull ^= 1;
		::g->skullAnimCounter = 8;
	}
}


//
// M_Init
//
void M_Init (void)
{	

	::g->currentMenu = &::g->MainDef;
	::g->menuactive = 0;
	::g->itemOn = ::g->currentMenu->lastOn;
	::g->whichSkull = 0;
	::g->skullAnimCounter = 10;
	::g->screenSize = ::g->screenblocks - 3;
	::g->messageToPrint = 0;
	::g->messageString = NULL;
	::g->messageLastMenuActive = ::g->menuactive;
	::g->quickSaveSlot = -1;

	// Here we could catch other version dependencies,
	//  like HELP1/2, and four episodes.


	switch ( ::g->gamemode )
	{
	case commercial:
		// This is used because DOOM 2 had only one HELP
		//  page. I use CREDIT as second page now, but
		//  kept this hack for educational purposes.
		//::g->MainMenu[readthis] = ::g->MainMenu[quitdoom];
		//::g->MainDef.numitems--;

		::g->MainDef.y += 8;
		::g->NewDef.prevMenu = &::g->MainDef;
		//::g->ReadDef1.routine = M_DrawReadThis1;
		//::g->ReadDef1.x = 330;
		//::g->ReadDef1.y = 165;
		//::g->ReadMenu1[0].routine = M_FinishReadThis;
		break;
	case shareware:
		// Episode 2 and 3 are handled,
		//  branching to an ad screen.
	case registered:
		// We need to remove the fourth episode.
		::g->EpiDef.numitems--;
		break;
	case retail:
		// We are fine.
	default:
		break;
	}
}

void M_RemapConfirm(int ch) {
	if (ch != KEY_ENTER) {
		M_StopMessage();
		M_SetupNextMenu(&::g->KeyDef);
		return;
	}
	idKeyInput::SetBinding(newmap, "");
	idKeyInput::SetBinding(newmap, keyboardBinds[::g->bindIndex].bind);
	::g->captureBind = false;
	::g->bindIndex = -1;
	M_StopMessage();
	M_SetupNextMenu(&::g->KeyDef);
}

void M_Remap(event_t* ev) {
	
	newmap = -1;
	if (ev->data1 == K_ESCAPE) {
		::g->captureBind = false;
		M_StopMessage();
		M_SetupNextMenu(&::g->KeyDef);
	}
	else if (ev->data1 != K_MINUS && ev->data1 != K_EQUALS && ev->data1 != K_F12 && (ev->data1 < K_JOY_STICK1_UP || ev->data1 > K_JOY_STICK2_RIGHT))
	{
		::g->captureBind = false;
		if (idStr::Icmp(idKeyInput::GetBinding(ev->data1), "") == 0)  	// no existing binding found
		{
			idKeyInput::SetBinding(ev->data1, keyboardBinds[::g->bindIndex].bind);

		}
		else  	// binding found prompt to change
		{

			const char* curBind = idKeyInput::GetBinding(ev->data1);

			if (idStr::Icmp(keyboardBinds[::g->bindIndex].bind, curBind) == 0)
			{

				idKeyInput::SetBinding(ev->data1, "");
			}
			else
			{
				newmap = ev->data1;
				idStr bindName = idKeyInput::LocalizedKeyName(static_cast<keyNum_t>(ev->data1));
				for (int b = 0; b < numLayBinds; b++)
				{
					if (bindName.Icmp(joyLayoutBinds[b].bind) == 0)
					{
						bindName = joyLayoutBinds[b].display;
						break;
					}
				}
				char* tempMsg = new char[128];
				if (!idLib::joystick) {
					sprintf(tempMsg, REMAPKEY, bindName.c_str(), keyboardBinds[::g->bindIndex].display);
					
				}
				else {
					sprintf(tempMsg, M_UseCircleForAccept() ? REMAPKEYGPJ : REMAPKEYGP, bindName.c_str(), keyboardBinds[::g->bindIndex].display);
				}
				M_StartMessage(tempMsg, M_RemapConfirm, true);
			}
		}
	}
	else {
		newmap = 0;
	}
	
	if (newmap < 0) {
		::g->bindIndex = -1;
		M_StopMessage();
		M_SetupNextMenu(&::g->KeyDef);
	}


}

void M_ChangeMenuExp(int exp) {
	if (exp > 0 && exp < 6) {
		if (exp > 1) {
			switch (exp) {
			case 2:
				if (DoomLib::hexp[0]) {
					DoomLib::SetCurrentExpansion(exp);
				}
				break;
			case 3:
				if (DoomLib::hexp[1]) {
					DoomLib::SetCurrentExpansion(exp);
				}
				break;
			case 4:
				if (DoomLib::hexp[2]) {
					DoomLib::SetCurrentExpansion(exp);
				}
				break;
			case 5:
				if (DoomLib::hexp[3]) {
					DoomLib::SetCurrentExpansion(exp);
				}
				break;
			}
		}
		else {
			DoomLib::SetCurrentExpansion(exp);
		}
	}
}

//GK: Direct copy from idSWF
bool M_UseCircleForAccept()
{
	int joynum = in_joylayout.GetInteger() + 1;
	if (joynum == 2 || (common->IsNewDOOM3() && joynum == 4)) {
		return in_joyjpn.GetBool();
	}
	else if (common->IsNewDOOM3() && joynum == 5) {
		return true;
	}
	return false;
}
