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
#pragma hdrstop
//#define XR_USE_GRAPHICS_API_OPENGL
#include "openxr/openxr.h"
#pragma warning( disable : 4005 )
#include <vector>
#include <string>

class idXR {
public:
	virtual bool InitXR() = 0;
	virtual void ShutDownXR() = 0;
	virtual void PollXREvents() = 0;
	virtual void StartFrame() = 0;
	virtual void BindSwapchainImage(int eye) = 0;
	virtual void ReleaseSwapchainImage() = 0;
	virtual void RenderFrame(int srcX, int srcY, int srcW, int srcH) = 0;
	virtual void EndFrame() = 0;
	virtual uint32_t GetWidth() = 0;
	virtual uint32_t GetHeight() = 0;
	virtual bool IsInitialized() = 0;
	virtual bool isFOVmutable() = 0;
	virtual void SetActionSet(idStr name) = 0;
	enum class idXRSwapchainType : uint8{
		COLOR,
		DEPTH
	};
};

extern idXR* xrSystem;