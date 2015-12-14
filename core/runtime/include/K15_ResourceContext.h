#ifndef _K15_Runtime_Resource_Context_h_
#define _K15_Runtime_Resource_Context_h_

#include "K15_RuntimePrerequisites.h"
#include "K15_CustomMemoryAllocator.h"

#include "generated/K15_ResourceStretchBuffer.h"

struct K15_ResourceArchive;
struct K15_ResourceFileData;
struct K15_ResourceData;
struct K15_ResourceCompilerContext;

typedef result8 (*K15_ResourceExistsInArchiveFnc)(K15_ResourceContext*, K15_ResourceArchive*, const char*);
typedef uint32 (*K15_GetResourceSizeFromArchiveFnc)(K15_ResourceContext*, K15_ResourceArchive*, const char*);
typedef byte* (*K15_GetResourceDataFromArchiveFnc)(K15_ResourceContext*, K15_ResourceArchive*, uint32, const char*);

typedef void (*K15_LoadResourceFromLoaderFnc)(K15_ResourceContext*, K15_ResourceFileData*, K15_ResourceData*);
typedef void (*K15_UnloadResourceFromLoaderFnc)(K15_ResourceContext*, K15_Resource*);
typedef void (*K15_LoadBackupResourceFromLoaderFnc)(K15_ResourceContext*, K15_Resource*);

/*********************************************************************************/
struct K15_ResourceFileData
{
	const char* path;
	byte* fileContent;
	uint32 fileContentSizeInBytes;
};
/*********************************************************************************/
struct K15_ResourceData
{
	byte* compiledResourceData;
	uint32 compiledResourceDataSizeInBytes;
};
/*********************************************************************************/
struct K15_Resource
{
	K15_ResourceHandle handle;
	K15_ResourceData resourceData;
	K15_ResourceFileData resourceFileData;
	uint32 identifier;
	uint32 refCount;
	uint32 flags;
};
/*********************************************************************************/
struct K15_ResourceLoader
{
	const char* name;
	uint32 identifier;	//custom identifier
	K15_LoadResourceFromLoaderFnc resourceLoader;
	K15_UnloadResourceFromLoaderFnc resourceUnloader;
	K15_LoadBackupResourceFromLoaderFnc resourceBackupLoader;
};
/*********************************************************************************/
struct K15_ResourceArchive
{
	K15_ResourceExistsInArchiveFnc resourceExists;		//Function to check if a resource exist within a resource context (directory/packed resource file).
	K15_GetResourceSizeFromArchiveFnc getResourceSize;	//Function to get the size of a resource within a resource context (directory/packed resource file).
	K15_GetResourceDataFromArchiveFnc getResourceData;	//Function to load a resource within a resource context (directory/packed resource file).
	char* resourcePath;									//Path to the resource context (directory/packed resource file).
};
/*********************************************************************************/
struct K15_ResourceContext
{
	K15_RenderContext* renderContext;							//render context for accessing render stuff
	K15_RenderCommandQueue* commandQueue;						//render command queue for queuing loading stuff (e.g. textures)
	K15_ResourceArchive resourceArchive;
	K15_CustomMemoryAllocator memoryAllocator;					//Allocator used to get memory for the resource memory.
	K15_AsyncContext* asyncContext;
	K15_Resource* resourceCache[K15_MAX_RESOURCES];
	K15_ResourceLoader resourceLoader[K15_MAX_RESOURCE_LOADER];	//Collection of different resource loader (provided by app)
	K15_Mutex* resourceCacheLock;								//Mutex to control resource cache access.
	
#ifdef K15_ENABLE_RUNTIME_RESOURCE_COMPILATION
	K15_ResourceCompilerContext* resourceCompiler;
#endif //K15_ENABLE_RUNTIME_RESOURCE_COMPILATION

	uint32 numResourceLoader;
	uint32 flags;
};
/*********************************************************************************/

K15_ResourceLoader* K15_AddResourceLoader(K15_ResourceContext* p_ResourceContext, const char* p_Name, uint32 p_ResourceIdentifier, K15_LoadResourceFromLoaderFnc p_ResourceLoaderFnc);

K15_ResourceData* K15_GetResourceData(K15_ResourceContext* p_ResourceContext, K15_ResourceHandle p_ResourceHandle);
K15_RenderResourceHandle* K15_GetResourceRenderHandle(K15_ResourceContext* p_ResourceContext, K15_ResourceHandle p_ResourceHandle);
K15_RenderMaterialDesc* K15_GetResourceRenderMaterialDesc(K15_ResourceContext* p_ResourceContext, K15_ResourceHandle p_ResourceHandle);
K15_RenderFontDesc* K15_GetResourceFontDesc(K15_ResourceContext* p_ResourceContext, K15_ResourceHandle p_ResourceHandle);

K15_ResourceContext* K15_CreateResourceContext(K15_AsyncContext* p_AsyncContext, K15_RenderContext* p_RenderContext, const char* p_ResourceArchivePath);
K15_ResourceContext* K15_CreateResourceContextWithCustomAllocator(K15_AsyncContext* p_AsyncContext, K15_RenderContext* p_RenderContext, const char* p_ResourceArchivePath, K15_CustomMemoryAllocator p_CustomMemoryAllocator);

K15_ResourceHandle K15_LoadResource(K15_ResourceContext* p_ResourceContext, uint32 p_ResourceIdentifier, const char* p_ResourcePath, uint32 p_Flags);
void K15_UnloadResource(K15_ResourceContext* p_ResourceContext, K15_ResourceHandle p_ResourceHandle);
//void K15_LoadMaterialResource(K15_ResourceContext* p_ResourceContext, byte* p_InputBuffer, uint32 p_InputBufferSize, byte** p_OutputBuffer, uint32* p_OutputBufferSize);

K15_AsyncOperation* K15_AsyncLoadResource(K15_ResourceHandle* p_ResourceHandlePtr, uint32 p_ResourceIdentifier, K15_ResourceContext* p_ResourceContext, const char* p_ResourcePath, uint32 p_ResourceFlags, uint32 p_AsyncFlags);

#endif //_K15_Runtime_Resource_Context_h_
