#Save All *.cpp files into variable CORE_SRC
file(GLOB CORE_SRC "./src/*.cpp")
file(GLOB CORE_INC "./include/*.h" "./include/*.inl")

#Add OS Specific files
if(WIN32)
	set(OS_SRC "./src/win32/K15_DynamicLibrary_Win32.cpp"
	     	"./src/win32/K15_Keyboard_Win32.cpp"
	     	"./src/win32/K15_Mouse_Win32.cpp"
	     	"./src/win32/K15_OSLayer_Win32.cpp"
	     	"./src/win32/K15_RenderWindow_Win32.cpp"
	     	"./src/win32/K15_TextConsoleLog_Win32.cpp"
	     	"./src/win32/K15_VisualStudioLog_Win32.cpp")

	set(OS_INC "./include/win32/K15_DynamicLibrary_Win32.h"
		"./include/win32/K15_OSLayer_Win32.h"
		"./include/win32/K15_RenderWindow_Win32.h"
		"./include/win32/K15_TextConsoleLog_Win32.h"
		"./include/win32/K15_VisualStudioLog_Win32.h")

	set(OS_ID "win32")

elseif(UNIX)
	set(OS_SRC "./src/linux/K15_DynamicLibrary_Linux.cpp")
	set(OS_INC "./include/linux/K15_DynamicLibrary_Linux.h")

	set(OS_ID "linux")

elseif(BUILD_ANDROID)
	set(OS_SRC "./src/linux/K15_DynamicLibrary_Linux.cpp"
		"./src/android/K15_Keyboard_Android.cpp"
 	     	"./src/android/K15_Logcat_Android.cpp"
	     	"./src/android/K15_Mouse_Android.cpp"
	     	"./src/android/K15_OSLayer_Android.cpp"
	     	"./src/android/K15_RenderWindow_Android.cpp"
	     	"./src/android/K15_ResourceArchive_Android.cpp")

	set(OS_INC "./include/android/K15_Logcat_Android.h"
		"./include/android/K15_OSLayer_Android.h"
		"./include/android/K15_OSLayer_Android.inl"
		"./include/android/K15_RenderWindow_Android.h"
		"./include/android/K15_RenderWindow_Android.inl"
		"./include/android/K15_ResourceArchiveAndroid.h")

	set(OS_ID "android")
endif()


#Define Include Directories
include_directories(./include/)
include_directories(../dependencies/enet/include)
include_directories(../dependencies/detour/include)
include_directories(../dependencies/jsoncpp/include)
include_directories(../dependencies/libtiff/include)
include_directories(../dependencies/minizip/include)
include_directories(../dependencies/tinyobjloader/include)
include_directories(../dependencies/glm/)

if(CPP11_IS_AVAILABLE)
	include_directories(../dependencies/tinythread/include)
endif()

source_group("${OS_ID}\\include" FILES ${OS_INC})
source_group("${OS_ID}\\src" FILES ${OS_SRC})
source_group("src" FILES ${CORE_SRC})
source_group("include" FILES ${CORE_INC})

if(BUILD_ANDROID) #When building for android, the core will be statically linked
	add_library(k15core STATIC ${CORE_SRC} ${CORE_INC} ${OS_SRC} ${OS_INC})
else()
	add_library(k15core SHARED ${CORE_SRC} ${CORE_INC} ${OS_SRC} ${OS_INC})
endif() 
