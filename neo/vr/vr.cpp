/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2015 Robert Beckebans

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

#undef strncmp
#include <OVR.h>


idCVar vr_enable( "vr_enable", "1", CVAR_BOOL, "" );
idCVar vr_oculusDeviceID( "vr_oculusDeviceID", "0", CVAR_INTEGER, "" );

class VRSystemLocal : public VRSystem
{
public:
	VRSystemLocal();
	
	virtual void				Init();
	virtual void				Shutdown();
	
	virtual void				Enable();
	virtual void				Disable();
	
	virtual bool				IsActive() const;
	
	virtual idAngles			GetOrientation() const;
	
private:



	ovrHmd						hmd;
};

VRSystemLocal				vrSystemLocal;
VRSystem* 					vrSystem = &vrSystemLocal;




VRSystemLocal::VRSystemLocal()
{
	hmd = NULL;
}

void VRSystemLocal::Init()
{
	if( !ovr_Initialize() )
	{
		common->FatalError( "Could not initialize LibOVR!" );
	}
	
	int numDevices = ovrHmd_Detect();
	
	bool found = false;
	
	common->Printf( "Found VR %i devices\n", numDevices );
	common->Printf( "Enumerating VR devices...\n" );
	
	int hmdDevice = 0;
	for( int i = 0; i < numDevices; i++ )
	{
		ovrHmd tmpHmd = ovrHmd_Create( i );
		
		if( tmpHmd )
		{
			common->Printf( "VR: Found device #%i '%s' s/n:%s\n", i, tmpHmd->ProductName, tmpHmd->SerialNumber );
			
			//sprintf(hmdnames[i].label,"%i: %s",i + 1, tempHmd->ProductName);
			//sprintf(hmdnames[i].serialnumber,"%s",tempHmd->SerialNumber);
			
			//if (!strncmp(vr_ovr_device->string,tempHmd->SerialNumber,strlen(tempHmd->SerialNumber)))
			{
				hmdDevice = i;
				found = true;
			}
			ovrHmd_Destroy( tmpHmd );
		}
	}
	
	hmd = ovrHmd_Create( hmdDevice );

	if( hmd )
	{
		cmdSystem->BufferCommandText( CMD_EXEC_NOW, "exec oculus_default.cfg\n" );
	}
}

void VRSystemLocal::Shutdown()
{
	if( hmd )
	{
		ovrHmd_Destroy( hmd );
	}
	ovr_Shutdown();
}

void VRSystemLocal::Enable()
{

}

void VRSystemLocal::Disable()
{

}

bool VRSystemLocal::IsActive() const
{
	return hmd != NULL;
}

idAngles	 VRSystemLocal::GetOrientation() const
{
	return idAngles();
}