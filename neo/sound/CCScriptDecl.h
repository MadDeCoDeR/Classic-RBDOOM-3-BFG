#ifndef __CCSCRIPTDECL_H__
#define __CCSCRIPTDECL_H__
#include "precompiled.h"

class idCaption {
public:
	idCaption() {}

	void SetName(const char* _name) {
		name = idStr(_name);
	}

	idStr GetName() {
		return name;
	}

	void SetColor(float x, float y, float z, float w) {
		color = idVec4(x, y, z, w);
	}

	idVec4 GetColor() {
		return color;
	}

	void SetCaption(const char* _caption) {
		caption = idStr(_caption);
	}

	idStr GetCaption() {
		return caption;
	}

	void SetPriority(int _priority) {
		priority = _priority;
	}

	int GetPriority() {
		return priority;
	}

	void SetTimeCode(int _timecode) {
		timecode = _timecode;
	}

	int GetTimeCode() {
		return timecode;
	}

private:
	idStr name;
	idVec4 color;
	idStr caption;
	int priority;
	int timecode;
};

class CCScriptDecl {
public:
	bool LoadFile(const char* fileName, bool OSPath = false);
	bool FindCaption(const char* name, idCaption** caption);
	bool FindCaptionWithTimeCode(const char* name, int timecode, idCaption** caption);
	bool HasMultipleCaptions(const char* name);
	void Clear();
private:
	bool ReadCaption(idLexer& src, idCaption* caption);
	idList<idCaption*> captions;
};



#endif // !__CCSCRIPTDECL_H__
