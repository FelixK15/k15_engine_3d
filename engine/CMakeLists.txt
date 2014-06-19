#Save All *.cpp files into variable CORE_SRC
file(GLOB CORE_SRC "./core/src/*.cpp")
file(GLOB CORE_INC "./core/include/*.h" "./include/*.inl")

file(GLOB RENDER_SRC "./rendering/src/*.cpp")
file(GLOB RENDER_INC "./rendering/include/*.h ./rendering/include/*.inl")

file(GLOB RESOURCE_SYSTEM_SRC "./resource_system/src/*.cpp")
file(GLOB RESOURCE_SYSTEM_INC "./resource_system/include/*.h" "./resource_system/include/*.inl")

file(GLOB SERIALIZATION_SRC "./serialization/src/*.cpp")
file(GLOB SERIALIZATION_INC "./serialization/include/*.h" "./serialization/include/*.inl")

file(GLOB COMPONENTS_SRC "./components/src/*.cpp")
file(GLOB COMPONENTS_INC "./components/include/*.h" "./component/include/*.inl")

#Add OS Specific files
if(WIN32)
	set(OS_SRC "./core/src/win32/K15_DynamicLibrary_Win32.cpp"
	     	"./core/src/win32/K15_Keyboard_Win32.cpp"
	     	"./core/src/win32/K15_Mouse_Win32.cpp"
	     	"./core/src/win32/K15_OSLayer_Win32.cpp"
	     	"./core/src/win32/K15_RenderWindow_Win32.cpp"
	     	"./core/src/win32/K15_TextConsoleLog_Win32.cpp"
	     	"./core/src/win32/K15_VisualStudioLog_Win32.cpp")

	set(OS_INC "./core/include/win32/K15_DynamicLibrary_Win32.h"
		"./core/include/win32/K15_OSLayer_Win32.h"
		"./core/include/win32/K15_RenderWindow_Win32.h"
		"./core/include/win32/K15_TextConsoleLog_Win32.h"
		"./core/include/win32/K15_VisualStudioLog_Win32.h")

	set(OS_ID "win32")

elseif(UNIX)
	set(OS_SRC "./core/src/linux/K15_DynamicLibrary_Linux.cpp")
	set(OS_INC "./core/include/linux/K15_DynamicLibrary_Linux.h")

	set(OS_ID "linux")

elseif(BUILD_ANDROID)
	set(OS_SRC "./core/src/linux/K15_DynamicLibrary_Linux.cpp"
		"./core/src/android/K15_Keyboard_Android.cpp"
 	     	"./core/src/android/K15_Logcat_Android.cpp"
	     	"./core/src/android/K15_Mouse_Android.cpp"
	     	"./core/src/android/K15_OSLayer_Android.cpp"
	     	"./core/src/android/K15_RenderWindow_Android.cpp"
	     	"./resource_system/src/android/K15_ResourceArchive_Android.cpp")

	set(OS_INC "./core/include/android/K15_Logcat_Android.h"
		"./coreinclude/android/K15_OSLayer_Android.h"
		"./core/include/android/K15_OSLayer_Android.inl"
		"./core/include/android/K15_RenderWindow_Android.h"
		"./core/include/android/K15_RenderWindow_Android.inl"
		"./resource_system/include/android/K15_ResourceArchiveAndroid.h")

	set(OS_ID "android")
endif()


#Define Include Directories
include_directories(./core/include/)
include_directories(./rendering/include/)
include_directories(./resource_system/include/)
include_directories(./components/include)
include_directories(./serialization/include)

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

source_group("core\\include" FILES ${CORE_INC})
source_group("core\\src" FILES ${CORE_SRC})

source_group("${OS_ID}\\include" FILES ${OS_INC})
source_group("${OS_ID}\\src" FILES ${OS_SRC})

source_group("resource_system\\include" FILES ${RESOURCE_SYSTEM_INC})
source_group("resource_system\\src" FILES ${RESOURCE_SYSTEM_SRC})

source_group("rendering\\include" FILES ${RENDERING_INC})
source_group("rendering\\src" FILES ${RENDERING_SRC})

source_group("components\\include" FILES ${COMPONENTS_INC})
source_group("components\\src" FILES ${COMPONENTS_SRC})

if(BUILD_ANDROID) #When building for android, the core will be statically linked
	add_library(core STATIC ${CORE_SRC} ${CORE_INC} ${OS_SRC} ${OS_INC} ${RESOURCE_SYSTEM_INC} ${RESOURCE_SYSTEM_SRC} ${RENDERING_INC} ${RENDERING_SRC} ${COMPONENTS_INC} ${COMPONENTS_SRC})
else()
	add_library(core SHARED ${CORE_SRC} ${CORE_INC} ${OS_SRC} ${OS_INC} ${RESOURCE_SYSTEM_INC} ${RESOURCE_SYSTEM_SRC} ${RENDERING_INC} ${RENDERING_SRC} ${COMPONENTS_INC} ${COMPONENTS_SRC})
endif() 
