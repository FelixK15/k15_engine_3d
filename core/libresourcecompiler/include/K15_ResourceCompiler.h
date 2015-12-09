#ifndef _K15_ResourceCompiler_h_
#define _K15_ResourceCompiler_h_

#include "K15_ResourceCompilerConfig.h"
#include "K15_Common.h"

struct K15_FileWatchEntry;
struct K15_ResourceCompilerContext;
struct K15_AsyncContext;
struct K15_ResourceCompiler;
struct K15_Mutex;
struct K15_ConfigFileContext;

typedef bool8 (*K15_CompileResourceFnc)(K15_ResourceCompilerContext*, K15_ResourceCompiler*, K15_ConfigFileContext*, const char*);

/*********************************************************************************/
enum K15_ResourceCompilerType
{
	K15_RESOURCE_COMPILER_INVALID = -1,
	K15_RESOURCE_COMPILER_MESH = 0,
	K15_RESOURCE_COMPILER_TEXTURE,
	K15_RESOURCE_COMPILER_FONT,
	K15_RESOURCE_COMPILER_MATERIAL,
	K15_RESOURCE_COMPILER_SAMPLER,

	K15_RESOURCE_COMPILER_COUNT
};
/*********************************************************************************/
struct K15_ResourceDependency
{
	char* dependencyPath;
	char* resourcePath;

	K15_FileWatchEntry* fileWatchEntry;
	K15_ResourceCompilerContext* resourceCompilerContext;
};
/*********************************************************************************/
struct K15_ResourceCompilerContext
{
	K15_AsyncContext* asyncContext;
	K15_Mutex* dependencyMutex;

	K15_ResourceCompiler* resourceCompiler[K15_RESOURCE_COMPILER_COUNT];
};
/*********************************************************************************/
struct K15_ResourceCompiler
{
	K15_CompileResourceFnc compileResource;
	const char* name;
	char* error;
};
/*********************************************************************************/
struct K15_ResourceCompilerAsyncParameter
{
	K15_ResourceCompilerContext* resourceCompilerContext;
	char* resourceFilePath;
};
/*********************************************************************************/

bool8 K15_CompileResource(K15_ResourceCompilerContext* p_ResourceCompilerContext, const char* p_ResourceFile);
void K15_CompileResourceAsync(void* p_ThreadParameter);
void K15_CompileResources(K15_ResourceCompilerContext* p_ResourceCompilerContext, char** p_FilesToCompile, uint32 p_NumFilesToCompile);


#endif //_K15_ResourceCompiler_h_