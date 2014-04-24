LOCAL_PATH 		:= $(call my-dir)
MATH_SOURCE 	:= $(LOCAL_PATH)/../../math
ENGINE_SOURCE	:= $(LOCAL_PATH)/../../engine

include $(CLEAR_VARS) 

LOCAL_MODULE 		:= Math
LOCAL_SRC_FILES 	:= $(wildcard $(MATH_SOURCE)/src/*.cpp)
LOCAL_C_INCLUDES 	:= $(MATH_SOURCE)/include
LOCAL_C_INCLUDES	+= $(ENGINE_SOURCE)/include

LOCAL_LDLIBS		:= lgnustl_shared

include $(BUILD_STATIC_LIBRARY)