#Check Which Compiler will be used
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	#GCC
	set(COMPILER_IS_GCC 1)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	#Clang
	set(COMPILER_IS_CLANG 1)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	#MSVC
	set(COMPILER_IS_MSVC 1)
endif()

if(COMPILER_IS_GCC)
	message(STATUS "Compiler found: GCC")
elseif(COMPILER_IS_CLANG)
	message(STATUS "Compiler found: CLANG")
elseif(COMPILER_IS_MSVC)
	message(STATUS "Compiler found: MSVC")
else()
	message(STATUS "Compiler found: UNKNOWN")
endif()

#Check if c++11 is supported
if(COMPILER_IS_GCC AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.7)
	set(CPP11_IS_AVAILABLE 1)
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=gnu++11")
elseif(COMPILER_IS_MSVC AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10.0)
	set(CPP11_IS_AVAILABLE 1)
elseif(COMPILER_IS_CLANG AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.0)
	set(CPP11_IS_AVAILABLE 1)
endif()

#message whether c++11 is supported or not.
if(CPP11_IS_AVAILABLE)
	message(STATUS "C++11 is available")
else()
	message(STATUS "C++11 is not available")
endif()
