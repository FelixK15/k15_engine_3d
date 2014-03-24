LOCAL_PATH 			:= $(call my-dir)
PATH_DEPENDENCIES 	:= $(LOCAL_PATH)/../../../dependencies

include $(CLEAR_VARS) 

LOCAL_MODULE 			:= JsonCPP
LOCAL_SRC_FILES 		:= $(wildcard $(PATH_DEPENDENCIES)/JsonCPP/src/*.cpp)
LOCAL_C_INCLUDES 		:= $(PATH_DEPENDENCIES)/JsonCPP/include
LOCAL_CFLAGS			:= -fexceptions

include $(BUILD_STATIC_LIBRARY)