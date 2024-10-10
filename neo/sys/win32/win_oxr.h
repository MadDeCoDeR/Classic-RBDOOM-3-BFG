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
	idXR_Win() {}
	virtual bool InitXR();
	virtual void ShutDownXR();
	virtual void PollXREvents();
	virtual void StartFrame();
	virtual void BindSwapchainImage(int eye);
	virtual void ReleaseSwapchainImage();
	virtual void RenderFrame(int srcX, int srcY, int srcW, int srcH);
	virtual void EndFrame();
	virtual uint32_t GetWidth() {
		return width;
	}
	virtual uint32_t GetHeight() {
		return height;
	}
	virtual bool IsInitialized() {
		return isInitialized;
	}
	virtual bool isFOVmutable() {
		if (isInitialized) {
			return viewProperties.fovMutable;
		}
		return false;
	}
	virtual void SetActionSet(idStr name);
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
	std::vector<XrViewConfigurationType> appViewConfigurations = { XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO };
	std::vector<XrViewConfigurationType> viewConfigurations;
	XrViewConfigurationType viewConfiguration = XR_VIEW_CONFIGURATION_TYPE_MAX_ENUM;
	std::vector<XrViewConfigurationView> configurationView;
	XrSpace localSpace = XR_NULL_HANDLE;
	XrSessionState sessionState = XR_SESSION_STATE_UNKNOWN;

	struct SwapchainInfo {
		XrSwapchain swapchain = XR_NULL_HANDLE;
		int64 swapchainFormat = 0;
		std::vector<void*> imageViews;
	};
	virtual uint EnumerateSwapchainImage(std::vector<SwapchainInfo> swapchainInfo, idXRSwapchainType type, int index);
	struct FrameBufferCreateInfo {
		void* image;
		enum class Type : uint8_t {
			RTV,
			DSV,
			SRV,
			UAV
		} type;
		enum class View : uint8_t {
			TYPE_1D,
			TYPE_2D,
			TYPE_3D,
			TYPE_CUBE,
			TYPE_1D_ARRAY,
			TYPE_2D_ARRAY,
			TYPE_CUBE_ARRAY
		} view;
		int64_t format;
		enum class Aspect : uint8_t {
			COLOR_BIT = 0x01,
			DEPTH_BIT = 0x02,
			STENCIL_BIT = 0x04
		} aspect;
		uint32_t baseMipLevel;
		uint32_t levelCount;
		uint32_t baseArrayLayer;
		uint32_t layerCount;
	};
	virtual void* CreateFrameBuffer(const FrameBufferCreateInfo &FbCI);
	std::vector<SwapchainInfo> colorSwapchainInfo = {};
	std::vector<SwapchainInfo> depthSwapchainInfo = {};

	std::unordered_map<XrSwapchain, std::pair<idXRSwapchainType, std::vector<XrSwapchainImageOpenGLKHR>>> swapchainImageMap{};
	int renderingEye = -1;
	std::vector<XrView> views;
	XrTime predictedDisplayTime;
	XrEnvironmentBlendMode environmentBlendMode;
	std::vector<XrCompositionLayerProjectionView> layers;
	GLuint glFBO = MAX_UNSIGNED_TYPE(int);
	bool inFrame = false;
	uint32_t width = 0;
	uint32_t height = 0;
	bool isInitialized = false;
	XrViewConfigurationProperties viewProperties;
	struct idXrAction {
		idStr name;
		XrAction action;
		XrActionType type;
		uint32_t mappedKey = K_NONE;
		XrActionStatePose poseState[2] = { {XR_TYPE_ACTION_STATE_POSE}, {XR_TYPE_ACTION_STATE_POSE} };
		XrActionStateBoolean booleanState[2] = { {XR_TYPE_ACTION_STATE_BOOLEAN}, {XR_TYPE_ACTION_STATE_BOOLEAN} };
		XrActionStateFloat floatState[2] = { {XR_TYPE_ACTION_STATE_FLOAT}, {XR_TYPE_ACTION_STATE_FLOAT} };
		XrActionStateVector2f vector2fState[2] = { {XR_TYPE_ACTION_STATE_VECTOR2F}, {XR_TYPE_ACTION_STATE_VECTOR2F} };
	};
	struct idXrActionSet {
		idStr name;
		XrActionSet actionSet;
		std::vector<idXrAction> actions;
	};
	std::vector<idXrActionSet> actionSets;
	std::vector<XrSpace> handPoseSpace;
	std::vector<XrPath> handPaths;
	XrPosef handPose[2] = { {XR_TYPE_SPACE_LOCATION}, {XR_TYPE_SPACE_LOCATION} };
	idStr expectedActionSet;

	XrPath StringToXRPath(const char* strPath);
	idStr XRPathToString(XrPath xrPath);
	void CreateXrMappings();
	idXrAction CreateAction(XrActionSet actionSet, const char* name, XrActionType type, uint32_t mappedKey, std::vector<const char*> subActions = {});
	void SuggestBindings(const char* profilePath, std::vector<XrActionSuggestedBinding> bindings);
	XrSpace CreateActionPoseSpace(XrAction action, const char* subPath = nullptr);
	void FinalizeActions();
	void PollActions();
	void RetrieveActionState(idXrAction action);
	idXrAction GetActionByName(idStr setName, idStr name);
	idXrActionSet GetActionSetByName(idStr setName);
	void MapActionStateToUsrCmd(idXrAction action);
};
