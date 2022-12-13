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

#include "Precompiled.h"

#include "globaldata.h"
#include "doomtype.h"
#include "i_system.h"

#include "d_act.h"


std::vector<std::string> getactlines(char* text) {
	std::vector<std::string> lines;
	int size = strlen(text);
	for (int i = 0; i < size /*- 7*/; i++) {
		std::string letter = "";
		//qboolean ignore = false;
		while (text[i] != '\n') {
				if (text[i] != '\r') {
					letter += text[i];
				}
			if (i < size /*- 7*/) {
				i++;
			}
			else {
				break;
			}
		}
		lines.push_back(letter);

	}
	return lines;
}

void parseacttext(char* text) {
	std::vector<std::string> lines = getactlines(text);
	int i = 0;
	for (std::string line : lines) {
		char* variable = strtok(strdup(line.c_str()), " = ");
		char* value = strtok(NULL, "");
		value = value+2;
		if (!idStr::Cmpn(variable, "sector", 6)) {
			::g->actind = atoi(value);
			if (::g->actind >= (int)::g->acts.size()) {
				::g->acts.resize(::g->actind+1);
			}
			i = 0;
			continue;
		}
		::g->acts[::g->actind].push_back(new actdef_t());
		if (!idStr::Icmp(variable, "command")) {
			::g->acts[::g->actind][i]->command = value;
		}
		else {
			::g->acts[::g->actind][i]->cvar = variable;
			::g->acts[::g->actind][i]->value = value;
			::g->acts[::g->actind][i]->oldValue = strdup(cvarSystem->GetCVarString(variable));
		}
		i++;
	}
}


void loadacts(int lump) {
	if (idStr::Icmpn(W_GetNameForNum(lump), "ACTMAP", 6)) {
		return;
	}
	char* text = (char*)malloc(W_LumpLength(lump) + 1);
	text[W_LumpLength(lump)] = '\0';
	W_ReadLump(lump, text);
	::g->actind = 1;
	::g->acts.reserve(::g->actind);
	//idLib::Printf("%s", text);
	parseacttext(text);
	::g->hasacts = true;
	free(text);
}