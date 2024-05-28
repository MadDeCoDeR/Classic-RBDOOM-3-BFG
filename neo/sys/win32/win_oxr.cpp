#include "renderer/OpenXR/XRCommon.h"
#include <string.h>
#include <framework/Licensee.h>
#include <stdlib.h>
#include <string>
#include <vector>


void InitXR() {
	XrApplicationInfo XRAppInfo;
	strncpy(XRAppInfo.applicationName, "DOOM BFA", 8);
	XRAppInfo.apiVersion = XR_CURRENT_API_VERSION;
	XRAppInfo.applicationVersion = atoi(ENGINE_VERSION);
	XRAppInfo.engineVersion = atoi(ENGINE_VERSION);
	strncpy(XRAppInfo.engineName, "DOOM BFA", 8);

	std::vector<std::string> extentions;
	extentions.push_back(OPEN_GL)
}