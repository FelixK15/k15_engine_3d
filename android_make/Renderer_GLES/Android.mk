LOCAL_PATH 				:= $(call my-dir)
ENGINE_SOURCE			:= $(LOCAL_PATH)/../../engine
RENDERER_SOURCE			:= $(LOCAL_PATH)/../../rendererOGL

include $(CLEAR_VARS) 

LOCAL_CFLAGS			:= -std=gnu++11 -D DEBUG

LOCAL_MODULE 			:= Renderer_GLES

LOCAL_SRC_FILES 		:= $(wildcard $(RENDERER_SOURCE)/src/*.cpp)
LOCAL_SRC_FILES			+= $(wildcard $(RENDERER_SOURCE)/GLES2/*.cpp)

LOCAL_C_INCLUDES 		:= $(ENGINE_SOURCE)/include
LOCAL_C_INCLUDES		+= $(RENDERER_SOURCE)/include

LOCAL_SHARED_LIBRARIES	:= Engine

LOCAL_LDLIBS 			:= -lGLESv2

include $(BUILD_SHARED_LIBRARY)

$(call import-module,Engine)