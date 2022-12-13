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
	const char* name;
	long limit;
	char** sval;
	int* ival;
	long* lval;
	float* fval;
	bool* bval;
	statenum_t* stval;
	ammotype_t* amval;
}expobj;
int episodecount = -1;
int realmap = 0;
//int episodecount1 = 0;
//int episodecount2 = 0;

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

bool beginepisode = false;
bool BFGEpisodic = false;

typedef struct {
	char** var;
	const char* name;
}fstr;
//Allow map names to be retrieved from the base ones
fstr expmapnames[] = {
	{ &mapnames[0],"HUSTR_E1M1" },
	{ &mapnames[1],"HUSTR_E1M2" },
	{ &mapnames[2],"HUSTR_E1M3" },
	{ &mapnames[3],"HUSTR_E1M4" },
	{ &mapnames[4],"HUSTR_E1M5" },
	{ &mapnames[5],"HUSTR_E1M6" },
	{ &mapnames[6],"HUSTR_E1M7" },
	{ &mapnames[7],"HUSTR_E1M8" },
	{ &mapnames[8],"HUSTR_E1M9" },
	{ &mapnames[9],"HUSTR_E2M1" },
	{ &mapnames[10],"HUSTR_E2M2" },
	{ &mapnames[11],"HUSTR_E2M3" },
	{ &mapnames[12],"HUSTR_E2M4" },
	{ &mapnames[13],"HUSTR_E2M5" },
	{ &mapnames[14],"HUSTR_E2M6" },
	{ &mapnames[15],"HUSTR_E2M7" },
	{ &mapnames[16],"HUSTR_E2M8" },
	{ &mapnames[17],"HUSTR_E2M9" },
	{ &mapnames[18],"HUSTR_E3M1" },
	{ &mapnames[19],"HUSTR_E3M2" },
	{ &mapnames[20],"HUSTR_E3M3" },
	{ &mapnames[21],"HUSTR_E3M4" },
	{ &mapnames[22],"HUSTR_E3M5" },
	{ &mapnames[23],"HUSTR_E3M6" },
	{ &mapnames[24],"HUSTR_E3M7" },
	{ &mapnames[25],"HUSTR_E3M8" },
	{ &mapnames[26],"HUSTR_E3M9" },
	{ &mapnames[27],"HUSTR_E4M1" },
	{ &mapnames[28],"HUSTR_E4M2" },
	{ &mapnames[29],"HUSTR_E4M3" },
	{ &mapnames[30],"HUSTR_E4M4" },
	{ &mapnames[31],"HUSTR_E4M5" },
	{ &mapnames[32],"HUSTR_E4M6" },
	{ &mapnames[33],"HUSTR_E4M7" },
	{ &mapnames[34],"HUSTR_E4M8" },
	{ &mapnames[35],"HUSTR_E4M9" },
	{ &mapnames2[0],"HUSTR_1" },
	{ &mapnames2[1],"HUSTR_2" },
	{ &mapnames2[2],"HUSTR_3" },
	{ &mapnames2[3],"HUSTR_4" },
	{ &mapnames2[4],"HUSTR_5" },
	{ &mapnames2[5],"HUSTR_6" },
	{ &mapnames2[6],"HUSTR_7" },
	{ &mapnames2[7],"HUSTR_8" },
	{ &mapnames2[8],"HUSTR_9" },
	{ &mapnames2[9],"HUSTR_10" },
	{ &mapnames2[10],"HUSTR_11" },
	{ &mapnames2[11],"HUSTR_12" },
	{ &mapnames2[12],"HUSTR_13" },
	{ &mapnames2[13],"HUSTR_14" },
	{ &mapnames2[14],"HUSTR_15" },
	{ &mapnames2[15],"HUSTR_16" },
	{ &mapnames2[16],"HUSTR_17" },
	{ &mapnames2[17],"HUSTR_18" },
	{ &mapnames2[18],"HUSTR_19" },
	{ &mapnames2[19],"HUSTR_20" },
	{ &mapnames2[20],"HUSTR_21" },
	{ &mapnames2[21],"HUSTR_22" },
	{ &mapnames2[22],"HUSTR_23" },
	{ &mapnames2[23],"HUSTR_24" },
	{ &mapnames2[24],"HUSTR_25" },
	{ &mapnames2[25],"HUSTR_26" },
	{ &mapnames2[26],"HUSTR_27" },
	{ &mapnames2[27],"HUSTR_28" },
	{ &mapnames2[28],"HUSTR_29" },
	{ &mapnames2[29],"HUSTR_30" },
	{ &mapnames2[30],"HUSTR_31" },
	{ &mapnames2[31],"HUSTR_32" },
	{ &mapnames2[32],"HUSTR_33" },
	{ &mapnamesp[0],"PHUSTR_1" },
	{ &mapnamesp[1],"PHUSTR_2" },
	{ &mapnamesp[2],"PHUSTR_3" },
	{ &mapnamesp[3],"PHUSTR_4" },
	{ &mapnamesp[4],"PHUSTR_5" },
	{ &mapnamesp[5],"PHUSTR_6" },
	{ &mapnamesp[6],"PHUSTR_7" },
	{ &mapnamesp[7],"PHUSTR_8" },
	{ &mapnamesp[8],"PHUSTR_9" },
	{ &mapnamesp[9],"PHUSTR_10" },
	{ &mapnamesp[10],"PHUSTR_11" },
	{ &mapnamesp[11],"PHUSTR_12" },
	{ &mapnamesp[12],"PHUSTR_13" },
	{ &mapnamesp[13],"PHUSTR_14" },
	{ &mapnamesp[14],"PHUSTR_15" },
	{ &mapnamesp[15],"PHUSTR_16" },
	{ &mapnamesp[16],"PHUSTR_17" },
	{ &mapnamesp[17],"PHUSTR_18" },
	{ &mapnamesp[18],"PHUSTR_19" },
	{ &mapnamesp[19],"PHUSTR_20" },
	{ &mapnamesp[20],"PHUSTR_21" },
	{ &mapnamesp[21],"PHUSTR_22" },
	{ &mapnamesp[22],"PHUSTR_23" },
	{ &mapnamesp[23],"PHUSTR_24" },
	{ &mapnamesp[24],"PHUSTR_25" },
	{ &mapnamesp[25],"PHUSTR_26" },
	{ &mapnamesp[26],"PHUSTR_27" },
	{ &mapnamesp[27],"PHUSTR_28" },
	{ &mapnamesp[28],"PHUSTR_29" },
	{ &mapnamesp[29],"PHUSTR_30" },
	{ &mapnamesp[30],"PHUSTR_31" },
	{ &mapnamesp[31],"PHUSTR_32" },
	{ &mapnamest[0],"THUSTR_1" },
	{ &mapnamest[1],"THUSTR_2" },
	{ &mapnamest[2],"THUSTR_3" },
	{ &mapnamest[3],"THUSTR_4" },
	{ &mapnamest[4],"THUSTR_5" },
	{ &mapnamest[5],"THUSTR_6" },
	{ &mapnamest[6],"THUSTR_7" },
	{ &mapnamest[7],"THUSTR_8" },
	{ &mapnamest[8],"THUSTR_9" },
	{ &mapnamest[9],"THUSTR_10" },
	{ &mapnamest[10],"THUSTR_11" },
	{ &mapnamest[11],"THUSTR_12" },
	{ &mapnamest[12],"THUSTR_13" },
	{ &mapnamest[13],"THUSTR_14" },
	{ &mapnamest[14],"THUSTR_15" },
	{ &mapnamest[15],"THUSTR_16" },
	{ &mapnamest[16],"THUSTR_17" },
	{ &mapnamest[17],"THUSTR_18" },
	{ &mapnamest[18],"THUSTR_19" },
	{ &mapnamest[19],"THUSTR_20" },
	{ &mapnamest[20],"THUSTR_21" },
	{ &mapnamest[21],"THUSTR_22" },
	{ &mapnamest[22],"THUSTR_23" },
	{ &mapnamest[23],"THUSTR_24" },
	{ &mapnamest[24],"THUSTR_25" },
	{ &mapnamest[25],"THUSTR_26" },
	{ &mapnamest[26],"THUSTR_27" },
	{ &mapnamest[27],"THUSTR_28" },
	{ &mapnamest[28],"THUSTR_29" },
	{ &mapnamest[29],"THUSTR_30" },
	{ &mapnamest[30],"THUSTR_31" },
	{ &mapnamest[31],"THUSTR_32" },

};

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

enum CLUSTER {
	FLAT,
	MUSIC,
	ENTERTXT,
	EXITTXT,
	STARTMAP,
	ENDMAP,
	TITLENAME,
	PICNAME,
	KEY,
	INTERPIC,
	ENDMODE,
	ALLOWALL,
	MAXCLUSTER

};

void setMapNum() {
	::g->map = 0;
	switch (::g->gamemode) {
	case retail:
		if (::g->clusters.size()) {
			if (::g->clusters[::g->gameepisode - 1].startmap) {
				int map = ::g->gamemap >= ::g->clusters[::g->gameepisode - 1].startmap ? (::g->gamemap - ::g->clusters[::g->gameepisode - 1].startmap) : (::g->gamemap - 1);
				::g->map = ::g->clusters[::g->gameepisode - 1].startmap + map;
			}
			else {
				::g->map = 0;
			}
		}
		break;
	case commercial:
		::g->map = ::g->gamemap;
		break;
	}
}

char* getFinalText(char* value)
{
	if (value) {
		bool found = false;
		for (int j = 0; j < 24; j++) {
			if (!idStr::Icmp(mval[j].name, value)) {
				return *mval[j].var;
#ifndef _MSC_VER
				found = true;
#endif
				break;
			}
		}
		if (!found) {
			return value;
		}
	}
	return NULL;
}

char* removequotes(char* value) {
	if (!value)
		return value;
	char* tmpvalue = value;
	if (tmpvalue[0] == '\"') {
		tmpvalue = tmpvalue + 1;
	}
	for (uint j = 0; j < strlen(tmpvalue); j++) {
		//Name with spaces HACK
		if (tmpvalue[j] == ';') {
			tmpvalue[j] = ' ';
		}
		if (tmpvalue[j] == '\"') {
			tmpvalue[j] = ' ';
		}
		if (tmpvalue[j] == '\\' && tmpvalue[j + 1] == 'n') {
			tmpvalue[j] = ' ';
			tmpvalue[j + 1] = '\n';
		}
	}
	return tmpvalue;
}

int calculateD1map(int map, int& counter) {
	//int endmap;
	int result = map;
	
	if (::g->clusters.size()) {
		/*if (::g->clusters[counter].endmap) {
			endmap = ::g->clusters[counter].endmap - ::g->clusters[counter].startmap;
			endmap++;
		}
		else {
			endmap = 8;
		}*/
		if (!BFGEpisodic) {
			result = ::g->clusters[counter].startmap + map;
		}
		else {
			result = realmap;
		}
		/*if (map == endmap) {
			counter++;
		}*/
	}
	return result;
}

void setMAP(int index,char* value1, char* value2, char* value3) {
	int map;
	//int endmap;
	
	if (::g->gamemode == retail) {
		if (beginepisode) {
			if (episodecount >= (int)::g->clusters.size()) {
				::g->clusters.resize(episodecount + 1);
				::g->clusters[episodecount].startmap = realmap;
			}
			
			beginepisode = false;
		}
		map = calculateD1map(index,episodecount);
	}
	else {
		map = index + 1;
	}
	if (!::g->mapmax) {
		if ( map > (int)::g->maps.size()) {
			::g->maps.resize(map);
			::g->mapind = map;
		}
	}
	else {
		if (map > (int)::g->maps.size()) {
			map = ::g->maps.size() - 1;
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
		//Either take the name from lookup or directly
		if (!idStr::Icmp("lookup", value2)) {
			value3 = removequotes(value3);
			for (int i = 0; i < sizeof(expmapnames) / sizeof(fstr); i++) {
				if (!idStr::Icmp(expmapnames[i].name, value3)) {
					::g->maps[map - 1].realname = *expmapnames[i].var;
					break;
				}
			}
		}
		else {
			::g->maps[map - 1].realname = value2;
		}
	}
}

void EX_add(int lump) {
	char* text = (char*)malloc(W_LumpLength(lump) + 1);
	text[W_LumpLength(lump)] = '\0';
	I_Printf("Applying Expansion Info ...\n");
	W_ReadLump(lump, text);
	::g->mapmax = 0;
	/*if (!::g->maps.empty()) {
		::g->maps.clear();
	}
	if (!::g->clusters.empty()) {
		::g->clusterind = 0;
		::g->clusters.clear();
	}*/
	if (::g->maps.empty()) {
		::g->maps.resize(1);
		::g->endmap = 30;
		realmap = 0;
		episodecount = -1;
	}
	::g->savedir = NULL;
	if (::g->gamemode == retail) {
		beginepisode = false;
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
	int val1 = 0;
	//int val2;
	int val3;
	int mapcount = 0;
	char* varname;
	char* varval;
	char* varopt = 0;
	char* varval2 = NULL; //SANITY CHECK
	/*if (!::g->mapind) {
		initMAPS(linedtext);
	}*/

	for (uint i = 0; i < linedtext.size(); i++) {
		varval = NULL;
		varval2 = NULL;
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
						if (::g->gamemode == retail && val1 == 0) {
							episodecount++;
							if (!beginepisode) {
								beginepisode = true;
							}
						}
						realmap++;
						if (realmap >= (int)::g->maps.size()) {
							::g->maps.resize(realmap + 1);
							::g->mapmax++;
						}
						varopt = t;
						t = strtok(NULL, " ");
						if (t != NULL) {
							varval = t;
						}
						t = strtok(NULL, " ");
						if (t != NULL) {
							varval2 = t;
						}
						setMAP(val1, varopt, varval, varval2);
						break;
					case 3:
						if (val1 == -1) {
							val1 = ::g->EpiDef.numitems;
						}
						if (atoi(t + 1)) {
							val1 = atoi(t + 1) - 1;
						}
						if ((!::g->clusterind || ::g->clusterind <= (int)::g->clusters.size()) && ::g->clusterind <= val1) {
							if (::g->gamemode == retail) {
								if (val1 + 1 > ::g->EpiDef.numitems) {
									::g->EpiDef.numitems = val1 + 1;
									int newval = val1 + 1;
									::g->EpisodeMenu = (menuitem_t*)realloc(::g->EpisodeMenu, newval * sizeof(menuitem_t));
								}
							}
							::g->clusters.resize(val1 + 1);
							::g->clusterind = val1 + 1;
						}
						::g->clusters[val1].fflat = -1;
						::g->clusters[val1].startmap = realmap + 1;
						episodecount = val1;
						if (!atoi(t)) {
							::g->clusters[val1].mapname = t;
							if (::g->gamemode == retail) {
								beginepisode = true;
							}
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
					else if (!idStr::Icmp(varname, "rich_precense")) {
						::g->acronymPrefix = t;
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
					setCluster(val1, varname, varval, varopt, i, linedtext );
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
		qboolean isname = false;
		while (text[i] != '\n') {
			if (text[i] == '#')
				ignore = true;

			if (!ignore) {
				//replace spaces with ; if we found quotes(indicating a name)
				if (text[i] == '\"') {
					isname = !isname;
				}
				if (text[i] != '\r' && text[i] != '{' && text[i] != '}' && text[i] != '\t') {
					if (isname && text[i] == ' ') {
						letter += ';';
					}
					else {
						letter += text[i];
					}
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
	const char* extable[] = {
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
		if (!::g->mapind || ::g->mapind >= (int)::g->maps.size()) {
			if (::g->mapmax > (int)::g->maps.size()) {
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
				::g->maps[i].nextmap = i + 2;
				::g->maps[i].index = i + 1;
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
	if (::g->gamemode == retail) {
		pos = calculateD1map(pos,episodecount)-1;
	}
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
	if (value != NULL) {
		if (value[strlen(value) - 1] == ' ') {
			value[strlen(value) - 1] = '\0';
		}
	}

	if (::g->gamemode == retail) {
		pos = calculateD1map(pos,episodecount) -1;
	}
		expobj mapstr[] = {
			{"miniboss",MAXINT, &::g->maps[pos].bossaction,NULL,NULL,NULL,&::g->maps[pos].miniboss},
			{"map07special",MAXINT, &::g->maps[pos].bossaction,NULL,NULL,NULL,&::g->maps[pos].miniboss},
			{"secret_final",MAXINT,NULL,NULL,NULL,NULL,&::g->maps[pos].fsecret},
			{"final_flat",MAXINT,&::g->maps[pos].fflatname,&::g->maps[pos].fflat},
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
			{"allowmonstertelefrags",MAXINT,NULL,NULL,NULL,NULL,&::g->maps[pos].monstertelefrag},
			{"mastermindboss", MAXINT, &::g->maps[pos].bossaction, &::g->maps[pos].bossname},
			{"cyberboss", MAXINT, &::g->maps[pos].bossaction, &::g->maps[pos].bossname}

		};

		for (int i = 0; i < 17; i++) {
			if (!idStr::Icmp(name, mapstr[i].name)) {
				bool found = false;
				switch (i + 1) {
				case 4:
					for (int j = 0; j < 12; j++) {
						if (!idStr::Icmp(finaleflat[j], value)) {
							*mapstr[i].ival = j;
							found = true;
						}
					}
					//if it failed to find it's index then take the name as is
					if (!found) {
						*mapstr[i].ival = -1;
						*mapstr[i].sval = value;
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
							break;
						}
					}
					break;
				case 8:
				case 9:
					for (int j = 0; j < (int)::g->maps.size(); j++) {
						if (!::g->maps[j].lumpname) {
							continue;
						}
						if (!idStr::Icmp(value, ::g->maps[j].lumpname)) {
							*mapstr[i].ival = ::g->maps[j].index;
							break;
						}
					}
					if (!*mapstr[i].ival) {
						*mapstr[i].sval = value;
						*mapstr[i].ival = -1;
					}
					if (!idStr::Icmpn(value, "EndGame", 7) || !idStr::Icmpn(value, "endbunny", 8)) {
						::g->endmap = pos + 1;
						if (::g->gamemode == retail) {
							::g->clusters[episodecount].endmap = ::g->endmap - (::g->clusters[episodecount].startmap - 1);
							beginepisode = true;
						}
					}
					break;
				case 5:
				case 10:
				case 11:
					*mapstr[i].sval = value;
					break;
				case 1:
				case 2:
					*mapstr[i].sval = value;
				case 3:
				case 12:
				case 13:
				case 14:
				case 15:
					*mapstr[i].bval = true;
					break;
				case 16:
					*mapstr[i].ival = MT_SPIDER;
					*mapstr[i].sval = value;
					break;
				case 17:
					*mapstr[i].ival = MT_CYBORG;
					*mapstr[i].sval = value;
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
		if (value[strlen(value) - 1] == ' ') {
			value[strlen(value) - 1] = '\0';
		}
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
		{"key",MAXINT,NULL},
		{"interpic",MAXINT,&::g->clusters[pos].interpic},
		{"endmode", MAXINT,NULL, &::g->clusters[pos].endmode},
		{"allowall", MAXINT, NULL,NULL,NULL,NULL, &::g->clusters[pos].allowall}
		
	};
	for (int i = 0; i < MAXCLUSTER; i++) {
		if (!idStr::Icmp(name, clusterobj[i].name)){
			switch (i) {
			case FLAT:
				for (int j = 0; j < 12; j++) {
					if (!idStr::Icmp(finaleflat[j], value)) {
						*clusterobj[i].ival = j;
						break;
					}
				}
				*clusterobj[i].sval = value;
				break;
			case MUSIC:
				c = 0;
				while (value[c] != '_' && c < (int)strlen(value)) {
					c++;
				}
				if (c == (int)strlen(value)) {
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
			case STARTMAP:
			case ENDMAP:
				*clusterobj[i].ival = atoi(value);
				BFGEpisodic = true;
				break;
			case TITLENAME:
			case PICNAME:
				::g->EpisodeMenu[pos].status = 1;
				strcpy(::g->EpisodeMenu[pos].name, value);
				::g->EpisodeMenu[pos].routine = M_Episode;
				::g->EpisodeMenu[pos].alphaKey = 'c';
				::g->EpiDef.menuitems = ::g->EpisodeMenu;
				break;
			case KEY:
				::g->EpisodeMenu[pos].alphaKey = value[0];
				::g->EpiDef.menuitems = ::g->EpisodeMenu;
				break;
			case INTERPIC:
				*clusterobj[i].sval = removequotes(value);
				break;
			case ENDMODE:
				*clusterobj[i].ival = atoi(value);
				break;
			case ALLOWALL:
				*clusterobj[i].bval = true;
				break;
			case ENTERTXT:
				tex++;
				::g->clusters[pos].textpr = tex;

			case EXITTXT:
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
					/*char* t = strtok(strdup(lines[linepos].c_str()), " ");
					t = strtok(NULL, " ");
					if (t[0] = '\"') {
						t = t + 1;
					}
					lval += t;
					lval += "\n";*/
					lines[linepos] = strtok(strdup(lines[linepos].c_str()), " ");
					lines[linepos] = strtok(NULL, " ");
					for (uint j = linepos; j < lines.size(); j++) {
						/*if (lines[j].c_str()[lines[j].size()-1] == '\"') {
							lines[j].at(lines[j].size()-1) = '\0';
							lval += lines[j].c_str();
							break;
						}*/
						const char* tocheck = removequotes(strdup(lines[j].c_str()));
						lval += tocheck;
						if (tocheck[strlen(tocheck) - 1] == ' ') {
							break;
						}
						/*lval += "\n";*/

					}
					*clusterobj[i].sval = new char[lval.Length()];
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