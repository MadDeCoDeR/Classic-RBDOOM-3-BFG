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
#pragma once

#include "OpenPlatform.h"

class OPlatformLocal: public OPlatform
{
public:
	virtual bool API_Init();
	virtual void API_Shutdown();
	virtual const char* GetAPIUserName();
	virtual bool GetAPIAchievement(const char* name, bool* status);
	virtual const char* GetAPINameAchievement(unsigned int id);
	virtual bool GetAPIAchievementPercent(const char* name, float* status);
	virtual bool SetAPIAchievement(const char* name);
	virtual const char* GetAPIAchievementDetail(const char* name, const char* type);
	virtual bool StoreAPIStats();
	virtual bool GetAPIStats(const char* name, int* stat);
	virtual bool GetAPIStats(const char* name, float* stat);
	virtual bool SetAPIStats(const char* name, int stat);
	virtual bool SetAPIStats(const char* name, float stat);
	virtual void ShowAPIUser(const char* type, unsigned long long id);
};