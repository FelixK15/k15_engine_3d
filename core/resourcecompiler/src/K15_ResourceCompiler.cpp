#ifdef K15_RESOURCE_COMPILER_ENABLE_ASSIMP
	#include "assimp/Importer.hpp"
	#include "assimp/scene.h"
	#include "assimp/postprocess.h"
#else
	#error missing mesh compiler implementation	
#endif //K15_RESOURCE_COMPILER_ENABLE_ASSIMP

#ifdef K15_RESOURCE_COMPILER_ENABLE_SQUISH
	#include "squish.h"
#else
	#error missing texture compiler implementation	
#endif //K15_RESOURCE_COMPILER_ENABLE_SQUISH

#ifdef K15_RESOURCE_COMPILER_ENABLE_STB_IMAGE
	#define STB_IMAGE_IMPLEMENTATION
	#define STB_IMAGE_RESIZE_IMPLEMENTATION
	#define STB_IMAGE_WRITE_IMPLEMENTATION
	#define STBI_FAILURE_USERMSG
	#define STBI_ONLY_JPEG
	#define STBI_ONLY_PNG
	#define STBI_ONLY_TGA
	#include "stb_image.h"
	#include "stb_image_resize.h"
	#include "stb_image_write.h"
#else
	#error missing texture loader implementation	
#endif //K15_RESOURCE_COMPILER_ENABLE_STB_IMAGE

#ifdef K15_RESOURCE_COMPILER_ENABLE_STB_TTF
	#define STB_TRUETYPE_IMPLEMENTATION
	#include "stb_truetype.h"
#else
	#error missing font loader implementation	
#endif //K15_RESOURCE_COMPILER_ENABLE_STB_TTF

#include "K15_CustomMemoryAllocator.h"
#include "K15_TextureFormat.h"
#include "K15_FontFormat.h"
#include "K15_SamplerFormat.h"
#include "K15_MaterialFormat.h"
#include "K15_TextureAtlas.h"
#include "K15_System.h"

//forward decl
struct K15_ResourceCompilerContext;
void K15_CompileResources(K15_ResourceCompilerContext* p_ResourceCompilerContext, char** p_FilesToCompile, uint32 p_NumFilesToCompile);

/*********************************************************************************/
struct K15_ResourceDependency
{
	char* dependencyPath;
	char* resourcePath;

	K15_FileWatchEntry* fileWatchEntry;
	K15_ResourceCompilerContext* resourceCompilerContext;
};
/*********************************************************************************/

#include "K15_TextureAtlas.cpp"
#include "generated/K15_ResourceDependencyStretchBuffer.cpp"

struct K15_ResourceCompiler;

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
struct K15_ResourceCompiler
{
	K15_CompileResourceFnc compileResource;
	const char* name;
	char* error;
};
/*********************************************************************************/
struct K15_ResourceCompilerContext
{
	K15_ResourceDependencyStretchBuffer resourceDependencyStretchBuffer;
	K15_ResourceDependencyStretchBuffer bufferedResourceDependencyStretchBuffer;
	K15_ArgumentParser* argumentParser;
	K15_AsyncContext* asyncContext;
	K15_Mutex* dependencyMutex;

	K15_ResourceCompiler* resourceCompiler[K15_RESOURCE_COMPILER_COUNT];
};
/*********************************************************************************/
struct K15_ResourceCompilerAsyncParameter
{
	K15_ResourceCompilerContext* resourceCompilerContext;
	char* resourceFilePath;
};
/*********************************************************************************/



/*********************************************************************************/
intern void K15_InternalOnResourceFileChanged(const char* p_ResourceFilePath, void* p_UserData)
{
	K15_ResourceCompilerContext* compilerContext = (K15_ResourceCompilerContext*)p_UserData;

	//filter for k15resourceinfo files
	if (K15_IsSubstringR(p_ResourceFilePath, ".k15resourceinfo"))
	{
		K15_CompileResources(compilerContext, K15_CreateStringArrayIntoBuffer((char**)alloca(K15_PTR_SIZE), 1, p_ResourceFilePath), 1);
	}
}
/*********************************************************************************/
intern void K15_InternalOnDependencyResourceFileChanged(const char* p_DependencyResourceFilePath, void* p_UserData)
{
	K15_ResourceDependency* resourceDependency = (K15_ResourceDependency*)p_UserData;
	K15_ResourceCompilerContext* resourceCompilerContext = resourceDependency->resourceCompilerContext;
	
	K15_ResourceDependencyStretchBuffer* resourceDependencyStretchBuffer = &resourceCompilerContext->resourceDependencyStretchBuffer;

	char** filesToRecompile = 0;
	uint32 numFilesToRecompile = 0;
	uint32 numResourceDependencies = resourceDependencyStretchBuffer->numElements;
	//iterate over all dependencies and get the actual resource file which we want to recompile
	//we need two passes. one to get the amount of files associated with the current dependency 
	//and another to add the files to an array.
	for (uint32 resourceDependencyIndex = 0;
		resourceDependencyIndex < numResourceDependencies;
		++resourceDependencyIndex)
	{
		K15_ResourceDependency* currentDependency = K15_GetResourceDependencyStretchBufferElementUnsafe(resourceDependencyStretchBuffer, resourceDependencyIndex);

		//if dependency paths match, add to list of resources that will get recompiled
		if (strcmp(currentDependency->dependencyPath, resourceDependency->dependencyPath) == 0)
		{
			numFilesToRecompile += 1;
		}
	}

	filesToRecompile = (char**)alloca(numFilesToRecompile * K15_PTR_SIZE);
	numFilesToRecompile = 0;

	//2nd pass
	for (uint32 resourceDependencyIndex = 0;
		resourceDependencyIndex < numResourceDependencies;
		++resourceDependencyIndex)
	{
		K15_ResourceDependency* currentDependency = K15_GetResourceDependencyStretchBufferElementUnsafe(resourceDependencyStretchBuffer, resourceDependencyIndex);

		//if dependency paths match, add to list of resources that will get recompiled
		if (strcmp(currentDependency->dependencyPath, resourceDependency->dependencyPath) == 0)
		{
			filesToRecompile[numFilesToRecompile++] = currentDependency->resourcePath;
		}
	}

	//recompile all affected resource files
	K15_CompileResources(resourceCompilerContext, filesToRecompile, numFilesToRecompile);
}
/*********************************************************************************/
intern void K15_InternalAddResourceDependencyFileWatch(K15_ResourceCompilerContext* p_ResourceCompilerContext, K15_ResourceDependency* p_ResourceDependency)
{
	//create link between dependency and compiler context
	p_ResourceDependency->resourceCompilerContext = p_ResourceCompilerContext;

	const char* dependencyFilePath = p_ResourceDependency->dependencyPath;

	//Check if the file is already being watched.
	if (!K15_FileIsBeingWatched(dependencyFilePath))
	{
		//add file watch to the dependency file (resourcedependency variable as parameter)
		K15_FileWatchEntry* watchEntry = K15_AddFileWatch(dependencyFilePath, K15_InternalOnDependencyResourceFileChanged, p_ResourceDependency);

		//save filewatch so we can delete it later
		p_ResourceDependency->fileWatchEntry = watchEntry;
	}
}
/*********************************************************************************/
intern uint8 K15_InternalResourceDependencyComparer(K15_ResourceDependency* p_Element, void* p_UserData)
{
	K15_ResourceDependency* userDependency = (K15_ResourceDependency*)p_UserData;
	bool8 found = strcmp(userDependency->dependencyPath, p_Element->dependencyPath) == 0 && strcmp(userDependency->resourcePath, p_Element->resourcePath) == 0;
	return found ? 0 : 1;
}
/*********************************************************************************/
intern void K15_InternalAddResourceDependency(K15_ResourceCompilerContext* p_ResourceCompilerContext, char* p_ResourcePath, char* p_DependencyPath)
{
	K15_ResourceDependencyStretchBuffer* resourceDependencyBuffer = &p_ResourceCompilerContext->resourceDependencyStretchBuffer;
	K15_ResourceDependencyStretchBuffer* bufferedResourceDependencyBuffer = &p_ResourceCompilerContext->bufferedResourceDependencyStretchBuffer;
	K15_ResourceDependency resourceDependency = {};

	resourceDependency.dependencyPath = K15_CopyString(p_DependencyPath);
	resourceDependency.resourcePath = K15_CopyString(p_ResourcePath);

	K15_LockMutex(p_ResourceCompilerContext->dependencyMutex);
		K15_ResourceDependency* existingDependency = K15_GetResourceDependencyStretchBufferElementConditional(resourceDependencyBuffer, K15_InternalResourceDependencyComparer, &resourceDependency);

		//add dependency only, if it has not been added before
		if (!existingDependency)
		{
			//add dependency to the resource compiler contextg
			K15_PushResourceDependencyStretchBufferElement(bufferedResourceDependencyBuffer, resourceDependency);
		}
	K15_UnlockMutex(p_ResourceCompilerContext->dependencyMutex);
}
/*********************************************************************************/
intern int K15_InternalLog2(int x)
{
	return (int)(floor(log10(x) / log10(2)));
}
/*********************************************************************************/
intern bool8 K15_InternalCheckPowerOfTwo(uint32 p_Number)
{
	return ((p_Number != 0) && !(p_Number & (p_Number - 1)));
}
/*********************************************************************************/
intern uint32 K15_InternalGetMipMapCount(uint32 p_Width, uint32 p_Height)
{
	return max(0, K15_InternalLog2(max(p_Width, p_Height)) - 2); //no 1x1 and 2x2 mip maps
}
/*********************************************************************************/
intern uint32 K15_InternalCalculateImageMemorySizeUncompressed(uint32 p_Width, uint32 p_Height, uint32 p_ComponentCount)
{
	uint32 mipmapCount = K15_InternalGetMipMapCount(p_Width, p_Height) + 1;
	uint32 memoryNeeded = 0;

	for(uint32 mipmapIndex = 0;
		mipmapIndex < mipmapCount;
		++mipmapIndex)
	{
		memoryNeeded += p_Width * p_Height;

		p_Width /= 2;
		p_Height /= 2;
	}

	return memoryNeeded * p_ComponentCount;
}
/*********************************************************************************/
#ifdef K15_RESOURCE_COMPILER_ENABLE_SQUISH
intern uint32 K15_InternalCalculateImageMemorySizeCompressedSquish(uint32 p_Width, uint32 p_Height, int p_CompressionFlags)
{
	return squish::GetStorageRequirements(p_Width, p_Height, p_CompressionFlags);
}
#endif //K15_RESOURCE_COMPILER_ENABLE_SQUISH
/*********************************************************************************/
intern uint8 K15_InternalConvertTextToSamplerFilter(const char* p_Text)
{
	uint8 filter = 0xFF;

	if (strcmp(p_Text, "Linear") == 0)
	{
		filter = K15_FILTER_MODE_LINEAR;
	}
	else if (strcmp(p_Text, "Nearest") == 0)
	{
		filter = K15_FILTER_MODE_NEAREST;
	}

	return filter;
}
/*********************************************************************************/
intern uint8 K15_InternalConvertTextToSamplerAddressMode(const char* p_Text)
{
	uint8 addressMode = 0xFF;

	if (strcmp(p_Text, "Clamp") == 0)
	{
		addressMode = K15_ADDRESS_MODE_CLAMP;
	}
	else if (strcmp(p_Text, "Mirror") == 0)
	{
		addressMode = K15_ADDRESS_MODE_MIRROR;
	}
	else if (strcmp(p_Text, "Repeat") == 0)
	{
		addressMode = K15_ADDRESS_MODE_REPEAT;
	}

	return addressMode;
}
/*********************************************************************************/
intern bool8 K15_InternalIsStringValue(char* p_ValueText)
{
	char* value = p_ValueText;
	bool8 isStringValue = K15_FALSE;

	while(*value)
	{
		if (isalpha(*value++))
		{
			isStringValue = K15_TRUE;
			break;
		}
	}

	return isStringValue;
}
/*********************************************************************************/
intern bool8 K15_InternalIsFloatValue(char* p_ValueText)
{
	char* value = p_ValueText;
	bool8 isFloatValue = K15_TRUE;

	while(*value)
	{
		if (!(isdigit(*value) || 
			*value == '.' || 
			*value == ','))
		{
			isFloatValue = K15_FALSE;
			break;
		}

		*value++;
	}

	return isFloatValue;
}
/*********************************************************************************/
intern bool8 K15_InternalIsIntValue(char* p_ValueText)
{
	char* value = p_ValueText;
	bool8 isIntValue = K15_TRUE;

	while(*value)
	{
		if (!isdigit(*value++))
		{
			isIntValue = K15_FALSE;
			break;
		}
	}

	return isIntValue;
}
/*********************************************************************************/



/*********************************************************************************/
void K15_AddBufferResourceDependenciesToFileWatch(K15_ResourceCompilerContext* p_ResourceCompilerContext)
{
	K15_ResourceDependencyStretchBuffer* bufferedDependencyStretchBuffer = &p_ResourceCompilerContext->bufferedResourceDependencyStretchBuffer;
	K15_ResourceDependencyStretchBuffer* resourceDependencyStretchBuffer = &p_ResourceCompilerContext->resourceDependencyStretchBuffer;

	uint32 numElements = bufferedDependencyStretchBuffer->numElements;

	//only lock the buffered dependency buffer if there are buffered elements
	if (numElements > 0)
	{
		K15_LockMutex(p_ResourceCompilerContext->dependencyMutex);
		//add all buffered resource dependencies to the file watcher and add them
		//to the 'real' resource dependency buffer
		for(uint32 elementIndex = 0;
			elementIndex < numElements;
			++elementIndex)
		{
			//get resource dependency from the buffered array and add it to the file watch.
			K15_ResourceDependency* currentResourceDependency = K15_GetResourceDependencyStretchBufferElementUnsafe(bufferedDependencyStretchBuffer, elementIndex);

			//Add to the real resource dependency buffer
			currentResourceDependency = K15_PushResourceDependencyStretchBufferElement(resourceDependencyStretchBuffer, *currentResourceDependency);
			
			K15_InternalAddResourceDependencyFileWatch(p_ResourceCompilerContext, currentResourceDependency);
		}

		//clear the buffered resource dependency buffer
		K15_ClearResourceDependencyStretchBuffer(bufferedDependencyStretchBuffer);
		K15_UnlockMutex(p_ResourceCompilerContext->dependencyMutex);
	}
}
/*********************************************************************************/
void K15_SetResourceCompilerError(K15_ResourceCompiler* p_ResourceCompiler, const char* p_ErrorMessage)
{
	for(;;)
	{
		K15_InterlockedCompareExchangePointer((volatile PVOID*)&p_ResourceCompiler->error, (void*)p_ErrorMessage, 0);
		
		if (p_ResourceCompiler->error == p_ErrorMessage)
		{
			break;
		}
	}
}
/*********************************************************************************/
#ifdef K15_RESOURCE_COMPILER_ENABLE_ASSIMP
bool8 K15_CompileMeshResourceWithAssimp(K15_ResourceCompilerContext* p_ResourceCompilerContext, K15_ResourceCompiler* p_ResourceCompiler, K15_ConfigFileContext* p_MeshConfig, const char* p_OutputPath)
{
	Assimp::Importer importer;

	char* resourcePath = K15_GetConfigValueAsString(p_MeshConfig, "Source");

	if (!resourcePath)
	{
		K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("No mesh file has been specified for resource config file '%s' (Define 'Source' in the resource config file).", (char*)malloc(512), resourcePath));
		free(resourcePath);
		return K15_FALSE;
	}

	if (!K15_FileExists(resourcePath))
	{
		K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Could not find mesh file '%s'.", (char*)malloc(512), resourcePath));
		free(resourcePath);
		return K15_FALSE;
	}

	const aiScene* scene = importer.ReadFile(std::string(resourcePath), 
		aiProcess_CalcTangentSpace			|
		aiProcess_Triangulate				|
		aiProcess_GenNormals				|
		aiProcess_JoinIdenticalVertices		|
		aiProcess_SortByPType);

	if(!scene)
	{
		K15_SetResourceCompilerError(p_ResourceCompiler, importer.GetErrorString());
		free(resourcePath);
		return K15_FALSE;
	}

	//Add mesh as dependency
	K15_InternalAddResourceDependency(p_ResourceCompilerContext, p_MeshConfig->path, resourcePath);

	uint32 sceneFlags = scene->mFlags;

	if((sceneFlags & AI_SCENE_FLAGS_INCOMPLETE) > 0)
	{
		K15_SetResourceCompilerError(p_ResourceCompiler, importer.GetErrorString());
		free(resourcePath);
		return K15_FALSE;
	}

	uint32 amountSubmeshes = scene->mNumMeshes;

	K15_MeshFormat meshFormat;

	K15_SetMeshName(&meshFormat, K15_GetFileNameWithoutExtension(resourcePath));
	K15_SetMeshSubmeshCount(&meshFormat, amountSubmeshes);

	for(uint32 submeshIndex = 0;
		submeshIndex < amountSubmeshes;
		++submeshIndex)
	{
		aiMesh* mesh = scene->mMeshes[submeshIndex];

		uint32 amountColorChannel = mesh->GetNumColorChannels();
		uint32 amountTextureChannel = mesh->GetNumUVChannels();

		K15_SetSubmeshVertexCount(&meshFormat.submeshes[submeshIndex], mesh->mNumVertices);
		K15_SetSubmeshTriangleCount(&meshFormat.submeshes[submeshIndex], mesh->mNumFaces);
		K15_SetSubmeshMaterial(&meshFormat.submeshes[submeshIndex], mesh->mMaterialIndex);

		K15_SetSubmeshData(&meshFormat.submeshes[submeshIndex], (float*)mesh->mVertices, K15_MESH_DATA_POSITION);
		K15_SetSubmeshData(&meshFormat.submeshes[submeshIndex], (float*)mesh->mNormals, K15_MESH_DATA_NORMAL);
		K15_SetSubmeshData(&meshFormat.submeshes[submeshIndex], (float*)mesh->mTangents, K15_MESH_DATA_TANGENT);
		K15_SetSubmeshData(&meshFormat.submeshes[submeshIndex], (float*)mesh->mBitangents, K15_MESH_DATA_BITANGENT);

		for(uint32 faceIndex = 0;
			faceIndex < mesh->mNumFaces;
			++faceIndex)
		{
			K15_SetSubmeshTriangleData(&meshFormat.submeshes[submeshIndex], faceIndex, mesh->mFaces[faceIndex].mIndices);
		}

		K15_SetSubmeshChannelCount(&meshFormat.submeshes[submeshIndex], amountColorChannel, K15_MESH_DATA_COLOR);
		K15_SetSubmeshChannelCount(&meshFormat.submeshes[submeshIndex], amountTextureChannel, K15_MESH_DATA_TEXCOORD);

		for(uint32 colorChannelIndex = 0; 
			colorChannelIndex < amountColorChannel; 
			++colorChannelIndex)
		{
			K15_SetSubmeshDataWithIndex(colorChannelIndex, &meshFormat.submeshes[submeshIndex], (float*)mesh->mColors[colorChannelIndex], K15_MESH_DATA_COLOR);
		}

		for(uint32 textureChannelIndex = 0;
			textureChannelIndex < amountTextureChannel;
			++textureChannelIndex)
		{
			K15_SetSubmeshDataWithIndex(textureChannelIndex, &meshFormat.submeshes[submeshIndex], (float*)mesh->mTextureCoords[textureChannelIndex], K15_MESH_DATA_TEXCOORD);
		}
	}

	K15_DataAccessContext accessContext = {};

	uint8 result = K15_SaveMeshFormatToFile(&meshFormat, p_OutputPath, K15_SAVE_FLAG_FREE_DATA);

	if (result != K15_SUCCESS)
	{
		if (result == K15_ERROR_FILE)
		{
			K15_LOG_ERROR_MESSAGE("Could not save file '%s' (%s).", p_OutputPath, strerror(errno));
		}
	}

	free(resourcePath);
	return result == K15_SUCCESS;
}
#endif //K15_RESOURCE_COMPILER_ENABLE_ASSIMP
/*********************************************************************************/


/*********************************************************************************/
#ifdef K15_RESOURCE_COMPILER_ENABLE_SQUISH
bool8 K15_CompileTextureResourceWithSquish(K15_ResourceCompilerContext* p_ResourceCompilerContext, K15_ResourceCompiler* p_ResourceCompiler, K15_ConfigFileContext* p_TextureConfig, const char* p_OutputPath)
{
	bool8 compiled = K15_FALSE;
	int width = 0, height = 0;
	int targetWidth = 0, targetHeight = 0;
	int numColorComponents = 0;
	char* compressionTypeString = 0;
	char* compressionQuality = 0;
	char* imageFileName = 0;
	char* imageName = 0;
	char* targetDimension = 0;
	bool8 generateMipMaps = 0;
	uint32 pixelFormat = K15_TEXTURE_R8G8B8_UBYTE;
	uint32 numImages = 1;

	stbi_uc** imageData = 0;
	stbi_uc** compressedImageData = 0;

	char* resourcePath = K15_GetConfigValueAsString(p_TextureConfig, "Source");

	if (!resourcePath)
	{
		K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("No image file has been specified for texture '%s' (Define 'Source' in the resource config file).", (char*)malloc(512), resourcePath));
		goto free_resources;
	}

	char* resourceAbsolutePath = K15_ConvertToAbsolutePath(resourcePath);
	free(resourcePath);
	resourcePath = resourceAbsolutePath;

	if (!K15_FileExists(resourcePath))
	{
		K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Could not find image file '%s'.", (char*)malloc(512), resourcePath));
		goto free_resources;
	}

	imageFileName = K15_GetFileNameWithoutPath(resourcePath);
	imageName = K15_GetFileNameWithoutExtension(resourcePath);

	bool8 infoResult = stbi_info(resourcePath, &width, &height, &numColorComponents);

	targetWidth = width;
	targetHeight = height;

	if (!infoResult)
	{
		const char* errorMsg = stbi_failure_reason();
		K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Could not load texture '%s' (%s).", (char*)malloc(512), imageFileName, errorMsg));
		goto free_resources;
	}

	//Add source image as dependency
	K15_InternalAddResourceDependency(p_ResourceCompilerContext, p_TextureConfig->path, resourcePath);

	targetDimension = K15_GetConfigValueAsString(p_TextureConfig, "TargetDimension");

	if (targetDimension == 0)
	{
		K15_GetConfigValueAsResolution(p_TextureConfig, "TargetDimension", &targetWidth, &targetHeight, width, height);
	}
	else
	{
		if (strcmp(targetDimension, "half") == 0)
		{
			targetWidth = width/2;
			targetHeight = height/2;
		}
		else if(strcmp(targetDimension, "quarter") == 0)
		{
			targetWidth = width/4;
			targetHeight = height/4;
		}
		else
		{
			K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Target dimension '%s' for texture '%s' is unsupported.", (char*)malloc(512), targetDimension, imageFileName));
		}
	}

	if (!K15_InternalCheckPowerOfTwo(targetWidth) ||
		!K15_InternalCheckPowerOfTwo(targetHeight))
	{
		if (width != targetWidth ||
			height != targetHeight)
		{
			K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Texture '%s' is not power of two (%dx%d) (Original size: %dx%d).", (char*)malloc(512), imageFileName, targetWidth, targetHeight, width, height));
		}
		else
		{
			K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Texture '%s' is not power of two (%dx%d).", (char*)malloc(512), imageFileName, width, height));
		}

		goto free_resources;
	}

	compressionTypeString = K15_GetConfigValueAsString(p_TextureConfig, "Compression");
	compressionQuality = K15_GetConfigValueAsString(p_TextureConfig, "CompressionQuality");
	generateMipMaps = K15_GetConfigValueAsBool(p_TextureConfig, "GenerateMipmaps", K15_FALSE);

	if (!compressionTypeString)
	{
		K15_LOG_WARNING_MESSAGE("No compression for texture '%s' specified. Texture gets saved uncompressed (Specify a 'Compression' value in the resource info file).", imageFileName);
	}
	else
	{
		if (strcmp(compressionTypeString, "dxt1") == 0)
		{
			pixelFormat = K15_TEXTURE_DXT1_COMPRESSION;
		}
		else if (strcmp(compressionTypeString, "dxt3") == 0)
		{
			pixelFormat = K15_TEXTURE_DXT3_COMPRESSION;
		}
		else if(strcmp(compressionTypeString, "dxt5") == 0)
		{
			pixelFormat = K15_TEXTURE_DXT5_COMPRESSION;
		}
		else if(strcmp(compressionTypeString, "uncompressed") != 0)
		{
			K15_LOG_WARNING_MESSAGE("Unrecognized texture compression '%s' for texture '%s'. Texture gets saved uncompressed.", compressionTypeString, imageFileName);
		}

		if (pixelFormat == K15_TEXTURE_R8G8B8_UBYTE)
		{
			pixelFormat = numColorComponents == 3 ? K15_TEXTURE_R8G8B8_UBYTE : K15_TEXTURE_R8G8B8A8_UBYTE;
		}
	}

	//create image array
	if (generateMipMaps)
	{
		numImages += K15_InternalGetMipMapCount(targetWidth, targetHeight);
	}

	imageData = (stbi_uc**)malloc(numImages * K15_PTR_SIZE);
	imageData[0] = stbi_load(resourcePath, &width, &height, &numColorComponents, numColorComponents);

	//scale texture to target size
	if (targetWidth != width || targetHeight != height)
	{
		stbi_uc* scaledImageData = (stbi_uc*)STBI_MALLOC(targetWidth * targetHeight * numColorComponents);

		bool8 resizeResult = stbir_resize_uint8(imageData[0], width, height, 0, 
			scaledImageData, targetWidth, targetHeight, 0, numColorComponents);

		if (resizeResult == K15_TRUE)
		{
			stbi_image_free(imageData[0]);
			imageData[0] = scaledImageData;
		}
	}

	//generate mipmaps
	if (generateMipMaps)
	{
		uint32 mipMapWidth = targetWidth / 2;
		uint32 mipMapHeight = targetHeight / 2;

		for (uint32 mipMapIndex = 1;
			mipMapIndex < numImages;
			++mipMapIndex)
		{
			stbi_uc* currentMipMap = (stbi_uc*)STBI_MALLOC(mipMapWidth * mipMapHeight * numColorComponents);

			bool8 resizeResult = stbir_resize_uint8(imageData[0], targetWidth, targetHeight, 0,
				currentMipMap, mipMapWidth, mipMapHeight, 0, numColorComponents);

			if (resizeResult != K15_TRUE)
			{
				K15_LOG_ERROR_MESSAGE("Could not create mip map %u for texture '%s'.", mipMapIndex, imageFileName);
				imageData[mipMapIndex] = 0;
				continue;
			}

			imageData[mipMapIndex] = currentMipMap;

			mipMapWidth /= 2;
			mipMapHeight /= 2;
		}
	}

	//compress texture
	if (pixelFormat != K15_TEXTURE_R8G8B8_UBYTE ||
		pixelFormat != K15_TEXTURE_R8G8B8A8_UBYTE)
	{
		if (pixelFormat == K15_TEXTURE_DXT1_COMPRESSION ||
			pixelFormat == K15_TEXTURE_DXT3_COMPRESSION ||
			pixelFormat == K15_TEXTURE_DXT5_COMPRESSION)
		{
			int compressionFlags = 0;

			if (pixelFormat == K15_TEXTURE_DXT1_COMPRESSION)
			{
				compressionFlags = squish::kDxt1;
			}
			else if (pixelFormat == K15_TEXTURE_DXT3_COMPRESSION)
			{
				compressionFlags = squish::kDxt3;
			}
			else
			{
				compressionFlags = squish::kDxt5;
			}

			if (compressionQuality)
			{
				if (strcmp(compressionQuality, "high") == 0)
				{
					compressionFlags |= squish::kColourIterativeClusterFit;
				}
				else if(strcmp(compressionQuality, "low") == 0)
				{
					compressionFlags |= squish::kColourRangeFit;
				}
				else
				{
					K15_LOG_WARNING_MESSAGE("Unknown compression quality '%s' for texture '%s' (using 'fast' quality by default).", compressionQuality, imageFileName);
					compressionFlags |= squish::kColourRangeFit;
				}
			}

			compressedImageData = (stbi_uc**)malloc(K15_PTR_SIZE * numImages);

			//compress images
			uint32 imageWidth = targetWidth;
			uint32 imageHeight = targetHeight;

			for (uint32 imageIndex = 0;
				imageIndex < numImages;
				++imageIndex)
			{
				if (imageData[imageIndex])
				{
					compressedImageData[imageIndex] = (stbi_uc*)malloc(K15_InternalCalculateImageMemorySizeCompressedSquish(imageWidth, imageHeight, compressionFlags));
					squish::CompressImage(imageData[imageIndex], imageWidth, imageHeight, compressedImageData[imageIndex], compressionFlags);

					//free source image data
					stbi_image_free(imageData[imageIndex]);
					imageData[imageIndex] = 0;
				}
				

				imageWidth /= 2;
				imageHeight /= 2;
			}

			free(imageData);
			imageData = compressedImageData;
		}
	}
	
	//save texture values and save texture format
	K15_TextureFormat textureFormat;

	K15_SetTextureName(&textureFormat, imageName);

	textureFormat.width = targetWidth;
	textureFormat.height = targetHeight;
	textureFormat.colorComponentCount = numColorComponents;
	textureFormat.pixelFormat = pixelFormat;
	textureFormat.mipMapCount = numImages;

	K15_SetTextureMipMapCount(&textureFormat, numImages);

	uint32 currentImageWidth = targetWidth;
	uint32 currentImageHeight = targetHeight;

	for(uint32 imageIndex = 0;
		imageIndex < numImages;
		++imageIndex)
	{
		if (imageData[imageIndex])
		{
			K15_SetTextureMipMap(&textureFormat, imageIndex, imageData[imageIndex]);

			currentImageHeight /= 2;
			currentImageWidth /= 2;
		}
	}

	if (K15_SaveTextureFormatToFile(&textureFormat, p_OutputPath, K15_SAVE_FLAG_FREE_DATA) == K15_SUCCESS)
	{
		compiled = K15_TRUE;
	}

free_resources:
	free(compressionTypeString);
	free(compressionQuality);
	free(imageFileName);
	free(imageName);
	free(targetDimension);
	free(resourcePath);

	if (imageData)
	{
		for (uint32 imageIndex = 0;
			imageIndex < numImages;
			++imageIndex)
		{
			STBI_FREE(imageData[imageIndex]);
		}
	}

	free(imageData);

	return compiled;
}
#endif //K15_RESOURCE_COMPILER_ENABLE_SQUISH
/*********************************************************************************/
#ifdef K15_RESOURCE_COMPILER_ENABLE_STB_TTF
bool8 K15_CompileFontResourceWithStbTTF(K15_ResourceCompilerContext* p_ResourceCompilerContext, K15_ResourceCompiler* p_ResourceCompiler, K15_ConfigFileContext* p_FontConfig, const char* p_OutputPath)
{
	bool8 compiled = K15_FALSE;

	char* resourcePath = K15_GetConfigValueAsString(p_FontConfig, "Source");
	char* resourceFileName = 0;
	char* resourceName = 0;

	int ascent = 0;
	int descent = 0;
	int lineGap = 0;

	uint32 posX = 0;
	uint32 posY = 0;

	int fontPixelWidth = 0;
	int maxGlyphHeight = 0;
	int startCharacter = K15_GetConfigValueAsInt(p_FontConfig, "StartChar", 0);
	int endCharacter = K15_GetConfigValueAsInt(p_FontConfig, "EndChar", 255);
	float fontSize = K15_GetConfigValueAsFloat(p_FontConfig, "FontSize", 12.f);
	byte* trueTypeData = 0;

	byte* fontPixelData = 0;

	stbtt_fontinfo fontInfo = {};
	K15_FontFormat fontFormat = {};
	K15_TextureAtlas fontTextureAtlas = K15_CreateTextureAtlas(1);

	if (!resourcePath)
	{
		K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Not font file specified for font resource '%s' (specify 'Source' in the resource config file).", (char*)malloc(512), p_FontConfig->path));
		goto free_resources;
	}

	char* resourceAbsolutePath = K15_ConvertToAbsolutePath(resourcePath);
	free(resourcePath);
	resourcePath = resourceAbsolutePath;

	if (!K15_FileExists(resourcePath))
	{
		K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Could not find font file '%s'.", (char*)malloc(512), resourcePath));
		goto free_resources;
	}

	resourceFileName = K15_GetFileNameWithoutPath(resourcePath);
	resourceName = K15_GetFileNameWithoutExtension(resourcePath);
	trueTypeData = K15_GetWholeFileContent(resourcePath);

	if (!trueTypeData)
	{
		K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Could not load load '%s' (%s).", (char*)malloc(512), resourceFileName, K15_CopySystemErrorMessageIntoBuffer((char*)alloca(512))));
		goto free_resources;
	}

	if (stbtt_InitFont(&fontInfo, trueTypeData, 0) == 0)
	{
		K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Could not get font information from '%s' (maybe not a font file?)", (char*)malloc(512), resourceFileName));
		goto free_resources;
	}

	//Add font as dependency
	K15_InternalAddResourceDependency(p_ResourceCompilerContext, p_FontConfig->path, resourcePath);

	K15_SetFontName(&fontFormat, resourceName);
	K15_SetFontGlyphRange(&fontFormat, startCharacter, endCharacter);

  //float scaleFactor = stbtt_ScaleForMappingEmToPixels(&fontInfo, fontSize);
	float scaleFactor = stbtt_ScaleForPixelHeight(&fontInfo, fontSize);
	stbtt_GetFontVMetrics(&fontInfo, &ascent, &descent, &lineGap);
	
	fontFormat.fontSize = fontSize;
	fontFormat.scaleFactor = scaleFactor;
	fontFormat.lineGap = scaleFactor * (float)lineGap;
	fontFormat.ascent = scaleFactor * (float)ascent;
	fontFormat.descent = scaleFactor * (float)descent;

	//set kerning data
	uint32 numCharacters = endCharacter - startCharacter;
	uint32 numCombinations = numCharacters * numCharacters;

	for (uint32 character1 = 0;
		character1 < numCharacters;
		++character1)
	{
		for (uint32 character2 = 0;
			character2 < numCharacters;
			++character2)
		{
			uint32 char1 = character1 + startCharacter;
			uint32 char2 = character2 + startCharacter;

			float kernAdvance = stbtt_GetCodepointKernAdvance(&fontInfo, char1, char2);
			kernAdvance *= scaleFactor;

			K15_AddFontKerningData(&fontFormat, char1, char2, kernAdvance);
		}
	}

	float shift_x = 0.f;
	float shift_y = 0.f;

	float totalPosX = 0.f;

	//generate glyph texture 
	K15_SetTextureCount(&fontTextureAtlas, endCharacter - startCharacter);

	//loop over all characters to calculate the size of the font texture
	for (int codePoint = startCharacter;
		codePoint < endCharacter;
		++codePoint)
	{
		int width = 0;
		int height = 0;
		int advance = 0;
		int lsb = 0;

		int glyphIndex = stbtt_FindGlyphIndex(&fontInfo, codePoint);

		int x0 = 0;
		int x1 = 0;
		int y0 = 0;
		int y1 = 0;

		stbtt_GetGlyphHMetrics(&fontInfo, glyphIndex, &advance, &lsb);
		stbtt_GetGlyphBox(&fontInfo, glyphIndex, &x0, &y0, &x1, &y1);

		x0 *= scaleFactor;
		x1 *= scaleFactor;
		y0 *= scaleFactor;
		y1 *= scaleFactor;

		byte* glyphPixelData = stbtt_GetGlyphBitmapSubpixel(&fontInfo, scaleFactor, scaleFactor, shift_x, shift_y, glyphIndex, &width, &height, 0, 0);
		K15_AddTextureToAtlas(&fontTextureAtlas, glyphPixelData, width, height, codePoint - startCharacter, &posX, &posY);
		
		K15_GlyphFormat currentGlyphFormat = {};

		currentGlyphFormat.character = codePoint;
		currentGlyphFormat.width = width;
		currentGlyphFormat.height = height;
		currentGlyphFormat.posX = posX;
		currentGlyphFormat.posY = posY;
		currentGlyphFormat.advance = (float)advance * scaleFactor;
		currentGlyphFormat.glyphLeft = x0;
		currentGlyphFormat.glyphRight = x1;
		currentGlyphFormat.glyphTop = y0;
		currentGlyphFormat.glyphBottom = y1;

		//shift_x = currentGlyphFormat.advance;

		K15_AddFontGlyphData(&fontFormat, &currentGlyphFormat, codePoint);
	}

	byte* textureAtlasPixelData = fontTextureAtlas.pixelData;
	uint32 textureAtlasWidth = fontTextureAtlas.width;
	uint32 textureAtlasHeight = fontTextureAtlas.height;

	K15_SetFontTexture(&fontFormat, textureAtlasPixelData, textureAtlasWidth, textureAtlasHeight);

	if(K15_SaveFontFormatToFile(&fontFormat, p_OutputPath, K15_SAVE_FLAG_FREE_DATA) != K15_SUCCESS)
	{
		K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Could not open output file '%s'.", (char*)malloc(512), p_OutputPath));
	}
	else
	{
		compiled = K15_TRUE;
	}
	

free_resources:
	free(resourcePath);
	free(trueTypeData);
	free(resourceName);
	free(resourceFileName);
	free(fontPixelData);
	K15_FreeTextureAtlas(&fontTextureAtlas);

	return compiled;
}
#endif //K15_RESOURCE_COMPILER_ENABLE_STB_TTF
/*********************************************************************************/
bool8 K15_CompileMaterialResource(K15_ResourceCompilerContext* p_ResourceCompilerContext, K15_ResourceCompiler* p_ResourceCompiler, K15_ConfigFileContext* p_MaterialConfig, const char* p_OutputPath)
{
	//TODO:
	// * RENDER STATES
	// * PARSE MATERIAL PASS DATA
	// * (MAYBE) PARSE SHADER TO AUTOMATICALLY FIND OUT WHAT VARIABLES EXIST
	// * DIFFERENT QUALITY LEVELS

	bool8 compiled = K15_FALSE;
	uint8 result = K15_SUCCESS;

	K15_ConfigValue** valueConfigValues = 0;
	K15_ConfigValue** textureConfigValues = 0;
	K15_ConfigValue** samplerConfigValues = 0;

	char* resourceName = K15_GetFileNameWithoutExtension(p_MaterialConfig->path);

	char* samplerName = 0;
	char* samplerPath = 0;

	char* materialPassTemplateConfigRelativePath = 0;
	char* materialPassDataConfigRelativePath = 0;

	char* materialPassTemplateConfigPath = 0;
	char* materialPassDataConfigPath = 0;
	char* materialPassDataConfigDirectory = 0;
	char* materialTemplatePassNameBuffer = 0;
	char* materialDataPassNameBuffer = 0;

	char* materialPassVertexShader = 0;
	char* materialPassFragmentShader = 0;
	char* materialPassVertexShaderAbsolute = 0;
	char* materialPassFragmentShaderAbsolute = 0;
	
	char* vertexShaderCode = 0;
	char* fragmentShaderCode = 0;

	K15_MaterialFormat materialFormat = {};
	K15_SetMaterialFormatName(&materialFormat, resourceName);

	//read of how many passes the current material consists.
	int32 amountPasses = K15_GetConfigValueAsInt(p_MaterialConfig, "AmountPasses");

	//no passes is an error
	if (amountPasses == 0)
	{
		K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("'AmountPasses' for material desc '%s' is 0 or non existent.", (char*)malloc(512), resourceName));
		goto free_resources;
	}

	K15_SetMaterialFormatPassCount(&materialFormat, amountPasses);

	//read data on a material pass level
	for (int32 passIndex = 0;
		passIndex < amountPasses;
		++passIndex)
	{
		K15_MaterialPassFormat materialPass = {};
		K15_MaterialPassTemplateFormat* materialTemplatePassFormat = &materialPass.materialPassTemplate;
		K15_MaterialPassDataFormat* maerialDataFormat = &materialPass.materialPassData;

		materialTemplatePassNameBuffer = (char*)malloc(512);
		materialDataPassNameBuffer = (char*)malloc(512);

		//generate the names for the pass values in the config file
		sprintf(materialTemplatePassNameBuffer, "MaterialPass%dTemplate", passIndex);
		sprintf(materialDataPassNameBuffer, "MaterialPass%dData", passIndex);

		//pass template is mandatory
		materialPassTemplateConfigRelativePath = K15_GetConfigValueAsString(p_MaterialConfig, materialTemplatePassNameBuffer);

		if (!materialPassTemplateConfigRelativePath)
		{
			K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Could not find config value '%s' in resource config file '%s'.", 
				(char*)malloc(512), materialTemplatePassNameBuffer, p_MaterialConfig->path));

			goto free_resources;
		}

		materialPassTemplateConfigPath = K15_ConvertToAbsolutePath(materialPassTemplateConfigRelativePath);
		materialPassDataConfigDirectory = K15_GetPathWithoutFileName(materialPassTemplateConfigPath);

		//pass data is optional
		materialPassDataConfigRelativePath = K15_GetConfigValueAsString(p_MaterialConfig, materialDataPassNameBuffer);

		if (!materialPassDataConfigRelativePath)
		{
			K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Could not find config value '%s' in resource config file '%s'.", 
				(char*)malloc(512), materialDataPassNameBuffer, p_MaterialConfig->path));

			goto free_resources;
		}

		materialPassDataConfigPath = K15_ConvertToAbsolutePath(materialPassDataConfigRelativePath);

		//try to read material pass template
		K15_ConfigFileContext materialPassTemplateConfig = {};
		if (K15_LoadConfigFile(materialPassTemplateConfigPath, &materialPassTemplateConfig) != K15_SUCCESS)
		{
			K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Could not open material pass template file '%s'.", (char*)malloc(512), materialPassTemplateConfigPath));
			goto free_resources;
		}

		//Add material template config as dependency
		K15_InternalAddResourceDependency(p_ResourceCompilerContext, p_MaterialConfig->path, materialPassTemplateConfigPath);

		//try to read material pass data (only if it has been specified by the config file)
		K15_ConfigFileContext materialPassDataConfig = {};
		if (materialPassDataConfigPath)
		{
			if (K15_LoadConfigFile(materialPassDataConfigPath, &materialPassDataConfig) != K15_SUCCESS)
			{
				K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Could not open material data template file '%s'.", 
					(char*)malloc(512), materialPassDataConfigPath));

				goto free_resources;
			}	
		}

		//Add material template data config as dependency
		K15_InternalAddResourceDependency(p_ResourceCompilerContext, p_MaterialConfig->path, materialPassDataConfigPath);

		//get shader file data from pass template
		materialPassVertexShader = K15_GetConfigValueAsString(&materialPassTemplateConfig, "VertexShader");
		materialPassFragmentShader = K15_GetConfigValueAsString(&materialPassTemplateConfig, "FragmentShader");

		if (materialPassVertexShader == 0)
		{
			K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("'VertexShader' not set in material pass template file '%s'.", 
				(char*)malloc(512), materialPassTemplateConfigPath));

			goto free_resources;
		}

		if (materialPassFragmentShader == 0)
		{
			K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("'FragmentShader' not set in material pass template file '%s'.", 
				(char*)malloc(512), materialPassTemplateConfigPath));

			goto free_resources;
		}

		//get absolute paths to the shader files
		materialPassVertexShaderAbsolute = K15_ConvertToAbsolutePath(materialPassVertexShader);//K15_ConcatStrings(materialPassDataConfigDirectory, materialPassVertexShader);
		materialPassFragmentShaderAbsolute = K15_ConvertToAbsolutePath(materialPassFragmentShader);//K15_ConcatStrings(materialPassDataConfigDirectory, materialPassFragmentShader);
		
		//check if the shader files are existent
		if (!K15_FileExists(materialPassVertexShaderAbsolute))
		{
			K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Vertex Shader file '%s' can not be found.", 
				(char*)malloc(512), materialPassVertexShaderAbsolute));

			goto free_resources;
		}

		if (!K15_FileExists(materialPassFragmentShaderAbsolute))
		{
			K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Fragment Shader file '%s' can not be found.", 
				(char*)malloc(512), materialPassFragmentShaderAbsolute));

			goto free_resources;
		}

		//add relative shader file path to material pass
		K15_SetMaterialPassTemplateVertexShaderPath(materialTemplatePassFormat, materialPassVertexShader);
		K15_SetMaterialPassTemplateFragmentShaderPath(materialTemplatePassFormat, materialPassFragmentShader);

		//add vertex and fragment shader as dependency
		K15_InternalAddResourceDependency(p_ResourceCompilerContext, p_MaterialConfig->path, materialPassVertexShaderAbsolute);
		K15_InternalAddResourceDependency(p_ResourceCompilerContext, p_MaterialConfig->path, materialPassFragmentShaderAbsolute);

		//read data values
		uint32 valueCount = K15_GetNumConfigValuesForCategory(&materialPassDataConfig, "Values");
		uint32 textureCount = K15_GetNumConfigValuesForCategory(&materialPassDataConfig, "Textures");
		uint32 samplerCount = K15_GetNumConfigValuesForCategory(&materialPassDataConfig, "Sampler");
		uint32 overallValueIndex = 0;

		K15_SetMaterialPassDataValueCount(maerialDataFormat, valueCount + textureCount + samplerCount);

		if (valueCount > 0)
		{
			valueConfigValues = (K15_ConfigValue**)K15_RF_MALLOC(valueCount * K15_PTR_SIZE);
			K15_CopyCategoryConfigValuesIntoBuffer(&materialPassDataConfig, "Values", valueConfigValues);
		}

		if (textureCount > 0)
		{
			textureConfigValues = (K15_ConfigValue**)K15_RF_MALLOC(textureCount * K15_PTR_SIZE);
			K15_CopyCategoryConfigValuesIntoBuffer(&materialPassDataConfig, "Textures", textureConfigValues);
		}

		if (samplerCount > 0)
		{
			samplerConfigValues = (K15_ConfigValue**)K15_RF_MALLOC(samplerCount * K15_PTR_SIZE);
			K15_CopyCategoryConfigValuesIntoBuffer(&materialPassDataConfig, "Sampler", samplerConfigValues);
		}

		//values
		for (uint32 valueIndex = 0;
			valueIndex < valueCount;
			++valueIndex)
		{
			K15_MaterialPassDataValue dataValue = {};
			K15_ConfigValue* value = valueConfigValues[valueIndex];

			bool8 foundType = K15_FALSE;

			if (K15_InternalIsIntValue(value->value))
			{
				foundType = K15_TRUE;
				dataValue.dataType = K15_MATERIAL_DATA_TYPE_INT;
				dataValue.asInt = K15_GetConfigValueAsInt(&materialPassDataConfig, value->name);
			}
			else if (K15_InternalIsFloatValue(value->value))
			{
				foundType = K15_TRUE;
				dataValue.dataType = K15_MATERIAL_DATA_TYPE_FLOAT;
				dataValue.asFloat = K15_GetConfigValueAsFloat(&materialPassDataConfig, value->name);
			}
			else
			{
				K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Could not convert value '%s' to int or float.", 
					(char*)malloc(512), value->name));

				goto free_resources;
			}

			if (foundType)
			{
				dataValue.name = K15_CopyString(value->name);
				K15_AddMaterialPassDataValue(maerialDataFormat, &dataValue, overallValueIndex++);
			}
		}

		//textures
		for (uint32 textureIndex = 0;
			textureIndex < textureCount;
			++textureIndex)
		{
			K15_MaterialPassDataValue textureValue = {};

			K15_ConfigValue* value = textureConfigValues[textureIndex];
			textureValue.name = K15_CopyString(value->name);

			if (K15_InternalIsStringValue(value->value))
			{
				char* texturePath = value->value;
				char* absoluteTexturePath = K15_ConvertToAbsolutePath(texturePath);//K15_ConcatStrings(materialPassDataConfigDirectory, texturePath);

				if (!K15_FileExists(absoluteTexturePath))
				{
					K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Could not find texture '%s' for material '%s' (%d. pass - file: '%s').", 
						(char*)malloc(512), texturePath, resourceName, passIndex, p_MaterialConfig->path));

					goto free_resources;
				}
				else
				{
					textureValue.dataType = K15_MATERIAL_DATA_TYPE_TEXTURE;
					textureValue.asString = K15_CopyString(texturePath);
				}

				free(absoluteTexturePath);
			}
			else
			{
				K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Could not convert value '%s' to texture path.", 
					(char*)malloc(512), value->name));

				goto free_resources;
			}

			K15_AddMaterialPassDataValue(maerialDataFormat, &textureValue, overallValueIndex++);
		}

		//samplers
		for (uint32 samplerIndex = 0;
			samplerIndex < samplerCount;
			++samplerIndex)
		{
			K15_MaterialPassDataValue samplerValue = {};

			K15_ConfigValue* value = samplerConfigValues[samplerIndex];
			samplerValue.name = K15_CopyString(value->name);

			if (K15_InternalIsStringValue(value->value))
			{
				char* samplerPath = value->value;
				char* absoluteSamplerPath = K15_ConvertToAbsolutePath(samplerPath);//K15_ConcatStrings(materialPassDataConfigDirectory, samplerPath);

				if (!K15_FileExists(absoluteSamplerPath))
				{
					K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Could not find sampler '%s' for material '%s' (%d. pass - file: '%s').", 
						(char*)malloc(512), samplerPath, resourceName, passIndex, p_MaterialConfig->path));

					goto free_resources;
				}
				else
				{
					samplerValue.dataType = K15_MATERIAL_DATA_TYPE_SAMPLER;
					samplerValue.asString = K15_CopyString(samplerPath);
				}

				free(absoluteSamplerPath);
			}
			else
			{
				K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Could not convert value '%s' to sampler path.", 
					(char*)malloc(512), value->name));

				goto free_resources;
			}

			K15_AddMaterialPassDataValue(maerialDataFormat, &samplerValue, overallValueIndex++);
		}

		K15_AddMaterialPassFormat(&materialFormat, &materialPass, passIndex);

		K15_SAFE_FREE(materialTemplatePassNameBuffer);
		K15_SAFE_FREE(materialDataPassNameBuffer);

		K15_SAFE_FREE(materialPassTemplateConfigRelativePath);
		K15_SAFE_FREE(materialPassTemplateConfigPath);

		K15_SAFE_FREE(materialPassDataConfigRelativePath);
		K15_SAFE_FREE(materialPassDataConfigPath);

		K15_SAFE_FREE(materialPassVertexShader);
		K15_SAFE_FREE(materialPassFragmentShader);

		K15_SAFE_FREE(materialPassVertexShaderAbsolute);
		K15_SAFE_FREE(materialPassFragmentShaderAbsolute);

		K15_SAFE_FREE(materialPassDataConfigDirectory);

		K15_SAFE_FREE(fragmentShaderCode);
		K15_SAFE_FREE(vertexShaderCode);

		K15_SAFE_FREE(valueConfigValues);
		K15_SAFE_FREE(textureConfigValues);
		K15_SAFE_FREE(samplerConfigValues);
	}
	
	compiled = K15_SaveMaterialFormatToFile(&materialFormat, p_OutputPath, K15_SAVE_FLAG_FREE_DATA) == K15_SUCCESS;

free_resources:
	free(resourceName);
	free(materialPassDataConfigDirectory);
	free(materialTemplatePassNameBuffer);
	free(materialDataPassNameBuffer);
	free(materialPassTemplateConfigRelativePath);
	free(materialPassDataConfigRelativePath);
	free(materialPassTemplateConfigPath);
	free(materialPassDataConfigPath);
	free(materialPassVertexShader);
	free(materialPassFragmentShader);
	free(materialPassVertexShaderAbsolute);
	free(materialPassFragmentShaderAbsolute);
	free(samplerName);
	free(samplerPath);

	return compiled;
}
/*********************************************************************************/
bool8 K15_CompileSamplerResource(K15_ResourceCompilerContext* p_ResourceCompilerContext, K15_ResourceCompiler* p_ResourceCompiler, K15_ConfigFileContext* p_SamplerConfig, const char* p_OutputPath)
{
	bool8 compiled = K15_FALSE;

	char* resourceName = K15_GetFileNameWithoutPath(p_SamplerConfig->path);

	K15_SamplerFormat samplerFormat = {};
	K15_SetSamplerFormatName(&samplerFormat, resourceName);

	char* minificationFilterText = K15_GetConfigValueAsString(p_SamplerConfig, "MinificationFilter", "Linear");
	char* magnificationFilterText = K15_GetConfigValueAsString(p_SamplerConfig, "MagnificationFilter", "Linear");
	char* addressModeUText = K15_GetConfigValueAsString(p_SamplerConfig, "AddressModeU", "Clamp");
	char* addressModeVText = K15_GetConfigValueAsString(p_SamplerConfig, "AddressModeU", "Clamp");
	char* addressModeWText = K15_GetConfigValueAsString(p_SamplerConfig, "AddressModeU", "Clamp");

	uint8 minificationFilter = K15_InternalConvertTextToSamplerFilter(minificationFilterText);
	uint8 magnificationFilter = K15_InternalConvertTextToSamplerFilter(magnificationFilterText);
	uint8 addressModeU = K15_InternalConvertTextToSamplerAddressMode(addressModeUText);
	uint8 addressModeV = K15_InternalConvertTextToSamplerAddressMode(addressModeVText);
	uint8 addressModeW = K15_InternalConvertTextToSamplerAddressMode(addressModeWText);

	K15_SetSamplerFormatMinificationFilter(&samplerFormat, minificationFilter);
	K15_SetSamplerFormatMagnificationFilter(&samplerFormat, magnificationFilter);

	K15_SetSamplerFormatAdressModeU(&samplerFormat, addressModeU);
	K15_SetSamplerFormatAdressModeU(&samplerFormat, addressModeV);
	K15_SetSamplerFormatAdressModeU(&samplerFormat, addressModeW);

	if (K15_SaveSamplerFormatToFile(&samplerFormat, p_OutputPath, K15_SAVE_FLAG_FREE_DATA) != K15_SUCCESS)
	{
		K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Could not save sampler format '%s'.", (char*)malloc(512), resourceName));
		goto free_resources;
	}
	else
	{
		compiled = K15_TRUE;
	}

free_resources:
	free(resourceName);

	return compiled;
}
/*********************************************************************************/
void K15_CreateDefaultResourceCompiler(K15_ResourceCompilerContext* p_ResourceCompilerContext)
{
	K15_ResourceCompiler* meshCompiler = (K15_ResourceCompiler*)malloc(sizeof(K15_ResourceCompiler));
	meshCompiler->name = "Mesh Compiler";
	meshCompiler->error = 0;

#ifdef K15_RESOURCE_COMPILER_ENABLE_ASSIMP
	meshCompiler->compileResource = K15_CompileMeshResourceWithAssimp;
#endif //K15_RESOURCE_COMPILER_ENABLE_ASSIMP

	K15_ResourceCompiler* textureCompiler = (K15_ResourceCompiler*)malloc(sizeof(K15_ResourceCompiler));
	textureCompiler->name = "Texture Compiler";
	textureCompiler->error = 0;

#ifdef K15_RESOURCE_COMPILER_ENABLE_SQUISH
	textureCompiler->compileResource = K15_CompileTextureResourceWithSquish;
#endif //K15_RESOURCE_COMPILER_ENABLE_SQUISH

	K15_ResourceCompiler* fontCompiler = (K15_ResourceCompiler*)malloc(sizeof(K15_ResourceCompiler));
	fontCompiler->name = "Font Compiler";
	fontCompiler->error = 0;

#ifdef K15_RESOURCE_COMPILER_ENABLE_STB_TTF
	fontCompiler->compileResource = K15_CompileFontResourceWithStbTTF;
#endif //K15_RESOURCE_COMPILER_ENABLE_STB_TTF

	K15_ResourceCompiler* materialCompiler = (K15_ResourceCompiler*)malloc(sizeof(K15_ResourceCompiler));
	materialCompiler->name = "Material Compiler";
	materialCompiler->error = 0;
	materialCompiler->compileResource = K15_CompileMaterialResource;

	K15_ResourceCompiler* samplerCompiler = (K15_ResourceCompiler*)malloc(sizeof(K15_ResourceCompiler));
	samplerCompiler->name = "Sampler Compiler";
	samplerCompiler->error = 0;
	samplerCompiler->compileResource = K15_CompileSamplerResource;

	//add directory watch for the input resource files
	K15_AddDirectoryWatch(p_ResourceCompilerContext->argumentParser->inputPath, K15_InternalOnResourceFileChanged, (void*)p_ResourceCompilerContext);

	p_ResourceCompilerContext->resourceCompiler[K15_RESOURCE_COMPILER_MESH] = meshCompiler;
	p_ResourceCompilerContext->resourceCompiler[K15_RESOURCE_COMPILER_TEXTURE] = textureCompiler;
 	p_ResourceCompilerContext->resourceCompiler[K15_RESOURCE_COMPILER_FONT] = fontCompiler;
	p_ResourceCompilerContext->resourceCompiler[K15_RESOURCE_COMPILER_MATERIAL] = materialCompiler;
	p_ResourceCompilerContext->resourceCompiler[K15_RESOURCE_COMPILER_SAMPLER] = samplerCompiler;
}
/*********************************************************************************/
bool8 K15_CompileResource(K15_ResourceCompilerContext* p_ResourceCompilerContext, const char* p_ResourceFile)
{
	assert(p_ResourceCompilerContext);

	bool8 compileResult = K15_FALSE;

	K15_ConfigFileContext resourceFileConfig = {};

	if(K15_LoadConfigFile(p_ResourceFile, &resourceFileConfig) != K15_SUCCESS)
	{
		K15_LOG_ERROR_MESSAGE("Could not load resource file '%s'", p_ResourceFile);
		return K15_FALSE;
	}

	char* resourceType = K15_GetConfigValueAsString(&resourceFileConfig, "ResourceType");
	char* outputPath = K15_GetConfigValueAsString(&resourceFileConfig, "Destination");
	const char* argumentInputPath = p_ResourceCompilerContext->argumentParser->inputPath;
	const char* argumentOutputPath = p_ResourceCompilerContext->argumentParser->outputPath;
	char* outputCompletePath = 0;

	if (!resourceType)
	{
		K15_LOG_ERROR_MESSAGE("No resource type for resource file '%s' (Add 'ResourceType' value to the resource file)", p_ResourceFile);
		goto free_resources;
	}

	if (!outputPath)
	{
		K15_LOG_ERROR_MESSAGE("No output path for resource '%s' (Add 'Destination' value to the resource file)", p_ResourceFile);
		goto free_resources;
	}

	outputCompletePath = K15_ConcatStrings(argumentOutputPath, outputPath);

	if (!p_ResourceCompilerContext->argumentParser->replace)
	{
		if (K15_FileExists(outputCompletePath))
		{
			K15_LOG_WARNING_MESSAGE("Will not compile resource '%s' as the resource has already been compiled to '%s'. Provide the '-u' flag to override already compiled resource files.", p_ResourceFile, outputCompletePath);
			goto free_resources;
		}
	}

	K15_ResourceCompilerType compilerType = K15_RESOURCE_COMPILER_INVALID;

	if (strcmp(resourceType, "Texture") == 0)
	{
		compilerType = K15_RESOURCE_COMPILER_TEXTURE;
	}
	else if (strcmp(resourceType, "Mesh") == 0)
	{
		compilerType = K15_RESOURCE_COMPILER_MESH;
	}
	else if (strcmp(resourceType, "Font") == 0)
	{
		compilerType = K15_RESOURCE_COMPILER_FONT;
	}
	else if (strcmp(resourceType, "Material") == 0)
	{
		compilerType = K15_RESOURCE_COMPILER_MATERIAL;
	}
	else if (strcmp(resourceType, "Sampler") == 0)
	{
		compilerType = K15_RESOURCE_COMPILER_SAMPLER;
	}
	else
	{
		K15_LOG_WARNING_MESSAGE("Invalid resource type '%s'.", resourceType);
		goto free_resources;
	}

	K15_ResourceCompiler* resourceCompiler = p_ResourceCompilerContext->resourceCompiler[compilerType];

	if (!resourceCompiler)
	{
		K15_LOG_ERROR_MESSAGE("No resource compiler for type '%s'.", resourceType);
		goto free_resources;
	}

	compileResult = resourceCompiler->compileResource(p_ResourceCompilerContext, resourceCompiler, &resourceFileConfig, outputCompletePath);

	if (compileResult == K15_FALSE)
	{
		K15_LOG_ERROR_MESSAGE("Error during compilation of resource '%s' (%s).", p_ResourceFile, resourceCompiler->error);
		free(resourceCompiler->error);
		resourceCompiler->error = 0;

		goto free_resources;
	}

	K15_LOG_SUCCESS_MESSAGE("Successful compiled resource '%s' to '%s'.", p_ResourceFile, outputPath);

free_resources:
	free(resourceType);
	free(outputPath);
	free(outputCompletePath);

	return compileResult;
}
/*********************************************************************************/
void K15_CompileResourceAsync(void* p_ThreadParameter)
{
	K15_ResourceCompilerAsyncParameter* resourceCompilerAsyncParameter = (K15_ResourceCompilerAsyncParameter*)p_ThreadParameter;

	K15_CompileResource(resourceCompilerAsyncParameter->resourceCompilerContext, resourceCompilerAsyncParameter->resourceFilePath);
}
/*********************************************************************************/
void K15_CompileResources(K15_ResourceCompilerContext* p_ResourceCompilerContext, char** p_FilesToCompile, uint32 p_NumFilesToCompile)
{
	assert(p_ResourceCompilerContext);

	char** resourceFiles = p_FilesToCompile;
	unsigned int numResourceFiles = p_NumFilesToCompile;

	for (unsigned int fileIndex = 0;
		fileIndex < numResourceFiles;
		++fileIndex)
	{
		char* resourceFile = resourceFiles[fileIndex];
		
		K15_ResourceCompilerAsyncParameter asyncParameter = {};
		asyncParameter.resourceCompilerContext = p_ResourceCompilerContext;
		asyncParameter.resourceFilePath = resourceFile;

		K15_CreateAsyncOperation(p_ResourceCompilerContext->asyncContext, K15_CompileResourceAsync, 0, &asyncParameter, sizeof(K15_ResourceCompilerAsyncParameter), 
			K15_ASYNC_OPERATION_FIRE_AND_FORGET_FLAG | K15_ASYNC_OPERATION_USER_DATA_COPY_FLAG);
	}
}
/*********************************************************************************/