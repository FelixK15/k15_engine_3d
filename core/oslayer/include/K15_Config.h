#ifndef _K15_Config_h_
#define _K15_Config_h_

#ifndef K15_OSLAYER_USE_CUSTOM_ALLOCATORS
	#define K15_OS_MALLOC(bytes)	malloc(bytes)
	#define K15_OS_FREE(ptr)		free(ptr)
#endif //K15_OSLAYER_USE_CUSTOM_ALLOCATORS

//comment to disable jni exception checks after each jni call
#define K15_CHECK_JNI_CALLS

//comment to use plain c asserts
#define K15_USE_CUSTOM_ASSERTS

//comment to disable asserts all together
#define K15_ENABLE_ASSERTS

//comment to disable automatic logging during K15_CHECK_RESULT.
#define K15_LOG_ON_RESULT_CHECK

#ifdef K15_OS_WINDOWS
	//#define K15_LOAD_GAME_LIB_DYNAMIC
	//#define K15_USE_DETERMINISTIC_GAME_MEM_ADDRESS
#endif //K15_OS_WINDOWS

#define K15_MAX_CONTROLLER 4

#endif //_K15_Config_h_
