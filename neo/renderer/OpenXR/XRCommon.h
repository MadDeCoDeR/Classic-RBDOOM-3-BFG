#pragma once
#define XR_USE_GRAPHICS_API_OPENGL
#include "openxr/openxr.h"
#include "openxr/openxr_platform.h"
#include "gfxwrapper_opengl.h"

class idXR {
public:
	void InitXR();
private:
	XrInstance instance = {};
	std::vector<std::string> activeAPILayers = {};
	std::vector<std::string> activeExtensions = {};
	std::vector<std::string> APILayers = {};
	std::vector<std::string> extensions = {};
#ifdef _DEBUG
	XrDebugUtilsMessengerEXT debugMessager = {};
#endif
	XrFormFactor formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
	XrSystemId systemId = {};
	XrSystemProperties systemProperties = { XR_TYPE_SYSTEM_PROPERTIES };
};