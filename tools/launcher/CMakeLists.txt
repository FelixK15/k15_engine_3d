set(LAUNCHER_SRC_WIN32 "win32/K15_LauncherWin32.cpp")
set(LAUNCHER_SRC_ANDROID "android/K15_LauncherAndroid.cpp")

source_group(win32 FILES ${LAUNCHER_SRC_WIN32})
source_group(android FILES ${LAUNCHER_SRC_ANDROID})

include_directories(../../dependencies/os_layer/include)
include_directories(../../dependencies/rendering/include)
include_directories(../../engine/core/include)
include_directories(../../dependencies/logging/include)

if(${K15_PLATFORM} STREQUAL "Win32")
	add_executable(launcher WIN32 ${LAUNCHER_SRC_WIN32} ${LAUNCHER_SRC_ANDROID})
elseif(${K15_PLATFORM} STREQUAL "Android")
	configure_file(android/AndroidManifest.xml.in AndroidManifest.xml)
	configure_file(android/java/LauncherMainActivity.java src/k15games/engine/launcher/LauncherMainActivity.java)
	add_executable(launcher ${LAUNCHER_SRC_WIN32} ${LAUNCHER_SRC_ANDROID} AndroidManifest.xml android/java/LauncherMainActivity.java)
endif()

target_link_libraries(launcher os_layer logging rendering)