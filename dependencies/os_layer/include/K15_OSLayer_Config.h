#ifndef _K15_OSLayer_Config_h_
#define _K15_OSLayer_Config_h_

#ifndef K15_OSLAYER_USE_CUSTOM_ALLOCATORS
	#define K15_OS_MALLOC(bytes)	malloc(bytes)
	#define K15_OS_FREE(ptr)		free(ptr)
#endif //K15_OSLAYER_USE_CUSTOM_ALLOCATORS

#endif //_K15_OSLayer_Config_h_