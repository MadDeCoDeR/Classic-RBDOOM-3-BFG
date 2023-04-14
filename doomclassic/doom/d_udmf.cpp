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
#include "doomtype.h"
#include "info.h"
#include "i_system.h"
#include "w_wad.h"
#include "p_local.h"
#include "m_bbox.h"
#include "d_udmf.h"
#include "m_swap.h"
//#ifdef USE_OPENAL
#include "s_efx.h"
//#endif

long numlinedefs;
long numthings;
long numsectors;
long numsidedefs;
long numvertexes;

typedef struct {
	const char* name;
	fixed_t* value;
	short* svalue;
	vertex_t** vvalue;
	long* lvalue;
	sector_t** secvalue;
}vertype;


bool ignoremult;

std::vector<std::string> getudmflines(char* text) {
	ignoremult = false;
	numlinedefs=0;
	numthings=0;
	numsectors=0;
	numsidedefs=0;
	numvertexes=0;
	std::vector<std::string> lines;
	typedef struct {
		const char* name;
		long* count;
	}Comp;
	Comp nets[5] = {
		{"linedef",&numlinedefs},
		{"sidedef",&numsidedefs},
		{"sector",&numsectors},
		{"vertex",&numvertexes},
		{"thing",&numthings}
	};
	int size = strlen(text);
	for (int i = 0; i < size /*- 7*/; i++) {
		std::string letter = "";
		qboolean ignore = false;
		while (text[i] != '\n') {
			if (text[i+1] == '{' || text[i+1] == '}'|| text[i] == ';')
				break;
			if ((text[i] == '/' && text[i + 1] == '/'))
				ignore = true;
			if (text[i] == '/' && text[i + 1] == '*') {
				ignoremult = true;
			}
			else if (text[i] == '*' && text[i + 1] == '/') {
				ignoremult = false;
			}

			if (!ignore && !ignoremult) {
				if (text[i] != '\r' && text[i] != ' ' && text[i] != '\"') {
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
		
		for (int g = 0; g < 5; g++) {
			if (!idStr::Icmp(nets[g].name, letter.c_str())) {
				*nets[g].count = *nets[g].count+1;
			}
		}
		if (idStr::Icmp(letter.c_str(), "")) {
			lines.push_back(letter);
		}

	}
	return lines;
}

void ParseLinedef(std::vector<std::string> lines, int index) {
	::g->lines[index].flags = 0;
	vertype lt[15] = {
		{"id",NULL,&::g->lines[index].tag},
		{"v1",NULL,NULL,&::g->lines[index].v1},
		{"v2",NULL,NULL,&::g->lines[index].v2},
		{"blocking",NULL,&::g->lines[index].flags},
		{"blockmonsters",NULL,&::g->lines[index].flags},
		{"twosided",NULL,&::g->lines[index].flags},
		{"dontpegtop",NULL,&::g->lines[index].flags},
		{"dontpegbottom",NULL,&::g->lines[index].flags},
		{"secret",NULL,&::g->lines[index].flags},
		{"blocksound",NULL,&::g->lines[index].flags },
		{"dontdraw",NULL,&::g->lines[index].flags },
		{"mapped",NULL,&::g->lines[index].flags },
		{"special",NULL,&::g->lines[index].special },
		{"sidefront",NULL,NULL,NULL,&::g->lines[index].sidenum[0] },
		{"sideback",NULL,NULL,NULL,&::g->lines[index].sidenum[1] },
	};
	char* token1, *token2;
	for (uint i = 0; i < lines.size(); i++) {
		token1 = strtok(strdup(lines[i].c_str()), "=");
		token2 = strtok(NULL, "=");
		for (int k = 0; k < 15; k++) {
			if (!idStr::Icmp(lt[k].name, token1)) {
				switch (k) {
				case 1:
				case 2:
					*lt[k].vvalue=&::g->vertexes[SHORT(atoi(token2)) & 0xffff];
					break;
				case 3:
					*lt[k].svalue = *lt[k].svalue | ML_BLOCKING;
					break;
				case 4:
					*lt[k].svalue = *lt[k].svalue | ML_BLOCKMONSTERS;
					break;
				case 5:
					*lt[k].svalue = *lt[k].svalue | ML_TWOSIDED;
					break;
				case 6:
					*lt[k].svalue = *lt[k].svalue | ML_DONTPEGTOP;
					break;
				case 7:
					*lt[k].svalue = *lt[k].svalue | ML_DONTPEGBOTTOM;
					break;
				case 8:
					*lt[k].svalue = *lt[k].svalue | ML_SECRET;
					break;
				case 9:
					*lt[k].svalue = *lt[k].svalue | ML_SOUNDBLOCK;
					break;
				case 10:
					*lt[k].svalue = *lt[k].svalue | ML_DONTDRAW;
					break;
				case 11:
					*lt[k].svalue = *lt[k].svalue | ML_MAPPED;
					break;
				case 13:
				case 14:
					*lt[k].lvalue=(int)atoi(token2) == -1 ? -1l : (int)atoi(token2) & 0xffff;
					break;
				default:
					*lt[k].svalue = SHORT(atoi(token2));
				}
			}
		}
	}

	::g->lines[index].dx = ::g->lines[index].v2->x - ::g->lines[index].v1->x;
	::g->lines[index].dy = ::g->lines[index].v2->y - ::g->lines[index].v1->y;

	if (!::g->lines[index].dx)
		::g->lines[index].slopetype = ST_VERTICAL;
	else if (!::g->lines[index].dy)
		::g->lines[index].slopetype = ST_HORIZONTAL;
	else
	{
		if (FixedDiv(::g->lines[index].dy, ::g->lines[index].dx) > 0)
			::g->lines[index].slopetype = ST_POSITIVE;
		else
			::g->lines[index].slopetype = ST_NEGATIVE;
	}

	if (::g->lines[index].v1->x < ::g->lines[index].v2->x)
	{
		::g->lines[index].bbox[BOXLEFT] = ::g->lines[index].v1->x;
		::g->lines[index].bbox[BOXRIGHT] = ::g->lines[index].v2->x;
	}
	else
	{
		::g->lines[index].bbox[BOXLEFT] = ::g->lines[index].v2->x;
		::g->lines[index].bbox[BOXRIGHT] = ::g->lines[index].v1->x;
	}

	if (::g->lines[index].v1->y < ::g->lines[index].v2->y)
	{
		::g->lines[index].bbox[BOXBOTTOM] = ::g->lines[index].v1->y;
		::g->lines[index].bbox[BOXTOP] = ::g->lines[index].v2->y;
	}
	else
	{
		::g->lines[index].bbox[BOXBOTTOM] = ::g->lines[index].v2->y;
		::g->lines[index].bbox[BOXTOP] = ::g->lines[index].v1->y;
	}
	::g->lines[index].frontsector = ::g->sides[::g->lines[index].sidenum[0]].sector;

	if (::g->lines[index].sidenum[1] != -1)
		::g->lines[index].backsector = ::g->sides[::g->lines[index].sidenum[1]].sector;
	else
		::g->lines[index].backsector = 0;
}

void ParseSidedef(std::vector<std::string> lines, int index) {
	vertype st[6] = {
		{"offsetx",&::g->sides[index].textureoffset},
		{"offsety",&::g->sides[index].rowoffset},
		{"texturetop",NULL,&::g->sides[index].toptexture},
		{"texturebottom",NULL,&::g->sides[index].bottomtexture},
		{"texturemiddle",NULL,&::g->sides[index].midtexture},
		{"sector",NULL,NULL,NULL,NULL,&::g->sides[index].sector}
	};
	char* token1, *token2;
	for (uint i = 0; i < lines.size(); i++) {
		token1 = strtok(strdup(lines[i].c_str()), "=");
		token2 = strtok(NULL, "=");
		for (int k = 0; k < 6; k++) {
			if (!idStr::Icmp(st[k].name, token1)) {
				switch (k) {
				case 0:
				case 1:
					*st[k].value = SHORT(atoi(token2)) << FRACBITS;
					break;
				case 2:
				case 3:
				case 4:
					*st[k].svalue = R_TextureNumForName(token2);
					break;
				case 5:
					*st[k].secvalue = &::g->sectors[SHORT(atoi(token2)) & 0xffff];
				}
			}
		}
	}
}

void ParseSector(std::vector<std::string> lines, int index) {
	
	vertype sect[8] = {
		{"heightfloor",&::g->sectors[index].floorheight},
		{"heightceiling",&::g->sectors[index].ceilingheight},
		{"texturefloor",NULL,&::g->sectors[index].floorpic},
		{"textureceiling",NULL,&::g->sectors[index].ceilingpic},
		{"lightlevel",NULL,&::g->sectors[index].lightlevel},
		{"type",NULL,&::g->sectors[index].special},
		{"id",NULL,&::g->sectors[index].tag},
		{"special",NULL,&::g->sectors[index].special}
	};
	//GK: Load the reverbs based on sector's index
	::g->sectors[index].counter = index;
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	if (!common->UseAlternativeAudioAPI()) {
#endif
		if (::g->hasreverb) {
			if ((int)::g->reverbs.size() < index + 1) {
				::g->reverbs.push_back(GetReverb(strdup(::g->mapname.c_str()), index));
			}
			else {
				::g->reverbs[index] = GetReverb(strdup(::g->mapname.c_str()), index);
			}
		}
#if defined(_MSC_VER) && defined(USE_XAUDIO2)
	}
#endif
	char* token1, *token2;
	for (uint i = 0; i < lines.size(); i++) {
		token1 = strtok(strdup(lines[i].c_str()), "=");
		token2 = strtok(NULL, "=");
		for (int k = 0; k < 7; k++) {
			if (!idStr::Icmp(sect[k].name, token1)) {
				switch (k) {
				case 0:
				case 1:
					*sect[k].value = SHORT(atoi(token2)) << FRACBITS;
					break;
				case 2:
				case 3:
					*sect[k].svalue = R_FlatNumForName(token2);
					break;
				default:
					*sect[k].svalue = SHORT(atoi(token2));
				}
			}
		}
	}
	::g->sectors[index].thinglist = NULL;
	::g->sectors[index].touching_thinglist = NULL;
}

void ParseThing(std::vector<std::string> lines, int index) {
	qboolean		spawn=true;
	qboolean hardpass = false;
	qboolean easypass = false;
	qboolean netpass = false;
	::g->mapthings[index].options = 0;
	vertype tht[12] = {
		{"x",NULL,&::g->mapthings[index].x},
		{"y",NULL,&::g->mapthings[index].y},
		{"angle",NULL,&::g->mapthings[index].angle},
		{"type",NULL,&::g->mapthings[index].type},
		{"skill1",NULL,&::g->mapthings[index].options},
		{"skill2",NULL,&::g->mapthings[index].options},
		{"skill3",NULL,&::g->mapthings[index].options},
		{"skill4",NULL,&::g->mapthings[index].options},
		{"skill5",NULL,&::g->mapthings[index].options},
		{"ambush",NULL,&::g->mapthings[index].options},
		{"dm",NULL,&::g->mapthings[index].options},
		{"coop",NULL,&::g->mapthings[index].options}
	};

	// Do spawn all other stuff. 
	char* token1, *token2;
	for (uint i = 0; i < lines.size(); i++) {
		token1 = strtok(strdup(lines[i].c_str()), "=");
		token2 = strtok(NULL, "=");
		for (int k = 0; k < 12; k++) {
			if (!idStr::Icmp(tht[k].name, token1)) {
				switch (k) {
				case 3:
					// Do not spawn cool, new monsters if !commercial
					if (::g->gamemode != commercial)
					{
						if (::g->gamemission == pack_custom && ::g->clusters[::g->gameepisode - 1].allowall) {
							//do nothing
						}
						else {
							switch (SHORT(atoi(token2)))
							{
							case 68:	// Arachnotron
							case 64:	// Archvile
							case 88:	// Boss Brain
							case 89:	// Boss Shooter
							case 69:	// Hell Knight
							case 67:	// Mancubus
							case 71:	// Pain Elemental
							case 65:	// Former Human Commando
							case 66:	// Revenant
							case 84:	// Wolf SS
								spawn = false;
								break;
							}
						}
					}
					if (spawn == false)
						return;

					*tht[k].svalue = SHORT(atoi(token2));
					break;
				case 4:
				case 5:
					if (!easypass) {
						*tht[k].svalue = *tht[k].svalue | MTF_EASY;
						easypass = true;
					}
					break;
				case 6:
					*tht[k].svalue = *tht[k].svalue | MTF_NORMAL;
					break;
				case 7:
				case 8:
					if (!hardpass) {
						*tht[k].svalue = *tht[k].svalue | MTF_HARD;
						hardpass = true;
					}
					break;
				case 9:
					*tht[k].svalue = *tht[k].svalue | MTF_AMBUSH;
					break;
				case 10:
				case 11:
					if (!netpass) {
						*tht[k].svalue = *tht[k].svalue | 16;
						netpass = true;
					}
					break;
				default:
					
					*tht[k].svalue = SHORT(atoi(token2));
				}
			}
		}
	}
}

void ParseVertex(std::vector<std::string> lines, int index) {
	//short x, y;
	vertype vt[2] = {
		{"x",&::g->vertexes[index].x},
		{"y",&::g->vertexes[index].y}
	};
	char* token1,*token2;
	for (uint i = 0; i < lines.size(); i++) {
		token1 = strtok(strdup(lines[i].c_str()), "=");
		token2 = strtok(NULL, "=");
		for (int k = 0; k < 2; k++) {
			if (!idStr::Icmp(vt[k].name, token1)) {
				*vt[k].value = SHORT(atoi(token2)) << FRACBITS;
			}
		}
	}
}

void parseudmf(char* text) {
	std::vector<std::string> linedtext = getudmflines(text);
	std::vector<std::string> subtext;
	long lc = numlinedefs-1;
	long sc = numsidedefs-1;
	long sec = numsectors-1;
	long tc = numthings-1;
	long vc = numvertexes-1;
	typedef void(*parseaction)(std::vector<std::string> lines, int index);
	typedef struct {
		const char* name;
		parseaction func;
		long* counter;
	}udmftype;
	udmftype ut[5] = {
		{"linedef",ParseLinedef,&lc},
		{"sidedef",ParseSidedef,&sc},
		{"sector",ParseSector,&sec},
		{"thing",ParseThing,&tc},
		{"vertex",ParseVertex,&vc}
	};
	::g->numlines = numlinedefs;
	::g->numsides = numsidedefs;
	::g->numsectors = numsectors;
	::g->numvertexes = numvertexes;
	::g->nummapthings = numthings;
	::g->lines =(line_t*) Z_Malloc(::g->numlines * sizeof(line_t), PU_LINES, ::g->lines);
	memset(::g->lines, 0, ::g->numlines * sizeof(line_t));
	::g->sides = (side_t*)Z_Malloc(::g->numsides * sizeof(side_t), PU_SIDES, ::g->sides);
	memset(::g->sides, 0, ::g->numsides * sizeof(side_t));
	::g->sectors = (sector_t*)Z_Malloc(::g->numsectors * sizeof(sector_t), PU_SECTORS, NULL);
	memset(::g->sectors, 0, ::g->numsectors * sizeof(sector_t));
	::g->vertexes = (vertex_t*)Z_Malloc(::g->numvertexes * sizeof(vertex_t), PU_VERTEX, ::g->vertexes);
	memset(::g->vertexes, 0, ::g->numvertexes * sizeof(vertex_t));
	::g->mapthings = (mapthing_t*)malloc(::g->nummapthings*sizeof(mapthing_t));
	memset(::g->mapthings, 0, ::g->nummapthings * sizeof(mapthing_t));

	for (int i = linedtext.size()-1; i >= 0; i--) {
		for (int j = 0; j < 5; j++) {
			if (!idStr::Icmp(ut[j].name, linedtext[i].c_str())) {
				linedtext[i] = " ";
				i++;
				while (linedtext[i].c_str()[0] != '}') {
					subtext.push_back(linedtext[i]);
					i++;
				}
				if (*ut[j].counter >= 0) {
					ut[j].func(subtext, *ut[j].counter);
					*ut[j].counter = *ut[j].counter - 1;
				}
				subtext.clear();
			}
		}
	}
}

void LoadUdmf(int lump) {
	char* text = (char*)malloc(W_LumpLength(lump) + 1);
	text[W_LumpLength(lump)] = '\0';
	I_Printf("Reading UDMF map ...\n");
	W_ReadLump(lump, text);

	//idLib::Printf("%s", text);
	parseudmf(text);
	free(text);
}