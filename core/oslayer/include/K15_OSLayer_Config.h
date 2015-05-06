#ifndef _K15_OSLayer_Config_h_
#define _K15_OSLayer_Config_h_

#ifndef K15_OSLAYER_USE_CUSTOM_ALLOCATORS
	#define K15_OS_MALLOC(bytes)	malloc(bytes)
	#define K15_OS_FREE(ptr)		free(ptr)
#endif //K15_OSLAYER_USE_CUSTOM_ALLOCATORS

//undef to disable jni exception checks after each jni call
#define K15_CHECK_JNI_CALLS

#define K15_MAX_CONTROLLER 4

#endif //_K15_OSLayer_Config_h_