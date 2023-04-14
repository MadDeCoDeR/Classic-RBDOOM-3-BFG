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
#include "../Game_local.h"

const static int NUM_CONTROLS_OPTIONS = 8;

enum advControlsMenuCmds_t
{
	ADV_CONTROLS_CMD_CROSSHAIR,
	ADV_CONTROLS_CMD_CPOS,
	ADV_CONTROLS_CMD_TOGGLE_RUN,
	ADV_CONTROLS_CMD_TOGGLE_CROUCH,
	ADV_CONTROLS_CMD_TOGGLE_ZOOM
};

/*
========================
idMenuScreen_Shell_Adv_Controls::Initialize
========================
*/
void idMenuScreen_Shell_Adv_Controls::Initialize( idMenuHandler* data )
{
	idMenuScreen::Initialize( data );
	
	if( data != NULL )
	{
		menuGUI = data->GetGUI();
	}
	
	SetSpritePath( "menuControls" );
	
	options = new( TAG_SWF ) idMenuWidget_DynamicList();
	options->SetNumVisibleOptions( NUM_CONTROLS_OPTIONS );
	options->SetSpritePath( GetSpritePath(), "info", "options" );
	options->SetWrappingAllowed( true );
	options->SetControlList( true );
	options->Initialize( data );
	AddChild( options );
	
	idMenuWidget_Help* const helpWidget = new( TAG_SWF ) idMenuWidget_Help();
	helpWidget->SetSpritePath( GetSpritePath(), "info", "helpTooltip" );
	AddChild( helpWidget );
	
	btnBack = new( TAG_SWF ) idMenuWidget_Button();
	btnBack->Initialize( data );
	idStr controls( idLocalization::GetString( "#str_04158" ) );
	controls.ToUpper();
	btnBack->SetLabel( controls );
	btnBack->SetSpritePath( GetSpritePath(), "info", "btnBack" );
	btnBack->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_GO_BACK );
	AddChild( btnBack );
	
	idMenuWidget_ControlButton* control;
	
	control = new( TAG_SWF ) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_TOGGLE );
	control->SetLabel( "#str_swf_crosshair" );
	control->SetDataSource( &controlData, idMenuDataSource_Adv_ControlSettings::ADV_CONTROLS_FIELD_CROSSHAIR );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, ADV_CONTROLS_CMD_CROSSHAIR );
	control->RegisterEventObserver( helpWidget );
	options->AddChild( control );
	
	control = new( TAG_SWF ) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_TOGGLE );
	control->SetLabel( "#str_swf_classic_pose" );
	control->SetDataSource( &controlData, idMenuDataSource_Adv_ControlSettings::ADV_CONTROLS_FIELD_CPOS );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, ADV_CONTROLS_CMD_CPOS );
	control->RegisterEventObserver( helpWidget );
	options->AddChild( control );
	
	control = new( TAG_SWF ) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_TOGGLE );
	control->SetLabel( "#str_swf_toggle_run" );
	control->SetDataSource( &controlData, idMenuDataSource_Adv_ControlSettings::ADV_CONTROLS_FIELD_TOGGLE_RUN );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, ADV_CONTROLS_CMD_TOGGLE_RUN );
	control->RegisterEventObserver( helpWidget );
	options->AddChild( control );

	control = new(TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType(OPTION_SLIDER_TOGGLE);
	control->SetLabel("#str_swf_toggle_crouch");
	control->SetDataSource(&controlData, idMenuDataSource_Adv_ControlSettings::ADV_CONTROLS_FIELD_TOGGLE_CROUCH);
	control->SetupEvents(DEFAULT_REPEAT_TIME, options->GetChildren().Num());
	control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, ADV_CONTROLS_CMD_TOGGLE_CROUCH);
	control->RegisterEventObserver(helpWidget);
	options->AddChild(control);

	control = new(TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType(OPTION_SLIDER_TOGGLE);
	control->SetLabel("#str_swf_toggle_zoom");
	control->SetDataSource(&controlData, idMenuDataSource_Adv_ControlSettings::ADV_CONTROLS_FIELD_TOGGLE_ZOOM);
	control->SetupEvents(DEFAULT_REPEAT_TIME, options->GetChildren().Num());
	control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, ADV_CONTROLS_CMD_TOGGLE_ZOOM);
	control->RegisterEventObserver(helpWidget);
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
idMenuScreen_Shell_Adv_Controls::Update
========================
*/
void idMenuScreen_Shell_Adv_Controls::Update()
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
			if((!common->IsNewDOOM3() && menuData->GetPlatform() != 2) || (common->IsNewDOOM3() && menuData->GetPlatform() != 5))
			{
				buttonInfo->label = "#str_SWF_SELECT";
			}
			buttonInfo->action.Set( WIDGET_ACTION_PRESS_FOCUSED );
		}
	}
	
	idSWFScriptObject& root = GetSWFObject()->GetRootObject();
	if( BindSprite( root ) )
	{
		idSWFTextInstance* heading = GetSprite()->GetScriptObject()->GetNestedText( "info", "txtHeading" );
		if( heading != NULL )
		{
			heading->SetText( "#str_swf_adv_controls_heading" );	// CONTROLS
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
idMenuScreen_Shell_Adv_Controls::ShowScreen
========================
*/
void idMenuScreen_Shell_Adv_Controls::ShowScreen( const mainMenuTransition_t transitionType )
{
	controlData.LoadData();
	idMenuScreen::ShowScreen( transitionType );
}

/*
========================
idMenuScreen_Shell_Adv_Controls::HideScreen
========================
*/
void idMenuScreen_Shell_Adv_Controls::HideScreen( const mainMenuTransition_t transitionType )
{

	if( controlData.IsDataChanged() )
	{
		controlData.CommitData();
	}
	
	if( menuData != NULL )
	{
		idMenuHandler_Shell* handler = dynamic_cast< idMenuHandler_ShellLocal* >( menuData );
		if( handler != NULL )
		{
			handler->SetupPCOptions();
		}
	}
	
	idMenuScreen::HideScreen( transitionType );
}

/*
========================
idMenuScreen_Shell_Adv_Controls::HandleAction
========================
*/
bool idMenuScreen_Shell_Adv_Controls::HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled )
{

	if( menuData == NULL )
	{
		return true;
	}
	
	if( menuData->ActiveScreen() != SHELL_AREA_ADV_CONTROLS )
	{
		return false;
	}

	bool updateUi = true;
	
	widgetAction_t actionType = action.GetType();
	const idSWFParmList& parms = action.GetParms();
	
	switch( actionType )
	{
		case WIDGET_ACTION_GO_BACK:
		{
			menuData->SetNextScreen( SHELL_AREA_CONTROLS, MENU_TRANSITION_SIMPLE );
			return true;
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
			
			if( selectionIndex != options->GetFocusIndex() )
			{
				options->SetViewIndex( options->GetViewOffset() + selectionIndex );
				options->SetFocusIndex( selectionIndex );
			}
			
			switch( parms[0].ToInteger() )
			{
				case ADV_CONTROLS_CMD_CROSSHAIR:
				{
					controlData.AdjustField( idMenuDataSource_Adv_ControlSettings::ADV_CONTROLS_FIELD_CROSSHAIR, 1 );
					options->Update();
					break;
				}
				case ADV_CONTROLS_CMD_CPOS:
				{
					controlData.AdjustField(idMenuDataSource_Adv_ControlSettings::ADV_CONTROLS_FIELD_CPOS, 1);
					options->Update();
					break;
				}
				case ADV_CONTROLS_CMD_TOGGLE_RUN:
				{
					controlData.AdjustField(idMenuDataSource_Adv_ControlSettings::ADV_CONTROLS_FIELD_TOGGLE_RUN, 1);
					options->Update();
					break;
				}
				case ADV_CONTROLS_CMD_TOGGLE_CROUCH:
				{
					controlData.AdjustField(idMenuDataSource_Adv_ControlSettings::ADV_CONTROLS_FIELD_TOGGLE_CROUCH, 1);
					options->Update();
					break;
				}
				case ADV_CONTROLS_CMD_TOGGLE_ZOOM:
				{
					controlData.AdjustField(idMenuDataSource_Adv_ControlSettings::ADV_CONTROLS_FIELD_TOGGLE_ZOOM, 1);
					options->Update();
					break;
				}
			}
			
			return true;
		}
		case WIDGET_ACTION_ADJUST_FIELD:
		{
			updateUi = false;
			break;
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

extern idCVar in_toggleRun;
extern idCVar in_toggleCrouch;
extern idCVar in_toggleZoom;

/*
========================
idMenuScreen_Shell_Adv_Controls::idMenuDataSource_Adv_ControlSettings::idMenuDataSource_AudioSettings
========================
*/
idMenuScreen_Shell_Adv_Controls::idMenuDataSource_Adv_ControlSettings::idMenuDataSource_Adv_ControlSettings()
{
	fields.SetNum(MAX_ADV_CONTROL_FIELDS);
	originalFields.SetNum(MAX_ADV_CONTROL_FIELDS);
}

/*
========================
idMenuScreen_Shell_Adv_Controls::idMenuDataSource_Adv_ControlSettings::LoadData
========================
*/
void idMenuScreen_Shell_Adv_Controls::idMenuDataSource_Adv_ControlSettings::LoadData()
{
	
	fields[ADV_CONTROLS_FIELD_CROSSHAIR].SetBool(game->GetCVarBool("pm_cursor"));
	fields[ADV_CONTROLS_FIELD_CPOS].SetBool(game->GetCVarBool("pm_classicPose"));
	fields[ADV_CONTROLS_CMD_TOGGLE_RUN].SetBool(in_toggleRun.GetBool());
	fields[ADV_CONTROLS_CMD_TOGGLE_CROUCH].SetBool(in_toggleCrouch.GetBool());
	fields[ADV_CONTROLS_CMD_TOGGLE_ZOOM].SetBool(in_toggleZoom.GetBool());
	
	originalFields = fields;
}

/*
========================
idMenuScreen_Shell_Adv_Controls::idMenuDataSource_Adv_ControlSettings::CommitData
========================
*/
void idMenuScreen_Shell_Adv_Controls::idMenuDataSource_Adv_ControlSettings::CommitData()
{

	game->SetCVarBool("pm_cursor", fields[ADV_CONTROLS_CMD_CROSSHAIR].ToBool());
	game->SetCVarBool("pm_classicPose", fields[ADV_CONTROLS_CMD_CPOS].ToBool());
	in_toggleRun.SetBool(fields[ADV_CONTROLS_CMD_TOGGLE_RUN].ToBool());
	in_toggleCrouch.SetBool(fields[ADV_CONTROLS_CMD_TOGGLE_CROUCH].ToBool());
	in_toggleZoom.SetBool(fields[ADV_CONTROLS_CMD_TOGGLE_ZOOM].ToBool());
	
	cvarSystem->SetModifiedFlags( CVAR_ARCHIVE );
	
	// make the committed fields into the backup fields
	originalFields = fields;
}

/*
========================
idMenuScreen_Shell_Adv_Controls::idMenuDataSource_Adv_ControlSettings::AdjustField
========================
*/
void idMenuScreen_Shell_Adv_Controls::idMenuDataSource_Adv_ControlSettings::AdjustField( const int fieldIndex, const int adjustAmount )
{
	fields[fieldIndex].SetBool(!fields[fieldIndex].ToBool());
	/*
	{
		float newValue = idMath::ClampFloat( 0.0f, 100.0f, fields[ fieldIndex ].ToFloat() + adjustAmount );
		fields[ fieldIndex ].SetFloat( newValue );
	}*/
}

/*
========================
idMenuScreen_Shell_Adv_Controls::idMenuDataSource_Adv_ControlSettings::IsDataChanged
========================
*/
bool idMenuScreen_Shell_Adv_Controls::idMenuDataSource_Adv_ControlSettings::IsDataChanged() const
{

	if( fields[ADV_CONTROLS_CMD_CROSSHAIR].ToBool() != originalFields[ADV_CONTROLS_CMD_CROSSHAIR].ToBool() )
	{
		return true;
	}
	
	if( fields[ADV_CONTROLS_CMD_CPOS].ToBool() != originalFields[ADV_CONTROLS_CMD_CPOS].ToBool() )
	{
		return true;
	}
	
	if( fields[ADV_CONTROLS_CMD_TOGGLE_RUN].ToBool() != originalFields[ADV_CONTROLS_CMD_TOGGLE_RUN].ToBool() )
	{
		return true;
	}
	
	if( fields[ADV_CONTROLS_CMD_TOGGLE_CROUCH].ToFloat() != originalFields[ADV_CONTROLS_CMD_TOGGLE_CROUCH].ToFloat() )
	{
		return true;
	}
	
	if( fields[ADV_CONTROLS_CMD_TOGGLE_ZOOM].ToFloat() != originalFields[ADV_CONTROLS_CMD_TOGGLE_ZOOM].ToFloat() )
	{
		return true;
	}
	
	return false;
}