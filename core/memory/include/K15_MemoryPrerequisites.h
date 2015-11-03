#ifndef _K15_Memory_Prerequisites_h_
#define _K15_Memory_Prerequisites_h_

#include "K15_Common.h"

#define K15_TRACK_ALLOCATOR_NAMES

struct K15_CustomMemoryAllocator;
struct K15_MemoryBuffer;

typedef void*(*K15_MallocFnc)(size_t p_SizeInBytes, void* p_UserData);
typedef void(*K15_FreeFnc)(void* p_Pointer, void* p_UserData);
typedef void(*K15_ClearFnc)(void* p_UserData);
#endif //_K15_Memory_Prerequisites_h_