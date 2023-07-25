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
#include "Main.h"
#include "sys/sys_signin.h"
#include "d3xp/Game_local.h"


#include <string.h>
#include <stdlib.h>

#include "doomdef.h" 
#include "doomstat.h"

#include "z_zone.h"
#include "f_finale.h"
#include "m_argv.h"
#include "m_misc.h"
#include "m_menu.h"
#include "m_random.h"
#include "i_system.h"

#include "p_setup.h"
#include "p_saveg.h"
#include "p_tick.h"

#include "d_main.h"

#include "wi_stuff.h"
#include "hu_stuff.h"
#include "st_stuff.h"
#include "am_map.h"

// Needs access to LFB.
#include "v_video.h"

#include "w_wad.h"

#include "p_local.h" 

#include "s_sound.h"

// Data.
#include "dstrings.h"
#include "sounds.h"

// SKY handling - still the wrong place.
#include "r_data.h"
#include "r_sky.h"

#include "g_game.h"

#include "framework/Common.h"
#include "sys/sys_lobby.h"

#include <limits>

#include "d_exp.h"


extern bool waitingForWipe;
extern idCVar in_alwaysRunCl;
extern idCVar cl_jump;
extern idCVar cl_engineHz_interp;

bool	loadingGame = false;

byte	demoversion = 0;

int circleWeaponPacifier = 0;

qboolean	G_CheckDemoStatus (void); 
void	G_ReadDemoTiccmd (ticcmd_t* cmd); 
void	G_WriteDemoTiccmd (ticcmd_t* cmd); 
void	G_PlayerReborn (int player); 
void	G_InitNew (skill_t skill, int episode, int map ); 

void	G_DoReborn (int playernum); 

void	G_DoLoadLevel (); 
void	G_DoNewGame (void); 
qboolean	G_DoLoadGame (); 
void	G_DoPlayDemo (void); 
void	G_DoCompleted (void); 
void	G_DoVictory (void); 
void	G_DoWorldDone (void); 
qboolean	G_DoSaveGame (void); 


#define	DEBUG_DEMOS
#ifdef _DEBUG
#define DEBUG_DEMOS_WRITE
#endif

#ifdef DEBUG_DEMOS
unsigned char testprndindex = 0;
int printErrorCount = 0;
bool demoDebugOn = false;
#endif

float ogHz = 0.0f;
int ogtr[3] = { 0, 0, 0 };
// 
// controls (have defaults) 
// 

// mouse values are used once 

// joystick values are repeated 


int G_CmdChecksum (ticcmd_t* cmd) 
{ 
	int		i;
	int		sum = 0; 

	for (i=0 ; i< sizeof(*cmd)/4 - 1 ; i++) 
		sum += ((int *)cmd)[i]; 

	return sum; 
} 

// jedi academy meets doom hehehehehehehe
void G_MouseClamp(int *x, int *y)
{
	float ax = (float)fabs((float)*x);
	float ay = (float)fabs((float)*y);

	ax = (ax-10)*(0.04676) * (ax-10) * (ax > 10);
	ay = (ay-10)*(0.04676) * (ay-10) * (ay > 10);
	if (*x < 0)
		*x = static_cast<int>(-ax);
	else
		*x = static_cast<int>(ax);
	if (*y < 0)
		*y = static_cast<int>(-ay);
	else
		*y = static_cast<int>(ay);
}

/*
========================
Returns true if the player is holding down the run button, or
if they have set "Always run" in the options. Returns false otherwise.
========================
*/
bool IsPlayerRunning( const usercmd_t & command ) {

	if( DoomLib::GetPlayer() < 0 ) {
		return false;
	}

	// DHM - Nerve :: Always Run setting
	idLocalUser * user = session->GetSignInManager().GetLocalUserByIndex( DoomLib::GetPlayer() );
	bool autorun = false;


	// GK DONE: PC
#if 1
	if( user) {
		//idPlayerProfileDoom * profile = static_cast< idPlayerProfileDoom * >( user->GetProfile() );

		//if( profile && profile->GetAlwaysRun() ) {
		if (in_alwaysRunCl.GetBool()){
			autorun = true;
		}
	}
#endif

	if ( command.buttons & BUTTON_RUN  ) {
		return !autorun;
	}

	

	

	return autorun;
}

/*
========================
G_PerformImpulse
========================
*/
int G_PerformImpulse( const int impulse, ticcmd_t* cmd ) {

	if( impulse & BUTTON_PREVWEAP ) {
		return 1;
	} else if( impulse & BUTTON_NEXTWEAP ) {
		return 2;
	}  
	return 0;
}

/*
========================
Converts a degree value to DOOM format angle value.
========================
*/
fixed_t DegreesToDoomAngleTurn( float degrees ) {
	const float anglefrac = degrees / 360.0f;
	const fixed_t doomangle = anglefrac * std::numeric_limits<unsigned short>::max();

	return doomangle;
}

//
// G_BuildTiccmd
// Builds a ticcmd from all of the available inputs
// or reads it from the demo buffer. 
// If recording a demo, write it out 
// 
void G_BuildTiccmd (ticcmd_t* cmd, idUserCmdMgr * userCmdMgr, int newTics ) 
{ 
	int		i; 
	int		speed;
	int		tspeed; 
	int		forward;
	int		side;

	ticcmd_t*	base;

	if (::g->demoplayback || ::g->demorecording) {
		com_engineHz_denominator = 100LL * TICRATE;
		com_engineHz_latched = TICRATE;
		int tempticrate[3] = { 0, -1, -1 };
		memcpy(::g->ticrate, tempticrate, sizeof(tempticrate));
	}
	else if (ogHz){
		com_engineHz_denominator = 100LL * (cl_engineHz_interp.GetBool() ? com_engineHz.GetInteger() : ogHz);
		com_engineHz_latched = ogHz;
		ogHz = 0;
		for (int i1 = 0; i1 < 3; i1++) {
			::g->ticrate[i1] = ogtr[i1];
			ogtr[i1] = 0;
		}
	}

	base = I_BaseTiccmd ();		// empty, or external driver
	memcpy (cmd,base,sizeof(*cmd)); 

	cmd->consistancy = ::g->consistancy[::g->consoleplayer][::g->maketic%BACKUPTICS]; 

	// Grab the tech5 tic so we can convert it to a doom tic.
	if ( userCmdMgr != NULL ) {
		const int playerIndex = DoomLib::GetPlayer();

		if( playerIndex < 0 ) {
			return;
		}

#ifdef ID_ENABLE_NETWORKING
		const int lobbyIndex = gameLocal->GetLobbyIndexFromDoomLibIndex( playerIndex );
		const idLocalUser * const localUser = session->GetGameLobbyBase().GetLocalUserFromLobbyUser( lobbyIndex );
#else
		const int lobbyIndex = 0;
		const idLocalUser * const localUser = session->GetSignInManager().GetMasterLocalUser();
#endif

		if ( localUser == NULL ) {
			return;
		}

		usercmd_t * tech5commands[2] = { 0, 0 };

		const int numCommands = userCmdMgr->GetPlayerCmds( lobbyIndex, tech5commands, 2 );

		usercmd_t prevTech5Command;
		usercmd_t curTech5Command;

		// Use default commands if the manager didn't have enough.
		if ( numCommands == 1 ) {
			curTech5Command = *(tech5commands)[0];
		}

		if ( numCommands == 2 ) {
			prevTech5Command = *(tech5commands)[0];
			curTech5Command = *(tech5commands)[1];
		}

		const bool isRunning = IsPlayerRunning( curTech5Command );

		// tech5 move commands range from -127 o 127. Scale to doom range of -25 to 25.
		const float scaledForward = curTech5Command.forwardmove / 127.0f;
		::g->oldforwardmove = cmd->forwardmove;
		if ( isRunning ) {
			cmd->forwardmove = scaledForward * 50.0f;
		} else {
			cmd->forwardmove = scaledForward * 25.0f;
		}
		//GK: In case of 2d flip the caracter if we want to go backwards
		if (game->GetCVarBool("pm_thirdPerson") && abs(game->GetCVarFloat("pm_thirdPersonAngle")) == 90.0F) {
			if (cmd->forwardmove < ::g->oldforwardmove) {
				::g->flip = true;
				cmd->forwardmove = cmd->forwardmove*-1;
			}
			else if (cmd->forwardmove > ::g->oldforwardmove) {
				::g->flip = false;
			}
		}
		// tech5 move commands range from -127 o 127. Scale to doom range of -24 to 24.
		const float scaledSide = curTech5Command.rightmove / 127.0f;
		
		if ( isRunning ) {
			cmd->sidemove = scaledSide * 40.0f;
		} else {
			cmd->sidemove = scaledSide * 24.0f;
		}
		//GK: also flip the movement commands
		if (game->GetCVarBool("pm_thirdPerson") && abs(game->GetCVarFloat("pm_thirdPersonAngle")) == 90.0F) {
			if (::g->flip) {
				cmd->sidemove = cmd->sidemove*-1;
			}
		}

		idAngles angleDelta;
		angleDelta.pitch	= SHORT2ANGLE( curTech5Command.angles[ 0 ] ) - SHORT2ANGLE( prevTech5Command.angles[ 0 ] );
		angleDelta.yaw		= SHORT2ANGLE( curTech5Command.angles[ 1 ] ) - SHORT2ANGLE( prevTech5Command.angles[ 1 ] );
		angleDelta.roll		= 0.0f;
		angleDelta.Normalize180();

		// We will be running a number of tics equal to newTics before we get a new command from tech5.
		// So to keep input smooth, divide the angles between all the newTics.
		if ( newTics > 0 ) {
			//I_Printf("New Tics: %d\n", newTics);
			angleDelta.yaw /= newTics;
			angleDelta.pitch /= newTics;
		}

		
		float engineHz_denominator = com_engineHz_denominator / 100.0f;
		float accelerator = engineHz_denominator / com_engineHz_latched;

		angleDelta.yaw *= accelerator;
		angleDelta.pitch *= accelerator;

		//I_Printf("Mouse Pitch: %f, Mouse Yaw: %f\n", angleDelta.pitch, angleDelta.yaw);

		// idAngles is stored in degrees. Convert to doom format.
		cmd->angleturn = DegreesToDoomAngleTurn( angleDelta.yaw );
		//GK: Since idTech 5 is already giving it then take it and use it
		cmd->angleview = DegreesToDoomAngleTurn(angleDelta.pitch); 

		// Translate buttons
		//if ( curTech5Command.inhibited == false ) {
			// Attack 1 attacks always, whether in the automap or not.
			if ( curTech5Command.buttons & BUTTON_ATTACK ) {
				cmd->buttons |= BT_ATTACK;
			}

#if 0
			// Attack 2 only attacks if not in the automap, because when in the automap,
			// it is the zoom function.
			if ( curTech5Command.buttons & BUTTON_ATTACK2 ) {
				if ( !::g->automapactive ) {
					cmd->buttons |= BT_ATTACK;
				}
			}
#endif

			// Try to read any impulses that have happened.
//			static int oldImpulseSequence = 0;
			int cimpulse = 0;
			//I_Printf("Impulse seq %d", curTech5Command.impulseSequence);
//			if( oldImpulseSequence != curTech5Command.impulseSequence ) {
			//int engineHz_denominator = com_engineHz_denominator / 100;
			cimpulse = G_PerformImpulse( curTech5Command.buttons, cmd );
			if (cimpulse > 0 && circleWeaponPacifier >= ( engineHz_denominator / 2)) { //GK: Weapon change event happend
				circleWeaponPacifier = 0;
				if (!::g->demorecording) {
					/*cmd->buttons |= BT_CHANGE;
					cmd->nextPrevWeapon = cimpulse;*/
					//GK: Just call it directly no more stupid cmd hacks
					P_CircleWeapons(&::g->players[::g->consoleplayer], cimpulse);
				}
				else {
					cmd->buttons |= BT_CHANGE;
					//GK: Super duper circle weapon hack for DEMO Recording
					int idealWeapon = cimpulse + wp_nochange;
					cmd->buttons |= idealWeapon << BT_WEAPONSHIFT;
				}
			}
			circleWeaponPacifier++;
//			}
//			oldImpulseSequence = curTech5Command.impulseSequence;

			// weapon toggle
			for (i = UB_IMPULSE0 ; i < UB_IMPULSE10 ; i++) 
			{   
				if ( usercmdGen->ButtonState( i ) ) 
				{ 
					if (i >= UB_IMPULSE1) {
						i = i - 1;
					}
					if (i >= UB_IMPULSE4) {
						i = i - 1;
					}
					cmd->buttons |= BT_CHANGE; 
					cmd->buttons |= (i - UB_IMPULSE0) <<BT_WEAPONSHIFT; 
					break; 
				}
			}


			if ( curTech5Command.buttons & BUTTON_USE  ) {
				cmd->buttons |= BT_USE;
			}
			::g->jump = false;
			if ( curTech5Command.buttons & BUTTON_JUMP) {
				if (cl_jump.GetBool() && !::g->demorecording) {
					::g->jump = true;
				}
				else {
					cmd->buttons |= BT_USE;
				}
			}

			// TODO: PC
#if 0
			if ( curTech5Command.buttons & BUTTON_WEAP_NEXT ) {
				cmd->buttons |= BT_CHANGE; 
				cmd->buttons |= 1 << BT_WEAPONSHIFT; 
			}

			if ( curTech5Command.buttons & BUTTON_WEAP_PREV ) {
				cmd->buttons |= BT_CHANGE; 
				cmd->buttons |= 0 << BT_WEAPONSHIFT; 
			}

			if( curTech5Command.buttons & BUTTON_WEAP_0 ) {
				cmd->buttons |= BT_CHANGE; 
				cmd->buttons |= 2 << BT_WEAPONSHIFT; 
			}

			if( curTech5Command.buttons & BUTTON_WEAP_1 ) {
				cmd->buttons |= BT_CHANGE; 
				cmd->buttons |= 3 << BT_WEAPONSHIFT; 
			}

			if( curTech5Command.buttons & BUTTON_WEAP_2 ) {
				cmd->buttons |= BT_CHANGE; 
				cmd->buttons |= 4 << BT_WEAPONSHIFT; 
			}

			if( curTech5Command.buttons & BUTTON_WEAP_3 ) {
				cmd->buttons |= BT_CHANGE; 
				cmd->buttons |= 5 << BT_WEAPONSHIFT; 
			}
#endif

		//}

		return;
	}

	// DHM - Nerve :: Always Run setting
	idLocalUser * user = session->GetSignInManager().GetLocalUserByIndex( DoomLib::GetPlayer() );

	if( user ) {
		// TODO: PC
#if 0
		idPlayerProfileDoom * profile = static_cast< idPlayerProfileDoom * >( user->GetProfile() );

		if( profile && profile->GetAlwaysRun() ) {
			speed = !::g->gamekeydown[::g->key_speed];
		} else
#endif
		{
			speed = ::g->gamekeydown[::g->key_speed];
		}

	} else  {

		// Should not happen.
		speed = !::g->gamekeydown[::g->key_speed];
	}

	forward = side = 0;

	// use two stage accelerative turning
	// on the keyboard and joystick
	if (/*:g->joyxmove != 0  ||*/ ::g->gamekeydown[::g->key_right] || ::g->gamekeydown[::g->key_left] || ::g->mousex != 0) 
		::g->turnheld += ::g->ticdup; 
	else 
		::g->turnheld = 0; 

	if (::g->turnheld < SLOWTURNTICS) 
		tspeed = 2;             // slow turn 
	else 
		tspeed = speed;


	// clamp for turning
	int mousex = ::g->mousex;
	int mousey = ::g->mousey;
	G_MouseClamp( &mousex, &mousey );

	if (::g->gamekeydown[::g->key_right] /*|| ::g->joyxmove > 0*/) 
		cmd->angleturn -= ::g->angleturn[tspeed]; 
	else if (::g->mousex > 0) {
		cmd->angleturn -= tspeed == 1 ? 2 * mousex : mousex;
	}

	if (::g->gamekeydown[::g->key_left] /*|| ::g->joyxmove < 0*/) 
		cmd->angleturn += ::g->angleturn[tspeed]; 
	else if (::g->mousex < 0) {
		cmd->angleturn += tspeed == 1 ? -2 * mousex : -mousex;
	}

	if (::g->mousey > 0 || ::g->mousey < 0) {
		//forward += ::g->forwardmove[speed]; 
		forward += speed == 1 ? 2 * ::g->mousey : ::g->mousey;
	}
/*
	if (::g->mousey < 0) {
		forward -= ::g->forwardmove[speed];
	}
*/
/*
	if (::g->gamekeydown[::g->key_straferight]) 
		side += ::g->sidemove[speed]; 
	if (::g->gamekeydown[::g->key_strafeleft]) 
		side -= ::g->sidemove[speed];
*/

	if ( ::g->joyxmove > 0 || ::g->joyxmove < 0 ) {
		side += speed == 1 ? 2 * ::g->joyxmove : ::g->joyxmove;
	}

	// buttons
	if (::g->gamekeydown[::g->key_fire] || ::g->mousebuttons[::g->mousebfire] || ::g->joybuttons[::g->joybfire]) 
		cmd->buttons |= BT_ATTACK; 

	if (::g->gamekeydown[::g->key_use] || ::g->joybuttons[::g->joybuse] ) 
		cmd->buttons |= BT_USE;

	// DHM - Nerve :: In the intermission or finale screens, make START also create a 'use' command.
	if ( (::g->gamestate == GS_INTERMISSION || ::g->gamestate == GS_FINALE) && ::g->gamekeydown[KEY_ESCAPE] ) {		
		cmd->buttons |= BT_USE;
	}

	// weapon toggle
	for (i=0 ; i<NUMWEAPONS-1 ; i++) 
	{   
		if (::g->gamekeydown['1'+i]) 
		{ 
			cmd->buttons |= BT_CHANGE; 
			cmd->buttons |= i<<BT_WEAPONSHIFT; 
			break; 
		}
	}

	::g->mousex = ::g->mousey = 0; 

	if (forward > MAXPLMOVE) 
		forward = MAXPLMOVE; 
	else if (forward < -MAXPLMOVE) 
		forward = -MAXPLMOVE; 
	if (side > MAXPLMOVE) 
		side = MAXPLMOVE; 
	else if (side < -MAXPLMOVE) 
		side = -MAXPLMOVE; 

	cmd->forwardmove += forward; 
	cmd->sidemove += side;

	// special buttons
	if (::g->sendpause) 
	{ 
		::g->sendpause = false; 
		cmd->buttons = BT_SPECIAL | BTS_PAUSE; 
	} 

	if (::g->sendsave) 
	{ 
		::g->sendsave = false; 
		cmd->buttons = BT_SPECIAL | BTS_SAVEGAME | (::g->savegameslot<<BTS_SAVESHIFT); 
	} 
} 


//
// G_DoLoadLevel 
//

void G_DoLoadLevel () 
{ 
	int             i; 

	M_ClearRandom();

	// Set the sky map.
	// First thing, we have a dummy sky texture name,
	//  a flat. The data is in the WAD only because
	//  we look for an actual index, instead of simply
	//  setting one.
	::g->skyflatnum = R_FlatNumForName ( SKYFLATNAME );
	//GK: Re-render the sky buffer on every map 
	//in order to have a color similar to the sky flat
	if (::g->skybuffer) {
		free(::g->skybuffer);
		::g->skybuffer = NULL;
	}
	// DOOM determines the sky texture to be used
	// depending on the current episode, and the game version.
	if ( ::g->gamemode == commercial )
	{
		if (::g->gamemission != pack_master) {
			::g->skytexture = R_TextureNumForName("SKY3");

			if (::g->gamemap < 12) {
				::g->skytexture = R_TextureNumForName("SKY1");
			}
			else if (::g->gamemap < 21) {
				::g->skytexture = R_TextureNumForName("SKY2");
			}
		}
		else {
			if (::g->gamemap > 18) {
				::g->skytexture = R_TextureNumForName("SKY3");
			}
			else {
				::g->skytexture = R_TextureNumForName("SKY1");
				switch (::g->gamemap) {
				case 4:
					::g->skytexture = R_TextureNumForName("STAR1");
					break;
				case 7:
					::g->skytexture = R_TextureNumForName("STAR2");
					break;
				case 11:
					::g->skytexture = R_TextureNumForName("STAR2");
					break;
				case 12:
					::g->skytexture = R_TextureNumForName("STAR3");
					break;
				case 13:
					::g->skytexture = R_TextureNumForName("STAR3");
					break;
				case 16:
					::g->skytexture = R_TextureNumForName("STAR3");
					break;
				case 17:
					::g->skytexture = R_TextureNumForName("STAR3");
					break;
				case 18:
					::g->skytexture = R_TextureNumForName("STAR3");
					break;
				}
			}
		}
	}
	if (::g->gamemission == pack_custom && ::g->map) { //GK: Custom expansion related stuff
		if (::g->maps[::g->map - 1].sky != NULL) {
			::g->skytexture = R_TextureNumForName(::g->maps[::g->map - 1].sky);
		}
	}
	if (::g->s_textures[::g->skytexture]->height >= 200) {//GK:Tall skies support
		::g->skytexturemid = 200 * FRACUNIT;
	}
	else {
		::g->skytexturemid = 100 * FRACUNIT;
	}
	::g->levelstarttic = ::g->gametic;        // for time calculation

	if (::g->wipegamestate == GS_LEVEL || ::g->wipegamestate == GS_DEMOLEVEL) {
		::g->wipegamestate = (gamestate_t)-1;             // force a wipe 
	} else if ( ::g->netgame ) {
		::g->wipegamestate = GS_LEVEL;
	}

	::g->gamestate = GS_LEVEL; 

	for (i=0 ; i<MAXPLAYERS ; i++) 
	{ 
		if (::g->playeringame[i] && ::g->players[i].playerstate == PST_DEAD) {
			if (::g->demorecording)
				G_CheckDemoStatus();
			::g->players[i].playerstate = PST_REBORN;
		}
		memset (::g->players[i].frags,0,sizeof(::g->players[i].frags));
		memset (&(::g->players[i].cmd),0,sizeof(::g->players[i].cmd)); 
	} 

	const char * difficultyNames[] = {  "I'm Too Young To Die!", "Hey, Not Too Rough!", "Hurt Me Plenty!", "Ultra-Violence", "Nightmare" };
	const ExpansionData * expansion = DoomLib::GetCurrentExpansion();
	
	int truemap = ::g->gamemap;

	if( ::g->gamemission == doom ) {
		truemap = ( ::g->gameepisode - 1 ) * 9 + ( ::g->gamemap );
	}

	idMatchParameters newParms = session->GetActingGameStateLobbyBase().GetMatchParms();
	DoomLib::SetCurrentMapName( expansion->mapNames[ truemap - 1 ] );
	DoomLib::SetCurrentDifficulty( difficultyNames[ ::g->gameskill ]  );
	// initialize the msecnode_t freelist.                     phares 3/25/98
	// any nodes in the freelist are gone by now, cleared
	// by Z_FreeTags() when the previous level ended or player
	// died.

	::g->headsecnode = NULL;

	P_SetupLevel (::g->gameepisode, ::g->gamemap, 0, ::g->gameskill);

	::g->displayplayer = ::g->consoleplayer;		// view the guy you are playing    
	::g->starttime = I_GetTime (); 
	::g->gameaction = ga_nothing; 

	// clear cmd building stuff
	memset (::g->gamekeydown, 0, sizeof(::g->gamekeydown)); 
	::g->joyxmove = ::g->joyymove = 0; 
	::g->mousex = ::g->mousey = 0; 
	::g->sendpause = ::g->sendsave = ::g->paused = false; 
	memset (::g->mousebuttons, 0, sizeof(::g->mousearray) / sizeof(qboolean)); 
	memset (::g->joybuttons, 0, sizeof(::g->joyarray) / sizeof(qboolean));
}

//
// G_Responder  
// Get info needed to make ticcmd_ts for the ::g->players.
// 
qboolean G_Responder (event_t* ev) 
{ 
	// allow spy mode changes even during the demo
	if ((::g->gamestate == GS_LEVEL || ::g->gamestate == GS_DEMOLEVEL) && ev->type == ev_keydown
		&& ev->data1 == KEY_F12 && (::g->singledemo || !::g->deathmatch) )
	{
		// spy mode 
		do 
		{ 
			::g->displayplayer++; 
			if (::g->displayplayer == MAXPLAYERS) 
				::g->displayplayer = 0; 
		} while (!::g->playeringame[::g->displayplayer] && ::g->displayplayer != ::g->consoleplayer); 
		return true; 
	}

	// any other key pops up menu if in demos
	if (::g->gameaction == ga_nothing && !::g->singledemo && 
		(::g->demoplayback || ::g->gamestate == GS_DEMOSCREEN) 
		) 
	{ 
		if (ev->type == ev_keydown ||  
			(ev->type == ev_mouse && ev->data1) ||
			(ev->type == ev_joystick && ev->data1) ) 
		{ 
			M_StartControlPanel (); 
			return true; 
		} 
		return false; 
	} 

	if ((::g->gamestate == GS_LEVEL || ::g->gamestate == GS_DEMOLEVEL) && ( ::g->usergame || ::g->netgame || ::g->demoplayback ))
	{ 
#if 0 
		if (::g->devparm && ev->type == ev_keydown && ev->data1 == ';') 
		{ 
			G_DeathMatchSpawnPlayer (0); 
			return true; 
		} 
#endif 
		if (HU_Responder (ev)) 
			return true;	// chat ate the event 
		if (ST_Responder (ev)) 
			return true;	// status window ate it 
		if (AM_Responder (ev)) 
			return true;	// automap ate it 
	} 

	if (::g->gamestate == GS_FINALE) 
	{ 
		if (F_Responder (ev)) 
			return true;	// finale ate the event 
	} 

	switch (ev->type) 
	{ 
	case ev_keydown: 
		if (ev->data1 == KEY_PAUSE) 
		{ 
			::g->sendpause = true; 
			return true; 
		} 
		if (ev->data1 <NUMKEYS) 
			::g->gamekeydown[ev->data1] = true; 
		return true;    // eat key down ::g->events 

	case ev_keyup:
		// DHM - Nerve :: Old School!
		//if ( ev->data1 == '-' ) {
			//App->Renderer->oldSchool = !App->Renderer->oldSchool;
		//}

		if (ev->data1 <NUMKEYS) 
			::g->gamekeydown[ev->data1] = false; 
		return false;   // always let key up ::g->events filter down 

	case ev_mouse: 
 		::g->mousebuttons[0] = ev->data1 & 1; 
 		::g->mousebuttons[1] = ev->data1 & 2; 
 		::g->mousebuttons[2] = ev->data1 & 4; 
 		::g->mousex = ev->data2*(::g->mouseSensitivity+5)/10; 
 		::g->mousey = ev->data3*(::g->mouseSensitivity+5)/10; 
 		return true;    // eat ::g->events 

	case ev_joystick: 
		::g->joybuttons[0] = ev->data1 & 1; 
		::g->joybuttons[1] = ev->data1 & 2; 
		::g->joybuttons[2] = ev->data1 & 4; 
		::g->joybuttons[3] = ev->data1 & 8; 
		::g->joyxmove = ev->data2; 
/*
		::g->gamekeydown[::g->key_straferight] = ::g->gamekeydown[::g->key_strafeleft] = 0;
		if (ev->data2 > 0)
			::g->gamekeydown[::g->key_straferight] = 1;
		else if (ev->data2 < 0)
			::g->gamekeydown[::g->key_strafeleft] = 1;
*/
		::g->joyymove = ev->data3; 
		return true;    // eat ::g->events 

	default: 
		break; 
	} 

	return false; 
} 



//
// G_Ticker
// Make ticcmd_ts for the ::g->players.
//
void G_Ticker (void) 
{ 
	int		i;
	int		buf; 
	ticcmd_t*	cmd;

	//GK: When Doom 3 says pause then pause
	::g->paused = cvarSystem->GetCVarBool("com_pause");
	if (::g->paused) {
		S_PauseSound();
		return;
	} else
		S_ResumeSound();

	// do player reborns if needed
	for (i=0 ; i<MAXPLAYERS ; i++) 
		if (::g->playeringame[i] && ::g->players[i].playerstate == PST_REBORN) {
			if (!::g->netgame && ::g->demorecording)
				G_CheckDemoStatus();
			G_DoReborn(i);
		}

	// do things to change the game state
	while (::g->gameaction != ga_nothing) 
	{ 
		switch (::g->gameaction) 
		{ 
		case ga_loadlevel: 
			G_DoLoadLevel (); 
			break; 
		case ga_newgame: 
			G_DoNewGame (); 
			break; 
		case ga_loadgame: 
			G_DoLoadGame ();
			break; 
		case ga_savegame:
			//GK: Show "Game saved" after saving
			if (G_DoSaveGame()) {
				::g->plyr->message = GGSAVED;
			}
			break; 
		case ga_playdemo: 
			G_DoPlayDemo (); 
			break; 
		case ga_completed: 
			G_DoCompleted (); 
			break; 
		case ga_victory: 
			F_StartFinale (); 
			break; 
		case ga_worlddone: 
			G_DoWorldDone ();
			break; 
		case ga_screenshot: 
			M_ScreenShot (); 
			::g->gameaction = ga_nothing; 
			break; 
		case ga_nothing: 
			break; 
		} 
	}

	// get commands, check ::g->consistancy,
	// and build new ::g->consistancy check
	buf = (::g->gametic/::g->ticdup)%BACKUPTICS; 

	for (i=0 ; i<MAXPLAYERS ; i++)
	{
		if (::g->playeringame[i]) 
		{ 
			cmd = &::g->players[i].cmd; 

			memcpy (cmd, &::g->netcmds[i][buf], sizeof(ticcmd_t)); 

			if ( ::g->demoplayback ) {
				G_ReadDemoTiccmd( cmd );
#ifdef DEBUG_DEMOS
				if ( demoDebugOn && testprndindex != ::g->prndindex && printErrorCount++ < 10 ) {
					I_Printf( "time: %d, g->prndindex(%d) does not match demo prndindex(%d)!\n", ::g->leveltime, ::g->prndindex, testprndindex );
				}
#endif
			}

			if ( ::g->demorecording ) {
				G_WriteDemoTiccmd (cmd);
			}

			// HACK ALERT ( the GS_FINALE CRAP IS A HACK.. )
			if (::g->netgame && !::g->netdemo && !(::g->gametic % ::g->ticdup) && !(::g->gamestate == GS_FINALE ) ) 
			{
				if (::g->gametic > BACKUPTICS && ::g->consistancy[i][buf] != cmd->consistancy) 
				{
					printf ("consistency failure (%i should be %i)",
						cmd->consistancy, ::g->consistancy[i][buf]); 

					// TODO: If we ever support splitscreen and online,
					// we'll have to call D_QuitNetGame for all local players.
					D_QuitNetGame();

					session->QuitMatch();
					common->Dialog().AddDialog( GDM_CONNECTION_LOST_HOST, DIALOG_ACCEPT, NULL, NULL, false );
				}

				if (::g->players[i].mo) 
					::g->consistancy[i][buf] = ::g->players[i].mo->x; 
				else 
					::g->consistancy[i][buf] = ::g->rndindex;
			} 
		}
	}
	

	// check for special buttons
	for (i=0 ; i<MAXPLAYERS ; i++)
	{
		if (::g->playeringame[i]) 
		{ 
			if (::g->players[i].cmd.buttons & BT_SPECIAL) 
			{ 
				switch (::g->players[i].cmd.buttons & BT_SPECIALMASK) 
				{ 
				case BTS_PAUSE: 
					::g->paused ^= 1;

					// DHM - Nerve :: Don't pause the music
					/*
					if (::g->paused) 
						S_PauseSound (); 
					else 
						S_ResumeSound (); 
					*/
					break; 

				case BTS_SAVEGAME: 
					
					if (!::g->savedescription[0]) 
						strcpy (::g->savedescription, "NET GAME"); 
					::g->savegameslot = (::g->players[i].cmd.buttons & BTS_SAVEMASK)>>BTS_SAVESHIFT; 
					::g->gameaction = ga_savegame; 
					
					break; 
				} 
			} 
		}
	}

	// do main actions
	switch (::g->gamestate) 
	{ 
	case GS_DEMOLEVEL:
	case GS_LEVEL: 
		P_Ticker (); 
		ST_Ticker (); 
		AM_Ticker (); 
		HU_Ticker ();            
		break; 

	case GS_INTERMISSION: 
		WI_Ticker (); 
		break; 

	case GS_FINALE: 
		F_Ticker (); 
		break; 

	case GS_DEMOSCREEN: 
		D_PageTicker (); 
		break; 
	}        
} 


//
// PLAYER STRUCTURE FUNCTIONS
// also see P_SpawnPlayer in P_Things
//

//
// G_InitPlayer 
// Called at the start.
// Called by the game initialization functions.
//
void G_InitPlayer (int player) 
{ 
	player_t*	p; 

	// set up the saved info         
	p = &::g->players[player]; 

	// clear everything else to defaults 
	G_PlayerReborn (player); 

} 



//
// G_PlayerFinishLevel
// Can when a player completes a level.
//
void G_PlayerFinishLevel (int player) 
{ 
	player_t*	p; 

	p = &::g->players[player]; 

	memset (p->powers, 0, sizeof (p->powers)); 
	memset (p->cards, 0, sizeof (p->cards)); 
	p->mo->flags &= ~MF_SHADOW;		// cancel invisibility 
	p->extralight = 0;			// cancel gun flashes 
	p->fixedcolormap = 0;		// cancel ir gogles 
	p->damagecount = 0;			// no palette changes 
	p->bonuscount = 0; 
} 

//
// G_PlayerReborn
// Called after a player dies 
// almost everything is cleared and initialized 
//
void G_PlayerReborn (int player) 
{ 
	player_t*	p; 
	int		i; 
	int		frags[MAXPLAYERS]; 
	int		killcount;
	int		itemcount;
	int		secretcount;

	// DHM - Nerve :: cards are saved across death in coop multiplayer
	qboolean cards[NUMCARDS];
	bool hasMapPowerup = false;

	hasMapPowerup = ::g->players[player].powers[pw_allmap] != 0;
	memcpy( cards, ::g->players[player].cards, sizeof(cards) );
	memcpy( frags, ::g->players[player].frags, sizeof(frags) );
	killcount = ::g->players[player].killcount;
	itemcount = ::g->players[player].itemcount;
	secretcount = ::g->players[player].secretcount;

	p = &::g->players[player];
	memset (p, 0, sizeof(*p));

	// DHM - Nerve :: restore cards in multiplayer
	// TODO: Networking
#ifdef ID_ENABLE_DOOM_CLASSIC_NETWORKING
	if ( common->IsMultiplayer() || /*gameLocal->IsSplitscreen() ||*/ (::g->demoplayback && ::g->netdemo) ) { //GK:No splitscreen
		if ( hasMapPowerup ) {
			::g->players[player].powers[pw_allmap] = 1;
		}
		memcpy (::g->players[player].cards, cards, sizeof(::g->players[player].cards));
	}
#endif
	memcpy (::g->players[player].frags, frags, sizeof(::g->players[player].frags));
	::g->players[player].killcount = killcount;
	::g->players[player].itemcount = itemcount;
	::g->players[player].secretcount = secretcount;

	p->usedown = p->attackdown = true;	// don't do anything immediately
	p->playerstate = PST_LIVE;
	p->health = ::g->ihealth;
	p->readyweapon = p->pendingweapon = wp_pistol;
	p->weaponowned[wp_fist] = true;
	p->weaponowned[wp_pistol] = true;
	p->ammo[am_clip] = ::g->iammo;
	// TODO: PC
#if 0
	p->cheats = gameLocal->cheats;
#else
	p->cheats = 0;
#endif

	for (i=0 ; i<NUMAMMO ; i++)
		p->maxammo[i] = maxammo[i];
}

//
// G_CheckSpot  
// Returns false if the player cannot be respawned
// at the given mapthing_t spot  
// because something is occupying it 
//
void P_SpawnPlayer (mapthing_t* mthing); 

qboolean
G_CheckSpot
( int		playernum,
 mapthing_t*	mthing ) 
{ 
	fixed_t		x;
	fixed_t		y; 
	subsector_t*	ss; 
	unsigned		an; 
	mobj_t*		mo; 
	int			i;

	if (!::g->players[playernum].mo)
	{
		// first spawn of level, before corpses
		for (i=0 ; i<playernum ; i++)
			if (::g->players[i].mo->x == mthing->x << FRACBITS
				&& ::g->players[i].mo->y == mthing->y << FRACBITS)
				return false;	
		return true;
	}

	x = mthing->x << FRACBITS; 
	y = mthing->y << FRACBITS; 

	if (!P_CheckPosition (::g->players[playernum].mo, x, y) ) 
		return false; 

	// flush an old corpse if needed 
	if (::g->bodyqueslot >= BODYQUESIZE) 
		P_RemoveMobj (::g->bodyque[::g->bodyqueslot%BODYQUESIZE]); 
	::g->bodyque[::g->bodyqueslot%BODYQUESIZE] = ::g->players[playernum].mo; 
	::g->bodyqueslot++; 

	// spawn a teleport fog 
	ss = R_PointInSubsector (x,y); 
	an = ( ANG45 * (mthing->angle/45) ) >> ANGLETOFINESHIFT; 

	mo = P_SpawnMobj (x+20*finecosine[an], y+20*finesine[an] 
	, ss->sector->floorheight 
		, MT_TFOG); 

	if (::g->players[::g->consoleplayer].viewz != 1 && (playernum == ::g->consoleplayer)) 
		S_StartSound (::g->players[::g->consoleplayer].mo, sfx_telept);	// don't start sound on first frame 

	return true; 
} 


//
// G_DeathMatchSpawnPlayer 
// Spawns a player at one of the random death match spots 
// called at level load and each death 
//
void G_DeathMatchSpawnPlayer (int playernum) 
{ 
	int             i,j; 
	int				selections; 

	selections = ::g->deathmatch_p - ::g->deathmatchstarts; 
	if (selections < 4) 
		I_Error ("Only %i ::g->deathmatch spots, 4 required", selections); 

	for (j=0 ; j<20 ; j++) 
	{ 
		i = P_Random() % selections; 
		if (G_CheckSpot (playernum, &::g->deathmatchstarts[i]) ) 
		{ 
			::g->deathmatchstarts[i].type = playernum+1; 
			P_SpawnPlayer (&::g->deathmatchstarts[i]); 
			return; 
		} 
	} 

	// no good spot, so the player will probably get stuck 
	P_SpawnPlayer (&::g->playerstarts[playernum]); 
} 


//
// G_DoReborn 
// 
void G_DoReborn (int playernum) 
{ 
	int                             i; 

	if (!::g->netgame)
	{
		// reload the level from scratch
		::g->gameaction = ga_loadlevel;  
	}
	else 
	{
		// respawn at the start

		// first dissasociate the corpse 
		::g->players[playernum].mo->player = NULL;   

		// spawn at random spot if in death match 
		if (::g->deathmatch) 
		{ 
			G_DeathMatchSpawnPlayer (playernum); 
			return; 
		} 

		if (G_CheckSpot (playernum, &::g->playerstarts[playernum]) ) 
		{ 
			P_SpawnPlayer (&::g->playerstarts[playernum]); 
			return; 
		}

		// try to spawn at one of the other ::g->players spots 
		for (i=0 ; i<MAXPLAYERS ; i++)
		{
			if (G_CheckSpot (playernum, &::g->playerstarts[i]) ) 
			{ 
				::g->playerstarts[i].type = playernum+1;	// fake as other player 
				P_SpawnPlayer (&::g->playerstarts[i]); 
				::g->playerstarts[i].type = i+1;		// restore 
				return; 
			}	    
			// he's going to be inside something.  Too bad.
		}
		P_SpawnPlayer (&::g->playerstarts[playernum]); 
	} 
} 


void G_ScreenShot (void) 
{ 
	::g->gameaction = ga_screenshot; 
} 


// DHM - Nerve :: Added episode 4 par times
// DOOM Par Times
/*const*/ int pars[5][10] = 
{ 
	{0}, 
	{0,30,75,120,90,165,180,180,30,165},
	{0,90,90,90,120,90,360,240,30,170},
	{0,90,45,90,150,90,90,165,30,135},
	{0,165,255,135,150,180,390,135,360,180}
}; 

// DOOM II Par Times
/*const*/ int cpars[33] =
{
	30,90,120,120,90,150,120,120,270,90,		//  1-10
	210,150,150,150,210,150,420,150,210,150,	// 11-20
	240,150,180,150,150,300,330,420,300,180,	// 21-30
	120,30,240									// 31-33
};

int npars[9] =
{
	75,105,120,105,210,105,165,105,135			// 1-9
};

void ResetPars(void) {
	 int tpars[5][10] =
	{
		{ 0 },
	{ 0,30,75,120,90,165,180,180,30,165 },
	{ 0,90,90,90,120,90,360,240,30,170 },
	{ 0,90,45,90,150,90,90,165,30,135 },
	{ 0,165,255,135,150,180,390,135,360,180 }
	};
	memcpy(pars, tpars, sizeof(tpars));

	int tcpars[33] =
	{
		30,90,120,120,90,150,120,120,270,90,		//  1-10
		210,150,150,150,210,150,420,150,210,150,	// 11-20
		240,150,180,150,150,300,330,420,300,180,	// 21-30
		120,30,240									// 31-33
	};
	memcpy(cpars, tcpars, sizeof(tcpars));
}


//
// G_DoCompleted 
//

void G_ExitLevel (void) 
{ 
	::g->secretexit = false; 
	::g->gameaction = ga_completed; 
} 

// Here's for the german edition.
void G_SecretExitLevel (void) 
{ 
	// IF NO WOLF3D LEVELS, NO SECRET EXIT!
	if ( (::g->gamemode == commercial)
		&& (W_CheckNumForName("map31")<0))
		::g->secretexit = false;
	else
		::g->secretexit = true; 
	::g->gameaction = ga_completed; 
} 

void FindNextMap(int map,bool issecret = false) {
	int* nmap = &::g->maps[map].nextmap;
	char* name = ::g->maps[map].nextmapname;
	if (issecret) {
		nmap = &::g->maps[map].secretmap;
		name = ::g->maps[map].secretmapname;
	}
	if (*nmap == -1) {
		for (uint j = 0; j < ::g->maps.size(); j++) {
			if (::g->maps[j].lumpname != NULL) {
				if (!idStr::Icmp(name, ::g->maps[j].lumpname)) {
					*nmap = j;
					break;
				}
			}
		}
	}
	else {
		for (uint j = 0; j < ::g->maps.size(); j++) {
			if (*nmap == ::g->maps[j].index) {
				*nmap = j;
			}
		}
	}
}

void G_DoCompleted (void) 
{ 
	int             i; 

	::g->gameaction = ga_nothing; 

	for (i=0 ; i<MAXPLAYERS ; i++) {
		if (::g->playeringame[i]) {
			G_PlayerFinishLevel (i);        // take away cards and stuff
		}
	}

	if (::g->automapactive) {
		AM_Stop();
	}

	if ( ::g->demoplayback ) {
		G_CheckDemoStatus();
		return;
	}

	if ( ::g->demorecording ) {
		G_CheckDemoStatus();
	}

	// DHM - Nerve :: Deathmatch doesn't go to finale screen, just do intermission
	if ( ::g->gamemode != commercial && !::g->deathmatch ) {
		switch(::g->gamemap) {

		case 8:
			::g->gameaction = ga_victory;
			return;
		case 9: 
			for (i=0 ; i<MAXPLAYERS ; i++) 
				::g->players[i].didsecret = true; 
			break;
		}
	}

	::g->wminfo.didsecret = ::g->players[::g->consoleplayer].didsecret; 
	::g->wminfo.epsd = ::g->gameepisode -1; 
	::g->wminfo.last = ::g->gamemap -1;

	// ::g->wminfo.next is 0 biased, unlike ::g->gamemap
	if ( ::g->gamemode == commercial)
	{
		if (::g->secretexit) {
			if ( ::g->gamemission == doom2 || ::g->gamemission == pack_tnt || ::g->gamemission == pack_plut ) {
				switch(::g->gamemap)
				{
				    case 2:  if (::g->isbfg) { ::g->wminfo.next = 32; }break; //GK: Enable the access to the xbox exclusive secret level
					case 15: ::g->wminfo.next = 30; break;
					case 31: ::g->wminfo.next = 31; break;
				}
			} else if( ::g->gamemission == pack_nerve ) {

				// DHM - Nerve :: Secret level is always level 9 on extra Doom2 missions
				//::g->prevmap = ::g->gamemap;
				::g->wminfo.next = 8;
			}
			else if (::g->gamemission == pack_master) {
				::g->wminfo.next = 20;
			}
			else if (::g->gamemission == pack_custom) { //GK: Custom expansion related stuff
				FindNextMap(::g->gamemap - 1, true);
					if (::g->maps[::g->gamemap-1].secretmap) {
						::g->wminfo.next = ::g->maps[::g->gamemap-1].secretmap;
					}
				
			}
		}
		else {
			if ( ::g->gamemission == doom2 || ::g->gamemission == pack_tnt || ::g->gamemission == pack_plut ) {
				switch(::g->gamemap)
				{
					case 31:
					case 32: ::g->wminfo.next = 15; break;
					case 33: ::g->wminfo.next = 2; break;
					default: ::g->wminfo.next = ::g->gamemap;
				}
			}
			else if( ::g->gamemission == pack_nerve) {
				switch(::g->gamemap)
				{	case 9:
						::g->wminfo.next = 4;//::g->prevmap;
						break;
					default:
						::g->wminfo.next = ::g->gamemap;
						break;
				}
			
			}
			else if (::g->gamemission == pack_custom) { //GK: Custom expansion related stuff
				FindNextMap(::g->gamemap - 1);
				if ((::g->maps[::g->gamemap - 1].nextmap) < ::g->mapmax)
					::g->wminfo.next = ::g->maps[::g->gamemap - 1].nextmap;
				else
					::g->wminfo.next = 0;
			}
			else {
				::g->wminfo.next = ::g->gamemap;
			}
		}
	}
	else
	{
			if (::g->secretexit) {
				::g->wminfo.next = 8; 	// go to secret level 
			}
			else if (::g->gamemap == 9)
			{
				// returning from secret level 
				switch (::g->gameepisode)
				{
				case 1:
					::g->wminfo.next = 3;
					break;
				case 2:
					::g->wminfo.next = 5;
					break;
				case 3:
					::g->wminfo.next = 6;
					break;
				case 4:
					::g->wminfo.next = 2;
					break;
				}
			}
			else
				::g->wminfo.next = ::g->gamemap;          // go to next level 

			if (::g->gamemission == pack_custom) { //GK: Custom expansion related stuff
			//int map = ::g->clusters[::g->gameepisode - 1].startmap + (::g->gamemap - 1);
			if (::g->map) {
				if (::g->secretexit) {
					FindNextMap(::g->map - 1, true);
					::g->wminfo.next = ::g->maps[::g->map - 1].secretmap;
				}
				else {
					FindNextMap(::g->map - 1);
					::g->wminfo.next = ::g->maps[::g->map - 1].nextmap - (::g->clusters[::g->gameepisode - 1].startmap - 1);
				}
			}
		}
	}

	// DHM - Nerve :: In deathmatch, repeat the current level.  User must exit and choose a new level.
	if ( ::g->deathmatch ) {
		::g->wminfo.next = ::g->wminfo.last;
	}

	::g->wminfo.maxkills = ::g->totalkills; 
	::g->wminfo.maxitems = ::g->totalitems; 
	::g->wminfo.maxsecret = ::g->totalsecret; 
	::g->wminfo.maxfrags = 0; 

	if ( ::g->gamemode == commercial ) {
		switch (::g->gamemission) {
		case pack_nerve:
			::g->wminfo.partime = TICRATE * npars[::g->gamemap - 1];
			break;
		default:
			::g->wminfo.partime = TICRATE * cpars[::g->gamemap - 1];
			break;
		}
	}
	else {
		if (::g->gameepisode < 5 && ::g->gamemap < 10) {
			::g->wminfo.partime = TICRATE * pars[::g->gameepisode][::g->gamemap];
		}
	}

	if (::g->gamemission == pack_custom && ::g->map) {
		if (::g->maps[::g->map -1].par)
		::g->wminfo.partime = TICRATE * ::g->maps[::g->map - 1].par;
	}

	::g->wminfo.pnum = ::g->consoleplayer; 

	for (i=0 ; i<MAXPLAYERS ; i++) 
	{ 
		::g->wminfo.plyr[i].in = ::g->playeringame[i]; 
		::g->wminfo.plyr[i].skills = ::g->players[i].killcount; 
		::g->wminfo.plyr[i].sitems = ::g->players[i].itemcount; 
		::g->wminfo.plyr[i].ssecret = ::g->players[i].secretcount; 
		::g->wminfo.plyr[i].stime = ::g->leveltime; 
		memcpy (::g->wminfo.plyr[i].frags, ::g->players[i].frags 
			, sizeof(::g->wminfo.plyr[i].frags)); 
	} 

	::g->gamestate = GS_INTERMISSION; 
	::g->viewactive = false; 
	::g->automapactive = false; 

	WI_Start (&::g->wminfo); 
} 


//
// G_WorldDone 
//
void G_WorldDone (void) 
{ 
	::g->gameaction = ga_worlddone; 

	if (::g->secretexit) 
		::g->players[::g->consoleplayer].didsecret = true; 

	if (::g->gamemission == pack_custom) { //GK: Custom expansion related stuff
		if (::g->gamemode == retail && (int)::g->clusters.size() >= ::g->gameepisode) {
			if (::g->clusters[::g->gameepisode - 1].startmap > -1 && ::g->clusters[::g->gameepisode - 1].endmap > ::g->clusters[::g->gameepisode - 1].startmap) {
				int map = (::g->clusters[::g->gameepisode - 1].startmap - 1) + ::g->gamemap;
				if (map == ::g->clusters[::g->gameepisode - 1].endmap) {
					F_StartFinale();
				}
			}
		}
		if (::g->gamemap == ::g->endmap || ::g->maps[::g->gamemap - 1].cluster != ::g->maps[::g->wminfo.next].cluster || ::g->maps[::g->gamemap - 1].ftext != NULL) {
			if (!::g->secretexit == !::g->maps[::g->gamemap - 1].fsecret) {
				F_StartFinale();
			}
		}
	}
	if ( ::g->gamemode == commercial )
	{

		if (::g->gamemission == pack_master) {

			if (DoomLib::use_doomit) {
				if (!::g->secretexit) {

					//::g->currentMenu->lastOn = ::g->itemOn;
					//M_ClearMenus();
					DoomLib::SetCurrentExpansion(doom2);
					DoomLib::use_doomit = false;
					D_StartTitle();
					return;
				}

			}
		}
		 {
			//if ( ::g->gamemission == doom2 || ::g->gamemission == pack_tnt || ::g->gamemission == pack_plut || ((::g->gamemission == pack_nerve || ::g->gamemission == pack_master) && ::g->modifiedtext)) {
			switch (::g->gamemap)
			{
			case 15:
				if (::g->gamemission == pack_master && !::g->modftext)
					break;
			case 31:
				if (!::g->secretexit)
					break;
			case 6:
				if ((::g->gamemission == pack_nerve || ::g->gamemission == pack_master) && !::g->modftext)
					break;
			case 11:
				if (::g->gamemission == pack_master && !::g->modftext)
					break;
			case 20:
				if (::g->gamemission == pack_master && ::g->secretexit)
					break;
			case 30:
				F_StartFinale();
				break;
			case 8:
				if (::g->gamemission == pack_nerve)
					F_StartFinale();
				break;
			case 21:
				if (::g->gamemission == pack_master)
					F_StartFinale();
				break;
			}
		}
	}
} 

void G_DoWorldDone (void) 
{        
	::g->gamestate = GS_LEVEL;

	::g->gamemap = ::g->wminfo.next+1;
	//GK: When changing the gamemap change and the map for the custom expansion
	if (::g->gamemission == pack_custom) {
		setMapNum();
	}

	M_ClearRandom();

	for ( int i = 0; i < MAXPLAYERS; i++ ) {
		if ( ::g->playeringame[i] ) {
			::g->players[i].usedown = ::g->players[i].attackdown = true;	// don't do anything immediately
		}
	}

	G_DoLoadLevel (); 
	::g->gameaction = ga_nothing; 
	::g->viewactive = true; 

} 



//
// G_InitFromSavegame
// Can be called by the startup code or the menu task. 
//
void R_ExecuteSetViewSize (void);


void G_LoadGame (char* name) 
{ 
	strcpy (::g->savename, name); 
	::g->gameaction = ga_loadgame; 
} 
//GK: Fix for DOOM II modded saves checking
qboolean G_CheckSave(char* name) {
	strcpy(::g->savename, name);
	char	vcheck[256];
	M_ReadFile(::g->savename, &::g->savebuffer);
	::g->save_p = ::g->savebuffer + SAVESTRINGSIZE;
	memset(vcheck, 0, sizeof(vcheck));
	sprintf(vcheck, "version %i", VERSION);
	char tlab[256];
	strcpy(tlab,(char*)::g->save_p);
	//GK: Make sure it is either version 111 or version 111 files
	if (tlab[11] != ' ') {
		tlab [11] = '\0';
	}
	bool hm = false;
	char* clab = new char[19];
	std::vector<std::string>filelist;
	if (strcmp(tlab, vcheck)) {
		//GK: Welp forgeting that was causing most of the problems with save files
		int o = strlen(tlab) - 1;
		while (tlab[o] != ',') {
			tlab[o] = '\0';
			o--;
		}
		strncpy(clab, tlab, 18);
		clab[18] = '\0';
		sprintf(vcheck, "version %i files ", VERSION);
		bool ok = false;
		if (!strcmp(clab, vcheck)) {
			hm = true;
			const char * fnames = tlab + 18;
			char* file = strtok(strdup(fnames), ",");
			int sc = 0;
			while (file) {
				filelist.push_back(file);
				if (idStr::Icmpn(file, "wads", 4)) {
					sc++;
				}
				file = strtok(NULL, ",");
			}
			ok = W_CheckMods(sc, filelist);
		}
		if (!ok) {
			loadingGame = false;
			waitingForWipe = false;
			if (hm) {
				//GK:If the game does not using the mods of the save file show this message
				M_StartMessage("Missing Mod Files!\n\npress any button", NULL, false);
			}
			return false;				// bad version
		}
		else {
			return true;
		}
	}
	else {
		return true;
	}
}

qboolean G_DoLoadGame () 
{ 
	int		i; 
	int		a,b,c;
	char	vcheck[256]; 

	loadingGame = true;

	::g->gameaction = ga_nothing; 

	M_ReadFile (::g->savename, &::g->savebuffer); 

	waitingForWipe = true;

	// DHM - Nerve :: Clear possible net demo state
	::g->netdemo = false;
	::g->netgame = false;
	::g->deathmatch = false;
	::g->playeringame[1] = ::g->playeringame[2] = ::g->playeringame[3] = 0;
	::g->respawnparm = false;
	::g->fastparm = false;
	::g->nomonsters = false;
	::g->consoleplayer = 0;

	::g->save_p = ::g->savebuffer + SAVESTRINGSIZE;

	// skip the description field 
	memset (vcheck,0,sizeof(vcheck)); 
	sprintf (vcheck,"version %i",VERSION); 
	//GK:Check if the save file uses mods
	char tlab[256];
	strcpy(tlab, (char*)::g->save_p);
	if (tlab[11] != ' ') {
		tlab[11] = '\0';
	}
	bool hm = false;
	char* clab = new char[19];
	std::vector<std::string>filelist;
	if (strcmp (tlab, vcheck)) {
		//GK: Welp forgeting that was causing most of the problems with save files
		int o = strlen(tlab) - 1;
		while (tlab[o] != ',') {
			tlab[o] = '\0';
			o--;
		}
		strncpy(clab, tlab, 18);
		clab[18] = '\0';
		sprintf(vcheck, "version %i files ", VERSION);
		bool ok = false;
		if (!strcmp(clab, vcheck)) {
			hm = true;
			const char * fnames = tlab + 18;
			char* file = strtok(strdup(fnames), ",");
			int sc = 0;
			while (file) {
				filelist.push_back(file);
				if (idStr::Icmpn(file, "wads", 4)) {
					sc++;
				}
				file = strtok(NULL, ",");
			}
			ok = W_CheckMods(sc, filelist);
		}
		if (!ok) {
			loadingGame = false;
			waitingForWipe = false;
			if (hm) {
				//GK:If the game does not using the mods of the save file show this message
				M_StartMessage("Missing Mod Files!\n\npress any button", NULL, false);
			}
			return false;				// bad version
		}
	}
	//GK: In case the save file has no mods
	if (hm) {
		char* tla = new char[256];
		strcpy(tla, clab);
		for (uint mf = 0; mf < filelist.size(); mf++) {
			strcat(tla, filelist[mf].c_str());
			strcat(tla, ",");
		}
		::g->save_p += strlen(tla);
	}
	else {
		::g->save_p += strlen(tlab);
	}

	::g->gameskill = (skill_t)*::g->save_p++; 
	::g->gameepisode = *::g->save_p++; 
	::g->gamemission = *::g->save_p++;
	::g->gamemap = *::g->save_p++; 
	for (i=0 ; i<MAXPLAYERS ; i++) 
		::g->playeringame[i] = *::g->save_p++; 

	// load a base level 
	G_InitNew (::g->gameskill, ::g->gameepisode, ::g->gamemap ); 

	// get the times 
	a = *::g->save_p++; 
	b = *::g->save_p++; 
	c = *::g->save_p++; 
	::g->leveltime = (a<<16) + (b<<8) + c; 

	// dearchive all the modifications
	P_UnArchivePlayers (); 
	P_UnArchiveWorld (); 
	P_UnArchiveThinkers ();

	// specials are archived with thinkers
	//P_UnArchiveSpecials (); 

	if (*::g->save_p != 0x1d) 
		I_Error ("Bad savegame");

	if (::g->setsizeneeded)
		R_ExecuteSetViewSize ();

	// draw the pattern into the back screen
	R_FillBackScreen ();
	//GK: Make sure the status bar is showing properly the weapons that are owned and which one is used
	//after it load a save file
	for (int i2 = 0; i2 < NUMWEAPONS; i2++) {
		if (::g->players[::g->consoleplayer].weaponowned[i2]) {
			::g->weaponcond[i2] = 1;
		}
	}
	if (::g->players[::g->consoleplayer].readyweapon == wp_supershotgun) {
		::g->weaponcond[wp_shotgun] = 2;
	}
	else {
		::g->weaponcond[::g->players[::g->consoleplayer].readyweapon] = 2;
	}
	loadingGame = false;

	Z_Free(g->savebuffer);

	return true;
} 


//
// G_SaveGame
// Called by the menu task.
// Description is a 24 byte text string 
//
void
G_SaveGame
( int	slot,
 char*	description ) 
{ 
	::g->savegameslot = slot; 
	strcpy (::g->savedescription, description); 
	::g->sendsave = true; 
	::g->gameaction = ga_savegame;
} 

qboolean G_DoSaveGame (void) 
{ 
	char	name[100]; 
	char*	name2; 
	char	sname[10];
	char*	description; 
	int		length; 
	int		i; 
	qboolean	bResult = true;
	idStr localsavedir;

	if ( ::g->gamestate != GS_LEVEL ) {
		return false;
	}
	
	description = ::g->savedescription; 
	//GK: quicksaves are using diferent name in order to be detected more easy
	if (::g->savegameslot == 7) {
		sprintf(sname, "%s", QUICKSAVENAME);
	}
	else {
		sprintf(sname, "%s", SAVEGAMENAME);
	}


	//GK: Add save directories for Evilution and Plutonia expansions
	if (::g->gamemission == pack_custom && ::g->savedir != NULL) { //GK: Custom expansion related stuff
		localsavedir = ::g->savedir;
	}
	else {
		switch (DoomLib::idealExpansion) {
			case doom:
				localsavedir = "DOOM";
				break;
			case doom2:
				localsavedir = "DOOM2";
				break;
			case pack_nerve:
				localsavedir = "DOOM2_NRFTL";
				break;
			case pack_tnt:
				localsavedir = "DOOM2_TNT";
				break;
			case pack_plut:
				localsavedir = "DOOM2_PLUT";
				break;
			case pack_master:
				localsavedir = "DOOM2_MASTER";
				break;
		}
	}

	sprintf(name, "%s\\%s%d.dsg", localsavedir.c_str(), sname, ::g->savegameslot);

	::g->save_p = ::g->savebuffer = ::g->screens[1];

	memcpy (::g->save_p, description, SAVESTRINGSIZE); 
	::g->save_p += SAVESTRINGSIZE; 

	 
	//GK: if the game uses mods store their names on the save file header
	if (M_CheckParm("-file")) {
		name2 = new char[256];
		memset(name2, 0, strlen(name2));
		sprintf(name2, "version %i files ", VERSION);
		for (int f=1; f<20; f++) {
			if (wadfiles[f] != NULL) {
				char* fname = strtok(strdup(wadfiles[f]), "\\");
				while (fname) {
					char* tname = strtok(NULL, "\\");
					if (tname) {
						fname = tname;
					}
					else {
						break;
					}
				}
				strcat(name2, fname);
				strcat(name2, ",");
			}
			else {
				break;
			}
		}
	}
	else {
		//GK: allow unmodded saves
		name2 = new char[VERSIONSIZE];
		memset(name2, 0, strlen(name2));
		sprintf(name2, "version %i", VERSION);
	}
	memcpy (::g->save_p, name2, strlen(name2));
	::g->save_p += strlen(name2); 

	*::g->save_p++ = ::g->gameskill; 
	*::g->save_p++ = ::g->gameepisode; 
	*::g->save_p++ = ::g->gamemission;
	*::g->save_p++ = ::g->gamemap; 

	for (i=0 ; i<MAXPLAYERS ; i++) {
		*::g->save_p++ = ::g->playeringame[i];
	}

	*::g->save_p++ = ::g->leveltime>>16; 
	*::g->save_p++ = ::g->leveltime>>8; 
	*::g->save_p++ = ::g->leveltime; 

	P_ArchivePlayers (); 
	P_ArchiveWorld (); 
	P_ArchiveThinkers ();

	// specials are archived with thinkers
	//P_ArchiveSpecials (); 

	*::g->save_p++ = 0x1d;		// ::g->consistancy marker 

	length = ::g->save_p - ::g->savebuffer; 
//	if (length > SAVEGAMESIZE) 
//		I_Error ("Savegame buffer overrun");

	::g->savebufferSize = length;
	
	M_WriteFile (name, ::g->savebuffer, length); 

	::g->gameaction = ga_nothing; 
	::g->savedescription[0] = 0;		 

	// draw the pattern into the back screen
	R_FillBackScreen ();

	return bResult;
} 


//
// G_InitNew
// Can be called by the startup code or the menu task,
// ::g->consoleplayer, ::g->displayplayer, ::g->playeringame[] should be set. 
//

void
G_DeferedInitNew
( skill_t	skill,
 int		episode,
 int		map) 
{ 
	::g->d_skill = skill; 
	::g->d_episode = episode; 
	::g->d_map = map;

	//::g->d_map = 30;

	::g->gameaction = ga_newgame; 
} 


void G_DoNewGame (void) 
{
	::g->demoplayback = false; 
	::g->netdemo = false;
	::g->netgame = false;
	::g->deathmatch = false;
	::g->playeringame[1] = ::g->playeringame[2] = ::g->playeringame[3] = 0;
	::g->respawnparm = false;
	::g->fastparm = false;
	::g->nomonsters = false;
	::g->consoleplayer = 0;
	G_InitNew (::g->d_skill, ::g->d_episode, ::g->d_map ); 
	::g->gameaction = ga_nothing; 
} 

// The sky texture to be used instead of the F_SKY1 dummy.


void
G_InitNew
( skill_t	skill,
 int		episode,
 int		map
 ) 
{ 
	int i; 
	m_inDemoMode.SetBool( false );
	R_SetViewSize (::g->screenblocks, ::g->detailLevel);

	if (::g->paused) 
	{ 
		::g->paused = false; 
		S_ResumeSound (); 
	} 

	if (skill > sk_nightmare) 
		skill = sk_nightmare;

	// This was quite messy with SPECIAL and commented parts.
	// Supposedly hacks to make the latest edition work.
	// It might not work properly.
	if (episode < 1)
		episode = 1; 

	if ( ::g->gamemode == retail )
	{
		//GK: No limits
		if (::g->gamemission != pack_custom)
			if (episode > 4)
				episode = 4;
	}
	else if ( ::g->gamemode == shareware )
	{
		if (episode > 1) 
			episode = 1;	// only start episode 1 on shareware
	}  
	else
	{
		if (episode > 3)
			episode = 3;
	}

	if (map < 1) 
		map = 1;

	if (skill == sk_nightmare || ::g->respawnparm )
		::g->respawnmonsters = true;
	else
		::g->respawnmonsters = false;

	//GK: Use original source code for the fast parameter
	if (::g->fastparm || (skill == sk_nightmare && ::g->gameskill != sk_nightmare))
	{
		for (i = S_SARG_RUN1; i <= S_SARG_PAIN2; i++)
			::g->states[i].tics >>= 1;
		mobjinfo[MT_BRUISERSHOT].speed = 20 * FRACUNIT;
		mobjinfo[MT_HEADSHOT].speed = 20 * FRACUNIT;
		mobjinfo[MT_TROOPSHOT].speed = 20 * FRACUNIT;
	}
	else if (skill != sk_nightmare && ::g->gameskill == sk_nightmare)
	{
		for (i = S_SARG_RUN1; i <= S_SARG_PAIN2; i++)
			::g->states[i].tics <<= 1;
		mobjinfo[MT_BRUISERSHOT].speed = 15 * FRACUNIT;
		mobjinfo[MT_HEADSHOT].speed = 10 * FRACUNIT;
		mobjinfo[MT_TROOPSHOT].speed = 10 * FRACUNIT;
	}
	// force ::g->players to be initialized upon first level load         
	for (i=0 ; i<MAXPLAYERS ; i++) 
		::g->players[i].playerstate = PST_REBORN; 

	::g->usergame = true;                // will be set false if a demo 
	::g->paused = false; 
	::g->demoplayback = false;
	::g->advancedemo = false;
	::g->automapactive = false; 
	::g->viewactive = true; 
	::g->gameepisode = episode; 
	//::g->gamemission = expansion->pack_type;
	::g->gamemap = map; 
	::g->gameskill = skill; 
	//GK: When setting the gamemap set and the map for the custom expansion
	if (::g->gamemission == pack_custom) {
		setMapNum();
	}

	::g->viewactive = true;

	// set the sky map for the episode
	if ( ::g->gamemode == commercial)
	{
		::g->skytexture = R_TextureNumForName ("SKY3");

		if (::g->gamemap < 12) {
			::g->skytexture = R_TextureNumForName ("SKY1");
		}
		else if (::g->gamemap < 21) {
			::g->skytexture = R_TextureNumForName ("SKY2");
		}
	}
	else {
		switch (episode) 
		{ 
		case 1: 
			::g->skytexture = R_TextureNumForName ("SKY1"); 
			break; 
		case 2: 
			::g->skytexture = R_TextureNumForName ("SKY2"); 
			break; 
		case 3: 
			::g->skytexture = R_TextureNumForName ("SKY3"); 
			break; 
		case 4:	// Special Edition sky
			::g->skytexture = R_TextureNumForName ("SKY4");
			break;
		default:
			::g->skytexture = R_TextureNumForName ("SKY1");
			break;
		}
	}

	G_DoLoadLevel( );
} 


//
// DEMO RECORDING 
// 

void G_ReadDemoTiccmd (ticcmd_t* cmd) 
{ 
	if (*::g->demo_p == DEMOMARKER) 
	{
		// end of demo data stream 
		G_CheckDemoStatus (); 
		return; 
	}

	cmd->forwardmove = ((signed char)*::g->demo_p++);
	cmd->sidemove = ((signed char)*::g->demo_p++);

	if ( demoversion == VERSION ) {
		short *temp = (short *)(::g->demo_p);
		cmd->angleturn = *temp;
		::g->demo_p += 2;
	}
	else {
		// DHM - Nerve :: Old format
		cmd->angleturn = ((unsigned char)*::g->demo_p++)<<8;
	}

	cmd->buttons = (unsigned char)*::g->demo_p++;

#ifdef DEBUG_DEMOS
	// TESTING
	if ( demoDebugOn ) {
		testprndindex = (unsigned char)*::g->demo_p++;
	}
#endif
} 


void G_WriteDemoTiccmd (ticcmd_t* cmd) 
{ 
	*::g->demo_p++ = cmd->forwardmove; 
	*::g->demo_p++ = cmd->sidemove;

	// NEW VERSION
	short *temp = (short *)(::g->demo_p);
	*temp = cmd->angleturn;
	::g->demo_p += 2;

	// OLD VERSION
	//*::g->demo_p++ = (cmd->angleturn+128)>>8; 

	*::g->demo_p++ = cmd->buttons;

	int cmdSize = 5;

#ifdef DEBUG_DEMOS_WRITE
	// TESTING
	*::g->demo_p++ = ::g->prndindex;
	cmdSize++;
#endif

	::g->demo_p -= cmdSize; 
	if (::g->demo_p > ::g->demoend - (cmdSize * 4))
	{
		// no more space 
		G_CheckDemoStatus (); 
		return; 
	} 

	G_ReadDemoTiccmd (cmd);         // make SURE it is exactly the same 
} 



//
// G_RecordDemo 
// 
void G_RecordDemo (char* name) 
{ 
	//::g->usergame = false; 
	strcpy(::g->demoname, "DEMO\\");
	strcat( ::g->demoname, name ); 
	strcat( ::g->demoname, ".lmp" );

	::g->demobuffer = new byte[ MAXDEMOSIZE ];
	::g->demoend = ::g->demobuffer + MAXDEMOSIZE;

	demoversion = VERSION;
	::g->demorecording = true;
	ogHz = com_engineHz_latched;
	for (int i = 0; i < 3; i++) {
		ogtr[i] = ::g->ticrate[i];
	}
} 
 
 
void G_BeginRecording (void) 
{ 
	int             i;

	::g->demo_p = ::g->demobuffer;

#ifdef DEBUG_DEMOS
#ifdef DEBUG_DEMOS_WRITE
	demoDebugOn = true;
	*::g->demo_p++ = VERSION + 1;
#else
	*::g->demo_p++ = VERSION;
#endif
#endif
	*::g->demo_p++ = ::g->gameskill;
	*::g->demo_p++ = ::g->gameepisode;
	*::g->demo_p++ = ::g->gamemission == pack_custom ? DoomLib::GetCurrentExpansion()->pack_type : ::g->gamemission;
	*::g->demo_p++ = ::g->gamemap;
	*::g->demo_p++ = ::g->deathmatch;
	*::g->demo_p++ = ::g->respawnparm;
	*::g->demo_p++ = ::g->fastparm;
	*::g->demo_p++ = ::g->nomonsters;
	*::g->demo_p++ = ::g->consoleplayer;

	for ( i=0 ; i<MAXPLAYERS ; i++ ) {
		*::g->demo_p++ = ::g->playeringame[i];
	}

	for ( i=0 ; i<MAXPLAYERS ; i++ ) {
		// Archive player state to demo
		if ( ::g->playeringame[i] ) {
			if (::g->playeringame[i]) {
				common->Printf("%d\n", i + 1);
			}
		    int* dest = (int *)::g->demo_p;
			*dest++ = ::g->players[i].health;
			*dest++ = ::g->players[i].armorpoints;
			*dest++ = ::g->players[i].armortype;
			*dest++ = ::g->players[i].readyweapon;
			for ( int j = 0; j < NUMWEAPONS; j++ ) {
				*dest++ = ::g->players[i].weaponowned[j];
			}
			for ( int j = 0; j < NUMAMMO; j++ ) {
				*dest++ = ::g->players[i].ammo[j];
				*dest++ = ::g->players[i].maxammo[j];
			}
			::g->demo_p = (byte *)dest;
		}
	}
}

//
// G_PlayDemo 
//
void G_DeferedPlayDemo (const char* name)
{ 
	if ( ::g->useDemo) {
		::g->defdemoname = (char*)name;
		::g->gameaction = ga_playdemo;
	}
} 

void G_DoPlayDemo (void) 
{ 
	skill_t skill; 
	int             i, episode, map, mission; 

	::g->gameaction = ga_nothing;

	// TODO: Networking
#if ID_ENABLE_DOOM_CLASSIC_NETWORKING
	if ( /*gameLocal->IsSplitscreen() &&*/ DoomLib::GetPlayer() > 0 ) {
		return;
	}
#endif
	

	// DEMO Testing
	bool useOriginalDemo = true;

	if ( useOriginalDemo ) {
		int demolump = W_GetNumForName( ::g->defdemoname );
		int demosize = W_LumpLength( demolump );

		::g->demobuffer = ::g->demo_p = new byte[ demosize ];
		W_ReadLump( demolump, ::g->demobuffer );
	}

	// DHM - Nerve :: We support old and new demo versions
	demoversion = *::g->demo_p++;

	skill = (skill_t)*::g->demo_p++; 
	episode = *::g->demo_p++;
	if ( demoversion == VERSION ) {
		mission =  *::g->demo_p++;
	}
	else {
		mission = 0;
	}
	map = *::g->demo_p++; 
	::g->deathmatch = *::g->demo_p++;
	::g->respawnparm = *::g->demo_p++;
	::g->fastparm = *::g->demo_p++;
	::g->nomonsters = *::g->demo_p++;
	::g->consoleplayer = *::g->demo_p++;

	for ( i=0 ; i<MAXPLAYERS ; i++ ) {
		::g->playeringame[i] = *::g->demo_p++;
	}

	::g->netgame = false;
	::g->netdemo = false; 
	if (::g->playeringame[1]) 
	{ 
		::g->netgame = true;
		::g->netdemo = true; 
	}

	// don't spend a lot of time in loadlevel 
	::g->precache = false;
	if (mission > doom2 && mission != ::g->gamemission)
		DoomLib::SetCurrentExpansion(mission);

	G_InitNew (skill, episode, map ); 
	R_SetViewSize (::g->screenblocks + 1, ::g->detailLevel);
	::g->demostarttic = ::g->gametic;
	m_inDemoMode.SetBool( true );
	::g->gamestate = GS_DEMOLEVEL;

	// JAF - Dont show messages when in Demo Mode. ::g->showMessages = false;
	::g->precache = true; 

	// DHM - Nerve :: We now read in the player state from the demo
	if ( demoversion == VERSION ) {
		for ( i=0 ; i<MAXPLAYERS ; i++ ) {
			if ( ::g->playeringame[i] ) {
				int* src = (int *)::g->demo_p;
				::g->players[i].health = *src++;
				::g->players[i].mo->health = ::g->players[i].health;
				::g->players[i].armorpoints = *src++;
				::g->players[i].armortype = *src++;
				::g->players[i].readyweapon = (weapontype_t)*src++;
				::g->weaponcond[::g->players[i].readyweapon] = 2; //GK: Why not -\('')/-
				for ( int j = 0; j < NUMWEAPONS; j++ ) {
					int owned = *src++;
					if (::g->weaponcond[j] != 2) {
						::g->players[i].weaponowned[j] = owned;
					}
				}
				for ( int j = 0; j < NUMAMMO; j++ ) {
					::g->players[i].ammo[j] = *src++;
					::g->players[i].maxammo[j] = *src++;
				}
				::g->demo_p = (byte *)src;

				P_SetupPsprites( &::g->players[i] );
			}
		}
	}

	::g->usergame = false;
	::g->demoplayback = true;
	ogHz = com_engineHz_latched;
	for (int i3 = 0; i3 < 3; i3++) {
		ogtr[i3] = ::g->ticrate[i3];
	}
} 

//
// G_TimeDemo 
//
void G_TimeDemo (const char* name)
{ 	 
	if (::g->useDemo) {
		::g->nodrawers = M_CheckParm("-nodraw");
		::g->noblit = M_CheckParm("-noblit");
		::g->timingdemo = true;
		//::g->singletics = true;

		::g->defdemoname = (char*)name;
		::g->gameaction = ga_playdemo;
	}
} 


/* 
=================== 
= 
= G_CheckDemoStatus 
= 
= Called after a death or level completion to allow demos to be cleaned up 
= Returns true if a new demo loop action will take place 
=================== 
*/ 

qboolean G_CheckDemoStatus (void) 
{
	if (::g->demoplayback) 
	{ 
		delete ::g->demobuffer;
		::g->demobuffer = NULL;
		::g->demo_p = NULL;
		::g->demoend = NULL;

		if (::g->timingdemo) {
			char statname[32];
			strcpy(statname, "DEMO\\");
			strcat(statname, ::g->defdemoname);
			strcat(statname, ".txt");
			idStr buff;
			int demotics = ::g->gametic - ::g->demostarttic;
			sprintf(buff, "GameTics: %d\nLevel Tics: %d\nLevel Time: %d\nFPS: %d\0", ::g->gametic, demotics, ::g->normaltime, TICRATE);
			M_WriteFile(statname, strdup(buff.c_str()), buff.Length());
			::g->timingdemo = false;
		}

		::g->demoplayback = false; 
		::g->netdemo = false;
		::g->netgame = false;
		::g->deathmatch = false;
		::g->playeringame[1] = ::g->playeringame[2] = ::g->playeringame[3] = 0;
		::g->respawnparm = false;
		::g->fastparm = false;
		::g->nomonsters = false;
		::g->consoleplayer = 0;
		D_AdvanceDemo (); 
		return true; 
	} 

	
	if (::g->demorecording) { 
		*::g->demo_p++ = DEMOMARKER; 
		int length = ::g->demo_p - ::g->demobuffer;
		M_WriteFile(::g->demoname, ::g->demobuffer, length);
		/*if ( ::g->leveltime > (TICRATE * 9) ) {
			gameLocal->DoomStoreDemoBuffer( gameLocal->GetPortForPlayer( DoomLib::GetPlayer() ), ::g->demobuffer, ::g->demo_p - ::g->demobuffer );
		}*/
		
		delete ::g->demobuffer;
		::g->demobuffer = NULL;
		::g->demo_p = NULL;
		::g->demoend = NULL;

		::g->demorecording = false; 
    }
	

	return false; 
} 




