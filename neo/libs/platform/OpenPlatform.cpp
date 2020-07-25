/*
Open Platform

Copyright(C) 2019 George Kalmpokis

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files(the "Software"), to deal in the Software without
restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// OpenPlatfom.cpp : Defines the exported functions for the DLL application.
//

#include "OpenPlatform.h"

class OPlatformLocal : public OPlatform
{
public:
	virtual bool API_Init();
	virtual bool API_Init(const char* data);
	virtual void API_Shutdown();
	virtual const char* GetPlatformUserName();
	virtual bool GetAchievement(const char* name, bool* status);
	virtual const char* GetAchievementDevName(unsigned int id);
	virtual bool GetAchievementPercent(const char* name, unsigned int progress, unsigned int max);
	virtual bool UnlockAchievement(const char* name);
	virtual bool LockAchievement(const char* name);
	virtual const char* GetAchievementName(const char* name);
	virtual const char* GetAchievementDescription(const char* name);
	virtual bool GetAchievementHidden(const char* name);
	virtual void ShowUser( unsigned int id);
	virtual bool isPlatformOverlayActive();
	virtual void SetNotificationsPosition(unsigned int x, unsigned int y);
	virtual unsigned long long CreateLobby(int type, int maxplayers);
	virtual bool GetCloudStats(unsigned long long* totalBytes, unsigned long long* availableBytes);
	virtual bool SetAdditionalInfo(const char* key, const char* value);
};

OPlatformLocal opl;

#if __MWERKS__
#pragma export on
#endif
#if __GNUC__ >= 4
#pragma GCC visibility push(default)
#endif
extern "C" OPlatform* GetPlatformAPI()
{
#if __MWERKS__
#pragma export off
#endif

	return &opl;
}
#if __GNUC__ >= 4
#pragma GCC visibility pop
#endif

OPlatform* op = &opl;

bool OPlatformLocal::API_Init() { return false; }
bool OPlatformLocal::API_Init(const char* data)
{
	return false;
}
void  OPlatformLocal::API_Shutdown() {}
const char* OPlatformLocal::GetPlatformUserName() { return ""; }
bool OPlatformLocal::GetAchievement(const char* name, bool* status) { return false; }
const char* OPlatformLocal::GetAchievementDevName(unsigned int id) { return ""; }
bool OPlatformLocal::GetAchievementPercent(const char* name, unsigned int progress, unsigned int max) { return false; }
bool OPlatformLocal::UnlockAchievement(const char* name) { return false; }
bool OPlatformLocal::LockAchievement(const char* name) { return false; }
const char* OPlatformLocal::GetAchievementName(const char* name) { return ""; }
const char* OPlatformLocal::GetAchievementDescription(const char* name) { return ""; }
bool OPlatformLocal::GetAchievementHidden(const char* name) { return false; }
void OPlatformLocal::ShowUser( unsigned int id) {}
bool OPlatformLocal::isPlatformOverlayActive() { return false; }
void OPlatformLocal::SetNotificationsPosition(unsigned int x, unsigned int y) {}
unsigned long long OPlatformLocal::CreateLobby(int type, int maxplayers) { return 0; }
bool OPlatformLocal::GetCloudStats(unsigned long long* totalBytes, unsigned long long* availableBytes)
{
	return false;
}

bool OPlatformLocal::SetAdditionalInfo(const char* key, const char* value)
{
	return false;
}
