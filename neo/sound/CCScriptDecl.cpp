#include "precompiled.h"

#include "CCScriptDecl.h"

idCVar com_debugCaptions("com_debugCaptions", "0", CVAR_BOOL | CVAR_SOUND | CVAR_NOCHEAT, "Enable/Disable Console Debug for the caption system");

bool CCScriptDecl::LoadFile(const char* fileName, bool OSPath)
{
	idLexer src;

	src.SetFlags(DECL_LEXER_FLAGS);
	src.LoadFile(fileName, OSPath);
	if (!src.IsLoaded()) {
		return false;
	}

	while (!src.EndOfFile()) {
		idCaption* caption = new idCaption;
		if (ReadCaption(src, caption)) {
			captions.Append(caption);
		}
	};

	return true;
}

bool CCScriptDecl::FindCaption(const char* name, idCaption** caption)
{
	for (int i = 0; i < captions.Num(); i++) {
		if (!idStr::Icmp(name, captions[i]->GetName().c_str())) {
			*caption = captions[i];
			return true;
		}
	}
	return false;
}

bool CCScriptDecl::FindCaptionWithTimeCode(const char* name, int timecode, idCaption** caption)
{
	*caption = NULL;
	for (int i = 0; i < captions.Num(); i++) {
		if (!idStr::Icmp(name, captions[i]->GetName()) && timecode >= captions[i]->GetTimeCode()) {
			*caption = captions[i];
		}
	}
	return *caption != NULL;
}

bool CCScriptDecl::HasMultipleCaptions(const char* name)
{
	int count = 0;
	for (int i = 0; i < captions.Num(); i++) {
		if (!idStr::Icmp(name, captions[i]->GetName().c_str())) {
			count++;
		}
	}
	return count > 1;
}

void CCScriptDecl::Clear()
{
	captions.DeleteContents(true);
}

bool CCScriptDecl::ReadCaption(idLexer& src, idCaption* caption)
{
	idToken name, token;

	src.SkipUntilString("{");

	while (1) {
		if (!src.ReadToken(&token))
			return false;

		if (!token.Icmp("}")) {
			break;
		}

		if (!token.Icmp("name")) {
			src.ParseRestOfLine(token);
			caption->SetName(token.c_str());
		}
		else if (!token.Icmp("color")) {
			float color[4];
			src.Parse1DMatrixJSON(4, color);
			caption->SetColor(color[0], color[1], color[2], color[3]);
		}
		else if (!token.Icmp("caption")) {
			int timecode = src.ParseInt();
			if (timecode > 0) {
				caption->SetTimeCode(timecode);
			}
			else {
				caption->SetTimeCode(0);
			}
			src.ParseRestOfLine(token);
			caption->SetCaption(idLocalization::FindString(token.c_str()));
		}
		else if (!token.Icmp("priority")) {
			caption->SetPriority(src.ParseInt());
		}

	}

	return true;
}
