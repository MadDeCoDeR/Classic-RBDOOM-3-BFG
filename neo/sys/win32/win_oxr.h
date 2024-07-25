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
#pragma once

#include "renderer/OpenXR/XRCommon.h"
#include <string.h>
#include <framework/Licensee.h>
#include <stdlib.h>
#include "gfxwrapper_opengl.h"
#include "renderer/RenderCommon.h"
#include <functional>

class idXR_Win :public idXR {
public:
	virtual void InitXR();
	virtual void ShutDownXR();
	virtual void PollXREvents();
	virtual void StartRendering(int eye);
	virtual void RenderFrame();
	virtual void EndRendering();
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
	XrSession session = XR_NULL_HANDLE;
	std::vector<XrViewConfigurationType> appViewConfigurations = { XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO, XR_VIEW_CONFIGURATION_TYPE_PRIMARY_MONO };
	std::vector<XrViewConfigurationType> viewConfigurations;
	XrViewConfigurationType viewConfiguration = XR_VIEW_CONFIGURATION_TYPE_MAX_ENUM;
	std::vector<XrViewConfigurationView> configurationView;
	XrSpace localSpace = XR_NULL_HANDLE;

	struct SwapchainInfo {
		XrSwapchain swapchain = XR_NULL_HANDLE;
		int64 swapchainFormat = 0;
		std::vector<void*> imageViews;
	};
	virtual void EnumerateSwapchainImage(std::vector<SwapchainInfo> swapchainInfo, idXRSwapchainType type, int index);
	std::vector<SwapchainInfo> colorSwapchainInfo = {};
	std::vector<SwapchainInfo> depthSwapchainInfo = {};

	std::unordered_map<XrSwapchain, std::pair<idXRSwapchainType, std::vector<XrSwapchainImageOpenGLKHR>>> swapchainImageMap{};
	int renderingEye = -1;
	bool inFrame = false;
	std::vector<XrView> views;
	XrTime predictedDisplayTime;
	SwapchainInfo &renderingColorSwapchainInfo;
	SwapchainInfo &renderingDepthSwapchainInfo;
};
