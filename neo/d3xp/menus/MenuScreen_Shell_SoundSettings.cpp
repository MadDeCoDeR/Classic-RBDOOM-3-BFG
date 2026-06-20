/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2014-2016 Robert Beckebans

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

const static int NUM_SOUND_OPTIONS_OPTIONS = 8;

extern idCVar s_volume_dB;
extern idCVar s_volume_voices;
extern idCVar s_volume_env;
extern idCVar s_volume_weap;
extern idCVar s_volume_self;
extern idCVar s_useEAX;
extern idCVar s_useCC;

/*
========================
idMenuScreen_Shell_SoundOptions::Initialize
========================
*/
void idMenuScreen_Shell_SoundOptions::Initialize( idMenuHandler* data )
{
	common->Printf("Initializing Sound Options\n");
	idMenuScreen::Initialize( data );

	if( data != NULL )
	{
		menuGUI = data->GetGUI();
	}
	
	SetSpritePath( "menuSystemOptions" );
	
	options = new( TAG_SWF ) idMenuWidget_DynamicList();
	options->SetNumVisibleOptions( NUM_SOUND_OPTIONS_OPTIONS );
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
	control->SetOptionType( OPTION_SLIDER_BAR );
	control->SetLabel( "#str_swf_master_volume" );	//Master Volume
	control->SetDataSource( &soundData, idMenuDataSource_SoundSettings::SOUND_FIELD_MASTER );
	control->SetupEvents( 2, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_SoundSettings::SOUND_FIELD_MASTER );
	options->AddChild( control );
	
	control = new( TAG_SWF ) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_BAR );
	control->SetLabel( "#str_swf_voice_volume" );	//Voice Volume
	control->SetDataSource( &soundData, idMenuDataSource_SoundSettings::SOUND_FIELD_VOICES );
	control->SetupEvents( 2, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_SoundSettings::SOUND_FIELD_VOICES );
	options->AddChild( control );

	control = new( TAG_SWF ) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_BAR );
	control->SetLabel( "#str_swf_env_volume" );	//Environment Volume
	control->SetDataSource( &soundData, idMenuDataSource_SoundSettings::SOUND_FIELD_ENV );
	control->SetupEvents( 2, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_SoundSettings::SOUND_FIELD_ENV );
	options->AddChild( control );

	control = new( TAG_SWF ) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_BAR );
	control->SetLabel( "#str_swf_weap_volume" );	//Weapon Volume
	control->SetDataSource( &soundData, idMenuDataSource_SoundSettings::SOUND_FIELD_WEAP );
	control->SetupEvents( 2, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_SoundSettings::SOUND_FIELD_WEAP );
	options->AddChild( control );

	control = new( TAG_SWF ) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_BAR );
	control->SetLabel( "#str_swf_self_volume" );	//Player Volume
	control->SetDataSource( &soundData, idMenuDataSource_SoundSettings::SOUND_FIELD_SELF );
	control->SetupEvents( 2, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_SoundSettings::SOUND_FIELD_SELF );
	options->AddChild( control );

	if (soundSystem->SupportsReverbs()) {
		control = new(TAG_SWF) idMenuWidget_ControlButton();
		control->SetOptionType(OPTION_SLIDER_TEXT);
		control->SetLabel("#str_swf_eax");
		control->SetDataSource(&soundData, idMenuDataSource_SoundSettings::SOUND_FIELD_EAX);
		control->SetupEvents(DEFAULT_REPEAT_TIME, options->GetChildren().Num());
		control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, idMenuDataSource_SoundSettings::SOUND_FIELD_EAX);
		options->AddChild(control);
	}

	if(soundSystem->HasSubtitles()) {
		control = new(TAG_SWF) idMenuWidget_ControlButton();
		control->SetOptionType(OPTION_SLIDER_TEXT);
		control->SetLabel("#str_swf_cc");
		control->SetDataSource(&soundData, idMenuDataSource_SoundSettings::SOUND_FIELD_CC);
		control->SetupEvents(DEFAULT_REPEAT_TIME, options->GetChildren().Num());
		control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, idMenuDataSource_SoundSettings::SOUND_FIELD_CC);
		options->AddChild(control);
	}


	
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
idMenuScreen_Shell_SoundOptions::Update
========================
*/
void idMenuScreen_Shell_SoundOptions::Update()
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
			heading->SetText( "#str_sound_heading" );	// SOUND
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
idMenuScreen_Shell_SoundOptions::ShowScreen
========================
*/
void idMenuScreen_Shell_SoundOptions::ShowScreen( const mainMenuTransition_t transitionType )
{

	soundData.LoadData();
	
	idMenuScreen::ShowScreen( transitionType );
}

/*
========================
idMenuScreen_Shell_SoundOptions::HideScreen
========================
*/
void idMenuScreen_Shell_SoundOptions::HideScreen( const mainMenuTransition_t transitionType )
{

	if( soundData.IsRestartRequired() )
	{
		class idSWFScriptFunction_Restart : public idSWFScriptFunction_RefCounted
		{
		public:
			idSWFScriptFunction_Restart( gameDialogMessages_t _msg, bool _restart )
			{
				msg = _msg;
				restart = _restart;
			}
			idSWFScriptVar Call( idSWFScriptObject* thisObject, const idSWFParmList& parms )
			{
				common->Dialog().ClearDialog( msg );
				if( restart )
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
		callbacks.Append( new idSWFScriptFunction_Restart( GDM_GAME_RESTART_REQUIRED, false ) );
		callbacks.Append( new idSWFScriptFunction_Restart( GDM_GAME_RESTART_REQUIRED, true ) );
		optionText.Append( idStrId( "#str_00100113" ) ); // Continue
		optionText.Append( idStrId( "#str_02487" ) ); // Restart Now
		common->Dialog().AddDynamicDialog( GDM_GAME_RESTART_REQUIRED, callbacks, optionText, true, idStr() );
	}
	
	if( soundData.IsDataChanged() )
	{
		soundData.CommitData();
	}
	
	idMenuScreen::HideScreen( transitionType );
}

/*
========================
idMenuScreen_Shell_SoundOptions::HandleAction h
========================
*/
bool idMenuScreen_Shell_SoundOptions::HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled )
{

	if( menuData == NULL )
	{
		return true;
	}
	
	if( menuData->ActiveScreen() != SHELL_AREA_AUDIO )
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
			if( menuData != NULL )
			{
				menuData->SetNextScreen( SHELL_AREA_SETTINGS, MENU_TRANSITION_SIMPLE );
			}
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
			
			if( options && selectionIndex != options->GetFocusIndex() )
			{
				options->SetViewIndex( options->GetViewOffset() + selectionIndex );
				options->SetFocusIndex( selectionIndex );
			}
			

			soundData.AdjustField( parms[0].ToInteger(), 1 );
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
idMenuScreen_Shell_SoundOptions::idMenuDataSource_SoundSettings::idMenuDataSource_SoundSettings
========================
*/
idMenuScreen_Shell_SoundOptions::idMenuDataSource_SoundSettings::idMenuDataSource_SoundSettings()
{
}

/*
========================
idMenuScreen_Shell_SoundOptions::idMenuDataSource_SoundSettings::LoadData
========================
*/
void idMenuScreen_Shell_SoundOptions::idMenuDataSource_SoundSettings::LoadData()
{
	originalMasterVolume = s_volume_dB.GetFloat();
	originalVoiceVolume = s_volume_voices.GetFloat();
	originalEnvVolume = s_volume_env.GetFloat();
	originalWeapVolume = s_volume_weap.GetFloat();
	originalSelfVolume = s_volume_self.GetFloat();
	originalEAX = s_useEAX.GetBool();
	originalCC = s_useCC.GetBool();
}

/*
========================
idMenuScreen_Shell_SoundOptions::idMenuDataSource_SoundSettings::IsRestartRequired
========================
*/
bool idMenuScreen_Shell_SoundOptions::idMenuDataSource_SoundSettings::IsRestartRequired() const
{
	return false;
}

/*
========================
idMenuScreen_Shell_SoundOptions::idMenuDataSource_SoundSettings::CommitData
========================
*/
void idMenuScreen_Shell_SoundOptions::idMenuDataSource_SoundSettings::CommitData()
{
	cvarSystem->SetModifiedFlags( CVAR_ARCHIVE );
}

/*
========================
AdjustOption
Given a current value in an array of possible values, returns the next n value
========================
*/
int idMenuScreen_Shell_SoundOptions::idMenuDataSource_SoundSettings::AdjustOption( int currentValue, const int values[], int numValues, int adjustment )
{
	int index = 0;
	for( int i = 0; i < numValues; i++ )
	{
		if( currentValue == values[i] )
		{
			index = i;
			break;
		}
	}
	index += adjustment;
	while( index < 0 )
	{
		index += numValues;
	}
	index %= numValues;
	return values[index];
}

/*
========================
LinearAdjust
Linearly converts a float from one scale to another
========================
*/
float idMenuScreen_Shell_SoundOptions::idMenuDataSource_SoundSettings::LinearAdjust( float input, float currentMin, float currentMax, float desiredMin, float desiredMax ) const
{
	return ( ( input - currentMin ) / ( currentMax - currentMin ) ) * ( desiredMax - desiredMin ) + desiredMin;
}

/*
========================
idMenuScreen_Shell_SoundOptions::idMenuDataSource_SoundSettings::AdjustField
========================
*/
void idMenuScreen_Shell_SoundOptions::idMenuDataSource_SoundSettings::AdjustField( const int fieldIndex, const int adjustAmount )
{
	switch( fieldIndex )
	{
	case SOUND_FIELD_MASTER:
	{
		const float percent = 100.0f * Square( 1.0f - ( s_volume_dB.GetFloat() / DB_SILENCE ) );
		const float adjusted = percent + ( float )adjustAmount;
		const float clamped = idMath::ClampFloat( 0.0f, 100.0f, adjusted );
		s_volume_dB.SetFloat( DB_SILENCE - ( idMath::Sqrt( clamped / 100.0f ) * DB_SILENCE ) );
		break;
	}
	case SOUND_FIELD_VOICES:
	{
		const float percent = 100.0f * Square( 1.0f - ( s_volume_voices.GetFloat() / DB_SILENCE ) );
		const float adjusted = percent + ( float )adjustAmount;
		const float clamped = idMath::ClampFloat( 0.0f, 100.0f, adjusted );
		s_volume_voices.SetFloat( DB_SILENCE - ( idMath::Sqrt( clamped / 100.0f ) * DB_SILENCE ) );
		break;
	}
	case SOUND_FIELD_ENV:
	{
		const float percent = 100.0f * Square( 1.0f - ( s_volume_env.GetFloat() / DB_SILENCE ) );
		const float adjusted = percent + ( float )adjustAmount;
		const float clamped = idMath::ClampFloat( 0.0f, 100.0f, adjusted );
		s_volume_env.SetFloat( DB_SILENCE - ( idMath::Sqrt( clamped / 100.0f ) * DB_SILENCE ) );
		break;
	}
	case SOUND_FIELD_WEAP:
	{
		const float percent = 100.0f * Square( 1.0f - ( s_volume_weap.GetFloat() / DB_SILENCE ) );
		const float adjusted = percent + ( float )adjustAmount;
		const float clamped = idMath::ClampFloat( 0.0f, 100.0f, adjusted );
		s_volume_weap.SetFloat( DB_SILENCE - ( idMath::Sqrt( clamped / 100.0f ) * DB_SILENCE ) );
		break;
	}
	case SOUND_FIELD_SELF:
	{
		const float percent = 100.0f * Square( 1.0f - ( s_volume_self.GetFloat() / DB_SILENCE ) );
		const float adjusted = percent + ( float )adjustAmount;
		const float clamped = idMath::ClampFloat( 0.0f, 100.0f, adjusted );
		s_volume_self.SetFloat( DB_SILENCE - ( idMath::Sqrt( clamped / 100.0f ) * DB_SILENCE ) );
		break;
	}
	case SOUND_FIELD_EAX:
		{
			static const int numValues = 2;
			static const int values[numValues] = { 0, 1 };
			s_useEAX.SetInteger( AdjustOption( s_useEAX.GetInteger(), values, numValues, adjustAmount ) );
			break;
		}
	case SOUND_FIELD_CC:
		{
			static const int numValues = 2;
			static const int values[numValues] = { 0, 1 };
			s_useCC.SetInteger( AdjustOption( s_useCC.GetInteger(), values, numValues, adjustAmount ) );
			break;
		}
	}
	cvarSystem->ClearModifiedFlags( CVAR_ARCHIVE );
}

/*
========================
idMenuScreen_Shell_SoundOptions::idMenuDataSource_SoundSettings::GetField
========================
*/
idSWFScriptVar idMenuScreen_Shell_SoundOptions::idMenuDataSource_SoundSettings::GetField( const int fieldIndex ) const
{
	switch( fieldIndex )
	{
		case SOUND_FIELD_MASTER:
		{
			return 100.0f * Square( 1.0f - ( s_volume_dB.GetFloat() / DB_SILENCE ) );
		}
		case SOUND_FIELD_VOICES:
		{
			return 100.0f * Square( 1.0f - ( s_volume_voices.GetFloat() / DB_SILENCE ) );
		}
		case SOUND_FIELD_ENV:
		{
			return 100.0f * Square( 1.0f - ( s_volume_env.GetFloat() / DB_SILENCE ) );
		}
		case SOUND_FIELD_WEAP:
		{
			return 100.0f * Square( 1.0f - ( s_volume_weap.GetFloat() / DB_SILENCE ) );
		}
		case SOUND_FIELD_SELF:
		{
			return 100.0f * Square( 1.0f - ( s_volume_self.GetFloat() / DB_SILENCE ) );
		}
		case SOUND_FIELD_EAX:
			return s_useEAX.GetBool() ? "#str_swf_enabled" : "#str_swf_disabled";
		case SOUND_FIELD_CC:
			return s_useCC.GetBool() ? "#str_swf_enabled" : "#str_swf_disabled";


	}
	return false;
}

/*
========================
idMenuScreen_Shell_SoundOptions::idMenuDataSource_SoundSettings::IsDataChanged
========================
*/
bool idMenuScreen_Shell_SoundOptions::idMenuDataSource_SoundSettings::IsDataChanged() const
{
	if( originalMasterVolume != s_volume_dB.GetFloat() )
	{
		return true;
	}
	if( originalVoiceVolume != s_volume_voices.GetFloat() )
	{
		return true;
	}
	if( originalEnvVolume != s_volume_env.GetFloat() )
	{
		return true;
	}
	if( originalWeapVolume != s_volume_weap.GetFloat() )
	{
		return true;
	}
	if( originalSelfVolume != s_volume_self.GetFloat() )
	{
		return true;
	}
	if( originalEAX != s_useEAX.GetBool() )
	{
		return true;
	}
	if( originalCC != s_useCC.GetBool() )
	{
		return true;
	}
	return false;
}
