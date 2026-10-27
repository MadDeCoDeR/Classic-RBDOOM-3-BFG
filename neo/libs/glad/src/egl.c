/**
 * SPDX-License-Identifier: (WTFPL OR CC0-1.0) AND Apache-2.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glad/egl.h>

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



int GLAD_EGL_VERSION_1_0 = 0;
int GLAD_EGL_VERSION_1_1 = 0;
int GLAD_EGL_VERSION_1_2 = 0;
int GLAD_EGL_VERSION_1_3 = 0;
int GLAD_EGL_VERSION_1_4 = 0;
int GLAD_EGL_VERSION_1_5 = 0;
int GLAD_EGL_ANDROID_GLES_layers = 0;
int GLAD_EGL_ANDROID_blob_cache = 0;
int GLAD_EGL_ANDROID_create_native_client_buffer = 0;
int GLAD_EGL_ANDROID_framebuffer_target = 0;
int GLAD_EGL_ANDROID_front_buffer_auto_refresh = 0;
int GLAD_EGL_ANDROID_get_frame_timestamps = 0;
int GLAD_EGL_ANDROID_get_native_client_buffer = 0;
int GLAD_EGL_ANDROID_image_native_buffer = 0;
int GLAD_EGL_ANDROID_native_fence_sync = 0;
int GLAD_EGL_ANDROID_presentation_time = 0;
int GLAD_EGL_ANDROID_recordable = 0;
int GLAD_EGL_ANDROID_telemetry_hint = 0;
int GLAD_EGL_ANGLE_d3d_share_handle_client_buffer = 0;
int GLAD_EGL_ANGLE_device_d3d = 0;
int GLAD_EGL_ANGLE_query_surface_pointer = 0;
int GLAD_EGL_ANGLE_surface_d3d_texture_2d_share_handle = 0;
int GLAD_EGL_ANGLE_sync_control_rate = 0;
int GLAD_EGL_ANGLE_window_fixed_size = 0;
int GLAD_EGL_ARM_image_format = 0;
int GLAD_EGL_ARM_implicit_external_sync = 0;
int GLAD_EGL_ARM_pixmap_multisample_discard = 0;
int GLAD_EGL_EXT_bind_to_front = 0;
int GLAD_EGL_EXT_buffer_age = 0;
int GLAD_EGL_EXT_client_extensions = 0;
int GLAD_EGL_EXT_client_sync = 0;
int GLAD_EGL_EXT_compositor = 0;
int GLAD_EGL_EXT_config_select_group = 0;
int GLAD_EGL_EXT_create_context_robustness = 0;
int GLAD_EGL_EXT_device_base = 0;
int GLAD_EGL_EXT_device_drm = 0;
int GLAD_EGL_EXT_device_drm_render_node = 0;
int GLAD_EGL_EXT_device_enumeration = 0;
int GLAD_EGL_EXT_device_openwf = 0;
int GLAD_EGL_EXT_device_persistent_id = 0;
int GLAD_EGL_EXT_device_query = 0;
int GLAD_EGL_EXT_device_query_name = 0;
int GLAD_EGL_EXT_device_type = 0;
int GLAD_EGL_EXT_display_alloc = 0;
int GLAD_EGL_EXT_explicit_device = 0;
int GLAD_EGL_EXT_gl_colorspace_bt2020_hlg = 0;
int GLAD_EGL_EXT_gl_colorspace_bt2020_linear = 0;
int GLAD_EGL_EXT_gl_colorspace_bt2020_pq = 0;
int GLAD_EGL_EXT_gl_colorspace_display_p3 = 0;
int GLAD_EGL_EXT_gl_colorspace_display_p3_linear = 0;
int GLAD_EGL_EXT_gl_colorspace_display_p3_passthrough = 0;
int GLAD_EGL_EXT_gl_colorspace_scrgb = 0;
int GLAD_EGL_EXT_gl_colorspace_scrgb_linear = 0;
int GLAD_EGL_EXT_image_dma_buf_import = 0;
int GLAD_EGL_EXT_image_dma_buf_import_modifiers = 0;
int GLAD_EGL_EXT_image_gl_colorspace = 0;
int GLAD_EGL_EXT_image_implicit_sync_control = 0;
int GLAD_EGL_EXT_multiview_window = 0;
int GLAD_EGL_EXT_output_base = 0;
int GLAD_EGL_EXT_output_drm = 0;
int GLAD_EGL_EXT_output_openwf = 0;
int GLAD_EGL_EXT_pixel_format_float = 0;
int GLAD_EGL_EXT_platform_base = 0;
int GLAD_EGL_EXT_platform_device = 0;
int GLAD_EGL_EXT_platform_wayland = 0;
int GLAD_EGL_EXT_platform_x11 = 0;
int GLAD_EGL_EXT_platform_xcb = 0;
int GLAD_EGL_EXT_present_opaque = 0;
int GLAD_EGL_EXT_protected_content = 0;
int GLAD_EGL_EXT_protected_surface = 0;
int GLAD_EGL_EXT_query_reset_notification_strategy = 0;
int GLAD_EGL_EXT_stream_consumer_egloutput = 0;
int GLAD_EGL_EXT_surface_CTA861_3_metadata = 0;
int GLAD_EGL_EXT_surface_SMPTE2086_metadata = 0;
int GLAD_EGL_EXT_surface_compression = 0;
int GLAD_EGL_EXT_swap_buffers_with_damage = 0;
int GLAD_EGL_EXT_sync_reuse = 0;
int GLAD_EGL_EXT_yuv_surface = 0;
int GLAD_EGL_HI_clientpixmap = 0;
int GLAD_EGL_HI_colorformats = 0;
int GLAD_EGL_IMG_context_priority = 0;
int GLAD_EGL_IMG_image_plane_attribs = 0;
int GLAD_EGL_KHR_cl_event = 0;
int GLAD_EGL_KHR_cl_event2 = 0;
int GLAD_EGL_KHR_client_get_all_proc_addresses = 0;
int GLAD_EGL_KHR_config_attribs = 0;
int GLAD_EGL_KHR_context_flush_control = 0;
int GLAD_EGL_KHR_create_context = 0;
int GLAD_EGL_KHR_create_context_no_error = 0;
int GLAD_EGL_KHR_debug = 0;
int GLAD_EGL_KHR_display_reference = 0;
int GLAD_EGL_KHR_fence_sync = 0;
int GLAD_EGL_KHR_get_all_proc_addresses = 0;
int GLAD_EGL_KHR_gl_colorspace = 0;
int GLAD_EGL_KHR_gl_renderbuffer_image = 0;
int GLAD_EGL_KHR_gl_texture_2D_image = 0;
int GLAD_EGL_KHR_gl_texture_3D_image = 0;
int GLAD_EGL_KHR_gl_texture_cubemap_image = 0;
int GLAD_EGL_KHR_image = 0;
int GLAD_EGL_KHR_image_base = 0;
int GLAD_EGL_KHR_image_pixmap = 0;
int GLAD_EGL_KHR_lock_surface = 0;
int GLAD_EGL_KHR_lock_surface2 = 0;
int GLAD_EGL_KHR_lock_surface3 = 0;
int GLAD_EGL_KHR_mutable_render_buffer = 0;
int GLAD_EGL_KHR_no_config_context = 0;
int GLAD_EGL_KHR_partial_update = 0;
int GLAD_EGL_KHR_platform_android = 0;
int GLAD_EGL_KHR_platform_gbm = 0;
int GLAD_EGL_KHR_platform_wayland = 0;
int GLAD_EGL_KHR_platform_x11 = 0;
int GLAD_EGL_KHR_reusable_sync = 0;
int GLAD_EGL_KHR_stream = 0;
int GLAD_EGL_KHR_stream_attrib = 0;
int GLAD_EGL_KHR_stream_consumer_gltexture = 0;
int GLAD_EGL_KHR_stream_cross_process_fd = 0;
int GLAD_EGL_KHR_stream_fifo = 0;
int GLAD_EGL_KHR_stream_producer_aldatalocator = 0;
int GLAD_EGL_KHR_stream_producer_eglsurface = 0;
int GLAD_EGL_KHR_surfaceless_context = 0;
int GLAD_EGL_KHR_swap_buffers_with_damage = 0;
int GLAD_EGL_KHR_vg_parent_image = 0;
int GLAD_EGL_KHR_wait_sync = 0;
int GLAD_EGL_MESA_drm_image = 0;
int GLAD_EGL_MESA_image_dma_buf_export = 0;
int GLAD_EGL_MESA_platform_gbm = 0;
int GLAD_EGL_MESA_platform_surfaceless = 0;
int GLAD_EGL_MESA_query_driver = 0;
int GLAD_EGL_NOK_swap_region = 0;
int GLAD_EGL_NOK_swap_region2 = 0;
int GLAD_EGL_NOK_texture_from_pixmap = 0;
int GLAD_EGL_NV_3dvision_surface = 0;
int GLAD_EGL_NV_context_priority_realtime = 0;
int GLAD_EGL_NV_coverage_sample = 0;
int GLAD_EGL_NV_coverage_sample_resolve = 0;
int GLAD_EGL_NV_cuda_event = 0;
int GLAD_EGL_NV_depth_nonlinear = 0;
int GLAD_EGL_NV_device_cuda = 0;
int GLAD_EGL_NV_native_query = 0;
int GLAD_EGL_NV_post_convert_rounding = 0;
int GLAD_EGL_NV_post_sub_buffer = 0;
int GLAD_EGL_NV_quadruple_buffer = 0;
int GLAD_EGL_NV_robustness_video_memory_purge = 0;
int GLAD_EGL_NV_stream_consumer_eglimage = 0;
int GLAD_EGL_NV_stream_consumer_eglimage_use_scanout_attrib = 0;
int GLAD_EGL_NV_stream_consumer_gltexture_yuv = 0;
int GLAD_EGL_NV_stream_cross_display = 0;
int GLAD_EGL_NV_stream_cross_object = 0;
int GLAD_EGL_NV_stream_cross_partition = 0;
int GLAD_EGL_NV_stream_cross_process = 0;
int GLAD_EGL_NV_stream_cross_system = 0;
int GLAD_EGL_NV_stream_dma = 0;
int GLAD_EGL_NV_stream_fifo_next = 0;
int GLAD_EGL_NV_stream_fifo_synchronous = 0;
int GLAD_EGL_NV_stream_flush = 0;
int GLAD_EGL_NV_stream_frame_limits = 0;
int GLAD_EGL_NV_stream_metadata = 0;
int GLAD_EGL_NV_stream_origin = 0;
int GLAD_EGL_NV_stream_remote = 0;
int GLAD_EGL_NV_stream_reset = 0;
int GLAD_EGL_NV_stream_socket = 0;
int GLAD_EGL_NV_stream_socket_inet = 0;
int GLAD_EGL_NV_stream_socket_unix = 0;
int GLAD_EGL_NV_stream_sync = 0;
int GLAD_EGL_NV_sync = 0;
int GLAD_EGL_NV_system_time = 0;
int GLAD_EGL_NV_triple_buffer = 0;
int GLAD_EGL_QNX_image_native_buffer = 0;
int GLAD_EGL_QNX_platform_screen = 0;
int GLAD_EGL_TIZEN_image_native_buffer = 0;
int GLAD_EGL_TIZEN_image_native_surface = 0;
int GLAD_EGL_WL_bind_wayland_display = 0;
int GLAD_EGL_WL_create_wayland_buffer_from_image = 0;


static void _pre_call_egl_callback_default(const char *name, GLADapiproc apiproc, int len_args, ...) {
    GLAD_UNUSED(name);
    GLAD_UNUSED(apiproc);
    GLAD_UNUSED(len_args);
}
static void _post_call_egl_callback_default(void *ret, const char *name, GLADapiproc apiproc, int len_args, ...) {
    GLAD_UNUSED(ret);
    GLAD_UNUSED(name);
    GLAD_UNUSED(apiproc);
    GLAD_UNUSED(len_args);
}

static GLADprecallback _pre_call_egl_callback = _pre_call_egl_callback_default;
void gladSetEGLPreCallback(GLADprecallback cb) {
    _pre_call_egl_callback = cb;
}
static GLADpostcallback _post_call_egl_callback = _post_call_egl_callback_default;
void gladSetEGLPostCallback(GLADpostcallback cb) {
    _post_call_egl_callback = cb;
}

PFNEGLBINDAPIPROC glad_eglBindAPI = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglBindAPI(EGLenum api) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglBindAPI", (GLADapiproc) glad_eglBindAPI, 1, api);
    ret = glad_eglBindAPI(api);
    _post_call_egl_callback((void*) &ret, "eglBindAPI", (GLADapiproc) glad_eglBindAPI, 1, api);
    return ret;
}
PFNEGLBINDAPIPROC glad_debug_eglBindAPI = glad_debug_impl_eglBindAPI;
PFNEGLBINDTEXIMAGEPROC glad_eglBindTexImage = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglBindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglBindTexImage", (GLADapiproc) glad_eglBindTexImage, 3, dpy, surface, buffer);
    ret = glad_eglBindTexImage(dpy, surface, buffer);
    _post_call_egl_callback((void*) &ret, "eglBindTexImage", (GLADapiproc) glad_eglBindTexImage, 3, dpy, surface, buffer);
    return ret;
}
PFNEGLBINDTEXIMAGEPROC glad_debug_eglBindTexImage = glad_debug_impl_eglBindTexImage;
PFNEGLBINDWAYLANDDISPLAYWLPROC glad_eglBindWaylandDisplayWL = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglBindWaylandDisplayWL(EGLDisplay dpy, struct wl_display * display) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglBindWaylandDisplayWL", (GLADapiproc) glad_eglBindWaylandDisplayWL, 2, dpy, display);
    ret = glad_eglBindWaylandDisplayWL(dpy, display);
    _post_call_egl_callback((void*) &ret, "eglBindWaylandDisplayWL", (GLADapiproc) glad_eglBindWaylandDisplayWL, 2, dpy, display);
    return ret;
}
PFNEGLBINDWAYLANDDISPLAYWLPROC glad_debug_eglBindWaylandDisplayWL = glad_debug_impl_eglBindWaylandDisplayWL;
PFNEGLCHOOSECONFIGPROC glad_eglChooseConfig = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglChooseConfig(EGLDisplay dpy, const EGLint * attrib_list, EGLConfig * configs, EGLint config_size, EGLint * num_config) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglChooseConfig", (GLADapiproc) glad_eglChooseConfig, 5, dpy, attrib_list, configs, config_size, num_config);
    ret = glad_eglChooseConfig(dpy, attrib_list, configs, config_size, num_config);
    _post_call_egl_callback((void*) &ret, "eglChooseConfig", (GLADapiproc) glad_eglChooseConfig, 5, dpy, attrib_list, configs, config_size, num_config);
    return ret;
}
PFNEGLCHOOSECONFIGPROC glad_debug_eglChooseConfig = glad_debug_impl_eglChooseConfig;
PFNEGLCLIENTSIGNALSYNCEXTPROC glad_eglClientSignalSyncEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglClientSignalSyncEXT(EGLDisplay dpy, EGLSync sync, const EGLAttrib * attrib_list) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglClientSignalSyncEXT", (GLADapiproc) glad_eglClientSignalSyncEXT, 3, dpy, sync, attrib_list);
    ret = glad_eglClientSignalSyncEXT(dpy, sync, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglClientSignalSyncEXT", (GLADapiproc) glad_eglClientSignalSyncEXT, 3, dpy, sync, attrib_list);
    return ret;
}
PFNEGLCLIENTSIGNALSYNCEXTPROC glad_debug_eglClientSignalSyncEXT = glad_debug_impl_eglClientSignalSyncEXT;
PFNEGLCLIENTWAITSYNCPROC glad_eglClientWaitSync = NULL;
static EGLint GLAD_API_PTR glad_debug_impl_eglClientWaitSync(EGLDisplay dpy, EGLSync sync, EGLint flags, EGLTime timeout) {
    EGLint ret;
    _pre_call_egl_callback("eglClientWaitSync", (GLADapiproc) glad_eglClientWaitSync, 4, dpy, sync, flags, timeout);
    ret = glad_eglClientWaitSync(dpy, sync, flags, timeout);
    _post_call_egl_callback((void*) &ret, "eglClientWaitSync", (GLADapiproc) glad_eglClientWaitSync, 4, dpy, sync, flags, timeout);
    return ret;
}
PFNEGLCLIENTWAITSYNCPROC glad_debug_eglClientWaitSync = glad_debug_impl_eglClientWaitSync;
PFNEGLCLIENTWAITSYNCKHRPROC glad_eglClientWaitSyncKHR = NULL;
static EGLint GLAD_API_PTR glad_debug_impl_eglClientWaitSyncKHR(EGLDisplay dpy, EGLSyncKHR sync, EGLint flags, EGLTimeKHR timeout) {
    EGLint ret;
    _pre_call_egl_callback("eglClientWaitSyncKHR", (GLADapiproc) glad_eglClientWaitSyncKHR, 4, dpy, sync, flags, timeout);
    ret = glad_eglClientWaitSyncKHR(dpy, sync, flags, timeout);
    _post_call_egl_callback((void*) &ret, "eglClientWaitSyncKHR", (GLADapiproc) glad_eglClientWaitSyncKHR, 4, dpy, sync, flags, timeout);
    return ret;
}
PFNEGLCLIENTWAITSYNCKHRPROC glad_debug_eglClientWaitSyncKHR = glad_debug_impl_eglClientWaitSyncKHR;
PFNEGLCLIENTWAITSYNCNVPROC glad_eglClientWaitSyncNV = NULL;
static EGLint GLAD_API_PTR glad_debug_impl_eglClientWaitSyncNV(EGLSyncNV sync, EGLint flags, EGLTimeNV timeout) {
    EGLint ret;
    _pre_call_egl_callback("eglClientWaitSyncNV", (GLADapiproc) glad_eglClientWaitSyncNV, 3, sync, flags, timeout);
    ret = glad_eglClientWaitSyncNV(sync, flags, timeout);
    _post_call_egl_callback((void*) &ret, "eglClientWaitSyncNV", (GLADapiproc) glad_eglClientWaitSyncNV, 3, sync, flags, timeout);
    return ret;
}
PFNEGLCLIENTWAITSYNCNVPROC glad_debug_eglClientWaitSyncNV = glad_debug_impl_eglClientWaitSyncNV;
PFNEGLCOMPOSITORBINDTEXWINDOWEXTPROC glad_eglCompositorBindTexWindowEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglCompositorBindTexWindowEXT(EGLint external_win_id) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglCompositorBindTexWindowEXT", (GLADapiproc) glad_eglCompositorBindTexWindowEXT, 1, external_win_id);
    ret = glad_eglCompositorBindTexWindowEXT(external_win_id);
    _post_call_egl_callback((void*) &ret, "eglCompositorBindTexWindowEXT", (GLADapiproc) glad_eglCompositorBindTexWindowEXT, 1, external_win_id);
    return ret;
}
PFNEGLCOMPOSITORBINDTEXWINDOWEXTPROC glad_debug_eglCompositorBindTexWindowEXT = glad_debug_impl_eglCompositorBindTexWindowEXT;
PFNEGLCOMPOSITORSETCONTEXTATTRIBUTESEXTPROC glad_eglCompositorSetContextAttributesEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglCompositorSetContextAttributesEXT(EGLint external_ref_id, const EGLint * context_attributes, EGLint num_entries) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglCompositorSetContextAttributesEXT", (GLADapiproc) glad_eglCompositorSetContextAttributesEXT, 3, external_ref_id, context_attributes, num_entries);
    ret = glad_eglCompositorSetContextAttributesEXT(external_ref_id, context_attributes, num_entries);
    _post_call_egl_callback((void*) &ret, "eglCompositorSetContextAttributesEXT", (GLADapiproc) glad_eglCompositorSetContextAttributesEXT, 3, external_ref_id, context_attributes, num_entries);
    return ret;
}
PFNEGLCOMPOSITORSETCONTEXTATTRIBUTESEXTPROC glad_debug_eglCompositorSetContextAttributesEXT = glad_debug_impl_eglCompositorSetContextAttributesEXT;
PFNEGLCOMPOSITORSETCONTEXTLISTEXTPROC glad_eglCompositorSetContextListEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglCompositorSetContextListEXT(const EGLint * external_ref_ids, EGLint num_entries) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglCompositorSetContextListEXT", (GLADapiproc) glad_eglCompositorSetContextListEXT, 2, external_ref_ids, num_entries);
    ret = glad_eglCompositorSetContextListEXT(external_ref_ids, num_entries);
    _post_call_egl_callback((void*) &ret, "eglCompositorSetContextListEXT", (GLADapiproc) glad_eglCompositorSetContextListEXT, 2, external_ref_ids, num_entries);
    return ret;
}
PFNEGLCOMPOSITORSETCONTEXTLISTEXTPROC glad_debug_eglCompositorSetContextListEXT = glad_debug_impl_eglCompositorSetContextListEXT;
PFNEGLCOMPOSITORSETSIZEEXTPROC glad_eglCompositorSetSizeEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglCompositorSetSizeEXT(EGLint external_win_id, EGLint width, EGLint height) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglCompositorSetSizeEXT", (GLADapiproc) glad_eglCompositorSetSizeEXT, 3, external_win_id, width, height);
    ret = glad_eglCompositorSetSizeEXT(external_win_id, width, height);
    _post_call_egl_callback((void*) &ret, "eglCompositorSetSizeEXT", (GLADapiproc) glad_eglCompositorSetSizeEXT, 3, external_win_id, width, height);
    return ret;
}
PFNEGLCOMPOSITORSETSIZEEXTPROC glad_debug_eglCompositorSetSizeEXT = glad_debug_impl_eglCompositorSetSizeEXT;
PFNEGLCOMPOSITORSETWINDOWATTRIBUTESEXTPROC glad_eglCompositorSetWindowAttributesEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglCompositorSetWindowAttributesEXT(EGLint external_win_id, const EGLint * window_attributes, EGLint num_entries) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglCompositorSetWindowAttributesEXT", (GLADapiproc) glad_eglCompositorSetWindowAttributesEXT, 3, external_win_id, window_attributes, num_entries);
    ret = glad_eglCompositorSetWindowAttributesEXT(external_win_id, window_attributes, num_entries);
    _post_call_egl_callback((void*) &ret, "eglCompositorSetWindowAttributesEXT", (GLADapiproc) glad_eglCompositorSetWindowAttributesEXT, 3, external_win_id, window_attributes, num_entries);
    return ret;
}
PFNEGLCOMPOSITORSETWINDOWATTRIBUTESEXTPROC glad_debug_eglCompositorSetWindowAttributesEXT = glad_debug_impl_eglCompositorSetWindowAttributesEXT;
PFNEGLCOMPOSITORSETWINDOWLISTEXTPROC glad_eglCompositorSetWindowListEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglCompositorSetWindowListEXT(EGLint external_ref_id, const EGLint * external_win_ids, EGLint num_entries) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglCompositorSetWindowListEXT", (GLADapiproc) glad_eglCompositorSetWindowListEXT, 3, external_ref_id, external_win_ids, num_entries);
    ret = glad_eglCompositorSetWindowListEXT(external_ref_id, external_win_ids, num_entries);
    _post_call_egl_callback((void*) &ret, "eglCompositorSetWindowListEXT", (GLADapiproc) glad_eglCompositorSetWindowListEXT, 3, external_ref_id, external_win_ids, num_entries);
    return ret;
}
PFNEGLCOMPOSITORSETWINDOWLISTEXTPROC glad_debug_eglCompositorSetWindowListEXT = glad_debug_impl_eglCompositorSetWindowListEXT;
PFNEGLCOMPOSITORSWAPPOLICYEXTPROC glad_eglCompositorSwapPolicyEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglCompositorSwapPolicyEXT(EGLint external_win_id, EGLint policy) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglCompositorSwapPolicyEXT", (GLADapiproc) glad_eglCompositorSwapPolicyEXT, 2, external_win_id, policy);
    ret = glad_eglCompositorSwapPolicyEXT(external_win_id, policy);
    _post_call_egl_callback((void*) &ret, "eglCompositorSwapPolicyEXT", (GLADapiproc) glad_eglCompositorSwapPolicyEXT, 2, external_win_id, policy);
    return ret;
}
PFNEGLCOMPOSITORSWAPPOLICYEXTPROC glad_debug_eglCompositorSwapPolicyEXT = glad_debug_impl_eglCompositorSwapPolicyEXT;
PFNEGLCOPYBUFFERSPROC glad_eglCopyBuffers = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglCopyBuffers(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglCopyBuffers", (GLADapiproc) glad_eglCopyBuffers, 3, dpy, surface, target);
    ret = glad_eglCopyBuffers(dpy, surface, target);
    _post_call_egl_callback((void*) &ret, "eglCopyBuffers", (GLADapiproc) glad_eglCopyBuffers, 3, dpy, surface, target);
    return ret;
}
PFNEGLCOPYBUFFERSPROC glad_debug_eglCopyBuffers = glad_debug_impl_eglCopyBuffers;
PFNEGLCREATECONTEXTPROC glad_eglCreateContext = NULL;
static EGLContext GLAD_API_PTR glad_debug_impl_eglCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint * attrib_list) {
    EGLContext ret;
    _pre_call_egl_callback("eglCreateContext", (GLADapiproc) glad_eglCreateContext, 4, dpy, config, share_context, attrib_list);
    ret = glad_eglCreateContext(dpy, config, share_context, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglCreateContext", (GLADapiproc) glad_eglCreateContext, 4, dpy, config, share_context, attrib_list);
    return ret;
}
PFNEGLCREATECONTEXTPROC glad_debug_eglCreateContext = glad_debug_impl_eglCreateContext;
PFNEGLCREATEDRMIMAGEMESAPROC glad_eglCreateDRMImageMESA = NULL;
static EGLImageKHR GLAD_API_PTR glad_debug_impl_eglCreateDRMImageMESA(EGLDisplay dpy, const EGLint * attrib_list) {
    EGLImageKHR ret;
    _pre_call_egl_callback("eglCreateDRMImageMESA", (GLADapiproc) glad_eglCreateDRMImageMESA, 2, dpy, attrib_list);
    ret = glad_eglCreateDRMImageMESA(dpy, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglCreateDRMImageMESA", (GLADapiproc) glad_eglCreateDRMImageMESA, 2, dpy, attrib_list);
    return ret;
}
PFNEGLCREATEDRMIMAGEMESAPROC glad_debug_eglCreateDRMImageMESA = glad_debug_impl_eglCreateDRMImageMESA;
PFNEGLCREATEFENCESYNCNVPROC glad_eglCreateFenceSyncNV = NULL;
static EGLSyncNV GLAD_API_PTR glad_debug_impl_eglCreateFenceSyncNV(EGLDisplay dpy, EGLenum condition, const EGLint * attrib_list) {
    EGLSyncNV ret;
    _pre_call_egl_callback("eglCreateFenceSyncNV", (GLADapiproc) glad_eglCreateFenceSyncNV, 3, dpy, condition, attrib_list);
    ret = glad_eglCreateFenceSyncNV(dpy, condition, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglCreateFenceSyncNV", (GLADapiproc) glad_eglCreateFenceSyncNV, 3, dpy, condition, attrib_list);
    return ret;
}
PFNEGLCREATEFENCESYNCNVPROC glad_debug_eglCreateFenceSyncNV = glad_debug_impl_eglCreateFenceSyncNV;
PFNEGLCREATEIMAGEPROC glad_eglCreateImage = NULL;
static EGLImage GLAD_API_PTR glad_debug_impl_eglCreateImage(EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLAttrib * attrib_list) {
    EGLImage ret;
    _pre_call_egl_callback("eglCreateImage", (GLADapiproc) glad_eglCreateImage, 5, dpy, ctx, target, buffer, attrib_list);
    ret = glad_eglCreateImage(dpy, ctx, target, buffer, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglCreateImage", (GLADapiproc) glad_eglCreateImage, 5, dpy, ctx, target, buffer, attrib_list);
    return ret;
}
PFNEGLCREATEIMAGEPROC glad_debug_eglCreateImage = glad_debug_impl_eglCreateImage;
PFNEGLCREATEIMAGEKHRPROC glad_eglCreateImageKHR = NULL;
static EGLImageKHR GLAD_API_PTR glad_debug_impl_eglCreateImageKHR(EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLint * attrib_list) {
    EGLImageKHR ret;
    _pre_call_egl_callback("eglCreateImageKHR", (GLADapiproc) glad_eglCreateImageKHR, 5, dpy, ctx, target, buffer, attrib_list);
    ret = glad_eglCreateImageKHR(dpy, ctx, target, buffer, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglCreateImageKHR", (GLADapiproc) glad_eglCreateImageKHR, 5, dpy, ctx, target, buffer, attrib_list);
    return ret;
}
PFNEGLCREATEIMAGEKHRPROC glad_debug_eglCreateImageKHR = glad_debug_impl_eglCreateImageKHR;
PFNEGLCREATENATIVECLIENTBUFFERANDROIDPROC glad_eglCreateNativeClientBufferANDROID = NULL;
static EGLClientBuffer GLAD_API_PTR glad_debug_impl_eglCreateNativeClientBufferANDROID(const EGLint * attrib_list) {
    EGLClientBuffer ret;
    _pre_call_egl_callback("eglCreateNativeClientBufferANDROID", (GLADapiproc) glad_eglCreateNativeClientBufferANDROID, 1, attrib_list);
    ret = glad_eglCreateNativeClientBufferANDROID(attrib_list);
    _post_call_egl_callback((void*) &ret, "eglCreateNativeClientBufferANDROID", (GLADapiproc) glad_eglCreateNativeClientBufferANDROID, 1, attrib_list);
    return ret;
}
PFNEGLCREATENATIVECLIENTBUFFERANDROIDPROC glad_debug_eglCreateNativeClientBufferANDROID = glad_debug_impl_eglCreateNativeClientBufferANDROID;
PFNEGLCREATEPBUFFERFROMCLIENTBUFFERPROC glad_eglCreatePbufferFromClientBuffer = NULL;
static EGLSurface GLAD_API_PTR glad_debug_impl_eglCreatePbufferFromClientBuffer(EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint * attrib_list) {
    EGLSurface ret;
    _pre_call_egl_callback("eglCreatePbufferFromClientBuffer", (GLADapiproc) glad_eglCreatePbufferFromClientBuffer, 5, dpy, buftype, buffer, config, attrib_list);
    ret = glad_eglCreatePbufferFromClientBuffer(dpy, buftype, buffer, config, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglCreatePbufferFromClientBuffer", (GLADapiproc) glad_eglCreatePbufferFromClientBuffer, 5, dpy, buftype, buffer, config, attrib_list);
    return ret;
}
PFNEGLCREATEPBUFFERFROMCLIENTBUFFERPROC glad_debug_eglCreatePbufferFromClientBuffer = glad_debug_impl_eglCreatePbufferFromClientBuffer;
PFNEGLCREATEPBUFFERSURFACEPROC glad_eglCreatePbufferSurface = NULL;
static EGLSurface GLAD_API_PTR glad_debug_impl_eglCreatePbufferSurface(EGLDisplay dpy, EGLConfig config, const EGLint * attrib_list) {
    EGLSurface ret;
    _pre_call_egl_callback("eglCreatePbufferSurface", (GLADapiproc) glad_eglCreatePbufferSurface, 3, dpy, config, attrib_list);
    ret = glad_eglCreatePbufferSurface(dpy, config, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglCreatePbufferSurface", (GLADapiproc) glad_eglCreatePbufferSurface, 3, dpy, config, attrib_list);
    return ret;
}
PFNEGLCREATEPBUFFERSURFACEPROC glad_debug_eglCreatePbufferSurface = glad_debug_impl_eglCreatePbufferSurface;
PFNEGLCREATEPIXMAPSURFACEPROC glad_eglCreatePixmapSurface = NULL;
static EGLSurface GLAD_API_PTR glad_debug_impl_eglCreatePixmapSurface(EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint * attrib_list) {
    EGLSurface ret;
    _pre_call_egl_callback("eglCreatePixmapSurface", (GLADapiproc) glad_eglCreatePixmapSurface, 4, dpy, config, pixmap, attrib_list);
    ret = glad_eglCreatePixmapSurface(dpy, config, pixmap, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglCreatePixmapSurface", (GLADapiproc) glad_eglCreatePixmapSurface, 4, dpy, config, pixmap, attrib_list);
    return ret;
}
PFNEGLCREATEPIXMAPSURFACEPROC glad_debug_eglCreatePixmapSurface = glad_debug_impl_eglCreatePixmapSurface;
PFNEGLCREATEPIXMAPSURFACEHIPROC glad_eglCreatePixmapSurfaceHI = NULL;
static EGLSurface GLAD_API_PTR glad_debug_impl_eglCreatePixmapSurfaceHI(EGLDisplay dpy, EGLConfig config, struct EGLClientPixmapHI * pixmap) {
    EGLSurface ret;
    _pre_call_egl_callback("eglCreatePixmapSurfaceHI", (GLADapiproc) glad_eglCreatePixmapSurfaceHI, 3, dpy, config, pixmap);
    ret = glad_eglCreatePixmapSurfaceHI(dpy, config, pixmap);
    _post_call_egl_callback((void*) &ret, "eglCreatePixmapSurfaceHI", (GLADapiproc) glad_eglCreatePixmapSurfaceHI, 3, dpy, config, pixmap);
    return ret;
}
PFNEGLCREATEPIXMAPSURFACEHIPROC glad_debug_eglCreatePixmapSurfaceHI = glad_debug_impl_eglCreatePixmapSurfaceHI;
PFNEGLCREATEPLATFORMPIXMAPSURFACEPROC glad_eglCreatePlatformPixmapSurface = NULL;
static EGLSurface GLAD_API_PTR glad_debug_impl_eglCreatePlatformPixmapSurface(EGLDisplay dpy, EGLConfig config, void * native_pixmap, const EGLAttrib * attrib_list) {
    EGLSurface ret;
    _pre_call_egl_callback("eglCreatePlatformPixmapSurface", (GLADapiproc) glad_eglCreatePlatformPixmapSurface, 4, dpy, config, native_pixmap, attrib_list);
    ret = glad_eglCreatePlatformPixmapSurface(dpy, config, native_pixmap, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglCreatePlatformPixmapSurface", (GLADapiproc) glad_eglCreatePlatformPixmapSurface, 4, dpy, config, native_pixmap, attrib_list);
    return ret;
}
PFNEGLCREATEPLATFORMPIXMAPSURFACEPROC glad_debug_eglCreatePlatformPixmapSurface = glad_debug_impl_eglCreatePlatformPixmapSurface;
PFNEGLCREATEPLATFORMPIXMAPSURFACEEXTPROC glad_eglCreatePlatformPixmapSurfaceEXT = NULL;
static EGLSurface GLAD_API_PTR glad_debug_impl_eglCreatePlatformPixmapSurfaceEXT(EGLDisplay dpy, EGLConfig config, void * native_pixmap, const EGLint * attrib_list) {
    EGLSurface ret;
    _pre_call_egl_callback("eglCreatePlatformPixmapSurfaceEXT", (GLADapiproc) glad_eglCreatePlatformPixmapSurfaceEXT, 4, dpy, config, native_pixmap, attrib_list);
    ret = glad_eglCreatePlatformPixmapSurfaceEXT(dpy, config, native_pixmap, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglCreatePlatformPixmapSurfaceEXT", (GLADapiproc) glad_eglCreatePlatformPixmapSurfaceEXT, 4, dpy, config, native_pixmap, attrib_list);
    return ret;
}
PFNEGLCREATEPLATFORMPIXMAPSURFACEEXTPROC glad_debug_eglCreatePlatformPixmapSurfaceEXT = glad_debug_impl_eglCreatePlatformPixmapSurfaceEXT;
PFNEGLCREATEPLATFORMWINDOWSURFACEPROC glad_eglCreatePlatformWindowSurface = NULL;
static EGLSurface GLAD_API_PTR glad_debug_impl_eglCreatePlatformWindowSurface(EGLDisplay dpy, EGLConfig config, void * native_window, const EGLAttrib * attrib_list) {
    EGLSurface ret;
    _pre_call_egl_callback("eglCreatePlatformWindowSurface", (GLADapiproc) glad_eglCreatePlatformWindowSurface, 4, dpy, config, native_window, attrib_list);
    ret = glad_eglCreatePlatformWindowSurface(dpy, config, native_window, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglCreatePlatformWindowSurface", (GLADapiproc) glad_eglCreatePlatformWindowSurface, 4, dpy, config, native_window, attrib_list);
    return ret;
}
PFNEGLCREATEPLATFORMWINDOWSURFACEPROC glad_debug_eglCreatePlatformWindowSurface = glad_debug_impl_eglCreatePlatformWindowSurface;
PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC glad_eglCreatePlatformWindowSurfaceEXT = NULL;
static EGLSurface GLAD_API_PTR glad_debug_impl_eglCreatePlatformWindowSurfaceEXT(EGLDisplay dpy, EGLConfig config, void * native_window, const EGLint * attrib_list) {
    EGLSurface ret;
    _pre_call_egl_callback("eglCreatePlatformWindowSurfaceEXT", (GLADapiproc) glad_eglCreatePlatformWindowSurfaceEXT, 4, dpy, config, native_window, attrib_list);
    ret = glad_eglCreatePlatformWindowSurfaceEXT(dpy, config, native_window, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglCreatePlatformWindowSurfaceEXT", (GLADapiproc) glad_eglCreatePlatformWindowSurfaceEXT, 4, dpy, config, native_window, attrib_list);
    return ret;
}
PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC glad_debug_eglCreatePlatformWindowSurfaceEXT = glad_debug_impl_eglCreatePlatformWindowSurfaceEXT;
PFNEGLCREATESTREAMATTRIBKHRPROC glad_eglCreateStreamAttribKHR = NULL;
static EGLStreamKHR GLAD_API_PTR glad_debug_impl_eglCreateStreamAttribKHR(EGLDisplay dpy, const EGLAttrib * attrib_list) {
    EGLStreamKHR ret;
    _pre_call_egl_callback("eglCreateStreamAttribKHR", (GLADapiproc) glad_eglCreateStreamAttribKHR, 2, dpy, attrib_list);
    ret = glad_eglCreateStreamAttribKHR(dpy, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglCreateStreamAttribKHR", (GLADapiproc) glad_eglCreateStreamAttribKHR, 2, dpy, attrib_list);
    return ret;
}
PFNEGLCREATESTREAMATTRIBKHRPROC glad_debug_eglCreateStreamAttribKHR = glad_debug_impl_eglCreateStreamAttribKHR;
PFNEGLCREATESTREAMFROMFILEDESCRIPTORKHRPROC glad_eglCreateStreamFromFileDescriptorKHR = NULL;
static EGLStreamKHR GLAD_API_PTR glad_debug_impl_eglCreateStreamFromFileDescriptorKHR(EGLDisplay dpy, EGLNativeFileDescriptorKHR file_descriptor) {
    EGLStreamKHR ret;
    _pre_call_egl_callback("eglCreateStreamFromFileDescriptorKHR", (GLADapiproc) glad_eglCreateStreamFromFileDescriptorKHR, 2, dpy, file_descriptor);
    ret = glad_eglCreateStreamFromFileDescriptorKHR(dpy, file_descriptor);
    _post_call_egl_callback((void*) &ret, "eglCreateStreamFromFileDescriptorKHR", (GLADapiproc) glad_eglCreateStreamFromFileDescriptorKHR, 2, dpy, file_descriptor);
    return ret;
}
PFNEGLCREATESTREAMFROMFILEDESCRIPTORKHRPROC glad_debug_eglCreateStreamFromFileDescriptorKHR = glad_debug_impl_eglCreateStreamFromFileDescriptorKHR;
PFNEGLCREATESTREAMKHRPROC glad_eglCreateStreamKHR = NULL;
static EGLStreamKHR GLAD_API_PTR glad_debug_impl_eglCreateStreamKHR(EGLDisplay dpy, const EGLint * attrib_list) {
    EGLStreamKHR ret;
    _pre_call_egl_callback("eglCreateStreamKHR", (GLADapiproc) glad_eglCreateStreamKHR, 2, dpy, attrib_list);
    ret = glad_eglCreateStreamKHR(dpy, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglCreateStreamKHR", (GLADapiproc) glad_eglCreateStreamKHR, 2, dpy, attrib_list);
    return ret;
}
PFNEGLCREATESTREAMKHRPROC glad_debug_eglCreateStreamKHR = glad_debug_impl_eglCreateStreamKHR;
PFNEGLCREATESTREAMPRODUCERSURFACEKHRPROC glad_eglCreateStreamProducerSurfaceKHR = NULL;
static EGLSurface GLAD_API_PTR glad_debug_impl_eglCreateStreamProducerSurfaceKHR(EGLDisplay dpy, EGLConfig config, EGLStreamKHR stream, const EGLint * attrib_list) {
    EGLSurface ret;
    _pre_call_egl_callback("eglCreateStreamProducerSurfaceKHR", (GLADapiproc) glad_eglCreateStreamProducerSurfaceKHR, 4, dpy, config, stream, attrib_list);
    ret = glad_eglCreateStreamProducerSurfaceKHR(dpy, config, stream, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglCreateStreamProducerSurfaceKHR", (GLADapiproc) glad_eglCreateStreamProducerSurfaceKHR, 4, dpy, config, stream, attrib_list);
    return ret;
}
PFNEGLCREATESTREAMPRODUCERSURFACEKHRPROC glad_debug_eglCreateStreamProducerSurfaceKHR = glad_debug_impl_eglCreateStreamProducerSurfaceKHR;
PFNEGLCREATESTREAMSYNCNVPROC glad_eglCreateStreamSyncNV = NULL;
static EGLSyncKHR GLAD_API_PTR glad_debug_impl_eglCreateStreamSyncNV(EGLDisplay dpy, EGLStreamKHR stream, EGLenum type, const EGLint * attrib_list) {
    EGLSyncKHR ret;
    _pre_call_egl_callback("eglCreateStreamSyncNV", (GLADapiproc) glad_eglCreateStreamSyncNV, 4, dpy, stream, type, attrib_list);
    ret = glad_eglCreateStreamSyncNV(dpy, stream, type, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglCreateStreamSyncNV", (GLADapiproc) glad_eglCreateStreamSyncNV, 4, dpy, stream, type, attrib_list);
    return ret;
}
PFNEGLCREATESTREAMSYNCNVPROC glad_debug_eglCreateStreamSyncNV = glad_debug_impl_eglCreateStreamSyncNV;
PFNEGLCREATESYNCPROC glad_eglCreateSync = NULL;
static EGLSync GLAD_API_PTR glad_debug_impl_eglCreateSync(EGLDisplay dpy, EGLenum type, const EGLAttrib * attrib_list) {
    EGLSync ret;
    _pre_call_egl_callback("eglCreateSync", (GLADapiproc) glad_eglCreateSync, 3, dpy, type, attrib_list);
    ret = glad_eglCreateSync(dpy, type, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglCreateSync", (GLADapiproc) glad_eglCreateSync, 3, dpy, type, attrib_list);
    return ret;
}
PFNEGLCREATESYNCPROC glad_debug_eglCreateSync = glad_debug_impl_eglCreateSync;
PFNEGLCREATESYNC64KHRPROC glad_eglCreateSync64KHR = NULL;
static EGLSyncKHR GLAD_API_PTR glad_debug_impl_eglCreateSync64KHR(EGLDisplay dpy, EGLenum type, const EGLAttribKHR * attrib_list) {
    EGLSyncKHR ret;
    _pre_call_egl_callback("eglCreateSync64KHR", (GLADapiproc) glad_eglCreateSync64KHR, 3, dpy, type, attrib_list);
    ret = glad_eglCreateSync64KHR(dpy, type, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglCreateSync64KHR", (GLADapiproc) glad_eglCreateSync64KHR, 3, dpy, type, attrib_list);
    return ret;
}
PFNEGLCREATESYNC64KHRPROC glad_debug_eglCreateSync64KHR = glad_debug_impl_eglCreateSync64KHR;
PFNEGLCREATESYNCKHRPROC glad_eglCreateSyncKHR = NULL;
static EGLSyncKHR GLAD_API_PTR glad_debug_impl_eglCreateSyncKHR(EGLDisplay dpy, EGLenum type, const EGLint * attrib_list) {
    EGLSyncKHR ret;
    _pre_call_egl_callback("eglCreateSyncKHR", (GLADapiproc) glad_eglCreateSyncKHR, 3, dpy, type, attrib_list);
    ret = glad_eglCreateSyncKHR(dpy, type, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglCreateSyncKHR", (GLADapiproc) glad_eglCreateSyncKHR, 3, dpy, type, attrib_list);
    return ret;
}
PFNEGLCREATESYNCKHRPROC glad_debug_eglCreateSyncKHR = glad_debug_impl_eglCreateSyncKHR;
PFNEGLCREATEWAYLANDBUFFERFROMIMAGEWLPROC glad_eglCreateWaylandBufferFromImageWL = NULL;
static struct wl_buffer * GLAD_API_PTR glad_debug_impl_eglCreateWaylandBufferFromImageWL(EGLDisplay dpy, EGLImageKHR image) {
    struct wl_buffer * ret;
    _pre_call_egl_callback("eglCreateWaylandBufferFromImageWL", (GLADapiproc) glad_eglCreateWaylandBufferFromImageWL, 2, dpy, image);
    ret = glad_eglCreateWaylandBufferFromImageWL(dpy, image);
    _post_call_egl_callback((void*) &ret, "eglCreateWaylandBufferFromImageWL", (GLADapiproc) glad_eglCreateWaylandBufferFromImageWL, 2, dpy, image);
    return ret;
}
PFNEGLCREATEWAYLANDBUFFERFROMIMAGEWLPROC glad_debug_eglCreateWaylandBufferFromImageWL = glad_debug_impl_eglCreateWaylandBufferFromImageWL;
PFNEGLCREATEWINDOWSURFACEPROC glad_eglCreateWindowSurface = NULL;
static EGLSurface GLAD_API_PTR glad_debug_impl_eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint * attrib_list) {
    EGLSurface ret;
    _pre_call_egl_callback("eglCreateWindowSurface", (GLADapiproc) glad_eglCreateWindowSurface, 4, dpy, config, win, attrib_list);
    ret = glad_eglCreateWindowSurface(dpy, config, win, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglCreateWindowSurface", (GLADapiproc) glad_eglCreateWindowSurface, 4, dpy, config, win, attrib_list);
    return ret;
}
PFNEGLCREATEWINDOWSURFACEPROC glad_debug_eglCreateWindowSurface = glad_debug_impl_eglCreateWindowSurface;
PFNEGLDEBUGMESSAGECONTROLKHRPROC glad_eglDebugMessageControlKHR = NULL;
static EGLint GLAD_API_PTR glad_debug_impl_eglDebugMessageControlKHR(EGLDEBUGPROCKHR callback, const EGLAttrib * attrib_list) {
    EGLint ret;
    _pre_call_egl_callback("eglDebugMessageControlKHR", (GLADapiproc) glad_eglDebugMessageControlKHR, 2, callback, attrib_list);
    ret = glad_eglDebugMessageControlKHR(callback, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglDebugMessageControlKHR", (GLADapiproc) glad_eglDebugMessageControlKHR, 2, callback, attrib_list);
    return ret;
}
PFNEGLDEBUGMESSAGECONTROLKHRPROC glad_debug_eglDebugMessageControlKHR = glad_debug_impl_eglDebugMessageControlKHR;
PFNEGLDESTROYCONTEXTPROC glad_eglDestroyContext = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglDestroyContext(EGLDisplay dpy, EGLContext ctx) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglDestroyContext", (GLADapiproc) glad_eglDestroyContext, 2, dpy, ctx);
    ret = glad_eglDestroyContext(dpy, ctx);
    _post_call_egl_callback((void*) &ret, "eglDestroyContext", (GLADapiproc) glad_eglDestroyContext, 2, dpy, ctx);
    return ret;
}
PFNEGLDESTROYCONTEXTPROC glad_debug_eglDestroyContext = glad_debug_impl_eglDestroyContext;
PFNEGLDESTROYDISPLAYEXTPROC glad_eglDestroyDisplayEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglDestroyDisplayEXT(EGLDisplay dpy) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglDestroyDisplayEXT", (GLADapiproc) glad_eglDestroyDisplayEXT, 1, dpy);
    ret = glad_eglDestroyDisplayEXT(dpy);
    _post_call_egl_callback((void*) &ret, "eglDestroyDisplayEXT", (GLADapiproc) glad_eglDestroyDisplayEXT, 1, dpy);
    return ret;
}
PFNEGLDESTROYDISPLAYEXTPROC glad_debug_eglDestroyDisplayEXT = glad_debug_impl_eglDestroyDisplayEXT;
PFNEGLDESTROYIMAGEPROC glad_eglDestroyImage = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglDestroyImage(EGLDisplay dpy, EGLImage image) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglDestroyImage", (GLADapiproc) glad_eglDestroyImage, 2, dpy, image);
    ret = glad_eglDestroyImage(dpy, image);
    _post_call_egl_callback((void*) &ret, "eglDestroyImage", (GLADapiproc) glad_eglDestroyImage, 2, dpy, image);
    return ret;
}
PFNEGLDESTROYIMAGEPROC glad_debug_eglDestroyImage = glad_debug_impl_eglDestroyImage;
PFNEGLDESTROYIMAGEKHRPROC glad_eglDestroyImageKHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglDestroyImageKHR(EGLDisplay dpy, EGLImageKHR image) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglDestroyImageKHR", (GLADapiproc) glad_eglDestroyImageKHR, 2, dpy, image);
    ret = glad_eglDestroyImageKHR(dpy, image);
    _post_call_egl_callback((void*) &ret, "eglDestroyImageKHR", (GLADapiproc) glad_eglDestroyImageKHR, 2, dpy, image);
    return ret;
}
PFNEGLDESTROYIMAGEKHRPROC glad_debug_eglDestroyImageKHR = glad_debug_impl_eglDestroyImageKHR;
PFNEGLDESTROYSTREAMKHRPROC glad_eglDestroyStreamKHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglDestroyStreamKHR(EGLDisplay dpy, EGLStreamKHR stream) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglDestroyStreamKHR", (GLADapiproc) glad_eglDestroyStreamKHR, 2, dpy, stream);
    ret = glad_eglDestroyStreamKHR(dpy, stream);
    _post_call_egl_callback((void*) &ret, "eglDestroyStreamKHR", (GLADapiproc) glad_eglDestroyStreamKHR, 2, dpy, stream);
    return ret;
}
PFNEGLDESTROYSTREAMKHRPROC glad_debug_eglDestroyStreamKHR = glad_debug_impl_eglDestroyStreamKHR;
PFNEGLDESTROYSURFACEPROC glad_eglDestroySurface = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglDestroySurface(EGLDisplay dpy, EGLSurface surface) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglDestroySurface", (GLADapiproc) glad_eglDestroySurface, 2, dpy, surface);
    ret = glad_eglDestroySurface(dpy, surface);
    _post_call_egl_callback((void*) &ret, "eglDestroySurface", (GLADapiproc) glad_eglDestroySurface, 2, dpy, surface);
    return ret;
}
PFNEGLDESTROYSURFACEPROC glad_debug_eglDestroySurface = glad_debug_impl_eglDestroySurface;
PFNEGLDESTROYSYNCPROC glad_eglDestroySync = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglDestroySync(EGLDisplay dpy, EGLSync sync) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglDestroySync", (GLADapiproc) glad_eglDestroySync, 2, dpy, sync);
    ret = glad_eglDestroySync(dpy, sync);
    _post_call_egl_callback((void*) &ret, "eglDestroySync", (GLADapiproc) glad_eglDestroySync, 2, dpy, sync);
    return ret;
}
PFNEGLDESTROYSYNCPROC glad_debug_eglDestroySync = glad_debug_impl_eglDestroySync;
PFNEGLDESTROYSYNCKHRPROC glad_eglDestroySyncKHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglDestroySyncKHR(EGLDisplay dpy, EGLSyncKHR sync) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglDestroySyncKHR", (GLADapiproc) glad_eglDestroySyncKHR, 2, dpy, sync);
    ret = glad_eglDestroySyncKHR(dpy, sync);
    _post_call_egl_callback((void*) &ret, "eglDestroySyncKHR", (GLADapiproc) glad_eglDestroySyncKHR, 2, dpy, sync);
    return ret;
}
PFNEGLDESTROYSYNCKHRPROC glad_debug_eglDestroySyncKHR = glad_debug_impl_eglDestroySyncKHR;
PFNEGLDESTROYSYNCNVPROC glad_eglDestroySyncNV = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglDestroySyncNV(EGLSyncNV sync) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglDestroySyncNV", (GLADapiproc) glad_eglDestroySyncNV, 1, sync);
    ret = glad_eglDestroySyncNV(sync);
    _post_call_egl_callback((void*) &ret, "eglDestroySyncNV", (GLADapiproc) glad_eglDestroySyncNV, 1, sync);
    return ret;
}
PFNEGLDESTROYSYNCNVPROC glad_debug_eglDestroySyncNV = glad_debug_impl_eglDestroySyncNV;
PFNEGLDUPNATIVEFENCEFDANDROIDPROC glad_eglDupNativeFenceFDANDROID = NULL;
static EGLint GLAD_API_PTR glad_debug_impl_eglDupNativeFenceFDANDROID(EGLDisplay dpy, EGLSyncKHR sync) {
    EGLint ret;
    _pre_call_egl_callback("eglDupNativeFenceFDANDROID", (GLADapiproc) glad_eglDupNativeFenceFDANDROID, 2, dpy, sync);
    ret = glad_eglDupNativeFenceFDANDROID(dpy, sync);
    _post_call_egl_callback((void*) &ret, "eglDupNativeFenceFDANDROID", (GLADapiproc) glad_eglDupNativeFenceFDANDROID, 2, dpy, sync);
    return ret;
}
PFNEGLDUPNATIVEFENCEFDANDROIDPROC glad_debug_eglDupNativeFenceFDANDROID = glad_debug_impl_eglDupNativeFenceFDANDROID;
PFNEGLEXPORTDMABUFIMAGEMESAPROC glad_eglExportDMABUFImageMESA = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglExportDMABUFImageMESA(EGLDisplay dpy, EGLImageKHR image, int * fds, EGLint * strides, EGLint * offsets) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglExportDMABUFImageMESA", (GLADapiproc) glad_eglExportDMABUFImageMESA, 5, dpy, image, fds, strides, offsets);
    ret = glad_eglExportDMABUFImageMESA(dpy, image, fds, strides, offsets);
    _post_call_egl_callback((void*) &ret, "eglExportDMABUFImageMESA", (GLADapiproc) glad_eglExportDMABUFImageMESA, 5, dpy, image, fds, strides, offsets);
    return ret;
}
PFNEGLEXPORTDMABUFIMAGEMESAPROC glad_debug_eglExportDMABUFImageMESA = glad_debug_impl_eglExportDMABUFImageMESA;
PFNEGLEXPORTDMABUFIMAGEQUERYMESAPROC glad_eglExportDMABUFImageQueryMESA = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglExportDMABUFImageQueryMESA(EGLDisplay dpy, EGLImageKHR image, int * fourcc, int * num_planes, EGLuint64KHR * modifiers) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglExportDMABUFImageQueryMESA", (GLADapiproc) glad_eglExportDMABUFImageQueryMESA, 5, dpy, image, fourcc, num_planes, modifiers);
    ret = glad_eglExportDMABUFImageQueryMESA(dpy, image, fourcc, num_planes, modifiers);
    _post_call_egl_callback((void*) &ret, "eglExportDMABUFImageQueryMESA", (GLADapiproc) glad_eglExportDMABUFImageQueryMESA, 5, dpy, image, fourcc, num_planes, modifiers);
    return ret;
}
PFNEGLEXPORTDMABUFIMAGEQUERYMESAPROC glad_debug_eglExportDMABUFImageQueryMESA = glad_debug_impl_eglExportDMABUFImageQueryMESA;
PFNEGLEXPORTDRMIMAGEMESAPROC glad_eglExportDRMImageMESA = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglExportDRMImageMESA(EGLDisplay dpy, EGLImageKHR image, EGLint * name, EGLint * handle, EGLint * stride) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglExportDRMImageMESA", (GLADapiproc) glad_eglExportDRMImageMESA, 5, dpy, image, name, handle, stride);
    ret = glad_eglExportDRMImageMESA(dpy, image, name, handle, stride);
    _post_call_egl_callback((void*) &ret, "eglExportDRMImageMESA", (GLADapiproc) glad_eglExportDRMImageMESA, 5, dpy, image, name, handle, stride);
    return ret;
}
PFNEGLEXPORTDRMIMAGEMESAPROC glad_debug_eglExportDRMImageMESA = glad_debug_impl_eglExportDRMImageMESA;
PFNEGLFENCENVPROC glad_eglFenceNV = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglFenceNV(EGLSyncNV sync) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglFenceNV", (GLADapiproc) glad_eglFenceNV, 1, sync);
    ret = glad_eglFenceNV(sync);
    _post_call_egl_callback((void*) &ret, "eglFenceNV", (GLADapiproc) glad_eglFenceNV, 1, sync);
    return ret;
}
PFNEGLFENCENVPROC glad_debug_eglFenceNV = glad_debug_impl_eglFenceNV;
PFNEGLGETCOMPOSITORTIMINGANDROIDPROC glad_eglGetCompositorTimingANDROID = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglGetCompositorTimingANDROID(EGLDisplay dpy, EGLSurface surface, EGLint numTimestamps, const EGLint * names, EGLnsecsANDROID * values) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglGetCompositorTimingANDROID", (GLADapiproc) glad_eglGetCompositorTimingANDROID, 5, dpy, surface, numTimestamps, names, values);
    ret = glad_eglGetCompositorTimingANDROID(dpy, surface, numTimestamps, names, values);
    _post_call_egl_callback((void*) &ret, "eglGetCompositorTimingANDROID", (GLADapiproc) glad_eglGetCompositorTimingANDROID, 5, dpy, surface, numTimestamps, names, values);
    return ret;
}
PFNEGLGETCOMPOSITORTIMINGANDROIDPROC glad_debug_eglGetCompositorTimingANDROID = glad_debug_impl_eglGetCompositorTimingANDROID;
PFNEGLGETCOMPOSITORTIMINGSUPPORTEDANDROIDPROC glad_eglGetCompositorTimingSupportedANDROID = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglGetCompositorTimingSupportedANDROID(EGLDisplay dpy, EGLSurface surface, EGLint name) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglGetCompositorTimingSupportedANDROID", (GLADapiproc) glad_eglGetCompositorTimingSupportedANDROID, 3, dpy, surface, name);
    ret = glad_eglGetCompositorTimingSupportedANDROID(dpy, surface, name);
    _post_call_egl_callback((void*) &ret, "eglGetCompositorTimingSupportedANDROID", (GLADapiproc) glad_eglGetCompositorTimingSupportedANDROID, 3, dpy, surface, name);
    return ret;
}
PFNEGLGETCOMPOSITORTIMINGSUPPORTEDANDROIDPROC glad_debug_eglGetCompositorTimingSupportedANDROID = glad_debug_impl_eglGetCompositorTimingSupportedANDROID;
PFNEGLGETCONFIGATTRIBPROC glad_eglGetConfigAttrib = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglGetConfigAttrib(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint * value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglGetConfigAttrib", (GLADapiproc) glad_eglGetConfigAttrib, 4, dpy, config, attribute, value);
    ret = glad_eglGetConfigAttrib(dpy, config, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglGetConfigAttrib", (GLADapiproc) glad_eglGetConfigAttrib, 4, dpy, config, attribute, value);
    return ret;
}
PFNEGLGETCONFIGATTRIBPROC glad_debug_eglGetConfigAttrib = glad_debug_impl_eglGetConfigAttrib;
PFNEGLGETCONFIGSPROC glad_eglGetConfigs = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglGetConfigs(EGLDisplay dpy, EGLConfig * configs, EGLint config_size, EGLint * num_config) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglGetConfigs", (GLADapiproc) glad_eglGetConfigs, 4, dpy, configs, config_size, num_config);
    ret = glad_eglGetConfigs(dpy, configs, config_size, num_config);
    _post_call_egl_callback((void*) &ret, "eglGetConfigs", (GLADapiproc) glad_eglGetConfigs, 4, dpy, configs, config_size, num_config);
    return ret;
}
PFNEGLGETCONFIGSPROC glad_debug_eglGetConfigs = glad_debug_impl_eglGetConfigs;
PFNEGLGETCURRENTCONTEXTPROC glad_eglGetCurrentContext = NULL;
static EGLContext GLAD_API_PTR glad_debug_impl_eglGetCurrentContext(void) {
    EGLContext ret;
    _pre_call_egl_callback("eglGetCurrentContext", (GLADapiproc) glad_eglGetCurrentContext, 0);
    ret = glad_eglGetCurrentContext();
    _post_call_egl_callback((void*) &ret, "eglGetCurrentContext", (GLADapiproc) glad_eglGetCurrentContext, 0);
    return ret;
}
PFNEGLGETCURRENTCONTEXTPROC glad_debug_eglGetCurrentContext = glad_debug_impl_eglGetCurrentContext;
PFNEGLGETCURRENTDISPLAYPROC glad_eglGetCurrentDisplay = NULL;
static EGLDisplay GLAD_API_PTR glad_debug_impl_eglGetCurrentDisplay(void) {
    EGLDisplay ret;
    _pre_call_egl_callback("eglGetCurrentDisplay", (GLADapiproc) glad_eglGetCurrentDisplay, 0);
    ret = glad_eglGetCurrentDisplay();
    _post_call_egl_callback((void*) &ret, "eglGetCurrentDisplay", (GLADapiproc) glad_eglGetCurrentDisplay, 0);
    return ret;
}
PFNEGLGETCURRENTDISPLAYPROC glad_debug_eglGetCurrentDisplay = glad_debug_impl_eglGetCurrentDisplay;
PFNEGLGETCURRENTSURFACEPROC glad_eglGetCurrentSurface = NULL;
static EGLSurface GLAD_API_PTR glad_debug_impl_eglGetCurrentSurface(EGLint readdraw) {
    EGLSurface ret;
    _pre_call_egl_callback("eglGetCurrentSurface", (GLADapiproc) glad_eglGetCurrentSurface, 1, readdraw);
    ret = glad_eglGetCurrentSurface(readdraw);
    _post_call_egl_callback((void*) &ret, "eglGetCurrentSurface", (GLADapiproc) glad_eglGetCurrentSurface, 1, readdraw);
    return ret;
}
PFNEGLGETCURRENTSURFACEPROC glad_debug_eglGetCurrentSurface = glad_debug_impl_eglGetCurrentSurface;
PFNEGLGETDISPLAYPROC glad_eglGetDisplay = NULL;
static EGLDisplay GLAD_API_PTR glad_debug_impl_eglGetDisplay(EGLNativeDisplayType display_id) {
    EGLDisplay ret;
    _pre_call_egl_callback("eglGetDisplay", (GLADapiproc) glad_eglGetDisplay, 1, display_id);
    ret = glad_eglGetDisplay(display_id);
    _post_call_egl_callback((void*) &ret, "eglGetDisplay", (GLADapiproc) glad_eglGetDisplay, 1, display_id);
    return ret;
}
PFNEGLGETDISPLAYPROC glad_debug_eglGetDisplay = glad_debug_impl_eglGetDisplay;
PFNEGLGETDISPLAYDRIVERCONFIGPROC glad_eglGetDisplayDriverConfig = NULL;
static char * GLAD_API_PTR glad_debug_impl_eglGetDisplayDriverConfig(EGLDisplay dpy) {
    char * ret;
    _pre_call_egl_callback("eglGetDisplayDriverConfig", (GLADapiproc) glad_eglGetDisplayDriverConfig, 1, dpy);
    ret = glad_eglGetDisplayDriverConfig(dpy);
    _post_call_egl_callback((void*) &ret, "eglGetDisplayDriverConfig", (GLADapiproc) glad_eglGetDisplayDriverConfig, 1, dpy);
    return ret;
}
PFNEGLGETDISPLAYDRIVERCONFIGPROC glad_debug_eglGetDisplayDriverConfig = glad_debug_impl_eglGetDisplayDriverConfig;
PFNEGLGETDISPLAYDRIVERNAMEPROC glad_eglGetDisplayDriverName = NULL;
static const char * GLAD_API_PTR glad_debug_impl_eglGetDisplayDriverName(EGLDisplay dpy) {
    const char * ret;
    _pre_call_egl_callback("eglGetDisplayDriverName", (GLADapiproc) glad_eglGetDisplayDriverName, 1, dpy);
    ret = glad_eglGetDisplayDriverName(dpy);
    _post_call_egl_callback((void*) &ret, "eglGetDisplayDriverName", (GLADapiproc) glad_eglGetDisplayDriverName, 1, dpy);
    return ret;
}
PFNEGLGETDISPLAYDRIVERNAMEPROC glad_debug_eglGetDisplayDriverName = glad_debug_impl_eglGetDisplayDriverName;
PFNEGLGETERRORPROC glad_eglGetError = NULL;
static EGLint GLAD_API_PTR glad_debug_impl_eglGetError(void) {
    EGLint ret;
    _pre_call_egl_callback("eglGetError", (GLADapiproc) glad_eglGetError, 0);
    ret = glad_eglGetError();
    _post_call_egl_callback((void*) &ret, "eglGetError", (GLADapiproc) glad_eglGetError, 0);
    return ret;
}
PFNEGLGETERRORPROC glad_debug_eglGetError = glad_debug_impl_eglGetError;
PFNEGLGETFRAMETIMESTAMPSUPPORTEDANDROIDPROC glad_eglGetFrameTimestampSupportedANDROID = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglGetFrameTimestampSupportedANDROID(EGLDisplay dpy, EGLSurface surface, EGLint timestamp) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglGetFrameTimestampSupportedANDROID", (GLADapiproc) glad_eglGetFrameTimestampSupportedANDROID, 3, dpy, surface, timestamp);
    ret = glad_eglGetFrameTimestampSupportedANDROID(dpy, surface, timestamp);
    _post_call_egl_callback((void*) &ret, "eglGetFrameTimestampSupportedANDROID", (GLADapiproc) glad_eglGetFrameTimestampSupportedANDROID, 3, dpy, surface, timestamp);
    return ret;
}
PFNEGLGETFRAMETIMESTAMPSUPPORTEDANDROIDPROC glad_debug_eglGetFrameTimestampSupportedANDROID = glad_debug_impl_eglGetFrameTimestampSupportedANDROID;
PFNEGLGETFRAMETIMESTAMPSANDROIDPROC glad_eglGetFrameTimestampsANDROID = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglGetFrameTimestampsANDROID(EGLDisplay dpy, EGLSurface surface, EGLuint64KHR frameId, EGLint numTimestamps, const EGLint * timestamps, EGLnsecsANDROID * values) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglGetFrameTimestampsANDROID", (GLADapiproc) glad_eglGetFrameTimestampsANDROID, 6, dpy, surface, frameId, numTimestamps, timestamps, values);
    ret = glad_eglGetFrameTimestampsANDROID(dpy, surface, frameId, numTimestamps, timestamps, values);
    _post_call_egl_callback((void*) &ret, "eglGetFrameTimestampsANDROID", (GLADapiproc) glad_eglGetFrameTimestampsANDROID, 6, dpy, surface, frameId, numTimestamps, timestamps, values);
    return ret;
}
PFNEGLGETFRAMETIMESTAMPSANDROIDPROC glad_debug_eglGetFrameTimestampsANDROID = glad_debug_impl_eglGetFrameTimestampsANDROID;
PFNEGLGETMSCRATEANGLEPROC glad_eglGetMscRateANGLE = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglGetMscRateANGLE(EGLDisplay dpy, EGLSurface surface, EGLint * numerator, EGLint * denominator) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglGetMscRateANGLE", (GLADapiproc) glad_eglGetMscRateANGLE, 4, dpy, surface, numerator, denominator);
    ret = glad_eglGetMscRateANGLE(dpy, surface, numerator, denominator);
    _post_call_egl_callback((void*) &ret, "eglGetMscRateANGLE", (GLADapiproc) glad_eglGetMscRateANGLE, 4, dpy, surface, numerator, denominator);
    return ret;
}
PFNEGLGETMSCRATEANGLEPROC glad_debug_eglGetMscRateANGLE = glad_debug_impl_eglGetMscRateANGLE;
PFNEGLGETNATIVECLIENTBUFFERANDROIDPROC glad_eglGetNativeClientBufferANDROID = NULL;
static EGLClientBuffer GLAD_API_PTR glad_debug_impl_eglGetNativeClientBufferANDROID(const struct AHardwareBuffer * buffer) {
    EGLClientBuffer ret;
    _pre_call_egl_callback("eglGetNativeClientBufferANDROID", (GLADapiproc) glad_eglGetNativeClientBufferANDROID, 1, buffer);
    ret = glad_eglGetNativeClientBufferANDROID(buffer);
    _post_call_egl_callback((void*) &ret, "eglGetNativeClientBufferANDROID", (GLADapiproc) glad_eglGetNativeClientBufferANDROID, 1, buffer);
    return ret;
}
PFNEGLGETNATIVECLIENTBUFFERANDROIDPROC glad_debug_eglGetNativeClientBufferANDROID = glad_debug_impl_eglGetNativeClientBufferANDROID;
PFNEGLGETNEXTFRAMEIDANDROIDPROC glad_eglGetNextFrameIdANDROID = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglGetNextFrameIdANDROID(EGLDisplay dpy, EGLSurface surface, EGLuint64KHR * frameId) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglGetNextFrameIdANDROID", (GLADapiproc) glad_eglGetNextFrameIdANDROID, 3, dpy, surface, frameId);
    ret = glad_eglGetNextFrameIdANDROID(dpy, surface, frameId);
    _post_call_egl_callback((void*) &ret, "eglGetNextFrameIdANDROID", (GLADapiproc) glad_eglGetNextFrameIdANDROID, 3, dpy, surface, frameId);
    return ret;
}
PFNEGLGETNEXTFRAMEIDANDROIDPROC glad_debug_eglGetNextFrameIdANDROID = glad_debug_impl_eglGetNextFrameIdANDROID;
PFNEGLGETOUTPUTLAYERSEXTPROC glad_eglGetOutputLayersEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglGetOutputLayersEXT(EGLDisplay dpy, const EGLAttrib * attrib_list, EGLOutputLayerEXT * layers, EGLint max_layers, EGLint * num_layers) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglGetOutputLayersEXT", (GLADapiproc) glad_eglGetOutputLayersEXT, 5, dpy, attrib_list, layers, max_layers, num_layers);
    ret = glad_eglGetOutputLayersEXT(dpy, attrib_list, layers, max_layers, num_layers);
    _post_call_egl_callback((void*) &ret, "eglGetOutputLayersEXT", (GLADapiproc) glad_eglGetOutputLayersEXT, 5, dpy, attrib_list, layers, max_layers, num_layers);
    return ret;
}
PFNEGLGETOUTPUTLAYERSEXTPROC glad_debug_eglGetOutputLayersEXT = glad_debug_impl_eglGetOutputLayersEXT;
PFNEGLGETOUTPUTPORTSEXTPROC glad_eglGetOutputPortsEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglGetOutputPortsEXT(EGLDisplay dpy, const EGLAttrib * attrib_list, EGLOutputPortEXT * ports, EGLint max_ports, EGLint * num_ports) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglGetOutputPortsEXT", (GLADapiproc) glad_eglGetOutputPortsEXT, 5, dpy, attrib_list, ports, max_ports, num_ports);
    ret = glad_eglGetOutputPortsEXT(dpy, attrib_list, ports, max_ports, num_ports);
    _post_call_egl_callback((void*) &ret, "eglGetOutputPortsEXT", (GLADapiproc) glad_eglGetOutputPortsEXT, 5, dpy, attrib_list, ports, max_ports, num_ports);
    return ret;
}
PFNEGLGETOUTPUTPORTSEXTPROC glad_debug_eglGetOutputPortsEXT = glad_debug_impl_eglGetOutputPortsEXT;
PFNEGLGETPLATFORMDISPLAYPROC glad_eglGetPlatformDisplay = NULL;
static EGLDisplay GLAD_API_PTR glad_debug_impl_eglGetPlatformDisplay(EGLenum platform, void * native_display, const EGLAttrib * attrib_list) {
    EGLDisplay ret;
    _pre_call_egl_callback("eglGetPlatformDisplay", (GLADapiproc) glad_eglGetPlatformDisplay, 3, platform, native_display, attrib_list);
    ret = glad_eglGetPlatformDisplay(platform, native_display, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglGetPlatformDisplay", (GLADapiproc) glad_eglGetPlatformDisplay, 3, platform, native_display, attrib_list);
    return ret;
}
PFNEGLGETPLATFORMDISPLAYPROC glad_debug_eglGetPlatformDisplay = glad_debug_impl_eglGetPlatformDisplay;
PFNEGLGETPLATFORMDISPLAYEXTPROC glad_eglGetPlatformDisplayEXT = NULL;
static EGLDisplay GLAD_API_PTR glad_debug_impl_eglGetPlatformDisplayEXT(EGLenum platform, void * native_display, const EGLint * attrib_list) {
    EGLDisplay ret;
    _pre_call_egl_callback("eglGetPlatformDisplayEXT", (GLADapiproc) glad_eglGetPlatformDisplayEXT, 3, platform, native_display, attrib_list);
    ret = glad_eglGetPlatformDisplayEXT(platform, native_display, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglGetPlatformDisplayEXT", (GLADapiproc) glad_eglGetPlatformDisplayEXT, 3, platform, native_display, attrib_list);
    return ret;
}
PFNEGLGETPLATFORMDISPLAYEXTPROC glad_debug_eglGetPlatformDisplayEXT = glad_debug_impl_eglGetPlatformDisplayEXT;
PFNEGLGETPROCADDRESSPROC glad_eglGetProcAddress = NULL;
static __eglMustCastToProperFunctionPointerType GLAD_API_PTR glad_debug_impl_eglGetProcAddress(const char * procname) {
    __eglMustCastToProperFunctionPointerType ret;
    _pre_call_egl_callback("eglGetProcAddress", (GLADapiproc) glad_eglGetProcAddress, 1, procname);
    ret = glad_eglGetProcAddress(procname);
    _post_call_egl_callback((void*) &ret, "eglGetProcAddress", (GLADapiproc) glad_eglGetProcAddress, 1, procname);
    return ret;
}
PFNEGLGETPROCADDRESSPROC glad_debug_eglGetProcAddress = glad_debug_impl_eglGetProcAddress;
PFNEGLGETSTREAMFILEDESCRIPTORKHRPROC glad_eglGetStreamFileDescriptorKHR = NULL;
static EGLNativeFileDescriptorKHR GLAD_API_PTR glad_debug_impl_eglGetStreamFileDescriptorKHR(EGLDisplay dpy, EGLStreamKHR stream) {
    EGLNativeFileDescriptorKHR ret;
    _pre_call_egl_callback("eglGetStreamFileDescriptorKHR", (GLADapiproc) glad_eglGetStreamFileDescriptorKHR, 2, dpy, stream);
    ret = glad_eglGetStreamFileDescriptorKHR(dpy, stream);
    _post_call_egl_callback((void*) &ret, "eglGetStreamFileDescriptorKHR", (GLADapiproc) glad_eglGetStreamFileDescriptorKHR, 2, dpy, stream);
    return ret;
}
PFNEGLGETSTREAMFILEDESCRIPTORKHRPROC glad_debug_eglGetStreamFileDescriptorKHR = glad_debug_impl_eglGetStreamFileDescriptorKHR;
PFNEGLGETSYNCATTRIBPROC glad_eglGetSyncAttrib = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglGetSyncAttrib(EGLDisplay dpy, EGLSync sync, EGLint attribute, EGLAttrib * value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglGetSyncAttrib", (GLADapiproc) glad_eglGetSyncAttrib, 4, dpy, sync, attribute, value);
    ret = glad_eglGetSyncAttrib(dpy, sync, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglGetSyncAttrib", (GLADapiproc) glad_eglGetSyncAttrib, 4, dpy, sync, attribute, value);
    return ret;
}
PFNEGLGETSYNCATTRIBPROC glad_debug_eglGetSyncAttrib = glad_debug_impl_eglGetSyncAttrib;
PFNEGLGETSYNCATTRIBKHRPROC glad_eglGetSyncAttribKHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglGetSyncAttribKHR(EGLDisplay dpy, EGLSyncKHR sync, EGLint attribute, EGLint * value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglGetSyncAttribKHR", (GLADapiproc) glad_eglGetSyncAttribKHR, 4, dpy, sync, attribute, value);
    ret = glad_eglGetSyncAttribKHR(dpy, sync, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglGetSyncAttribKHR", (GLADapiproc) glad_eglGetSyncAttribKHR, 4, dpy, sync, attribute, value);
    return ret;
}
PFNEGLGETSYNCATTRIBKHRPROC glad_debug_eglGetSyncAttribKHR = glad_debug_impl_eglGetSyncAttribKHR;
PFNEGLGETSYNCATTRIBNVPROC glad_eglGetSyncAttribNV = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglGetSyncAttribNV(EGLSyncNV sync, EGLint attribute, EGLint * value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglGetSyncAttribNV", (GLADapiproc) glad_eglGetSyncAttribNV, 3, sync, attribute, value);
    ret = glad_eglGetSyncAttribNV(sync, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglGetSyncAttribNV", (GLADapiproc) glad_eglGetSyncAttribNV, 3, sync, attribute, value);
    return ret;
}
PFNEGLGETSYNCATTRIBNVPROC glad_debug_eglGetSyncAttribNV = glad_debug_impl_eglGetSyncAttribNV;
PFNEGLGETSYSTEMTIMEFREQUENCYNVPROC glad_eglGetSystemTimeFrequencyNV = NULL;
static EGLuint64NV GLAD_API_PTR glad_debug_impl_eglGetSystemTimeFrequencyNV(void) {
    EGLuint64NV ret;
    _pre_call_egl_callback("eglGetSystemTimeFrequencyNV", (GLADapiproc) glad_eglGetSystemTimeFrequencyNV, 0);
    ret = glad_eglGetSystemTimeFrequencyNV();
    _post_call_egl_callback((void*) &ret, "eglGetSystemTimeFrequencyNV", (GLADapiproc) glad_eglGetSystemTimeFrequencyNV, 0);
    return ret;
}
PFNEGLGETSYSTEMTIMEFREQUENCYNVPROC glad_debug_eglGetSystemTimeFrequencyNV = glad_debug_impl_eglGetSystemTimeFrequencyNV;
PFNEGLGETSYSTEMTIMENVPROC glad_eglGetSystemTimeNV = NULL;
static EGLuint64NV GLAD_API_PTR glad_debug_impl_eglGetSystemTimeNV(void) {
    EGLuint64NV ret;
    _pre_call_egl_callback("eglGetSystemTimeNV", (GLADapiproc) glad_eglGetSystemTimeNV, 0);
    ret = glad_eglGetSystemTimeNV();
    _post_call_egl_callback((void*) &ret, "eglGetSystemTimeNV", (GLADapiproc) glad_eglGetSystemTimeNV, 0);
    return ret;
}
PFNEGLGETSYSTEMTIMENVPROC glad_debug_eglGetSystemTimeNV = glad_debug_impl_eglGetSystemTimeNV;
PFNEGLINITIALIZEPROC glad_eglInitialize = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglInitialize(EGLDisplay dpy, EGLint * major, EGLint * minor) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglInitialize", (GLADapiproc) glad_eglInitialize, 3, dpy, major, minor);
    ret = glad_eglInitialize(dpy, major, minor);
    _post_call_egl_callback((void*) &ret, "eglInitialize", (GLADapiproc) glad_eglInitialize, 3, dpy, major, minor);
    return ret;
}
PFNEGLINITIALIZEPROC glad_debug_eglInitialize = glad_debug_impl_eglInitialize;
PFNEGLLABELOBJECTKHRPROC glad_eglLabelObjectKHR = NULL;
static EGLint GLAD_API_PTR glad_debug_impl_eglLabelObjectKHR(EGLDisplay display, EGLenum objectType, EGLObjectKHR object, EGLLabelKHR label) {
    EGLint ret;
    _pre_call_egl_callback("eglLabelObjectKHR", (GLADapiproc) glad_eglLabelObjectKHR, 4, display, objectType, object, label);
    ret = glad_eglLabelObjectKHR(display, objectType, object, label);
    _post_call_egl_callback((void*) &ret, "eglLabelObjectKHR", (GLADapiproc) glad_eglLabelObjectKHR, 4, display, objectType, object, label);
    return ret;
}
PFNEGLLABELOBJECTKHRPROC glad_debug_eglLabelObjectKHR = glad_debug_impl_eglLabelObjectKHR;
PFNEGLLOCKSURFACEKHRPROC glad_eglLockSurfaceKHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglLockSurfaceKHR(EGLDisplay dpy, EGLSurface surface, const EGLint * attrib_list) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglLockSurfaceKHR", (GLADapiproc) glad_eglLockSurfaceKHR, 3, dpy, surface, attrib_list);
    ret = glad_eglLockSurfaceKHR(dpy, surface, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglLockSurfaceKHR", (GLADapiproc) glad_eglLockSurfaceKHR, 3, dpy, surface, attrib_list);
    return ret;
}
PFNEGLLOCKSURFACEKHRPROC glad_debug_eglLockSurfaceKHR = glad_debug_impl_eglLockSurfaceKHR;
PFNEGLMAKECURRENTPROC glad_eglMakeCurrent = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglMakeCurrent", (GLADapiproc) glad_eglMakeCurrent, 4, dpy, draw, read, ctx);
    ret = glad_eglMakeCurrent(dpy, draw, read, ctx);
    _post_call_egl_callback((void*) &ret, "eglMakeCurrent", (GLADapiproc) glad_eglMakeCurrent, 4, dpy, draw, read, ctx);
    return ret;
}
PFNEGLMAKECURRENTPROC glad_debug_eglMakeCurrent = glad_debug_impl_eglMakeCurrent;
PFNEGLOUTPUTLAYERATTRIBEXTPROC glad_eglOutputLayerAttribEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglOutputLayerAttribEXT(EGLDisplay dpy, EGLOutputLayerEXT layer, EGLint attribute, EGLAttrib value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglOutputLayerAttribEXT", (GLADapiproc) glad_eglOutputLayerAttribEXT, 4, dpy, layer, attribute, value);
    ret = glad_eglOutputLayerAttribEXT(dpy, layer, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglOutputLayerAttribEXT", (GLADapiproc) glad_eglOutputLayerAttribEXT, 4, dpy, layer, attribute, value);
    return ret;
}
PFNEGLOUTPUTLAYERATTRIBEXTPROC glad_debug_eglOutputLayerAttribEXT = glad_debug_impl_eglOutputLayerAttribEXT;
PFNEGLOUTPUTPORTATTRIBEXTPROC glad_eglOutputPortAttribEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglOutputPortAttribEXT(EGLDisplay dpy, EGLOutputPortEXT port, EGLint attribute, EGLAttrib value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglOutputPortAttribEXT", (GLADapiproc) glad_eglOutputPortAttribEXT, 4, dpy, port, attribute, value);
    ret = glad_eglOutputPortAttribEXT(dpy, port, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglOutputPortAttribEXT", (GLADapiproc) glad_eglOutputPortAttribEXT, 4, dpy, port, attribute, value);
    return ret;
}
PFNEGLOUTPUTPORTATTRIBEXTPROC glad_debug_eglOutputPortAttribEXT = glad_debug_impl_eglOutputPortAttribEXT;
PFNEGLPOSTSUBBUFFERNVPROC glad_eglPostSubBufferNV = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglPostSubBufferNV(EGLDisplay dpy, EGLSurface surface, EGLint x, EGLint y, EGLint width, EGLint height) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglPostSubBufferNV", (GLADapiproc) glad_eglPostSubBufferNV, 6, dpy, surface, x, y, width, height);
    ret = glad_eglPostSubBufferNV(dpy, surface, x, y, width, height);
    _post_call_egl_callback((void*) &ret, "eglPostSubBufferNV", (GLADapiproc) glad_eglPostSubBufferNV, 6, dpy, surface, x, y, width, height);
    return ret;
}
PFNEGLPOSTSUBBUFFERNVPROC glad_debug_eglPostSubBufferNV = glad_debug_impl_eglPostSubBufferNV;
PFNEGLPRESENTATIONTIMEANDROIDPROC glad_eglPresentationTimeANDROID = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglPresentationTimeANDROID(EGLDisplay dpy, EGLSurface surface, EGLnsecsANDROID time) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglPresentationTimeANDROID", (GLADapiproc) glad_eglPresentationTimeANDROID, 3, dpy, surface, time);
    ret = glad_eglPresentationTimeANDROID(dpy, surface, time);
    _post_call_egl_callback((void*) &ret, "eglPresentationTimeANDROID", (GLADapiproc) glad_eglPresentationTimeANDROID, 3, dpy, surface, time);
    return ret;
}
PFNEGLPRESENTATIONTIMEANDROIDPROC glad_debug_eglPresentationTimeANDROID = glad_debug_impl_eglPresentationTimeANDROID;
PFNEGLQUERYAPIPROC glad_eglQueryAPI = NULL;
static EGLenum GLAD_API_PTR glad_debug_impl_eglQueryAPI(void) {
    EGLenum ret;
    _pre_call_egl_callback("eglQueryAPI", (GLADapiproc) glad_eglQueryAPI, 0);
    ret = glad_eglQueryAPI();
    _post_call_egl_callback((void*) &ret, "eglQueryAPI", (GLADapiproc) glad_eglQueryAPI, 0);
    return ret;
}
PFNEGLQUERYAPIPROC glad_debug_eglQueryAPI = glad_debug_impl_eglQueryAPI;
PFNEGLQUERYCONTEXTPROC glad_eglQueryContext = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQueryContext(EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint * value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQueryContext", (GLADapiproc) glad_eglQueryContext, 4, dpy, ctx, attribute, value);
    ret = glad_eglQueryContext(dpy, ctx, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglQueryContext", (GLADapiproc) glad_eglQueryContext, 4, dpy, ctx, attribute, value);
    return ret;
}
PFNEGLQUERYCONTEXTPROC glad_debug_eglQueryContext = glad_debug_impl_eglQueryContext;
PFNEGLQUERYDEBUGKHRPROC glad_eglQueryDebugKHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQueryDebugKHR(EGLint attribute, EGLAttrib * value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQueryDebugKHR", (GLADapiproc) glad_eglQueryDebugKHR, 2, attribute, value);
    ret = glad_eglQueryDebugKHR(attribute, value);
    _post_call_egl_callback((void*) &ret, "eglQueryDebugKHR", (GLADapiproc) glad_eglQueryDebugKHR, 2, attribute, value);
    return ret;
}
PFNEGLQUERYDEBUGKHRPROC glad_debug_eglQueryDebugKHR = glad_debug_impl_eglQueryDebugKHR;
PFNEGLQUERYDEVICEATTRIBEXTPROC glad_eglQueryDeviceAttribEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQueryDeviceAttribEXT(EGLDeviceEXT device, EGLint attribute, EGLAttrib * value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQueryDeviceAttribEXT", (GLADapiproc) glad_eglQueryDeviceAttribEXT, 3, device, attribute, value);
    ret = glad_eglQueryDeviceAttribEXT(device, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglQueryDeviceAttribEXT", (GLADapiproc) glad_eglQueryDeviceAttribEXT, 3, device, attribute, value);
    return ret;
}
PFNEGLQUERYDEVICEATTRIBEXTPROC glad_debug_eglQueryDeviceAttribEXT = glad_debug_impl_eglQueryDeviceAttribEXT;
PFNEGLQUERYDEVICEBINARYEXTPROC glad_eglQueryDeviceBinaryEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQueryDeviceBinaryEXT(EGLDeviceEXT device, EGLint name, EGLint max_size, void * value, EGLint * size) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQueryDeviceBinaryEXT", (GLADapiproc) glad_eglQueryDeviceBinaryEXT, 5, device, name, max_size, value, size);
    ret = glad_eglQueryDeviceBinaryEXT(device, name, max_size, value, size);
    _post_call_egl_callback((void*) &ret, "eglQueryDeviceBinaryEXT", (GLADapiproc) glad_eglQueryDeviceBinaryEXT, 5, device, name, max_size, value, size);
    return ret;
}
PFNEGLQUERYDEVICEBINARYEXTPROC glad_debug_eglQueryDeviceBinaryEXT = glad_debug_impl_eglQueryDeviceBinaryEXT;
PFNEGLQUERYDEVICESTRINGEXTPROC glad_eglQueryDeviceStringEXT = NULL;
static const char * GLAD_API_PTR glad_debug_impl_eglQueryDeviceStringEXT(EGLDeviceEXT device, EGLint name) {
    const char * ret;
    _pre_call_egl_callback("eglQueryDeviceStringEXT", (GLADapiproc) glad_eglQueryDeviceStringEXT, 2, device, name);
    ret = glad_eglQueryDeviceStringEXT(device, name);
    _post_call_egl_callback((void*) &ret, "eglQueryDeviceStringEXT", (GLADapiproc) glad_eglQueryDeviceStringEXT, 2, device, name);
    return ret;
}
PFNEGLQUERYDEVICESTRINGEXTPROC glad_debug_eglQueryDeviceStringEXT = glad_debug_impl_eglQueryDeviceStringEXT;
PFNEGLQUERYDEVICESEXTPROC glad_eglQueryDevicesEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQueryDevicesEXT(EGLint max_devices, EGLDeviceEXT * devices, EGLint * num_devices) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQueryDevicesEXT", (GLADapiproc) glad_eglQueryDevicesEXT, 3, max_devices, devices, num_devices);
    ret = glad_eglQueryDevicesEXT(max_devices, devices, num_devices);
    _post_call_egl_callback((void*) &ret, "eglQueryDevicesEXT", (GLADapiproc) glad_eglQueryDevicesEXT, 3, max_devices, devices, num_devices);
    return ret;
}
PFNEGLQUERYDEVICESEXTPROC glad_debug_eglQueryDevicesEXT = glad_debug_impl_eglQueryDevicesEXT;
PFNEGLQUERYDISPLAYATTRIBEXTPROC glad_eglQueryDisplayAttribEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQueryDisplayAttribEXT(EGLDisplay dpy, EGLint attribute, EGLAttrib * value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQueryDisplayAttribEXT", (GLADapiproc) glad_eglQueryDisplayAttribEXT, 3, dpy, attribute, value);
    ret = glad_eglQueryDisplayAttribEXT(dpy, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglQueryDisplayAttribEXT", (GLADapiproc) glad_eglQueryDisplayAttribEXT, 3, dpy, attribute, value);
    return ret;
}
PFNEGLQUERYDISPLAYATTRIBEXTPROC glad_debug_eglQueryDisplayAttribEXT = glad_debug_impl_eglQueryDisplayAttribEXT;
PFNEGLQUERYDISPLAYATTRIBKHRPROC glad_eglQueryDisplayAttribKHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQueryDisplayAttribKHR(EGLDisplay dpy, EGLint name, EGLAttrib * value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQueryDisplayAttribKHR", (GLADapiproc) glad_eglQueryDisplayAttribKHR, 3, dpy, name, value);
    ret = glad_eglQueryDisplayAttribKHR(dpy, name, value);
    _post_call_egl_callback((void*) &ret, "eglQueryDisplayAttribKHR", (GLADapiproc) glad_eglQueryDisplayAttribKHR, 3, dpy, name, value);
    return ret;
}
PFNEGLQUERYDISPLAYATTRIBKHRPROC glad_debug_eglQueryDisplayAttribKHR = glad_debug_impl_eglQueryDisplayAttribKHR;
PFNEGLQUERYDISPLAYATTRIBNVPROC glad_eglQueryDisplayAttribNV = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQueryDisplayAttribNV(EGLDisplay dpy, EGLint attribute, EGLAttrib * value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQueryDisplayAttribNV", (GLADapiproc) glad_eglQueryDisplayAttribNV, 3, dpy, attribute, value);
    ret = glad_eglQueryDisplayAttribNV(dpy, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglQueryDisplayAttribNV", (GLADapiproc) glad_eglQueryDisplayAttribNV, 3, dpy, attribute, value);
    return ret;
}
PFNEGLQUERYDISPLAYATTRIBNVPROC glad_debug_eglQueryDisplayAttribNV = glad_debug_impl_eglQueryDisplayAttribNV;
PFNEGLQUERYDMABUFFORMATSEXTPROC glad_eglQueryDmaBufFormatsEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQueryDmaBufFormatsEXT(EGLDisplay dpy, EGLint max_formats, EGLint * formats, EGLint * num_formats) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQueryDmaBufFormatsEXT", (GLADapiproc) glad_eglQueryDmaBufFormatsEXT, 4, dpy, max_formats, formats, num_formats);
    ret = glad_eglQueryDmaBufFormatsEXT(dpy, max_formats, formats, num_formats);
    _post_call_egl_callback((void*) &ret, "eglQueryDmaBufFormatsEXT", (GLADapiproc) glad_eglQueryDmaBufFormatsEXT, 4, dpy, max_formats, formats, num_formats);
    return ret;
}
PFNEGLQUERYDMABUFFORMATSEXTPROC glad_debug_eglQueryDmaBufFormatsEXT = glad_debug_impl_eglQueryDmaBufFormatsEXT;
PFNEGLQUERYDMABUFMODIFIERSEXTPROC glad_eglQueryDmaBufModifiersEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQueryDmaBufModifiersEXT(EGLDisplay dpy, EGLint format, EGLint max_modifiers, EGLuint64KHR * modifiers, EGLBoolean * external_only, EGLint * num_modifiers) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQueryDmaBufModifiersEXT", (GLADapiproc) glad_eglQueryDmaBufModifiersEXT, 6, dpy, format, max_modifiers, modifiers, external_only, num_modifiers);
    ret = glad_eglQueryDmaBufModifiersEXT(dpy, format, max_modifiers, modifiers, external_only, num_modifiers);
    _post_call_egl_callback((void*) &ret, "eglQueryDmaBufModifiersEXT", (GLADapiproc) glad_eglQueryDmaBufModifiersEXT, 6, dpy, format, max_modifiers, modifiers, external_only, num_modifiers);
    return ret;
}
PFNEGLQUERYDMABUFMODIFIERSEXTPROC glad_debug_eglQueryDmaBufModifiersEXT = glad_debug_impl_eglQueryDmaBufModifiersEXT;
PFNEGLQUERYNATIVEDISPLAYNVPROC glad_eglQueryNativeDisplayNV = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQueryNativeDisplayNV(EGLDisplay dpy, EGLNativeDisplayType * display_id) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQueryNativeDisplayNV", (GLADapiproc) glad_eglQueryNativeDisplayNV, 2, dpy, display_id);
    ret = glad_eglQueryNativeDisplayNV(dpy, display_id);
    _post_call_egl_callback((void*) &ret, "eglQueryNativeDisplayNV", (GLADapiproc) glad_eglQueryNativeDisplayNV, 2, dpy, display_id);
    return ret;
}
PFNEGLQUERYNATIVEDISPLAYNVPROC glad_debug_eglQueryNativeDisplayNV = glad_debug_impl_eglQueryNativeDisplayNV;
PFNEGLQUERYNATIVEPIXMAPNVPROC glad_eglQueryNativePixmapNV = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQueryNativePixmapNV(EGLDisplay dpy, EGLSurface surf, EGLNativePixmapType * pixmap) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQueryNativePixmapNV", (GLADapiproc) glad_eglQueryNativePixmapNV, 3, dpy, surf, pixmap);
    ret = glad_eglQueryNativePixmapNV(dpy, surf, pixmap);
    _post_call_egl_callback((void*) &ret, "eglQueryNativePixmapNV", (GLADapiproc) glad_eglQueryNativePixmapNV, 3, dpy, surf, pixmap);
    return ret;
}
PFNEGLQUERYNATIVEPIXMAPNVPROC glad_debug_eglQueryNativePixmapNV = glad_debug_impl_eglQueryNativePixmapNV;
PFNEGLQUERYNATIVEWINDOWNVPROC glad_eglQueryNativeWindowNV = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQueryNativeWindowNV(EGLDisplay dpy, EGLSurface surf, EGLNativeWindowType * window) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQueryNativeWindowNV", (GLADapiproc) glad_eglQueryNativeWindowNV, 3, dpy, surf, window);
    ret = glad_eglQueryNativeWindowNV(dpy, surf, window);
    _post_call_egl_callback((void*) &ret, "eglQueryNativeWindowNV", (GLADapiproc) glad_eglQueryNativeWindowNV, 3, dpy, surf, window);
    return ret;
}
PFNEGLQUERYNATIVEWINDOWNVPROC glad_debug_eglQueryNativeWindowNV = glad_debug_impl_eglQueryNativeWindowNV;
PFNEGLQUERYOUTPUTLAYERATTRIBEXTPROC glad_eglQueryOutputLayerAttribEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQueryOutputLayerAttribEXT(EGLDisplay dpy, EGLOutputLayerEXT layer, EGLint attribute, EGLAttrib * value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQueryOutputLayerAttribEXT", (GLADapiproc) glad_eglQueryOutputLayerAttribEXT, 4, dpy, layer, attribute, value);
    ret = glad_eglQueryOutputLayerAttribEXT(dpy, layer, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglQueryOutputLayerAttribEXT", (GLADapiproc) glad_eglQueryOutputLayerAttribEXT, 4, dpy, layer, attribute, value);
    return ret;
}
PFNEGLQUERYOUTPUTLAYERATTRIBEXTPROC glad_debug_eglQueryOutputLayerAttribEXT = glad_debug_impl_eglQueryOutputLayerAttribEXT;
PFNEGLQUERYOUTPUTLAYERSTRINGEXTPROC glad_eglQueryOutputLayerStringEXT = NULL;
static const char * GLAD_API_PTR glad_debug_impl_eglQueryOutputLayerStringEXT(EGLDisplay dpy, EGLOutputLayerEXT layer, EGLint name) {
    const char * ret;
    _pre_call_egl_callback("eglQueryOutputLayerStringEXT", (GLADapiproc) glad_eglQueryOutputLayerStringEXT, 3, dpy, layer, name);
    ret = glad_eglQueryOutputLayerStringEXT(dpy, layer, name);
    _post_call_egl_callback((void*) &ret, "eglQueryOutputLayerStringEXT", (GLADapiproc) glad_eglQueryOutputLayerStringEXT, 3, dpy, layer, name);
    return ret;
}
PFNEGLQUERYOUTPUTLAYERSTRINGEXTPROC glad_debug_eglQueryOutputLayerStringEXT = glad_debug_impl_eglQueryOutputLayerStringEXT;
PFNEGLQUERYOUTPUTPORTATTRIBEXTPROC glad_eglQueryOutputPortAttribEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQueryOutputPortAttribEXT(EGLDisplay dpy, EGLOutputPortEXT port, EGLint attribute, EGLAttrib * value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQueryOutputPortAttribEXT", (GLADapiproc) glad_eglQueryOutputPortAttribEXT, 4, dpy, port, attribute, value);
    ret = glad_eglQueryOutputPortAttribEXT(dpy, port, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglQueryOutputPortAttribEXT", (GLADapiproc) glad_eglQueryOutputPortAttribEXT, 4, dpy, port, attribute, value);
    return ret;
}
PFNEGLQUERYOUTPUTPORTATTRIBEXTPROC glad_debug_eglQueryOutputPortAttribEXT = glad_debug_impl_eglQueryOutputPortAttribEXT;
PFNEGLQUERYOUTPUTPORTSTRINGEXTPROC glad_eglQueryOutputPortStringEXT = NULL;
static const char * GLAD_API_PTR glad_debug_impl_eglQueryOutputPortStringEXT(EGLDisplay dpy, EGLOutputPortEXT port, EGLint name) {
    const char * ret;
    _pre_call_egl_callback("eglQueryOutputPortStringEXT", (GLADapiproc) glad_eglQueryOutputPortStringEXT, 3, dpy, port, name);
    ret = glad_eglQueryOutputPortStringEXT(dpy, port, name);
    _post_call_egl_callback((void*) &ret, "eglQueryOutputPortStringEXT", (GLADapiproc) glad_eglQueryOutputPortStringEXT, 3, dpy, port, name);
    return ret;
}
PFNEGLQUERYOUTPUTPORTSTRINGEXTPROC glad_debug_eglQueryOutputPortStringEXT = glad_debug_impl_eglQueryOutputPortStringEXT;
PFNEGLQUERYSTREAMATTRIBKHRPROC glad_eglQueryStreamAttribKHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQueryStreamAttribKHR(EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLAttrib * value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQueryStreamAttribKHR", (GLADapiproc) glad_eglQueryStreamAttribKHR, 4, dpy, stream, attribute, value);
    ret = glad_eglQueryStreamAttribKHR(dpy, stream, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglQueryStreamAttribKHR", (GLADapiproc) glad_eglQueryStreamAttribKHR, 4, dpy, stream, attribute, value);
    return ret;
}
PFNEGLQUERYSTREAMATTRIBKHRPROC glad_debug_eglQueryStreamAttribKHR = glad_debug_impl_eglQueryStreamAttribKHR;
PFNEGLQUERYSTREAMCONSUMEREVENTNVPROC glad_eglQueryStreamConsumerEventNV = NULL;
static EGLint GLAD_API_PTR glad_debug_impl_eglQueryStreamConsumerEventNV(EGLDisplay dpy, EGLStreamKHR stream, EGLTime timeout, EGLenum * event, EGLAttrib * aux) {
    EGLint ret;
    _pre_call_egl_callback("eglQueryStreamConsumerEventNV", (GLADapiproc) glad_eglQueryStreamConsumerEventNV, 5, dpy, stream, timeout, event, aux);
    ret = glad_eglQueryStreamConsumerEventNV(dpy, stream, timeout, event, aux);
    _post_call_egl_callback((void*) &ret, "eglQueryStreamConsumerEventNV", (GLADapiproc) glad_eglQueryStreamConsumerEventNV, 5, dpy, stream, timeout, event, aux);
    return ret;
}
PFNEGLQUERYSTREAMCONSUMEREVENTNVPROC glad_debug_eglQueryStreamConsumerEventNV = glad_debug_impl_eglQueryStreamConsumerEventNV;
PFNEGLQUERYSTREAMKHRPROC glad_eglQueryStreamKHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQueryStreamKHR(EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLint * value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQueryStreamKHR", (GLADapiproc) glad_eglQueryStreamKHR, 4, dpy, stream, attribute, value);
    ret = glad_eglQueryStreamKHR(dpy, stream, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglQueryStreamKHR", (GLADapiproc) glad_eglQueryStreamKHR, 4, dpy, stream, attribute, value);
    return ret;
}
PFNEGLQUERYSTREAMKHRPROC glad_debug_eglQueryStreamKHR = glad_debug_impl_eglQueryStreamKHR;
PFNEGLQUERYSTREAMMETADATANVPROC glad_eglQueryStreamMetadataNV = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQueryStreamMetadataNV(EGLDisplay dpy, EGLStreamKHR stream, EGLenum name, EGLint n, EGLint offset, EGLint size, void * data) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQueryStreamMetadataNV", (GLADapiproc) glad_eglQueryStreamMetadataNV, 7, dpy, stream, name, n, offset, size, data);
    ret = glad_eglQueryStreamMetadataNV(dpy, stream, name, n, offset, size, data);
    _post_call_egl_callback((void*) &ret, "eglQueryStreamMetadataNV", (GLADapiproc) glad_eglQueryStreamMetadataNV, 7, dpy, stream, name, n, offset, size, data);
    return ret;
}
PFNEGLQUERYSTREAMMETADATANVPROC glad_debug_eglQueryStreamMetadataNV = glad_debug_impl_eglQueryStreamMetadataNV;
PFNEGLQUERYSTREAMTIMEKHRPROC glad_eglQueryStreamTimeKHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQueryStreamTimeKHR(EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLTimeKHR * value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQueryStreamTimeKHR", (GLADapiproc) glad_eglQueryStreamTimeKHR, 4, dpy, stream, attribute, value);
    ret = glad_eglQueryStreamTimeKHR(dpy, stream, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglQueryStreamTimeKHR", (GLADapiproc) glad_eglQueryStreamTimeKHR, 4, dpy, stream, attribute, value);
    return ret;
}
PFNEGLQUERYSTREAMTIMEKHRPROC glad_debug_eglQueryStreamTimeKHR = glad_debug_impl_eglQueryStreamTimeKHR;
PFNEGLQUERYSTREAMU64KHRPROC glad_eglQueryStreamu64KHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQueryStreamu64KHR(EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLuint64KHR * value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQueryStreamu64KHR", (GLADapiproc) glad_eglQueryStreamu64KHR, 4, dpy, stream, attribute, value);
    ret = glad_eglQueryStreamu64KHR(dpy, stream, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglQueryStreamu64KHR", (GLADapiproc) glad_eglQueryStreamu64KHR, 4, dpy, stream, attribute, value);
    return ret;
}
PFNEGLQUERYSTREAMU64KHRPROC glad_debug_eglQueryStreamu64KHR = glad_debug_impl_eglQueryStreamu64KHR;
PFNEGLQUERYSTRINGPROC glad_eglQueryString = NULL;
static const char * GLAD_API_PTR glad_debug_impl_eglQueryString(EGLDisplay dpy, EGLint name) {
    const char * ret;
    _pre_call_egl_callback("eglQueryString", (GLADapiproc) glad_eglQueryString, 2, dpy, name);
    ret = glad_eglQueryString(dpy, name);
    _post_call_egl_callback((void*) &ret, "eglQueryString", (GLADapiproc) glad_eglQueryString, 2, dpy, name);
    return ret;
}
PFNEGLQUERYSTRINGPROC glad_debug_eglQueryString = glad_debug_impl_eglQueryString;
PFNEGLQUERYSUPPORTEDCOMPRESSIONRATESEXTPROC glad_eglQuerySupportedCompressionRatesEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQuerySupportedCompressionRatesEXT(EGLDisplay dpy, EGLConfig config, const EGLAttrib * attrib_list, EGLint * rates, EGLint rate_size, EGLint * num_rates) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQuerySupportedCompressionRatesEXT", (GLADapiproc) glad_eglQuerySupportedCompressionRatesEXT, 6, dpy, config, attrib_list, rates, rate_size, num_rates);
    ret = glad_eglQuerySupportedCompressionRatesEXT(dpy, config, attrib_list, rates, rate_size, num_rates);
    _post_call_egl_callback((void*) &ret, "eglQuerySupportedCompressionRatesEXT", (GLADapiproc) glad_eglQuerySupportedCompressionRatesEXT, 6, dpy, config, attrib_list, rates, rate_size, num_rates);
    return ret;
}
PFNEGLQUERYSUPPORTEDCOMPRESSIONRATESEXTPROC glad_debug_eglQuerySupportedCompressionRatesEXT = glad_debug_impl_eglQuerySupportedCompressionRatesEXT;
PFNEGLQUERYSURFACEPROC glad_eglQuerySurface = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQuerySurface(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint * value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQuerySurface", (GLADapiproc) glad_eglQuerySurface, 4, dpy, surface, attribute, value);
    ret = glad_eglQuerySurface(dpy, surface, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglQuerySurface", (GLADapiproc) glad_eglQuerySurface, 4, dpy, surface, attribute, value);
    return ret;
}
PFNEGLQUERYSURFACEPROC glad_debug_eglQuerySurface = glad_debug_impl_eglQuerySurface;
PFNEGLQUERYSURFACE64KHRPROC glad_eglQuerySurface64KHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQuerySurface64KHR(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLAttribKHR * value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQuerySurface64KHR", (GLADapiproc) glad_eglQuerySurface64KHR, 4, dpy, surface, attribute, value);
    ret = glad_eglQuerySurface64KHR(dpy, surface, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglQuerySurface64KHR", (GLADapiproc) glad_eglQuerySurface64KHR, 4, dpy, surface, attribute, value);
    return ret;
}
PFNEGLQUERYSURFACE64KHRPROC glad_debug_eglQuerySurface64KHR = glad_debug_impl_eglQuerySurface64KHR;
PFNEGLQUERYSURFACEPOINTERANGLEPROC glad_eglQuerySurfacePointerANGLE = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQuerySurfacePointerANGLE(EGLDisplay dpy, EGLSurface surface, EGLint attribute, void ** value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQuerySurfacePointerANGLE", (GLADapiproc) glad_eglQuerySurfacePointerANGLE, 4, dpy, surface, attribute, value);
    ret = glad_eglQuerySurfacePointerANGLE(dpy, surface, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglQuerySurfacePointerANGLE", (GLADapiproc) glad_eglQuerySurfacePointerANGLE, 4, dpy, surface, attribute, value);
    return ret;
}
PFNEGLQUERYSURFACEPOINTERANGLEPROC glad_debug_eglQuerySurfacePointerANGLE = glad_debug_impl_eglQuerySurfacePointerANGLE;
PFNEGLQUERYWAYLANDBUFFERWLPROC glad_eglQueryWaylandBufferWL = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglQueryWaylandBufferWL(EGLDisplay dpy, struct wl_resource * buffer, EGLint attribute, EGLint * value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglQueryWaylandBufferWL", (GLADapiproc) glad_eglQueryWaylandBufferWL, 4, dpy, buffer, attribute, value);
    ret = glad_eglQueryWaylandBufferWL(dpy, buffer, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglQueryWaylandBufferWL", (GLADapiproc) glad_eglQueryWaylandBufferWL, 4, dpy, buffer, attribute, value);
    return ret;
}
PFNEGLQUERYWAYLANDBUFFERWLPROC glad_debug_eglQueryWaylandBufferWL = glad_debug_impl_eglQueryWaylandBufferWL;
PFNEGLRELEASETEXIMAGEPROC glad_eglReleaseTexImage = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglReleaseTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglReleaseTexImage", (GLADapiproc) glad_eglReleaseTexImage, 3, dpy, surface, buffer);
    ret = glad_eglReleaseTexImage(dpy, surface, buffer);
    _post_call_egl_callback((void*) &ret, "eglReleaseTexImage", (GLADapiproc) glad_eglReleaseTexImage, 3, dpy, surface, buffer);
    return ret;
}
PFNEGLRELEASETEXIMAGEPROC glad_debug_eglReleaseTexImage = glad_debug_impl_eglReleaseTexImage;
PFNEGLRELEASETHREADPROC glad_eglReleaseThread = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglReleaseThread(void) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglReleaseThread", (GLADapiproc) glad_eglReleaseThread, 0);
    ret = glad_eglReleaseThread();
    _post_call_egl_callback((void*) &ret, "eglReleaseThread", (GLADapiproc) glad_eglReleaseThread, 0);
    return ret;
}
PFNEGLRELEASETHREADPROC glad_debug_eglReleaseThread = glad_debug_impl_eglReleaseThread;
PFNEGLRESETSTREAMNVPROC glad_eglResetStreamNV = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglResetStreamNV(EGLDisplay dpy, EGLStreamKHR stream) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglResetStreamNV", (GLADapiproc) glad_eglResetStreamNV, 2, dpy, stream);
    ret = glad_eglResetStreamNV(dpy, stream);
    _post_call_egl_callback((void*) &ret, "eglResetStreamNV", (GLADapiproc) glad_eglResetStreamNV, 2, dpy, stream);
    return ret;
}
PFNEGLRESETSTREAMNVPROC glad_debug_eglResetStreamNV = glad_debug_impl_eglResetStreamNV;
PFNEGLSETBLOBCACHEFUNCSANDROIDPROC glad_eglSetBlobCacheFuncsANDROID = NULL;
static void GLAD_API_PTR glad_debug_impl_eglSetBlobCacheFuncsANDROID(EGLDisplay dpy, EGLSetBlobFuncANDROID set, EGLGetBlobFuncANDROID get) {
    _pre_call_egl_callback("eglSetBlobCacheFuncsANDROID", (GLADapiproc) glad_eglSetBlobCacheFuncsANDROID, 3, dpy, set, get);
    glad_eglSetBlobCacheFuncsANDROID(dpy, set, get);
    _post_call_egl_callback(NULL, "eglSetBlobCacheFuncsANDROID", (GLADapiproc) glad_eglSetBlobCacheFuncsANDROID, 3, dpy, set, get);
    
}
PFNEGLSETBLOBCACHEFUNCSANDROIDPROC glad_debug_eglSetBlobCacheFuncsANDROID = glad_debug_impl_eglSetBlobCacheFuncsANDROID;
PFNEGLSETDAMAGEREGIONKHRPROC glad_eglSetDamageRegionKHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglSetDamageRegionKHR(EGLDisplay dpy, EGLSurface surface, EGLint * rects, EGLint n_rects) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglSetDamageRegionKHR", (GLADapiproc) glad_eglSetDamageRegionKHR, 4, dpy, surface, rects, n_rects);
    ret = glad_eglSetDamageRegionKHR(dpy, surface, rects, n_rects);
    _post_call_egl_callback((void*) &ret, "eglSetDamageRegionKHR", (GLADapiproc) glad_eglSetDamageRegionKHR, 4, dpy, surface, rects, n_rects);
    return ret;
}
PFNEGLSETDAMAGEREGIONKHRPROC glad_debug_eglSetDamageRegionKHR = glad_debug_impl_eglSetDamageRegionKHR;
PFNEGLSETSTREAMATTRIBKHRPROC glad_eglSetStreamAttribKHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglSetStreamAttribKHR(EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLAttrib value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglSetStreamAttribKHR", (GLADapiproc) glad_eglSetStreamAttribKHR, 4, dpy, stream, attribute, value);
    ret = glad_eglSetStreamAttribKHR(dpy, stream, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglSetStreamAttribKHR", (GLADapiproc) glad_eglSetStreamAttribKHR, 4, dpy, stream, attribute, value);
    return ret;
}
PFNEGLSETSTREAMATTRIBKHRPROC glad_debug_eglSetStreamAttribKHR = glad_debug_impl_eglSetStreamAttribKHR;
PFNEGLSETSTREAMMETADATANVPROC glad_eglSetStreamMetadataNV = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglSetStreamMetadataNV(EGLDisplay dpy, EGLStreamKHR stream, EGLint n, EGLint offset, EGLint size, const void * data) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglSetStreamMetadataNV", (GLADapiproc) glad_eglSetStreamMetadataNV, 6, dpy, stream, n, offset, size, data);
    ret = glad_eglSetStreamMetadataNV(dpy, stream, n, offset, size, data);
    _post_call_egl_callback((void*) &ret, "eglSetStreamMetadataNV", (GLADapiproc) glad_eglSetStreamMetadataNV, 6, dpy, stream, n, offset, size, data);
    return ret;
}
PFNEGLSETSTREAMMETADATANVPROC glad_debug_eglSetStreamMetadataNV = glad_debug_impl_eglSetStreamMetadataNV;
PFNEGLSIGNALSYNCKHRPROC glad_eglSignalSyncKHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglSignalSyncKHR(EGLDisplay dpy, EGLSyncKHR sync, EGLenum mode) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglSignalSyncKHR", (GLADapiproc) glad_eglSignalSyncKHR, 3, dpy, sync, mode);
    ret = glad_eglSignalSyncKHR(dpy, sync, mode);
    _post_call_egl_callback((void*) &ret, "eglSignalSyncKHR", (GLADapiproc) glad_eglSignalSyncKHR, 3, dpy, sync, mode);
    return ret;
}
PFNEGLSIGNALSYNCKHRPROC glad_debug_eglSignalSyncKHR = glad_debug_impl_eglSignalSyncKHR;
PFNEGLSIGNALSYNCNVPROC glad_eglSignalSyncNV = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglSignalSyncNV(EGLSyncNV sync, EGLenum mode) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglSignalSyncNV", (GLADapiproc) glad_eglSignalSyncNV, 2, sync, mode);
    ret = glad_eglSignalSyncNV(sync, mode);
    _post_call_egl_callback((void*) &ret, "eglSignalSyncNV", (GLADapiproc) glad_eglSignalSyncNV, 2, sync, mode);
    return ret;
}
PFNEGLSIGNALSYNCNVPROC glad_debug_eglSignalSyncNV = glad_debug_impl_eglSignalSyncNV;
PFNEGLSTREAMACQUIREIMAGENVPROC glad_eglStreamAcquireImageNV = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglStreamAcquireImageNV(EGLDisplay dpy, EGLStreamKHR stream, EGLImage * pImage, EGLSync sync) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglStreamAcquireImageNV", (GLADapiproc) glad_eglStreamAcquireImageNV, 4, dpy, stream, pImage, sync);
    ret = glad_eglStreamAcquireImageNV(dpy, stream, pImage, sync);
    _post_call_egl_callback((void*) &ret, "eglStreamAcquireImageNV", (GLADapiproc) glad_eglStreamAcquireImageNV, 4, dpy, stream, pImage, sync);
    return ret;
}
PFNEGLSTREAMACQUIREIMAGENVPROC glad_debug_eglStreamAcquireImageNV = glad_debug_impl_eglStreamAcquireImageNV;
PFNEGLSTREAMATTRIBKHRPROC glad_eglStreamAttribKHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglStreamAttribKHR(EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLint value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglStreamAttribKHR", (GLADapiproc) glad_eglStreamAttribKHR, 4, dpy, stream, attribute, value);
    ret = glad_eglStreamAttribKHR(dpy, stream, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglStreamAttribKHR", (GLADapiproc) glad_eglStreamAttribKHR, 4, dpy, stream, attribute, value);
    return ret;
}
PFNEGLSTREAMATTRIBKHRPROC glad_debug_eglStreamAttribKHR = glad_debug_impl_eglStreamAttribKHR;
PFNEGLSTREAMCONSUMERACQUIREATTRIBKHRPROC glad_eglStreamConsumerAcquireAttribKHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglStreamConsumerAcquireAttribKHR(EGLDisplay dpy, EGLStreamKHR stream, const EGLAttrib * attrib_list) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglStreamConsumerAcquireAttribKHR", (GLADapiproc) glad_eglStreamConsumerAcquireAttribKHR, 3, dpy, stream, attrib_list);
    ret = glad_eglStreamConsumerAcquireAttribKHR(dpy, stream, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglStreamConsumerAcquireAttribKHR", (GLADapiproc) glad_eglStreamConsumerAcquireAttribKHR, 3, dpy, stream, attrib_list);
    return ret;
}
PFNEGLSTREAMCONSUMERACQUIREATTRIBKHRPROC glad_debug_eglStreamConsumerAcquireAttribKHR = glad_debug_impl_eglStreamConsumerAcquireAttribKHR;
PFNEGLSTREAMCONSUMERACQUIREKHRPROC glad_eglStreamConsumerAcquireKHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglStreamConsumerAcquireKHR(EGLDisplay dpy, EGLStreamKHR stream) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglStreamConsumerAcquireKHR", (GLADapiproc) glad_eglStreamConsumerAcquireKHR, 2, dpy, stream);
    ret = glad_eglStreamConsumerAcquireKHR(dpy, stream);
    _post_call_egl_callback((void*) &ret, "eglStreamConsumerAcquireKHR", (GLADapiproc) glad_eglStreamConsumerAcquireKHR, 2, dpy, stream);
    return ret;
}
PFNEGLSTREAMCONSUMERACQUIREKHRPROC glad_debug_eglStreamConsumerAcquireKHR = glad_debug_impl_eglStreamConsumerAcquireKHR;
PFNEGLSTREAMCONSUMERGLTEXTUREEXTERNALATTRIBSNVPROC glad_eglStreamConsumerGLTextureExternalAttribsNV = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglStreamConsumerGLTextureExternalAttribsNV(EGLDisplay dpy, EGLStreamKHR stream, const EGLAttrib * attrib_list) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglStreamConsumerGLTextureExternalAttribsNV", (GLADapiproc) glad_eglStreamConsumerGLTextureExternalAttribsNV, 3, dpy, stream, attrib_list);
    ret = glad_eglStreamConsumerGLTextureExternalAttribsNV(dpy, stream, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglStreamConsumerGLTextureExternalAttribsNV", (GLADapiproc) glad_eglStreamConsumerGLTextureExternalAttribsNV, 3, dpy, stream, attrib_list);
    return ret;
}
PFNEGLSTREAMCONSUMERGLTEXTUREEXTERNALATTRIBSNVPROC glad_debug_eglStreamConsumerGLTextureExternalAttribsNV = glad_debug_impl_eglStreamConsumerGLTextureExternalAttribsNV;
PFNEGLSTREAMCONSUMERGLTEXTUREEXTERNALKHRPROC glad_eglStreamConsumerGLTextureExternalKHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglStreamConsumerGLTextureExternalKHR(EGLDisplay dpy, EGLStreamKHR stream) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglStreamConsumerGLTextureExternalKHR", (GLADapiproc) glad_eglStreamConsumerGLTextureExternalKHR, 2, dpy, stream);
    ret = glad_eglStreamConsumerGLTextureExternalKHR(dpy, stream);
    _post_call_egl_callback((void*) &ret, "eglStreamConsumerGLTextureExternalKHR", (GLADapiproc) glad_eglStreamConsumerGLTextureExternalKHR, 2, dpy, stream);
    return ret;
}
PFNEGLSTREAMCONSUMERGLTEXTUREEXTERNALKHRPROC glad_debug_eglStreamConsumerGLTextureExternalKHR = glad_debug_impl_eglStreamConsumerGLTextureExternalKHR;
PFNEGLSTREAMCONSUMEROUTPUTEXTPROC glad_eglStreamConsumerOutputEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglStreamConsumerOutputEXT(EGLDisplay dpy, EGLStreamKHR stream, EGLOutputLayerEXT layer) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglStreamConsumerOutputEXT", (GLADapiproc) glad_eglStreamConsumerOutputEXT, 3, dpy, stream, layer);
    ret = glad_eglStreamConsumerOutputEXT(dpy, stream, layer);
    _post_call_egl_callback((void*) &ret, "eglStreamConsumerOutputEXT", (GLADapiproc) glad_eglStreamConsumerOutputEXT, 3, dpy, stream, layer);
    return ret;
}
PFNEGLSTREAMCONSUMEROUTPUTEXTPROC glad_debug_eglStreamConsumerOutputEXT = glad_debug_impl_eglStreamConsumerOutputEXT;
PFNEGLSTREAMCONSUMERRELEASEATTRIBKHRPROC glad_eglStreamConsumerReleaseAttribKHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglStreamConsumerReleaseAttribKHR(EGLDisplay dpy, EGLStreamKHR stream, const EGLAttrib * attrib_list) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglStreamConsumerReleaseAttribKHR", (GLADapiproc) glad_eglStreamConsumerReleaseAttribKHR, 3, dpy, stream, attrib_list);
    ret = glad_eglStreamConsumerReleaseAttribKHR(dpy, stream, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglStreamConsumerReleaseAttribKHR", (GLADapiproc) glad_eglStreamConsumerReleaseAttribKHR, 3, dpy, stream, attrib_list);
    return ret;
}
PFNEGLSTREAMCONSUMERRELEASEATTRIBKHRPROC glad_debug_eglStreamConsumerReleaseAttribKHR = glad_debug_impl_eglStreamConsumerReleaseAttribKHR;
PFNEGLSTREAMCONSUMERRELEASEKHRPROC glad_eglStreamConsumerReleaseKHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglStreamConsumerReleaseKHR(EGLDisplay dpy, EGLStreamKHR stream) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglStreamConsumerReleaseKHR", (GLADapiproc) glad_eglStreamConsumerReleaseKHR, 2, dpy, stream);
    ret = glad_eglStreamConsumerReleaseKHR(dpy, stream);
    _post_call_egl_callback((void*) &ret, "eglStreamConsumerReleaseKHR", (GLADapiproc) glad_eglStreamConsumerReleaseKHR, 2, dpy, stream);
    return ret;
}
PFNEGLSTREAMCONSUMERRELEASEKHRPROC glad_debug_eglStreamConsumerReleaseKHR = glad_debug_impl_eglStreamConsumerReleaseKHR;
PFNEGLSTREAMFLUSHNVPROC glad_eglStreamFlushNV = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglStreamFlushNV(EGLDisplay dpy, EGLStreamKHR stream) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglStreamFlushNV", (GLADapiproc) glad_eglStreamFlushNV, 2, dpy, stream);
    ret = glad_eglStreamFlushNV(dpy, stream);
    _post_call_egl_callback((void*) &ret, "eglStreamFlushNV", (GLADapiproc) glad_eglStreamFlushNV, 2, dpy, stream);
    return ret;
}
PFNEGLSTREAMFLUSHNVPROC glad_debug_eglStreamFlushNV = glad_debug_impl_eglStreamFlushNV;
PFNEGLSTREAMIMAGECONSUMERCONNECTNVPROC glad_eglStreamImageConsumerConnectNV = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglStreamImageConsumerConnectNV(EGLDisplay dpy, EGLStreamKHR stream, EGLint num_modifiers, const EGLuint64KHR * modifiers, const EGLAttrib * attrib_list) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglStreamImageConsumerConnectNV", (GLADapiproc) glad_eglStreamImageConsumerConnectNV, 5, dpy, stream, num_modifiers, modifiers, attrib_list);
    ret = glad_eglStreamImageConsumerConnectNV(dpy, stream, num_modifiers, modifiers, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglStreamImageConsumerConnectNV", (GLADapiproc) glad_eglStreamImageConsumerConnectNV, 5, dpy, stream, num_modifiers, modifiers, attrib_list);
    return ret;
}
PFNEGLSTREAMIMAGECONSUMERCONNECTNVPROC glad_debug_eglStreamImageConsumerConnectNV = glad_debug_impl_eglStreamImageConsumerConnectNV;
PFNEGLSTREAMRELEASEIMAGENVPROC glad_eglStreamReleaseImageNV = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglStreamReleaseImageNV(EGLDisplay dpy, EGLStreamKHR stream, EGLImage image, EGLSync sync) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglStreamReleaseImageNV", (GLADapiproc) glad_eglStreamReleaseImageNV, 4, dpy, stream, image, sync);
    ret = glad_eglStreamReleaseImageNV(dpy, stream, image, sync);
    _post_call_egl_callback((void*) &ret, "eglStreamReleaseImageNV", (GLADapiproc) glad_eglStreamReleaseImageNV, 4, dpy, stream, image, sync);
    return ret;
}
PFNEGLSTREAMRELEASEIMAGENVPROC glad_debug_eglStreamReleaseImageNV = glad_debug_impl_eglStreamReleaseImageNV;
PFNEGLSURFACEATTRIBPROC glad_eglSurfaceAttrib = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglSurfaceAttrib(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglSurfaceAttrib", (GLADapiproc) glad_eglSurfaceAttrib, 4, dpy, surface, attribute, value);
    ret = glad_eglSurfaceAttrib(dpy, surface, attribute, value);
    _post_call_egl_callback((void*) &ret, "eglSurfaceAttrib", (GLADapiproc) glad_eglSurfaceAttrib, 4, dpy, surface, attribute, value);
    return ret;
}
PFNEGLSURFACEATTRIBPROC glad_debug_eglSurfaceAttrib = glad_debug_impl_eglSurfaceAttrib;
PFNEGLSWAPBUFFERSPROC glad_eglSwapBuffers = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglSwapBuffers", (GLADapiproc) glad_eglSwapBuffers, 2, dpy, surface);
    ret = glad_eglSwapBuffers(dpy, surface);
    _post_call_egl_callback((void*) &ret, "eglSwapBuffers", (GLADapiproc) glad_eglSwapBuffers, 2, dpy, surface);
    return ret;
}
PFNEGLSWAPBUFFERSPROC glad_debug_eglSwapBuffers = glad_debug_impl_eglSwapBuffers;
PFNEGLSWAPBUFFERSREGION2NOKPROC glad_eglSwapBuffersRegion2NOK = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglSwapBuffersRegion2NOK(EGLDisplay dpy, EGLSurface surface, EGLint numRects, const EGLint * rects) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglSwapBuffersRegion2NOK", (GLADapiproc) glad_eglSwapBuffersRegion2NOK, 4, dpy, surface, numRects, rects);
    ret = glad_eglSwapBuffersRegion2NOK(dpy, surface, numRects, rects);
    _post_call_egl_callback((void*) &ret, "eglSwapBuffersRegion2NOK", (GLADapiproc) glad_eglSwapBuffersRegion2NOK, 4, dpy, surface, numRects, rects);
    return ret;
}
PFNEGLSWAPBUFFERSREGION2NOKPROC glad_debug_eglSwapBuffersRegion2NOK = glad_debug_impl_eglSwapBuffersRegion2NOK;
PFNEGLSWAPBUFFERSREGIONNOKPROC glad_eglSwapBuffersRegionNOK = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglSwapBuffersRegionNOK(EGLDisplay dpy, EGLSurface surface, EGLint numRects, const EGLint * rects) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglSwapBuffersRegionNOK", (GLADapiproc) glad_eglSwapBuffersRegionNOK, 4, dpy, surface, numRects, rects);
    ret = glad_eglSwapBuffersRegionNOK(dpy, surface, numRects, rects);
    _post_call_egl_callback((void*) &ret, "eglSwapBuffersRegionNOK", (GLADapiproc) glad_eglSwapBuffersRegionNOK, 4, dpy, surface, numRects, rects);
    return ret;
}
PFNEGLSWAPBUFFERSREGIONNOKPROC glad_debug_eglSwapBuffersRegionNOK = glad_debug_impl_eglSwapBuffersRegionNOK;
PFNEGLSWAPBUFFERSWITHDAMAGEEXTPROC glad_eglSwapBuffersWithDamageEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglSwapBuffersWithDamageEXT(EGLDisplay dpy, EGLSurface surface, const EGLint * rects, EGLint n_rects) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglSwapBuffersWithDamageEXT", (GLADapiproc) glad_eglSwapBuffersWithDamageEXT, 4, dpy, surface, rects, n_rects);
    ret = glad_eglSwapBuffersWithDamageEXT(dpy, surface, rects, n_rects);
    _post_call_egl_callback((void*) &ret, "eglSwapBuffersWithDamageEXT", (GLADapiproc) glad_eglSwapBuffersWithDamageEXT, 4, dpy, surface, rects, n_rects);
    return ret;
}
PFNEGLSWAPBUFFERSWITHDAMAGEEXTPROC glad_debug_eglSwapBuffersWithDamageEXT = glad_debug_impl_eglSwapBuffersWithDamageEXT;
PFNEGLSWAPBUFFERSWITHDAMAGEKHRPROC glad_eglSwapBuffersWithDamageKHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglSwapBuffersWithDamageKHR(EGLDisplay dpy, EGLSurface surface, const EGLint * rects, EGLint n_rects) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglSwapBuffersWithDamageKHR", (GLADapiproc) glad_eglSwapBuffersWithDamageKHR, 4, dpy, surface, rects, n_rects);
    ret = glad_eglSwapBuffersWithDamageKHR(dpy, surface, rects, n_rects);
    _post_call_egl_callback((void*) &ret, "eglSwapBuffersWithDamageKHR", (GLADapiproc) glad_eglSwapBuffersWithDamageKHR, 4, dpy, surface, rects, n_rects);
    return ret;
}
PFNEGLSWAPBUFFERSWITHDAMAGEKHRPROC glad_debug_eglSwapBuffersWithDamageKHR = glad_debug_impl_eglSwapBuffersWithDamageKHR;
PFNEGLSWAPINTERVALPROC glad_eglSwapInterval = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglSwapInterval(EGLDisplay dpy, EGLint interval) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglSwapInterval", (GLADapiproc) glad_eglSwapInterval, 2, dpy, interval);
    ret = glad_eglSwapInterval(dpy, interval);
    _post_call_egl_callback((void*) &ret, "eglSwapInterval", (GLADapiproc) glad_eglSwapInterval, 2, dpy, interval);
    return ret;
}
PFNEGLSWAPINTERVALPROC glad_debug_eglSwapInterval = glad_debug_impl_eglSwapInterval;
PFNEGLTERMINATEPROC glad_eglTerminate = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglTerminate(EGLDisplay dpy) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglTerminate", (GLADapiproc) glad_eglTerminate, 1, dpy);
    ret = glad_eglTerminate(dpy);
    _post_call_egl_callback((void*) &ret, "eglTerminate", (GLADapiproc) glad_eglTerminate, 1, dpy);
    return ret;
}
PFNEGLTERMINATEPROC glad_debug_eglTerminate = glad_debug_impl_eglTerminate;
PFNEGLUNBINDWAYLANDDISPLAYWLPROC glad_eglUnbindWaylandDisplayWL = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglUnbindWaylandDisplayWL(EGLDisplay dpy, struct wl_display * display) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglUnbindWaylandDisplayWL", (GLADapiproc) glad_eglUnbindWaylandDisplayWL, 2, dpy, display);
    ret = glad_eglUnbindWaylandDisplayWL(dpy, display);
    _post_call_egl_callback((void*) &ret, "eglUnbindWaylandDisplayWL", (GLADapiproc) glad_eglUnbindWaylandDisplayWL, 2, dpy, display);
    return ret;
}
PFNEGLUNBINDWAYLANDDISPLAYWLPROC glad_debug_eglUnbindWaylandDisplayWL = glad_debug_impl_eglUnbindWaylandDisplayWL;
PFNEGLUNLOCKSURFACEKHRPROC glad_eglUnlockSurfaceKHR = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglUnlockSurfaceKHR(EGLDisplay dpy, EGLSurface surface) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglUnlockSurfaceKHR", (GLADapiproc) glad_eglUnlockSurfaceKHR, 2, dpy, surface);
    ret = glad_eglUnlockSurfaceKHR(dpy, surface);
    _post_call_egl_callback((void*) &ret, "eglUnlockSurfaceKHR", (GLADapiproc) glad_eglUnlockSurfaceKHR, 2, dpy, surface);
    return ret;
}
PFNEGLUNLOCKSURFACEKHRPROC glad_debug_eglUnlockSurfaceKHR = glad_debug_impl_eglUnlockSurfaceKHR;
PFNEGLUNSIGNALSYNCEXTPROC glad_eglUnsignalSyncEXT = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglUnsignalSyncEXT(EGLDisplay dpy, EGLSync sync, const EGLAttrib * attrib_list) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglUnsignalSyncEXT", (GLADapiproc) glad_eglUnsignalSyncEXT, 3, dpy, sync, attrib_list);
    ret = glad_eglUnsignalSyncEXT(dpy, sync, attrib_list);
    _post_call_egl_callback((void*) &ret, "eglUnsignalSyncEXT", (GLADapiproc) glad_eglUnsignalSyncEXT, 3, dpy, sync, attrib_list);
    return ret;
}
PFNEGLUNSIGNALSYNCEXTPROC glad_debug_eglUnsignalSyncEXT = glad_debug_impl_eglUnsignalSyncEXT;
PFNEGLWAITCLIENTPROC glad_eglWaitClient = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglWaitClient(void) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglWaitClient", (GLADapiproc) glad_eglWaitClient, 0);
    ret = glad_eglWaitClient();
    _post_call_egl_callback((void*) &ret, "eglWaitClient", (GLADapiproc) glad_eglWaitClient, 0);
    return ret;
}
PFNEGLWAITCLIENTPROC glad_debug_eglWaitClient = glad_debug_impl_eglWaitClient;
PFNEGLWAITGLPROC glad_eglWaitGL = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglWaitGL(void) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglWaitGL", (GLADapiproc) glad_eglWaitGL, 0);
    ret = glad_eglWaitGL();
    _post_call_egl_callback((void*) &ret, "eglWaitGL", (GLADapiproc) glad_eglWaitGL, 0);
    return ret;
}
PFNEGLWAITGLPROC glad_debug_eglWaitGL = glad_debug_impl_eglWaitGL;
PFNEGLWAITNATIVEPROC glad_eglWaitNative = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglWaitNative(EGLint engine) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglWaitNative", (GLADapiproc) glad_eglWaitNative, 1, engine);
    ret = glad_eglWaitNative(engine);
    _post_call_egl_callback((void*) &ret, "eglWaitNative", (GLADapiproc) glad_eglWaitNative, 1, engine);
    return ret;
}
PFNEGLWAITNATIVEPROC glad_debug_eglWaitNative = glad_debug_impl_eglWaitNative;
PFNEGLWAITSYNCPROC glad_eglWaitSync = NULL;
static EGLBoolean GLAD_API_PTR glad_debug_impl_eglWaitSync(EGLDisplay dpy, EGLSync sync, EGLint flags) {
    EGLBoolean ret;
    _pre_call_egl_callback("eglWaitSync", (GLADapiproc) glad_eglWaitSync, 3, dpy, sync, flags);
    ret = glad_eglWaitSync(dpy, sync, flags);
    _post_call_egl_callback((void*) &ret, "eglWaitSync", (GLADapiproc) glad_eglWaitSync, 3, dpy, sync, flags);
    return ret;
}
PFNEGLWAITSYNCPROC glad_debug_eglWaitSync = glad_debug_impl_eglWaitSync;
PFNEGLWAITSYNCKHRPROC glad_eglWaitSyncKHR = NULL;
static EGLint GLAD_API_PTR glad_debug_impl_eglWaitSyncKHR(EGLDisplay dpy, EGLSyncKHR sync, EGLint flags) {
    EGLint ret;
    _pre_call_egl_callback("eglWaitSyncKHR", (GLADapiproc) glad_eglWaitSyncKHR, 3, dpy, sync, flags);
    ret = glad_eglWaitSyncKHR(dpy, sync, flags);
    _post_call_egl_callback((void*) &ret, "eglWaitSyncKHR", (GLADapiproc) glad_eglWaitSyncKHR, 3, dpy, sync, flags);
    return ret;
}
PFNEGLWAITSYNCKHRPROC glad_debug_eglWaitSyncKHR = glad_debug_impl_eglWaitSyncKHR;


static void glad_egl_load_EGL_VERSION_1_0( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_VERSION_1_0) return;
    glad_eglChooseConfig = (PFNEGLCHOOSECONFIGPROC) load(userptr, "eglChooseConfig");
    glad_eglCopyBuffers = (PFNEGLCOPYBUFFERSPROC) load(userptr, "eglCopyBuffers");
    glad_eglCreateContext = (PFNEGLCREATECONTEXTPROC) load(userptr, "eglCreateContext");
    glad_eglCreatePbufferSurface = (PFNEGLCREATEPBUFFERSURFACEPROC) load(userptr, "eglCreatePbufferSurface");
    glad_eglCreatePixmapSurface = (PFNEGLCREATEPIXMAPSURFACEPROC) load(userptr, "eglCreatePixmapSurface");
    glad_eglCreateWindowSurface = (PFNEGLCREATEWINDOWSURFACEPROC) load(userptr, "eglCreateWindowSurface");
    glad_eglDestroyContext = (PFNEGLDESTROYCONTEXTPROC) load(userptr, "eglDestroyContext");
    glad_eglDestroySurface = (PFNEGLDESTROYSURFACEPROC) load(userptr, "eglDestroySurface");
    glad_eglGetConfigAttrib = (PFNEGLGETCONFIGATTRIBPROC) load(userptr, "eglGetConfigAttrib");
    glad_eglGetConfigs = (PFNEGLGETCONFIGSPROC) load(userptr, "eglGetConfigs");
    glad_eglGetCurrentDisplay = (PFNEGLGETCURRENTDISPLAYPROC) load(userptr, "eglGetCurrentDisplay");
    glad_eglGetCurrentSurface = (PFNEGLGETCURRENTSURFACEPROC) load(userptr, "eglGetCurrentSurface");
    glad_eglGetDisplay = (PFNEGLGETDISPLAYPROC) load(userptr, "eglGetDisplay");
    glad_eglGetError = (PFNEGLGETERRORPROC) load(userptr, "eglGetError");
    glad_eglGetProcAddress = (PFNEGLGETPROCADDRESSPROC) load(userptr, "eglGetProcAddress");
    glad_eglInitialize = (PFNEGLINITIALIZEPROC) load(userptr, "eglInitialize");
    glad_eglMakeCurrent = (PFNEGLMAKECURRENTPROC) load(userptr, "eglMakeCurrent");
    glad_eglQueryContext = (PFNEGLQUERYCONTEXTPROC) load(userptr, "eglQueryContext");
    glad_eglQueryString = (PFNEGLQUERYSTRINGPROC) load(userptr, "eglQueryString");
    glad_eglQuerySurface = (PFNEGLQUERYSURFACEPROC) load(userptr, "eglQuerySurface");
    glad_eglSwapBuffers = (PFNEGLSWAPBUFFERSPROC) load(userptr, "eglSwapBuffers");
    glad_eglTerminate = (PFNEGLTERMINATEPROC) load(userptr, "eglTerminate");
    glad_eglWaitGL = (PFNEGLWAITGLPROC) load(userptr, "eglWaitGL");
    glad_eglWaitNative = (PFNEGLWAITNATIVEPROC) load(userptr, "eglWaitNative");
}
static void glad_egl_load_EGL_VERSION_1_1( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_VERSION_1_1) return;
    glad_eglBindTexImage = (PFNEGLBINDTEXIMAGEPROC) load(userptr, "eglBindTexImage");
    glad_eglReleaseTexImage = (PFNEGLRELEASETEXIMAGEPROC) load(userptr, "eglReleaseTexImage");
    glad_eglSurfaceAttrib = (PFNEGLSURFACEATTRIBPROC) load(userptr, "eglSurfaceAttrib");
    glad_eglSwapInterval = (PFNEGLSWAPINTERVALPROC) load(userptr, "eglSwapInterval");
}
static void glad_egl_load_EGL_VERSION_1_2( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_VERSION_1_2) return;
    glad_eglBindAPI = (PFNEGLBINDAPIPROC) load(userptr, "eglBindAPI");
    glad_eglCreatePbufferFromClientBuffer = (PFNEGLCREATEPBUFFERFROMCLIENTBUFFERPROC) load(userptr, "eglCreatePbufferFromClientBuffer");
    glad_eglQueryAPI = (PFNEGLQUERYAPIPROC) load(userptr, "eglQueryAPI");
    glad_eglReleaseThread = (PFNEGLRELEASETHREADPROC) load(userptr, "eglReleaseThread");
    glad_eglWaitClient = (PFNEGLWAITCLIENTPROC) load(userptr, "eglWaitClient");
}
static void glad_egl_load_EGL_VERSION_1_4( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_VERSION_1_4) return;
    glad_eglGetCurrentContext = (PFNEGLGETCURRENTCONTEXTPROC) load(userptr, "eglGetCurrentContext");
}
static void glad_egl_load_EGL_VERSION_1_5( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_VERSION_1_5) return;
    glad_eglClientWaitSync = (PFNEGLCLIENTWAITSYNCPROC) load(userptr, "eglClientWaitSync");
    glad_eglCreateImage = (PFNEGLCREATEIMAGEPROC) load(userptr, "eglCreateImage");
    glad_eglCreatePlatformPixmapSurface = (PFNEGLCREATEPLATFORMPIXMAPSURFACEPROC) load(userptr, "eglCreatePlatformPixmapSurface");
    glad_eglCreatePlatformWindowSurface = (PFNEGLCREATEPLATFORMWINDOWSURFACEPROC) load(userptr, "eglCreatePlatformWindowSurface");
    glad_eglCreateSync = (PFNEGLCREATESYNCPROC) load(userptr, "eglCreateSync");
    glad_eglDestroyImage = (PFNEGLDESTROYIMAGEPROC) load(userptr, "eglDestroyImage");
    glad_eglDestroySync = (PFNEGLDESTROYSYNCPROC) load(userptr, "eglDestroySync");
    glad_eglGetPlatformDisplay = (PFNEGLGETPLATFORMDISPLAYPROC) load(userptr, "eglGetPlatformDisplay");
    glad_eglGetSyncAttrib = (PFNEGLGETSYNCATTRIBPROC) load(userptr, "eglGetSyncAttrib");
    glad_eglWaitSync = (PFNEGLWAITSYNCPROC) load(userptr, "eglWaitSync");
}
static void glad_egl_load_EGL_ANDROID_blob_cache( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_ANDROID_blob_cache) return;
    glad_eglSetBlobCacheFuncsANDROID = (PFNEGLSETBLOBCACHEFUNCSANDROIDPROC) load(userptr, "eglSetBlobCacheFuncsANDROID");
}
static void glad_egl_load_EGL_ANDROID_create_native_client_buffer( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_ANDROID_create_native_client_buffer) return;
    glad_eglCreateNativeClientBufferANDROID = (PFNEGLCREATENATIVECLIENTBUFFERANDROIDPROC) load(userptr, "eglCreateNativeClientBufferANDROID");
}
static void glad_egl_load_EGL_ANDROID_get_frame_timestamps( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_ANDROID_get_frame_timestamps) return;
    glad_eglGetCompositorTimingANDROID = (PFNEGLGETCOMPOSITORTIMINGANDROIDPROC) load(userptr, "eglGetCompositorTimingANDROID");
    glad_eglGetCompositorTimingSupportedANDROID = (PFNEGLGETCOMPOSITORTIMINGSUPPORTEDANDROIDPROC) load(userptr, "eglGetCompositorTimingSupportedANDROID");
    glad_eglGetFrameTimestampSupportedANDROID = (PFNEGLGETFRAMETIMESTAMPSUPPORTEDANDROIDPROC) load(userptr, "eglGetFrameTimestampSupportedANDROID");
    glad_eglGetFrameTimestampsANDROID = (PFNEGLGETFRAMETIMESTAMPSANDROIDPROC) load(userptr, "eglGetFrameTimestampsANDROID");
    glad_eglGetNextFrameIdANDROID = (PFNEGLGETNEXTFRAMEIDANDROIDPROC) load(userptr, "eglGetNextFrameIdANDROID");
}
static void glad_egl_load_EGL_ANDROID_get_native_client_buffer( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_ANDROID_get_native_client_buffer) return;
    glad_eglGetNativeClientBufferANDROID = (PFNEGLGETNATIVECLIENTBUFFERANDROIDPROC) load(userptr, "eglGetNativeClientBufferANDROID");
}
static void glad_egl_load_EGL_ANDROID_native_fence_sync( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_ANDROID_native_fence_sync) return;
    glad_eglDupNativeFenceFDANDROID = (PFNEGLDUPNATIVEFENCEFDANDROIDPROC) load(userptr, "eglDupNativeFenceFDANDROID");
}
static void glad_egl_load_EGL_ANDROID_presentation_time( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_ANDROID_presentation_time) return;
    glad_eglPresentationTimeANDROID = (PFNEGLPRESENTATIONTIMEANDROIDPROC) load(userptr, "eglPresentationTimeANDROID");
}
static void glad_egl_load_EGL_ANGLE_query_surface_pointer( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_ANGLE_query_surface_pointer) return;
    glad_eglQuerySurfacePointerANGLE = (PFNEGLQUERYSURFACEPOINTERANGLEPROC) load(userptr, "eglQuerySurfacePointerANGLE");
}
static void glad_egl_load_EGL_ANGLE_sync_control_rate( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_ANGLE_sync_control_rate) return;
    glad_eglGetMscRateANGLE = (PFNEGLGETMSCRATEANGLEPROC) load(userptr, "eglGetMscRateANGLE");
}
static void glad_egl_load_EGL_EXT_client_sync( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_EXT_client_sync) return;
    glad_eglClientSignalSyncEXT = (PFNEGLCLIENTSIGNALSYNCEXTPROC) load(userptr, "eglClientSignalSyncEXT");
}
static void glad_egl_load_EGL_EXT_compositor( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_EXT_compositor) return;
    glad_eglCompositorBindTexWindowEXT = (PFNEGLCOMPOSITORBINDTEXWINDOWEXTPROC) load(userptr, "eglCompositorBindTexWindowEXT");
    glad_eglCompositorSetContextAttributesEXT = (PFNEGLCOMPOSITORSETCONTEXTATTRIBUTESEXTPROC) load(userptr, "eglCompositorSetContextAttributesEXT");
    glad_eglCompositorSetContextListEXT = (PFNEGLCOMPOSITORSETCONTEXTLISTEXTPROC) load(userptr, "eglCompositorSetContextListEXT");
    glad_eglCompositorSetSizeEXT = (PFNEGLCOMPOSITORSETSIZEEXTPROC) load(userptr, "eglCompositorSetSizeEXT");
    glad_eglCompositorSetWindowAttributesEXT = (PFNEGLCOMPOSITORSETWINDOWATTRIBUTESEXTPROC) load(userptr, "eglCompositorSetWindowAttributesEXT");
    glad_eglCompositorSetWindowListEXT = (PFNEGLCOMPOSITORSETWINDOWLISTEXTPROC) load(userptr, "eglCompositorSetWindowListEXT");
    glad_eglCompositorSwapPolicyEXT = (PFNEGLCOMPOSITORSWAPPOLICYEXTPROC) load(userptr, "eglCompositorSwapPolicyEXT");
}
static void glad_egl_load_EGL_EXT_device_base( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_EXT_device_base) return;
    glad_eglQueryDeviceAttribEXT = (PFNEGLQUERYDEVICEATTRIBEXTPROC) load(userptr, "eglQueryDeviceAttribEXT");
    glad_eglQueryDeviceStringEXT = (PFNEGLQUERYDEVICESTRINGEXTPROC) load(userptr, "eglQueryDeviceStringEXT");
    glad_eglQueryDevicesEXT = (PFNEGLQUERYDEVICESEXTPROC) load(userptr, "eglQueryDevicesEXT");
    glad_eglQueryDisplayAttribEXT = (PFNEGLQUERYDISPLAYATTRIBEXTPROC) load(userptr, "eglQueryDisplayAttribEXT");
}
static void glad_egl_load_EGL_EXT_device_enumeration( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_EXT_device_enumeration) return;
    glad_eglQueryDevicesEXT = (PFNEGLQUERYDEVICESEXTPROC) load(userptr, "eglQueryDevicesEXT");
}
static void glad_egl_load_EGL_EXT_device_persistent_id( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_EXT_device_persistent_id) return;
    glad_eglQueryDeviceBinaryEXT = (PFNEGLQUERYDEVICEBINARYEXTPROC) load(userptr, "eglQueryDeviceBinaryEXT");
}
static void glad_egl_load_EGL_EXT_device_query( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_EXT_device_query) return;
    glad_eglQueryDeviceAttribEXT = (PFNEGLQUERYDEVICEATTRIBEXTPROC) load(userptr, "eglQueryDeviceAttribEXT");
    glad_eglQueryDeviceStringEXT = (PFNEGLQUERYDEVICESTRINGEXTPROC) load(userptr, "eglQueryDeviceStringEXT");
    glad_eglQueryDisplayAttribEXT = (PFNEGLQUERYDISPLAYATTRIBEXTPROC) load(userptr, "eglQueryDisplayAttribEXT");
}
static void glad_egl_load_EGL_EXT_display_alloc( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_EXT_display_alloc) return;
    glad_eglDestroyDisplayEXT = (PFNEGLDESTROYDISPLAYEXTPROC) load(userptr, "eglDestroyDisplayEXT");
}
static void glad_egl_load_EGL_EXT_image_dma_buf_import_modifiers( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_EXT_image_dma_buf_import_modifiers) return;
    glad_eglQueryDmaBufFormatsEXT = (PFNEGLQUERYDMABUFFORMATSEXTPROC) load(userptr, "eglQueryDmaBufFormatsEXT");
    glad_eglQueryDmaBufModifiersEXT = (PFNEGLQUERYDMABUFMODIFIERSEXTPROC) load(userptr, "eglQueryDmaBufModifiersEXT");
}
static void glad_egl_load_EGL_EXT_output_base( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_EXT_output_base) return;
    glad_eglGetOutputLayersEXT = (PFNEGLGETOUTPUTLAYERSEXTPROC) load(userptr, "eglGetOutputLayersEXT");
    glad_eglGetOutputPortsEXT = (PFNEGLGETOUTPUTPORTSEXTPROC) load(userptr, "eglGetOutputPortsEXT");
    glad_eglOutputLayerAttribEXT = (PFNEGLOUTPUTLAYERATTRIBEXTPROC) load(userptr, "eglOutputLayerAttribEXT");
    glad_eglOutputPortAttribEXT = (PFNEGLOUTPUTPORTATTRIBEXTPROC) load(userptr, "eglOutputPortAttribEXT");
    glad_eglQueryOutputLayerAttribEXT = (PFNEGLQUERYOUTPUTLAYERATTRIBEXTPROC) load(userptr, "eglQueryOutputLayerAttribEXT");
    glad_eglQueryOutputLayerStringEXT = (PFNEGLQUERYOUTPUTLAYERSTRINGEXTPROC) load(userptr, "eglQueryOutputLayerStringEXT");
    glad_eglQueryOutputPortAttribEXT = (PFNEGLQUERYOUTPUTPORTATTRIBEXTPROC) load(userptr, "eglQueryOutputPortAttribEXT");
    glad_eglQueryOutputPortStringEXT = (PFNEGLQUERYOUTPUTPORTSTRINGEXTPROC) load(userptr, "eglQueryOutputPortStringEXT");
}
static void glad_egl_load_EGL_EXT_platform_base( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_EXT_platform_base) return;
    glad_eglCreatePlatformPixmapSurfaceEXT = (PFNEGLCREATEPLATFORMPIXMAPSURFACEEXTPROC) load(userptr, "eglCreatePlatformPixmapSurfaceEXT");
    glad_eglCreatePlatformWindowSurfaceEXT = (PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC) load(userptr, "eglCreatePlatformWindowSurfaceEXT");
    glad_eglGetPlatformDisplayEXT = (PFNEGLGETPLATFORMDISPLAYEXTPROC) load(userptr, "eglGetPlatformDisplayEXT");
}
static void glad_egl_load_EGL_EXT_stream_consumer_egloutput( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_EXT_stream_consumer_egloutput) return;
    glad_eglStreamConsumerOutputEXT = (PFNEGLSTREAMCONSUMEROUTPUTEXTPROC) load(userptr, "eglStreamConsumerOutputEXT");
}
static void glad_egl_load_EGL_EXT_surface_compression( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_EXT_surface_compression) return;
    glad_eglQuerySupportedCompressionRatesEXT = (PFNEGLQUERYSUPPORTEDCOMPRESSIONRATESEXTPROC) load(userptr, "eglQuerySupportedCompressionRatesEXT");
}
static void glad_egl_load_EGL_EXT_swap_buffers_with_damage( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_EXT_swap_buffers_with_damage) return;
    glad_eglSwapBuffersWithDamageEXT = (PFNEGLSWAPBUFFERSWITHDAMAGEEXTPROC) load(userptr, "eglSwapBuffersWithDamageEXT");
}
static void glad_egl_load_EGL_EXT_sync_reuse( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_EXT_sync_reuse) return;
    glad_eglUnsignalSyncEXT = (PFNEGLUNSIGNALSYNCEXTPROC) load(userptr, "eglUnsignalSyncEXT");
}
static void glad_egl_load_EGL_HI_clientpixmap( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_HI_clientpixmap) return;
    glad_eglCreatePixmapSurfaceHI = (PFNEGLCREATEPIXMAPSURFACEHIPROC) load(userptr, "eglCreatePixmapSurfaceHI");
}
static void glad_egl_load_EGL_KHR_cl_event2( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_KHR_cl_event2) return;
    glad_eglCreateSync64KHR = (PFNEGLCREATESYNC64KHRPROC) load(userptr, "eglCreateSync64KHR");
}
static void glad_egl_load_EGL_KHR_debug( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_KHR_debug) return;
    glad_eglDebugMessageControlKHR = (PFNEGLDEBUGMESSAGECONTROLKHRPROC) load(userptr, "eglDebugMessageControlKHR");
    glad_eglLabelObjectKHR = (PFNEGLLABELOBJECTKHRPROC) load(userptr, "eglLabelObjectKHR");
    glad_eglQueryDebugKHR = (PFNEGLQUERYDEBUGKHRPROC) load(userptr, "eglQueryDebugKHR");
}
static void glad_egl_load_EGL_KHR_display_reference( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_KHR_display_reference) return;
    glad_eglQueryDisplayAttribKHR = (PFNEGLQUERYDISPLAYATTRIBKHRPROC) load(userptr, "eglQueryDisplayAttribKHR");
}
static void glad_egl_load_EGL_KHR_fence_sync( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_KHR_fence_sync) return;
    glad_eglClientWaitSyncKHR = (PFNEGLCLIENTWAITSYNCKHRPROC) load(userptr, "eglClientWaitSyncKHR");
    glad_eglCreateSyncKHR = (PFNEGLCREATESYNCKHRPROC) load(userptr, "eglCreateSyncKHR");
    glad_eglDestroySyncKHR = (PFNEGLDESTROYSYNCKHRPROC) load(userptr, "eglDestroySyncKHR");
    glad_eglGetSyncAttribKHR = (PFNEGLGETSYNCATTRIBKHRPROC) load(userptr, "eglGetSyncAttribKHR");
}
static void glad_egl_load_EGL_KHR_image( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_KHR_image) return;
    glad_eglCreateImageKHR = (PFNEGLCREATEIMAGEKHRPROC) load(userptr, "eglCreateImageKHR");
    glad_eglDestroyImageKHR = (PFNEGLDESTROYIMAGEKHRPROC) load(userptr, "eglDestroyImageKHR");
}
static void glad_egl_load_EGL_KHR_image_base( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_KHR_image_base) return;
    glad_eglCreateImageKHR = (PFNEGLCREATEIMAGEKHRPROC) load(userptr, "eglCreateImageKHR");
    glad_eglDestroyImageKHR = (PFNEGLDESTROYIMAGEKHRPROC) load(userptr, "eglDestroyImageKHR");
}
static void glad_egl_load_EGL_KHR_lock_surface( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_KHR_lock_surface) return;
    glad_eglLockSurfaceKHR = (PFNEGLLOCKSURFACEKHRPROC) load(userptr, "eglLockSurfaceKHR");
    glad_eglUnlockSurfaceKHR = (PFNEGLUNLOCKSURFACEKHRPROC) load(userptr, "eglUnlockSurfaceKHR");
}
static void glad_egl_load_EGL_KHR_lock_surface3( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_KHR_lock_surface3) return;
    glad_eglLockSurfaceKHR = (PFNEGLLOCKSURFACEKHRPROC) load(userptr, "eglLockSurfaceKHR");
    glad_eglQuerySurface64KHR = (PFNEGLQUERYSURFACE64KHRPROC) load(userptr, "eglQuerySurface64KHR");
    glad_eglUnlockSurfaceKHR = (PFNEGLUNLOCKSURFACEKHRPROC) load(userptr, "eglUnlockSurfaceKHR");
}
static void glad_egl_load_EGL_KHR_partial_update( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_KHR_partial_update) return;
    glad_eglSetDamageRegionKHR = (PFNEGLSETDAMAGEREGIONKHRPROC) load(userptr, "eglSetDamageRegionKHR");
}
static void glad_egl_load_EGL_KHR_reusable_sync( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_KHR_reusable_sync) return;
    glad_eglClientWaitSyncKHR = (PFNEGLCLIENTWAITSYNCKHRPROC) load(userptr, "eglClientWaitSyncKHR");
    glad_eglCreateSyncKHR = (PFNEGLCREATESYNCKHRPROC) load(userptr, "eglCreateSyncKHR");
    glad_eglDestroySyncKHR = (PFNEGLDESTROYSYNCKHRPROC) load(userptr, "eglDestroySyncKHR");
    glad_eglGetSyncAttribKHR = (PFNEGLGETSYNCATTRIBKHRPROC) load(userptr, "eglGetSyncAttribKHR");
    glad_eglSignalSyncKHR = (PFNEGLSIGNALSYNCKHRPROC) load(userptr, "eglSignalSyncKHR");
}
static void glad_egl_load_EGL_KHR_stream( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_KHR_stream) return;
    glad_eglCreateStreamKHR = (PFNEGLCREATESTREAMKHRPROC) load(userptr, "eglCreateStreamKHR");
    glad_eglDestroyStreamKHR = (PFNEGLDESTROYSTREAMKHRPROC) load(userptr, "eglDestroyStreamKHR");
    glad_eglQueryStreamKHR = (PFNEGLQUERYSTREAMKHRPROC) load(userptr, "eglQueryStreamKHR");
    glad_eglQueryStreamu64KHR = (PFNEGLQUERYSTREAMU64KHRPROC) load(userptr, "eglQueryStreamu64KHR");
    glad_eglStreamAttribKHR = (PFNEGLSTREAMATTRIBKHRPROC) load(userptr, "eglStreamAttribKHR");
}
static void glad_egl_load_EGL_KHR_stream_attrib( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_KHR_stream_attrib) return;
    glad_eglCreateStreamAttribKHR = (PFNEGLCREATESTREAMATTRIBKHRPROC) load(userptr, "eglCreateStreamAttribKHR");
    glad_eglQueryStreamAttribKHR = (PFNEGLQUERYSTREAMATTRIBKHRPROC) load(userptr, "eglQueryStreamAttribKHR");
    glad_eglSetStreamAttribKHR = (PFNEGLSETSTREAMATTRIBKHRPROC) load(userptr, "eglSetStreamAttribKHR");
    glad_eglStreamConsumerAcquireAttribKHR = (PFNEGLSTREAMCONSUMERACQUIREATTRIBKHRPROC) load(userptr, "eglStreamConsumerAcquireAttribKHR");
    glad_eglStreamConsumerReleaseAttribKHR = (PFNEGLSTREAMCONSUMERRELEASEATTRIBKHRPROC) load(userptr, "eglStreamConsumerReleaseAttribKHR");
}
static void glad_egl_load_EGL_KHR_stream_consumer_gltexture( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_KHR_stream_consumer_gltexture) return;
    glad_eglStreamConsumerAcquireKHR = (PFNEGLSTREAMCONSUMERACQUIREKHRPROC) load(userptr, "eglStreamConsumerAcquireKHR");
    glad_eglStreamConsumerGLTextureExternalKHR = (PFNEGLSTREAMCONSUMERGLTEXTUREEXTERNALKHRPROC) load(userptr, "eglStreamConsumerGLTextureExternalKHR");
    glad_eglStreamConsumerReleaseKHR = (PFNEGLSTREAMCONSUMERRELEASEKHRPROC) load(userptr, "eglStreamConsumerReleaseKHR");
}
static void glad_egl_load_EGL_KHR_stream_cross_process_fd( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_KHR_stream_cross_process_fd) return;
    glad_eglCreateStreamFromFileDescriptorKHR = (PFNEGLCREATESTREAMFROMFILEDESCRIPTORKHRPROC) load(userptr, "eglCreateStreamFromFileDescriptorKHR");
    glad_eglGetStreamFileDescriptorKHR = (PFNEGLGETSTREAMFILEDESCRIPTORKHRPROC) load(userptr, "eglGetStreamFileDescriptorKHR");
}
static void glad_egl_load_EGL_KHR_stream_fifo( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_KHR_stream_fifo) return;
    glad_eglQueryStreamTimeKHR = (PFNEGLQUERYSTREAMTIMEKHRPROC) load(userptr, "eglQueryStreamTimeKHR");
}
static void glad_egl_load_EGL_KHR_stream_producer_eglsurface( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_KHR_stream_producer_eglsurface) return;
    glad_eglCreateStreamProducerSurfaceKHR = (PFNEGLCREATESTREAMPRODUCERSURFACEKHRPROC) load(userptr, "eglCreateStreamProducerSurfaceKHR");
}
static void glad_egl_load_EGL_KHR_swap_buffers_with_damage( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_KHR_swap_buffers_with_damage) return;
    glad_eglSwapBuffersWithDamageKHR = (PFNEGLSWAPBUFFERSWITHDAMAGEKHRPROC) load(userptr, "eglSwapBuffersWithDamageKHR");
}
static void glad_egl_load_EGL_KHR_wait_sync( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_KHR_wait_sync) return;
    glad_eglWaitSyncKHR = (PFNEGLWAITSYNCKHRPROC) load(userptr, "eglWaitSyncKHR");
}
static void glad_egl_load_EGL_MESA_drm_image( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_MESA_drm_image) return;
    glad_eglCreateDRMImageMESA = (PFNEGLCREATEDRMIMAGEMESAPROC) load(userptr, "eglCreateDRMImageMESA");
    glad_eglExportDRMImageMESA = (PFNEGLEXPORTDRMIMAGEMESAPROC) load(userptr, "eglExportDRMImageMESA");
}
static void glad_egl_load_EGL_MESA_image_dma_buf_export( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_MESA_image_dma_buf_export) return;
    glad_eglExportDMABUFImageMESA = (PFNEGLEXPORTDMABUFIMAGEMESAPROC) load(userptr, "eglExportDMABUFImageMESA");
    glad_eglExportDMABUFImageQueryMESA = (PFNEGLEXPORTDMABUFIMAGEQUERYMESAPROC) load(userptr, "eglExportDMABUFImageQueryMESA");
}
static void glad_egl_load_EGL_MESA_query_driver( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_MESA_query_driver) return;
    glad_eglGetDisplayDriverConfig = (PFNEGLGETDISPLAYDRIVERCONFIGPROC) load(userptr, "eglGetDisplayDriverConfig");
    glad_eglGetDisplayDriverName = (PFNEGLGETDISPLAYDRIVERNAMEPROC) load(userptr, "eglGetDisplayDriverName");
}
static void glad_egl_load_EGL_NOK_swap_region( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_NOK_swap_region) return;
    glad_eglSwapBuffersRegionNOK = (PFNEGLSWAPBUFFERSREGIONNOKPROC) load(userptr, "eglSwapBuffersRegionNOK");
}
static void glad_egl_load_EGL_NOK_swap_region2( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_NOK_swap_region2) return;
    glad_eglSwapBuffersRegion2NOK = (PFNEGLSWAPBUFFERSREGION2NOKPROC) load(userptr, "eglSwapBuffersRegion2NOK");
}
static void glad_egl_load_EGL_NV_native_query( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_NV_native_query) return;
    glad_eglQueryNativeDisplayNV = (PFNEGLQUERYNATIVEDISPLAYNVPROC) load(userptr, "eglQueryNativeDisplayNV");
    glad_eglQueryNativePixmapNV = (PFNEGLQUERYNATIVEPIXMAPNVPROC) load(userptr, "eglQueryNativePixmapNV");
    glad_eglQueryNativeWindowNV = (PFNEGLQUERYNATIVEWINDOWNVPROC) load(userptr, "eglQueryNativeWindowNV");
}
static void glad_egl_load_EGL_NV_post_sub_buffer( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_NV_post_sub_buffer) return;
    glad_eglPostSubBufferNV = (PFNEGLPOSTSUBBUFFERNVPROC) load(userptr, "eglPostSubBufferNV");
}
static void glad_egl_load_EGL_NV_stream_consumer_eglimage( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_NV_stream_consumer_eglimage) return;
    glad_eglQueryStreamConsumerEventNV = (PFNEGLQUERYSTREAMCONSUMEREVENTNVPROC) load(userptr, "eglQueryStreamConsumerEventNV");
    glad_eglStreamAcquireImageNV = (PFNEGLSTREAMACQUIREIMAGENVPROC) load(userptr, "eglStreamAcquireImageNV");
    glad_eglStreamImageConsumerConnectNV = (PFNEGLSTREAMIMAGECONSUMERCONNECTNVPROC) load(userptr, "eglStreamImageConsumerConnectNV");
    glad_eglStreamReleaseImageNV = (PFNEGLSTREAMRELEASEIMAGENVPROC) load(userptr, "eglStreamReleaseImageNV");
}
static void glad_egl_load_EGL_NV_stream_consumer_gltexture_yuv( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_NV_stream_consumer_gltexture_yuv) return;
    glad_eglStreamConsumerGLTextureExternalAttribsNV = (PFNEGLSTREAMCONSUMERGLTEXTUREEXTERNALATTRIBSNVPROC) load(userptr, "eglStreamConsumerGLTextureExternalAttribsNV");
}
static void glad_egl_load_EGL_NV_stream_flush( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_NV_stream_flush) return;
    glad_eglStreamFlushNV = (PFNEGLSTREAMFLUSHNVPROC) load(userptr, "eglStreamFlushNV");
}
static void glad_egl_load_EGL_NV_stream_metadata( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_NV_stream_metadata) return;
    glad_eglQueryDisplayAttribNV = (PFNEGLQUERYDISPLAYATTRIBNVPROC) load(userptr, "eglQueryDisplayAttribNV");
    glad_eglQueryStreamMetadataNV = (PFNEGLQUERYSTREAMMETADATANVPROC) load(userptr, "eglQueryStreamMetadataNV");
    glad_eglSetStreamMetadataNV = (PFNEGLSETSTREAMMETADATANVPROC) load(userptr, "eglSetStreamMetadataNV");
}
static void glad_egl_load_EGL_NV_stream_reset( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_NV_stream_reset) return;
    glad_eglResetStreamNV = (PFNEGLRESETSTREAMNVPROC) load(userptr, "eglResetStreamNV");
}
static void glad_egl_load_EGL_NV_stream_sync( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_NV_stream_sync) return;
    glad_eglCreateStreamSyncNV = (PFNEGLCREATESTREAMSYNCNVPROC) load(userptr, "eglCreateStreamSyncNV");
}
static void glad_egl_load_EGL_NV_sync( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_NV_sync) return;
    glad_eglClientWaitSyncNV = (PFNEGLCLIENTWAITSYNCNVPROC) load(userptr, "eglClientWaitSyncNV");
    glad_eglCreateFenceSyncNV = (PFNEGLCREATEFENCESYNCNVPROC) load(userptr, "eglCreateFenceSyncNV");
    glad_eglDestroySyncNV = (PFNEGLDESTROYSYNCNVPROC) load(userptr, "eglDestroySyncNV");
    glad_eglFenceNV = (PFNEGLFENCENVPROC) load(userptr, "eglFenceNV");
    glad_eglGetSyncAttribNV = (PFNEGLGETSYNCATTRIBNVPROC) load(userptr, "eglGetSyncAttribNV");
    glad_eglSignalSyncNV = (PFNEGLSIGNALSYNCNVPROC) load(userptr, "eglSignalSyncNV");
}
static void glad_egl_load_EGL_NV_system_time( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_NV_system_time) return;
    glad_eglGetSystemTimeFrequencyNV = (PFNEGLGETSYSTEMTIMEFREQUENCYNVPROC) load(userptr, "eglGetSystemTimeFrequencyNV");
    glad_eglGetSystemTimeNV = (PFNEGLGETSYSTEMTIMENVPROC) load(userptr, "eglGetSystemTimeNV");
}
static void glad_egl_load_EGL_WL_bind_wayland_display( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_WL_bind_wayland_display) return;
    glad_eglBindWaylandDisplayWL = (PFNEGLBINDWAYLANDDISPLAYWLPROC) load(userptr, "eglBindWaylandDisplayWL");
    glad_eglQueryWaylandBufferWL = (PFNEGLQUERYWAYLANDBUFFERWLPROC) load(userptr, "eglQueryWaylandBufferWL");
    glad_eglUnbindWaylandDisplayWL = (PFNEGLUNBINDWAYLANDDISPLAYWLPROC) load(userptr, "eglUnbindWaylandDisplayWL");
}
static void glad_egl_load_EGL_WL_create_wayland_buffer_from_image( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_EGL_WL_create_wayland_buffer_from_image) return;
    glad_eglCreateWaylandBufferFromImageWL = (PFNEGLCREATEWAYLANDBUFFERFROMIMAGEWLPROC) load(userptr, "eglCreateWaylandBufferFromImageWL");
}


static void glad_egl_resolve_aliases(void) {
    if (glad_eglClientWaitSync == NULL && glad_eglClientWaitSyncKHR != NULL) glad_eglClientWaitSync = (PFNEGLCLIENTWAITSYNCPROC)glad_eglClientWaitSyncKHR;
    if (glad_eglClientWaitSyncKHR == NULL && glad_eglClientWaitSync != NULL) glad_eglClientWaitSyncKHR = (PFNEGLCLIENTWAITSYNCKHRPROC)glad_eglClientWaitSync;
    if (glad_eglCreateSync == NULL && glad_eglCreateSync64KHR != NULL) glad_eglCreateSync = (PFNEGLCREATESYNCPROC)glad_eglCreateSync64KHR;
    if (glad_eglCreateSync64KHR == NULL && glad_eglCreateSync != NULL) glad_eglCreateSync64KHR = (PFNEGLCREATESYNC64KHRPROC)glad_eglCreateSync;
    if (glad_eglDestroyImage == NULL && glad_eglDestroyImageKHR != NULL) glad_eglDestroyImage = (PFNEGLDESTROYIMAGEPROC)glad_eglDestroyImageKHR;
    if (glad_eglDestroyImageKHR == NULL && glad_eglDestroyImage != NULL) glad_eglDestroyImageKHR = (PFNEGLDESTROYIMAGEKHRPROC)glad_eglDestroyImage;
    if (glad_eglDestroySync == NULL && glad_eglDestroySyncKHR != NULL) glad_eglDestroySync = (PFNEGLDESTROYSYNCPROC)glad_eglDestroySyncKHR;
    if (glad_eglDestroySyncKHR == NULL && glad_eglDestroySync != NULL) glad_eglDestroySyncKHR = (PFNEGLDESTROYSYNCKHRPROC)glad_eglDestroySync;
    if (glad_eglQueryDisplayAttribEXT == NULL && glad_eglQueryDisplayAttribKHR != NULL) glad_eglQueryDisplayAttribEXT = (PFNEGLQUERYDISPLAYATTRIBEXTPROC)glad_eglQueryDisplayAttribKHR;
    if (glad_eglQueryDisplayAttribEXT == NULL && glad_eglQueryDisplayAttribNV != NULL) glad_eglQueryDisplayAttribEXT = (PFNEGLQUERYDISPLAYATTRIBEXTPROC)glad_eglQueryDisplayAttribNV;
    if (glad_eglQueryDisplayAttribKHR == NULL && glad_eglQueryDisplayAttribEXT != NULL) glad_eglQueryDisplayAttribKHR = (PFNEGLQUERYDISPLAYATTRIBKHRPROC)glad_eglQueryDisplayAttribEXT;
    if (glad_eglQueryDisplayAttribKHR == NULL && glad_eglQueryDisplayAttribNV != NULL) glad_eglQueryDisplayAttribKHR = (PFNEGLQUERYDISPLAYATTRIBKHRPROC)glad_eglQueryDisplayAttribNV;
    if (glad_eglQueryDisplayAttribNV == NULL && glad_eglQueryDisplayAttribEXT != NULL) glad_eglQueryDisplayAttribNV = (PFNEGLQUERYDISPLAYATTRIBNVPROC)glad_eglQueryDisplayAttribEXT;
    if (glad_eglQueryDisplayAttribNV == NULL && glad_eglQueryDisplayAttribKHR != NULL) glad_eglQueryDisplayAttribNV = (PFNEGLQUERYDISPLAYATTRIBNVPROC)glad_eglQueryDisplayAttribKHR;
}

static int glad_egl_get_extensions(EGLDisplay display, const char **extensions) {
    *extensions = eglQueryString(display, EGL_EXTENSIONS);

    return extensions != NULL;
}

static int glad_egl_has_extension(const char *extensions, const char *ext) {
    const char *loc;
    const char *terminator;
    if(extensions == NULL) {
        return 0;
    }
    while(1) {
        loc = strstr(extensions, ext);
        if(loc == NULL) {
            return 0;
        }
        terminator = loc + strlen(ext);
        if((loc == extensions || *(loc - 1) == ' ') &&
            (*terminator == ' ' || *terminator == '\0')) {
            return 1;
        }
        extensions = terminator;
    }
}

static GLADapiproc glad_egl_get_proc_from_userptr(void *userptr, const char *name) {
    return (GLAD_GNUC_EXTENSION (GLADapiproc (*)(const char *name)) userptr)(name);
}

static int glad_egl_find_extensions_egl(EGLDisplay display) {
    const char *extensions;
    if (!glad_egl_get_extensions(display, &extensions)) return 0;

    GLAD_EGL_ANDROID_GLES_layers = glad_egl_has_extension(extensions, "EGL_ANDROID_GLES_layers");
    GLAD_EGL_ANDROID_blob_cache = glad_egl_has_extension(extensions, "EGL_ANDROID_blob_cache");
    GLAD_EGL_ANDROID_create_native_client_buffer = glad_egl_has_extension(extensions, "EGL_ANDROID_create_native_client_buffer");
    GLAD_EGL_ANDROID_framebuffer_target = glad_egl_has_extension(extensions, "EGL_ANDROID_framebuffer_target");
    GLAD_EGL_ANDROID_front_buffer_auto_refresh = glad_egl_has_extension(extensions, "EGL_ANDROID_front_buffer_auto_refresh");
    GLAD_EGL_ANDROID_get_frame_timestamps = glad_egl_has_extension(extensions, "EGL_ANDROID_get_frame_timestamps");
    GLAD_EGL_ANDROID_get_native_client_buffer = glad_egl_has_extension(extensions, "EGL_ANDROID_get_native_client_buffer");
    GLAD_EGL_ANDROID_image_native_buffer = glad_egl_has_extension(extensions, "EGL_ANDROID_image_native_buffer");
    GLAD_EGL_ANDROID_native_fence_sync = glad_egl_has_extension(extensions, "EGL_ANDROID_native_fence_sync");
    GLAD_EGL_ANDROID_presentation_time = glad_egl_has_extension(extensions, "EGL_ANDROID_presentation_time");
    GLAD_EGL_ANDROID_recordable = glad_egl_has_extension(extensions, "EGL_ANDROID_recordable");
    GLAD_EGL_ANDROID_telemetry_hint = glad_egl_has_extension(extensions, "EGL_ANDROID_telemetry_hint");
    GLAD_EGL_ANGLE_d3d_share_handle_client_buffer = glad_egl_has_extension(extensions, "EGL_ANGLE_d3d_share_handle_client_buffer");
    GLAD_EGL_ANGLE_device_d3d = glad_egl_has_extension(extensions, "EGL_ANGLE_device_d3d");
    GLAD_EGL_ANGLE_query_surface_pointer = glad_egl_has_extension(extensions, "EGL_ANGLE_query_surface_pointer");
    GLAD_EGL_ANGLE_surface_d3d_texture_2d_share_handle = glad_egl_has_extension(extensions, "EGL_ANGLE_surface_d3d_texture_2d_share_handle");
    GLAD_EGL_ANGLE_sync_control_rate = glad_egl_has_extension(extensions, "EGL_ANGLE_sync_control_rate");
    GLAD_EGL_ANGLE_window_fixed_size = glad_egl_has_extension(extensions, "EGL_ANGLE_window_fixed_size");
    GLAD_EGL_ARM_image_format = glad_egl_has_extension(extensions, "EGL_ARM_image_format");
    GLAD_EGL_ARM_implicit_external_sync = glad_egl_has_extension(extensions, "EGL_ARM_implicit_external_sync");
    GLAD_EGL_ARM_pixmap_multisample_discard = glad_egl_has_extension(extensions, "EGL_ARM_pixmap_multisample_discard");
    GLAD_EGL_EXT_bind_to_front = glad_egl_has_extension(extensions, "EGL_EXT_bind_to_front");
    GLAD_EGL_EXT_buffer_age = glad_egl_has_extension(extensions, "EGL_EXT_buffer_age");
    GLAD_EGL_EXT_client_extensions = glad_egl_has_extension(extensions, "EGL_EXT_client_extensions");
    GLAD_EGL_EXT_client_sync = glad_egl_has_extension(extensions, "EGL_EXT_client_sync");
    GLAD_EGL_EXT_compositor = glad_egl_has_extension(extensions, "EGL_EXT_compositor");
    GLAD_EGL_EXT_config_select_group = glad_egl_has_extension(extensions, "EGL_EXT_config_select_group");
    GLAD_EGL_EXT_create_context_robustness = glad_egl_has_extension(extensions, "EGL_EXT_create_context_robustness");
    GLAD_EGL_EXT_device_base = glad_egl_has_extension(extensions, "EGL_EXT_device_base");
    GLAD_EGL_EXT_device_drm = glad_egl_has_extension(extensions, "EGL_EXT_device_drm");
    GLAD_EGL_EXT_device_drm_render_node = glad_egl_has_extension(extensions, "EGL_EXT_device_drm_render_node");
    GLAD_EGL_EXT_device_enumeration = glad_egl_has_extension(extensions, "EGL_EXT_device_enumeration");
    GLAD_EGL_EXT_device_openwf = glad_egl_has_extension(extensions, "EGL_EXT_device_openwf");
    GLAD_EGL_EXT_device_persistent_id = glad_egl_has_extension(extensions, "EGL_EXT_device_persistent_id");
    GLAD_EGL_EXT_device_query = glad_egl_has_extension(extensions, "EGL_EXT_device_query");
    GLAD_EGL_EXT_device_query_name = glad_egl_has_extension(extensions, "EGL_EXT_device_query_name");
    GLAD_EGL_EXT_device_type = glad_egl_has_extension(extensions, "EGL_EXT_device_type");
    GLAD_EGL_EXT_display_alloc = glad_egl_has_extension(extensions, "EGL_EXT_display_alloc");
    GLAD_EGL_EXT_explicit_device = glad_egl_has_extension(extensions, "EGL_EXT_explicit_device");
    GLAD_EGL_EXT_gl_colorspace_bt2020_hlg = glad_egl_has_extension(extensions, "EGL_EXT_gl_colorspace_bt2020_hlg");
    GLAD_EGL_EXT_gl_colorspace_bt2020_linear = glad_egl_has_extension(extensions, "EGL_EXT_gl_colorspace_bt2020_linear");
    GLAD_EGL_EXT_gl_colorspace_bt2020_pq = glad_egl_has_extension(extensions, "EGL_EXT_gl_colorspace_bt2020_pq");
    GLAD_EGL_EXT_gl_colorspace_display_p3 = glad_egl_has_extension(extensions, "EGL_EXT_gl_colorspace_display_p3");
    GLAD_EGL_EXT_gl_colorspace_display_p3_linear = glad_egl_has_extension(extensions, "EGL_EXT_gl_colorspace_display_p3_linear");
    GLAD_EGL_EXT_gl_colorspace_display_p3_passthrough = glad_egl_has_extension(extensions, "EGL_EXT_gl_colorspace_display_p3_passthrough");
    GLAD_EGL_EXT_gl_colorspace_scrgb = glad_egl_has_extension(extensions, "EGL_EXT_gl_colorspace_scrgb");
    GLAD_EGL_EXT_gl_colorspace_scrgb_linear = glad_egl_has_extension(extensions, "EGL_EXT_gl_colorspace_scrgb_linear");
    GLAD_EGL_EXT_image_dma_buf_import = glad_egl_has_extension(extensions, "EGL_EXT_image_dma_buf_import");
    GLAD_EGL_EXT_image_dma_buf_import_modifiers = glad_egl_has_extension(extensions, "EGL_EXT_image_dma_buf_import_modifiers");
    GLAD_EGL_EXT_image_gl_colorspace = glad_egl_has_extension(extensions, "EGL_EXT_image_gl_colorspace");
    GLAD_EGL_EXT_image_implicit_sync_control = glad_egl_has_extension(extensions, "EGL_EXT_image_implicit_sync_control");
    GLAD_EGL_EXT_multiview_window = glad_egl_has_extension(extensions, "EGL_EXT_multiview_window");
    GLAD_EGL_EXT_output_base = glad_egl_has_extension(extensions, "EGL_EXT_output_base");
    GLAD_EGL_EXT_output_drm = glad_egl_has_extension(extensions, "EGL_EXT_output_drm");
    GLAD_EGL_EXT_output_openwf = glad_egl_has_extension(extensions, "EGL_EXT_output_openwf");
    GLAD_EGL_EXT_pixel_format_float = glad_egl_has_extension(extensions, "EGL_EXT_pixel_format_float");
    GLAD_EGL_EXT_platform_base = glad_egl_has_extension(extensions, "EGL_EXT_platform_base");
    GLAD_EGL_EXT_platform_device = glad_egl_has_extension(extensions, "EGL_EXT_platform_device");
    GLAD_EGL_EXT_platform_wayland = glad_egl_has_extension(extensions, "EGL_EXT_platform_wayland");
    GLAD_EGL_EXT_platform_x11 = glad_egl_has_extension(extensions, "EGL_EXT_platform_x11");
    GLAD_EGL_EXT_platform_xcb = glad_egl_has_extension(extensions, "EGL_EXT_platform_xcb");
    GLAD_EGL_EXT_present_opaque = glad_egl_has_extension(extensions, "EGL_EXT_present_opaque");
    GLAD_EGL_EXT_protected_content = glad_egl_has_extension(extensions, "EGL_EXT_protected_content");
    GLAD_EGL_EXT_protected_surface = glad_egl_has_extension(extensions, "EGL_EXT_protected_surface");
    GLAD_EGL_EXT_query_reset_notification_strategy = glad_egl_has_extension(extensions, "EGL_EXT_query_reset_notification_strategy");
    GLAD_EGL_EXT_stream_consumer_egloutput = glad_egl_has_extension(extensions, "EGL_EXT_stream_consumer_egloutput");
    GLAD_EGL_EXT_surface_CTA861_3_metadata = glad_egl_has_extension(extensions, "EGL_EXT_surface_CTA861_3_metadata");
    GLAD_EGL_EXT_surface_SMPTE2086_metadata = glad_egl_has_extension(extensions, "EGL_EXT_surface_SMPTE2086_metadata");
    GLAD_EGL_EXT_surface_compression = glad_egl_has_extension(extensions, "EGL_EXT_surface_compression");
    GLAD_EGL_EXT_swap_buffers_with_damage = glad_egl_has_extension(extensions, "EGL_EXT_swap_buffers_with_damage");
    GLAD_EGL_EXT_sync_reuse = glad_egl_has_extension(extensions, "EGL_EXT_sync_reuse");
    GLAD_EGL_EXT_yuv_surface = glad_egl_has_extension(extensions, "EGL_EXT_yuv_surface");
    GLAD_EGL_HI_clientpixmap = glad_egl_has_extension(extensions, "EGL_HI_clientpixmap");
    GLAD_EGL_HI_colorformats = glad_egl_has_extension(extensions, "EGL_HI_colorformats");
    GLAD_EGL_IMG_context_priority = glad_egl_has_extension(extensions, "EGL_IMG_context_priority");
    GLAD_EGL_IMG_image_plane_attribs = glad_egl_has_extension(extensions, "EGL_IMG_image_plane_attribs");
    GLAD_EGL_KHR_cl_event = glad_egl_has_extension(extensions, "EGL_KHR_cl_event");
    GLAD_EGL_KHR_cl_event2 = glad_egl_has_extension(extensions, "EGL_KHR_cl_event2");
    GLAD_EGL_KHR_client_get_all_proc_addresses = glad_egl_has_extension(extensions, "EGL_KHR_client_get_all_proc_addresses");
    GLAD_EGL_KHR_config_attribs = glad_egl_has_extension(extensions, "EGL_KHR_config_attribs");
    GLAD_EGL_KHR_context_flush_control = glad_egl_has_extension(extensions, "EGL_KHR_context_flush_control");
    GLAD_EGL_KHR_create_context = glad_egl_has_extension(extensions, "EGL_KHR_create_context");
    GLAD_EGL_KHR_create_context_no_error = glad_egl_has_extension(extensions, "EGL_KHR_create_context_no_error");
    GLAD_EGL_KHR_debug = glad_egl_has_extension(extensions, "EGL_KHR_debug");
    GLAD_EGL_KHR_display_reference = glad_egl_has_extension(extensions, "EGL_KHR_display_reference");
    GLAD_EGL_KHR_fence_sync = glad_egl_has_extension(extensions, "EGL_KHR_fence_sync");
    GLAD_EGL_KHR_get_all_proc_addresses = glad_egl_has_extension(extensions, "EGL_KHR_get_all_proc_addresses");
    GLAD_EGL_KHR_gl_colorspace = glad_egl_has_extension(extensions, "EGL_KHR_gl_colorspace");
    GLAD_EGL_KHR_gl_renderbuffer_image = glad_egl_has_extension(extensions, "EGL_KHR_gl_renderbuffer_image");
    GLAD_EGL_KHR_gl_texture_2D_image = glad_egl_has_extension(extensions, "EGL_KHR_gl_texture_2D_image");
    GLAD_EGL_KHR_gl_texture_3D_image = glad_egl_has_extension(extensions, "EGL_KHR_gl_texture_3D_image");
    GLAD_EGL_KHR_gl_texture_cubemap_image = glad_egl_has_extension(extensions, "EGL_KHR_gl_texture_cubemap_image");
    GLAD_EGL_KHR_image = glad_egl_has_extension(extensions, "EGL_KHR_image");
    GLAD_EGL_KHR_image_base = glad_egl_has_extension(extensions, "EGL_KHR_image_base");
    GLAD_EGL_KHR_image_pixmap = glad_egl_has_extension(extensions, "EGL_KHR_image_pixmap");
    GLAD_EGL_KHR_lock_surface = glad_egl_has_extension(extensions, "EGL_KHR_lock_surface");
    GLAD_EGL_KHR_lock_surface2 = glad_egl_has_extension(extensions, "EGL_KHR_lock_surface2");
    GLAD_EGL_KHR_lock_surface3 = glad_egl_has_extension(extensions, "EGL_KHR_lock_surface3");
    GLAD_EGL_KHR_mutable_render_buffer = glad_egl_has_extension(extensions, "EGL_KHR_mutable_render_buffer");
    GLAD_EGL_KHR_no_config_context = glad_egl_has_extension(extensions, "EGL_KHR_no_config_context");
    GLAD_EGL_KHR_partial_update = glad_egl_has_extension(extensions, "EGL_KHR_partial_update");
    GLAD_EGL_KHR_platform_android = glad_egl_has_extension(extensions, "EGL_KHR_platform_android");
    GLAD_EGL_KHR_platform_gbm = glad_egl_has_extension(extensions, "EGL_KHR_platform_gbm");
    GLAD_EGL_KHR_platform_wayland = glad_egl_has_extension(extensions, "EGL_KHR_platform_wayland");
    GLAD_EGL_KHR_platform_x11 = glad_egl_has_extension(extensions, "EGL_KHR_platform_x11");
    GLAD_EGL_KHR_reusable_sync = glad_egl_has_extension(extensions, "EGL_KHR_reusable_sync");
    GLAD_EGL_KHR_stream = glad_egl_has_extension(extensions, "EGL_KHR_stream");
    GLAD_EGL_KHR_stream_attrib = glad_egl_has_extension(extensions, "EGL_KHR_stream_attrib");
    GLAD_EGL_KHR_stream_consumer_gltexture = glad_egl_has_extension(extensions, "EGL_KHR_stream_consumer_gltexture");
    GLAD_EGL_KHR_stream_cross_process_fd = glad_egl_has_extension(extensions, "EGL_KHR_stream_cross_process_fd");
    GLAD_EGL_KHR_stream_fifo = glad_egl_has_extension(extensions, "EGL_KHR_stream_fifo");
    GLAD_EGL_KHR_stream_producer_aldatalocator = glad_egl_has_extension(extensions, "EGL_KHR_stream_producer_aldatalocator");
    GLAD_EGL_KHR_stream_producer_eglsurface = glad_egl_has_extension(extensions, "EGL_KHR_stream_producer_eglsurface");
    GLAD_EGL_KHR_surfaceless_context = glad_egl_has_extension(extensions, "EGL_KHR_surfaceless_context");
    GLAD_EGL_KHR_swap_buffers_with_damage = glad_egl_has_extension(extensions, "EGL_KHR_swap_buffers_with_damage");
    GLAD_EGL_KHR_vg_parent_image = glad_egl_has_extension(extensions, "EGL_KHR_vg_parent_image");
    GLAD_EGL_KHR_wait_sync = glad_egl_has_extension(extensions, "EGL_KHR_wait_sync");
    GLAD_EGL_MESA_drm_image = glad_egl_has_extension(extensions, "EGL_MESA_drm_image");
    GLAD_EGL_MESA_image_dma_buf_export = glad_egl_has_extension(extensions, "EGL_MESA_image_dma_buf_export");
    GLAD_EGL_MESA_platform_gbm = glad_egl_has_extension(extensions, "EGL_MESA_platform_gbm");
    GLAD_EGL_MESA_platform_surfaceless = glad_egl_has_extension(extensions, "EGL_MESA_platform_surfaceless");
    GLAD_EGL_MESA_query_driver = glad_egl_has_extension(extensions, "EGL_MESA_query_driver");
    GLAD_EGL_NOK_swap_region = glad_egl_has_extension(extensions, "EGL_NOK_swap_region");
    GLAD_EGL_NOK_swap_region2 = glad_egl_has_extension(extensions, "EGL_NOK_swap_region2");
    GLAD_EGL_NOK_texture_from_pixmap = glad_egl_has_extension(extensions, "EGL_NOK_texture_from_pixmap");
    GLAD_EGL_NV_3dvision_surface = glad_egl_has_extension(extensions, "EGL_NV_3dvision_surface");
    GLAD_EGL_NV_context_priority_realtime = glad_egl_has_extension(extensions, "EGL_NV_context_priority_realtime");
    GLAD_EGL_NV_coverage_sample = glad_egl_has_extension(extensions, "EGL_NV_coverage_sample");
    GLAD_EGL_NV_coverage_sample_resolve = glad_egl_has_extension(extensions, "EGL_NV_coverage_sample_resolve");
    GLAD_EGL_NV_cuda_event = glad_egl_has_extension(extensions, "EGL_NV_cuda_event");
    GLAD_EGL_NV_depth_nonlinear = glad_egl_has_extension(extensions, "EGL_NV_depth_nonlinear");
    GLAD_EGL_NV_device_cuda = glad_egl_has_extension(extensions, "EGL_NV_device_cuda");
    GLAD_EGL_NV_native_query = glad_egl_has_extension(extensions, "EGL_NV_native_query");
    GLAD_EGL_NV_post_convert_rounding = glad_egl_has_extension(extensions, "EGL_NV_post_convert_rounding");
    GLAD_EGL_NV_post_sub_buffer = glad_egl_has_extension(extensions, "EGL_NV_post_sub_buffer");
    GLAD_EGL_NV_quadruple_buffer = glad_egl_has_extension(extensions, "EGL_NV_quadruple_buffer");
    GLAD_EGL_NV_robustness_video_memory_purge = glad_egl_has_extension(extensions, "EGL_NV_robustness_video_memory_purge");
    GLAD_EGL_NV_stream_consumer_eglimage = glad_egl_has_extension(extensions, "EGL_NV_stream_consumer_eglimage");
    GLAD_EGL_NV_stream_consumer_eglimage_use_scanout_attrib = glad_egl_has_extension(extensions, "EGL_NV_stream_consumer_eglimage_use_scanout_attrib");
    GLAD_EGL_NV_stream_consumer_gltexture_yuv = glad_egl_has_extension(extensions, "EGL_NV_stream_consumer_gltexture_yuv");
    GLAD_EGL_NV_stream_cross_display = glad_egl_has_extension(extensions, "EGL_NV_stream_cross_display");
    GLAD_EGL_NV_stream_cross_object = glad_egl_has_extension(extensions, "EGL_NV_stream_cross_object");
    GLAD_EGL_NV_stream_cross_partition = glad_egl_has_extension(extensions, "EGL_NV_stream_cross_partition");
    GLAD_EGL_NV_stream_cross_process = glad_egl_has_extension(extensions, "EGL_NV_stream_cross_process");
    GLAD_EGL_NV_stream_cross_system = glad_egl_has_extension(extensions, "EGL_NV_stream_cross_system");
    GLAD_EGL_NV_stream_dma = glad_egl_has_extension(extensions, "EGL_NV_stream_dma");
    GLAD_EGL_NV_stream_fifo_next = glad_egl_has_extension(extensions, "EGL_NV_stream_fifo_next");
    GLAD_EGL_NV_stream_fifo_synchronous = glad_egl_has_extension(extensions, "EGL_NV_stream_fifo_synchronous");
    GLAD_EGL_NV_stream_flush = glad_egl_has_extension(extensions, "EGL_NV_stream_flush");
    GLAD_EGL_NV_stream_frame_limits = glad_egl_has_extension(extensions, "EGL_NV_stream_frame_limits");
    GLAD_EGL_NV_stream_metadata = glad_egl_has_extension(extensions, "EGL_NV_stream_metadata");
    GLAD_EGL_NV_stream_origin = glad_egl_has_extension(extensions, "EGL_NV_stream_origin");
    GLAD_EGL_NV_stream_remote = glad_egl_has_extension(extensions, "EGL_NV_stream_remote");
    GLAD_EGL_NV_stream_reset = glad_egl_has_extension(extensions, "EGL_NV_stream_reset");
    GLAD_EGL_NV_stream_socket = glad_egl_has_extension(extensions, "EGL_NV_stream_socket");
    GLAD_EGL_NV_stream_socket_inet = glad_egl_has_extension(extensions, "EGL_NV_stream_socket_inet");
    GLAD_EGL_NV_stream_socket_unix = glad_egl_has_extension(extensions, "EGL_NV_stream_socket_unix");
    GLAD_EGL_NV_stream_sync = glad_egl_has_extension(extensions, "EGL_NV_stream_sync");
    GLAD_EGL_NV_sync = glad_egl_has_extension(extensions, "EGL_NV_sync");
    GLAD_EGL_NV_system_time = glad_egl_has_extension(extensions, "EGL_NV_system_time");
    GLAD_EGL_NV_triple_buffer = glad_egl_has_extension(extensions, "EGL_NV_triple_buffer");
    GLAD_EGL_QNX_image_native_buffer = glad_egl_has_extension(extensions, "EGL_QNX_image_native_buffer");
    GLAD_EGL_QNX_platform_screen = glad_egl_has_extension(extensions, "EGL_QNX_platform_screen");
    GLAD_EGL_TIZEN_image_native_buffer = glad_egl_has_extension(extensions, "EGL_TIZEN_image_native_buffer");
    GLAD_EGL_TIZEN_image_native_surface = glad_egl_has_extension(extensions, "EGL_TIZEN_image_native_surface");
    GLAD_EGL_WL_bind_wayland_display = glad_egl_has_extension(extensions, "EGL_WL_bind_wayland_display");
    GLAD_EGL_WL_create_wayland_buffer_from_image = glad_egl_has_extension(extensions, "EGL_WL_create_wayland_buffer_from_image");

    return 1;
}

static int glad_egl_find_core_egl(EGLDisplay display) {
    int major, minor;
    const char *version;

    if (display == NULL) {
        display = EGL_NO_DISPLAY; /* this is usually NULL, better safe than sorry */
    }
    if (display == EGL_NO_DISPLAY) {
        display = eglGetCurrentDisplay();
    }
#ifdef EGL_VERSION_1_4
    if (display == EGL_NO_DISPLAY) {
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    }
#endif
#ifndef EGL_VERSION_1_5
    if (display == EGL_NO_DISPLAY) {
        return 0;
    }
#endif

    version = eglQueryString(display, EGL_VERSION);
    (void) eglGetError();

    if (version == NULL) {
        major = 1;
        minor = 0;
    } else {
        GLAD_IMPL_UTIL_SSCANF(version, "%d.%d", &major, &minor);
    }

    GLAD_EGL_VERSION_1_0 = (major == 1 && minor >= 0) || major > 1;
    GLAD_EGL_VERSION_1_1 = (major == 1 && minor >= 1) || major > 1;
    GLAD_EGL_VERSION_1_2 = (major == 1 && minor >= 2) || major > 1;
    GLAD_EGL_VERSION_1_3 = (major == 1 && minor >= 3) || major > 1;
    GLAD_EGL_VERSION_1_4 = (major == 1 && minor >= 4) || major > 1;
    GLAD_EGL_VERSION_1_5 = (major == 1 && minor >= 5) || major > 1;

    return GLAD_MAKE_VERSION(major, minor);
}

int gladLoadEGLUserPtr(EGLDisplay display, GLADuserptrloadfunc load, void* userptr) {
    int version;
    eglGetDisplay = (PFNEGLGETDISPLAYPROC) load(userptr, "eglGetDisplay");
    eglGetCurrentDisplay = (PFNEGLGETCURRENTDISPLAYPROC) load(userptr, "eglGetCurrentDisplay");
    eglQueryString = (PFNEGLQUERYSTRINGPROC) load(userptr, "eglQueryString");
    eglGetError = (PFNEGLGETERRORPROC) load(userptr, "eglGetError");
    if (eglGetDisplay == NULL || eglGetCurrentDisplay == NULL || eglQueryString == NULL || eglGetError == NULL) return 0;

    version = glad_egl_find_core_egl(display);
    if (!version) return 0;
    glad_egl_load_EGL_VERSION_1_0(load, userptr);
    glad_egl_load_EGL_VERSION_1_1(load, userptr);
    glad_egl_load_EGL_VERSION_1_2(load, userptr);
    glad_egl_load_EGL_VERSION_1_4(load, userptr);
    glad_egl_load_EGL_VERSION_1_5(load, userptr);

    if (!glad_egl_find_extensions_egl(display)) return 0;
    glad_egl_load_EGL_ANDROID_blob_cache(load, userptr);
    glad_egl_load_EGL_ANDROID_create_native_client_buffer(load, userptr);
    glad_egl_load_EGL_ANDROID_get_frame_timestamps(load, userptr);
    glad_egl_load_EGL_ANDROID_get_native_client_buffer(load, userptr);
    glad_egl_load_EGL_ANDROID_native_fence_sync(load, userptr);
    glad_egl_load_EGL_ANDROID_presentation_time(load, userptr);
    glad_egl_load_EGL_ANGLE_query_surface_pointer(load, userptr);
    glad_egl_load_EGL_ANGLE_sync_control_rate(load, userptr);
    glad_egl_load_EGL_EXT_client_sync(load, userptr);
    glad_egl_load_EGL_EXT_compositor(load, userptr);
    glad_egl_load_EGL_EXT_device_base(load, userptr);
    glad_egl_load_EGL_EXT_device_enumeration(load, userptr);
    glad_egl_load_EGL_EXT_device_persistent_id(load, userptr);
    glad_egl_load_EGL_EXT_device_query(load, userptr);
    glad_egl_load_EGL_EXT_display_alloc(load, userptr);
    glad_egl_load_EGL_EXT_image_dma_buf_import_modifiers(load, userptr);
    glad_egl_load_EGL_EXT_output_base(load, userptr);
    glad_egl_load_EGL_EXT_platform_base(load, userptr);
    glad_egl_load_EGL_EXT_stream_consumer_egloutput(load, userptr);
    glad_egl_load_EGL_EXT_surface_compression(load, userptr);
    glad_egl_load_EGL_EXT_swap_buffers_with_damage(load, userptr);
    glad_egl_load_EGL_EXT_sync_reuse(load, userptr);
    glad_egl_load_EGL_HI_clientpixmap(load, userptr);
    glad_egl_load_EGL_KHR_cl_event2(load, userptr);
    glad_egl_load_EGL_KHR_debug(load, userptr);
    glad_egl_load_EGL_KHR_display_reference(load, userptr);
    glad_egl_load_EGL_KHR_fence_sync(load, userptr);
    glad_egl_load_EGL_KHR_image(load, userptr);
    glad_egl_load_EGL_KHR_image_base(load, userptr);
    glad_egl_load_EGL_KHR_lock_surface(load, userptr);
    glad_egl_load_EGL_KHR_lock_surface3(load, userptr);
    glad_egl_load_EGL_KHR_partial_update(load, userptr);
    glad_egl_load_EGL_KHR_reusable_sync(load, userptr);
    glad_egl_load_EGL_KHR_stream(load, userptr);
    glad_egl_load_EGL_KHR_stream_attrib(load, userptr);
    glad_egl_load_EGL_KHR_stream_consumer_gltexture(load, userptr);
    glad_egl_load_EGL_KHR_stream_cross_process_fd(load, userptr);
    glad_egl_load_EGL_KHR_stream_fifo(load, userptr);
    glad_egl_load_EGL_KHR_stream_producer_eglsurface(load, userptr);
    glad_egl_load_EGL_KHR_swap_buffers_with_damage(load, userptr);
    glad_egl_load_EGL_KHR_wait_sync(load, userptr);
    glad_egl_load_EGL_MESA_drm_image(load, userptr);
    glad_egl_load_EGL_MESA_image_dma_buf_export(load, userptr);
    glad_egl_load_EGL_MESA_query_driver(load, userptr);
    glad_egl_load_EGL_NOK_swap_region(load, userptr);
    glad_egl_load_EGL_NOK_swap_region2(load, userptr);
    glad_egl_load_EGL_NV_native_query(load, userptr);
    glad_egl_load_EGL_NV_post_sub_buffer(load, userptr);
    glad_egl_load_EGL_NV_stream_consumer_eglimage(load, userptr);
    glad_egl_load_EGL_NV_stream_consumer_gltexture_yuv(load, userptr);
    glad_egl_load_EGL_NV_stream_flush(load, userptr);
    glad_egl_load_EGL_NV_stream_metadata(load, userptr);
    glad_egl_load_EGL_NV_stream_reset(load, userptr);
    glad_egl_load_EGL_NV_stream_sync(load, userptr);
    glad_egl_load_EGL_NV_sync(load, userptr);
    glad_egl_load_EGL_NV_system_time(load, userptr);
    glad_egl_load_EGL_WL_bind_wayland_display(load, userptr);
    glad_egl_load_EGL_WL_create_wayland_buffer_from_image(load, userptr);

    glad_egl_resolve_aliases();

    return version;
}

int gladLoadEGL(EGLDisplay display, GLADloadfunc load) {
    return gladLoadEGLUserPtr(display, glad_egl_get_proc_from_userptr, GLAD_GNUC_EXTENSION (void*) load);
}

 
void gladInstallEGLDebug(void) {
    glad_debug_eglBindAPI = glad_debug_impl_eglBindAPI;
    glad_debug_eglBindTexImage = glad_debug_impl_eglBindTexImage;
    glad_debug_eglBindWaylandDisplayWL = glad_debug_impl_eglBindWaylandDisplayWL;
    glad_debug_eglChooseConfig = glad_debug_impl_eglChooseConfig;
    glad_debug_eglClientSignalSyncEXT = glad_debug_impl_eglClientSignalSyncEXT;
    glad_debug_eglClientWaitSync = glad_debug_impl_eglClientWaitSync;
    glad_debug_eglClientWaitSyncKHR = glad_debug_impl_eglClientWaitSyncKHR;
    glad_debug_eglClientWaitSyncNV = glad_debug_impl_eglClientWaitSyncNV;
    glad_debug_eglCompositorBindTexWindowEXT = glad_debug_impl_eglCompositorBindTexWindowEXT;
    glad_debug_eglCompositorSetContextAttributesEXT = glad_debug_impl_eglCompositorSetContextAttributesEXT;
    glad_debug_eglCompositorSetContextListEXT = glad_debug_impl_eglCompositorSetContextListEXT;
    glad_debug_eglCompositorSetSizeEXT = glad_debug_impl_eglCompositorSetSizeEXT;
    glad_debug_eglCompositorSetWindowAttributesEXT = glad_debug_impl_eglCompositorSetWindowAttributesEXT;
    glad_debug_eglCompositorSetWindowListEXT = glad_debug_impl_eglCompositorSetWindowListEXT;
    glad_debug_eglCompositorSwapPolicyEXT = glad_debug_impl_eglCompositorSwapPolicyEXT;
    glad_debug_eglCopyBuffers = glad_debug_impl_eglCopyBuffers;
    glad_debug_eglCreateContext = glad_debug_impl_eglCreateContext;
    glad_debug_eglCreateDRMImageMESA = glad_debug_impl_eglCreateDRMImageMESA;
    glad_debug_eglCreateFenceSyncNV = glad_debug_impl_eglCreateFenceSyncNV;
    glad_debug_eglCreateImage = glad_debug_impl_eglCreateImage;
    glad_debug_eglCreateImageKHR = glad_debug_impl_eglCreateImageKHR;
    glad_debug_eglCreateNativeClientBufferANDROID = glad_debug_impl_eglCreateNativeClientBufferANDROID;
    glad_debug_eglCreatePbufferFromClientBuffer = glad_debug_impl_eglCreatePbufferFromClientBuffer;
    glad_debug_eglCreatePbufferSurface = glad_debug_impl_eglCreatePbufferSurface;
    glad_debug_eglCreatePixmapSurface = glad_debug_impl_eglCreatePixmapSurface;
    glad_debug_eglCreatePixmapSurfaceHI = glad_debug_impl_eglCreatePixmapSurfaceHI;
    glad_debug_eglCreatePlatformPixmapSurface = glad_debug_impl_eglCreatePlatformPixmapSurface;
    glad_debug_eglCreatePlatformPixmapSurfaceEXT = glad_debug_impl_eglCreatePlatformPixmapSurfaceEXT;
    glad_debug_eglCreatePlatformWindowSurface = glad_debug_impl_eglCreatePlatformWindowSurface;
    glad_debug_eglCreatePlatformWindowSurfaceEXT = glad_debug_impl_eglCreatePlatformWindowSurfaceEXT;
    glad_debug_eglCreateStreamAttribKHR = glad_debug_impl_eglCreateStreamAttribKHR;
    glad_debug_eglCreateStreamFromFileDescriptorKHR = glad_debug_impl_eglCreateStreamFromFileDescriptorKHR;
    glad_debug_eglCreateStreamKHR = glad_debug_impl_eglCreateStreamKHR;
    glad_debug_eglCreateStreamProducerSurfaceKHR = glad_debug_impl_eglCreateStreamProducerSurfaceKHR;
    glad_debug_eglCreateStreamSyncNV = glad_debug_impl_eglCreateStreamSyncNV;
    glad_debug_eglCreateSync = glad_debug_impl_eglCreateSync;
    glad_debug_eglCreateSync64KHR = glad_debug_impl_eglCreateSync64KHR;
    glad_debug_eglCreateSyncKHR = glad_debug_impl_eglCreateSyncKHR;
    glad_debug_eglCreateWaylandBufferFromImageWL = glad_debug_impl_eglCreateWaylandBufferFromImageWL;
    glad_debug_eglCreateWindowSurface = glad_debug_impl_eglCreateWindowSurface;
    glad_debug_eglDebugMessageControlKHR = glad_debug_impl_eglDebugMessageControlKHR;
    glad_debug_eglDestroyContext = glad_debug_impl_eglDestroyContext;
    glad_debug_eglDestroyDisplayEXT = glad_debug_impl_eglDestroyDisplayEXT;
    glad_debug_eglDestroyImage = glad_debug_impl_eglDestroyImage;
    glad_debug_eglDestroyImageKHR = glad_debug_impl_eglDestroyImageKHR;
    glad_debug_eglDestroyStreamKHR = glad_debug_impl_eglDestroyStreamKHR;
    glad_debug_eglDestroySurface = glad_debug_impl_eglDestroySurface;
    glad_debug_eglDestroySync = glad_debug_impl_eglDestroySync;
    glad_debug_eglDestroySyncKHR = glad_debug_impl_eglDestroySyncKHR;
    glad_debug_eglDestroySyncNV = glad_debug_impl_eglDestroySyncNV;
    glad_debug_eglDupNativeFenceFDANDROID = glad_debug_impl_eglDupNativeFenceFDANDROID;
    glad_debug_eglExportDMABUFImageMESA = glad_debug_impl_eglExportDMABUFImageMESA;
    glad_debug_eglExportDMABUFImageQueryMESA = glad_debug_impl_eglExportDMABUFImageQueryMESA;
    glad_debug_eglExportDRMImageMESA = glad_debug_impl_eglExportDRMImageMESA;
    glad_debug_eglFenceNV = glad_debug_impl_eglFenceNV;
    glad_debug_eglGetCompositorTimingANDROID = glad_debug_impl_eglGetCompositorTimingANDROID;
    glad_debug_eglGetCompositorTimingSupportedANDROID = glad_debug_impl_eglGetCompositorTimingSupportedANDROID;
    glad_debug_eglGetConfigAttrib = glad_debug_impl_eglGetConfigAttrib;
    glad_debug_eglGetConfigs = glad_debug_impl_eglGetConfigs;
    glad_debug_eglGetCurrentContext = glad_debug_impl_eglGetCurrentContext;
    glad_debug_eglGetCurrentDisplay = glad_debug_impl_eglGetCurrentDisplay;
    glad_debug_eglGetCurrentSurface = glad_debug_impl_eglGetCurrentSurface;
    glad_debug_eglGetDisplay = glad_debug_impl_eglGetDisplay;
    glad_debug_eglGetDisplayDriverConfig = glad_debug_impl_eglGetDisplayDriverConfig;
    glad_debug_eglGetDisplayDriverName = glad_debug_impl_eglGetDisplayDriverName;
    glad_debug_eglGetError = glad_debug_impl_eglGetError;
    glad_debug_eglGetFrameTimestampSupportedANDROID = glad_debug_impl_eglGetFrameTimestampSupportedANDROID;
    glad_debug_eglGetFrameTimestampsANDROID = glad_debug_impl_eglGetFrameTimestampsANDROID;
    glad_debug_eglGetMscRateANGLE = glad_debug_impl_eglGetMscRateANGLE;
    glad_debug_eglGetNativeClientBufferANDROID = glad_debug_impl_eglGetNativeClientBufferANDROID;
    glad_debug_eglGetNextFrameIdANDROID = glad_debug_impl_eglGetNextFrameIdANDROID;
    glad_debug_eglGetOutputLayersEXT = glad_debug_impl_eglGetOutputLayersEXT;
    glad_debug_eglGetOutputPortsEXT = glad_debug_impl_eglGetOutputPortsEXT;
    glad_debug_eglGetPlatformDisplay = glad_debug_impl_eglGetPlatformDisplay;
    glad_debug_eglGetPlatformDisplayEXT = glad_debug_impl_eglGetPlatformDisplayEXT;
    glad_debug_eglGetProcAddress = glad_debug_impl_eglGetProcAddress;
    glad_debug_eglGetStreamFileDescriptorKHR = glad_debug_impl_eglGetStreamFileDescriptorKHR;
    glad_debug_eglGetSyncAttrib = glad_debug_impl_eglGetSyncAttrib;
    glad_debug_eglGetSyncAttribKHR = glad_debug_impl_eglGetSyncAttribKHR;
    glad_debug_eglGetSyncAttribNV = glad_debug_impl_eglGetSyncAttribNV;
    glad_debug_eglGetSystemTimeFrequencyNV = glad_debug_impl_eglGetSystemTimeFrequencyNV;
    glad_debug_eglGetSystemTimeNV = glad_debug_impl_eglGetSystemTimeNV;
    glad_debug_eglInitialize = glad_debug_impl_eglInitialize;
    glad_debug_eglLabelObjectKHR = glad_debug_impl_eglLabelObjectKHR;
    glad_debug_eglLockSurfaceKHR = glad_debug_impl_eglLockSurfaceKHR;
    glad_debug_eglMakeCurrent = glad_debug_impl_eglMakeCurrent;
    glad_debug_eglOutputLayerAttribEXT = glad_debug_impl_eglOutputLayerAttribEXT;
    glad_debug_eglOutputPortAttribEXT = glad_debug_impl_eglOutputPortAttribEXT;
    glad_debug_eglPostSubBufferNV = glad_debug_impl_eglPostSubBufferNV;
    glad_debug_eglPresentationTimeANDROID = glad_debug_impl_eglPresentationTimeANDROID;
    glad_debug_eglQueryAPI = glad_debug_impl_eglQueryAPI;
    glad_debug_eglQueryContext = glad_debug_impl_eglQueryContext;
    glad_debug_eglQueryDebugKHR = glad_debug_impl_eglQueryDebugKHR;
    glad_debug_eglQueryDeviceAttribEXT = glad_debug_impl_eglQueryDeviceAttribEXT;
    glad_debug_eglQueryDeviceBinaryEXT = glad_debug_impl_eglQueryDeviceBinaryEXT;
    glad_debug_eglQueryDeviceStringEXT = glad_debug_impl_eglQueryDeviceStringEXT;
    glad_debug_eglQueryDevicesEXT = glad_debug_impl_eglQueryDevicesEXT;
    glad_debug_eglQueryDisplayAttribEXT = glad_debug_impl_eglQueryDisplayAttribEXT;
    glad_debug_eglQueryDisplayAttribKHR = glad_debug_impl_eglQueryDisplayAttribKHR;
    glad_debug_eglQueryDisplayAttribNV = glad_debug_impl_eglQueryDisplayAttribNV;
    glad_debug_eglQueryDmaBufFormatsEXT = glad_debug_impl_eglQueryDmaBufFormatsEXT;
    glad_debug_eglQueryDmaBufModifiersEXT = glad_debug_impl_eglQueryDmaBufModifiersEXT;
    glad_debug_eglQueryNativeDisplayNV = glad_debug_impl_eglQueryNativeDisplayNV;
    glad_debug_eglQueryNativePixmapNV = glad_debug_impl_eglQueryNativePixmapNV;
    glad_debug_eglQueryNativeWindowNV = glad_debug_impl_eglQueryNativeWindowNV;
    glad_debug_eglQueryOutputLayerAttribEXT = glad_debug_impl_eglQueryOutputLayerAttribEXT;
    glad_debug_eglQueryOutputLayerStringEXT = glad_debug_impl_eglQueryOutputLayerStringEXT;
    glad_debug_eglQueryOutputPortAttribEXT = glad_debug_impl_eglQueryOutputPortAttribEXT;
    glad_debug_eglQueryOutputPortStringEXT = glad_debug_impl_eglQueryOutputPortStringEXT;
    glad_debug_eglQueryStreamAttribKHR = glad_debug_impl_eglQueryStreamAttribKHR;
    glad_debug_eglQueryStreamConsumerEventNV = glad_debug_impl_eglQueryStreamConsumerEventNV;
    glad_debug_eglQueryStreamKHR = glad_debug_impl_eglQueryStreamKHR;
    glad_debug_eglQueryStreamMetadataNV = glad_debug_impl_eglQueryStreamMetadataNV;
    glad_debug_eglQueryStreamTimeKHR = glad_debug_impl_eglQueryStreamTimeKHR;
    glad_debug_eglQueryStreamu64KHR = glad_debug_impl_eglQueryStreamu64KHR;
    glad_debug_eglQueryString = glad_debug_impl_eglQueryString;
    glad_debug_eglQuerySupportedCompressionRatesEXT = glad_debug_impl_eglQuerySupportedCompressionRatesEXT;
    glad_debug_eglQuerySurface = glad_debug_impl_eglQuerySurface;
    glad_debug_eglQuerySurface64KHR = glad_debug_impl_eglQuerySurface64KHR;
    glad_debug_eglQuerySurfacePointerANGLE = glad_debug_impl_eglQuerySurfacePointerANGLE;
    glad_debug_eglQueryWaylandBufferWL = glad_debug_impl_eglQueryWaylandBufferWL;
    glad_debug_eglReleaseTexImage = glad_debug_impl_eglReleaseTexImage;
    glad_debug_eglReleaseThread = glad_debug_impl_eglReleaseThread;
    glad_debug_eglResetStreamNV = glad_debug_impl_eglResetStreamNV;
    glad_debug_eglSetBlobCacheFuncsANDROID = glad_debug_impl_eglSetBlobCacheFuncsANDROID;
    glad_debug_eglSetDamageRegionKHR = glad_debug_impl_eglSetDamageRegionKHR;
    glad_debug_eglSetStreamAttribKHR = glad_debug_impl_eglSetStreamAttribKHR;
    glad_debug_eglSetStreamMetadataNV = glad_debug_impl_eglSetStreamMetadataNV;
    glad_debug_eglSignalSyncKHR = glad_debug_impl_eglSignalSyncKHR;
    glad_debug_eglSignalSyncNV = glad_debug_impl_eglSignalSyncNV;
    glad_debug_eglStreamAcquireImageNV = glad_debug_impl_eglStreamAcquireImageNV;
    glad_debug_eglStreamAttribKHR = glad_debug_impl_eglStreamAttribKHR;
    glad_debug_eglStreamConsumerAcquireAttribKHR = glad_debug_impl_eglStreamConsumerAcquireAttribKHR;
    glad_debug_eglStreamConsumerAcquireKHR = glad_debug_impl_eglStreamConsumerAcquireKHR;
    glad_debug_eglStreamConsumerGLTextureExternalAttribsNV = glad_debug_impl_eglStreamConsumerGLTextureExternalAttribsNV;
    glad_debug_eglStreamConsumerGLTextureExternalKHR = glad_debug_impl_eglStreamConsumerGLTextureExternalKHR;
    glad_debug_eglStreamConsumerOutputEXT = glad_debug_impl_eglStreamConsumerOutputEXT;
    glad_debug_eglStreamConsumerReleaseAttribKHR = glad_debug_impl_eglStreamConsumerReleaseAttribKHR;
    glad_debug_eglStreamConsumerReleaseKHR = glad_debug_impl_eglStreamConsumerReleaseKHR;
    glad_debug_eglStreamFlushNV = glad_debug_impl_eglStreamFlushNV;
    glad_debug_eglStreamImageConsumerConnectNV = glad_debug_impl_eglStreamImageConsumerConnectNV;
    glad_debug_eglStreamReleaseImageNV = glad_debug_impl_eglStreamReleaseImageNV;
    glad_debug_eglSurfaceAttrib = glad_debug_impl_eglSurfaceAttrib;
    glad_debug_eglSwapBuffers = glad_debug_impl_eglSwapBuffers;
    glad_debug_eglSwapBuffersRegion2NOK = glad_debug_impl_eglSwapBuffersRegion2NOK;
    glad_debug_eglSwapBuffersRegionNOK = glad_debug_impl_eglSwapBuffersRegionNOK;
    glad_debug_eglSwapBuffersWithDamageEXT = glad_debug_impl_eglSwapBuffersWithDamageEXT;
    glad_debug_eglSwapBuffersWithDamageKHR = glad_debug_impl_eglSwapBuffersWithDamageKHR;
    glad_debug_eglSwapInterval = glad_debug_impl_eglSwapInterval;
    glad_debug_eglTerminate = glad_debug_impl_eglTerminate;
    glad_debug_eglUnbindWaylandDisplayWL = glad_debug_impl_eglUnbindWaylandDisplayWL;
    glad_debug_eglUnlockSurfaceKHR = glad_debug_impl_eglUnlockSurfaceKHR;
    glad_debug_eglUnsignalSyncEXT = glad_debug_impl_eglUnsignalSyncEXT;
    glad_debug_eglWaitClient = glad_debug_impl_eglWaitClient;
    glad_debug_eglWaitGL = glad_debug_impl_eglWaitGL;
    glad_debug_eglWaitNative = glad_debug_impl_eglWaitNative;
    glad_debug_eglWaitSync = glad_debug_impl_eglWaitSync;
    glad_debug_eglWaitSyncKHR = glad_debug_impl_eglWaitSyncKHR;
}

void gladUninstallEGLDebug(void) {
    glad_debug_eglBindAPI = glad_eglBindAPI;
    glad_debug_eglBindTexImage = glad_eglBindTexImage;
    glad_debug_eglBindWaylandDisplayWL = glad_eglBindWaylandDisplayWL;
    glad_debug_eglChooseConfig = glad_eglChooseConfig;
    glad_debug_eglClientSignalSyncEXT = glad_eglClientSignalSyncEXT;
    glad_debug_eglClientWaitSync = glad_eglClientWaitSync;
    glad_debug_eglClientWaitSyncKHR = glad_eglClientWaitSyncKHR;
    glad_debug_eglClientWaitSyncNV = glad_eglClientWaitSyncNV;
    glad_debug_eglCompositorBindTexWindowEXT = glad_eglCompositorBindTexWindowEXT;
    glad_debug_eglCompositorSetContextAttributesEXT = glad_eglCompositorSetContextAttributesEXT;
    glad_debug_eglCompositorSetContextListEXT = glad_eglCompositorSetContextListEXT;
    glad_debug_eglCompositorSetSizeEXT = glad_eglCompositorSetSizeEXT;
    glad_debug_eglCompositorSetWindowAttributesEXT = glad_eglCompositorSetWindowAttributesEXT;
    glad_debug_eglCompositorSetWindowListEXT = glad_eglCompositorSetWindowListEXT;
    glad_debug_eglCompositorSwapPolicyEXT = glad_eglCompositorSwapPolicyEXT;
    glad_debug_eglCopyBuffers = glad_eglCopyBuffers;
    glad_debug_eglCreateContext = glad_eglCreateContext;
    glad_debug_eglCreateDRMImageMESA = glad_eglCreateDRMImageMESA;
    glad_debug_eglCreateFenceSyncNV = glad_eglCreateFenceSyncNV;
    glad_debug_eglCreateImage = glad_eglCreateImage;
    glad_debug_eglCreateImageKHR = glad_eglCreateImageKHR;
    glad_debug_eglCreateNativeClientBufferANDROID = glad_eglCreateNativeClientBufferANDROID;
    glad_debug_eglCreatePbufferFromClientBuffer = glad_eglCreatePbufferFromClientBuffer;
    glad_debug_eglCreatePbufferSurface = glad_eglCreatePbufferSurface;
    glad_debug_eglCreatePixmapSurface = glad_eglCreatePixmapSurface;
    glad_debug_eglCreatePixmapSurfaceHI = glad_eglCreatePixmapSurfaceHI;
    glad_debug_eglCreatePlatformPixmapSurface = glad_eglCreatePlatformPixmapSurface;
    glad_debug_eglCreatePlatformPixmapSurfaceEXT = glad_eglCreatePlatformPixmapSurfaceEXT;
    glad_debug_eglCreatePlatformWindowSurface = glad_eglCreatePlatformWindowSurface;
    glad_debug_eglCreatePlatformWindowSurfaceEXT = glad_eglCreatePlatformWindowSurfaceEXT;
    glad_debug_eglCreateStreamAttribKHR = glad_eglCreateStreamAttribKHR;
    glad_debug_eglCreateStreamFromFileDescriptorKHR = glad_eglCreateStreamFromFileDescriptorKHR;
    glad_debug_eglCreateStreamKHR = glad_eglCreateStreamKHR;
    glad_debug_eglCreateStreamProducerSurfaceKHR = glad_eglCreateStreamProducerSurfaceKHR;
    glad_debug_eglCreateStreamSyncNV = glad_eglCreateStreamSyncNV;
    glad_debug_eglCreateSync = glad_eglCreateSync;
    glad_debug_eglCreateSync64KHR = glad_eglCreateSync64KHR;
    glad_debug_eglCreateSyncKHR = glad_eglCreateSyncKHR;
    glad_debug_eglCreateWaylandBufferFromImageWL = glad_eglCreateWaylandBufferFromImageWL;
    glad_debug_eglCreateWindowSurface = glad_eglCreateWindowSurface;
    glad_debug_eglDebugMessageControlKHR = glad_eglDebugMessageControlKHR;
    glad_debug_eglDestroyContext = glad_eglDestroyContext;
    glad_debug_eglDestroyDisplayEXT = glad_eglDestroyDisplayEXT;
    glad_debug_eglDestroyImage = glad_eglDestroyImage;
    glad_debug_eglDestroyImageKHR = glad_eglDestroyImageKHR;
    glad_debug_eglDestroyStreamKHR = glad_eglDestroyStreamKHR;
    glad_debug_eglDestroySurface = glad_eglDestroySurface;
    glad_debug_eglDestroySync = glad_eglDestroySync;
    glad_debug_eglDestroySyncKHR = glad_eglDestroySyncKHR;
    glad_debug_eglDestroySyncNV = glad_eglDestroySyncNV;
    glad_debug_eglDupNativeFenceFDANDROID = glad_eglDupNativeFenceFDANDROID;
    glad_debug_eglExportDMABUFImageMESA = glad_eglExportDMABUFImageMESA;
    glad_debug_eglExportDMABUFImageQueryMESA = glad_eglExportDMABUFImageQueryMESA;
    glad_debug_eglExportDRMImageMESA = glad_eglExportDRMImageMESA;
    glad_debug_eglFenceNV = glad_eglFenceNV;
    glad_debug_eglGetCompositorTimingANDROID = glad_eglGetCompositorTimingANDROID;
    glad_debug_eglGetCompositorTimingSupportedANDROID = glad_eglGetCompositorTimingSupportedANDROID;
    glad_debug_eglGetConfigAttrib = glad_eglGetConfigAttrib;
    glad_debug_eglGetConfigs = glad_eglGetConfigs;
    glad_debug_eglGetCurrentContext = glad_eglGetCurrentContext;
    glad_debug_eglGetCurrentDisplay = glad_eglGetCurrentDisplay;
    glad_debug_eglGetCurrentSurface = glad_eglGetCurrentSurface;
    glad_debug_eglGetDisplay = glad_eglGetDisplay;
    glad_debug_eglGetDisplayDriverConfig = glad_eglGetDisplayDriverConfig;
    glad_debug_eglGetDisplayDriverName = glad_eglGetDisplayDriverName;
    glad_debug_eglGetError = glad_eglGetError;
    glad_debug_eglGetFrameTimestampSupportedANDROID = glad_eglGetFrameTimestampSupportedANDROID;
    glad_debug_eglGetFrameTimestampsANDROID = glad_eglGetFrameTimestampsANDROID;
    glad_debug_eglGetMscRateANGLE = glad_eglGetMscRateANGLE;
    glad_debug_eglGetNativeClientBufferANDROID = glad_eglGetNativeClientBufferANDROID;
    glad_debug_eglGetNextFrameIdANDROID = glad_eglGetNextFrameIdANDROID;
    glad_debug_eglGetOutputLayersEXT = glad_eglGetOutputLayersEXT;
    glad_debug_eglGetOutputPortsEXT = glad_eglGetOutputPortsEXT;
    glad_debug_eglGetPlatformDisplay = glad_eglGetPlatformDisplay;
    glad_debug_eglGetPlatformDisplayEXT = glad_eglGetPlatformDisplayEXT;
    glad_debug_eglGetProcAddress = glad_eglGetProcAddress;
    glad_debug_eglGetStreamFileDescriptorKHR = glad_eglGetStreamFileDescriptorKHR;
    glad_debug_eglGetSyncAttrib = glad_eglGetSyncAttrib;
    glad_debug_eglGetSyncAttribKHR = glad_eglGetSyncAttribKHR;
    glad_debug_eglGetSyncAttribNV = glad_eglGetSyncAttribNV;
    glad_debug_eglGetSystemTimeFrequencyNV = glad_eglGetSystemTimeFrequencyNV;
    glad_debug_eglGetSystemTimeNV = glad_eglGetSystemTimeNV;
    glad_debug_eglInitialize = glad_eglInitialize;
    glad_debug_eglLabelObjectKHR = glad_eglLabelObjectKHR;
    glad_debug_eglLockSurfaceKHR = glad_eglLockSurfaceKHR;
    glad_debug_eglMakeCurrent = glad_eglMakeCurrent;
    glad_debug_eglOutputLayerAttribEXT = glad_eglOutputLayerAttribEXT;
    glad_debug_eglOutputPortAttribEXT = glad_eglOutputPortAttribEXT;
    glad_debug_eglPostSubBufferNV = glad_eglPostSubBufferNV;
    glad_debug_eglPresentationTimeANDROID = glad_eglPresentationTimeANDROID;
    glad_debug_eglQueryAPI = glad_eglQueryAPI;
    glad_debug_eglQueryContext = glad_eglQueryContext;
    glad_debug_eglQueryDebugKHR = glad_eglQueryDebugKHR;
    glad_debug_eglQueryDeviceAttribEXT = glad_eglQueryDeviceAttribEXT;
    glad_debug_eglQueryDeviceBinaryEXT = glad_eglQueryDeviceBinaryEXT;
    glad_debug_eglQueryDeviceStringEXT = glad_eglQueryDeviceStringEXT;
    glad_debug_eglQueryDevicesEXT = glad_eglQueryDevicesEXT;
    glad_debug_eglQueryDisplayAttribEXT = glad_eglQueryDisplayAttribEXT;
    glad_debug_eglQueryDisplayAttribKHR = glad_eglQueryDisplayAttribKHR;
    glad_debug_eglQueryDisplayAttribNV = glad_eglQueryDisplayAttribNV;
    glad_debug_eglQueryDmaBufFormatsEXT = glad_eglQueryDmaBufFormatsEXT;
    glad_debug_eglQueryDmaBufModifiersEXT = glad_eglQueryDmaBufModifiersEXT;
    glad_debug_eglQueryNativeDisplayNV = glad_eglQueryNativeDisplayNV;
    glad_debug_eglQueryNativePixmapNV = glad_eglQueryNativePixmapNV;
    glad_debug_eglQueryNativeWindowNV = glad_eglQueryNativeWindowNV;
    glad_debug_eglQueryOutputLayerAttribEXT = glad_eglQueryOutputLayerAttribEXT;
    glad_debug_eglQueryOutputLayerStringEXT = glad_eglQueryOutputLayerStringEXT;
    glad_debug_eglQueryOutputPortAttribEXT = glad_eglQueryOutputPortAttribEXT;
    glad_debug_eglQueryOutputPortStringEXT = glad_eglQueryOutputPortStringEXT;
    glad_debug_eglQueryStreamAttribKHR = glad_eglQueryStreamAttribKHR;
    glad_debug_eglQueryStreamConsumerEventNV = glad_eglQueryStreamConsumerEventNV;
    glad_debug_eglQueryStreamKHR = glad_eglQueryStreamKHR;
    glad_debug_eglQueryStreamMetadataNV = glad_eglQueryStreamMetadataNV;
    glad_debug_eglQueryStreamTimeKHR = glad_eglQueryStreamTimeKHR;
    glad_debug_eglQueryStreamu64KHR = glad_eglQueryStreamu64KHR;
    glad_debug_eglQueryString = glad_eglQueryString;
    glad_debug_eglQuerySupportedCompressionRatesEXT = glad_eglQuerySupportedCompressionRatesEXT;
    glad_debug_eglQuerySurface = glad_eglQuerySurface;
    glad_debug_eglQuerySurface64KHR = glad_eglQuerySurface64KHR;
    glad_debug_eglQuerySurfacePointerANGLE = glad_eglQuerySurfacePointerANGLE;
    glad_debug_eglQueryWaylandBufferWL = glad_eglQueryWaylandBufferWL;
    glad_debug_eglReleaseTexImage = glad_eglReleaseTexImage;
    glad_debug_eglReleaseThread = glad_eglReleaseThread;
    glad_debug_eglResetStreamNV = glad_eglResetStreamNV;
    glad_debug_eglSetBlobCacheFuncsANDROID = glad_eglSetBlobCacheFuncsANDROID;
    glad_debug_eglSetDamageRegionKHR = glad_eglSetDamageRegionKHR;
    glad_debug_eglSetStreamAttribKHR = glad_eglSetStreamAttribKHR;
    glad_debug_eglSetStreamMetadataNV = glad_eglSetStreamMetadataNV;
    glad_debug_eglSignalSyncKHR = glad_eglSignalSyncKHR;
    glad_debug_eglSignalSyncNV = glad_eglSignalSyncNV;
    glad_debug_eglStreamAcquireImageNV = glad_eglStreamAcquireImageNV;
    glad_debug_eglStreamAttribKHR = glad_eglStreamAttribKHR;
    glad_debug_eglStreamConsumerAcquireAttribKHR = glad_eglStreamConsumerAcquireAttribKHR;
    glad_debug_eglStreamConsumerAcquireKHR = glad_eglStreamConsumerAcquireKHR;
    glad_debug_eglStreamConsumerGLTextureExternalAttribsNV = glad_eglStreamConsumerGLTextureExternalAttribsNV;
    glad_debug_eglStreamConsumerGLTextureExternalKHR = glad_eglStreamConsumerGLTextureExternalKHR;
    glad_debug_eglStreamConsumerOutputEXT = glad_eglStreamConsumerOutputEXT;
    glad_debug_eglStreamConsumerReleaseAttribKHR = glad_eglStreamConsumerReleaseAttribKHR;
    glad_debug_eglStreamConsumerReleaseKHR = glad_eglStreamConsumerReleaseKHR;
    glad_debug_eglStreamFlushNV = glad_eglStreamFlushNV;
    glad_debug_eglStreamImageConsumerConnectNV = glad_eglStreamImageConsumerConnectNV;
    glad_debug_eglStreamReleaseImageNV = glad_eglStreamReleaseImageNV;
    glad_debug_eglSurfaceAttrib = glad_eglSurfaceAttrib;
    glad_debug_eglSwapBuffers = glad_eglSwapBuffers;
    glad_debug_eglSwapBuffersRegion2NOK = glad_eglSwapBuffersRegion2NOK;
    glad_debug_eglSwapBuffersRegionNOK = glad_eglSwapBuffersRegionNOK;
    glad_debug_eglSwapBuffersWithDamageEXT = glad_eglSwapBuffersWithDamageEXT;
    glad_debug_eglSwapBuffersWithDamageKHR = glad_eglSwapBuffersWithDamageKHR;
    glad_debug_eglSwapInterval = glad_eglSwapInterval;
    glad_debug_eglTerminate = glad_eglTerminate;
    glad_debug_eglUnbindWaylandDisplayWL = glad_eglUnbindWaylandDisplayWL;
    glad_debug_eglUnlockSurfaceKHR = glad_eglUnlockSurfaceKHR;
    glad_debug_eglUnsignalSyncEXT = glad_eglUnsignalSyncEXT;
    glad_debug_eglWaitClient = glad_eglWaitClient;
    glad_debug_eglWaitGL = glad_eglWaitGL;
    glad_debug_eglWaitNative = glad_eglWaitNative;
    glad_debug_eglWaitSync = glad_eglWaitSync;
    glad_debug_eglWaitSyncKHR = glad_eglWaitSyncKHR;
}


#ifdef __cplusplus
}
#endif
