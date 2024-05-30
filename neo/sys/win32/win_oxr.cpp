#include "precompiled.h"
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
#ifdef _DEBUG
	extentions.push_back(XR_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
	extentions.push_back(XR_KHR_OPENGL_ENABLE_EXTENSION_NAME);

}