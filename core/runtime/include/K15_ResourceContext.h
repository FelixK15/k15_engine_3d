#ifndef _K15_Runtime_Resource_Context_h_
#define _K15_Runtime_Resource_Context_h_

#include "K15_RuntimePrerequisites.h"

#include "generated/K15_ResourceStretchBuffer.h"

typedef uint8 (*K15_ResourceExistsFnc)(K15_ResourceContext*, const char*);
typedef uint32 (*K15_GetResourceSizeFnc)(K15_ResourceContext*, const char*);
typedef byte* (*K15_LoadResourceFnc)(K15_ResourceContext*, uint32, const char*);

/*********************************************************************************/
struct K15_Resource
{
	char* resourceFilePath;
	byte* data;
	uint32 dataSizeInBytes;
	uint32 flags;
};
/*********************************************************************************/
struct K15_ResourceContext
{
	K15_ResourceExistsFnc resourceExists;			//Function to check if a resource exist within a resource context (directory/packed resource file).
	K15_GetResourceSizeFnc resourceSize;			//Function to get the size of a resource within a resource context (directory/packed resource file).
	K15_LoadResourceFnc loadResource;				//Function to load a resource within a resource context (directory/packed resource file).

	K15_CustomMemoryAllocator* memoryAllocator;		//Allocator used to get memory for the resource memory.
	K15_ResourceStretchBuffer resourceCache;		//Resource cache for accessing already loaded resources.
	K15_Mutex* resourceCacheLock;					//Mutex to control resource cache access.

	char* resourcePath;								//Path to the resource context (directory/packed resource file).
	uint32 flags;
};
/*********************************************************************************/

K15_ResourceContext* K15_CreateResourceContext(const char* p_ResourceCollectionPath, uint32 p_Flags);
K15_ResourceContext* K15_CreateResourceContextWithCustomAllocator(const char* p_ResourceCollectionPath, K15_CustomMemoryAllocator* p_CustomMemoryAllocator, uint32 p_Flags);

K15_Resource* K15_LoadResource(K15_ResourceContext* p_ResourceContext, const char* p_ResourcePath, uint32 p_Flags);
K15_AsyncOperation* K15_AsyncLoadResource(K15_AsyncContext* p_AsyncContext, K15_Resource** p_ResourcePtr, K15_ResourceContext* p_ResourceContext, const char* p_ResourcePath, uint32 p_ResourceFlags, uint32 p_AsyncFlags);

#endif //_K15_Runtime_Resource_Context_h_