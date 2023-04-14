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


static bool resetVideo = false;

/*
========================
idMenuScreen_Shell_AdvancedGraphics::Initialize
========================
*/
void idMenuScreen_Shell_AdvancedGraphics::Initialize( idMenuHandler* data )
{
	idMenuScreen::Initialize( data );

	resetVideo = false;
	
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
	control->SetDataSource( &advData, idMenuDataSource_AdvancedGraphics::ADV_FIELD_SHADOWMAPPING );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedGraphics::ADV_FIELD_SHADOWMAPPING);
	options->AddChild( control );

	control = new(TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType(OPTION_SLIDER_BAR);
	control->SetLabel("#str_shadow_mapping_lod"); //Soft ShadowsLOD
	control->SetDataSource(&advData, idMenuDataSource_AdvancedGraphics::ADV_FIELD_SHADOWMAPLOD);
	control->SetupEvents(DEFAULT_REPEAT_TIME, options->GetChildren().Num());
	control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedGraphics::ADV_FIELD_SHADOWMAPLOD);
	options->AddChild(control);

	control = new(TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType(OPTION_SLIDER_TEXT);
	control->SetLabel("#str_hdr"); //HDR
	control->SetDataSource(&advData, idMenuDataSource_AdvancedGraphics::ADV_FIELD_HDR);
	control->SetupEvents(DEFAULT_REPEAT_TIME, options->GetChildren().Num());
	control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedGraphics::ADV_FIELD_HDR);
	options->AddChild(control);

	control = new( TAG_SWF ) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_TEXT );
	control->SetLabel( "#str_04128" ); //Anti Aliasing
	control->SetDataSource( &advData, idMenuDataSource_AdvancedGraphics::ADV_FIELD_ANTIALIASING );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedGraphics::ADV_FIELD_ANTIALIASING);
	options->AddChild( control );
	
	control = new( TAG_SWF ) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_TEXT );
	control->SetLabel( "#str_ssao" ); //SSAO
	control->SetDataSource( &advData, idMenuDataSource_AdvancedGraphics::ADV_FIELD_SSAO );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedGraphics::ADV_FIELD_SSAO );
	options->AddChild( control );
	
	control = new( TAG_SWF ) idMenuWidget_ControlButton();
	control->SetOptionType( OPTION_SLIDER_TEXT );
	control->SetLabel( "#str_filmic" ); //Filmic Post Process effect
	control->SetDataSource( &advData, idMenuDataSource_AdvancedGraphics::ADV_FIELD_FPPE );
	control->SetupEvents( DEFAULT_REPEAT_TIME, options->GetChildren().Num() );
	control->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedGraphics::ADV_FIELD_FPPE );
	options->AddChild( control );
	
	control = new(TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType(OPTION_SLIDER_TEXT);
	control->SetLabel("#str_ssgi");	// SSGI
	control->SetDataSource(&advData, idMenuDataSource_AdvancedGraphics::ADV_FIELD_SSGI);
	control->SetupEvents(DEFAULT_REPEAT_TIME, options->GetChildren().Num());
	control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedGraphics::ADV_FIELD_SSGI);
	options->AddChild(control);

	control = new(TAG_SWF) idMenuWidget_ControlButton();
	control->SetOptionType(OPTION_SLIDER_TEXT);
	control->SetLabel("#str_hli"); //Half-Lambert Lighting
	control->SetDataSource(&advData, idMenuDataSource_AdvancedGraphics::ADV_FIELD_HALF_LIGHT);
	control->SetupEvents(DEFAULT_REPEAT_TIME, options->GetChildren().Num());
	control->AddEventAction(WIDGET_EVENT_PRESS).Set(WIDGET_ACTION_COMMAND, idMenuDataSource_AdvancedGraphics::ADV_FIELD_HALF_LIGHT);
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
void idMenuScreen_Shell_AdvancedGraphics::Update()
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
			heading->SetText( "#str_advanced_graphics_heading" );	// ADVANCED GRAPHICS
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
void idMenuScreen_Shell_AdvancedGraphics::ShowScreen( const mainMenuTransition_t transitionType )
{
	advData.LoadData();
	
	idMenuScreen::ShowScreen( transitionType );
}

/*
========================
idMenuScreen_Shell_SystemOptions::HideScreen
========================
*/
void idMenuScreen_Shell_AdvancedGraphics::HideScreen( const mainMenuTransition_t transitionType )
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
bool idMenuScreen_Shell_AdvancedGraphics::HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled )
{

	if( menuData == NULL )
	{
		return true;
	}
	
	if( menuData->ActiveScreen() != SHELL_AREA_ADV_GRAPHICS )
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
idMenuScreen_Shell_AdvancedGraphics::idMenuDataSource_AdvancedGraphics::idMenuDataSource_AdvancedGraphics()
{
}

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::LoadData
========================
*/
void idMenuScreen_Shell_AdvancedGraphics::idMenuDataSource_AdvancedGraphics::LoadData()
{
	originalShadowMapping = r_useShadowMapping.GetInteger();
	originalHDR = r_useHDR.GetInteger();
	originalAA = r_antiAliasing.GetInteger();
	originalSSAO = r_useSSAO.GetInteger();
	originalFilmic = r_useFilmicPostProcessEffects.GetInteger();
	originalSSGI = r_useSSGI.GetInteger();
	originalColor = r_useHalfLambertLighting.GetInteger();
	originalShadowMapLod = r_shadowMapLodScale.GetFloat();
}

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::CommitData
========================
*/
void idMenuScreen_Shell_AdvancedGraphics::idMenuDataSource_AdvancedGraphics::CommitData()
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
int idMenuScreen_Shell_AdvancedGraphics::idMenuDataSource_AdvancedGraphics::AdjustOption(int currentValue, const int values[], int numValues, int adjustment)
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
float idMenuScreen_Shell_AdvancedGraphics::idMenuDataSource_AdvancedGraphics::LinearAdjust(float input, float currentMin, float currentMax, float desiredMin, float desiredMax) const
{
	return ((input - currentMin) / (currentMax - currentMin)) * (desiredMax - desiredMin) + desiredMin;
}

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::AdjustField
========================
*/
void idMenuScreen_Shell_AdvancedGraphics::idMenuDataSource_AdvancedGraphics::AdjustField( const int fieldIndex, const int adjustAmount )
{
	static const int genericNumValues = 2;
	static const int genericValues[genericNumValues] = { 0, 1 };
	switch( fieldIndex )
	{
		case ADV_FIELD_SHADOWMAPPING:
		{
			r_useShadowMapping.SetBool(AdjustOption(r_useShadowMapping.GetBool(), genericValues, genericNumValues, adjustAmount));
			break;
		}
		case ADV_FIELD_SHADOWMAPLOD:
		{
			r_shadowMapLodScale.SetFloat(r_shadowMapLodScale.GetFloat() + adjustAmount * 0.1f);
			break;
		}
		case ADV_FIELD_HDR:
		{
			r_useHDR.SetBool(AdjustOption(r_useHDR.GetBool(), genericValues, genericNumValues, adjustAmount));
			break;
		}
		case ADV_FIELD_ANTIALIASING:
		{
			// RB: disabled 16x MSAA
			static const int numValues = 5;
			static const int values[numValues] =
			{
				ANTI_ALIASING_NONE,
				ANTI_ALIASING_SMAA_1X,
				ANTI_ALIASING_MSAA_2X,
				ANTI_ALIASING_MSAA_4X,
				ANTI_ALIASING_MSAA_8X
			};
			// RB end
			r_antiAliasing.SetInteger(AdjustOption(r_antiAliasing.GetInteger(), values, numValues, adjustAmount));
			break;
		}
		case ADV_FIELD_SSAO:
		{
			r_useSSAO.SetBool(AdjustOption(r_useSSAO.GetBool(), genericValues, genericNumValues, adjustAmount));
			break;
		}
		case ADV_FIELD_FPPE:
		{
			r_useFilmicPostProcessEffects.SetBool(AdjustOption(r_useFilmicPostProcessEffects.GetBool(), genericValues, genericNumValues, adjustAmount));
			break;
		}
		case ADV_FIELD_SSGI:
		{
			r_useSSGI.SetBool(AdjustOption(r_useSSGI.GetBool(), genericValues, genericNumValues, adjustAmount));
			break;
		}
		case ADV_FIELD_HALF_LIGHT:
		{
			r_useHalfLambertLighting.SetBool(AdjustOption(r_useHalfLambertLighting.GetBool(), genericValues, genericNumValues, adjustAmount));
			break;
		}
	}
	cvarSystem->ClearModifiedFlags( CVAR_ARCHIVE );
}

bool idMenuScreen_Shell_AdvancedGraphics::idMenuDataSource_AdvancedGraphics::IsRestartRequired() const
{
	if (originalAA != r_antiAliasing.GetInteger())
	{
		return true;
	}
	return false;
}

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::GetField
========================
*/
idSWFScriptVar idMenuScreen_Shell_AdvancedGraphics::idMenuDataSource_AdvancedGraphics::GetField( const int fieldIndex ) const
{
	switch( fieldIndex )
	{
		case ADV_FIELD_SHADOWMAPPING:
		{
			return r_useShadowMapping.GetBool() ? "#str_swf_enabled" : "#str_swf_disabled";
		}
		case ADV_FIELD_SHADOWMAPLOD:
		{
			return LinearAdjust(r_shadowMapLodScale.GetFloat(), 0.1f, 2.0f, 0.0f, 100.0f);
		}
		case ADV_FIELD_HDR:
		{
			return r_useHDR.GetBool() ? "#str_swf_enabled" : "#str_swf_disabled";
		}
		case ADV_FIELD_ANTIALIASING:
		{
			if (r_antiAliasing.GetInteger() == 0)
			{
				return "#str_swf_disabled";
			}

			static const int numValues = 5;
			static const char* values[numValues] =
			{
				"None",
				"SMAA 1X",
				"MSAA 2X",
				"MSAA 4X",
				"MSAA 8X"
			};

			compile_time_assert(numValues == (ANTI_ALIASING_MSAA_8X + 1));

			return values[r_antiAliasing.GetInteger()];
		}
		case ADV_FIELD_SSAO:
		{
			return r_useSSAO.GetBool() ? "#str_swf_enabled" : "#str_swf_disabled";
		}
		case ADV_FIELD_FPPE:
		{
			return r_useFilmicPostProcessEffects.GetBool() ? "#str_swf_enabled" : "#str_swf_disabled";
		}
		case ADV_FIELD_SSGI:
			return r_useSSGI.GetBool() ? "#str_swf_enabled" : "#str_swf_disabled";
		case ADV_FIELD_HALF_LIGHT:
			return r_useHalfLambertLighting.GetBool() ? "#str_swf_enabled" : "#str_swf_disabled";
	}
	return false;
}

/*
========================
idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::IsDataChanged
========================
*/
bool idMenuScreen_Shell_AdvancedGraphics::idMenuDataSource_AdvancedGraphics::IsDataChanged() const
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
		resetVideo = true;
		return true;
	}
	if( originalAA != r_antiAliasing.GetInteger())
	{
		return true;
	}
	if( originalSSAO != r_useSSAO.GetInteger() )
	{
		resetVideo = true;
		return true;
	}
	if( originalFilmic != r_useFilmicPostProcessEffects.GetInteger() )
	{
		return true;
	}
	if( originalSSGI != r_useSSGI.GetInteger())
	{
		return true;
	}
	if (originalColor != r_useHalfLambertLighting.GetInteger())
	{
		resetVideo = true;
		return true;
	}
	return false;
}
