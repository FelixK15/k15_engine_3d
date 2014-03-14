LOCAL_PATH 			:= $(call my-dir)

ifneq ($(LOCAL_PATH),.)
	PATH_DEPENDENCIES	:= ./../../../dependencies
else
	PATH_DEPENDENCIES 	:= $(LOCAL_PATH)/../../../dependencies
endif

include $(CLEAR_VARS)

LOCAL_MODULE		:= Enet
LOCAL_SRC_FILES		:= $(wildcard $(PATH_DEPENDENCIES)/Enet/src/*.c)
LOCAL_C_INCLUDES 	:= $(PATH_DEPENDENCIES)/Enet/include

include $(BUILD_STATIC_LIBRARY)