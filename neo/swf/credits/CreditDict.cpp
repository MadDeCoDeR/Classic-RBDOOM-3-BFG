/**
* Copyright (C) 2020 George Kalmpokis
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
#include "CreditDict.h"
#include <string>
#include <map>
#include <vector>

std::vector<creditInfo_t> creditList;

std::map<std::string, int> types = {
	{"Header", 3},
	{"SubHeader", 2},
	{"Title", 1},
	{"Entry", 0}

};

void idCredits::LoadCredits(const byte* buffer, const int bufferLen, const char* name)
{

	if (buffer == NULL || bufferLen <= 0)
	{
		// let whoever called us deal with the failure (so sys_lang can be reset)
		return;
	}

	idLib::Printf("Reading %s", name);

	bool utf8 = false;

	// in all but retail builds, ensure that the byte-order mark is NOT MISSING so that
	// we can avoid debugging UTF-8 code
#ifndef ID_RETAIL
	utf8Encoding_t encoding = idLocalization::VerifyUTF8(buffer, bufferLen, name);
	if (encoding == UTF8_ENCODED_BOM)
	{
		utf8 = true;
	}
	else if (encoding == UTF8_PURE_ASCII)
	{
		utf8 = false;
	}
	else
	{
		assert(false);	// this should have been handled in VerifyUTF8 with a FatalError
		return;
	}
#else
	// in release we just check the BOM so we're not scanning the lang file twice on startup
	if (bufferLen > 3 && buffer[0] == 0xEF && buffer[1] == 0xBB && buffer[2] == 0xBF)
	{
		utf8 = true;
	}
#endif

	if (utf8)
	{
		idLib::Printf(" as UTF-8\n");
	}
	else
	{
		idLib::Printf(" as ASCII\n");
	}

	int line = 0;

	int i = 0;
	while (i < bufferLen)
	{
		bool skipLine = false;
		int tempType = -1;
		idStr tempEntry;
		uint32 c = buffer[i++];
		if (c == '/')    // comment, read until new line
		{
			while (i < bufferLen)
			{
				c = buffer[i++];
				if (c == '\n')
				{
					skipLine = true;
					line++;
					break;
				}
			}
		}
		else if (c == '}')
		{
			break;
		}
		else if (c == '\t' || c == '{' || c == '\r' || c == '\n')
		{
			continue;
		}
		else if (c == '\"')
		{
			idStr tempLine;
			
			int keyStart = i;
			int keyEnd = -1;
			while (i < bufferLen)
			{
				c = buffer[i++];
				if (c == '\"')
				{
					keyEnd = i - 1;
					break;
				}
			}
			if (keyEnd < keyStart)
			{
				idLib::FatalError("%s File ended while reading key at line %d", name, line);
			}
			tempLine.CopyRange((char*)buffer, keyStart, keyEnd);

			char* token = strtok(strdup(tempLine.c_str()), "||");
			int tokenIndex = 0;
			while (token != NULL) {
				if (tokenIndex == 0) {
					if (types.find(token) == types.end()) {
						idLib::FatalError("Token %s is not a supported credit Type", token);
					}
					tempType = types[token];
				}
				else {
					tempEntry += token;
				}
				token = strtok(NULL, "||");
				tokenIndex++;
			}
			
		}
		if (!skipLine) {
			if (tempType < 0) {
				creditList.push_back(creditInfo_t());
			}
			else {
				creditList.push_back(creditInfo_t(tempType, tempEntry));
			}
			line++;
		}
	}
}

void idCredits::PopulateList(idList<creditInfo_t>& list)
{
	list.Clear();
	for (uint i = 0; i < creditList.size(); i++) {
		list.Append(creditList[i]);
	}

}

void idCredits::ClearList()
{
	creditList.clear();
}
