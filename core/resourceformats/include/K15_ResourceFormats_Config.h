#ifndef _K15_ResourceFormat_Config_h_
#define _K15_ResourceFormat_Config_h_

#ifndef K15_RESOURCEFORMATS_USE_CUSTOM_ALLOCATOR
	#define K15_RF_MALLOC(bytes)  malloc(bytes)
	#define K15_RF_FREE(ptr)	  free(ptr)
#endif //K15_RESOURCEFORMATS_USE_CUSTOM_ALLOCATOR

#define K15_RESOURCEFORMATS_USE_ZLIB

#endif //_K15_ResourceFormat_Config_h_