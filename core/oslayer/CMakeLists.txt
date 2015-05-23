set(K15_OS_LAYER_SRC 	"src/K15_SystemEvents.cpp"
			"src/K15_DynamicLibrary.cpp"
			"src/K15_Window.cpp"
			"src/K15_Thread.cpp"
			"src/K15_OSContext.cpp"
			"src/K15_FileSystem.cpp"
			"src/K15_System.cpp")

set(K15_OS_LAYER_WIN32_SRC  	"src/win32/K15_EventsWin32.cpp"
				"src/win32/K15_DynamicLibraryWin32.cpp"
				"src/win32/K15_WindowWin32.cpp"
				"src/win32/K15_ThreadWin32.cpp"
				"src/win32/K15_FileSystemWin32.cpp"
				"src/win32/K15_HelperWin32.cpp"
				"src/win32/K15_OSContextWin32.cpp")

set(K15_OS_LAYER_LINUX_SRC	"src/posix/K15_ThreadPosix.cpp"
				"src/posix/K15_DynamicLibrary.cpp"
				"src/linux/K15_OSContextLinux.cpp"
				"src/linux/K15_WindowLinux.cpp"
				"src/linux/K15_EventsLinux.cpp")

set(K15_OS_LAYER_ANDROID_SRC 	"src/android/K15_OSContextAndroid.cpp"
				"src/android/K15_EventsAndroid.cpp"
				"src/android/K15_WindowAndroid.cpp"
				"src/posix/K15_DynamicLibraryPosix.cpp"
				"src/posix/K15_ThreadPosix.cpp"
				"src/posix/K15_SystemPosix.cpp"
				"src/posix/K15_FileSystemPosix.cpp")

set(K15_OS_LAYER_INC 	"include/K15_EventTypes.h"
			"include/K15_ErrorCodes.h"
			"include/K15_Thread.h"
			"include/K15_DynamicLibrary.h"
			"include/K15_Prerequisites.h"
			"include/K15_Window.h"
			"include/K15_SystemEvents.h"
			"include/K15_OSContext.h"
			"include/K15_FileSystem.h"
			"include/K15_System.h"
			"include/K15_Config.h"
			"include/K15_DefaultCLibraries.h")

set(K15_OS_LAYER_WIN32_INC 	"include/win32/K15_EventsWin32.h"
				"include/win32/K15_DynamicLibraryWin32.h"
				"include/win32/K15_WindowWin32.h"
				"include/win32/K15_ThreadWin32.h"
				"include/win32/K15_FileSystemWin32.h"
				"include/win32/K15_HelperWin32.h"
				"include/win32/K15_OSContextWin32.h"
				"include/win32/K15_HeaderDefaultWin32.h"
				"include/win32/K15_HeaderExtensionsWin32.h")

set(K15_OS_LAYER_LINUX_INC	"include/posix/K15_ThreadPosix.h"
				"include/posix/K15_DynamicLibraryPosix.h"
				"include/linux/K15_OSContextLinux.h"
				"include/linux/K15_WindowLinux.h"
				"include/linux/K15_EventsLinux.h")

set(K15_OS_LAYER_ANDROID_INC 	"include/android/K15_OSContextAndroid.h"
				"include/android/K15_EventsAndroid.h"
				"include/android/K15_WindowAndroid.h"
				"include/posix/K15_ThreadPosix.h"
				"include/posix/K15_DynamicLibraryPosix.h"
				"include/posix/K15_SystemPosix.h"
				"include/posix/K15_FileSystemPosix.h")

include_directories(src)
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

add_library(oslayer ${K15_OS_LAYER_COMPLETE_SRC})
