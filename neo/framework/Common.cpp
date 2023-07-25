/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012-2014 Robert Beckebans

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

#include "Common_local.h"

#include "ConsoleHistory.h"

#include "../sound/sound.h"


// RB begin
#if defined(USE_DOOMCLASSIC)
#include "../../doomclassic/doom/doomlib.h"
#include "../../doomclassic/doom/d_event.h"
#include "../../doomclassic/doom/d_main.h"
#include "../../doomclassic/doom/w_wad.h"
#include "../../doomclassic/doom/globaldata.h"
#endif
// RB end


#include "../sys/sys_savegame.h"

#include "../swf/credits/CreditDict.h"



#if defined( _DEBUG )
#define BUILD_DEBUG "-debug"
#else
#define BUILD_DEBUG ""
#endif

struct version_s
{
	/*version_s()
	{
		sprintf( string, "%s.%d%s %s %s %s", common->IsNewDOOM3() ? NEW_CONSOLE_NAME : CONSOLE_NAME, BUILD_NUMBER, BUILD_DEBUG, BUILD_STRING, __DATE__, __TIME__ );
	}*/
	char	string[256];
} version;

idCVar com_version( "si_version", version.string, CVAR_SYSTEM | CVAR_ROM | CVAR_SERVERINFO, "engine version" );
idCVar com_forceGenericSIMD( "com_forceGenericSIMD", "0", CVAR_BOOL | CVAR_SYSTEM | CVAR_NOCHEAT, "force generic platform independent SIMD" );

#ifdef ID_RETAIL
idCVar com_allowConsole( "com_allowConsole", "0", CVAR_BOOL | CVAR_SYSTEM | CVAR_INIT, "allow toggling console with the tilde key" );
#else
idCVar com_allowConsole( "com_allowConsole", "1", CVAR_BOOL | CVAR_SYSTEM | CVAR_INIT, "allow toggling console with the tilde key" );
#endif

idCVar com_developer( "developer", "0", CVAR_BOOL | CVAR_SYSTEM | CVAR_NOCHEAT, "developer mode" );
idCVar com_speeds( "com_speeds", "0", CVAR_BOOL | CVAR_SYSTEM | CVAR_NOCHEAT, "show engine timings" );
// DG: support "com_showFPS 2" for fps-only view like in classic doom3 => make it CVAR_INTEGER
idCVar com_showFPS( "com_showFPS", "0", CVAR_INTEGER | CVAR_SYSTEM | CVAR_ARCHIVE | CVAR_NOCHEAT, "show frames rendered per second. 0: off 1: default bfg values, 2: only show FPS (classic view)" );
// DG end
idCVar com_showMemoryUsage( "com_showMemoryUsage", "0", CVAR_INTEGER| CVAR_SYSTEM | CVAR_NOCHEAT | CVAR_ARCHIVE, "show total and per frame memory usage" );
idCVar com_updateLoadSize( "com_updateLoadSize", "0", CVAR_BOOL | CVAR_SYSTEM | CVAR_NOCHEAT, "update the load size after loading a map" );

idCVar com_productionMode( "com_productionMode", "0", CVAR_SYSTEM | CVAR_BOOL, "0 - no special behavior, 1 - building a production build, 2 - running a production build" );

idCVar com_japaneseCensorship( "com_japaneseCensorship", "0", CVAR_NOCHEAT, "Enable Japanese censorship" );

idCVar preload_CommonAssets( "preload_CommonAssets", "1", CVAR_SYSTEM | CVAR_BOOL, "preload common assets" );

idCVar net_inviteOnly( "net_inviteOnly", "1", CVAR_BOOL | CVAR_ARCHIVE, "whether or not the private server you create allows friends to join or invite only" );

// DG: add cvar for pause
idCVar com_pause( "com_pause", "0", CVAR_BOOL | CVAR_SYSTEM | CVAR_NOCHEAT, "set to 1 to pause game, to 0 to unpause again" );
// DG end
//GK: com_game_mode
idCVar com_game_mode("com_game_mode", "0", CVAR_INTEGER | CVAR_SYSTEM | CVAR_NOCHEAT, "Set which game to run 1: DOOM 2:DOOM2 3:DOOM3");
//GK: add cvar to pause the platform bump
idCVar com_pausePlatform("com_pausePlatform", "0", CVAR_BOOL | CVAR_SYSTEM | CVAR_NOCHEAT, "set to 1 to pause the platform bump, to 0 to unpause again");

idCVar com_emergencyexit("com_emergencyexit", "0", CVAR_BOOL | CVAR_ROM, "Stops Engine Initiation for faster shutdown");
//GK End
//extern idCVar g_demoMode; //GK: get it from game object

idCVar com_engineHz( "com_engineHz", "60", CVAR_FLOAT | CVAR_ARCHIVE, "Frames per second the engine runs at", 10.0f, 1024.0f );
idCVar cl_engineHz("cl_engineHz", "35", CVAR_FLOAT | CVAR_ARCHIVE, "Frames per second the classic engine runs at", 35.0f, 40.0f);
idCVar cl_engineHz_interp("cl_engineHz_interp", "0", CVAR_BOOL | CVAR_ARCHIVE, "Enable Classic Doom Engine Iterpolation");
float com_engineHz_latched = 60.0f; // Latched version of cvar, updated between map loads
int64 com_engineHz_numerator = 100LL * 1000LL;
int64 com_engineHz_denominator = 100LL * 60LL;

// RB begin
int com_editors = 0;

#if defined(_WIN32)
HWND com_hwndMsg = NULL;
#endif
// RB end

#ifndef __MONOLITH__ //GK: We want DOOM_DLL to be always on but also have monolithic executable
idGame* 		game = NULL;
idGameEdit* 	gameEdit = NULL;
#endif


intptr_t platformDLL = 0;

idCommonLocal	commonLocal;
idCommon* 		common = &commonLocal;

OPlatform* op;

GetClassicData_t GetClassicData = &GetClassicDoomData;

// RB: defaulted this to 1 because we don't have a sound for the intro .bik video
//GK : defaulted back to 0 the audio plays again (not perfectly but it will do for now)
idCVar com_skipIntroVideos( "com_skipIntroVideos", "0", CVAR_BOOL , "skips intro videos" );

// For doom classic
struct Globals;
extern idCVar r_aspectcorrect; //GK: And that is why you can't have cool stuff in the game
extern idCVar in_joyjpn;
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
extern idCVar s_useXAudio2;
#endif

/*
==================
idCommonLocal::idCommonLocal
==================
*/
idCommonLocal::idCommonLocal() :
	readSnapshotIndex( 0 ),
	writeSnapshotIndex( 0 ),
	optimalPCTBuffer( 0.5f ),
	optimalTimeBuffered( 0.0f ),
	optimalTimeBufferedWindow( 0.0f ),
	lastPacifierSessionTime( 0 ),
	lastPacifierGuiTime( 0 ),
	lastPacifierDialogState( false ),
	showShellRequested( false )
	// RB begin
#if defined(USE_DOOMCLASSIC)
	,
	currentGame( DOOM3_BFG ),
	idealCurrentGame( DOOM3_BFG ),
	doomClassicMaterial( NULL )
#endif
	// RB end
{

	snapCurrent.localTime = -1;
	snapPrevious.localTime = -1;
	snapCurrent.serverTime = -1;
	snapPrevious.serverTime = -1;
	snapTimeBuffered	= 0.0f;
	effectiveSnapRate	= 0.0f;
	totalBufferedTime	= 0;
	totalRecvTime		= 0;
	
	com_fullyInitialized = false;
	com_refreshOnPrint = false;
	com_errorEntered = ERP_NONE;
	com_shuttingDown = false;
	com_isJapaneseSKU = false;
	
	logFile = NULL;
	
	strcpy( errorMessage, "" );
	
	rd_buffer = NULL;
	rd_buffersize = 0;
	rd_flush = NULL;
	
	gameDLL = 0;
	
	loadGUI = NULL;
	nextLoadTip = 0;
	isHellMap = false;
	wipeForced = false;
	defaultLoadscreen = false;
	
	menuSoundWorld = NULL;
	
	insideUpdateScreen = false;
	insideExecuteMapChange = false;
	
	mapSpawnData.savegameFile = NULL;
	
	currentMapName.Clear();
	aviDemoShortName.Clear();
	
	renderWorld = NULL;
	soundWorld = NULL;
	menuSoundWorld = NULL;
	readDemo = NULL;
	writeDemo = NULL;
	
	gameFrame = 0;
	gameTimeResidual = 0;
	syncNextGameFrame = true;
	mapSpawned = false;
	aviCaptureMode = false;
	timeDemo = TD_NO;
	
	nextSnapshotSendTime = 0;
	nextUsercmdSendTime = 0;
	
	clientPrediction = 0;
	
	saveFile = NULL;
	stringsFile = NULL;

	photoMode = new PhotoMode();

	layoutchange = false;
	newd3 = false;
	usecustom = false;
	
	ClearWipe();
}

/*
==================
idCommonLocal::Quit
==================
*/
void idCommonLocal::Quit()
{
	if ((currentGame == DOOM_CLASSIC || currentGame == DOOM2_CLASSIC) && ::g != NULL) {
		//GK: Delete uncompressed files for classic DOOM mods AFTER un allocating them (since it is still in classic doom)
		CleanUncompFiles(true);
		game->SetCVarBool("cl_close", true);
	}
	else {
		//GK: Delete uncompressed files for classic DOOM mods WITHOUT un allocating them
		CleanUncompFiles(false);
	}
	// don't try to shutdown if we are in a recursive error
	if( !com_errorEntered )
	{
		Shutdown();
	}
	Printf("Something Went Wrong\n");
	Sys_Quit();
}


/*
============================================================================

COMMAND LINE FUNCTIONS

+ characters separate the commandLine string into multiple console
command lines.

All of these are valid:

doom +set test blah +map test
doom set test blah+map test
doom set test blah + map test

============================================================================
*/

#define		MAX_CONSOLE_LINES	128 //GK: MORE WORDS
int			com_numConsoleLines;
idCmdArgs	com_consoleLines[MAX_CONSOLE_LINES];

bool isnumber(char* arg) {
	int m = strlen(arg);
	bool isint = true;
	for (int i = 0; i < m; i++) {
		if (!isdigit(arg[i])) {
			isint = false;
			break;
		}
	}
	return isint;
}
/*
==================
idCommonLocal::ParseCommandLine
==================
*/
void idCommonLocal::ParseCommandLine( int argc, const char* const* argv )
{
	int i, current_count;
	
	com_numConsoleLines = 0;
	current_count = 0;
	//GK begin
	int j = 0;
	classicargv[j] = (char*)"doomlauncher";
	j++;
	//GK end
	// API says no program path
	for( i = 0; i < argc; i++ )
	{
		if( idStr::Icmp( argv[ i ], "+connect_lobby" ) == 0 )
		{
			// Handle Steam bootable invites.
			
			// RB begin
#if defined(_WIN32)
			session->HandleBootableInvite( _atoi64( argv[ i + 1 ] ) );
#else
			session->HandleBootableInvite( atol( argv[ i + 1 ] ) );
#endif
			// RB end
		}
		
		else if( argv[ i ][ 0 ] == '+' )
		{
			com_numConsoleLines++;
			com_consoleLines[ com_numConsoleLines - 1 ].AppendArg( argv[ i ] + 1 );
		}
		//GK begin
		else if (argv[i][0] == '-' && !isnumber(strdup(argv[i]+1))) {

			while (i<argc && argv[i][0] != '+') {
				classicargv[j] = strdup(argv[i]);
				i++;
				j++;
			}
			if (i >= argc)
				break;
			if (argv[i][0] == '+') {
				i = i - 1;
			}
		}
		//GK end
		else
		{
			if( !com_numConsoleLines )
			{
				com_numConsoleLines++;
			}
			com_consoleLines[ com_numConsoleLines - 1 ].AppendArg( argv[ i ] );
		}
	}
}

/*
==================
idCommonLocal::SafeMode

Check for "safe" on the command line, which will
skip loading of config file (DoomConfig.cfg)
==================
*/
bool idCommonLocal::SafeMode()
{
	int			i;
	
	for( i = 0 ; i < com_numConsoleLines ; i++ )
	{
		if( !idStr::Icmp( com_consoleLines[ i ].Argv( 0 ), "safe" )
				|| !idStr::Icmp( com_consoleLines[ i ].Argv( 0 ), "cvar_restart" ) )
		{
			com_consoleLines[ i ].Clear();
			return true;
		}
	}
	return false;
}

/*
==================
idCommonLocal::StartupVariable

Searches for command line parameters that are set commands.
If match is not NULL, only that cvar will be looked for.
That is necessary because cddir and basedir need to be set
before the filesystem is started, but all other sets should
be after execing the config and default.
==================
*/
void idCommonLocal::StartupVariable( const char* match )
{
	int i = 0;
	while(	i < com_numConsoleLines )
	{
		if( strcmp( com_consoleLines[ i ].Argv( 0 ), "set" ) != 0 )
		{
			i++;
			continue;
		}
		const char* s = com_consoleLines[ i ].Argv( 1 );
		
		if( !match || !idStr::Icmp( s, match ) )
		{
			cvarSystem->SetCVarString( s, com_consoleLines[ i ].Argv( 2 ) );
		}
		i++;
	}
}

/*
==================
idCommonLocal::AddStartupCommands

Adds command line parameters as script statements
Commands are separated by + signs

Returns true if any late commands were added, which
will keep the demoloop from immediately starting
==================
*/
void idCommonLocal::AddStartupCommands()
{
	// quote every token, so args with semicolons can work
	for( int i = 0; i < com_numConsoleLines; i++ )
	{
		if( !com_consoleLines[i].Argc() )
		{
			continue;
		}
		// directly as tokenized so nothing gets screwed
		cmdSystem->BufferCommandArgs( CMD_EXEC_APPEND, com_consoleLines[i] );
	}
}

/*
==================
idCommonLocal::WriteConfigToFile
==================
*/
void idCommonLocal::WriteConfigToFile( const char* filename )
{
	idFile* f = fileSystem->OpenFileWrite( filename );
	if( !f )
	{
		Printf( "Couldn't write %s.\n", filename );
		return;
	}
	
	idKeyInput::WriteBindings( f );
	cvarSystem->WriteFlaggedVariables( CVAR_ARCHIVE, "set", f );
	fileSystem->CloseFile( f );
}

/*
===============
idCommonLocal::WriteConfiguration

Writes key bindings and archived cvars to config file if modified
===============
*/
void idCommonLocal::WriteConfiguration()
{
	// if we are quiting without fully initializing, make sure
	// we don't write out anything
	if( !com_fullyInitialized )
	{
		return;
	}
	
	if( !( cvarSystem->GetModifiedFlags() & CVAR_ARCHIVE ) )
	{
		return;
	}
	cvarSystem->ClearModifiedFlags( CVAR_ARCHIVE );
	
	// save to the profile
	idLocalUser* user = session->GetSignInManager().GetMasterLocalUser();
	if( user != NULL )
	{
		user->SaveProfileSettings();
	}
	
#ifdef CONFIG_FILE
	// disable printing out the "Writing to:" message
	bool developer = com_developer.GetBool();
	com_developer.SetBool( false );
	
	WriteConfigToFile( CONFIG_FILE );
	
	// restore the developer cvar
	com_developer.SetBool( developer );
#endif
}

/*
===============
KeysFromBinding()
Returns the key bound to the command
===============
*/
const char* idCommonLocal::KeysFromBinding( const char* bind )
{
	return idKeyInput::KeysFromBinding( bind );
}

/*
===============
BindingFromKey()
Returns the binding bound to key
===============
*/
const char* idCommonLocal::BindingFromKey( const char* key )
{
	return idKeyInput::BindingFromKey( key );
}

/*
===============
ButtonState()
Returns the state of the button
===============
*/
int	idCommonLocal::ButtonState( int key )
{
	return usercmdGen->ButtonState( key );
}

/*
===============
ButtonState()
Returns the state of the key
===============
*/
int	idCommonLocal::KeyState( int key )
{
	return usercmdGen->KeyState( key );
}

/*
============
idCmdSystemLocal::PrintMemInfo_f

This prints out memory debugging data
============
*/
CONSOLE_COMMAND( printMemInfo, "prints memory debugging data", NULL )
{
	MemInfo_t mi;
	memset( &mi, 0, sizeof( mi ) );
	mi.filebase = commonLocal.GetCurrentMapName();
	
	renderSystem->PrintMemInfo( &mi );			// textures and models
	soundSystem->PrintMemInfo( &mi );			// sounds
	
	common->Printf( " Used image memory: %s bytes\n", idStr::FormatNumber( mi.imageAssetsTotal ).c_str() );
	mi.assetTotals += mi.imageAssetsTotal;
	
	common->Printf( " Used model memory: %s bytes\n", idStr::FormatNumber( mi.modelAssetsTotal ).c_str() );
	mi.assetTotals += mi.modelAssetsTotal;
	
	common->Printf( " Used sound memory: %s bytes\n", idStr::FormatNumber( mi.soundAssetsTotal ).c_str() );
	mi.assetTotals += mi.soundAssetsTotal;
	
	common->Printf( " Used asset memory: %s bytes\n", idStr::FormatNumber( mi.assetTotals ).c_str() );
	
	// write overview file
	idFile* f;
	
	f = fileSystem->OpenFileAppend( "maps/printmeminfo.txt" );
	if( !f )
	{
		return;
	}
	
	f->Printf( "total(%s ) image(%s ) model(%s ) sound(%s ): %s\n", idStr::FormatNumber( mi.assetTotals ).c_str(), idStr::FormatNumber( mi.imageAssetsTotal ).c_str(),
			   idStr::FormatNumber( mi.modelAssetsTotal ).c_str(), idStr::FormatNumber( mi.soundAssetsTotal ).c_str(), mi.filebase.c_str() );
			   
	fileSystem->CloseFile( f );
}

/*
==================
Com_Error_f

Just throw a fatal error to test error shutdown procedures.
==================
*/
CONSOLE_COMMAND( error, "causes an error", NULL )
{
	if( !com_developer.GetBool() )
	{
		commonLocal.Printf( "error may only be used in developer mode\n" );
		return;
	}
	
	if( args.Argc() > 1 )
	{
		commonLocal.FatalError( "Testing fatal error" );
	}
	else
	{
		commonLocal.Error( "Testing drop error" );
	}
}

/*
==================
Com_Freeze_f

Just freeze in place for a given number of seconds to test error recovery.
==================
*/
CONSOLE_COMMAND( freeze, "freezes the game for a number of seconds", NULL )
{
	float	s;
	int		start, now;
	
	if( args.Argc() != 2 )
	{
		commonLocal.Printf( "freeze <seconds>\n" );
		return;
	}
	
	if( !com_developer.GetBool() )
	{
		commonLocal.Printf( "freeze may only be used in developer mode\n" );
		return;
	}
	
	s = atof( args.Argv( 1 ) );
	
	start = eventLoop->Milliseconds();
	
	while( 1 )
	{
		now = eventLoop->Milliseconds();
		if( ( now - start ) * 0.001f > s )
		{
			break;
		}
	}
}

/*
=================
Com_Crash_f

A way to force a bus error for development reasons
=================
*/
CONSOLE_COMMAND( crash, "causes a crash", NULL )
{
	if( !com_developer.GetBool() )
	{
		commonLocal.Printf( "crash may only be used in developer mode\n" );
		return;
	}
#ifdef __GNUC__
	__builtin_trap();
#else
	* ( int* ) 0 = 0x12345678;
#endif
}

/*
=================
Com_Quit_f
=================
*/
CONSOLE_COMMAND_SHIP( quit, "quits the game", NULL )
{
	commonLocal.Quit();
}
CONSOLE_COMMAND_SHIP( exit, "exits the game", NULL )
{
	commonLocal.Quit();
}

/*
===============
Com_WriteConfig_f

Write the config file to a specific name
===============
*/
CONSOLE_COMMAND( writeConfig, "writes a config file", NULL )
{
	idStr	filename;
	
	if( args.Argc() != 2 )
	{
		commonLocal.Printf( "Usage: writeconfig <filename>\n" );
		return;
	}
	
	filename = args.Argv( 1 );
	filename.DefaultFileExtension( ".cfg" );
	commonLocal.Printf( "Writing %s.\n", filename.c_str() );
	commonLocal.WriteConfigToFile( filename );
}

/*
========================
idCommonLocal::CheckStartupStorageRequirements
========================
*/
void idCommonLocal::CheckStartupStorageRequirements()
{
	// RB: disabled savegame and profile storage checks, because it fails sometimes without any clear reason
#if 0
	int64 availableSpace = 0;
	
	// ------------------------------------------------------------------------
	// Savegame and Profile required storage
	// ------------------------------------------------------------------------
	{
		// Make sure the save path exists in case it was deleted.
		// If the path cannot be created we can safely assume there is no
		// free space because in that case nothing can be saved anyway.
		const char* savepath = cvarSystem->GetCVarString( "fs_savepath" );
		idStr directory = savepath;
		//idStr directory = fs_savepath.GetString();
		directory += "\\";	// so it doesn't think the last part is a file and ignores in the directory creation
		fileSystem->CreateOSPath( directory );
		
		// Get the free space on the save path.
		availableSpace = Sys_GetDriveFreeSpaceInBytes( savepath );
		
		// If free space fails then get space on drive as a fall back
		// (the directory will be created later anyway)
		if( availableSpace <= 1 )
		{
			idStr savePath( savepath );
			if( savePath.Length() >= 3 )
			{
				if( savePath[ 1 ] == ':' && savePath[ 2 ] == '\\' &&
						( ( savePath[ 0 ] >= 'A' && savePath[ 0 ] <= 'Z' ) ||
						  ( savePath[ 0 ] >= 'a' && savePath[ 0 ] <= 'z' ) ) )
				{
					savePath = savePath.Left( 3 );
					availableSpace = Sys_GetDriveFreeSpaceInBytes( savePath );
				}
			}
		}
	}
	
	const int MIN_SAVE_STORAGE_PROFILE		= 1024 * 1024;
	const int MIN_SAVE_STORAGE_SAVEGAME		= MIN_SAVEGAME_SIZE_BYTES;
	
	uint64 requiredSizeBytes = MIN_SAVE_STORAGE_SAVEGAME + MIN_SAVE_STORAGE_PROFILE;
	
	idLib::Printf( "requiredSizeBytes: %lld\n", requiredSizeBytes );
	
	if( ( int64 )( requiredSizeBytes - availableSpace ) > 0 )
	{
		class idSWFScriptFunction_Continue : public idSWFScriptFunction_RefCounted
		{
		public:
			virtual ~idSWFScriptFunction_Continue() {}
			idSWFScriptVar Call( idSWFScriptObject* thisObject, const idSWFParmList& parms )
			{
				common->Dialog().ClearDialog( GDM_INSUFFICENT_STORAGE_SPACE );
				common->Quit();
				return idSWFScriptVar();
			}
		};
		
		idStaticList< idSWFScriptFunction*, 4 > callbacks;
		idStaticList< idStrId, 4 > optionText;
		callbacks.Append( new( TAG_SWF ) idSWFScriptFunction_Continue() );
		optionText.Append( idStrId( "#STR_SWF_ACCEPT" ) );
		
		// build custom space required string
		// #str_dlg_space_required ~= "There is insufficient storage available.  Please free %s and try again."
		idStr format = idStrId( "#str_dlg_startup_insufficient_storage" ).GetLocalizedString();
		idStr size;
		if( requiredSizeBytes > ( 1024 * 1024 ) )
		{
			size = va( "%.1f MB", ( float )requiredSizeBytes / ( 1024.0f * 1024.0f ) + 0.1f );	// +0.1 to avoid truncation
		}
		else
		{
			size = va( "%.1f KB", ( float )requiredSizeBytes / 1024.0f + 0.1f );
		}
		idStr msg = va( format.c_str(), size.c_str() );
		
		common->Dialog().AddDynamicDialog( GDM_INSUFFICENT_STORAGE_SPACE, callbacks, optionText, true, msg );
	}
#endif
	// RB end
	
	session->GetAchievementSystem().Start();
}

/*
===============
idCommonLocal::JapaneseCensorship
===============
*/
bool idCommonLocal::JapaneseCensorship() const
{
	return com_japaneseCensorship.GetBool() || com_isJapaneseSKU;
}

/*
===============
idCommonLocal::FilterLangList
===============
*/
void idCommonLocal::FilterLangList( idStrList* list, idStr lang, bool strict )
{

	idStr temp;
	for( int i = 0; i < list->Num(); i++ )
	{
		temp = ( *list )[i];
		temp = temp.Right( temp.Length() - strlen( "strings/" ) );
		if (strict) {
			temp = temp.Left(temp.Length() - strlen(".lang"));
		}
		else {
			temp = temp.Left(lang.Length());
		}
		if( idStr::Icmp( temp, lang ) != 0 )
		{
			list->RemoveIndex( i );
			i--;
		}
	}
}

/*
===============
idCommonLocal::InitLanguageDict
===============
*/
extern idCVar sys_lang;
void idCommonLocal::InitLanguageDict()
{
	idStr fileName;
	
	//D3XP: Instead of just loading a single lang file for each language
	//we are going to load all files that begin with the language name
	//similar to the way pak files work. So you can place english001.lang
	//to add new strings to the english language dictionary
	idFileList*	langFiles;
	langFiles =  fileSystem->ListFilesTree( "strings", ".lang", true );
	
	idStrList langList = langFiles->GetList();
	
	// Loop through the list and filter
	idStrList currentLangList = langList;
	FilterLangList( &currentLangList, sys_lang.GetString(), true );
	
	int index = 0;
	while( currentLangList.Num() == 0 && index < Sys_NumLangs())
	{
		//GK: Loop through all available languages until it find the installed one
		sys_lang.SetString(Sys_Lang(index));
		currentLangList = langList;
		FilterLangList(&currentLangList, sys_lang.GetString(), true);
		index++;
	}
	if (currentLangList.Num() == 0) {
		common->FatalError("Failed to Load Language data");
	}
	currentLangList = langList;
	FilterLangList(&currentLangList, sys_lang.GetString());

	
	idLocalization::ClearDictionary();
	for( int i = 0; i < currentLangList.Num(); i++ )
	{
		//common->Printf("%s\n", currentLangList[i].c_str());
		const byte* buffer = NULL;
		int len = fileSystem->ReadFile( currentLangList[i], ( void** )&buffer );
		if( len <= 0 )
		{
			assert( false && "couldn't read the language dict file" );
			break;
		}
		idLocalization::LoadDictionary( buffer, len, currentLangList[i] );
		fileSystem->FreeFile( ( void* )buffer );
	}
	
	fileSystem->FreeFileList( langFiles );
}

void idCommonLocal::InitCreditList()
{
	idStr fileName;

	//D3XP: Instead of just loading a single lang file for each language
	//we are going to load all files that begin with the language name
	//similar to the way pak files work. So you can place english001.lang
	//to add new strings to the english language dictionary
	idFileList* langFiles;
	langFiles = fileSystem->ListFilesTree("credits", ".lang", true);

	idStrList langList = langFiles->GetList();

	// Loop through the list and filter
	idStrList currentLangList = langList;
	FilterLangList(&currentLangList, sys_lang.GetString());

	if (currentLangList.Num() == 0)
	{
		// reset to english and try to load again
		currentLangList = langList;
		FilterLangList(&currentLangList, ID_LANG_ENGLISH);
	}

	idCredits::ClearList();
	for (int i = currentLangList.Num() - 1; i >= 0; i--)
	{
		//common->Printf("%s\n", currentLangList[i].c_str());
		const byte* buffer = NULL;
		int len = fileSystem->ReadFile(currentLangList[i], (void**)&buffer);
		if (len <= 0)
		{
			assert(false && "couldn't read the language dict file");
			break;
		}
		idCredits::LoadCredits(buffer, len, currentLangList[i]);
		fileSystem->FreeFile((void*)buffer);
	}

	fileSystem->FreeFileList(langFiles);
}

/*
=================
ReloadLanguage_f
=================
*/
CONSOLE_COMMAND( reloadLanguage, "reload language dict", NULL )
{
	commonLocal.InitLanguageDict();
}

#include "../renderer/Image.h"

/*
=================
Com_FinishBuild_f
=================
*/
CONSOLE_COMMAND( finishBuild, "finishes the build process", NULL )
{
	if( game )
	{
		game->CacheDictionaryMedia( NULL );
	}
}

bool getInput() {
	int	mouseEvents[MAX_MOUSE_EVENTS][2];
	Sys_GenerateEvents();

	// queue system events ready for polling
	Sys_GetEvent();

	// RB: allow to escape video by pressing anything
	int numKeyEvents = Sys_PollKeyboardInputEvents();
	if (numKeyEvents > 0)
	{
		idLib::joystick = false;
		for (int i = 0; i < numKeyEvents; i++)
		{
			int key;
			bool state;

			if (Sys_ReturnKeyboardInputEvent(i, key, state))
			{
				if (key == K_ESCAPE && state == true)
				{
					return true;
				}
				break;
			}
		}

		Sys_EndKeyboardInputEvents();
	}

	int numMouseEvents = Sys_PollMouseInputEvents(mouseEvents);
	if (numMouseEvents > 0)
	{
		idLib::joystick = false;
		for (int i = 0; i < numMouseEvents; i++)
		{
			int action = mouseEvents[i][0];
			switch (action)
			{
			case M_ACTION1:
			case M_ACTION2:
			case M_ACTION3:
			case M_ACTION4:
			case M_ACTION5:
			case M_ACTION6:
			case M_ACTION7:
			case M_ACTION8:
				return true;
				break;

			default:	// some other undefined button
				break;
			}
		}
	}

	int numJoystickEvents = 0;
	for (int i = 0; i < MAX_INPUT_DEVICES; i++) {
		numJoystickEvents = Sys_PollJoystickInputEvents(i);
		if (numJoystickEvents > 0) {
			break;
		}
	}
	if (numJoystickEvents > 0)
	{
		int validevents = 0;
		for (int i = 0; i < numJoystickEvents; i++)
		{
			int action;
			int value;

			if (Sys_ReturnJoystickInputEvent(i, action, value))
			{
				if (action >= J_ACTION1 && action <= J_ACTION_MAX)
				{
					if (value != 0)
					{
						validevents++;
						return true;
						break;
					}
				}
			}
		}
		if (validevents > 0) {
			idLib::joystick = true;
		}
		Sys_EndJoystickInputEvents();
	}
	return false;
}

/*
=================
idCommonLocal::RenderSplash
=================
*/
void idCommonLocal::RenderSplash()
{
	//const emptyCommand_t* renderCommands = NULL;
	
	// RB: this is the same as Doom 3 renderSystem->BeginFrame()
	//renderCommands = renderSystem->SwapCommandBuffers_FinishCommandBuffers();
	
	const int renderWidth = renderSystem->GetWidth() > 0 ? renderSystem->GetWidth() : 1280;
	const float sysWidth = renderWidth * renderSystem->GetPixelAspect();
	const float sysHeight = renderSystem->GetHeight() > 0 ? renderSystem->GetHeight() : 720;
	const float sysAspect = sysWidth / sysHeight;
	const float splashAspect = 16.0f / 9.0f;
	const float adjustment = sysAspect / splashAspect;
	const float barHeight = ( adjustment >= 1.0f ) ? 0.0f : ( 1.0f - adjustment ) * ( float )renderSystem->GetVirtualHeight() * 0.25f;
	const float barWidth = ( adjustment <= 1.0f ) ? 0.0f : ( adjustment - 1.0f ) * ( float )renderSystem->GetVirtualWidth() * 0.25f;
	if( barHeight > 0.0f )
	{
		renderSystem->SetColor( colorBlack );
		renderSystem->DrawStretchPic( 0, 0, renderSystem->GetVirtualWidth(), barHeight, 0, 0, 1, 1, whiteMaterial );
		renderSystem->DrawStretchPic( 0, renderSystem->GetVirtualHeight() - barHeight, renderSystem->GetVirtualWidth(), barHeight, 0, 0, 1, 1, whiteMaterial );
	}
	if( barWidth > 0.0f )
	{
		renderSystem->SetColor( colorBlack );
		renderSystem->DrawStretchPic( 0, 0, barWidth, renderSystem->GetVirtualHeight(), 0, 0, 1, 1, whiteMaterial );
		renderSystem->DrawStretchPic( renderSystem->GetVirtualWidth() - barWidth, 0, barWidth, renderSystem->GetVirtualHeight(), 0, 0, 1, 1, whiteMaterial );
	}
	renderSystem->SetColor4( 1, 1, 1, 1 );
	renderSystem->DrawStretchPic( barWidth, barHeight, renderSystem->GetVirtualWidth() - barWidth * 2.0f, renderSystem->GetVirtualHeight() - barHeight * 2.0f, 0, 0, 1, 1, splashScreen );
	
	const emptyCommand_t* cmd = renderSystem->SwapCommandBuffers( &time_frontend, &time_backend, &time_shadows, &time_gpu );
	renderSystem->RenderCommandBuffers( cmd );
	
	// RB: this is the same as Doom 3 renderSystem->EndFrame()
	//renderSystem->SwapCommandBuffers_FinishRendering( &time_frontend, &time_backend, &time_shadows, &time_gpu );
}

/*
=================
idCommonLocal::RenderPhotosensitivity
=================
*/
void idCommonLocal::RenderPhotosensitivity()
{
	const int renderWidth = renderSystem->GetWidth() > 0 ? renderSystem->GetWidth() : 1280;
	const float sysWidth = renderWidth * renderSystem->GetPixelAspect();
	const float sysHeight = renderSystem->GetHeight() > 0 ? renderSystem->GetHeight() : 720;
	const float sysAspect = sysWidth / sysHeight;
	const float splashAspect = 16.0f / 9.0f;
	const float adjustment = sysAspect / splashAspect;
	const float barHeight = (adjustment >= 1.0f) ? 0.0f : (1.0f - adjustment) * (float)renderSystem->GetVirtualHeight() * 0.25f;
	const float barWidth = (adjustment <= 1.0f) ? 0.0f : (adjustment - 1.0f) * (float)renderSystem->GetVirtualWidth() * 0.25f;
	if (barHeight > 0.0f)
	{
		renderSystem->SetColor(colorBlack);
		renderSystem->DrawStretchPic(0, 0, renderSystem->GetVirtualWidth(), barHeight, 0, 0, 1, 1, whiteMaterial);
		renderSystem->DrawStretchPic(0, renderSystem->GetVirtualHeight() - barHeight, renderSystem->GetVirtualWidth(), barHeight, 0, 0, 1, 1, whiteMaterial);
	}
	if (barWidth > 0.0f)
	{
		renderSystem->SetColor(colorBlack);
		renderSystem->DrawStretchPic(0, 0, barWidth, renderSystem->GetVirtualHeight(), 0, 0, 1, 1, whiteMaterial);
		renderSystem->DrawStretchPic(renderSystem->GetVirtualWidth() - barWidth, 0, barWidth, renderSystem->GetVirtualHeight(), 0, 0, 1, 1, whiteMaterial);
	}
	renderSystem->SetColor4(1, 1, 1, 1);
	renderSystem->DrawStretchPic(barWidth, barHeight, renderSystem->GetVirtualWidth() - barWidth * 2.0f, renderSystem->GetVirtualHeight() - barHeight * 2.0f, 0, 0, 1, 1, photsensitivityscreen);

	const emptyCommand_t* cmd = renderSystem->SwapCommandBuffers(&time_frontend, &time_backend, &time_shadows, &time_gpu);
	renderSystem->RenderCommandBuffers(cmd);
}

/*
=================
idCommonLocal::RenderBink
=================
*/
void idCommonLocal::RenderBink( const char* path )
{
	const int renderWidth = renderSystem->GetWidth() > 0 ? renderSystem->GetWidth() : 1280;
	const float sysWidth = renderWidth * renderSystem->GetPixelAspect();
	const float sysHeight = renderSystem->GetHeight() > 0 ? renderSystem->GetHeight() : 720;
	const float sysAspect = sysWidth / sysHeight;
	const float movieAspect = ( 16.0f / 9.0f );
	const float imageWidth = renderSystem->GetVirtualWidth() * movieAspect / sysAspect;
	const float chop = 0.5f * ( renderSystem->GetVirtualWidth() - imageWidth );
	
	idStr materialText;
	materialText.Format( "{ translucent { videoMap %s } }", path );
	
	idMaterial* material = const_cast<idMaterial*>( declManager->FindMaterial( "splashbink" ) );
	material->Parse( materialText.c_str(), materialText.Length(), false );
	
	// RB: FFmpeg might return the wrong play length so I changed the intro video to play max 30 seconds until finished
	//GK: No it doesn't
	int cinematicLength = material->CinematicLength();
	
	bool escapeEvent = false;
	//GK: expecting all the code in the loop to consume no time (especially the sleep) is wrong and it was causing the timer to be incorect on every loop, which resultedon the cinematic to cut before is finished
	int elapsedTime = 0;
	//GK: Set it here in order to be more accurate
	material->ResetCinematicTime(Sys_Milliseconds());
	while( ( (Sys_Milliseconds() - elapsedTime) <= ( material->GetCinematicStartTime() + cinematicLength ) ) && material->CinematicIsPlaying() )
	{
		int starttime = Sys_Milliseconds();
		renderSystem->DrawStretchPic( chop, 0, imageWidth, renderSystem->GetVirtualHeight(), 0, 0, 1, 1, material );
		const emptyCommand_t* cmd = renderSystem->SwapCommandBuffers( &time_frontend, &time_backend, &time_shadows, &time_gpu );
		renderSystem->RenderCommandBuffers( cmd );
		
		escapeEvent = getInput();
		
		if( escapeEvent || com_emergencyexit.GetBool())
		{
			break;
		}
		
		Sys_Sleep( 10 );
		int endtime = Sys_Milliseconds();
		elapsedTime += endtime - starttime;
	}
	// RB end
	
	material->MakeDefault();
}

/*
=================
idCommonLocal::InitSIMD
=================
*/
void idCommonLocal::InitSIMD()
{
	idSIMD::InitProcessor( "doom", com_forceGenericSIMD.GetBool() );
	com_forceGenericSIMD.ClearModified();
}

void LoadPlatformDLL()
{
	char			dllPath[MAX_OSPATH];
	GetPlatformAPI_t GetPlatformAPI;
	fileSystem->FindDLL("OpenPlatform", dllPath);

	if (dllPath[0])
	{
		common->DPrintf("Loading Platform DLL: '%s'\n", dllPath);
		platformDLL = sys->DLL_Load(dllPath);
		if (platformDLL) //GK: If the library fail don't let it to take the game with it
		{


			const char* functionName = "GetPlatformAPI";
			GetPlatformAPI = (GetPlatformAPI_t)Sys_DLL_GetProcAddress(platformDLL, functionName);
			if (!GetPlatformAPI)
			{
				Sys_DLL_Unload(platformDLL);
				platformDLL = -1;
				//common->FatalError("couldn't find platform DLL API");
				::op = NULL;
#ifdef WIN32
				//GK: Just some 64-bit paranoia
				int lastError = GetLastError();
				char msgbuf[256];
				FormatMessage(
					FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					lastError,
					MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), // Default language
					(LPTSTR)&msgbuf,
					sizeof(msgbuf),
					NULL
				);

				Sys_Error("Sys_DLL_: GetProcAddress failed - %s (%d)", msgbuf, lastError);
#endif
				return;
			}

			::op = GetPlatformAPI();



#ifdef _UWP
			int res = ::op->API_Init();
			while (res == -1) {
				res = ::op->API_Init();
			}
			if (res == 1) {
				common->Printf("Platform loaded sucessfully !!!\n");
			}
#else
			if (::op->API_Init()) {
				::op->SetAdditionalInfo("large image", "dbfa");
				::op->SetAdditionalInfo("small image", "dbfa");
				::op->SetAdditionalInfo("status", "Strting Game");
				if (!idStr::Icmp("Steam", ::op->API_Name())) {
					::op->openInput()->RegisterInputConfigurationFile(fileSystem->RelativePathToOSPath("bfa_actions.vdf"));
				}
				//::op->SetNotificationsPosition(0, 0); //GK: Who knows maybe someone want it on top left
				common->Printf("%s Platform loaded sucessfully !!!\n", ::op->API_Name());
			}
#endif
		}
	}
	
}

void UnloadPlatformDLL()
{
	// shut down the platform object
	if (::op != NULL)
	{
		::op->API_Shutdown();		
	}

	if (platformDLL)
	{
		Sys_DLL_Unload(platformDLL);
		platformDLL = -1;
	}
	::op = NULL;

}


/*
=================
idCommonLocal::LoadGameDLL
=================
*/
void idCommonLocal::LoadGameDLL()
{
#ifdef __DOOM_DLL__
	char			dllPath[ MAX_OSPATH ];
	
	gameImport_t	gameImport;
	gameExport_t	gameExport;
	GetGameAPI_t	GetGameAPI;
	
	fileSystem->FindDLL( "game", dllPath);
	
	if (dllPath[0])
	{
		//GK: If no dll be ok and load monolithic game
		//common->FatalError( "couldn't find game dynamic library" );
		//return;

		common->DPrintf("Loading game DLL: '%s'\n", dllPath);
		gameDLL = sys->DLL_Load(dllPath);
		if (!gameDLL)
		{
			common->FatalError("couldn't load game dynamic library");
			return;
		}

		const char* functionName = "GetGameAPI";
		GetGameAPI = (GetGameAPI_t)Sys_DLL_GetProcAddress(gameDLL, functionName);
		if (!GetGameAPI)
		{
			Sys_DLL_Unload(gameDLL);
			gameDLL = -1;
			common->FatalError("couldn't find game DLL API");
#ifdef WIN32
			//GK: Just some 64-bit paranoia
			int lastError = GetLastError();
			char msgbuf[256];
			FormatMessage(
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				lastError,
				MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), // Default language
				(LPTSTR)&msgbuf,
				sizeof(msgbuf),
				NULL
			);

			Sys_Error("Sys_DLL_: GetProcAddress failed - %s (%d)", msgbuf, lastError);
#endif
			
			return;
		}

		gameImport.version = GAME_API_VERSION;
		gameImport.sys = ::sys;
		gameImport.common = ::common;
		gameImport.cmdSystem = ::cmdSystem;
		gameImport.cvarSystem = ::cvarSystem;
		gameImport.fileSystem = ::fileSystem;
		gameImport.renderSystem = ::renderSystem;
		gameImport.soundSystem = ::soundSystem;
		gameImport.renderModelManager = ::renderModelManager;
		gameImport.uiManager = ::uiManager;
		gameImport.declManager = ::declManager;
		gameImport.AASFileManager = ::AASFileManager;
		gameImport.collisionModelManager = ::collisionModelManager;
		gameImport.keys = ::keys;
		gameImport.session = ::session;
		gameImport.GetClassicData = &GetClassicDoomData;
		gameExport = *GetGameAPI(&gameImport);

		if (gameExport.version != GAME_API_VERSION)
		{
			Sys_DLL_Unload(gameDLL);
			gameDLL = -1;
			common->FatalError("wrong game DLL API version");
			return;
		}
		game = gameExport.game;
		gameEdit = gameExport.gameEdit;
	}
#ifdef __MONOLITH__
	else {
		game = CreateGameInstance();
		gameEdit = new idGameEdit();
	}
#endif
#endif
}

/*
=================
idCommonLocal::UnloadGameDLL
=================
*/
void idCommonLocal::CleanupShell()
{
	if( game != NULL )
	{
		game->Shell_Cleanup();
	}
}

/*
=================
idCommonLocal::UnloadGameDLL
=================
*/
void idCommonLocal::UnloadGameDLL()
{

	// shut down the game object
	if( game != NULL )
	{
		game->Shutdown();
		delete(game);
	}
	if (gameEdit != NULL) {
		delete(gameEdit);
	}
	
#ifdef __DOOM_DLL__
	
	if( gameDLL )
	{
		Sys_DLL_Unload( gameDLL );
		gameDLL = -1;
	}
	game = NULL;
	gameEdit = NULL;
	
#endif
}

/*
=================
idCommonLocal::IsInitialized
=================
*/
bool idCommonLocal::IsInitialized() const
{
	return com_fullyInitialized;
}


//======================================================================================


/*
=================
idCommonLocal::Init
=================
*/
void idCommonLocal::Init( int argc, const char* const* argv, const char* cmdline )
{
	try
	{
		// set interface pointers used by idLib
		idLib::sys			= sys;
		idLib::common		= common;
		idLib::cvarSystem	= cvarSystem;
		idLib::fileSystem	= fileSystem;
		
		// initialize idLib
		idLib::Init();
		
		// clear warning buffer
		ClearWarnings( ENGINE_NAME " initialization" );
		
		idLib::Printf( "Command line: %s\n", cmdline );
		//::MessageBox( NULL, cmdline, "blah", MB_OK );
		// parse command line options
		idCmdArgs args;
		if( cmdline )
		{
			// tokenize if the OS doesn't do it for us
			args.TokenizeString( cmdline, true );
			argv = args.GetArgs( &argc );
		}
		ParseCommandLine( argc, argv );
		
		// init console command system
		cmdSystem->Init();
		
		// init CVar system
		cvarSystem->Init();
		
		// register all static CVars
		idCVar::RegisterStaticVars();
		
		idLib::Printf( "QA Timing INIT: %06dms\n", Sys_Milliseconds() );
		
		// print engine version
		Printf( "%s\n", version.string );
		
		// initialize key input/binding, done early so bind command exists
		idKeyInput::Init();
		
		// init the console so we can take prints
		console->Init();
		
		// get architecture info
		Sys_Init();
		
		// initialize networking
		Sys_InitNetworking();
		
		// override cvars from command line
		StartupVariable( NULL );
		
		consoleUsed = com_allowConsole.GetBool();
		
		if( Sys_AlreadyRunning() )
		{
			Sys_Quit();
		}
		
		// initialize processor specific SIMD implementation
		InitSIMD();
		
		// initialize the file system
		fileSystem->Init();
		
		const char* defaultLang = Sys_DefaultLanguage();
		com_isJapaneseSKU = ( idStr::Icmp( defaultLang, ID_LANG_JAPANESE ) == 0 );
		
		// Allow the system to set a default lanugage
		Sys_SetLanguageFromSystem();
		
		// Pre-allocate our 20 MB save buffer here on time, instead of on-demand for each save....
		
		saveFile.SetNameAndType( SAVEGAME_CHECKPOINT_FILENAME, SAVEGAMEFILE_BINARY );
		saveFile.PreAllocate( MIN_SAVEGAME_SIZE_BYTES );
		
		stringsFile.SetNameAndType( SAVEGAME_STRINGS_FILENAME, SAVEGAMEFILE_BINARY );
		stringsFile.PreAllocate( MAX_SAVEGAME_STRING_TABLE_SIZE );

		fileSystem->BeginLevelLoad("_startup", saveFile.GetDataPtr(), saveFile.GetAllocated());

		// initialize the declaration manager
		declManager->Init();
		
		// init journalling, etc
		eventLoop->Init();
		
		// init the parallel job manager
		parallelJobManager->Init();
		
		// exec the startup scripts
		cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "exec default.cfg\n" );
		cmdSystem->BufferCommandText(CMD_EXEC_APPEND, "exec joy_360_0.cfg\n");
		cmdSystem->BufferCommandText(CMD_EXEC_APPEND, "exec joy_righty.cfg\n");
//#ifndef __MONOLITH__ //GK: Non Monolithic doesn't have the game object until it loads the dll
		// load the game dll
		LoadGameDLL();	
//#endif
#ifdef CONFIG_FILE
		// skip the config file if "safe" is on the command line
		if( !SafeMode() && !game->GetCVarBool("g_demoMode") )
		{
			std::string configname = CONFIG_FILE;
			std::string command = "exec " + configname;
			//GK: Explicit load saved configurations from the current mod folder
			if (cvarSystem->GetCVarString("fs_game") != NULL && cvarSystem->GetCVarString("fs_game")[0] != 0) {
				command += " ";
				command += "\"";
				command += cvarSystem->GetCVarString("fs_game");
				command += "\"";
			}
			else if (cvarSystem->GetCVarString("fs_game_base") != NULL && cvarSystem->GetCVarString("fs_game_base")[0] != 0) {
				command += " ";
				command += "\"";
				command += cvarSystem->GetCVarString("fs_game_base");
				command += "\"";
			}
			command += "\n";
			cmdSystem->BufferCommandText( CMD_EXEC_APPEND, command.c_str() );
		}
#endif
		
		cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "exec autoexec.cfg\n" );
		
		// run cfg execution
		cmdSystem->ExecuteCommandBuffer();
		
		// re-override anything from the config files with command line args
		StartupVariable( NULL );
		
		// if any archived cvars are modified after this, we will trigger a writing of the config file
		cvarSystem->ClearModifiedFlags( CVAR_ARCHIVE );
		
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
		useSecondaryAudioAPI = s_useXAudio2.GetBool();
#endif

		// init OpenGL, which will open a window and connect sound and input hardware
		renderSystem->InitOpenGL();
		
		// Support up to 2 digits after the decimal point
		com_engineHz_denominator = 100LL * com_engineHz.GetFloat();
		com_engineHz_latched = com_engineHz.GetFloat();
		
		// start the sound system, but don't do any hardware operations yet
		soundSystem->Init();
		
		// initialize the renderSystem data structures
		renderSystem->Init();

		common->Printf("Initializing Platform\n");
		LoadPlatformDLL();
		if (::op == NULL) {
			common->Printf("Failed to initialize\n");
		}
		
		whiteMaterial = declManager->FindMaterial( "_white" );
		
		if( idStr::Icmp( sys_lang.GetString(), ID_LANG_FRENCH ) == 0 )
		{
			// If the user specified french, we show french no matter what SKU
			splashScreen = declManager->FindMaterial( "guis/assets/splash/legal_french" );
		}
		else if( idStr::Icmp( defaultLang, ID_LANG_FRENCH ) == 0 )
		{
			// If the lead sku is french (ie: europe), display figs
			splashScreen = declManager->FindMaterial( "guis/assets/splash/legal_figs" );
		}
		else
		{
			// Otherwise show it in english
			splashScreen = declManager->FindMaterial( "guis/assets/splash/legal_english" );
		}

		photsensitivityscreen = declManager->FindMaterial("guis/assets/splash/legal_photosensitivity");
		//GK: very dirty Hack in order to detect D3(2019)
		idImage* photoimage = photsensitivityscreen->GetStage(0)->texture.image;
		photoimage->ActuallyLoadImage(true);
		newd3 = photoimage->IsActuallyLoaded();
		sprintf(version.string, "%s.%d%s %s %s %s", common->IsNewDOOM3() ? NEW_CONSOLE_NAME : CONSOLE_NAME, BUILD_NUMBER, BUILD_DEBUG, BUILD_STRING, __DATE__, __TIME__);
		com_version.SetString(version.string);

		if (!(common->IsNewDOOM3() && com_game_mode.GetInteger() > 0)) {
			Sys_ChangeTitle(common->IsNewDOOM3() ? NEW_GAME_NAME : GAME_NAME);
		}

		const int legalMinTime = !common->IsNewDOOM3() ? 4000 : 8000;
		
		const bool showVideo = ( !com_skipIntroVideos.GetBool() && fileSystem->UsingResourceFiles() );
		if( showVideo )
		{
			RenderBink( "video\\loadvideo.bik" );
			RenderSplash();
			RenderSplash();
		}
		else
		{
			idLib::Printf( "Skipping Intro Videos!\n" );
			// display the legal splash screen
			// No clue why we have to render this twice to show up...
			RenderSplash();
			//RenderSplash();
		}
		if (com_emergencyexit.GetBool()) {
			return;
		}
		if (this->IsNewDOOM3()) {
			fileSystem->Restart();
		}
		
		int legalStartTime = sys->GetMilliseconds();
		declManager->Init2();
		
		// initialize string database so we can use it for loading messages
		InitLanguageDict();

		InitCreditList();
		
		// spawn the game thread, even if we are going to run without SMP
		// one meg stack, because it can parse decls from gui surfaces (unfortunately)
		// use a lower priority so job threads can run on the same core
		gameThread.StartWorkerThread( "Game/Draw", CORE_1B, THREAD_BELOW_NORMAL, 0x100000 );
		// boost this thread's priority, so it will prevent job threads from running while
		// the render back end still has work to do
		
		// init the user command input code
		usercmdGen->Init();
		
		Sys_SetRumble( 0, 0, 0 );
		
		// initialize the user interfaces
		uiManager->Init();
		
		// startup the script debugger
		// DebuggerServerInit();
		
		// Init tool commands
		InitCommands();

		// load the game dll
		//LoadGameDLL();

		// initialize the game object
		if (game != NULL)
		{
			game->Init();
		}
		
		// On the PC touch them all so they get included in the resource build
		if( !fileSystem->UsingResourceFiles() )
		{
			declManager->FindMaterial( "guis/assets/splash/legal_english" );
			declManager->FindMaterial( "guis/assets/splash/legal_french" );
			declManager->FindMaterial( "guis/assets/splash/legal_figs" );
			// register the japanese font so it gets included
			renderSystem->RegisterFont( "DFPHeiseiGothicW7" );
			// Make sure all videos get touched because you can bring videos from one map to another, they need to be included in all maps
			for( int i = 0; i < declManager->GetNumDecls( DECL_VIDEO ); i++ )
			{
				declManager->DeclByIndex( DECL_VIDEO, i );
			}
		}
		
		fileSystem->UnloadResourceContainer( "_ordered" );
		
		// the same idRenderWorld will be used for all games
		// and demos, insuring that level specific models
		// will be freed
		renderWorld = renderSystem->AllocRenderWorld();
		soundWorld = soundSystem->AllocSoundWorld( renderWorld );
		
		menuSoundWorld = soundSystem->AllocSoundWorld( NULL );
		menuSoundWorld->PlaceListener( vec3_origin, mat3_identity, 0,"Undefined" );
		
		// init the session
		session->Initialize();
		session->InitializeSoundRelatedSystems();
		
		InitializeMPMapsModes();
		
		// leaderboards need to be initialized after InitializeMPMapsModes, which populates the MP Map list.
		if( game != NULL )
		{
			game->Leaderboards_Init();
		}
		
		CreateMainMenu();
		
		commonDialog.Init();
		
		// load the console history file
		consoleHistory.LoadHistoryFile();
		
		AddStartupCommands();
		
		StartMenu( true );
		
		bool escapeEvent = false;
		bool finalEscape = !common->IsNewDOOM3();

		while( Sys_Milliseconds() - legalStartTime < legalMinTime )
		{
			escapeEvent = getInput();
			if (escapeEvent && !finalEscape) {
				escapeEvent = false;
				finalEscape = true;
				legalStartTime = legalMinTime / 2.0;
			}
			if ((finalEscape && escapeEvent) || com_emergencyexit.GetBool()) {
				break;
			}
			if ((Sys_Milliseconds() - legalStartTime) >= legalMinTime / 2.0 && common->IsNewDOOM3()) {
				RenderPhotosensitivity();
			}
			else {
				RenderSplash();
			}
			
			Sys_Sleep(10);
		}
		//Sys_ClearEvents();
		if (com_emergencyexit.GetBool()) {
			return;
		}
		// print all warnings queued during initialization
		PrintWarnings();
		
		// remove any prints from the notify lines
		console->ClearNotifyLines();
		
		CheckStartupStorageRequirements();
		
		
		if( preload_CommonAssets.GetBool() && fileSystem->UsingResourceFiles() )
		{
			idPreloadManifest manifest;
			manifest.LoadManifest( "_common.preload" );
			globalImages->Preload( manifest, false );
			soundSystem->Preload( manifest );
		}
		
		fileSystem->EndLevelLoad();
		
		// RB begin
#if defined(USE_DOOMCLASSIC)
		// Initialize support for Doom classic.
		W_CheckExp();
		doomClassicMaterial = declManager->FindMaterial( "_doomClassic" );
		idImage* image = globalImages->GetImage( "_doomClassic" );
		if( image != NULL )
		{
			idImageOpts opts;
			opts.format = FMT_RGBA8;
			opts.colorFormat = CFM_DEFAULT;
			//GK: The classic Doom's aspect ratio is relying on this resolution too
			//otherwise be ready for some really weird stuff
			if (!r_aspectcorrect.GetBool()) {
				opts.width = DOOMCLASSIC_RENDERWIDTH;
			}
			else {
				opts.width = DOOMCLASSIC_RENDERWIDTH_CORRECT;
			}
			//GK: End
			opts.height = DOOMCLASSIC_RENDERHEIGHT;
			opts.numLevels = 1;
			image->AllocImage( opts, TF_LINEAR, TR_REPEAT );
		}
#endif
		// RB end
		
		com_fullyInitialized = true;
		
		if (common->IsNewDOOM3() && (com_game_mode.GetInteger() <= 0 || com_game_mode.GetInteger() > 3)) {
			com_game_mode.SetInteger(3);
		}
		
		// No longer need the splash screen
		if( splashScreen != NULL )
		{
			for( int i = 0; i < splashScreen->GetNumStages(); i++ )
			{
				idImage* image1 = splashScreen->GetStage( i )->texture.image;
				if( image1 != NULL )
				{
					image1->PurgeImage();
				}
			}
		}
		if (photsensitivityscreen != NULL)
		{
			for (int i = 0; i < photsensitivityscreen->GetNumStages(); i++)
			{
				idImage* image2 = photsensitivityscreen->GetStage(i)->texture.image;
				if (image2 != NULL)
				{
					image2->PurgeImage();
				}
			}
		}
		/*idPlayerProfile* profile = session->GetProfileFromMasterLocalUser();
		profile->SetLeftyFlip(profile->GetLeftyFlip());
		cvarSystem->SetModifiedFlags(CVAR_ARCHIVE);*/
		in_joyjpn.SetBool(JapaneseCensorship());
		if (com_game_mode.GetInteger() == 1) {
			SwitchToGame(DOOM_CLASSIC);
		}
		else if (com_game_mode.GetInteger() == 2) {
			SwitchToGame(DOOM2_CLASSIC);
		}
		else if (com_game_mode.GetInteger() == 3) {
			session->GetSignInManager().RegisterLocalUser(0);
		}
		Printf( "--- Common Initialization Complete ---\n" );
		
		idLib::Printf( "QA Timing IIS: %06dms\n", Sys_Milliseconds() );
	}
	catch( idException& )
	{
		Sys_Error( "Error during initialization" );
	}
}

void minPrint(const char* msg) {
#if defined(_WIN32)
	OutputDebugString(msg);
#else
	printf("%s", msg);
#endif
}

/*
=================
idCommonLocal::Shutdown
=================
*/
void idCommonLocal::Shutdown()
{

	if( com_shuttingDown )
	{
		return;
	}
	com_shuttingDown = true;
	
	
	// Kill any pending saves...
	minPrint( "session->GetSaveGameManager().CancelToTerminate();\n" );
	session->GetSaveGameManager().CancelToTerminate();
	
	// kill sound first
	minPrint( "soundSystem->StopAllSounds();\n" );
	soundSystem->StopAllSounds();
	
	// shutdown the script debugger
	// DebuggerServerShutdown();
	
	if( aviCaptureMode )
	{
		minPrint( "EndAVICapture();\n" );
		EndAVICapture();
	}
	
	minPrint( "Stop();\n" );
	Stop();
	
	minPrint( "CleanupShell();\n" );
	CleanupShell();
	
	minPrint( "delete loadGUI;\n" );
	delete loadGUI;
	loadGUI = NULL;
	
	minPrint( "delete renderWorld;\n" );
	delete renderWorld;
	renderWorld = NULL;
	
	minPrint( "delete soundWorld;\n" );
	delete soundWorld;
	soundWorld = NULL;
	
	minPrint( "delete menuSoundWorld;\n" );
	delete menuSoundWorld;
	menuSoundWorld = NULL;
	
	// shut down the session
	minPrint( "session->ShutdownSoundRelatedSystems();\n" );
	session->ShutdownSoundRelatedSystems();
	minPrint( "session->Shutdown();\n" );
	session->Shutdown();
	
	// shutdown, deallocate leaderboard definitions.
	if( game != NULL )
	{
		minPrint( "game->Leaderboards_Shutdown();\n" );
		game->Leaderboards_Shutdown();
	}
	
	// shut down the user interfaces
	minPrint( "uiManager->Shutdown();\n" );
	uiManager->Shutdown();
	
	// shut down the sound system
	minPrint( "soundSystem->Shutdown();\n" );
	soundSystem->Shutdown();
	
	// shut down the user command input code
	minPrint( "usercmdGen->Shutdown();\n" );
	usercmdGen->Shutdown();
	
	// shut down the event loop
	minPrint( "eventLoop->Shutdown();\n" );
	eventLoop->Shutdown();
	
	// shutdown the decl manager
	minPrint( "declManager->Shutdown();\n" );
	declManager->Shutdown();
	
	// shut down the renderSystem
	minPrint( "renderSystem->Shutdown();\n" );
	renderSystem->Shutdown();
	
	minPrint( "commonDialog.Shutdown();\n" );
	commonDialog.Shutdown();
	
	// unload the game dll
	minPrint( "UnloadGameDLL();\n" );
	UnloadGameDLL();
	
	minPrint( "saveFile.Clear( true );\n" );
	saveFile.Clear( true );
	minPrint( "stringsFile.Clear( true );\n" );
	stringsFile.Clear( true );
	
	// only shut down the log file after all output is done
	minPrint( "CloseLogFile();\n" );
	CloseLogFile();

	if (::op) {
		UnloadPlatformDLL();
	}
	
	// shut down the file system
	minPrint( "fileSystem->Shutdown( false );\n" );
	fileSystem->Shutdown( false );
	
	// shut down non-portable system services
	minPrint( "Sys_Shutdown();\n" );
	Sys_Shutdown();
	
	// shut down the console
	minPrint( "console->Shutdown();\n" );
	console->Shutdown();
	
	// shut down the key system
	minPrint( "idKeyInput::Shutdown();\n" );
	idKeyInput::Shutdown();
	
	// shut down the cvar system
	minPrint( "cvarSystem->Shutdown();\n" );
	cvarSystem->Shutdown();
	
	// shut down the console command system
	minPrint( "cmdSystem->Shutdown();\n" );
	cmdSystem->Shutdown();
	
	// free any buffered warning messages
	char* msg = new char[70];
	sprintf(msg, "ClearWarnings( %s \" shutdown\" );\n", common->IsNewDOOM3() ? NEW_GAME_NAME : GAME_NAME);
	minPrint( msg );
	ClearWarnings(common->IsNewDOOM3() ? NEW_GAME_NAME : GAME_NAME " shutdown" );
	minPrint( "warningCaption.Clear();\n" );
	warningCaption.Clear();
	minPrint( "errorList.Clear();\n" );
	errorList.Clear();
	
	// shutdown idLib
	minPrint( "idLib::ShutDown();\n" );
	idLib::ShutDown();
}

/*
========================
idCommonLocal::CreateMainMenu
========================
*/
void idCommonLocal::CreateMainMenu()
{
	if( game != NULL )
	{
		// note which media we are going to need to load
		declManager->BeginLevelLoad();
		renderSystem->BeginLevelLoad();
		soundSystem->BeginLevelLoad();
		uiManager->BeginLevelLoad();
		
		// create main inside an "empty" game level load - so assets get
		// purged automagically when we transition to a "real" map
		game->Shell_CreateMenu( false );
		game->Shell_Show( true );
		game->Shell_SyncWithSession();
		
		// load
		renderSystem->EndLevelLoad();
		soundSystem->EndLevelLoad("");
		declManager->EndLevelLoad();
		uiManager->EndLevelLoad( "" );
	}
}

/*
===============
idCommonLocal::Stop

called on errors and game exits
===============
*/
void idCommonLocal::Stop( bool resetSession )
{
	ClearWipe();
	
	// clear mapSpawned and demo playing flags
	UnloadMap();
	
	soundSystem->StopAllSounds();
	
	insideUpdateScreen = false;
	insideExecuteMapChange = false;
	
	// drop all guis
	ExitMenu();
	
	if( resetSession )
	{
		session->QuitMatchToTitle();
	}
}

/*
===============
idCommonLocal::BusyWait
===============
*/
void idCommonLocal::BusyWait()
{
	Sys_GenerateEvents();
	
	const bool captureToImage = false;
	UpdateScreen( captureToImage );
	
	session->UpdateSignInManager();
	session->Pump();
}


/*
===============
idCommonLocal::InitCommands
===============
*/
void idCommonLocal::InitCommands()
{
	// compilers
	cmdSystem->AddCommand( "dmap", Dmap_f, CMD_FL_TOOL, "compiles a map", idCmdSystem::ArgCompletion_MapName );
	cmdSystem->AddCommand( "runAAS", RunAAS_f, CMD_FL_TOOL, "compiles an AAS file for a map", idCmdSystem::ArgCompletion_MapName );
	cmdSystem->AddCommand( "runAASDir", RunAASDir_f, CMD_FL_TOOL, "compiles AAS files for all maps in a folder", idCmdSystem::ArgCompletion_MapName );
	cmdSystem->AddCommand( "runReach", RunReach_f, CMD_FL_TOOL, "calculates reachability for an AAS file", idCmdSystem::ArgCompletion_MapName );
}

/*
===============
idCommonLocal::WaitForSessionState
===============
*/
bool idCommonLocal::WaitForSessionState( idSession::sessionState_t desiredState )
{
	if( session->GetState() == desiredState )
	{
		return true;
	}
	
	while( true )
	{
		BusyWait();
		
		idSession::sessionState_t sessionState = session->GetState();
		if( sessionState == desiredState )
		{
			return true;
		}
		if( sessionState != idSession::LOADING &&
				sessionState != idSession::SEARCHING &&
				sessionState != idSession::CONNECTING &&
				sessionState != idSession::BUSY &&
				sessionState != desiredState )
		{
			return false;
		}
		
		Sys_Sleep( 10 );
	}
}

/*
========================
idCommonLocal::LeaveGame
========================
*/
void idCommonLocal::LeaveGame()
{

	const bool captureToImage = false;
	UpdateScreen( captureToImage );
	
	ResetNetworkingState();
	
	
	Stop( false );
	
	CreateMainMenu();
	
	StartMenu();
	
	
}

/*
===============
idCommonLocal::ProcessEvent
===============
*/
bool idCommonLocal::ProcessEvent( const sysEvent_t* event )
{
	// hitting escape anywhere brings up the menu
	if( game && game->IsInGame() )
	{
		if( event->evType == SE_KEY && event->evValue2 == 1 && ( event->evValue == K_ESCAPE || event->evValue == K_JOY9 ) )
		{
			if( game->CheckInCinematic() == true )
			{
				game->SkipCinematicScene();
			}
			else
			{
				if( !game->Shell_IsActive() )
				{
				
					// menus / etc
					if( MenuEvent( event ) )
					{
						return true;
					}
					
					console->Close();
					
					StartMenu();
					return true;
				}
				else
				{
					console->Close();
					
					// menus / etc
					if( MenuEvent( event ) )
					{
						return true;
					}
					
					game->Shell_ClosePause();
				}
			}
		}
	}
	
	// let the pull-down console take it if desired
	if( console->ProcessEvent( event, false ) )
	{
		return true;
	}
	if( session->ProcessInputEvent( event ) )
	{
		return true;
	}
	
	if( Dialog().IsDialogActive() )
	{
		Dialog().HandleDialogEvent( event );
		return true;
	}
	
	// RB begin
#if defined(USE_DOOMCLASSIC)
	
	// Let Doom classic run events.
	if( IsPlayingDoomClassic() )
	{
		// Translate the event to Doom classic format.
		event_t classicEvent;
		if( event->evType == SE_KEY )
		{
		
			if( event->evValue2 == 1 )
			{
				classicEvent.type = ev_keydown;
			}
			else if( event->evValue2 == 0 )
			{
				classicEvent.type = ev_keyup;
			}
			
			DoomLib::SetPlayer( 0 );
			
			extern Globals* g;
			if( g != NULL )
			{
				if (::g->captureBind) {
					classicEvent.data1 = event->GetKey();
				}
				else {
					classicEvent.data1 = DoomLib::RemapControl(event->GetKey());
				}
				
				D_PostEvent( &classicEvent );
			}
			DoomLib::SetPlayer( -1 );
		}
		
		// Let the classics eat all events.
		return true;
	}
#endif
	// RB end
	
	// menus / etc
	if( MenuEvent( event ) )
	{
		return true;
	}
	
	// if we aren't in a game, force the console to take it
	if( !mapSpawned )
	{
		console->ProcessEvent( event, true );
		return true;
	}
	
	// in game, exec bindings for all key downs
	if( event->evType == SE_KEY && event->evValue2 == 1 )
	{
		idKeyInput::ExecKeyBinding( event->evValue );
		return true;
	}
	
	return false;
}

/*
========================
idCommonLocal::ResetPlayerInput
========================
*/
void idCommonLocal::ResetPlayerInput( int playerIndex )
{
	userCmdMgr.ResetPlayer( playerIndex );
}

// RB begin
#if defined(USE_DOOMCLASSIC)

/*
========================
idCommonLocal::SwitchToGame
========================
*/
void idCommonLocal::SwitchToGame( currentGame_t newGame, bool restart )
{
	idealCurrentGame = newGame;
	clRestart = restart;
}

/*
========================
idCommonLocal::PerformGameSwitch
========================
*/
void idCommonLocal::PerformGameSwitch()
{
	// If the session state is past the menu, we should be in Doom 3.
	// This will happen if, for example, we accept an invite while playing
	// Doom or Doom 2.
	if( session->GetState() > idSession::IDLE )
	{
		idealCurrentGame = DOOM3_BFG;
	}
	
	if( currentGame == idealCurrentGame )
	{
		return;
	}
	
	//const int DOOM_CLASSIC_HZ = 35;
	
	if( idealCurrentGame == DOOM_CLASSIC || idealCurrentGame == DOOM2_CLASSIC )
	{
		// Pause Doom 3 sound.
		if( menuSoundWorld != NULL )
		{
			menuSoundWorld->Pause();
		}
		
		DoomLib::skipToNew = false;
		DoomLib::skipToLoad = false;
		
		// Reset match parameters for the classics.
		DoomLib::matchParms = idMatchParameters();
		
		// The classics use the usercmd manager too, clear it.
		userCmdMgr.SetDefaults();
		
		// Classics need a local user too.
		session->UpdateSignInManager();
		session->GetSignInManager().RegisterLocalUser( 0 );
		//GK:Re-stabilize the framerate on classic DOOM
		com_engineHz_denominator = 100LL * (cl_engineHz_interp.GetBool() ? com_engineHz.GetInteger() : cl_engineHz.GetInteger());
		com_engineHz_latched = cl_engineHz.GetInteger();
		//GK: End
		DoomLib::SetCurrentExpansion( idealCurrentGame );
		if (::op != NULL) {
			::op->openInput()->ChangeControllerConfiguration("classic", 0);
		}
		
	}
	else if( idealCurrentGame == DOOM3_BFG )
	{
		DoomLib::Interface.Shutdown();
		com_engineHz_denominator = 100LL * com_engineHz.GetFloat();
		com_engineHz_latched = com_engineHz.GetFloat();
		Sys_ChangeTitle(common->IsNewDOOM3() ? NEW_GAME_NAME : GAME_NAME);
		
		// Don't MoveToPressStart if we have an invite, we need to go
		// directly to the lobby.
		if( session->GetState() <= idSession::IDLE )
		{
			if (::op) {
				::op->SetAdditionalInfo("status", "Game Selection Menu");
				::op->SetAdditionalInfo("large image", "dbfa");
				::op->openInput()->ChangeControllerConfiguration("modern", 0);
			}
			session->MoveToPressStart();
		}
		
		// Unpause Doom 3 sound.
		if( menuSoundWorld != NULL )
		{
			menuSoundWorld->UnPause();
		}
		//GK: Delete Uncompressed files from classic DOOM mods
		CleanUncompFiles(false);

		if (clRestart) {
			Sys_ReLaunch();
		}
	}
	
	currentGame = idealCurrentGame;
	
}

#endif // #if defined(USE_DOOMCLASSIC)
// RB end

/*
==================
Common_WritePrecache_f
==================
*/
CONSOLE_COMMAND( writePrecache, "writes precache commands", NULL )
{
	if( args.Argc() != 2 )
	{
		common->Printf( "USAGE: writePrecache <execFile>\n" );
		return;
	}
	idStr	str = args.Argv( 1 );
	str.DefaultFileExtension( ".cfg" );
	idFile* f = fileSystem->OpenFileWrite( str );
	declManager->WritePrecacheCommands( f );
	renderModelManager->WritePrecacheCommands( f );
	uiManager->WritePrecacheCommands( f );
	
	fileSystem->CloseFile( f );
}

/*
================
Common_Disconnect_f
================
*/
CONSOLE_COMMAND_SHIP( disconnect, "disconnects from a game", NULL )
{
	session->QuitMatch();
}

/*
===============
Common_Hitch_f
===============
*/
CONSOLE_COMMAND( hitch, "hitches the game", NULL )
{
	if( args.Argc() == 2 )
	{
		Sys_Sleep( atoi( args.Argv( 1 ) ) );
	}
	else
	{
		Sys_Sleep( 100 );
	}
}

CONSOLE_COMMAND( showStringMemory, "shows memory used by strings", NULL )
{
	idStr::ShowMemoryUsage_f( args );
}
CONSOLE_COMMAND( showDictMemory, "shows memory used by dictionaries", NULL )
{
	idDict::ShowMemoryUsage_f( args );
}
CONSOLE_COMMAND( listDictKeys, "lists all keys used by dictionaries", NULL )
{
	idDict::ListKeys_f( args );
}
CONSOLE_COMMAND( listDictValues, "lists all values used by dictionaries", NULL )
{
	idDict::ListValues_f( args );
}
CONSOLE_COMMAND( testSIMD, "test SIMD code", NULL )
{
	idSIMD::Test_f( args );
}

// RB begin
CONSOLE_COMMAND( testFormattingSizes, "test printf format security", 0 )
{
	common->Printf( " sizeof( int32 ): %" PRIuSIZE " bytes\n", sizeof( int32 ) );
	common->Printf( " sizeof( int64 ): %" PRIuSIZE " bytes\n", sizeof( int64 ) );
}
// RB end
//GK Begin
void idCommonLocal::WriteDemoInt(int value) {
		writeDemo->WriteInt(value);
}

int idCommonLocal::ReadDemoInt(int& var)
{
		return readDemo->ReadInt(var);
}
//GK End