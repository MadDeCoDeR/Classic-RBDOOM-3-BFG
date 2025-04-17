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

#include "Precompiled.h"
#include "globaldata.h"
#include "doomlib.h"
#include <assert.h>
#include "Main.h"
#include "sys/sys_session.h"
#include "idlib/Thread.h"


#include <sys/types.h>
#include <map>

// Store master volume settings in archived cvars, becausue we want them to apply
// even if a user isn't signed in.
// The range is from 0 to 15, which matches the setting in vanilla DOOM.
idCVar s_volume_sound( "s_volume_sound", "8", CVAR_ARCHIVE | CVAR_INTEGER, "sound volume", 0, 15 );
idCVar s_volume_midi( "s_volume_midi", "8", CVAR_ARCHIVE | CVAR_INTEGER, "music volume", 0, 15 );
idCVar m_show_messages( "m_show_messages", "1", CVAR_ARCHIVE | CVAR_INTEGER, "show messages", 0, 1 );
idCVar m_inDemoMode( "m_inDemoMode", "1", CVAR_INTEGER, "in demo mode", 0, 1 );

extern idCVar in_joystickRumble;
extern idCVar r_useHDR;

bool	globalNetworking	= false;
bool	globalPauseTime		= false;
int		PLAYERCOUNT			= 1;
#ifdef _WINDOWS //GK: Linux already have that
typedef int socklen_t;
#endif


#ifdef _DEBUG
bool	debugOutput			= true;
#else
bool	debugOutput			= false;
#endif

namespace DoomLib
{
	static const char * Expansion_Names[] = {
		"Ultimate DOOM", "DOOM II: Hell On Earth", "Final DOOM: TNT Evilution", "Final DOOM: Plutonia Experiment", "DOOM II: Master Levels", "DOOM II: No Rest For The Living"
	};

	static const char* Skill_Names[] = {
		"I'm Too Young To Die!", "Hey, Not Too Rough!", "Hurt Me Plenty!", "Ultra-Violence", "Nightmare"
	};

	static const char* Filter_Names[] = {
		"#str_friends", "#str_around", "#str_top15"
	};

	// Game-specific setup values.
	static const char *  Doom_MapNames[] = {
		"E1M1: Hangar", "E1M2: Nuclear Plant", "E1M3: Toxin Refinery", "E1M4: Command Control", "E1M5: Phobos Lab", "E1M6: Central Processing", "E1M7: Computer Station", "E1M8: Phobos Anomaly", "E1M9: Military Base",
		"E2M1: Deimos Anomaly", "E2M2: Containment Area", "E2M3: Refinery", "E2M4: Deimos Lab", "E2M5: Command Center", "E2M6: Halls of the Damned", "E2M7: Spawning Vats", "E2M8: Tower of Babel",  "E2M9: Fortress of Mystery",
		"E3M1: Hell Keep", "E3M2: Slough of Despair", "E3M3: Pandemonium", "E3M4: House of Pain", "E3M5: Unholy Cathedral", "E3M6: MT. Erebus", "E3M7: Gate to Limbo", "E3M8: DIS", "E3M9: Warrens", 
		"E4M1: Hell Beneath", "E4M2: Perfect Hatred", "E4M3: Sever The Wicked", "E4M4: Unruly Evil", "E4M5: They Will Repent", "E4M6: Against Thee Wickedly", "E4M7: And Hell Followed", "E4M8: Unto The Cruel", "E4M9: Fear"
	};

	static const char * Doom2_MapNames[] = {
		"1: Entryway", "2: Underhalls", "3: The Gantlet", "4: The Focus", "5: The Waste Tunnels", "6: The Crusher", "7: Dead Simple", "8: Tricks and Traps", "9: The Pit", "10: Refueling Base", 
		"11: Circle of Death", "12: The Factory", "13: Downtown", "14: The Inmost Dens", "15: Industrial Zone", "16: Suburbs", "17: Tenements", "18: The Courtyard", "19: The Citadel", "20: Gotcha!", 
		"21: Nirvana", "22: The Catacombs", "23: Barrels O' Fun", "24: The Chasm", "25: Bloodfalls", "26: The Abandoned Mines", "27: Monster Condo", "28: The Spirit World", "29: The Living End",
		"30: Icon of Sin", "31: IDKFA", "32: Keen"
	};

	static const char * TNT_MapNames[] = {
		"1: System Control", "2: Human BBQ", "3: Power Control", "4: Wormhole", "5: Hangar", "6: Open Season", "7: Prison", "8: Metal", "9: Stronghold", "10: Redemption", "11: Storage Facility",
		"12: Crater", "13: Nukage Processing", "14: Steel Works", "15: Dead Zone", "16: Deepest Reaches", "17: Processing Area", "18: Mill", "19: Shipping & Respawning", "20: Central Processing",
		"21: Administration Center", "22: Habitat", "23: Lunar Mining Project", "24: Quarry", "25: Baron's Den", "26: Ballistyx", "27: Mount Pain", "28: Heck", "29: River Styx", "30: Last Call", "31: Pharaoh", "32: Caribbean"
	};

	static const char * Plut_MapNames[] = {
		"1: Congo", "2: Well of Souls", "3: Aztec", "4: Caged", "5: Ghost Town", "6: Baron's Lair", "7: Caughtyard", "8: Realm", "9: Abattoire", "10: Onslaught", "11: Hunted", "12: Speed", "13: The Crypt", "14: Genesis", 
		"15: The Twilight", "16: The Omen", "17: Compound", "18: Neurosphere", "19: NME", "20: The Death Domain", "21: Slayer", "22: Impossible Mission", "23: Tombstone", "24: The Final Frontier", "25: The Temple of Darkness",
		"26: Bunker", "27: Anti-Christ", "28: The Sewers", "29: Odyssey of Noises", "30: The Gateway of Hell", "31: Cyberden", "32: Go 2 It"
	};

	const char * Mast_MapNames[] = {
		"1: Attack", "2: Canyon","3: The Catwalk", "4: The Combine", "5: The Fistula", "6: The Garrison", "7: Titan Manor", "8: Paradox", "9: Subspace", "10: Subterra", "11: Trapped On Titan", "12: Virgil's Lead", "13: Minos' Judgement", 
		"14: Bloodsea Keep", "15: Mephisto's Maosoleum", "16: Nessus", "17: Geryon", "18: Vesperas", "19: Black Tower", "20: The Express Elevator To Hell", "21: Bad Dream"
	};

	const char * Nerve_MapNames[] = {
		"1: The Earth Base", "2: The Pain Labs", "3: Canyon of the Dead", "4: Hell Mountain", "5: Vivisection", "6: Inferno of Blood", "7: Baron's Banquet", "8: Tomb of Malevolence", "9: March of Demons"
	};

	const ExpansionData App_Expansion_Data_Local[] = {
		{	ExpansionData::IWAD, retail,		doom,			"DOOM",								DOOMWADDIR"DOOM.WAD",		{},																											"base/textures/DOOMICON.PNG"	, Doom_MapNames },
		{	ExpansionData::IWAD, commercial,	doom2,			"DOOM 2",							DOOMWADDIR"DOOM2.WAD",		{},																											"base/textures/DOOM2ICON.PNG"	, Doom2_MapNames },
		{	ExpansionData::IWAD, commercial,	pack_tnt,		"FINAL DOOM: TNT EVILUTION",		DOOMWADDIR"TNT.WAD",		{},																											"base/textures/TNTICON.PNG"		, TNT_MapNames },
		{	ExpansionData::IWAD, commercial,	pack_plut,		"FINAL DOOM: PLUTONIA EXPERIMENT",	DOOMWADDIR"PLUTONIA.WAD",	{},																											"base/textures/PLUTICON.PNG"	, Plut_MapNames },
		{	ExpansionData::PWAD, commercial,	pack_master,	"DOOM 2: MASTER LEVELS",			DOOMWADDIR"DOOM2.WAD",		{DOOMWADDIR"MASTERLEVELS.WAD", DOOMWADDIR"mlbls.wad"},														"base/textures/MASTICON.PNG"	, Mast_MapNames },
		{	ExpansionData::PWAD, commercial,	pack_nerve,		"DOOM 2: NO REST FOR THE LIVING",	DOOMWADDIR"DOOM2.WAD",		{DOOMWADDIR"NERVE.WAD"},																					"base/textures/NERVEICON.PNG"	, Nerve_MapNames },
		{	ExpansionData::PWAD, retail,		pack_romero,	"DOOM: SIGIL",						DOOMWADDIR"DOOM.WAD",		{DOOMWADDIR"SIGIL.WAD", DOOMWADDIR"SIGIL_BFG.wad", DOOMWADDIR"SIGIL2.WAD", DOOMWADDIR"SIGIL_II_BFG.wad"},	"base/textures/SIGIL.PNG"		, Doom_MapNames },
		{	ExpansionData::PWAD, commercial,	pack_lor,		"DOOM: Legacy Of Rust",				DOOMWADDIR"DOOM2.WAD",		{DOOMWADDIR"ID1.WAD", DOOMWADDIR"ID1_BFG.wad"},																"base/textures/LOR.PNG"			, Doom2_MapNames },
		{	ExpansionData::IWAD, shareware,		doom,			"DOOM Shareware",					DOOMWADDIR"DOOM1.WAD",		{},																										    "base/textures/DOOMICON.PNG"	, Doom_MapNames }
	};

	int classicRemap[K_LAST_KEY];

	std::map<std::string, int> classicRemapMap;

	const ExpansionData * GetCurrentExpansion() {
		return &App_Expansion_Data_Local[ DoomLib::expansionSelected ];
	}

	const ExpansionData* GetDemo() {
		return &App_Expansion_Data_Local[6];
	}

	void				  SetCurrentExpansion( int expansion )  { 
		expansionDirty = true; 
		expansionSelected = expansion; 
	}

	void						SetIdealExpansion( int expansion ) {
		idealExpansion = expansion;
	}

	idStr						currentMapName;
	idStr						currentDifficulty;

	void						SetCurrentMapName( idStr name ) { currentMapName = name; }
	const idStr &				GetCurrentMapName() { return currentMapName; }
	void						SetCurrentDifficulty( idStr name ) { currentDifficulty = name; }
	const idStr &				GetCurrentDifficulty() { return currentDifficulty; }

	int currentplayer = -1;

	Globals *globaldata[4];
	//GK:They're now on use (I think)
	RecvFunc Recv;
	SendFunc Send;
	SendRemoteFunc SendRemote;


	bool							Active = true;
	DoomInterface					Interface;

	int								idealExpansion = 0;
	int								expansionSelected = 0;
	bool							expansionDirty = true;

	bool							skipToLoad = false;
	char							loadGamePath[MAX_PATH];

	bool							skipToNew = false;
	int								chosenSkill = 0;
	int								chosenEpisode = 1;
	bool							use_doomit = false;
	bool							skipToMenu = false;
	bool							hexp[6];
	int								selection = -1;
	char*							otherfiles[5][20];
	char*							generalfiles[20]; //GK:Keep track of global mods for save file checks

	idMatchParameters				matchParms;

	static float							high;
	static int								highDuration;
	static float							low;
	static int								lowDuration;

	void * (*Z_Malloc)( unsigned size, int tag, void* user ) = NULL;
	void	(*Z_Free)(void* user) = NULL;
	void 	(*Z_FreeTag)(int lowtag );

	idArray< idSysMutex, 4 >		playerScreenMutexes;

	void ExitGame() {
		// TODO: If we ever support splitscreen and online,
		// we'll have to call D_QuitNetGame for all local players.
		DoomLib::SetPlayer( 0 );
		D_QuitNetGame();

		session->QuitMatch();
	}

	void ShowXToContinue( bool activate ) {
	}

	/*
	========================
	DoomLib::GetGameSKU
	========================
	*/
	gameSKU_t GetGameSKU() {
	
		if ( common->GetCurrentGame() == DOOM_CLASSIC ) {
			return GAME_SKU_DOOM1_BFG;
		} else if ( common->GetCurrentGame() == DOOM2_CLASSIC ) {
			return GAME_SKU_DOOM2_BFG;
		}

		assert( false && "Invalid basepath" );
		return GAME_SKU_DCC;
	}

	/*
	========================
	DoomLib::ActivateGame
	========================
	*/
	void ActivateGame() {
		Active = true;

		// Turn off menu toggler
		int originalPlayer = DoomLib::GetPlayer();

		for ( int i = 0; i < Interface.GetNumPlayers(); i++ ) {
			DoomLib::SetPlayer(i);
			::g->menuactive = false;
		}

		globalPauseTime = false;

		DoomLib::SetPlayer( originalPlayer );
	}

	/*
	========================
	DoomLib::HandleEndMatch
	========================
	*/
	void HandleEndMatch() {
		if ( session->GetGameLobbyBase().IsHost() ) {
			ShowXToContinue( false );
			session->EndMatch();
		}
	}
};


extern void I_InitGraphics();
extern void D_DoomMain();
extern bool D_DoomMainPoll();
extern void I_InitInput();
extern void D_RunFrame( bool );
extern void I_ShutdownSound();
extern void I_ShutdownMusic();
extern void I_ShutdownGraphics();
extern void I_ProcessSoundEvents();


void DoomLib::InitGlobals( void *ptr /*= NULL */ )
{
	if (ptr == NULL)
		ptr = new Globals;

	globaldata[currentplayer] = static_cast<Globals*>(ptr);

	memset( globaldata[currentplayer], 0, sizeof(Globals) );
	g = globaldata[currentplayer];
	g->InitGlobals();
	init_cptrs();
	
}

void *DoomLib::GetGlobalData( int player ) {
	return globaldata[player];
}

void DoomLib::InitControlRemap() {

	memset( classicRemap, K_NONE, sizeof( classicRemap ) );

	classicRemap[K_JOY3] = KEY_TAB ; 
	classicRemap[K_JOY4] = K_MINUS;
	classicRemap[K_JOY2] = K_EQUALS;
	classicRemap[K_JOY9] = K_ESCAPE ;
	classicRemap[K_JOY_STICK1_UP] = K_UPARROW ;
	classicRemap[K_JOY_DPAD_UP] = K_UPARROW ;
	classicRemap[K_JOY_STICK1_DOWN] = K_DOWNARROW ;
	classicRemap[K_JOY_DPAD_DOWN] = K_DOWNARROW ;
	classicRemap[K_JOY_STICK1_LEFT] = K_LEFTARROW ;
	classicRemap[K_JOY_DPAD_LEFT] = K_LEFTARROW ;
	classicRemap[K_JOY_STICK1_RIGHT] = K_RIGHTARROW ;
	classicRemap[K_JOY_DPAD_RIGHT] = K_RIGHTARROW ;	
	classicRemap[K_JOY1] = K_ENTER;
	classicRemapMap.insert(std::pair<std::string, int>("_impulse19", KEY_TAB));
	classicRemapMap.insert(std::pair<std::string, int>("help", KEY_F1));
	classicRemapMap.insert(std::pair<std::string, int>("savegame", KEY_F2));
	classicRemapMap.insert(std::pair<std::string, int>("loadgame", KEY_F3));
	classicRemapMap.insert(std::pair<std::string, int>("soundsetting", KEY_F4));
	classicRemapMap.insert(std::pair<std::string, int>("rlight", KEY_F5));
	classicRemapMap.insert(std::pair<std::string, int>("savegame quick", KEY_F6));
	classicRemapMap.insert(std::pair<std::string, int>("endgame", KEY_F7));
	classicRemapMap.insert(std::pair<std::string, int>("messages", KEY_F8));
	classicRemapMap.insert(std::pair<std::string, int>("loadgame quick", KEY_F9));
	classicRemapMap.insert(std::pair<std::string, int>("exitgame", KEY_F10));
	classicRemapMap.insert(std::pair<std::string, int>("gamma", KEY_F11));

}

keyNum_t DoomLib::RemapControl( keyNum_t key ) {

	if( classicRemap[ key ] == K_NONE ) {
		std::string s(idKeyInput::GetBinding(key));
		if (classicRemapMap.find(s) != classicRemapMap.end()) {
			return (keyNum_t)classicRemapMap[idKeyInput::GetBinding(key)];
		}
		else {
			return key;
		}
	} else {

		if( ::g->menuactive && key == K_JOY2 ) {
			return K_BACKSPACE;
		}

		return (keyNum_t)classicRemap[ key ];
	}

}

void DoomLib::InitGame( int argc, char** argv )
{
	::g->myargc = argc;
	::g->myargv = argv;

	InitControlRemap();

	D_DoomMain();
}

bool DoomLib::Poll()
{
	return D_DoomMainPoll();
}

bool TryRunTics( idUserCmdMgr * userCmdMgr );
bool DoomLib::Tic( idUserCmdMgr * userCmdMgr )
{
	return TryRunTics( userCmdMgr );
}

void D_Wipe();
void DoomLib::Wipe()
{
	D_Wipe();
}

void DoomLib::Frame( int realoffset, int buffer )
{
	::g->realoffset = realoffset;

	// The render thread needs to read the player's screens[0] array,
	// so updating it needs to be in a critical section.
	// This may seem like a really broad mutex (which it is), and if performance
	// suffers too much, we can try to narrow the scope.
	// Just be careful, because the player's screen data is updated in many different
	// places.
	if ( 0 <= currentplayer && currentplayer <= 4 ) {
		idScopedCriticalSection crit( playerScreenMutexes[currentplayer] );

		D_RunFrame( true );
	}
}

void DoomLib::Draw()
{
	R_RenderPlayerView (&::g->players[::g->displayplayer]);
}

angle_t GetViewAngle()
{
	return g->viewangle;
}
//GK: With tis function we are getting the mouse pitch for freelook
angle_t GetViewAngle2()
{
	return g->viewangle2;
}

void SetViewAngle( angle_t ang,angle_t viewang )
{
	g->viewangle = ang;
	g->viewangle2 = viewang;
	::g->viewxoffset = (finesine[g->viewangle>>ANGLETOFINESHIFT]*::g->realoffset) >> 8;
	::g->viewyoffset = (finecosine[g->viewangle>>ANGLETOFINESHIFT]*::g->realoffset) >> 8;
	
}


void SetViewX( fixed_t x )
{
	::g->viewx = x;
}

void SetViewY( fixed_t y )
{
	::g->viewy = y;
}


fixed_t GetViewX()
{
	return ::g->viewx + ::g->viewxoffset;
}

fixed_t GetViewY()
{
	return ::g->viewy + ::g->viewyoffset;
}

template<class T>
void CleanVector(std::vector<T*> myVector) {
	for (size_t i = 0; i < myVector.size(); i++) {
		if (myVector[i] != NULL) {
			void* val = myVector[i];
			myVector[i] = NULL;
			delete (T*)val;
		}
	}
}

void DoomLib::Shutdown() {
	//GK: Reset Dehacked patches also here just in case
	/*resetValues();
	resetWeapons();
	ResetAmmo();
	resetMapNames();
	//resetEndings();
	resetTexts();
	resetSprnames();
	//D_QuitNetGame ();*/
	I_ShutdownSound();
	I_ShutdownGraphics();

	W_Shutdown();

	// De-allocate the zone memory (never happened in original doom, until quit)
	if ( ::g->mainzone ) {
		free( ::g->mainzone );
	}

	// Delete the globals
	if ( globaldata[currentplayer] ) {
		Globals* glob = globaldata[currentplayer];
		globaldata[currentplayer] = NULL;
		//CleanVector(glob->activeceilings); //GK: Contains memory managed by the Zone Memory. Skip
		//CleanVector(glob->activeplats); //GK: Contains memory managed by the Zone Memory. Skip
		glob->intercepts.clear();
		glob->drawsegs.clear();
		CleanVector(glob->sprites);
		glob->vissprites.clear();
		CleanVector(glob->cpatch);
		glob->visplanes.clear();
		CleanVector(glob->reverbs);
		glob->sector_list.clear();
		for (size_t j = 0; j < glob->acts.size(); j++) {
			CleanVector(glob->acts[j]);
		}
		free(glob->EpisodeMenu);
		if (glob->nummapthings) {
			free(glob->mapthings);
		}
		delete glob;
	}
}

// static
void DoomLib::SetPlayer( int id )
{
	currentplayer = id;

	if ( id < 0 || id >= MAX_PLAYERS ) {
		g = NULL;
	}
	else {

		// Big Fucking hack.
		if( globalNetworking && session->GetGameLobbyBase().GetMatchParms().matchFlags | MATCH_ONLINE ) {
			currentplayer = 0;
		} 
		
		g = globaldata[currentplayer];
	}
}

#if 1 //GK:Revival of the un-implemented
void DoomLib::SetNetworking( RecvFunc rf, SendFunc sf, SendRemoteFunc sendRemote )
{
	Recv = rf;
	Send = sf;
	SendRemote = sendRemote;
}
#endif

int DoomLib::GetPlayer() 
{ 
	return currentplayer; 
}

byte DoomLib::BuildSourceDest( int toNode ) {
	byte sourceDest = 0;
	sourceDest |= ::g->consoleplayer << 2;
	sourceDest |= RemoteNodeToPlayerIndex( toNode );
	return sourceDest;
}

void I_Printf(char *error, ...);

void DoomLib::GetSourceDest( byte sourceDest, int* source, int* dest ) {

	int src = (sourceDest & 12) >> 2;
	int dst = sourceDest & 3;

	*source = PlayerIndexToRemoteNode( src );

	//I_Printf( "GetSourceDest Current Player(%d) %d --> %d\n", GetPlayer(), src, *source );
	*dest = PlayerIndexToRemoteNode( dst );
}

int nodeMap[4][4] = {
	{0, 1, 2, 3},	//Player 0
	{1, 0, 2, 3},	//Player 1
	{2, 0, 1, 3},	//Player 2
	{3, 0, 1, 2}	//Player 3
};

int DoomLib::RemoteNodeToPlayerIndex( int node ) {
	//This needs to be called with the proper doom globals set so this calculation will work properly
	
	/*
	int player = ::g->consoleplayer;
	if (player == 2 && node == 2 ) {
		int suck = 0;
	}
	if( node == player ) {
		return 0;
	}
	if( node - player <= 0 ) {
		return node+1;
	}
	return node;*/
	return nodeMap[::g->consoleplayer][node];

}

int indexMap[4][4] = {
	{0, 1, 2, 3},	//Player 0
	{1, 0, 2, 3},	//Player 1
	{1, 2, 0, 3},	//Player 2
	{1, 2, 3, 0}	//Player 3
};

int DoomLib::PlayerIndexToRemoteNode( int index ) {
	/*int player = ::g->consoleplayer;
	if( index == 0 ) {
		return player;
	}
	if( index <= player ) {
		return index-1;
	}
	return index;*/
	return indexMap[::g->consoleplayer][index];
}

void I_Error (const char *error, ...);
bool useTech5Packets;

void DoomLib::PollNetwork() {
#if 1
	if ( !useTech5Packets ) {

		if ( !globalNetworking ) {
			return;
		}

		//int			c;
		struct sockaddr	fromaddress;

		socklen_t		fromlen;

		doomdata_t		sw;

		while(1) {
			int receivedSize = recvfrom( ::g->insocket,(char *) &sw, sizeof( doomdata_t ), MSG_PEEK, &fromaddress, &fromlen ); //GK:Convert to char array and put another flag in order to deal with compatibility issues
			//c = WSARecvFrom(::g->insocket, &buffer, 1, &num_recieved, &flags, (struct sockaddr*)&fromaddress, &fromlen, 0, 0);

			if ( receivedSize < 0 )
			{
#ifdef _WINDOWS //GK: WSA is windows exclusive
				int err = WSAGetLastError();  //GK:We're using winsocks from now on
				if (err != WSAEINPROGRESS) {
					I_Error ("GetPacket: %d", err );
					//I_Printf ("GetPacket: %s",strerror(errno));
				}
#endif
				return;
			}

			common->Printf( "RECEIVED PACKET!!\n" );

			int source;
			int dest;
			GetSourceDest( sw.sourceDest, &source, &dest );

			//Push the packet onto the network stack to be processed.
			DoomLib::Send( (char*)&sw, receivedSize, NULL, dest );
		}
	}
#endif
}

void DoomLib::SendNetwork() {

	if ( !globalNetworking ) {
		return;
	}
	//DoomLib::SendRemote();
}

void DoomLib::RunSound() {

	I_ProcessSoundEvents();
}

void DoomLib::SetRumble(float high, int highDuration, float low, int lowDuration) {
	DoomLib::high = high;
	DoomLib::highDuration = highDuration;
	DoomLib::low = low;
	DoomLib::lowDuration = lowDuration;
}

void DoomLib::ApplyRumble() {
	if (idLib::joystick && in_joystickRumble.GetBool()) {
		Sys_SetRumble(session->GetSignInManager().GetMasterLocalUser()->GetInputDevice(), low, high);
		highDuration--;
		lowDuration--;
		if (highDuration <= 0) {
			high = 0;
		}
		if (lowDuration <= 0) {
			low = 0;
		}
	}
}