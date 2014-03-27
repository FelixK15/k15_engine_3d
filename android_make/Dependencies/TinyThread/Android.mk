LOCAL_PATH 				:= $(call my-dir)
PATH_DEPENDENCIES 	:= $(LOCAL_PATH)/../../../dependencies

include $(CLEAR_VARS) 

LOCAL_MODULE 			:= TinyThread
LOCAL_SRC_FILES 		:= $(wildcard $(PATH_DEPENDENCIES)/TinyThread/src/*.cpp)
LOCAL_C_INCLUDES 		:= $(PATH_DEPENDENCIES)/TinyThread/include
LOCAL_EXPORT_C_INCLUDES	:= ../../dependencies/TinyThread/include
LOCAL_EXPORT_SRC_FILES	:= $(wildcard ../../dependencies/TinyThread/src/*.cpp)
LOCAL_CFLAGS			:= -fexceptions

include $(BUILD_STATIC_LIBRARY)