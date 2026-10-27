/**
 * SPDX-License-Identifier: (WTFPL OR CC0-1.0) AND Apache-2.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glad/glx.h>

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



int GLAD_GLX_VERSION_1_0 = 0;
int GLAD_GLX_VERSION_1_1 = 0;
int GLAD_GLX_VERSION_1_2 = 0;
int GLAD_GLX_VERSION_1_3 = 0;
int GLAD_GLX_VERSION_1_4 = 0;
int GLAD_GLX_3DFX_multisample = 0;
int GLAD_GLX_AMD_gpu_association = 0;
int GLAD_GLX_ARB_context_flush_control = 0;
int GLAD_GLX_ARB_create_context = 0;
int GLAD_GLX_ARB_create_context_no_error = 0;
int GLAD_GLX_ARB_create_context_profile = 0;
int GLAD_GLX_ARB_create_context_robustness = 0;
int GLAD_GLX_ARB_fbconfig_float = 0;
int GLAD_GLX_ARB_framebuffer_sRGB = 0;
int GLAD_GLX_ARB_get_proc_address = 0;
int GLAD_GLX_ARB_multisample = 0;
int GLAD_GLX_ARB_robustness_application_isolation = 0;
int GLAD_GLX_ARB_robustness_share_group_isolation = 0;
int GLAD_GLX_ARB_vertex_buffer_object = 0;
int GLAD_GLX_EXT_buffer_age = 0;
int GLAD_GLX_EXT_context_priority = 0;
int GLAD_GLX_EXT_create_context_es2_profile = 0;
int GLAD_GLX_EXT_create_context_es_profile = 0;
int GLAD_GLX_EXT_fbconfig_packed_float = 0;
int GLAD_GLX_EXT_framebuffer_sRGB = 0;
int GLAD_GLX_EXT_get_drawable_type = 0;
int GLAD_GLX_EXT_import_context = 0;
int GLAD_GLX_EXT_libglvnd = 0;
int GLAD_GLX_EXT_no_config_context = 0;
int GLAD_GLX_EXT_stereo_tree = 0;
int GLAD_GLX_EXT_swap_control = 0;
int GLAD_GLX_EXT_swap_control_tear = 0;
int GLAD_GLX_EXT_texture_from_pixmap = 0;
int GLAD_GLX_EXT_visual_info = 0;
int GLAD_GLX_EXT_visual_rating = 0;
int GLAD_GLX_INTEL_swap_event = 0;
int GLAD_GLX_MESA_agp_offset = 0;
int GLAD_GLX_MESA_copy_sub_buffer = 0;
int GLAD_GLX_MESA_pixmap_colormap = 0;
int GLAD_GLX_MESA_query_renderer = 0;
int GLAD_GLX_MESA_release_buffers = 0;
int GLAD_GLX_MESA_set_3dfx_mode = 0;
int GLAD_GLX_MESA_swap_control = 0;
int GLAD_GLX_NV_copy_buffer = 0;
int GLAD_GLX_NV_copy_image = 0;
int GLAD_GLX_NV_delay_before_swap = 0;
int GLAD_GLX_NV_float_buffer = 0;
int GLAD_GLX_NV_multigpu_context = 0;
int GLAD_GLX_NV_multisample_coverage = 0;
int GLAD_GLX_NV_present_video = 0;
int GLAD_GLX_NV_robustness_video_memory_purge = 0;
int GLAD_GLX_NV_swap_group = 0;
int GLAD_GLX_NV_video_capture = 0;
int GLAD_GLX_NV_video_out = 0;
int GLAD_GLX_OML_swap_method = 0;
int GLAD_GLX_OML_sync_control = 0;
int GLAD_GLX_SGIS_blended_overlay = 0;
int GLAD_GLX_SGIS_multisample = 0;
int GLAD_GLX_SGIS_shared_multisample = 0;
int GLAD_GLX_SGIX_fbconfig = 0;
int GLAD_GLX_SGIX_hyperpipe = 0;
int GLAD_GLX_SGIX_pbuffer = 0;
int GLAD_GLX_SGIX_swap_barrier = 0;
int GLAD_GLX_SGIX_swap_group = 0;
int GLAD_GLX_SGIX_video_resize = 0;
int GLAD_GLX_SGIX_visual_select_group = 0;
int GLAD_GLX_SGI_cushion = 0;
int GLAD_GLX_SGI_make_current_read = 0;
int GLAD_GLX_SGI_swap_control = 0;
int GLAD_GLX_SGI_video_sync = 0;
int GLAD_GLX_SUN_get_transparent_index = 0;


static void _pre_call_glx_callback_default(const char *name, GLADapiproc apiproc, int len_args, ...) {
    GLAD_UNUSED(name);
    GLAD_UNUSED(apiproc);
    GLAD_UNUSED(len_args);
}
static void _post_call_glx_callback_default(void *ret, const char *name, GLADapiproc apiproc, int len_args, ...) {
    GLAD_UNUSED(ret);
    GLAD_UNUSED(name);
    GLAD_UNUSED(apiproc);
    GLAD_UNUSED(len_args);
}

static GLADprecallback _pre_call_glx_callback = _pre_call_glx_callback_default;
void gladSetGLXPreCallback(GLADprecallback cb) {
    _pre_call_glx_callback = cb;
}
static GLADpostcallback _post_call_glx_callback = _post_call_glx_callback_default;
void gladSetGLXPostCallback(GLADpostcallback cb) {
    _post_call_glx_callback = cb;
}

PFNGLXBINDCHANNELTOWINDOWSGIXPROC glad_glXBindChannelToWindowSGIX = NULL;
static int GLAD_API_PTR glad_debug_impl_glXBindChannelToWindowSGIX(Display * display, int screen, int channel, Window window) {
    int ret;
    _pre_call_glx_callback("glXBindChannelToWindowSGIX", (GLADapiproc) glad_glXBindChannelToWindowSGIX, 4, display, screen, channel, window);
    ret = glad_glXBindChannelToWindowSGIX(display, screen, channel, window);
    _post_call_glx_callback((void*) &ret, "glXBindChannelToWindowSGIX", (GLADapiproc) glad_glXBindChannelToWindowSGIX, 4, display, screen, channel, window);
    return ret;
}
PFNGLXBINDCHANNELTOWINDOWSGIXPROC glad_debug_glXBindChannelToWindowSGIX = glad_debug_impl_glXBindChannelToWindowSGIX;
PFNGLXBINDHYPERPIPESGIXPROC glad_glXBindHyperpipeSGIX = NULL;
static int GLAD_API_PTR glad_debug_impl_glXBindHyperpipeSGIX(Display * dpy, int hpId) {
    int ret;
    _pre_call_glx_callback("glXBindHyperpipeSGIX", (GLADapiproc) glad_glXBindHyperpipeSGIX, 2, dpy, hpId);
    ret = glad_glXBindHyperpipeSGIX(dpy, hpId);
    _post_call_glx_callback((void*) &ret, "glXBindHyperpipeSGIX", (GLADapiproc) glad_glXBindHyperpipeSGIX, 2, dpy, hpId);
    return ret;
}
PFNGLXBINDHYPERPIPESGIXPROC glad_debug_glXBindHyperpipeSGIX = glad_debug_impl_glXBindHyperpipeSGIX;
PFNGLXBINDSWAPBARRIERNVPROC glad_glXBindSwapBarrierNV = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXBindSwapBarrierNV(Display * dpy, GLuint group, GLuint barrier) {
    Bool ret;
    _pre_call_glx_callback("glXBindSwapBarrierNV", (GLADapiproc) glad_glXBindSwapBarrierNV, 3, dpy, group, barrier);
    ret = glad_glXBindSwapBarrierNV(dpy, group, barrier);
    _post_call_glx_callback((void*) &ret, "glXBindSwapBarrierNV", (GLADapiproc) glad_glXBindSwapBarrierNV, 3, dpy, group, barrier);
    return ret;
}
PFNGLXBINDSWAPBARRIERNVPROC glad_debug_glXBindSwapBarrierNV = glad_debug_impl_glXBindSwapBarrierNV;
PFNGLXBINDSWAPBARRIERSGIXPROC glad_glXBindSwapBarrierSGIX = NULL;
static void GLAD_API_PTR glad_debug_impl_glXBindSwapBarrierSGIX(Display * dpy, GLXDrawable drawable, int barrier) {
    _pre_call_glx_callback("glXBindSwapBarrierSGIX", (GLADapiproc) glad_glXBindSwapBarrierSGIX, 3, dpy, drawable, barrier);
    glad_glXBindSwapBarrierSGIX(dpy, drawable, barrier);
    _post_call_glx_callback(NULL, "glXBindSwapBarrierSGIX", (GLADapiproc) glad_glXBindSwapBarrierSGIX, 3, dpy, drawable, barrier);
    
}
PFNGLXBINDSWAPBARRIERSGIXPROC glad_debug_glXBindSwapBarrierSGIX = glad_debug_impl_glXBindSwapBarrierSGIX;
PFNGLXBINDTEXIMAGEEXTPROC glad_glXBindTexImageEXT = NULL;
static void GLAD_API_PTR glad_debug_impl_glXBindTexImageEXT(Display * dpy, GLXDrawable drawable, int buffer, const int * attrib_list) {
    _pre_call_glx_callback("glXBindTexImageEXT", (GLADapiproc) glad_glXBindTexImageEXT, 4, dpy, drawable, buffer, attrib_list);
    glad_glXBindTexImageEXT(dpy, drawable, buffer, attrib_list);
    _post_call_glx_callback(NULL, "glXBindTexImageEXT", (GLADapiproc) glad_glXBindTexImageEXT, 4, dpy, drawable, buffer, attrib_list);
    
}
PFNGLXBINDTEXIMAGEEXTPROC glad_debug_glXBindTexImageEXT = glad_debug_impl_glXBindTexImageEXT;
PFNGLXBINDVIDEOCAPTUREDEVICENVPROC glad_glXBindVideoCaptureDeviceNV = NULL;
static int GLAD_API_PTR glad_debug_impl_glXBindVideoCaptureDeviceNV(Display * dpy, unsigned int video_capture_slot, GLXVideoCaptureDeviceNV device) {
    int ret;
    _pre_call_glx_callback("glXBindVideoCaptureDeviceNV", (GLADapiproc) glad_glXBindVideoCaptureDeviceNV, 3, dpy, video_capture_slot, device);
    ret = glad_glXBindVideoCaptureDeviceNV(dpy, video_capture_slot, device);
    _post_call_glx_callback((void*) &ret, "glXBindVideoCaptureDeviceNV", (GLADapiproc) glad_glXBindVideoCaptureDeviceNV, 3, dpy, video_capture_slot, device);
    return ret;
}
PFNGLXBINDVIDEOCAPTUREDEVICENVPROC glad_debug_glXBindVideoCaptureDeviceNV = glad_debug_impl_glXBindVideoCaptureDeviceNV;
PFNGLXBINDVIDEODEVICENVPROC glad_glXBindVideoDeviceNV = NULL;
static int GLAD_API_PTR glad_debug_impl_glXBindVideoDeviceNV(Display * dpy, unsigned int video_slot, unsigned int video_device, const int * attrib_list) {
    int ret;
    _pre_call_glx_callback("glXBindVideoDeviceNV", (GLADapiproc) glad_glXBindVideoDeviceNV, 4, dpy, video_slot, video_device, attrib_list);
    ret = glad_glXBindVideoDeviceNV(dpy, video_slot, video_device, attrib_list);
    _post_call_glx_callback((void*) &ret, "glXBindVideoDeviceNV", (GLADapiproc) glad_glXBindVideoDeviceNV, 4, dpy, video_slot, video_device, attrib_list);
    return ret;
}
PFNGLXBINDVIDEODEVICENVPROC glad_debug_glXBindVideoDeviceNV = glad_debug_impl_glXBindVideoDeviceNV;
PFNGLXBINDVIDEOIMAGENVPROC glad_glXBindVideoImageNV = NULL;
static int GLAD_API_PTR glad_debug_impl_glXBindVideoImageNV(Display * dpy, GLXVideoDeviceNV VideoDevice, GLXPbuffer pbuf, int iVideoBuffer) {
    int ret;
    _pre_call_glx_callback("glXBindVideoImageNV", (GLADapiproc) glad_glXBindVideoImageNV, 4, dpy, VideoDevice, pbuf, iVideoBuffer);
    ret = glad_glXBindVideoImageNV(dpy, VideoDevice, pbuf, iVideoBuffer);
    _post_call_glx_callback((void*) &ret, "glXBindVideoImageNV", (GLADapiproc) glad_glXBindVideoImageNV, 4, dpy, VideoDevice, pbuf, iVideoBuffer);
    return ret;
}
PFNGLXBINDVIDEOIMAGENVPROC glad_debug_glXBindVideoImageNV = glad_debug_impl_glXBindVideoImageNV;
PFNGLXBLITCONTEXTFRAMEBUFFERAMDPROC glad_glXBlitContextFramebufferAMD = NULL;
static void GLAD_API_PTR glad_debug_impl_glXBlitContextFramebufferAMD(GLXContext dstCtx, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) {
    _pre_call_glx_callback("glXBlitContextFramebufferAMD", (GLADapiproc) glad_glXBlitContextFramebufferAMD, 11, dstCtx, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
    glad_glXBlitContextFramebufferAMD(dstCtx, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
    _post_call_glx_callback(NULL, "glXBlitContextFramebufferAMD", (GLADapiproc) glad_glXBlitContextFramebufferAMD, 11, dstCtx, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
    
}
PFNGLXBLITCONTEXTFRAMEBUFFERAMDPROC glad_debug_glXBlitContextFramebufferAMD = glad_debug_impl_glXBlitContextFramebufferAMD;
PFNGLXCHANNELRECTSGIXPROC glad_glXChannelRectSGIX = NULL;
static int GLAD_API_PTR glad_debug_impl_glXChannelRectSGIX(Display * display, int screen, int channel, int x, int y, int w, int h) {
    int ret;
    _pre_call_glx_callback("glXChannelRectSGIX", (GLADapiproc) glad_glXChannelRectSGIX, 7, display, screen, channel, x, y, w, h);
    ret = glad_glXChannelRectSGIX(display, screen, channel, x, y, w, h);
    _post_call_glx_callback((void*) &ret, "glXChannelRectSGIX", (GLADapiproc) glad_glXChannelRectSGIX, 7, display, screen, channel, x, y, w, h);
    return ret;
}
PFNGLXCHANNELRECTSGIXPROC glad_debug_glXChannelRectSGIX = glad_debug_impl_glXChannelRectSGIX;
PFNGLXCHANNELRECTSYNCSGIXPROC glad_glXChannelRectSyncSGIX = NULL;
static int GLAD_API_PTR glad_debug_impl_glXChannelRectSyncSGIX(Display * display, int screen, int channel, GLenum synctype) {
    int ret;
    _pre_call_glx_callback("glXChannelRectSyncSGIX", (GLADapiproc) glad_glXChannelRectSyncSGIX, 4, display, screen, channel, synctype);
    ret = glad_glXChannelRectSyncSGIX(display, screen, channel, synctype);
    _post_call_glx_callback((void*) &ret, "glXChannelRectSyncSGIX", (GLADapiproc) glad_glXChannelRectSyncSGIX, 4, display, screen, channel, synctype);
    return ret;
}
PFNGLXCHANNELRECTSYNCSGIXPROC glad_debug_glXChannelRectSyncSGIX = glad_debug_impl_glXChannelRectSyncSGIX;
PFNGLXCHOOSEFBCONFIGPROC glad_glXChooseFBConfig = NULL;
static GLXFBConfig * GLAD_API_PTR glad_debug_impl_glXChooseFBConfig(Display * dpy, int screen, const int * attrib_list, int * nelements) {
    GLXFBConfig * ret;
    _pre_call_glx_callback("glXChooseFBConfig", (GLADapiproc) glad_glXChooseFBConfig, 4, dpy, screen, attrib_list, nelements);
    ret = glad_glXChooseFBConfig(dpy, screen, attrib_list, nelements);
    _post_call_glx_callback((void*) &ret, "glXChooseFBConfig", (GLADapiproc) glad_glXChooseFBConfig, 4, dpy, screen, attrib_list, nelements);
    return ret;
}
PFNGLXCHOOSEFBCONFIGPROC glad_debug_glXChooseFBConfig = glad_debug_impl_glXChooseFBConfig;
PFNGLXCHOOSEFBCONFIGSGIXPROC glad_glXChooseFBConfigSGIX = NULL;
static GLXFBConfigSGIX * GLAD_API_PTR glad_debug_impl_glXChooseFBConfigSGIX(Display * dpy, int screen, int * attrib_list, int * nelements) {
    GLXFBConfigSGIX * ret;
    _pre_call_glx_callback("glXChooseFBConfigSGIX", (GLADapiproc) glad_glXChooseFBConfigSGIX, 4, dpy, screen, attrib_list, nelements);
    ret = glad_glXChooseFBConfigSGIX(dpy, screen, attrib_list, nelements);
    _post_call_glx_callback((void*) &ret, "glXChooseFBConfigSGIX", (GLADapiproc) glad_glXChooseFBConfigSGIX, 4, dpy, screen, attrib_list, nelements);
    return ret;
}
PFNGLXCHOOSEFBCONFIGSGIXPROC glad_debug_glXChooseFBConfigSGIX = glad_debug_impl_glXChooseFBConfigSGIX;
PFNGLXCHOOSEVISUALPROC glad_glXChooseVisual = NULL;
static XVisualInfo * GLAD_API_PTR glad_debug_impl_glXChooseVisual(Display * dpy, int screen, int * attribList) {
    XVisualInfo * ret;
    _pre_call_glx_callback("glXChooseVisual", (GLADapiproc) glad_glXChooseVisual, 3, dpy, screen, attribList);
    ret = glad_glXChooseVisual(dpy, screen, attribList);
    _post_call_glx_callback((void*) &ret, "glXChooseVisual", (GLADapiproc) glad_glXChooseVisual, 3, dpy, screen, attribList);
    return ret;
}
PFNGLXCHOOSEVISUALPROC glad_debug_glXChooseVisual = glad_debug_impl_glXChooseVisual;
PFNGLXCOPYBUFFERSUBDATANVPROC glad_glXCopyBufferSubDataNV = NULL;
static void GLAD_API_PTR glad_debug_impl_glXCopyBufferSubDataNV(Display * dpy, GLXContext readCtx, GLXContext writeCtx, GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) {
    _pre_call_glx_callback("glXCopyBufferSubDataNV", (GLADapiproc) glad_glXCopyBufferSubDataNV, 8, dpy, readCtx, writeCtx, readTarget, writeTarget, readOffset, writeOffset, size);
    glad_glXCopyBufferSubDataNV(dpy, readCtx, writeCtx, readTarget, writeTarget, readOffset, writeOffset, size);
    _post_call_glx_callback(NULL, "glXCopyBufferSubDataNV", (GLADapiproc) glad_glXCopyBufferSubDataNV, 8, dpy, readCtx, writeCtx, readTarget, writeTarget, readOffset, writeOffset, size);
    
}
PFNGLXCOPYBUFFERSUBDATANVPROC glad_debug_glXCopyBufferSubDataNV = glad_debug_impl_glXCopyBufferSubDataNV;
PFNGLXCOPYCONTEXTPROC glad_glXCopyContext = NULL;
static void GLAD_API_PTR glad_debug_impl_glXCopyContext(Display * dpy, GLXContext src, GLXContext dst, unsigned long mask) {
    _pre_call_glx_callback("glXCopyContext", (GLADapiproc) glad_glXCopyContext, 4, dpy, src, dst, mask);
    glad_glXCopyContext(dpy, src, dst, mask);
    _post_call_glx_callback(NULL, "glXCopyContext", (GLADapiproc) glad_glXCopyContext, 4, dpy, src, dst, mask);
    
}
PFNGLXCOPYCONTEXTPROC glad_debug_glXCopyContext = glad_debug_impl_glXCopyContext;
PFNGLXCOPYIMAGESUBDATANVPROC glad_glXCopyImageSubDataNV = NULL;
static void GLAD_API_PTR glad_debug_impl_glXCopyImageSubDataNV(Display * dpy, GLXContext srcCtx, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLXContext dstCtx, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth) {
    _pre_call_glx_callback("glXCopyImageSubDataNV", (GLADapiproc) glad_glXCopyImageSubDataNV, 18, dpy, srcCtx, srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstCtx, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, width, height, depth);
    glad_glXCopyImageSubDataNV(dpy, srcCtx, srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstCtx, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, width, height, depth);
    _post_call_glx_callback(NULL, "glXCopyImageSubDataNV", (GLADapiproc) glad_glXCopyImageSubDataNV, 18, dpy, srcCtx, srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstCtx, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, width, height, depth);
    
}
PFNGLXCOPYIMAGESUBDATANVPROC glad_debug_glXCopyImageSubDataNV = glad_debug_impl_glXCopyImageSubDataNV;
PFNGLXCOPYSUBBUFFERMESAPROC glad_glXCopySubBufferMESA = NULL;
static void GLAD_API_PTR glad_debug_impl_glXCopySubBufferMESA(Display * dpy, GLXDrawable drawable, int x, int y, int width, int height) {
    _pre_call_glx_callback("glXCopySubBufferMESA", (GLADapiproc) glad_glXCopySubBufferMESA, 6, dpy, drawable, x, y, width, height);
    glad_glXCopySubBufferMESA(dpy, drawable, x, y, width, height);
    _post_call_glx_callback(NULL, "glXCopySubBufferMESA", (GLADapiproc) glad_glXCopySubBufferMESA, 6, dpy, drawable, x, y, width, height);
    
}
PFNGLXCOPYSUBBUFFERMESAPROC glad_debug_glXCopySubBufferMESA = glad_debug_impl_glXCopySubBufferMESA;
PFNGLXCREATEASSOCIATEDCONTEXTAMDPROC glad_glXCreateAssociatedContextAMD = NULL;
static GLXContext GLAD_API_PTR glad_debug_impl_glXCreateAssociatedContextAMD(unsigned int id, GLXContext share_list) {
    GLXContext ret;
    _pre_call_glx_callback("glXCreateAssociatedContextAMD", (GLADapiproc) glad_glXCreateAssociatedContextAMD, 2, id, share_list);
    ret = glad_glXCreateAssociatedContextAMD(id, share_list);
    _post_call_glx_callback((void*) &ret, "glXCreateAssociatedContextAMD", (GLADapiproc) glad_glXCreateAssociatedContextAMD, 2, id, share_list);
    return ret;
}
PFNGLXCREATEASSOCIATEDCONTEXTAMDPROC glad_debug_glXCreateAssociatedContextAMD = glad_debug_impl_glXCreateAssociatedContextAMD;
PFNGLXCREATEASSOCIATEDCONTEXTATTRIBSAMDPROC glad_glXCreateAssociatedContextAttribsAMD = NULL;
static GLXContext GLAD_API_PTR glad_debug_impl_glXCreateAssociatedContextAttribsAMD(unsigned int id, GLXContext share_context, const int * attribList) {
    GLXContext ret;
    _pre_call_glx_callback("glXCreateAssociatedContextAttribsAMD", (GLADapiproc) glad_glXCreateAssociatedContextAttribsAMD, 3, id, share_context, attribList);
    ret = glad_glXCreateAssociatedContextAttribsAMD(id, share_context, attribList);
    _post_call_glx_callback((void*) &ret, "glXCreateAssociatedContextAttribsAMD", (GLADapiproc) glad_glXCreateAssociatedContextAttribsAMD, 3, id, share_context, attribList);
    return ret;
}
PFNGLXCREATEASSOCIATEDCONTEXTATTRIBSAMDPROC glad_debug_glXCreateAssociatedContextAttribsAMD = glad_debug_impl_glXCreateAssociatedContextAttribsAMD;
PFNGLXCREATECONTEXTPROC glad_glXCreateContext = NULL;
static GLXContext GLAD_API_PTR glad_debug_impl_glXCreateContext(Display * dpy, XVisualInfo * vis, GLXContext shareList, Bool direct) {
    GLXContext ret;
    _pre_call_glx_callback("glXCreateContext", (GLADapiproc) glad_glXCreateContext, 4, dpy, vis, shareList, direct);
    ret = glad_glXCreateContext(dpy, vis, shareList, direct);
    _post_call_glx_callback((void*) &ret, "glXCreateContext", (GLADapiproc) glad_glXCreateContext, 4, dpy, vis, shareList, direct);
    return ret;
}
PFNGLXCREATECONTEXTPROC glad_debug_glXCreateContext = glad_debug_impl_glXCreateContext;
PFNGLXCREATECONTEXTATTRIBSARBPROC glad_glXCreateContextAttribsARB = NULL;
static GLXContext GLAD_API_PTR glad_debug_impl_glXCreateContextAttribsARB(Display * dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int * attrib_list) {
    GLXContext ret;
    _pre_call_glx_callback("glXCreateContextAttribsARB", (GLADapiproc) glad_glXCreateContextAttribsARB, 5, dpy, config, share_context, direct, attrib_list);
    ret = glad_glXCreateContextAttribsARB(dpy, config, share_context, direct, attrib_list);
    _post_call_glx_callback((void*) &ret, "glXCreateContextAttribsARB", (GLADapiproc) glad_glXCreateContextAttribsARB, 5, dpy, config, share_context, direct, attrib_list);
    return ret;
}
PFNGLXCREATECONTEXTATTRIBSARBPROC glad_debug_glXCreateContextAttribsARB = glad_debug_impl_glXCreateContextAttribsARB;
PFNGLXCREATECONTEXTWITHCONFIGSGIXPROC glad_glXCreateContextWithConfigSGIX = NULL;
static GLXContext GLAD_API_PTR glad_debug_impl_glXCreateContextWithConfigSGIX(Display * dpy, GLXFBConfigSGIX config, int render_type, GLXContext share_list, Bool direct) {
    GLXContext ret;
    _pre_call_glx_callback("glXCreateContextWithConfigSGIX", (GLADapiproc) glad_glXCreateContextWithConfigSGIX, 5, dpy, config, render_type, share_list, direct);
    ret = glad_glXCreateContextWithConfigSGIX(dpy, config, render_type, share_list, direct);
    _post_call_glx_callback((void*) &ret, "glXCreateContextWithConfigSGIX", (GLADapiproc) glad_glXCreateContextWithConfigSGIX, 5, dpy, config, render_type, share_list, direct);
    return ret;
}
PFNGLXCREATECONTEXTWITHCONFIGSGIXPROC glad_debug_glXCreateContextWithConfigSGIX = glad_debug_impl_glXCreateContextWithConfigSGIX;
PFNGLXCREATEGLXPBUFFERSGIXPROC glad_glXCreateGLXPbufferSGIX = NULL;
static GLXPbufferSGIX GLAD_API_PTR glad_debug_impl_glXCreateGLXPbufferSGIX(Display * dpy, GLXFBConfigSGIX config, unsigned int width, unsigned int height, int * attrib_list) {
    GLXPbufferSGIX ret;
    _pre_call_glx_callback("glXCreateGLXPbufferSGIX", (GLADapiproc) glad_glXCreateGLXPbufferSGIX, 5, dpy, config, width, height, attrib_list);
    ret = glad_glXCreateGLXPbufferSGIX(dpy, config, width, height, attrib_list);
    _post_call_glx_callback((void*) &ret, "glXCreateGLXPbufferSGIX", (GLADapiproc) glad_glXCreateGLXPbufferSGIX, 5, dpy, config, width, height, attrib_list);
    return ret;
}
PFNGLXCREATEGLXPBUFFERSGIXPROC glad_debug_glXCreateGLXPbufferSGIX = glad_debug_impl_glXCreateGLXPbufferSGIX;
PFNGLXCREATEGLXPIXMAPPROC glad_glXCreateGLXPixmap = NULL;
static GLXPixmap GLAD_API_PTR glad_debug_impl_glXCreateGLXPixmap(Display * dpy, XVisualInfo * visual, Pixmap pixmap) {
    GLXPixmap ret;
    _pre_call_glx_callback("glXCreateGLXPixmap", (GLADapiproc) glad_glXCreateGLXPixmap, 3, dpy, visual, pixmap);
    ret = glad_glXCreateGLXPixmap(dpy, visual, pixmap);
    _post_call_glx_callback((void*) &ret, "glXCreateGLXPixmap", (GLADapiproc) glad_glXCreateGLXPixmap, 3, dpy, visual, pixmap);
    return ret;
}
PFNGLXCREATEGLXPIXMAPPROC glad_debug_glXCreateGLXPixmap = glad_debug_impl_glXCreateGLXPixmap;
PFNGLXCREATEGLXPIXMAPMESAPROC glad_glXCreateGLXPixmapMESA = NULL;
static GLXPixmap GLAD_API_PTR glad_debug_impl_glXCreateGLXPixmapMESA(Display * dpy, XVisualInfo * visual, Pixmap pixmap, Colormap cmap) {
    GLXPixmap ret;
    _pre_call_glx_callback("glXCreateGLXPixmapMESA", (GLADapiproc) glad_glXCreateGLXPixmapMESA, 4, dpy, visual, pixmap, cmap);
    ret = glad_glXCreateGLXPixmapMESA(dpy, visual, pixmap, cmap);
    _post_call_glx_callback((void*) &ret, "glXCreateGLXPixmapMESA", (GLADapiproc) glad_glXCreateGLXPixmapMESA, 4, dpy, visual, pixmap, cmap);
    return ret;
}
PFNGLXCREATEGLXPIXMAPMESAPROC glad_debug_glXCreateGLXPixmapMESA = glad_debug_impl_glXCreateGLXPixmapMESA;
PFNGLXCREATEGLXPIXMAPWITHCONFIGSGIXPROC glad_glXCreateGLXPixmapWithConfigSGIX = NULL;
static GLXPixmap GLAD_API_PTR glad_debug_impl_glXCreateGLXPixmapWithConfigSGIX(Display * dpy, GLXFBConfigSGIX config, Pixmap pixmap) {
    GLXPixmap ret;
    _pre_call_glx_callback("glXCreateGLXPixmapWithConfigSGIX", (GLADapiproc) glad_glXCreateGLXPixmapWithConfigSGIX, 3, dpy, config, pixmap);
    ret = glad_glXCreateGLXPixmapWithConfigSGIX(dpy, config, pixmap);
    _post_call_glx_callback((void*) &ret, "glXCreateGLXPixmapWithConfigSGIX", (GLADapiproc) glad_glXCreateGLXPixmapWithConfigSGIX, 3, dpy, config, pixmap);
    return ret;
}
PFNGLXCREATEGLXPIXMAPWITHCONFIGSGIXPROC glad_debug_glXCreateGLXPixmapWithConfigSGIX = glad_debug_impl_glXCreateGLXPixmapWithConfigSGIX;
PFNGLXCREATENEWCONTEXTPROC glad_glXCreateNewContext = NULL;
static GLXContext GLAD_API_PTR glad_debug_impl_glXCreateNewContext(Display * dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct) {
    GLXContext ret;
    _pre_call_glx_callback("glXCreateNewContext", (GLADapiproc) glad_glXCreateNewContext, 5, dpy, config, render_type, share_list, direct);
    ret = glad_glXCreateNewContext(dpy, config, render_type, share_list, direct);
    _post_call_glx_callback((void*) &ret, "glXCreateNewContext", (GLADapiproc) glad_glXCreateNewContext, 5, dpy, config, render_type, share_list, direct);
    return ret;
}
PFNGLXCREATENEWCONTEXTPROC glad_debug_glXCreateNewContext = glad_debug_impl_glXCreateNewContext;
PFNGLXCREATEPBUFFERPROC glad_glXCreatePbuffer = NULL;
static GLXPbuffer GLAD_API_PTR glad_debug_impl_glXCreatePbuffer(Display * dpy, GLXFBConfig config, const int * attrib_list) {
    GLXPbuffer ret;
    _pre_call_glx_callback("glXCreatePbuffer", (GLADapiproc) glad_glXCreatePbuffer, 3, dpy, config, attrib_list);
    ret = glad_glXCreatePbuffer(dpy, config, attrib_list);
    _post_call_glx_callback((void*) &ret, "glXCreatePbuffer", (GLADapiproc) glad_glXCreatePbuffer, 3, dpy, config, attrib_list);
    return ret;
}
PFNGLXCREATEPBUFFERPROC glad_debug_glXCreatePbuffer = glad_debug_impl_glXCreatePbuffer;
PFNGLXCREATEPIXMAPPROC glad_glXCreatePixmap = NULL;
static GLXPixmap GLAD_API_PTR glad_debug_impl_glXCreatePixmap(Display * dpy, GLXFBConfig config, Pixmap pixmap, const int * attrib_list) {
    GLXPixmap ret;
    _pre_call_glx_callback("glXCreatePixmap", (GLADapiproc) glad_glXCreatePixmap, 4, dpy, config, pixmap, attrib_list);
    ret = glad_glXCreatePixmap(dpy, config, pixmap, attrib_list);
    _post_call_glx_callback((void*) &ret, "glXCreatePixmap", (GLADapiproc) glad_glXCreatePixmap, 4, dpy, config, pixmap, attrib_list);
    return ret;
}
PFNGLXCREATEPIXMAPPROC glad_debug_glXCreatePixmap = glad_debug_impl_glXCreatePixmap;
PFNGLXCREATEWINDOWPROC glad_glXCreateWindow = NULL;
static GLXWindow GLAD_API_PTR glad_debug_impl_glXCreateWindow(Display * dpy, GLXFBConfig config, Window win, const int * attrib_list) {
    GLXWindow ret;
    _pre_call_glx_callback("glXCreateWindow", (GLADapiproc) glad_glXCreateWindow, 4, dpy, config, win, attrib_list);
    ret = glad_glXCreateWindow(dpy, config, win, attrib_list);
    _post_call_glx_callback((void*) &ret, "glXCreateWindow", (GLADapiproc) glad_glXCreateWindow, 4, dpy, config, win, attrib_list);
    return ret;
}
PFNGLXCREATEWINDOWPROC glad_debug_glXCreateWindow = glad_debug_impl_glXCreateWindow;
PFNGLXCUSHIONSGIPROC glad_glXCushionSGI = NULL;
static void GLAD_API_PTR glad_debug_impl_glXCushionSGI(Display * dpy, Window window, float cushion) {
    _pre_call_glx_callback("glXCushionSGI", (GLADapiproc) glad_glXCushionSGI, 3, dpy, window, cushion);
    glad_glXCushionSGI(dpy, window, cushion);
    _post_call_glx_callback(NULL, "glXCushionSGI", (GLADapiproc) glad_glXCushionSGI, 3, dpy, window, cushion);
    
}
PFNGLXCUSHIONSGIPROC glad_debug_glXCushionSGI = glad_debug_impl_glXCushionSGI;
PFNGLXDELAYBEFORESWAPNVPROC glad_glXDelayBeforeSwapNV = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXDelayBeforeSwapNV(Display * dpy, GLXDrawable drawable, GLfloat seconds) {
    Bool ret;
    _pre_call_glx_callback("glXDelayBeforeSwapNV", (GLADapiproc) glad_glXDelayBeforeSwapNV, 3, dpy, drawable, seconds);
    ret = glad_glXDelayBeforeSwapNV(dpy, drawable, seconds);
    _post_call_glx_callback((void*) &ret, "glXDelayBeforeSwapNV", (GLADapiproc) glad_glXDelayBeforeSwapNV, 3, dpy, drawable, seconds);
    return ret;
}
PFNGLXDELAYBEFORESWAPNVPROC glad_debug_glXDelayBeforeSwapNV = glad_debug_impl_glXDelayBeforeSwapNV;
PFNGLXDELETEASSOCIATEDCONTEXTAMDPROC glad_glXDeleteAssociatedContextAMD = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXDeleteAssociatedContextAMD(GLXContext ctx) {
    Bool ret;
    _pre_call_glx_callback("glXDeleteAssociatedContextAMD", (GLADapiproc) glad_glXDeleteAssociatedContextAMD, 1, ctx);
    ret = glad_glXDeleteAssociatedContextAMD(ctx);
    _post_call_glx_callback((void*) &ret, "glXDeleteAssociatedContextAMD", (GLADapiproc) glad_glXDeleteAssociatedContextAMD, 1, ctx);
    return ret;
}
PFNGLXDELETEASSOCIATEDCONTEXTAMDPROC glad_debug_glXDeleteAssociatedContextAMD = glad_debug_impl_glXDeleteAssociatedContextAMD;
PFNGLXDESTROYCONTEXTPROC glad_glXDestroyContext = NULL;
static void GLAD_API_PTR glad_debug_impl_glXDestroyContext(Display * dpy, GLXContext ctx) {
    _pre_call_glx_callback("glXDestroyContext", (GLADapiproc) glad_glXDestroyContext, 2, dpy, ctx);
    glad_glXDestroyContext(dpy, ctx);
    _post_call_glx_callback(NULL, "glXDestroyContext", (GLADapiproc) glad_glXDestroyContext, 2, dpy, ctx);
    
}
PFNGLXDESTROYCONTEXTPROC glad_debug_glXDestroyContext = glad_debug_impl_glXDestroyContext;
PFNGLXDESTROYGLXPBUFFERSGIXPROC glad_glXDestroyGLXPbufferSGIX = NULL;
static void GLAD_API_PTR glad_debug_impl_glXDestroyGLXPbufferSGIX(Display * dpy, GLXPbufferSGIX pbuf) {
    _pre_call_glx_callback("glXDestroyGLXPbufferSGIX", (GLADapiproc) glad_glXDestroyGLXPbufferSGIX, 2, dpy, pbuf);
    glad_glXDestroyGLXPbufferSGIX(dpy, pbuf);
    _post_call_glx_callback(NULL, "glXDestroyGLXPbufferSGIX", (GLADapiproc) glad_glXDestroyGLXPbufferSGIX, 2, dpy, pbuf);
    
}
PFNGLXDESTROYGLXPBUFFERSGIXPROC glad_debug_glXDestroyGLXPbufferSGIX = glad_debug_impl_glXDestroyGLXPbufferSGIX;
PFNGLXDESTROYGLXPIXMAPPROC glad_glXDestroyGLXPixmap = NULL;
static void GLAD_API_PTR glad_debug_impl_glXDestroyGLXPixmap(Display * dpy, GLXPixmap pixmap) {
    _pre_call_glx_callback("glXDestroyGLXPixmap", (GLADapiproc) glad_glXDestroyGLXPixmap, 2, dpy, pixmap);
    glad_glXDestroyGLXPixmap(dpy, pixmap);
    _post_call_glx_callback(NULL, "glXDestroyGLXPixmap", (GLADapiproc) glad_glXDestroyGLXPixmap, 2, dpy, pixmap);
    
}
PFNGLXDESTROYGLXPIXMAPPROC glad_debug_glXDestroyGLXPixmap = glad_debug_impl_glXDestroyGLXPixmap;
PFNGLXDESTROYHYPERPIPECONFIGSGIXPROC glad_glXDestroyHyperpipeConfigSGIX = NULL;
static int GLAD_API_PTR glad_debug_impl_glXDestroyHyperpipeConfigSGIX(Display * dpy, int hpId) {
    int ret;
    _pre_call_glx_callback("glXDestroyHyperpipeConfigSGIX", (GLADapiproc) glad_glXDestroyHyperpipeConfigSGIX, 2, dpy, hpId);
    ret = glad_glXDestroyHyperpipeConfigSGIX(dpy, hpId);
    _post_call_glx_callback((void*) &ret, "glXDestroyHyperpipeConfigSGIX", (GLADapiproc) glad_glXDestroyHyperpipeConfigSGIX, 2, dpy, hpId);
    return ret;
}
PFNGLXDESTROYHYPERPIPECONFIGSGIXPROC glad_debug_glXDestroyHyperpipeConfigSGIX = glad_debug_impl_glXDestroyHyperpipeConfigSGIX;
PFNGLXDESTROYPBUFFERPROC glad_glXDestroyPbuffer = NULL;
static void GLAD_API_PTR glad_debug_impl_glXDestroyPbuffer(Display * dpy, GLXPbuffer pbuf) {
    _pre_call_glx_callback("glXDestroyPbuffer", (GLADapiproc) glad_glXDestroyPbuffer, 2, dpy, pbuf);
    glad_glXDestroyPbuffer(dpy, pbuf);
    _post_call_glx_callback(NULL, "glXDestroyPbuffer", (GLADapiproc) glad_glXDestroyPbuffer, 2, dpy, pbuf);
    
}
PFNGLXDESTROYPBUFFERPROC glad_debug_glXDestroyPbuffer = glad_debug_impl_glXDestroyPbuffer;
PFNGLXDESTROYPIXMAPPROC glad_glXDestroyPixmap = NULL;
static void GLAD_API_PTR glad_debug_impl_glXDestroyPixmap(Display * dpy, GLXPixmap pixmap) {
    _pre_call_glx_callback("glXDestroyPixmap", (GLADapiproc) glad_glXDestroyPixmap, 2, dpy, pixmap);
    glad_glXDestroyPixmap(dpy, pixmap);
    _post_call_glx_callback(NULL, "glXDestroyPixmap", (GLADapiproc) glad_glXDestroyPixmap, 2, dpy, pixmap);
    
}
PFNGLXDESTROYPIXMAPPROC glad_debug_glXDestroyPixmap = glad_debug_impl_glXDestroyPixmap;
PFNGLXDESTROYWINDOWPROC glad_glXDestroyWindow = NULL;
static void GLAD_API_PTR glad_debug_impl_glXDestroyWindow(Display * dpy, GLXWindow win) {
    _pre_call_glx_callback("glXDestroyWindow", (GLADapiproc) glad_glXDestroyWindow, 2, dpy, win);
    glad_glXDestroyWindow(dpy, win);
    _post_call_glx_callback(NULL, "glXDestroyWindow", (GLADapiproc) glad_glXDestroyWindow, 2, dpy, win);
    
}
PFNGLXDESTROYWINDOWPROC glad_debug_glXDestroyWindow = glad_debug_impl_glXDestroyWindow;
PFNGLXENUMERATEVIDEOCAPTUREDEVICESNVPROC glad_glXEnumerateVideoCaptureDevicesNV = NULL;
static GLXVideoCaptureDeviceNV * GLAD_API_PTR glad_debug_impl_glXEnumerateVideoCaptureDevicesNV(Display * dpy, int screen, int * nelements) {
    GLXVideoCaptureDeviceNV * ret;
    _pre_call_glx_callback("glXEnumerateVideoCaptureDevicesNV", (GLADapiproc) glad_glXEnumerateVideoCaptureDevicesNV, 3, dpy, screen, nelements);
    ret = glad_glXEnumerateVideoCaptureDevicesNV(dpy, screen, nelements);
    _post_call_glx_callback((void*) &ret, "glXEnumerateVideoCaptureDevicesNV", (GLADapiproc) glad_glXEnumerateVideoCaptureDevicesNV, 3, dpy, screen, nelements);
    return ret;
}
PFNGLXENUMERATEVIDEOCAPTUREDEVICESNVPROC glad_debug_glXEnumerateVideoCaptureDevicesNV = glad_debug_impl_glXEnumerateVideoCaptureDevicesNV;
PFNGLXENUMERATEVIDEODEVICESNVPROC glad_glXEnumerateVideoDevicesNV = NULL;
static unsigned int * GLAD_API_PTR glad_debug_impl_glXEnumerateVideoDevicesNV(Display * dpy, int screen, int * nelements) {
    unsigned int * ret;
    _pre_call_glx_callback("glXEnumerateVideoDevicesNV", (GLADapiproc) glad_glXEnumerateVideoDevicesNV, 3, dpy, screen, nelements);
    ret = glad_glXEnumerateVideoDevicesNV(dpy, screen, nelements);
    _post_call_glx_callback((void*) &ret, "glXEnumerateVideoDevicesNV", (GLADapiproc) glad_glXEnumerateVideoDevicesNV, 3, dpy, screen, nelements);
    return ret;
}
PFNGLXENUMERATEVIDEODEVICESNVPROC glad_debug_glXEnumerateVideoDevicesNV = glad_debug_impl_glXEnumerateVideoDevicesNV;
PFNGLXFREECONTEXTEXTPROC glad_glXFreeContextEXT = NULL;
static void GLAD_API_PTR glad_debug_impl_glXFreeContextEXT(Display * dpy, GLXContext context) {
    _pre_call_glx_callback("glXFreeContextEXT", (GLADapiproc) glad_glXFreeContextEXT, 2, dpy, context);
    glad_glXFreeContextEXT(dpy, context);
    _post_call_glx_callback(NULL, "glXFreeContextEXT", (GLADapiproc) glad_glXFreeContextEXT, 2, dpy, context);
    
}
PFNGLXFREECONTEXTEXTPROC glad_debug_glXFreeContextEXT = glad_debug_impl_glXFreeContextEXT;
PFNGLXGETAGPOFFSETMESAPROC glad_glXGetAGPOffsetMESA = NULL;
static unsigned int GLAD_API_PTR glad_debug_impl_glXGetAGPOffsetMESA(const void * pointer) {
    unsigned int ret;
    _pre_call_glx_callback("glXGetAGPOffsetMESA", (GLADapiproc) glad_glXGetAGPOffsetMESA, 1, pointer);
    ret = glad_glXGetAGPOffsetMESA(pointer);
    _post_call_glx_callback((void*) &ret, "glXGetAGPOffsetMESA", (GLADapiproc) glad_glXGetAGPOffsetMESA, 1, pointer);
    return ret;
}
PFNGLXGETAGPOFFSETMESAPROC glad_debug_glXGetAGPOffsetMESA = glad_debug_impl_glXGetAGPOffsetMESA;
PFNGLXGETCLIENTSTRINGPROC glad_glXGetClientString = NULL;
static const char * GLAD_API_PTR glad_debug_impl_glXGetClientString(Display * dpy, int name) {
    const char * ret;
    _pre_call_glx_callback("glXGetClientString", (GLADapiproc) glad_glXGetClientString, 2, dpy, name);
    ret = glad_glXGetClientString(dpy, name);
    _post_call_glx_callback((void*) &ret, "glXGetClientString", (GLADapiproc) glad_glXGetClientString, 2, dpy, name);
    return ret;
}
PFNGLXGETCLIENTSTRINGPROC glad_debug_glXGetClientString = glad_debug_impl_glXGetClientString;
PFNGLXGETCONFIGPROC glad_glXGetConfig = NULL;
static int GLAD_API_PTR glad_debug_impl_glXGetConfig(Display * dpy, XVisualInfo * visual, int attrib, int * value) {
    int ret;
    _pre_call_glx_callback("glXGetConfig", (GLADapiproc) glad_glXGetConfig, 4, dpy, visual, attrib, value);
    ret = glad_glXGetConfig(dpy, visual, attrib, value);
    _post_call_glx_callback((void*) &ret, "glXGetConfig", (GLADapiproc) glad_glXGetConfig, 4, dpy, visual, attrib, value);
    return ret;
}
PFNGLXGETCONFIGPROC glad_debug_glXGetConfig = glad_debug_impl_glXGetConfig;
PFNGLXGETCONTEXTGPUIDAMDPROC glad_glXGetContextGPUIDAMD = NULL;
static unsigned int GLAD_API_PTR glad_debug_impl_glXGetContextGPUIDAMD(GLXContext ctx) {
    unsigned int ret;
    _pre_call_glx_callback("glXGetContextGPUIDAMD", (GLADapiproc) glad_glXGetContextGPUIDAMD, 1, ctx);
    ret = glad_glXGetContextGPUIDAMD(ctx);
    _post_call_glx_callback((void*) &ret, "glXGetContextGPUIDAMD", (GLADapiproc) glad_glXGetContextGPUIDAMD, 1, ctx);
    return ret;
}
PFNGLXGETCONTEXTGPUIDAMDPROC glad_debug_glXGetContextGPUIDAMD = glad_debug_impl_glXGetContextGPUIDAMD;
PFNGLXGETCONTEXTIDEXTPROC glad_glXGetContextIDEXT = NULL;
static GLXContextID GLAD_API_PTR glad_debug_impl_glXGetContextIDEXT(const GLXContext context) {
    GLXContextID ret;
    _pre_call_glx_callback("glXGetContextIDEXT", (GLADapiproc) glad_glXGetContextIDEXT, 1, context);
    ret = glad_glXGetContextIDEXT(context);
    _post_call_glx_callback((void*) &ret, "glXGetContextIDEXT", (GLADapiproc) glad_glXGetContextIDEXT, 1, context);
    return ret;
}
PFNGLXGETCONTEXTIDEXTPROC glad_debug_glXGetContextIDEXT = glad_debug_impl_glXGetContextIDEXT;
PFNGLXGETCURRENTASSOCIATEDCONTEXTAMDPROC glad_glXGetCurrentAssociatedContextAMD = NULL;
static GLXContext GLAD_API_PTR glad_debug_impl_glXGetCurrentAssociatedContextAMD(void) {
    GLXContext ret;
    _pre_call_glx_callback("glXGetCurrentAssociatedContextAMD", (GLADapiproc) glad_glXGetCurrentAssociatedContextAMD, 0);
    ret = glad_glXGetCurrentAssociatedContextAMD();
    _post_call_glx_callback((void*) &ret, "glXGetCurrentAssociatedContextAMD", (GLADapiproc) glad_glXGetCurrentAssociatedContextAMD, 0);
    return ret;
}
PFNGLXGETCURRENTASSOCIATEDCONTEXTAMDPROC glad_debug_glXGetCurrentAssociatedContextAMD = glad_debug_impl_glXGetCurrentAssociatedContextAMD;
PFNGLXGETCURRENTCONTEXTPROC glad_glXGetCurrentContext = NULL;
static GLXContext GLAD_API_PTR glad_debug_impl_glXGetCurrentContext(void) {
    GLXContext ret;
    _pre_call_glx_callback("glXGetCurrentContext", (GLADapiproc) glad_glXGetCurrentContext, 0);
    ret = glad_glXGetCurrentContext();
    _post_call_glx_callback((void*) &ret, "glXGetCurrentContext", (GLADapiproc) glad_glXGetCurrentContext, 0);
    return ret;
}
PFNGLXGETCURRENTCONTEXTPROC glad_debug_glXGetCurrentContext = glad_debug_impl_glXGetCurrentContext;
PFNGLXGETCURRENTDISPLAYPROC glad_glXGetCurrentDisplay = NULL;
static Display * GLAD_API_PTR glad_debug_impl_glXGetCurrentDisplay(void) {
    Display * ret;
    _pre_call_glx_callback("glXGetCurrentDisplay", (GLADapiproc) glad_glXGetCurrentDisplay, 0);
    ret = glad_glXGetCurrentDisplay();
    _post_call_glx_callback((void*) &ret, "glXGetCurrentDisplay", (GLADapiproc) glad_glXGetCurrentDisplay, 0);
    return ret;
}
PFNGLXGETCURRENTDISPLAYPROC glad_debug_glXGetCurrentDisplay = glad_debug_impl_glXGetCurrentDisplay;
PFNGLXGETCURRENTDISPLAYEXTPROC glad_glXGetCurrentDisplayEXT = NULL;
static Display * GLAD_API_PTR glad_debug_impl_glXGetCurrentDisplayEXT(void) {
    Display * ret;
    _pre_call_glx_callback("glXGetCurrentDisplayEXT", (GLADapiproc) glad_glXGetCurrentDisplayEXT, 0);
    ret = glad_glXGetCurrentDisplayEXT();
    _post_call_glx_callback((void*) &ret, "glXGetCurrentDisplayEXT", (GLADapiproc) glad_glXGetCurrentDisplayEXT, 0);
    return ret;
}
PFNGLXGETCURRENTDISPLAYEXTPROC glad_debug_glXGetCurrentDisplayEXT = glad_debug_impl_glXGetCurrentDisplayEXT;
PFNGLXGETCURRENTDRAWABLEPROC glad_glXGetCurrentDrawable = NULL;
static GLXDrawable GLAD_API_PTR glad_debug_impl_glXGetCurrentDrawable(void) {
    GLXDrawable ret;
    _pre_call_glx_callback("glXGetCurrentDrawable", (GLADapiproc) glad_glXGetCurrentDrawable, 0);
    ret = glad_glXGetCurrentDrawable();
    _post_call_glx_callback((void*) &ret, "glXGetCurrentDrawable", (GLADapiproc) glad_glXGetCurrentDrawable, 0);
    return ret;
}
PFNGLXGETCURRENTDRAWABLEPROC glad_debug_glXGetCurrentDrawable = glad_debug_impl_glXGetCurrentDrawable;
PFNGLXGETCURRENTREADDRAWABLEPROC glad_glXGetCurrentReadDrawable = NULL;
static GLXDrawable GLAD_API_PTR glad_debug_impl_glXGetCurrentReadDrawable(void) {
    GLXDrawable ret;
    _pre_call_glx_callback("glXGetCurrentReadDrawable", (GLADapiproc) glad_glXGetCurrentReadDrawable, 0);
    ret = glad_glXGetCurrentReadDrawable();
    _post_call_glx_callback((void*) &ret, "glXGetCurrentReadDrawable", (GLADapiproc) glad_glXGetCurrentReadDrawable, 0);
    return ret;
}
PFNGLXGETCURRENTREADDRAWABLEPROC glad_debug_glXGetCurrentReadDrawable = glad_debug_impl_glXGetCurrentReadDrawable;
PFNGLXGETCURRENTREADDRAWABLESGIPROC glad_glXGetCurrentReadDrawableSGI = NULL;
static GLXDrawable GLAD_API_PTR glad_debug_impl_glXGetCurrentReadDrawableSGI(void) {
    GLXDrawable ret;
    _pre_call_glx_callback("glXGetCurrentReadDrawableSGI", (GLADapiproc) glad_glXGetCurrentReadDrawableSGI, 0);
    ret = glad_glXGetCurrentReadDrawableSGI();
    _post_call_glx_callback((void*) &ret, "glXGetCurrentReadDrawableSGI", (GLADapiproc) glad_glXGetCurrentReadDrawableSGI, 0);
    return ret;
}
PFNGLXGETCURRENTREADDRAWABLESGIPROC glad_debug_glXGetCurrentReadDrawableSGI = glad_debug_impl_glXGetCurrentReadDrawableSGI;
PFNGLXGETFBCONFIGATTRIBPROC glad_glXGetFBConfigAttrib = NULL;
static int GLAD_API_PTR glad_debug_impl_glXGetFBConfigAttrib(Display * dpy, GLXFBConfig config, int attribute, int * value) {
    int ret;
    _pre_call_glx_callback("glXGetFBConfigAttrib", (GLADapiproc) glad_glXGetFBConfigAttrib, 4, dpy, config, attribute, value);
    ret = glad_glXGetFBConfigAttrib(dpy, config, attribute, value);
    _post_call_glx_callback((void*) &ret, "glXGetFBConfigAttrib", (GLADapiproc) glad_glXGetFBConfigAttrib, 4, dpy, config, attribute, value);
    return ret;
}
PFNGLXGETFBCONFIGATTRIBPROC glad_debug_glXGetFBConfigAttrib = glad_debug_impl_glXGetFBConfigAttrib;
PFNGLXGETFBCONFIGATTRIBSGIXPROC glad_glXGetFBConfigAttribSGIX = NULL;
static int GLAD_API_PTR glad_debug_impl_glXGetFBConfigAttribSGIX(Display * dpy, GLXFBConfigSGIX config, int attribute, int * value) {
    int ret;
    _pre_call_glx_callback("glXGetFBConfigAttribSGIX", (GLADapiproc) glad_glXGetFBConfigAttribSGIX, 4, dpy, config, attribute, value);
    ret = glad_glXGetFBConfigAttribSGIX(dpy, config, attribute, value);
    _post_call_glx_callback((void*) &ret, "glXGetFBConfigAttribSGIX", (GLADapiproc) glad_glXGetFBConfigAttribSGIX, 4, dpy, config, attribute, value);
    return ret;
}
PFNGLXGETFBCONFIGATTRIBSGIXPROC glad_debug_glXGetFBConfigAttribSGIX = glad_debug_impl_glXGetFBConfigAttribSGIX;
PFNGLXGETFBCONFIGFROMVISUALSGIXPROC glad_glXGetFBConfigFromVisualSGIX = NULL;
static GLXFBConfigSGIX GLAD_API_PTR glad_debug_impl_glXGetFBConfigFromVisualSGIX(Display * dpy, XVisualInfo * vis) {
    GLXFBConfigSGIX ret;
    _pre_call_glx_callback("glXGetFBConfigFromVisualSGIX", (GLADapiproc) glad_glXGetFBConfigFromVisualSGIX, 2, dpy, vis);
    ret = glad_glXGetFBConfigFromVisualSGIX(dpy, vis);
    _post_call_glx_callback((void*) &ret, "glXGetFBConfigFromVisualSGIX", (GLADapiproc) glad_glXGetFBConfigFromVisualSGIX, 2, dpy, vis);
    return ret;
}
PFNGLXGETFBCONFIGFROMVISUALSGIXPROC glad_debug_glXGetFBConfigFromVisualSGIX = glad_debug_impl_glXGetFBConfigFromVisualSGIX;
PFNGLXGETFBCONFIGSPROC glad_glXGetFBConfigs = NULL;
static GLXFBConfig * GLAD_API_PTR glad_debug_impl_glXGetFBConfigs(Display * dpy, int screen, int * nelements) {
    GLXFBConfig * ret;
    _pre_call_glx_callback("glXGetFBConfigs", (GLADapiproc) glad_glXGetFBConfigs, 3, dpy, screen, nelements);
    ret = glad_glXGetFBConfigs(dpy, screen, nelements);
    _post_call_glx_callback((void*) &ret, "glXGetFBConfigs", (GLADapiproc) glad_glXGetFBConfigs, 3, dpy, screen, nelements);
    return ret;
}
PFNGLXGETFBCONFIGSPROC glad_debug_glXGetFBConfigs = glad_debug_impl_glXGetFBConfigs;
PFNGLXGETGPUIDSAMDPROC glad_glXGetGPUIDsAMD = NULL;
static unsigned int GLAD_API_PTR glad_debug_impl_glXGetGPUIDsAMD(unsigned int maxCount, unsigned int * ids) {
    unsigned int ret;
    _pre_call_glx_callback("glXGetGPUIDsAMD", (GLADapiproc) glad_glXGetGPUIDsAMD, 2, maxCount, ids);
    ret = glad_glXGetGPUIDsAMD(maxCount, ids);
    _post_call_glx_callback((void*) &ret, "glXGetGPUIDsAMD", (GLADapiproc) glad_glXGetGPUIDsAMD, 2, maxCount, ids);
    return ret;
}
PFNGLXGETGPUIDSAMDPROC glad_debug_glXGetGPUIDsAMD = glad_debug_impl_glXGetGPUIDsAMD;
PFNGLXGETGPUINFOAMDPROC glad_glXGetGPUInfoAMD = NULL;
static int GLAD_API_PTR glad_debug_impl_glXGetGPUInfoAMD(unsigned int id, int property, GLenum dataType, unsigned int size, void * data) {
    int ret;
    _pre_call_glx_callback("glXGetGPUInfoAMD", (GLADapiproc) glad_glXGetGPUInfoAMD, 5, id, property, dataType, size, data);
    ret = glad_glXGetGPUInfoAMD(id, property, dataType, size, data);
    _post_call_glx_callback((void*) &ret, "glXGetGPUInfoAMD", (GLADapiproc) glad_glXGetGPUInfoAMD, 5, id, property, dataType, size, data);
    return ret;
}
PFNGLXGETGPUINFOAMDPROC glad_debug_glXGetGPUInfoAMD = glad_debug_impl_glXGetGPUInfoAMD;
PFNGLXGETMSCRATEOMLPROC glad_glXGetMscRateOML = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXGetMscRateOML(Display * dpy, GLXDrawable drawable, int32_t * numerator, int32_t * denominator) {
    Bool ret;
    _pre_call_glx_callback("glXGetMscRateOML", (GLADapiproc) glad_glXGetMscRateOML, 4, dpy, drawable, numerator, denominator);
    ret = glad_glXGetMscRateOML(dpy, drawable, numerator, denominator);
    _post_call_glx_callback((void*) &ret, "glXGetMscRateOML", (GLADapiproc) glad_glXGetMscRateOML, 4, dpy, drawable, numerator, denominator);
    return ret;
}
PFNGLXGETMSCRATEOMLPROC glad_debug_glXGetMscRateOML = glad_debug_impl_glXGetMscRateOML;
PFNGLXGETPROCADDRESSPROC glad_glXGetProcAddress = NULL;
static __GLXextFuncPtr GLAD_API_PTR glad_debug_impl_glXGetProcAddress(const GLubyte * procName) {
    __GLXextFuncPtr ret;
    _pre_call_glx_callback("glXGetProcAddress", (GLADapiproc) glad_glXGetProcAddress, 1, procName);
    ret = glad_glXGetProcAddress(procName);
    _post_call_glx_callback((void*) &ret, "glXGetProcAddress", (GLADapiproc) glad_glXGetProcAddress, 1, procName);
    return ret;
}
PFNGLXGETPROCADDRESSPROC glad_debug_glXGetProcAddress = glad_debug_impl_glXGetProcAddress;
PFNGLXGETPROCADDRESSARBPROC glad_glXGetProcAddressARB = NULL;
static __GLXextFuncPtr GLAD_API_PTR glad_debug_impl_glXGetProcAddressARB(const GLubyte * procName) {
    __GLXextFuncPtr ret;
    _pre_call_glx_callback("glXGetProcAddressARB", (GLADapiproc) glad_glXGetProcAddressARB, 1, procName);
    ret = glad_glXGetProcAddressARB(procName);
    _post_call_glx_callback((void*) &ret, "glXGetProcAddressARB", (GLADapiproc) glad_glXGetProcAddressARB, 1, procName);
    return ret;
}
PFNGLXGETPROCADDRESSARBPROC glad_debug_glXGetProcAddressARB = glad_debug_impl_glXGetProcAddressARB;
PFNGLXGETSELECTEDEVENTPROC glad_glXGetSelectedEvent = NULL;
static void GLAD_API_PTR glad_debug_impl_glXGetSelectedEvent(Display * dpy, GLXDrawable draw, unsigned long * event_mask) {
    _pre_call_glx_callback("glXGetSelectedEvent", (GLADapiproc) glad_glXGetSelectedEvent, 3, dpy, draw, event_mask);
    glad_glXGetSelectedEvent(dpy, draw, event_mask);
    _post_call_glx_callback(NULL, "glXGetSelectedEvent", (GLADapiproc) glad_glXGetSelectedEvent, 3, dpy, draw, event_mask);
    
}
PFNGLXGETSELECTEDEVENTPROC glad_debug_glXGetSelectedEvent = glad_debug_impl_glXGetSelectedEvent;
PFNGLXGETSELECTEDEVENTSGIXPROC glad_glXGetSelectedEventSGIX = NULL;
static void GLAD_API_PTR glad_debug_impl_glXGetSelectedEventSGIX(Display * dpy, GLXDrawable drawable, unsigned long * mask) {
    _pre_call_glx_callback("glXGetSelectedEventSGIX", (GLADapiproc) glad_glXGetSelectedEventSGIX, 3, dpy, drawable, mask);
    glad_glXGetSelectedEventSGIX(dpy, drawable, mask);
    _post_call_glx_callback(NULL, "glXGetSelectedEventSGIX", (GLADapiproc) glad_glXGetSelectedEventSGIX, 3, dpy, drawable, mask);
    
}
PFNGLXGETSELECTEDEVENTSGIXPROC glad_debug_glXGetSelectedEventSGIX = glad_debug_impl_glXGetSelectedEventSGIX;
PFNGLXGETSWAPINTERVALMESAPROC glad_glXGetSwapIntervalMESA = NULL;
static int GLAD_API_PTR glad_debug_impl_glXGetSwapIntervalMESA(void) {
    int ret;
    _pre_call_glx_callback("glXGetSwapIntervalMESA", (GLADapiproc) glad_glXGetSwapIntervalMESA, 0);
    ret = glad_glXGetSwapIntervalMESA();
    _post_call_glx_callback((void*) &ret, "glXGetSwapIntervalMESA", (GLADapiproc) glad_glXGetSwapIntervalMESA, 0);
    return ret;
}
PFNGLXGETSWAPINTERVALMESAPROC glad_debug_glXGetSwapIntervalMESA = glad_debug_impl_glXGetSwapIntervalMESA;
PFNGLXGETSYNCVALUESOMLPROC glad_glXGetSyncValuesOML = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXGetSyncValuesOML(Display * dpy, GLXDrawable drawable, int64_t * ust, int64_t * msc, int64_t * sbc) {
    Bool ret;
    _pre_call_glx_callback("glXGetSyncValuesOML", (GLADapiproc) glad_glXGetSyncValuesOML, 5, dpy, drawable, ust, msc, sbc);
    ret = glad_glXGetSyncValuesOML(dpy, drawable, ust, msc, sbc);
    _post_call_glx_callback((void*) &ret, "glXGetSyncValuesOML", (GLADapiproc) glad_glXGetSyncValuesOML, 5, dpy, drawable, ust, msc, sbc);
    return ret;
}
PFNGLXGETSYNCVALUESOMLPROC glad_debug_glXGetSyncValuesOML = glad_debug_impl_glXGetSyncValuesOML;
PFNGLXGETTRANSPARENTINDEXSUNPROC glad_glXGetTransparentIndexSUN = NULL;
static Status GLAD_API_PTR glad_debug_impl_glXGetTransparentIndexSUN(Display * dpy, Window overlay, Window underlay, unsigned long * pTransparentIndex) {
    Status ret;
    _pre_call_glx_callback("glXGetTransparentIndexSUN", (GLADapiproc) glad_glXGetTransparentIndexSUN, 4, dpy, overlay, underlay, pTransparentIndex);
    ret = glad_glXGetTransparentIndexSUN(dpy, overlay, underlay, pTransparentIndex);
    _post_call_glx_callback((void*) &ret, "glXGetTransparentIndexSUN", (GLADapiproc) glad_glXGetTransparentIndexSUN, 4, dpy, overlay, underlay, pTransparentIndex);
    return ret;
}
PFNGLXGETTRANSPARENTINDEXSUNPROC glad_debug_glXGetTransparentIndexSUN = glad_debug_impl_glXGetTransparentIndexSUN;
PFNGLXGETVIDEODEVICENVPROC glad_glXGetVideoDeviceNV = NULL;
static int GLAD_API_PTR glad_debug_impl_glXGetVideoDeviceNV(Display * dpy, int screen, int numVideoDevices, GLXVideoDeviceNV * pVideoDevice) {
    int ret;
    _pre_call_glx_callback("glXGetVideoDeviceNV", (GLADapiproc) glad_glXGetVideoDeviceNV, 4, dpy, screen, numVideoDevices, pVideoDevice);
    ret = glad_glXGetVideoDeviceNV(dpy, screen, numVideoDevices, pVideoDevice);
    _post_call_glx_callback((void*) &ret, "glXGetVideoDeviceNV", (GLADapiproc) glad_glXGetVideoDeviceNV, 4, dpy, screen, numVideoDevices, pVideoDevice);
    return ret;
}
PFNGLXGETVIDEODEVICENVPROC glad_debug_glXGetVideoDeviceNV = glad_debug_impl_glXGetVideoDeviceNV;
PFNGLXGETVIDEOINFONVPROC glad_glXGetVideoInfoNV = NULL;
static int GLAD_API_PTR glad_debug_impl_glXGetVideoInfoNV(Display * dpy, int screen, GLXVideoDeviceNV VideoDevice, unsigned long * pulCounterOutputPbuffer, unsigned long * pulCounterOutputVideo) {
    int ret;
    _pre_call_glx_callback("glXGetVideoInfoNV", (GLADapiproc) glad_glXGetVideoInfoNV, 5, dpy, screen, VideoDevice, pulCounterOutputPbuffer, pulCounterOutputVideo);
    ret = glad_glXGetVideoInfoNV(dpy, screen, VideoDevice, pulCounterOutputPbuffer, pulCounterOutputVideo);
    _post_call_glx_callback((void*) &ret, "glXGetVideoInfoNV", (GLADapiproc) glad_glXGetVideoInfoNV, 5, dpy, screen, VideoDevice, pulCounterOutputPbuffer, pulCounterOutputVideo);
    return ret;
}
PFNGLXGETVIDEOINFONVPROC glad_debug_glXGetVideoInfoNV = glad_debug_impl_glXGetVideoInfoNV;
PFNGLXGETVIDEOSYNCSGIPROC glad_glXGetVideoSyncSGI = NULL;
static int GLAD_API_PTR glad_debug_impl_glXGetVideoSyncSGI(unsigned int * count) {
    int ret;
    _pre_call_glx_callback("glXGetVideoSyncSGI", (GLADapiproc) glad_glXGetVideoSyncSGI, 1, count);
    ret = glad_glXGetVideoSyncSGI(count);
    _post_call_glx_callback((void*) &ret, "glXGetVideoSyncSGI", (GLADapiproc) glad_glXGetVideoSyncSGI, 1, count);
    return ret;
}
PFNGLXGETVIDEOSYNCSGIPROC glad_debug_glXGetVideoSyncSGI = glad_debug_impl_glXGetVideoSyncSGI;
PFNGLXGETVISUALFROMFBCONFIGPROC glad_glXGetVisualFromFBConfig = NULL;
static XVisualInfo * GLAD_API_PTR glad_debug_impl_glXGetVisualFromFBConfig(Display * dpy, GLXFBConfig config) {
    XVisualInfo * ret;
    _pre_call_glx_callback("glXGetVisualFromFBConfig", (GLADapiproc) glad_glXGetVisualFromFBConfig, 2, dpy, config);
    ret = glad_glXGetVisualFromFBConfig(dpy, config);
    _post_call_glx_callback((void*) &ret, "glXGetVisualFromFBConfig", (GLADapiproc) glad_glXGetVisualFromFBConfig, 2, dpy, config);
    return ret;
}
PFNGLXGETVISUALFROMFBCONFIGPROC glad_debug_glXGetVisualFromFBConfig = glad_debug_impl_glXGetVisualFromFBConfig;
PFNGLXGETVISUALFROMFBCONFIGSGIXPROC glad_glXGetVisualFromFBConfigSGIX = NULL;
static XVisualInfo * GLAD_API_PTR glad_debug_impl_glXGetVisualFromFBConfigSGIX(Display * dpy, GLXFBConfigSGIX config) {
    XVisualInfo * ret;
    _pre_call_glx_callback("glXGetVisualFromFBConfigSGIX", (GLADapiproc) glad_glXGetVisualFromFBConfigSGIX, 2, dpy, config);
    ret = glad_glXGetVisualFromFBConfigSGIX(dpy, config);
    _post_call_glx_callback((void*) &ret, "glXGetVisualFromFBConfigSGIX", (GLADapiproc) glad_glXGetVisualFromFBConfigSGIX, 2, dpy, config);
    return ret;
}
PFNGLXGETVISUALFROMFBCONFIGSGIXPROC glad_debug_glXGetVisualFromFBConfigSGIX = glad_debug_impl_glXGetVisualFromFBConfigSGIX;
PFNGLXHYPERPIPEATTRIBSGIXPROC glad_glXHyperpipeAttribSGIX = NULL;
static int GLAD_API_PTR glad_debug_impl_glXHyperpipeAttribSGIX(Display * dpy, int timeSlice, int attrib, int size, void * attribList) {
    int ret;
    _pre_call_glx_callback("glXHyperpipeAttribSGIX", (GLADapiproc) glad_glXHyperpipeAttribSGIX, 5, dpy, timeSlice, attrib, size, attribList);
    ret = glad_glXHyperpipeAttribSGIX(dpy, timeSlice, attrib, size, attribList);
    _post_call_glx_callback((void*) &ret, "glXHyperpipeAttribSGIX", (GLADapiproc) glad_glXHyperpipeAttribSGIX, 5, dpy, timeSlice, attrib, size, attribList);
    return ret;
}
PFNGLXHYPERPIPEATTRIBSGIXPROC glad_debug_glXHyperpipeAttribSGIX = glad_debug_impl_glXHyperpipeAttribSGIX;
PFNGLXHYPERPIPECONFIGSGIXPROC glad_glXHyperpipeConfigSGIX = NULL;
static int GLAD_API_PTR glad_debug_impl_glXHyperpipeConfigSGIX(Display * dpy, int networkId, int npipes, GLXHyperpipeConfigSGIX * cfg, int * hpId) {
    int ret;
    _pre_call_glx_callback("glXHyperpipeConfigSGIX", (GLADapiproc) glad_glXHyperpipeConfigSGIX, 5, dpy, networkId, npipes, cfg, hpId);
    ret = glad_glXHyperpipeConfigSGIX(dpy, networkId, npipes, cfg, hpId);
    _post_call_glx_callback((void*) &ret, "glXHyperpipeConfigSGIX", (GLADapiproc) glad_glXHyperpipeConfigSGIX, 5, dpy, networkId, npipes, cfg, hpId);
    return ret;
}
PFNGLXHYPERPIPECONFIGSGIXPROC glad_debug_glXHyperpipeConfigSGIX = glad_debug_impl_glXHyperpipeConfigSGIX;
PFNGLXIMPORTCONTEXTEXTPROC glad_glXImportContextEXT = NULL;
static GLXContext GLAD_API_PTR glad_debug_impl_glXImportContextEXT(Display * dpy, GLXContextID contextID) {
    GLXContext ret;
    _pre_call_glx_callback("glXImportContextEXT", (GLADapiproc) glad_glXImportContextEXT, 2, dpy, contextID);
    ret = glad_glXImportContextEXT(dpy, contextID);
    _post_call_glx_callback((void*) &ret, "glXImportContextEXT", (GLADapiproc) glad_glXImportContextEXT, 2, dpy, contextID);
    return ret;
}
PFNGLXIMPORTCONTEXTEXTPROC glad_debug_glXImportContextEXT = glad_debug_impl_glXImportContextEXT;
PFNGLXISDIRECTPROC glad_glXIsDirect = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXIsDirect(Display * dpy, GLXContext ctx) {
    Bool ret;
    _pre_call_glx_callback("glXIsDirect", (GLADapiproc) glad_glXIsDirect, 2, dpy, ctx);
    ret = glad_glXIsDirect(dpy, ctx);
    _post_call_glx_callback((void*) &ret, "glXIsDirect", (GLADapiproc) glad_glXIsDirect, 2, dpy, ctx);
    return ret;
}
PFNGLXISDIRECTPROC glad_debug_glXIsDirect = glad_debug_impl_glXIsDirect;
PFNGLXJOINSWAPGROUPNVPROC glad_glXJoinSwapGroupNV = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXJoinSwapGroupNV(Display * dpy, GLXDrawable drawable, GLuint group) {
    Bool ret;
    _pre_call_glx_callback("glXJoinSwapGroupNV", (GLADapiproc) glad_glXJoinSwapGroupNV, 3, dpy, drawable, group);
    ret = glad_glXJoinSwapGroupNV(dpy, drawable, group);
    _post_call_glx_callback((void*) &ret, "glXJoinSwapGroupNV", (GLADapiproc) glad_glXJoinSwapGroupNV, 3, dpy, drawable, group);
    return ret;
}
PFNGLXJOINSWAPGROUPNVPROC glad_debug_glXJoinSwapGroupNV = glad_debug_impl_glXJoinSwapGroupNV;
PFNGLXJOINSWAPGROUPSGIXPROC glad_glXJoinSwapGroupSGIX = NULL;
static void GLAD_API_PTR glad_debug_impl_glXJoinSwapGroupSGIX(Display * dpy, GLXDrawable drawable, GLXDrawable member) {
    _pre_call_glx_callback("glXJoinSwapGroupSGIX", (GLADapiproc) glad_glXJoinSwapGroupSGIX, 3, dpy, drawable, member);
    glad_glXJoinSwapGroupSGIX(dpy, drawable, member);
    _post_call_glx_callback(NULL, "glXJoinSwapGroupSGIX", (GLADapiproc) glad_glXJoinSwapGroupSGIX, 3, dpy, drawable, member);
    
}
PFNGLXJOINSWAPGROUPSGIXPROC glad_debug_glXJoinSwapGroupSGIX = glad_debug_impl_glXJoinSwapGroupSGIX;
PFNGLXLOCKVIDEOCAPTUREDEVICENVPROC glad_glXLockVideoCaptureDeviceNV = NULL;
static void GLAD_API_PTR glad_debug_impl_glXLockVideoCaptureDeviceNV(Display * dpy, GLXVideoCaptureDeviceNV device) {
    _pre_call_glx_callback("glXLockVideoCaptureDeviceNV", (GLADapiproc) glad_glXLockVideoCaptureDeviceNV, 2, dpy, device);
    glad_glXLockVideoCaptureDeviceNV(dpy, device);
    _post_call_glx_callback(NULL, "glXLockVideoCaptureDeviceNV", (GLADapiproc) glad_glXLockVideoCaptureDeviceNV, 2, dpy, device);
    
}
PFNGLXLOCKVIDEOCAPTUREDEVICENVPROC glad_debug_glXLockVideoCaptureDeviceNV = glad_debug_impl_glXLockVideoCaptureDeviceNV;
PFNGLXMAKEASSOCIATEDCONTEXTCURRENTAMDPROC glad_glXMakeAssociatedContextCurrentAMD = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXMakeAssociatedContextCurrentAMD(GLXContext ctx) {
    Bool ret;
    _pre_call_glx_callback("glXMakeAssociatedContextCurrentAMD", (GLADapiproc) glad_glXMakeAssociatedContextCurrentAMD, 1, ctx);
    ret = glad_glXMakeAssociatedContextCurrentAMD(ctx);
    _post_call_glx_callback((void*) &ret, "glXMakeAssociatedContextCurrentAMD", (GLADapiproc) glad_glXMakeAssociatedContextCurrentAMD, 1, ctx);
    return ret;
}
PFNGLXMAKEASSOCIATEDCONTEXTCURRENTAMDPROC glad_debug_glXMakeAssociatedContextCurrentAMD = glad_debug_impl_glXMakeAssociatedContextCurrentAMD;
PFNGLXMAKECONTEXTCURRENTPROC glad_glXMakeContextCurrent = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXMakeContextCurrent(Display * dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx) {
    Bool ret;
    _pre_call_glx_callback("glXMakeContextCurrent", (GLADapiproc) glad_glXMakeContextCurrent, 4, dpy, draw, read, ctx);
    ret = glad_glXMakeContextCurrent(dpy, draw, read, ctx);
    _post_call_glx_callback((void*) &ret, "glXMakeContextCurrent", (GLADapiproc) glad_glXMakeContextCurrent, 4, dpy, draw, read, ctx);
    return ret;
}
PFNGLXMAKECONTEXTCURRENTPROC glad_debug_glXMakeContextCurrent = glad_debug_impl_glXMakeContextCurrent;
PFNGLXMAKECURRENTPROC glad_glXMakeCurrent = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXMakeCurrent(Display * dpy, GLXDrawable drawable, GLXContext ctx) {
    Bool ret;
    _pre_call_glx_callback("glXMakeCurrent", (GLADapiproc) glad_glXMakeCurrent, 3, dpy, drawable, ctx);
    ret = glad_glXMakeCurrent(dpy, drawable, ctx);
    _post_call_glx_callback((void*) &ret, "glXMakeCurrent", (GLADapiproc) glad_glXMakeCurrent, 3, dpy, drawable, ctx);
    return ret;
}
PFNGLXMAKECURRENTPROC glad_debug_glXMakeCurrent = glad_debug_impl_glXMakeCurrent;
PFNGLXMAKECURRENTREADSGIPROC glad_glXMakeCurrentReadSGI = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXMakeCurrentReadSGI(Display * dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx) {
    Bool ret;
    _pre_call_glx_callback("glXMakeCurrentReadSGI", (GLADapiproc) glad_glXMakeCurrentReadSGI, 4, dpy, draw, read, ctx);
    ret = glad_glXMakeCurrentReadSGI(dpy, draw, read, ctx);
    _post_call_glx_callback((void*) &ret, "glXMakeCurrentReadSGI", (GLADapiproc) glad_glXMakeCurrentReadSGI, 4, dpy, draw, read, ctx);
    return ret;
}
PFNGLXMAKECURRENTREADSGIPROC glad_debug_glXMakeCurrentReadSGI = glad_debug_impl_glXMakeCurrentReadSGI;
PFNGLXNAMEDCOPYBUFFERSUBDATANVPROC glad_glXNamedCopyBufferSubDataNV = NULL;
static void GLAD_API_PTR glad_debug_impl_glXNamedCopyBufferSubDataNV(Display * dpy, GLXContext readCtx, GLXContext writeCtx, GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) {
    _pre_call_glx_callback("glXNamedCopyBufferSubDataNV", (GLADapiproc) glad_glXNamedCopyBufferSubDataNV, 8, dpy, readCtx, writeCtx, readBuffer, writeBuffer, readOffset, writeOffset, size);
    glad_glXNamedCopyBufferSubDataNV(dpy, readCtx, writeCtx, readBuffer, writeBuffer, readOffset, writeOffset, size);
    _post_call_glx_callback(NULL, "glXNamedCopyBufferSubDataNV", (GLADapiproc) glad_glXNamedCopyBufferSubDataNV, 8, dpy, readCtx, writeCtx, readBuffer, writeBuffer, readOffset, writeOffset, size);
    
}
PFNGLXNAMEDCOPYBUFFERSUBDATANVPROC glad_debug_glXNamedCopyBufferSubDataNV = glad_debug_impl_glXNamedCopyBufferSubDataNV;
PFNGLXQUERYCHANNELDELTASSGIXPROC glad_glXQueryChannelDeltasSGIX = NULL;
static int GLAD_API_PTR glad_debug_impl_glXQueryChannelDeltasSGIX(Display * display, int screen, int channel, int * x, int * y, int * w, int * h) {
    int ret;
    _pre_call_glx_callback("glXQueryChannelDeltasSGIX", (GLADapiproc) glad_glXQueryChannelDeltasSGIX, 7, display, screen, channel, x, y, w, h);
    ret = glad_glXQueryChannelDeltasSGIX(display, screen, channel, x, y, w, h);
    _post_call_glx_callback((void*) &ret, "glXQueryChannelDeltasSGIX", (GLADapiproc) glad_glXQueryChannelDeltasSGIX, 7, display, screen, channel, x, y, w, h);
    return ret;
}
PFNGLXQUERYCHANNELDELTASSGIXPROC glad_debug_glXQueryChannelDeltasSGIX = glad_debug_impl_glXQueryChannelDeltasSGIX;
PFNGLXQUERYCHANNELRECTSGIXPROC glad_glXQueryChannelRectSGIX = NULL;
static int GLAD_API_PTR glad_debug_impl_glXQueryChannelRectSGIX(Display * display, int screen, int channel, int * dx, int * dy, int * dw, int * dh) {
    int ret;
    _pre_call_glx_callback("glXQueryChannelRectSGIX", (GLADapiproc) glad_glXQueryChannelRectSGIX, 7, display, screen, channel, dx, dy, dw, dh);
    ret = glad_glXQueryChannelRectSGIX(display, screen, channel, dx, dy, dw, dh);
    _post_call_glx_callback((void*) &ret, "glXQueryChannelRectSGIX", (GLADapiproc) glad_glXQueryChannelRectSGIX, 7, display, screen, channel, dx, dy, dw, dh);
    return ret;
}
PFNGLXQUERYCHANNELRECTSGIXPROC glad_debug_glXQueryChannelRectSGIX = glad_debug_impl_glXQueryChannelRectSGIX;
PFNGLXQUERYCONTEXTPROC glad_glXQueryContext = NULL;
static int GLAD_API_PTR glad_debug_impl_glXQueryContext(Display * dpy, GLXContext ctx, int attribute, int * value) {
    int ret;
    _pre_call_glx_callback("glXQueryContext", (GLADapiproc) glad_glXQueryContext, 4, dpy, ctx, attribute, value);
    ret = glad_glXQueryContext(dpy, ctx, attribute, value);
    _post_call_glx_callback((void*) &ret, "glXQueryContext", (GLADapiproc) glad_glXQueryContext, 4, dpy, ctx, attribute, value);
    return ret;
}
PFNGLXQUERYCONTEXTPROC glad_debug_glXQueryContext = glad_debug_impl_glXQueryContext;
PFNGLXQUERYCONTEXTINFOEXTPROC glad_glXQueryContextInfoEXT = NULL;
static int GLAD_API_PTR glad_debug_impl_glXQueryContextInfoEXT(Display * dpy, GLXContext context, int attribute, int * value) {
    int ret;
    _pre_call_glx_callback("glXQueryContextInfoEXT", (GLADapiproc) glad_glXQueryContextInfoEXT, 4, dpy, context, attribute, value);
    ret = glad_glXQueryContextInfoEXT(dpy, context, attribute, value);
    _post_call_glx_callback((void*) &ret, "glXQueryContextInfoEXT", (GLADapiproc) glad_glXQueryContextInfoEXT, 4, dpy, context, attribute, value);
    return ret;
}
PFNGLXQUERYCONTEXTINFOEXTPROC glad_debug_glXQueryContextInfoEXT = glad_debug_impl_glXQueryContextInfoEXT;
PFNGLXQUERYCURRENTRENDERERINTEGERMESAPROC glad_glXQueryCurrentRendererIntegerMESA = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXQueryCurrentRendererIntegerMESA(int attribute, unsigned int * value) {
    Bool ret;
    _pre_call_glx_callback("glXQueryCurrentRendererIntegerMESA", (GLADapiproc) glad_glXQueryCurrentRendererIntegerMESA, 2, attribute, value);
    ret = glad_glXQueryCurrentRendererIntegerMESA(attribute, value);
    _post_call_glx_callback((void*) &ret, "glXQueryCurrentRendererIntegerMESA", (GLADapiproc) glad_glXQueryCurrentRendererIntegerMESA, 2, attribute, value);
    return ret;
}
PFNGLXQUERYCURRENTRENDERERINTEGERMESAPROC glad_debug_glXQueryCurrentRendererIntegerMESA = glad_debug_impl_glXQueryCurrentRendererIntegerMESA;
PFNGLXQUERYCURRENTRENDERERSTRINGMESAPROC glad_glXQueryCurrentRendererStringMESA = NULL;
static const char * GLAD_API_PTR glad_debug_impl_glXQueryCurrentRendererStringMESA(int attribute) {
    const char * ret;
    _pre_call_glx_callback("glXQueryCurrentRendererStringMESA", (GLADapiproc) glad_glXQueryCurrentRendererStringMESA, 1, attribute);
    ret = glad_glXQueryCurrentRendererStringMESA(attribute);
    _post_call_glx_callback((void*) &ret, "glXQueryCurrentRendererStringMESA", (GLADapiproc) glad_glXQueryCurrentRendererStringMESA, 1, attribute);
    return ret;
}
PFNGLXQUERYCURRENTRENDERERSTRINGMESAPROC glad_debug_glXQueryCurrentRendererStringMESA = glad_debug_impl_glXQueryCurrentRendererStringMESA;
PFNGLXQUERYDRAWABLEPROC glad_glXQueryDrawable = NULL;
static void GLAD_API_PTR glad_debug_impl_glXQueryDrawable(Display * dpy, GLXDrawable draw, int attribute, unsigned int * value) {
    _pre_call_glx_callback("glXQueryDrawable", (GLADapiproc) glad_glXQueryDrawable, 4, dpy, draw, attribute, value);
    glad_glXQueryDrawable(dpy, draw, attribute, value);
    _post_call_glx_callback(NULL, "glXQueryDrawable", (GLADapiproc) glad_glXQueryDrawable, 4, dpy, draw, attribute, value);
    
}
PFNGLXQUERYDRAWABLEPROC glad_debug_glXQueryDrawable = glad_debug_impl_glXQueryDrawable;
PFNGLXQUERYEXTENSIONPROC glad_glXQueryExtension = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXQueryExtension(Display * dpy, int * errorb, int * event) {
    Bool ret;
    _pre_call_glx_callback("glXQueryExtension", (GLADapiproc) glad_glXQueryExtension, 3, dpy, errorb, event);
    ret = glad_glXQueryExtension(dpy, errorb, event);
    _post_call_glx_callback((void*) &ret, "glXQueryExtension", (GLADapiproc) glad_glXQueryExtension, 3, dpy, errorb, event);
    return ret;
}
PFNGLXQUERYEXTENSIONPROC glad_debug_glXQueryExtension = glad_debug_impl_glXQueryExtension;
PFNGLXQUERYEXTENSIONSSTRINGPROC glad_glXQueryExtensionsString = NULL;
static const char * GLAD_API_PTR glad_debug_impl_glXQueryExtensionsString(Display * dpy, int screen) {
    const char * ret;
    _pre_call_glx_callback("glXQueryExtensionsString", (GLADapiproc) glad_glXQueryExtensionsString, 2, dpy, screen);
    ret = glad_glXQueryExtensionsString(dpy, screen);
    _post_call_glx_callback((void*) &ret, "glXQueryExtensionsString", (GLADapiproc) glad_glXQueryExtensionsString, 2, dpy, screen);
    return ret;
}
PFNGLXQUERYEXTENSIONSSTRINGPROC glad_debug_glXQueryExtensionsString = glad_debug_impl_glXQueryExtensionsString;
PFNGLXQUERYFRAMECOUNTNVPROC glad_glXQueryFrameCountNV = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXQueryFrameCountNV(Display * dpy, int screen, GLuint * count) {
    Bool ret;
    _pre_call_glx_callback("glXQueryFrameCountNV", (GLADapiproc) glad_glXQueryFrameCountNV, 3, dpy, screen, count);
    ret = glad_glXQueryFrameCountNV(dpy, screen, count);
    _post_call_glx_callback((void*) &ret, "glXQueryFrameCountNV", (GLADapiproc) glad_glXQueryFrameCountNV, 3, dpy, screen, count);
    return ret;
}
PFNGLXQUERYFRAMECOUNTNVPROC glad_debug_glXQueryFrameCountNV = glad_debug_impl_glXQueryFrameCountNV;
PFNGLXQUERYGLXPBUFFERSGIXPROC glad_glXQueryGLXPbufferSGIX = NULL;
static void GLAD_API_PTR glad_debug_impl_glXQueryGLXPbufferSGIX(Display * dpy, GLXPbufferSGIX pbuf, int attribute, unsigned int * value) {
    _pre_call_glx_callback("glXQueryGLXPbufferSGIX", (GLADapiproc) glad_glXQueryGLXPbufferSGIX, 4, dpy, pbuf, attribute, value);
    glad_glXQueryGLXPbufferSGIX(dpy, pbuf, attribute, value);
    _post_call_glx_callback(NULL, "glXQueryGLXPbufferSGIX", (GLADapiproc) glad_glXQueryGLXPbufferSGIX, 4, dpy, pbuf, attribute, value);
    
}
PFNGLXQUERYGLXPBUFFERSGIXPROC glad_debug_glXQueryGLXPbufferSGIX = glad_debug_impl_glXQueryGLXPbufferSGIX;
PFNGLXQUERYHYPERPIPEATTRIBSGIXPROC glad_glXQueryHyperpipeAttribSGIX = NULL;
static int GLAD_API_PTR glad_debug_impl_glXQueryHyperpipeAttribSGIX(Display * dpy, int timeSlice, int attrib, int size, void * returnAttribList) {
    int ret;
    _pre_call_glx_callback("glXQueryHyperpipeAttribSGIX", (GLADapiproc) glad_glXQueryHyperpipeAttribSGIX, 5, dpy, timeSlice, attrib, size, returnAttribList);
    ret = glad_glXQueryHyperpipeAttribSGIX(dpy, timeSlice, attrib, size, returnAttribList);
    _post_call_glx_callback((void*) &ret, "glXQueryHyperpipeAttribSGIX", (GLADapiproc) glad_glXQueryHyperpipeAttribSGIX, 5, dpy, timeSlice, attrib, size, returnAttribList);
    return ret;
}
PFNGLXQUERYHYPERPIPEATTRIBSGIXPROC glad_debug_glXQueryHyperpipeAttribSGIX = glad_debug_impl_glXQueryHyperpipeAttribSGIX;
PFNGLXQUERYHYPERPIPEBESTATTRIBSGIXPROC glad_glXQueryHyperpipeBestAttribSGIX = NULL;
static int GLAD_API_PTR glad_debug_impl_glXQueryHyperpipeBestAttribSGIX(Display * dpy, int timeSlice, int attrib, int size, void * attribList, void * returnAttribList) {
    int ret;
    _pre_call_glx_callback("glXQueryHyperpipeBestAttribSGIX", (GLADapiproc) glad_glXQueryHyperpipeBestAttribSGIX, 6, dpy, timeSlice, attrib, size, attribList, returnAttribList);
    ret = glad_glXQueryHyperpipeBestAttribSGIX(dpy, timeSlice, attrib, size, attribList, returnAttribList);
    _post_call_glx_callback((void*) &ret, "glXQueryHyperpipeBestAttribSGIX", (GLADapiproc) glad_glXQueryHyperpipeBestAttribSGIX, 6, dpy, timeSlice, attrib, size, attribList, returnAttribList);
    return ret;
}
PFNGLXQUERYHYPERPIPEBESTATTRIBSGIXPROC glad_debug_glXQueryHyperpipeBestAttribSGIX = glad_debug_impl_glXQueryHyperpipeBestAttribSGIX;
PFNGLXQUERYHYPERPIPECONFIGSGIXPROC glad_glXQueryHyperpipeConfigSGIX = NULL;
static GLXHyperpipeConfigSGIX * GLAD_API_PTR glad_debug_impl_glXQueryHyperpipeConfigSGIX(Display * dpy, int hpId, int * npipes) {
    GLXHyperpipeConfigSGIX * ret;
    _pre_call_glx_callback("glXQueryHyperpipeConfigSGIX", (GLADapiproc) glad_glXQueryHyperpipeConfigSGIX, 3, dpy, hpId, npipes);
    ret = glad_glXQueryHyperpipeConfigSGIX(dpy, hpId, npipes);
    _post_call_glx_callback((void*) &ret, "glXQueryHyperpipeConfigSGIX", (GLADapiproc) glad_glXQueryHyperpipeConfigSGIX, 3, dpy, hpId, npipes);
    return ret;
}
PFNGLXQUERYHYPERPIPECONFIGSGIXPROC glad_debug_glXQueryHyperpipeConfigSGIX = glad_debug_impl_glXQueryHyperpipeConfigSGIX;
PFNGLXQUERYHYPERPIPENETWORKSGIXPROC glad_glXQueryHyperpipeNetworkSGIX = NULL;
static GLXHyperpipeNetworkSGIX * GLAD_API_PTR glad_debug_impl_glXQueryHyperpipeNetworkSGIX(Display * dpy, int * npipes) {
    GLXHyperpipeNetworkSGIX * ret;
    _pre_call_glx_callback("glXQueryHyperpipeNetworkSGIX", (GLADapiproc) glad_glXQueryHyperpipeNetworkSGIX, 2, dpy, npipes);
    ret = glad_glXQueryHyperpipeNetworkSGIX(dpy, npipes);
    _post_call_glx_callback((void*) &ret, "glXQueryHyperpipeNetworkSGIX", (GLADapiproc) glad_glXQueryHyperpipeNetworkSGIX, 2, dpy, npipes);
    return ret;
}
PFNGLXQUERYHYPERPIPENETWORKSGIXPROC glad_debug_glXQueryHyperpipeNetworkSGIX = glad_debug_impl_glXQueryHyperpipeNetworkSGIX;
PFNGLXQUERYMAXSWAPBARRIERSSGIXPROC glad_glXQueryMaxSwapBarriersSGIX = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXQueryMaxSwapBarriersSGIX(Display * dpy, int screen, int * max) {
    Bool ret;
    _pre_call_glx_callback("glXQueryMaxSwapBarriersSGIX", (GLADapiproc) glad_glXQueryMaxSwapBarriersSGIX, 3, dpy, screen, max);
    ret = glad_glXQueryMaxSwapBarriersSGIX(dpy, screen, max);
    _post_call_glx_callback((void*) &ret, "glXQueryMaxSwapBarriersSGIX", (GLADapiproc) glad_glXQueryMaxSwapBarriersSGIX, 3, dpy, screen, max);
    return ret;
}
PFNGLXQUERYMAXSWAPBARRIERSSGIXPROC glad_debug_glXQueryMaxSwapBarriersSGIX = glad_debug_impl_glXQueryMaxSwapBarriersSGIX;
PFNGLXQUERYMAXSWAPGROUPSNVPROC glad_glXQueryMaxSwapGroupsNV = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXQueryMaxSwapGroupsNV(Display * dpy, int screen, GLuint * maxGroups, GLuint * maxBarriers) {
    Bool ret;
    _pre_call_glx_callback("glXQueryMaxSwapGroupsNV", (GLADapiproc) glad_glXQueryMaxSwapGroupsNV, 4, dpy, screen, maxGroups, maxBarriers);
    ret = glad_glXQueryMaxSwapGroupsNV(dpy, screen, maxGroups, maxBarriers);
    _post_call_glx_callback((void*) &ret, "glXQueryMaxSwapGroupsNV", (GLADapiproc) glad_glXQueryMaxSwapGroupsNV, 4, dpy, screen, maxGroups, maxBarriers);
    return ret;
}
PFNGLXQUERYMAXSWAPGROUPSNVPROC glad_debug_glXQueryMaxSwapGroupsNV = glad_debug_impl_glXQueryMaxSwapGroupsNV;
PFNGLXQUERYRENDERERINTEGERMESAPROC glad_glXQueryRendererIntegerMESA = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXQueryRendererIntegerMESA(Display * dpy, int screen, int renderer, int attribute, unsigned int * value) {
    Bool ret;
    _pre_call_glx_callback("glXQueryRendererIntegerMESA", (GLADapiproc) glad_glXQueryRendererIntegerMESA, 5, dpy, screen, renderer, attribute, value);
    ret = glad_glXQueryRendererIntegerMESA(dpy, screen, renderer, attribute, value);
    _post_call_glx_callback((void*) &ret, "glXQueryRendererIntegerMESA", (GLADapiproc) glad_glXQueryRendererIntegerMESA, 5, dpy, screen, renderer, attribute, value);
    return ret;
}
PFNGLXQUERYRENDERERINTEGERMESAPROC glad_debug_glXQueryRendererIntegerMESA = glad_debug_impl_glXQueryRendererIntegerMESA;
PFNGLXQUERYRENDERERSTRINGMESAPROC glad_glXQueryRendererStringMESA = NULL;
static const char * GLAD_API_PTR glad_debug_impl_glXQueryRendererStringMESA(Display * dpy, int screen, int renderer, int attribute) {
    const char * ret;
    _pre_call_glx_callback("glXQueryRendererStringMESA", (GLADapiproc) glad_glXQueryRendererStringMESA, 4, dpy, screen, renderer, attribute);
    ret = glad_glXQueryRendererStringMESA(dpy, screen, renderer, attribute);
    _post_call_glx_callback((void*) &ret, "glXQueryRendererStringMESA", (GLADapiproc) glad_glXQueryRendererStringMESA, 4, dpy, screen, renderer, attribute);
    return ret;
}
PFNGLXQUERYRENDERERSTRINGMESAPROC glad_debug_glXQueryRendererStringMESA = glad_debug_impl_glXQueryRendererStringMESA;
PFNGLXQUERYSERVERSTRINGPROC glad_glXQueryServerString = NULL;
static const char * GLAD_API_PTR glad_debug_impl_glXQueryServerString(Display * dpy, int screen, int name) {
    const char * ret;
    _pre_call_glx_callback("glXQueryServerString", (GLADapiproc) glad_glXQueryServerString, 3, dpy, screen, name);
    ret = glad_glXQueryServerString(dpy, screen, name);
    _post_call_glx_callback((void*) &ret, "glXQueryServerString", (GLADapiproc) glad_glXQueryServerString, 3, dpy, screen, name);
    return ret;
}
PFNGLXQUERYSERVERSTRINGPROC glad_debug_glXQueryServerString = glad_debug_impl_glXQueryServerString;
PFNGLXQUERYSWAPGROUPNVPROC glad_glXQuerySwapGroupNV = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXQuerySwapGroupNV(Display * dpy, GLXDrawable drawable, GLuint * group, GLuint * barrier) {
    Bool ret;
    _pre_call_glx_callback("glXQuerySwapGroupNV", (GLADapiproc) glad_glXQuerySwapGroupNV, 4, dpy, drawable, group, barrier);
    ret = glad_glXQuerySwapGroupNV(dpy, drawable, group, barrier);
    _post_call_glx_callback((void*) &ret, "glXQuerySwapGroupNV", (GLADapiproc) glad_glXQuerySwapGroupNV, 4, dpy, drawable, group, barrier);
    return ret;
}
PFNGLXQUERYSWAPGROUPNVPROC glad_debug_glXQuerySwapGroupNV = glad_debug_impl_glXQuerySwapGroupNV;
PFNGLXQUERYVERSIONPROC glad_glXQueryVersion = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXQueryVersion(Display * dpy, int * maj, int * min) {
    Bool ret;
    _pre_call_glx_callback("glXQueryVersion", (GLADapiproc) glad_glXQueryVersion, 3, dpy, maj, min);
    ret = glad_glXQueryVersion(dpy, maj, min);
    _post_call_glx_callback((void*) &ret, "glXQueryVersion", (GLADapiproc) glad_glXQueryVersion, 3, dpy, maj, min);
    return ret;
}
PFNGLXQUERYVERSIONPROC glad_debug_glXQueryVersion = glad_debug_impl_glXQueryVersion;
PFNGLXQUERYVIDEOCAPTUREDEVICENVPROC glad_glXQueryVideoCaptureDeviceNV = NULL;
static int GLAD_API_PTR glad_debug_impl_glXQueryVideoCaptureDeviceNV(Display * dpy, GLXVideoCaptureDeviceNV device, int attribute, int * value) {
    int ret;
    _pre_call_glx_callback("glXQueryVideoCaptureDeviceNV", (GLADapiproc) glad_glXQueryVideoCaptureDeviceNV, 4, dpy, device, attribute, value);
    ret = glad_glXQueryVideoCaptureDeviceNV(dpy, device, attribute, value);
    _post_call_glx_callback((void*) &ret, "glXQueryVideoCaptureDeviceNV", (GLADapiproc) glad_glXQueryVideoCaptureDeviceNV, 4, dpy, device, attribute, value);
    return ret;
}
PFNGLXQUERYVIDEOCAPTUREDEVICENVPROC glad_debug_glXQueryVideoCaptureDeviceNV = glad_debug_impl_glXQueryVideoCaptureDeviceNV;
PFNGLXRELEASEBUFFERSMESAPROC glad_glXReleaseBuffersMESA = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXReleaseBuffersMESA(Display * dpy, GLXDrawable drawable) {
    Bool ret;
    _pre_call_glx_callback("glXReleaseBuffersMESA", (GLADapiproc) glad_glXReleaseBuffersMESA, 2, dpy, drawable);
    ret = glad_glXReleaseBuffersMESA(dpy, drawable);
    _post_call_glx_callback((void*) &ret, "glXReleaseBuffersMESA", (GLADapiproc) glad_glXReleaseBuffersMESA, 2, dpy, drawable);
    return ret;
}
PFNGLXRELEASEBUFFERSMESAPROC glad_debug_glXReleaseBuffersMESA = glad_debug_impl_glXReleaseBuffersMESA;
PFNGLXRELEASETEXIMAGEEXTPROC glad_glXReleaseTexImageEXT = NULL;
static void GLAD_API_PTR glad_debug_impl_glXReleaseTexImageEXT(Display * dpy, GLXDrawable drawable, int buffer) {
    _pre_call_glx_callback("glXReleaseTexImageEXT", (GLADapiproc) glad_glXReleaseTexImageEXT, 3, dpy, drawable, buffer);
    glad_glXReleaseTexImageEXT(dpy, drawable, buffer);
    _post_call_glx_callback(NULL, "glXReleaseTexImageEXT", (GLADapiproc) glad_glXReleaseTexImageEXT, 3, dpy, drawable, buffer);
    
}
PFNGLXRELEASETEXIMAGEEXTPROC glad_debug_glXReleaseTexImageEXT = glad_debug_impl_glXReleaseTexImageEXT;
PFNGLXRELEASEVIDEOCAPTUREDEVICENVPROC glad_glXReleaseVideoCaptureDeviceNV = NULL;
static void GLAD_API_PTR glad_debug_impl_glXReleaseVideoCaptureDeviceNV(Display * dpy, GLXVideoCaptureDeviceNV device) {
    _pre_call_glx_callback("glXReleaseVideoCaptureDeviceNV", (GLADapiproc) glad_glXReleaseVideoCaptureDeviceNV, 2, dpy, device);
    glad_glXReleaseVideoCaptureDeviceNV(dpy, device);
    _post_call_glx_callback(NULL, "glXReleaseVideoCaptureDeviceNV", (GLADapiproc) glad_glXReleaseVideoCaptureDeviceNV, 2, dpy, device);
    
}
PFNGLXRELEASEVIDEOCAPTUREDEVICENVPROC glad_debug_glXReleaseVideoCaptureDeviceNV = glad_debug_impl_glXReleaseVideoCaptureDeviceNV;
PFNGLXRELEASEVIDEODEVICENVPROC glad_glXReleaseVideoDeviceNV = NULL;
static int GLAD_API_PTR glad_debug_impl_glXReleaseVideoDeviceNV(Display * dpy, int screen, GLXVideoDeviceNV VideoDevice) {
    int ret;
    _pre_call_glx_callback("glXReleaseVideoDeviceNV", (GLADapiproc) glad_glXReleaseVideoDeviceNV, 3, dpy, screen, VideoDevice);
    ret = glad_glXReleaseVideoDeviceNV(dpy, screen, VideoDevice);
    _post_call_glx_callback((void*) &ret, "glXReleaseVideoDeviceNV", (GLADapiproc) glad_glXReleaseVideoDeviceNV, 3, dpy, screen, VideoDevice);
    return ret;
}
PFNGLXRELEASEVIDEODEVICENVPROC glad_debug_glXReleaseVideoDeviceNV = glad_debug_impl_glXReleaseVideoDeviceNV;
PFNGLXRELEASEVIDEOIMAGENVPROC glad_glXReleaseVideoImageNV = NULL;
static int GLAD_API_PTR glad_debug_impl_glXReleaseVideoImageNV(Display * dpy, GLXPbuffer pbuf) {
    int ret;
    _pre_call_glx_callback("glXReleaseVideoImageNV", (GLADapiproc) glad_glXReleaseVideoImageNV, 2, dpy, pbuf);
    ret = glad_glXReleaseVideoImageNV(dpy, pbuf);
    _post_call_glx_callback((void*) &ret, "glXReleaseVideoImageNV", (GLADapiproc) glad_glXReleaseVideoImageNV, 2, dpy, pbuf);
    return ret;
}
PFNGLXRELEASEVIDEOIMAGENVPROC glad_debug_glXReleaseVideoImageNV = glad_debug_impl_glXReleaseVideoImageNV;
PFNGLXRESETFRAMECOUNTNVPROC glad_glXResetFrameCountNV = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXResetFrameCountNV(Display * dpy, int screen) {
    Bool ret;
    _pre_call_glx_callback("glXResetFrameCountNV", (GLADapiproc) glad_glXResetFrameCountNV, 2, dpy, screen);
    ret = glad_glXResetFrameCountNV(dpy, screen);
    _post_call_glx_callback((void*) &ret, "glXResetFrameCountNV", (GLADapiproc) glad_glXResetFrameCountNV, 2, dpy, screen);
    return ret;
}
PFNGLXRESETFRAMECOUNTNVPROC glad_debug_glXResetFrameCountNV = glad_debug_impl_glXResetFrameCountNV;
PFNGLXSELECTEVENTPROC glad_glXSelectEvent = NULL;
static void GLAD_API_PTR glad_debug_impl_glXSelectEvent(Display * dpy, GLXDrawable draw, unsigned long event_mask) {
    _pre_call_glx_callback("glXSelectEvent", (GLADapiproc) glad_glXSelectEvent, 3, dpy, draw, event_mask);
    glad_glXSelectEvent(dpy, draw, event_mask);
    _post_call_glx_callback(NULL, "glXSelectEvent", (GLADapiproc) glad_glXSelectEvent, 3, dpy, draw, event_mask);
    
}
PFNGLXSELECTEVENTPROC glad_debug_glXSelectEvent = glad_debug_impl_glXSelectEvent;
PFNGLXSELECTEVENTSGIXPROC glad_glXSelectEventSGIX = NULL;
static void GLAD_API_PTR glad_debug_impl_glXSelectEventSGIX(Display * dpy, GLXDrawable drawable, unsigned long mask) {
    _pre_call_glx_callback("glXSelectEventSGIX", (GLADapiproc) glad_glXSelectEventSGIX, 3, dpy, drawable, mask);
    glad_glXSelectEventSGIX(dpy, drawable, mask);
    _post_call_glx_callback(NULL, "glXSelectEventSGIX", (GLADapiproc) glad_glXSelectEventSGIX, 3, dpy, drawable, mask);
    
}
PFNGLXSELECTEVENTSGIXPROC glad_debug_glXSelectEventSGIX = glad_debug_impl_glXSelectEventSGIX;
PFNGLXSENDPBUFFERTOVIDEONVPROC glad_glXSendPbufferToVideoNV = NULL;
static int GLAD_API_PTR glad_debug_impl_glXSendPbufferToVideoNV(Display * dpy, GLXPbuffer pbuf, int iBufferType, unsigned long * pulCounterPbuffer, GLboolean bBlock) {
    int ret;
    _pre_call_glx_callback("glXSendPbufferToVideoNV", (GLADapiproc) glad_glXSendPbufferToVideoNV, 5, dpy, pbuf, iBufferType, pulCounterPbuffer, bBlock);
    ret = glad_glXSendPbufferToVideoNV(dpy, pbuf, iBufferType, pulCounterPbuffer, bBlock);
    _post_call_glx_callback((void*) &ret, "glXSendPbufferToVideoNV", (GLADapiproc) glad_glXSendPbufferToVideoNV, 5, dpy, pbuf, iBufferType, pulCounterPbuffer, bBlock);
    return ret;
}
PFNGLXSENDPBUFFERTOVIDEONVPROC glad_debug_glXSendPbufferToVideoNV = glad_debug_impl_glXSendPbufferToVideoNV;
PFNGLXSET3DFXMODEMESAPROC glad_glXSet3DfxModeMESA = NULL;
static GLboolean GLAD_API_PTR glad_debug_impl_glXSet3DfxModeMESA(GLint mode) {
    GLboolean ret;
    _pre_call_glx_callback("glXSet3DfxModeMESA", (GLADapiproc) glad_glXSet3DfxModeMESA, 1, mode);
    ret = glad_glXSet3DfxModeMESA(mode);
    _post_call_glx_callback((void*) &ret, "glXSet3DfxModeMESA", (GLADapiproc) glad_glXSet3DfxModeMESA, 1, mode);
    return ret;
}
PFNGLXSET3DFXMODEMESAPROC glad_debug_glXSet3DfxModeMESA = glad_debug_impl_glXSet3DfxModeMESA;
PFNGLXSWAPBUFFERSPROC glad_glXSwapBuffers = NULL;
static void GLAD_API_PTR glad_debug_impl_glXSwapBuffers(Display * dpy, GLXDrawable drawable) {
    _pre_call_glx_callback("glXSwapBuffers", (GLADapiproc) glad_glXSwapBuffers, 2, dpy, drawable);
    glad_glXSwapBuffers(dpy, drawable);
    _post_call_glx_callback(NULL, "glXSwapBuffers", (GLADapiproc) glad_glXSwapBuffers, 2, dpy, drawable);
    
}
PFNGLXSWAPBUFFERSPROC glad_debug_glXSwapBuffers = glad_debug_impl_glXSwapBuffers;
PFNGLXSWAPBUFFERSMSCOMLPROC glad_glXSwapBuffersMscOML = NULL;
static int64_t GLAD_API_PTR glad_debug_impl_glXSwapBuffersMscOML(Display * dpy, GLXDrawable drawable, int64_t target_msc, int64_t divisor, int64_t remainder) {
    int64_t ret;
    _pre_call_glx_callback("glXSwapBuffersMscOML", (GLADapiproc) glad_glXSwapBuffersMscOML, 5, dpy, drawable, target_msc, divisor, remainder);
    ret = glad_glXSwapBuffersMscOML(dpy, drawable, target_msc, divisor, remainder);
    _post_call_glx_callback((void*) &ret, "glXSwapBuffersMscOML", (GLADapiproc) glad_glXSwapBuffersMscOML, 5, dpy, drawable, target_msc, divisor, remainder);
    return ret;
}
PFNGLXSWAPBUFFERSMSCOMLPROC glad_debug_glXSwapBuffersMscOML = glad_debug_impl_glXSwapBuffersMscOML;
PFNGLXSWAPINTERVALEXTPROC glad_glXSwapIntervalEXT = NULL;
static void GLAD_API_PTR glad_debug_impl_glXSwapIntervalEXT(Display * dpy, GLXDrawable drawable, int interval) {
    _pre_call_glx_callback("glXSwapIntervalEXT", (GLADapiproc) glad_glXSwapIntervalEXT, 3, dpy, drawable, interval);
    glad_glXSwapIntervalEXT(dpy, drawable, interval);
    _post_call_glx_callback(NULL, "glXSwapIntervalEXT", (GLADapiproc) glad_glXSwapIntervalEXT, 3, dpy, drawable, interval);
    
}
PFNGLXSWAPINTERVALEXTPROC glad_debug_glXSwapIntervalEXT = glad_debug_impl_glXSwapIntervalEXT;
PFNGLXSWAPINTERVALMESAPROC glad_glXSwapIntervalMESA = NULL;
static int GLAD_API_PTR glad_debug_impl_glXSwapIntervalMESA(unsigned int interval) {
    int ret;
    _pre_call_glx_callback("glXSwapIntervalMESA", (GLADapiproc) glad_glXSwapIntervalMESA, 1, interval);
    ret = glad_glXSwapIntervalMESA(interval);
    _post_call_glx_callback((void*) &ret, "glXSwapIntervalMESA", (GLADapiproc) glad_glXSwapIntervalMESA, 1, interval);
    return ret;
}
PFNGLXSWAPINTERVALMESAPROC glad_debug_glXSwapIntervalMESA = glad_debug_impl_glXSwapIntervalMESA;
PFNGLXSWAPINTERVALSGIPROC glad_glXSwapIntervalSGI = NULL;
static int GLAD_API_PTR glad_debug_impl_glXSwapIntervalSGI(int interval) {
    int ret;
    _pre_call_glx_callback("glXSwapIntervalSGI", (GLADapiproc) glad_glXSwapIntervalSGI, 1, interval);
    ret = glad_glXSwapIntervalSGI(interval);
    _post_call_glx_callback((void*) &ret, "glXSwapIntervalSGI", (GLADapiproc) glad_glXSwapIntervalSGI, 1, interval);
    return ret;
}
PFNGLXSWAPINTERVALSGIPROC glad_debug_glXSwapIntervalSGI = glad_debug_impl_glXSwapIntervalSGI;
PFNGLXUSEXFONTPROC glad_glXUseXFont = NULL;
static void GLAD_API_PTR glad_debug_impl_glXUseXFont(Font font, int first, int count, int list) {
    _pre_call_glx_callback("glXUseXFont", (GLADapiproc) glad_glXUseXFont, 4, font, first, count, list);
    glad_glXUseXFont(font, first, count, list);
    _post_call_glx_callback(NULL, "glXUseXFont", (GLADapiproc) glad_glXUseXFont, 4, font, first, count, list);
    
}
PFNGLXUSEXFONTPROC glad_debug_glXUseXFont = glad_debug_impl_glXUseXFont;
PFNGLXWAITFORMSCOMLPROC glad_glXWaitForMscOML = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXWaitForMscOML(Display * dpy, GLXDrawable drawable, int64_t target_msc, int64_t divisor, int64_t remainder, int64_t * ust, int64_t * msc, int64_t * sbc) {
    Bool ret;
    _pre_call_glx_callback("glXWaitForMscOML", (GLADapiproc) glad_glXWaitForMscOML, 8, dpy, drawable, target_msc, divisor, remainder, ust, msc, sbc);
    ret = glad_glXWaitForMscOML(dpy, drawable, target_msc, divisor, remainder, ust, msc, sbc);
    _post_call_glx_callback((void*) &ret, "glXWaitForMscOML", (GLADapiproc) glad_glXWaitForMscOML, 8, dpy, drawable, target_msc, divisor, remainder, ust, msc, sbc);
    return ret;
}
PFNGLXWAITFORMSCOMLPROC glad_debug_glXWaitForMscOML = glad_debug_impl_glXWaitForMscOML;
PFNGLXWAITFORSBCOMLPROC glad_glXWaitForSbcOML = NULL;
static Bool GLAD_API_PTR glad_debug_impl_glXWaitForSbcOML(Display * dpy, GLXDrawable drawable, int64_t target_sbc, int64_t * ust, int64_t * msc, int64_t * sbc) {
    Bool ret;
    _pre_call_glx_callback("glXWaitForSbcOML", (GLADapiproc) glad_glXWaitForSbcOML, 6, dpy, drawable, target_sbc, ust, msc, sbc);
    ret = glad_glXWaitForSbcOML(dpy, drawable, target_sbc, ust, msc, sbc);
    _post_call_glx_callback((void*) &ret, "glXWaitForSbcOML", (GLADapiproc) glad_glXWaitForSbcOML, 6, dpy, drawable, target_sbc, ust, msc, sbc);
    return ret;
}
PFNGLXWAITFORSBCOMLPROC glad_debug_glXWaitForSbcOML = glad_debug_impl_glXWaitForSbcOML;
PFNGLXWAITGLPROC glad_glXWaitGL = NULL;
static void GLAD_API_PTR glad_debug_impl_glXWaitGL(void) {
    _pre_call_glx_callback("glXWaitGL", (GLADapiproc) glad_glXWaitGL, 0);
    glad_glXWaitGL();
    _post_call_glx_callback(NULL, "glXWaitGL", (GLADapiproc) glad_glXWaitGL, 0);
    
}
PFNGLXWAITGLPROC glad_debug_glXWaitGL = glad_debug_impl_glXWaitGL;
PFNGLXWAITVIDEOSYNCSGIPROC glad_glXWaitVideoSyncSGI = NULL;
static int GLAD_API_PTR glad_debug_impl_glXWaitVideoSyncSGI(int divisor, int remainder, unsigned int * count) {
    int ret;
    _pre_call_glx_callback("glXWaitVideoSyncSGI", (GLADapiproc) glad_glXWaitVideoSyncSGI, 3, divisor, remainder, count);
    ret = glad_glXWaitVideoSyncSGI(divisor, remainder, count);
    _post_call_glx_callback((void*) &ret, "glXWaitVideoSyncSGI", (GLADapiproc) glad_glXWaitVideoSyncSGI, 3, divisor, remainder, count);
    return ret;
}
PFNGLXWAITVIDEOSYNCSGIPROC glad_debug_glXWaitVideoSyncSGI = glad_debug_impl_glXWaitVideoSyncSGI;
PFNGLXWAITXPROC glad_glXWaitX = NULL;
static void GLAD_API_PTR glad_debug_impl_glXWaitX(void) {
    _pre_call_glx_callback("glXWaitX", (GLADapiproc) glad_glXWaitX, 0);
    glad_glXWaitX();
    _post_call_glx_callback(NULL, "glXWaitX", (GLADapiproc) glad_glXWaitX, 0);
    
}
PFNGLXWAITXPROC glad_debug_glXWaitX = glad_debug_impl_glXWaitX;


static void glad_glx_load_GLX_VERSION_1_0( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_VERSION_1_0) return;
    glad_glXChooseVisual = (PFNGLXCHOOSEVISUALPROC) load(userptr, "glXChooseVisual");
    glad_glXCopyContext = (PFNGLXCOPYCONTEXTPROC) load(userptr, "glXCopyContext");
    glad_glXCreateContext = (PFNGLXCREATECONTEXTPROC) load(userptr, "glXCreateContext");
    glad_glXCreateGLXPixmap = (PFNGLXCREATEGLXPIXMAPPROC) load(userptr, "glXCreateGLXPixmap");
    glad_glXDestroyContext = (PFNGLXDESTROYCONTEXTPROC) load(userptr, "glXDestroyContext");
    glad_glXDestroyGLXPixmap = (PFNGLXDESTROYGLXPIXMAPPROC) load(userptr, "glXDestroyGLXPixmap");
    glad_glXGetConfig = (PFNGLXGETCONFIGPROC) load(userptr, "glXGetConfig");
    glad_glXGetCurrentContext = (PFNGLXGETCURRENTCONTEXTPROC) load(userptr, "glXGetCurrentContext");
    glad_glXGetCurrentDrawable = (PFNGLXGETCURRENTDRAWABLEPROC) load(userptr, "glXGetCurrentDrawable");
    glad_glXIsDirect = (PFNGLXISDIRECTPROC) load(userptr, "glXIsDirect");
    glad_glXMakeCurrent = (PFNGLXMAKECURRENTPROC) load(userptr, "glXMakeCurrent");
    glad_glXQueryExtension = (PFNGLXQUERYEXTENSIONPROC) load(userptr, "glXQueryExtension");
    glad_glXQueryVersion = (PFNGLXQUERYVERSIONPROC) load(userptr, "glXQueryVersion");
    glad_glXSwapBuffers = (PFNGLXSWAPBUFFERSPROC) load(userptr, "glXSwapBuffers");
    glad_glXUseXFont = (PFNGLXUSEXFONTPROC) load(userptr, "glXUseXFont");
    glad_glXWaitGL = (PFNGLXWAITGLPROC) load(userptr, "glXWaitGL");
    glad_glXWaitX = (PFNGLXWAITXPROC) load(userptr, "glXWaitX");
}
static void glad_glx_load_GLX_VERSION_1_1( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_VERSION_1_1) return;
    glad_glXGetClientString = (PFNGLXGETCLIENTSTRINGPROC) load(userptr, "glXGetClientString");
    glad_glXQueryExtensionsString = (PFNGLXQUERYEXTENSIONSSTRINGPROC) load(userptr, "glXQueryExtensionsString");
    glad_glXQueryServerString = (PFNGLXQUERYSERVERSTRINGPROC) load(userptr, "glXQueryServerString");
}
static void glad_glx_load_GLX_VERSION_1_2( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_VERSION_1_2) return;
    glad_glXGetCurrentDisplay = (PFNGLXGETCURRENTDISPLAYPROC) load(userptr, "glXGetCurrentDisplay");
}
static void glad_glx_load_GLX_VERSION_1_3( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_VERSION_1_3) return;
    glad_glXChooseFBConfig = (PFNGLXCHOOSEFBCONFIGPROC) load(userptr, "glXChooseFBConfig");
    glad_glXCreateNewContext = (PFNGLXCREATENEWCONTEXTPROC) load(userptr, "glXCreateNewContext");
    glad_glXCreatePbuffer = (PFNGLXCREATEPBUFFERPROC) load(userptr, "glXCreatePbuffer");
    glad_glXCreatePixmap = (PFNGLXCREATEPIXMAPPROC) load(userptr, "glXCreatePixmap");
    glad_glXCreateWindow = (PFNGLXCREATEWINDOWPROC) load(userptr, "glXCreateWindow");
    glad_glXDestroyPbuffer = (PFNGLXDESTROYPBUFFERPROC) load(userptr, "glXDestroyPbuffer");
    glad_glXDestroyPixmap = (PFNGLXDESTROYPIXMAPPROC) load(userptr, "glXDestroyPixmap");
    glad_glXDestroyWindow = (PFNGLXDESTROYWINDOWPROC) load(userptr, "glXDestroyWindow");
    glad_glXGetCurrentReadDrawable = (PFNGLXGETCURRENTREADDRAWABLEPROC) load(userptr, "glXGetCurrentReadDrawable");
    glad_glXGetFBConfigAttrib = (PFNGLXGETFBCONFIGATTRIBPROC) load(userptr, "glXGetFBConfigAttrib");
    glad_glXGetFBConfigs = (PFNGLXGETFBCONFIGSPROC) load(userptr, "glXGetFBConfigs");
    glad_glXGetSelectedEvent = (PFNGLXGETSELECTEDEVENTPROC) load(userptr, "glXGetSelectedEvent");
    glad_glXGetVisualFromFBConfig = (PFNGLXGETVISUALFROMFBCONFIGPROC) load(userptr, "glXGetVisualFromFBConfig");
    glad_glXMakeContextCurrent = (PFNGLXMAKECONTEXTCURRENTPROC) load(userptr, "glXMakeContextCurrent");
    glad_glXQueryContext = (PFNGLXQUERYCONTEXTPROC) load(userptr, "glXQueryContext");
    glad_glXQueryDrawable = (PFNGLXQUERYDRAWABLEPROC) load(userptr, "glXQueryDrawable");
    glad_glXSelectEvent = (PFNGLXSELECTEVENTPROC) load(userptr, "glXSelectEvent");
}
static void glad_glx_load_GLX_VERSION_1_4( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_VERSION_1_4) return;
    glad_glXGetProcAddress = (PFNGLXGETPROCADDRESSPROC) load(userptr, "glXGetProcAddress");
}
static void glad_glx_load_GLX_AMD_gpu_association( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_AMD_gpu_association) return;
    glad_glXBlitContextFramebufferAMD = (PFNGLXBLITCONTEXTFRAMEBUFFERAMDPROC) load(userptr, "glXBlitContextFramebufferAMD");
    glad_glXCreateAssociatedContextAMD = (PFNGLXCREATEASSOCIATEDCONTEXTAMDPROC) load(userptr, "glXCreateAssociatedContextAMD");
    glad_glXCreateAssociatedContextAttribsAMD = (PFNGLXCREATEASSOCIATEDCONTEXTATTRIBSAMDPROC) load(userptr, "glXCreateAssociatedContextAttribsAMD");
    glad_glXDeleteAssociatedContextAMD = (PFNGLXDELETEASSOCIATEDCONTEXTAMDPROC) load(userptr, "glXDeleteAssociatedContextAMD");
    glad_glXGetContextGPUIDAMD = (PFNGLXGETCONTEXTGPUIDAMDPROC) load(userptr, "glXGetContextGPUIDAMD");
    glad_glXGetCurrentAssociatedContextAMD = (PFNGLXGETCURRENTASSOCIATEDCONTEXTAMDPROC) load(userptr, "glXGetCurrentAssociatedContextAMD");
    glad_glXGetGPUIDsAMD = (PFNGLXGETGPUIDSAMDPROC) load(userptr, "glXGetGPUIDsAMD");
    glad_glXGetGPUInfoAMD = (PFNGLXGETGPUINFOAMDPROC) load(userptr, "glXGetGPUInfoAMD");
    glad_glXMakeAssociatedContextCurrentAMD = (PFNGLXMAKEASSOCIATEDCONTEXTCURRENTAMDPROC) load(userptr, "glXMakeAssociatedContextCurrentAMD");
}
static void glad_glx_load_GLX_ARB_create_context( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_ARB_create_context) return;
    glad_glXCreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC) load(userptr, "glXCreateContextAttribsARB");
}
static void glad_glx_load_GLX_ARB_get_proc_address( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_ARB_get_proc_address) return;
    glad_glXGetProcAddressARB = (PFNGLXGETPROCADDRESSARBPROC) load(userptr, "glXGetProcAddressARB");
}
static void glad_glx_load_GLX_EXT_import_context( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_EXT_import_context) return;
    glad_glXFreeContextEXT = (PFNGLXFREECONTEXTEXTPROC) load(userptr, "glXFreeContextEXT");
    glad_glXGetContextIDEXT = (PFNGLXGETCONTEXTIDEXTPROC) load(userptr, "glXGetContextIDEXT");
    glad_glXGetCurrentDisplayEXT = (PFNGLXGETCURRENTDISPLAYEXTPROC) load(userptr, "glXGetCurrentDisplayEXT");
    glad_glXImportContextEXT = (PFNGLXIMPORTCONTEXTEXTPROC) load(userptr, "glXImportContextEXT");
    glad_glXQueryContextInfoEXT = (PFNGLXQUERYCONTEXTINFOEXTPROC) load(userptr, "glXQueryContextInfoEXT");
}
static void glad_glx_load_GLX_EXT_swap_control( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_EXT_swap_control) return;
    glad_glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC) load(userptr, "glXSwapIntervalEXT");
}
static void glad_glx_load_GLX_EXT_texture_from_pixmap( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_EXT_texture_from_pixmap) return;
    glad_glXBindTexImageEXT = (PFNGLXBINDTEXIMAGEEXTPROC) load(userptr, "glXBindTexImageEXT");
    glad_glXReleaseTexImageEXT = (PFNGLXRELEASETEXIMAGEEXTPROC) load(userptr, "glXReleaseTexImageEXT");
}
static void glad_glx_load_GLX_MESA_agp_offset( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_MESA_agp_offset) return;
    glad_glXGetAGPOffsetMESA = (PFNGLXGETAGPOFFSETMESAPROC) load(userptr, "glXGetAGPOffsetMESA");
}
static void glad_glx_load_GLX_MESA_copy_sub_buffer( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_MESA_copy_sub_buffer) return;
    glad_glXCopySubBufferMESA = (PFNGLXCOPYSUBBUFFERMESAPROC) load(userptr, "glXCopySubBufferMESA");
}
static void glad_glx_load_GLX_MESA_pixmap_colormap( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_MESA_pixmap_colormap) return;
    glad_glXCreateGLXPixmapMESA = (PFNGLXCREATEGLXPIXMAPMESAPROC) load(userptr, "glXCreateGLXPixmapMESA");
}
static void glad_glx_load_GLX_MESA_query_renderer( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_MESA_query_renderer) return;
    glad_glXQueryCurrentRendererIntegerMESA = (PFNGLXQUERYCURRENTRENDERERINTEGERMESAPROC) load(userptr, "glXQueryCurrentRendererIntegerMESA");
    glad_glXQueryCurrentRendererStringMESA = (PFNGLXQUERYCURRENTRENDERERSTRINGMESAPROC) load(userptr, "glXQueryCurrentRendererStringMESA");
    glad_glXQueryRendererIntegerMESA = (PFNGLXQUERYRENDERERINTEGERMESAPROC) load(userptr, "glXQueryRendererIntegerMESA");
    glad_glXQueryRendererStringMESA = (PFNGLXQUERYRENDERERSTRINGMESAPROC) load(userptr, "glXQueryRendererStringMESA");
}
static void glad_glx_load_GLX_MESA_release_buffers( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_MESA_release_buffers) return;
    glad_glXReleaseBuffersMESA = (PFNGLXRELEASEBUFFERSMESAPROC) load(userptr, "glXReleaseBuffersMESA");
}
static void glad_glx_load_GLX_MESA_set_3dfx_mode( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_MESA_set_3dfx_mode) return;
    glad_glXSet3DfxModeMESA = (PFNGLXSET3DFXMODEMESAPROC) load(userptr, "glXSet3DfxModeMESA");
}
static void glad_glx_load_GLX_MESA_swap_control( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_MESA_swap_control) return;
    glad_glXGetSwapIntervalMESA = (PFNGLXGETSWAPINTERVALMESAPROC) load(userptr, "glXGetSwapIntervalMESA");
    glad_glXSwapIntervalMESA = (PFNGLXSWAPINTERVALMESAPROC) load(userptr, "glXSwapIntervalMESA");
}
static void glad_glx_load_GLX_NV_copy_buffer( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_NV_copy_buffer) return;
    glad_glXCopyBufferSubDataNV = (PFNGLXCOPYBUFFERSUBDATANVPROC) load(userptr, "glXCopyBufferSubDataNV");
    glad_glXNamedCopyBufferSubDataNV = (PFNGLXNAMEDCOPYBUFFERSUBDATANVPROC) load(userptr, "glXNamedCopyBufferSubDataNV");
}
static void glad_glx_load_GLX_NV_copy_image( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_NV_copy_image) return;
    glad_glXCopyImageSubDataNV = (PFNGLXCOPYIMAGESUBDATANVPROC) load(userptr, "glXCopyImageSubDataNV");
}
static void glad_glx_load_GLX_NV_delay_before_swap( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_NV_delay_before_swap) return;
    glad_glXDelayBeforeSwapNV = (PFNGLXDELAYBEFORESWAPNVPROC) load(userptr, "glXDelayBeforeSwapNV");
}
static void glad_glx_load_GLX_NV_present_video( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_NV_present_video) return;
    glad_glXBindVideoDeviceNV = (PFNGLXBINDVIDEODEVICENVPROC) load(userptr, "glXBindVideoDeviceNV");
    glad_glXEnumerateVideoDevicesNV = (PFNGLXENUMERATEVIDEODEVICESNVPROC) load(userptr, "glXEnumerateVideoDevicesNV");
}
static void glad_glx_load_GLX_NV_swap_group( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_NV_swap_group) return;
    glad_glXBindSwapBarrierNV = (PFNGLXBINDSWAPBARRIERNVPROC) load(userptr, "glXBindSwapBarrierNV");
    glad_glXJoinSwapGroupNV = (PFNGLXJOINSWAPGROUPNVPROC) load(userptr, "glXJoinSwapGroupNV");
    glad_glXQueryFrameCountNV = (PFNGLXQUERYFRAMECOUNTNVPROC) load(userptr, "glXQueryFrameCountNV");
    glad_glXQueryMaxSwapGroupsNV = (PFNGLXQUERYMAXSWAPGROUPSNVPROC) load(userptr, "glXQueryMaxSwapGroupsNV");
    glad_glXQuerySwapGroupNV = (PFNGLXQUERYSWAPGROUPNVPROC) load(userptr, "glXQuerySwapGroupNV");
    glad_glXResetFrameCountNV = (PFNGLXRESETFRAMECOUNTNVPROC) load(userptr, "glXResetFrameCountNV");
}
static void glad_glx_load_GLX_NV_video_capture( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_NV_video_capture) return;
    glad_glXBindVideoCaptureDeviceNV = (PFNGLXBINDVIDEOCAPTUREDEVICENVPROC) load(userptr, "glXBindVideoCaptureDeviceNV");
    glad_glXEnumerateVideoCaptureDevicesNV = (PFNGLXENUMERATEVIDEOCAPTUREDEVICESNVPROC) load(userptr, "glXEnumerateVideoCaptureDevicesNV");
    glad_glXLockVideoCaptureDeviceNV = (PFNGLXLOCKVIDEOCAPTUREDEVICENVPROC) load(userptr, "glXLockVideoCaptureDeviceNV");
    glad_glXQueryVideoCaptureDeviceNV = (PFNGLXQUERYVIDEOCAPTUREDEVICENVPROC) load(userptr, "glXQueryVideoCaptureDeviceNV");
    glad_glXReleaseVideoCaptureDeviceNV = (PFNGLXRELEASEVIDEOCAPTUREDEVICENVPROC) load(userptr, "glXReleaseVideoCaptureDeviceNV");
}
static void glad_glx_load_GLX_NV_video_out( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_NV_video_out) return;
    glad_glXBindVideoImageNV = (PFNGLXBINDVIDEOIMAGENVPROC) load(userptr, "glXBindVideoImageNV");
    glad_glXGetVideoDeviceNV = (PFNGLXGETVIDEODEVICENVPROC) load(userptr, "glXGetVideoDeviceNV");
    glad_glXGetVideoInfoNV = (PFNGLXGETVIDEOINFONVPROC) load(userptr, "glXGetVideoInfoNV");
    glad_glXReleaseVideoDeviceNV = (PFNGLXRELEASEVIDEODEVICENVPROC) load(userptr, "glXReleaseVideoDeviceNV");
    glad_glXReleaseVideoImageNV = (PFNGLXRELEASEVIDEOIMAGENVPROC) load(userptr, "glXReleaseVideoImageNV");
    glad_glXSendPbufferToVideoNV = (PFNGLXSENDPBUFFERTOVIDEONVPROC) load(userptr, "glXSendPbufferToVideoNV");
}
static void glad_glx_load_GLX_OML_sync_control( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_OML_sync_control) return;
    glad_glXGetMscRateOML = (PFNGLXGETMSCRATEOMLPROC) load(userptr, "glXGetMscRateOML");
    glad_glXGetSyncValuesOML = (PFNGLXGETSYNCVALUESOMLPROC) load(userptr, "glXGetSyncValuesOML");
    glad_glXSwapBuffersMscOML = (PFNGLXSWAPBUFFERSMSCOMLPROC) load(userptr, "glXSwapBuffersMscOML");
    glad_glXWaitForMscOML = (PFNGLXWAITFORMSCOMLPROC) load(userptr, "glXWaitForMscOML");
    glad_glXWaitForSbcOML = (PFNGLXWAITFORSBCOMLPROC) load(userptr, "glXWaitForSbcOML");
}
static void glad_glx_load_GLX_SGIX_fbconfig( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_SGIX_fbconfig) return;
    glad_glXChooseFBConfigSGIX = (PFNGLXCHOOSEFBCONFIGSGIXPROC) load(userptr, "glXChooseFBConfigSGIX");
    glad_glXCreateContextWithConfigSGIX = (PFNGLXCREATECONTEXTWITHCONFIGSGIXPROC) load(userptr, "glXCreateContextWithConfigSGIX");
    glad_glXCreateGLXPixmapWithConfigSGIX = (PFNGLXCREATEGLXPIXMAPWITHCONFIGSGIXPROC) load(userptr, "glXCreateGLXPixmapWithConfigSGIX");
    glad_glXGetFBConfigAttribSGIX = (PFNGLXGETFBCONFIGATTRIBSGIXPROC) load(userptr, "glXGetFBConfigAttribSGIX");
    glad_glXGetFBConfigFromVisualSGIX = (PFNGLXGETFBCONFIGFROMVISUALSGIXPROC) load(userptr, "glXGetFBConfigFromVisualSGIX");
    glad_glXGetVisualFromFBConfigSGIX = (PFNGLXGETVISUALFROMFBCONFIGSGIXPROC) load(userptr, "glXGetVisualFromFBConfigSGIX");
}
static void glad_glx_load_GLX_SGIX_hyperpipe( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_SGIX_hyperpipe) return;
    glad_glXBindHyperpipeSGIX = (PFNGLXBINDHYPERPIPESGIXPROC) load(userptr, "glXBindHyperpipeSGIX");
    glad_glXDestroyHyperpipeConfigSGIX = (PFNGLXDESTROYHYPERPIPECONFIGSGIXPROC) load(userptr, "glXDestroyHyperpipeConfigSGIX");
    glad_glXHyperpipeAttribSGIX = (PFNGLXHYPERPIPEATTRIBSGIXPROC) load(userptr, "glXHyperpipeAttribSGIX");
    glad_glXHyperpipeConfigSGIX = (PFNGLXHYPERPIPECONFIGSGIXPROC) load(userptr, "glXHyperpipeConfigSGIX");
    glad_glXQueryHyperpipeAttribSGIX = (PFNGLXQUERYHYPERPIPEATTRIBSGIXPROC) load(userptr, "glXQueryHyperpipeAttribSGIX");
    glad_glXQueryHyperpipeBestAttribSGIX = (PFNGLXQUERYHYPERPIPEBESTATTRIBSGIXPROC) load(userptr, "glXQueryHyperpipeBestAttribSGIX");
    glad_glXQueryHyperpipeConfigSGIX = (PFNGLXQUERYHYPERPIPECONFIGSGIXPROC) load(userptr, "glXQueryHyperpipeConfigSGIX");
    glad_glXQueryHyperpipeNetworkSGIX = (PFNGLXQUERYHYPERPIPENETWORKSGIXPROC) load(userptr, "glXQueryHyperpipeNetworkSGIX");
}
static void glad_glx_load_GLX_SGIX_pbuffer( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_SGIX_pbuffer) return;
    glad_glXCreateGLXPbufferSGIX = (PFNGLXCREATEGLXPBUFFERSGIXPROC) load(userptr, "glXCreateGLXPbufferSGIX");
    glad_glXDestroyGLXPbufferSGIX = (PFNGLXDESTROYGLXPBUFFERSGIXPROC) load(userptr, "glXDestroyGLXPbufferSGIX");
    glad_glXGetSelectedEventSGIX = (PFNGLXGETSELECTEDEVENTSGIXPROC) load(userptr, "glXGetSelectedEventSGIX");
    glad_glXQueryGLXPbufferSGIX = (PFNGLXQUERYGLXPBUFFERSGIXPROC) load(userptr, "glXQueryGLXPbufferSGIX");
    glad_glXSelectEventSGIX = (PFNGLXSELECTEVENTSGIXPROC) load(userptr, "glXSelectEventSGIX");
}
static void glad_glx_load_GLX_SGIX_swap_barrier( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_SGIX_swap_barrier) return;
    glad_glXBindSwapBarrierSGIX = (PFNGLXBINDSWAPBARRIERSGIXPROC) load(userptr, "glXBindSwapBarrierSGIX");
    glad_glXQueryMaxSwapBarriersSGIX = (PFNGLXQUERYMAXSWAPBARRIERSSGIXPROC) load(userptr, "glXQueryMaxSwapBarriersSGIX");
}
static void glad_glx_load_GLX_SGIX_swap_group( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_SGIX_swap_group) return;
    glad_glXJoinSwapGroupSGIX = (PFNGLXJOINSWAPGROUPSGIXPROC) load(userptr, "glXJoinSwapGroupSGIX");
}
static void glad_glx_load_GLX_SGIX_video_resize( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_SGIX_video_resize) return;
    glad_glXBindChannelToWindowSGIX = (PFNGLXBINDCHANNELTOWINDOWSGIXPROC) load(userptr, "glXBindChannelToWindowSGIX");
    glad_glXChannelRectSGIX = (PFNGLXCHANNELRECTSGIXPROC) load(userptr, "glXChannelRectSGIX");
    glad_glXChannelRectSyncSGIX = (PFNGLXCHANNELRECTSYNCSGIXPROC) load(userptr, "glXChannelRectSyncSGIX");
    glad_glXQueryChannelDeltasSGIX = (PFNGLXQUERYCHANNELDELTASSGIXPROC) load(userptr, "glXQueryChannelDeltasSGIX");
    glad_glXQueryChannelRectSGIX = (PFNGLXQUERYCHANNELRECTSGIXPROC) load(userptr, "glXQueryChannelRectSGIX");
}
static void glad_glx_load_GLX_SGI_cushion( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_SGI_cushion) return;
    glad_glXCushionSGI = (PFNGLXCUSHIONSGIPROC) load(userptr, "glXCushionSGI");
}
static void glad_glx_load_GLX_SGI_make_current_read( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_SGI_make_current_read) return;
    glad_glXGetCurrentReadDrawableSGI = (PFNGLXGETCURRENTREADDRAWABLESGIPROC) load(userptr, "glXGetCurrentReadDrawableSGI");
    glad_glXMakeCurrentReadSGI = (PFNGLXMAKECURRENTREADSGIPROC) load(userptr, "glXMakeCurrentReadSGI");
}
static void glad_glx_load_GLX_SGI_swap_control( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_SGI_swap_control) return;
    glad_glXSwapIntervalSGI = (PFNGLXSWAPINTERVALSGIPROC) load(userptr, "glXSwapIntervalSGI");
}
static void glad_glx_load_GLX_SGI_video_sync( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_SGI_video_sync) return;
    glad_glXGetVideoSyncSGI = (PFNGLXGETVIDEOSYNCSGIPROC) load(userptr, "glXGetVideoSyncSGI");
    glad_glXWaitVideoSyncSGI = (PFNGLXWAITVIDEOSYNCSGIPROC) load(userptr, "glXWaitVideoSyncSGI");
}
static void glad_glx_load_GLX_SUN_get_transparent_index( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GLX_SUN_get_transparent_index) return;
    glad_glXGetTransparentIndexSUN = (PFNGLXGETTRANSPARENTINDEXSUNPROC) load(userptr, "glXGetTransparentIndexSUN");
}


static void glad_glx_resolve_aliases(void) {
}

static int glad_glx_has_extension(Display *display, int screen, const char *ext) {
#ifndef GLX_VERSION_1_1
    GLAD_UNUSED(display);
    GLAD_UNUSED(screen);
    GLAD_UNUSED(ext);
#else
    const char *terminator;
    const char *loc;
    const char *extensions;

    if (glXQueryExtensionsString == NULL) {
        return 0;
    }

    extensions = glXQueryExtensionsString(display, screen);

    if(extensions == NULL || ext == NULL) {
        return 0;
    }

    while(1) {
        loc = strstr(extensions, ext);
        if(loc == NULL)
            break;

        terminator = loc + strlen(ext);
        if((loc == extensions || *(loc - 1) == ' ') &&
            (*terminator == ' ' || *terminator == '\0')) {
            return 1;
        }
        extensions = terminator;
    }
#endif

    return 0;
}

static GLADapiproc glad_glx_get_proc_from_userptr(void *userptr, const char* name) {
    return (GLAD_GNUC_EXTENSION (GLADapiproc (*)(const char *name)) userptr)(name);
}

static int glad_glx_find_extensions(Display *display, int screen) {
    GLAD_GLX_3DFX_multisample = glad_glx_has_extension(display, screen, "GLX_3DFX_multisample");
    GLAD_GLX_AMD_gpu_association = glad_glx_has_extension(display, screen, "GLX_AMD_gpu_association");
    GLAD_GLX_ARB_context_flush_control = glad_glx_has_extension(display, screen, "GLX_ARB_context_flush_control");
    GLAD_GLX_ARB_create_context = glad_glx_has_extension(display, screen, "GLX_ARB_create_context");
    GLAD_GLX_ARB_create_context_no_error = glad_glx_has_extension(display, screen, "GLX_ARB_create_context_no_error");
    GLAD_GLX_ARB_create_context_profile = glad_glx_has_extension(display, screen, "GLX_ARB_create_context_profile");
    GLAD_GLX_ARB_create_context_robustness = glad_glx_has_extension(display, screen, "GLX_ARB_create_context_robustness");
    GLAD_GLX_ARB_fbconfig_float = glad_glx_has_extension(display, screen, "GLX_ARB_fbconfig_float");
    GLAD_GLX_ARB_framebuffer_sRGB = glad_glx_has_extension(display, screen, "GLX_ARB_framebuffer_sRGB");
    GLAD_GLX_ARB_get_proc_address = glad_glx_has_extension(display, screen, "GLX_ARB_get_proc_address");
    GLAD_GLX_ARB_multisample = glad_glx_has_extension(display, screen, "GLX_ARB_multisample");
    GLAD_GLX_ARB_robustness_application_isolation = glad_glx_has_extension(display, screen, "GLX_ARB_robustness_application_isolation");
    GLAD_GLX_ARB_robustness_share_group_isolation = glad_glx_has_extension(display, screen, "GLX_ARB_robustness_share_group_isolation");
    GLAD_GLX_ARB_vertex_buffer_object = glad_glx_has_extension(display, screen, "GLX_ARB_vertex_buffer_object");
    GLAD_GLX_EXT_buffer_age = glad_glx_has_extension(display, screen, "GLX_EXT_buffer_age");
    GLAD_GLX_EXT_context_priority = glad_glx_has_extension(display, screen, "GLX_EXT_context_priority");
    GLAD_GLX_EXT_create_context_es2_profile = glad_glx_has_extension(display, screen, "GLX_EXT_create_context_es2_profile");
    GLAD_GLX_EXT_create_context_es_profile = glad_glx_has_extension(display, screen, "GLX_EXT_create_context_es_profile");
    GLAD_GLX_EXT_fbconfig_packed_float = glad_glx_has_extension(display, screen, "GLX_EXT_fbconfig_packed_float");
    GLAD_GLX_EXT_framebuffer_sRGB = glad_glx_has_extension(display, screen, "GLX_EXT_framebuffer_sRGB");
    GLAD_GLX_EXT_get_drawable_type = glad_glx_has_extension(display, screen, "GLX_EXT_get_drawable_type");
    GLAD_GLX_EXT_import_context = glad_glx_has_extension(display, screen, "GLX_EXT_import_context");
    GLAD_GLX_EXT_libglvnd = glad_glx_has_extension(display, screen, "GLX_EXT_libglvnd");
    GLAD_GLX_EXT_no_config_context = glad_glx_has_extension(display, screen, "GLX_EXT_no_config_context");
    GLAD_GLX_EXT_stereo_tree = glad_glx_has_extension(display, screen, "GLX_EXT_stereo_tree");
    GLAD_GLX_EXT_swap_control = glad_glx_has_extension(display, screen, "GLX_EXT_swap_control");
    GLAD_GLX_EXT_swap_control_tear = glad_glx_has_extension(display, screen, "GLX_EXT_swap_control_tear");
    GLAD_GLX_EXT_texture_from_pixmap = glad_glx_has_extension(display, screen, "GLX_EXT_texture_from_pixmap");
    GLAD_GLX_EXT_visual_info = glad_glx_has_extension(display, screen, "GLX_EXT_visual_info");
    GLAD_GLX_EXT_visual_rating = glad_glx_has_extension(display, screen, "GLX_EXT_visual_rating");
    GLAD_GLX_INTEL_swap_event = glad_glx_has_extension(display, screen, "GLX_INTEL_swap_event");
    GLAD_GLX_MESA_agp_offset = glad_glx_has_extension(display, screen, "GLX_MESA_agp_offset");
    GLAD_GLX_MESA_copy_sub_buffer = glad_glx_has_extension(display, screen, "GLX_MESA_copy_sub_buffer");
    GLAD_GLX_MESA_pixmap_colormap = glad_glx_has_extension(display, screen, "GLX_MESA_pixmap_colormap");
    GLAD_GLX_MESA_query_renderer = glad_glx_has_extension(display, screen, "GLX_MESA_query_renderer");
    GLAD_GLX_MESA_release_buffers = glad_glx_has_extension(display, screen, "GLX_MESA_release_buffers");
    GLAD_GLX_MESA_set_3dfx_mode = glad_glx_has_extension(display, screen, "GLX_MESA_set_3dfx_mode");
    GLAD_GLX_MESA_swap_control = glad_glx_has_extension(display, screen, "GLX_MESA_swap_control");
    GLAD_GLX_NV_copy_buffer = glad_glx_has_extension(display, screen, "GLX_NV_copy_buffer");
    GLAD_GLX_NV_copy_image = glad_glx_has_extension(display, screen, "GLX_NV_copy_image");
    GLAD_GLX_NV_delay_before_swap = glad_glx_has_extension(display, screen, "GLX_NV_delay_before_swap");
    GLAD_GLX_NV_float_buffer = glad_glx_has_extension(display, screen, "GLX_NV_float_buffer");
    GLAD_GLX_NV_multigpu_context = glad_glx_has_extension(display, screen, "GLX_NV_multigpu_context");
    GLAD_GLX_NV_multisample_coverage = glad_glx_has_extension(display, screen, "GLX_NV_multisample_coverage");
    GLAD_GLX_NV_present_video = glad_glx_has_extension(display, screen, "GLX_NV_present_video");
    GLAD_GLX_NV_robustness_video_memory_purge = glad_glx_has_extension(display, screen, "GLX_NV_robustness_video_memory_purge");
    GLAD_GLX_NV_swap_group = glad_glx_has_extension(display, screen, "GLX_NV_swap_group");
    GLAD_GLX_NV_video_capture = glad_glx_has_extension(display, screen, "GLX_NV_video_capture");
    GLAD_GLX_NV_video_out = glad_glx_has_extension(display, screen, "GLX_NV_video_out");
    GLAD_GLX_OML_swap_method = glad_glx_has_extension(display, screen, "GLX_OML_swap_method");
    GLAD_GLX_OML_sync_control = glad_glx_has_extension(display, screen, "GLX_OML_sync_control");
    GLAD_GLX_SGIS_blended_overlay = glad_glx_has_extension(display, screen, "GLX_SGIS_blended_overlay");
    GLAD_GLX_SGIS_multisample = glad_glx_has_extension(display, screen, "GLX_SGIS_multisample");
    GLAD_GLX_SGIS_shared_multisample = glad_glx_has_extension(display, screen, "GLX_SGIS_shared_multisample");
    GLAD_GLX_SGIX_fbconfig = glad_glx_has_extension(display, screen, "GLX_SGIX_fbconfig");
    GLAD_GLX_SGIX_hyperpipe = glad_glx_has_extension(display, screen, "GLX_SGIX_hyperpipe");
    GLAD_GLX_SGIX_pbuffer = glad_glx_has_extension(display, screen, "GLX_SGIX_pbuffer");
    GLAD_GLX_SGIX_swap_barrier = glad_glx_has_extension(display, screen, "GLX_SGIX_swap_barrier");
    GLAD_GLX_SGIX_swap_group = glad_glx_has_extension(display, screen, "GLX_SGIX_swap_group");
    GLAD_GLX_SGIX_video_resize = glad_glx_has_extension(display, screen, "GLX_SGIX_video_resize");
    GLAD_GLX_SGIX_visual_select_group = glad_glx_has_extension(display, screen, "GLX_SGIX_visual_select_group");
    GLAD_GLX_SGI_cushion = glad_glx_has_extension(display, screen, "GLX_SGI_cushion");
    GLAD_GLX_SGI_make_current_read = glad_glx_has_extension(display, screen, "GLX_SGI_make_current_read");
    GLAD_GLX_SGI_swap_control = glad_glx_has_extension(display, screen, "GLX_SGI_swap_control");
    GLAD_GLX_SGI_video_sync = glad_glx_has_extension(display, screen, "GLX_SGI_video_sync");
    GLAD_GLX_SUN_get_transparent_index = glad_glx_has_extension(display, screen, "GLX_SUN_get_transparent_index");
    return 1;
}

static int glad_glx_find_core_glx(Display **display, int *screen) {
    int major = 0, minor = 0;
    if(*display == NULL) {
#ifdef GLAD_GLX_NO_X11
        GLAD_UNUSED(screen);
        return 0;
#else
        *display = XOpenDisplay(0);
        if (*display == NULL) {
            return 0;
        }
        *screen = XScreenNumberOfScreen(XDefaultScreenOfDisplay(*display));
#endif
    }
    glXQueryVersion(*display, &major, &minor);
    GLAD_GLX_VERSION_1_0 = (major == 1 && minor >= 0) || major > 1;
    GLAD_GLX_VERSION_1_1 = (major == 1 && minor >= 1) || major > 1;
    GLAD_GLX_VERSION_1_2 = (major == 1 && minor >= 2) || major > 1;
    GLAD_GLX_VERSION_1_3 = (major == 1 && minor >= 3) || major > 1;
    GLAD_GLX_VERSION_1_4 = (major == 1 && minor >= 4) || major > 1;
    return GLAD_MAKE_VERSION(major, minor);
}

int gladLoadGLXUserPtr(Display *display, int screen, GLADuserptrloadfunc load, void *userptr) {
    int version;
    glXQueryVersion = (PFNGLXQUERYVERSIONPROC) load(userptr, "glXQueryVersion");
    if(glXQueryVersion == NULL) return 0;
    version = glad_glx_find_core_glx(&display, &screen);

    glad_glx_load_GLX_VERSION_1_0(load, userptr);
    glad_glx_load_GLX_VERSION_1_1(load, userptr);
    glad_glx_load_GLX_VERSION_1_2(load, userptr);
    glad_glx_load_GLX_VERSION_1_3(load, userptr);
    glad_glx_load_GLX_VERSION_1_4(load, userptr);

    if (!glad_glx_find_extensions(display, screen)) return 0;
    glad_glx_load_GLX_AMD_gpu_association(load, userptr);
    glad_glx_load_GLX_ARB_create_context(load, userptr);
    glad_glx_load_GLX_ARB_get_proc_address(load, userptr);
    glad_glx_load_GLX_EXT_import_context(load, userptr);
    glad_glx_load_GLX_EXT_swap_control(load, userptr);
    glad_glx_load_GLX_EXT_texture_from_pixmap(load, userptr);
    glad_glx_load_GLX_MESA_agp_offset(load, userptr);
    glad_glx_load_GLX_MESA_copy_sub_buffer(load, userptr);
    glad_glx_load_GLX_MESA_pixmap_colormap(load, userptr);
    glad_glx_load_GLX_MESA_query_renderer(load, userptr);
    glad_glx_load_GLX_MESA_release_buffers(load, userptr);
    glad_glx_load_GLX_MESA_set_3dfx_mode(load, userptr);
    glad_glx_load_GLX_MESA_swap_control(load, userptr);
    glad_glx_load_GLX_NV_copy_buffer(load, userptr);
    glad_glx_load_GLX_NV_copy_image(load, userptr);
    glad_glx_load_GLX_NV_delay_before_swap(load, userptr);
    glad_glx_load_GLX_NV_present_video(load, userptr);
    glad_glx_load_GLX_NV_swap_group(load, userptr);
    glad_glx_load_GLX_NV_video_capture(load, userptr);
    glad_glx_load_GLX_NV_video_out(load, userptr);
    glad_glx_load_GLX_OML_sync_control(load, userptr);
    glad_glx_load_GLX_SGIX_fbconfig(load, userptr);
    glad_glx_load_GLX_SGIX_hyperpipe(load, userptr);
    glad_glx_load_GLX_SGIX_pbuffer(load, userptr);
    glad_glx_load_GLX_SGIX_swap_barrier(load, userptr);
    glad_glx_load_GLX_SGIX_swap_group(load, userptr);
    glad_glx_load_GLX_SGIX_video_resize(load, userptr);
    glad_glx_load_GLX_SGI_cushion(load, userptr);
    glad_glx_load_GLX_SGI_make_current_read(load, userptr);
    glad_glx_load_GLX_SGI_swap_control(load, userptr);
    glad_glx_load_GLX_SGI_video_sync(load, userptr);
    glad_glx_load_GLX_SUN_get_transparent_index(load, userptr);

    glad_glx_resolve_aliases();

    return version;
}

int gladLoadGLX(Display *display, int screen, GLADloadfunc load) {
    return gladLoadGLXUserPtr(display, screen, glad_glx_get_proc_from_userptr, GLAD_GNUC_EXTENSION (void*) load);
}

 
void gladInstallGLXDebug(void) {
    glad_debug_glXBindChannelToWindowSGIX = glad_debug_impl_glXBindChannelToWindowSGIX;
    glad_debug_glXBindHyperpipeSGIX = glad_debug_impl_glXBindHyperpipeSGIX;
    glad_debug_glXBindSwapBarrierNV = glad_debug_impl_glXBindSwapBarrierNV;
    glad_debug_glXBindSwapBarrierSGIX = glad_debug_impl_glXBindSwapBarrierSGIX;
    glad_debug_glXBindTexImageEXT = glad_debug_impl_glXBindTexImageEXT;
    glad_debug_glXBindVideoCaptureDeviceNV = glad_debug_impl_glXBindVideoCaptureDeviceNV;
    glad_debug_glXBindVideoDeviceNV = glad_debug_impl_glXBindVideoDeviceNV;
    glad_debug_glXBindVideoImageNV = glad_debug_impl_glXBindVideoImageNV;
    glad_debug_glXBlitContextFramebufferAMD = glad_debug_impl_glXBlitContextFramebufferAMD;
    glad_debug_glXChannelRectSGIX = glad_debug_impl_glXChannelRectSGIX;
    glad_debug_glXChannelRectSyncSGIX = glad_debug_impl_glXChannelRectSyncSGIX;
    glad_debug_glXChooseFBConfig = glad_debug_impl_glXChooseFBConfig;
    glad_debug_glXChooseFBConfigSGIX = glad_debug_impl_glXChooseFBConfigSGIX;
    glad_debug_glXChooseVisual = glad_debug_impl_glXChooseVisual;
    glad_debug_glXCopyBufferSubDataNV = glad_debug_impl_glXCopyBufferSubDataNV;
    glad_debug_glXCopyContext = glad_debug_impl_glXCopyContext;
    glad_debug_glXCopyImageSubDataNV = glad_debug_impl_glXCopyImageSubDataNV;
    glad_debug_glXCopySubBufferMESA = glad_debug_impl_glXCopySubBufferMESA;
    glad_debug_glXCreateAssociatedContextAMD = glad_debug_impl_glXCreateAssociatedContextAMD;
    glad_debug_glXCreateAssociatedContextAttribsAMD = glad_debug_impl_glXCreateAssociatedContextAttribsAMD;
    glad_debug_glXCreateContext = glad_debug_impl_glXCreateContext;
    glad_debug_glXCreateContextAttribsARB = glad_debug_impl_glXCreateContextAttribsARB;
    glad_debug_glXCreateContextWithConfigSGIX = glad_debug_impl_glXCreateContextWithConfigSGIX;
    glad_debug_glXCreateGLXPbufferSGIX = glad_debug_impl_glXCreateGLXPbufferSGIX;
    glad_debug_glXCreateGLXPixmap = glad_debug_impl_glXCreateGLXPixmap;
    glad_debug_glXCreateGLXPixmapMESA = glad_debug_impl_glXCreateGLXPixmapMESA;
    glad_debug_glXCreateGLXPixmapWithConfigSGIX = glad_debug_impl_glXCreateGLXPixmapWithConfigSGIX;
    glad_debug_glXCreateNewContext = glad_debug_impl_glXCreateNewContext;
    glad_debug_glXCreatePbuffer = glad_debug_impl_glXCreatePbuffer;
    glad_debug_glXCreatePixmap = glad_debug_impl_glXCreatePixmap;
    glad_debug_glXCreateWindow = glad_debug_impl_glXCreateWindow;
    glad_debug_glXCushionSGI = glad_debug_impl_glXCushionSGI;
    glad_debug_glXDelayBeforeSwapNV = glad_debug_impl_glXDelayBeforeSwapNV;
    glad_debug_glXDeleteAssociatedContextAMD = glad_debug_impl_glXDeleteAssociatedContextAMD;
    glad_debug_glXDestroyContext = glad_debug_impl_glXDestroyContext;
    glad_debug_glXDestroyGLXPbufferSGIX = glad_debug_impl_glXDestroyGLXPbufferSGIX;
    glad_debug_glXDestroyGLXPixmap = glad_debug_impl_glXDestroyGLXPixmap;
    glad_debug_glXDestroyHyperpipeConfigSGIX = glad_debug_impl_glXDestroyHyperpipeConfigSGIX;
    glad_debug_glXDestroyPbuffer = glad_debug_impl_glXDestroyPbuffer;
    glad_debug_glXDestroyPixmap = glad_debug_impl_glXDestroyPixmap;
    glad_debug_glXDestroyWindow = glad_debug_impl_glXDestroyWindow;
    glad_debug_glXEnumerateVideoCaptureDevicesNV = glad_debug_impl_glXEnumerateVideoCaptureDevicesNV;
    glad_debug_glXEnumerateVideoDevicesNV = glad_debug_impl_glXEnumerateVideoDevicesNV;
    glad_debug_glXFreeContextEXT = glad_debug_impl_glXFreeContextEXT;
    glad_debug_glXGetAGPOffsetMESA = glad_debug_impl_glXGetAGPOffsetMESA;
    glad_debug_glXGetClientString = glad_debug_impl_glXGetClientString;
    glad_debug_glXGetConfig = glad_debug_impl_glXGetConfig;
    glad_debug_glXGetContextGPUIDAMD = glad_debug_impl_glXGetContextGPUIDAMD;
    glad_debug_glXGetContextIDEXT = glad_debug_impl_glXGetContextIDEXT;
    glad_debug_glXGetCurrentAssociatedContextAMD = glad_debug_impl_glXGetCurrentAssociatedContextAMD;
    glad_debug_glXGetCurrentContext = glad_debug_impl_glXGetCurrentContext;
    glad_debug_glXGetCurrentDisplay = glad_debug_impl_glXGetCurrentDisplay;
    glad_debug_glXGetCurrentDisplayEXT = glad_debug_impl_glXGetCurrentDisplayEXT;
    glad_debug_glXGetCurrentDrawable = glad_debug_impl_glXGetCurrentDrawable;
    glad_debug_glXGetCurrentReadDrawable = glad_debug_impl_glXGetCurrentReadDrawable;
    glad_debug_glXGetCurrentReadDrawableSGI = glad_debug_impl_glXGetCurrentReadDrawableSGI;
    glad_debug_glXGetFBConfigAttrib = glad_debug_impl_glXGetFBConfigAttrib;
    glad_debug_glXGetFBConfigAttribSGIX = glad_debug_impl_glXGetFBConfigAttribSGIX;
    glad_debug_glXGetFBConfigFromVisualSGIX = glad_debug_impl_glXGetFBConfigFromVisualSGIX;
    glad_debug_glXGetFBConfigs = glad_debug_impl_glXGetFBConfigs;
    glad_debug_glXGetGPUIDsAMD = glad_debug_impl_glXGetGPUIDsAMD;
    glad_debug_glXGetGPUInfoAMD = glad_debug_impl_glXGetGPUInfoAMD;
    glad_debug_glXGetMscRateOML = glad_debug_impl_glXGetMscRateOML;
    glad_debug_glXGetProcAddress = glad_debug_impl_glXGetProcAddress;
    glad_debug_glXGetProcAddressARB = glad_debug_impl_glXGetProcAddressARB;
    glad_debug_glXGetSelectedEvent = glad_debug_impl_glXGetSelectedEvent;
    glad_debug_glXGetSelectedEventSGIX = glad_debug_impl_glXGetSelectedEventSGIX;
    glad_debug_glXGetSwapIntervalMESA = glad_debug_impl_glXGetSwapIntervalMESA;
    glad_debug_glXGetSyncValuesOML = glad_debug_impl_glXGetSyncValuesOML;
    glad_debug_glXGetTransparentIndexSUN = glad_debug_impl_glXGetTransparentIndexSUN;
    glad_debug_glXGetVideoDeviceNV = glad_debug_impl_glXGetVideoDeviceNV;
    glad_debug_glXGetVideoInfoNV = glad_debug_impl_glXGetVideoInfoNV;
    glad_debug_glXGetVideoSyncSGI = glad_debug_impl_glXGetVideoSyncSGI;
    glad_debug_glXGetVisualFromFBConfig = glad_debug_impl_glXGetVisualFromFBConfig;
    glad_debug_glXGetVisualFromFBConfigSGIX = glad_debug_impl_glXGetVisualFromFBConfigSGIX;
    glad_debug_glXHyperpipeAttribSGIX = glad_debug_impl_glXHyperpipeAttribSGIX;
    glad_debug_glXHyperpipeConfigSGIX = glad_debug_impl_glXHyperpipeConfigSGIX;
    glad_debug_glXImportContextEXT = glad_debug_impl_glXImportContextEXT;
    glad_debug_glXIsDirect = glad_debug_impl_glXIsDirect;
    glad_debug_glXJoinSwapGroupNV = glad_debug_impl_glXJoinSwapGroupNV;
    glad_debug_glXJoinSwapGroupSGIX = glad_debug_impl_glXJoinSwapGroupSGIX;
    glad_debug_glXLockVideoCaptureDeviceNV = glad_debug_impl_glXLockVideoCaptureDeviceNV;
    glad_debug_glXMakeAssociatedContextCurrentAMD = glad_debug_impl_glXMakeAssociatedContextCurrentAMD;
    glad_debug_glXMakeContextCurrent = glad_debug_impl_glXMakeContextCurrent;
    glad_debug_glXMakeCurrent = glad_debug_impl_glXMakeCurrent;
    glad_debug_glXMakeCurrentReadSGI = glad_debug_impl_glXMakeCurrentReadSGI;
    glad_debug_glXNamedCopyBufferSubDataNV = glad_debug_impl_glXNamedCopyBufferSubDataNV;
    glad_debug_glXQueryChannelDeltasSGIX = glad_debug_impl_glXQueryChannelDeltasSGIX;
    glad_debug_glXQueryChannelRectSGIX = glad_debug_impl_glXQueryChannelRectSGIX;
    glad_debug_glXQueryContext = glad_debug_impl_glXQueryContext;
    glad_debug_glXQueryContextInfoEXT = glad_debug_impl_glXQueryContextInfoEXT;
    glad_debug_glXQueryCurrentRendererIntegerMESA = glad_debug_impl_glXQueryCurrentRendererIntegerMESA;
    glad_debug_glXQueryCurrentRendererStringMESA = glad_debug_impl_glXQueryCurrentRendererStringMESA;
    glad_debug_glXQueryDrawable = glad_debug_impl_glXQueryDrawable;
    glad_debug_glXQueryExtension = glad_debug_impl_glXQueryExtension;
    glad_debug_glXQueryExtensionsString = glad_debug_impl_glXQueryExtensionsString;
    glad_debug_glXQueryFrameCountNV = glad_debug_impl_glXQueryFrameCountNV;
    glad_debug_glXQueryGLXPbufferSGIX = glad_debug_impl_glXQueryGLXPbufferSGIX;
    glad_debug_glXQueryHyperpipeAttribSGIX = glad_debug_impl_glXQueryHyperpipeAttribSGIX;
    glad_debug_glXQueryHyperpipeBestAttribSGIX = glad_debug_impl_glXQueryHyperpipeBestAttribSGIX;
    glad_debug_glXQueryHyperpipeConfigSGIX = glad_debug_impl_glXQueryHyperpipeConfigSGIX;
    glad_debug_glXQueryHyperpipeNetworkSGIX = glad_debug_impl_glXQueryHyperpipeNetworkSGIX;
    glad_debug_glXQueryMaxSwapBarriersSGIX = glad_debug_impl_glXQueryMaxSwapBarriersSGIX;
    glad_debug_glXQueryMaxSwapGroupsNV = glad_debug_impl_glXQueryMaxSwapGroupsNV;
    glad_debug_glXQueryRendererIntegerMESA = glad_debug_impl_glXQueryRendererIntegerMESA;
    glad_debug_glXQueryRendererStringMESA = glad_debug_impl_glXQueryRendererStringMESA;
    glad_debug_glXQueryServerString = glad_debug_impl_glXQueryServerString;
    glad_debug_glXQuerySwapGroupNV = glad_debug_impl_glXQuerySwapGroupNV;
    glad_debug_glXQueryVersion = glad_debug_impl_glXQueryVersion;
    glad_debug_glXQueryVideoCaptureDeviceNV = glad_debug_impl_glXQueryVideoCaptureDeviceNV;
    glad_debug_glXReleaseBuffersMESA = glad_debug_impl_glXReleaseBuffersMESA;
    glad_debug_glXReleaseTexImageEXT = glad_debug_impl_glXReleaseTexImageEXT;
    glad_debug_glXReleaseVideoCaptureDeviceNV = glad_debug_impl_glXReleaseVideoCaptureDeviceNV;
    glad_debug_glXReleaseVideoDeviceNV = glad_debug_impl_glXReleaseVideoDeviceNV;
    glad_debug_glXReleaseVideoImageNV = glad_debug_impl_glXReleaseVideoImageNV;
    glad_debug_glXResetFrameCountNV = glad_debug_impl_glXResetFrameCountNV;
    glad_debug_glXSelectEvent = glad_debug_impl_glXSelectEvent;
    glad_debug_glXSelectEventSGIX = glad_debug_impl_glXSelectEventSGIX;
    glad_debug_glXSendPbufferToVideoNV = glad_debug_impl_glXSendPbufferToVideoNV;
    glad_debug_glXSet3DfxModeMESA = glad_debug_impl_glXSet3DfxModeMESA;
    glad_debug_glXSwapBuffers = glad_debug_impl_glXSwapBuffers;
    glad_debug_glXSwapBuffersMscOML = glad_debug_impl_glXSwapBuffersMscOML;
    glad_debug_glXSwapIntervalEXT = glad_debug_impl_glXSwapIntervalEXT;
    glad_debug_glXSwapIntervalMESA = glad_debug_impl_glXSwapIntervalMESA;
    glad_debug_glXSwapIntervalSGI = glad_debug_impl_glXSwapIntervalSGI;
    glad_debug_glXUseXFont = glad_debug_impl_glXUseXFont;
    glad_debug_glXWaitForMscOML = glad_debug_impl_glXWaitForMscOML;
    glad_debug_glXWaitForSbcOML = glad_debug_impl_glXWaitForSbcOML;
    glad_debug_glXWaitGL = glad_debug_impl_glXWaitGL;
    glad_debug_glXWaitVideoSyncSGI = glad_debug_impl_glXWaitVideoSyncSGI;
    glad_debug_glXWaitX = glad_debug_impl_glXWaitX;
}

void gladUninstallGLXDebug(void) {
    glad_debug_glXBindChannelToWindowSGIX = glad_glXBindChannelToWindowSGIX;
    glad_debug_glXBindHyperpipeSGIX = glad_glXBindHyperpipeSGIX;
    glad_debug_glXBindSwapBarrierNV = glad_glXBindSwapBarrierNV;
    glad_debug_glXBindSwapBarrierSGIX = glad_glXBindSwapBarrierSGIX;
    glad_debug_glXBindTexImageEXT = glad_glXBindTexImageEXT;
    glad_debug_glXBindVideoCaptureDeviceNV = glad_glXBindVideoCaptureDeviceNV;
    glad_debug_glXBindVideoDeviceNV = glad_glXBindVideoDeviceNV;
    glad_debug_glXBindVideoImageNV = glad_glXBindVideoImageNV;
    glad_debug_glXBlitContextFramebufferAMD = glad_glXBlitContextFramebufferAMD;
    glad_debug_glXChannelRectSGIX = glad_glXChannelRectSGIX;
    glad_debug_glXChannelRectSyncSGIX = glad_glXChannelRectSyncSGIX;
    glad_debug_glXChooseFBConfig = glad_glXChooseFBConfig;
    glad_debug_glXChooseFBConfigSGIX = glad_glXChooseFBConfigSGIX;
    glad_debug_glXChooseVisual = glad_glXChooseVisual;
    glad_debug_glXCopyBufferSubDataNV = glad_glXCopyBufferSubDataNV;
    glad_debug_glXCopyContext = glad_glXCopyContext;
    glad_debug_glXCopyImageSubDataNV = glad_glXCopyImageSubDataNV;
    glad_debug_glXCopySubBufferMESA = glad_glXCopySubBufferMESA;
    glad_debug_glXCreateAssociatedContextAMD = glad_glXCreateAssociatedContextAMD;
    glad_debug_glXCreateAssociatedContextAttribsAMD = glad_glXCreateAssociatedContextAttribsAMD;
    glad_debug_glXCreateContext = glad_glXCreateContext;
    glad_debug_glXCreateContextAttribsARB = glad_glXCreateContextAttribsARB;
    glad_debug_glXCreateContextWithConfigSGIX = glad_glXCreateContextWithConfigSGIX;
    glad_debug_glXCreateGLXPbufferSGIX = glad_glXCreateGLXPbufferSGIX;
    glad_debug_glXCreateGLXPixmap = glad_glXCreateGLXPixmap;
    glad_debug_glXCreateGLXPixmapMESA = glad_glXCreateGLXPixmapMESA;
    glad_debug_glXCreateGLXPixmapWithConfigSGIX = glad_glXCreateGLXPixmapWithConfigSGIX;
    glad_debug_glXCreateNewContext = glad_glXCreateNewContext;
    glad_debug_glXCreatePbuffer = glad_glXCreatePbuffer;
    glad_debug_glXCreatePixmap = glad_glXCreatePixmap;
    glad_debug_glXCreateWindow = glad_glXCreateWindow;
    glad_debug_glXCushionSGI = glad_glXCushionSGI;
    glad_debug_glXDelayBeforeSwapNV = glad_glXDelayBeforeSwapNV;
    glad_debug_glXDeleteAssociatedContextAMD = glad_glXDeleteAssociatedContextAMD;
    glad_debug_glXDestroyContext = glad_glXDestroyContext;
    glad_debug_glXDestroyGLXPbufferSGIX = glad_glXDestroyGLXPbufferSGIX;
    glad_debug_glXDestroyGLXPixmap = glad_glXDestroyGLXPixmap;
    glad_debug_glXDestroyHyperpipeConfigSGIX = glad_glXDestroyHyperpipeConfigSGIX;
    glad_debug_glXDestroyPbuffer = glad_glXDestroyPbuffer;
    glad_debug_glXDestroyPixmap = glad_glXDestroyPixmap;
    glad_debug_glXDestroyWindow = glad_glXDestroyWindow;
    glad_debug_glXEnumerateVideoCaptureDevicesNV = glad_glXEnumerateVideoCaptureDevicesNV;
    glad_debug_glXEnumerateVideoDevicesNV = glad_glXEnumerateVideoDevicesNV;
    glad_debug_glXFreeContextEXT = glad_glXFreeContextEXT;
    glad_debug_glXGetAGPOffsetMESA = glad_glXGetAGPOffsetMESA;
    glad_debug_glXGetClientString = glad_glXGetClientString;
    glad_debug_glXGetConfig = glad_glXGetConfig;
    glad_debug_glXGetContextGPUIDAMD = glad_glXGetContextGPUIDAMD;
    glad_debug_glXGetContextIDEXT = glad_glXGetContextIDEXT;
    glad_debug_glXGetCurrentAssociatedContextAMD = glad_glXGetCurrentAssociatedContextAMD;
    glad_debug_glXGetCurrentContext = glad_glXGetCurrentContext;
    glad_debug_glXGetCurrentDisplay = glad_glXGetCurrentDisplay;
    glad_debug_glXGetCurrentDisplayEXT = glad_glXGetCurrentDisplayEXT;
    glad_debug_glXGetCurrentDrawable = glad_glXGetCurrentDrawable;
    glad_debug_glXGetCurrentReadDrawable = glad_glXGetCurrentReadDrawable;
    glad_debug_glXGetCurrentReadDrawableSGI = glad_glXGetCurrentReadDrawableSGI;
    glad_debug_glXGetFBConfigAttrib = glad_glXGetFBConfigAttrib;
    glad_debug_glXGetFBConfigAttribSGIX = glad_glXGetFBConfigAttribSGIX;
    glad_debug_glXGetFBConfigFromVisualSGIX = glad_glXGetFBConfigFromVisualSGIX;
    glad_debug_glXGetFBConfigs = glad_glXGetFBConfigs;
    glad_debug_glXGetGPUIDsAMD = glad_glXGetGPUIDsAMD;
    glad_debug_glXGetGPUInfoAMD = glad_glXGetGPUInfoAMD;
    glad_debug_glXGetMscRateOML = glad_glXGetMscRateOML;
    glad_debug_glXGetProcAddress = glad_glXGetProcAddress;
    glad_debug_glXGetProcAddressARB = glad_glXGetProcAddressARB;
    glad_debug_glXGetSelectedEvent = glad_glXGetSelectedEvent;
    glad_debug_glXGetSelectedEventSGIX = glad_glXGetSelectedEventSGIX;
    glad_debug_glXGetSwapIntervalMESA = glad_glXGetSwapIntervalMESA;
    glad_debug_glXGetSyncValuesOML = glad_glXGetSyncValuesOML;
    glad_debug_glXGetTransparentIndexSUN = glad_glXGetTransparentIndexSUN;
    glad_debug_glXGetVideoDeviceNV = glad_glXGetVideoDeviceNV;
    glad_debug_glXGetVideoInfoNV = glad_glXGetVideoInfoNV;
    glad_debug_glXGetVideoSyncSGI = glad_glXGetVideoSyncSGI;
    glad_debug_glXGetVisualFromFBConfig = glad_glXGetVisualFromFBConfig;
    glad_debug_glXGetVisualFromFBConfigSGIX = glad_glXGetVisualFromFBConfigSGIX;
    glad_debug_glXHyperpipeAttribSGIX = glad_glXHyperpipeAttribSGIX;
    glad_debug_glXHyperpipeConfigSGIX = glad_glXHyperpipeConfigSGIX;
    glad_debug_glXImportContextEXT = glad_glXImportContextEXT;
    glad_debug_glXIsDirect = glad_glXIsDirect;
    glad_debug_glXJoinSwapGroupNV = glad_glXJoinSwapGroupNV;
    glad_debug_glXJoinSwapGroupSGIX = glad_glXJoinSwapGroupSGIX;
    glad_debug_glXLockVideoCaptureDeviceNV = glad_glXLockVideoCaptureDeviceNV;
    glad_debug_glXMakeAssociatedContextCurrentAMD = glad_glXMakeAssociatedContextCurrentAMD;
    glad_debug_glXMakeContextCurrent = glad_glXMakeContextCurrent;
    glad_debug_glXMakeCurrent = glad_glXMakeCurrent;
    glad_debug_glXMakeCurrentReadSGI = glad_glXMakeCurrentReadSGI;
    glad_debug_glXNamedCopyBufferSubDataNV = glad_glXNamedCopyBufferSubDataNV;
    glad_debug_glXQueryChannelDeltasSGIX = glad_glXQueryChannelDeltasSGIX;
    glad_debug_glXQueryChannelRectSGIX = glad_glXQueryChannelRectSGIX;
    glad_debug_glXQueryContext = glad_glXQueryContext;
    glad_debug_glXQueryContextInfoEXT = glad_glXQueryContextInfoEXT;
    glad_debug_glXQueryCurrentRendererIntegerMESA = glad_glXQueryCurrentRendererIntegerMESA;
    glad_debug_glXQueryCurrentRendererStringMESA = glad_glXQueryCurrentRendererStringMESA;
    glad_debug_glXQueryDrawable = glad_glXQueryDrawable;
    glad_debug_glXQueryExtension = glad_glXQueryExtension;
    glad_debug_glXQueryExtensionsString = glad_glXQueryExtensionsString;
    glad_debug_glXQueryFrameCountNV = glad_glXQueryFrameCountNV;
    glad_debug_glXQueryGLXPbufferSGIX = glad_glXQueryGLXPbufferSGIX;
    glad_debug_glXQueryHyperpipeAttribSGIX = glad_glXQueryHyperpipeAttribSGIX;
    glad_debug_glXQueryHyperpipeBestAttribSGIX = glad_glXQueryHyperpipeBestAttribSGIX;
    glad_debug_glXQueryHyperpipeConfigSGIX = glad_glXQueryHyperpipeConfigSGIX;
    glad_debug_glXQueryHyperpipeNetworkSGIX = glad_glXQueryHyperpipeNetworkSGIX;
    glad_debug_glXQueryMaxSwapBarriersSGIX = glad_glXQueryMaxSwapBarriersSGIX;
    glad_debug_glXQueryMaxSwapGroupsNV = glad_glXQueryMaxSwapGroupsNV;
    glad_debug_glXQueryRendererIntegerMESA = glad_glXQueryRendererIntegerMESA;
    glad_debug_glXQueryRendererStringMESA = glad_glXQueryRendererStringMESA;
    glad_debug_glXQueryServerString = glad_glXQueryServerString;
    glad_debug_glXQuerySwapGroupNV = glad_glXQuerySwapGroupNV;
    glad_debug_glXQueryVersion = glad_glXQueryVersion;
    glad_debug_glXQueryVideoCaptureDeviceNV = glad_glXQueryVideoCaptureDeviceNV;
    glad_debug_glXReleaseBuffersMESA = glad_glXReleaseBuffersMESA;
    glad_debug_glXReleaseTexImageEXT = glad_glXReleaseTexImageEXT;
    glad_debug_glXReleaseVideoCaptureDeviceNV = glad_glXReleaseVideoCaptureDeviceNV;
    glad_debug_glXReleaseVideoDeviceNV = glad_glXReleaseVideoDeviceNV;
    glad_debug_glXReleaseVideoImageNV = glad_glXReleaseVideoImageNV;
    glad_debug_glXResetFrameCountNV = glad_glXResetFrameCountNV;
    glad_debug_glXSelectEvent = glad_glXSelectEvent;
    glad_debug_glXSelectEventSGIX = glad_glXSelectEventSGIX;
    glad_debug_glXSendPbufferToVideoNV = glad_glXSendPbufferToVideoNV;
    glad_debug_glXSet3DfxModeMESA = glad_glXSet3DfxModeMESA;
    glad_debug_glXSwapBuffers = glad_glXSwapBuffers;
    glad_debug_glXSwapBuffersMscOML = glad_glXSwapBuffersMscOML;
    glad_debug_glXSwapIntervalEXT = glad_glXSwapIntervalEXT;
    glad_debug_glXSwapIntervalMESA = glad_glXSwapIntervalMESA;
    glad_debug_glXSwapIntervalSGI = glad_glXSwapIntervalSGI;
    glad_debug_glXUseXFont = glad_glXUseXFont;
    glad_debug_glXWaitForMscOML = glad_glXWaitForMscOML;
    glad_debug_glXWaitForSbcOML = glad_glXWaitForSbcOML;
    glad_debug_glXWaitGL = glad_glXWaitGL;
    glad_debug_glXWaitVideoSyncSGI = glad_glXWaitVideoSyncSGI;
    glad_debug_glXWaitX = glad_glXWaitX;
}


#ifdef __cplusplus
}
#endif
