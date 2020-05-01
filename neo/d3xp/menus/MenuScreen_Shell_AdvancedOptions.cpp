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
#pragma hdrstop
#include "precompiled.h"
#include "../Game_local.h"

const static int NUM_ADVANCED_OPTIONS_OPTIONS = 8;

extern idCVar r_useShadowMapping;
extern idCVar r_shadowMapLodScale;
extern idCVar r_useHDR;
extern idCVar r_hdrAutoExposure;
extern idCVar r_useSSAO;
extern idCVar r_useFilmicPostProcessEffects;
//extern idCVar flashlight_old;
//extern idCVar pm_vmfov;


static bool hdrChanged = 0;

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
	control->SetLabel( "#str_shadow_mapping" ); //Soft Shadows
	control->SetDataSource( &advData, idMenuDataSource_AdvancedSettings::ADV_FIELD_SHADOWMAPPING );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedSettings::ADV_FIELD_SHADOWMAPPING);
	options->AddChild( control );

	control = new(TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType(OPTION_SLIDER_BAR);
	control->SetLabel("#str_shadow_mapping_lod"); //Soft ShadowsLOD
	control->SetDataSource(&advData, idMenuDataSource_AdvancedSettings::ADV_FIELD_SHADOWMAPLOD);
	control->SetupEvents(DEFAULT_REPEAT_TIME, options->GetChildren().Num());
	control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedSettings::ADV_FIELD_SHADOWMAPLOD);
	options->AddChild(control);

	control = new(TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType(OPTION_SLIDER_TEXT);
	control->SetLabel("#str_hdr"); //HDR
	control->SetDataSource(&advData, idMenuDataSource_AdvancedSettings::ADV_FIELD_HDR);
	control->SetupEvents(DEFAULT_REPEAT_TIME, options->GetChildren().Num());
	control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedSettings::ADV_FIELD_HDR);
	options->AddChild(control);

	control = new( TAG_SWF ) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_TEXT );
	control->SetLabel( "#str_adaptive_hdr" ); //Adaptive Tonemapping HDR
	control->SetDataSource( &advData, idMenuDataSource_AdvancedSettings::ADV_FIELD_ATHDR );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedSettings::ADV_FIELD_ATHDR );
	options->AddChild( control );
	
	control = new( TAG_SWF ) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_TEXT );
	control->SetLabel( "#str_ssao" ); //SSAO
	control->SetDataSource( &advData, idMenuDataSource_AdvancedSettings::ADV_FIELD_SSAO );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedSettings::ADV_FIELD_SSAO );
	options->AddChild( control );
	
	control = new( TAG_SWF ) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_TEXT );
	control->SetLabel( "#str_filmic" ); //Filmic Post Process effect
	control->SetDataSource( &advData, idMenuDataSource_AdvancedSettings::ADV_FIELD_FPPE );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedSettings::ADV_FIELD_FPPE );
	options->AddChild( control );
	
	if (!game->GetLocalPlayer()) {
		control = new(TAG_SWF) idMenuWidget_ControlButton();
		control->SetOptionType(OPTION_SLIDER_TEXT);
		control->SetLabel("#str_flashlight");	// Flashlight
		control->SetDataSource(&advData, idMenuDataSource_AdvancedSettings::ADV_FIELD_FLASH);
		control->SetupEvents(2, options->GetChildren().Num());
		control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedSettings::ADV_FIELD_FLASH);
		options->AddChild(control);
	}
	control = new(TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType(OPTION_SLIDER_BAR);
	control->SetLabel("#str_pm_fov"); //Player Model FOV
	control->SetDataSource(&advData, idMenuDataSource_AdvancedSettings::ADV_FIELD_VMFOV);
	control->SetupEvents(DEFAULT_REPEAT_TIME, options->GetChildren().Num());
	control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedSettings::ADV_FIELD_VMFOV);
	options->AddChild(control);
	
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
			if((!idLib::newd3 && menuData->GetPlatform() != 2) || (idLib::newd3 && menuData->GetPlatform() != 5))
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
	int index = widget->GetDataSourceFieldIndex();
	
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
			switch (index) {
				case idMenuDataSource_AdvancedSettings::ADV_FIELD_HDR:
				{
					if (hdrChanged) {
						hdrChanged = false;
						return false;
					}
					hdrChanged = true;
				}
			}
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
			if( parms.Num() > 0 )
			{
				selectionIndex = parms[0].ToInteger();
			}
			
			if( options && selectionIndex != options->GetFocusIndex() )
			{
				options->SetViewIndex( options->GetViewOffset() + selectionIndex );
				options->SetFocusIndex( selectionIndex );
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
	originalShadowMapping = r_useShadowMapping.GetInteger();
	originalHDR = r_useHDR.GetInteger();
	originalATHDR = r_hdrAutoExposure.GetInteger();
	originalSSAO = r_useSSAO.GetInteger();
	originalFilmic = r_useFilmicPostProcessEffects.GetInteger();
	originalFlashlight = game->GetCVarInteger("flashlight_old");
	originalVmfov = game->GetCVarInteger("pm_vmfov");
	originalShadowMapLod = r_shadowMapLodScale.GetFloat();
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
	switch( fieldIndex )
	{
		case ADV_FIELD_SHADOWMAPPING:
		{
			r_useShadowMapping.SetBool(!r_useShadowMapping.GetBool());
			break;
		}
		case ADV_FIELD_SHADOWMAPLOD:
		{
			r_shadowMapLodScale.SetFloat(r_shadowMapLodScale.GetFloat() + adjustAmount * 0.1f);
			break;
		}
		case ADV_FIELD_HDR:
		{
			r_useHDR.SetBool(!r_useHDR.GetBool());
			break;
		}
		case ADV_FIELD_ATHDR:
		{
			r_hdrAutoExposure.SetBool( !r_hdrAutoExposure.GetBool() );
			break;
		}
		case ADV_FIELD_SSAO:
		{
			r_useSSAO.SetBool( !r_useSSAO.GetBool() );
			break;
		}
		case ADV_FIELD_FPPE:
		{
			r_useFilmicPostProcessEffects.SetBool( !r_useFilmicPostProcessEffects.GetBool() );
			break;
		}
		case ADV_FIELD_FLASH:
		{
			if (game->GetCVarInteger("flashlight_old") == 2) {
				game->SetCVarInteger("flashlight_old",idMath::ClampInt(0, 2, game->GetCVarInteger("flashlight_old") - 2));
			}
			else {
				game->SetCVarInteger("flashlight_old",idMath::ClampInt(0, 2, game->GetCVarInteger("flashlight_old") + 1));
			}
			break;
		}
		case ADV_FIELD_VMFOV:
		{
			game->SetCVarInteger("pm_vmfov", game->GetCVarInteger("pm_vmfov") + adjustAmount);
			break;
		}
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
		case ADV_FIELD_SHADOWMAPPING:
		{
			if (r_useShadowMapping.GetInteger() == 1)
			{
				return "#str_swf_enabled";
			}
			else
			{
				return "#str_swf_disabled";
			}
		}
		case ADV_FIELD_SHADOWMAPLOD:
		{
			return ReLinearAdjust(r_shadowMapLodScale.GetFloat(), 0.1f, 2.0f, 0.0f, 100.0f);
		}
		case ADV_FIELD_HDR:
		{
			if (r_useHDR.GetInteger() == 1)
			{
				return "#str_swf_enabled";
			}
			else
			{
				return "#str_swf_disabled";
			}
		}
		case ADV_FIELD_ATHDR:
		{
			if (r_hdrAutoExposure.GetInteger() == 1)
			{
				return "#str_swf_enabled";
			}
			else
			{
				return "#str_swf_disabled";
			}
		}
		case ADV_FIELD_SSAO:
		{
			if (r_useSSAO.GetInteger() == 1)
			{
				return "#str_swf_enabled";
			}
			else
			{
				return "#str_swf_disabled";
			}
		}
		case ADV_FIELD_FPPE:
		{
			if (r_useFilmicPostProcessEffects.GetInteger() == 1)
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
	if( originalShadowMapping != r_useShadowMapping.GetInteger() )
	{
		return true;
	}
	if (originalShadowMapLod != r_shadowMapLodScale.GetFloat()) {
		return true;
	}
	if( originalHDR != r_useHDR.GetInteger() )
	{
		return true;
	}
	if( originalATHDR != r_hdrAutoExposure.GetInteger() )
	{
		return true;
	}
	if( originalSSAO != r_useSSAO.GetInteger() )
	{
		return true;
	}
	if( originalFilmic != r_useFilmicPostProcessEffects.GetInteger() )
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
	return false;
}
