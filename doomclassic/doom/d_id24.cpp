/**
* Copyright (C) 2025 George Kalmpokis
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

#include <vector>
#include <map>
#include <string>

#include "d_id24.h"
#include "w_wad.h"
#include "i_system.h"


int64 FindClosure(const char* json, int64 startIndex,const char openingToken, const char closingToken) {
	int64 closingIndex = -1;
	int64 closures = 1;
	idStr idJson = idStr(json);
	while (true) {
		closingIndex = idJson.Find(closingToken, startIndex);
		int64 openingIndex = idJson.Find(openingToken, startIndex);
		if (openingIndex < closingIndex && openingIndex > -1) {
			closures++;
			startIndex = openingIndex + 1;
		}
		else {
			closures--;
			startIndex = closingIndex + 1;
		}

		if (closures == 0) {
			break;
		}
	}
	return closingIndex;
}

int64 FindEndOfLine(const char* json, int64 startIndex) {
	
	idStr idJson = idStr(json);
	int64 eolIndex = idJson.Length();
	int64 indexes[5] = {
		idJson.Find(',', startIndex + 1),
		idJson.Find('\r', startIndex + 1),
		idJson.Find('\n', startIndex + 1),
		idJson.Find('}', startIndex + 1),
		idJson.Find(']', startIndex + 1)
	};
	for (int i = 0; i < 5; i++) {
		if (indexes[i] > -1 && indexes[i] < eolIndex) {
			eolIndex = indexes[i];
		}
	}

	return eolIndex == idJson.Length() ? -1 : eolIndex;
}


std::map<std::string, idStr> RetrieveFlatJsonObj(const char* json) {
	std::map<std::string, idStr> jObj;
	idStr idJson = idStr(json);
	int jsonCharIndex = 0;
	idStr key = "";
	bool activeKey = false;
	while (jsonCharIndex < idJson.Length()) {
		char jsonChar = idJson[jsonCharIndex];
		switch (jsonChar) {
		case '{': {
			if (activeKey) {
				int64 closingIndex = FindClosure(json, jsonCharIndex + 1, '{', '}');
				
				idStr value = idJson.SubStr(jsonCharIndex, closingIndex + 1);
				jObj.insert({ key.c_str(), value});
				jsonCharIndex = closingIndex + 1;
			}
			else {
				jsonCharIndex++;
			}
			break;
		}
		case '\r':
		case '\n':
		case ' ':
		case ':':
		case '}': {
			jsonCharIndex++;
			break;
		}
		case ',': {
			if (activeKey) {
				activeKey = false;
			}
			else {
				I_Error("JSON Parsing Error: Comma is used outside JSON Schema\n");
			}
			jsonCharIndex++;
			break;
		}
		case '\"': {
			int64 lastIndex = idJson.Find('\"', jsonCharIndex + 1);
			idStr tkey = idJson.SubStr(jsonCharIndex + 1, lastIndex);
			jsonCharIndex = lastIndex + 1;
			if (!activeKey) {
				activeKey = true;
				key = tkey;
			}
			else {
				jObj.insert({ key.c_str(), tkey});
			}
			break;
		}
		case '[': {
			if (activeKey) {
				int64 closingIndex = FindClosure(json, jsonCharIndex + 1, '[', ']');

				idStr value = idJson.SubStr(jsonCharIndex, closingIndex + 1);
				jObj.insert({ key.c_str(), value });
				jsonCharIndex = closingIndex + 1;
			}
			else {
				I_Error("JSON Parsing Error: Raw Array is used outside JSON Schema\n");
			}
			break;
		}
		default: {
			if (activeKey) {
				int64 commaIndex = FindEndOfLine(idJson.c_str(), jsonCharIndex);
				idStr value = idJson.SubStr(jsonCharIndex, commaIndex);
				if (commaIndex > -1) {
					jsonCharIndex = commaIndex;
				}
				else {
					jsonCharIndex++;
				}
				jObj.insert({ key.c_str(), value});
			}
			else {
				I_Error("JSON Parsing Error: Raw value is used outside JSON Schema\n");
			}
			break;
		}
		}
	}

	return jObj;
}

std::vector<std::map<std::string, idStr>> RetrieveJsonObjArray(const char* json) {
	std::vector<std::map<std::string, idStr>> jArr;
	idStr idJson = idStr(json);
	int jsonCharIndex = 0;
	while (jsonCharIndex < idJson.Length()) {
		char jsonChar = idJson[jsonCharIndex];
		switch (jsonChar) {
		case '{': {
			int64 closingIndex = FindClosure(json, jsonCharIndex + 1, '{', '}');

			idStr value = idJson.SubStr(jsonCharIndex, closingIndex + 1);
			jArr.push_back(RetrieveFlatJsonObj(value.c_str()));
			jsonCharIndex = closingIndex + 1;
			break;
		}
		case '\r':
		case '\n':
		case ' ':
		case ':':
		case '}':
		case '[': 
		case ']':
		case ',': {
			jsonCharIndex++;
			break;
		}
		}
	}
	return jArr;
}

idStr RetrieveFromJsonPath(const char* path, char* json) {
	idList<idStr> pathParts = idStr(path).Split("/");
	idStr tJson = idStr(json);
	for (int i = 0; i < pathParts.Num(); i++) {
		std::map<std::string, idStr> jObj = RetrieveFlatJsonObj(tJson.c_str());
		if (jObj.count(pathParts[i].c_str())) {
			tJson = jObj[pathParts[i].c_str()];
		}
	}
	return tJson;
}

void MapSkyFlatMaps(const char* json) {
	std::vector<std::map<std::string, idStr>> jArr = RetrieveJsonObjArray(json);
	::g->skyFlatMaps.reserve(jArr.size());
	for (size_t i = 0; i < jArr.size(); i++) {
		::g->skyFlatMaps.emplace_back(std::make_unique<skyflatmap_t>());
		::g->skyFlatMaps[i].get()->flat = strdup(jArr[i]["flat"].c_str());
		::g->skyFlatMaps[i].get()->sky = strdup(jArr[i]["sky"].c_str());
	}

}

void ReadSkyDef(int lump)
{
	//Allocate necessary storage for the SkyDef file
	char* text = (char*)malloc(W_LumpLength(lump) + 1);
	//Sanity Check. Set last character to ending 0
	text[W_LumpLength(lump)] = '\0';
	//Load the SkyDef file to the text buffer
	W_ReadLump(lump, text);

	idStr flatMappingJson = RetrieveFromJsonPath("/data/flatmapping", text);
	if (flatMappingJson.Cmpn("null", 4)) {
		MapSkyFlatMaps(flatMappingJson.c_str());
	}
	free(text);
}
