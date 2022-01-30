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
#pragma hdrstop
#include "precompiled.h"

#include "achievements.h"
#include "../sys_session_local.h"

extern idCVar achievements_Verbose;

#define STEAM_ACHIEVEMENT_PREFIX		"ach_"

/*
========================
idAchievementSystemWin::idAchievementSystemWin
========================
*/
idAchievementSystemWin::idAchievementSystemWin()
{
}

/*
========================
idAchievementSystemWin::IsInitialized
========================
*/
bool idAchievementSystemWin::IsInitialized()
{
	if (::op) {
		return true;
	}
	return false;
}

/*
================================
idAchievementSystemWin::AchievementUnlock
================================
*/
void idAchievementSystemWin::AchievementUnlock( idLocalUser* user, int achievementID )
{
	if (::op) {
		::op->openAchievement()->UnlockAchievement(va("%s%d", STEAM_ACHIEVEMENT_PREFIX, achievementID));
	}
}

/*
========================
idAchievementSystemWin::AchievementLock
========================
*/
void idAchievementSystemWin::AchievementLock( idLocalUser* user, const int achievementID )
{
	if (::op) {
		::op->openAchievement()->LockAchievement(va("%s%d",STEAM_ACHIEVEMENT_PREFIX,achievementID));
	}
}

/*
========================
idAchievementSystemWin::AchievementLockAll
========================
*/
void idAchievementSystemWin::AchievementLockAll( idLocalUser* user, const int maxId )
{
	if (::op) {
		for (int i = 1; i < maxId; i++) {
			::op->openAchievement()->LockAchievement(va("%s%d", STEAM_ACHIEVEMENT_PREFIX, i));
		}
	}
}

/*
========================
idAchievementSystemWin::GetAchievementDescription
========================
*/
bool idAchievementSystemWin::GetAchievementDescription( idLocalUser* user, const int achievementID, achievementDescription_t& data ) const
{
	if (::op) {
		strcpy(data.name, ::op->openAchievement()->GetAchievementName(va("%s%d", STEAM_ACHIEVEMENT_PREFIX, achievementID)));
		strcpy(data.description, ::op->openAchievement()->GetAchievementDescription(va("%s%d", STEAM_ACHIEVEMENT_PREFIX, achievementID)));
		data.hidden = ::op->openAchievement()->GetAchievementHidden(va("%s%d", STEAM_ACHIEVEMENT_PREFIX, achievementID));
		return true;
	}
	return false;
}

/*
========================
idAchievementSystemWin::GetAchievementState
========================
*/
bool idAchievementSystemWin::GetAchievementState( idLocalUser* user, idArray< bool, idAchievementSystem::MAX_ACHIEVEMENTS >& achievements ) const
{
	if (::op) {
		for (int i = 1; i < achievements.Num(); i++) {
			if (!::op->openAchievement()->GetAchievement(va("%s%d", STEAM_ACHIEVEMENT_PREFIX, i), &achievements[i])) {
				return false;
			}
		}
		return true;
	}
	return false;
}

/*
================================
idAchievementSystemWin::Pump
================================
*/
void idAchievementSystemWin::Pump()
{
}


void idAchievementSystemWin::ShowAchievementProgress(const int achievementID, int progress, int max) {
	if (::op) {
		::op->openAchievement()->GetAchievementPercent(va("%s%d", STEAM_ACHIEVEMENT_PREFIX, achievementID), progress, max);
	}
}