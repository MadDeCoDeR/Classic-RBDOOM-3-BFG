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
#include "win_oxr.h"

idXR_Win xrWinSystem;
idXR* xrSystem = &xrWinSystem;

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

void idXR_Win::PollXREvents()
{
	XrEventDataBuffer eventData{ XR_TYPE_EVENT_DATA_BUFFER };
	std::function xrPollEvents = [&]() -> bool {
		eventData = { XR_TYPE_EVENT_DATA_BUFFER };
		return xrPollEvent(instance, &eventData) == XR_SUCCESS;
		};

	while (xrPollEvents()) {
		switch (eventData.type) {
		case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED:
			XrEventDataSessionStateChanged* sessionStateChanged = reinterpret_cast<XrEventDataSessionStateChanged*>(&eventData);
			if (sessionStateChanged->session != session) {
				common->Warning("OpenXR: State Change Session is different from created Session");
				break;
			}
			if (sessionStateChanged->state == XR_SESSION_STATE_READY) {
				XrSessionBeginInfo sessionbi{ XR_TYPE_SESSION_BEGIN_INFO };
				sessionbi.primaryViewConfigurationType = viewConfiguration;
				if (xrBeginSession(session, &sessionbi) != XR_SUCCESS) {
					common->Warning("OpenXR: Failed to begin Session");
				}
			} else if (sessionStateChanged->state == XR_SESSION_STATE_STOPPING){
				if (xrEndSession(session) != XR_SUCCESS) {
					common->Warning("OpenXR: Failed to end Session");
				}
			}
			sessionState = sessionStateChanged->state;
			break;
		}
	}
}

void idXR_Win::StartFrame()
{
	bool activeSession = (sessionState == XR_SESSION_STATE_SYNCHRONIZED || sessionState == XR_SESSION_STATE_VISIBLE || sessionState == XR_SESSION_STATE_FOCUSED || sessionState == XR_SESSION_STATE_READY);
	if (!inFrame && activeSession) {
		XrFrameState frameState{ XR_TYPE_FRAME_STATE };
		XrFrameWaitInfo frameWaitInfo{ XR_TYPE_FRAME_WAIT_INFO };
		XrResult waitRes = xrWaitFrame(session, &frameWaitInfo, &frameState);
		if (waitRes != XR_SUCCESS && waitRes != XR_SESSION_LOSS_PENDING) {
			common->Warning("OpenXR Error: Failed to wait for Frame\n");
			return;
		}

		predictedDisplayTime = frameState.predictedDisplayTime;

		XrFrameBeginInfo frameBeginInfo{ XR_TYPE_FRAME_BEGIN_INFO };
		XrResult beginRes = xrBeginFrame(session, &frameBeginInfo);
		if (beginRes != XR_SUCCESS && beginRes != XR_SESSION_LOSS_PENDING && beginRes != XR_FRAME_DISCARDED) {
			common->Warning("OpenXR Error: Failed to begin Frame\n");
			return;
		}
		views.clear();
		views.assign(configurationView.size(), { XR_TYPE_VIEW });

		if (predictedDisplayTime > 0) {
			XrViewState viewState{ XR_TYPE_VIEW_STATE };
			XrViewLocateInfo viewLocateInfo{ XR_TYPE_VIEW_LOCATE_INFO };
			viewLocateInfo.viewConfigurationType = viewConfiguration;
			viewLocateInfo.displayTime = predictedDisplayTime;
			viewLocateInfo.space = localSpace;
			uint viewCount = 0;
			XrResult locateResult = xrLocateViews(session, &viewLocateInfo, &viewState, static_cast<uint>(views.size()), &viewCount, views.data());
			if (locateResult != XR_SUCCESS && locateResult != XR_SESSION_LOSS_PENDING) {
				common->Warning("OpenXR Error: Failed to Locate Views\n");
				return;
			}

			layers.resize(viewCount, { XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW });
		}
		inFrame = frameState.shouldRender && activeSession;
	}
}

void idXR_Win::BindSwapchainImage(int eye)
{
	if (inFrame) {
		renderingEye = eye;
		SwapchainInfo& renderingColorSwapchainInfo = colorSwapchainInfo[renderingEye];
		//SwapchainInfo& renderingDepthSwapchainInfo = depthSwapchainInfo[renderingEye];

		uint colorIndex = 0;
		//uint depthIndex = 0;
		XrSwapchainImageAcquireInfo acquireInfo{ XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO };
		if (xrAcquireSwapchainImage(renderingColorSwapchainInfo.swapchain, &acquireInfo, &colorIndex) != XR_SUCCESS) {
			common->Warning("OpenXR Error: Failed to acquire Image for Color Swapchain\n");
			return;
		}
		/*if (xrAcquireSwapchainImage(renderingDepthSwapchainInfo.swapchain, &acquireInfo, &depthIndex) != XR_SUCCESS) {
			common->Warning("OpenXR Error: Failed to acquire Image for Depth Swapchain\n");
			return;
		}*/

		XrSwapchainImageWaitInfo waitInfo = { XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO };
		waitInfo.timeout = XR_INFINITE_DURATION;
		if (xrWaitSwapchainImage(renderingColorSwapchainInfo.swapchain, &waitInfo) != XR_SUCCESS) {
			common->Warning("OpenXR Error: Failed to wait for Image for Color Swapchain\n");
			return;
		}
		/*if (xrWaitSwapchainImage(renderingDepthSwapchainInfo.swapchain, &waitInfo) != XR_SUCCESS) {
			common->Warning("OpenXR Error: Failed to wait for Image for Depth Swapchain\n");
			return;
		}*/

		layers[renderingEye] = { XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW };
		layers[renderingEye].fov = { -0.750491619f, 0.785398185f, 0.837758064f, -0.872664630f }; //views[renderingEye].fov;
		layers[renderingEye].pose = views[renderingEye].pose;
		layers[renderingEye].subImage.swapchain = renderingColorSwapchainInfo.swapchain;
		layers[renderingEye].subImage.imageRect.offset.x = 0;
		layers[renderingEye].subImage.imageRect.offset.y = 0;
		layers[renderingEye].subImage.imageRect.extent.width = width;
		layers[renderingEye].subImage.imageRect.extent.height = height;
		layers[renderingEye].subImage.imageArrayIndex = 0;

		glFBO = (GLuint)(uint64_t)colorSwapchainInfo[renderingEye].imageViews[colorIndex];

		

		//glClearColor(0, 0, 0, 1);
	}

}

void idXR_Win::ReleaseSwapchainImage()
{
	if (inFrame) {
		//glDeleteFramebuffers(1, &glFBO);
		glFBO = MAX_UNSIGNED_TYPE(int);
		XrSwapchainImageReleaseInfo releaseInfo{ XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO };
		if (xrReleaseSwapchainImage(colorSwapchainInfo[renderingEye].swapchain, &releaseInfo) != XR_SUCCESS) {
			common->Warning("OpenXR Error: Failed to release Color swapchain Image");
			return;
		}
		
		/*if (xrReleaseSwapchainImage(depthSwapchainInfo[renderingEye].swapchain, &releaseInfo) != XR_SUCCESS) {
			common->Warning("OpenXR Error: Failed to release Depth swapchain Image");
			return;
		}*/
	}
}

void idXR_Win::RenderFrame(int srcX, int srcY, int srcW, int srcH)
{
	if (inFrame) {
		if (glConfig.directStateAccess) {
			glBlitNamedFramebuffer(0, glFBO, srcX, srcY, srcW, srcH, 0, 0, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
		}
		else {
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, glFBO);
			glBlitFramebuffer(srcX, srcY, srcW, srcH, 0, 0, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
		}
	}
}

void idXR_Win::EndFrame()
{
	if (inFrame) {
		XrFrameEndInfo frameEndInfo{ XR_TYPE_FRAME_END_INFO };
		frameEndInfo.displayTime = predictedDisplayTime;
		frameEndInfo.environmentBlendMode = environmentBlendMode;

		XrCompositionLayerProjection projection = { XR_TYPE_COMPOSITION_LAYER_PROJECTION };
		projection.layerFlags = XR_COMPOSITION_LAYER_BLEND_TEXTURE_SOURCE_ALPHA_BIT | XR_COMPOSITION_LAYER_CORRECT_CHROMATIC_ABERRATION_BIT;
		projection.space = localSpace;
		projection.viewCount = layers.size();
		projection.views = layers.data();
		std::vector<XrCompositionLayerBaseHeader*> renderLayers;
		renderLayers.push_back(reinterpret_cast<XrCompositionLayerBaseHeader*>(&projection));


		frameEndInfo.layerCount = renderLayers.size();
		frameEndInfo.layers = renderLayers.data();
		XrResult endFrameRes = xrEndFrame(session, &frameEndInfo);
		if (endFrameRes != XR_SUCCESS && endFrameRes != XR_SESSION_LOSS_PENDING) {
			common->Warning("OpenXR Error: Failed to End Frame\n");
			return;
		}
		inFrame = false;
	}
	
}

uint idXR_Win::EnumerateSwapchainImage(std::vector<SwapchainInfo> swapchainInfo, idXRSwapchainType type, int index)
{
	uint imageCount = 0;
	XrSwapchain targetSwapchain = swapchainInfo[index].swapchain;
	if (xrEnumerateSwapchainImages(targetSwapchain, 0, &imageCount, nullptr) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to Initiate the enumeration for Swapchain Images");
	}
	swapchainImageMap[targetSwapchain].first = type;
	swapchainImageMap[targetSwapchain].second.resize(imageCount, { XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_KHR });
	if (xrEnumerateSwapchainImages(targetSwapchain, imageCount, &imageCount, reinterpret_cast<XrSwapchainImageBaseHeader*>(swapchainImageMap[targetSwapchain].second.data())) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to enumeration for Swapchain Images");
	}
	return imageCount;
}

void* idXR_Win::CreateFrameBuffer(const FrameBufferCreateInfo& FbCI)
{
	GLuint fb = 0;
	if (glConfig.directStateAccess) {
		glCreateFramebuffers(1, &fb);
		GLenum attachment = FbCI.aspect == FrameBufferCreateInfo::Aspect::COLOR_BIT ? GL_COLOR_ATTACHMENT0 : GL_DEPTH_ATTACHMENT;
		if (FbCI.view == FrameBufferCreateInfo::View::TYPE_2D_ARRAY) {
			glNamedFramebufferTextureMultiviewOVR(fb, attachment, (GLuint)(uint64_t)FbCI.image, FbCI.baseMipLevel, FbCI.baseArrayLayer, FbCI.layerCount);
		}
		else if (FbCI.view == FrameBufferCreateInfo::View::TYPE_2D) {
			glNamedFramebufferTexture(fb, attachment, (GLuint)(uint64_t)FbCI.image, FbCI.baseMipLevel);
		}
		else {
			common->Warning("OpenXR Error: Framebuffer Create Info had invalid View type");
			return nullptr;
		}
		int status = glCheckNamedFramebufferStatus(fb, GL_DRAW_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			common->Warning("OpenXR Error: Failed to Create Framebuffer");
			return nullptr;
		}
		
	}
	else {
		glGenFramebuffers(1, &fb);
		GLenum attachment = FbCI.aspect == FrameBufferCreateInfo::Aspect::COLOR_BIT ? GL_COLOR_ATTACHMENT0 : GL_DEPTH_ATTACHMENT;
		glBindFramebuffer(GL_FRAMEBUFFER, fb);
		if (FbCI.view == FrameBufferCreateInfo::View::TYPE_2D_ARRAY) {
			glFramebufferTextureMultiviewOVR(GL_DRAW_FRAMEBUFFER, attachment, (GLuint)(uint64_t)FbCI.image, FbCI.baseMipLevel, FbCI.baseArrayLayer, FbCI.layerCount);
		}
		else if (FbCI.view == FrameBufferCreateInfo::View::TYPE_2D) {
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment, GL_TEXTURE_2D, (GLuint)(uint64_t)FbCI.image, FbCI.baseMipLevel);
		}
		else {
			common->Warning("OpenXR Error: Framebuffer Create Info had invalid View type");
			return nullptr;
		}
		int status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			common->Warning("OpenXR Error: Failed to Create Framebuffer");
			return nullptr;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	return (void*)(uint64_t)fb;
}



void idXR_Win::InitXR() {
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
					common->Warning("Failed to find OpenXR Extension: %s\n", requestExtension.c_str());
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
		common->Warning("Failed to initiate OpenXR\n");
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
				common->Warning("OpenXR Error: Failed to create Debug Messenger");
			}
		}
	}
#endif

	XrSystemGetInfo systemGI{ XR_TYPE_SYSTEM_GET_INFO };
	systemGI.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
	if (xrGetSystem(instance, &systemGI, &systemId) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to retrieve SystemId");
	}
	if (xrGetSystemProperties(instance, systemId, &systemProperties) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to retrieve System properties");
	}
	common->Printf("OpenXR Compatible System Havebeen Found\n------------------------------------------------\nVendor: %d\nSystem: %s\n------------------------------------------------\n", systemProperties.vendorId, systemProperties.systemName);

	//Required before Creating a Session
	XrGraphicsRequirementsOpenGLKHR requirements{ XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_KHR };
	PFN_xrGetOpenGLGraphicsRequirementsKHR xrGetOpenGLGraphicsRequirementsKHR;
	if (xrGetInstanceProcAddr(instance, "xrGetOpenGLGraphicsRequirementsKHR", (PFN_xrVoidFunction*)&xrGetOpenGLGraphicsRequirementsKHR) == XR_SUCCESS) {
		if (xrGetOpenGLGraphicsRequirementsKHR(instance, systemId, &requirements) != XR_SUCCESS) {
			common->Warning("OpenXR Error: Failed to Retrieve Graphic Requirements");
		}
	}

	uint viewConfigurationCount = 0;
	if (xrEnumerateViewConfigurations(instance, systemId, 0, &viewConfigurationCount, nullptr) == XR_SUCCESS) {
		viewConfigurations.resize(viewConfigurationCount);
		if (xrEnumerateViewConfigurations(instance, systemId, viewConfigurationCount, &viewConfigurationCount, viewConfigurations.data()) != XR_SUCCESS) {
			common->Warning("OpenXR Error: Failed to Enumerate View Configurations");
		}
	}

	for (const XrViewConfigurationType viewConf : appViewConfigurations) {
		if (std::find(viewConfigurations.begin(), viewConfigurations.end(), viewConf) != viewConfigurations.end()) {
			viewConfiguration = viewConf;
			break;
		}
	}

	if (viewConfiguration == XR_VIEW_CONFIGURATION_TYPE_MAX_ENUM) {
		viewConfiguration = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
	}

	if (xrEnumerateViewConfigurationViews(instance, systemId, viewConfiguration, 0, &viewConfigurationCount, nullptr) == XR_SUCCESS) {
		configurationView.resize(viewConfigurationCount, {XR_TYPE_VIEW_CONFIGURATION_VIEW});
		if (xrEnumerateViewConfigurationViews(instance, systemId, viewConfiguration, viewConfigurationCount, &viewConfigurationCount, configurationView.data()) != XR_SUCCESS) {
			common->Warning("OpenXR Error: Failed to Enumerate View Configuration Views");
		}
	}

	//Create a Session
	XrSessionCreateInfo sci{ XR_TYPE_SESSION_CREATE_INFO };
	sci.next = GetOpenXRGraphicsBinding();
	sci.createFlags = 0;
	sci.systemId = systemId;
	if (xrCreateSession(instance, &sci, &session) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to create Session");
	}

	XrReferenceSpaceCreateInfo referenceSpaceCI{ XR_TYPE_REFERENCE_SPACE_CREATE_INFO };
	referenceSpaceCI.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_LOCAL;
	referenceSpaceCI.poseInReferenceSpace = { {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };
	if (xrCreateReferenceSpace(session, &referenceSpaceCI, &localSpace) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to create Reference Space");
	}

	uint formatCount = 0;
	if (xrEnumerateSwapchainFormats(session, 0, &formatCount, nullptr) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to Initiate the enumeration for Swapchain Formats");
	}
	std::vector<int64> formats(formatCount);
	if (xrEnumerateSwapchainFormats(session, formatCount, &formatCount, formats.data()) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to Enumerate Swapchain Formats");
	}

	std::vector<int64> supportedColorFormats = {
		GL_RGB10_A2,
		GL_RGBA16F,
		GL_RGBA8,
		GL_RGBA8_SNORM
	};
	int64 colorFormat = 0;
	std::vector<int64>::const_iterator colorFormatIterator = std::find_first_of(formats.begin(), formats.end(), supportedColorFormats.begin(), supportedColorFormats.end());

	if (colorFormatIterator == formats.end()) {
		common->Warning("OpenXR Error: Failed to find Color Format");
	}
	else {
		colorFormat = *colorFormatIterator;
	}

	std::vector<int64> supportedDepthFormats = {
		GL_DEPTH_COMPONENT32F,
		GL_DEPTH_COMPONENT32,
		GL_DEPTH_COMPONENT24,
		GL_DEPTH_COMPONENT16
	};
	int64 depthFormat = 0;
	std::vector<int64>::const_iterator depthFormatIterator = std::find_first_of(formats.begin(), formats.end(), supportedDepthFormats.begin(), supportedDepthFormats.end());
	if (depthFormatIterator == formats.end()) {
		common->Warning("OpenXR Error: Failed to find Depth Format");
	}
	else {
		depthFormat = *depthFormatIterator;
	}

	colorSwapchainInfo.resize(configurationView.size());
	depthSwapchainInfo.resize(configurationView.size());

	width = 2244;//configurationView[0].recommendedImageRectWidth; //2244
	height = 2352;//configurationView[0].recommendedImageRectHeight; //2352

	for (int i = 0; i < configurationView.size(); i++) {
		XrSwapchainCreateInfo swci{ XR_TYPE_SWAPCHAIN_CREATE_INFO };
		swci.createFlags = 0;
		swci.usageFlags = XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT;
		swci.format = colorFormat;
		swci.sampleCount = configurationView[i].recommendedSwapchainSampleCount;
		swci.width = width;
		swci.height = height;
		swci.faceCount = 1;
		swci.arraySize = 1;
		swci.mipCount = 1;
		if (xrCreateSwapchain(session, &swci, &colorSwapchainInfo[i].swapchain) != XR_SUCCESS) {
			common->Warning("OpenXR Error: Failed to Create Color Swapchain");
		}

		swci.createFlags = 0;
		swci.usageFlags = XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		swci.format = depthFormat;
		swci.sampleCount = configurationView[i].recommendedSwapchainSampleCount;
		swci.width = width;
		swci.height = height;
		swci.faceCount = 1;
		swci.arraySize = 1;
		swci.mipCount = 1;
		if (xrCreateSwapchain(session, &swci, &depthSwapchainInfo[i].swapchain) != XR_SUCCESS) {
			common->Warning("OpenXR Error: Failed to Create Depth Swapchain");
		}

		uint colorSwapchainImageCount = this->EnumerateSwapchainImage(colorSwapchainInfo, idXRSwapchainType::COLOR, i);

		uint depthSwapchainImageCount = this->EnumerateSwapchainImage(depthSwapchainInfo, idXRSwapchainType::DEPTH, i);

		for (uint ci = 0; ci < colorSwapchainImageCount; ci++) {
			FrameBufferCreateInfo fbCI;
			fbCI.image = (void*)(uint64_t)swapchainImageMap[colorSwapchainInfo[i].swapchain].second[ci].image;
			fbCI.type = FrameBufferCreateInfo::Type::RTV;
			fbCI.view = FrameBufferCreateInfo::View::TYPE_2D;
			fbCI.format = colorSwapchainInfo[i].swapchainFormat;
			fbCI.aspect = FrameBufferCreateInfo::Aspect::COLOR_BIT;
			fbCI.baseMipLevel = 0;
			fbCI.levelCount = 1;
			fbCI.baseArrayLayer = 0;
			fbCI.layerCount = 1;
			colorSwapchainInfo[i].imageViews.push_back(CreateFrameBuffer(fbCI));
		}

		for (uint di = 0; di < depthSwapchainImageCount; di++) {
			FrameBufferCreateInfo fbCI;
			fbCI.image = (void*)(uint64_t)swapchainImageMap[depthSwapchainInfo[i].swapchain].second[di].image;
			fbCI.type = FrameBufferCreateInfo::Type::DSV;
			fbCI.view = FrameBufferCreateInfo::View::TYPE_2D;
			fbCI.format = depthSwapchainInfo[i].swapchainFormat;
			fbCI.aspect = FrameBufferCreateInfo::Aspect::DEPTH_BIT;
			fbCI.baseMipLevel = 0;
			fbCI.levelCount = 1;
			fbCI.baseArrayLayer = 0;
			fbCI.layerCount = 1;
			depthSwapchainInfo[i].imageViews.push_back(CreateFrameBuffer(fbCI));
		}
		
		//XrSwapchain targetSwapchain = colorSwapchainInfo[i].swapchain;
	}

	uint ebmc = 0;
	if (xrEnumerateEnvironmentBlendModes(instance, systemId, viewConfiguration, 0, &ebmc, nullptr) == XR_SUCCESS) {
		std::vector<XrEnvironmentBlendMode> environmentBlendModes;
		environmentBlendModes.resize(ebmc);
		if (xrEnumerateEnvironmentBlendModes(instance, systemId, viewConfiguration, ebmc, &ebmc, environmentBlendModes.data()) == XR_SUCCESS) {
			if (std::find(environmentBlendModes.begin(), environmentBlendModes.end(), XR_ENVIRONMENT_BLEND_MODE_OPAQUE) != environmentBlendModes.end()) {
				environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
			}
		}
	}
}


void idXR_Win::ShutDownXR()
{
	if (colorSwapchainInfo.size() > 0) {
		for (int i = 0; i < colorSwapchainInfo.size(); i++) {
			for (int j = 0; j < colorSwapchainInfo[i].imageViews.size(); j++) {
				GLuint fbo = (GLuint)(uint64_t)colorSwapchainInfo[i].imageViews[j];
				glDeleteFramebuffers(1, &fbo);
			}
			colorSwapchainInfo[i].imageViews.clear();
			swapchainImageMap[colorSwapchainInfo[i].swapchain].second.clear();
			if (xrDestroySwapchain(colorSwapchainInfo[i].swapchain) != XR_SUCCESS) {
				common->Warning("OpenXR Error: Failed to delete Color Swapchain Image\n");
			}
		}
	}
	if (depthSwapchainInfo.size() > 0) {
		for (int i = 0; i < depthSwapchainInfo.size(); i++) {
			for (int j = 0; j < depthSwapchainInfo[i].imageViews.size(); j++) {
				GLuint fbo = (GLuint)(uint64_t)depthSwapchainInfo[i].imageViews[j];
				glDeleteFramebuffers(1, &fbo);
			}
			depthSwapchainInfo[i].imageViews.clear();
			swapchainImageMap[depthSwapchainInfo[i].swapchain].second.clear();
			if (xrDestroySwapchain(depthSwapchainInfo[i].swapchain) != XR_SUCCESS) {
				common->Warning("OpenXR Error: Failed to delete Depth Swapchain Image\n");
			}
		}
	}
	swapchainImageMap.clear();

	if (localSpace != XR_NULL_HANDLE && xrDestroySpace(localSpace) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to close OpenXR Space\n");
	}
	if (session != XR_NULL_HANDLE && xrDestroySession(session) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to close OpenXR Session\n");
	}
#ifdef _DEBUG
	PFN_xrDestroyDebugUtilsMessengerEXT xrDestroyDebugMessager;
	if (xrGetInstanceProcAddr(instance, "xrDestroyDebugUtilsMessengerEXT", (PFN_xrVoidFunction*)&xrDestroyDebugMessager) == XR_SUCCESS) {
		if (debugMessager != XR_NULL_HANDLE && xrDestroyDebugMessager(debugMessager) != XR_SUCCESS) {
			common->Warning("OpenXR Error: Failed to Destroy Debug Messenger");
		}
	}
#endif
	if (instance != XR_NULL_HANDLE && xrDestroyInstance(instance) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to close OpenXR\n");
	}
}
