LOCAL_PATH 				:= $(call my-dir)
PATH_DEPENDENCIES 		:= $(LOCAL_PATH)/../../dependencies
ENGINE_SOURCE			:= $(LOCAL_PATH)/../../engine
MATH_SOURCE				:= $(LOCAL_PATH)/../../math

include $(CLEAR_VARS) 

LOCAL_CFLAGS			:= -std=gnu++11 -D DEBUG

LOCAL_MODULE 			:= Engine

LOCAL_SRC_FILES 		:= $(wildcard $(ENGINE_SOURCE)/src/*.cpp)
LOCAL_SRC_FILES			+= $(wildcard $(ENGINE_SOURCE)/src/Android/*.cpp)
LOCAL_SRC_FILES			+= $(wildcard $(ENGINE_SOURCE)/src/Linux/*.cpp)

LOCAL_C_INCLUDES 		:= $(ENGINE_SOURCE)/include
LOCAL_C_INCLUDES		+= $(MATH_SOURCE)/include
LOCAL_C_INCLUDES		+= $(PATH_DEPENDENCIES)/Enet/include
LOCAL_C_INCLUDES		+= $(PATH_DEPENDENCIES)/JsonCPP/include
LOCAL_C_INCLUDES		+= $(PATH_DEPENDENCIES)/TinyThread/include

#filter out unused stuff
LOCAL_SRC_FILES			:= $(filter-out $(ENGINE_SOURCE)/src/K15_Font.cpp, $(LOCAL_SRC_FILES))
LOCAL_SRC_FILES			:= $(filter-out $(ENGINE_SOURCE)/src/K15_FontManager.cpp, $(LOCAL_SRC_FILES))
LOCAL_SRC_FILES			:= $(filter-out $(ENGINE_SOURCE)/src/K15_TrueTypeFont.cpp, $(LOCAL_SRC_FILES))

LOCAL_STATIC_LIBRARIES 	:= android_native_app_glue 
LOCAL_STATIC_LIBRARIES 	+= Math
LOCAL_STATIC_LIBRARIES 	+= JsonCPP 
LOCAL_STATIC_LIBRARIES	+= Enet
LOCAL_STATIC_LIBRARIES  += TinyThread

LOCAL_LDLIBS 			:= -llog -ldl -lGLESv2 -lEGL -lOpenSLES -landroid

LOCAL_LDFLAGS			:=	-L$(LOCAL_PATH)/../../bin/dependencies/Android_armeabi

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,Math)