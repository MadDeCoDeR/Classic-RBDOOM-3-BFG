/**
* Copyright (C) 2026 George Kalampokis
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
#pragma hdrstop
#include "precompiled.h"

#include "../renderer/Image.h"

#include "DeviceContext.h"
#include "Window.h"
#include "UserInterfaceLocal.h"
#include "GameDOOMWindow.h"
#include "../../doomclassic/doom/doomlib.h"

extern idCVar cl_engineHz_interp;
extern idCVar cl_engineHz;
extern idCVar r_clear;
idCVar cl_inGUI("cl_inGUI", "0", CVAR_BOOL | CVAR_ROM, "");
idCVar cl_closeGame("cl_closeGame", "0", CVAR_BOOL | CVAR_ROM, "");


idGameDOOMWindow::idGameDOOMWindow(idUserInterfaceLocal* gui) : idWindow(gui) {
	gameruning = false;
	gameMode = 0;
	originalInterpolation = false;
}

idGameDOOMWindow::~idGameDOOMWindow() {
	DoomLib::Interface.Shutdown();
}


void idGameDOOMWindow::Draw(int time, float x, float y) {
	if (!gameruning) {
		ResetGame();
	}
	else {
		
		common->RunDoomClassicFrame();
		DoomLib::ApplyRumble();
		if (cl_closeGame.GetBool()) {
			CloseGame();
		}
	}
}

const char* idGameDOOMWindow::HandleEvent(const sysEvent_t* event, bool* updateVisuals) {

	// need to call this to allow proper focus and capturing on embedded children
	const char* ret = idWindow::HandleEvent(event, updateVisuals);

	return ret;
}

idWinVar* idGameDOOMWindow::GetWinVarByName(const char* _name, bool winLookup, drawWin_t** owner) {
	idWinVar* retVar = NULL;

	if (idStr::Icmp(_name, "gameMode") == 0)
	{
		retVar = &gameMode;
	}

	if (idStr::Icmp(_name, "gameruning") == 0)
	{
		retVar = &gameruning;
	}


	if (retVar)
	{
		return retVar;
	}

	return idWindow::GetWinVarByName(_name, winLookup, owner);
}

bool idGameDOOMWindow::ParseInternalVar(const char* _name, idTokenParser* src)
{

	if (idStr::Icmp(_name, "gameMode") == 0)
	{
		gameMode = src->ParseInt();
		return true;
	}
	if (idStr::Icmp(_name, "gameruning") == 0)
	{
		gameruning = src->ParseBool();
		return true;
	}

	return idWindow::ParseInternalVar(_name, src);
}

void idGameDOOMWindow::ResetGame() {
	DoomLib::Interface.Shutdown();
	DoomLib::skipToNew = false;
	DoomLib::skipToLoad = false;

	common->SetDOOMClassicResolution(0, 0);

	// Reset match parameters for the classics.
	DoomLib::matchParms = idMatchParameters();
	//GK:Re-stabilize the framerate on classic DOOM
	originalInterpolation = cl_engineHz_interp.GetBool();
	cl_engineHz_interp.SetBool(true);
	cl_engineHz_interp.ClearModified();
	com_engineHz_denominator = 100LL * (cl_engineHz_interp.GetBool() ? com_engineHz.GetFloat() : cl_engineHz.GetFloat());
	com_engineHz_latched = cl_engineHz.GetFloat();
	//GK: End
	DoomLib::SetCurrentExpansion(gameMode - 1);
	if (::op != NULL) {
		::op->openInput()->ChangeControllerConfiguration("classic", 0);
	}
	gameMode = 0;
	gameruning = true;
	cl_inGUI.SetBool(true);
}

void idGameDOOMWindow::CloseGame() {
	DoomLib::Interface.Shutdown();
	gameruning = false;
	cl_inGUI.SetBool(false);
	cl_closeGame.SetBool(false);
	cl_engineHz_interp.SetBool(originalInterpolation);
	this->visible = 0;
	this->parent->GetParent()->SetChildWinVarVal("gameSelection", "visible", "1");
	this->parent->GetParent()->SetChildWinVarVal("Desktop", "hideCursor", "0");
}