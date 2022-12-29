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
#pragma once
class PhotoMode
{
public:
	void Start(idVec3* viewangles);
	void End(idVec3* viewangles);
	void AdjustCamera(int viewDelta, int heightDelta, usercmd_t cmd);
private:
	//GK: Keep the original values of the pm_thirdPerson CVars when entering photomode
	bool		oldpmtp; //pm_thirdPerson
	float	oldpmtpangl; //pm_thirdPersonAngle
	float	oldpmtprangle; //pm_thirdPersonRange
	float	oldpmtpxoffs; //pm_thirdPersonXoff
	float	oldpmtpheight; //pm_thirdPersonHeight
	idVec3			oldTPViewAngles; //In order to prevent the camera of the player to follow the camera of the photo mode
	bool	oldtpclip;	//pm_thirdPersonClip
	//GK: End
};

