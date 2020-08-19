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
#ifndef  __CREDITDICT_H__
#define __CREDITDICT_H__
#include <map>
#include <vector>

struct creditInfo_t
{

	creditInfo_t()
	{
		type = -1;
		entry = "";
	}

	creditInfo_t(int t, const char* val)
	{
		type = t;
		entry = val;
	}

	int type;
	idStr entry;
};

struct creditType_t {
	char* name;
	int value;
};

class idCredits {
public:
	static void LoadCredits(const byte* buffer, const int bufferLen, const char* name);
	static void PopulateList(idList<creditInfo_t>& list);
	static void ClearList();
};

#endif