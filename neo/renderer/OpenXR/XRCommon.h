#pragma hdrstop
#define XR_USE_GRAPHICS_API_OPENGL
#include "openxr/openxr.h"
#include "openxr/openxr_platform.h"
#pragma warning( disable : 4005 )
#include <vector>
#include <string>

class idXR {
public:
	void InitXR();
	void ShutDownXR();
private:
	XrInstance instance = {};
	std::vector<const char*> activeAPILayers = {};
	std::vector<const char*> activeExtensions = {};
	std::vector<std::string> APILayers = {};
	std::vector<std::string> extensions = {};
#ifdef _DEBUG
	XrDebugUtilsMessengerEXT debugMessager = {};
#endif
	XrFormFactor formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
	XrSystemId systemId = {};
	XrSystemProperties systemProperties = { XR_TYPE_SYSTEM_PROPERTIES };
};