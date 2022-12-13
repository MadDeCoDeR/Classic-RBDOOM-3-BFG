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

// Functions.
#include "i_system.h"
#include "m_swap.h"
#include "z_zone.h"
#include "v_video.h"
#include "w_wad.h"
#include "s_sound.h"

// Data.
#include "dstrings.h"
#include "sounds.h"

#include "doomstat.h"
#include "r_state.h"

#include "Main.h"
#include "d3xp/Game_local.h"
#include "../framework/Common_local.h"
#include "d_exp.h"

// ?
//#include "doomstat.h"
//#include "r_local.h"
//#include "f_finale.h"

// Stage of animation:
//  0 = text, 1 = art screen, 2 = character cast


//GK: No constants
/*const*/ char*	e1text = (char*) E1TEXT;
/*const*/ char*	e2text = (char*) E2TEXT;
/*const*/ char*	e3text = (char*) E3TEXT;
/*const*/ char*	e4text = (char*) E4TEXT;

/*const*/ char*	c1text = (char*) C1TEXT;
/*const*/ char*	c2text = (char*) C2TEXT;
/*const*/ char*	c3text = (char*) C3TEXT;
/*const*/ char*	c4text = (char*) C4TEXT;
/*const*/ char*	c5text = (char*) C5TEXT;
/*const*/ char*	c6text = (char*) C6TEXT;
/*const*/ char* c7text = (char*) C7TEXT;
/*const*/ char* c8Text = (char*) C8TEXT;
/*const*/ char* c9Text = (char*) C9TEXT;

/*const*/ char*	p1text = (char*) P1TEXT;
/*const*/ char*	p2text = (char*) P2TEXT;
/*const*/ char*	p3text = (char*) P3TEXT;
/*const*/ char*	p4text = (char*) P4TEXT;
/*const*/ char*	p5text = (char*) P5TEXT;
/*const*/ char*	p6text = (char*) P6TEXT;

/*const*/ char*	t1text = (char*) T1TEXT;
/*const*/ char*	t2text = (char*) T2TEXT;
/*const*/ char*	t3text = (char*) T3TEXT;
/*const*/ char*	t4text = (char*) T4TEXT;
/*const*/ char*	t5text = (char*) T5TEXT;
/*const*/ char*	t6text = (char*) T6TEXT;

void resetEndings() {
	e1text = (char*) E1TEXT;
	e2text = (char*) E2TEXT;
    e3text = (char*) E3TEXT;
    e4text = (char*) E4TEXT;

	c1text = (char*) C1TEXT;
    c2text = (char*) C2TEXT;
	c3text = (char*) C3TEXT;
	c4text = (char*) C4TEXT;
	c5text = (char*) C5TEXT;
	c6text = (char*) C6TEXT;
	c7text = (char*) C7TEXT;
	c8Text = (char*) C8TEXT; 
	c9Text = (char*) C9TEXT;

	p1text = (char*) P1TEXT;
	p2text = (char*) P2TEXT;
	p3text = (char*) P3TEXT;
	p4text = (char*) P4TEXT;
	p5text = (char*) P5TEXT;
	p6text = (char*) P6TEXT;

	t1text = (char*) T1TEXT;
	t2text = (char*) T2TEXT;
	t3text = (char*) T3TEXT;
	t4text = (char*) T4TEXT;
	t5text = (char*) T5TEXT;
	t6text = (char*) T6TEXT;
}



const char*	finaletext;
char* flt;//GK:use this to retrive custom expansion finale flat name
int flatind = -1;
/*const*/ char*	finaleflat[] = { //GK: Linux got issue with brackets inside brackets for char**
	(char*) "FLOOR4_8",
 (char*) "SFLR6_1" ,
 (char*) "MFLR8_4" ,
 (char*) "MFLR8_3" ,
 (char*) "SLIME16" ,
 (char*) "RROCK14" ,
 (char*) "RROCK07" ,
 (char*) "RROCK17" ,
 (char*) "RROCK13" ,
 (char*) "RROCK19" ,
(char*) "F_SKY1",
(char*) "BOSSBACK" //GK:Make even the cast background modifiable by dehacked
};

void ResetFinalflat() {
	/*const*/ char*	tfinaleflat[] = {
		 (char*) "FLOOR4_8" ,
	 (char*) "SFLR6_1" ,
	 (char*) "MFLR8_4" ,
	 (char*) "MFLR8_3" ,
	 (char*) "SLIME16" ,
	 (char*) "RROCK14" ,
	 (char*) "RROCK07" ,
	 (char*) "RROCK17" ,
	 (char*) "RROCK13" ,
	 (char*) "RROCK19" ,
	 (char*) "F_SKY1" ,
	 (char*) "BOSSBACK" 
	};
	memcpy(finaleflat, tfinaleflat, sizeof(tfinaleflat));
}

void	F_StartCast (void);
void	F_CastTicker (void);
qboolean F_CastResponder (event_t *ev);
void	F_CastDrawer (void);
extern "C"
{
	void A_RandomJump(mobj_t* actor, pspdef_t* psp);
}

//
// F_StartFinale
//
void F_StartFinale (void)
{
    ::g->gameaction = ga_nothing;
    ::g->gamestate = GS_FINALE;
    ::g->viewactive = false;
    ::g->automapactive = false;
	int map = 0;

	// Check for end of episode/mission
	bool endOfMission = false;

	if ( ( ::g->gamemission == doom || ::g->gamemission == doom2 || ::g->gamemission == pack_tnt ||  ::g->gamemission == pack_plut  ) && ::g->gamemap == 30 ) {
		endOfMission = true;
	}
	else if ( ::g->gamemission == pack_nerve && ::g->gamemap == 8 ) {
		endOfMission = true;
	}
	else if (::g->gamemission == pack_master && ((::g->gamemap == 20 && !::g->secretexit) || ::g->gamemap == 21)) {
		endOfMission = true;
	}
	if (::g->gamemission == pack_custom ) { //GK: Custom expansion related stuff
		if (::g->gamemode == retail && (int)::g->clusters.size() >= ::g->gameepisode) {
			map = ::g->clusters[::g->gameepisode - 1].startmap + (::g->gamemap - 1);
			if (map == ::g->clusters[::g->gameepisode - 1].endmap) {
				endOfMission = true;
			}
		}
		else if (::g->gamemode == commercial) {
			if (::g->gamemap == ::g->endmap) {
				endOfMission = true;
			}
		}
	}

	localCalculateAchievements( endOfMission );

    // Okay - IWAD dependend stuff.
    // This has been changed severly, and
    //  some stuff might have changed in the process.
	//GK: just something for the lols
	const char* fooltext = BTLTEXT;
    switch ( ::g->gamemode )
    {

		// DOOM 1 - E1, E3 or E4, but each nine missions
		case shareware:
		case registered:
		case retail:
		{
			{
				S_ChangeMusic(mus_victor, true);

				switch (::g->gameepisode)
				{
				case 1:
					flatind = 0;
					finaletext = e1text;
					break;
				case 2:
					flatind = 1;
					finaletext = e2text;
					break;
				case 3:
					flatind = 2;
					finaletext = e3text;
					break;
				case 4:
					flatind = 3;
					finaletext = e4text;
					break;
				default:
					// Ouch.
					S_ChangeMusic(mus_victor, true);
					flt = finaleflat[10]; // Not used anywhere else.
					finaletext = fooltext;   //GK: NO finale text found??
					break;
				}
			}
			if (::g->gamemission == pack_custom) { //GK: Custom expansion related stuff
				if (::g->clusters[::g->gameepisode - 1].ftext != NULL) {
					S_ChangeMusic(::g->clusters[::g->gameepisode - 1].fmusic, true);
					if (::g->clusters[::g->gameepisode - 1].ftex != NULL) {
						flt = ::g->clusters[::g->gameepisode - 1].ftex;
					}
					else {
						flt = finaleflat[::g->clusters[::g->gameepisode - 1].fflat];
					}
					finaletext = ::g->clusters[::g->gameepisode - 1].ftext;
				} else if (getFinalText(::g->maps[map - 1].ftext)) {
					S_ChangeMusic(::g->maps[map - 1].fmusic, true);
					if (::g->maps[map - 1].fflatname != NULL) {
						flt = ::g->maps[map - 1].fflatname;
					}
					else {
						flt = finaleflat[::g->maps[map - 1].fflat];
					}
					finaletext = getFinalText(::g->maps[map - 1].ftext);
				}
			}
			break;
		}
      
		// DOOM II and missions packs with E1, M34
		case commercial:
		{
			if (::g->gamemission == pack_custom) { //GK: Custom expansion related stuff
				if (getFinalText(::g->maps[::g->gamemap - 1].ftext)) {
					S_ChangeMusic(::g->maps[::g->gamemap - 1].fmusic, true);
					if (::g->maps[::g->gamemap - 1].fflat > -1) { //SANITY CHECK
						flatind = ::g->maps[::g->gamemap - 1].fflat;
						flt = finaleflat[flatind];
					}
					else {
						flatind = 11;
						flt = ::g->maps[::g->gamemap - 1].fflatname;
					}
					finaletext = getFinalText(::g->maps[::g->gamemap - 1].ftext);
				}
				else {
					int c2 = 0;
					int c1 = ::g->maps[::g->gamemap - 1].cluster - 1;
					if (::g->wminfo.next < (int)::g->maps.size()) {
						c2 = ::g->maps[::g->wminfo.next].cluster - 1;
					}
					int c3 = ::g->maps[::g->endmap - 1].cluster - 1;
					if (::g->clusters.size() > 0 && c1 >=0) {
						if (c2 >= 0) {
							if (::g->clusters[c1].textpr >= ::g->clusters[c2].textpr) {
								S_ChangeMusic(::g->clusters[c1].fmusic, true);
								if (::g->clusters[c1].fflat < 0) {
									flt = ::g->clusters[c1].ftex;
								}
								else {
									flt = finaleflat[::g->clusters[c1].fflat]; // Not used anywhere else.
								}
								if (::g->clusters[c1].ftext != NULL) {
									finaletext = ::g->clusters[c1].ftext;
								}
								else {
									finaletext = fooltext; //GK: NO finale text found??
								}
							}
							else {
								S_ChangeMusic(::g->clusters[c2].fmusic, true);
								if (::g->clusters[c2].fflat < 0) {
									flt = ::g->clusters[c2].ftex;
								}
								else {
									flt = finaleflat[::g->clusters[c2].fflat]; // Not used anywhere else.
								}
								if (::g->clusters[c2].ftext != NULL) {
									finaletext = ::g->clusters[c2].ftext;
								}
								else {
									finaletext = fooltext; //GK: NO finale text found??
								}
							}
						}
						if (::g->gamemap == ::g->endmap) {
							S_ChangeMusic(::g->clusters[c3].fmusic, true);
							if (::g->clusters[c3].fflat < 0) {
								flt = ::g->clusters[c3].ftex;
							}
							else {
								flt = finaleflat[::g->clusters[c3].fflat]; // Not used anywhere else.
							}
							if (::g->clusters[c3].ftext != NULL) {
							finaletext = ::g->clusters[c3].ftext; 
							}
							else {
								finaletext = fooltext; //GK: NO finale text found??
							}
						
						}
					}
					else {//GK: NO finale??
						S_ChangeMusic(mus_read_m, true);
						flt = finaleflat[10]; // Not used anywhere else.
						finaletext = fooltext;   //GK: NO finale text found??
					}
				}
			}
			else {
				S_ChangeMusic(mus_read_m, true);

				switch (::g->gamemap)
				{
				case 6:
					flatind = 4;
					//GK: Show properly Evilution and Plutonia finale texts
					if (::g->gamemission == pack_tnt) {
						finaletext = t1text;
					}
					else if (::g->gamemission == pack_plut) {
						finaletext = p1text;
					}
					else if (::g->gamemission == doom2 || ::g->modftext) {
						finaletext = c1text;
					}
					break;
					//GK: Insert nerve and master levels cases here for simplicity
				case 8:
					if (::g->gamemission == pack_nerve && ::g->modind <= 0) {
						flatind = 4;
						finaletext = c7text;
					}
					else if (::g->gamemission == pack_nerve && ::g->modind > 0) {
						flatind = ::g->modind -1;
						switch (::g->modind) {
						case 1:
							finaletext = e1text;
							break;
						case 2:
							finaletext = e2text;
							break;
						case 3:
							finaletext = e3text;
							break;
						case 4:
							finaletext = e4text;
							break;
						}
					}
					break;
				case 11:
					flatind = 5;
					if (::g->gamemission == pack_tnt) {
						finaletext = t2text;
					}
					else if (::g->gamemission == pack_plut) {
						finaletext = p2text;
					}
					else if (::g->gamemission == doom2 || ::g->modftext) {
						finaletext = c2text;
					}
					break;
				case 20:
					flatind = 6;
					if (::g->gamemission == pack_tnt) {
						finaletext = t3text;
					}
					else if (::g->gamemission == pack_plut) {
						finaletext = p3text;
					}
					else if (::g->gamemission == pack_master) {
						flatind = 4;
						finaletext = c8Text;
					}
					else if (::g->gamemission == doom2 || ::g->modftext) {
						finaletext = c3text;
					}
					break;
				case 21:
					if (::g->gamemission == pack_master) {
						flatind = 4;
						finaletext = c9Text;
						if (::g->gameskill >= 2) { //GK: No reward for "Cry babies"
							if (com_allowConsole.GetInteger() == 0 && !::g->classiccheats) {
								doomit.SetInteger(1); // GK: Reward the player for finishing all the Master Levels by enabling the doom-it level selction for master levels
							}
						}

					}
					break;
				case 30:
					flatind = 7;
					if (::g->gamemission == pack_tnt) {
						finaletext = t4text;
					}
					else if (::g->gamemission == pack_plut) {
						finaletext = p4text;
					}
					else if (::g->gamemission == doom2 || ::g->modftext) {
						finaletext = c4text;
					}
					break;
				case 15:
					flatind = 8;
					if (::g->gamemission == pack_tnt) {
						finaletext = t5text;
					}
					else if (::g->gamemission == pack_plut) {
						finaletext = p5text;
					}
					else if (::g->gamemission == doom2 || ::g->modftext) {
						finaletext = c5text;
					}
					break;
				case 31:
					flatind = 9;
					if (::g->gamemission == pack_tnt) {
						finaletext = t6text;
					}
					else if (::g->gamemission == pack_plut) {
						finaletext = p6text;
					}
					else if (::g->gamemission == doom2 || ::g->modftext) {
						finaletext = c6text;
					}
					break;
				default:
					// Ouch.
					S_ChangeMusic(mus_read_m, true);
					flt = finaleflat[10]; // Not used anywhere else.
					finaletext = fooltext;   //GK: NO finale text found??
					break;
				}
			}
			break;
		}

		// Indeterminate.
		default:
			S_ChangeMusic(mus_read_m, true);
			flatind=10; // Not used anywhere else.
			finaletext = fooltext;  // FIXME - other text, music? GK: just give it the bootleg ending
			break;
	}
    
    ::g->finalestage = 0;
    ::g->finalecount = 0;
}


bool finaleButtonPressed = false;
bool startButtonPressed = false;
qboolean F_Responder (event_t *event)
{
	if( !common->IsMultiplayer() && event->type == ev_keydown && event->data1 == KEY_ESCAPE ) {
		startButtonPressed = true;
		return true;
	}

	if (::g->finalestage == 2)
		return F_CastResponder (event);

    return false;
}


//
// F_Ticker
//
void F_Ticker (void)
{
    int		i;
    
	// check for skipping
	int map = 0;
	bool keepRolling = false;
	if (::g->gamemode == retail && ::g->gamemission == pack_custom && (int)::g->clusters.size() >= ::g->gameepisode) {
		map = ::g->clusters[::g->gameepisode - 1].startmap + (::g->gamemap - 1);
		keepRolling = ::g->gamemission == pack_custom && ::g->clusters[::g->gameepisode - 1].startmap && map != ::g->clusters[::g->gameepisode - 1].endmap;
	}
	if ( (::g->gamemode == commercial || (::g->gamemode == retail && keepRolling)) && ( ::g->finalecount > 50) )
	{
		// go on to the next level
		for (i=0 ; i<MAXPLAYERS ; i++)
			if (::g->players[i].cmd.buttons)
				break;

		if ( finaleButtonPressed || i < MAXPLAYERS)
		{	
			bool castStarted = false;
			if( ::g->gamemission == doom2 || ::g->gamemission == pack_plut || ::g->gamemission == pack_tnt ) {
				if (::g->gamemap == 30) {
					F_StartCast ();
					castStarted = true;
				}

			} else if(  ::g->gamemission == pack_master ) {
				if (::g->gamemap == 20 && !::g->secretexit) {
					F_StartCast();
					castStarted = true;
				}else if( :: g->gamemap == 21 ) {
					F_StartCast ();
					castStarted = true;
				}

			} else if(  ::g->gamemission == pack_nerve ) {
				if( :: g->gamemap == 8 ) {
					F_StartCast ();
					castStarted = true;
				}

			}
			else if (::g->gamemission == pack_custom) { //GK: Custom expansion related stuff
				if (::g->gamemap == ::g->endmap) {
					F_StartCast();
					castStarted = true;
				}
			}

			if( castStarted == false ) {
				::g->gameaction = ga_worlddone;
			}
		}
	}

	bool SkipTheText = 	finaleButtonPressed;

    // advance animation
    ::g->finalecount++;
	finaleButtonPressed = false;
	
    if (::g->finalestage == 2)
    {
		F_CastTicker ();
		return;
    }
	
    if ( ::g->gamemode == commercial) {
		startButtonPressed = false;
		return;
	}
	
	if( SkipTheText && ( ::g->finalecount > 50) ) {
		::g->finalecount =  static_cast<int>(strlen(finaletext)) * TEXTSPEED + TEXTWAIT;
	}

    if (!::g->finalestage && ::g->finalecount > static_cast<int>(strlen(finaletext)) * TEXTSPEED + TEXTWAIT)
    {
		::g->finalecount = 0;
		::g->finalestage = 1;
		::g->wipegamestate = (gamestate_t)-1;		// force a wipe
		if (::g->gameepisode == 3)
		    S_StartMusic (mus_bunny);
    }

	startButtonPressed = false;

}



//
// F_TextWrite
//

#include "hu_stuff.h"
#include "m_random.h"


void F_TextWrite (void)
{
    byte*	src = NULL;
    byte*	dest;
    
    int		x,y,w;
    int		count;
    const char*	ch;
    int		c;
    int		cx;
    int		cy;
    
	if(::g->finalecount == 60 ) {
		DoomLib::ShowXToContinue( true );
	}

    // erase the entire screen to a tiled background
	if (flatind > -1) {
		src = (byte*)W_CacheLumpName(finaleflat[flatind], PU_CACHE_SHARED);
	}
	if (::g->gamemission == pack_custom && flt != NULL) { //GK: Custom expansion related stuff
		src = (byte*)W_CacheLumpName(flt, PU_CACHE_SHARED);
	}
    dest = ::g->screens[0];
	
    for (y=0 ; y<SCREENHEIGHT ; y++)
    {
	for (x=0 ; x< ::g->SCREENWIDTH/64 ; x++)
	{
	    memcpy (dest, src+((y&63)<<6), 64);
	    dest += 64;
	}
	if (::g->SCREENWIDTH&63)
	{
	    memcpy (dest, src+((y&63)<<6), ::g->SCREENWIDTH&63);
	    dest += (::g->SCREENWIDTH&63);
	}
    }

    V_MarkRect (0, 0, ::g->SCREENWIDTH, SCREENHEIGHT);
    
    // draw some of the text onto the screen
    cx = 10;
    cy = 10;
    ch = finaletext;
	
    count = (::g->finalecount - 10)/TEXTSPEED;
    if (count < 0)
	count = 0;
    for ( ; count ; count-- )
    {
	c = *ch++;
	if (!c)
	    break;
	//GK:For improved text parsing from the BEX text editor it replace the \\n with \n and \b and so it ignores the \b character
	if (c == '\b') {
		while (c == '\b') {
			c = *ch++;
		}
		c = *ch--;
		continue;
	}
	if (c == '\n')
	{
	    cx = 10;
	    cy += 11;
	    continue;
	}
		
	c = toupper(c) - HU_FONTSTART;
	if (c < 0 || c> HU_FONTSIZE)
	{
	    cx += 4;
	    continue;
	}
		
	w = SHORT (::g->hu_font[c]->width);
	if (cx+w > ::g->SCREENWIDTH)
	    break;
	V_DrawPatch(cx, cy, 0, ::g->hu_font[c], false);
	cx+=w;
    }
	
}

//
// Final DOOM 2 animation
// Casting by id Software.
//   in order of appearance
//
//GK:Make sure the modified names of the monsters are shown
castinfo_t	castorder[] = 
{
    {&CC_ZOMBIE, MT_POSSESSED},
    {&CC_SHOTGUN, MT_SHOTGUY},
    {&CC_HEAVY, MT_CHAINGUY},
    {&CC_IMP, MT_TROOP},
    {&CC_DEMON, MT_SERGEANT},
    {&CC_LOST, MT_SKULL},
    {&CC_CACO, MT_HEAD},
    {&CC_HELL, MT_KNIGHT},
    {&CC_BARON, MT_BRUISER},
    {&CC_ARACH, MT_BABY},
    {&CC_PAIN, MT_PAIN},
    {&CC_REVEN, MT_UNDEAD},
    {&CC_MANCU, MT_FATSO},
    {&CC_ARCH, MT_VILE},
    {&CC_SPIDER, MT_SPIDER},
    {&CC_CYBER, MT_CYBORG},
    {&CC_HERO, MT_PLAYER},

    {NULL,(mobjtype_t)0}
};



//
// F_StartCast
//


void F_StartCast (void)
{
	if ( ::g->finalestage != 2 ) {
		::g->wipegamestate = (gamestate_t)-1;		// force a screen wipe
		::g->castnum = 0;
		::g->caststate = &::g->states[mobjinfo[castorder[::g->castnum].type].seestate];
		::g->casttics = ::g->caststate->tics;
		::g->castdeath = false;
		::g->finalestage = 2;	
		::g->castframes = 0;
		::g->castonmelee = 0;
		::g->castattacking = false;
		S_ChangeMusic(mus_evil, true);

		::g->caststartmenu = ::g->finalecount + 50;
	}	
}

int F_CastRandomJump(state_t* frame) {
	if ((frame->action == (actionf_p2)A_RandomJump) && M_Random() < frame->misc2) {
		return frame->misc1;
	}
	else {
		return frame->nextstate;
	}
}

//
// F_CastTicker
//
void F_CastTicker (void)
{
    int		st;
    int		sfx;

	if( ::g->finalecount == ::g->caststartmenu ) {
		DoomLib::ShowXToContinue( true );
	}

    if (--::g->casttics > 0)
	return;			// not time to change state yet
		
    if (::g->caststate->tics == -1 || ::g->caststate->nextstate == S_NULL || ::g->castnum < 0)
    {
	// switch from deathstate to next monster
	::g->castnum++;
	::g->castdeath = false;
	if (::g->castnum >= (sizeof(castorder) / sizeof(castinfo_t))) {
		::g->castnum = (sizeof(castorder) / sizeof(castinfo_t)) - 1;
	}
	
	if (castorder[::g->castnum].name == NULL) {
		if (!::g->castcredit) {
			::g->castcredit = true;
		}
		return;
	}
	if (mobjinfo[castorder[::g->castnum].type].seesound)
	    S_StartSound (NULL, mobjinfo[castorder[::g->castnum].type].seesound);
	::g->caststate = &::g->states[mobjinfo[castorder[::g->castnum].type].seestate];
	::g->castframes = 0;
    }
    else
    {
	// just advance to next state in animation
	if (!::g->castdeath && ::g->caststate == &::g->states[S_PLAY_ATK1])
	    goto stopattack;	// Oh, gross hack!
	st = F_CastRandomJump( ::g->caststate);
	::g->caststate = &::g->states[st];
	::g->castframes++;
	
	// sound hacks....
	switch (st)
	{
	  case S_PLAY_ATK1:	sfx = sfx_dshtgn; break;
	  case S_POSS_ATK2:	sfx = sfx_pistol; break;
	  case S_SPOS_ATK2:	sfx = sfx_shotgn; break;
	  case S_VILE_ATK2:	sfx = sfx_vilatk; break;
	  case S_SKEL_FIST2:	sfx = sfx_skeswg; break;
	  case S_SKEL_FIST4:	sfx = sfx_skepch; break;
	  case S_SKEL_MISS2:	sfx = sfx_skeatk; break;
	  case S_FATT_ATK8:
	  case S_FATT_ATK5:
	  case S_FATT_ATK2:	sfx = sfx_firsht; break;
	  case S_CPOS_ATK2:
	  case S_CPOS_ATK3:
	  case S_CPOS_ATK4:	sfx = sfx_shotgn; break;
	  case S_TROO_ATK3:	sfx = sfx_claw; break;
	  case S_SARG_ATK2:	sfx = sfx_sgtatk; break;
	  case S_BOSS_ATK2:
	  case S_BOS2_ATK2:
	  case S_HEAD_ATK2:	sfx = sfx_firsht; break;
	  case S_SKULL_ATK2:	sfx = sfx_sklatk; break;
	  case S_SPID_ATK2:
	  case S_SPID_ATK3:	sfx = sfx_shotgn; break;
	  case S_BSPI_ATK2:	sfx = sfx_plasma; break;
	  case S_CYBER_ATK2:
	  case S_CYBER_ATK4:
	  case S_CYBER_ATK6:	sfx = sfx_rlaunc; break;
	  case S_PAIN_ATK3:	sfx = sfx_sklatk; break;
	  default: sfx = 0; break;
	}
		
	if (sfx)
	    S_StartSound (NULL, sfx);
    }
	
    if (!::g->castdeath && ::g->castframes == 12)
    {
	// go into attack frame
	::g->castattacking = true;
	if (::g->castonmelee)
	    ::g->caststate=&::g->states[mobjinfo[castorder[::g->castnum].type].meleestate];
	else
	    ::g->caststate=&::g->states[mobjinfo[castorder[::g->castnum].type].missilestate];
	::g->castonmelee ^= 1;
	if (::g->caststate == &::g->states[S_NULL])
	{
	    if (::g->castonmelee)
		::g->caststate=
		    &::g->states[mobjinfo[castorder[::g->castnum].type].meleestate];
	    else
		::g->caststate=
		    &::g->states[mobjinfo[castorder[::g->castnum].type].missilestate];
	}
    }
	
    if (::g->castattacking)
    {
	if (::g->castframes == 24
	    ||	::g->caststate == &::g->states[mobjinfo[castorder[::g->castnum].type].seestate] )
	{
	  stopattack:
	    ::g->castattacking = false;
	    ::g->castframes = 0;
	    ::g->caststate = &::g->states[mobjinfo[castorder[::g->castnum].type].seestate];
	}
    }
	
    ::g->casttics = ::g->caststate->tics;
	if (::g->casttics == -1)
	{
		if (F_CastRandomJump(::g->caststate)) {
			::g->caststate = &::g->states[F_CastRandomJump(::g->caststate)];
			::g->casttics = ::g->caststate->tics;
		}

		if (::g->casttics == -1)
			::g->casttics = 15;
	}
}


//
// F_CastResponder
//

qboolean F_CastResponder (event_t* ev)
{
    if (ev->type != ev_keydown)
	return false;
		
    if (::g->castdeath)
	return true;			// already in dying frames

	if (::g->castcredit) {
		::g->castnum = -1;
		::g->castcredit = false;
		return true;
	}
		
    // go into death frame
    ::g->castdeath = true;
    ::g->caststate = &::g->states[mobjinfo[castorder[::g->castnum].type].deathstate];
    ::g->casttics = ::g->caststate->tics;
	if (::g->casttics == -1)
	{
		::g->caststate = &::g->states[F_CastRandomJump(::g->caststate)];
		::g->casttics = ::g->caststate->tics;
	}
    ::g->castframes = 0;
    ::g->castattacking = false;
    if (mobjinfo[castorder[::g->castnum].type].deathsound)
	S_StartSound (NULL, mobjinfo[castorder[::g->castnum].type].deathsound);
	
    return true;
}


void F_CastPrint (const char* text)
{
    const char*	ch;
    int		c;
    int		cx;
    int		w;
    int		width;
    
    // find width
    ch = text;
    width = 0;
	
    while (ch)
    {
	c = *ch++;
	if (!c)
	    break;
	c = toupper(c) - HU_FONTSTART;
	if (c < 0 || c> HU_FONTSIZE)
	{
	    width += 4;
	    continue;
	}
		
	w = SHORT (::g->hu_font[c]->width);
	width += w;
    }
    
    // draw it
    cx = 160-width/2;
    ch = text;
    while (ch)
    {
	c = *ch++;
	if (!c)
	    break;
	c = toupper(c) - HU_FONTSTART;
	if (c < 0 || c> HU_FONTSIZE)
	{
	    cx += 4;
	    continue;
	}
		
	w = SHORT (::g->hu_font[c]->width);
	V_DrawPatch(cx, 180, 0, ::g->hu_font[c], false);
	cx+=w;
    }
	
}


//
// F_CastDrawer
//
void V_DrawPatchFlipped (int x, int y, int scrn, patch_t *patch);

void F_CastDrawer (void)
{
    spritedef_t*	sprdef;
    spriteframe_t*	sprframe;
    int			lump;
    qboolean		flip;
    patch_t*		patch;

	if (::g->castcredit) {
		V_DrawPatch(0, 0, 0, /*(patch_t*)*/img2lmp(W_CacheLumpName("CREDIT", PU_CACHE_SHARED), W_GetNumForName("CREDIT")), false);
		return;
	}

	if (::g->castnum < 0)
		return;
    
    // erase the entire screen to a background
    V_DrawPatch (0,0,0, /*(patch_t*)*/img2lmp(W_CacheLumpName (finaleflat[11], PU_CACHE_SHARED), W_GetNumForName(finaleflat[11])), false);

    F_CastPrint (*castorder[::g->castnum].name);
    
    // draw the current frame in the middle of the screen
    sprdef = ::g->sprites[::g->caststate->sprite];
    sprframe = &sprdef->spriteframes[ ::g->caststate->frame & FF_FRAMEMASK];
    lump = sprframe->lump[0];
    flip = (qboolean)sprframe->flip[0];
			
    patch =  /*(patch_t*)*/img2lmp(W_CacheLumpNum (lump+::g->firstspritelump, PU_CACHE_SHARED),lump + ::g->firstspritelump);
    if (flip)
		V_DrawPatchFlipped (160,170,0,patch);
    else
		V_DrawPatch (160,170,0,patch, false);
}


//
// F_DrawPatchCol
//
void
F_DrawPatchCol( int x, patch_t* patch, int col ) {
    postColumn_t*	column;
    byte*			source;
    int				count;
	
	column = (postColumn_t *)((byte *)patch + LONG(patch->columnofs[col]));

	int destx = x;
	int desty = 0;

    // step through the posts in a column
    while (column->topdelta != 0xff )
    {
		source = (byte *)column + 3;
		desty = column->topdelta;
		count = column->length;
			
		while (count--)
		{
			int scaledx, scaledy;
			scaledx = destx * ::g->ASPECT_IMAGE_SCALER;
			scaledy = desty * GLOBAL_IMAGE_SCALER;
			byte src = *source++;

			for ( int i = 0; i < GLOBAL_IMAGE_SCALER; i++ ) {
				for ( int j = 0; j < ::g->ASPECT_IMAGE_SCALER; j++ ) {
					::g->screens[0][( scaledx + j ) + ( scaledy + i ) * ::g->SCREENWIDTH] = src;
				}
			}

			desty++;
		}
		column = (postColumn_t *)(  (byte *)column + column->length + 4 );
    }
}


//
// F_BunnyScroll
//
void F_BunnyScroll (void)
{
    int		scrolled;
    int		x;
    patch_t*	p1;
    patch_t*	p2;
    char	name[13];
    int		stage;
		
    p1 =  /*(patch_t*)*/img2lmp(W_CacheLumpName ("PFUB2", PU_FINALE), W_GetNumForName("PFUB2"));
    p2 =  /*(patch_t*)*/img2lmp(W_CacheLumpName ("PFUB1", PU_FINALE), W_GetNumForName("PFUB1"));

    V_MarkRect (0, 0, ::g->SCREENWIDTH, SCREENHEIGHT);
	
    scrolled = 320 - (::g->finalecount-230)/2;
    if (scrolled > 320)
	scrolled = 320;
    if (scrolled < 0)
	scrolled = 0;
		
    for ( x=0 ; x<ORIGINAL_WIDTH ; x++)
    {
	if (x+scrolled < 320)
	    F_DrawPatchCol (x, p1, x+scrolled);
	else
	    F_DrawPatchCol (x, p2, x+scrolled - 320);		
    }
	
    if (::g->finalecount < 1130)
	return;
    if (::g->finalecount < 1180)
    {
	V_DrawPatch ((ORIGINAL_WIDTH-13*8)/2,
		     (ORIGINAL_HEIGHT-8*8)/2,0,  /*(patch_t*)*/img2lmp(W_CacheLumpName ("END0",PU_CACHE_SHARED), W_GetNumForName("END0")), false);
	::g->laststage = 0;
	return;
    }
	
    stage = (::g->finalecount-1180) / 5;
    if (stage > 6)
	stage = 6;
    if (stage > ::g->laststage)
    {
	S_StartSound (NULL, sfx_pistol);
	::g->laststage = stage;
    }
	
    sprintf (name,"END%i",stage);
    V_DrawPatch ((ORIGINAL_WIDTH-13*8)/2, (ORIGINAL_HEIGHT-8*8)/2,0,  /*(patch_t*)*/img2lmp(W_CacheLumpName (name,PU_CACHE_SHARED), W_GetNumForName(name)), false);
}


//
// F_Drawer
//
void F_Drawer (void)
{
    if (::g->finalestage == 2)
    {
	F_CastDrawer ();
	return;
    }

    if (!::g->finalestage)
	F_TextWrite ();
    else
    {
		int ending = ::g->gameepisode;
		if (::g->gamemission == pack_custom && ::g->clusters[::g->gameepisode -1].endmode > 0){
			ending = ::g->clusters[::g->gameepisode - 1].endmode;
		}
	switch (ending)
	{
	  case 1:
	    if ( ::g->gamemode == retail )
	      V_DrawPatch (0,0,0,
			  /*(patch_t*)*/img2lmp(W_CacheLumpName("CREDIT",PU_CACHE_SHARED), W_GetNumForName("CREDIT")), false);
	    else
	      V_DrawPatch (0,0,0,
			  /*(patch_t*)*/img2lmp(W_CacheLumpName("HELP2",PU_CACHE_SHARED), W_GetNumForName("HELP2")), false);
	    break;
	  case 2:
	    V_DrawPatch(0,0,0,
			/*(patch_t*)*/img2lmp(W_CacheLumpName("VICTORY2",PU_CACHE_SHARED), W_GetNumForName("VICTORY2")), false);
	    break;
	  case 3:
	    F_BunnyScroll ();
	    break;
	  case 4:
	    V_DrawPatch (0,0,0,
			 /*(patch_t*)*/img2lmp(W_CacheLumpName("ENDPIC",PU_CACHE_SHARED), W_GetNumForName("ENDPIC")), false);
	    break;
	}
    }
			
}



