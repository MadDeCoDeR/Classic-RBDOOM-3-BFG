/**
* Copyright (C) 2018 George Kalmpokis
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

#include <ctype.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <vector>
#include "d_exp.h"

#include "doomtype.h"

#include "info.h"

#include "w_wad.h"

#include "i_system.h"

#include "d_items.h"
#include "sounds.h"
#include "f_finale.h"
#include "g_game.h"

void parseexptext(char* text);
std::vector<std::string> getexplines(char* text);
int checkexpstate(char* text);
void setEXP(char* name,int value);
void setSAVEDIR( char* value);
void setMAPINT(int pos,char* name, int value);
void setMAPSTR(int pos, char* name, char* value);
void initMAPS(std::vector<std::string> lines);
void setCluster(int pos, char* name, char*value, char* option, int linepos, std::vector<std::string> lines);
int tex = 1;
void setExpData(char* name, char* value);

typedef struct {
	char** var;
	char* name;
}fstr;

fstr strval[] = {

{ &e1text,"E1TEXT" },
{ &e2text,"E2TEXT" },
{ &e3text,"E3TEXT" },
{ &e4text,"E4TEXT" },
{ &c1text,"C1TEXT" },
{ &c2text,"C2TEXT" },
{ &c3text,"C3TEXT" },
{ &c4text,"C4TEXT" },
{ &c5text,"C5TEXT" },
{ &c6text,"C6TEXT" },
{ &c7text,"C7TEXT" },
{ &c8Text,"C8TEXT" },
{ &p1text,"P1TEXT" },
{ &p2text,"P2TEXT" },
{ &p3text,"P3TEXT" },
{ &p4text,"P4TEXT" },
{ &p5text,"P5TEXT" },
{ &p6text,"P6TEXT" },
{ &t1text,"T1TEXT" },
{ &t2text,"T2TEXT" },
{ &t3text,"T3TEXT" },
{ &t4text,"T4TEXT" },
{ &t5text,"T5TEXT" },
{ &t6text,"T6TEXT" }

};

void EX_add(int lump) {
	char* text = (char*)malloc(W_LumpLength(lump) + 1);
	text[W_LumpLength(lump)] = '\0';
	I_Printf("Applying Expansion Info ...\n");
	W_ReadLump(lump, text);
	::g->mapmax = 33;
	if (!::g->maps.empty()) {
		::g->maps.clear();
	}
	::g->maps.resize(1);
	::g->endmap = 30;
	::g->savedir = NULL;
	::g->intermusic = mus_dm2int;
	//idLib::Printf("%s", text);
	parseexptext(text);
	free(text);
	I_Printf("Expansion Info succesfully applied\n");
}

void parseexptext(char* text) {
	std::vector<std::string> linedtext = getexplines(text);
	int state = 0;
	int val1;
	int val2;
	int val3;
	char* varname;
	char* varval;
	char* varopt;
	if (!::g->mapind) {
		initMAPS(linedtext);
	}

	for (int i = 0; i < linedtext.size(); i++) {
		char* t = strtok(strdup(linedtext[i].c_str()), " ");
		if (t == NULL)
			continue;
		int tstate= checkexpstate(t);
			if (tstate) {
				state = tstate;
				t = strtok(NULL, " ");
				if (t != NULL) {
					if (!idStr::Icmpn(t, "MAP",3)) {
						char* s = t + 3;
						val1 = atoi(s) - 1;
					}
					else {
						val1 = atoi(t) - 1;
						if (state == 3) {
							if (!::g->clusterind || ::g->clusterind <= ::g->clusters.size()) {
								::g->clusters.resize(val1 + 1);
								::g->clusterind = val1 + 1;
							}
							::g->clusters[val1].fflat = -1;
						}
					}
					t = strtok(NULL, " ");
					if (t != NULL) {
						val2 = atoi(t)-1;
					}
				}
			}
			else {
					varname = t;

				t = strtok(NULL, " ");
				if (t != NULL) {
					if (!idStr::Icmp(t, "=")) {
						t = strtok(NULL, " ");
						if (t == NULL)
							continue;
					}
					else {
						varopt = t;
						t = strtok(NULL, " ");
						if (t == NULL || !idStr::Icmpn(varopt, "\"", 1) || !idStr::Icmp(varname, "sky1")) {
							t = varopt;
							varopt = NULL;
						}

					}
					val3 = atoi(t);
				}
				else {
					val3 = 0;
				}

				
				switch (state) {
				case 1:
					if (!idStr::Icmp(varname, "save_dir")) {
						varval = t;
						setSAVEDIR(varval);
					}
					else {
						setEXP(varname, val3);
					}
					break;
				case 2:
							if (!val3) {
								varval = t;
								setMAPSTR(val1, varname, varval);
							} 
							else {
								setMAPINT(val1, varname, val3);
							}
					break;
				case 3:
					varval = t;
					setCluster(val1, varname, varval,varopt,i,linedtext );
					break;
				default:
					varval = t;
					setExpData(varname, varval);
					break;

				}
			}
		

		
	}
}

std::vector<std::string> getexplines(char* text) {
	std::vector<std::string> lines;
	int size = strlen(text);
	for (int i = 0; i < size /*- 7*/; i++) {
		std::string letter = "";
		qboolean ignore = false;
		while (text[i] != '\n') {

			if (!ignore) {
				if (text[i] != '\r') {
					letter += text[i];
				}
			}
			if (i < size /*- 7*/) {
				i++;
			}
			else {
				break;
			}

		}
		//i++;
		lines.push_back(letter);

	}
	return lines;
}

int checkexpstate(char* text) {
	if (!idStr::Icmp(text, "EXP")) {
		return 1;
	}
	if (!idStr::Icmp(text, "MAP")) {
		return 2;
	}
	if (!idStr::Icmp(text, "clusterdef")) {
		return 3;
	}
	return 0;
}

void setEXP(char* name, int value) {
	if (!idStr::Icmp(name, "max_maps")) {
		::g->mapmax = value;
		if (!::g->mapind || ::g->mapind >= ::g->maps.size()) {
			::g->maps.resize(::g->mapmax);
			::g->mapind = ::g->mapmax;
		}
			for (int i = 0; i < ::g->mapmax; i++) {
				char* tname = new char[6];
				if (i + 1 < 10)
					sprintf(tname, "MAP0%i", i + 1);
				else
					sprintf(tname, "MAP%i", i + 1);
				::g->maps[i].lumpname = tname;
				::g->maps[i].nextmap = i + 1;
			}
		return;
	}
	if (!idStr::Icmp(name, "final_map")) {
		::g->endmap = value;
		return;
	}
}

void setSAVEDIR( char* value) {
	::g->savedir = value;
}

void setMAPINT(int pos,char* name, int value) {
	if (!idStr::Icmp(name, "secret_map")) {
		::g->maps[pos].secretmap = value;
		::g->maps[value-1].nextmap = pos+1;
	}
	if (!idStr::Icmp(name, "next_map")) {
		::g->maps[pos].nextmap = value-1;
	}
	
	if (!idStr::Icmp(name, "cluster")) {
		::g->maps[pos].cluster = value;
	}
	if (!idStr::Icmp(name, "par")) {
		cpars[pos - 1] = value;
	}
}

void setMAPSTR(int pos, char* name, char* value) {
	if (!idStr::Icmp(name, "miniboss") || !idStr::Icmp(name, "map07special")) {
		::g->maps[pos].miniboss = true;
	}
	if (!idStr::Icmp(name, "secret_final")) {
		::g->maps[pos].fsecret = true;
	}
	if(!idStr::Icmp(name,"final_flat")){
		for (int i = 0; i < 12; i++) {
			if (!idStr::Icmp(finaleflat[i], value)) {
				::g->maps[pos].fflat = i;
				return;
			}
		}
	}
	if (!idStr::Icmp(name, "final_text")) {
		for (int i = 0; i < 24; i++) {
			if (!idStr::Icmp(strval[i].name, value)) {
				::g->maps[pos].ftext = *strval[i].var;
				return;
			}
		}
	}

	if (!idStr::Icmp(name, "final_music")) {
		for (int i = 1; i < 80; i++) {
			char* musname = value + 2;
			if (::g->S_music[i].name == NULL) {
				::g->totalmus++;
				::g->S_music[i].name = musname;
				::g->maps[pos].fmusic = i;
				return;
			}
			if (!idStr::Icmp(musname, ::g->S_music[i].name)) {
				::g->maps[pos].fmusic = i;
				return;
			}
		}

	}

	if (!idStr::Icmp(name, "music")) {
		for (int i = 1; i < 80; i++) {
			char* musname = value + 2;
			if (::g->S_music[i].name == NULL) {
				::g->S_music[i].name = musname;
				::g->totalmus++;
				::g->maps[pos].fmusic = i;
				return;
			}
			if (!idStr::Icmp(musname, ::g->S_music[i].name)) {
				::g->maps[pos].music = i;
				return;
			}
		}
	}

	if (!idStr::Icmp(name, "next")) {
		for (int i = 0; i < ::g->maps.size(); i++) {
			if (!::g->maps[i].lumpname) {
				continue;
			}
			if (!idStr::Icmp(value, ::g->maps[i].lumpname)) {
				::g->maps[pos].nextmap = i;
				return;
			}
		}
		if (!idStr::Icmpn(value, "EndGame", 7)) {
			::g->endmap = pos + 1;
			return;
		}
	}
	if (!idStr::Icmp(name, "secretnext")) {
		for (int i = pos; i < ::g->maps.size(); i++) {
			if (!::g->maps[i].lumpname) {
				continue;
			}
			if (!idStr::Icmp(value, ::g->maps[i].lumpname)) {
				if (::g->maps[pos].nextmap != i) {
					::g->maps[pos].secretmap = i+1;
					return;
				}
				else
					return;
			}
		}
		if (!idStr::Icmpn(value, "EndGame", 7)) {
			if (::g->endmap != pos + 1) {
				::g->endmap = pos + 1;
				return;
			}
			else
				return;
		}
	}

	if (!idStr::Icmp(name, "sky_tex") || !idStr::Icmp(name, "sky1")) {
		::g->maps[pos].sky = value;
	}
}

void initMAPS(std::vector<std::string> lines) {
	int map = 0;
	for (int i = 0; i < lines.size(); i++) {
		char* t = strtok(strdup(lines[i].c_str()), " ");
		if (t == NULL)
			continue;
		if (!idStr::Icmp(t, "map")) {
			t = strtok(NULL, " ");
			if (t != NULL) {
				if (!idStr::Icmpn(t, "MAP", 3)) {
					int tmap = atoi(t + 3);
					if (tmap - 1 != map) {
						::g->endmap = map;
						map++;
					}
					else {
						map = tmap;
						::g->endmap = map;
					}
					::g->mapmax = map;
					::g->mapind = map;
					if (tmap >= ::g->maps.size())
						::g->maps.resize(tmap);
					::g->maps[tmap - 1].lumpname = t;
					::g->maps[tmap-1].nextmap = map;
				}
				else {
					if (strlen(t) > 2)
						map++;
					else {
						map = atoi(t);
					}
					::g->mapmax = map;
					::g->endmap = map;
					::g->mapind = map;
					if (map >= ::g->maps.size()) {
						::g->maps.resize(map);
					}
					if(strlen(t)>2)
						::g->maps[map - 1].lumpname = t;
					else {
						char* tname = new char[6];
						if(map<10)
						sprintf(tname,"MAP0%i\0",map);
						else
							sprintf(tname, "MAP%i\0", map);
						::g->maps[map - 1].lumpname = tname;
					}
					::g->maps[map - 1].nextmap = map;
				}
			}
		}
	}
}

void setCluster(int pos, char* name, char*value, char* option, int linepos, std::vector<std::string> lines) {
	if (!idStr::Icmp(name, "flat")) {
		if (value[0] == '\"') {
			value++;
		}
		if (value[sizeof(value) - 1] == '\"') {
			value[sizeof(value) - 1] = '\0';
		}
		for (int i = 0; i < 12; i++) {
			if (!idStr::Icmp(finaleflat[i], value)) {
				::g->clusters[pos].fflat = i;
				return;
			}
		}
		::g->clusters[pos].ftex = value;
		return;
	}
	if (!idStr::Icmp(name, "music")) {
		int c = 0;
		while (value[c] != '_') {
			c++;
		}
		c++;
		char* musname = value + c;
		if (musname[sizeof(musname) - 2] == '\"') {
			musname[sizeof(musname) - 2] = '\0';
		}
		for (int i = 1; i < 80; i++) {
			
			if (::g->S_music[i].name == NULL) {
				::g->S_music[i].name = musname;
				::g->totalmus++;
				::g->maps[pos].fmusic = i;
				return;
			}
			if (!idStr::Icmp(musname, ::g->S_music[i].name)) {
				::g->clusters[pos].fmusic = i;
				return;
			}
		}
	}
	if (!idStr::Icmp(name, "entertext") || !idStr::Icmp(name, "exittext")) {
		if (!idStr::Icmp(name, "entertext")) {
			tex++;
			::g->clusters[pos].textpr = tex;
		}
		else {
			::g->clusters[pos].textpr = 1;
		}
		if (option != NULL) {
			if (!idStr::Icmp(option, "lookup")) {
				if (value[0] == '\"') {
					value++;
				}
				if (value[sizeof(value) - 2] == '\"') {
					value[sizeof(value) - 2] = '\0';
				}
				if (value[0] == 'X') {
					value[0] = 'C';
				}
				for (int i = 0; i < 24; i++) {
					if (!idStr::Icmp(strval[i].name, value)) {
						::g->clusters[pos].ftext = *strval[i].var;
						return;
					}
				}
				
			}
		}
		else {
			int size = 0;
			for (int j = linepos; j < lines.size(); j++) {
				char* t = strtok(strdup(lines[linepos].c_str()), " ");
				while (t != NULL) {
					if (!idStr::Icmpn(t, "\"", 1)) {
						size += strlen(t);
					}
					else{
						char breake = t[strlen(t) - 1];
						if (breake == '\"') {
							break;
						}
						if (size > 0) {
							size += 1;
							size += strlen(t);
						}
					}
					t = strtok(NULL, " ");
				}
			}
			char* t = strtok(strdup(lines[linepos].c_str()), " ");
			while (t != NULL) {
				if (!idStr::Icmpn(t, "\"", 1)) {
					::g->clusters[pos].ftext = new char[size];
					strcpy(::g->clusters[pos].ftext,t+1);
				}
				else if (::g->clusters[pos].ftext != NULL) {
					strcat(::g->clusters[pos].ftext, " ");
					strcat(::g->clusters[pos].ftext, t);
				}
				t = strtok(NULL, " ");
			}
			strcat(::g->clusters[pos].ftext, "\n");
			for (int i = linepos + 1; i < lines.size(); i++) {
				int lsize = strlen(lines[i].c_str());
				char breaker = lines[i].c_str()[lsize - 1];
				if (breaker == '\"') {
					strcat(::g->clusters[pos].ftext, lines[i].c_str());
					::g->clusters[pos].ftext[strlen(::g->clusters[pos].ftext)-1]='\0';
					break;
				}
				else {
					if (lines[i].c_str() == NULL) {
						strcat(::g->clusters[pos].ftext, "\r\n");
					}
					else {
						strcat(::g->clusters[pos].ftext, lines[i].c_str());
						strcat(::g->clusters[pos].ftext, "\n");
					}
				}
			}
		}
	}
}

void setExpData(char* name, char* value) {
	if (!idStr::Icmp(name, "intermusic")) {
		for (int i = 1; i < 80; i++) {
			char* musname = value + 2;
			if (::g->S_music[i].name == NULL) {
				::g->totalmus++;
				::g->S_music[i].name = musname;
				::g->intermusic = i;
				return;
			}
			if (!idStr::Icmp(musname, ::g->S_music[i].name)) {
				::g->intermusic = i;
				return;
			}
		}
	}
}