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
#include "precompiled.h"
//No one does simply include classic Doom headers in Doom 3 headers
//Instead include them to cpp files
//Also can there be more than that????
#include "../../doomclassic/doom/globaldata.h"

#ifdef GAME_DLL
//GK: Declare the fuctions here
void idTest(const idCmdArgs& args);
void A_InstaKill(mobj_t* mo, void*);

//GK: Declare custom Code Pointers here
dehcptr customcptr[] = {
	{"InstaKill", (actionf_p2)A_InstaKill}
};
#endif

void InitClassic() {
#ifdef GAME_DLL
	//GK: Register the commands to the System
	::cmdSystem->AddCommand("idtest", idTest, 0, "", 0);
	//Sys_CreateThread((xthread_t)setupCodePointers, NULL, THREAD_HIGHEST, "ClassicSDK", CORE_ANY);
#endif
}

void SetupCodePointers(/*void* data*/) {
#ifdef GAME_DLL
	//bool isInitialized = false;
	Globals* classic = (Globals*)::GetClassicData();
	//while (true) {
		/*if (cvarSystem->GetCVarBool("cl_close")) {
			break;
		}*/
		if (::GetClassicData != NULL) {
			classic = (Globals*)::GetClassicData();
			if (classic == NULL) {
				return;
				//continue;
			}
			else {
				//Add new codepointers
				int size = sizeof(customcptr) / sizeof(dehcptr);
				for (int i = 0; i < size; i++) {
					classic->cptrval.push_back(customcptr[i]);
				}
			}
		//}
	}
#endif
}
#ifdef GAME_DLL
//GK: Implement the command
void idTest(const idCmdArgs& args) {
	Globals* classic = (Globals*)::GetClassicData();
	if (classic == NULL) {
		return;
	}
	if (classic->gamestate != GS_LEVEL) {
		return;
	}
	classic->plyr->message = "Hello World !!!";
}

void A_InstaKill(mobj_t* mo, void*) {
	mo->health = 0;
}
#endif