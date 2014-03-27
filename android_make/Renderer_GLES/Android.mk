LOCAL_PATH 				:= $(call my-dir)
ENGINE_SOURCE			:= $(LOCAL_PATH)/../../engine
RENDERER_SOURCE			:= $(LOCAL_PATH)/../../rendererOGL

include $(CLEAR_VARS) 

LOCAL_CFLAGS			:= -std=gnu++11 -D DEBUG

LOCAL_MODULE 			:= Renderer_GLES

LOCAL_SRC_FILES 		:= $(wildcard $(RENDERER_SOURCE)/src/*.cpp)
LOCAL_SRC_FILES			+= $(wildcard $(RENDERER_SOURCE)/src/GLES2/*.cpp)

LOCAL_C_INCLUDES 		:= $(ENGINE_SOURCE)/include
LOCAL_C_INCLUDES		+= $(RENDERER_SOURCE)/include
LOCAL_C_INCLUDES		+= $(RENDERER_SOURCE)/include/GLES2

LOCAL_STATIC_LIBRARIES	:= android_native_app_glue
LOCAL_STATIC_LIBRARIES	+= Engine

include $(BUILD_STATIC_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,Engine)
