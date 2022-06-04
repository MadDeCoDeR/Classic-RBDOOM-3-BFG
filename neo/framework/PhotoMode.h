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

