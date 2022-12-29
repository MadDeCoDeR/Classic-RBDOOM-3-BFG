/**
* Copyright (C) 2022 George Kalmpokis
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
* of the Software, and to permit persons to whom the Software is furnished to
* do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software. As clarification, there
* is no requirement that the copyright notice and permission be included in
* binary distributions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
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
	oldtpclip = game->GetCVarBool("pm_thirdPersonClip");
	game->SetCVarBool("pm_thirdPersonClip", false);
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
	game->SetCVarBool("pm_thirdPersonClip", oldtpclip);
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
