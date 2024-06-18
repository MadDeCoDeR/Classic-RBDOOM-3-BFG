/**
* Copyright (C) 2024 George Kalampokis
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
#include "renderer/OpenXR/XRCommon.h"
#include <string.h>
#include <framework/Licensee.h>
#include <stdlib.h>
#include "gfxwrapper_opengl.h"



XrBool32 DebugXR(XrDebugUtilsMessageSeverityFlagsEXT messageSeverity, XrDebugUtilsMessageTypeFlagsEXT messageType, const XrDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
	// it probably isn't safe to do an idLib::Printf at this point
	idStr typeStr = "";
	switch (messageType) {
	case XR_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
		typeStr = "General Call";
		break;
	case XR_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
		typeStr = "Performance Call";
		break;
	case XR_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
		typeStr = "Validation Call";
		break;
	case XR_DEBUG_UTILS_MESSAGE_TYPE_CONFORMANCE_BIT_EXT:
		typeStr = "Conformance Call";
		break;
	default:
		typeStr = "Unknown";
		break;
	}

	idStr severityStr = "";
	switch (messageSeverity) {
	case XR_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		severityStr = "Error";
		break;
	case XR_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		severityStr = "Warning";
		break;
	case XR_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		severityStr = "Info";
		break;
	default:
		severityStr = "Notification";
		break;
	}
	char callstack[5000];
	Sys_GetCallStack(callstack);
	// RB: printf should be thread safe on Linux
	idLib::Printf("caught OpenXR Error:\n\tType: %s\n\tSeverity: %s\n\tMessage: %s\n%s", typeStr.c_str(), severityStr.c_str(), pCallbackData->message, callstack);
	// RB end

	return XrBool32();
}

void idXR::InitXR() {
	XrApplicationInfo XRAppInfo;
	strncpy(XRAppInfo.applicationName, "DOOM BFA\0", 9);
	XRAppInfo.apiVersion = XR_CURRENT_API_VERSION;
	XRAppInfo.applicationVersion = atoi(ENGINE_VERSION);
	XRAppInfo.engineVersion = atoi(ENGINE_VERSION);
	strncpy(XRAppInfo.engineName, "DOOM BFA\0", 9);

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
					common->Error("Failed to find OpenXR Extension: %s\n", requestExtension.c_str());
				}
			}
		}
	}

	XrInstanceCreateInfo ici{ XR_TYPE_INSTANCE_CREATE_INFO };
	ici.createFlags = 0;
	ici.applicationInfo = XRAppInfo;
	ici.enabledApiLayerCount = static_cast<uint32_t>(activeAPILayers.size());
	ici.enabledApiLayerNames = activeAPILayers.data();
	ici.enabledExtensionCount = static_cast<uint32_t>(activeExtensions.size());
	ici.enabledExtensionNames = activeExtensions.data();
	XrResult result = xrCreateInstance(&ici, &instance);
	if (result == XR_ERROR_API_VERSION_UNSUPPORTED) {
		XRAppInfo.apiVersion = XR_API_VERSION_1_0;
		ici.applicationInfo = XRAppInfo;
		result = xrCreateInstance(&ici, &instance);
	}
	if (result != XR_SUCCESS) {
		common->Error("Failed to initiate OpenXR\n");
		return;
	}

	XrInstanceProperties instanceProperties{ XR_TYPE_INSTANCE_PROPERTIES };
	if (xrGetInstanceProperties(instance, &instanceProperties) == XR_SUCCESS) {
		char* xrVersion = new char[20];
		sprintf(xrVersion, "%d.%d.%d", XR_VERSION_MAJOR(instanceProperties.runtimeVersion), XR_VERSION_MINOR(instanceProperties.runtimeVersion), XR_VERSION_PATCH(instanceProperties.runtimeVersion));
		common->Printf("OpenXR Have been initialized\n------------------------------------------------\nRuntime Name: %s\nRuntime Version: %s\n------------------------------------------------\n", instanceProperties.runtimeName, xrVersion);
	}
#ifdef _DEBUG
	if (std::find(activeExtensions.begin(), activeExtensions.end(), XR_EXT_DEBUG_UTILS_EXTENSION_NAME) != activeExtensions.end()) {
		XrDebugUtilsMessengerCreateInfoEXT dmci{ XR_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
		dmci.messageSeverities = XR_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | XR_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | XR_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
		dmci.messageTypes = XR_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | XR_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | XR_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | XR_DEBUG_UTILS_MESSAGE_TYPE_CONFORMANCE_BIT_EXT;
		dmci.userCallback = (PFN_xrDebugUtilsMessengerCallbackEXT)DebugXR;
		dmci.userData = nullptr;

		PFN_xrCreateDebugUtilsMessengerEXT xrCreateDebugUtils;
		if (xrGetInstanceProcAddr(instance, "xrCreateDebugUtilsMessengerEXT", (PFN_xrVoidFunction*)&xrCreateDebugUtils) == XR_SUCCESS) {
			if (xrCreateDebugUtils(instance, &dmci, &debugMessager) != XR_SUCCESS) {
				common->Error("OpenXR Error: Failed to create Debug Messenger");
			}
		}
	}
#endif

	XrSystemGetInfo systemGI{ XR_TYPE_SYSTEM_GET_INFO };
	systemGI.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
	if (xrGetSystem(instance, &systemGI, &systemId) != XR_SUCCESS) {
		common->Error("OpenXR Error: Failed to retrieve SystemId");
	}
	if (xrGetSystemProperties(instance, systemId, &systemProperties) != XR_SUCCESS) {
		common->Error("OpenXR Error: Failed to retrieve System properties");
	}
	common->Printf("OpenXR Compatible System Havebeen Found\n------------------------------------------------\nVendor: %d\nSystem: %s\n------------------------------------------------\n", systemProperties.vendorId, systemProperties.systemName);

	//Create a Session
	XrSessionCreateInfo sci{ XR_TYPE_SESSION_CREATE_INFO };
	//TODO: Interact and retrieve data from renderingSystem

}


void idXR::ShutDownXR()
{
	if (xrDestroyInstance(instance) != XR_SUCCESS) {
		common->Error("Failed to close OpenXR\n");
	}
}
