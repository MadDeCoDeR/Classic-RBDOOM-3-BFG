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

#include "doomdef.h"
#include "d_event.h"


#include "m_random.h"
#include "p_local.h"
#include "s_sound.h"

// State.
#include "doomstat.h"

// Data.
#include "sounds.h"

#include "p_pspr.h"
#include "d3xp/Game_local.h"

extern bool globalNetworking;
extern idCVar cl_freelook;


static const float	PISTOL_MAGNITUDE_HIGH			= 0.5f;
static const int	PISTOL_DURATION_HIGH			= 250;
static const float	PISTOL_MAGNITUDE_LOW			= 1.0f;
static const int	PISTOL_DURATION_LOW				= 150;

static const float	SHOTGUN_MAGNITUDE_HIGH			= 0.5f;
static const int	SHOTGUN_DURATION_HIGH			= 250;
static const float	SHOTGUN_MAGNITUDE_LOW			= 1.0f;
static const int	SHOTGUN_DURATION_LOW			= 350;

static const float	CHAINGUN_MAGNITUDE_HIGH			= 0.5f;
static const int	CHAINGUN_DURATION_HIGH			= 250;
static const float	CHAINGUN_MAGNITUDE_LOW			= 1.0f;
static const int	CHAINGUN_DURATION_LOW			= 150;

static const float	PLASMAGUN_MAGNITUDE_HIGH		= 0.5f;
static const int	PLASMAGUN_DURATION_HIGH			= 250;
static const float	PLASMAGUN_MAGNITUDE_LOW			= 1.0f;
static const int	PLASMAGUN_DURATION_LOW			= 150;

static const float	SUPERSHOTGUN_MAGNITUDE_HIGH		= 1.0f;
static const int	SUPERSHOTGUN_DURATION_HIGH		= 250;
static const float	SUPERSHOTGUN_MAGNITUDE_LOW		= 1.0f;
static const int	SUPERSHOTGUN_DURATION_LOW		= 350;

static const float	ROCKET_MAGNITUDE_HIGH			= 1.5f;
static const int	ROCKET_DURATION_HIGH			= 250;
static const float	ROCKET_MAGNITUDE_LOW			= 1.0f;
static const int	ROCKET_DURATION_LOW				= 350;

static const float	BFG_MAGNITUDE_HIGH				= 1.5f;
static const int	BFG_DURATION_HIGH				= 250;
static const float	BFG_MAGNITUDE_LOW				= 1.0f;
static const int	BFG_DURATION_LOW				= 400;


static const float	SAW_IDL_MAGNITUDE_HIGH			= 0.0f;
static const int	SAW_IDL_DURATION_HIGH			= 0;
static const float	SAW_IDL_MAGNITUDE_LOW			= 0.4f;
static const int	SAW_IDL_DURATION_LOW			= 150;

static const float	SAW_ATK_MAGNITUDE_HIGH			= 1.0f;
static const int	SAW_ATK_DURATION_HIGH			= 250;
static const float	SAW_ATK_MAGNITUDE_LOW			= 0.0f;
static const int	SAW_ATK_DURATION_LOW			= 0;

// plasma cells for a bfg attack


//
// P_SetPsprite
//
void
P_SetPsprite
( player_t*	player,
 int		position,
 int	stnum ) 
{
	pspdef_t*	psp;
	const state_t*	state;

	psp = &player->psprites[position];

	do
	{
		if (!stnum)
		{
			// object removed itself
			psp->state = NULL;
			break;	
		}

		state = &::g->states[stnum];
		psp->state = state;
		psp->tics = state->tics;	// could be 0

		if (state->misc1)
		{
			// coordinate set
			psp->sx = state->misc1 << FRACBITS;
			psp->sy = state->misc2 << FRACBITS;
		}

		// Call action routine.
		// Modified handling.
		if (const actionf_p2* action_p2 = std::get_if<actionf_p2>(&state->action))
		{
			(*action_p2)(player, psp);
			if (!psp->state)
				break;
		}
		stnum = psp->state->nextstate;

	} while (!psp->tics);
	// an initial state of 0 could cycle through
}



//
// P_CalcSwing
//	

void P_CalcSwing (player_t*	player)
{
	fixed_t	swing;
	int		angle;

	// OPTIMIZE: tablify this.
	// A LUT would allow for different modes,
	//  and add flexibility.

	swing = player->bob;

	angle = (FINEANGLES/70*::g->leveltime)&FINEMASK;
	::g->swingx = FixedMul ( swing, finesine[angle]);

	angle = (FINEANGLES/70*::g->leveltime+FINEANGLES/2)&FINEMASK;
	::g->swingy = -FixedMul ( ::g->swingx, finesine[angle]);
}



//
// P_BringUpWeapon
// Starts bringing the pending weapon up
// from the bottom of the screen.
// Uses player
//
void P_BringUpWeapon (player_t* player)
{
	int	newstate;

	if (player->pendingweapon == wp_nochange)
		player->pendingweapon = player->readyweapon;

	if (player->pendingweapon == wp_chainsaw && (globalNetworking || (player == &::g->players[::g->consoleplayer])) )
		S_StartSound (player->mo, sfx_sawup);

	newstate = (weaponinfo[player->pendingweapon].upstate);

	player->pendingweapon = wp_nochange;
	player->psprites[ps_weapon].sy = WEAPONBOTTOM;

	P_SetPsprite (player, ps_weapon, newstate);
}

//
// P_CheckAmmo
// Returns true if there is enough ammo to shoot.
// If not, selects the next weapon to use.
//
qboolean P_CheckAmmo (player_t* player)
{
	ammotype_t		ammo;
	int			count;

	ammo = weaponinfo[player->readyweapon].ammo;

	// Minimal amount for one shot varies.
	if (player->readyweapon == wp_bfg && ::g->BFGCELL != 40){
		weaponinfo[wp_bfg].clipAmmo = ::g->BFGCELL;
		count = ::g->BFGCELL;
	}
	else
		count = weaponinfo[player->readyweapon].clipAmmo;	// Regular.

	// Some do not need ammunition anyway.
	// Return if current ammunition sufficient.
	if (ammo == am_noammo || player->ammo[ammo] >= count)
		return true;

	// Out of ammo, pick a weapon to change to.
	// Preferences are set here.
	do
	{
		if (player->weaponowned[wp_plasma]
		&& player->ammo[am_cell]>weaponinfo[wp_plasma].clipAmmo
		&& (::g->gamemode != shareware) )
		{
			player->pendingweapon = wp_plasma;
		}
		else if (player->weaponowned[wp_supershotgun] 
		&& player->ammo[am_shell]>weaponinfo[wp_supershotgun].clipAmmo
			&& (::g->gamemode == commercial) )
		{
			player->pendingweapon = wp_supershotgun;
		}
		else if (player->weaponowned[wp_chaingun]
		&& player->ammo[am_clip]>weaponinfo[wp_chaingun].clipAmmo)
		{
			player->pendingweapon = wp_chaingun;
		}
		else if (player->weaponowned[wp_shotgun]
		&& player->ammo[am_shell]>weaponinfo[wp_shotgun].clipAmmo)
		{
			player->pendingweapon = wp_shotgun;
		}
		else if (player->ammo[am_clip]>weaponinfo[wp_pistol].clipAmmo)
		{
			player->pendingweapon = wp_pistol;
		}
		else if (player->weaponowned[wp_chainsaw])
		{
			player->pendingweapon = wp_chainsaw;
		}
		else if (player->weaponowned[wp_missile]
		&& player->ammo[am_misl]>weaponinfo[wp_missile].clipAmmo)
		{
			player->pendingweapon = wp_missile;
		}
		else if (player->weaponowned[wp_bfg]
		&& player->ammo[am_cell]>weaponinfo[wp_bfg].clipAmmo
			&& (::g->gamemode != shareware) )
		{
			player->pendingweapon = wp_bfg;
		}
		else
		{
			// If everything fails.
			player->pendingweapon = wp_fist;
		}

	} while (player->pendingweapon == wp_nochange);

	// Now set appropriate weapon overlay.
	P_SetPsprite (player,
		ps_weapon,
		(weaponinfo[player->readyweapon].downstate));

	return false;	
}


//
// P_FireWeapon.
//
void P_FireWeapon (player_t* player)
{
	int	newstate;

	if (!P_CheckAmmo (player))
		return;

	P_SetMobjState (player->mo, S_PLAY_ATK1);
	newstate = weaponinfo[player->readyweapon].atkstate;
	P_SetPsprite (player, ps_weapon, newstate);
	P_NoiseAlert (player->mo, player->mo);

	if (player->readyweapon == wp_chainsaw )
	{	
		if( ::g->plyr == player ) {
		}
	}

}



//
// P_DropWeapon
// Player died, so put the weapon away.
//
void P_DropWeapon (player_t* player)
{
	P_SetPsprite (player,
		ps_weapon,
		weaponinfo[player->readyweapon].downstate);
}

int P_RandomHitscanSlope(fixed_t spread)
{
	int angle;

	angle = (FixedToAngle(abs(spread)) * (P_Random() - P_Random())) / 255;

	// clamp it, yo
	if (angle > ANG90)
		return finetangent[0];
	else if (-angle > ANG90)
		return finetangent[FINEANGLES / 2 - 1];
	else
		return finetangent[(ANG90 - angle) >> ANGLETOFINESHIFT];
}

extern "C" {
//
// A_WeaponReady
// The player can fire the weapon
// or change to another weapon at this time.
// Follows after getting weapon up,
// or after previous attack/fire sequence.
//
void
A_WeaponReady
( player_t*	player,
 pspdef_t*	psp )
{	
	player->CGunShoots = 0;
	int	newstate;
	int		angle;
	if (!psp) //GK:SANITY CHECK
		return;
	// get out of attack state
	if (player->mo->state == &::g->states[S_PLAY_ATK1]
	|| player->mo->state == &::g->states[S_PLAY_ATK2] )
	{
		P_SetMobjState (player->mo, S_PLAY);
	}

	if (player->readyweapon == wp_chainsaw
		&& psp->state == &::g->states[S_SAW])
	{
		if (globalNetworking || (player == &::g->players[::g->consoleplayer]))
			S_StartSound (player->mo, sfx_sawidl);
	}

	// check for change
	//  if player is dead, put the weapon away
	if (player->pendingweapon != wp_nochange || !player->health)
	{
		// change weapon
		//  (pending weapon should allready be validated)
		newstate = weaponinfo[player->readyweapon].downstate;
		P_SetPsprite (player, ps_weapon, newstate);
		return;	
	}

	// check for fire
	//  the missile launcher and bfg do not auto fire
	if (player->cmd.buttons & BT_ATTACK)
	{
		if ( !player->attackdown
			|| !(weaponinfo[player->readyweapon].flags & WPF_NOAUTOFIRE) )
		{
			player->attackdown = true;
			P_FireWeapon (player);		
			return;
		}
	}
	else
		player->attackdown = false;

	// bob the weapon based on movement speed
	angle = (128*::g->leveltime)&FINEMASK;
	psp->sx = FRACUNIT + FixedMul (player->bob, finecosine[angle]);
	angle &= FINEANGLES/2-1;
	psp->sy = WEAPONTOP + FixedMul (player->bob, finesine[angle]);
}



//
// A_ReFire
// The player can re-fire the weapon
// without lowering it entirely.
//
void A_ReFire
( player_t*	player,
 pspdef_t*	psp )
{

	// check for fire
	//  (if a weaponchange is pending, let it go through instead)
	if ( (player->cmd.buttons & BT_ATTACK) 
		&& player->pendingweapon == wp_nochange
		&& player->health)
	{
		player->refire++;
		P_FireWeapon (player);
	}
	else
	{
		player->refire = 0;
		P_CheckAmmo (player);
	}
}


void
A_CheckReload
( player_t*	player,
 pspdef_t*	psp )
{
	P_CheckAmmo (player);
#if 0
	if (player->ammo[am_shell]<2)
		P_SetPsprite (player, ps_weapon, S_DSNR1);
#endif
}



//
// A_Lower
// Lowers current weapon,
//  and changes weapon at bottom.
//
void
A_Lower
( player_t*	player,
 pspdef_t*	psp )
{	
	psp->sy += LOWERSPEED;

	// Is already down.
	if (psp->sy < WEAPONBOTTOM )
		return;

	// Player is dead.
	if (player->playerstate == PST_DEAD)
	{
		psp->sy = WEAPONBOTTOM;

		// don't bring weapon back up
		return;		
	}

	// The old weapon has been lowered off the screen,
	// so change the weapon and start raising it
	if (!player->health)
	{
		// Player is dead, so keep the weapon off screen.
		P_SetPsprite (player,  ps_weapon, S_NULL);
		return;	
	}
	if (player->readyweapon == wp_supershotgun) { //GK: supershotgun is using the same slot as the shotgun
		::g->weaponcond[wp_shotgun] = 1; //GK: No longer in use just keep owning it
	}
	else {
		::g->weaponcond[player->readyweapon] = 1; //GK: No longer in use just keep owning it
	}
	player->readyweapon = player->pendingweapon;
	//GK: The weapon is ready set it on use
	if (player->readyweapon == wp_supershotgun) { //GK: supershotgun is using the same slot as the shotgun
		::g->weaponcond[wp_shotgun] = 2;
	}
	else {
		::g->weaponcond[player->readyweapon] = 2;
	}

	P_BringUpWeapon (player);
}


//
// A_Raise
//
void
A_Raise
( player_t*	player,
 pspdef_t*	psp )
{
	int	newstate;

	psp->sy -= RAISESPEED;

	if (psp->sy > WEAPONTOP )
		return;

	psp->sy = WEAPONTOP;

	// The weapon has been raised all the way,
	//  so change to the ready state.
	newstate = weaponinfo[player->readyweapon].readystate;

	P_SetPsprite (player, ps_weapon, newstate);
}



//
// A_GunFlash
//
void
A_GunFlash
( player_t*	player,
 pspdef_t*	psp ) 
{
	P_SetMobjState (player->mo, S_PLAY_ATK2);
	P_SetPsprite (player,ps_flash,weaponinfo[player->readyweapon].flashstate);
}



//
// WEAPON ATTACKS
//


//
// A_Punch
//
void
A_Punch
( player_t*	player,
 pspdef_t*	psp ) 
{
	angle_t	angle;
	int		damage;
	int		slope;

	damage = (P_Random ()%10+1)<<1;

	if (player->powers[pw_strength])	
		damage *= 10;

	angle = player->mo->angle;
	angle += (P_Random()-P_Random())<<18;
	slope = P_AimLineAttack (player->mo, angle, player->mo->info->meleeRange);
	//GK: Move puffs up and down based on player's view
	if (cl_freelook.GetBool() && !::g->demorecording && ::g->gamestate != GS_DEMOLEVEL)
	{
		angle -= 2 << 26;
		slope = -(((::g->mouseposy) << FRACBITS) / (::g->SCREENHEIGHT - (127 * (::g->GLOBAL_IMAGE_SCALER/ 3.0f))));
	}
	P_LineAttack (player->mo, angle, player->mo->info->meleeRange, slope, damage);

	// turn to face target
	if (::g->linetarget)
	{
		S_StartSound (player->mo, sfx_punch);
		player->mo->angle = R_PointToAngle2 (player->mo->x,
			player->mo->y,
			::g->linetarget->x,
			::g->linetarget->y);
	}
}


//
// A_Saw
//
void
A_Saw
( player_t*	player,
 pspdef_t*	psp ) 
{
	angle_t	angle;
	int		damage;
	int		slope;

	damage = 2*(P_Random ()%10+1);
	angle = player->mo->angle;
	angle += (P_Random()-P_Random())<<18;

	// use meleerange + 1 se the puff doesn't skip the flash
	slope = P_AimLineAttack (player->mo, angle, player->mo->info->meleeRange+1);
	//GK: Move puffs up and down based on player's view
	if (cl_freelook.GetBool() && !::g->demorecording && ::g->gamestate != GS_DEMOLEVEL)
	{
		angle -= 2 << 26;
		slope = -(((::g->mouseposy) << FRACBITS) / (::g->SCREENHEIGHT - (127 * (::g->GLOBAL_IMAGE_SCALER/ 3.0f))));
	}
	P_LineAttack (player->mo, angle, player->mo->info->meleeRange+1, slope, damage);

	if (!::g->linetarget)
	{
		if (globalNetworking || (player == &::g->players[::g->consoleplayer]))
			S_StartSound (player->mo, sfx_sawful);
		return;
	}
	if (globalNetworking || (player == &::g->players[::g->consoleplayer]))
		S_StartSound (player->mo, sfx_sawhit);

	// turn to face target
	angle = R_PointToAngle2 (player->mo->x, player->mo->y,
		::g->linetarget->x, ::g->linetarget->y);
	if (angle - player->mo->angle > ANG180)
	{
		if (angle - player->mo->angle < (0 - ANG90)/20)//GKHACK W2 UNSIGNED FROM DOOMWORD FORUMS (circa 2009)
			player->mo->angle = angle + ANG90/21;
		else
			player->mo->angle -= ANG90/20;
	}
	else
	{
		if (angle - player->mo->angle > ANG90/20)
			player->mo->angle = angle - ANG90/21;
		else
			player->mo->angle += ANG90/20;
	}
	player->mo->flags |= MF_JUSTATTACKED;
}



//
// A_FireMissile
//
void
A_FireMissile
( player_t*	player,
 pspdef_t*	psp ) 
{
	if( (player->cheats & CF_INFAMMO) == false ) {
		player->ammo[weaponinfo[player->readyweapon].ammo] -= weaponinfo[player->readyweapon].clipAmmo;
	}
	P_SpawnPlayerMissile (player->mo, MT_ROCKET);

	if( ::g->plyr == player ) {
	}

}


//
// A_FireBFG
//
void
A_FireBFG
( player_t*	player,
 pspdef_t*	psp ) 
{
	if( (player->cheats & CF_INFAMMO) == false ) {
		player->ammo[weaponinfo[player->readyweapon].ammo] -= weaponinfo[player->readyweapon].clipAmmo;
	}

	P_SpawnPlayerMissile (player->mo, MT_BFG);
	if (!player->inBFGStates) {
		player->inBFGStates = true;
		player->bfgTargets = 0;
	}
	if( ::g->plyr == player ) {
	}
}



//
// A_FirePlasma
//
void
A_FirePlasma
( player_t*	player,
 pspdef_t*	psp ) 
{	
	if( (player->cheats & CF_INFAMMO) == false ) {
		player->ammo[weaponinfo[player->readyweapon].ammo] -= weaponinfo[player->readyweapon].clipAmmo;
	}

	P_SetPsprite (player,
		ps_flash,
		(weaponinfo[player->readyweapon].flashstate+(P_Random ()&1)) );

	P_SpawnPlayerMissile (player->mo, MT_PLASMA);

	if( ::g->plyr == player ) {
	}
}



//
// P_BulletSlope
// Sets a slope so a near miss is at aproximately
// the height of the intended target
//


void P_BulletSlope (mobj_t*	mo)
{
	angle_t	an;

	// see which target is to be aimed at
	an = mo->angle;
	::g->bulletslope = P_AimLineAttack (mo, an, 16*64*FRACUNIT);

	if (!::g->linetarget)
	{
		an += 1<<26;
		::g->bulletslope = P_AimLineAttack (mo, an, 16*64*FRACUNIT);
		if (!::g->linetarget)
		{
			an -= 2<<26;
			::g->bulletslope = P_AimLineAttack (mo, an, 16*64*FRACUNIT);
		}
	}
	//GK: Move puffs up and down based on player's view
	if (cl_freelook.GetBool() && !::g->demoplayback && !::g->demorecording && mo == ::g->viewplayer->mo)
	{
		if ((game->GetCVarBool("aa_targetAimAssistEnable") && !::g->linetarget) || !game->GetCVarBool("aa_targetAimAssistEnable")) {
			an -= 2 << 26;
			::g->bulletslope = -(((::g->mouseposy) << FRACBITS) / (::g->SCREENHEIGHT - (127 * (::g->GLOBAL_IMAGE_SCALER/ 3.0f))));
		}
	}
}


//
// P_GunShot
//
void
P_GunShot
( mobj_t*	mo,
 qboolean	accurate )
{
	angle_t	angle;
	int		damage;

	damage = 5*(P_Random ()%3+1);
	angle = mo->angle;

	if (!accurate)
		angle += (P_Random()-P_Random())<<18;

	P_LineAttack (mo, angle, MISSILERANGE, ::g->bulletslope, damage);
}


//
// A_FirePistol
//
void
A_FirePistol
( player_t*	player,
 pspdef_t*	psp ) 
{
	if (globalNetworking || (player == &::g->players[::g->consoleplayer]))
		S_StartSound (player->mo, sfx_pistol);

	P_SetMobjState (player->mo, S_PLAY_ATK2);
	if( (player->cheats & CF_INFAMMO ) == false ) {
		player->ammo[weaponinfo[player->readyweapon].ammo] -= weaponinfo[player->readyweapon].clipAmmo;
	}

	P_SetPsprite (player,
		ps_flash,
		weaponinfo[player->readyweapon].flashstate);

	P_BulletSlope (player->mo);
	P_GunShot (player->mo, !player->refire);

	if( ::g->plyr == player ) {
	}
}


//
// A_FireShotgun
//
void
A_FireShotgun
( player_t*	player,
 pspdef_t*	psp ) 
{
	int		i;

	if (globalNetworking || (player == &::g->players[::g->consoleplayer]))
		S_StartSound (player->mo, sfx_shotgn);
	P_SetMobjState (player->mo, S_PLAY_ATK2);

	if( ( player->cheats & CF_INFAMMO ) == false ) {
		player->ammo[weaponinfo[player->readyweapon].ammo] -= weaponinfo[player->readyweapon].clipAmmo;
	}

	P_SetPsprite (player,
		ps_flash,
		weaponinfo[player->readyweapon].flashstate);

	P_BulletSlope (player->mo);

	for (i=0 ; i<7 ; i++)
		P_GunShot (player->mo, false);

	if( ::g->plyr == player ) {
	}
}



//
// A_FireShotgun2
//
void
A_FireShotgun2
( player_t*	player,
 pspdef_t*	psp ) 
{
	int		i;
	angle_t	angle;
	int		damage;


	if (globalNetworking || (player == &::g->players[::g->consoleplayer]))
		S_StartSound (player->mo, sfx_dshtgn);
	P_SetMobjState (player->mo, S_PLAY_ATK2);

	if( (player->cheats & CF_INFAMMO) == false ) {
		player->ammo[weaponinfo[player->readyweapon].ammo] -= weaponinfo[player->readyweapon].clipAmmo;
	}

	P_SetPsprite (player,
		ps_flash,
		weaponinfo[player->readyweapon].flashstate);

	P_BulletSlope (player->mo);

	for (i=0 ; i<20 ; i++)
	{
		damage = 5*(P_Random ()%3+1);
		angle = player->mo->angle;
		angle += (P_Random()-P_Random())<<19;
		P_LineAttack (player->mo,
			angle,
			MISSILERANGE,
			::g->bulletslope + ((P_Random()-P_Random())<<5), damage);
	}

	if( ::g->plyr == player ) {
	}
}


//
// A_FireCGun
//
void
A_FireCGun
( player_t*	player,
 pspdef_t*	psp ) 
{
	if (globalNetworking || (player == &::g->players[::g->consoleplayer]))
		S_StartSound (player->mo, sfx_pistol);

	if (!player->ammo[weaponinfo[player->readyweapon].ammo])
		return;

	P_SetMobjState (player->mo, S_PLAY_ATK2);
	if( (player->cheats & CF_INFAMMO) == false ) {
		player->ammo[weaponinfo[player->readyweapon].ammo] -= weaponinfo[player->readyweapon].clipAmmo;
	}
	P_SetPsprite (player,
		ps_flash,

		(
		weaponinfo[player->readyweapon].flashstate
		+ psp->state
		- &::g->states[S_CHAIN1] ));

	P_BulletSlope (player->mo);

	P_GunShot (player->mo, !player->refire);
	player->CGunShoots += weaponinfo[player->readyweapon].clipAmmo;
	if (idAchievementManager::isClassicDoomOnly() && player->CGunShoots >= 200) {
		idAchievementManager::LocalUser_CompleteAchievement(CLASSIC_ACHIEVEMENT_CHAINGUN);
	}
	if( ::g->plyr == player ) {
	}
}



//
// ?
//
void A_Light0 (player_t *player, pspdef_t *psp)
{
	player->extralight = 0;
}

void A_Light1 (player_t *player, pspdef_t *psp)
{
	player->extralight = 1;
}

void A_Light2 (player_t *player, pspdef_t *psp)
{
	player->extralight = 2;
}


//
// A_BFGSpray
// Spawn a BFG explosion on every monster in view
//
void A_BFGSpray (mobj_t* mo) 
{
	int			i;
	int			j;
	int			damage;
	angle_t		an;

	// offset angles from its attack angle
	for (i=0 ; i<40 ; i++)
	{
		an = mo->angle - ANG90/2 + ANG90/40*i;

		// mo->target is the originator (player)
		//  of the missile
		P_AimLineAttack (mo->target, an, 16*64*FRACUNIT);

		if (!::g->linetarget)
			continue;

		P_SpawnMobj (::g->linetarget->x,
			::g->linetarget->y,
			::g->linetarget->z + (::g->linetarget->height>>2),
			MT_EXTRABFG);

		damage = 0;
		for (j=0;j<15;j++)
			damage += (P_Random()&7) + 1;

		P_DamageMobj (::g->linetarget, mo, mo->target, damage);
	}

	//GK: D1&2 BFG Overkill
	if (idAchievementManager::isClassicDoomOnly() && mo->target->player->bfgTargets == 1) {
		idAchievementManager::LocalUser_CompleteAchievement(CLASSIC_ACHIEVEMENT_OVERKILL);
	}
	mo->target->player->bfgTargets = 0;
	mo->target->player->inBFGStates = false;
}


//
// A_BFGsound
//
void
A_BFGsound
( player_t*	player,
 pspdef_t*	psp )
{
	S_StartSound (player->mo, sfx_bfg);
}

void A_WeaponProjectile(player_t* player,
	pspdef_t* psp)
{
	int type, angle, pitch, spawnofs_xy, spawnofs_z;
	mobj_t* mo;
	int an;

	if (!psp->state || !psp->state->args[0])
		return;

	type = psp->state->args[0] - 1;
	angle = psp->state->args[1];
	pitch = psp->state->args[2];
	spawnofs_xy = psp->state->args[3];
	spawnofs_z = psp->state->args[4];

	mo = P_SpawnPlayerMissile(player->mo, type);
	if (!mo)
		return;

	// adjust angle
	mo->angle += (unsigned int)(((int64_t)angle << FRACBITS) / 360);
	an = mo->angle >> ANGLETOFINESHIFT;
	mo->momx = FixedMul(mo->info->speed, finecosine[an]);
	mo->momy = FixedMul(mo->info->speed, finesine[an]);

	// adjust pitch (approximated, using Doom's ye olde
	// finetangent table; same method as autoaim)
	mo->momz += FixedMul(mo->info->speed, DegToSlope(pitch));

	// adjust position
	an = (player->mo->angle - ANG90) >> ANGLETOFINESHIFT;
	mo->x += FixedMul(spawnofs_xy, finecosine[an]);
	mo->y += FixedMul(spawnofs_xy, finesine[an]);
	mo->z += spawnofs_z;

	// set tracer to the player's autoaim target,
	// so player seeker missiles prioritizing the
	// baddie the player is actually aiming at. ;)
	mo->tracer = ::g->linetarget;
}

void A_WeaponBulletAttack(player_t* player,
	pspdef_t* psp)
{
	angle_t	angle;
	fixed_t slope;
	int		damage;
	fixed_t hspread, vspread;
	uint	numBullets = 1;
	uint	damageBase = 5;
	uint	damageDice = 3;

	hspread = psp->state->args[0];
	vspread = psp->state->args[1];
	if (psp->state->args[2] >= 0) {
		numBullets = psp->state->args[2];
	}
	if (psp->state->args[3] >= 0) {
		damageBase = psp->state->args[3];
	}
	if (psp->state->args[4] >= 0) {
		damageDice = psp->state->args[4];
	}

	P_BulletSlope(player->mo);

	for (uint i = 0; i < numBullets; i++)
	{
		damage = damageBase * (P_Random() % damageDice + 1);
		angle = player->mo->angle;
		angle += (FixedToAngle(abs(hspread)) * (P_Random() - P_Random())) / 255;
		slope = ::g->bulletslope;
		slope += P_RandomHitscanSlope(vspread);
		P_LineAttack(player->mo,
			angle,
			MISSILERANGE,
			slope, damage);
	}
}

void A_WeaponSound(player_t* player,
	pspdef_t* psp) {
	if (psp->state && psp->state->args[0] >= 0) {
		S_StartSound(psp->state->args[1] ? NULL : player->mo, psp->state->args[0]);
	}
}

void A_WeaponJump(player_t* player,
	pspdef_t* psp)
{
	if (P_Random() < psp->state->args[1]) {
		P_SetPsprite(player, ps_weapon, psp->state->args[0]);
	}
}

void A_ConsumeAmmo(player_t* player,
	pspdef_t* psp) {
	if (psp->state) {
		int clipAmount = psp->state->args[0] == 0 ? weaponinfo[player->readyweapon].clipAmmo : psp->state->args[0];
		if ((player->cheats & CF_INFAMMO) == false && weaponinfo[player->readyweapon].ammo != am_noammo) {
			player->ammo[weaponinfo[player->readyweapon].ammo] -= clipAmount;
		}
		if (player->ammo[weaponinfo[player->readyweapon].ammo] < 0) {
			player->ammo[weaponinfo[player->readyweapon].ammo] = 0;
		}
	}
}

void A_CheckAmmo(player_t* player,
	pspdef_t* psp) {
	if (psp->state) {
		int minAmount = psp->state->args[1] ? psp->state->args[1] : weaponinfo[player->readyweapon].clipAmmo;
		if ((player->cheats & CF_INFAMMO) == false && weaponinfo[player->readyweapon].ammo != am_noammo) {
			if (player->ammo[weaponinfo[player->readyweapon].ammo] < minAmount) {
				P_SetPsprite(player, ps_weapon, psp->state->args[0]);
			}
		}
	}
}

void A_RefireTo(player_t* player,
	pspdef_t* psp) {
	if (psp->state) {
		bool hasAmmo = psp->state->args[1] ? true : player->ammo[weaponinfo[player->readyweapon].ammo] > 0;
		if ((player->cmd.buttons & BT_ATTACK)
			&& player->pendingweapon == wp_nochange
			&& player->health && hasAmmo) {
			P_SetPsprite(player, ps_weapon, psp->state->args[0]);
		}
	}
}
void A_GunFlashTo(player_t* player,
	pspdef_t* psp) {
	if (psp->state) {
		if (psp->state->args[1]) {
			P_SetMobjState(player->mo, S_PLAY_ATK2);
		}
		P_SetPsprite(player, ps_flash, psp->state->args[0]);
	}
}
void A_WeaponAlert(player_t* player,
	pspdef_t* psp) {
	P_NoiseAlert(player->mo, player->mo);
}

}; // extern "C"


//
// P_SetupPsprites
// Called at start of level for each player.
//
void P_SetupPsprites (player_t* player) 
{
	int	i;

	// remove all psprites
	for (i=0 ; i<NUMPSPRITES ; i++)
		player->psprites[i].state = NULL;

	// spawn the gun
	player->pendingweapon = player->readyweapon;
	P_BringUpWeapon (player);
}




//
// P_MovePsprites
// Called every tic by player thinking routine.
//
void P_MovePsprites (player_t* player) 
{
	int		i;
	pspdef_t*	psp;
	const state_t*	state;

	psp = &player->psprites[0];
	for (i=0 ; i<NUMPSPRITES ; i++, psp++)
	{
		// a null state means not active
		if ( (state = psp->state) != NULL )	
		{
			// drop tic count and possibly change state

			// a -1 tic count never changes
			if (psp->tics != -1)	
			{
				psp->tics--;
				if (!psp->tics)
					P_SetPsprite (player, i, psp->state->nextstate);
			}				
		}
	}

	player->psprites[ps_flash].sx = player->psprites[ps_weapon].sx;
	player->psprites[ps_flash].sy = player->psprites[ps_weapon].sy;
}

