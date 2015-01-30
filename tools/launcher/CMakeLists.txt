set(LAUNCHER_SRC "src/win32/K15_LauncherWin32.cpp")

source_group(src\\win32 FILES ${LAUNCHER_SRC})

include_directories(../../dependencies/os_layer/include)
include_directories(../../engine/core/include)
include_directories(../../dependencies/logging/include)

if(COMPILER_IS_MSVC)
	add_executable(launcher WIN32 ${LAUNCHER_SRC})
endif()

target_link_libraries(launcher os_layer logging)