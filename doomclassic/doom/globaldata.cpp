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
#include "globaldata.h"
#include "Main.h"

//
// PROTOTYPES
//
void M_Dev(int choice);
void M_NewGame(int choice);
void M_Episode(int choice);
void M_Expansion(int choice);
void M_ChooseSkill(int choice);
void M_LoadGame(int choice);
void M_LoadExpansion(int choice);
void M_SaveGame(int choice);
void M_Options(int choice);
void M_EndGame(int choice);
void M_MasterSelect(int choice);
void M_Doom_IT(int choice);
void M_ReadThis(int choice);
void M_ReadThis2(int choice);
void M_QuitDOOM(int choice);
void M_ExitGame(int choice);
void M_GameSelection(int choice);
void M_CancelExit(int choice);
void M_StartDev(int choice);

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
void M_QuickSave(void);
void M_QuickLoad(void);

void M_DrawDev(void);
void M_DrawMainDev(void);
void M_DrawMainMenu(void);
void M_DrawQuit(void);
void M_DrawReadThis1(void);
void M_DrawReadThis2(void);
void M_DrawNewGame(void);
void M_DrawEpisode(void);
void M_DrawExpansion(void);
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
void M_StartMessage(const char *string,messageRoutine_t routine,qboolean input);
void M_StopMessage(void);
void M_ClearMenus (void);

bool M_True(int index);
bool M_CheckVideoSettings(int index);
bool M_CheckGameSettings(int index);
bool M_CheckAvailableGames(int index);
bool M_CheckExpansions(int index);
bool M_CheckEpisodes(int index);

extern const anim_t temp_epsd0animinfo[10];
extern const anim_t temp_epsd1animinfo[9];
extern const anim_t temp_epsd2animinfo[6];
extern const char* const temp_chat_macros[];

void Globals::InitGlobals()
{
#include "constructs.h"
}

Globals *g;

void* GetClassicDoomData()
{
	return DoomLib::GetGlobalData(0);
}