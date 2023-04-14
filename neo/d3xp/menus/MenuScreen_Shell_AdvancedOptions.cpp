/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2017-2018 George Kalampokis

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

const static int NUM_ADVANCED_OPTIONS_OPTIONS = 8;

//extern idCVar flashlight_old;
//extern idCVar pm_vmfov;

extern idCVar sys_lang;
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
extern idCVar s_useXAudio2;
#endif

/*
========================
idMenuScreen_Shell_AdvancedOptions::Initialize
========================
*/
void idMenuScreen_Shell_AdvancedOptions::Initialize( idMenuHandler* data )
{
	idMenuScreen::Initialize( data );
	
	if( data != NULL )
	{
		menuGUI = data->GetGUI();
	}
	
	SetSpritePath( "menuSystemOptions" );
	
	options = new( TAG_SWF ) idMenuWidget_DynamicList();
	options->SetNumVisibleOptions( NUM_ADVANCED_OPTIONS_OPTIONS );
	options->SetSpritePath( GetSpritePath(), "info", "options" );
	options->SetWrappingAllowed( true );
	options->SetControlList( true );
	options->Initialize( data );
	
	btnBack = new( TAG_SWF ) idMenuWidget_Button();
	btnBack->Initialize( data );
	btnBack->SetLabel( "#str_swf_settings" );
	btnBack->SetSpritePath( GetSpritePath(), "info", "btnBack" );
	btnBack->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_GO_BACK );
	
	AddChild( options );
	AddChild( btnBack );
	
	idMenuWidget_ControlButton* control;

	control = new( TAG_SWF ) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_TEXT );
	control->SetLabel( "#str_damage_motion" ); //Damage Motion (Adaptive tone mapping HDR was depedent on HDR and not causing huge performace issues)
	control->SetDataSource( &advData, idMenuDataSource_AdvancedSettings::ADV_FIELD_DAMMOT );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedSettings::ADV_FIELD_DAMMOT );
	options->AddChild( control );
	
	control = new(TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType(OPTION_SLIDER_TEXT);
	control->SetLabel("#str_flashlight");	// Flashlight
	control->SetDataSource(&advData, idMenuDataSource_AdvancedSettings::ADV_FIELD_FLASH);
	control->SetDisabled(game->GetLocalPlayer());
	if (!game->GetLocalPlayer()) {
		control->SetupEvents(2, options->GetChildren().Num());
		control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedSettings::ADV_FIELD_FLASH);
	}
	options->AddChild(control);

	control = new(TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType(OPTION_SLIDER_BAR);
	control->SetLabel("#str_pm_fov"); //Player Model FOV
	control->SetDataSource(&advData, idMenuDataSource_AdvancedSettings::ADV_FIELD_VMFOV);
	control->SetupEvents(DEFAULT_REPEAT_TIME, options->GetChildren().Num());
	control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedSettings::ADV_FIELD_VMFOV);
	options->AddChild(control);

	control = new(TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType(OPTION_SLIDER_TEXT);
	control->SetLabel("#str_FPS_counter");	// FPS Counter
	control->SetDataSource(&advData, idMenuDataSource_AdvancedSettings::ADV_FIELD_FPS);
	control->SetupEvents(2, options->GetChildren().Num());
	control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedSettings::ADV_FIELD_FPS);
	options->AddChild(control);

	if (Sys_NumLangs() > 1) {
		control = new(TAG_SWF) idMenuWidget_ControlButton();
		control->SetOptionType(OPTION_SLIDER_TEXT);
		control->SetLabel("#str_lang_menu");	// Language
		control->SetDataSource(&advData, idMenuDataSource_AdvancedSettings::ADV_FIELD_LANG);
		control->SetupEvents(2, options->GetChildren().Num());
		control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedSettings::ADV_FIELD_LANG);
		options->AddChild(control);
	}

	control = new(TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType(OPTION_SLIDER_TEXT);
	control->SetLabel("#str_smart_hud"); //Smart HUD: Hide ammo count if it's visible on the weapon's gui
	control->SetDataSource(&advData, idMenuDataSource_AdvancedSettings::ADV_FIELD_SMHUD);
	control->SetupEvents(2, options->GetChildren().Num());
	control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedSettings::ADV_FIELD_SMHUD);
	options->AddChild(control);

#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	control = new(TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType(OPTION_SLIDER_TEXT);
	control->SetLabel("#str_sapi"); //Audio API (Windows ONLY)
	control->SetDataSource(&advData, idMenuDataSource_AdvancedSettings::ADV_FIELD_SAPI);
	control->SetupEvents(2, options->GetChildren().Num());
	control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedSettings::ADV_FIELD_SAPI);
	options->AddChild(control);
#endif

	options->AddEventAction( WIDGET_EVENT_SCROLL_DOWN ).Set( new( TAG_SWF ) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_SCROLL_DOWN_START_REPEATER, WIDGET_EVENT_SCROLL_DOWN ) );
	options->AddEventAction( WIDGET_EVENT_SCROLL_UP ).Set( new( TAG_SWF ) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_SCROLL_UP_START_REPEATER, WIDGET_EVENT_SCROLL_UP ) );
	options->AddEventAction( WIDGET_EVENT_SCROLL_DOWN_RELEASE ).Set( new( TAG_SWF ) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_STOP_REPEATER, WIDGET_EVENT_SCROLL_DOWN_RELEASE ) );
	options->AddEventAction( WIDGET_EVENT_SCROLL_UP_RELEASE ).Set( new( TAG_SWF ) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_STOP_REPEATER, WIDGET_EVENT_SCROLL_UP_RELEASE ) );
	options->AddEventAction( WIDGET_EVENT_SCROLL_DOWN_LSTICK ).Set( new( TAG_SWF ) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_SCROLL_DOWN_START_REPEATER, WIDGET_EVENT_SCROLL_DOWN_LSTICK ) );
	options->AddEventAction( WIDGET_EVENT_SCROLL_UP_LSTICK ).Set( new( TAG_SWF ) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_SCROLL_UP_START_REPEATER, WIDGET_EVENT_SCROLL_UP_LSTICK ) );
	options->AddEventAction( WIDGET_EVENT_SCROLL_DOWN_LSTICK_RELEASE ).Set( new( TAG_SWF ) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_STOP_REPEATER, WIDGET_EVENT_SCROLL_DOWN_LSTICK_RELEASE ) );
	options->AddEventAction( WIDGET_EVENT_SCROLL_UP_LSTICK_RELEASE ).Set( new( TAG_SWF ) idWidgetActionHandler( options, WIDGET_ACTION_EVENT_STOP_REPEATER, WIDGET_EVENT_SCROLL_UP_LSTICK_RELEASE ) );
}

/*
========================
idMenuScreen_Shell_SystemOptions::Update
========================
*/
void idMenuScreen_Shell_AdvancedOptions::Update()
{

	if( menuData != NULL )
	{
		idMenuWidget_CommandBar* cmdBar = menuData->GetCmdBar();
		if( cmdBar != NULL )
		{
			cmdBar->ClearAllButtons();
			idMenuWidget_CommandBar::buttonInfo_t* buttonInfo;
			buttonInfo = cmdBar->GetButton( idMenuWidget_CommandBar::BUTTON_JOY2 );
			if((!common->IsNewDOOM3() && menuData->GetPlatform() != 2) || (common->IsNewDOOM3() && menuData->GetPlatform() != 5))
			{
				buttonInfo->label = "#str_00395";
			}
			buttonInfo->action.Set( WIDGET_ACTION_GO_BACK );
			
			buttonInfo = cmdBar->GetButton( idMenuWidget_CommandBar::BUTTON_JOY1 );
			buttonInfo->action.Set( WIDGET_ACTION_PRESS_FOCUSED );
		}
	}
	
	idSWFScriptObject& root = GetSWFObject()->GetRootObject();
	if( BindSprite( root ) )
	{
		idSWFTextInstance* heading = GetSprite()->GetScriptObject()->GetNestedText( "info", "txtHeading" );
		if( heading != NULL )
		{
			heading->SetText( "#str_advanced_heading" );	// ADVANCED
			heading->SetStrokeInfo( true, 0.75f, 1.75f );
		}
		
		idSWFSpriteInstance* gradient = GetSprite()->GetScriptObject()->GetNestedSprite( "info", "gradient" );
		if( gradient != NULL && heading != NULL )
		{
			gradient->SetXPos( heading->GetTextLength() );
		}
	}
	
	if( btnBack != NULL )
	{
		btnBack->BindSprite( root );
	}
	
	idMenuScreen::Update();
}

/*
========================
idMenuScreen_Shell_SystemOptions::ShowScreen
========================
*/
void idMenuScreen_Shell_AdvancedOptions::ShowScreen( const mainMenuTransition_t transitionType )
{
	advData.LoadData();
	
	idMenuScreen::ShowScreen( transitionType );
}

/*
========================
idMenuScreen_Shell_SystemOptions::HideScreen
========================
*/
void idMenuScreen_Shell_AdvancedOptions::HideScreen( const mainMenuTransition_t transitionType )
{
	if (advData.IsRestartRequired())
	{
		class idSWFScriptFunction_Restart : public idSWFScriptFunction_RefCounted
		{
		public:
			idSWFScriptFunction_Restart(gameDialogMessages_t _msg, bool _restart)
			{
				msg = _msg;
				restart = _restart;
			}
			idSWFScriptVar Call(idSWFScriptObject* thisObject, const idSWFParmList& parms)
			{
				common->Dialog().ClearDialog(msg);
				if (restart)
				{
					// DG: Sys_ReLaunch() doesn't need any options anymore
					//     (the old way would have been unnecessarily painful on POSIX systems)
					Sys_ReLaunch();
					// DG end
				}
				return idSWFScriptVar();
			}
		private:
			gameDialogMessages_t msg;
			bool restart;
		};
		idStaticList<idSWFScriptFunction*, 4> callbacks;
		idStaticList<idStrId, 4> optionText;
		callbacks.Append(new idSWFScriptFunction_Restart(GDM_GAME_RESTART_REQUIRED, false));
		callbacks.Append(new idSWFScriptFunction_Restart(GDM_GAME_RESTART_REQUIRED, true));
		optionText.Append(idStrId("#str_00100113")); // Continue
		optionText.Append(idStrId("#str_02487")); // Restart Now
		common->Dialog().AddDynamicDialog(GDM_GAME_RESTART_REQUIRED, callbacks, optionText, true, idStr());
	}

	if( advData.IsDataChanged() )
	{
		advData.CommitData();
	}
	
	idMenuScreen::HideScreen( transitionType );
}

/*
========================
idMenuScreen_Shell_SystemOptions::HandleAction h
========================
*/
bool idMenuScreen_Shell_AdvancedOptions::HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled )
{

	if( menuData == NULL )
	{
		return true;
	}
	
	if( menuData->ActiveScreen() != SHELL_AREA_ADVANCED )
	{
		return false;
	}

	bool updateUi = true;
	
	widgetAction_t actionType = action.GetType();
	const idSWFParmList& parms = action.GetParms();
	//int index = widget->GetDataSourceFieldIndex();
	
	switch( actionType )
	{
		case WIDGET_ACTION_GO_BACK:
		{
			if( menuData != NULL )
			{
				menuData->SetNextScreen( SHELL_AREA_SETTINGS, MENU_TRANSITION_SIMPLE );
			}
			return true;
		}
		case WIDGET_ACTION_ADJUST_FIELD:
		{
			updateUi = false;
			break;
		}
		case WIDGET_ACTION_COMMAND:
		{
		
			if( options == NULL )
			{
				return true;
			}
			
			int selectionIndex = options->GetFocusIndex();
			if (selectionIndex <= 3) {
				if (parms.Num() > 0)
				{
					selectionIndex = parms[0].ToInteger();
				}

				if (options && selectionIndex != options->GetFocusIndex())
				{
					options->SetViewIndex(options->GetViewOffset() + selectionIndex);
					options->SetFocusIndex(selectionIndex);
				}
			}
			advData.AdjustField( parms[0].ToInteger(), 1 );
			options->Update();
			
			return true;
		}
		case WIDGET_ACTION_START_REPEATER:
		{
		
			if( options == NULL )
			{
				return true;
			}
			
			if( parms.Num() == 4 )
			{
				int selectionIndex = parms[3].ToInteger();
				if( selectionIndex != options->GetFocusIndex() )
				{
					options->SetViewIndex( options->GetViewOffset() + selectionIndex );
					options->SetFocusIndex( selectionIndex );
				}
			}
			updateUi = false;
			break;
		}
	}

	if (updateUi) {
		this->Update();
	}
	
	return idMenuWidget::HandleAction( action, event, widget, forceHandled );
}

/////////////////////////////////
// SCREEN SETTINGS
/////////////////////////////////

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::idMenuDataSource_SystemSettings
========================
*/
idMenuScreen_Shell_AdvancedOptions::idMenuDataSource_AdvancedSettings::idMenuDataSource_AdvancedSettings()
{
}

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::LoadData
========================
*/
void idMenuScreen_Shell_AdvancedOptions::idMenuDataSource_AdvancedSettings::LoadData()
{
	originalATHDR = game->GetCVarInteger("g_damageKickEffect");
	originalFlashlight = game->GetCVarInteger("flashlight_old");
	originalVmfov = game->GetCVarInteger("pm_vmfov");
	originalFPS = com_showFPS.GetInteger();
	originalLang = sys_lang.GetString();
	originalSMHUD = game->GetCVarInteger("pm_smartHUD");
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	originalSAPI = s_useXAudio2.GetInteger();
#endif
}

bool idMenuScreen_Shell_AdvancedOptions::idMenuDataSource_AdvancedSettings::IsRestartRequired() const
{
	if (idStr::Icmp(originalLang, sys_lang.GetString()) != 0) {
		return true;
	}
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (originalSAPI != s_useXAudio2.GetInteger()) {
		return true;
	}
#endif
	return false;
}

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::CommitData
========================
*/
void idMenuScreen_Shell_AdvancedOptions::idMenuDataSource_AdvancedSettings::CommitData()
{
	cvarSystem->SetModifiedFlags( CVAR_ARCHIVE );
}

/*
========================
AdjustOption
Given a current value in an array of possible values, returns the next n value
========================
*/
int idMenuScreen_Shell_AdvancedOptions::idMenuDataSource_AdvancedSettings::AdjustOption(int currentValue, const int values[], int numValues, int adjustment)
{
	int index = 0;
	for (int i = 0; i < numValues; i++)
	{
		if (currentValue == values[i])
		{
			index = i;
			break;
		}
	}
	index += adjustment;
	while (index < 0)
	{
		index += numValues;
	}
	index %= numValues;
	return values[index];
}

/*
========================
ReLinearAdjust
Linearly converts a float from one scale to another
========================
*/
float ReLinearAdjust(float input, float currentMin, float currentMax, float desiredMin, float desiredMax)
{
	return ((input - currentMin) / (currentMax - currentMin)) * (desiredMax - desiredMin) + desiredMin;
}

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::AdjustField
========================
*/
void idMenuScreen_Shell_AdvancedOptions::idMenuDataSource_AdvancedSettings::AdjustField( const int fieldIndex, const int adjustAmount )
{
	static const int genericNumValues = 2;
	static const int genericValues[genericNumValues] = { 0, 1 };
	static const int specializedNumValues = 3;
	static const int specializedValues[specializedNumValues] = { 0, 1, 2 };
	switch( fieldIndex )
	{
		case ADV_FIELD_DAMMOT:
		{
			game->SetCVarBool("g_damageKickEffect", AdjustOption(game->GetCVarBool("g_damageKickEffect"), genericValues, genericNumValues, adjustAmount));
			break;
		}
		case ADV_FIELD_FLASH:
		{
			game->SetCVarInteger("flashlight_old", AdjustOption(game->GetCVarInteger("flashlight_old"), specializedValues, specializedNumValues, adjustAmount));
			/*if (game->GetCVarInteger("flashlight_old") == 2) {
				game->SetCVarInteger("flashlight_old",idMath::ClampInt(0, 2, game->GetCVarInteger("flashlight_old") - 2));
			}
			else {
				game->SetCVarInteger("flashlight_old",idMath::ClampInt(0, 2, game->GetCVarInteger("flashlight_old") + 1));
			}*/
			break;
		}
		case ADV_FIELD_VMFOV:
		{
			game->SetCVarInteger("pm_vmfov", game->GetCVarInteger("pm_vmfov") + adjustAmount);
			break;
		}
		case ADV_FIELD_FPS:
			com_showFPS.SetInteger(AdjustOption(com_showFPS.GetInteger(), specializedValues, specializedNumValues, adjustAmount));
			break;
		case ADV_FIELD_SMHUD:
		{
			game->SetCVarBool("pm_smartHUD", AdjustOption(game->GetCVarBool("pm_smartHUD"), genericValues, genericNumValues, adjustAmount));
			break;
		}
		case ADV_FIELD_LANG:
		{
			idList<int> langValues;
			langValues.Clear();

			for (int i = 0; i < Sys_NumLangs(); i++) {
				langValues.AddUnique(i);
			}
			sys_lang.SetString(Sys_Lang(AdjustOption(Sys_LangIndex(sys_lang.GetString()), langValues.Ptr(), Sys_NumLangs(), adjustAmount)));
			break;
		}
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
		case ADV_FIELD_SAPI:
			s_useXAudio2.SetBool(AdjustOption(s_useXAudio2.GetBool(), genericValues, genericNumValues, adjustAmount));
			break;
#endif
		
	}
	cvarSystem->ClearModifiedFlags( CVAR_ARCHIVE );
}

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::GetField
========================
*/
idSWFScriptVar idMenuScreen_Shell_AdvancedOptions::idMenuDataSource_AdvancedSettings::GetField( const int fieldIndex ) const
{
	switch( fieldIndex )
	{
		case ADV_FIELD_DAMMOT:
		{
			if (game->GetCVarInteger("g_damageKickEffect") == 1)
			{
				return "#str_swf_enabled";
			}
			else
			{
				return "#str_swf_disabled";
			}
		}
		case ADV_FIELD_FLASH:
			switch (game->GetCVarInteger("flashlight_old"))
			{
			case 2:
				return "#str_flashlight_bfg_mix";
			case 1:
				return "#str_flashlight_og";
			case 0:
				return "#str_flashlight_bfg";
			}
		case ADV_FIELD_VMFOV:
			return ReLinearAdjust(game->GetCVarInteger("pm_vmfov"), 0.0f, 64.0f, 0.0f, 100.0f);
		case ADV_FIELD_FPS:
			switch (com_showFPS.GetInteger()) {
			case 2:
				return "#str_FPS_only";
			case 1:
				return "#str_FPS_all";
			case 0:
				return "#str_swf_disabled";
			}
		case ADV_FIELD_LANG:
			return va("#str_lang_%s", sys_lang.GetString());
		case ADV_FIELD_SMHUD:
		{
			if (game->GetCVarInteger("pm_smartHUD") == 1)
			{
				return "#str_swf_enabled";
			}
			else
			{
				return "#str_swf_disabled";
			}
		}
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
		case ADV_FIELD_SAPI:
		{
			return s_useXAudio2.GetBool() ? "#str_swf_xaudio" : "#str_swf_openAL";
		}
#endif
	}
	return false;
}

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::IsDataChanged
========================
*/
bool idMenuScreen_Shell_AdvancedOptions::idMenuDataSource_AdvancedSettings::IsDataChanged() const
{
	if( originalATHDR != game->GetCVarInteger("g_damageKickEffect"))
	{
		return true;
	}
	if( originalFlashlight != game->GetCVarInteger("flashlight_old"))
	{
		return true;
	}
	if (originalVmfov != game->GetCVarInteger("pm_vmfov"))
	{
		return true;
	}
	if (originalFPS != com_showFPS.GetInteger()) {
		return true;
	}
	if (idStr::Icmp(originalLang, sys_lang.GetString()) != 0) {
		return true;
	}
	if (originalSMHUD != game->GetCVarInteger("pm_smartHUD"))
	{
		return true;
	}
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (originalSAPI != s_useXAudio2.GetInteger()) {
		return true;
	}
#endif
	return false;
}
