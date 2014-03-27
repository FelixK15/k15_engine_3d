LOCAL_PATH 				:= $(call my-dir)
ENGINE_SOURCE			:= $(LOCAL_PATH)/../../engine
ENGINETEST_SOURCE		:= $(LOCAL_PATH)/../../test
GLES2_RENDERER_SOURCE	:= $(LOCAL_PATH)/../../rendererOGL

include $(CLEAR_VARS) 

#APP_OPTIM 				:= debug 

LOCAL_CFLAGS			:= -std=gnu++11 -D DEBUG

LOCAL_MODULE 			:= EngineTest

LOCAL_SRC_FILES 		:= $(wildcard $(ENGINETEST_SOURCE)/src/*.cpp)

LOCAL_C_INCLUDES 		:= $(ENGINE_SOURCE)/include
LOCAL_C_INCLUDES		+= $(GLES2_RENDERER_SOURCE)/include

LOCAL_STATIC_LIBRARIES	:= android_native_app_glue
LOCAL_STATIC_LIBRARIES	+= Engine
LOCAL_STATIC_LIBRARIES	+= Renderer_GLES

LOCAL_LDLIBS 			:= -llog -ldl -lGLESv2 -lEGL -lOpenSLES -landroid

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,Engine)
$(call import-module,Renderer_GLES)