LOCAL_PATH 				:= $(call my-dir)
ENGINE_SOURCE			:= $(LOCAL_PATH)/../../engine
MATH_SOURCE				:= $(LOCAL_PATH)/../../math
PLUGIN_SOURCE			:= $(LOCAL_PATH)/../../plugins

include $(CLEAR_VARS) 

LOCAL_CFLAGS			:= -std=gnu++11 -D DEBUG

LOCAL_MODULE 			:= Plugin_RendererTest

LOCAL_SRC_FILES 		:= $(wildcard $(PLUGIN_SOURCE)/rendertest/src/*.cpp)

#filter plugin.cpp
LOCAL_SRC_FILES			:= $(filter-out $(PLUGIN_SOURCE)/rendertest/src/plugin.cpp, $(LOCAL_SRC_FILES))

LOCAL_C_INCLUDES 		:= $(ENGINE_SOURCE)/include
LOCAL_C_INCLUDES		+= $(MATH_SOURCE)/include
LOCAL_C_INCLUDES		+= $(PLUGIN_SOURCE)/rendertest/include

LOCAL_LDLIBS			:= lgnustl_shared

include $(BUILD_STATIC_LIBRARY)

