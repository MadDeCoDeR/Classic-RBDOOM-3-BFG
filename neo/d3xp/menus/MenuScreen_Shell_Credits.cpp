/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013 Robert Beckebans

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
#include "../swf/credits/CreditDict.h"

static const int NUM_CREDIT_LINES = 16;

void idMenuScreen_Shell_Credits::SetupCreditList()
{

	class idRefreshCredits : public idSWFScriptFunction_RefCounted
	{
	public:
		idRefreshCredits( idMenuScreen_Shell_Credits* _screen ) :
			screen( _screen )
		{
		}
		
		idSWFScriptVar Call( idSWFScriptObject* thisObject, const idSWFParmList& parms )
		{
		
			if( screen == NULL )
			{
				return idSWFScriptVar();
			}
			
			screen->UpdateCredits();
			return idSWFScriptVar();
		}
	private:
		idMenuScreen_Shell_Credits* screen;
	};
	
	if( GetSWFObject() )
	{
		GetSWFObject()->SetGlobal( "updateCredits", new( TAG_SWF ) idRefreshCredits( this ) );
	}
	
	idCredits::PopulateList(creditList);
};

/*
========================
idMenuScreen_Shell_Credits::Initialize
========================
*/
void idMenuScreen_Shell_Credits::Initialize( idMenuHandler* data )
{
	idMenuScreen::Initialize( data );
	
	if( data != NULL )
	{
		menuGUI = data->GetGUI();
	}
	
	SetSpritePath( "menuCredits" );
	
	btnBack = new( TAG_SWF ) idMenuWidget_Button();
	btnBack->Initialize( data );
	btnBack->SetLabel( "#str_02305" );
	btnBack->SetSpritePath( GetSpritePath(), "info", "btnBack" );
	btnBack->AddEventAction( WIDGET_EVENT_PRESS ).Set( WIDGET_ACTION_GO_BACK );
	AddChild( btnBack );
	
	SetupCreditList();
}

/*
========================
idMenuScreen_Shell_Credits::Update
========================
*/
void idMenuScreen_Shell_Credits::Update()
{

	if( menuData != NULL )
	{
		idMenuWidget_CommandBar* cmdBar = menuData->GetCmdBar();
		if( cmdBar != NULL )
		{
			cmdBar->ClearAllButtons();
			
			idMenuHandler_Shell* shell = dynamic_cast< idMenuHandler_ShellLocal* >( menuData );
			bool complete = false;
			if( shell != NULL )
			{
				complete = shell->GetGameComplete();
			}
			
			idMenuWidget_CommandBar::buttonInfo_t* buttonInfo;
			if( !complete )
			{
				buttonInfo = cmdBar->GetButton( idMenuWidget_CommandBar::BUTTON_JOY2 );
				if((!common->IsNewDOOM3() && menuData->GetPlatform() != 2) || (common->IsNewDOOM3() && menuData->GetPlatform() != 5))
				{
					buttonInfo->label = "#str_00395";
				}
				buttonInfo->action.Set( WIDGET_ACTION_GO_BACK );
			}
			else
			{
				buttonInfo = cmdBar->GetButton( idMenuWidget_CommandBar::BUTTON_JOY1 );
				if((!common->IsNewDOOM3() && menuData->GetPlatform() != 2) || (common->IsNewDOOM3() && menuData->GetPlatform() != 5))
				{
					buttonInfo->label = "#str_swf_continue";
				}
				buttonInfo->action.Set( WIDGET_ACTION_GO_BACK );
			}
		}
	}
	
	idSWFScriptObject& root = GetSWFObject()->GetRootObject();
	if( BindSprite( root ) )
	{
		idSWFTextInstance* heading = GetSprite()->GetScriptObject()->GetNestedText( "info", "txtHeading" );
		if( heading != NULL )
		{
			heading->SetText( "#str_02218" );
			heading->SetStrokeInfo( true, 0.75f, 1.75f );
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
idMenuScreen_Shell_Credits::ShowScreen
========================
*/
void idMenuScreen_Shell_Credits::ShowScreen( const mainMenuTransition_t transitionType )
{

	if( menuData != NULL )
	{
		idMenuHandler_Shell* shell = dynamic_cast< idMenuHandler_ShellLocal* >( menuData );
		bool complete = false;
		if( shell != NULL )
		{
			complete = shell->GetGameComplete();
		}
		
		if( complete )
		{
			menuData->PlaySound( GUI_SOUND_MUSIC );
		}
	}
	
	idMenuScreen::ShowScreen( transitionType );
	creditIndex = 0;
	UpdateCredits();
}

/*
========================
idMenuScreen_Shell_Credits::HideScreen
========================
*/
void idMenuScreen_Shell_Credits::HideScreen( const mainMenuTransition_t transitionType )
{
	idMenuScreen::HideScreen( transitionType );
}

/*
========================
idMenuScreen_Shell_Credits::HandleAction
========================
*/
bool idMenuScreen_Shell_Credits::HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled )
{

	if( menuData == NULL )
	{
		return true;
	}
	
	if( menuData->ActiveScreen() != SHELL_AREA_CREDITS )
	{
		return false;
	}

	this->Update();
	
	widgetAction_t actionType = action.GetType();
	switch( actionType )
	{
		case WIDGET_ACTION_GO_BACK:
		{
		
			idMenuHandler_Shell* shell = dynamic_cast< idMenuHandler_ShellLocal* >( menuData );
			bool complete = false;
			if( shell != NULL )
			{
				complete = shell->GetGameComplete();
			}
			
			if( complete )
			{
				cmdSystem->BufferCommandText( CMD_EXEC_NOW, "disconnect" );
			}
			else
			{
				menuData->SetNextScreen( SHELL_AREA_ROOT, MENU_TRANSITION_SIMPLE );
			}
			
			return true;
		}
	}
	
	return idMenuWidget::HandleAction( action, event, widget, forceHandled );
}

/*
========================
idMenuScreen_Shell_Credits::UpdateCredits
========================
*/
void idMenuScreen_Shell_Credits::UpdateCredits()
{

	if( menuData == NULL || GetSWFObject() == NULL )
	{
		return;
	}
	
	if( menuData->ActiveScreen() != SHELL_AREA_CREDITS && menuData->NextScreen() != SHELL_AREA_CREDITS )
	{
		return;
	}
	
	if( creditIndex >= creditList.Num() + NUM_CREDIT_LINES )
	{
		idMenuHandler_Shell* shell = dynamic_cast< idMenuHandler_ShellLocal* >( menuData );
		bool complete = false;
		if( shell != NULL )
		{
			complete = shell->GetGameComplete();
		}
		
		if( complete )
		{
			cmdSystem->BufferCommandText( CMD_EXEC_NOW, "disconnect" );
		}
		else
		{
			menuData->SetNextScreen( SHELL_AREA_ROOT, MENU_TRANSITION_SIMPLE );
		}
		return;
	}
	
	idSWFScriptObject* options = GetSWFObject()->GetRootObject().GetNestedObj( "menuCredits", "info", "options" );
	if( options != NULL )
	{
		for( int i = 15; i >= 0; --i )
		{
			int curIndex = creditIndex - i;
			idSWFTextInstance* heading = options->GetNestedText( va( "item%d", 15 - i ), "heading" );
			idSWFTextInstance* subHeading = options->GetNestedText( va( "item%d", 15 - i ), "subHeading" );
			idSWFTextInstance* title = options->GetNestedText( va( "item%d", 15 - i ), "title" );
			idSWFTextInstance* txtEntry = options->GetNestedText( va( "item%d", 15 - i ), "entry" );
			
			if( curIndex >= 0 && curIndex < creditList.Num() )
			{
			
				int type = creditList[ curIndex ].type;
				idStr entry = creditList[ curIndex ].entry;
				
				if( heading )
				{
					heading->SetText( type == 3 ? entry : "" );
					heading->SetStrokeInfo( true );
				}
				
				if( subHeading )
				{
					subHeading->SetText( type == 2 ? entry : "" );
					subHeading->SetStrokeInfo( true );
				}
				
				if( title )
				{
					title->SetText( type == 1 ? entry : "" );
					title->SetStrokeInfo( true );
				}
				
				if( txtEntry )
				{
					txtEntry->SetText( type == 0 ? entry : "" );
					txtEntry->SetStrokeInfo( true );
				}
				
			}
			else
			{
			
				if( heading )
				{
					heading->SetText( "" );
				}
				
				if( subHeading )
				{
					subHeading->SetText( "" );
				}
				
				if( txtEntry )
				{
					txtEntry->SetText( "" );
				}
				
				if( title )
				{
					title->SetText( "" );
				}
				
			}
		}
		if( options->GetSprite() )
		{
			options->GetSprite()->PlayFrame( "roll" );
		}
	}
	
	creditIndex++;
}