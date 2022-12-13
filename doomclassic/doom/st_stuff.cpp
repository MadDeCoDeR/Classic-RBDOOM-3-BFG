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


#include <stdio.h>

#include "i_system.h"
#include "i_video.h"
#include "z_zone.h"
#include "m_random.h"
#include "w_wad.h"

#include "doomdef.h"

#include "g_game.h"

#include "st_stuff.h"
#include "st_lib.h"
#include "r_local.h"

#include "p_local.h"
#include "p_inter.h"

#include "am_map.h"
#include "m_cheat.h"

#include "s_sound.h"

// Needs access to LFB.
#include "v_video.h"

// State.
#include "doomstat.h"

// Data.
#include "dstrings.h"
#include "sounds.h"

//
// STATUS BAR DATA
//


// Palette indices.
// For damage/bonus red-/gold-shifts
// Radiation suit, green shift.

// N/256*100% probability
//  that the normal face state will change

// For Responder

// Location of status bar


// Should be set to patch width
//  for tall numbers later on

// Number of status ::g->faces.









// Location and size of statistics,
//  justified according to widget type.
// Problem is, within which space? STbar? Screen?
// Note: this could be read in by a lump.
//       Problem is, is the stuff rendered
//       into a buffer,
//       or into the frame buffer?

// AMMO number pos.

// HEALTH number pos.

// Weapon pos.

// Frags pos.

// ARMOR number pos.

// Key icon positions.

// Ammunition counter.

// Indicate maximum ammunition.
// Only needed because backpack exists.

// pistol

// shotgun

// chain gun

// missile launcher

// plasma gun

// bfg

// WPNS title

// DETH title

//Incoming messages window location
//UNUSED
// #define ST_MSGTEXTX	   (::g->viewwindowx)
// #define ST_MSGTEXTY	   (::g->viewwindowy+::g->viewheight-18)
// Dimensions given in characters.
// Or shall I say, in lines?


// Width, in characters again.
// Height, in ::g->lines. 





// main player in game

// ST_Start() has just been called

// used to execute ST_Init() only once

// lump number for PLAYPAL

// used for timing

// used for making messages go away

// used when in chat 

// States for the intermission


// whether in automap or first-person

// whether left-side main status bar is active

// whether status bar chat is active

// value of ::g->st_chat before message popped up

// whether chat window has the cursor on

// !::g->deathmatch

// !::g->deathmatch && ::g->st_statusbaron

// !::g->deathmatch

// main bar left

// 0-9, tall numbers

// tall % sign

// 0-9, short, yellow (,different!) numbers

// 3 key-cards, 3 skulls

// face status patches

// face background

// main bar right

// weapon ownership patches

// ready-weapon widget

// in ::g->deathmatch only, summary of frags stats

// health widget

// ::g->arms background


// weapon ownership widgets

// face status widget

// keycard widgets

// armor widget

// ammo widgets

// max ammo widgets



// number of frags so far in ::g->deathmatch

// used to use appopriately pained face

// used for evil grin

// count until face changes

// current face index, used by ::g->w_faces

// holds key-type for each key box on bar

// a random number per tick



// Massive bunches of cheat shit
//  to keep it from being easy to figure them out.
// Yeah, right...
//GK: re-write char sequences
//GK: Using VS Debugger I find out which chars are recognized from the game and they are literraly different from the original chars
const unsigned char	cheat_mus_seq[] =
{
	'\x17',' ','2','\x16','\x1f',0xff
	//0xb2, 0x26, 0xb6, 0xae, 0xea, 1, 0, 0, 0xff
};

const unsigned char	cheat_choppers_seq[] =
{
	'\x17',' ','.','#','\x18','\x19','\x19','\x12','\x13','\x1f',0xff
	//0xb2, 0x26, 0xe2, 0x32, 0xf6, 0x2a, 0x2a, 0xa6, 0x6a, 0xea, 0xff // id...
};

const unsigned char	cheat_god_seq[] =
{
	'\x17',' ',' ','\x10',' ',0xff
	//0xb2, 0x26, 0x26, 0xaa, 0x26, 0xff  // iddqd
};

const unsigned char	cheat_ammo_seq[] =
{
	'\x17',' ','%','!','\x1e',0xff
	//0xb2, 0x26, 0xf2, 0x66, 0xa2, 0xff	// idkfa
};

const unsigned char	cheat_ammonokey_seq[] =
{
	'\x17',' ','!','\x1e',0xff
	//0xb2, 0x26, 0x66, 0xa2, 0xff	// idfa
};


// Smashing Pumpkins Into Samml Piles Of Putried Debris. 
const unsigned char	cheat_noclip_seq[] =
{
	'\x17',' ','\x1f','\x19','\x17','\x1f','\x19','\x18','\x19',' ',0xff
	//0xb2, 0x26, 0xea, 0x2a, 0xb2,	// idspispopd
	//0xea, 0x2a, 0xf6, 0x2a, 0x26, 0xff
};

//
const unsigned char	cheat_commercial_noclip_seq[] =
{
	'\x17',' ','.','&','\x17','\x19',0xff
	//0xb2, 0x26, 0xe2, 0x36, 0xb2, 0x2a, 0xff	// idclip
};



const unsigned char	cheat_powerup_seq[7][10] =
{
	{ '\x17',' ','0','\x12','#','\x18','&',' ','/',0xff },
	//{ 0xb2, 0x26, 0x62, 0xa6, 0x32, 0xf6, 0x36, 0x26, 0x6e, 0xff }, 	// beholdv
	{ '\x17',' ','0','\x12','#','\x18','&',' ','\x1f',0xff },
	//{ 0xb2, 0x26, 0x62, 0xa6, 0x32, 0xf6, 0x36, 0x26, 0xea, 0xff }, 	// beholds
	{ '\x17',' ','0','\x12','#','\x18','&',' ','\x17',0xff },
	//{ 0xb2, 0x26, 0x62, 0xa6, 0x32, 0xf6, 0x36, 0x26, 0xb2, 0xff }, 	// beholdi
	{ '\x17',' ','0','\x12','#','\x18','&',' ','\x13',0xff },
	//{ 0xb2, 0x26, 0x62, 0xa6, 0x32, 0xf6, 0x36, 0x26, 0x6a, 0xff }, 	// beholdr
	{ '\x17',' ','0','\x12','#','\x18','&',' ','\x1e',0xff },
	//{ 0xb2, 0x26, 0x62, 0xa6, 0x32, 0xf6, 0x36, 0x26, 0xa2, 0xff }, 	// beholda
	{ '\x17',' ','0','\x12','#','\x18','&',' ','&',0xff },
	//{ 0xb2, 0x26, 0x62, 0xa6, 0x32, 0xf6, 0x36, 0x26, 0x36, 0xff }, 	// beholdl
	{ '\x17',' ','0','\x12','#','\x18','&',' ',0xff },
	//{ 0xb2, 0x26, 0x62, 0xa6, 0x32, 0xf6, 0x36, 0x26, 0xff }		// behold
};


const unsigned char	cheat_clev_seq[] =
{
	'\x17',' ','.','&','\x12','/',0xff
	//0xb2, 0x26,  0xe2, 0x36, 0xa6, 0x6e, 1, 0, 0, 0xff	// idclev
};


// my position cheat
const unsigned char	cheat_mypos_seq[] =
{
	'\x17',' ','2','\x15','\x19','\x18','\x1f',0xff
	//0xb2, 0x26, 0xb6, 0xba, 0x2a, 0xf6, 0xea, 0xff	// idmypos
};
//GK begin
char buf[3];
//GK End

// Now what?
cheatseq_t	cheat_mus = cheatseq_t( cheat_mus_seq, 0 );
cheatseq_t	cheat_god = cheatseq_t( cheat_god_seq, 0 );
cheatseq_t	cheat_ammo = cheatseq_t( cheat_ammo_seq, 0 );
cheatseq_t	cheat_ammonokey = cheatseq_t( cheat_ammonokey_seq, 0 );
cheatseq_t	cheat_noclip = cheatseq_t( cheat_noclip_seq, 0 );
cheatseq_t	cheat_commercial_noclip = cheatseq_t( cheat_commercial_noclip_seq, 0 );

// ALAN

// DISABLED cheatseq_t( cheat_powerup_seq[0], 0 ), cheatseq_t( cheat_powerup_seq[1], 0 ),
// cheatseq_t( cheat_powerup_seq[2], 0 ),
// DISABLED cheatseq_t( cheat_powerup_seq[3], 0 ), 
// cheatseq_t( cheat_powerup_seq[4], 0 ),cheatseq_t( cheat_powerup_seq[5], 0 ),cheatseq_t( cheat_powerup_seq[6], 0 ) };

cheatseq_t	cheat_choppers = cheatseq_t( cheat_choppers_seq, 0 );
cheatseq_t	cheat_clev = cheatseq_t( cheat_clev_seq, 0 );
cheatseq_t	cheat_mypos = cheatseq_t( cheat_mypos_seq, 0 );


// GK:For DeHackeD Text Editor
/*const*/ extern char*	mapnames[];


idCVar cl_HUD("cl_HUD", "0", CVAR_ARCHIVE | CVAR_BOOL, "Enable/Disable Heads up Display on Classic DOOM (if the status bar is hidden)");
extern idCVar in_photomode;

//
// STATUS BAR CODE
//
void ST_Stop(void);

void ST_refreshBackground(void)
{
	if (!in_photomode.GetBool()) {
		if (::g->st_statusbaron)
		{
			short widthoffset = 0;
			if (::g->ASPECT_IMAGE_SCALER > GLOBAL_IMAGE_SCALER) {
				V_DrawPatch(ST_X, 0, BG, ::g->mapt, true);
				widthoffset += ::g->mapt->width;
				V_DrawPatch(ST_X + ST_WIDTH + widthoffset, 0, BG, ::g->spwr, true);
				widthoffset += ::g->spwr->width;
			}

			V_DrawPatch(ST_X + ::g->ASPECT_POS_OFFSET, 0, BG, ::g->sbar, true);


			if (::g->netgame)
				V_DrawPatch(ST_FX + ::g->ASPECT_POS_OFFSET, 0, BG, ::g->faceback, true);

			V_CopyRect(ST_X, 0, BG, ST_WIDTH + widthoffset, ST_HEIGHT, ST_X, ST_Y, FG, true);
		}
		else if (cl_HUD.GetBool()) {
			V_DrawPatch(ST_X, ST_ARMORY, FG, ::g->hear, true);

			V_DrawPatch(ST_AMMO0X + (::g->ASPECT_POS_OFFSET - 7) - ((4 - ::g->ASPECT_IMAGE_SCALER) * 50), ST_ARMORY + 4, FG, ::g->fullarms, true);

			V_DrawPatch(ST_AMMO0X + (::g->ASPECT_POS_OFFSET + 36) - ((4 - ::g->ASPECT_IMAGE_SCALER) * 50), ST_ARMORY + 8, FG, ::g->fullslash, true);

			V_DrawPatch(ST_X, ORIGINAL_HEIGHT - 11, FG, ::g->fullkeys, true);

			int powerX = (::g->SCREENWIDTH / GLOBAL_IMAGE_SCALER) - 30;

			V_DrawPatch(powerX, (ORIGINAL_HEIGHT / 2) - 5, FG, ::g->fullpwr, true);

			V_DrawPatch(powerX - 10, 16, FG, ::g->fulltime, true);

			if (::g->deathmatch) {
				V_DrawPatch((::g->SCREENWIDTH / 2) - 30, (SCREENHEIGHT / GLOBAL_IMAGE_SCALER) - 20, FG, ::g->fullfrag, true);
			}
			//V_CopyRect(ST_X, 0, BG, (::g->SCREENWIDTH / GLOBAL_IMAGE_SCALER), (SCREENHEIGHT / GLOBAL_IMAGE_SCALER), ST_X, 0, FG, true);
		}
	}

}


// Respond to keyboard input ::g->events,
//  intercept cheats.
qboolean
ST_Responder (event_t* ev)
{
	int		i;

	// Filter automap on/off.
	if (ev->type == ev_keyup
		&& ((ev->data1 & 0xffff0000) == AM_MSGHEADER))
	{
		switch(ev->data1)
		{
		case AM_MSGENTERED:
			::g->st_gamestate = AutomapState;
			::g->st_firsttime = true;
			break;

		case AM_MSGEXITED:
			//	I_PrintfE( "AM exited\n");
			::g->st_gamestate = FirstPersonState;
			break;
		}
	}

	// if a user keypress...
	else if (ev->type == ev_keydown)
	{
		if (!::g->netgame)
		{
			// b. - enabled for more debug fun.
			// if (::g->gameskill != sk_nightmare) {

			//GK store cheat sequence
			//GK: Using an improved cheat system that no longer relies on time limits
					::g->cheat[::g->cheatind] = ev->data1;
					::g->cheatind++;
					::g->markfordelete = 0;
			// 'dqd' cheat for toggleable god mode
			if (cht_CheckCheat(cheat_god_seq, ::g->cheat, ::g->cheatind, ::g->markfordelete))
			{
				::g->markfordelete = 0;
				::g->plyr->cheats ^= CF_GODMODE;
				if (::g->plyr->cheats & CF_GODMODE)
				{
					if (::g->plyr->mo)
						::g->plyr->mo->health = ::g->ghealth;

					::g->plyr->health = ::g->ghealth;
					::g->plyr->message = STSTR_DQDON;
				}
				else
					::g->plyr->message = STSTR_DQDOFF;
			}
			// 'fa' cheat for killer fucking arsenal
			else if (cht_CheckCheat(cheat_ammonokey_seq, ::g->cheat, ::g->cheatind, ::g->markfordelete))
			{
				::g->markfordelete = 0;
				::g->plyr->armorpoints = ::g->farmor;
				::g->plyr->armortype = ::g->fart;

				for (i = 0; i < NUMWEAPONS; i++) {
					::g->plyr->weaponowned[i] = true;
					if (::g->weaponcond[i] != 2) { //GK: Everytime you get a weapon record that
						::g->weaponcond[i] = 1;
					}
				}

				for (i = 0; i<NUMAMMO; i++)
					::g->plyr->ammo[i] = ::g->plyr->maxammo[i];

				::g->plyr->message = STSTR_FAADDED;
			}
			// 'kfa' cheat for key full ammo
			else if (cht_CheckCheat(cheat_ammo_seq, ::g->cheat, ::g->cheatind, ::g->markfordelete))
			{
				::g->markfordelete = 0;
				::g->plyr->armorpoints = ::g->kfarmor;
				::g->plyr->armortype = ::g->kfart;
				//GK: Give the Backpack with the kfa cheat
				if (!::g->plyr->backpack) { //GK: That was something I forgot to look for
					for (i = 0; i < NUMAMMO; i++) {
						::g->plyr->maxammo[i] *= 2;
					}
					::g->plyr->backpack = true;
				}
				//GK: End
				for (i = 0; i < NUMWEAPONS; i++) {
					::g->plyr->weaponowned[i] = true;
					if (::g->weaponcond[i] != 2) { //GK: Everytime you get a weapon record that
						::g->weaponcond[i] = 1;
					}
				}

				for (i = 0; i<NUMAMMO; i++)
					::g->plyr->ammo[i] = ::g->plyr->maxammo[i];

				for (i = 0; i<NUMCARDS; i++)
					::g->plyr->cards[i] = true;

				::g->plyr->message = STSTR_KFAADDED;
			}
			// 'mus' cheat for changing music
			else if (cht_CheckCheat(cheat_mus_seq, ::g->cheat, ::g->cheatind, ::g->markfordelete, true))
			{
				::g->markfordelete = 0;
				//char	buf[3];
				int		musnum;
				for (int o = 0; o < 3; o++) {
					if (buf[o] == 11) { //0=11 so set it to 1
						buf[o] = 1;
					}
				}

				::g->plyr->message = STSTR_MUS;
				//cht_GetParam(&cheat_mus, buf);

				if (::g->gamemode == commercial)
				{
					//GK: Buffers are geting number + 1 (except 0 which equals 11)
					musnum = mus_runnin + (buf[0] - 1) * 10 + (buf[1] - 1) - 1;

					if (((buf[0] - 1) * 10 + buf[1] - 1) > 35)
						::g->plyr->message = STSTR_NOMUS;
					else
						S_ChangeMusic(musnum, 1);
				}
				else
				{
					musnum = mus_e1m1 + (buf[0] - 2) * 9 + (buf[1] - 2);

					if (((buf[0] - 2) * 9 + buf[1] - 2) > 31)
						::g->plyr->message = STSTR_NOMUS;
					else
						S_ChangeMusic(musnum, 1);
				}
			}
			// Simplified, accepting both "noclip" and "idspispopd".
			// no clipping mode cheat
			else if (cht_CheckCheat(cheat_noclip_seq, ::g->cheat, ::g->cheatind, ::g->markfordelete)
				|| cht_CheckCheat(cheat_commercial_noclip_seq, ::g->cheat, ::g->cheatind, ::g->markfordelete))
			{
				::g->markfordelete = 0;
				::g->plyr->cheats ^= CF_NOCLIP;

				if (::g->plyr->cheats & CF_NOCLIP)
					::g->plyr->message = STSTR_NCON;
				else
					::g->plyr->message = STSTR_NCOFF;
			}
			// 'behold?' power-up cheats
			for (i = 0; i<6; i++)
			{
				if (cht_CheckCheat(cheat_powerup_seq[i], ::g->cheat, ::g->cheatind, ::g->markfordelete))
				{
					::g->markfordelete = 0;
					if (!::g->plyr->powers[i])
						P_GivePower(::g->plyr, i);
					else if (i != pw_strength)
						::g->plyr->powers[i] = 1;
					else
						::g->plyr->powers[i] = 0;

					::g->plyr->message = STSTR_BEHOLDX;
				}
			}

			// 'behold' power-up menu
			if (cht_CheckCheat(cheat_powerup_seq[6], ::g->cheat, ::g->cheatind, ::g->markfordelete))
			{
				::g->markfordelete = 0;
				::g->plyr->message = STSTR_BEHOLD;
			}
			// 'choppers' invulnerability & chainsaw
			else if (cht_CheckCheat(cheat_choppers_seq, ::g->cheat, ::g->cheatind, ::g->markfordelete))
			{
				::g->markfordelete = 0;
				::g->plyr->weaponowned[wp_chainsaw] = true;
				if (::g->weaponcond[wp_chainsaw] != 2) { //GK: Everytime you get a weapon record that
					::g->weaponcond[wp_chainsaw] = 1;
				}
				::g->plyr->powers[pw_invulnerability] = true;
				::g->plyr->message = STSTR_CHOPPERS;
			}
			// 'mypos' for player position
			else if (cht_CheckCheat(cheat_mypos_seq, ::g->cheat, ::g->cheatind, ::g->markfordelete))
			{
				::g->markfordelete = 0;
				static char	buf[ST_MSGWIDTH];
				sprintf(buf, "ang=0x%x;x,y=(0x%x,0x%x)",
					::g->players[::g->consoleplayer].mo->angle,
					::g->players[::g->consoleplayer].mo->x,
					::g->players[::g->consoleplayer].mo->y);
				::g->plyr->message = buf;
			}
		}

		// 'clev' change-level cheat
		// ALAN NETWORKING
		if (cht_CheckCheat(cheat_clev_seq, ::g->cheat, ::g->cheatind, ::g->markfordelete, true)) // cht_CheckCheat(&cheat_clev, ev->data1))
		{
			::g->markfordelete = 0;
			//char		buf[3];
			int		epsd;
			int		map;
			for (int o = 0; o < 3; o++) {
				if (buf[o] == 11) { //0=11 so set it to 1
					buf[o] = 1;
				}
			}
			//cht_GetParam(&cheat_clev, buf);
			//for (int i = 0; i < 3; i++) buf[i] = ev->data1;

			if (::g->gamemode == commercial)
			{
				epsd = 0;
				//GK: Buffers are geting number + 1 (except 0 which equals 11)
				map = (buf[0] - 1) * 10 + (buf[1] - 1);//(buf[0] - '0')*10 + buf[1] - '0';

			}
			else
			{
				epsd = buf[0] - 1;
				map = buf[1] - 1;
			}

			// Catch invalid maps.
			if ((::g->gamemode == retail)
				&& (epsd < 1))
				return false;

			if (map < 1)
				return false;
			//GK: Handles cases where you put map number greater than the available
			if (::g->gamemode == commercial) {
				if (::g->gamemission == pack_nerve) {
					if (map > 9)
						return false;
				}
				if (::g->gamemission == pack_master) {
					if (map > 21)
						return false;
				}

				if (map > 33) {
					return false;
				}else if (!::g->isbfg && map > 32) {
					return false;
				}
			}

			if (::g->gamemission == pack_custom) {//GK:Custom expansion related stuff
				if (::g->gamemode == commercial) {
					if (map > ::g->mapmax)
						return false;
					while (!::g->maps[map - 1].lumpname) {
						map++;
					}
					::g->prevmap = map;
				}
				else
				{
					if (epsd > (int)::g->clusters.size())
						return false;
					if (::g->clusters[epsd - 1].startmap != ::g->clusters[epsd - 1].startmap)
					{
						if (map <= 0 || ::g->clusters[::g->startepisode - 1].startmap - 1 + map > ::g->clusters[::g->startepisode - 1].endmap + 1)
							return false;
					}
				}
			}

			// Ohmygod - this is not going to work.
			if ((::g->gamemode == retail && ::g->gamemission == doom)
				&& ((epsd > 4) || (map > 9)))
				return false;

			if ((::g->gamemode == registered)
				&& ((epsd > 3) || (map > 9)))
				return false;

			if ((::g->gamemode == shareware)
				&& ((epsd > 1) || (map > 9)))
				return false;

			if ((::g->gamemode == commercial && ::g->gamemission == doom2)
				&& ((epsd > 1) || (map > 34)))
				return false;

			// So be it.
			::g->plyr->message = STSTR_CLEV;
			G_DeferedInitNew(::g->gameskill, epsd, map);
		}
		//GK end
		if (!::g->markfordelete || ::g->cheatind>=14) {
			for (int u = 0; u < 14; u++) {
				::g->cheat[u] = '\0';
			}
			::g->cheatind = 0;
		}
	}
	return false;
}



int ST_calcPainOffset(void)
{
	int		health;

	health = ::g->plyr->health > 100 ? 100 : ::g->plyr->health;

	if (health != ::g->oldhealth)
	{
		::g->lastcalc = ST_FACESTRIDE * (((100 - health) * ST_NUMPAINFACES) / 101);
		::g->oldhealth = health;
	}
	return ::g->lastcalc;
}


//
// This is a not-very-pretty routine which handles
//  the face states and their timing.
// the precedence of expressions is:
//  dead > evil grin > turned head > straight ahead
//
void ST_updateFaceWidget(void)
{
	int		i;
	angle_t	badguyangle;
	angle_t	diffang;
	qboolean	doevilgrin;

	if (::g->priority < 10)
	{
		// dead
		if (!::g->plyr->health)
		{
			::g->priority = 9;
			::g->st_faceindex = ST_DEADFACE;
			::g->st_facecount = 1;
		}
	}

	if (::g->priority < 9)
	{
		if (::g->plyr->bonuscount)
		{
			// picking up bonus
			doevilgrin = false;

			for (i=0;i<NUMWEAPONS;i++)
			{
				if (::g->oldweaponsowned[i] != ::g->plyr->weaponowned[i])
				{
					doevilgrin = true;
					::g->oldweaponsowned[i] = ::g->plyr->weaponowned[i];
				}
			}
			if (doevilgrin) 
			{
				// evil grin if just picked up weapon
				::g->priority = 8;
				::g->st_facecount = ST_EVILGRINCOUNT;
				::g->st_faceindex = ST_calcPainOffset() + ST_EVILGRINOFFSET;
			}
		}

	}

	if (::g->priority < 8)
	{
		if (::g->plyr->damagecount
			&& ::g->plyr->attacker
			&& ::g->plyr->attacker != ::g->plyr->mo)
		{
			// being attacked
			::g->priority = 7;

			if (::g->plyr->health - ::g->st_oldhealth > ST_MUCHPAIN)
			{
				::g->st_facecount = ST_TURNCOUNT;
				::g->st_faceindex = ST_calcPainOffset() + ST_OUCHOFFSET;
			}
			else
			{
				badguyangle = R_PointToAngle2(::g->plyr->mo->x,
					::g->plyr->mo->y,
					::g->plyr->attacker->x,
					::g->plyr->attacker->y);

				if (badguyangle > ::g->plyr->mo->angle)
				{
					// whether right or left
					diffang = badguyangle - ::g->plyr->mo->angle;
					i = diffang > ANG180; 
				}
				else
				{
					// whether left or right
					diffang = ::g->plyr->mo->angle - badguyangle;
					i = diffang <= ANG180; 
				} // confusing, aint it?


				::g->st_facecount = ST_TURNCOUNT;
				::g->st_faceindex = ST_calcPainOffset();

				if (diffang < ANG45)
				{
					// head-on    
					::g->st_faceindex += ST_RAMPAGEOFFSET;
				}
				else if (i)
				{
					// turn face right
					::g->st_faceindex += ST_TURNOFFSET;
				}
				else
				{
					// turn face left
					::g->st_faceindex += ST_TURNOFFSET+1;
				}
			}
		}
	}

	if (::g->priority < 7)
	{
		// getting hurt because of your own damn stupidity
		if (::g->plyr->damagecount)
		{
			if (::g->plyr->health - ::g->st_oldhealth > ST_MUCHPAIN)
			{
				::g->priority = 7;
				::g->st_facecount = ST_TURNCOUNT;
				::g->st_faceindex = ST_calcPainOffset() + ST_OUCHOFFSET;
			}
			else
			{
				::g->priority = 6;
				::g->st_facecount = ST_TURNCOUNT;
				::g->st_faceindex = ST_calcPainOffset() + ST_RAMPAGEOFFSET;
			}

		}

	}

	if (::g->priority < 6)
	{
		// rapid firing
		if (::g->plyr->attackdown)
		{
			if (::g->lastattackdown==-1)
				::g->lastattackdown = ST_RAMPAGEDELAY;
			else if (!--::g->lastattackdown)
			{
				::g->priority = 5;
				::g->st_faceindex = ST_calcPainOffset() + ST_RAMPAGEOFFSET;
				::g->st_facecount = 1;
				::g->lastattackdown = 1;
			}
		}
		else
			::g->lastattackdown = -1;

	}

	if (::g->priority < 5)
	{
		// invulnerability
		if ((::g->plyr->cheats & CF_GODMODE)
			|| ::g->plyr->powers[pw_invulnerability])
		{
			::g->priority = 4;

			::g->st_faceindex = ST_GODFACE;
			::g->st_facecount = 1;

		}

	}

	// look left or look right if the facecount has timed out
	if (!::g->st_facecount)
	{
		::g->st_faceindex = ST_calcPainOffset() + (::g->st_randomnumber % 3);
		::g->st_facecount = ST_STRAIGHTFACECOUNT;
		::g->priority = 0;
	}

	::g->st_facecount--;

}

void ST_updateWidgets(void)
{
	int		i;

	// must redirect the pointer if the ready weapon has changed.
	//  if (::g->w_ready.data != ::g->plyr->readyweapon)
	//  {
	if (weaponinfo[::g->plyr->readyweapon].ammo == am_noammo)
		::g->w_ready.num = &::g->largeammo;
	else
		::g->w_ready.num = &::g->plyr->ammo[weaponinfo[::g->plyr->readyweapon].ammo];
	//{
	// static int tic=0;
	// static int dir=-1;
	// if (!(tic&15))
	//   ::g->plyr->ammo[weaponinfo[::g->plyr->readyweapon].ammo]+=dir;
	// if (::g->plyr->ammo[weaponinfo[::g->plyr->readyweapon].ammo] == -100)
	//   dir = 1;
	// tic++;
	// }
	::g->w_ready.data = ::g->plyr->readyweapon;

	// if (*::g->w_ready.on)
	//  STlib_updateNum(&::g->w_ready, true);
	// refresh weapon change
	//  }

	// update keycard multiple widgets
	for (i=0;i<3;i++)
	{
		::g->keyboxes[i] = ::g->plyr->cards[i] ? i : -1;

		if (::g->plyr->cards[i+3])
			::g->keyboxes[i] = i+3;
	}

	// refresh everything if this is him coming back to life
	ST_updateFaceWidget();

	// used by the ::g->w_armsbg widget
	::g->st_notdeathmatch = !::g->deathmatch;

	// used by ::g->w_arms[] widgets
	::g->st_armson = ::g->st_statusbaron && !::g->deathmatch; 

	// used by ::g->w_frags widget
	::g->st_fragson = ::g->deathmatch && ::g->st_statusbaron; 
	::g->st_fragscount = 0;

	for (i=0 ; i<MAXPLAYERS ; i++)
	{
		if (i != ::g->consoleplayer)
			::g->st_fragscount += ::g->plyr->frags[i];
		else
			::g->st_fragscount -= ::g->plyr->frags[i];
	}

	// get rid of chat window if up because of message
	if (!--::g->st_msgcounter)
		::g->st_chat = ::g->st_oldchat;

}

void ST_updateFullWidgets(void)
{
	int		i;

	// must redirect the pointer if the ready weapon has changed.
	//  if (::g->w_ready.data != ::g->plyr->readyweapon)
	//  {
	if (weaponinfo[::g->plyr->readyweapon].ammo == am_noammo)
		::g->w_f_ready.num = &::g->largeammo;
	else
		::g->w_f_ready.num = &::g->plyr->ammo[weaponinfo[::g->plyr->readyweapon].ammo];
	//{
	// static int tic=0;
	// static int dir=-1;
	// if (!(tic&15))
	//   ::g->plyr->ammo[weaponinfo[::g->plyr->readyweapon].ammo]+=dir;
	// if (::g->plyr->ammo[weaponinfo[::g->plyr->readyweapon].ammo] == -100)
	//   dir = 1;
	// tic++;
	// }
	::g->w_f_ready.data = ::g->plyr->readyweapon;

	// if (*::g->w_ready.on)
	//  STlib_updateNum(&::g->w_ready, true);
	// refresh weapon change
	//  }

	// update keycard multiple widgets
	for (i = 0; i < 3; i++)
	{
		::g->keyboxes[i] = ::g->plyr->cards[i] ? i : -1;

		if (::g->plyr->cards[i + 3])
			::g->keyboxes[i] = i + 3;
	}

	// used by the ::g->w_armsbg widget
	::g->st_notdeathmatch = !::g->deathmatch;

	// used by ::g->w_arms[] widgets
	::g->st_armson = true;

	// used by ::g->w_frags widget
	::g->st_fragson = ::g->deathmatch;
	::g->st_fragscount = 0;

	for (i = 0; i < MAXPLAYERS; i++)
	{
		if (i != ::g->consoleplayer)
			::g->st_fragscount += ::g->plyr->frags[i];
		else
			::g->st_fragscount -= ::g->plyr->frags[i];
	}

	// get rid of chat window if up because of message
	if (!--::g->st_msgcounter)
		::g->st_chat = ::g->st_oldchat;

}

void ST_Ticker (void)
{

	::g->st_clock++;
	::g->st_randomnumber = M_Random();
	if (!in_photomode.GetBool()) {
		if (::g->st_statusbaron) {
			ST_updateWidgets();
		}
		else if (cl_HUD.GetBool()) {
			ST_updateFullWidgets();
		}
	}
	::g->st_oldhealth = ::g->plyr->health;

}


void ST_doPaletteStuff(void)
{

	int		palette;
	byte*	pal;
	int		cnt;
	int		bzc;

	cnt = ::g->plyr->damagecount;

	if (::g->plyr->powers[pw_strength])
	{
		// slowly fade the berzerk out
		bzc = 12 - (::g->plyr->powers[pw_strength]>>6);

		if (bzc > cnt)
			cnt = bzc;
	}

	if (cnt)
	{
		palette = (cnt+7)>>3;

		if (palette >= NUMREDPALS)
			palette = NUMREDPALS-1;

		palette += STARTREDPALS;
		if (::g->plyr->powers[pw_strength])
			::g->blurryoffset = ::g->blurryoffsetmap[pw_strength];
	}

	else if (::g->plyr->bonuscount)
	{
		palette = (::g->plyr->bonuscount+7)>>3;

		if (palette >= NUMBONUSPALS)
			palette = NUMBONUSPALS-1;

		palette += STARTBONUSPALS;
	}

	else if (::g->plyr->powers[pw_ironfeet] > 4 * 32
		|| ::g->plyr->powers[pw_ironfeet] & 8) {
		::g->blurryoffset = ::g->blurryoffsetmap[pw_ironfeet];
		palette = RADIATIONPAL;
	}
	else {
		::g->blurryoffset = 0;
		palette = 0;
	}

	if (palette != ::g->st_palette)
	{
		::g->st_palette = palette;
		pal = (byte *) W_CacheLumpNum (::g->lu_palette, PU_CACHE_SHARED)+palette*768;
		I_SetPalette (pal, W_LumpLength(::g->lu_palette));
	}

}

void ST_drawWidgets(qboolean refresh)
{
	int		i;

	::g->st_notdeathmatch = !::g->deathmatch;

	// used by ::g->w_arms[] widgets
	::g->st_armson = ::g->st_statusbaron && !::g->deathmatch;

	// used by ::g->w_frags widget
	::g->st_fragson = ::g->deathmatch && ::g->st_statusbaron; 
	STlib_updateNum(&::g->w_time, refresh);
	STlib_updateNum(&::g->w_ready, refresh);

	for (i=0;i<4;i++)
	{
		STlib_updateNum(&::g->w_ammo[i], refresh);
		STlib_updateNum(&::g->w_maxammo[i], refresh);
	}

	STlib_updatePercent(&::g->w_health, refresh);
	STlib_updatePercent(&::g->w_armor, refresh);

	STlib_updateBinIcon(&::g->w_armsbg, refresh);

	for (i=0;i<6;i++)
		STlib_updateMultIcon(&::g->w_arms[i], refresh);

	STlib_updateMultIcon(&::g->w_faces, refresh);

	for (i=0;i<3;i++)
		STlib_updateMultIcon(&::g->w_keyboxes[i], refresh);

	for (i = 0; i < 5; i++)
		STlib_updateNum(&::g->w_power[i], refresh);

	STlib_updateNum(&::g->w_frags, refresh);

}

void ST_drawFullWidgets(qboolean refresh)
{
	int		i;

	::g->st_notdeathmatch = !::g->deathmatch;

	// used by ::g->w_arms[] widgets
	::g->st_armson = cl_HUD.GetBool();

	// used by ::g->w_frags widget
	::g->st_fragson = ::g->deathmatch && cl_HUD.GetBool();
	STlib_updateNum(&::g->w_f_time, refresh);
	STlib_updateNum(&::g->w_f_ready, refresh);

	int maxammoind = 0;
	switch (::g->plyr->readyweapon)
	{
	case wp_pistol:
	case wp_chaingun:
		maxammoind = 0;
		break;
	case wp_shotgun:
	case wp_supershotgun:
		maxammoind = 1;
		break;
	case wp_plasma:
	case wp_bfg:
		maxammoind = 2;
		break;
	case wp_missile:
		maxammoind = 3;
		break;
	default:
		maxammoind = -1;
		break;
	}
	if (maxammoind >= 0) {
		STlib_updateNum(&::g->w_f_maxammo[maxammoind], refresh);
	}

	STlib_updatePercent(&::g->w_f_health, refresh);
	STlib_updatePercent(&::g->w_f_armor, refresh);

	for (i = 0; i < 6; i++)
		STlib_updateMultIcon(&::g->w_f_arms[i], refresh);

	for (i = 0; i < 3; i++)
		STlib_updateMultIcon(&::g->w_f_keyboxes[i], refresh);

	for (i = 0; i < 5; i++)
		STlib_updateNum(&::g->w_f_power[i], refresh);

	STlib_updateNum(&::g->w_f_frags, refresh);

}

void ST_doRefresh(void)
{
	::g->st_firsttime = false;

	// draw status bar background to off-screen buff
	ST_refreshBackground();

	if (!in_photomode.GetBool()) {
		// and refresh all widgets
		if (::g->st_statusbaron) {
			ST_drawWidgets(true);
		}
		else if (cl_HUD.GetBool()) {
			ST_drawFullWidgets(true);
		}
	}
}

void ST_diffDraw(void)
{
	if (!in_photomode.GetBool()) {
		// update all widgets
		if (::g->st_statusbaron) {
			ST_drawWidgets(false);
		}
		else if (cl_HUD.GetBool()) {
			ST_drawFullWidgets(false);
		}
	}
}

void ST_Drawer (qboolean fullscreen, qboolean refresh)
{
	::g->st_statusbaron = (!fullscreen) || ::g->automapactive;
	::g->st_statusbaroff = !::g->st_statusbaron && cl_HUD.GetBool();
	::g->st_firsttime = ::g->st_firsttime || refresh;

	// Do red-/gold-shifts from damage/items
	ST_doPaletteStuff();

	// If just after ST_Start(), refresh all
	 ST_doRefresh(); //GK:Keep refreshing the status bar background in order to prevent graphical artifacts on it
	// Otherwise, update as little as possible
	 ST_diffDraw();
}

void ST_loadGraphics(void)
{
	static bool ST_HasBeenCalled = false;

//	if (ST_HasBeenCalled == true)
//		return;
	ST_HasBeenCalled = true;
	
	int		i;
	int		j;
	int		facenum;

	char	namebuf[10]; //GK: Linux gcc 7 is too paranoid and see error where they don't exist
	namebuf[9] = '\0';

	// Load the numbers, tall and short
	for (i=0;i<10;i++)
	{
		sprintf(namebuf, "STTNUM%d", i);
		::g->tallnum[i] = /*(patch_t *)*/ img2lmp(W_CacheLumpName(namebuf, PU_STATUS_FRONT), W_GetNumForName(namebuf));

		sprintf(namebuf, "STCFN0%d", 48 + i);
		::g->fullnum[i] = /*(patch_t *)*/ img2lmp(W_CacheLumpName(namebuf, PU_STATUS_FRONT), W_GetNumForName(namebuf));

		sprintf(namebuf, "STYSNUM%d", i);
		::g->shortnum[i] = /*(patch_t *)*/ img2lmp(W_CacheLumpName(namebuf, PU_STATUS_FRONT), W_GetNumForName(namebuf));
	}

	// Load percent key.
	//Note: why not load STMINUS here, too?
	::g->tallpercent = /*(patch_t *)*/ img2lmp(W_CacheLumpName("STTPRCNT", PU_STATUS_FRONT), W_GetNumForName("STTPRCNT"));

	::g->fullpercent = /*(patch_t *)*/ img2lmp(W_CacheLumpName("STCFN037", PU_STATUS_FRONT), W_GetNumForName("STCFN037"));

	::g->fullslash = /*(patch_t *)*/ img2lmp(W_CacheLumpName("STCFN047", PU_STATUS_FRONT), W_GetNumForName("STCFN047"));

	::g->fullminus = /*(patch_t *)*/ img2lmp(W_CacheLumpName("STCFN045", PU_STATUS_FRONT), W_GetNumForName("STCFN045"));

	// key cards
	for (i=0;i<NUMCARDS;i++)
	{
		sprintf(namebuf, "STKEYS%d", i);
		::g->keys[i] = /*(patch_t *)*/ img2lmp(W_CacheLumpName(namebuf, PU_STATUS_FRONT), W_GetNumForName(namebuf));
	}

	// ::g->arms background
	::g->armsbg = /*(patch_t *)*/ img2lmp(W_CacheLumpName("STARMS", PU_STATUS_BACK), W_GetNumForName("STARMS"));

	// ::g->arms ownership widgets
	for (i=0;i<6;i++)
	{
		sprintf(namebuf, "STGNUM%d", i+2);

		// gray #
		::g->arms[i][0] = /*(patch_t *)*/ img2lmp(W_CacheLumpName(namebuf, PU_STATUS_FRONT), W_GetNumForName(namebuf));

		// yellow #
		::g->arms[i][1] = ::g->shortnum[i+2]; 
		//GK: pink-ish #
		sprintf(namebuf, "STPNNUM%d", i + 2); //GK: That number indicating which weapon is in use on the status bar

		::g->arms[i][2] = img2lmp(W_CacheLumpName(namebuf, PU_STATUS_FRONT), W_GetNumForName(namebuf));
	}

	// face backgrounds for different color ::g->players
	sprintf(namebuf, "STFB%d", ::g->consoleplayer);
	::g->faceback = /*(patch_t *)*/ img2lmp(W_CacheLumpName(namebuf, PU_STATUS_BACK), W_GetNumForName(namebuf));

	// status bar background bits
	::g->sbar = /*(patch_t *)*/ img2lmp(W_CacheLumpName("STBAR", PU_STATUS_BACK), W_GetNumForName("STBAR"));
	::g->mapt = /*(patch_t *)*/ img2lmp(W_CacheLumpName("STMAPT", PU_STATUS_BACK), W_GetNumForName("STMAPT"));
	::g->spwr = /*(patch_t *)*/ img2lmp(W_CacheLumpName("STPWR", PU_STATUS_BACK), W_GetNumForName("STPWR"));
	::g->hear = /*(patch_t *)*/ img2lmp(W_CacheLumpName("ST_HEAR", PU_STATUS_BACK), W_GetNumForName("ST_HEAR"));
	::g->fullarms = /*(patch_t *)*/ img2lmp(W_CacheLumpName("ST_ARMS", PU_STATUS_BACK), W_GetNumForName("ST_ARMS"));
	::g->fullkeys = /*(patch_t *)*/ img2lmp(W_CacheLumpName("ST_KEYS", PU_STATUS_BACK), W_GetNumForName("ST_KEYS"));
	::g->fullpwr = /*(patch_t *)*/ img2lmp(W_CacheLumpName("ST_PWR", PU_STATUS_BACK), W_GetNumForName("ST_PWR"));
	::g->fulltime = /*(patch_t *)*/ img2lmp(W_CacheLumpName("ST_TIME", PU_STATUS_BACK), W_GetNumForName("ST_TIME"));
	::g->fullfrag = /*(patch_t *)*/ img2lmp(W_CacheLumpName("ST_FRAG", PU_STATUS_BACK), W_GetNumForName("ST_FRAG"));
	// face states
	facenum = 0;
	for (i=0;i<ST_NUMPAINFACES;i++)
	{
		for (j=0;j<ST_NUMSTRAIGHTFACES;j++)
		{
			sprintf(namebuf, "STFST%d%d", i, j);
			::g->faces[facenum++] = /*(patch_t*)*/img2lmp(W_CacheLumpName(namebuf, PU_STATUS_FRONT), W_GetNumForName(namebuf));
		}
		sprintf(namebuf, "STFTR%d0", i);	// turn right
		::g->faces[facenum++] = /*(patch_t*)*/img2lmp(W_CacheLumpName(namebuf, PU_STATUS_FRONT), W_GetNumForName(namebuf));
		sprintf(namebuf, "STFTL%d0", i);	// turn left
		::g->faces[facenum++] = /*(patch_t*)*/img2lmp(W_CacheLumpName(namebuf, PU_STATUS_FRONT), W_GetNumForName(namebuf));
		sprintf(namebuf, "STFOUCH%d", i);	// ouch!
		::g->faces[facenum++] = /*(patch_t*)*/img2lmp(W_CacheLumpName(namebuf, PU_STATUS_FRONT), W_GetNumForName(namebuf));
		sprintf(namebuf, "STFEVL%d", i);	// evil grin ;)
		::g->faces[facenum++] = /*(patch_t*)*/img2lmp(W_CacheLumpName(namebuf, PU_STATUS_FRONT), W_GetNumForName(namebuf));
		sprintf(namebuf, "STFKILL%d", i);	// pissed off
		::g->faces[facenum++] = /*(patch_t*)*/img2lmp(W_CacheLumpName(namebuf, PU_STATUS_FRONT), W_GetNumForName(namebuf));
	}
	::g->faces[facenum++] = /*(patch_t*)*/img2lmp(W_CacheLumpName("STFGOD0", PU_STATUS_FRONT), W_GetNumForName("STFGOD0"));
	::g->faces[facenum++] = /*(patch_t*)*/img2lmp(W_CacheLumpName("STFDEAD0", PU_STATUS_FRONT), W_GetNumForName("STFDEAD0"));

}

void ST_loadData(void)
{
	::g->lu_palette = W_GetNumForName ("PLAYPAL");
	ST_loadGraphics();
}

void ST_unloadGraphics(void)
{
	// These things are always reloaded... so just don't bother to clean them up!
}

void ST_unloadData(void)
{
	ST_unloadGraphics();
}

void ST_initData(void)
{

	int		i;

	::g->st_firsttime = true;
	::g->plyr = &::g->players[::g->consoleplayer];

	::g->st_clock = 0;
	::g->st_chatstate = StartChatState;
	::g->st_gamestate = FirstPersonState;

	::g->st_statusbaron = true;
	::g->st_statusbaroff = false;
	::g->st_oldchat = ::g->st_chat = false;
	::g->st_cursoron = false;

	::g->st_faceindex = 0;
	::g->st_palette = -1;

	::g->st_oldhealth = -1;

	for (i=0;i<NUMWEAPONS;i++)
		::g->oldweaponsowned[i] = ::g->plyr->weaponowned[i];

	for (i=0;i<3;i++)
		::g->keyboxes[i] = -1;

	STlib_init();

}



void ST_createWidgets(void)
{

	int i;

	// map time
	STlib_initAspectNum(&::g->w_time,
		ST_TIMEX,
		ST_TIMEY,
		::g->tallnum,
		&::g->normaltime,
		&::g->ASPECT_POS_OFFSET,
		ST_TIMEWIDTH);

	// ready weapon ammo
	STlib_initNum(&::g->w_ready,
		ST_AMMOX,
		ST_AMMOY,
		::g->tallnum,
		&::g->plyr->ammo[weaponinfo[::g->plyr->readyweapon].ammo],
		&::g->st_statusbaron,
		ST_AMMOWIDTH );

	// the last weapon type
	::g->w_ready.data = ::g->plyr->readyweapon; 

	// health percentage
	STlib_initPercent(&::g->w_health,
		ST_HEALTHX,
		ST_HEALTHY,
		::g->tallnum,
		&::g->plyr->health,
		&::g->st_statusbaron,
		::g->tallpercent);

	// ::g->arms background
	STlib_initBinIcon(&::g->w_armsbg,
		ST_ARMSBGX,
		ST_ARMSBGY,
		::g->armsbg,
		&::g->st_notdeathmatch,
		&::g->st_statusbaron);

	// weapons owned
	for(i=0;i<6;i++)
	{
		//GK: For the first time initialize it.
		//Otherwise it might not work so well
		if (::g->plyr->readyweapon == i + 1) {
			::g->weaponcond[i+1] = 2;
		}
		else {
			::g->weaponcond[i + 1] = ::g->plyr->weaponowned[i + 1];
		}
		if (::g->plyr->readyweapon == wp_supershotgun) {
			::g->weaponcond[wp_shotgun] = 2;
		}
		STlib_initMultIcon(&::g->w_arms[i],
			ST_ARMSX+(i%3)*ST_ARMSXSPACE,
			ST_ARMSY+(i/3)*ST_ARMSYSPACE,
			::g->arms[i], (int *) &::g->weaponcond[i + 1],
			&::g->st_armson);
	}

	// frags sum
	STlib_initNum(&::g->w_frags,
		ST_FRAGSX,
		ST_FRAGSY,
		::g->tallnum,
		&::g->st_fragscount,
		&::g->st_fragson,
		ST_FRAGSWIDTH);

	// ::g->faces
	STlib_initMultIcon(&::g->w_faces,
		ST_FACESX,
		ST_FACESY,
		::g->faces,
		&::g->st_faceindex,
		&::g->st_statusbaron);

	// armor percentage - should be colored later
	STlib_initPercent(&::g->w_armor,
		ST_ARMORX,
		ST_ARMORY,
		::g->tallnum,
		&::g->plyr->armorpoints,
		&::g->st_statusbaron, ::g->tallpercent);

	// ::g->keyboxes 0-2
	STlib_initMultIcon(&::g->w_keyboxes[0],
		ST_KEY0X,
		ST_KEY0Y,
		::g->keys,
		&::g->keyboxes[0],
		&::g->st_statusbaron);

	STlib_initMultIcon(&::g->w_keyboxes[1],
		ST_KEY1X,
		ST_KEY1Y,
		::g->keys,
		&::g->keyboxes[1],
		&::g->st_statusbaron);

	STlib_initMultIcon(&::g->w_keyboxes[2],
		ST_KEY2X,
		ST_KEY2Y,
		::g->keys,
		&::g->keyboxes[2],
		&::g->st_statusbaron);

	// ammo count (all four kinds)
	STlib_initNum(&::g->w_ammo[0],
		ST_AMMO0X,
		ST_AMMO0Y,
		::g->shortnum,
		&::g->plyr->ammo[0],
		&::g->st_statusbaron,
		ST_AMMO0WIDTH);

	STlib_initNum(&::g->w_ammo[1],
		ST_AMMO1X,
		ST_AMMO1Y,
		::g->shortnum,
		&::g->plyr->ammo[1],
		&::g->st_statusbaron,
		ST_AMMO1WIDTH);

	STlib_initNum(&::g->w_ammo[2],
		ST_AMMO2X,
		ST_AMMO2Y,
		::g->shortnum,
		&::g->plyr->ammo[2],
		&::g->st_statusbaron,
		ST_AMMO2WIDTH);

	STlib_initNum(&::g->w_ammo[3],
		ST_AMMO3X,
		ST_AMMO3Y,
		::g->shortnum,
		&::g->plyr->ammo[3],
		&::g->st_statusbaron,
		ST_AMMO3WIDTH);

	// max ammo count (all four kinds)
	STlib_initNum(&::g->w_maxammo[0],
		ST_MAXAMMO0X,
		ST_MAXAMMO0Y,
		::g->shortnum,
		&::g->plyr->maxammo[0],
		&::g->st_statusbaron,
		ST_MAXAMMO0WIDTH);

	STlib_initNum(&::g->w_maxammo[1],
		ST_MAXAMMO1X,
		ST_MAXAMMO1Y,
		::g->shortnum,
		&::g->plyr->maxammo[1],
		&::g->st_statusbaron,
		ST_MAXAMMO1WIDTH);

	STlib_initNum(&::g->w_maxammo[2],
		ST_MAXAMMO2X,
		ST_MAXAMMO2Y,
		::g->shortnum,
		&::g->plyr->maxammo[2],
		&::g->st_statusbaron,
		ST_MAXAMMO2WIDTH);

	STlib_initNum(&::g->w_maxammo[3],
		ST_MAXAMMO3X,
		ST_MAXAMMO3Y,
		::g->shortnum,
		&::g->plyr->maxammo[3],
		&::g->st_statusbaron,
		ST_MAXAMMO3WIDTH);

	// power up timers(all 5 of them)
	STlib_initAspectNum(&::g->w_power[0],
		ST_POWER0X,
		ST_POWER0Y,
		::g->shortnum,
		&::g->normalpowers[pw_invulnerability],
		&::g->ASPECT_POS_OFFSET,
		ST_POWER0WIDTH);

	STlib_initAspectNum(&::g->w_power[1],
		ST_POWER1X,
		ST_POWER1Y,
		::g->shortnum,
		&::g->normalpowers[pw_strength],
		&::g->ASPECT_POS_OFFSET,
		ST_POWER1WIDTH);

	STlib_initAspectNum(&::g->w_power[2],
		ST_POWER2X,
		ST_POWER2Y,
		::g->shortnum,
		&::g->normalpowers[pw_infrared],
		&::g->ASPECT_POS_OFFSET,
		ST_POWER2WIDTH);

	STlib_initAspectNum(&::g->w_power[3],
		ST_POWER3X,
		ST_POWER3Y,
		::g->shortnum,
		&::g->normalpowers[pw_invisibility],
		&::g->ASPECT_POS_OFFSET,
		ST_POWER3WIDTH);

	STlib_initAspectNum(&::g->w_power[4],
		ST_POWER4X,
		ST_POWER4Y,
		::g->shortnum,
		&::g->normalpowers[pw_ironfeet],
		&::g->ASPECT_POS_OFFSET,
		ST_POWER4WIDTH);

}

void ST_createFullScreenWidgets() {

	int xscale = (2 - (::g->ASPECT_IMAGE_SCALER - GLOBAL_IMAGE_SCALER));
	int powerX = (::g->SCREENWIDTH / GLOBAL_IMAGE_SCALER) - 2;
	int powerY = (SCREENHEIGHT / GLOBAL_IMAGE_SCALER) / 2;
	int i;
	// map time
	STlib_initAspectNum(&::g->w_f_time,
		powerX,
		20,
		::g->fullnum,
		&::g->normaltime,
		&::g->st_statusbaroff,
		ST_TIMEWIDTH);

	// ready weapon ammo
	STlib_initNum(&::g->w_f_ready,
		ST_ARMORX + (100 / xscale),
		ST_AMMOY + 8,
		::g->fullnum,
		&::g->plyr->ammo[weaponinfo[::g->plyr->readyweapon].ammo],
		&::g->st_statusbaroff,
		ST_AMMOWIDTH);

	// the last weapon type
	::g->w_f_ready.data = ::g->plyr->readyweapon;

	// health percentage
	STlib_initPercent(&::g->w_f_health,
		ST_HEALTHX - (35 / xscale) + (30 * (xscale - 1)),
		ST_HEALTHY + 12,
		::g->fullnum,
		&::g->plyr->health,
		&::g->st_statusbaroff,
		::g->fullpercent);

	// weapons owned
	for (i = 0; i < 6; i++)
	{
		//GK: For the first time initialize it.
		//Otherwise it might not work so well
		if (::g->plyr->readyweapon == i + 1) {
			::g->weaponcond[i + 1] = 2;
		}
		else {
			::g->weaponcond[i + 1] = ::g->plyr->weaponowned[i + 1];
		}
		if (::g->plyr->readyweapon == wp_supershotgun) {
			::g->weaponcond[wp_shotgun] = 2;
		}
		STlib_initMultIcon(&::g->w_f_arms[i],
			ST_ARMORX + (66 / xscale) - (10 * (xscale - 1)) + (i * 13) - ((4 - ::g->ASPECT_IMAGE_SCALER) * 7),
			ST_AMMOY + 20,
			::g->arms[i], (int*)&::g->weaponcond[i + 1],
			&::g->st_armson);
	}

	// frags sum
	STlib_initNum(&::g->w_f_frags,
		(::g->SCREENWIDTH / 2) - ((45 / xscale) -  (30 * (xscale - 1))),
		ORIGINAL_HEIGHT - 13,
		::g->fullnum,
		&::g->st_fragscount,
		&::g->st_fragson,
		ST_FRAGSWIDTH);

	// armor percentage - should be colored later
	STlib_initPercent(&::g->w_f_armor,
		ST_HEALTHX - (92 / xscale) + (4 * (xscale - 1)) + (2 * (xscale - 2)),
		ST_ARMORY + 7,
		::g->fullnum,
		&::g->plyr->armorpoints,
		&::g->st_statusbaroff, ::g->fullpercent);

	// ::g->keyboxes 0-2
	STlib_initAspectMultIcon(&::g->w_f_keyboxes[0],
		3,
		ORIGINAL_HEIGHT - 9,
		::g->keys,
		&::g->keyboxes[0],
		&::g->st_statusbaroff);

	STlib_initAspectMultIcon(&::g->w_f_keyboxes[1],
		16,
		ORIGINAL_HEIGHT - 9,
		::g->keys,
		&::g->keyboxes[1],
		&::g->st_statusbaroff);

	STlib_initAspectMultIcon(&::g->w_f_keyboxes[2],
		29,
		ORIGINAL_HEIGHT - 9,
		::g->keys,
		&::g->keyboxes[2],
		&::g->st_statusbaroff);

	// max ammo count (all four kinds)
	STlib_initNum(&::g->w_f_maxammo[0],
		ST_ARMORX + (135 / xscale) + (20 * (xscale - 1)),
		ST_AMMOY + 8,
		::g->fullnum,
		&::g->plyr->maxammo[0],
		&::g->st_statusbaroff,
		ST_MAXAMMO0WIDTH);

	STlib_initNum(&::g->w_f_maxammo[1],
		ST_ARMORX + (135 / xscale) + (20 * (xscale - 1)),
		ST_AMMOY + 8,
		::g->fullnum,
		&::g->plyr->maxammo[1],
		&::g->st_statusbaroff,
		ST_MAXAMMO1WIDTH);

	STlib_initNum(&::g->w_f_maxammo[2],
		ST_ARMORX + (135 / xscale) + (20 * (xscale - 1)),
		ST_AMMOY + 8,
		::g->fullnum,
		&::g->plyr->maxammo[2],
		&::g->st_statusbaroff,
		ST_MAXAMMO2WIDTH);

	STlib_initNum(&::g->w_f_maxammo[3],
		ST_ARMORX + (135 / xscale) + (20 * (xscale - 1)),
		ST_AMMOY + 8,
		::g->fullnum,
		&::g->plyr->maxammo[3],
		&::g->st_statusbaroff,
		ST_MAXAMMO3WIDTH);

	// power up timers(all 5 of them)
	STlib_initAspectNum(&::g->w_f_power[0],
		powerX,
		powerY,
		::g->shortnum,
		&::g->normalpowers[pw_invulnerability],
		&::g->st_statusbaroff,
		ST_POWER0WIDTH);

	STlib_initAspectNum(&::g->w_f_power[1],
		powerX,
		powerY + 8,
		::g->shortnum,
		&::g->normalpowers[pw_strength],
		&::g->st_statusbaroff,
		ST_POWER1WIDTH);

	STlib_initAspectNum(&::g->w_f_power[2],
		powerX,
		powerY + 16,
		::g->shortnum,
		&::g->normalpowers[pw_infrared],
		&::g->st_statusbaroff,
		ST_POWER2WIDTH);

	STlib_initAspectNum(&::g->w_f_power[3],
		powerX,
		powerY + 24,
		::g->shortnum,
		&::g->normalpowers[pw_invisibility],
		&::g->st_statusbaroff,
		ST_POWER3WIDTH);

	STlib_initAspectNum(&::g->w_f_power[4],
		powerX,
		powerY + 32,
		::g->shortnum,
		&::g->normalpowers[pw_ironfeet],
		&::g->st_statusbaroff,
		ST_POWER4WIDTH);
}



void ST_Start (void)
{

	if (!::g->st_stopped)
		ST_Stop();

	ST_initData();
	ST_createWidgets();
	ST_createFullScreenWidgets();
	::g->st_stopped = false;

}

void ST_Stop (void)
{
	if (::g->st_stopped)
		return;

	I_SetPalette ((byte*)W_CacheLumpNum ((int)::g->lu_palette, PU_CACHE_SHARED), W_LumpLength(::g->lu_palette));

	::g->st_stopped = true;
}

void ST_Init (void)
{
	::g->veryfirsttime = 0;
	ST_loadData();
	::g->screens[4] = (byte *) DoomLib::Z_Malloc(::g->SCREENWIDTH * SCREENHEIGHT /*ST_WIDTH*ST_HEIGHT*/, PU_STATIC, 0);
	memset( ::g->screens[4], 0, ::g->SCREENWIDTH * SCREENHEIGHT );
}


CONSOLE_COMMAND_SHIP( idqd, "cheat for toggleable god mode", 0 ) {
	int oldPlayer = DoomLib::GetPlayer();
	DoomLib::SetPlayer( 0 );
	if ( ::g == NULL ) {
		::common->Printf(WRONGCHEATGAME);
		return;
	}

	if (::g->gamestate != GS_LEVEL) {
		return;
	}

	::g->plyr->cheats ^= CF_GODMODE;
	if (::g->plyr->cheats & CF_GODMODE)
	{
		if (::g->plyr->mo)
			::g->plyr->mo->health = ::g->ghealth;

		::g->plyr->health = ::g->ghealth;
		::g->plyr->message = STSTR_DQDON;
	}
	else 
		::g->plyr->message = STSTR_DQDOFF;

	DoomLib::SetPlayer( oldPlayer );
}

CONSOLE_COMMAND_SHIP( idfa, "cheat for killer fucking arsenal", 0 ) {
	int oldPlayer = DoomLib::GetPlayer();
	DoomLib::SetPlayer( 0 );
	if ( ::g == NULL ) {
		::common->Printf(WRONGCHEATGAME);
		return;
	}

	if (::g->gamestate != GS_LEVEL) {
		return;
	}

	int i = 0;
	::g->plyr->armorpoints = ::g->farmor;
	::g->plyr->armortype = ::g->fart;

	for (i = 0; i < NUMWEAPONS; i++) {
		::g->plyr->weaponowned[i] = true;
		if (::g->weaponcond[i] != 2) { //GK: Everytime you get a weapon record that
			::g->weaponcond[i] = 1;
		}
	}

	for (i=0;i<NUMAMMO;i++)
		::g->plyr->ammo[i] = ::g->plyr->maxammo[i];

	::g->plyr->message = STSTR_FAADDED;

	DoomLib::SetPlayer( oldPlayer );
}

CONSOLE_COMMAND_SHIP( idkfa, "cheat for key full ammo", 0 ) {
	int oldPlayer = DoomLib::GetPlayer();
	DoomLib::SetPlayer( 0 );
	if ( ::g == NULL ) {
		::common->Printf(WRONGCHEATGAME);
		return;
	}

	if (::g->gamestate != GS_LEVEL) {
		return;
	}

	int i = 0;
	::g->plyr->armorpoints = ::g->kfarmor;
	::g->plyr->armortype = ::g->kfart;
	//GK: Give the Backpack with the kfa cheat
	if (!::g->plyr->backpack) {//GK: That was something I forgot to look for
		for (i = 0; i < NUMAMMO; i++) {
			::g->plyr->maxammo[i] *= 2;
		}
		::g->plyr->backpack = true;
	}
	//GK: End
	for (i = 0; i < NUMWEAPONS; i++) {
		::g->plyr->weaponowned[i] = true;
		if (::g->weaponcond[i] != 2) { //GK: Everytime you get a weapon record that
			::g->weaponcond[i] = 1;
		}
	}

	for (i=0;i<NUMAMMO;i++)
		::g->plyr->ammo[i] = ::g->plyr->maxammo[i];

	for (i=0;i<NUMCARDS;i++)
		::g->plyr->cards[i] = true;

	::g->plyr->message = STSTR_KFAADDED;

	DoomLib::SetPlayer( oldPlayer );
}


CONSOLE_COMMAND_SHIP( idclip, "cheat for no clip", 0 ) {
	int oldPlayer = DoomLib::GetPlayer();
	DoomLib::SetPlayer( 0 );
	if ( ::g == NULL ) {
		::common->Printf(WRONGCHEATGAME);
		return;
	}

	if (::g->gamestate != GS_LEVEL) {
		return;
	}

	::g->plyr->cheats ^= CF_NOCLIP;

	if (::g->plyr->cheats & CF_NOCLIP)
		::g->plyr->message = STSTR_NCON;
	else
		::g->plyr->message = STSTR_NCOFF;

	DoomLib::SetPlayer( oldPlayer );
}
CONSOLE_COMMAND_SHIP( idmypos, "for player position", 0 ) {
	int oldPlayer = DoomLib::GetPlayer();
	DoomLib::SetPlayer( 0 );
	if ( ::g == NULL ) {
		::common->Printf(WRONGCHEATGAME);
		return;
	}

	if (::g->gamestate != GS_LEVEL) {
		return;
	}

	static char	buf[ST_MSGWIDTH];
	sprintf(buf, "ang=0x%x;x,y=(0x%x,0x%x)",
		::g->players[::g->consoleplayer].mo->angle,
		::g->players[::g->consoleplayer].mo->x,
		::g->players[::g->consoleplayer].mo->y);
	::g->plyr->message = buf;

	DoomLib::SetPlayer( oldPlayer );
}

CONSOLE_COMMAND_SHIP( idclev, "warp to next level", 0 ) {
	int oldPlayer = DoomLib::GetPlayer();
	DoomLib::SetPlayer( 0 );
	if ( ::g == NULL ) {
		::common->Printf(WRONGCHEATGAME);
		return;
	}

	if (::g->gamestate != GS_LEVEL) {
		return;
	}

	int		epsd;
	int		map;

	if (::g->gamemode == commercial)
	{
		
		if( args.Argc() > 1 ) {
			epsd = 1;
			map = atoi( args.Argv( 1 ) );
		} else {
			idLib::Printf( "idclev takes map as first argument \n"  );
			return;
		}

		if( map > 33 ) {
			map = 1;
		}else
		if (::g->gamemission == pack_nerve && map > 9) {
			map = 1;
		}
		else if(::g->gamemission == pack_master && map > 21){
			map = 1;
		}
		else if (!::g->isbfg && map > 32) {
			map = 1;
		}
	}
	else
	{
		if( args.Argc() > 2 ) {
			epsd = atoi( args.Argv( 1 ) );
			map = atoi( args.Argv( 2 ) );
		} else {
			idLib::Printf( "idclev takes episode and map as first two arguments \n"  );
			return;
		}
	}

	if (::g->gamemission == pack_custom) {//GK:Custom expansion related stuff
		if (args.Argc() > 2) {
			epsd = atoi(args.Argv(1));
			map = atoi(args.Argv(2));

			if (epsd < 1 || epsd > (int)::g->clusters.size())
				epsd = 1;

			if (::g->clusters[epsd - 1].startmap != ::g->clusters[epsd - 1].endmap)
			{
				if (map <= 0 || ::g->clusters[epsd - 1].startmap - 1 + map > ::g->clusters[epsd - 1].endmap + 1)
					map = 1;
			}
		}
		else if (args.Argc() > 1)
		{
			map = atoi(args.Argv(1));

			if (map > ::g->mapmax)
				map = 1;
			while (!::g->maps[map - 1].lumpname) {
				map++;
			}
			::g->prevmap = map;
		}
	}

	// Catch invalid maps.
	if (epsd < 1)
		return;

	if (map < 1)
		return;

	// Ohmygod - this is not going to work.
	if ((::g->gamemode == retail && ::g->gamemission == doom)
		&& ((epsd > 4) || (map > 9)))
		return;

	if ((::g->gamemode == registered)
		&& ((epsd > 3) || (map > 9)))
		return;

	if ((::g->gamemode == shareware)
		&& ((epsd > 1) || (map > 9)))
		return;

	if ((::g->gamemode == commercial && ::g->gamemission == doom2)
		&& (( epsd > 1) || (map > 34)))
		return;

	if ((::g->gamemission == pack_nerve)
		&& ((epsd > 1) || (map > 9)))
		return;

	if ((::g->gamemission == pack_master)
		&& ((epsd > 1) || (map > 21)))
		return;
	/*if ((::g->gamemission == pack_custom)//GK:Custom expansion related stuff
		&& ((epsd > 1) || (map > ::g->mapmax)))
		return;*/
	// So be it.
	::g->plyr->message = STSTR_CLEV;
	G_DeferedInitNew(::g->gameskill, epsd, map);

	DoomLib::SetPlayer( oldPlayer );
}

CONSOLE_COMMAND_SHIP(idmus, "change level music", 0) {
	int		musnum;
	int		map;
	int		epsd;

	//int oldPlayer = DoomLib::GetPlayer();
	DoomLib::SetPlayer(0);
	if (::g == NULL) {
		return;
	}
	::g->plyr->message = STSTR_MUS;
	//cht_GetParam(&cheat_mus, buf);

	if (::g->gamemode == commercial)
	{
		if (args.Argc() > 1) {
			map = atoi(args.Argv(1))-1;
		}
		else {
			idLib::Printf("idmus takes map as first argument \n");
			return;
		}
		//GK: Buffers are geting number + 1 (except 0 which equals 11)
		musnum = mus_runnin + map;

		if (map > 35)
			::g->plyr->message = STSTR_NOMUS;
		else
			S_ChangeMusic(musnum, 1);
	}
	else
	{
		if (args.Argc() > 2) {
			epsd = atoi(args.Argv(1))-1;
			map = atoi(args.Argv(2))-1;
		}
		else {
			idLib::Printf("idmus takes episode and map as first two arguments \n");
			return;
		}
		musnum = mus_e1m1 + epsd*9 + map;

		if (epsd*9 + map > 31)
			::g->plyr->message = STSTR_NOMUS;
		else
			S_ChangeMusic(musnum, 1);
	}
}