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

#include "precompiled.h"
#pragma hdrstop
#include "../Game_local.h"
#ifdef USE_OPENXR
#include "../renderer/OpenXR/XRCommon.h"
#endif
//extern idCVar pm_stamina;

//extern idCVar flashlight_batteryDrainTimeMS;

extern idCVar in_alwaysRun;
extern idCVar com_debugCaptions;

/*
========================
idMenuScreen_HUD::Initialize
========================
*/
void idMenuScreen_HUDLocal::Initialize( idMenuHandler* data )
{
	idMenuScreen::Initialize( data );
}

/*
========================
idMenuScreen_HUD::ShowScreen
========================
*/
void idMenuScreen_HUDLocal::ShowScreen( const mainMenuTransition_t transitionType )
{
	if( menuData != NULL )
	{
		menuGUI = menuData->GetGUI();
	}
	
	if( menuGUI == NULL )
	{
		return;
	}

	float xOffset = 0.0f;
	float yOffset = 0.0f;
	float topXOffset = 0.0f;
	float topYOffset = 0.0f;
#ifdef USE_OPENXR
	if (renderSystem->GetStereo3DMode() == STEREO3D_VR) {
		xOffset = xrSystem->GetWidth() / 9.0f;
		yOffset = (xrSystem->GetHeight() / 10.0f) * -1.0f;
		topXOffset = xrSystem->GetWidth() / 11.0f;
		topYOffset = (xrSystem->GetHeight() / 26.0f) * -1.0f;
	}
#endif
	idSWFScriptObject& root = menuGUI->GetRootObject();
	playerInfo = root.GetNestedObj( "_bottomLeft", "playerInfo", "info" );
	playerInfo->GetSprite()->SetXPos(playerInfo->GetSprite()->GetXPos() + xOffset);
	playerInfo->GetSprite()->parent->SetYPos(playerInfo->GetSprite()->parent->GetYPos() + yOffset);
	stamina = root.GetNestedObj( "_bottomLeft", "stamina" );
	stamina->GetSprite()->SetXPos(stamina->GetSprite()->GetXPos() + xOffset);
	stamina->GetSprite()->SetYPos(stamina->GetSprite()->GetYPos() + yOffset);
	location = root.GetNestedSprite("_bottomLeft", "location");
	locationName = root.GetNestedText( "_bottomLeft", "location", "txtVal" );
	location->SetXPos(location->GetXPos() + xOffset);
	location->SetYPos((location->GetYPos() + yOffset));
	if (renderSystem->GetStereo3DMode() == STEREO3D_VR) {
		location->SetXPos(location->GetXPos() - 190);
		location->SetYPos(location->GetYPos() + 15);
	}
	tipInfo = root.GetNestedObj( "_left", "tip" );
	tipInfo->GetSprite()->SetXPos(tipInfo->GetSprite()->GetXPos() + topXOffset);
	tipInfo->GetSprite()->SetYPos(tipInfo->GetSprite()->GetYPos() - topYOffset);
	
	if( playerInfo )
	{
		healthBorder = playerInfo->GetNestedSprite( "healthBorder", "damage" );
		healthPulse = playerInfo->GetNestedSprite( "healthBorder", "pulse" );
		armorFrame = playerInfo->GetNestedSprite( "armorFrame" );
	}
	
	// Security Update
	security = root.GetNestedSprite( "_center", "security" );
	security->SetYPos(security->GetYPos() + yOffset);
	securityText = root.GetNestedText( "_center", "security", "info", "txtVal" );

	// Subtitles Update
	if (game->GetGameType() == GAME_SP) {
		subtitles = root.GetNestedSprite("_center", "respawnMessage");
		subtitlesText = root.GetNestedText("_center", "respawnMessage", "info", "txtMessage");
		subtitles->SetYPos(subtitles->GetYPos() + 300.0f);
		subtitles->SetXPos(subtitles->GetXPos() - 100.0f);
		subtitlesText->editText->bounds.br.y = 100.0f;
		subtitlesText->editText->bounds.br.x = 600.0f;
	}
	
	// PDA Download
	newPDADownload = root.GetNestedSprite( "_center", "pdaDownload" );
	newPDADownload->SetYPos(newPDADownload->GetYPos() + yOffset);
	newPDAName = root.GetNestedText( "_center", "pdaDownload", "info", "txtName" );
	newPDAHeading = root.GetNestedText( "_center", "pdaDownload", "info", "txtHeading" );
	newPDA = root.GetNestedSprite( "_bottomLeft", "newPDA" );
	newPDA->SetXPos(newPDA->GetXPos() + xOffset);
	newPDA->SetYPos(newPDA->GetYPos() + yOffset);
	
	// Video Download
	newVideoDownload = root.GetNestedSprite( "_center", "videoDownload" );
	newVideoDownload->SetYPos(newVideoDownload->GetYPos() + yOffset);
	newVideoHeading = root.GetNestedText( "_center", "videoDownload", "info", "txtHeading" );
	newVideo = root.GetNestedSprite( "_bottomLeft", "newVideo" );
	newVideo->SetXPos(newVideo->GetXPos() + xOffset);
	newVideo->SetYPos(newVideo->GetYPos() + yOffset);
	
	// Audio Log
	audioLog = root.GetNestedSprite( "_bottomLeft", "audioLog" );
	audioLog->SetXPos(audioLog->GetXPos() + xOffset);
	audioLog->SetYPos(audioLog->GetYPos() + yOffset);
	
	// Radio Communication
	communication = root.GetNestedSprite( "_bottomLeft", "communication" );
	communication->SetXPos(communication->GetXPos() + xOffset);
	communication->SetYPos(communication->GetYPos() + yOffset);
	
	// Oxygen
	oxygen = root.GetNestedSprite( "_bottomLeft", "oxygen" );
	oxygen->SetXPos(oxygen->GetXPos() + xOffset);
	oxygen->SetYPos(oxygen->GetYPos() + yOffset);
	flashlight = root.GetNestedSprite( "_bottomLeft", "flashlight" );
	flashlight->SetXPos(flashlight->GetXPos() + xOffset);
	flashlight->SetYPos(flashlight->GetYPos() + yOffset);
	
	// Objective
	objective = root.GetNestedSprite( "_right", "objective" );
	objectiveComplete = root.GetNestedSprite( "_right", "objectiveComplete" );
	
	// Ammo Info
	ammoInfo = root.GetNestedSprite( "_bottomRight", "ammoInfo" );
	ammoInfo->SetXPos(ammoInfo->GetXPos() - xOffset);
	ammoInfo->parent->SetYPos(ammoInfo->parent->GetYPos() + yOffset);
	bsInfo = root.GetNestedSprite( "_bottomRight", "bsInfo" );
	if (renderSystem->GetStereo3DMode() == STEREO3D_VR) {
		bsInfo->SetXPos((bsInfo->GetXPos() - xOffset) + 150);
		bsInfo->SetYPos(bsInfo->GetYPos() - 150);
	}
	soulcubeInfo = root.GetNestedSprite( "_bottomRight", "soulcube" );
	soulcubeInfo->SetXPos(soulcubeInfo->GetXPos() - xOffset);
	
	// If the player loaded a save with enough souls to use the cube, the icon wouldn't show.  We're setting this flag in idPlayer::Restore so we can show the cube after loading a game
	if( showSoulCubeInfoOnLoad == true )
	{
		showSoulCubeInfoOnLoad = false;
		UpdateSoulCube( true );
	}
	
	// Weapon pills
	weaponPills = root.GetNestedObj( "_bottomRight", "weaponState" );
	weaponPills->GetSprite()->SetXPos(weaponPills->GetSprite()->GetXPos() - xOffset);
	weaponImg = root.GetNestedSprite( "_bottomRight", "weaponIcon" );
	weaponImg->SetXPos(weaponImg->GetXPos() - xOffset);
	weaponName = root.GetNestedObj( "_bottomRight", "weaponName" );
	weaponName->GetSprite()->SetXPos(weaponName->GetSprite()->GetXPos() - xOffset);
	
	// Pickup Info
	newWeapon = root.GetNestedSprite( "_center", "newWeapon" );
	pickupInfo = root.GetNestedSprite( "_bottomLeft", "pickupInfo" );
	pickupInfo->SetXPos(pickupInfo->GetXPos() + xOffset);
	pickupInfo->SetYPos(pickupInfo->GetYPos() + yOffset);
	newItem = root.GetNestedSprite( "_left", "newItem" );
	newItem->SetXPos(newItem->GetXPos() + xOffset);
	
	// Cursors
	talkCursor = root.GetNestedSprite( "_center", "crosshairTalk" );
	combatCursor = root.GetNestedSprite( "_center", "crosshairCombat" );
	grabberCursor = root.GetNestedSprite( "_center", "crosshairGrabber" );
	respawnMessage = root.GetNestedSprite( "_center", "respawnMessage" );
	
	// MP OBJECTS
	mpInfo = root.GetNestedSprite( "_top", "mp_info" );
	mpHitInfo = root.GetNestedSprite( "_bottom", "hitInfo" );
	mpTime = root.GetNestedText( "_top", "mp_info", "txtTime" );
	mpMessage = root.GetNestedText( "_top", "mp_info", "txtInfo" );
	mpWeapons = root.GetNestedObj( "_bottom", "mpWeapons" );
	mpChatObject = root.GetNestedSprite( "_left", "mpChat" );
	mpConnection = root.GetNestedSprite( "_center", "connectionMsg" );
	
	// Functions
	
	class idTriggerNewPDAOrVideo : public idSWFScriptFunction_RefCounted
	{
	public:
		idTriggerNewPDAOrVideo( idMenuScreen_HUD* _screen ) :
			screen( _screen )
		{
		}
		
		idSWFScriptVar Call( idSWFScriptObject* thisObject, const idSWFParmList& parms )
		{
		
			if( screen == NULL )
			{
				return idSWFScriptVar();
			}
			
			if( parms.Num() != 1 )
			{
				return idSWFScriptVar();
			}
			
			bool pdaDownload = parms[0].ToBool();
			if( pdaDownload )
			{
				screen->ToggleNewPDA( true );
			}
			else
			{
				screen->ToggleNewVideo( true );
			}
			
			return idSWFScriptVar();
		}
	private:
		idMenuScreen_HUD* screen;
	};
	
	menuGUI->SetGlobal( "toggleNewNotification", new idTriggerNewPDAOrVideo( this ) );
	
}

/*
========================
idMenuScreen_HUD::HideScreen
========================
*/
void idMenuScreen_HUDLocal::HideScreen( const mainMenuTransition_t transitionType )
{

}

/*
========================
idMenuScreen_HUD::Update
========================
*/
void idMenuScreen_HUDLocal::Update()
{

	idPlayer* player = game->GetLocalPlayer();
	if( player == NULL )
	{
		return;
	}

	idMenuScreen::Update();
}

/*
========================
idMenuScreen_HUD::UpdateHealth
========================
*/
void idMenuScreen_HUDLocal::UpdateHealthArmor( idPlayer* player )
{

	if( !playerInfo || !player || game->CheckInCinematic() || this->inCinematic)
	{
		return;
	}
	
	if( common->IsMultiplayer() )
	{
		playerInfo->GetSprite()->SetYPos( 20.0f );
	}
	else
	{
		playerInfo->GetSprite()->SetYPos( 0.0f );
	}

	idSWFTextInstance* txtVal = playerInfo->GetNestedText( "health", "txtVal" );
	if( txtVal != NULL )
	{
		txtVal->SetText( va( "%d", player->health ) );
		txtVal->SetStrokeInfo( true, 0.75f, 1.5f );
		
		// Set the damage color
		swfColorRGBA_t color;
		color.r = 255;
		color.a = 255;
		uint8 gbColor;
		if( player->health > 60 )
		{
			gbColor = 255;
		}
		else if( player->health > 30 )
		{
			gbColor = 156;
		}
		else
		{
			gbColor = 0;
		}
		color.g = gbColor;
		color.b = gbColor;
		txtVal->color = color;
	}
	
	txtVal = playerInfo->GetNestedText( "armor", "txtVal" );
	if( txtVal != NULL )
	{
		txtVal->SetText( va( "%d", player->inventory.armor ) );
		txtVal->SetStrokeInfo( true, 0.75f, 1.5f );
		
		if( armorFrame != NULL )
		{
			if( player->inventory.armor == 0 )
			{
				armorFrame->StopFrame( 2 );
			}
			else
			{
				armorFrame->StopFrame( 1 );
			}
		}
	}
	
	if( healthBorder != NULL )
	{
		healthBorder->StopFrame( 100 - player->health + 1 );
	}
	
	if( healthPulse != NULL )
	{
		if( player->healthPulse )
		{
			game->StartSound(player,"snd_healthpulse", SND_CHANNEL_ITEM, 0, false, NULL );
			player->healthPulse = false;
			healthPulse->SetVisible( true );
			healthPulse->PlayFrame( "rollOn" );
		}
		
		if( player->healthTake )
		{
			game->StartSound(player,"snd_healthtake", SND_CHANNEL_ITEM, 0, false, NULL );
			player->healthTake = false;
			healthPulse->SetVisible( true );
			healthPulse->PlayFrame( "rollOn" );
		}
	}
}

/*
========================
idMenuScreen_HUD::UpdateStamina
========================
*/
void idMenuScreen_HUDLocal::UpdateStamina( idPlayer* player )
{

	if( !stamina || !player || game->CheckInCinematic() || this->inCinematic)
	{
		return;
	}
	
	idSWFSpriteInstance* stamSprite = stamina->GetSprite();
	if( stamSprite != NULL )
	{
	
		if( common->IsMultiplayer() || in_alwaysRun.GetInteger() == 1)
		{
			stamSprite->SetVisible( false );
		}
		else
		{
			float max_stamina = game->GetCVarFloat("pm_stamina");
			if( !max_stamina )
			{
				stamSprite->SetVisible( false );
			}
			else
			{
				stamSprite->SetVisible( true );
				float staminaPercent = idMath::Ftoi( 100.0f * player->stamina / max_stamina );
				stamSprite->StopFrame( staminaPercent + 1 );
			}
		}
	}
}

/*
========================
idMenuScreen_HUD::UpdateLocation
========================
*/
void idMenuScreen_HUDLocal::UpdateWeaponInfo( idPlayer* player )
{

	if( !player || !ammoInfo || game->CheckInCinematic() || this->inCinematic)
	{
		return;
	}

	idEntityPtr<idWeapon> weapon = player->weapon;
	
	assert( weapon.GetEntity() );
	
	int inClip = game->AmmoInClip(weapon.GetEntity());
	int ammoAmount = game->AmmoAvailable(weapon.GetEntity());
	
	//Make sure the hud always knows how many bloodstone charges there are
	int ammoRequired;
	int bloodstoneAmmo = 0;
	if( player->weapon_bloodstone >= 0 )
	{
		ammo_t ammo_i = game->AmmoIndexForWeaponClass(player,"weapon_bloodstone_passive", &ammoRequired );
		bloodstoneAmmo = game->HasAmmo(player,ammo_i, ammoRequired );
	}
	if( bsInfo )
	{
		if( bloodstoneAmmo > 0 )
		{
			bsInfo->SetVisible( true );
			bsInfo->StopFrame( bloodstoneAmmo + 1 );
		}
		else
		{
			bsInfo->SetVisible( false );
		}
	}
	
	if( ammoAmount == -1 || player->GetCurrentWeaponSlot() == player->weapon_bloodstone || player->GetCurrentWeaponSlot() == player->weapon_soulcube )
	{
	
		ammoInfo->SetVisible( false );
		
	}
	else
	{
	
		idStr totalAmmo;
		idStr playerAmmo;
		idStr playerClip;
		
		bool showClip = true;
		
		//Hack to stop the bloodstone ammo to display when it is being activated
		if( !game->IsReady(weapon.GetEntity()) )
		{
			// show infinite ammo
			playerAmmo = "";
			totalAmmo = "";
		}
		else
		{
			// show remaining ammo
			totalAmmo = va( "%i", ammoAmount );
			playerAmmo = game->ClipSize(weapon.GetEntity()) ? va( "%i", inClip ) : "--";		// how much in the current clip
			playerClip = game->ClipSize(weapon.GetEntity()) ? va( "%i", ammoAmount / game->ClipSize(weapon.GetEntity())) : "--";
			//allAmmo = va( "%i/%i", inClip, ammoAmount );
		}
		
		if( !game->ClipSize(weapon.GetEntity()))
		{
			showClip = false;
		}
		
		bool ammoEmpty = ( ammoAmount == 0 );
		bool clipEmpty = (game->ClipSize(weapon.GetEntity()) ? inClip == 0 : false );
		bool clipLow = (game->ClipSize(weapon.GetEntity()) ? inClip <= game->LowAmmo(weapon.GetEntity()) : false );
		
		//Hack to stop the bloodstone ammo to display when it is being activated
		if( player->GetCurrentWeaponSlot() == player->weapon_bloodstone )
		{
			ammoEmpty = false;
			clipEmpty = false;
			clipLow = false;
		}
		
		if( showClip )
		{
		
			ammoInfo->SetVisible( true );
			ammoInfo->StopFrame( 1 );
			if( common->IsMultiplayer() )
			{
				ammoInfo->SetYPos( 20.0f );
			}
			else
			{
				ammoInfo->SetYPos( 0.0f );
			}
			idSWFSpriteInstance* txtClipSprite = ammoInfo->GetScriptObject()->GetNestedSprite( "info", "clip" );
			idSWFSpriteInstance* clipLowSprite = ammoInfo->GetScriptObject()->GetNestedSprite( "info", "lowAmmo" );
			idSWFSpriteInstance* clipEmptySprite = ammoInfo->GetScriptObject()->GetNestedSprite( "info", "clipEmpty" );
			idSWFSpriteInstance* ammoEmptySprite = ammoInfo->GetScriptObject()->GetNestedSprite( "info", "noAmmo" );
			idSWFSpriteInstance* txtAmmoSprite = ammoInfo->GetScriptObject()->GetNestedSprite( "info", "ammoCount" );
			
			idSWFTextInstance* txtClip = ammoInfo->GetScriptObject()->GetNestedText( "info", "clip", "clipCount", "txtVal" );
			idSWFTextInstance* txtAmmo = ammoInfo->GetScriptObject()->GetNestedText( "info", "ammoCount", "txtVal" );
			
			if( txtClipSprite && clipLowSprite && clipEmptySprite )
			{
			
				if( clipEmpty )
				{
					clipLowSprite->SetVisible( false );
					clipEmptySprite->SetVisible( true );
					txtClipSprite->StopFrame( 3 );
				}
				else if( clipLow )
				{
					clipLowSprite->SetVisible( true );
					clipEmptySprite->SetVisible( false );
					txtClipSprite->StopFrame( 2 );
				}
				else
				{
					clipLowSprite->SetVisible( false );
					clipEmptySprite->SetVisible( false );
					txtClipSprite->StopFrame( 1 );
				}
				
				if( txtClip != NULL )
				{
					txtClip->SetText( playerAmmo );
					txtClip->SetStrokeInfo( true, 0.75f, 1.5f );
				}
			}
			
			if( txtAmmo != NULL )
			{
			
				if( ammoEmptySprite && txtAmmoSprite )
				{
					if( ammoEmpty )
					{
						ammoEmptySprite->SetVisible( true );
						txtAmmoSprite->StopFrame( 2 );
					}
					else
					{
						ammoEmptySprite->SetVisible( false );
						txtAmmoSprite->StopFrame( 1 );
					}
				}
				
				txtAmmo->SetText( totalAmmo );
				txtAmmo->SetStrokeInfo( true, 0.75f, 1.5f );
			}
		}
		else
		{
		
			ammoInfo->SetVisible( true );
			ammoInfo->StopFrame( 2 );
			
			if( common->IsMultiplayer() )
			{
				ammoInfo->SetYPos( 20.0f );
			}
			else
			{
				ammoInfo->SetYPos( 0.0f );
			}
			
			idSWFTextInstance* txtAmmo = ammoInfo->GetScriptObject()->GetNestedText( "info", "txtVal" );
			
			if( txtAmmo != NULL )
			{
				txtAmmo->SetText( totalAmmo );
				txtAmmo->SetStrokeInfo( true, 0.75f, 1.5f );
			}
			
		}
	}
}

/*
========================
idMenuScreen_HUD::GiveWeapon
========================
*/
void idMenuScreen_HUDLocal::GiveWeapon( idPlayer* player, int weaponIndex )
{

	if( common->IsMultiplayer() || game->CheckInCinematic() || this->inCinematic)
	{
		return;
	}
	
	const char* weapnum = va( "def_weapon%d", weaponIndex );
	const char* weap = player->spawnArgs.GetString( weapnum );
	if( weap != NULL && *weap != '\0' )
	{
		const idDeclEntityDef* weaponDef = game->FindEntityDef( weap, false );
		if( weaponDef != NULL )
		{
			const char* hudIconName = weaponDef->dict.GetString( "hudIcon" );
			if( hudIconName[ 0 ] == '\0' )
			{
				idLib::Warning( "idMenuScreen_HUD: Missing hudIcon for weapon %s", weap );
				return;
			}
			
			const idMaterial* hudIcon = declManager->FindMaterial( hudIconName, false );
			if( newWeapon != NULL )
			{
				newWeapon->SetVisible( true );
				newWeapon->PlayFrame( 2 );
				
				idSWFSpriteInstance* topImg = newWeapon->GetScriptObject()->GetNestedSprite( "topImg" );
				idSWFSpriteInstance* botImg = newWeapon->GetScriptObject()->GetNestedSprite( "botImg" );
				
				if( topImg && botImg )
				{
					topImg->SetMaterial( hudIcon );
					botImg->SetMaterial( hudIcon );
				}
			}
		}
	}
}

/*
========================
idMenuScreen_HUD::UpdateWeaponStates
========================
*/
void idMenuScreen_HUDLocal::UpdatePickupInfo( int index, const idStr& name )
{

	if( !pickupInfo || game->CheckInCinematic() || this->inCinematic)
	{
		return;
	}
	
	idSWFTextInstance* txtItem = pickupInfo->GetScriptObject()->GetNestedText( va( "item%d", index ), "txtVal" );
	if( txtItem != NULL )
	{
		txtItem->SetText( name );
		txtItem->SetStrokeInfo( true, 0.6f, 2.0f );
	}
	
}

/*
========================
idMenuScreen_HUD::IsPickupListReady
========================
*/
bool idMenuScreen_HUDLocal::IsPickupListReady()
{

	if( !pickupInfo || game->CheckInCinematic() || this->inCinematic)
	{
		return false;
	}
	
	if( pickupInfo->GetCurrentFrame() == 1 )
	{
		return true;
	}
	
	return false;
}

/*
========================
idMenuScreen_HUD::UpdateWeaponStates
========================
*/
void idMenuScreen_HUDLocal::ShowPickups()
{

	if( !pickupInfo || game->CheckInCinematic() || this->inCinematic)
	{
		return;
	}
	
	pickupInfo->SetVisible( true );
	pickupInfo->PlayFrame( "rollOn" );
}

/*
========================
idMenuScreen_HUD::SetCursorState
========================
*/
void idMenuScreen_HUDLocal::SetCursorState( idPlayer* player, cursorState_t state, int set )
{

	switch( state )
	{
		case CURSOR_TALK:
		{
			cursorNone = 0;
			cursorTalking = set;
			break;
		}
		case CURSOR_IN_COMBAT:
		{
			cursorNone = 0;
			cursorInCombat = set;
			break;
		}
		case CURSOR_ITEM:
		{
			cursorNone = 0;
			cursorItem = set;
			break;
		}
		case CURSOR_GRABBER:
		{
			cursorNone = 0;
			cursorGrabber = set;
			break;
		}
		case CURSOR_NONE:
		{
			// so that talk button still appears for 3D view
			if( cursorState != CURSOR_TALK || cursorTalking != 1 )
			{
				cursorTalking = 0;
				cursorGrabber = 0;
				cursorInCombat = 0;
				cursorItem = 0;
				cursorNone = 1;
			}
			break;
		}
	}
	
}

/*
========================
idMenuScreen_HUD::SetCursorText
========================
*/
void idMenuScreen_HUDLocal::SetCursorText( const idStr& action, const idStr& focus )
{
	cursorAction = action;
	cursorFocus = focus;
}

/*
========================
idMenuScreen_HUD::CombatCursorFlash
========================
*/
void idMenuScreen_HUDLocal::CombatCursorFlash()
{

	if( cursorInCombat )
	{
		if( cursorState == CURSOR_IN_COMBAT )
		{
			if( combatCursor )
			{
				combatCursor->PlayFrame( "hit" );
			}
		}
	}
	
}

/*
========================
idMenuScreen_HUD::UpdateCursorState
========================
*/
void idMenuScreen_HUDLocal::UpdateCursorState()
{

	if( (!cursorTalking && !cursorInCombat && !cursorGrabber && !cursorItem) || (game->CheckInCinematic() || this->inCinematic))
	{
	
		cursorNone = true;
		cursorState = CURSOR_NONE;
		
		// hide all cursors
		if( combatCursor )
		{
			combatCursor->StopFrame( 1 );
			combatCursor->SetVisible( false );
		}
		
		if( talkCursor )
		{
			talkCursor->StopFrame( 1 );
			talkCursor->SetVisible( false );
		}
		
		if( grabberCursor )
		{
			grabberCursor->StopFrame( 1 );
			grabberCursor->SetVisible( false );
		}
		
	}
	else
	{
	
		if( cursorTalking )
		{
		
			if( cursorTalking == 1 )  	// ready to talk
			{
			
			}
			else if( cursorTalking == 2 )      // already talking / busy
			{
			
			}
			
			if( cursorState != CURSOR_TALK )
			{
			
				if( combatCursor )
				{
					combatCursor->StopFrame( 1 );
					combatCursor->SetVisible( false );
				}
				
				if( grabberCursor )
				{
					grabberCursor->StopFrame( 1 );
					grabberCursor->SetVisible( false );
				}
				
				// play roll on
				if( talkCursor )
				{
					talkCursor->SetVisible( true );
					talkCursor->PlayFrame( 2 );
					
					idSWFSpriteInstance* topBacking = talkCursor->GetScriptObject()->GetNestedSprite( "backing", "topBar" );
					idSWFSpriteInstance* bottomBacking = talkCursor->GetScriptObject()->GetNestedSprite( "backing", "botBar" );
					
					idSWFTextInstance* txtAction = talkCursor->GetScriptObject()->GetNestedText( "info", "txtAction" );
					idSWFTextInstance* txtFocus = talkCursor->GetScriptObject()->GetNestedText( "info", "txtFocus" );
					
					idSWFTextInstance* txtPrompt = talkCursor->GetScriptObject()->GetNestedText( "talkPrompt", "txtPrompt" );
					
					if( txtAction )
					{
					
						if( !idLib::joystick )
						{
							txtAction->tooltip = true;
							keyBindings_t bind = idKeyInput::KeyBindingsFromBinding( "_use", true );
							idStr actionText = idLocalization::GetString( cursorAction );
							if( !bind.mouse.IsEmpty() )
							{
								actionText.Append( " [" );
								actionText.Append( bind.mouse );
								actionText.Append( "]" );
							}
							else if( !bind.keyboard.IsEmpty() )
							{
								actionText.Append( " [" );
								actionText.Append( bind.keyboard );
								actionText.Append( "]" );
							}
							
							txtAction->SetText( actionText );
						}
						else
						{
							txtAction->tooltip = false;
							txtAction->SetText( cursorAction );
						}
						txtAction->SetStrokeInfo( true, 0.75f, 1.5f );
						float actionLength = txtAction->GetTextLength();
						
						if( topBacking )
						{
							if( !cursorAction.IsEmpty() )
							{
								topBacking->SetXPos( actionLength );
							}
							else
							{
								topBacking->SetXPos( -75.0f );
							}
						}
					}
					
					if( txtFocus )
					{
						txtFocus->SetText( cursorFocus );
						txtFocus->SetStrokeInfo( true, 0.75f, 1.5f );
						float focusLength = txtFocus->GetTextLength();
						
						if( bottomBacking )
						{
							if( !cursorFocus.IsEmpty() )
							{
								bottomBacking->SetXPos( focusLength );
							}
							else
							{
								bottomBacking->SetXPos( -75.0f );
							}
						}
					}
					
					if( txtPrompt )
					{
						if( idLib::joystick )
						{
							txtPrompt->tooltip = true;
							txtPrompt->SetText( "_use" );
						}
						else
						{
							txtPrompt->tooltip = false;
							txtPrompt->SetText( "" );
						}
					}
				}
				cursorState = CURSOR_TALK;
			}
			
		}
		else if( cursorGrabber )
		{
		
			if( talkCursor )
			{
				talkCursor->StopFrame( 1 );
				talkCursor->SetVisible( false );
			}
			
			if( combatCursor )
			{
				combatCursor->StopFrame( 1 );
				combatCursor->SetVisible( false );
			}
			
			if( cursorState != CURSOR_GRABBER )
			{
				if( grabberCursor )
				{
					grabberCursor->SetVisible( true );
					grabberCursor->PlayFrame( "loop" );
				}
			}
			
			cursorState = CURSOR_GRABBER;
			
		}
		else if( cursorItem )
		{
		
			cursorState = CURSOR_ITEM;
			
		}
		else if( cursorInCombat )
		{
		
			if( cursorState == CURSOR_TALK )
			{
				if( talkCursor )
				{
					talkCursor->StopFrame( 1 );
					talkCursor->SetVisible( false );
				}
				
				if( combatCursor )
				{
					combatCursor->SetVisible( true );
					combatCursor->PlayFrame( "rollOn" );
				}
				
				// play cursor roll on
			}
			else if( cursorState != CURSOR_IN_COMBAT )
			{
			
				if( grabberCursor )
				{
					grabberCursor->StopFrame( 1 );
					grabberCursor->SetVisible( false );
				}
				
				// set cursor visible
				if( combatCursor )
				{
					combatCursor->SetVisible( true );
					combatCursor->StopFrame( 2 );
				}
				
			}
			
			cursorState = CURSOR_IN_COMBAT;
			
		}
	}
}

/*
========================
idMenuScreen_HUD::UpdateSoulCube
========================
*/
void idMenuScreen_HUDLocal::UpdateSoulCube( bool ready )
{

	if( !soulcubeInfo || game->CheckInCinematic() || this->inCinematic)
	{
		return;
	}
	
	if( ready && !soulcubeInfo->IsVisible() )
	{
		soulcubeInfo->SetVisible( true );
		soulcubeInfo->PlayFrame( "rollOn" );
	}
	else if( !ready )
	{
		soulcubeInfo->PlayFrame( "rollOff" );
	}
	
}

/*
========================
idMenuScreen_HUD::ShowRespawnMessage
========================
*/
void idMenuScreen_HUDLocal::ShowRespawnMessage( bool show )
{

	if( !respawnMessage )
	{
		return;
	}
	
	if( show )
	{
		respawnMessage->SetVisible( true );
		respawnMessage->PlayFrame( "rollOn" );
		
		idSWFTextInstance* message = respawnMessage->GetScriptObject()->GetNestedText( "info", "txtMessage" );
		if( message != NULL )
		{
			message->tooltip = true;
			message->SetText( "#str_respawn_message" );
			message->SetStrokeInfo( true );
		}
		
	}
	else
	{
		if( respawnMessage->IsVisible() )
		{
			respawnMessage->PlayFrame( "rollOff" );
		}
	}
}

/*
========================
idMenuScreen_HUD::UpdateWeaponStates
========================
*/
void idMenuScreen_HUDLocal::UpdateWeaponStates( idPlayer* player, bool weaponChanged )
{

	if( !weaponPills || game->CheckInCinematic() || this->inCinematic)
	{
		return;
	}
	
	if( player == NULL )
	{
		return;
	}
	
	idStr displayName;
	if( common->IsMultiplayer() )
	{
	
		if( !mpWeapons || player->GetIdealWeapon() == 0 )
		{
			return;
		}
		
		weaponPills->GetSprite()->SetVisible( false );
		
		if( weaponChanged )
		{
			mpWeapons->GetSprite()->SetVisible( true );
			mpWeapons->GetSprite()->PlayFrame( "rollOn" );
			
			int weaponDefIndex = -1;
			idList< idStr > weaponDefNames;
			// start at 1 so we skip the fists
			for( int i = 1; i < MAX_WEAPONS; ++i )
			{
				if( player->inventory.weapons & ( 1 << i ) )
				{
					if( i == player->GetIdealWeapon() )
					{
						weaponDefIndex = weaponDefNames.Num();
					}
					weaponDefNames.Append( va( "def_weapon%d", i ) );
				}
			}
			
			int numRightWeapons = 0;
			int numLeftWeapons = 0;
			
			if( weaponDefNames.Num() == 2 )
			{
				numRightWeapons = 1 - weaponDefIndex;
				numLeftWeapons = weaponDefIndex;
			}
			else if( weaponDefNames.Num() == 3 )
			{
				numRightWeapons = 1;
				numLeftWeapons = 1;
			}
			else if( weaponDefNames.Num() > 3 )
			{
				numRightWeapons = 2;
				numLeftWeapons = 2;
			}
			
			for( int i = -2; i < 3; ++i )
			{
			
				bool hide = false;
				
				if( i < 0 && idMath::Abs( i ) > numLeftWeapons )
				{
					hide = true;
				}
				else if( i > numRightWeapons )
				{
					hide = true;
				}
				else if( weaponDefNames.Num() == 0 )
				{
					hide = true;
				}
				
				int index = i;
				if( i < 0 )
				{
					index = 2 + idMath::Abs( i );
				}
				
				idSWFSpriteInstance* topValid = mpWeapons->GetNestedSprite( "list", va( "weapon%i", index ), "topValid" );
				idSWFSpriteInstance* botValid = mpWeapons->GetNestedSprite( "list", va( "weapon%i", index ), "botValid" );
				idSWFSpriteInstance* topInvalid = mpWeapons->GetNestedSprite( "list", va( "weapon%i", index ), "topInvalid" );
				idSWFSpriteInstance* botInvalid = mpWeapons->GetNestedSprite( "list", va( "weapon%i", index ), "botInvalid" );
				
				if( !topValid || !botValid || !topInvalid || !botInvalid )
				{
					mpWeapons->GetSprite()->SetVisible( false );
					break;
				}
				
				if( hide )
				{
					topValid->SetVisible( false );
					botValid->SetVisible( false );
					topInvalid->SetVisible( false );
					botInvalid->SetVisible( false );
					continue;
				}
				
				int weaponIndex = weaponDefIndex + i;
				if( weaponIndex < 0 )
				{
					weaponIndex = weaponDefNames.Num() + weaponIndex;
				}
				else if( weaponIndex >= weaponDefNames.Num() )
				{
					weaponIndex = ( weaponIndex - weaponDefNames.Num() );
				}
				
				int weapState = 1;
				const idMaterial* hudIcon = NULL;
				const char* weapNum = weaponDefNames[ weaponIndex ];
				const char* weap = player->spawnArgs.GetString( weapNum );
				if( weap != NULL && *weap != '\0' )
				{
					const idDeclEntityDef* weaponDef = game->FindEntityDef( weap, false );
					if( weaponDef != NULL )
					{
						hudIcon = declManager->FindMaterial( weaponDef->dict.GetString( "hudIcon" ), false );
						if( i == 0 )
						{
							displayName = weaponDef->dict.GetString( "display_name" );
							weapState++;
						}
					}
					
					if( !game->HasAmmo(player, weap, true, player ) )
					{
						weapState = 0;
					}
				}
				
				topValid->SetVisible( false );
				botValid->SetVisible( false );
				topInvalid->SetVisible( false );
				botInvalid->SetVisible( false );
				
				topValid->SetMaterial( hudIcon );
				botValid->SetMaterial( hudIcon );
				topInvalid->SetMaterial( hudIcon );
				botInvalid->SetMaterial( hudIcon );
				
				if( weapState == 0 )
				{
					botInvalid->SetVisible( true );
					if( i == 0 )
					{
						topInvalid->SetVisible( true );
					}
				}
				else if( weapState == 2 )
				{
					topValid->SetVisible( true );
					botValid->SetVisible( true );
				}
				else
				{
					botValid->SetVisible( true );
				}
			}
		}
		
	}
	else
	{
	
		bool hasWeapons = false;
		const idMaterial* hudIcon = NULL;
		
		for( int i = 0; i < MAX_WEAPONS; i++ )
		{
			const char* weapnum = va( "def_weapon%d", i );
			int weapstate = 0;
			if( player->inventory.weapons & ( 1 << i ) )
			{
				hasWeapons = true;
				const char* weap = player->spawnArgs.GetString( weapnum );
				if( weap != NULL && *weap != '\0' )
				{
					weapstate++;
				}
				if( player->GetIdealWeapon() == i )
				{
				
					const idDeclEntityDef* weaponDef = game->FindEntityDef( weap, false );
					if( weaponDef != NULL )
					{
						hudIcon = declManager->FindMaterial( weaponDef->dict.GetString( "hudIcon" ), false );
						displayName = weaponDef->dict.GetString( "display_name" );
					}
					
					weapstate++;
				}
			}
			
			idSWFSpriteInstance* pill = weaponPills->GetNestedSprite( va( "pill%d", i ) );
			if( pill )
			{
				pill->StopFrame( weapstate + 1 );
			}
		}
		
		if( !hasWeapons )
		{
			weaponPills->GetSprite()->SetVisible( false );
		}
		else
		{
			weaponPills->GetSprite()->SetVisible( true );
		}
		
		if( weaponImg )
		{
			if( weaponChanged && hudIcon != NULL )
			{
				weaponImg->SetVisible( true );
				weaponImg->PlayFrame( 2 );
				
				idSWFSpriteInstance* topImg = weaponImg->GetScriptObject()->GetNestedSprite( "topImg" );
				idSWFSpriteInstance* botImg = weaponImg->GetScriptObject()->GetNestedSprite( "botImg" );
				
				if( topImg != NULL && botImg != NULL )
				{
					topImg->SetMaterial( hudIcon );
					botImg->SetMaterial( hudIcon );
				}
				
				/*if ( weaponName && weaponName->GetSprite() ) {
					weaponName->GetSprite()->SetVisible( true );
					weaponName->GetSprite()->PlayFrame( 2 );
				
					idSWFTextInstance * txtVal = weaponName->GetNestedText( "info", "txtVal" );
					if ( txtVal != NULL ) {
						txtVal->SetText( displayName );
						txtVal->SetStrokeInfo( true, 0.6f, 2.0f );
					}
				}*/
			}
		}
	}
	
}

/*
========================
idMenuScreen_HUD::UpdateLocation
========================
*/
void idMenuScreen_HUDLocal::UpdateLocation( idPlayer* player )
{

	if( !locationName || !player || game->CheckInCinematic() || this->inCinematic)
	{
		return;
	}
	
	idPlayer* playertoLoc = player;
	if( player->spectating && player->spectator != player->entityNumber )
	{
		playertoLoc = static_cast< idPlayer* >( game->GetEntities()[ player->spectator ] );
		if( playertoLoc == NULL )
		{
			playertoLoc = player;
		}
	}
	
	idLocationEntity* locationEntity = game->LocationForPoint( game->GetEyePosition(playertoLoc) );
	if( locationEntity )
	{
		locationName->SetText( game->GetLocation(locationEntity) );
	}
	else
	{
		locationName->SetText( idLocalization::GetString( "#str_02911" ) );
	}
	locationName->SetStrokeInfo( true, 0.6f, 2.0f );
	
}

/*
========================
idMenuScreen_HUD::ShowTip
========================
*/
void idMenuScreen_HUDLocal::ShowTip( const char* title, const char* tip )
{
	if( !tipInfo || game->CheckInCinematic())
	{
		return;
	}
	
	idSWFSpriteInstance* tipSprite = tipInfo->GetSprite();
	
	if( !tipSprite )
	{
		return;
	}
	
	tipSprite->SetVisible( true );
	tipSprite->PlayFrame( "rollOn" );
	
	idSWFTextInstance* txtTitle = tipInfo->GetNestedText( "info", "txtTitle" );
	idSWFTextInstance* txtTip = tipInfo->GetNestedText( "info", "txtTip" );
	
	if( txtTitle != NULL )
	{
		txtTitle->SetText( title );
		txtTitle->SetStrokeInfo( true, 0.75f, 1.5f );
	}
	
	if( txtTip != NULL )
	{
		txtTip->SetText( tip );
		txtTip->tooltip = true;
		txtTip->SetStrokeInfo( true, 0.75f, 1.5f );
		int numLines = txtTip->CalcNumLines();
		if( numLines == 0 )
		{
			numLines = 1;
		}
		idSWFSpriteInstance* backing = tipInfo->GetNestedSprite( "info", "backing" );
		if( backing != NULL )
		{
			backing->StopFrame( numLines );
		}
	}
}

/*
========================
idMenuScreen_HUD::HideTip
========================
*/
void idMenuScreen_HUDLocal::HideTip()
{

	if( !tipInfo || game->CheckInCinematic())
	{
		return;
	}
	
	idSWFSpriteInstance* tipSprite = tipInfo->GetSprite();
	
	if( !tipSprite )
	{
		return;
	}
	
	tipSprite->SetVisible( true );
	tipSprite->PlayFrame( "rollOff" );
	
}

/*
========================
idMenuScreen_HUD::DownloadPDA
========================
*/
void idMenuScreen_HUDLocal::DownloadPDA( const idDeclPDA* pda, bool newSecurity )
{

	if( newPDADownload )
	{
		newPDADownload->SetVisible( true );
		newPDADownload->PlayFrame( "rollOn" );
		
		newPDAName = newPDADownload->GetScriptObject()->GetNestedText( "info", "txtName" );
		newPDAHeading = newPDADownload->GetScriptObject()->GetNestedText( "info", "txtHeading" );
		
		if( newPDAName && GetSWFObject() != NULL )
		{
			idStr pdaName = pda->GetPdaName();
			pdaName.RemoveColors();
			GetSWFObject()->SetGlobal( "pdaNameDownload", pdaName );
			newPDAName->SetStrokeInfo( true, 0.9f, 2.0f );
		}
		
		if( newPDAHeading && GetSWFObject() != NULL )
		{
			GetSWFObject()->SetGlobal( "pdaDownloadHeading", "#str_02031" );
			newPDAHeading->SetStrokeInfo( true, 0.9f, 2.0f );
		}
	}
	
	if( newSecurity )
	{
		UpdatedSecurity();
	}
}

/*
========================
idMenuScreen_HUD::DownloadVideo
========================
*/
void idMenuScreen_HUDLocal::DownloadVideo()
{

	if( newVideoDownload )
	{
		newVideoDownload->SetVisible( true );
		newVideoDownload->PlayFrame( "rollOn" );
		
		newVideoHeading = newVideoDownload->GetScriptObject()->GetNestedText( "info", "txtHeading" );
		
		if( newVideoHeading )
		{
			newVideoHeading->SetText( "#str_02033" );
			newVideoHeading->SetStrokeInfo( true, 0.9f, 2.0f );
		}
	}
}

/*
========================
idMenuScreen_HUD::UpdatedSecurity
========================
*/
void idMenuScreen_HUDLocal::UpdatedSecurity()
{
	if( security != NULL && securityText != NULL )
	{
		security->SetVisible( true );
		security->PlayFrame( "rollOn" );
		securityText->SetText( "#str_02032" );
		securityText->SetStrokeInfo( true, 0.9f, 2.0f );
	}
}

/*
========================
idMenuScreen_HUD::ClearNewPDAInfo
========================
*/
void idMenuScreen_HUDLocal::ClearNewPDAInfo()
{

	ToggleNewVideo( false );
	ToggleNewPDA( false );
	
	if( security )
	{
		security->StopFrame( 1 );
	}
	
	if( newPDADownload )
	{
		newPDADownload->StopFrame( 1 );
	}
	
	if( newVideoDownload )
	{
		newVideoDownload->StopFrame( 1 );
	}
	
}

/*
========================
idMenuScreen_HUD::UpdatedSecurity
========================
*/
void  idMenuScreen_HUDLocal::ToggleNewVideo( bool show )
{

	if( !newVideo || game->CheckInCinematic() || this->inCinematic)
	{
		return;
	}
	
	if( show && !newVideo->IsVisible() )
	{
		newVideo->SetVisible( true );
		newVideo->PlayFrame( "rollOn" );
	}
	else if( !show )
	{
		newVideo->StopFrame( 1 );
	}
	
}

/*
========================
idMenuScreen_HUD::UpdatedSecurity
========================
*/
void  idMenuScreen_HUDLocal::ToggleNewPDA( bool show )
{

	if( !newPDA || game->CheckInCinematic() || this->inCinematic)
	{
		return;
	}
	
	if( show && !newPDA->IsVisible() )
	{
		newPDA->SetVisible( true );
		newPDA->PlayFrame( "rollOn" );
	}
	else if( !show )
	{
		newPDA->StopFrame( 1 );
	}
	
}

/*
========================
idMenuScreen_HUD::UpdatedSecurity
========================
*/
void  idMenuScreen_HUDLocal::UpdateAudioLog( bool show )
{

	if( !audioLog || game->CheckInCinematic() || this->inCinematic)
	{
		return;
	}
	
	if( show && !audioLog->IsVisible() )
	{
		audioLog->SetVisible( true );
		audioLog->StopFrame( "2" );
		
		for( int index = 0; index < 13; ++index )
		{
			idSWFSpriteInstance* node = audioLog->GetScriptObject()->GetNestedSprite( "bar", va( "node%d", index ) );
			if( node != NULL )
			{
				int frame = game->GetRandomInt( 100 );
				node->SetScale( 100.0f, frame );
				float toFrame = game->GetRandomFloat();
				node->SetMoveToScale( -1.0f, toFrame );
			}
		}
		
	}
	else if( !show )
	{
	
		audioLog->StopFrame( 1 );
		
	}
	else if( show )
	{
	
		if( audioLogPrevTime == 0 )
		{
			audioLogPrevTime = game->GetTime();
		}
		
		for( int index = 0; index < 13; ++index )
		{
			idSWFSpriteInstance* node = audioLog->GetScriptObject()->GetNestedSprite( "bar", va( "node%d", index ) );
			if( node != NULL )
			{
				float diff = game->GetTime() - audioLogPrevTime;
				float speed = ( diff / 350.0f ) * 100.0f;
				if( !node->UpdateMoveToScale( speed ) )
				{
					int frame = game->GetRandomInt( 100 );
					float scale = frame / 100.0f;
					node->SetMoveToScale( -1.0f, scale );
				}
			}
		}
		audioLogPrevTime = game->GetTime();
	}
}

/*
========================
idMenuScreen_HUD::UpdatedSecurity
========================
*/
void  idMenuScreen_HUDLocal::UpdateCommunication( bool show, idPlayer* player )
{

	if( !communication || !player || game->CheckInCinematic())
	{
		return;
	}
	
	bool oxygenChanged = false;
	if( inVaccuum != oxygenComm )
	{
		oxygenChanged = true;
	}
	
	if( show && !communication->IsVisible() )
	{
		communication->SetVisible( true );
		if( inVaccuum )
		{
			communication->StopFrame( "oxygen" );
		}
		else
		{
			communication->StopFrame( "2" );
		}
		
		for( int index = 0; index < 16; ++index )
		{
			idSWFSpriteInstance* node = communication->GetScriptObject()->GetNestedSprite( "info", "bar", va( "node%d", index ) );
			if( node != NULL )
			{
				int frame = game->GetRandomInt( 100 );
				node->SetScale( 100.0f, frame );
				float toFrame = game->GetRandomFloat();
				node->SetMoveToScale( -1.0f, toFrame );
			}
		}
	}
	else if( !show )
	{
		communication->StopFrame( 1 );
	}
	else if( show )
	{
	
		if( oxygenChanged )
		{
			if( inVaccuum )
			{
				communication->PlayFrame( "rollUp" );
			}
			else
			{
				communication->PlayFrame( "rollDown" );
			}
		}
		
		if( commPrevTime == 0 )
		{
			commPrevTime = game->GetTime();
		}
		
		for( int index = 0; index < 16; ++index )
		{
			idSWFSpriteInstance* node = communication->GetScriptObject()->GetNestedSprite( "info", "bar", va( "node%d", index ) );
			if( node != NULL )
			{
				float diff = game->GetTime() - commPrevTime;
				float speed = ( diff / 350.0f ) * 100.0f;
				if( !node->UpdateMoveToScale( speed ) )
				{
					int frame = game->GetRandomInt( 100 );
					float scale = frame / 100.0f;
					node->SetMoveToScale( -1.0f, scale );
				}
			}
		}
		
		commPrevTime = game->GetTime();
	}
	
	oxygenComm = inVaccuum;
}

/*
========================
idMenuScreen_HUD::UpdateOxygen
========================
*/
void  idMenuScreen_HUDLocal::UpdateOxygen( bool show, int val )
{

	if( !oxygen || game->CheckInCinematic())
	{
		return;
	}
	
	if( show )
	{
		if( !oxygen->IsVisible() )
		{
			inVaccuum = true;
			oxygen->SetVisible( true );
			oxygen->PlayFrame( "rollOn" );
		}
		
		idSWFSpriteInstance* info = oxygen->GetScriptObject()->GetNestedSprite( "info" );
		if( info != NULL )
		{
			info->StopFrame( val + 1 );
		}
		
		idSWFSpriteInstance* goodFrame = oxygen->GetScriptObject()->GetNestedSprite( "goodFrame" );
		idSWFSpriteInstance* badFrame = oxygen->GetScriptObject()->GetNestedSprite( "badFrame" );
		
		if( goodFrame != NULL && badFrame != NULL )
		{
			if( val + 1 >= 36 )
			{
				goodFrame->SetVisible( true );
				badFrame->SetVisible( false );
			}
			else
			{
				goodFrame->SetVisible( false );
				badFrame->SetVisible( true );
			}
		}
		
		idSWFTextInstance* txtVal = oxygen->GetScriptObject()->GetNestedText( "info", "txtHeading" );
		if( txtVal != NULL )
		{
			if ((game->GetLocalPlayer()->inventory.powerups & (1 << 6)) != 0) { //GK: Enviro Suit
				txtVal->SetText("#str_00100204");
			}
			else {
				txtVal->SetText("#str_00100922");
			}
			txtVal->SetStrokeInfo( true, 0.9f, 2.0f );
		}
		
		txtVal = oxygen->GetScriptObject()->GetNestedText( "info", "txtVal" );
		if( txtVal != NULL )
		{
			txtVal->SetText( va( "%d", val ) );
			txtVal->SetStrokeInfo( true, 0.9f, 2.0f );
		}
		
	}
	else if( !show )
	{
		inVaccuum = false;
		oxygen->StopFrame( 1 );
	}
}

/*
========================
idMenuScreen_HUD::SetupObjective
========================
*/
void idMenuScreen_HUDLocal::SetupObjective( const idStr& title, const idStr& desc, const idMaterial* screenshot )
{
	objTitle = title;
	objDesc = desc;
	objScreenshot = screenshot;
}

/*
========================
idMenuScreen_HUD::SetupObjective
========================
*/
void idMenuScreen_HUDLocal::SetupObjectiveComplete( const idStr& title )
{

	objCompleteTitle = title;
	
}

/*
========================
idMenuScreen_HUD::ShowObjective
========================
*/
void idMenuScreen_HUDLocal::ShowObjective( bool complete )
{

	if( complete )
	{
	
		if( !objectiveComplete )
		{
			return;
		}
		
		objectiveComplete->SetVisible( true );
		objectiveComplete->PlayFrame( "rollOn" );
		
		idSWFTextInstance* txtComplete = objectiveComplete->GetScriptObject()->GetNestedText( "info", "txtComplete" );
		idSWFTextInstance* txtTitle = objectiveComplete->GetScriptObject()->GetNestedText( "info", "txtTitle" );
		idSWFSpriteInstance* rightArrow = objectiveComplete->GetScriptObject()->GetNestedSprite( "info", "right_arrows" );
		
		if( txtComplete != NULL )
		{
			txtComplete->SetStrokeInfo( true, 0.9f, 2.0f );
			
			if( rightArrow != NULL )
			{
				rightArrow->SetXPos( txtComplete->GetTextLength() + 30.0f );
			}
		}
		
		if( txtTitle != NULL )
		{
			txtTitle->SetText( objCompleteTitle );
			txtTitle->SetStrokeInfo( true, 0.9f, 2.0f );
		}
		
	}
	else
	{
	
		if( !objective )
		{
			return;
		}
		
		objective->SetVisible( true );
		objective->PlayFrame( "rollOn" );
		
		idSWFTextInstance* txtNew = objective->GetScriptObject()->GetNestedText( "info", "txtComplete" );
		idSWFTextInstance* txtTitle = objective->GetScriptObject()->GetNestedText( "info", "txtTitle" );
		idSWFTextInstance* txtDesc = objective->GetScriptObject()->GetNestedText( "info", "txtDesc" );
		idSWFSpriteInstance* img = objective->GetScriptObject()->GetNestedSprite( "info", "img" );
		idSWFSpriteInstance* rightArrow = objective->GetScriptObject()->GetNestedSprite( "info", "right_arrows" );
		
		if( txtNew != NULL )
		{
			txtNew->SetStrokeInfo( true, 0.9f, 2.0f );
			
			if( rightArrow != NULL )
			{
				rightArrow->SetXPos( txtNew->GetTextLength() + 55.0f );
			}
		}
		
		if( txtTitle != NULL )
		{
			txtTitle->SetText( objTitle );
			txtTitle->SetStrokeInfo( true, 0.9f, 2.0f );
		}
		
		if( txtDesc )
		{
			txtDesc->SetText( objDesc );
		}
		
		if( img != NULL )
		{
			img->SetMaterial( objScreenshot );
		}
		
	}
	
}

/*
========================
idMenuScreen_HUD::HideObjective
========================
*/
void idMenuScreen_HUDLocal::HideObjective( bool complete )
{

	if( complete )
	{
	
		if( !objectiveComplete )
		{
			return;
		}
		
		objectiveComplete->PlayFrame( "rollOff" );
		
	}
	else
	{
	
		if( !objective )
		{
			return;
		}
		
		objective->PlayFrame( "rollOff" );
		
	}
	
}


//******************************************************************************************
// MULTIPLAYER FUNCITONS
//******************************************************************************************

/*
========================
idMenuScreen_HUD::ToggleMPInfo
========================
*/
void idMenuScreen_HUDLocal::ToggleMPInfo( bool show, bool showTeams, bool isCTF )
{

	if( !mpInfo )
	{
		return;
	}
	
	if( show )
	{
	
		mpInfo->SetVisible( true );
		
		idSWFSpriteInstance* redTeam = mpInfo->GetScriptObject()->GetNestedSprite( "redTeam" );
		idSWFSpriteInstance* blueTeam = mpInfo->GetScriptObject()->GetNestedSprite( "blueTeam" );
		idSWFSpriteInstance* redFlag = mpInfo->GetScriptObject()->GetNestedSprite( "redFlag" );
		idSWFSpriteInstance* blueFlag = mpInfo->GetScriptObject()->GetNestedSprite( "blueFlag" );
		
		if( redFlag )
		{
			redFlag->SetVisible( isCTF );
		}
		
		if( blueFlag )
		{
			blueFlag->SetVisible( isCTF );
		}
		
		if( !showTeams )
		{
			if( redTeam )
			{
				redTeam->SetVisible( false );
			}
			
			if( blueTeam )
			{
				blueTeam->SetVisible( false );
			}
		}
		else
		{
			if( redTeam )
			{
				redTeam->SetVisible( true );
			}
			
			if( blueTeam )
			{
				blueTeam->SetVisible( true );
			}
		}
		
	}
	else
	{
		mpInfo->SetVisible( false );
	}
	
}

/*
========================
idMenuScreen_HUD::SetFlagState
========================
*/
void idMenuScreen_HUDLocal::SetFlagState( int team, int state )
{

	if( !mpInfo )
	{
		return;
	}
	
	
	idSWFSpriteInstance* flag = NULL;
	if( team == 0 )
	{
		flag = mpInfo->GetScriptObject()->GetNestedSprite( "redFlag" );
	}
	else if( team == 1 )
	{
		flag = mpInfo->GetScriptObject()->GetNestedSprite( "blueFlag" );
	}
	
	if( flag )
	{
		if( state == 3 )    //FLAGSTATUS_NONE
		{
			flag->StopFrame( 1 );
		}
		else
		{
			flag->SetVisible( true );
			flag->StopFrame( state + 2 );
		}
	}
	
}

/*
========================
idMenuScreen_HUD::SetTeamScore
========================
*/
void idMenuScreen_HUDLocal::SetTeamScore( int team, int score )
{

	if( !mpInfo )
	{
		return;
	}
	
	idSWFTextInstance* txtScore = NULL;
	
	if( team == 0 )
	{
		txtScore = mpInfo->GetScriptObject()->GetNestedText( "redTeam", "txtRedScore" );
	}
	else if( team == 1 )
	{
		txtScore = mpInfo->GetScriptObject()->GetNestedText( "blueTeam", "txtBlueScore" );
	}
	
	if( txtScore )
	{
		txtScore->SetText( va( "%i", score ) );
		txtScore->SetStrokeInfo( true, 0.75f, 1.5f );
	}
	
}

/*
========================
idMenuScreen_HUD::SetTeam
========================
*/
void idMenuScreen_HUDLocal::SetTeam( int team )
{

	if( !mpInfo )
	{
		return;
	}
	
	idSWFSpriteInstance* teamBacking = mpInfo->GetScriptObject()->GetNestedSprite( "teamBack" );
	
	if( teamBacking )
	{
		if( team < 0 )
		{
			teamBacking->StopFrame( 3 );
		}
		else
		{
			teamBacking->StopFrame( team + 1 );
		}
	}
	
}

/*
========================
idMenuScreen_HUD::TriggerHitTarget
========================
*/
void idMenuScreen_HUDLocal::TriggerHitTarget( bool show, const idStr& target, int color )
{

	if( !mpHitInfo )
	{
		return;
	}
	
	if( show )
	{
	
		mpHitInfo->SetVisible( true );
		mpHitInfo->PlayFrame( "rollOn" );
		
		if( menuGUI )
		{
			menuGUI->SetGlobal( "hitTargetName", target.c_str() );
		}
		
		idSWFSpriteInstance* backing = mpHitInfo->GetScriptObject()->GetNestedSprite( "bgColor" );
		if( backing )
		{
			if( color <= 0 || !game->IsGametypeTeamBased() )
			{
				color = 1;
			}
			backing->StopFrame( color );
		}
		
	}
	else
	{
		mpHitInfo->PlayFrame( "rollOff" );
	}
	
}

/*
========================
idMenuScreen_HUD::ToggleLagged
========================
*/
void idMenuScreen_HUDLocal::ToggleLagged( bool show )
{

	if( !mpConnection )
	{
		return;
	}
	
	mpConnection->SetVisible( show );
}

/*
========================
idMenuScreen_HUD::UpdateGameTime
========================
*/
void idMenuScreen_HUDLocal::UpdateGameTime( const char* time )
{

	if( !mpTime )
	{
		return;
	}
	
	UpdateMessage( false, "" );
	
	mpTime->SetText( time );
	mpTime->SetStrokeInfo( true, 0.75f, 1.5f );
	
}

/*
========================
idMenuScreen_HUD::UpdateMessage
========================
*/
void idMenuScreen_HUDLocal::UpdateMessage( bool show, const idStr& message )
{

	if( !mpMessage )
	{
		return;
	}
	
	if( show )
	{
		if( mpTime )
		{
			mpTime->SetText( "" );
		}
		
		mpMessage->SetText( message );
		mpMessage->SetStrokeInfo( true, 0.75f, 1.5f );
	}
	else
	{
		mpMessage->SetText( "" );
	}
	
}

/*
========================
idMenuScreen_HUD::ShowNewItem
========================
*/
void idMenuScreen_HUDLocal::ShowNewItem( const char* name, const char* icon )
{

	if( !newItem )
	{
		return;
	}
	
	newItem->SetVisible( true );
	newItem->PlayFrame( "rollOn" );
	
	idSWFSpriteInstance* topImg = newItem->GetScriptObject()->GetNestedSprite( "info", "topImg" );
	idSWFSpriteInstance* botImg = newItem->GetScriptObject()->GetNestedSprite( "info", "botImg" );
	idSWFTextInstance* heading = newItem->GetScriptObject()->GetNestedText( "info", "txtTitle" );
	idSWFTextInstance* itemName = newItem->GetScriptObject()->GetNestedText( "info", "txtItem" );
	
	const idMaterial* mat = declManager->FindMaterial( icon, false );
	if( topImg != NULL && botImg != NULL && mat != NULL )
	{
		topImg->SetMaterial( mat );
		botImg->SetMaterial( mat );
	}
	
	if( heading != NULL )
	{
		heading->SetText( "#str_02027" );
		heading->SetStrokeInfo( true, 0.75f, 1.5f );
	}
	
	if( itemName != NULL )
	{
		itemName->SetText( name );
		itemName->SetStrokeInfo( true, 0.75f, 1.5f );
	}
	
}

/*
========================
idMenuScreen_HUD::UpdateFlashlight
========================
*/
void idMenuScreen_HUDLocal::UpdateFlashlight( idPlayer* player )
{

	if( !player || !flashlight || game->CheckInCinematic() || this->inCinematic)
	{
		return;
	}
	
	if( player->flashlightBattery != cvarSystem->GetCVarInteger("flashlight_batteryDrainTimeMS") )
	{
		flashlight->StopFrame( 2 );
		flashlight->SetVisible( true );
		idSWFSpriteInstance* batteryLife = flashlight->GetScriptObject()->GetNestedSprite( "info" );
		if( batteryLife )
		{
			float power = ( ( float )player->flashlightBattery / ( float )cvarSystem->GetCVarInteger("flashlight_batteryDrainTimeMS")) * 100.0f;
			batteryLife->StopFrame( power );
		}
	}
	else
	{
		flashlight->StopFrame( 1 );
	}
	
}

/*
========================
idMenuScreen_HUD::UpdateChattingHud
========================
*/
void idMenuScreen_HUDLocal::UpdateChattingHud( idPlayer* player )
{

	if( !mpChatObject || !GetSWFObject() )
	{
		return;
	}
	
	idSWF* gui = GetSWFObject();
	
	if( player->isChatting == 0 )
	{
		if( mpChatObject->GetCurrentFrame() != 1 )
		{
			mpChatObject->StopFrame( 1 );
			gui->ForceInhibitControl( false );
			
			// RB: 64 bit fixes, changed NULL to 0
			gui->SetGlobal( "focusWindow", 0 );
			// RB end
		}
	}
	else
	{
		if( !mpChatObject->IsVisible() )
		{
			mpChatObject->SetVisible( true );
			mpChatObject->PlayFrame( "rollOn" );
			gui->ForceInhibitControl( true );
			
			idSWFTextInstance* txtType = mpChatObject->GetScriptObject()->GetNestedText( "info", "saybox" );
			int length = 0;
			if( txtType )
			{
				if( player->isChatting == 1 )
				{
					txtType->SetText( "#str_swf_talk_all" );
				}
				else if( player->isChatting == 2 )
				{
					txtType->SetText( "#str_swf_talk_team" );
				}
				txtType->SetStrokeInfo( true );
				length = txtType->GetTextLength();
			}
			
			idSWFSpriteInstance* sayBox = mpChatObject->GetScriptObject()->GetNestedSprite( "info", "textEntry" );
			if( sayBox )
			{
				sayBox->SetXPos( length + 10 );
			}
			
			idSWFTextInstance* say = mpChatObject->GetScriptObject()->GetNestedText( "info", "textEntry", "txtVal" );
			if( say != NULL )
			{
				say->SetIgnoreColor( false );
				say->SetText( "" );
				say->SetStrokeInfo( true );
				say->renderMode = SWF_TEXT_RENDER_AUTOSCROLL;
			}
			
			idSWFScriptObject* const sayObj = mpChatObject->GetScriptObject()->GetNestedObj( "info", "textEntry", "txtVal" );
			if( sayObj != NULL )
			{
			
				gui->SetGlobal( "focusWindow", sayObj );
				
				class idPostTextChat : public idSWFScriptFunction_RefCounted
				{
				public:
					idPostTextChat( idPlayer* _player, idSWFTextInstance* _text )
					{
						player = _player;
						text = _text;
					}
					idSWFScriptVar Call( idSWFScriptObject* thisObject, const idSWFParmList& parms )
					{
						if( !player || !text )
						{
							return idSWFScriptVar();
						}
						
						idStr val = text->text;
						val.Replace( "\'", "" );
						val.Replace( "\"", "" );
						idStr command;
						if( player->isChatting == 2 )
						{
							command = va( "sayTeam %s\n", val.c_str() );
						}
						else
						{
							command = va( "say %s\n", val.c_str() );
						}
						
						cmdSystem->BufferCommandText( CMD_EXEC_NOW, command.c_str() );
						
						player->isChatting = 0;
						return idSWFScriptVar();
					}
					idPlayer* player;
					idSWFTextInstance* text;
				};
				
				class idCancelTextChat : public idSWFScriptFunction_RefCounted
				{
				public:
					idCancelTextChat( idPlayer* _player )
					{
						player = _player;
					}
					idSWFScriptVar Call( idSWFScriptObject* thisObject, const idSWFParmList& parms )
					{
						if( !player )
						{
							return idSWFScriptVar();
						}
						
						player->isChatting = 0;
						return idSWFScriptVar();
					}
					idPlayer* player;
				};
				
				sayObj->Set( "onPress", new( TAG_SWF ) idPostTextChat( player, say ) );
				
				idSWFScriptObject* const shortcutKeys = gui->GetGlobal( "shortcutKeys" ).GetObject();
				if( verify( shortcutKeys != NULL ) )
				{
					shortcutKeys->Set( "ENTER", sayObj );
					shortcutKeys->Set( "ESCAPE", new( TAG_SWF ) idCancelTextChat( player ) );
				}
			}
		}
	}
}

void idMenuScreen_HUDLocal::HideAmmoCount() {
	if (ammoInfo != NULL) {
		ammoInfo->SetVisible(false);
	}
}

const char*		idMenuScreen_HUDLocal::GetlocationName() {
	if (locationName == nullptr) {
		return NULL;
	}
	return locationName->text.c_str();
}

void	idMenuScreen_HUDLocal::setCaption(idStr caption, idVec4 color, int priority, idStr shaderName) {
	if (subtitles != NULL) {
		
		if (!subtitles->IsVisible()) {
			subtitles->SetVisible(true);
			subtitles->PlayFrame("rollOn");
		}
		else {
			if (com_debugCaptions.GetBool()) {
				common->Printf("\nNew Caption priority: %d, Old Caption Priority: %d\n", priority, subtitlePriority);
			}
			if (priority > subtitlePriority) {
				return;
			}
		}
		subtitlesText->SetText(caption);
		swfColorRGBA_t textColor;
		textColor.r = color.x;
		textColor.g = color.y;
		textColor.b = color.z;
		textColor.a = color.w;
		subtitlesText->color = textColor;
		subtitlePriority = priority;
		subtitleShaderName = shaderName;

	}
}

bool idMenuScreen_HUDLocal::hasCaption()
{
	return subtitles->IsVisible();
}

void	idMenuScreen_HUDLocal::clearCaption(idStr shaderName) {
	bool mustClear = subtitles != NULL && !shaderName.Icmp(this->subtitleShaderName);
	if (mustClear) {
		subtitles->PlayFrame("rollOff");
		subtitlesText->SetText("");
		subtitlePriority = 1000;
	}
}

void idMenuScreen_HUDLocal::setCinematic(bool value)
{
	if (this->inCinematic == value) {
		return;
	}
	// subtitles->PlayFrame("rollOff");
	// subtitlesText->SetText("");
	// subtitlePriority = 1000;
	this->inCinematic = value;
	if (stamina != NULL) {
		stamina->GetSprite()->SetVisible(!value);
		stamina->GetSprite()->PlayFrame(value ? "rollOn" : "rollOff");
	}
	if (playerInfo != NULL) {
		playerInfo->GetSprite()->SetVisible(!value);
	}
	if (tipInfo != NULL) {
		tipInfo->GetSprite()->SetVisible(!value);
	}
	if (location != NULL && value) {
		location->SetVisible(!value);
	}
	if (security != NULL) {
		security->SetVisible(!value);
	}
	if (securityText != NULL && value) {
		securityText->SetText("");
	}
	if (newPDADownload != NULL) {
		newPDADownload->SetVisible(!value);
	}
	if (newPDAName != NULL && value) {
		newPDAName->SetText("");
	}
	if (newPDAHeading != NULL && value) {
		newPDAHeading->SetText("");
	}
	if (newPDA != NULL) {
		newPDA->SetVisible(!value);
	}
	if (newVideoDownload != NULL) {
		newVideoDownload->SetVisible(!value);
	}
	if (newVideo != NULL) {
		newVideo->SetVisible(!value);
	}
	if (audioLog != NULL) {
		audioLog->SetVisible(false);
	}
	if (communication != NULL) {
		communication->SetVisible(false);
	}
	if (oxygen->IsVisible() == true) {
		oxygen->SetVisible(false);
	}
	if (flashlight != NULL) {
		flashlight->SetVisible(!value);
	}
	if (objective != NULL) {
		objective->SetVisible(!value);
	}
	if (objectiveComplete != NULL) {
		objectiveComplete->SetVisible(!value);
	}
	if (ammoInfo != NULL) {
		ammoInfo->SetVisible(!value);
	}
	if (bsInfo != NULL) {
		bsInfo->SetVisible(!value);
	}
	if (soulcubeInfo != NULL) {
		soulcubeInfo->SetVisible(!value);
	}
	if (weaponImg != NULL) {
		weaponImg->SetVisible(!value);
	}
	if (newWeapon != NULL) {
		newWeapon->SetVisible(!value);
	}
	if (pickupInfo != NULL) {
		pickupInfo->SetVisible(!value);
	}
	if (newItem != NULL) {
		newItem->SetVisible(!value);
	}
	if (combatCursor != NULL) {
		combatCursor->SetVisible(!value);
	}
	if (talkCursor != NULL && value) {
		talkCursor->SetVisible(!value);
	}
	if (grabberCursor != NULL && value) {
		grabberCursor->SetVisible(!value);
	}
	if (weaponPills != NULL) {
		weaponPills->GetSprite()->SetVisible(!value);
		if (game->GetLocalPlayer()->inventory.weapons == 1) {
			weaponPills->GetSprite()->SetVisible(false);
		}
	}
	if (weaponName != NULL) {
		weaponName->GetSprite()->SetVisible(!value);
	}
	if (newVideoHeading != NULL && value) {
		newVideoHeading->SetText("");
	}
}
