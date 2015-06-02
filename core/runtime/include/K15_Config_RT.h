#ifndef _K15_Runtime_Config_h_
#define _K15_Runtime_Config_h_

//you can provide your own malloc fnc if you want
#define K15_RT_MALLOC(size) malloc(size)
#define K15_RT_FREE(ptr) free(ptr)

#define K15_DEFAULT_RESOURCE_CONTEXT_RESOURCE_BUFFER_SIZE size_megabyte(5)

#endif //_K15_Runtime_Config_h_