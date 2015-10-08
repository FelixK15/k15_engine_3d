#ifndef _K15_Runtime_Config_h_
#define _K15_Runtime_Config_h_

//you can provide your own malloc fnc if you want
#define K15_RT_MALLOC(size) malloc(size)
#define K15_RT_FREE(ptr) free(ptr)

#define K15_DEFAULT_RESOURCE_CONTEXT_RESOURCE_BUFFER_SIZE size_megabyte(5)

#define K15_DEFAULT_ASYNC_OPERATION_MEMORY_POOL_ELEMENTS 512

#define K15_MAX_RESOURCE_LOADER 8
#define K15_MAX_RESOURCES 2048
#define K15_INVALID_RESOURCE_HANDLE 0xFFFFFFFF

#define K15_MATERIAL_RESOURCE_IDENTIFIER 1
#define K15_TEXTURE_RESOURCE_IDENTIFIER 2
#define K15_SAMPLER_RESOURCE_IDENTIFIER 3
#define K15_FONT_RESOURCE_IDENTIFIER 4
#define K15_MESH_RESOURCE_IDENTIFIER 5
#define K15_SHADER_RESOURCE_IDENTIFIER 6

typedef unsigned int K15_ResourceHandle;

#endif //_K15_Runtime_Config_h_