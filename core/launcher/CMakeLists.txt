include_directories(./../)

include_directories(../rendering/include)

include_directories(../oslayer/include)
include_directories(../logging/include)
include_directories(../runtime/include)
include_directories(../memory/include)

set(LAUNCHER_LIBRARIES resourceformats oslayer logging guilayer rendering runtime string zlib memory libresourcecompiler)

if(${K15_BUILD_DYNAMIC_GAME_LIBRARY} EQUAL 0)
	set(LAUNCHER_LIBRARIES ${LAUNCHER_LIBRARIES} testgame)
else()
	add_definitions(-DK15_LOAD_GAME_LIB_DYNAMIC)
endif()

if (${K15_PLATFORM} STREQUAL "Android")
	set(LAUNCHER_SRC_ANDROID "android/jni/K15_LauncherAndroid.cpp")
	set(LAUNCHER_SRC_ANDROID_JAVA "${CMAKE_CURRENT_BINARY_DIR}/src/k15games/engine/launcher/LauncherMainActivity.java")
	set(LAUNCHER_SRC_ANDROID_FILES "AndroidManifest.xml")

	set(LAUNCHER_LIBRARIES ${LAUNCHER_LIBRARIES} android)

	configure_file(android/AndroidManifest.xml.in AndroidManifest.xml)
	configure_file(android/src/k15games/engine/launcher/LauncherMainActivity.java src/k15games/engine/launcher/LauncherMainActivity.java)

	source_group(android FILES ${LAUNCHER_SRC_ANDROID})
	source_group(android\\java FILES ${LAUNCHER_SRC_ANDROID_JAVA})
	source_group(android\\setup FILES ${LAUNCHER_SRC_ANDROID_FILES})
	add_executable(launcher ${LAUNCHER_SRC_ANDROID} ${LAUNCHER_SRC_ANDROID_JAVA} ${LAUNCHER_SRC_ANDROID_FILES})
endif()

if(${K15_PLATFORM} STREQUAL "Win32")
	set(LAUNCHER_SRC_WIN32 "win32/K15_LauncherWin32.cpp")

	source_group(win32 FILES ${LAUNCHER_SRC_WIN32})
	add_executable(launcher WIN32 ${LAUNCHER_SRC_WIN32})
endif()

target_link_libraries(launcher ${LAUNCHER_LIBRARIES})
