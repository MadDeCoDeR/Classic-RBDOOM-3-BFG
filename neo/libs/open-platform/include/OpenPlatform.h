/*
Open Platform

Copyright(C) 2021 George Kalmpokis

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
#include "OpenAchievement.h"
#include "OpenDLC.h"
#include "OpenApplication.h"
#include "OpenInput.h"

class OPlatform
{
public:
	/*
	* Return the Name of the Implemented API
	*/
	virtual const char* API_Name() = 0;
	/*
	* Initialize the Implemented API, by using either a local file to determine the App Id or an environment variable with the App Id
	*/
	virtual bool API_Init() = 0;
	/*
	* Initialize the Implemented API, by getting directly the App Id
	*/
	virtual bool API_Init(const char* data) = 0;
	/*
	* Shuts down the implemented API
	*/
	virtual void API_Shutdown() = 0;
	/*
	* Get an Instance of the OpenAchievement Class. (for achievement functions)
	*/
	virtual OpenAchievement* openAchievement() = 0;
	/*
	* Get an Instance of the OpenDLC Class. (for Dlc functions)
	*/
	virtual OpenDLC* openDLC() = 0;
	/*
	* Get an Instance of the OpenApp Class. (for App functions)
	*/
	virtual OpenApp* openApp() = 0;
	/*
	* Get an Instance of the OpenInput Class. (for special API Input functions)
	*/
	virtual OpenInput* openInput() = 0;
	virtual const char* GetPlatformUserName() = 0;
	virtual void ShowUser( unsigned int id) = 0;
	/*
	* Run Callbacks of the Implemented API and return if it's overlay is active or not
	*/
	virtual bool API_pump() = 0;
	/*
	* Check if the game runs on a portable device
	*/
	virtual bool IsPortable() = 0;
	/*
	* Check Battery Level on portable Device
	*/
	virtual unsigned char GetBatteryLevel() = 0;
	virtual bool ShowFloatingTextBox(int type, int xpos, int ypos, int width, int height) = 0;
	virtual void SetNotificationsPosition(unsigned int x, unsigned int y) = 0;
	virtual unsigned long long CreateLobby(int type, int maxplayers) = 0;
	virtual bool SetAdditionalInfo(const char* key, const char* value) = 0;
};

extern OPlatform* op;

extern "C" {
	typedef OPlatform* (*GetPlatformAPI_t)();
}