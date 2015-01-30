set(K15_OS_LAYER_SRC "src/K15_OSLayer_SystemEvents.cpp"
					 "src/K15_OSLayer_Window.cpp"
					 "src/K15_OSLayer_OSContext.cpp")

set(K15_OS_LAYER_WIN32_SRC  "src/win32/K15_EventsWin32.cpp"
							"src/win32/K15_WindowWin32.cpp"
							"src/win32/K15_EnvironmentWin32.cpp")

set(K15_OS_LAYER_INC "include/K15_OSLayer_EventTypes.h"
					 "include/K15_OSLayer_ErrorCodes.h"
					 "include/K15_OSLayer_Prerequisites.h"
					 "include/K15_OSLayer_Window.h"
					 "include/K15_OSLayer_SystemEvents.h"
					 "include/K15_OSLayer_OSContext.h")

set(K15_OS_LAYER_WIN32_INC 	"include/win32/K15_EventsWin32.h"
							"include/win32/K15_WindowWin32.h"
							"include/win32/K15_EnvironmentWin32.h")

include_directories(include)
include_directories(../logging/include)

source_group(include\\win32 FILES ${K15_OS_LAYER_WIN32_INC})
source_group(include FILES ${K15_OS_LAYER_INC})

source_group(src\\win32 FILES ${K15_OS_LAYER_WIN32_SRC})
source_group(src FILES ${K15_OS_LAYER_SRC})

add_library(os_layer ${K15_OS_LAYER_SRC} ${K15_OS_LAYER_INC} ${K15_OS_LAYER_WIN32_SRC} ${K15_OS_LAYER_WIN32_INC})