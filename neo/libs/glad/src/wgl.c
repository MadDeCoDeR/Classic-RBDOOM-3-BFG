/**
 * SPDX-License-Identifier: (WTFPL OR CC0-1.0) AND Apache-2.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glad/wgl.h>

#ifndef GLAD_IMPL_UTIL_C_
#define GLAD_IMPL_UTIL_C_

#ifdef _MSC_VER
#define GLAD_IMPL_UTIL_SSCANF sscanf_s
#else
#define GLAD_IMPL_UTIL_SSCANF sscanf
#endif

#endif /* GLAD_IMPL_UTIL_C_ */

#ifdef __cplusplus
extern "C" {
#endif



int GLAD_WGL_VERSION_1_0 = 0;
int GLAD_WGL_3DFX_multisample = 0;
int GLAD_WGL_3DL_stereo_control = 0;
int GLAD_WGL_AMD_gpu_association = 0;
int GLAD_WGL_ARB_buffer_region = 0;
int GLAD_WGL_ARB_context_flush_control = 0;
int GLAD_WGL_ARB_create_context = 0;
int GLAD_WGL_ARB_create_context_no_error = 0;
int GLAD_WGL_ARB_create_context_profile = 0;
int GLAD_WGL_ARB_create_context_robustness = 0;
int GLAD_WGL_ARB_extensions_string = 0;
int GLAD_WGL_ARB_framebuffer_sRGB = 0;
int GLAD_WGL_ARB_make_current_read = 0;
int GLAD_WGL_ARB_multisample = 0;
int GLAD_WGL_ARB_pbuffer = 0;
int GLAD_WGL_ARB_pixel_format = 0;
int GLAD_WGL_ARB_pixel_format_float = 0;
int GLAD_WGL_ARB_render_texture = 0;
int GLAD_WGL_ARB_robustness_application_isolation = 0;
int GLAD_WGL_ARB_robustness_share_group_isolation = 0;
int GLAD_WGL_ATI_pixel_format_float = 0;
int GLAD_WGL_ATI_render_texture_rectangle = 0;
int GLAD_WGL_EXT_colorspace = 0;
int GLAD_WGL_EXT_create_context_es2_profile = 0;
int GLAD_WGL_EXT_create_context_es_profile = 0;
int GLAD_WGL_EXT_depth_float = 0;
int GLAD_WGL_EXT_display_color_table = 0;
int GLAD_WGL_EXT_extensions_string = 0;
int GLAD_WGL_EXT_framebuffer_sRGB = 0;
int GLAD_WGL_EXT_make_current_read = 0;
int GLAD_WGL_EXT_multisample = 0;
int GLAD_WGL_EXT_pbuffer = 0;
int GLAD_WGL_EXT_pixel_format = 0;
int GLAD_WGL_EXT_pixel_format_packed_float = 0;
int GLAD_WGL_EXT_swap_control = 0;
int GLAD_WGL_EXT_swap_control_tear = 0;
int GLAD_WGL_I3D_digital_video_control = 0;
int GLAD_WGL_I3D_gamma = 0;
int GLAD_WGL_I3D_genlock = 0;
int GLAD_WGL_I3D_image_buffer = 0;
int GLAD_WGL_I3D_swap_frame_lock = 0;
int GLAD_WGL_I3D_swap_frame_usage = 0;
int GLAD_WGL_NV_DX_interop = 0;
int GLAD_WGL_NV_DX_interop2 = 0;
int GLAD_WGL_NV_copy_image = 0;
int GLAD_WGL_NV_delay_before_swap = 0;
int GLAD_WGL_NV_float_buffer = 0;
int GLAD_WGL_NV_gpu_affinity = 0;
int GLAD_WGL_NV_multigpu_context = 0;
int GLAD_WGL_NV_multisample_coverage = 0;
int GLAD_WGL_NV_present_video = 0;
int GLAD_WGL_NV_render_depth_texture = 0;
int GLAD_WGL_NV_render_texture_rectangle = 0;
int GLAD_WGL_NV_swap_group = 0;
int GLAD_WGL_NV_vertex_array_range = 0;
int GLAD_WGL_NV_video_capture = 0;
int GLAD_WGL_NV_video_output = 0;
int GLAD_WGL_OML_sync_control = 0;


static void _pre_call_wgl_callback_default(const char *name, GLADapiproc apiproc, int len_args, ...) {
    GLAD_UNUSED(name);
    GLAD_UNUSED(apiproc);
    GLAD_UNUSED(len_args);
}
static void _post_call_wgl_callback_default(void *ret, const char *name, GLADapiproc apiproc, int len_args, ...) {
    GLAD_UNUSED(ret);
    GLAD_UNUSED(name);
    GLAD_UNUSED(apiproc);
    GLAD_UNUSED(len_args);
}

static GLADprecallback _pre_call_wgl_callback = _pre_call_wgl_callback_default;
void gladSetWGLPreCallback(GLADprecallback cb) {
    _pre_call_wgl_callback = cb;
}
static GLADpostcallback _post_call_wgl_callback = _post_call_wgl_callback_default;
void gladSetWGLPostCallback(GLADpostcallback cb) {
    _post_call_wgl_callback = cb;
}

PFNWGLALLOCATEMEMORYNVPROC glad_wglAllocateMemoryNV = NULL;
static void * GLAD_API_PTR glad_debug_impl_wglAllocateMemoryNV(GLsizei size, GLfloat readfreq, GLfloat writefreq, GLfloat priority) {
    void * ret;
    _pre_call_wgl_callback("wglAllocateMemoryNV", (GLADapiproc) glad_wglAllocateMemoryNV, 4, size, readfreq, writefreq, priority);
    ret = glad_wglAllocateMemoryNV(size, readfreq, writefreq, priority);
    _post_call_wgl_callback((void*) &ret, "wglAllocateMemoryNV", (GLADapiproc) glad_wglAllocateMemoryNV, 4, size, readfreq, writefreq, priority);
    return ret;
}
PFNWGLALLOCATEMEMORYNVPROC glad_debug_wglAllocateMemoryNV = glad_debug_impl_wglAllocateMemoryNV;
PFNWGLASSOCIATEIMAGEBUFFEREVENTSI3DPROC glad_wglAssociateImageBufferEventsI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglAssociateImageBufferEventsI3D(HDC hDC, const HANDLE * pEvent, const LPVOID * pAddress, const DWORD * pSize, UINT count) {
    BOOL ret;
    _pre_call_wgl_callback("wglAssociateImageBufferEventsI3D", (GLADapiproc) glad_wglAssociateImageBufferEventsI3D, 5, hDC, pEvent, pAddress, pSize, count);
    ret = glad_wglAssociateImageBufferEventsI3D(hDC, pEvent, pAddress, pSize, count);
    _post_call_wgl_callback((void*) &ret, "wglAssociateImageBufferEventsI3D", (GLADapiproc) glad_wglAssociateImageBufferEventsI3D, 5, hDC, pEvent, pAddress, pSize, count);
    return ret;
}
PFNWGLASSOCIATEIMAGEBUFFEREVENTSI3DPROC glad_debug_wglAssociateImageBufferEventsI3D = glad_debug_impl_wglAssociateImageBufferEventsI3D;
PFNWGLBEGINFRAMETRACKINGI3DPROC glad_wglBeginFrameTrackingI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglBeginFrameTrackingI3D(void) {
    BOOL ret;
    _pre_call_wgl_callback("wglBeginFrameTrackingI3D", (GLADapiproc) glad_wglBeginFrameTrackingI3D, 0);
    ret = glad_wglBeginFrameTrackingI3D();
    _post_call_wgl_callback((void*) &ret, "wglBeginFrameTrackingI3D", (GLADapiproc) glad_wglBeginFrameTrackingI3D, 0);
    return ret;
}
PFNWGLBEGINFRAMETRACKINGI3DPROC glad_debug_wglBeginFrameTrackingI3D = glad_debug_impl_wglBeginFrameTrackingI3D;
PFNWGLBINDDISPLAYCOLORTABLEEXTPROC glad_wglBindDisplayColorTableEXT = NULL;
static GLboolean GLAD_API_PTR glad_debug_impl_wglBindDisplayColorTableEXT(GLushort id) {
    GLboolean ret;
    _pre_call_wgl_callback("wglBindDisplayColorTableEXT", (GLADapiproc) glad_wglBindDisplayColorTableEXT, 1, id);
    ret = glad_wglBindDisplayColorTableEXT(id);
    _post_call_wgl_callback((void*) &ret, "wglBindDisplayColorTableEXT", (GLADapiproc) glad_wglBindDisplayColorTableEXT, 1, id);
    return ret;
}
PFNWGLBINDDISPLAYCOLORTABLEEXTPROC glad_debug_wglBindDisplayColorTableEXT = glad_debug_impl_wglBindDisplayColorTableEXT;
PFNWGLBINDSWAPBARRIERNVPROC glad_wglBindSwapBarrierNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglBindSwapBarrierNV(GLuint group, GLuint barrier) {
    BOOL ret;
    _pre_call_wgl_callback("wglBindSwapBarrierNV", (GLADapiproc) glad_wglBindSwapBarrierNV, 2, group, barrier);
    ret = glad_wglBindSwapBarrierNV(group, barrier);
    _post_call_wgl_callback((void*) &ret, "wglBindSwapBarrierNV", (GLADapiproc) glad_wglBindSwapBarrierNV, 2, group, barrier);
    return ret;
}
PFNWGLBINDSWAPBARRIERNVPROC glad_debug_wglBindSwapBarrierNV = glad_debug_impl_wglBindSwapBarrierNV;
PFNWGLBINDTEXIMAGEARBPROC glad_wglBindTexImageARB = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglBindTexImageARB(HPBUFFERARB hPbuffer, int iBuffer) {
    BOOL ret;
    _pre_call_wgl_callback("wglBindTexImageARB", (GLADapiproc) glad_wglBindTexImageARB, 2, hPbuffer, iBuffer);
    ret = glad_wglBindTexImageARB(hPbuffer, iBuffer);
    _post_call_wgl_callback((void*) &ret, "wglBindTexImageARB", (GLADapiproc) glad_wglBindTexImageARB, 2, hPbuffer, iBuffer);
    return ret;
}
PFNWGLBINDTEXIMAGEARBPROC glad_debug_wglBindTexImageARB = glad_debug_impl_wglBindTexImageARB;
PFNWGLBINDVIDEOCAPTUREDEVICENVPROC glad_wglBindVideoCaptureDeviceNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglBindVideoCaptureDeviceNV(UINT uVideoSlot, HVIDEOINPUTDEVICENV hDevice) {
    BOOL ret;
    _pre_call_wgl_callback("wglBindVideoCaptureDeviceNV", (GLADapiproc) glad_wglBindVideoCaptureDeviceNV, 2, uVideoSlot, hDevice);
    ret = glad_wglBindVideoCaptureDeviceNV(uVideoSlot, hDevice);
    _post_call_wgl_callback((void*) &ret, "wglBindVideoCaptureDeviceNV", (GLADapiproc) glad_wglBindVideoCaptureDeviceNV, 2, uVideoSlot, hDevice);
    return ret;
}
PFNWGLBINDVIDEOCAPTUREDEVICENVPROC glad_debug_wglBindVideoCaptureDeviceNV = glad_debug_impl_wglBindVideoCaptureDeviceNV;
PFNWGLBINDVIDEODEVICENVPROC glad_wglBindVideoDeviceNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglBindVideoDeviceNV(HDC hDc, unsigned int uVideoSlot, HVIDEOOUTPUTDEVICENV hVideoDevice, const int * piAttribList) {
    BOOL ret;
    _pre_call_wgl_callback("wglBindVideoDeviceNV", (GLADapiproc) glad_wglBindVideoDeviceNV, 4, hDc, uVideoSlot, hVideoDevice, piAttribList);
    ret = glad_wglBindVideoDeviceNV(hDc, uVideoSlot, hVideoDevice, piAttribList);
    _post_call_wgl_callback((void*) &ret, "wglBindVideoDeviceNV", (GLADapiproc) glad_wglBindVideoDeviceNV, 4, hDc, uVideoSlot, hVideoDevice, piAttribList);
    return ret;
}
PFNWGLBINDVIDEODEVICENVPROC glad_debug_wglBindVideoDeviceNV = glad_debug_impl_wglBindVideoDeviceNV;
PFNWGLBINDVIDEOIMAGENVPROC glad_wglBindVideoImageNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglBindVideoImageNV(HPVIDEODEV hVideoDevice, HPBUFFERARB hPbuffer, int iVideoBuffer) {
    BOOL ret;
    _pre_call_wgl_callback("wglBindVideoImageNV", (GLADapiproc) glad_wglBindVideoImageNV, 3, hVideoDevice, hPbuffer, iVideoBuffer);
    ret = glad_wglBindVideoImageNV(hVideoDevice, hPbuffer, iVideoBuffer);
    _post_call_wgl_callback((void*) &ret, "wglBindVideoImageNV", (GLADapiproc) glad_wglBindVideoImageNV, 3, hVideoDevice, hPbuffer, iVideoBuffer);
    return ret;
}
PFNWGLBINDVIDEOIMAGENVPROC glad_debug_wglBindVideoImageNV = glad_debug_impl_wglBindVideoImageNV;
PFNWGLBLITCONTEXTFRAMEBUFFERAMDPROC glad_wglBlitContextFramebufferAMD = NULL;
static VOID GLAD_API_PTR glad_debug_impl_wglBlitContextFramebufferAMD(HGLRC dstCtx, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) {
    _pre_call_wgl_callback("wglBlitContextFramebufferAMD", (GLADapiproc) glad_wglBlitContextFramebufferAMD, 11, dstCtx, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
    glad_wglBlitContextFramebufferAMD(dstCtx, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
    _post_call_wgl_callback(NULL, "wglBlitContextFramebufferAMD", (GLADapiproc) glad_wglBlitContextFramebufferAMD, 11, dstCtx, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
    
}
PFNWGLBLITCONTEXTFRAMEBUFFERAMDPROC glad_debug_wglBlitContextFramebufferAMD = glad_debug_impl_wglBlitContextFramebufferAMD;
PFNWGLCHOOSEPIXELFORMATARBPROC glad_wglChoosePixelFormatARB = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglChoosePixelFormatARB(HDC hdc, const int * piAttribIList, const FLOAT * pfAttribFList, UINT nMaxFormats, int * piFormats, UINT * nNumFormats) {
    BOOL ret;
    _pre_call_wgl_callback("wglChoosePixelFormatARB", (GLADapiproc) glad_wglChoosePixelFormatARB, 6, hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats);
    ret = glad_wglChoosePixelFormatARB(hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats);
    _post_call_wgl_callback((void*) &ret, "wglChoosePixelFormatARB", (GLADapiproc) glad_wglChoosePixelFormatARB, 6, hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats);
    return ret;
}
PFNWGLCHOOSEPIXELFORMATARBPROC glad_debug_wglChoosePixelFormatARB = glad_debug_impl_wglChoosePixelFormatARB;
PFNWGLCHOOSEPIXELFORMATEXTPROC glad_wglChoosePixelFormatEXT = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglChoosePixelFormatEXT(HDC hdc, const int * piAttribIList, const FLOAT * pfAttribFList, UINT nMaxFormats, int * piFormats, UINT * nNumFormats) {
    BOOL ret;
    _pre_call_wgl_callback("wglChoosePixelFormatEXT", (GLADapiproc) glad_wglChoosePixelFormatEXT, 6, hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats);
    ret = glad_wglChoosePixelFormatEXT(hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats);
    _post_call_wgl_callback((void*) &ret, "wglChoosePixelFormatEXT", (GLADapiproc) glad_wglChoosePixelFormatEXT, 6, hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats);
    return ret;
}
PFNWGLCHOOSEPIXELFORMATEXTPROC glad_debug_wglChoosePixelFormatEXT = glad_debug_impl_wglChoosePixelFormatEXT;
PFNWGLCOPYIMAGESUBDATANVPROC glad_wglCopyImageSubDataNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglCopyImageSubDataNV(HGLRC hSrcRC, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, HGLRC hDstRC, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth) {
    BOOL ret;
    _pre_call_wgl_callback("wglCopyImageSubDataNV", (GLADapiproc) glad_wglCopyImageSubDataNV, 17, hSrcRC, srcName, srcTarget, srcLevel, srcX, srcY, srcZ, hDstRC, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, width, height, depth);
    ret = glad_wglCopyImageSubDataNV(hSrcRC, srcName, srcTarget, srcLevel, srcX, srcY, srcZ, hDstRC, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, width, height, depth);
    _post_call_wgl_callback((void*) &ret, "wglCopyImageSubDataNV", (GLADapiproc) glad_wglCopyImageSubDataNV, 17, hSrcRC, srcName, srcTarget, srcLevel, srcX, srcY, srcZ, hDstRC, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, width, height, depth);
    return ret;
}
PFNWGLCOPYIMAGESUBDATANVPROC glad_debug_wglCopyImageSubDataNV = glad_debug_impl_wglCopyImageSubDataNV;
PFNWGLCREATEAFFINITYDCNVPROC glad_wglCreateAffinityDCNV = NULL;
static HDC GLAD_API_PTR glad_debug_impl_wglCreateAffinityDCNV(const HGPUNV * phGpuList) {
    HDC ret;
    _pre_call_wgl_callback("wglCreateAffinityDCNV", (GLADapiproc) glad_wglCreateAffinityDCNV, 1, phGpuList);
    ret = glad_wglCreateAffinityDCNV(phGpuList);
    _post_call_wgl_callback((void*) &ret, "wglCreateAffinityDCNV", (GLADapiproc) glad_wglCreateAffinityDCNV, 1, phGpuList);
    return ret;
}
PFNWGLCREATEAFFINITYDCNVPROC glad_debug_wglCreateAffinityDCNV = glad_debug_impl_wglCreateAffinityDCNV;
PFNWGLCREATEASSOCIATEDCONTEXTAMDPROC glad_wglCreateAssociatedContextAMD = NULL;
static HGLRC GLAD_API_PTR glad_debug_impl_wglCreateAssociatedContextAMD(UINT id) {
    HGLRC ret;
    _pre_call_wgl_callback("wglCreateAssociatedContextAMD", (GLADapiproc) glad_wglCreateAssociatedContextAMD, 1, id);
    ret = glad_wglCreateAssociatedContextAMD(id);
    _post_call_wgl_callback((void*) &ret, "wglCreateAssociatedContextAMD", (GLADapiproc) glad_wglCreateAssociatedContextAMD, 1, id);
    return ret;
}
PFNWGLCREATEASSOCIATEDCONTEXTAMDPROC glad_debug_wglCreateAssociatedContextAMD = glad_debug_impl_wglCreateAssociatedContextAMD;
PFNWGLCREATEASSOCIATEDCONTEXTATTRIBSAMDPROC glad_wglCreateAssociatedContextAttribsAMD = NULL;
static HGLRC GLAD_API_PTR glad_debug_impl_wglCreateAssociatedContextAttribsAMD(UINT id, HGLRC hShareContext, const int * attribList) {
    HGLRC ret;
    _pre_call_wgl_callback("wglCreateAssociatedContextAttribsAMD", (GLADapiproc) glad_wglCreateAssociatedContextAttribsAMD, 3, id, hShareContext, attribList);
    ret = glad_wglCreateAssociatedContextAttribsAMD(id, hShareContext, attribList);
    _post_call_wgl_callback((void*) &ret, "wglCreateAssociatedContextAttribsAMD", (GLADapiproc) glad_wglCreateAssociatedContextAttribsAMD, 3, id, hShareContext, attribList);
    return ret;
}
PFNWGLCREATEASSOCIATEDCONTEXTATTRIBSAMDPROC glad_debug_wglCreateAssociatedContextAttribsAMD = glad_debug_impl_wglCreateAssociatedContextAttribsAMD;
PFNWGLCREATEBUFFERREGIONARBPROC glad_wglCreateBufferRegionARB = NULL;
static HANDLE GLAD_API_PTR glad_debug_impl_wglCreateBufferRegionARB(HDC hDC, int iLayerPlane, UINT uType) {
    HANDLE ret;
    _pre_call_wgl_callback("wglCreateBufferRegionARB", (GLADapiproc) glad_wglCreateBufferRegionARB, 3, hDC, iLayerPlane, uType);
    ret = glad_wglCreateBufferRegionARB(hDC, iLayerPlane, uType);
    _post_call_wgl_callback((void*) &ret, "wglCreateBufferRegionARB", (GLADapiproc) glad_wglCreateBufferRegionARB, 3, hDC, iLayerPlane, uType);
    return ret;
}
PFNWGLCREATEBUFFERREGIONARBPROC glad_debug_wglCreateBufferRegionARB = glad_debug_impl_wglCreateBufferRegionARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC glad_wglCreateContextAttribsARB = NULL;
static HGLRC GLAD_API_PTR glad_debug_impl_wglCreateContextAttribsARB(HDC hDC, HGLRC hShareContext, const int * attribList) {
    HGLRC ret;
    _pre_call_wgl_callback("wglCreateContextAttribsARB", (GLADapiproc) glad_wglCreateContextAttribsARB, 3, hDC, hShareContext, attribList);
    ret = glad_wglCreateContextAttribsARB(hDC, hShareContext, attribList);
    _post_call_wgl_callback((void*) &ret, "wglCreateContextAttribsARB", (GLADapiproc) glad_wglCreateContextAttribsARB, 3, hDC, hShareContext, attribList);
    return ret;
}
PFNWGLCREATECONTEXTATTRIBSARBPROC glad_debug_wglCreateContextAttribsARB = glad_debug_impl_wglCreateContextAttribsARB;
PFNWGLCREATEDISPLAYCOLORTABLEEXTPROC glad_wglCreateDisplayColorTableEXT = NULL;
static GLboolean GLAD_API_PTR glad_debug_impl_wglCreateDisplayColorTableEXT(GLushort id) {
    GLboolean ret;
    _pre_call_wgl_callback("wglCreateDisplayColorTableEXT", (GLADapiproc) glad_wglCreateDisplayColorTableEXT, 1, id);
    ret = glad_wglCreateDisplayColorTableEXT(id);
    _post_call_wgl_callback((void*) &ret, "wglCreateDisplayColorTableEXT", (GLADapiproc) glad_wglCreateDisplayColorTableEXT, 1, id);
    return ret;
}
PFNWGLCREATEDISPLAYCOLORTABLEEXTPROC glad_debug_wglCreateDisplayColorTableEXT = glad_debug_impl_wglCreateDisplayColorTableEXT;
PFNWGLCREATEIMAGEBUFFERI3DPROC glad_wglCreateImageBufferI3D = NULL;
static LPVOID GLAD_API_PTR glad_debug_impl_wglCreateImageBufferI3D(HDC hDC, DWORD dwSize, UINT uFlags) {
    LPVOID ret;
    _pre_call_wgl_callback("wglCreateImageBufferI3D", (GLADapiproc) glad_wglCreateImageBufferI3D, 3, hDC, dwSize, uFlags);
    ret = glad_wglCreateImageBufferI3D(hDC, dwSize, uFlags);
    _post_call_wgl_callback((void*) &ret, "wglCreateImageBufferI3D", (GLADapiproc) glad_wglCreateImageBufferI3D, 3, hDC, dwSize, uFlags);
    return ret;
}
PFNWGLCREATEIMAGEBUFFERI3DPROC glad_debug_wglCreateImageBufferI3D = glad_debug_impl_wglCreateImageBufferI3D;
PFNWGLCREATEPBUFFERARBPROC glad_wglCreatePbufferARB = NULL;
static HPBUFFERARB GLAD_API_PTR glad_debug_impl_wglCreatePbufferARB(HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int * piAttribList) {
    HPBUFFERARB ret;
    _pre_call_wgl_callback("wglCreatePbufferARB", (GLADapiproc) glad_wglCreatePbufferARB, 5, hDC, iPixelFormat, iWidth, iHeight, piAttribList);
    ret = glad_wglCreatePbufferARB(hDC, iPixelFormat, iWidth, iHeight, piAttribList);
    _post_call_wgl_callback((void*) &ret, "wglCreatePbufferARB", (GLADapiproc) glad_wglCreatePbufferARB, 5, hDC, iPixelFormat, iWidth, iHeight, piAttribList);
    return ret;
}
PFNWGLCREATEPBUFFERARBPROC glad_debug_wglCreatePbufferARB = glad_debug_impl_wglCreatePbufferARB;
PFNWGLCREATEPBUFFEREXTPROC glad_wglCreatePbufferEXT = NULL;
static HPBUFFEREXT GLAD_API_PTR glad_debug_impl_wglCreatePbufferEXT(HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int * piAttribList) {
    HPBUFFEREXT ret;
    _pre_call_wgl_callback("wglCreatePbufferEXT", (GLADapiproc) glad_wglCreatePbufferEXT, 5, hDC, iPixelFormat, iWidth, iHeight, piAttribList);
    ret = glad_wglCreatePbufferEXT(hDC, iPixelFormat, iWidth, iHeight, piAttribList);
    _post_call_wgl_callback((void*) &ret, "wglCreatePbufferEXT", (GLADapiproc) glad_wglCreatePbufferEXT, 5, hDC, iPixelFormat, iWidth, iHeight, piAttribList);
    return ret;
}
PFNWGLCREATEPBUFFEREXTPROC glad_debug_wglCreatePbufferEXT = glad_debug_impl_wglCreatePbufferEXT;
PFNWGLDXCLOSEDEVICENVPROC glad_wglDXCloseDeviceNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglDXCloseDeviceNV(HANDLE hDevice) {
    BOOL ret;
    _pre_call_wgl_callback("wglDXCloseDeviceNV", (GLADapiproc) glad_wglDXCloseDeviceNV, 1, hDevice);
    ret = glad_wglDXCloseDeviceNV(hDevice);
    _post_call_wgl_callback((void*) &ret, "wglDXCloseDeviceNV", (GLADapiproc) glad_wglDXCloseDeviceNV, 1, hDevice);
    return ret;
}
PFNWGLDXCLOSEDEVICENVPROC glad_debug_wglDXCloseDeviceNV = glad_debug_impl_wglDXCloseDeviceNV;
PFNWGLDXLOCKOBJECTSNVPROC glad_wglDXLockObjectsNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglDXLockObjectsNV(HANDLE hDevice, GLint count, HANDLE * hObjects) {
    BOOL ret;
    _pre_call_wgl_callback("wglDXLockObjectsNV", (GLADapiproc) glad_wglDXLockObjectsNV, 3, hDevice, count, hObjects);
    ret = glad_wglDXLockObjectsNV(hDevice, count, hObjects);
    _post_call_wgl_callback((void*) &ret, "wglDXLockObjectsNV", (GLADapiproc) glad_wglDXLockObjectsNV, 3, hDevice, count, hObjects);
    return ret;
}
PFNWGLDXLOCKOBJECTSNVPROC glad_debug_wglDXLockObjectsNV = glad_debug_impl_wglDXLockObjectsNV;
PFNWGLDXOBJECTACCESSNVPROC glad_wglDXObjectAccessNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglDXObjectAccessNV(HANDLE hObject, GLenum access) {
    BOOL ret;
    _pre_call_wgl_callback("wglDXObjectAccessNV", (GLADapiproc) glad_wglDXObjectAccessNV, 2, hObject, access);
    ret = glad_wglDXObjectAccessNV(hObject, access);
    _post_call_wgl_callback((void*) &ret, "wglDXObjectAccessNV", (GLADapiproc) glad_wglDXObjectAccessNV, 2, hObject, access);
    return ret;
}
PFNWGLDXOBJECTACCESSNVPROC glad_debug_wglDXObjectAccessNV = glad_debug_impl_wglDXObjectAccessNV;
PFNWGLDXOPENDEVICENVPROC glad_wglDXOpenDeviceNV = NULL;
static HANDLE GLAD_API_PTR glad_debug_impl_wglDXOpenDeviceNV(void * dxDevice) {
    HANDLE ret;
    _pre_call_wgl_callback("wglDXOpenDeviceNV", (GLADapiproc) glad_wglDXOpenDeviceNV, 1, dxDevice);
    ret = glad_wglDXOpenDeviceNV(dxDevice);
    _post_call_wgl_callback((void*) &ret, "wglDXOpenDeviceNV", (GLADapiproc) glad_wglDXOpenDeviceNV, 1, dxDevice);
    return ret;
}
PFNWGLDXOPENDEVICENVPROC glad_debug_wglDXOpenDeviceNV = glad_debug_impl_wglDXOpenDeviceNV;
PFNWGLDXREGISTEROBJECTNVPROC glad_wglDXRegisterObjectNV = NULL;
static HANDLE GLAD_API_PTR glad_debug_impl_wglDXRegisterObjectNV(HANDLE hDevice, void * dxObject, GLuint name, GLenum type, GLenum access) {
    HANDLE ret;
    _pre_call_wgl_callback("wglDXRegisterObjectNV", (GLADapiproc) glad_wglDXRegisterObjectNV, 5, hDevice, dxObject, name, type, access);
    ret = glad_wglDXRegisterObjectNV(hDevice, dxObject, name, type, access);
    _post_call_wgl_callback((void*) &ret, "wglDXRegisterObjectNV", (GLADapiproc) glad_wglDXRegisterObjectNV, 5, hDevice, dxObject, name, type, access);
    return ret;
}
PFNWGLDXREGISTEROBJECTNVPROC glad_debug_wglDXRegisterObjectNV = glad_debug_impl_wglDXRegisterObjectNV;
PFNWGLDXSETRESOURCESHAREHANDLENVPROC glad_wglDXSetResourceShareHandleNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglDXSetResourceShareHandleNV(void * dxObject, HANDLE shareHandle) {
    BOOL ret;
    _pre_call_wgl_callback("wglDXSetResourceShareHandleNV", (GLADapiproc) glad_wglDXSetResourceShareHandleNV, 2, dxObject, shareHandle);
    ret = glad_wglDXSetResourceShareHandleNV(dxObject, shareHandle);
    _post_call_wgl_callback((void*) &ret, "wglDXSetResourceShareHandleNV", (GLADapiproc) glad_wglDXSetResourceShareHandleNV, 2, dxObject, shareHandle);
    return ret;
}
PFNWGLDXSETRESOURCESHAREHANDLENVPROC glad_debug_wglDXSetResourceShareHandleNV = glad_debug_impl_wglDXSetResourceShareHandleNV;
PFNWGLDXUNLOCKOBJECTSNVPROC glad_wglDXUnlockObjectsNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglDXUnlockObjectsNV(HANDLE hDevice, GLint count, HANDLE * hObjects) {
    BOOL ret;
    _pre_call_wgl_callback("wglDXUnlockObjectsNV", (GLADapiproc) glad_wglDXUnlockObjectsNV, 3, hDevice, count, hObjects);
    ret = glad_wglDXUnlockObjectsNV(hDevice, count, hObjects);
    _post_call_wgl_callback((void*) &ret, "wglDXUnlockObjectsNV", (GLADapiproc) glad_wglDXUnlockObjectsNV, 3, hDevice, count, hObjects);
    return ret;
}
PFNWGLDXUNLOCKOBJECTSNVPROC glad_debug_wglDXUnlockObjectsNV = glad_debug_impl_wglDXUnlockObjectsNV;
PFNWGLDXUNREGISTEROBJECTNVPROC glad_wglDXUnregisterObjectNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglDXUnregisterObjectNV(HANDLE hDevice, HANDLE hObject) {
    BOOL ret;
    _pre_call_wgl_callback("wglDXUnregisterObjectNV", (GLADapiproc) glad_wglDXUnregisterObjectNV, 2, hDevice, hObject);
    ret = glad_wglDXUnregisterObjectNV(hDevice, hObject);
    _post_call_wgl_callback((void*) &ret, "wglDXUnregisterObjectNV", (GLADapiproc) glad_wglDXUnregisterObjectNV, 2, hDevice, hObject);
    return ret;
}
PFNWGLDXUNREGISTEROBJECTNVPROC glad_debug_wglDXUnregisterObjectNV = glad_debug_impl_wglDXUnregisterObjectNV;
PFNWGLDELAYBEFORESWAPNVPROC glad_wglDelayBeforeSwapNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglDelayBeforeSwapNV(HDC hDC, GLfloat seconds) {
    BOOL ret;
    _pre_call_wgl_callback("wglDelayBeforeSwapNV", (GLADapiproc) glad_wglDelayBeforeSwapNV, 2, hDC, seconds);
    ret = glad_wglDelayBeforeSwapNV(hDC, seconds);
    _post_call_wgl_callback((void*) &ret, "wglDelayBeforeSwapNV", (GLADapiproc) glad_wglDelayBeforeSwapNV, 2, hDC, seconds);
    return ret;
}
PFNWGLDELAYBEFORESWAPNVPROC glad_debug_wglDelayBeforeSwapNV = glad_debug_impl_wglDelayBeforeSwapNV;
PFNWGLDELETEASSOCIATEDCONTEXTAMDPROC glad_wglDeleteAssociatedContextAMD = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglDeleteAssociatedContextAMD(HGLRC hglrc) {
    BOOL ret;
    _pre_call_wgl_callback("wglDeleteAssociatedContextAMD", (GLADapiproc) glad_wglDeleteAssociatedContextAMD, 1, hglrc);
    ret = glad_wglDeleteAssociatedContextAMD(hglrc);
    _post_call_wgl_callback((void*) &ret, "wglDeleteAssociatedContextAMD", (GLADapiproc) glad_wglDeleteAssociatedContextAMD, 1, hglrc);
    return ret;
}
PFNWGLDELETEASSOCIATEDCONTEXTAMDPROC glad_debug_wglDeleteAssociatedContextAMD = glad_debug_impl_wglDeleteAssociatedContextAMD;
PFNWGLDELETEBUFFERREGIONARBPROC glad_wglDeleteBufferRegionARB = NULL;
static VOID GLAD_API_PTR glad_debug_impl_wglDeleteBufferRegionARB(HANDLE hRegion) {
    _pre_call_wgl_callback("wglDeleteBufferRegionARB", (GLADapiproc) glad_wglDeleteBufferRegionARB, 1, hRegion);
    glad_wglDeleteBufferRegionARB(hRegion);
    _post_call_wgl_callback(NULL, "wglDeleteBufferRegionARB", (GLADapiproc) glad_wglDeleteBufferRegionARB, 1, hRegion);
    
}
PFNWGLDELETEBUFFERREGIONARBPROC glad_debug_wglDeleteBufferRegionARB = glad_debug_impl_wglDeleteBufferRegionARB;
PFNWGLDELETEDCNVPROC glad_wglDeleteDCNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglDeleteDCNV(HDC hdc) {
    BOOL ret;
    _pre_call_wgl_callback("wglDeleteDCNV", (GLADapiproc) glad_wglDeleteDCNV, 1, hdc);
    ret = glad_wglDeleteDCNV(hdc);
    _post_call_wgl_callback((void*) &ret, "wglDeleteDCNV", (GLADapiproc) glad_wglDeleteDCNV, 1, hdc);
    return ret;
}
PFNWGLDELETEDCNVPROC glad_debug_wglDeleteDCNV = glad_debug_impl_wglDeleteDCNV;
PFNWGLDESTROYDISPLAYCOLORTABLEEXTPROC glad_wglDestroyDisplayColorTableEXT = NULL;
static VOID GLAD_API_PTR glad_debug_impl_wglDestroyDisplayColorTableEXT(GLushort id) {
    _pre_call_wgl_callback("wglDestroyDisplayColorTableEXT", (GLADapiproc) glad_wglDestroyDisplayColorTableEXT, 1, id);
    glad_wglDestroyDisplayColorTableEXT(id);
    _post_call_wgl_callback(NULL, "wglDestroyDisplayColorTableEXT", (GLADapiproc) glad_wglDestroyDisplayColorTableEXT, 1, id);
    
}
PFNWGLDESTROYDISPLAYCOLORTABLEEXTPROC glad_debug_wglDestroyDisplayColorTableEXT = glad_debug_impl_wglDestroyDisplayColorTableEXT;
PFNWGLDESTROYIMAGEBUFFERI3DPROC glad_wglDestroyImageBufferI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglDestroyImageBufferI3D(HDC hDC, LPVOID pAddress) {
    BOOL ret;
    _pre_call_wgl_callback("wglDestroyImageBufferI3D", (GLADapiproc) glad_wglDestroyImageBufferI3D, 2, hDC, pAddress);
    ret = glad_wglDestroyImageBufferI3D(hDC, pAddress);
    _post_call_wgl_callback((void*) &ret, "wglDestroyImageBufferI3D", (GLADapiproc) glad_wglDestroyImageBufferI3D, 2, hDC, pAddress);
    return ret;
}
PFNWGLDESTROYIMAGEBUFFERI3DPROC glad_debug_wglDestroyImageBufferI3D = glad_debug_impl_wglDestroyImageBufferI3D;
PFNWGLDESTROYPBUFFERARBPROC glad_wglDestroyPbufferARB = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglDestroyPbufferARB(HPBUFFERARB hPbuffer) {
    BOOL ret;
    _pre_call_wgl_callback("wglDestroyPbufferARB", (GLADapiproc) glad_wglDestroyPbufferARB, 1, hPbuffer);
    ret = glad_wglDestroyPbufferARB(hPbuffer);
    _post_call_wgl_callback((void*) &ret, "wglDestroyPbufferARB", (GLADapiproc) glad_wglDestroyPbufferARB, 1, hPbuffer);
    return ret;
}
PFNWGLDESTROYPBUFFERARBPROC glad_debug_wglDestroyPbufferARB = glad_debug_impl_wglDestroyPbufferARB;
PFNWGLDESTROYPBUFFEREXTPROC glad_wglDestroyPbufferEXT = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglDestroyPbufferEXT(HPBUFFEREXT hPbuffer) {
    BOOL ret;
    _pre_call_wgl_callback("wglDestroyPbufferEXT", (GLADapiproc) glad_wglDestroyPbufferEXT, 1, hPbuffer);
    ret = glad_wglDestroyPbufferEXT(hPbuffer);
    _post_call_wgl_callback((void*) &ret, "wglDestroyPbufferEXT", (GLADapiproc) glad_wglDestroyPbufferEXT, 1, hPbuffer);
    return ret;
}
PFNWGLDESTROYPBUFFEREXTPROC glad_debug_wglDestroyPbufferEXT = glad_debug_impl_wglDestroyPbufferEXT;
PFNWGLDISABLEFRAMELOCKI3DPROC glad_wglDisableFrameLockI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglDisableFrameLockI3D(void) {
    BOOL ret;
    _pre_call_wgl_callback("wglDisableFrameLockI3D", (GLADapiproc) glad_wglDisableFrameLockI3D, 0);
    ret = glad_wglDisableFrameLockI3D();
    _post_call_wgl_callback((void*) &ret, "wglDisableFrameLockI3D", (GLADapiproc) glad_wglDisableFrameLockI3D, 0);
    return ret;
}
PFNWGLDISABLEFRAMELOCKI3DPROC glad_debug_wglDisableFrameLockI3D = glad_debug_impl_wglDisableFrameLockI3D;
PFNWGLDISABLEGENLOCKI3DPROC glad_wglDisableGenlockI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglDisableGenlockI3D(HDC hDC) {
    BOOL ret;
    _pre_call_wgl_callback("wglDisableGenlockI3D", (GLADapiproc) glad_wglDisableGenlockI3D, 1, hDC);
    ret = glad_wglDisableGenlockI3D(hDC);
    _post_call_wgl_callback((void*) &ret, "wglDisableGenlockI3D", (GLADapiproc) glad_wglDisableGenlockI3D, 1, hDC);
    return ret;
}
PFNWGLDISABLEGENLOCKI3DPROC glad_debug_wglDisableGenlockI3D = glad_debug_impl_wglDisableGenlockI3D;
PFNWGLENABLEFRAMELOCKI3DPROC glad_wglEnableFrameLockI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglEnableFrameLockI3D(void) {
    BOOL ret;
    _pre_call_wgl_callback("wglEnableFrameLockI3D", (GLADapiproc) glad_wglEnableFrameLockI3D, 0);
    ret = glad_wglEnableFrameLockI3D();
    _post_call_wgl_callback((void*) &ret, "wglEnableFrameLockI3D", (GLADapiproc) glad_wglEnableFrameLockI3D, 0);
    return ret;
}
PFNWGLENABLEFRAMELOCKI3DPROC glad_debug_wglEnableFrameLockI3D = glad_debug_impl_wglEnableFrameLockI3D;
PFNWGLENABLEGENLOCKI3DPROC glad_wglEnableGenlockI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglEnableGenlockI3D(HDC hDC) {
    BOOL ret;
    _pre_call_wgl_callback("wglEnableGenlockI3D", (GLADapiproc) glad_wglEnableGenlockI3D, 1, hDC);
    ret = glad_wglEnableGenlockI3D(hDC);
    _post_call_wgl_callback((void*) &ret, "wglEnableGenlockI3D", (GLADapiproc) glad_wglEnableGenlockI3D, 1, hDC);
    return ret;
}
PFNWGLENABLEGENLOCKI3DPROC glad_debug_wglEnableGenlockI3D = glad_debug_impl_wglEnableGenlockI3D;
PFNWGLENDFRAMETRACKINGI3DPROC glad_wglEndFrameTrackingI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglEndFrameTrackingI3D(void) {
    BOOL ret;
    _pre_call_wgl_callback("wglEndFrameTrackingI3D", (GLADapiproc) glad_wglEndFrameTrackingI3D, 0);
    ret = glad_wglEndFrameTrackingI3D();
    _post_call_wgl_callback((void*) &ret, "wglEndFrameTrackingI3D", (GLADapiproc) glad_wglEndFrameTrackingI3D, 0);
    return ret;
}
PFNWGLENDFRAMETRACKINGI3DPROC glad_debug_wglEndFrameTrackingI3D = glad_debug_impl_wglEndFrameTrackingI3D;
PFNWGLENUMGPUDEVICESNVPROC glad_wglEnumGpuDevicesNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglEnumGpuDevicesNV(HGPUNV hGpu, UINT iDeviceIndex, PGPU_DEVICE lpGpuDevice) {
    BOOL ret;
    _pre_call_wgl_callback("wglEnumGpuDevicesNV", (GLADapiproc) glad_wglEnumGpuDevicesNV, 3, hGpu, iDeviceIndex, lpGpuDevice);
    ret = glad_wglEnumGpuDevicesNV(hGpu, iDeviceIndex, lpGpuDevice);
    _post_call_wgl_callback((void*) &ret, "wglEnumGpuDevicesNV", (GLADapiproc) glad_wglEnumGpuDevicesNV, 3, hGpu, iDeviceIndex, lpGpuDevice);
    return ret;
}
PFNWGLENUMGPUDEVICESNVPROC glad_debug_wglEnumGpuDevicesNV = glad_debug_impl_wglEnumGpuDevicesNV;
PFNWGLENUMGPUSFROMAFFINITYDCNVPROC glad_wglEnumGpusFromAffinityDCNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglEnumGpusFromAffinityDCNV(HDC hAffinityDC, UINT iGpuIndex, HGPUNV * hGpu) {
    BOOL ret;
    _pre_call_wgl_callback("wglEnumGpusFromAffinityDCNV", (GLADapiproc) glad_wglEnumGpusFromAffinityDCNV, 3, hAffinityDC, iGpuIndex, hGpu);
    ret = glad_wglEnumGpusFromAffinityDCNV(hAffinityDC, iGpuIndex, hGpu);
    _post_call_wgl_callback((void*) &ret, "wglEnumGpusFromAffinityDCNV", (GLADapiproc) glad_wglEnumGpusFromAffinityDCNV, 3, hAffinityDC, iGpuIndex, hGpu);
    return ret;
}
PFNWGLENUMGPUSFROMAFFINITYDCNVPROC glad_debug_wglEnumGpusFromAffinityDCNV = glad_debug_impl_wglEnumGpusFromAffinityDCNV;
PFNWGLENUMGPUSNVPROC glad_wglEnumGpusNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglEnumGpusNV(UINT iGpuIndex, HGPUNV * phGpu) {
    BOOL ret;
    _pre_call_wgl_callback("wglEnumGpusNV", (GLADapiproc) glad_wglEnumGpusNV, 2, iGpuIndex, phGpu);
    ret = glad_wglEnumGpusNV(iGpuIndex, phGpu);
    _post_call_wgl_callback((void*) &ret, "wglEnumGpusNV", (GLADapiproc) glad_wglEnumGpusNV, 2, iGpuIndex, phGpu);
    return ret;
}
PFNWGLENUMGPUSNVPROC glad_debug_wglEnumGpusNV = glad_debug_impl_wglEnumGpusNV;
PFNWGLENUMERATEVIDEOCAPTUREDEVICESNVPROC glad_wglEnumerateVideoCaptureDevicesNV = NULL;
static UINT GLAD_API_PTR glad_debug_impl_wglEnumerateVideoCaptureDevicesNV(HDC hDc, HVIDEOINPUTDEVICENV * phDeviceList) {
    UINT ret;
    _pre_call_wgl_callback("wglEnumerateVideoCaptureDevicesNV", (GLADapiproc) glad_wglEnumerateVideoCaptureDevicesNV, 2, hDc, phDeviceList);
    ret = glad_wglEnumerateVideoCaptureDevicesNV(hDc, phDeviceList);
    _post_call_wgl_callback((void*) &ret, "wglEnumerateVideoCaptureDevicesNV", (GLADapiproc) glad_wglEnumerateVideoCaptureDevicesNV, 2, hDc, phDeviceList);
    return ret;
}
PFNWGLENUMERATEVIDEOCAPTUREDEVICESNVPROC glad_debug_wglEnumerateVideoCaptureDevicesNV = glad_debug_impl_wglEnumerateVideoCaptureDevicesNV;
PFNWGLENUMERATEVIDEODEVICESNVPROC glad_wglEnumerateVideoDevicesNV = NULL;
static int GLAD_API_PTR glad_debug_impl_wglEnumerateVideoDevicesNV(HDC hDc, HVIDEOOUTPUTDEVICENV * phDeviceList) {
    int ret;
    _pre_call_wgl_callback("wglEnumerateVideoDevicesNV", (GLADapiproc) glad_wglEnumerateVideoDevicesNV, 2, hDc, phDeviceList);
    ret = glad_wglEnumerateVideoDevicesNV(hDc, phDeviceList);
    _post_call_wgl_callback((void*) &ret, "wglEnumerateVideoDevicesNV", (GLADapiproc) glad_wglEnumerateVideoDevicesNV, 2, hDc, phDeviceList);
    return ret;
}
PFNWGLENUMERATEVIDEODEVICESNVPROC glad_debug_wglEnumerateVideoDevicesNV = glad_debug_impl_wglEnumerateVideoDevicesNV;
PFNWGLFREEMEMORYNVPROC glad_wglFreeMemoryNV = NULL;
static void GLAD_API_PTR glad_debug_impl_wglFreeMemoryNV(void * pointer) {
    _pre_call_wgl_callback("wglFreeMemoryNV", (GLADapiproc) glad_wglFreeMemoryNV, 1, pointer);
    glad_wglFreeMemoryNV(pointer);
    _post_call_wgl_callback(NULL, "wglFreeMemoryNV", (GLADapiproc) glad_wglFreeMemoryNV, 1, pointer);
    
}
PFNWGLFREEMEMORYNVPROC glad_debug_wglFreeMemoryNV = glad_debug_impl_wglFreeMemoryNV;
PFNWGLGENLOCKSAMPLERATEI3DPROC glad_wglGenlockSampleRateI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglGenlockSampleRateI3D(HDC hDC, UINT uRate) {
    BOOL ret;
    _pre_call_wgl_callback("wglGenlockSampleRateI3D", (GLADapiproc) glad_wglGenlockSampleRateI3D, 2, hDC, uRate);
    ret = glad_wglGenlockSampleRateI3D(hDC, uRate);
    _post_call_wgl_callback((void*) &ret, "wglGenlockSampleRateI3D", (GLADapiproc) glad_wglGenlockSampleRateI3D, 2, hDC, uRate);
    return ret;
}
PFNWGLGENLOCKSAMPLERATEI3DPROC glad_debug_wglGenlockSampleRateI3D = glad_debug_impl_wglGenlockSampleRateI3D;
PFNWGLGENLOCKSOURCEDELAYI3DPROC glad_wglGenlockSourceDelayI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglGenlockSourceDelayI3D(HDC hDC, UINT uDelay) {
    BOOL ret;
    _pre_call_wgl_callback("wglGenlockSourceDelayI3D", (GLADapiproc) glad_wglGenlockSourceDelayI3D, 2, hDC, uDelay);
    ret = glad_wglGenlockSourceDelayI3D(hDC, uDelay);
    _post_call_wgl_callback((void*) &ret, "wglGenlockSourceDelayI3D", (GLADapiproc) glad_wglGenlockSourceDelayI3D, 2, hDC, uDelay);
    return ret;
}
PFNWGLGENLOCKSOURCEDELAYI3DPROC glad_debug_wglGenlockSourceDelayI3D = glad_debug_impl_wglGenlockSourceDelayI3D;
PFNWGLGENLOCKSOURCEEDGEI3DPROC glad_wglGenlockSourceEdgeI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglGenlockSourceEdgeI3D(HDC hDC, UINT uEdge) {
    BOOL ret;
    _pre_call_wgl_callback("wglGenlockSourceEdgeI3D", (GLADapiproc) glad_wglGenlockSourceEdgeI3D, 2, hDC, uEdge);
    ret = glad_wglGenlockSourceEdgeI3D(hDC, uEdge);
    _post_call_wgl_callback((void*) &ret, "wglGenlockSourceEdgeI3D", (GLADapiproc) glad_wglGenlockSourceEdgeI3D, 2, hDC, uEdge);
    return ret;
}
PFNWGLGENLOCKSOURCEEDGEI3DPROC glad_debug_wglGenlockSourceEdgeI3D = glad_debug_impl_wglGenlockSourceEdgeI3D;
PFNWGLGENLOCKSOURCEI3DPROC glad_wglGenlockSourceI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglGenlockSourceI3D(HDC hDC, UINT uSource) {
    BOOL ret;
    _pre_call_wgl_callback("wglGenlockSourceI3D", (GLADapiproc) glad_wglGenlockSourceI3D, 2, hDC, uSource);
    ret = glad_wglGenlockSourceI3D(hDC, uSource);
    _post_call_wgl_callback((void*) &ret, "wglGenlockSourceI3D", (GLADapiproc) glad_wglGenlockSourceI3D, 2, hDC, uSource);
    return ret;
}
PFNWGLGENLOCKSOURCEI3DPROC glad_debug_wglGenlockSourceI3D = glad_debug_impl_wglGenlockSourceI3D;
PFNWGLGETCONTEXTGPUIDAMDPROC glad_wglGetContextGPUIDAMD = NULL;
static UINT GLAD_API_PTR glad_debug_impl_wglGetContextGPUIDAMD(HGLRC hglrc) {
    UINT ret;
    _pre_call_wgl_callback("wglGetContextGPUIDAMD", (GLADapiproc) glad_wglGetContextGPUIDAMD, 1, hglrc);
    ret = glad_wglGetContextGPUIDAMD(hglrc);
    _post_call_wgl_callback((void*) &ret, "wglGetContextGPUIDAMD", (GLADapiproc) glad_wglGetContextGPUIDAMD, 1, hglrc);
    return ret;
}
PFNWGLGETCONTEXTGPUIDAMDPROC glad_debug_wglGetContextGPUIDAMD = glad_debug_impl_wglGetContextGPUIDAMD;
PFNWGLGETCURRENTASSOCIATEDCONTEXTAMDPROC glad_wglGetCurrentAssociatedContextAMD = NULL;
static HGLRC GLAD_API_PTR glad_debug_impl_wglGetCurrentAssociatedContextAMD(void) {
    HGLRC ret;
    _pre_call_wgl_callback("wglGetCurrentAssociatedContextAMD", (GLADapiproc) glad_wglGetCurrentAssociatedContextAMD, 0);
    ret = glad_wglGetCurrentAssociatedContextAMD();
    _post_call_wgl_callback((void*) &ret, "wglGetCurrentAssociatedContextAMD", (GLADapiproc) glad_wglGetCurrentAssociatedContextAMD, 0);
    return ret;
}
PFNWGLGETCURRENTASSOCIATEDCONTEXTAMDPROC glad_debug_wglGetCurrentAssociatedContextAMD = glad_debug_impl_wglGetCurrentAssociatedContextAMD;
PFNWGLGETCURRENTREADDCARBPROC glad_wglGetCurrentReadDCARB = NULL;
static HDC GLAD_API_PTR glad_debug_impl_wglGetCurrentReadDCARB(void) {
    HDC ret;
    _pre_call_wgl_callback("wglGetCurrentReadDCARB", (GLADapiproc) glad_wglGetCurrentReadDCARB, 0);
    ret = glad_wglGetCurrentReadDCARB();
    _post_call_wgl_callback((void*) &ret, "wglGetCurrentReadDCARB", (GLADapiproc) glad_wglGetCurrentReadDCARB, 0);
    return ret;
}
PFNWGLGETCURRENTREADDCARBPROC glad_debug_wglGetCurrentReadDCARB = glad_debug_impl_wglGetCurrentReadDCARB;
PFNWGLGETCURRENTREADDCEXTPROC glad_wglGetCurrentReadDCEXT = NULL;
static HDC GLAD_API_PTR glad_debug_impl_wglGetCurrentReadDCEXT(void) {
    HDC ret;
    _pre_call_wgl_callback("wglGetCurrentReadDCEXT", (GLADapiproc) glad_wglGetCurrentReadDCEXT, 0);
    ret = glad_wglGetCurrentReadDCEXT();
    _post_call_wgl_callback((void*) &ret, "wglGetCurrentReadDCEXT", (GLADapiproc) glad_wglGetCurrentReadDCEXT, 0);
    return ret;
}
PFNWGLGETCURRENTREADDCEXTPROC glad_debug_wglGetCurrentReadDCEXT = glad_debug_impl_wglGetCurrentReadDCEXT;
PFNWGLGETDIGITALVIDEOPARAMETERSI3DPROC glad_wglGetDigitalVideoParametersI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglGetDigitalVideoParametersI3D(HDC hDC, int iAttribute, int * piValue) {
    BOOL ret;
    _pre_call_wgl_callback("wglGetDigitalVideoParametersI3D", (GLADapiproc) glad_wglGetDigitalVideoParametersI3D, 3, hDC, iAttribute, piValue);
    ret = glad_wglGetDigitalVideoParametersI3D(hDC, iAttribute, piValue);
    _post_call_wgl_callback((void*) &ret, "wglGetDigitalVideoParametersI3D", (GLADapiproc) glad_wglGetDigitalVideoParametersI3D, 3, hDC, iAttribute, piValue);
    return ret;
}
PFNWGLGETDIGITALVIDEOPARAMETERSI3DPROC glad_debug_wglGetDigitalVideoParametersI3D = glad_debug_impl_wglGetDigitalVideoParametersI3D;
PFNWGLGETEXTENSIONSSTRINGARBPROC glad_wglGetExtensionsStringARB = NULL;
static const char * GLAD_API_PTR glad_debug_impl_wglGetExtensionsStringARB(HDC hdc) {
    const char * ret;
    _pre_call_wgl_callback("wglGetExtensionsStringARB", (GLADapiproc) glad_wglGetExtensionsStringARB, 1, hdc);
    ret = glad_wglGetExtensionsStringARB(hdc);
    _post_call_wgl_callback((void*) &ret, "wglGetExtensionsStringARB", (GLADapiproc) glad_wglGetExtensionsStringARB, 1, hdc);
    return ret;
}
PFNWGLGETEXTENSIONSSTRINGARBPROC glad_debug_wglGetExtensionsStringARB = glad_debug_impl_wglGetExtensionsStringARB;
PFNWGLGETEXTENSIONSSTRINGEXTPROC glad_wglGetExtensionsStringEXT = NULL;
static const char * GLAD_API_PTR glad_debug_impl_wglGetExtensionsStringEXT(void) {
    const char * ret;
    _pre_call_wgl_callback("wglGetExtensionsStringEXT", (GLADapiproc) glad_wglGetExtensionsStringEXT, 0);
    ret = glad_wglGetExtensionsStringEXT();
    _post_call_wgl_callback((void*) &ret, "wglGetExtensionsStringEXT", (GLADapiproc) glad_wglGetExtensionsStringEXT, 0);
    return ret;
}
PFNWGLGETEXTENSIONSSTRINGEXTPROC glad_debug_wglGetExtensionsStringEXT = glad_debug_impl_wglGetExtensionsStringEXT;
PFNWGLGETFRAMEUSAGEI3DPROC glad_wglGetFrameUsageI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglGetFrameUsageI3D(float * pUsage) {
    BOOL ret;
    _pre_call_wgl_callback("wglGetFrameUsageI3D", (GLADapiproc) glad_wglGetFrameUsageI3D, 1, pUsage);
    ret = glad_wglGetFrameUsageI3D(pUsage);
    _post_call_wgl_callback((void*) &ret, "wglGetFrameUsageI3D", (GLADapiproc) glad_wglGetFrameUsageI3D, 1, pUsage);
    return ret;
}
PFNWGLGETFRAMEUSAGEI3DPROC glad_debug_wglGetFrameUsageI3D = glad_debug_impl_wglGetFrameUsageI3D;
PFNWGLGETGPUIDSAMDPROC glad_wglGetGPUIDsAMD = NULL;
static UINT GLAD_API_PTR glad_debug_impl_wglGetGPUIDsAMD(UINT maxCount, UINT * ids) {
    UINT ret;
    _pre_call_wgl_callback("wglGetGPUIDsAMD", (GLADapiproc) glad_wglGetGPUIDsAMD, 2, maxCount, ids);
    ret = glad_wglGetGPUIDsAMD(maxCount, ids);
    _post_call_wgl_callback((void*) &ret, "wglGetGPUIDsAMD", (GLADapiproc) glad_wglGetGPUIDsAMD, 2, maxCount, ids);
    return ret;
}
PFNWGLGETGPUIDSAMDPROC glad_debug_wglGetGPUIDsAMD = glad_debug_impl_wglGetGPUIDsAMD;
PFNWGLGETGPUINFOAMDPROC glad_wglGetGPUInfoAMD = NULL;
static INT GLAD_API_PTR glad_debug_impl_wglGetGPUInfoAMD(UINT id, INT property, GLenum dataType, UINT size, void * data) {
    INT ret;
    _pre_call_wgl_callback("wglGetGPUInfoAMD", (GLADapiproc) glad_wglGetGPUInfoAMD, 5, id, property, dataType, size, data);
    ret = glad_wglGetGPUInfoAMD(id, property, dataType, size, data);
    _post_call_wgl_callback((void*) &ret, "wglGetGPUInfoAMD", (GLADapiproc) glad_wglGetGPUInfoAMD, 5, id, property, dataType, size, data);
    return ret;
}
PFNWGLGETGPUINFOAMDPROC glad_debug_wglGetGPUInfoAMD = glad_debug_impl_wglGetGPUInfoAMD;
PFNWGLGETGAMMATABLEI3DPROC glad_wglGetGammaTableI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglGetGammaTableI3D(HDC hDC, int iEntries, USHORT * puRed, USHORT * puGreen, USHORT * puBlue) {
    BOOL ret;
    _pre_call_wgl_callback("wglGetGammaTableI3D", (GLADapiproc) glad_wglGetGammaTableI3D, 5, hDC, iEntries, puRed, puGreen, puBlue);
    ret = glad_wglGetGammaTableI3D(hDC, iEntries, puRed, puGreen, puBlue);
    _post_call_wgl_callback((void*) &ret, "wglGetGammaTableI3D", (GLADapiproc) glad_wglGetGammaTableI3D, 5, hDC, iEntries, puRed, puGreen, puBlue);
    return ret;
}
PFNWGLGETGAMMATABLEI3DPROC glad_debug_wglGetGammaTableI3D = glad_debug_impl_wglGetGammaTableI3D;
PFNWGLGETGAMMATABLEPARAMETERSI3DPROC glad_wglGetGammaTableParametersI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglGetGammaTableParametersI3D(HDC hDC, int iAttribute, int * piValue) {
    BOOL ret;
    _pre_call_wgl_callback("wglGetGammaTableParametersI3D", (GLADapiproc) glad_wglGetGammaTableParametersI3D, 3, hDC, iAttribute, piValue);
    ret = glad_wglGetGammaTableParametersI3D(hDC, iAttribute, piValue);
    _post_call_wgl_callback((void*) &ret, "wglGetGammaTableParametersI3D", (GLADapiproc) glad_wglGetGammaTableParametersI3D, 3, hDC, iAttribute, piValue);
    return ret;
}
PFNWGLGETGAMMATABLEPARAMETERSI3DPROC glad_debug_wglGetGammaTableParametersI3D = glad_debug_impl_wglGetGammaTableParametersI3D;
PFNWGLGETGENLOCKSAMPLERATEI3DPROC glad_wglGetGenlockSampleRateI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglGetGenlockSampleRateI3D(HDC hDC, UINT * uRate) {
    BOOL ret;
    _pre_call_wgl_callback("wglGetGenlockSampleRateI3D", (GLADapiproc) glad_wglGetGenlockSampleRateI3D, 2, hDC, uRate);
    ret = glad_wglGetGenlockSampleRateI3D(hDC, uRate);
    _post_call_wgl_callback((void*) &ret, "wglGetGenlockSampleRateI3D", (GLADapiproc) glad_wglGetGenlockSampleRateI3D, 2, hDC, uRate);
    return ret;
}
PFNWGLGETGENLOCKSAMPLERATEI3DPROC glad_debug_wglGetGenlockSampleRateI3D = glad_debug_impl_wglGetGenlockSampleRateI3D;
PFNWGLGETGENLOCKSOURCEDELAYI3DPROC glad_wglGetGenlockSourceDelayI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglGetGenlockSourceDelayI3D(HDC hDC, UINT * uDelay) {
    BOOL ret;
    _pre_call_wgl_callback("wglGetGenlockSourceDelayI3D", (GLADapiproc) glad_wglGetGenlockSourceDelayI3D, 2, hDC, uDelay);
    ret = glad_wglGetGenlockSourceDelayI3D(hDC, uDelay);
    _post_call_wgl_callback((void*) &ret, "wglGetGenlockSourceDelayI3D", (GLADapiproc) glad_wglGetGenlockSourceDelayI3D, 2, hDC, uDelay);
    return ret;
}
PFNWGLGETGENLOCKSOURCEDELAYI3DPROC glad_debug_wglGetGenlockSourceDelayI3D = glad_debug_impl_wglGetGenlockSourceDelayI3D;
PFNWGLGETGENLOCKSOURCEEDGEI3DPROC glad_wglGetGenlockSourceEdgeI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglGetGenlockSourceEdgeI3D(HDC hDC, UINT * uEdge) {
    BOOL ret;
    _pre_call_wgl_callback("wglGetGenlockSourceEdgeI3D", (GLADapiproc) glad_wglGetGenlockSourceEdgeI3D, 2, hDC, uEdge);
    ret = glad_wglGetGenlockSourceEdgeI3D(hDC, uEdge);
    _post_call_wgl_callback((void*) &ret, "wglGetGenlockSourceEdgeI3D", (GLADapiproc) glad_wglGetGenlockSourceEdgeI3D, 2, hDC, uEdge);
    return ret;
}
PFNWGLGETGENLOCKSOURCEEDGEI3DPROC glad_debug_wglGetGenlockSourceEdgeI3D = glad_debug_impl_wglGetGenlockSourceEdgeI3D;
PFNWGLGETGENLOCKSOURCEI3DPROC glad_wglGetGenlockSourceI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglGetGenlockSourceI3D(HDC hDC, UINT * uSource) {
    BOOL ret;
    _pre_call_wgl_callback("wglGetGenlockSourceI3D", (GLADapiproc) glad_wglGetGenlockSourceI3D, 2, hDC, uSource);
    ret = glad_wglGetGenlockSourceI3D(hDC, uSource);
    _post_call_wgl_callback((void*) &ret, "wglGetGenlockSourceI3D", (GLADapiproc) glad_wglGetGenlockSourceI3D, 2, hDC, uSource);
    return ret;
}
PFNWGLGETGENLOCKSOURCEI3DPROC glad_debug_wglGetGenlockSourceI3D = glad_debug_impl_wglGetGenlockSourceI3D;
PFNWGLGETMSCRATEOMLPROC glad_wglGetMscRateOML = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglGetMscRateOML(HDC hdc, INT32 * numerator, INT32 * denominator) {
    BOOL ret;
    _pre_call_wgl_callback("wglGetMscRateOML", (GLADapiproc) glad_wglGetMscRateOML, 3, hdc, numerator, denominator);
    ret = glad_wglGetMscRateOML(hdc, numerator, denominator);
    _post_call_wgl_callback((void*) &ret, "wglGetMscRateOML", (GLADapiproc) glad_wglGetMscRateOML, 3, hdc, numerator, denominator);
    return ret;
}
PFNWGLGETMSCRATEOMLPROC glad_debug_wglGetMscRateOML = glad_debug_impl_wglGetMscRateOML;
PFNWGLGETPBUFFERDCARBPROC glad_wglGetPbufferDCARB = NULL;
static HDC GLAD_API_PTR glad_debug_impl_wglGetPbufferDCARB(HPBUFFERARB hPbuffer) {
    HDC ret;
    _pre_call_wgl_callback("wglGetPbufferDCARB", (GLADapiproc) glad_wglGetPbufferDCARB, 1, hPbuffer);
    ret = glad_wglGetPbufferDCARB(hPbuffer);
    _post_call_wgl_callback((void*) &ret, "wglGetPbufferDCARB", (GLADapiproc) glad_wglGetPbufferDCARB, 1, hPbuffer);
    return ret;
}
PFNWGLGETPBUFFERDCARBPROC glad_debug_wglGetPbufferDCARB = glad_debug_impl_wglGetPbufferDCARB;
PFNWGLGETPBUFFERDCEXTPROC glad_wglGetPbufferDCEXT = NULL;
static HDC GLAD_API_PTR glad_debug_impl_wglGetPbufferDCEXT(HPBUFFEREXT hPbuffer) {
    HDC ret;
    _pre_call_wgl_callback("wglGetPbufferDCEXT", (GLADapiproc) glad_wglGetPbufferDCEXT, 1, hPbuffer);
    ret = glad_wglGetPbufferDCEXT(hPbuffer);
    _post_call_wgl_callback((void*) &ret, "wglGetPbufferDCEXT", (GLADapiproc) glad_wglGetPbufferDCEXT, 1, hPbuffer);
    return ret;
}
PFNWGLGETPBUFFERDCEXTPROC glad_debug_wglGetPbufferDCEXT = glad_debug_impl_wglGetPbufferDCEXT;
PFNWGLGETPIXELFORMATATTRIBFVARBPROC glad_wglGetPixelFormatAttribfvARB = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglGetPixelFormatAttribfvARB(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int * piAttributes, FLOAT * pfValues) {
    BOOL ret;
    _pre_call_wgl_callback("wglGetPixelFormatAttribfvARB", (GLADapiproc) glad_wglGetPixelFormatAttribfvARB, 6, hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, pfValues);
    ret = glad_wglGetPixelFormatAttribfvARB(hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, pfValues);
    _post_call_wgl_callback((void*) &ret, "wglGetPixelFormatAttribfvARB", (GLADapiproc) glad_wglGetPixelFormatAttribfvARB, 6, hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, pfValues);
    return ret;
}
PFNWGLGETPIXELFORMATATTRIBFVARBPROC glad_debug_wglGetPixelFormatAttribfvARB = glad_debug_impl_wglGetPixelFormatAttribfvARB;
PFNWGLGETPIXELFORMATATTRIBFVEXTPROC glad_wglGetPixelFormatAttribfvEXT = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglGetPixelFormatAttribfvEXT(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int * piAttributes, FLOAT * pfValues) {
    BOOL ret;
    _pre_call_wgl_callback("wglGetPixelFormatAttribfvEXT", (GLADapiproc) glad_wglGetPixelFormatAttribfvEXT, 6, hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, pfValues);
    ret = glad_wglGetPixelFormatAttribfvEXT(hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, pfValues);
    _post_call_wgl_callback((void*) &ret, "wglGetPixelFormatAttribfvEXT", (GLADapiproc) glad_wglGetPixelFormatAttribfvEXT, 6, hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, pfValues);
    return ret;
}
PFNWGLGETPIXELFORMATATTRIBFVEXTPROC glad_debug_wglGetPixelFormatAttribfvEXT = glad_debug_impl_wglGetPixelFormatAttribfvEXT;
PFNWGLGETPIXELFORMATATTRIBIVARBPROC glad_wglGetPixelFormatAttribivARB = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglGetPixelFormatAttribivARB(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int * piAttributes, int * piValues) {
    BOOL ret;
    _pre_call_wgl_callback("wglGetPixelFormatAttribivARB", (GLADapiproc) glad_wglGetPixelFormatAttribivARB, 6, hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, piValues);
    ret = glad_wglGetPixelFormatAttribivARB(hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, piValues);
    _post_call_wgl_callback((void*) &ret, "wglGetPixelFormatAttribivARB", (GLADapiproc) glad_wglGetPixelFormatAttribivARB, 6, hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, piValues);
    return ret;
}
PFNWGLGETPIXELFORMATATTRIBIVARBPROC glad_debug_wglGetPixelFormatAttribivARB = glad_debug_impl_wglGetPixelFormatAttribivARB;
PFNWGLGETPIXELFORMATATTRIBIVEXTPROC glad_wglGetPixelFormatAttribivEXT = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglGetPixelFormatAttribivEXT(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int * piAttributes, int * piValues) {
    BOOL ret;
    _pre_call_wgl_callback("wglGetPixelFormatAttribivEXT", (GLADapiproc) glad_wglGetPixelFormatAttribivEXT, 6, hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, piValues);
    ret = glad_wglGetPixelFormatAttribivEXT(hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, piValues);
    _post_call_wgl_callback((void*) &ret, "wglGetPixelFormatAttribivEXT", (GLADapiproc) glad_wglGetPixelFormatAttribivEXT, 6, hdc, iPixelFormat, iLayerPlane, nAttributes, piAttributes, piValues);
    return ret;
}
PFNWGLGETPIXELFORMATATTRIBIVEXTPROC glad_debug_wglGetPixelFormatAttribivEXT = glad_debug_impl_wglGetPixelFormatAttribivEXT;
PFNWGLGETSWAPINTERVALEXTPROC glad_wglGetSwapIntervalEXT = NULL;
static int GLAD_API_PTR glad_debug_impl_wglGetSwapIntervalEXT(void) {
    int ret;
    _pre_call_wgl_callback("wglGetSwapIntervalEXT", (GLADapiproc) glad_wglGetSwapIntervalEXT, 0);
    ret = glad_wglGetSwapIntervalEXT();
    _post_call_wgl_callback((void*) &ret, "wglGetSwapIntervalEXT", (GLADapiproc) glad_wglGetSwapIntervalEXT, 0);
    return ret;
}
PFNWGLGETSWAPINTERVALEXTPROC glad_debug_wglGetSwapIntervalEXT = glad_debug_impl_wglGetSwapIntervalEXT;
PFNWGLGETSYNCVALUESOMLPROC glad_wglGetSyncValuesOML = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglGetSyncValuesOML(HDC hdc, INT64 * ust, INT64 * msc, INT64 * sbc) {
    BOOL ret;
    _pre_call_wgl_callback("wglGetSyncValuesOML", (GLADapiproc) glad_wglGetSyncValuesOML, 4, hdc, ust, msc, sbc);
    ret = glad_wglGetSyncValuesOML(hdc, ust, msc, sbc);
    _post_call_wgl_callback((void*) &ret, "wglGetSyncValuesOML", (GLADapiproc) glad_wglGetSyncValuesOML, 4, hdc, ust, msc, sbc);
    return ret;
}
PFNWGLGETSYNCVALUESOMLPROC glad_debug_wglGetSyncValuesOML = glad_debug_impl_wglGetSyncValuesOML;
PFNWGLGETVIDEODEVICENVPROC glad_wglGetVideoDeviceNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglGetVideoDeviceNV(HDC hDC, int numDevices, HPVIDEODEV * hVideoDevice) {
    BOOL ret;
    _pre_call_wgl_callback("wglGetVideoDeviceNV", (GLADapiproc) glad_wglGetVideoDeviceNV, 3, hDC, numDevices, hVideoDevice);
    ret = glad_wglGetVideoDeviceNV(hDC, numDevices, hVideoDevice);
    _post_call_wgl_callback((void*) &ret, "wglGetVideoDeviceNV", (GLADapiproc) glad_wglGetVideoDeviceNV, 3, hDC, numDevices, hVideoDevice);
    return ret;
}
PFNWGLGETVIDEODEVICENVPROC glad_debug_wglGetVideoDeviceNV = glad_debug_impl_wglGetVideoDeviceNV;
PFNWGLGETVIDEOINFONVPROC glad_wglGetVideoInfoNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglGetVideoInfoNV(HPVIDEODEV hpVideoDevice, unsigned long * pulCounterOutputPbuffer, unsigned long * pulCounterOutputVideo) {
    BOOL ret;
    _pre_call_wgl_callback("wglGetVideoInfoNV", (GLADapiproc) glad_wglGetVideoInfoNV, 3, hpVideoDevice, pulCounterOutputPbuffer, pulCounterOutputVideo);
    ret = glad_wglGetVideoInfoNV(hpVideoDevice, pulCounterOutputPbuffer, pulCounterOutputVideo);
    _post_call_wgl_callback((void*) &ret, "wglGetVideoInfoNV", (GLADapiproc) glad_wglGetVideoInfoNV, 3, hpVideoDevice, pulCounterOutputPbuffer, pulCounterOutputVideo);
    return ret;
}
PFNWGLGETVIDEOINFONVPROC glad_debug_wglGetVideoInfoNV = glad_debug_impl_wglGetVideoInfoNV;
PFNWGLISENABLEDFRAMELOCKI3DPROC glad_wglIsEnabledFrameLockI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglIsEnabledFrameLockI3D(BOOL * pFlag) {
    BOOL ret;
    _pre_call_wgl_callback("wglIsEnabledFrameLockI3D", (GLADapiproc) glad_wglIsEnabledFrameLockI3D, 1, pFlag);
    ret = glad_wglIsEnabledFrameLockI3D(pFlag);
    _post_call_wgl_callback((void*) &ret, "wglIsEnabledFrameLockI3D", (GLADapiproc) glad_wglIsEnabledFrameLockI3D, 1, pFlag);
    return ret;
}
PFNWGLISENABLEDFRAMELOCKI3DPROC glad_debug_wglIsEnabledFrameLockI3D = glad_debug_impl_wglIsEnabledFrameLockI3D;
PFNWGLISENABLEDGENLOCKI3DPROC glad_wglIsEnabledGenlockI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglIsEnabledGenlockI3D(HDC hDC, BOOL * pFlag) {
    BOOL ret;
    _pre_call_wgl_callback("wglIsEnabledGenlockI3D", (GLADapiproc) glad_wglIsEnabledGenlockI3D, 2, hDC, pFlag);
    ret = glad_wglIsEnabledGenlockI3D(hDC, pFlag);
    _post_call_wgl_callback((void*) &ret, "wglIsEnabledGenlockI3D", (GLADapiproc) glad_wglIsEnabledGenlockI3D, 2, hDC, pFlag);
    return ret;
}
PFNWGLISENABLEDGENLOCKI3DPROC glad_debug_wglIsEnabledGenlockI3D = glad_debug_impl_wglIsEnabledGenlockI3D;
PFNWGLJOINSWAPGROUPNVPROC glad_wglJoinSwapGroupNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglJoinSwapGroupNV(HDC hDC, GLuint group) {
    BOOL ret;
    _pre_call_wgl_callback("wglJoinSwapGroupNV", (GLADapiproc) glad_wglJoinSwapGroupNV, 2, hDC, group);
    ret = glad_wglJoinSwapGroupNV(hDC, group);
    _post_call_wgl_callback((void*) &ret, "wglJoinSwapGroupNV", (GLADapiproc) glad_wglJoinSwapGroupNV, 2, hDC, group);
    return ret;
}
PFNWGLJOINSWAPGROUPNVPROC glad_debug_wglJoinSwapGroupNV = glad_debug_impl_wglJoinSwapGroupNV;
PFNWGLLOADDISPLAYCOLORTABLEEXTPROC glad_wglLoadDisplayColorTableEXT = NULL;
static GLboolean GLAD_API_PTR glad_debug_impl_wglLoadDisplayColorTableEXT(const GLushort * table, GLuint length) {
    GLboolean ret;
    _pre_call_wgl_callback("wglLoadDisplayColorTableEXT", (GLADapiproc) glad_wglLoadDisplayColorTableEXT, 2, table, length);
    ret = glad_wglLoadDisplayColorTableEXT(table, length);
    _post_call_wgl_callback((void*) &ret, "wglLoadDisplayColorTableEXT", (GLADapiproc) glad_wglLoadDisplayColorTableEXT, 2, table, length);
    return ret;
}
PFNWGLLOADDISPLAYCOLORTABLEEXTPROC glad_debug_wglLoadDisplayColorTableEXT = glad_debug_impl_wglLoadDisplayColorTableEXT;
PFNWGLLOCKVIDEOCAPTUREDEVICENVPROC glad_wglLockVideoCaptureDeviceNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglLockVideoCaptureDeviceNV(HDC hDc, HVIDEOINPUTDEVICENV hDevice) {
    BOOL ret;
    _pre_call_wgl_callback("wglLockVideoCaptureDeviceNV", (GLADapiproc) glad_wglLockVideoCaptureDeviceNV, 2, hDc, hDevice);
    ret = glad_wglLockVideoCaptureDeviceNV(hDc, hDevice);
    _post_call_wgl_callback((void*) &ret, "wglLockVideoCaptureDeviceNV", (GLADapiproc) glad_wglLockVideoCaptureDeviceNV, 2, hDc, hDevice);
    return ret;
}
PFNWGLLOCKVIDEOCAPTUREDEVICENVPROC glad_debug_wglLockVideoCaptureDeviceNV = glad_debug_impl_wglLockVideoCaptureDeviceNV;
PFNWGLMAKEASSOCIATEDCONTEXTCURRENTAMDPROC glad_wglMakeAssociatedContextCurrentAMD = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglMakeAssociatedContextCurrentAMD(HGLRC hglrc) {
    BOOL ret;
    _pre_call_wgl_callback("wglMakeAssociatedContextCurrentAMD", (GLADapiproc) glad_wglMakeAssociatedContextCurrentAMD, 1, hglrc);
    ret = glad_wglMakeAssociatedContextCurrentAMD(hglrc);
    _post_call_wgl_callback((void*) &ret, "wglMakeAssociatedContextCurrentAMD", (GLADapiproc) glad_wglMakeAssociatedContextCurrentAMD, 1, hglrc);
    return ret;
}
PFNWGLMAKEASSOCIATEDCONTEXTCURRENTAMDPROC glad_debug_wglMakeAssociatedContextCurrentAMD = glad_debug_impl_wglMakeAssociatedContextCurrentAMD;
PFNWGLMAKECONTEXTCURRENTARBPROC glad_wglMakeContextCurrentARB = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglMakeContextCurrentARB(HDC hDrawDC, HDC hReadDC, HGLRC hglrc) {
    BOOL ret;
    _pre_call_wgl_callback("wglMakeContextCurrentARB", (GLADapiproc) glad_wglMakeContextCurrentARB, 3, hDrawDC, hReadDC, hglrc);
    ret = glad_wglMakeContextCurrentARB(hDrawDC, hReadDC, hglrc);
    _post_call_wgl_callback((void*) &ret, "wglMakeContextCurrentARB", (GLADapiproc) glad_wglMakeContextCurrentARB, 3, hDrawDC, hReadDC, hglrc);
    return ret;
}
PFNWGLMAKECONTEXTCURRENTARBPROC glad_debug_wglMakeContextCurrentARB = glad_debug_impl_wglMakeContextCurrentARB;
PFNWGLMAKECONTEXTCURRENTEXTPROC glad_wglMakeContextCurrentEXT = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglMakeContextCurrentEXT(HDC hDrawDC, HDC hReadDC, HGLRC hglrc) {
    BOOL ret;
    _pre_call_wgl_callback("wglMakeContextCurrentEXT", (GLADapiproc) glad_wglMakeContextCurrentEXT, 3, hDrawDC, hReadDC, hglrc);
    ret = glad_wglMakeContextCurrentEXT(hDrawDC, hReadDC, hglrc);
    _post_call_wgl_callback((void*) &ret, "wglMakeContextCurrentEXT", (GLADapiproc) glad_wglMakeContextCurrentEXT, 3, hDrawDC, hReadDC, hglrc);
    return ret;
}
PFNWGLMAKECONTEXTCURRENTEXTPROC glad_debug_wglMakeContextCurrentEXT = glad_debug_impl_wglMakeContextCurrentEXT;
PFNWGLQUERYCURRENTCONTEXTNVPROC glad_wglQueryCurrentContextNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglQueryCurrentContextNV(int iAttribute, int * piValue) {
    BOOL ret;
    _pre_call_wgl_callback("wglQueryCurrentContextNV", (GLADapiproc) glad_wglQueryCurrentContextNV, 2, iAttribute, piValue);
    ret = glad_wglQueryCurrentContextNV(iAttribute, piValue);
    _post_call_wgl_callback((void*) &ret, "wglQueryCurrentContextNV", (GLADapiproc) glad_wglQueryCurrentContextNV, 2, iAttribute, piValue);
    return ret;
}
PFNWGLQUERYCURRENTCONTEXTNVPROC glad_debug_wglQueryCurrentContextNV = glad_debug_impl_wglQueryCurrentContextNV;
PFNWGLQUERYFRAMECOUNTNVPROC glad_wglQueryFrameCountNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglQueryFrameCountNV(HDC hDC, GLuint * count) {
    BOOL ret;
    _pre_call_wgl_callback("wglQueryFrameCountNV", (GLADapiproc) glad_wglQueryFrameCountNV, 2, hDC, count);
    ret = glad_wglQueryFrameCountNV(hDC, count);
    _post_call_wgl_callback((void*) &ret, "wglQueryFrameCountNV", (GLADapiproc) glad_wglQueryFrameCountNV, 2, hDC, count);
    return ret;
}
PFNWGLQUERYFRAMECOUNTNVPROC glad_debug_wglQueryFrameCountNV = glad_debug_impl_wglQueryFrameCountNV;
PFNWGLQUERYFRAMELOCKMASTERI3DPROC glad_wglQueryFrameLockMasterI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglQueryFrameLockMasterI3D(BOOL * pFlag) {
    BOOL ret;
    _pre_call_wgl_callback("wglQueryFrameLockMasterI3D", (GLADapiproc) glad_wglQueryFrameLockMasterI3D, 1, pFlag);
    ret = glad_wglQueryFrameLockMasterI3D(pFlag);
    _post_call_wgl_callback((void*) &ret, "wglQueryFrameLockMasterI3D", (GLADapiproc) glad_wglQueryFrameLockMasterI3D, 1, pFlag);
    return ret;
}
PFNWGLQUERYFRAMELOCKMASTERI3DPROC glad_debug_wglQueryFrameLockMasterI3D = glad_debug_impl_wglQueryFrameLockMasterI3D;
PFNWGLQUERYFRAMETRACKINGI3DPROC glad_wglQueryFrameTrackingI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglQueryFrameTrackingI3D(DWORD * pFrameCount, DWORD * pMissedFrames, float * pLastMissedUsage) {
    BOOL ret;
    _pre_call_wgl_callback("wglQueryFrameTrackingI3D", (GLADapiproc) glad_wglQueryFrameTrackingI3D, 3, pFrameCount, pMissedFrames, pLastMissedUsage);
    ret = glad_wglQueryFrameTrackingI3D(pFrameCount, pMissedFrames, pLastMissedUsage);
    _post_call_wgl_callback((void*) &ret, "wglQueryFrameTrackingI3D", (GLADapiproc) glad_wglQueryFrameTrackingI3D, 3, pFrameCount, pMissedFrames, pLastMissedUsage);
    return ret;
}
PFNWGLQUERYFRAMETRACKINGI3DPROC glad_debug_wglQueryFrameTrackingI3D = glad_debug_impl_wglQueryFrameTrackingI3D;
PFNWGLQUERYGENLOCKMAXSOURCEDELAYI3DPROC glad_wglQueryGenlockMaxSourceDelayI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglQueryGenlockMaxSourceDelayI3D(HDC hDC, UINT * uMaxLineDelay, UINT * uMaxPixelDelay) {
    BOOL ret;
    _pre_call_wgl_callback("wglQueryGenlockMaxSourceDelayI3D", (GLADapiproc) glad_wglQueryGenlockMaxSourceDelayI3D, 3, hDC, uMaxLineDelay, uMaxPixelDelay);
    ret = glad_wglQueryGenlockMaxSourceDelayI3D(hDC, uMaxLineDelay, uMaxPixelDelay);
    _post_call_wgl_callback((void*) &ret, "wglQueryGenlockMaxSourceDelayI3D", (GLADapiproc) glad_wglQueryGenlockMaxSourceDelayI3D, 3, hDC, uMaxLineDelay, uMaxPixelDelay);
    return ret;
}
PFNWGLQUERYGENLOCKMAXSOURCEDELAYI3DPROC glad_debug_wglQueryGenlockMaxSourceDelayI3D = glad_debug_impl_wglQueryGenlockMaxSourceDelayI3D;
PFNWGLQUERYMAXSWAPGROUPSNVPROC glad_wglQueryMaxSwapGroupsNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglQueryMaxSwapGroupsNV(HDC hDC, GLuint * maxGroups, GLuint * maxBarriers) {
    BOOL ret;
    _pre_call_wgl_callback("wglQueryMaxSwapGroupsNV", (GLADapiproc) glad_wglQueryMaxSwapGroupsNV, 3, hDC, maxGroups, maxBarriers);
    ret = glad_wglQueryMaxSwapGroupsNV(hDC, maxGroups, maxBarriers);
    _post_call_wgl_callback((void*) &ret, "wglQueryMaxSwapGroupsNV", (GLADapiproc) glad_wglQueryMaxSwapGroupsNV, 3, hDC, maxGroups, maxBarriers);
    return ret;
}
PFNWGLQUERYMAXSWAPGROUPSNVPROC glad_debug_wglQueryMaxSwapGroupsNV = glad_debug_impl_wglQueryMaxSwapGroupsNV;
PFNWGLQUERYPBUFFERARBPROC glad_wglQueryPbufferARB = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglQueryPbufferARB(HPBUFFERARB hPbuffer, int iAttribute, int * piValue) {
    BOOL ret;
    _pre_call_wgl_callback("wglQueryPbufferARB", (GLADapiproc) glad_wglQueryPbufferARB, 3, hPbuffer, iAttribute, piValue);
    ret = glad_wglQueryPbufferARB(hPbuffer, iAttribute, piValue);
    _post_call_wgl_callback((void*) &ret, "wglQueryPbufferARB", (GLADapiproc) glad_wglQueryPbufferARB, 3, hPbuffer, iAttribute, piValue);
    return ret;
}
PFNWGLQUERYPBUFFERARBPROC glad_debug_wglQueryPbufferARB = glad_debug_impl_wglQueryPbufferARB;
PFNWGLQUERYPBUFFEREXTPROC glad_wglQueryPbufferEXT = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglQueryPbufferEXT(HPBUFFEREXT hPbuffer, int iAttribute, int * piValue) {
    BOOL ret;
    _pre_call_wgl_callback("wglQueryPbufferEXT", (GLADapiproc) glad_wglQueryPbufferEXT, 3, hPbuffer, iAttribute, piValue);
    ret = glad_wglQueryPbufferEXT(hPbuffer, iAttribute, piValue);
    _post_call_wgl_callback((void*) &ret, "wglQueryPbufferEXT", (GLADapiproc) glad_wglQueryPbufferEXT, 3, hPbuffer, iAttribute, piValue);
    return ret;
}
PFNWGLQUERYPBUFFEREXTPROC glad_debug_wglQueryPbufferEXT = glad_debug_impl_wglQueryPbufferEXT;
PFNWGLQUERYSWAPGROUPNVPROC glad_wglQuerySwapGroupNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglQuerySwapGroupNV(HDC hDC, GLuint * group, GLuint * barrier) {
    BOOL ret;
    _pre_call_wgl_callback("wglQuerySwapGroupNV", (GLADapiproc) glad_wglQuerySwapGroupNV, 3, hDC, group, barrier);
    ret = glad_wglQuerySwapGroupNV(hDC, group, barrier);
    _post_call_wgl_callback((void*) &ret, "wglQuerySwapGroupNV", (GLADapiproc) glad_wglQuerySwapGroupNV, 3, hDC, group, barrier);
    return ret;
}
PFNWGLQUERYSWAPGROUPNVPROC glad_debug_wglQuerySwapGroupNV = glad_debug_impl_wglQuerySwapGroupNV;
PFNWGLQUERYVIDEOCAPTUREDEVICENVPROC glad_wglQueryVideoCaptureDeviceNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglQueryVideoCaptureDeviceNV(HDC hDc, HVIDEOINPUTDEVICENV hDevice, int iAttribute, int * piValue) {
    BOOL ret;
    _pre_call_wgl_callback("wglQueryVideoCaptureDeviceNV", (GLADapiproc) glad_wglQueryVideoCaptureDeviceNV, 4, hDc, hDevice, iAttribute, piValue);
    ret = glad_wglQueryVideoCaptureDeviceNV(hDc, hDevice, iAttribute, piValue);
    _post_call_wgl_callback((void*) &ret, "wglQueryVideoCaptureDeviceNV", (GLADapiproc) glad_wglQueryVideoCaptureDeviceNV, 4, hDc, hDevice, iAttribute, piValue);
    return ret;
}
PFNWGLQUERYVIDEOCAPTUREDEVICENVPROC glad_debug_wglQueryVideoCaptureDeviceNV = glad_debug_impl_wglQueryVideoCaptureDeviceNV;
PFNWGLRELEASEIMAGEBUFFEREVENTSI3DPROC glad_wglReleaseImageBufferEventsI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglReleaseImageBufferEventsI3D(HDC hDC, const LPVOID * pAddress, UINT count) {
    BOOL ret;
    _pre_call_wgl_callback("wglReleaseImageBufferEventsI3D", (GLADapiproc) glad_wglReleaseImageBufferEventsI3D, 3, hDC, pAddress, count);
    ret = glad_wglReleaseImageBufferEventsI3D(hDC, pAddress, count);
    _post_call_wgl_callback((void*) &ret, "wglReleaseImageBufferEventsI3D", (GLADapiproc) glad_wglReleaseImageBufferEventsI3D, 3, hDC, pAddress, count);
    return ret;
}
PFNWGLRELEASEIMAGEBUFFEREVENTSI3DPROC glad_debug_wglReleaseImageBufferEventsI3D = glad_debug_impl_wglReleaseImageBufferEventsI3D;
PFNWGLRELEASEPBUFFERDCARBPROC glad_wglReleasePbufferDCARB = NULL;
static int GLAD_API_PTR glad_debug_impl_wglReleasePbufferDCARB(HPBUFFERARB hPbuffer, HDC hDC) {
    int ret;
    _pre_call_wgl_callback("wglReleasePbufferDCARB", (GLADapiproc) glad_wglReleasePbufferDCARB, 2, hPbuffer, hDC);
    ret = glad_wglReleasePbufferDCARB(hPbuffer, hDC);
    _post_call_wgl_callback((void*) &ret, "wglReleasePbufferDCARB", (GLADapiproc) glad_wglReleasePbufferDCARB, 2, hPbuffer, hDC);
    return ret;
}
PFNWGLRELEASEPBUFFERDCARBPROC glad_debug_wglReleasePbufferDCARB = glad_debug_impl_wglReleasePbufferDCARB;
PFNWGLRELEASEPBUFFERDCEXTPROC glad_wglReleasePbufferDCEXT = NULL;
static int GLAD_API_PTR glad_debug_impl_wglReleasePbufferDCEXT(HPBUFFEREXT hPbuffer, HDC hDC) {
    int ret;
    _pre_call_wgl_callback("wglReleasePbufferDCEXT", (GLADapiproc) glad_wglReleasePbufferDCEXT, 2, hPbuffer, hDC);
    ret = glad_wglReleasePbufferDCEXT(hPbuffer, hDC);
    _post_call_wgl_callback((void*) &ret, "wglReleasePbufferDCEXT", (GLADapiproc) glad_wglReleasePbufferDCEXT, 2, hPbuffer, hDC);
    return ret;
}
PFNWGLRELEASEPBUFFERDCEXTPROC glad_debug_wglReleasePbufferDCEXT = glad_debug_impl_wglReleasePbufferDCEXT;
PFNWGLRELEASETEXIMAGEARBPROC glad_wglReleaseTexImageARB = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglReleaseTexImageARB(HPBUFFERARB hPbuffer, int iBuffer) {
    BOOL ret;
    _pre_call_wgl_callback("wglReleaseTexImageARB", (GLADapiproc) glad_wglReleaseTexImageARB, 2, hPbuffer, iBuffer);
    ret = glad_wglReleaseTexImageARB(hPbuffer, iBuffer);
    _post_call_wgl_callback((void*) &ret, "wglReleaseTexImageARB", (GLADapiproc) glad_wglReleaseTexImageARB, 2, hPbuffer, iBuffer);
    return ret;
}
PFNWGLRELEASETEXIMAGEARBPROC glad_debug_wglReleaseTexImageARB = glad_debug_impl_wglReleaseTexImageARB;
PFNWGLRELEASEVIDEOCAPTUREDEVICENVPROC glad_wglReleaseVideoCaptureDeviceNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglReleaseVideoCaptureDeviceNV(HDC hDc, HVIDEOINPUTDEVICENV hDevice) {
    BOOL ret;
    _pre_call_wgl_callback("wglReleaseVideoCaptureDeviceNV", (GLADapiproc) glad_wglReleaseVideoCaptureDeviceNV, 2, hDc, hDevice);
    ret = glad_wglReleaseVideoCaptureDeviceNV(hDc, hDevice);
    _post_call_wgl_callback((void*) &ret, "wglReleaseVideoCaptureDeviceNV", (GLADapiproc) glad_wglReleaseVideoCaptureDeviceNV, 2, hDc, hDevice);
    return ret;
}
PFNWGLRELEASEVIDEOCAPTUREDEVICENVPROC glad_debug_wglReleaseVideoCaptureDeviceNV = glad_debug_impl_wglReleaseVideoCaptureDeviceNV;
PFNWGLRELEASEVIDEODEVICENVPROC glad_wglReleaseVideoDeviceNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglReleaseVideoDeviceNV(HPVIDEODEV hVideoDevice) {
    BOOL ret;
    _pre_call_wgl_callback("wglReleaseVideoDeviceNV", (GLADapiproc) glad_wglReleaseVideoDeviceNV, 1, hVideoDevice);
    ret = glad_wglReleaseVideoDeviceNV(hVideoDevice);
    _post_call_wgl_callback((void*) &ret, "wglReleaseVideoDeviceNV", (GLADapiproc) glad_wglReleaseVideoDeviceNV, 1, hVideoDevice);
    return ret;
}
PFNWGLRELEASEVIDEODEVICENVPROC glad_debug_wglReleaseVideoDeviceNV = glad_debug_impl_wglReleaseVideoDeviceNV;
PFNWGLRELEASEVIDEOIMAGENVPROC glad_wglReleaseVideoImageNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglReleaseVideoImageNV(HPBUFFERARB hPbuffer, int iVideoBuffer) {
    BOOL ret;
    _pre_call_wgl_callback("wglReleaseVideoImageNV", (GLADapiproc) glad_wglReleaseVideoImageNV, 2, hPbuffer, iVideoBuffer);
    ret = glad_wglReleaseVideoImageNV(hPbuffer, iVideoBuffer);
    _post_call_wgl_callback((void*) &ret, "wglReleaseVideoImageNV", (GLADapiproc) glad_wglReleaseVideoImageNV, 2, hPbuffer, iVideoBuffer);
    return ret;
}
PFNWGLRELEASEVIDEOIMAGENVPROC glad_debug_wglReleaseVideoImageNV = glad_debug_impl_wglReleaseVideoImageNV;
PFNWGLRESETFRAMECOUNTNVPROC glad_wglResetFrameCountNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglResetFrameCountNV(HDC hDC) {
    BOOL ret;
    _pre_call_wgl_callback("wglResetFrameCountNV", (GLADapiproc) glad_wglResetFrameCountNV, 1, hDC);
    ret = glad_wglResetFrameCountNV(hDC);
    _post_call_wgl_callback((void*) &ret, "wglResetFrameCountNV", (GLADapiproc) glad_wglResetFrameCountNV, 1, hDC);
    return ret;
}
PFNWGLRESETFRAMECOUNTNVPROC glad_debug_wglResetFrameCountNV = glad_debug_impl_wglResetFrameCountNV;
PFNWGLRESTOREBUFFERREGIONARBPROC glad_wglRestoreBufferRegionARB = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglRestoreBufferRegionARB(HANDLE hRegion, int x, int y, int width, int height, int xSrc, int ySrc) {
    BOOL ret;
    _pre_call_wgl_callback("wglRestoreBufferRegionARB", (GLADapiproc) glad_wglRestoreBufferRegionARB, 7, hRegion, x, y, width, height, xSrc, ySrc);
    ret = glad_wglRestoreBufferRegionARB(hRegion, x, y, width, height, xSrc, ySrc);
    _post_call_wgl_callback((void*) &ret, "wglRestoreBufferRegionARB", (GLADapiproc) glad_wglRestoreBufferRegionARB, 7, hRegion, x, y, width, height, xSrc, ySrc);
    return ret;
}
PFNWGLRESTOREBUFFERREGIONARBPROC glad_debug_wglRestoreBufferRegionARB = glad_debug_impl_wglRestoreBufferRegionARB;
PFNWGLSAVEBUFFERREGIONARBPROC glad_wglSaveBufferRegionARB = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglSaveBufferRegionARB(HANDLE hRegion, int x, int y, int width, int height) {
    BOOL ret;
    _pre_call_wgl_callback("wglSaveBufferRegionARB", (GLADapiproc) glad_wglSaveBufferRegionARB, 5, hRegion, x, y, width, height);
    ret = glad_wglSaveBufferRegionARB(hRegion, x, y, width, height);
    _post_call_wgl_callback((void*) &ret, "wglSaveBufferRegionARB", (GLADapiproc) glad_wglSaveBufferRegionARB, 5, hRegion, x, y, width, height);
    return ret;
}
PFNWGLSAVEBUFFERREGIONARBPROC glad_debug_wglSaveBufferRegionARB = glad_debug_impl_wglSaveBufferRegionARB;
PFNWGLSENDPBUFFERTOVIDEONVPROC glad_wglSendPbufferToVideoNV = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglSendPbufferToVideoNV(HPBUFFERARB hPbuffer, int iBufferType, unsigned long * pulCounterPbuffer, BOOL bBlock) {
    BOOL ret;
    _pre_call_wgl_callback("wglSendPbufferToVideoNV", (GLADapiproc) glad_wglSendPbufferToVideoNV, 4, hPbuffer, iBufferType, pulCounterPbuffer, bBlock);
    ret = glad_wglSendPbufferToVideoNV(hPbuffer, iBufferType, pulCounterPbuffer, bBlock);
    _post_call_wgl_callback((void*) &ret, "wglSendPbufferToVideoNV", (GLADapiproc) glad_wglSendPbufferToVideoNV, 4, hPbuffer, iBufferType, pulCounterPbuffer, bBlock);
    return ret;
}
PFNWGLSENDPBUFFERTOVIDEONVPROC glad_debug_wglSendPbufferToVideoNV = glad_debug_impl_wglSendPbufferToVideoNV;
PFNWGLSETDIGITALVIDEOPARAMETERSI3DPROC glad_wglSetDigitalVideoParametersI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglSetDigitalVideoParametersI3D(HDC hDC, int iAttribute, const int * piValue) {
    BOOL ret;
    _pre_call_wgl_callback("wglSetDigitalVideoParametersI3D", (GLADapiproc) glad_wglSetDigitalVideoParametersI3D, 3, hDC, iAttribute, piValue);
    ret = glad_wglSetDigitalVideoParametersI3D(hDC, iAttribute, piValue);
    _post_call_wgl_callback((void*) &ret, "wglSetDigitalVideoParametersI3D", (GLADapiproc) glad_wglSetDigitalVideoParametersI3D, 3, hDC, iAttribute, piValue);
    return ret;
}
PFNWGLSETDIGITALVIDEOPARAMETERSI3DPROC glad_debug_wglSetDigitalVideoParametersI3D = glad_debug_impl_wglSetDigitalVideoParametersI3D;
PFNWGLSETGAMMATABLEI3DPROC glad_wglSetGammaTableI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglSetGammaTableI3D(HDC hDC, int iEntries, const USHORT * puRed, const USHORT * puGreen, const USHORT * puBlue) {
    BOOL ret;
    _pre_call_wgl_callback("wglSetGammaTableI3D", (GLADapiproc) glad_wglSetGammaTableI3D, 5, hDC, iEntries, puRed, puGreen, puBlue);
    ret = glad_wglSetGammaTableI3D(hDC, iEntries, puRed, puGreen, puBlue);
    _post_call_wgl_callback((void*) &ret, "wglSetGammaTableI3D", (GLADapiproc) glad_wglSetGammaTableI3D, 5, hDC, iEntries, puRed, puGreen, puBlue);
    return ret;
}
PFNWGLSETGAMMATABLEI3DPROC glad_debug_wglSetGammaTableI3D = glad_debug_impl_wglSetGammaTableI3D;
PFNWGLSETGAMMATABLEPARAMETERSI3DPROC glad_wglSetGammaTableParametersI3D = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglSetGammaTableParametersI3D(HDC hDC, int iAttribute, const int * piValue) {
    BOOL ret;
    _pre_call_wgl_callback("wglSetGammaTableParametersI3D", (GLADapiproc) glad_wglSetGammaTableParametersI3D, 3, hDC, iAttribute, piValue);
    ret = glad_wglSetGammaTableParametersI3D(hDC, iAttribute, piValue);
    _post_call_wgl_callback((void*) &ret, "wglSetGammaTableParametersI3D", (GLADapiproc) glad_wglSetGammaTableParametersI3D, 3, hDC, iAttribute, piValue);
    return ret;
}
PFNWGLSETGAMMATABLEPARAMETERSI3DPROC glad_debug_wglSetGammaTableParametersI3D = glad_debug_impl_wglSetGammaTableParametersI3D;
PFNWGLSETPBUFFERATTRIBARBPROC glad_wglSetPbufferAttribARB = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglSetPbufferAttribARB(HPBUFFERARB hPbuffer, const int * piAttribList) {
    BOOL ret;
    _pre_call_wgl_callback("wglSetPbufferAttribARB", (GLADapiproc) glad_wglSetPbufferAttribARB, 2, hPbuffer, piAttribList);
    ret = glad_wglSetPbufferAttribARB(hPbuffer, piAttribList);
    _post_call_wgl_callback((void*) &ret, "wglSetPbufferAttribARB", (GLADapiproc) glad_wglSetPbufferAttribARB, 2, hPbuffer, piAttribList);
    return ret;
}
PFNWGLSETPBUFFERATTRIBARBPROC glad_debug_wglSetPbufferAttribARB = glad_debug_impl_wglSetPbufferAttribARB;
PFNWGLSETSTEREOEMITTERSTATE3DLPROC glad_wglSetStereoEmitterState3DL = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglSetStereoEmitterState3DL(HDC hDC, UINT uState) {
    BOOL ret;
    _pre_call_wgl_callback("wglSetStereoEmitterState3DL", (GLADapiproc) glad_wglSetStereoEmitterState3DL, 2, hDC, uState);
    ret = glad_wglSetStereoEmitterState3DL(hDC, uState);
    _post_call_wgl_callback((void*) &ret, "wglSetStereoEmitterState3DL", (GLADapiproc) glad_wglSetStereoEmitterState3DL, 2, hDC, uState);
    return ret;
}
PFNWGLSETSTEREOEMITTERSTATE3DLPROC glad_debug_wglSetStereoEmitterState3DL = glad_debug_impl_wglSetStereoEmitterState3DL;
PFNWGLSWAPBUFFERSMSCOMLPROC glad_wglSwapBuffersMscOML = NULL;
static INT64 GLAD_API_PTR glad_debug_impl_wglSwapBuffersMscOML(HDC hdc, INT64 target_msc, INT64 divisor, INT64 remainder) {
    INT64 ret;
    _pre_call_wgl_callback("wglSwapBuffersMscOML", (GLADapiproc) glad_wglSwapBuffersMscOML, 4, hdc, target_msc, divisor, remainder);
    ret = glad_wglSwapBuffersMscOML(hdc, target_msc, divisor, remainder);
    _post_call_wgl_callback((void*) &ret, "wglSwapBuffersMscOML", (GLADapiproc) glad_wglSwapBuffersMscOML, 4, hdc, target_msc, divisor, remainder);
    return ret;
}
PFNWGLSWAPBUFFERSMSCOMLPROC glad_debug_wglSwapBuffersMscOML = glad_debug_impl_wglSwapBuffersMscOML;
PFNWGLSWAPINTERVALEXTPROC glad_wglSwapIntervalEXT = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglSwapIntervalEXT(int interval) {
    BOOL ret;
    _pre_call_wgl_callback("wglSwapIntervalEXT", (GLADapiproc) glad_wglSwapIntervalEXT, 1, interval);
    ret = glad_wglSwapIntervalEXT(interval);
    _post_call_wgl_callback((void*) &ret, "wglSwapIntervalEXT", (GLADapiproc) glad_wglSwapIntervalEXT, 1, interval);
    return ret;
}
PFNWGLSWAPINTERVALEXTPROC glad_debug_wglSwapIntervalEXT = glad_debug_impl_wglSwapIntervalEXT;
PFNWGLSWAPLAYERBUFFERSMSCOMLPROC glad_wglSwapLayerBuffersMscOML = NULL;
static INT64 GLAD_API_PTR glad_debug_impl_wglSwapLayerBuffersMscOML(HDC hdc, INT fuPlanes, INT64 target_msc, INT64 divisor, INT64 remainder) {
    INT64 ret;
    _pre_call_wgl_callback("wglSwapLayerBuffersMscOML", (GLADapiproc) glad_wglSwapLayerBuffersMscOML, 5, hdc, fuPlanes, target_msc, divisor, remainder);
    ret = glad_wglSwapLayerBuffersMscOML(hdc, fuPlanes, target_msc, divisor, remainder);
    _post_call_wgl_callback((void*) &ret, "wglSwapLayerBuffersMscOML", (GLADapiproc) glad_wglSwapLayerBuffersMscOML, 5, hdc, fuPlanes, target_msc, divisor, remainder);
    return ret;
}
PFNWGLSWAPLAYERBUFFERSMSCOMLPROC glad_debug_wglSwapLayerBuffersMscOML = glad_debug_impl_wglSwapLayerBuffersMscOML;
PFNWGLWAITFORMSCOMLPROC glad_wglWaitForMscOML = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglWaitForMscOML(HDC hdc, INT64 target_msc, INT64 divisor, INT64 remainder, INT64 * ust, INT64 * msc, INT64 * sbc) {
    BOOL ret;
    _pre_call_wgl_callback("wglWaitForMscOML", (GLADapiproc) glad_wglWaitForMscOML, 7, hdc, target_msc, divisor, remainder, ust, msc, sbc);
    ret = glad_wglWaitForMscOML(hdc, target_msc, divisor, remainder, ust, msc, sbc);
    _post_call_wgl_callback((void*) &ret, "wglWaitForMscOML", (GLADapiproc) glad_wglWaitForMscOML, 7, hdc, target_msc, divisor, remainder, ust, msc, sbc);
    return ret;
}
PFNWGLWAITFORMSCOMLPROC glad_debug_wglWaitForMscOML = glad_debug_impl_wglWaitForMscOML;
PFNWGLWAITFORSBCOMLPROC glad_wglWaitForSbcOML = NULL;
static BOOL GLAD_API_PTR glad_debug_impl_wglWaitForSbcOML(HDC hdc, INT64 target_sbc, INT64 * ust, INT64 * msc, INT64 * sbc) {
    BOOL ret;
    _pre_call_wgl_callback("wglWaitForSbcOML", (GLADapiproc) glad_wglWaitForSbcOML, 5, hdc, target_sbc, ust, msc, sbc);
    ret = glad_wglWaitForSbcOML(hdc, target_sbc, ust, msc, sbc);
    _post_call_wgl_callback((void*) &ret, "wglWaitForSbcOML", (GLADapiproc) glad_wglWaitForSbcOML, 5, hdc, target_sbc, ust, msc, sbc);
    return ret;
}
PFNWGLWAITFORSBCOMLPROC glad_debug_wglWaitForSbcOML = glad_debug_impl_wglWaitForSbcOML;


static void glad_wgl_load_WGL_3DL_stereo_control(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_3DL_stereo_control) return;
    glad_wglSetStereoEmitterState3DL = (PFNWGLSETSTEREOEMITTERSTATE3DLPROC) load(userptr, "wglSetStereoEmitterState3DL");
}
static void glad_wgl_load_WGL_AMD_gpu_association(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_AMD_gpu_association) return;
    glad_wglBlitContextFramebufferAMD = (PFNWGLBLITCONTEXTFRAMEBUFFERAMDPROC) load(userptr, "wglBlitContextFramebufferAMD");
    glad_wglCreateAssociatedContextAMD = (PFNWGLCREATEASSOCIATEDCONTEXTAMDPROC) load(userptr, "wglCreateAssociatedContextAMD");
    glad_wglCreateAssociatedContextAttribsAMD = (PFNWGLCREATEASSOCIATEDCONTEXTATTRIBSAMDPROC) load(userptr, "wglCreateAssociatedContextAttribsAMD");
    glad_wglDeleteAssociatedContextAMD = (PFNWGLDELETEASSOCIATEDCONTEXTAMDPROC) load(userptr, "wglDeleteAssociatedContextAMD");
    glad_wglGetContextGPUIDAMD = (PFNWGLGETCONTEXTGPUIDAMDPROC) load(userptr, "wglGetContextGPUIDAMD");
    glad_wglGetCurrentAssociatedContextAMD = (PFNWGLGETCURRENTASSOCIATEDCONTEXTAMDPROC) load(userptr, "wglGetCurrentAssociatedContextAMD");
    glad_wglGetGPUIDsAMD = (PFNWGLGETGPUIDSAMDPROC) load(userptr, "wglGetGPUIDsAMD");
    glad_wglGetGPUInfoAMD = (PFNWGLGETGPUINFOAMDPROC) load(userptr, "wglGetGPUInfoAMD");
    glad_wglMakeAssociatedContextCurrentAMD = (PFNWGLMAKEASSOCIATEDCONTEXTCURRENTAMDPROC) load(userptr, "wglMakeAssociatedContextCurrentAMD");
}
static void glad_wgl_load_WGL_ARB_buffer_region(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_ARB_buffer_region) return;
    glad_wglCreateBufferRegionARB = (PFNWGLCREATEBUFFERREGIONARBPROC) load(userptr, "wglCreateBufferRegionARB");
    glad_wglDeleteBufferRegionARB = (PFNWGLDELETEBUFFERREGIONARBPROC) load(userptr, "wglDeleteBufferRegionARB");
    glad_wglRestoreBufferRegionARB = (PFNWGLRESTOREBUFFERREGIONARBPROC) load(userptr, "wglRestoreBufferRegionARB");
    glad_wglSaveBufferRegionARB = (PFNWGLSAVEBUFFERREGIONARBPROC) load(userptr, "wglSaveBufferRegionARB");
}
static void glad_wgl_load_WGL_ARB_create_context(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_ARB_create_context) return;
    glad_wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) load(userptr, "wglCreateContextAttribsARB");
}
static void glad_wgl_load_WGL_ARB_extensions_string(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_ARB_extensions_string) return;
    glad_wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC) load(userptr, "wglGetExtensionsStringARB");
}
static void glad_wgl_load_WGL_ARB_make_current_read(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_ARB_make_current_read) return;
    glad_wglGetCurrentReadDCARB = (PFNWGLGETCURRENTREADDCARBPROC) load(userptr, "wglGetCurrentReadDCARB");
    glad_wglMakeContextCurrentARB = (PFNWGLMAKECONTEXTCURRENTARBPROC) load(userptr, "wglMakeContextCurrentARB");
}
static void glad_wgl_load_WGL_ARB_pbuffer(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_ARB_pbuffer) return;
    glad_wglCreatePbufferARB = (PFNWGLCREATEPBUFFERARBPROC) load(userptr, "wglCreatePbufferARB");
    glad_wglDestroyPbufferARB = (PFNWGLDESTROYPBUFFERARBPROC) load(userptr, "wglDestroyPbufferARB");
    glad_wglGetPbufferDCARB = (PFNWGLGETPBUFFERDCARBPROC) load(userptr, "wglGetPbufferDCARB");
    glad_wglQueryPbufferARB = (PFNWGLQUERYPBUFFERARBPROC) load(userptr, "wglQueryPbufferARB");
    glad_wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARBPROC) load(userptr, "wglReleasePbufferDCARB");
}
static void glad_wgl_load_WGL_ARB_pixel_format(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_ARB_pixel_format) return;
    glad_wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) load(userptr, "wglChoosePixelFormatARB");
    glad_wglGetPixelFormatAttribfvARB = (PFNWGLGETPIXELFORMATATTRIBFVARBPROC) load(userptr, "wglGetPixelFormatAttribfvARB");
    glad_wglGetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC) load(userptr, "wglGetPixelFormatAttribivARB");
}
static void glad_wgl_load_WGL_ARB_render_texture(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_ARB_render_texture) return;
    glad_wglBindTexImageARB = (PFNWGLBINDTEXIMAGEARBPROC) load(userptr, "wglBindTexImageARB");
    glad_wglReleaseTexImageARB = (PFNWGLRELEASETEXIMAGEARBPROC) load(userptr, "wglReleaseTexImageARB");
    glad_wglSetPbufferAttribARB = (PFNWGLSETPBUFFERATTRIBARBPROC) load(userptr, "wglSetPbufferAttribARB");
}
static void glad_wgl_load_WGL_EXT_display_color_table(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_EXT_display_color_table) return;
    glad_wglBindDisplayColorTableEXT = (PFNWGLBINDDISPLAYCOLORTABLEEXTPROC) load(userptr, "wglBindDisplayColorTableEXT");
    glad_wglCreateDisplayColorTableEXT = (PFNWGLCREATEDISPLAYCOLORTABLEEXTPROC) load(userptr, "wglCreateDisplayColorTableEXT");
    glad_wglDestroyDisplayColorTableEXT = (PFNWGLDESTROYDISPLAYCOLORTABLEEXTPROC) load(userptr, "wglDestroyDisplayColorTableEXT");
    glad_wglLoadDisplayColorTableEXT = (PFNWGLLOADDISPLAYCOLORTABLEEXTPROC) load(userptr, "wglLoadDisplayColorTableEXT");
}
static void glad_wgl_load_WGL_EXT_extensions_string(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_EXT_extensions_string) return;
    glad_wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC) load(userptr, "wglGetExtensionsStringEXT");
}
static void glad_wgl_load_WGL_EXT_make_current_read(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_EXT_make_current_read) return;
    glad_wglGetCurrentReadDCEXT = (PFNWGLGETCURRENTREADDCEXTPROC) load(userptr, "wglGetCurrentReadDCEXT");
    glad_wglMakeContextCurrentEXT = (PFNWGLMAKECONTEXTCURRENTEXTPROC) load(userptr, "wglMakeContextCurrentEXT");
}
static void glad_wgl_load_WGL_EXT_pbuffer(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_EXT_pbuffer) return;
    glad_wglCreatePbufferEXT = (PFNWGLCREATEPBUFFEREXTPROC) load(userptr, "wglCreatePbufferEXT");
    glad_wglDestroyPbufferEXT = (PFNWGLDESTROYPBUFFEREXTPROC) load(userptr, "wglDestroyPbufferEXT");
    glad_wglGetPbufferDCEXT = (PFNWGLGETPBUFFERDCEXTPROC) load(userptr, "wglGetPbufferDCEXT");
    glad_wglQueryPbufferEXT = (PFNWGLQUERYPBUFFEREXTPROC) load(userptr, "wglQueryPbufferEXT");
    glad_wglReleasePbufferDCEXT = (PFNWGLRELEASEPBUFFERDCEXTPROC) load(userptr, "wglReleasePbufferDCEXT");
}
static void glad_wgl_load_WGL_EXT_pixel_format(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_EXT_pixel_format) return;
    glad_wglChoosePixelFormatEXT = (PFNWGLCHOOSEPIXELFORMATEXTPROC) load(userptr, "wglChoosePixelFormatEXT");
    glad_wglGetPixelFormatAttribfvEXT = (PFNWGLGETPIXELFORMATATTRIBFVEXTPROC) load(userptr, "wglGetPixelFormatAttribfvEXT");
    glad_wglGetPixelFormatAttribivEXT = (PFNWGLGETPIXELFORMATATTRIBIVEXTPROC) load(userptr, "wglGetPixelFormatAttribivEXT");
}
static void glad_wgl_load_WGL_EXT_swap_control(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_EXT_swap_control) return;
    glad_wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC) load(userptr, "wglGetSwapIntervalEXT");
    glad_wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) load(userptr, "wglSwapIntervalEXT");
}
static void glad_wgl_load_WGL_I3D_digital_video_control(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_I3D_digital_video_control) return;
    glad_wglGetDigitalVideoParametersI3D = (PFNWGLGETDIGITALVIDEOPARAMETERSI3DPROC) load(userptr, "wglGetDigitalVideoParametersI3D");
    glad_wglSetDigitalVideoParametersI3D = (PFNWGLSETDIGITALVIDEOPARAMETERSI3DPROC) load(userptr, "wglSetDigitalVideoParametersI3D");
}
static void glad_wgl_load_WGL_I3D_gamma(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_I3D_gamma) return;
    glad_wglGetGammaTableI3D = (PFNWGLGETGAMMATABLEI3DPROC) load(userptr, "wglGetGammaTableI3D");
    glad_wglGetGammaTableParametersI3D = (PFNWGLGETGAMMATABLEPARAMETERSI3DPROC) load(userptr, "wglGetGammaTableParametersI3D");
    glad_wglSetGammaTableI3D = (PFNWGLSETGAMMATABLEI3DPROC) load(userptr, "wglSetGammaTableI3D");
    glad_wglSetGammaTableParametersI3D = (PFNWGLSETGAMMATABLEPARAMETERSI3DPROC) load(userptr, "wglSetGammaTableParametersI3D");
}
static void glad_wgl_load_WGL_I3D_genlock(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_I3D_genlock) return;
    glad_wglDisableGenlockI3D = (PFNWGLDISABLEGENLOCKI3DPROC) load(userptr, "wglDisableGenlockI3D");
    glad_wglEnableGenlockI3D = (PFNWGLENABLEGENLOCKI3DPROC) load(userptr, "wglEnableGenlockI3D");
    glad_wglGenlockSampleRateI3D = (PFNWGLGENLOCKSAMPLERATEI3DPROC) load(userptr, "wglGenlockSampleRateI3D");
    glad_wglGenlockSourceDelayI3D = (PFNWGLGENLOCKSOURCEDELAYI3DPROC) load(userptr, "wglGenlockSourceDelayI3D");
    glad_wglGenlockSourceEdgeI3D = (PFNWGLGENLOCKSOURCEEDGEI3DPROC) load(userptr, "wglGenlockSourceEdgeI3D");
    glad_wglGenlockSourceI3D = (PFNWGLGENLOCKSOURCEI3DPROC) load(userptr, "wglGenlockSourceI3D");
    glad_wglGetGenlockSampleRateI3D = (PFNWGLGETGENLOCKSAMPLERATEI3DPROC) load(userptr, "wglGetGenlockSampleRateI3D");
    glad_wglGetGenlockSourceDelayI3D = (PFNWGLGETGENLOCKSOURCEDELAYI3DPROC) load(userptr, "wglGetGenlockSourceDelayI3D");
    glad_wglGetGenlockSourceEdgeI3D = (PFNWGLGETGENLOCKSOURCEEDGEI3DPROC) load(userptr, "wglGetGenlockSourceEdgeI3D");
    glad_wglGetGenlockSourceI3D = (PFNWGLGETGENLOCKSOURCEI3DPROC) load(userptr, "wglGetGenlockSourceI3D");
    glad_wglIsEnabledGenlockI3D = (PFNWGLISENABLEDGENLOCKI3DPROC) load(userptr, "wglIsEnabledGenlockI3D");
    glad_wglQueryGenlockMaxSourceDelayI3D = (PFNWGLQUERYGENLOCKMAXSOURCEDELAYI3DPROC) load(userptr, "wglQueryGenlockMaxSourceDelayI3D");
}
static void glad_wgl_load_WGL_I3D_image_buffer(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_I3D_image_buffer) return;
    glad_wglAssociateImageBufferEventsI3D = (PFNWGLASSOCIATEIMAGEBUFFEREVENTSI3DPROC) load(userptr, "wglAssociateImageBufferEventsI3D");
    glad_wglCreateImageBufferI3D = (PFNWGLCREATEIMAGEBUFFERI3DPROC) load(userptr, "wglCreateImageBufferI3D");
    glad_wglDestroyImageBufferI3D = (PFNWGLDESTROYIMAGEBUFFERI3DPROC) load(userptr, "wglDestroyImageBufferI3D");
    glad_wglReleaseImageBufferEventsI3D = (PFNWGLRELEASEIMAGEBUFFEREVENTSI3DPROC) load(userptr, "wglReleaseImageBufferEventsI3D");
}
static void glad_wgl_load_WGL_I3D_swap_frame_lock(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_I3D_swap_frame_lock) return;
    glad_wglDisableFrameLockI3D = (PFNWGLDISABLEFRAMELOCKI3DPROC) load(userptr, "wglDisableFrameLockI3D");
    glad_wglEnableFrameLockI3D = (PFNWGLENABLEFRAMELOCKI3DPROC) load(userptr, "wglEnableFrameLockI3D");
    glad_wglIsEnabledFrameLockI3D = (PFNWGLISENABLEDFRAMELOCKI3DPROC) load(userptr, "wglIsEnabledFrameLockI3D");
    glad_wglQueryFrameLockMasterI3D = (PFNWGLQUERYFRAMELOCKMASTERI3DPROC) load(userptr, "wglQueryFrameLockMasterI3D");
}
static void glad_wgl_load_WGL_I3D_swap_frame_usage(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_I3D_swap_frame_usage) return;
    glad_wglBeginFrameTrackingI3D = (PFNWGLBEGINFRAMETRACKINGI3DPROC) load(userptr, "wglBeginFrameTrackingI3D");
    glad_wglEndFrameTrackingI3D = (PFNWGLENDFRAMETRACKINGI3DPROC) load(userptr, "wglEndFrameTrackingI3D");
    glad_wglGetFrameUsageI3D = (PFNWGLGETFRAMEUSAGEI3DPROC) load(userptr, "wglGetFrameUsageI3D");
    glad_wglQueryFrameTrackingI3D = (PFNWGLQUERYFRAMETRACKINGI3DPROC) load(userptr, "wglQueryFrameTrackingI3D");
}
static void glad_wgl_load_WGL_NV_DX_interop(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_NV_DX_interop) return;
    glad_wglDXCloseDeviceNV = (PFNWGLDXCLOSEDEVICENVPROC) load(userptr, "wglDXCloseDeviceNV");
    glad_wglDXLockObjectsNV = (PFNWGLDXLOCKOBJECTSNVPROC) load(userptr, "wglDXLockObjectsNV");
    glad_wglDXObjectAccessNV = (PFNWGLDXOBJECTACCESSNVPROC) load(userptr, "wglDXObjectAccessNV");
    glad_wglDXOpenDeviceNV = (PFNWGLDXOPENDEVICENVPROC) load(userptr, "wglDXOpenDeviceNV");
    glad_wglDXRegisterObjectNV = (PFNWGLDXREGISTEROBJECTNVPROC) load(userptr, "wglDXRegisterObjectNV");
    glad_wglDXSetResourceShareHandleNV = (PFNWGLDXSETRESOURCESHAREHANDLENVPROC) load(userptr, "wglDXSetResourceShareHandleNV");
    glad_wglDXUnlockObjectsNV = (PFNWGLDXUNLOCKOBJECTSNVPROC) load(userptr, "wglDXUnlockObjectsNV");
    glad_wglDXUnregisterObjectNV = (PFNWGLDXUNREGISTEROBJECTNVPROC) load(userptr, "wglDXUnregisterObjectNV");
}
static void glad_wgl_load_WGL_NV_copy_image(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_NV_copy_image) return;
    glad_wglCopyImageSubDataNV = (PFNWGLCOPYIMAGESUBDATANVPROC) load(userptr, "wglCopyImageSubDataNV");
}
static void glad_wgl_load_WGL_NV_delay_before_swap(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_NV_delay_before_swap) return;
    glad_wglDelayBeforeSwapNV = (PFNWGLDELAYBEFORESWAPNVPROC) load(userptr, "wglDelayBeforeSwapNV");
}
static void glad_wgl_load_WGL_NV_gpu_affinity(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_NV_gpu_affinity) return;
    glad_wglCreateAffinityDCNV = (PFNWGLCREATEAFFINITYDCNVPROC) load(userptr, "wglCreateAffinityDCNV");
    glad_wglDeleteDCNV = (PFNWGLDELETEDCNVPROC) load(userptr, "wglDeleteDCNV");
    glad_wglEnumGpuDevicesNV = (PFNWGLENUMGPUDEVICESNVPROC) load(userptr, "wglEnumGpuDevicesNV");
    glad_wglEnumGpusFromAffinityDCNV = (PFNWGLENUMGPUSFROMAFFINITYDCNVPROC) load(userptr, "wglEnumGpusFromAffinityDCNV");
    glad_wglEnumGpusNV = (PFNWGLENUMGPUSNVPROC) load(userptr, "wglEnumGpusNV");
}
static void glad_wgl_load_WGL_NV_present_video(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_NV_present_video) return;
    glad_wglBindVideoDeviceNV = (PFNWGLBINDVIDEODEVICENVPROC) load(userptr, "wglBindVideoDeviceNV");
    glad_wglEnumerateVideoDevicesNV = (PFNWGLENUMERATEVIDEODEVICESNVPROC) load(userptr, "wglEnumerateVideoDevicesNV");
    glad_wglQueryCurrentContextNV = (PFNWGLQUERYCURRENTCONTEXTNVPROC) load(userptr, "wglQueryCurrentContextNV");
}
static void glad_wgl_load_WGL_NV_swap_group(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_NV_swap_group) return;
    glad_wglBindSwapBarrierNV = (PFNWGLBINDSWAPBARRIERNVPROC) load(userptr, "wglBindSwapBarrierNV");
    glad_wglJoinSwapGroupNV = (PFNWGLJOINSWAPGROUPNVPROC) load(userptr, "wglJoinSwapGroupNV");
    glad_wglQueryFrameCountNV = (PFNWGLQUERYFRAMECOUNTNVPROC) load(userptr, "wglQueryFrameCountNV");
    glad_wglQueryMaxSwapGroupsNV = (PFNWGLQUERYMAXSWAPGROUPSNVPROC) load(userptr, "wglQueryMaxSwapGroupsNV");
    glad_wglQuerySwapGroupNV = (PFNWGLQUERYSWAPGROUPNVPROC) load(userptr, "wglQuerySwapGroupNV");
    glad_wglResetFrameCountNV = (PFNWGLRESETFRAMECOUNTNVPROC) load(userptr, "wglResetFrameCountNV");
}
static void glad_wgl_load_WGL_NV_vertex_array_range(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_NV_vertex_array_range) return;
    glad_wglAllocateMemoryNV = (PFNWGLALLOCATEMEMORYNVPROC) load(userptr, "wglAllocateMemoryNV");
    glad_wglFreeMemoryNV = (PFNWGLFREEMEMORYNVPROC) load(userptr, "wglFreeMemoryNV");
}
static void glad_wgl_load_WGL_NV_video_capture(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_NV_video_capture) return;
    glad_wglBindVideoCaptureDeviceNV = (PFNWGLBINDVIDEOCAPTUREDEVICENVPROC) load(userptr, "wglBindVideoCaptureDeviceNV");
    glad_wglEnumerateVideoCaptureDevicesNV = (PFNWGLENUMERATEVIDEOCAPTUREDEVICESNVPROC) load(userptr, "wglEnumerateVideoCaptureDevicesNV");
    glad_wglLockVideoCaptureDeviceNV = (PFNWGLLOCKVIDEOCAPTUREDEVICENVPROC) load(userptr, "wglLockVideoCaptureDeviceNV");
    glad_wglQueryVideoCaptureDeviceNV = (PFNWGLQUERYVIDEOCAPTUREDEVICENVPROC) load(userptr, "wglQueryVideoCaptureDeviceNV");
    glad_wglReleaseVideoCaptureDeviceNV = (PFNWGLRELEASEVIDEOCAPTUREDEVICENVPROC) load(userptr, "wglReleaseVideoCaptureDeviceNV");
}
static void glad_wgl_load_WGL_NV_video_output(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_NV_video_output) return;
    glad_wglBindVideoImageNV = (PFNWGLBINDVIDEOIMAGENVPROC) load(userptr, "wglBindVideoImageNV");
    glad_wglGetVideoDeviceNV = (PFNWGLGETVIDEODEVICENVPROC) load(userptr, "wglGetVideoDeviceNV");
    glad_wglGetVideoInfoNV = (PFNWGLGETVIDEOINFONVPROC) load(userptr, "wglGetVideoInfoNV");
    glad_wglReleaseVideoDeviceNV = (PFNWGLRELEASEVIDEODEVICENVPROC) load(userptr, "wglReleaseVideoDeviceNV");
    glad_wglReleaseVideoImageNV = (PFNWGLRELEASEVIDEOIMAGENVPROC) load(userptr, "wglReleaseVideoImageNV");
    glad_wglSendPbufferToVideoNV = (PFNWGLSENDPBUFFERTOVIDEONVPROC) load(userptr, "wglSendPbufferToVideoNV");
}
static void glad_wgl_load_WGL_OML_sync_control(GLADuserptrloadfunc load, void *userptr) {
    if(!GLAD_WGL_OML_sync_control) return;
    glad_wglGetMscRateOML = (PFNWGLGETMSCRATEOMLPROC) load(userptr, "wglGetMscRateOML");
    glad_wglGetSyncValuesOML = (PFNWGLGETSYNCVALUESOMLPROC) load(userptr, "wglGetSyncValuesOML");
    glad_wglSwapBuffersMscOML = (PFNWGLSWAPBUFFERSMSCOMLPROC) load(userptr, "wglSwapBuffersMscOML");
    glad_wglSwapLayerBuffersMscOML = (PFNWGLSWAPLAYERBUFFERSMSCOMLPROC) load(userptr, "wglSwapLayerBuffersMscOML");
    glad_wglWaitForMscOML = (PFNWGLWAITFORMSCOMLPROC) load(userptr, "wglWaitForMscOML");
    glad_wglWaitForSbcOML = (PFNWGLWAITFORSBCOMLPROC) load(userptr, "wglWaitForSbcOML");
}


static void glad_wgl_resolve_aliases(void) {
}

static int glad_wgl_has_extension(HDC hdc, const char *ext) {
    const char *terminator;
    const char *loc;
    const char *extensions;

    if(wglGetExtensionsStringEXT == NULL && wglGetExtensionsStringARB == NULL)
        return 0;

    if(wglGetExtensionsStringARB == NULL || hdc == INVALID_HANDLE_VALUE)
        extensions = wglGetExtensionsStringEXT();
    else
        extensions = wglGetExtensionsStringARB(hdc);

    if(extensions == NULL || ext == NULL)
        return 0;

    while(1) {
        loc = strstr(extensions, ext);
        if(loc == NULL)
            break;

        terminator = loc + strlen(ext);
        if((loc == extensions || *(loc - 1) == ' ') &&
            (*terminator == ' ' || *terminator == '\0'))
        {
            return 1;
        }
        extensions = terminator;
    }

    return 0;
}

static GLADapiproc glad_wgl_get_proc_from_userptr(void *userptr, const char* name) {
    return (GLAD_GNUC_EXTENSION (GLADapiproc (*)(const char *name)) userptr)(name);
}

static int glad_wgl_find_extensions_wgl(HDC hdc) {
    GLAD_WGL_3DFX_multisample = glad_wgl_has_extension(hdc, "WGL_3DFX_multisample");
    GLAD_WGL_3DL_stereo_control = glad_wgl_has_extension(hdc, "WGL_3DL_stereo_control");
    GLAD_WGL_AMD_gpu_association = glad_wgl_has_extension(hdc, "WGL_AMD_gpu_association");
    GLAD_WGL_ARB_buffer_region = glad_wgl_has_extension(hdc, "WGL_ARB_buffer_region");
    GLAD_WGL_ARB_context_flush_control = glad_wgl_has_extension(hdc, "WGL_ARB_context_flush_control");
    GLAD_WGL_ARB_create_context = glad_wgl_has_extension(hdc, "WGL_ARB_create_context");
    GLAD_WGL_ARB_create_context_no_error = glad_wgl_has_extension(hdc, "WGL_ARB_create_context_no_error");
    GLAD_WGL_ARB_create_context_profile = glad_wgl_has_extension(hdc, "WGL_ARB_create_context_profile");
    GLAD_WGL_ARB_create_context_robustness = glad_wgl_has_extension(hdc, "WGL_ARB_create_context_robustness");
    GLAD_WGL_ARB_extensions_string = glad_wgl_has_extension(hdc, "WGL_ARB_extensions_string");
    GLAD_WGL_ARB_framebuffer_sRGB = glad_wgl_has_extension(hdc, "WGL_ARB_framebuffer_sRGB");
    GLAD_WGL_ARB_make_current_read = glad_wgl_has_extension(hdc, "WGL_ARB_make_current_read");
    GLAD_WGL_ARB_multisample = glad_wgl_has_extension(hdc, "WGL_ARB_multisample");
    GLAD_WGL_ARB_pbuffer = glad_wgl_has_extension(hdc, "WGL_ARB_pbuffer");
    GLAD_WGL_ARB_pixel_format = glad_wgl_has_extension(hdc, "WGL_ARB_pixel_format");
    GLAD_WGL_ARB_pixel_format_float = glad_wgl_has_extension(hdc, "WGL_ARB_pixel_format_float");
    GLAD_WGL_ARB_render_texture = glad_wgl_has_extension(hdc, "WGL_ARB_render_texture");
    GLAD_WGL_ARB_robustness_application_isolation = glad_wgl_has_extension(hdc, "WGL_ARB_robustness_application_isolation");
    GLAD_WGL_ARB_robustness_share_group_isolation = glad_wgl_has_extension(hdc, "WGL_ARB_robustness_share_group_isolation");
    GLAD_WGL_ATI_pixel_format_float = glad_wgl_has_extension(hdc, "WGL_ATI_pixel_format_float");
    GLAD_WGL_ATI_render_texture_rectangle = glad_wgl_has_extension(hdc, "WGL_ATI_render_texture_rectangle");
    GLAD_WGL_EXT_colorspace = glad_wgl_has_extension(hdc, "WGL_EXT_colorspace");
    GLAD_WGL_EXT_create_context_es2_profile = glad_wgl_has_extension(hdc, "WGL_EXT_create_context_es2_profile");
    GLAD_WGL_EXT_create_context_es_profile = glad_wgl_has_extension(hdc, "WGL_EXT_create_context_es_profile");
    GLAD_WGL_EXT_depth_float = glad_wgl_has_extension(hdc, "WGL_EXT_depth_float");
    GLAD_WGL_EXT_display_color_table = glad_wgl_has_extension(hdc, "WGL_EXT_display_color_table");
    GLAD_WGL_EXT_extensions_string = glad_wgl_has_extension(hdc, "WGL_EXT_extensions_string");
    GLAD_WGL_EXT_framebuffer_sRGB = glad_wgl_has_extension(hdc, "WGL_EXT_framebuffer_sRGB");
    GLAD_WGL_EXT_make_current_read = glad_wgl_has_extension(hdc, "WGL_EXT_make_current_read");
    GLAD_WGL_EXT_multisample = glad_wgl_has_extension(hdc, "WGL_EXT_multisample");
    GLAD_WGL_EXT_pbuffer = glad_wgl_has_extension(hdc, "WGL_EXT_pbuffer");
    GLAD_WGL_EXT_pixel_format = glad_wgl_has_extension(hdc, "WGL_EXT_pixel_format");
    GLAD_WGL_EXT_pixel_format_packed_float = glad_wgl_has_extension(hdc, "WGL_EXT_pixel_format_packed_float");
    GLAD_WGL_EXT_swap_control = glad_wgl_has_extension(hdc, "WGL_EXT_swap_control");
    GLAD_WGL_EXT_swap_control_tear = glad_wgl_has_extension(hdc, "WGL_EXT_swap_control_tear");
    GLAD_WGL_I3D_digital_video_control = glad_wgl_has_extension(hdc, "WGL_I3D_digital_video_control");
    GLAD_WGL_I3D_gamma = glad_wgl_has_extension(hdc, "WGL_I3D_gamma");
    GLAD_WGL_I3D_genlock = glad_wgl_has_extension(hdc, "WGL_I3D_genlock");
    GLAD_WGL_I3D_image_buffer = glad_wgl_has_extension(hdc, "WGL_I3D_image_buffer");
    GLAD_WGL_I3D_swap_frame_lock = glad_wgl_has_extension(hdc, "WGL_I3D_swap_frame_lock");
    GLAD_WGL_I3D_swap_frame_usage = glad_wgl_has_extension(hdc, "WGL_I3D_swap_frame_usage");
    GLAD_WGL_NV_DX_interop = glad_wgl_has_extension(hdc, "WGL_NV_DX_interop");
    GLAD_WGL_NV_DX_interop2 = glad_wgl_has_extension(hdc, "WGL_NV_DX_interop2");
    GLAD_WGL_NV_copy_image = glad_wgl_has_extension(hdc, "WGL_NV_copy_image");
    GLAD_WGL_NV_delay_before_swap = glad_wgl_has_extension(hdc, "WGL_NV_delay_before_swap");
    GLAD_WGL_NV_float_buffer = glad_wgl_has_extension(hdc, "WGL_NV_float_buffer");
    GLAD_WGL_NV_gpu_affinity = glad_wgl_has_extension(hdc, "WGL_NV_gpu_affinity");
    GLAD_WGL_NV_multigpu_context = glad_wgl_has_extension(hdc, "WGL_NV_multigpu_context");
    GLAD_WGL_NV_multisample_coverage = glad_wgl_has_extension(hdc, "WGL_NV_multisample_coverage");
    GLAD_WGL_NV_present_video = glad_wgl_has_extension(hdc, "WGL_NV_present_video");
    GLAD_WGL_NV_render_depth_texture = glad_wgl_has_extension(hdc, "WGL_NV_render_depth_texture");
    GLAD_WGL_NV_render_texture_rectangle = glad_wgl_has_extension(hdc, "WGL_NV_render_texture_rectangle");
    GLAD_WGL_NV_swap_group = glad_wgl_has_extension(hdc, "WGL_NV_swap_group");
    GLAD_WGL_NV_vertex_array_range = glad_wgl_has_extension(hdc, "WGL_NV_vertex_array_range");
    GLAD_WGL_NV_video_capture = glad_wgl_has_extension(hdc, "WGL_NV_video_capture");
    GLAD_WGL_NV_video_output = glad_wgl_has_extension(hdc, "WGL_NV_video_output");
    GLAD_WGL_OML_sync_control = glad_wgl_has_extension(hdc, "WGL_OML_sync_control");
    return 1;
}

static int glad_wgl_find_core_wgl(void) {
    int major = 1, minor = 0;
    GLAD_WGL_VERSION_1_0 = (major == 1 && minor >= 0) || major > 1;
    return GLAD_MAKE_VERSION(major, minor);
}

int gladLoadWGLUserPtr(HDC hdc, GLADuserptrloadfunc load, void *userptr) {
    int version;
    wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC) load(userptr, "wglGetExtensionsStringARB");
    wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC) load(userptr, "wglGetExtensionsStringEXT");
    if(wglGetExtensionsStringARB == NULL && wglGetExtensionsStringEXT == NULL) return 0;
    version = glad_wgl_find_core_wgl();


    if (!glad_wgl_find_extensions_wgl(hdc)) return 0;
    glad_wgl_load_WGL_3DL_stereo_control(load, userptr);
    glad_wgl_load_WGL_AMD_gpu_association(load, userptr);
    glad_wgl_load_WGL_ARB_buffer_region(load, userptr);
    glad_wgl_load_WGL_ARB_create_context(load, userptr);
    glad_wgl_load_WGL_ARB_extensions_string(load, userptr);
    glad_wgl_load_WGL_ARB_make_current_read(load, userptr);
    glad_wgl_load_WGL_ARB_pbuffer(load, userptr);
    glad_wgl_load_WGL_ARB_pixel_format(load, userptr);
    glad_wgl_load_WGL_ARB_render_texture(load, userptr);
    glad_wgl_load_WGL_EXT_display_color_table(load, userptr);
    glad_wgl_load_WGL_EXT_extensions_string(load, userptr);
    glad_wgl_load_WGL_EXT_make_current_read(load, userptr);
    glad_wgl_load_WGL_EXT_pbuffer(load, userptr);
    glad_wgl_load_WGL_EXT_pixel_format(load, userptr);
    glad_wgl_load_WGL_EXT_swap_control(load, userptr);
    glad_wgl_load_WGL_I3D_digital_video_control(load, userptr);
    glad_wgl_load_WGL_I3D_gamma(load, userptr);
    glad_wgl_load_WGL_I3D_genlock(load, userptr);
    glad_wgl_load_WGL_I3D_image_buffer(load, userptr);
    glad_wgl_load_WGL_I3D_swap_frame_lock(load, userptr);
    glad_wgl_load_WGL_I3D_swap_frame_usage(load, userptr);
    glad_wgl_load_WGL_NV_DX_interop(load, userptr);
    glad_wgl_load_WGL_NV_copy_image(load, userptr);
    glad_wgl_load_WGL_NV_delay_before_swap(load, userptr);
    glad_wgl_load_WGL_NV_gpu_affinity(load, userptr);
    glad_wgl_load_WGL_NV_present_video(load, userptr);
    glad_wgl_load_WGL_NV_swap_group(load, userptr);
    glad_wgl_load_WGL_NV_vertex_array_range(load, userptr);
    glad_wgl_load_WGL_NV_video_capture(load, userptr);
    glad_wgl_load_WGL_NV_video_output(load, userptr);
    glad_wgl_load_WGL_OML_sync_control(load, userptr);

    glad_wgl_resolve_aliases();

    return version;
}

int gladLoadWGL(HDC hdc, GLADloadfunc load) {
    return gladLoadWGLUserPtr(hdc, glad_wgl_get_proc_from_userptr, GLAD_GNUC_EXTENSION (void*) load);
}
 
void gladInstallWGLDebug(void) {
    glad_debug_wglAllocateMemoryNV = glad_debug_impl_wglAllocateMemoryNV;
    glad_debug_wglAssociateImageBufferEventsI3D = glad_debug_impl_wglAssociateImageBufferEventsI3D;
    glad_debug_wglBeginFrameTrackingI3D = glad_debug_impl_wglBeginFrameTrackingI3D;
    glad_debug_wglBindDisplayColorTableEXT = glad_debug_impl_wglBindDisplayColorTableEXT;
    glad_debug_wglBindSwapBarrierNV = glad_debug_impl_wglBindSwapBarrierNV;
    glad_debug_wglBindTexImageARB = glad_debug_impl_wglBindTexImageARB;
    glad_debug_wglBindVideoCaptureDeviceNV = glad_debug_impl_wglBindVideoCaptureDeviceNV;
    glad_debug_wglBindVideoDeviceNV = glad_debug_impl_wglBindVideoDeviceNV;
    glad_debug_wglBindVideoImageNV = glad_debug_impl_wglBindVideoImageNV;
    glad_debug_wglBlitContextFramebufferAMD = glad_debug_impl_wglBlitContextFramebufferAMD;
    glad_debug_wglChoosePixelFormatARB = glad_debug_impl_wglChoosePixelFormatARB;
    glad_debug_wglChoosePixelFormatEXT = glad_debug_impl_wglChoosePixelFormatEXT;
    glad_debug_wglCopyImageSubDataNV = glad_debug_impl_wglCopyImageSubDataNV;
    glad_debug_wglCreateAffinityDCNV = glad_debug_impl_wglCreateAffinityDCNV;
    glad_debug_wglCreateAssociatedContextAMD = glad_debug_impl_wglCreateAssociatedContextAMD;
    glad_debug_wglCreateAssociatedContextAttribsAMD = glad_debug_impl_wglCreateAssociatedContextAttribsAMD;
    glad_debug_wglCreateBufferRegionARB = glad_debug_impl_wglCreateBufferRegionARB;
    glad_debug_wglCreateContextAttribsARB = glad_debug_impl_wglCreateContextAttribsARB;
    glad_debug_wglCreateDisplayColorTableEXT = glad_debug_impl_wglCreateDisplayColorTableEXT;
    glad_debug_wglCreateImageBufferI3D = glad_debug_impl_wglCreateImageBufferI3D;
    glad_debug_wglCreatePbufferARB = glad_debug_impl_wglCreatePbufferARB;
    glad_debug_wglCreatePbufferEXT = glad_debug_impl_wglCreatePbufferEXT;
    glad_debug_wglDXCloseDeviceNV = glad_debug_impl_wglDXCloseDeviceNV;
    glad_debug_wglDXLockObjectsNV = glad_debug_impl_wglDXLockObjectsNV;
    glad_debug_wglDXObjectAccessNV = glad_debug_impl_wglDXObjectAccessNV;
    glad_debug_wglDXOpenDeviceNV = glad_debug_impl_wglDXOpenDeviceNV;
    glad_debug_wglDXRegisterObjectNV = glad_debug_impl_wglDXRegisterObjectNV;
    glad_debug_wglDXSetResourceShareHandleNV = glad_debug_impl_wglDXSetResourceShareHandleNV;
    glad_debug_wglDXUnlockObjectsNV = glad_debug_impl_wglDXUnlockObjectsNV;
    glad_debug_wglDXUnregisterObjectNV = glad_debug_impl_wglDXUnregisterObjectNV;
    glad_debug_wglDelayBeforeSwapNV = glad_debug_impl_wglDelayBeforeSwapNV;
    glad_debug_wglDeleteAssociatedContextAMD = glad_debug_impl_wglDeleteAssociatedContextAMD;
    glad_debug_wglDeleteBufferRegionARB = glad_debug_impl_wglDeleteBufferRegionARB;
    glad_debug_wglDeleteDCNV = glad_debug_impl_wglDeleteDCNV;
    glad_debug_wglDestroyDisplayColorTableEXT = glad_debug_impl_wglDestroyDisplayColorTableEXT;
    glad_debug_wglDestroyImageBufferI3D = glad_debug_impl_wglDestroyImageBufferI3D;
    glad_debug_wglDestroyPbufferARB = glad_debug_impl_wglDestroyPbufferARB;
    glad_debug_wglDestroyPbufferEXT = glad_debug_impl_wglDestroyPbufferEXT;
    glad_debug_wglDisableFrameLockI3D = glad_debug_impl_wglDisableFrameLockI3D;
    glad_debug_wglDisableGenlockI3D = glad_debug_impl_wglDisableGenlockI3D;
    glad_debug_wglEnableFrameLockI3D = glad_debug_impl_wglEnableFrameLockI3D;
    glad_debug_wglEnableGenlockI3D = glad_debug_impl_wglEnableGenlockI3D;
    glad_debug_wglEndFrameTrackingI3D = glad_debug_impl_wglEndFrameTrackingI3D;
    glad_debug_wglEnumGpuDevicesNV = glad_debug_impl_wglEnumGpuDevicesNV;
    glad_debug_wglEnumGpusFromAffinityDCNV = glad_debug_impl_wglEnumGpusFromAffinityDCNV;
    glad_debug_wglEnumGpusNV = glad_debug_impl_wglEnumGpusNV;
    glad_debug_wglEnumerateVideoCaptureDevicesNV = glad_debug_impl_wglEnumerateVideoCaptureDevicesNV;
    glad_debug_wglEnumerateVideoDevicesNV = glad_debug_impl_wglEnumerateVideoDevicesNV;
    glad_debug_wglFreeMemoryNV = glad_debug_impl_wglFreeMemoryNV;
    glad_debug_wglGenlockSampleRateI3D = glad_debug_impl_wglGenlockSampleRateI3D;
    glad_debug_wglGenlockSourceDelayI3D = glad_debug_impl_wglGenlockSourceDelayI3D;
    glad_debug_wglGenlockSourceEdgeI3D = glad_debug_impl_wglGenlockSourceEdgeI3D;
    glad_debug_wglGenlockSourceI3D = glad_debug_impl_wglGenlockSourceI3D;
    glad_debug_wglGetContextGPUIDAMD = glad_debug_impl_wglGetContextGPUIDAMD;
    glad_debug_wglGetCurrentAssociatedContextAMD = glad_debug_impl_wglGetCurrentAssociatedContextAMD;
    glad_debug_wglGetCurrentReadDCARB = glad_debug_impl_wglGetCurrentReadDCARB;
    glad_debug_wglGetCurrentReadDCEXT = glad_debug_impl_wglGetCurrentReadDCEXT;
    glad_debug_wglGetDigitalVideoParametersI3D = glad_debug_impl_wglGetDigitalVideoParametersI3D;
    glad_debug_wglGetExtensionsStringARB = glad_debug_impl_wglGetExtensionsStringARB;
    glad_debug_wglGetExtensionsStringEXT = glad_debug_impl_wglGetExtensionsStringEXT;
    glad_debug_wglGetFrameUsageI3D = glad_debug_impl_wglGetFrameUsageI3D;
    glad_debug_wglGetGPUIDsAMD = glad_debug_impl_wglGetGPUIDsAMD;
    glad_debug_wglGetGPUInfoAMD = glad_debug_impl_wglGetGPUInfoAMD;
    glad_debug_wglGetGammaTableI3D = glad_debug_impl_wglGetGammaTableI3D;
    glad_debug_wglGetGammaTableParametersI3D = glad_debug_impl_wglGetGammaTableParametersI3D;
    glad_debug_wglGetGenlockSampleRateI3D = glad_debug_impl_wglGetGenlockSampleRateI3D;
    glad_debug_wglGetGenlockSourceDelayI3D = glad_debug_impl_wglGetGenlockSourceDelayI3D;
    glad_debug_wglGetGenlockSourceEdgeI3D = glad_debug_impl_wglGetGenlockSourceEdgeI3D;
    glad_debug_wglGetGenlockSourceI3D = glad_debug_impl_wglGetGenlockSourceI3D;
    glad_debug_wglGetMscRateOML = glad_debug_impl_wglGetMscRateOML;
    glad_debug_wglGetPbufferDCARB = glad_debug_impl_wglGetPbufferDCARB;
    glad_debug_wglGetPbufferDCEXT = glad_debug_impl_wglGetPbufferDCEXT;
    glad_debug_wglGetPixelFormatAttribfvARB = glad_debug_impl_wglGetPixelFormatAttribfvARB;
    glad_debug_wglGetPixelFormatAttribfvEXT = glad_debug_impl_wglGetPixelFormatAttribfvEXT;
    glad_debug_wglGetPixelFormatAttribivARB = glad_debug_impl_wglGetPixelFormatAttribivARB;
    glad_debug_wglGetPixelFormatAttribivEXT = glad_debug_impl_wglGetPixelFormatAttribivEXT;
    glad_debug_wglGetSwapIntervalEXT = glad_debug_impl_wglGetSwapIntervalEXT;
    glad_debug_wglGetSyncValuesOML = glad_debug_impl_wglGetSyncValuesOML;
    glad_debug_wglGetVideoDeviceNV = glad_debug_impl_wglGetVideoDeviceNV;
    glad_debug_wglGetVideoInfoNV = glad_debug_impl_wglGetVideoInfoNV;
    glad_debug_wglIsEnabledFrameLockI3D = glad_debug_impl_wglIsEnabledFrameLockI3D;
    glad_debug_wglIsEnabledGenlockI3D = glad_debug_impl_wglIsEnabledGenlockI3D;
    glad_debug_wglJoinSwapGroupNV = glad_debug_impl_wglJoinSwapGroupNV;
    glad_debug_wglLoadDisplayColorTableEXT = glad_debug_impl_wglLoadDisplayColorTableEXT;
    glad_debug_wglLockVideoCaptureDeviceNV = glad_debug_impl_wglLockVideoCaptureDeviceNV;
    glad_debug_wglMakeAssociatedContextCurrentAMD = glad_debug_impl_wglMakeAssociatedContextCurrentAMD;
    glad_debug_wglMakeContextCurrentARB = glad_debug_impl_wglMakeContextCurrentARB;
    glad_debug_wglMakeContextCurrentEXT = glad_debug_impl_wglMakeContextCurrentEXT;
    glad_debug_wglQueryCurrentContextNV = glad_debug_impl_wglQueryCurrentContextNV;
    glad_debug_wglQueryFrameCountNV = glad_debug_impl_wglQueryFrameCountNV;
    glad_debug_wglQueryFrameLockMasterI3D = glad_debug_impl_wglQueryFrameLockMasterI3D;
    glad_debug_wglQueryFrameTrackingI3D = glad_debug_impl_wglQueryFrameTrackingI3D;
    glad_debug_wglQueryGenlockMaxSourceDelayI3D = glad_debug_impl_wglQueryGenlockMaxSourceDelayI3D;
    glad_debug_wglQueryMaxSwapGroupsNV = glad_debug_impl_wglQueryMaxSwapGroupsNV;
    glad_debug_wglQueryPbufferARB = glad_debug_impl_wglQueryPbufferARB;
    glad_debug_wglQueryPbufferEXT = glad_debug_impl_wglQueryPbufferEXT;
    glad_debug_wglQuerySwapGroupNV = glad_debug_impl_wglQuerySwapGroupNV;
    glad_debug_wglQueryVideoCaptureDeviceNV = glad_debug_impl_wglQueryVideoCaptureDeviceNV;
    glad_debug_wglReleaseImageBufferEventsI3D = glad_debug_impl_wglReleaseImageBufferEventsI3D;
    glad_debug_wglReleasePbufferDCARB = glad_debug_impl_wglReleasePbufferDCARB;
    glad_debug_wglReleasePbufferDCEXT = glad_debug_impl_wglReleasePbufferDCEXT;
    glad_debug_wglReleaseTexImageARB = glad_debug_impl_wglReleaseTexImageARB;
    glad_debug_wglReleaseVideoCaptureDeviceNV = glad_debug_impl_wglReleaseVideoCaptureDeviceNV;
    glad_debug_wglReleaseVideoDeviceNV = glad_debug_impl_wglReleaseVideoDeviceNV;
    glad_debug_wglReleaseVideoImageNV = glad_debug_impl_wglReleaseVideoImageNV;
    glad_debug_wglResetFrameCountNV = glad_debug_impl_wglResetFrameCountNV;
    glad_debug_wglRestoreBufferRegionARB = glad_debug_impl_wglRestoreBufferRegionARB;
    glad_debug_wglSaveBufferRegionARB = glad_debug_impl_wglSaveBufferRegionARB;
    glad_debug_wglSendPbufferToVideoNV = glad_debug_impl_wglSendPbufferToVideoNV;
    glad_debug_wglSetDigitalVideoParametersI3D = glad_debug_impl_wglSetDigitalVideoParametersI3D;
    glad_debug_wglSetGammaTableI3D = glad_debug_impl_wglSetGammaTableI3D;
    glad_debug_wglSetGammaTableParametersI3D = glad_debug_impl_wglSetGammaTableParametersI3D;
    glad_debug_wglSetPbufferAttribARB = glad_debug_impl_wglSetPbufferAttribARB;
    glad_debug_wglSetStereoEmitterState3DL = glad_debug_impl_wglSetStereoEmitterState3DL;
    glad_debug_wglSwapBuffersMscOML = glad_debug_impl_wglSwapBuffersMscOML;
    glad_debug_wglSwapIntervalEXT = glad_debug_impl_wglSwapIntervalEXT;
    glad_debug_wglSwapLayerBuffersMscOML = glad_debug_impl_wglSwapLayerBuffersMscOML;
    glad_debug_wglWaitForMscOML = glad_debug_impl_wglWaitForMscOML;
    glad_debug_wglWaitForSbcOML = glad_debug_impl_wglWaitForSbcOML;
}

void gladUninstallWGLDebug(void) {
    glad_debug_wglAllocateMemoryNV = glad_wglAllocateMemoryNV;
    glad_debug_wglAssociateImageBufferEventsI3D = glad_wglAssociateImageBufferEventsI3D;
    glad_debug_wglBeginFrameTrackingI3D = glad_wglBeginFrameTrackingI3D;
    glad_debug_wglBindDisplayColorTableEXT = glad_wglBindDisplayColorTableEXT;
    glad_debug_wglBindSwapBarrierNV = glad_wglBindSwapBarrierNV;
    glad_debug_wglBindTexImageARB = glad_wglBindTexImageARB;
    glad_debug_wglBindVideoCaptureDeviceNV = glad_wglBindVideoCaptureDeviceNV;
    glad_debug_wglBindVideoDeviceNV = glad_wglBindVideoDeviceNV;
    glad_debug_wglBindVideoImageNV = glad_wglBindVideoImageNV;
    glad_debug_wglBlitContextFramebufferAMD = glad_wglBlitContextFramebufferAMD;
    glad_debug_wglChoosePixelFormatARB = glad_wglChoosePixelFormatARB;
    glad_debug_wglChoosePixelFormatEXT = glad_wglChoosePixelFormatEXT;
    glad_debug_wglCopyImageSubDataNV = glad_wglCopyImageSubDataNV;
    glad_debug_wglCreateAffinityDCNV = glad_wglCreateAffinityDCNV;
    glad_debug_wglCreateAssociatedContextAMD = glad_wglCreateAssociatedContextAMD;
    glad_debug_wglCreateAssociatedContextAttribsAMD = glad_wglCreateAssociatedContextAttribsAMD;
    glad_debug_wglCreateBufferRegionARB = glad_wglCreateBufferRegionARB;
    glad_debug_wglCreateContextAttribsARB = glad_wglCreateContextAttribsARB;
    glad_debug_wglCreateDisplayColorTableEXT = glad_wglCreateDisplayColorTableEXT;
    glad_debug_wglCreateImageBufferI3D = glad_wglCreateImageBufferI3D;
    glad_debug_wglCreatePbufferARB = glad_wglCreatePbufferARB;
    glad_debug_wglCreatePbufferEXT = glad_wglCreatePbufferEXT;
    glad_debug_wglDXCloseDeviceNV = glad_wglDXCloseDeviceNV;
    glad_debug_wglDXLockObjectsNV = glad_wglDXLockObjectsNV;
    glad_debug_wglDXObjectAccessNV = glad_wglDXObjectAccessNV;
    glad_debug_wglDXOpenDeviceNV = glad_wglDXOpenDeviceNV;
    glad_debug_wglDXRegisterObjectNV = glad_wglDXRegisterObjectNV;
    glad_debug_wglDXSetResourceShareHandleNV = glad_wglDXSetResourceShareHandleNV;
    glad_debug_wglDXUnlockObjectsNV = glad_wglDXUnlockObjectsNV;
    glad_debug_wglDXUnregisterObjectNV = glad_wglDXUnregisterObjectNV;
    glad_debug_wglDelayBeforeSwapNV = glad_wglDelayBeforeSwapNV;
    glad_debug_wglDeleteAssociatedContextAMD = glad_wglDeleteAssociatedContextAMD;
    glad_debug_wglDeleteBufferRegionARB = glad_wglDeleteBufferRegionARB;
    glad_debug_wglDeleteDCNV = glad_wglDeleteDCNV;
    glad_debug_wglDestroyDisplayColorTableEXT = glad_wglDestroyDisplayColorTableEXT;
    glad_debug_wglDestroyImageBufferI3D = glad_wglDestroyImageBufferI3D;
    glad_debug_wglDestroyPbufferARB = glad_wglDestroyPbufferARB;
    glad_debug_wglDestroyPbufferEXT = glad_wglDestroyPbufferEXT;
    glad_debug_wglDisableFrameLockI3D = glad_wglDisableFrameLockI3D;
    glad_debug_wglDisableGenlockI3D = glad_wglDisableGenlockI3D;
    glad_debug_wglEnableFrameLockI3D = glad_wglEnableFrameLockI3D;
    glad_debug_wglEnableGenlockI3D = glad_wglEnableGenlockI3D;
    glad_debug_wglEndFrameTrackingI3D = glad_wglEndFrameTrackingI3D;
    glad_debug_wglEnumGpuDevicesNV = glad_wglEnumGpuDevicesNV;
    glad_debug_wglEnumGpusFromAffinityDCNV = glad_wglEnumGpusFromAffinityDCNV;
    glad_debug_wglEnumGpusNV = glad_wglEnumGpusNV;
    glad_debug_wglEnumerateVideoCaptureDevicesNV = glad_wglEnumerateVideoCaptureDevicesNV;
    glad_debug_wglEnumerateVideoDevicesNV = glad_wglEnumerateVideoDevicesNV;
    glad_debug_wglFreeMemoryNV = glad_wglFreeMemoryNV;
    glad_debug_wglGenlockSampleRateI3D = glad_wglGenlockSampleRateI3D;
    glad_debug_wglGenlockSourceDelayI3D = glad_wglGenlockSourceDelayI3D;
    glad_debug_wglGenlockSourceEdgeI3D = glad_wglGenlockSourceEdgeI3D;
    glad_debug_wglGenlockSourceI3D = glad_wglGenlockSourceI3D;
    glad_debug_wglGetContextGPUIDAMD = glad_wglGetContextGPUIDAMD;
    glad_debug_wglGetCurrentAssociatedContextAMD = glad_wglGetCurrentAssociatedContextAMD;
    glad_debug_wglGetCurrentReadDCARB = glad_wglGetCurrentReadDCARB;
    glad_debug_wglGetCurrentReadDCEXT = glad_wglGetCurrentReadDCEXT;
    glad_debug_wglGetDigitalVideoParametersI3D = glad_wglGetDigitalVideoParametersI3D;
    glad_debug_wglGetExtensionsStringARB = glad_wglGetExtensionsStringARB;
    glad_debug_wglGetExtensionsStringEXT = glad_wglGetExtensionsStringEXT;
    glad_debug_wglGetFrameUsageI3D = glad_wglGetFrameUsageI3D;
    glad_debug_wglGetGPUIDsAMD = glad_wglGetGPUIDsAMD;
    glad_debug_wglGetGPUInfoAMD = glad_wglGetGPUInfoAMD;
    glad_debug_wglGetGammaTableI3D = glad_wglGetGammaTableI3D;
    glad_debug_wglGetGammaTableParametersI3D = glad_wglGetGammaTableParametersI3D;
    glad_debug_wglGetGenlockSampleRateI3D = glad_wglGetGenlockSampleRateI3D;
    glad_debug_wglGetGenlockSourceDelayI3D = glad_wglGetGenlockSourceDelayI3D;
    glad_debug_wglGetGenlockSourceEdgeI3D = glad_wglGetGenlockSourceEdgeI3D;
    glad_debug_wglGetGenlockSourceI3D = glad_wglGetGenlockSourceI3D;
    glad_debug_wglGetMscRateOML = glad_wglGetMscRateOML;
    glad_debug_wglGetPbufferDCARB = glad_wglGetPbufferDCARB;
    glad_debug_wglGetPbufferDCEXT = glad_wglGetPbufferDCEXT;
    glad_debug_wglGetPixelFormatAttribfvARB = glad_wglGetPixelFormatAttribfvARB;
    glad_debug_wglGetPixelFormatAttribfvEXT = glad_wglGetPixelFormatAttribfvEXT;
    glad_debug_wglGetPixelFormatAttribivARB = glad_wglGetPixelFormatAttribivARB;
    glad_debug_wglGetPixelFormatAttribivEXT = glad_wglGetPixelFormatAttribivEXT;
    glad_debug_wglGetSwapIntervalEXT = glad_wglGetSwapIntervalEXT;
    glad_debug_wglGetSyncValuesOML = glad_wglGetSyncValuesOML;
    glad_debug_wglGetVideoDeviceNV = glad_wglGetVideoDeviceNV;
    glad_debug_wglGetVideoInfoNV = glad_wglGetVideoInfoNV;
    glad_debug_wglIsEnabledFrameLockI3D = glad_wglIsEnabledFrameLockI3D;
    glad_debug_wglIsEnabledGenlockI3D = glad_wglIsEnabledGenlockI3D;
    glad_debug_wglJoinSwapGroupNV = glad_wglJoinSwapGroupNV;
    glad_debug_wglLoadDisplayColorTableEXT = glad_wglLoadDisplayColorTableEXT;
    glad_debug_wglLockVideoCaptureDeviceNV = glad_wglLockVideoCaptureDeviceNV;
    glad_debug_wglMakeAssociatedContextCurrentAMD = glad_wglMakeAssociatedContextCurrentAMD;
    glad_debug_wglMakeContextCurrentARB = glad_wglMakeContextCurrentARB;
    glad_debug_wglMakeContextCurrentEXT = glad_wglMakeContextCurrentEXT;
    glad_debug_wglQueryCurrentContextNV = glad_wglQueryCurrentContextNV;
    glad_debug_wglQueryFrameCountNV = glad_wglQueryFrameCountNV;
    glad_debug_wglQueryFrameLockMasterI3D = glad_wglQueryFrameLockMasterI3D;
    glad_debug_wglQueryFrameTrackingI3D = glad_wglQueryFrameTrackingI3D;
    glad_debug_wglQueryGenlockMaxSourceDelayI3D = glad_wglQueryGenlockMaxSourceDelayI3D;
    glad_debug_wglQueryMaxSwapGroupsNV = glad_wglQueryMaxSwapGroupsNV;
    glad_debug_wglQueryPbufferARB = glad_wglQueryPbufferARB;
    glad_debug_wglQueryPbufferEXT = glad_wglQueryPbufferEXT;
    glad_debug_wglQuerySwapGroupNV = glad_wglQuerySwapGroupNV;
    glad_debug_wglQueryVideoCaptureDeviceNV = glad_wglQueryVideoCaptureDeviceNV;
    glad_debug_wglReleaseImageBufferEventsI3D = glad_wglReleaseImageBufferEventsI3D;
    glad_debug_wglReleasePbufferDCARB = glad_wglReleasePbufferDCARB;
    glad_debug_wglReleasePbufferDCEXT = glad_wglReleasePbufferDCEXT;
    glad_debug_wglReleaseTexImageARB = glad_wglReleaseTexImageARB;
    glad_debug_wglReleaseVideoCaptureDeviceNV = glad_wglReleaseVideoCaptureDeviceNV;
    glad_debug_wglReleaseVideoDeviceNV = glad_wglReleaseVideoDeviceNV;
    glad_debug_wglReleaseVideoImageNV = glad_wglReleaseVideoImageNV;
    glad_debug_wglResetFrameCountNV = glad_wglResetFrameCountNV;
    glad_debug_wglRestoreBufferRegionARB = glad_wglRestoreBufferRegionARB;
    glad_debug_wglSaveBufferRegionARB = glad_wglSaveBufferRegionARB;
    glad_debug_wglSendPbufferToVideoNV = glad_wglSendPbufferToVideoNV;
    glad_debug_wglSetDigitalVideoParametersI3D = glad_wglSetDigitalVideoParametersI3D;
    glad_debug_wglSetGammaTableI3D = glad_wglSetGammaTableI3D;
    glad_debug_wglSetGammaTableParametersI3D = glad_wglSetGammaTableParametersI3D;
    glad_debug_wglSetPbufferAttribARB = glad_wglSetPbufferAttribARB;
    glad_debug_wglSetStereoEmitterState3DL = glad_wglSetStereoEmitterState3DL;
    glad_debug_wglSwapBuffersMscOML = glad_wglSwapBuffersMscOML;
    glad_debug_wglSwapIntervalEXT = glad_wglSwapIntervalEXT;
    glad_debug_wglSwapLayerBuffersMscOML = glad_wglSwapLayerBuffersMscOML;
    glad_debug_wglWaitForMscOML = glad_wglWaitForMscOML;
    glad_debug_wglWaitForSbcOML = glad_wglWaitForSbcOML;
}


#ifdef __cplusplus
}
#endif
