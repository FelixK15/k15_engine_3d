#ifndef _K15_Runtime_Resource_Context_h_
#define _K15_Runtime_Resource_Context_h_

#include "K15_RuntimePrerequisites.h"

#include "generated/K15_ResourceFixedBuffer.h"

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
	K15_ResourceFixedBuffer resourceCache;
	K15_ResourceExistsFnc resourceExists;
	K15_GetResourceSizeFnc resourceSize;
	K15_LoadResourceFnc loadResource;
	K15_MemoryBuffer* resourceMemoryBuffer;
	K15_CustomMemoryAllocator* memoryAllocator; 

	K15_Mutex* resourceCacheLock;

	char* resourcePath;
	uint32 flags;
};
/*********************************************************************************/

K15_ResourceContext* K15_CreateResourceContext(const char* p_ResourceCollectionPath, uint32 p_Flags);
K15_ResourceContext* K15_CreateResourceContextWithCustomAllocator(const char* p_ResourceCollectionPath, K15_CustomMemoryAllocator* p_CustomMemoryAllocator, uint32 p_Flags);

K15_Resource* K15_LoadResource(K15_ResourceContext* p_ResourceContext, const char* p_ResourcePath, uint32 p_Flags);
K15_AsyncOperation* K15_AsyncLoadResource(K15_AsyncContext* p_AsyncContext, K15_Resource** p_ResourcePtr, K15_ResourceContext* p_ResourceContext, const char* p_ResourcePath, uint32 p_ResourceFlags, uint32 p_AsyncFlags);

#endif //_K15_Runtime_Resource_Context_h_