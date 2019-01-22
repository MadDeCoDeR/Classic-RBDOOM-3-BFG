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
#include "OpenPlatformLocal.h"

OPlatformLocal opl;
OPlatform* op = &opl;

bool OPlatformLocal::API_Init() { return false; }
void OPlatformLocal::API_Shutdown() {}
const char* OPlatformLocal::GetAPIUserName() { return ""; }
bool OPlatformLocal::GetAPIAchievement(const char* name, bool* status) { return false; }
const char* OPlatformLocal::GetAPINameAchievement(unsigned int id) { return ""; }
bool OPlatformLocal::GetAPIAchievementPercent(const char* name, float* status) { return false; }
bool OPlatformLocal::SetAPIAchievement(const char* name) { return false; }
const char* OPlatformLocal::GetAPIAchievementDetail(const char* name, const char* type) { return ""; }
bool OPlatformLocal::StoreAPIStats() { return false; }
bool OPlatformLocal::GetAPIStats(const char* name, int* stat) { return false; }
bool OPlatformLocal::GetAPIStats(const char* name, float* stat) { return false; }
bool OPlatformLocal::SetAPIStats(const char* name, int stat) { return false; }
bool OPlatformLocal::SetAPIStats(const char* name, float stat) { return false; }
void OPlatformLocal::ShowAPIUser(const char* type, unsigned long long id) {}