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

const static int NUM_SYSTEM_OPTIONS_OPTIONS = 8;

extern idCVar r_fullscreen;
extern idCVar r_motionBlur;
extern idCVar r_swapInterval;
extern idCVar s_volume_dB;
extern idCVar r_exposure; // RB: use this to control HDR exposure or brightness in LDR mode
extern idCVar r_lightScale;
//extern idCVar r_aspectratio; //GK: use forced aspect ratio

idList<int> refreshList;
int numOfDisplays;
static bool resetVideo = false;

/*
========================
idMenuScreen_Shell_SystemOptions::Initialize
========================
*/
void idMenuScreen_Shell_SystemOptions::Initialize( idMenuHandler* data )
{
	common->Printf("Initializing System Options\n");
	idMenuScreen::Initialize( data );
	
	resetVideo = false;

	if( data != NULL )
	{
		menuGUI = data->GetGUI();
	}

	for (int displayNum = 0; ; displayNum++)
	{
		idList<vidMode_t> modeList;
		if (!R_GetModeListForDisplay(displayNum, modeList))
		{
			numOfDisplays = displayNum;
			break;
		}
	}
	
	SetSpritePath( "menuSystemOptions" );
	
	options = new( TAG_SWF ) idMenuWidget_DynamicList();
	options->SetNumVisibleOptions( NUM_SYSTEM_OPTIONS_OPTIONS );
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
	if (renderSystem->GetStereo3DMode() != STEREO3D_VR) {
		control = new(TAG_SWF) idMenuWidget_ControlButton();
		control->SetOptionType(OPTION_SLIDER_TEXT);
		control->SetLabel("#str_02154");
		control->SetDataSource(&systemData, idMenuDataSource_SystemSettings::SYSTEM_FIELD_FULLSCREEN);
		control->SetupEvents(DEFAULT_REPEAT_TIME, options->GetChildren().Num());
		control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, idMenuDataSource_SystemSettings::SYSTEM_FIELD_FULLSCREEN);
		options->AddChild(control);
	
	//GK: Begin

		control = new(TAG_SWF) idMenuWidget_ControlButton();
		control->SetOptionType(OPTION_SLIDER_TEXT);
		control->SetLabel("#str_fullscreen");
		control->SetDataSource(&systemData, idMenuDataSource_SystemSettings::SYSTEM_FIELD_FULLSCREEN_MODE);
		control->SetupEvents(DEFAULT_REPEAT_TIME, options->GetChildren().Num());
		control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, idMenuDataSource_SystemSettings::SYSTEM_FIELD_FULLSCREEN_MODE);
		options->AddChild(control);
	}
	control = new(TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType(OPTION_SLIDER_TEXT);
	control->SetLabel("#str_swf_aspect"); //Aspect Ratio
	control->SetDataSource(&systemData, idMenuDataSource_SystemSettings::SYSTEM_FIELD_ASPECTRATIO);
	control->SetupEvents(DEFAULT_REPEAT_TIME, options->GetChildren().Num());
	control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, idMenuDataSource_SystemSettings::SYSTEM_FIELD_ASPECTRATIO);
	options->AddChild(control);
	//GK: End
	if (renderSystem->GetStereo3DMode() != STEREO3D_VR) {
		control = new(TAG_SWF) idMenuWidget_ControlButton();
		control->SetOptionType(OPTION_SLIDER_TEXT);
		control->SetLabel("#str_swf_framerate");
		control->SetDataSource(&systemData, idMenuDataSource_SystemSettings::SYSTEM_FIELD_FRAMERATE);
		control->SetupEvents(DEFAULT_REPEAT_TIME, options->GetChildren().Num());
		control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, idMenuDataSource_SystemSettings::SYSTEM_FIELD_FRAMERATE);
		options->AddChild(control);
	}
	
	control = new( TAG_SWF ) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_TEXT );
	control->SetLabel( "#str_04126" );
	control->SetDataSource( &systemData, idMenuDataSource_SystemSettings::SYSTEM_FIELD_VSYNC );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_SystemSettings::SYSTEM_FIELD_VSYNC );
	options->AddChild( control );
	
	
	
	control = new( TAG_SWF ) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_TEXT );
	control->SetLabel( "#str_swf_motionblur" );
	control->SetDataSource( &systemData, idMenuDataSource_SystemSettings::SYSTEM_FIELD_MOTIONBLUR );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_SystemSettings::SYSTEM_FIELD_MOTIONBLUR );
	options->AddChild( control );
	
	// RB begin //GK: Some option had to be sacrificed and since it's available on the launcher and requires restart then it's the perfect candinate
	/*control = new( TAG_SWF ) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_TEXT );
	control->SetLabel( "Soft Shadows" );
	control->SetDataSource( &systemData, idMenuDataSource_SystemSettings::SYSTEM_FIELD_SHADOWMAPPING );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_SystemSettings::SYSTEM_FIELD_SHADOWMAPPING );
	options->AddChild( control );*/
	
	/*control = new( TAG_SWF ) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_BAR );
	control->SetLabel( "#str_swf_lodbias" );
	control->SetDataSource( &systemData, idMenuDataSource_SystemSettings::SYSTEM_FIELD_LODBIAS );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_SystemSettings::SYSTEM_FIELD_LODBIAS );
	options->AddChild( control );*/
	// RB end
	
	control = new( TAG_SWF ) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_BAR );
	control->SetLabel( "#str_02155" );	// Brightness
	control->SetDataSource( &systemData, idMenuDataSource_SystemSettings::SYSTEM_FIELD_BRIGHTNESS );
	control->SetupEvents( 2, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_SystemSettings::SYSTEM_FIELD_BRIGHTNESS );
	options->AddChild( control );
	
	control = new( TAG_SWF ) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_BAR );
	control->SetLabel( "#str_02163" );	// Volume
	control->SetDataSource( &systemData, idMenuDataSource_SystemSettings::SYSTEM_FIELD_VOLUME );
	control->SetupEvents( 2, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_SystemSettings::SYSTEM_FIELD_VOLUME );
	options->AddChild( control );
	
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
void idMenuScreen_Shell_SystemOptions::Update()
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
			heading->SetText( "#str_00183" );	// FULLSCREEN
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
void idMenuScreen_Shell_SystemOptions::ShowScreen( const mainMenuTransition_t transitionType )
{

	systemData.LoadData();
	
	idMenuScreen::ShowScreen( transitionType );
}

/*
========================
idMenuScreen_Shell_SystemOptions::HideScreen
========================
*/
void idMenuScreen_Shell_SystemOptions::HideScreen( const mainMenuTransition_t transitionType )
{

	if( systemData.IsRestartRequired() )
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
	
	if( systemData.IsDataChanged() )
	{
		systemData.CommitData();
	}
	
	idMenuScreen::HideScreen( transitionType );
}

/*
========================
idMenuScreen_Shell_SystemOptions::HandleAction h
========================
*/
bool idMenuScreen_Shell_SystemOptions::HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled )
{

	if( menuData == NULL )
	{
		return true;
	}
	
	if( menuData->ActiveScreen() != SHELL_AREA_SYSTEM_OPTIONS )
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
		case WIDGET_ACTION_ADJUST_FIELD:
			if( widget->GetDataSourceFieldIndex() == idMenuDataSource_SystemSettings::SYSTEM_FIELD_FULLSCREEN )
			{
				menuData->SetNextScreen( SHELL_AREA_RESOLUTION, MENU_TRANSITION_SIMPLE );
				return true;
			}
			updateUi = false;
			break;
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
			
			switch( parms[0].ToInteger() )
			{
				case idMenuDataSource_SystemSettings::SYSTEM_FIELD_FULLSCREEN:
				{
					menuData->SetNextScreen( SHELL_AREA_RESOLUTION, MENU_TRANSITION_SIMPLE );
					return true;
				}
				default:
				{
					systemData.AdjustField( parms[0].ToInteger(), 1 );
					options->Update();
				}
			}
			
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
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::idMenuDataSource_SystemSettings()
{
}

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::LoadData
========================
*/
void idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::LoadData()
{
	originalFramerate = com_engineHz.GetInteger();
	originalRefreshRate = r_displayRefresh.GetInteger();
	originalFullscreen = r_fullscreen.GetInteger();
	originalMotionBlur = r_motionBlur.GetInteger();
	originalVsync = r_swapInterval.GetInteger();
	originalBrightness = r_lightScale.GetFloat();
	originalVolume = s_volume_dB.GetFloat();
	originalScreenXpos = r_windowX.GetInteger();
	originalScreenYpos = r_windowY.GetInteger();
	// RB begin
	//originalShadowMapping = r_useShadowMapping.GetInteger();
	// RB end
	
	const int fullscreen = r_fullscreen.GetInteger();
	if( fullscreen != 0 )
	{
		R_GetModeListForDisplay( fullscreen - 1, modeList );
	}
	else
	{
		modeList.Clear();
	}
}

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::IsRestartRequired
========================
*/
bool idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::IsRestartRequired() const
{
	
	if( originalFramerate != com_engineHz.GetInteger() )
	{
		return true;
	}

	if (originalRefreshRate != r_displayRefresh.GetInteger())
	{
		return true;
	}
	
//	if( originalShadowMapping != r_useShadowMapping.GetInteger() )
	//{
	//	return true;
	//}
	
	return false;
}

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::CommitData
========================
*/
void idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::CommitData()
{
	cvarSystem->SetModifiedFlags( CVAR_ARCHIVE );
	if (resetVideo) {
		resetVideo = false;
		cmdSystem->BufferCommandText(CMD_EXEC_APPEND, "vid_restart\n");
	}
}

/*
========================
AdjustOption
Given a current value in an array of possible values, returns the next n value
========================
*/
int idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::AdjustOption( int currentValue, const int values[], int numValues, int adjustment )
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
float idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::LinearAdjust( float input, float currentMin, float currentMax, float desiredMin, float desiredMax ) const
{
	return ( ( input - currentMin ) / ( currentMax - currentMin ) ) * ( desiredMax - desiredMin ) + desiredMin;
}

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::AdjustField
========================
*/
void idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::AdjustField( const int fieldIndex, const int adjustAmount )
{
	switch( fieldIndex )
	{
		//GK: BEgin
	case SYSTEM_FIELD_ASPECTRATIO:
	{
		static const int numValues = 2;
		static const int values[numValues] = { 0, 1 };
		game->SetCVarInteger("r_aspectratio",AdjustOption(game->GetCVarInteger("r_aspectratio"), values, numValues, adjustAmount));
		break;
	}
	//GK: End
		case SYSTEM_FIELD_FRAMERATE:
		{
			if (R_GetRefreshListForDisplay(r_fullscreen.GetInteger() > 0 ? r_fullscreen.GetInteger() - 1 : 0, refreshList)) {
				idList<int> framerateList = refreshList;
				framerateList.AddUnique(60);
				framerateList.AddUnique(120);
				int fps = AdjustOption(com_engineHz.GetInteger(), framerateList.Ptr(), framerateList.Num(), adjustAmount);
				com_engineHz.SetInteger(fps);
				if (refreshList.Find(fps) != NULL) {
					r_displayRefresh.SetInteger(fps);
				}
			}
			else {
				static const int numValues = 2;
				static const int values[numValues] = { 60, 120 };
				com_engineHz.SetInteger(AdjustOption(com_engineHz.GetInteger(), values, numValues, adjustAmount));
			}
			break;
		}
		case SYSTEM_FIELD_VSYNC:
		{
			static const int numValues = 3;
			static const int values[numValues] = { 0, 1, 2 };
			r_swapInterval.SetInteger( AdjustOption( r_swapInterval.GetInteger(), values, numValues, adjustAmount ) );
			break;
		}
		case SYSTEM_FIELD_FULLSCREEN_MODE:
		{
			idList<int> screenValues;
			screenValues.Clear();

			screenValues.AddUnique(-1);
			screenValues.AddUnique(0);
			for (int i = 0; i < numOfDisplays; i++) {
				screenValues.AddUnique(i + 1);
			}
			r_fullscreen.SetInteger(AdjustOption(r_fullscreen.GetInteger(), screenValues.Ptr(), screenValues.Num(), adjustAmount));
			break;
		}
		case SYSTEM_FIELD_MOTIONBLUR:
		{
			static const int numValues = 5;
			static const int values[numValues] = { 0, 2, 3, 4, 5 };
			r_motionBlur.SetInteger( AdjustOption( r_motionBlur.GetInteger(), values, numValues, adjustAmount ) );
			break;
		}
		// RB begin
	//	case SYSTEM_FIELD_SHADOWMAPPING:
	//	{
	//		static const int numValues = 2;
	//		static const int values[numValues] = { 0, 1 };
	//		r_useShadowMapping.SetInteger( AdjustOption( r_useShadowMapping.GetInteger(), values, numValues, adjustAmount ) );
	//		break;
	//	}
		/*case SYSTEM_FIELD_LODBIAS:
		{
			const float percent = LinearAdjust( r_lodBias.GetFloat(), -1.0f, 1.0f, 0.0f, 100.0f );
			const float adjusted = percent + ( float )adjustAmount * 5.0f;
			const float clamped = idMath::ClampFloat( 0.0f, 100.0f, adjusted );
			r_lodBias.SetFloat( LinearAdjust( clamped, 0.0f, 100.0f, -1.0f, 1.0f ) );
			break;
		}*/
		// RB end
		case SYSTEM_FIELD_BRIGHTNESS:
		{
			const float percent = LinearAdjust(r_lightScale.GetFloat(), 0.0f, 5.0f, 0.0f, 100.0f );
			const float adjusted = percent + ( float )adjustAmount;
			const float clamped = idMath::ClampFloat( 0.0f, 100.0f, adjusted );
			//r_exposure.SetFloat( LinearAdjust( clamped, 0.0f, 100.0f, 0.0f, 1.0f ) );
			r_lightScale.SetFloat( LinearAdjust( clamped, 0.0f, 100.0f, 0.0f, 5.0f ) ); //GK: More light options
			break;
		}
		case SYSTEM_FIELD_VOLUME:
		{
			const float percent = 100.0f * Square( 1.0f - ( s_volume_dB.GetFloat() / DB_SILENCE ) );
			const float adjusted = percent + ( float )adjustAmount;
			const float clamped = idMath::ClampFloat( 0.0f, 100.0f, adjusted );
			s_volume_dB.SetFloat( DB_SILENCE - ( idMath::Sqrt( clamped / 100.0f ) * DB_SILENCE ) );
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
idSWFScriptVar idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::GetField( const int fieldIndex ) const
{
	switch( fieldIndex )
	{
		case SYSTEM_FIELD_FULLSCREEN:
		{
			const int fullscreen = r_fullscreen.GetInteger();
			//const int vidmode = r_vidMode.GetInteger();
			if( fullscreen == 0 )
			{
				return "#str_swf_disabled";
			}
			/*if( vidmode < 0 || vidmode >= modeList.Num() )
			{
				return "???";
			}*/
			if( r_displayRefresh.GetInteger() == 60 || fullscreen < 0 )
			{
				return va( "%4i x %4i", r_customWidth.GetInteger(), r_customHeight.GetInteger());
			}
			else
			{
				return va( "%4i x %4i @ %dhz", r_customWidth.GetInteger(), r_customHeight.GetInteger(), r_displayRefresh.GetInteger()/*modeList[vidmode].displayHz*/ );
			}
		}
		//GK: Begin
		case SYSTEM_FIELD_ASPECTRATIO:
			if (game->GetCVarInteger("r_aspectratio"))
			{
				return "#str_swf_aspect_dynamic";
			}
			else
			{
				return "4:3";
			}
			//GK: End
		case SYSTEM_FIELD_FRAMERATE:
			return va( "%d FPS", com_engineHz.GetInteger() );
		case SYSTEM_FIELD_VSYNC:
			if( r_swapInterval.GetInteger() == 1 )
			{
				return "#str_swf_smart";
			}
			else if( r_swapInterval.GetInteger() == 2 )
			{
				return "#str_swf_enabled";
			}
			else
			{
				return "#str_swf_disabled";
			}
		case SYSTEM_FIELD_FULLSCREEN_MODE:
		{
			switch (r_fullscreen.GetInteger()) {
				case -1:
					return "#str_swf_borderless";
				case 0:
					return "#str_swf_windowed";
				case 1:
					return "#str_swf_exclusive_fullscreen";
				default:
					return va("%s %d", idLocalization::GetString("#str_swf_monitor"), r_fullscreen.GetInteger());
			}
		}
		case SYSTEM_FIELD_MOTIONBLUR:
			if( r_motionBlur.GetInteger() == 0 )
			{
				return "#str_swf_disabled";
			}
			return va( "%dx", idMath::IPow( 2, r_motionBlur.GetInteger() ) );
		// RB begin
	//	case SYSTEM_FIELD_SHADOWMAPPING:
	//		if( r_useShadowMapping.GetInteger() == 1 )
	//		{
	//			return "#str_swf_enabled";
	//		}
	//		else
	//		{
	//			return "#str_swf_disabled";
	//		}
		//case SYSTEM_FIELD_LODBIAS:
		//	return LinearAdjust( r_lodBias.GetFloat(), -1.0f, 1.0f, 0.0f, 100.0f );
		// RB end
		case SYSTEM_FIELD_BRIGHTNESS:
			return LinearAdjust( r_lightScale.GetFloat(), 0.0f, 5.0f, 0.0f, 100.0f );
		case SYSTEM_FIELD_VOLUME:
		{
			return 100.0f * Square( 1.0f - ( s_volume_dB.GetFloat() / DB_SILENCE ) );
		}
	}
	return false;
}

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::IsDataChanged
========================
*/
bool idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::IsDataChanged() const
{
	if( originalFramerate != com_engineHz.GetInteger() )
	{
		return true;
	}
	if (originalRefreshRate != r_displayRefresh.GetInteger())
	{
		return true;
	}
	if( originalFullscreen != r_fullscreen.GetInteger() )
	{
		if (r_fullscreen.GetInteger() == -1) {
			r_windowX.SetInteger(0);
			r_windowY.SetInteger(0);
		}
		else {
			r_windowX.SetInteger(originalScreenXpos);
			r_windowY.SetInteger(originalScreenYpos);
		}
		resetVideo = true;
		return true;
	}
	if( originalMotionBlur != r_motionBlur.GetInteger() )
	{
		return true;
	}
	if( originalVsync != r_swapInterval.GetInteger() )
	{
		return true;
	}
	if( originalBrightness != r_lightScale.GetFloat() )
	{
		return true;
	}
	if( originalVolume != s_volume_dB.GetFloat() )
	{
		return true;
	}
	// RB begin
//	if( originalShadowMapping != r_useShadowMapping.GetInteger() )
//	{
//		return true;
//	}
	// RB end
	return false;
}
