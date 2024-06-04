#include "precompiled.h"
#include "renderer/OpenXR/XRCommon.h"
#include <string.h>
#include <framework/Licensee.h>
#include <stdlib.h>
#include <string>
#include <vector>


void idXR::InitXR() {
	XrApplicationInfo XRAppInfo;
	strncpy(XRAppInfo.applicationName, "DOOM BFA", 8);
	XRAppInfo.apiVersion = XR_CURRENT_API_VERSION;
	XRAppInfo.applicationVersion = atoi(ENGINE_VERSION);
	XRAppInfo.engineVersion = atoi(ENGINE_VERSION);
	strncpy(XRAppInfo.engineName, "DOOM BFA", 8);

#ifdef _DEBUG
	this->extensions.push_back(XR_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
	this->extensions.push_back(XR_KHR_OPENGL_ENABLE_EXTENSION_NAME);

	uint apiLayerCount = 0;
	std::vector<XrApiLayerProperties> apiLayerProperties;
	if (xrEnumerateApiLayerProperties(0, &apiLayerCount, nullptr) == XR_SUCCESS) {
		apiLayerProperties.resize(apiLayerCount, { XR_TYPE_API_LAYER_PROPERTIES });
		if (xrEnumerateApiLayerProperties(apiLayerCount, &apiLayerCount, apiLayerProperties.data()) == XR_SUCCESS) {
			for (std::string& requestLayer : APILayers) {
				for (XrApiLayerProperties& layerProperty : apiLayerProperties) {
					if (idStr::Icmp(requestLayer.c_str(), layerProperty.layerName)) {
						continue;
					}
					else {
						activeAPILayers.push_back(requestLayer.c_str());
						break;
					}
				}
			}
		}
	}

	uint extensionCount = 0;
	std::vector<XrExtensionProperties> extensionProperties;
	if (xrEnumerateInstanceExtensionProperties(nullptr, 0, &extensionCount, nullptr) == XR_SUCCESS) {
		extensionProperties.resize(extensionCount, { XR_TYPE_EXTENSION_PROPERTIES });
		if (xrEnumerateInstanceExtensionProperties(nullptr, extensionCount, &extensionCount, extensionProperties.data()) == XR_SUCCESS) {
			for (std::string& requestExtension : extensions) {
				bool found = false;
				for (XrExtensionProperties& extensionProperty : extensionProperties) {
					if (idStr::Icmp(requestExtension.c_str(), extensionProperty.extensionName)) {
						continue;
					}
					else {
						activeExtensions.push_back(requestExtension.c_str());
						found = true;
						break;
					}
				}
				if (!found) {
					common->Error("Failed to find OpenXR Extension: %s\n", requestExtension);
				}
			}
		}
	}
}