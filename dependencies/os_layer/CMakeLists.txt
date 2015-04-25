set(K15_OS_LAYER_SRC 	"src/K15_OSLayer_SystemEvents.cpp"
			"src/K15_OSLayer_Window.cpp"
			"src/K15_OSLayer_Thread.cpp"
			"src/K15_OSLayer_OSContext.cpp"
			"src/K15_OSLayer_FileSystem.cpp"
			"src/K15_OSLayer_System.cpp")

set(K15_OS_LAYER_WIN32_SRC  	"src/win32/K15_EventsWin32.cpp"
				"src/win32/K15_WindowWin32.cpp"
				"src/win32/K15_ThreadWin32.cpp"
				"src/win32/K15_FileSystemWin32.cpp"
				"src/win32/K15_EnvironmentWin32.cpp")

set(K15_OS_LAYER_LINUX_SRC	"src/posix/K15_ThreadPosix.cpp"
				"src/linux/K15_EnvironmentLinux.cpp"
				"src/linux/K15_WindowLinux.cpp"
				"src/linux/K15_EventsLinux.cpp")

set(K15_OS_LAYER_ANDROID_SRC "src/android/K15_EnvironmentAndroid.cpp"
				"src/android/K15_EventsAndroid.cpp"
				"src/posix/K15_ThreadPosix.cpp"
				"src/posix/K15_SystemPosix.cpp"
				"src/posix/K15_FileSystemPosix.cpp")

set(K15_OS_LAYER_INC 	"include/K15_OSLayer_EventTypes.h"
			"include/K15_OSLayer_ErrorCodes.h"
			"include/K15_OSLayer_Thread.h"
			"include/K15_OSLayer_Prerequisites.h"
			"include/K15_OSLayer_Window.h"
			"include/K15_OSLayer_SystemEvents.h"
			"include/K15_OSLayer_OSContext.h"
			"include/K15_OSLayer_FileSystem.h"
			"include/K15_OSLayer_System.h"
			"include/K15_OSLayer_Config.h")

set(K15_OS_LAYER_WIN32_INC 	"include/win32/K15_EventsWin32.h"
				"include/win32/K15_WindowWin32.h"
				"include/win32/K15_ThreadWin32.h"
				"include/win32/K15_FileSystemWin32.h"
				"include/win32/K15_EnvironmentWin32.h")

set(K15_OS_LAYER_LINUX_INC	"include/linux/K15_ThreadLinux.h"
				"include/linux/K15_EnvironmentLinux.h"
				"include/linux/K15_WindowLinux.h"
				"include/linux/K15_EventsLinux.h")

set(K15_OS_LAYER_ANDROID_INC "include/android/K15_EnvironmentAndroid.h"
				"include/android/K15_EventsAndroid.h"
				"include/posix/K15_ThreadPosix.h"
				"include/posix/K15_SystemPosix.h"
				"include/posix/K15_FileSystemPosix.h")

include_directories(include)
include_directories(../logging/include)

source_group(include FILES ${K15_OS_LAYER_INC})
source_group(src FILES ${K15_OS_LAYER_SRC})

if(${K15_PLATFORM} STREQUAL "Win32")
	source_group(include\\win32 FILES ${K15_OS_LAYER_WIN32_INC})
	source_group(src\\win32 FILES ${K15_OS_LAYER_WIN32_SRC})
	set(K15_OS_LAYER_COMPLETE_SRC ${K15_OS_LAYER_SRC} ${K15_OS_LAYER_INC} ${K15_OS_LAYER_WIN32_SRC} ${K15_OS_LAYER_WIN32_INC})
elseif(${K15_PLATFORM} STREQUAL "Android")
	source_group(include\\android FILES ${K15_OS_LAYER_ANDROID_INC})
	source_group(src\\android FILES ${K15_OS_LAYER_ANDROID_SRC})
	set(K15_OS_LAYER_COMPLETE_SRC ${K15_OS_LAYER_SRC} ${K15_OS_LAYER_INC} ${K15_OS_LAYER_ANDROID_INC} ${K15_OS_LAYER_ANDROID_SRC})
elseif(${K15_PLATFORM} STREQUAL "Linux")
	source_group(include\\linux FILES ${K15_OS_LAYER_LINUX_INC})
	source_group(src\\linux FILES ${K15_OS_LAYER_LINUX_SRC})
	set(K15_OS_LAYER_COMPLETE_SRC ${K15_OS_LAYER_SRC} ${K15_OS_LAYER_INC} ${K15_OS_LAYER_LINUX_INC} ${K15_OS_LAYER_LINUX_SRC})
endif()

add_library(os_layer ${K15_OS_LAYER_COMPLETE_SRC})
