#include "precompiled.h"
#include "PhotoMode.h"
#include "../../doomclassic/doom/doomlib.h"

void PhotoMode::Start(idVec3* viewangles) {
	oldTPViewAngles = *viewangles;
	oldpmtp = game->GetCVarBool("pm_thirdPerson");
	game->SetCVarBool("pm_thirdPerson", true);
	oldpmtpangl = game->GetCVarFloat("pm_thirdPersonAngle");
	oldpmtprangle = game->GetCVarFloat("pm_thirdPersonRange");
	oldpmtpxoffs = game->GetCVarFloat("pm_thirdPersonXOff");
	oldpmtpheight = game->GetCVarFloat("pm_thirdPersonHeight");
}

void PhotoMode::End(idVec3* viewangles)
{
	game->SetCVarBool("pm_thirdPerson", oldpmtp);
	game->SetCVarFloat("pm_thirdPersonRange", oldpmtprangle);
	game->SetCVarFloat("pm_thirdPersonXOff", oldpmtpxoffs);
	game->SetCVarFloat("pm_thirdPersonAngle", oldpmtpangl);
	game->SetCVarFloat("pm_thirdPersonHeight", oldpmtpheight);
	if (DoomLib::GetGlobalData(0) == NULL) {
		*viewangles = oldTPViewAngles;
	}
}

void PhotoMode::AdjustCamera(int viewDelta, int heightDelta, usercmd_t cmd)
{
	int offset = cmd.forwardmove / (cmd.forwardmove != 0 ? abs(cmd.forwardmove) : 1);

	game->SetCVarFloat("pm_thirdPersonRange", game->GetCVarFloat("pm_thirdPersonRange") - offset);

	offset = cmd.rightmove / (cmd.rightmove != 0 ? abs(cmd.rightmove) : 1);

	game->SetCVarFloat("pm_thirdPersonXOff", game->GetCVarFloat("pm_thirdPersonXOff") - offset);

	
	offset = viewDelta; /*/ (delta != 0 ? abs(delta) : 1);*/

	game->SetCVarFloat("pm_thirdPersonAngle", game->GetCVarFloat("pm_thirdPersonAngle") + offset);


	offset = heightDelta;
	game->SetCVarFloat("pm_thirdPersonHeight", game->GetCVarFloat("pm_thirdPersonHeight") + offset);
}
