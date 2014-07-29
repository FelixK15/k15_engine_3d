#set all the libraries we need
link_directories("${DEPENDENCY_PATH}")

find_library(ENET_LIB NAMES enet libenet)
find_library(DETOUR_LIB NAMES detour libdetour)
find_library(JSONCPP_LIB NAMES jsoncpp libjsoncpp)
find_library(TIFF_LIB NAMES libtiff)
find_library(MINIZIP_LIB NAMES minizip libminizip)
find_library(OBJLOADER_LIB NAMES tinyobjloader libtinyonjloader)
find_library(ZLIB_LIB NAMES zlib libzlib)

if("${ENET_LIB}" STREQUAL "ENET_LIB-NOTFOUND")
        message(SEND_ERROR "Could not found library 'enet' ... Wrong DEPENDENCY_PATH?")
else()
        message(STATUS "Successfully found library 'enet' (${ENET_LIB}).")
endif()

if("${DETOUR_LIB}" STREQUAL "DETOUR_LIB-NOTFOUND")
	message(SEND_ERROR "Could not found library 'detour' ... Wrong DEPENDENCY_PATH?")
else()
	message(STATUS "Successfully found library 'detour' (${DETOUR_LIB}).")
endif()

if("${JSONCPP_LIB}" STREQUAL "JSONCPP_LIB-NOTFOUND")
	message(SEND_ERROR "Could not found library 'jsoncpp' ... Wrong DEPENDENCY_PATH?")
else()
	message(STATUS "Successfully found library 'jsoncpp' (${JSONCPP_LIB}).")
endif()

if("${TIFF_LIB}" STREQUAL "TIFF_LIB-NOTFOUND")
	message(SEND_ERROR "Could not found library 'libtiff' ... Wrong DEPENDENCY_PATH?")
else()
	message(STATUS "Successfully found library 'libtiff' (${TIFF_LIB}).")
endif()

if("${MINIZIP_LIB}" STREQUAL "MINIZIP_LIB-NOTFOUND")
	message(SEND_ERROR "Could not found library 'minizip' ... Wrong DEPENDENCY_PATH?")
else()
	message(STATUS "Successfully found library 'minizip' (${MINIZIP_LIB}).")
endif()

if("${OBJLOADER_LIB}" STREQUAL "OBJLOADER_LIB-NOTFOUND")
	message(SEND_ERROR "Could not found library 'tinyobjloader' ... Wrong DEPENDENCY_PATH?")
else()
	message(STATUS "Successfully found library 'tinyobjloader' (${OBJLOADER_LIB}).")
endif()

if("${ZLIB_LIB}" STREQUAL "ZLIB_LIB-NOTFOUND")
	message(SEND_ERROR "Could not found library 'zlib' ... Wrong DEPENDENCY_PATH?")
else()
	message(STATUS "Successfully found library 'zlib' (${ZLIB_LIB}).")
endif()

set(ENGINE_LIBS "${ENGINE_LIBS}"
"${ENET_LIB}"
"${DETOUR_LIB}"
"${JSONCPP_LIB}"
"${TIFF_LIB}"
"${MINIZIP_LIB}"
"${OBJLOADER_LIB}"
"${ZLIB_LIB}"
"k15math"
)
