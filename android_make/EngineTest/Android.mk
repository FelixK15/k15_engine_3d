LOCAL_PATH 				:= $(call my-dir)
ENGINE_SOURCE			:= $(LOCAL_PATH)/../../engine
ENGINETEST_SOURCE		:= $(LOCAL_PATH)/../../test
include $(CLEAR_VARS) 

LOCAL_CFLAGS			:= -std=gnu++11 -D DEBUG

LOCAL_MODULE 			:= EngineTest

LOCAL_SRC_FILES 		:= $(wildcard $(ENGINETEST_SOURCE)/src/*.cpp)

LOCAL_C_INCLUDES 		:= $(ENGINE_SOURCE)/include

LOCAL_SHARED_LIBRARIES	:= Engine

LOCAL_STATIC_LIBRARIES	:= android_native_app_glue
LOCAL_LDLIBS			:= -landroid

#LOCAL_JNI_SHARED_LIBRARIES := $(LOCAL_PATH)/libs/armeabi/libEngine
#LOCAL_JNI_SHARED_LIBRARIES += $(LOCAL_PATH)/libs/armeabi/libEngineTest

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,Engine)