/**
* Copyright (C) 2019 George Kalmpokis
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
#include "OpenPlatform.h"

class OPlatformLocal : public OPlatform
{
public:
	virtual bool API_Init();
	virtual void API_Shutdown();
	virtual const char* GetUserName();
	virtual bool GetAchievement(const char* name, bool* status);
	virtual const char* GetAchievementName(unsigned int id);
	virtual bool GetAchievementPercent(const char* name, float* status);
	virtual bool UnlockAchievement(const char* name);
	virtual const char* GetAchievementDetails(const char* name, const char* type);
	virtual void ShowUser(const char* type, unsigned long long id);
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
void OPlatformLocal::API_Shutdown() {}
const char* OPlatformLocal::GetUserName() { return ""; }
bool OPlatformLocal::GetAchievement(const char* name, bool* status) { return false; }
const char* OPlatformLocal::GetAchievementName(unsigned int id) { return ""; }
bool OPlatformLocal::GetAchievementPercent(const char* name, float* status) { return false; }
bool OPlatformLocal::UnlockAchievement(const char* name) { return false; }
const char* OPlatformLocal::GetAchievementDetails(const char* name, const char* type) { return ""; }
void OPlatformLocal::ShowUser(const char* type, unsigned long long id) {}