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
#ifndef __GAME_DOOM_WINDOW_H__
#define __GAME_DOOM_WINDOW_H__

class idGameDOOMWindow;

class idGameDOOMWindow : public idWindow {
public:
	idGameDOOMWindow(idUserInterfaceLocal* gui);
	~idGameDOOMWindow();

	virtual const char* HandleEvent(const sysEvent_t* event, bool* updateVisuals);
	virtual void		Draw(int time, float x, float y);
	virtual idWinVar* GetWinVarByName(const char* _name, bool winLookup = false, drawWin_t** owner = NULL);
private:
	virtual bool		ParseInternalVar(const char* name, idTokenParser* src);
	void ResetGame();
	void CloseGame(bool resetCVars);
private:
	idWinBool gameruning;
	idWinInt gameMode;
	idWinStr gameArgs;
	bool originalInterpolation;
	bool originalAspect;
	idVec3 originalViewAngles;
	
};

#endif
