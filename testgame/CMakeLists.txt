set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR})

include_directories(include/)
include_directories(../core/oslayer/include/)
include_directories(../core/runtime/include/)
include_directories(../core/rendering/include/)
include_directories(../core/logging/include/)
include_directories(../core/resourceformats/include/)
include_directories(../core/math/include/)

set(K15_TESTGAME_SRC 			"src/K15_GameMain.cpp")
set(K15_TESTGAME_INCLUDE 		"")
set(K15_TESTGAME_COMPLETE_SRC 	${K15_TESTGAME_SRC} ${K15_TESTGAME_INCLUDE})
set(K15_TESTGAME_LIBS 			oslayer runtime logging resourceformats math)

source_group(include FILES ${K15_TESTGAME_INCLUDE})
source_group(src FILES ${K15_TESTGAME_SRC})


if(${K15_BUILD_DYNAMIC_GAME_LIBRARY} EQUAL 1)
	add_library(testgame SHARED ${K15_TESTGAME_COMPLETE_SRC})
	target_link_libraries(testgame ${K15_TESTGAME_LIBS})
else()
	add_library(testgame STATIC ${K15_TESTGAME_COMPLETE_SRC})
endif()

set_target_properties(testgame PROPERTIES FOLDER samples)
