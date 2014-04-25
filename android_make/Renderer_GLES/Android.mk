LOCAL_PATH 				:= $(call my-dir)
ENGINE_SOURCE			:= $(LOCAL_PATH)/../../engine
RENDERER_SOURCE			:= $(LOCAL_PATH)/../../renderer_opengl_es

include $(CLEAR_VARS) 

LOCAL_CFLAGS			:= -std=gnu++11 -D DEBUG

LOCAL_MODULE 			:= Renderer_GLES

LOCAL_SRC_FILES 		:= $(wildcard $(RENDERER_SOURCE)/src/GLES2/*.cpp)

LOCAL_C_INCLUDES 		:= $(ENGINE_SOURCE)/include
LOCAL_C_INCLUDES		+= $(RENDERER_SOURCE)/include
LOCAL_C_INCLUDES		+= $(RENDERER_SOURCE)/include/GLES2

LOCAL_STATIC_LIBRARIES	+= Engine

LOCAL_LDLIBS			:= lgnustl_shared

include $(BUILD_STATIC_LIBRARY)

$(call import-module,Engine)
