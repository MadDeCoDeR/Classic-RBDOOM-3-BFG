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

#include <ctype.h>

#include "doomdef.h"

#include "z_zone.h"

#include "m_swap.h"

#include "hu_stuff.h"
#include "hu_lib.h"
#include "w_wad.h"

#include "s_sound.h"

#include "doomstat.h"

// Data.
#include "dstrings.h"
#include "sounds.h"

#include "Main.h"

#include "d_exp.h"

extern idCVar in_joylayout;

//
// Locally used constants, shortcuts.
//

extern const char* const temp_chat_macros[];
const char*	const temp_chat_macros[] =
{
	HUSTR_CHATMACRO0,
	HUSTR_CHATMACRO1,
	HUSTR_CHATMACRO2,
	HUSTR_CHATMACRO3,
	HUSTR_CHATMACRO4,
	HUSTR_CHATMACRO5,
	HUSTR_CHATMACRO6,
	HUSTR_CHATMACRO7,
	HUSTR_CHATMACRO8,
	HUSTR_CHATMACRO9
};

extern const char* const player_names[];
const char*	const player_names[] =
{
	HUSTR_PLRGREEN,
	HUSTR_PLRINDIGO,
	HUSTR_PLRBROWN,
	HUSTR_PLRRED
};







//
// Builtin map names.
// The actual names can be found in DStrings.h.
//

char* mapnames[45];
char* mapnames2[33];
char* mapnamesp[32];
char* mapnamest[32];

void initMapNames() {
	char*	tmapnames[] =
	{

		(char*) HUSTR_E1M1,
		(char*) HUSTR_E1M2,
		(char*) HUSTR_E1M3,
		(char*) HUSTR_E1M4,
		(char*) HUSTR_E1M5,
		(char*) HUSTR_E1M6,
		(char*) HUSTR_E1M7,
		(char*) HUSTR_E1M8,
		(char*) HUSTR_E1M9,

		(char*) HUSTR_E2M1,
		(char*) HUSTR_E2M2,
		(char*) HUSTR_E2M3,
		(char*) HUSTR_E2M4,
		(char*) HUSTR_E2M5,
		(char*) HUSTR_E2M6,
		(char*) HUSTR_E2M7,
		(char*) HUSTR_E2M8,
		(char*) HUSTR_E2M9,

		(char*) HUSTR_E3M1,
		(char*) HUSTR_E3M2,
		(char*) HUSTR_E3M3,
		(char*) HUSTR_E3M4,
		(char*) HUSTR_E3M5,
		(char*) HUSTR_E3M6,
		(char*) HUSTR_E3M7,
		(char*) HUSTR_E3M8,
		(char*) HUSTR_E3M9,

		(char*) HUSTR_E4M1,
		(char*) HUSTR_E4M2,
		(char*) HUSTR_E4M3,
		(char*) HUSTR_E4M4,
		(char*) HUSTR_E4M5,
		(char*) HUSTR_E4M6,
		(char*) HUSTR_E4M7,
		(char*) HUSTR_E4M8,
		(char*) HUSTR_E4M9,

		(char*) "NEWLEVEL",
		(char*) "NEWLEVEL",
		(char*) "NEWLEVEL",
		(char*) "NEWLEVEL",
		(char*) "NEWLEVEL",
		(char*) "NEWLEVEL",
		(char*) "NEWLEVEL",
		(char*) "NEWLEVEL",
		(char*) "NEWLEVEL"
	};
	memcpy(mapnames, tmapnames, sizeof(tmapnames));
	char*	tmapnames2[] =
	{
		(char*) HUSTR_1,
		(char*) HUSTR_2,
		(char*) HUSTR_3,
		(char*) HUSTR_4,
		(char*) HUSTR_5,
		(char*) HUSTR_6,
		(char*) HUSTR_7,
		(char*) HUSTR_8,
		(char*) HUSTR_9,
		(char*) HUSTR_10,
		(char*) HUSTR_11,

		(char*) HUSTR_12,
		(char*) HUSTR_13,
		(char*) HUSTR_14,
		(char*) HUSTR_15,
		(char*) HUSTR_16,
		(char*) HUSTR_17,
		(char*) HUSTR_18,
		(char*) HUSTR_19,
		(char*) HUSTR_20,

		(char*) HUSTR_21,
		(char*) HUSTR_22,
		(char*) HUSTR_23,
		(char*) HUSTR_24,
		(char*) HUSTR_25,
		(char*) HUSTR_26,
		(char*) HUSTR_27,
		(char*) HUSTR_28,
		(char*) HUSTR_29,
		(char*) HUSTR_30,
		(char*) HUSTR_31,
		(char*) HUSTR_32,
		(char*) HUSTR_33

	};
	memcpy(mapnames2, tmapnames2, sizeof(tmapnames2));
	/*const*/ char*	tmapnamesp[] =
	{
		(char*) PHUSTR_1,
		(char*) PHUSTR_2,
		(char*) PHUSTR_3,
		(char*) PHUSTR_4,
		(char*) PHUSTR_5,
		(char*) PHUSTR_6,
		(char*) PHUSTR_7,
		(char*) PHUSTR_8,
		(char*) PHUSTR_9,
		(char*) PHUSTR_10,
		(char*) PHUSTR_11,

		(char*) PHUSTR_12,
		(char*) PHUSTR_13,
		(char*) PHUSTR_14,
		(char*) PHUSTR_15,
		(char*) PHUSTR_16,
		(char*) PHUSTR_17,
		(char*) PHUSTR_18,
		(char*) PHUSTR_19,
		(char*) PHUSTR_20,

		(char*) PHUSTR_21,
		(char*) PHUSTR_22,
		(char*) PHUSTR_23,
		(char*) PHUSTR_24,
		(char*) PHUSTR_25,
		(char*) PHUSTR_26,
		(char*) PHUSTR_27,
		(char*) PHUSTR_28,
		(char*) PHUSTR_29,
		(char*) PHUSTR_30,
		(char*) PHUSTR_31,
		(char*) PHUSTR_32
	};
	memcpy(mapnamesp, tmapnamesp, sizeof(tmapnamesp));
	// TNT WAD map names.
	/*const*/ char *tmapnamest[] =
	{
		(char*) THUSTR_1,
		(char*) THUSTR_2,
		(char*) THUSTR_3,
		(char*) THUSTR_4,
		(char*) THUSTR_5,
		(char*) THUSTR_6,
		(char*) THUSTR_7,
		(char*) THUSTR_8,
		(char*) THUSTR_9,
		(char*) THUSTR_10,
		(char*) THUSTR_11,

		(char*) THUSTR_12,
		(char*) THUSTR_13,
		(char*) THUSTR_14,
		(char*) THUSTR_15,
		(char*) THUSTR_16,
		(char*) THUSTR_17,
		(char*) THUSTR_18,
		(char*) THUSTR_19,
		(char*) THUSTR_20,

		(char*) THUSTR_21,
		(char*) THUSTR_22,
		(char*) THUSTR_23,
		(char*) THUSTR_24,
		(char*) THUSTR_25,
		(char*) THUSTR_26,
		(char*) THUSTR_27,
		(char*) THUSTR_28,
		(char*) THUSTR_29,
		(char*) THUSTR_30,
		(char*) THUSTR_31,
		(char*) THUSTR_32
	};
	memcpy(mapnamest, tmapnamest, sizeof(tmapnamest));
}



//GK: cl_messages
idCVar cl_messages("cl_messages", "1", CVAR_INIT | CVAR_INTEGER | CVAR_ARCHIVE, "Set how many messages will be shown", 1, 4);
idCVar cl_showStats("cl_showStats", "0", CVAR_BOOL | CVAR_ARCHIVE | CVAR_NOCHEAT, "Display map status on automap");
//GK End
const char*	shiftxform;

const char english_shiftxform[] =
{

	0,
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
		21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
		31,
		' ', '!', '"', '#', '$', '%', '&',
		'"', // shift-'
		'(', ')', '*', '+',
		'<', // shift-,
		'_', // shift--
		'>', // shift-.
		'?', // shift-/
		')', // shift-0
		'!', // shift-1
		'@', // shift-2
		'#', // shift-3
		'$', // shift-4
		'%', // shift-5
		'^', // shift-6
		'&', // shift-7
		'*', // shift-8
		'(', // shift-9
		':',
		':', // shift-;
		'<',
		'+', // shift-=
		'>', '?', '@',
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
		'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
		'[', // shift-[
		'!', // shift-backslash - OH MY GOD DOES WATCOM SUCK
		']', // shift-]
		'"', '_',
		'\'', // shift-`
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
		'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
		'{', '|', '}', '~', 127
};

char ForeignTranslation(unsigned char ch)
{
	return ch;
}

void HU_Init(void)
{

	int		i;
	int		j;
	char	buffer[9];

	shiftxform = english_shiftxform;

	// load the heads-up font
	j = HU_FONTSTART;
	for (i=0;i<HU_FONTSIZE;i++)
	{
		sprintf(buffer, "STCFN%.3d", j++);
		::g->hu_font[i] = /*(patch_t *)*/ img2lmp(W_CacheLumpName(buffer, PU_FONT), W_GetNumForName(buffer));
	}
	
	HU_UpdateGlyphs();
}

void HU_Stop(void)
{
	::g->headsupactive = false;
}

void HU_Start(void)
{

	int		i;
	const char*	s;

	if (::g->headsupactive)
		HU_Stop();

	::g->plr = &::g->players[::g->consoleplayer];
	::g->message_on = false;
	::g->message_dontfuckwithme = false;
	::g->message_nottobefuckedwith = false;
	::g->chat_on = false;

	// create the message widget
	HUlib_initSText(&::g->w_message,
		HU_MSGX, HU_MSGY, cl_messages.GetInteger(),
		::g->hu_font,
		HU_FONTSTART, &::g->message_on);

	// create the map title widget
	HUlib_initTextLine(&::g->w_title,
		HU_TITLEX, HU_TITLEY,
		::g->hu_font,
		HU_FONTSTART);

	HUlib_initTextLine(&::g->w_kills,
		HU_STATS, HU_TITLEY - SHORT(::g->hu_font[0]->height * 2),
		::g->hu_font,
		HU_FONTSTART);
	HUlib_initTextLine(&::g->w_items,
		HU_STATS, HU_TITLEY - SHORT(::g->hu_font[0]->height),
		::g->hu_font,
		HU_FONTSTART);
	HUlib_initTextLine(&::g->w_secrets,
		HU_STATS, HU_TITLEY,
		::g->hu_font,
		HU_FONTSTART);

	switch ( ::g->gamemode )
	{
	case shareware:
	case registered:
	case retail:
		s = HU_TITLE;
		break;
	case commercial:
		//GK: Show properly on automap the level name based on expansion
		if (DoomLib::expansionSelected == doom2) {
			//GK:Use the mapnames2 instead of DOOM2_mapnames
			s = HU_TITLE2;
		}
		else if (DoomLib::expansionSelected == pack_tnt) {
			s = HU_TITLET;
		}
		else if (DoomLib::expansionSelected == pack_plut) {
			s = HU_TITLEP;
		}
		else {
			if (DoomLib::expansionSelected == 5) {
				int map = ::g->gamemap;
				if (::g->gamemap > 9) {
					map = 0;
				}
				//GK: Give modified level name even on nerve and master
				if (!::g->modifiedtext) {
					char ts[512];
					sprintf(ts, "level %s", DoomLib::GetCurrentExpansion()->mapNames[map - 1]);
					s = (const char*)ts;
				}
				else {
					s= HU_TITLE2;
				}
				
			}
			else {
				if (!::g->modifiedtext) {
					char ts[512];
					sprintf(ts, "level %s", DoomLib::GetCurrentExpansion()->mapNames[::g->gamemap - 1]);
					s = (const char*)ts;
				}
				else {
					s = HU_TITLE2;
				}
			}
		}
		break;
	default:
		if( DoomLib::expansionSelected == 5 ) {
			int map = ::g->gamemap;
			if( ::g->gamemap > 9 ) {
				map = 0;
			} 
			if (!::g->modifiedtext) {
				s = DoomLib::GetCurrentExpansion()->mapNames[map - 1];
			}
			else {
				s = HU_TITLE2;
			}
		} else {
			if (!::g->modifiedtext) {
				s = DoomLib::GetCurrentExpansion()->mapNames[::g->gamemap - 1];
			}
			else {
				s = HU_TITLE2;
			}
		}

		
		break;
	}

	if (::g->gamemission == pack_custom && ::g->map) {
		if (::g->maps[::g->map - 1].realname != NULL) {
			s = ::g->maps[::g->map - 1].realname;
		}
	}
	if (::op) {
		::op->SetAdditionalInfo("status", va("%s:%s", ::g->acronymPrefix, s));
	}
	while (*s)
		HUlib_addCharToTextLine(&::g->w_title, *(s++));

	// create the chat widget
	HUlib_initIText(&::g->w_chat,
		HU_INPUTX, HU_INPUTY,
		::g->hu_font,
		HU_FONTSTART, &::g->chat_on);

	// create the inputbuffer widgets
	for (i=0 ; i<MAXPLAYERS ; i++)
		HUlib_initIText(&::g->w_inputbuffer[i], 0, 0, 0, 0, &::g->always_off);

	::g->headsupactive = true;

}

void HU_Drawer(void)
{

	HUlib_drawSText(&::g->w_message);
	HUlib_drawIText(&::g->w_chat);
	if (::g->automapactive) {
		HUlib_drawTextLine(&::g->w_title, false);

		if (cl_showStats.GetBool()) {
			HUlib_clearTextLine(&::g->w_kills);
			idStr stkills;
			sprintf(stkills, "Kills: %d / %d\0", ::g->players[::g->consoleplayer].killcount, ::g->totalkills);
			const char* s = stkills.c_str();
			::g->w_kills.x = HU_STATS - (stkills.Length() * ::g->hu_font[0]->width);
			while (*s) {
				HUlib_addCharToTextLine(&::g->w_kills, *(s++));
			}
			HUlib_drawTextLine(&::g->w_kills, false);

			HUlib_clearTextLine(&::g->w_items);
			idStr stitems;
			sprintf(stitems, "Items: %d / %d\0", ::g->players[::g->consoleplayer].itemcount, ::g->totalitems);
			const char* s1 = stitems.c_str();
			::g->w_items.x = HU_STATS - (stitems.Length() * ::g->hu_font[0]->width);
			while (*s1) {
				HUlib_addCharToTextLine(&::g->w_items, *(s1++));
			}
			HUlib_drawTextLine(&::g->w_items, false);

			HUlib_clearTextLine(&::g->w_secrets);
			idStr stsecrets;
			sprintf(stsecrets, "Secrets: %d / %d\0", ::g->players[::g->consoleplayer].secretcount, ::g->totalsecret);
			const char* s2 = stsecrets.c_str();
			::g->w_secrets.x = HU_STATS - (stsecrets.Length() * ::g->hu_font[0]->width);
			while (*s2) {
				HUlib_addCharToTextLine(&::g->w_secrets, *(s2++));
			}
			HUlib_drawTextLine(&::g->w_secrets, false);
		}
	}

}

void HU_Erase(void)
{

	HUlib_eraseSText(&::g->w_message);
	HUlib_eraseIText(&::g->w_chat);
	HUlib_eraseTextLine(&::g->w_title);
	HUlib_eraseTextLine(&::g->w_kills);
	HUlib_eraseTextLine(&::g->w_items);
	HUlib_eraseTextLine(&::g->w_secrets);

}

void HU_UpdateGlyphs(void)
{
	char buffer[9];
	for (int i = 0; i < HU_GLYPHSIZE; i++)
	{
		char joysuffix = in_joylayout.GetBool() ? 'P' : 'X';
		sprintf(buffer, "STJOY%c%.2d", joysuffix, i + 1);
		::g->hu_glyph[i] = /*(patch_t *)*/ img2lmp(W_CacheLumpName(buffer, PU_FONT), W_GetNumForName(buffer));
	}
}

void HU_Ticker(void)
{
	// tick down message counter if message is up
	if (::g->message_counter && !--::g->message_counter)
	{
		::g->message_on = false;
		::g->message_nottobefuckedwith = false;
	}

	if ( ( m_inDemoMode.GetBool() == false && m_show_messages.GetBool() ) || ::g->message_dontfuckwithme)
	{

		// display message if necessary
		if ((::g->plr->message && !::g->message_nottobefuckedwith)
			|| (::g->plr->message && ::g->message_dontfuckwithme))
		{
			HUlib_addMessageToSText(&::g->w_message, 0, ::g->plr->message);
			::g->plr->message = 0;
			::g->message_on = true;
			::g->message_counter = HU_MSGTIMEOUT;
			::g->message_nottobefuckedwith = ::g->message_dontfuckwithme;
			::g->message_dontfuckwithme = 0;
		}

	} // else ::g->message_on = false;
}




void HU_queueChatChar(char c)
{
	if (((::g->head + 1) & (QUEUESIZE-1)) == ::g->tail)
	{
		::g->plr->message = HUSTR_MSGU;
	}
	else
	{
		::g->chatchars[::g->head] = c;
		::g->head = (::g->head + 1) & (QUEUESIZE-1);
	}
}

char HU_dequeueChatChar(void)
{
	char c;

	if (::g->head != ::g->tail)
	{
		c = ::g->chatchars[::g->tail];
		::g->tail = (::g->tail + 1) & (QUEUESIZE-1);
	}
	else
	{
		c = 0;
	}

	return c;
}

qboolean HU_Responder(event_t *ev)
{

	const char*		macromessage;
	qboolean		eatkey = false;
	unsigned char 	c;
	int			i;
	int			numplayers;

	const static char		destination_keys[MAXPLAYERS] =
	{
		HUSTR_KEYGREEN,
			HUSTR_KEYINDIGO,
			HUSTR_KEYBROWN,
			HUSTR_KEYRED
	};


	numplayers = 0;
	for (i=0 ; i<MAXPLAYERS ; i++)
		numplayers += ::g->playeringame[i];

	if (ev->data1 == KEY_RSHIFT)
	{
		::g->shiftdown = ev->type == ev_keydown;
		return false;
	}
	else if (ev->data1 == KEY_RALT || ev->data1 == KEY_LALT)
	{
		::g->altdown = ev->type == ev_keydown;
		return false;
	}

	if (ev->type != ev_keydown)
		return false;

	if (!::g->chat_on)
	{
		//GK : Disable the message show when pressing enter
		/*if (ev->data1 == HU_MSGREFRESH)
		{
			::g->message_on = true;
			::g->message_counter = HU_MSGTIMEOUT;
			eatkey = true;
		}
		else*/ if (::g->netgame && ev->data1 == HU_INPUTTOGGLE)
		{
			eatkey = ::g->chat_on = true;
			HUlib_resetIText(&::g->w_chat);
			HU_queueChatChar(HU_BROADCAST);
		}
		else if (::g->netgame && numplayers > 2)
		{
			for (i=0; i<MAXPLAYERS ; i++)
			{
				if (ev->data1 == destination_keys[i])
				{
					if (::g->playeringame[i] && i!=::g->consoleplayer)
					{
						eatkey = ::g->chat_on = true;
						HUlib_resetIText(&::g->w_chat);
						HU_queueChatChar(i+1);
						break;
					}
					else if (i == ::g->consoleplayer)
					{
						::g->num_nobrainers++;
						if (::g->num_nobrainers < 3)
							::g->plr->message = HUSTR_TALKTOSELF1;
						else if (::g->num_nobrainers < 6)
							::g->plr->message = HUSTR_TALKTOSELF2;
						else if (::g->num_nobrainers < 9)
							::g->plr->message = HUSTR_TALKTOSELF3;
						else if (::g->num_nobrainers < 32)
							::g->plr->message = HUSTR_TALKTOSELF4;
						else
							::g->plr->message = HUSTR_TALKTOSELF5;
					}
				}
			}
		}
	}
	else
	{
		c = ev->data1;
		// send a macro
		if (::g->altdown)
		{
			c = c - '0';
			if (c > 9)
				return false;
			// I_PrintfE( "got here\n");
			macromessage = temp_chat_macros[c];

			// kill last message with a '\n'
			HU_queueChatChar(KEY_ENTER); // DEBUG!!!

			// send the macro message
			while (*macromessage)
				HU_queueChatChar(*macromessage++);
			HU_queueChatChar(KEY_ENTER);

			// leave chat mode and notify that it was sent
			::g->chat_on = false;
			strcpy(::g->lastmessage, temp_chat_macros[c]);
			::g->plr->message = ::g->lastmessage;
			eatkey = true;
		}
		else
		{
			if (::g->shiftdown || (c >= 'a' && c <= 'z'))
				c = shiftxform[c];
			eatkey = HUlib_keyInIText(&::g->w_chat, c);
			if (eatkey)
			{
				// static unsigned char buf[20]; // DEBUG
				HU_queueChatChar(c);

				// sprintf(buf, "KEY: %d => %d", ev->data1, c);
				//      ::g->plr->message = buf;
			}
			if (c == KEY_ENTER)
			{
				::g->chat_on = false;
				if (::g->w_chat.l.len)
				{
					strcpy(::g->lastmessage, ::g->w_chat.l.l);
					::g->plr->message = ::g->lastmessage;
				}
			}
			else if (c == KEY_ESCAPE)
				::g->chat_on = false;
		}
	}

	return eatkey;

}

