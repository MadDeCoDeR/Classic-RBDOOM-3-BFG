/**
* Copyright (C) 2018 - 2019 George Kalmpokis
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

typedef struct {
	char* name;
	long limit;
	char** sval;
	int* ival;
	long* lval;
	float* fval;
	bool* bval;
	statenum_t* stval;
	ammotype_t* amval;
}expobj;
int episodecount = 0;

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
const char* getENumName(int i);
void M_Episode(int choice);

typedef struct {
	char** var;
	char* name;
}fstr;

fstr mval[] = {

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

int getMapNum() {
	int map = 0;
	switch (::g->gamemode) {
	case retail:
		if (::g->clusters.size()) {
			if (::g->clusters[::g->gameepisode - 1].startmap) {
				map = ::g->clusters[::g->gameepisode - 1].startmap + (::g->gamemap - 1);
			}
			else {
				map = 0;
			}
		}
		break;
	case commercial:
		map = ::g->gamemap;
		break;
	}
	return map;
}

char* removequotes(char* value) {
	if (!value)
		return value;
	char* tmpvalue = value;
	if (tmpvalue[0] == '\"') {
		tmpvalue = tmpvalue + 1;
	}
	for (int j = 0; j < strlen(tmpvalue); j++) {
		if (tmpvalue[j] == '\"') {
			tmpvalue[j] = '\0';
		}
	}
	return tmpvalue;
}

void setMAP(int index,char* value1, char* value2) {
	int map;
	int endmap;
	
	if (::g->gamemode == retail && ::g->clusters.size()) {
		if (::g->clusters[episodecount].endmap) {
			endmap = ::g->clusters[episodecount].endmap - ::g->clusters[episodecount].startmap;
			endmap++;
		}
		else {
			endmap = 8;
		}
		map = ::g->clusters[episodecount].startmap + index;
		if (index == endmap) {
			episodecount++;
		}
	}
	else {
		map = index + 1;
	}
	if (!::g->mapmax) {
		if ( map > ::g->maps.size()) {
			::g->maps.resize(map);
			::g->mapind = map;
		}
	}
	else {
		if (map > ::g->mapmax) {
			map = ::g->mapmax;
		}
	}

	if (!atoi(value1)) {
		::g->maps[map -1].lumpname = value1;
	}
	else {
		::g->maps[map - 1].lumpname = (char*)malloc(6 * sizeof(char));
		sprintf(::g->maps[map-1].lumpname, "MAP%02d", map);
	}
	if (value2 != NULL) {
		value2 = removequotes(value2);
		::g->maps[map - 1].realname = value2;
	}
}

void EX_add(int lump) {
	char* text = (char*)malloc(W_LumpLength(lump) + 1);
	text[W_LumpLength(lump)] = '\0';
	I_Printf("Applying Expansion Info ...\n");
	W_ReadLump(lump, text);
	::g->mapmax = 0;
	if (!::g->maps.empty()) {
		::g->maps.clear();
	}
	::g->maps.resize(1);
	::g->endmap = 30;
	::g->savedir = NULL;
	if (::g->gamemode == retail) {
		::g->intermusic = mus_inter;
	}
	else {
		::g->intermusic = mus_dm2int;
	}
	//idLib::Printf("%s", text);
	parseexptext(text);
	if (!::g->mapmax) {
		::g->mapmax = ::g->maps.size();
	}
	free(text);
	I_Printf("Expansion Info succesfully applied\n");
}

void parseexptext(char* text) {
	std::vector<std::string> linedtext = getexplines(text);
	int state = 0;
	int val1;
	int val2;
	int val3;
	int mapcount = 0;
	char* varname;
	char* varval;
	char* varopt;
	/*if (!::g->mapind) {
		initMAPS(linedtext);
	}*/

	for (int i = 0; i < linedtext.size(); i++) {
		char* t = strtok(strdup(linedtext[i].c_str()), " ");
		if (t == NULL)
			continue;
		int tstate= checkexpstate(t);
		varname = t;
		t = strtok(NULL, " ");
		if (t != NULL) {
			
			if (!idStr::Icmp(t, "=")) {
				t = strtok(NULL, " ");
			}
		}
			if (tstate) {
				state = tstate;
				if (t != NULL) {
					if (atoi(t + 3)) {
						val1 = atoi(t + 3) - 1;
					}
					else {
						val1 = atoi(t) - 1;
					}
					switch (state) {
					case 2:
						if (val1 == -1) {
							mapcount++;
							val1 = mapcount - 1;
						}
						varopt = t;
						t = strtok(NULL, " ");
						if (t != NULL) {
							varval = t;
						}
						setMAP(val1, varopt, varval);
						break;
					case 3:
						if (val1 == -1) {
							val1 = ::g->EpiDef.numitems;
						}
						if (!::g->clusterind || ::g->clusterind <= ::g->clusters.size()) {
							if (::g->gamemode == retail) {
								if (val1 + 1 > ::g->EpiDef.numitems) {
									::g->EpiDef.numitems = val1 + 1;
									int newval = val1 + 1;
									::g->EpisodeMenu = (menuitem_t*)realloc(::g->EpisodeMenu, newval * sizeof(menuitem_t));
								}
								if (!::g->clusterind) {
									episodecount = val1;
								}
							}
							::g->clusters.resize(val1 + 1);
							::g->clusterind = val1 + 1;
						}
						::g->clusters[val1].fflat = -1;
						if (!atoi(t)) {
							::g->clusters[val1].mapname = t;
						}
						break;
					}
				}
			}
			else {
					//varname = t;

				if (t != NULL) {
					
						varopt = t;
						t = strtok(NULL, " ");
						if (t == NULL || !idStr::Icmpn(varopt, "\"", 1) || !idStr::Icmp(varname, "sky1")) {
							t = varopt;
							varopt = NULL;
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
					if (::g->mapmax && val1 > ::g->mapmax) {
						val1 = ::g->mapmax - 1;
					}
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
				if (text[i] != '\r' && text[i] != '{' && text[i] != '}') {
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
	char* extable[] = {
		"EXP",
		"MAP",
		"clusterdef",
		"episode"
	};
	for (int i = 0; i < 4; i++) {
		if (!idStr::Icmp(text, extable[i])) {
			if (i == 3) {
				return i;
			}
			return i+1;
		}
	}
	return 0;
}

void setEXP(char* name, int value) {
	if (!idStr::Icmp(name, "max_maps")) {
		::g->mapmax = value;
		if (!::g->mapind || ::g->mapind >= ::g->maps.size()) {
			if (::g->mapmax > ::g->maps.size()) {
				::g->maps.resize(::g->mapmax);
			}
			::g->mapind = ::g->mapmax;
		}
			for (int i = 0; i < ::g->mapmax; i++) {
				if (::g->maps[i].lumpname != NULL) {
					continue;
				}
				char* tname = new char[6];
				sprintf(tname, "MAP%02d", i + 1);
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
	expobj mapint[] = {
	{ "secret_map" ,MAXINT,NULL,&::g->maps[pos].secretmap},
	{ "next_map" ,MAXINT,NULL,&::g->maps[pos].nextmap },
	{ "cluster" ,MAXINT,NULL,&::g->maps[pos].cluster },
	{ "par" ,MAXINT,NULL,&::g->maps[pos].par },
	{ "secret_map" ,MAXINT,NULL,&::g->maps[pos].otel }
	};
	for (int i = 0; i < 5; i++) {
		if (!idStr::Icmp(name, mapint[i].name)) {
			switch(i+1) {
			case 1:
				::g->maps[value - 1].nextmap = pos + 1;
			default:
				*mapint[i].ival = value;
				break;
			}
			break;
		}
	}
}

void setMAPSTR(int pos, char* name, char* value) {
	value = removequotes(value);

		expobj mapstr[] = {
			{"miniboss",MAXINT,NULL,NULL,NULL,NULL,&::g->maps[pos].miniboss},
			{"map07special",MAXINT,NULL,NULL,NULL,NULL,&::g->maps[pos].miniboss},
			{"secret_final",MAXINT,NULL,NULL,NULL,NULL,&::g->maps[pos].fsecret},
			{"final_flat",MAXINT,NULL,&::g->maps[pos].fflat},
			{"final_text",MAXINT,&::g->maps[pos].ftext},
			{"final_music",MAXINT,NULL,&::g->maps[pos].fmusic},
			{"music",MAXINT,NULL,&::g->maps[pos].music},
			{"next",MAXINT,&::g->maps[pos].nextmapname,&::g->maps[pos].nextmap},
			{"secretnext",MAXINT,&::g->maps[pos].secretmapname,&::g->maps[pos].secretmap},
			{"sky_tex",MAXINT,&::g->maps[pos].sky},
			{"sky1",MAXINT,&::g->maps[pos].sky},
			{"doorsecret",MAXINT,NULL,NULL,NULL,NULL,&::g->maps[pos].dsecret},
			{"thingsecret",MAXINT,NULL,NULL,NULL,NULL,&::g->maps[pos].tsecret},
			{"cspeclsecret",MAXINT,NULL,NULL,NULL,NULL,&::g->maps[pos].cspecls},
		};

		for (int i = 0; i < 14; i++) {
			if (!idStr::Icmp(name, mapstr[i].name)) {
				switch (i + 1) {
				case 4:
					for (int j = 0; j < 12; j++) {
						if (!idStr::Icmp(finaleflat[j], value)) {
							*mapstr[i].ival = j;
						}
					}
					break;
				case 5:
					for (int j = 0; j < 24; j++) {
						if (!idStr::Icmp(mval[j].name, value)) {
							*mapstr[i].sval = *mval[j].var;
						}
					}
					break;
				case 6:
				case 7:
					for (int j = 1; j < 80; j++) {
						char* musname = value + 2;
						if (::g->S_music[j].name == NULL) {
							::g->S_music[j].name = musname;
							::g->totalmus++;
							*mapstr[i].ival = j;
						}
						if (!idStr::Icmp(musname, ::g->S_music[j].name)) {
							*mapstr[i].ival = j;
						}
					}
					break;
				case 8:
				case 9:
					for (int j = 0; j < ::g->maps.size(); j++) {
						if (!::g->maps[j].lumpname) {
							continue;
						}
						if (!idStr::Icmp(value, ::g->maps[j].lumpname)) {
							*mapstr[i].ival = j;
						}
						else {
							*mapstr[i].sval = value;
							*mapstr[i].ival = -1;
						}
					}
					if (!idStr::Icmpn(value, "EndGame", 7)) {
						::g->endmap = pos + 1;
					}
					break;
				case 10:
				case 11:
					*mapstr[i].sval = value;
					break;
				case 1:
				case 2:
				case 3:
				case 12:
				case 13:
				case 14:
					*mapstr[i].bval = true;
					break;
				}
				break;
			}
		}

}

/*void initMAPS(std::vector<std::string> lines) {
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
						if (map < 10) {
							sprintf(tname, "MAP0%i", map);
							tname[5] = '\0';
						}
						else {
							sprintf(tname, "MAP%i", map);
							tname[5] = '\0';
						}
						::g->maps[map - 1].lumpname = tname;
					}
					::g->maps[map - 1].nextmap = map;
				}
			}
		}
	}
}*/

void setCluster(int pos, char* name, char*value, char* option, int linepos, std::vector<std::string> lines) {
	if (value != NULL) {
		value = removequotes(value);
	}
	int c = 0;
	char* musname;
	expobj clusterobj[] = {
		{"flat",MAXINT,&::g->clusters[pos].ftex,&::g->clusters[pos].fflat},
		{"music",MAXINT,NULL,&::g->clusters[pos].fmusic},
		{"entertext",MAXINT,&::g->clusters[pos].ftext},
		{"exittext",MAXINT,&::g->clusters[pos].ftext},
		{"startmap",MAXINT,NULL,&::g->clusters[pos].startmap},
		{"endmap",MAXINT,NULL,&::g->clusters[pos].endmap},
		{"titlename",MAXINT,NULL},
		{"picname",MAXINT,NULL},
		{"key",MAXINT,NULL}
	};
	for (int i = 0; i < 7; i++) {
		if (!idStr::Icmp(name, clusterobj[i].name)){
			switch (i + 1) {
			case 1:
				for (int j = 0; j < 12; j++) {
					if (!idStr::Icmp(finaleflat[j], value)) {
						*clusterobj[i].ival = j;
						break;
					}
				}
				*clusterobj[i].sval = value;
				break;
			case 2:
				c = 0;
				while (value[c] != '_' && c < strlen(value)) {
					c++;
				}
				if (c == strlen(value)) {
					c = 0;
				}
				if (c) {
					c++;
				}
				musname = new char[strlen(value + c)];
				musname = value + c;
				for (int j = 1; j < 80; j++) {

					if (::g->S_music[j].name == NULL) {
						::g->S_music[j].name = musname;
						::g->totalmus++;
						*clusterobj[i].ival = j;
					}
					if (!idStr::Icmp(musname, ::g->S_music[j].name)) {
						*clusterobj[i].ival = j;
						break;
					}
				}
				break;
			case 5:
			case 6:
				*clusterobj[i].ival = atoi(value);
				break;
			case 7:
			case 8:
				::g->EpisodeMenu[pos].status = 1;
				strcpy(::g->EpisodeMenu[pos].name, value);
				::g->EpisodeMenu[pos].routine = M_Episode;
				::g->EpisodeMenu[pos].alphaKey = 'c';
				::g->EpiDef.menuitems = ::g->EpisodeMenu;
				break;
			case 9:
				::g->EpisodeMenu[pos].alphaKey = value[0];
				::g->EpiDef.menuitems = ::g->EpisodeMenu;
				break;
			case 3:
				tex++;
				::g->clusters[pos].textpr = tex;

			case 4:
				if (i + 1 == 4) {
					::g->clusters[pos].textpr = 1;
				}
			default:
				if (option != NULL) {
					if (!idStr::Icmp(option, "lookup")) {
						if (value[0] == 'X') {
							value[0] = 'C';
						}
						for (int j = 0; j < 24; j++) {
							if (!idStr::Icmp(mval[j].name, value)) {
								*clusterobj[i].sval = *mval[j].var;
							}
						}

					}
				}
				else {
					idStr lval;
					char* t = strtok(strdup(lines[linepos].c_str()), " ");
					t = strtok(NULL, " ");
					if (t[0] = '\"') {
						t = t + 1;
					}
					lval += t;
					lval += "\n";
					for (int j = linepos+1; j < lines.size(); j++) {
						if (lines[j].c_str()[lines[j].size()-1] == '\"') {
							lines[j].at(lines[j].size()-1) = '\0';
							lval += lines[j].c_str();
							break;
						}
						lval += lines[j].c_str();
						lval += "\n";
					}
					*clusterobj[i].sval = new char[lval.Size()];
					strcpy(*clusterobj[i].sval, lval.c_str());
				}
				break;
			}
			break;
		}
	}
	/*return;
	if (!idStr::Icmp(name, "flat")) {
		
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
		char* option = value - c;
		if (strlen(option) > 2) {
			option = option + 2;
		}
		char* musname = value + c;
		if (musname[sizeof(musname) - 2] == '\"') {
			musname[sizeof(musname) - 2] = '\0';
		}
		if (!idStr::Icmp(option, "music")) {
			for (int i = 1; i < NUMMUSIC; i++) {
				if (!idStr::Icmp(musname, getENumName(i))) {
					::g->clusters[pos].fmusic = i;
					return;
				}
			}
		}
		for (int i = 1; i < 80; i++) {
			
			if (::g->S_music[i].name == NULL) {
				::g->S_music[i].name = musname;
				::g->totalmus++;
				::g->clusters[pos].fmusic = i;
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
					if (!idStr::Icmp(mval[i].name, value)) {
						::g->clusters[pos].ftext = *mval[i].var;
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

	if (!idStr::Icmp(name, "startmap")) {
		::g->clusters[pos].startmap = atoi(value);
	}
	if (!idStr::Icmp(name, "endmap")) {
		::g->clusters[pos].endmap = atoi(value);
	}
	if (!idStr::Icmp(name, "titlename")) {
		::g->EpisodeMenu[pos].status = 1 ;
		strcpy(::g->EpisodeMenu[pos].name, value);
		::g->EpisodeMenu[pos].routine = M_Episode;
		::g->EpisodeMenu[pos].alphaKey = 'c';
		::g->EpiDef.menuitems = ::g->EpisodeMenu;
	}*/
}

void setExpData(char* name, char* value) {
	if (!idStr::Icmp(name,"clearepisodes")) {
		::g->EpiDef.numitems = 0;
		free(::g->EpisodeMenu);
		::g->EpisodeMenu = (menuitem_t*)malloc(sizeof(menuitem_t));
		return;
	}
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

const char* getENumName(int i) {
	musicinfo_t temp_S_music[80] = {
		{ 0 },
	{ "e1m1", 0 },
	{ "e1m2", 0 },
	{ "e1m3", 0 },
	{ "e1m4", 0 },
	{ "e1m5", 0 },
	{ "e1m6", 0 },
	{ "e1m7", 0 },
	{ "e1m8", 0 },
	{ "e1m9", 0 },
	{ "e2m1", 0 },
	{ "e2m2", 0 },
	{ "e2m3", 0 },
	{ "e2m4", 0 },
	{ "e2m5", 0 },
	{ "e2m6", 0 },
	{ "e2m7", 0 },
	{ "e2m8", 0 },
	{ "e2m9", 0 },
	{ "e3m1", 0 },
	{ "e3m2", 0 },
	{ "e3m3", 0 },
	{ "e3m4", 0 },
	{ "e3m5", 0 },
	{ "e3m6", 0 },
	{ "e3m7", 0 },
	{ "e3m8", 0 },
	{ "e3m9", 0 },
	{ "inter", 0 },
	{ "intro", 0 },
	{ "bunny", 0 },
	{ "victor", 0 },
	{ "introa", 0 },
	{ "runnin", 0 },
	{ "stalks", 0 },
	{ "countd", 0 },
	{ "betwee", 0 },
	{ "doom", 0 },
	{ "the_da", 0 },
	{ "shawn", 0 },
	{ "ddtblu", 0 },
	{ "in_cit", 0 },
	{ "dead", 0 },
	{ "stlks2", 0 },
	{ "theda2", 0 },
	{ "doom2", 0 },
	{ "ddtbl2", 0 },
	{ "runni2", 0 },
	{ "dead2", 0 },
	{ "stlks3", 0 },
	{ "romero", 0 },
	{ "shawn2", 0 },
	{ "messag", 0 },
	{ "count2", 0 },
	{ "ddtbl3", 0 },
	{ "ampie", 0 },
	{ "theda3", 0 },
	{ "adrian", 0 },
	{ "messg2", 0 },
	{ "romer2", 0 },
	{ "tense", 0 },
	{ "shawn3", 0 },
	{ "openin", 0 },
	{ "evil", 0 },
	{ "ultima", 0 },
	{ "read_m", 0 },
	{ "dm2ttl", 0 },
	{ "dm2int", 0 }
	};
	return temp_S_music[i].name;
}