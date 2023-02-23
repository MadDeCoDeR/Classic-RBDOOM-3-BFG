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
#ifndef __MENUDATA_H__
#define __MENUDATA_H__

enum shellAreas_t
{
	SHELL_AREA_INVALID = -1,
	SHELL_AREA_START,
	SHELL_AREA_ROOT,
	SHELL_AREA_DEV,
	SHELL_AREA_CAMPAIGN,
	SHELL_AREA_LOAD,
	SHELL_AREA_SAVE,
	SHELL_AREA_NEW_GAME,
	SHELL_AREA_GAME_OPTIONS,
	SHELL_AREA_SYSTEM_OPTIONS,
	SHELL_AREA_MULTIPLAYER,
	SHELL_AREA_GAME_LOBBY,
	SHELL_AREA_STEREOSCOPICS,
	SHELL_AREA_PARTY_LOBBY,
	SHELL_AREA_SETTINGS,
	SHELL_AREA_AUDIO,
	SHELL_AREA_VIDEO,
	SHELL_AREA_KEYBOARD,
	SHELL_AREA_CONTROLS,
	SHELL_AREA_CONTROLLER_LAYOUT,
	SHELL_AREA_GAMEPAD,
	SHELL_AREA_ADV_CONTROLS,
	SHELL_AREA_PAUSE,
	SHELL_AREA_LEADERBOARDS,
	SHELL_AREA_PLAYSTATION,
	SHELL_AREA_DIFFICULTY,
	SHELL_AREA_RESOLUTION,
	SHELL_AREA_MATCH_SETTINGS,
	SHELL_AREA_MODE_SELECT,
	SHELL_AREA_BROWSER,
	SHELL_AREA_CREDITS,
	SHELL_AREA_ADVANCED, //GK: New option stuff
	SHELL_AREA_ADV_GRAPHICS,
	SHELL_NUM_AREAS
};

enum shellState_t
{
	SHELL_STATE_INVALID = -1,
	SHELL_STATE_PRESS_START,
	SHELL_STATE_IDLE,
	SHELL_STATE_PARTY_LOBBY,
	SHELL_STATE_GAME_LOBBY,
	SHELL_STATE_PAUSED,
	SHELL_STATE_CONNECTING,
	SHELL_STATE_SEARCHING,
	SHELL_STATE_LOADING,
	SHELL_STATE_BUSY,
	SHELL_STATE_IN_GAME
};

enum pdaAreas_t
{
	PDA_AREA_INVALID = -1,
	PDA_AREA_USER_DATA,
	PDA_AREA_USER_EMAIL,
	PDA_AREA_VIDEO_DISKS,
	PDA_AREA_INVENTORY,
	PDA_NUM_AREAS
};

enum hudArea_t
{
	HUD_AREA_INVALID = -1,
	HUD_AREA_PLAYING,
	HUD_NUM_AREAS
};

enum scoreboardArea_t
{
	SCOREBOARD_AREA_INVALID = -1,
	SCOREBOARD_AREA_DEFAULT,
	SCOREBOARD_AREA_TEAM,
	SCOREBOARD_AREA_CTF,
	SCOREBOARD_NUM_AREAS
};

enum pdaHandlerWidgets_t
{
	PDA_WIDGET_NAV_BAR,
	PDA_WIDGET_PDA_LIST,
	PDA_WIDGET_PDA_LIST_SCROLLBAR,
	PDA_WIDGET_CMD_BAR
};

enum scoreboardHandlerWidgets_t
{
	SCOREBOARD_WIDGET_CMD_BAR,
};

enum menuSounds_t
{
	GUI_SOUND_MUSIC,
	GUI_SOUND_SCROLL,
	GUI_SOUND_ADVANCE,
	GUI_SOUND_BACK,
	GUI_SOUND_BUILD_ON,
	GUI_SOUND_BUILD_OFF,
	GUI_SOUND_FOCUS,
	GUI_SOUND_ROLL_OVER,
	GUI_SOUND_ROLL_OUT,
	NUM_GUI_SOUNDS,
};

static const int MAX_SCREEN_AREAS = 32;
static const int DEFAULT_REPEAT_TIME = 150;
static const int WAIT_START_TIME_LONG = 30000;
static const int WAIT_START_TIME_SHORT = 5000;

struct actionRepeater_t
{
	actionRepeater_t() :
		widget( NULL ),
		numRepetitions( 0 ),
		nextRepeatTime( 0 ),
		repeatDelay( DEFAULT_REPEAT_TIME ),
		screenIndex( -1 ),
		isActive( false )
	{
	}
	
	idMenuWidget* 		widget;
	idWidgetEvent		event;
	idWidgetAction		action;
	int					numRepetitions;
	int					nextRepeatTime;
	int					repeatDelay;
	int					screenIndex;
	bool				isActive;
};

class mpScoreboardInfo
{
public:

	mpScoreboardInfo() :
		voiceState( VOICECHAT_DISPLAY_NONE ),
		score( 0 ),
		wins( 0 ),
		ping( 0 ),
		team( -1 ),
		playerNum( 0 )
	{
	}
	
	mpScoreboardInfo( const mpScoreboardInfo& src )
	{
		voiceState = src.voiceState;
		score = src.score;
		wins = src.wins;
		ping = src.ping;
		spectateData = src.spectateData;
		name = src.name;
		team = src.team;
		playerNum = src.playerNum;
	}
	
	void operator=( const mpScoreboardInfo& src )
	{
		voiceState = src.voiceState;
		score = src.score;
		wins = src.wins;
		ping = src.ping;
		spectateData = src.spectateData;
		name = src.name;
		team = src.team;
		playerNum = src.playerNum;
	}
	
	bool operator!=( const mpScoreboardInfo& otherInfo ) const
	{
	
		if( otherInfo.score != score || otherInfo.wins != wins || otherInfo.ping != ping ||
				otherInfo.spectateData != spectateData || otherInfo.name != name || otherInfo.team != team ||
				otherInfo.playerNum != playerNum || otherInfo.voiceState != voiceState )
		{
			return true;
		}
		
		return false;
	}
	
	bool operator==( const mpScoreboardInfo& otherInfo ) const
	{
	
		if( otherInfo.score != score || otherInfo.wins != wins || otherInfo.ping != ping ||
				otherInfo.spectateData != spectateData || otherInfo.name != name || otherInfo.team != team ||
				otherInfo.playerNum != playerNum || otherInfo.voiceState != voiceState )
		{
			return false;
		}
		
		return true;
	}
	
	voiceStateDisplay_t voiceState;
	int score;
	int wins;
	int ping;
	int team;
	int playerNum;
	idStr spectateData;
	idStr name;
	
};

/*
================================================
idMenuHandler
================================================
*/
class idMenuHandler
{
public:
	idMenuHandler();
	virtual					~idMenuHandler();
	virtual void			Initialize( const char* swfFile, idSoundWorld* sw );
	virtual void			Cleanup();
	virtual void			Update();
	virtual void			UpdateChildren();
	virtual void			UpdateMenuDisplay( int menu );
	virtual bool			HandleGuiEvent( const sysEvent_t* sev );
	virtual bool			IsActive();
	virtual void			ActivateMenu( bool show );
	virtual void			TriggerMenu();
	virtual bool			HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );
	virtual int				ActiveScreen()
	{
		return activeScreen;
	}
	virtual int				NextScreen()
	{
		return nextScreen;
	}
	virtual int				MenuTransition()
	{
		return transition;
	}
	virtual idMenuScreen* 	GetMenuScreen( int index )
	{
		return NULL;
	}
	virtual void			SetNextScreen( int screen, int trans )
	{
		nextScreen = screen;
		transition = trans;
	}
	
	virtual void			StartWidgetActionRepeater( idMenuWidget* widget, const idWidgetAction& action, const idWidgetEvent& event );
	virtual void			PumpWidgetActionRepeater();
	virtual void			ClearWidgetActionRepeater();
	virtual idSWF* 			GetGUI()
	{
		return gui;
	}
	virtual void			AddChild( idMenuWidget* widget );
	virtual idMenuWidget* 	GetChildFromIndex( int index );
	virtual int				GetPlatform( bool realPlatform = false );
	virtual void			PlaySound( menuSounds_t type, int channel = -1 );
	virtual void			StopSound( int channel = SCHANNEL_ANY );
	
	idMenuWidget_CommandBar* 	GetCmdBar()
	{
		return cmdBar;
	}
	
protected:

	bool						scrollingMenu;
	int							scrollCounter;
	int							activeScreen;
	int							nextScreen;
	int							transition;
	int							platform;
	idSWF* 						gui;
	actionRepeater_t			actionRepeater;
	idMenuScreen* 				menuScreens[MAX_SCREEN_AREAS];
	idList< idMenuWidget*, TAG_IDLIB_LIST_MENU>	children;
	
	idStaticList< idStr, NUM_GUI_SOUNDS >		sounds;
	
	idMenuWidget_CommandBar* 	cmdBar;
};

/*
================================================
lobbyPlayerInfo_t
================================================
*/
struct lobbyPlayerInfo_t
{
	lobbyPlayerInfo_t() :
		partyToken( 0 ),
		voiceState( VOICECHAT_DISPLAY_NONE )
	{
	}
	
	idStr					name;
	int						partyToken;
	voiceStateDisplay_t		voiceState;
};

/*
================================================
idMenuHandler_Shell
================================================
*/
//GK: Make these classes abstract for better communication with the dll
class idMenuHandler_Shell : public idMenuHandler
{
public:
	virtual void			Update() = 0;
	virtual void			ActivateMenu(bool show) = 0;
	virtual void			Initialize(const char* swfFile, idSoundWorld* sw) = 0;
	virtual void			Cleanup() = 0;
	virtual bool			HandleAction(idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false) = 0;
	virtual idMenuScreen* 	GetMenuScreen(int index) = 0;
	virtual bool			HandleGuiEvent(const sysEvent_t* sev) = 0;

	virtual void					UpdateSavedGames() = 0;
	virtual void					ShowSmallFrame(bool show) = 0;
	virtual void					ShowMPFrame(bool show) = 0;
	virtual void					ShowLogo(bool show) = 0;
	virtual void					SetShellState(shellState_t s) = 0;
	virtual bool					IsSmallFrameShowing() = 0;

	virtual void					UpdateBGState() = 0;
	//virtual void					GetMapName(int index, idStr& name) = 0;
	//virtual void					GetModeName(int index, idStr& name) = 0;

	virtual idMenuWidget* 			GetPacifier() = 0;

	virtual idMenuWidget_MenuBar* 	GetMenuBar() = 0;

	virtual bool					IsPacifierVisible() const = 0;

	virtual void					ShowPacifier(const idStr& msg) = 0;
	virtual void					HidePacifier() = 0;

	virtual void					SetTimeRemaining(int time) = 0;

	virtual int						GetTimeRemaining() = 0;

	virtual void					SetNewGameType(int type) = 0;

	virtual int						GetNewGameType() = 0;
	
	virtual void					SetInGame(bool val) = 0;

	virtual bool					GetInGame() = 0;
	
	virtual void					HandleExitGameBtn() = 0;
	virtual void					SetupPCOptions() = 0;
	virtual void					SetWaitForBinding(const char* bind) = 0;
	
	virtual void					ClearWaitForBinding() = 0;
	
	virtual void					UpdateLeaderboard(const idLeaderboardCallback* callback) = 0;
	virtual void					UpdateLobby(idMenuWidget_LobbyList* lobbyList) = 0;
	virtual void					ShowDoomIntro() = 0;
	virtual void					ShowROEIntro() = 0;
	virtual void					ShowLEIntro() = 0;
	virtual void					StartGame(int index) = 0;
	virtual void					SetContinueWaitForEnumerate(bool wait) = 0;
	
	virtual void					SetCanContinue(bool valid) = 0;
	virtual void					SetGameComplete() = 0;
	
	virtual bool					GetGameComplete() = 0;

};
class idMenuHandler_ShellLocal : public idMenuHandler_Shell
{
public:
	idMenuHandler_ShellLocal() :
		state(SHELL_STATE_INVALID),
		nextState(SHELL_STATE_INVALID),
		smallFrameShowing(false),
		largeFrameShowing(false),
		bgShowing(true),
		waitForBinding(false),
		waitBind(NULL),
		menuBar(NULL),
		pacifier(NULL),
		timeRemaining(0),
		nextPeerUpdateMs(0),
		newGameType(0),
		inGame(false),
		showingIntro(false),
		continueWaitForEnumerate(false),
		gameComplete(false),
		introGui(NULL),
		typeSoundShader(NULL),
		doom3Intro(NULL),
		roeIntro(NULL),
		lmIntro(NULL),
		marsRotation(NULL)
	{
	}
	virtual void			Update();
	virtual void			ActivateMenu( bool show );
	virtual void			Initialize( const char* swfFile, idSoundWorld* sw );
	virtual void			Cleanup();
	virtual bool			HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );
	virtual idMenuScreen* 	GetMenuScreen( int index );
	virtual bool			HandleGuiEvent( const sysEvent_t* sev );
	
	virtual void					UpdateSavedGames();
	virtual void					ShowSmallFrame( bool show );
	virtual void					ShowMPFrame( bool show );
	virtual void					ShowLogo( bool show );
	virtual void					SetShellState( shellState_t s )
	{
		nextState = s;
	}
	virtual bool					IsSmallFrameShowing()
	{
		return smallFrameShowing;
	}
	virtual void					UpdateBGState();
	//virtual void					GetMapName( int index, idStr& name );
	//virtual void					GetModeName( int index, idStr& name );
	
	virtual idMenuWidget* 			GetPacifier()
	{
		return pacifier;
	}
	virtual idMenuWidget_MenuBar* 	GetMenuBar()
	{
		return menuBar;
	}
	virtual bool					IsPacifierVisible() const
	{
		return ( pacifier != NULL && pacifier->GetSprite() != NULL ) ? pacifier->GetSprite()->IsVisible() : false;
	}
	virtual void					ShowPacifier( const idStr& msg );
	virtual void					HidePacifier();
	
	virtual void					SetTimeRemaining( int time )
	{
		timeRemaining = time;
	}
	virtual int						GetTimeRemaining()
	{
		return timeRemaining;
	}
	virtual void					SetNewGameType( int type )
	{
		newGameType = type;
	}
	virtual int						GetNewGameType()
	{
		return newGameType;
	}
	virtual void					SetInGame( bool val )
	{
		inGame = val;
	}
	virtual bool					GetInGame()
	{
		return inGame;
	}
	virtual void					HandleExitGameBtn();
	virtual void					SetupPCOptions();
	virtual void					SetWaitForBinding( const char* bind )
	{
		waitForBinding = true;
		waitBind = bind;
	}
	virtual void					ClearWaitForBinding()
	{
		waitForBinding = false;
	}
	virtual void					UpdateLeaderboard( const idLeaderboardCallback* callback );
	virtual void					UpdateLobby( idMenuWidget_LobbyList* lobbyList );
	virtual void					ShowDoomIntro();
	virtual void					ShowROEIntro();
	virtual void					ShowLEIntro();
	virtual void					StartGame( int index );
	virtual void					SetContinueWaitForEnumerate( bool wait )
	{
		continueWaitForEnumerate = wait;
	}
	virtual void					SetCanContinue( bool valid );
	virtual void					SetGameComplete()
	{
		gameComplete = true;
	}
	virtual bool					GetGameComplete()
	{
		return gameComplete;
	}
	
	private:

		shellState_t			state;
		shellState_t			nextState;
		bool					smallFrameShowing;
		bool					largeFrameShowing;
		bool					bgShowing;
		bool					waitForBinding;
		const char* 			waitBind;
		//idSysSignal				deviceRequestedSignal;

		idList<const char*, TAG_IDLIB_LIST_MENU>	mpGameModes;
		idList<mpMap_t, TAG_IDLIB_LIST_MENU>			mpGameMaps;
		idMenuWidget_MenuBar* 	menuBar;
		idMenuWidget* 			pacifier;
		int						timeRemaining;
		int						nextPeerUpdateMs;
		int						newGameType;
		bool					inGame;
		bool					showingIntro;
		bool					continueWaitForEnumerate;
		bool					gameComplete;
		idSWF* 					introGui;
		const idSoundShader* 	typeSoundShader;
		const idMaterial* 		doom3Intro;
		const idMaterial* 		roeIntro;
		const idMaterial* 		lmIntro;
		const idMaterial* 		marsRotation;
		idList< idStr, TAG_IDLIB_LIST_MENU>			navOptions;

};

/*
================================================
idMenuHandler_PDA
================================================
*/
class idMenuHandler_PDA : public idMenuHandler
{
public:
	virtual ~idMenuHandler_PDA() {};

	virtual void			Update() = 0;
	virtual void			ActivateMenu(bool show) = 0;
	virtual void			TriggerMenu() = 0;
	virtual void			Initialize(const char* swfFile, idSoundWorld* sw) = 0;
	virtual bool			HandleAction(idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false) = 0;
	virtual idMenuScreen* 	GetMenuScreen(int index) = 0;
	virtual void					UpdateAudioLogPlaying(bool playing) = 0;
	virtual void					UdpateVideoPlaying(bool playing) = 0;
	virtual void					ClearVideoPlaying() = 0;
	

	virtual bool					PlayPDAAudioLog(int pdaIndex, int audioIndex) = 0;
	virtual void			Cleanup() = 0;
};
class idMenuHandler_PDALocal : public idMenuHandler_PDA
{
public:

	idMenuHandler_PDALocal() :
		audioLogPlaying(false),
		videoPlaying(false),
		audioFile(NULL)
	{
	}
	virtual ~idMenuHandler_PDALocal();
	
	virtual void			Update();
	virtual void			ActivateMenu( bool show );
	virtual void			TriggerMenu();
	virtual void			Initialize( const char* swfFile, idSoundWorld* sw );
	virtual bool			HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );
	virtual idMenuScreen* 	GetMenuScreen( int index );
	virtual void					UpdateAudioLogPlaying( bool playing );
	virtual void					UdpateVideoPlaying( bool playing );
	virtual void					ClearVideoPlaying()
	{
		videoPlaying = false;
	}
	
	virtual bool					PlayPDAAudioLog( int pdaIndex, int audioIndex );
	virtual void			Cleanup();

protected:

	bool							audioLogPlaying;
	bool							videoPlaying;
	idList< idList< idStr, TAG_IDLIB_LIST_MENU >, TAG_IDLIB_LIST_MENU >		pdaNames;
	idList< idStr, TAG_IDLIB_LIST_MENU >					navOptions;
	const idDeclAudio* 				audioFile;
	idMenuWidget_ScrollBar pdaScrollBar;
	idMenuWidget_DynamicList pdaList;
	idMenuWidget_NavBar navBar;
	idMenuWidget_CommandBar commandBarWidget;
};

/*
================================================
idMenuHandler_PDA
================================================
*/
class idMenuHandler_HUD : public idMenuHandler
{
public:



	virtual void			Update()=0;
	virtual void			ActivateMenu(bool show)=0;
	virtual void			Initialize(const char* swfFile, idSoundWorld* sw)=0;
	virtual idMenuScreen* 	GetMenuScreen(int index)=0;

	virtual idMenuScreen_HUD* 		GetHud()=0;
	virtual void					ShowTip(const char* title, const char* tip, bool autoHide)=0;
	virtual void					HideTip()=0;
	virtual void					SetRadioMessage(bool show) = 0;
};
class idMenuHandler_HUDLocal : public idMenuHandler_HUD
{
public:

	idMenuHandler_HUDLocal() :
		autoHideTip(true),
		tipStartTime(0),
		hiding(false),
		radioMessage(false)
	{
	}
	
	virtual void			Update();
	virtual void			ActivateMenu( bool show );
	virtual void			Initialize( const char* swfFile, idSoundWorld* sw );
	virtual idMenuScreen* 	GetMenuScreen( int index );
	
	virtual idMenuScreen_HUD* 		GetHud();
	virtual void					ShowTip( const char* title, const char* tip, bool autoHide );
	virtual void					HideTip();
	virtual void					SetRadioMessage( bool show )
	{
		radioMessage = show;
	}

protected:

	bool	autoHideTip;
	int		tipStartTime;
	bool	hiding;
	bool	radioMessage;
	
};

/*
================================================
idMenuHandler_Scoreboard
================================================
*/
class idMenuHandler_Scoreboard : public idMenuHandler
{
public:
	virtual void			Update() = 0;
	virtual void			TriggerMenu() = 0;
	virtual void			ActivateMenu(bool show) = 0;
	virtual void			Initialize(const char* swfFile, idSoundWorld* sw) = 0;
	virtual idMenuScreen* 	GetMenuScreen(int index) = 0;
	virtual bool			HandleAction(idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false) = 0;

	virtual void					AddPlayerInfo(int index, voiceStateDisplay_t voiceState, int team, idStr name, int score, int wins, int ping, idStr spectateData) = 0;
	virtual void					UpdateScoreboard(idList< mpScoreboardInfo >& data, idStr gameInfo) = 0;
	//virtual void					UpdateVoiceStates() = 0;
	virtual void					UpdateSpectating(idStr spectate, idStr follow) = 0;
	virtual void					SetTeamScores(int r, int b) = 0;
	virtual int						GetNumPlayers(int team) = 0;
	virtual void					SetActivationScreen(int screen, int trans) = 0;
	virtual void					ViewPlayerProfile(int slot) = 0;
	virtual void					MutePlayer(int slot) = 0;
	virtual void					GetUserID(int slot, lobbyUserID_t& luid) = 0;
	virtual void					UpdateScoreboardSelection() = 0;

};
class idMenuHandler_ScoreboardLocal : public idMenuHandler_Scoreboard
{
public:
	idMenuHandler_ScoreboardLocal() :
		redScore(0),
		blueScore(0),
		activationScreen(SCOREBOARD_AREA_INVALID)
	{
	}
	
	virtual void			Update();
	virtual void			TriggerMenu();
	virtual void			ActivateMenu( bool show );
	virtual void			Initialize( const char* swfFile, idSoundWorld* sw );
	virtual idMenuScreen* 	GetMenuScreen( int index );
	virtual bool			HandleAction( idWidgetAction& action, const idWidgetEvent& event, idMenuWidget* widget, bool forceHandled = false );
	
	virtual void					AddPlayerInfo( int index, voiceStateDisplay_t voiceState, int team, idStr name, int score, int wins, int ping, idStr spectateData );
	virtual void					UpdateScoreboard( idList< mpScoreboardInfo >& data, idStr gameInfo );
	//virtual void					UpdateVoiceStates();
	virtual void					UpdateSpectating( idStr spectate, idStr follow );
	virtual void					SetTeamScores( int r, int b );
	virtual int						GetNumPlayers( int team );
	virtual void					SetActivationScreen( int screen, int trans );
	virtual void					ViewPlayerProfile( int slot );
	virtual void					MutePlayer( int slot );
	virtual void					GetUserID( int slot, lobbyUserID_t& luid );
	virtual void					UpdateScoreboardSelection();

protected:

	int		redScore;
	int		blueScore;
	int		activationScreen;

	idList< mpScoreboardInfo > scoreboardInfo;
	idList< scoreboardInfo_t, TAG_IDLIB_LIST_MENU >		redInfo;
	idList< scoreboardInfo_t, TAG_IDLIB_LIST_MENU>		blueInfo;
	
};


#endif //__MENUDATA_H__
