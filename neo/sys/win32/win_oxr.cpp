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
	if (isInitialized) {
		XrEventDataBuffer eventData{ XR_TYPE_EVENT_DATA_BUFFER };
		std::function xrPollEvents = [&]() -> bool {
			eventData = { XR_TYPE_EVENT_DATA_BUFFER };
			return xrPollEvent(instance, &eventData) == XR_SUCCESS;
			};

		while (xrPollEvents()) {
			switch (eventData.type) {
			case XR_TYPE_EVENT_DATA_INTERACTION_PROFILE_CHANGED: {
				XrEventDataInteractionProfileChanged* interactionProfileChanged = reinterpret_cast<XrEventDataInteractionProfileChanged*>(&eventData);
				if (interactionProfileChanged->session != session) {
					common->Warning("OpenXR: Interaction Profile Change Session is different from created Session");
					break;
				}
				XrInteractionProfileState profileState = { XR_TYPE_INTERACTION_PROFILE_STATE, 0, 0 };
				for (int i = 0; i < 2; i++) {
					XrResult res = xrGetCurrentInteractionProfile(session, handPaths[i], &profileState);
					if (res != XR_SUCCESS || profileState.interactionProfile == 0) {
						common->Warning("OpenXR Error: Failed to retrieve Interaction Profile");
					}
				}
				break;
			}
			case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED:
				XrEventDataSessionStateChanged* sessionStateChanged = reinterpret_cast<XrEventDataSessionStateChanged*>(&eventData);
				if (sessionStateChanged->session != session) {
					common->Warning("OpenXR: State Change Session is different from created Session");
					break;
				}
				switch (sessionStateChanged->state) {
				case XR_SESSION_STATE_READY: {
						XrSessionBeginInfo sessionbi{ XR_TYPE_SESSION_BEGIN_INFO };
						sessionbi.primaryViewConfigurationType = viewConfiguration;
						if (xrBeginSession(session, &sessionbi) != XR_SUCCESS) {
							common->Warning("OpenXR: Failed to begin Session");
						}
						break;
					}
				case XR_SESSION_STATE_STOPPING: {
						if (xrEndSession(session) != XR_SUCCESS) {
							common->Warning("OpenXR: Failed to end Session");
						}
						break;
					}
				}
				sessionState = sessionStateChanged->state;
				break;
			}
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
			common->Warning("OpenXR Error: Failed to wait for Frame");
			return;
		}

		predictedDisplayTime = frameState.predictedDisplayTime;

		XrFrameBeginInfo frameBeginInfo{ XR_TYPE_FRAME_BEGIN_INFO };
		XrResult beginRes = xrBeginFrame(session, &frameBeginInfo);
		if (beginRes != XR_SUCCESS && beginRes != XR_SESSION_LOSS_PENDING && beginRes != XR_FRAME_DISCARDED) {
			common->Warning("OpenXR Error: Failed to begin Frame");
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
				common->Warning("OpenXR Error: Failed to Locate Views");
				return;
			}
			//HMD input
			if (!expectedActionSet.Cmp("GAME")) {
				usercmdGen->SetAngles(idQuat(views[0].pose.orientation.x, views[0].pose.orientation.y, views[0].pose.orientation.z, views[0].pose.orientation.w).ToAngles());
			}
			layers.resize(viewCount, { XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW });
			PollActions();
		}
		inFrame = frameState.shouldRender && activeSession;
		if (inFrame) {
			glBindFramebuffer(GL_FRAMEBUFFER, GeneralFB);
		}
	}
}

void idXR_Win::BindSwapchainImage(int eye)
{
	if (inFrame) {
		renderingEye = eye;
		SwapchainInfo& renderingColorSwapchainInfo = colorSwapchainInfo[renderingEye];

		uint colorIndex = 0;
		//uint depthIndex = 0;
		XrSwapchainImageAcquireInfo acquireInfo{ XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO };
		if (xrAcquireSwapchainImage(renderingColorSwapchainInfo.swapchain, &acquireInfo, &colorIndex) != XR_SUCCESS) {
			common->Warning("OpenXR Error: Failed to acquire Image for Color Swapchain");
			return;
		}

		XrSwapchainImageWaitInfo waitInfo = { XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO };
		waitInfo.timeout = XR_INFINITE_DURATION;
		if (xrWaitSwapchainImage(renderingColorSwapchainInfo.swapchain, &waitInfo) != XR_SUCCESS) {
			common->Warning("OpenXR Error: Failed to wait for Image for Color Swapchain");
			return;
		}
		//GK: Image Alignment Calculation: Using the left and right FOV angles from an FOV that aligns the images it calculates an offset that will apply to the frame in order to keep the images aligned. 
		// It's not pretty but it works with dynamic resolutions and dynamic lens distances
		float FOVFixConst = -0.750491619f;
		float FOVFixConst2 = 0.785398185f;
		uint32_t xOffsL = abs((views[renderingEye].fov.angleLeft - FOVFixConst) * 1000);
		xOffsL = xOffsL * (renderingEye == 0 ? 1 : -1);

		uint32_t xOffsR = abs((views[renderingEye].fov.angleRight - FOVFixConst2) * 1000);
		xOffsR = xOffsR * (renderingEye == 0 ? -1 : 1);
		xOffs = xOffsL - xOffsR;

		layers[renderingEye] = { XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW };
		layers[renderingEye].fov = views[renderingEye].fov;
		layers[renderingEye].pose = views[renderingEye].pose;
		layers[renderingEye].subImage.swapchain = renderingColorSwapchainInfo.swapchain;
		layers[renderingEye].subImage.imageRect.offset.x = 0;
		layers[renderingEye].subImage.imageRect.offset.y = 0;
		layers[renderingEye].subImage.imageRect.extent.width = width;
		layers[renderingEye].subImage.imageRect.extent.height = height;
		layers[renderingEye].subImage.imageArrayIndex = 0;

		glFBO = (GLuint)(uint64_t)colorSwapchainInfo[renderingEye].imageViews[colorIndex];
		}

}

void idXR_Win::ReleaseSwapchainImage()
{
	if (inFrame) {
		glFBO = MAX_UNSIGNED_TYPE(int);
		XrSwapchainImageReleaseInfo releaseInfo{ XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO };
		if (xrReleaseSwapchainImage(colorSwapchainInfo[renderingEye].swapchain, &releaseInfo) != XR_SUCCESS) {
			common->Warning("OpenXR Error: Failed to release Color swapchain Image");
			return;
		}
	}
}

void idXR_Win::RenderFrame(int srcX, int srcY, int srcW, int srcH)
{
	if (inFrame) {
		if (glConfig.directStateAccess) {
			glBlitNamedFramebuffer(GeneralFB, glFBO, srcX, srcY, srcW, srcH, xOffs, 0, width + xOffs, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
		}
		else {
			glBindFramebuffer(GL_READ_FRAMEBUFFER, GeneralFB);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, glFBO);
			glBlitFramebuffer(srcX, srcY, srcW, srcH, xOffs, 0, width + xOffs, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
		}
	}
}

void idXR_Win::EndFrame()
{
	if (inFrame) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		XrFrameEndInfo frameEndInfo{ XR_TYPE_FRAME_END_INFO };
		frameEndInfo.displayTime = predictedDisplayTime;
		frameEndInfo.environmentBlendMode = environmentBlendMode;
		std::vector<XrCompositionLayerBaseHeader*> renderLayers;
		XrCompositionLayerProjection projection = { XR_TYPE_COMPOSITION_LAYER_PROJECTION };
		projection.layerFlags = XR_COMPOSITION_LAYER_BLEND_TEXTURE_SOURCE_ALPHA_BIT | XR_COMPOSITION_LAYER_CORRECT_CHROMATIC_ABERRATION_BIT;
		projection.space = localSpace;
		projection.viewCount = layers.size();
		projection.views = layers.data();
			
		renderLayers.push_back(reinterpret_cast<XrCompositionLayerBaseHeader*>(&projection));


		frameEndInfo.layerCount = renderLayers.size();
		frameEndInfo.layers = renderLayers.data();
		XrResult endFrameRes = xrEndFrame(session, &frameEndInfo);
		if (endFrameRes != XR_SUCCESS && endFrameRes != XR_SESSION_LOSS_PENDING) {
			common->Warning("OpenXR Error: Failed to End Frame");
			return;
		}
		inFrame = false;
	}
	
}

void idXR_Win::SetActionSet(idStr name)
{
	for (idXrActionSet actionSet : actionSets) {
		if (!actionSet.name.Cmp(name.c_str())) {
			expectedActionSet = actionSet.name;
		}
	}
}

XrPath idXR_Win::StringToXRPath(const char* strPath)
{
	XrPath xrPath;
	if (xrStringToPath(instance, strPath, &xrPath) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to Convert String to XrPath");
	}
	return xrPath;
}

idStr idXR_Win::XRPathToString(XrPath xrPath)
{
	uint32_t length;
	char buffer[XR_MAX_PATH_LENGTH];
	XrResult res = xrPathToString(instance, xrPath, XR_MAX_PATH_LENGTH, &length, buffer);
	if (res == XR_SUCCESS) {
		return idStr(buffer);
	}
	return idStr();
}

void idXR_Win::CreateXrMappings()
{
	handPaths.push_back(StringToXRPath("/user/hand/left"));
	handPaths.push_back(StringToXRPath("/user/hand/right"));
	//Menu Action Set
	idXrActionSet menuActionSet;
	menuActionSet.name = "MENU";
	XrActionSetCreateInfo xrActCI{ XR_TYPE_ACTION_SET_CREATE_INFO };
	strncpy(xrActCI.actionSetName, "doom-bfa-menu-action-set", 26);
	strncpy(xrActCI.localizedActionSetName, "DOOM BFA Menu Action Set", 26);
	xrActCI.priority = 0;
	if (xrCreateActionSet(instance, &xrActCI, &menuActionSet.actionSet) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to Create Menu Action Set");
	}
	menuActionSet.actions = {};
	idXrAction menuPointer = CreateAction(menuActionSet.actionSet, "menu-pointer", XR_ACTION_TYPE_POSE_INPUT, K_NONE, { "/user/hand/left", "/user/hand/right" });
	idXrAction menuSelect = CreateAction(menuActionSet.actionSet, "menu-select", XR_ACTION_TYPE_BOOLEAN_INPUT, K_JOY1, { "/user/hand/left", "/user/hand/right" });
	idXrAction menuBack = CreateAction(menuActionSet.actionSet, "menu-back", XR_ACTION_TYPE_BOOLEAN_INPUT, K_JOY2, { "/user/hand/left", "/user/hand/right" });
	idXrAction menuScroll = CreateAction(menuActionSet.actionSet, "menu-scroll", XR_ACTION_TYPE_FLOAT_INPUT, J_AXIS_LEFT_Y, { "/user/hand/left", "/user/hand/right" });
	SuggestBindings("/interaction_profiles/oculus/touch_controller", { 
		{menuPointer.action, StringToXRPath("/user/hand/right/input/aim/pose")},
		{menuSelect.action, StringToXRPath("/user/hand/right/input/trigger/touch")},
		{menuBack.action, StringToXRPath("/user/hand/right/input/b/click")},
		{menuScroll.action, StringToXRPath("/user/hand/right/input/thumbstick/y")}
		});
	menuActionSet.actions.push_back(menuPointer);
	menuActionSet.actions.push_back(menuSelect);
	menuActionSet.actions.push_back(menuBack);
	menuActionSet.actions.push_back(menuScroll);
	actionSets.push_back(menuActionSet);
	idXrActionSet gameActionSet;
	gameActionSet.name = "GAME";
	XrActionSetCreateInfo gxrActCI{ XR_TYPE_ACTION_SET_CREATE_INFO };
	strncpy(gxrActCI.actionSetName, "doom-bfa-game-action-set", 26);
	strncpy(gxrActCI.localizedActionSetName, "DOOM BFA Game Action Set", 26);
	gxrActCI.priority = 0;
	if (xrCreateActionSet(instance, &gxrActCI, &gameActionSet.actionSet) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to Create Game Action Set");
	}
	actionSets.push_back(gameActionSet);
}

idXR_Win::idXrAction idXR_Win::CreateAction(XrActionSet actionSet, const char* name, XrActionType type, uint32_t mappedKey, std::vector<const char*> subActions)
{
	idXrAction action;
	action.name = name;
	action.type = type;
	action.mappedKey = mappedKey;
	XrActionCreateInfo actCI{ XR_TYPE_ACTION_CREATE_INFO };
	actCI.actionType = type;
	std::vector<XrPath> subXrActions;
	for (const char* subAction : subActions) {
		subXrActions.push_back(StringToXRPath(subAction));
	}
	actCI.countSubactionPaths = (uint32_t)subXrActions.size();
	actCI.subactionPaths = subXrActions.data();
	strncpy(actCI.actionName, name, XR_MAX_ACTION_NAME_SIZE);
	strncpy(actCI.localizedActionName, name, XR_MAX_LOCALIZED_ACTION_NAME_SIZE);
	XrResult res = xrCreateAction(actionSet, &actCI, &action.action);
	if (res != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to Create Action");
	}
	return action;
}

void idXR_Win::SuggestBindings(const char* profilePath, std::vector<XrActionSuggestedBinding> bindings)
{
	XrInteractionProfileSuggestedBinding ipsb{ XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING };
	ipsb.interactionProfile = StringToXRPath(profilePath);
	ipsb.suggestedBindings = bindings.data();
	ipsb.countSuggestedBindings = (uint32_t)bindings.size();
	XrResult res = xrSuggestInteractionProfileBindings(instance, &ipsb);
	if (res != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to Create Suggestion Binding");
	}
}

XrSpace idXR_Win::CreateActionPoseSpace(XrAction action, const char* subPath)
{
	XrSpace space;
	const XrPosef poseId = { {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };
	XrActionSpaceCreateInfo asCI{ XR_TYPE_ACTION_SPACE_CREATE_INFO };
	asCI.action = action;
	asCI.poseInActionSpace = poseId;
	if (subPath) {
		asCI.subactionPath = StringToXRPath(subPath);
	}
	if (xrCreateActionSpace(session, &asCI, &space) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to Create Action Space");
	}
	return space;
}

void idXR_Win::FinalizeActions()
{
	idXrActionSet menuActionSet = GetActionSetByName("MENU");
	idXrAction menuPointer = GetActionByName("MENU", "menu-pointer");
	handPoseSpace.push_back(CreateActionPoseSpace(menuPointer.action, "/user/hand/left"));
	handPoseSpace.push_back(CreateActionPoseSpace(menuPointer.action, "/user/hand/right"));
	XrSessionActionSetsAttachInfo sasaI{ XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO };
	sasaI.countActionSets = 1;
	sasaI.actionSets = &menuActionSet.actionSet;
	if (xrAttachSessionActionSets(session, &sasaI) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to Attach Action Sets");
	}

}

void idXR_Win::PollActions()
{
	idXrActionSet expectedIdActionSet = GetActionSetByName(expectedActionSet);
	XrActiveActionSet activeActionSet{};
	activeActionSet.actionSet = expectedIdActionSet.actionSet;
	activeActionSet.subactionPath = XR_NULL_PATH;
	XrActionsSyncInfo actionSyncInfo{ XR_TYPE_ACTIONS_SYNC_INFO };
	actionSyncInfo.activeActionSets = &activeActionSet;
	actionSyncInfo.countActiveActionSets = 1;
	if (xrSyncActions(session, &actionSyncInfo) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to Sync Action Sets");
	}
	for (idXrAction action : expectedIdActionSet.actions) {
		RetrieveActionState(action);
		MapActionStateToUsrCmd(action);
	}
}

void idXR_Win::RetrieveActionState(idXrAction action)
{
	XrActionStateGetInfo actionStateGI{ XR_TYPE_ACTION_STATE_GET_INFO };
	actionStateGI.action = action.action;
	for (int i = 0; i < 2; i++) {
		actionStateGI.subactionPath = handPaths[i];
		XrResult res = XR_ERROR_PATH_UNSUPPORTED;
		switch (action.type) {
		case XR_ACTION_TYPE_POSE_INPUT: {
			res = xrGetActionStatePose(session, &actionStateGI, &action.poseState[i]);
			if (res == XR_SUCCESS && action.poseState[i].isActive) {
				XrSpaceLocation handLocation{ XR_TYPE_SPACE_LOCATION };
				res = xrLocateSpace(handPoseSpace[i], localSpace, predictedDisplayTime, &handLocation);
				if (XR_UNQUALIFIED_SUCCESS(res) &&
					(handLocation.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT) != 0 &&
					(handLocation.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) != 0) {
					handPose[i] = handLocation.pose;
				}
				else {
					action.poseState[i].isActive = false;
				}
			}
			break;
		}
		case XR_ACTION_TYPE_BOOLEAN_INPUT:
			res = xrGetActionStateBoolean(session, &actionStateGI, &action.booleanState[i]);
			break;
		case XR_ACTION_TYPE_FLOAT_INPUT:
			res = xrGetActionStateFloat(session, &actionStateGI, &action.floatState[i]);
			break;
		case XR_ACTION_TYPE_VECTOR2F_INPUT:
			res = xrGetActionStateVector2f(session, &actionStateGI, &action.vector2fState[i]);
			break;
		}
		if (res != XR_SUCCESS) {
			common->Warning("OpenXR Error: Failed to retrieve action state");
		}
	}

}

idXR_Win::idXrAction idXR_Win::GetActionByName(idStr setName, idStr name)
{
	for (idXrActionSet actionSet : actionSets) {
		if (!actionSet.name.Cmp(setName)) {
			for (idXrAction action : actionSet.actions) {
				if (!action.name.Cmp(name)) {
					return action;
				}
			}
		}
	}
	return {};
}

idXR_Win::idXrActionSet idXR_Win::GetActionSetByName(idStr setName)
{
	for (idXrActionSet actionSet : actionSets) {
		if (!actionSet.name.Cmp(setName)) {
			return actionSet;
		}
	}
	return {};
}

void idXR_Win::MapActionStateToUsrCmd(idXrAction action)
{
	for (int i = 0; i < 2; i++) {
		switch (action.type) {
			case XR_ACTION_TYPE_POSE_INPUT:
			{
				float deltaX = (handPose[i].position.x - oldhandPose[i][0]);
				float deltaY = (handPose[i].position.y - oldhandPose[i][1]);
				oldhandPose[i][0] = handPose[i].position.x;
				oldhandPose[i][1] = handPose[i].position.y;
				if (deltaX != 0.0f) {
					uint32_t calculatedX = abs(handPose[i].position.x) * renderSystem->GetWidth();
					Sys_QueEvent(SE_MOUSE, calculatedX, 0, 0, NULL, 0);
				}
				if (deltaY != 0.0f) {
					uint32_t calculatedY = abs(handPose[i].position.y) * renderSystem->GetHeight();
					Sys_QueEvent(SE_MOUSE, 0, calculatedY, 0, NULL, 0);
				}
				break;
			}
			case XR_ACTION_TYPE_BOOLEAN_INPUT:
				if (action.booleanState[i].currentState) {
					Sys_QueEvent(SE_KEY, action.mappedKey, action.booleanState[i].currentState, 0, NULL, 0);
				}
				break;
			case XR_ACTION_TYPE_FLOAT_INPUT: {
				if (action.floatState[i].currentState != 0.0f) {
					Sys_QueEvent(SE_JOYSTICK, action.mappedKey, action.floatState[i].currentState, 0, NULL, 0);
					int targetKey = K_JOY_STICK1_LEFT;
					switch (action.mappedKey - J_AXIS_MIN) {
					case 0:
						targetKey = action.floatState[i].currentState < 0 ? K_JOY_STICK1_LEFT : K_JOY_STICK1_RIGHT;
						break;
					case 1:
						targetKey = action.floatState[i].currentState < 0 ? K_JOY_STICK1_UP : K_JOY_STICK1_DOWN;
						break;
					case 2:
						targetKey = action.floatState[i].currentState < 0 ? K_JOY_STICK2_LEFT : K_JOY_STICK2_RIGHT;
						break;
					case 3:
						targetKey = action.floatState[i].currentState < 0 ? K_JOY_STICK2_UP : K_JOY_STICK2_DOWN;
						break;
					}
					Sys_QueEvent(SE_KEY, targetKey, 1, 0, NULL, 0);
				}
				break;
			}

		}
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



bool idXR_Win::InitXR() {
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
	this->extensions.push_back(XR_FB_DISPLAY_REFRESH_RATE_EXTENSION_NAME);

	uint apiLayerCount = 0;
	std::vector<XrApiLayerProperties> apiLayerProperties;
	if (xrEnumerateApiLayerProperties(0, &apiLayerCount, nullptr) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to enumerate App Layer Properties");
		return false;
	}
	apiLayerProperties.resize(apiLayerCount, { XR_TYPE_API_LAYER_PROPERTIES });
	if (xrEnumerateApiLayerProperties(apiLayerCount, &apiLayerCount, apiLayerProperties.data()) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to retrieve App Layer Properties");
		return false;
	}
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

	uint extensionCount = 0;
	std::vector<XrExtensionProperties> extensionProperties;
	if (xrEnumerateInstanceExtensionProperties(nullptr, 0, &extensionCount, nullptr) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to enumerate Instance Extension Properties");
		return false;
	}
	extensionProperties.resize(extensionCount, { XR_TYPE_EXTENSION_PROPERTIES });
	if (xrEnumerateInstanceExtensionProperties(nullptr, extensionCount, &extensionCount, extensionProperties.data()) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to retrieve Instance Extension Properties");
		return false;
	}
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
			common->Warning("Failed to find OpenXR Extension: %s", requestExtension.c_str());
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
		common->Warning("OpenXR Error: Failed to initiate Instace");
		return false;
	}

	XrInstanceProperties instanceProperties{ XR_TYPE_INSTANCE_PROPERTIES };
	if (xrGetInstanceProperties(instance, &instanceProperties) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to retrieve Instace Properties");
		return false;
	}
	char* xrVersion = new char[20];
	sprintf(xrVersion, "%d.%d.%d", XR_VERSION_MAJOR(instanceProperties.runtimeVersion), XR_VERSION_MINOR(instanceProperties.runtimeVersion), XR_VERSION_PATCH(instanceProperties.runtimeVersion));
	common->Printf("OpenXR Have been initialized\n------------------------------------------------\nRuntime Name: %s\nRuntime Version: %s\n------------------------------------------------\n", instanceProperties.runtimeName, xrVersion);
	
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
				return false;
			}
		}
	}
#endif

	XrSystemGetInfo systemGI{ XR_TYPE_SYSTEM_GET_INFO };
	systemGI.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
	if (xrGetSystem(instance, &systemGI, &systemId) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to retrieve SystemId");
		return false;
	}
	if (xrGetSystemProperties(instance, systemId, &systemProperties) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to retrieve System properties");
		return false;
	}
	common->Printf("OpenXR Compatible System Have been Found\n------------------------------------------------\nVendor: %d\nSystem: %s\n------------------------------------------------\n", systemProperties.vendorId, systemProperties.systemName);

	//Required before Creating a Session

	CreateXrMappings();
	XrGraphicsRequirementsOpenGLKHR requirements{ XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_KHR };
	PFN_xrGetOpenGLGraphicsRequirementsKHR xrGetOpenGLGraphicsRequirementsKHR;
	if (xrGetInstanceProcAddr(instance, "xrGetOpenGLGraphicsRequirementsKHR", (PFN_xrVoidFunction*)&xrGetOpenGLGraphicsRequirementsKHR) == XR_SUCCESS) {
		if (xrGetOpenGLGraphicsRequirementsKHR(instance, systemId, &requirements) != XR_SUCCESS) {
			common->Warning("OpenXR Error: Failed to Retrieve Graphic Requirements");
			return false;
		}
	}

	uint viewConfigurationCount = 0;
	if (xrEnumerateViewConfigurations(instance, systemId, 0, &viewConfigurationCount, nullptr) == XR_SUCCESS) {
		viewConfigurations.resize(viewConfigurationCount);
		if (xrEnumerateViewConfigurations(instance, systemId, viewConfigurationCount, &viewConfigurationCount, viewConfigurations.data()) != XR_SUCCESS) {
			common->Warning("OpenXR Error: Failed to Enumerate View Configurations");
			return false;
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
			return false;
		}
	}

	viewProperties = { XR_TYPE_VIEW_CONFIGURATION_PROPERTIES };
	if (xrGetViewConfigurationProperties(instance, systemId, viewConfiguration, &viewProperties) == XR_SUCCESS) {
		width = configurationView[0].recommendedImageRectWidth; 
		height = configurationView[0].recommendedImageRectHeight;
	}

	//Create a Session
	XrSessionCreateInfo sci{ XR_TYPE_SESSION_CREATE_INFO };
	sci.next = GetOpenXRGraphicsBinding();
	sci.createFlags = 0;
	sci.systemId = systemId;
	if (xrCreateSession(instance, &sci, &session) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to create Session");
		return false;
	}
	float refreshRate = 60.0f;
	PFN_xrGetDisplayRefreshRateFB xrGetDisplayRefreshRate;
	if (xrGetInstanceProcAddr(instance, "xrGetDisplayRefreshRateFB", (PFN_xrVoidFunction*)&xrGetDisplayRefreshRate) == XR_SUCCESS) {
		if (xrGetDisplayRefreshRate(session, &refreshRate) != XR_SUCCESS) {
			refreshRate = 60.0f;
			common->Warning("OpenXR Error: Failed to retrieve VR Refresh rate");
		}
	}

	com_engineHz.SetFloat(refreshRate);
	FinalizeActions();

	XrReferenceSpaceCreateInfo referenceSpaceCI{ XR_TYPE_REFERENCE_SPACE_CREATE_INFO };
	referenceSpaceCI.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_LOCAL;
	referenceSpaceCI.poseInReferenceSpace = { {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };
	if (xrCreateReferenceSpace(session, &referenceSpaceCI, &localSpace) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to create Reference Space");
		return false;
	}

	uint formatCount = 0;
	if (xrEnumerateSwapchainFormats(session, 0, &formatCount, nullptr) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to Initiate the enumeration for Swapchain Formats");
		return false;
	}
	std::vector<int64> formats(formatCount);
	if (xrEnumerateSwapchainFormats(session, formatCount, &formatCount, formats.data()) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to Enumerate Swapchain Formats");
		return false;
	}

	std::vector<int64> supportedColorFormats = {
		GL_SRGB8_ALPHA8
	};
	int64 colorFormat = 0;
	std::vector<int64>::const_iterator colorFormatIterator = std::find_first_of(formats.begin(), formats.end(), supportedColorFormats.begin(), supportedColorFormats.end());

	if (colorFormatIterator == formats.end()) {
		common->Warning("OpenXR Error: Failed to find Color Format");
		return false;
	}
	else {
		colorFormat = *colorFormatIterator;
	}

	colorSwapchainInfo.resize(configurationView.size());

	

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
			return false;
		}

		uint colorSwapchainImageCount = this->EnumerateSwapchainImage(colorSwapchainInfo, idXRSwapchainType::COLOR, i);

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
	}

	GeneralImage = new (TAG_IMAGE)idImage("VR");
	idImageOpts opts;
	opts.format = FMT_RGBA8;
	opts.colorFormat = CFM_DEFAULT;
	opts.width = width;
	opts.height = height;
	opts.numLevels = 1;
	GeneralImage->AllocImage(opts, TF_LINEAR, TR_REPEAT);
	FrameBufferCreateInfo fbCI;
	fbCI.image = (void*)(uint64_t)GeneralImage->texnum;
	fbCI.type = FrameBufferCreateInfo::Type::RTV;
	fbCI.view = FrameBufferCreateInfo::View::TYPE_2D;
	fbCI.format = GL_SRGB8_ALPHA8;
	fbCI.aspect = FrameBufferCreateInfo::Aspect::COLOR_BIT;
	fbCI.baseMipLevel = 0;
	fbCI.levelCount = 1;
	fbCI.baseArrayLayer = 0;
	fbCI.layerCount = 1;
	GeneralFB = (GLuint)(uint64_t)CreateFrameBuffer(fbCI);

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

	isInitialized = true;
	return isInitialized;
}


void idXR_Win::ShutDownXR()
{
	if (GeneralFB > 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &GeneralFB);
	}
	if (GeneralImage != NULL) {
		GeneralImage->PurgeImage();
	}

	if (colorSwapchainInfo.size() > 0) {
		for (int i = 0; i < colorSwapchainInfo.size(); i++) {
			for (int j = 0; j < colorSwapchainInfo[i].imageViews.size(); j++) {
				GLuint fbo = (GLuint)(uint64_t)colorSwapchainInfo[i].imageViews[j];
				glDeleteFramebuffers(1, &fbo);
			}
			colorSwapchainInfo[i].imageViews.clear();
			swapchainImageMap[colorSwapchainInfo[i].swapchain].second.clear();
			if (xrDestroySwapchain(colorSwapchainInfo[i].swapchain) != XR_SUCCESS) {
				common->Warning("OpenXR Error: Failed to delete Color Swapchain Image");
			}
		}
	}
	swapchainImageMap.clear();

	if (localSpace != XR_NULL_HANDLE && xrDestroySpace(localSpace) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to close OpenXR Space");
	}
	if (session != XR_NULL_HANDLE && xrDestroySession(session) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to close OpenXR Session");
	}
#ifdef _DEBUG
	PFN_xrDestroyDebugUtilsMessengerEXT xrDestroyDebugMessager;
	if (instance != XR_NULL_HANDLE && xrGetInstanceProcAddr(instance, "xrDestroyDebugUtilsMessengerEXT", (PFN_xrVoidFunction*)&xrDestroyDebugMessager) == XR_SUCCESS) {
		if (debugMessager != XR_NULL_HANDLE && xrDestroyDebugMessager(debugMessager) != XR_SUCCESS) {
			common->Warning("OpenXR Error: Failed to Destroy Debug Messenger");
		}
	}
#endif
	if (instance != XR_NULL_HANDLE && xrDestroyInstance(instance) != XR_SUCCESS) {
		common->Warning("OpenXR Error: Failed to close OpenXR");
	}
	isInitialized = false;
}
