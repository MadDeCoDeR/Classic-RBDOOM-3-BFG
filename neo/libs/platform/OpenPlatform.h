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
#pragma once

class OPlatform
{
public:
	virtual bool API_Init() = 0;
	virtual bool API_Init(const char* data) = 0;
	virtual void API_Shutdown() = 0;
	virtual const char* GetPlatformUserName() = 0;
	virtual bool GetAchievement(const char* name, bool* status) = 0;
	virtual const char* GetAchievementDevName(unsigned int id) = 0;
	virtual bool GetAchievementPercent(const char* name, unsigned int progress, unsigned int max) = 0;
	virtual bool UnlockAchievement(const char* name) = 0;
	virtual bool LockAchievement(const char* name) = 0;
	virtual const char* GetAchievementName(const char* name) = 0;
	virtual const char* GetAchievementDescription(const char* name) = 0;
	virtual bool GetAchievementHidden(const char* name) = 0;
	virtual void ShowUser( unsigned int id) = 0;
	virtual bool isPlatformOverlayActive() = 0;
	virtual void SetNotificationsPosition(unsigned int x, unsigned int y) = 0;
	virtual unsigned long long CreateLobby(int type, int maxplayers) = 0;
	virtual bool GetCloudStats(unsigned long long* totalBytes, unsigned long long* availableBytes) = 0;
	virtual bool SetAdditionalInfo(const char* key, const char* value) = 0;
};

extern OPlatform* op;

extern "C" {
	typedef OPlatform* (*GetPlatformAPI_t)();
}