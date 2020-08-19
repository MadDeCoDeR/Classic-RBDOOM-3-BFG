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