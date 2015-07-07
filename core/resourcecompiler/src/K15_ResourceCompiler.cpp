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
	#define STBI_FAILURE_USERMSG
	#define STBI_ONLY_JPEG
	#define STBI_ONLY_PNG
	#define STBI_ONLY_TGA
	#include "stb_image.h"
	#include "stb_image_resize.h"
#else
	#error missing texture loader implementation	
#endif //K15_RESOURCE_COMPILER_ENABLE_STB_IMAGE

#ifdef K15_RESOURCE_COMPILER_ENABLE_STB_TTF
	#define STB_TRUETYPE_IMPLEMENTATION
	#include "stb_truetype.h"
#else
	#error missing font loader implementation	
#endif //K15_RESOURCE_COMPILER_ENABLE_STB_TTF

#include "K15_Memory.h"
#include "K15_TextureFormat.h"
#include "K15_FontFormat.h"
#include "K15_System.h"

struct K15_ResourceCompiler;

typedef bool8 (*K15_CompileResourceFnc)(K15_ResourceCompiler*, K15_ConfigFileContext*, const char*, const char*);

/*********************************************************************************/
enum K15_ResourceCompilerType
{
	K15_RESOURCE_COMPILER_INVALID = -1,
	K15_RESOURCE_COMPILER_MESH = 0,
	K15_RESOURCE_COMPILER_TEXTURE,
	K15_RESOURCE_COMPILER_FONT,

	K15_RESOURCE_COMPILER_COUNT
};
/*********************************************************************************/
struct K15_ResourceCompiler
{
	K15_CompileResourceFnc compileResource;
	const char* name;
	char* error;
	char** supportedExtensions;
};
/*********************************************************************************/
struct K15_ResourceCompilerContext
{
	K15_ArgumentParser* argumentParser;
	K15_AsyncContext* asyncContext;

	K15_ResourceCompiler* resourceCompiler[K15_RESOURCE_COMPILER_COUNT];
};
/*********************************************************************************/
struct K15_ResourceCompilerAsyncParameter
{
	K15_ResourceCompilerContext* resourceCompilerContext;
	const char* resourceFilePath;
};
/*********************************************************************************/



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
intern uint32 K15_InternalCalculateImageMemorySizeCompressed(uint32 p_Width, uint32 p_Height, int p_CompressionFlags)
{
	return squish::GetStorageRequirements(p_Width, p_Height, p_CompressionFlags);
}
/*********************************************************************************/
intern uint32 K15_InternalGetNextPowerOfTwoSize(uint32 p_NonPowerOfTwoNumber)
{
	uint32 powerOfTwoNumber = 2;

	while (powerOfTwoNumber < p_NonPowerOfTwoNumber)
	{
		powerOfTwoNumber *= 2;
	}

	return powerOfTwoNumber;
}
/*********************************************************************************/


/*********************************************************************************/
void K15_SetResourceCompilerError(K15_ResourceCompiler* p_ResourceCompiler, const char* p_ErrorMessage)
{
	if (p_ResourceCompiler->error)
	{
		free(p_ResourceCompiler->error);
	}

	p_ResourceCompiler->error = K15_CopyString(p_ErrorMessage);
}
/*********************************************************************************/
#ifdef K15_RESOURCE_COMPILER_ENABLE_ASSIMP
bool8 K15_CompileMeshResourceWithAssimp(K15_ResourceCompiler* p_ResourceCompiler, K15_ConfigFileContext* p_MeshConfig, const char* p_SourceFile, const char* p_OutputPath)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(std::string(p_SourceFile), 
		aiProcess_CalcTangentSpace			|
		aiProcess_Triangulate				|
		aiProcess_GenNormals				|
		aiProcess_JoinIdenticalVertices		|
		aiProcess_SortByPType);

	if(!scene)
	{
		K15_SetResourceCompilerError(p_ResourceCompiler, importer.GetErrorString());
		return K15_FALSE;
	}

	uint32 sceneFlags = scene->mFlags;

	if((sceneFlags & AI_SCENE_FLAGS_INCOMPLETE) > 0)
	{
		K15_SetResourceCompilerError(p_ResourceCompiler, importer.GetErrorString());
		return K15_FALSE;
	}

	uint32 amountSubmeshes = scene->mNumMeshes;

	K15_MeshFormat meshFormat;

	K15_SetMeshName(&meshFormat, K15_GetFileNameWithoutExtension(p_SourceFile));
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

	return result == K15_SUCCESS;
}
#endif //K15_RESOURCE_COMPILER_ENABLE_ASSIMP
/*********************************************************************************/


/*********************************************************************************/
#ifdef K15_RESOURCE_COMPILER_ENABLE_SQUISH
bool8 K15_CompileTextureResourceWithSquish(K15_ResourceCompiler* p_ResourceCompiler, K15_ConfigFileContext* p_TextureConfig, const char* p_SourceFile, const char* p_OutputPath)
{
	bool8 compiled = K15_FALSE;
	int width = 0, height = 0;
	int targetWidth = 0, targetHeight = 0;
	int numColorComponents = 0;
	char* compressionTypeString = 0;
	char* compressionQuality = 0;
	char* imageFileName = K15_GetFileNameWithoutPath(p_SourceFile);
	char* imageName = K15_GetFileNameWithoutExtension(p_SourceFile);
	char* targetDimension = 0;
	bool8 generateMipMaps = 0;
	uint32 compressionType = K15_TEXTURE_NO_COMPRESSION;
	uint32 numImages = 1;

	stbi_uc** imageData = 0;
	stbi_uc** compressedImageData = 0;

	bool8 infoResult = stbi_info(p_SourceFile, &width, &height, &numColorComponents);

	targetWidth = width;
	targetHeight = height;

	if (!infoResult)
	{
		const char* errorMsg = stbi_failure_reason();
		K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Could not load texture '%s' (%s).", (char*)malloc(512), imageFileName, errorMsg));
		goto free_resources;
	}

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
	generateMipMaps = K15_GetConfigValueAsBool(p_TextureConfig, "GenerateMipMaps", K15_FALSE);

	if (!compressionTypeString)
	{
		K15_LOG_WARNING_MESSAGE("No compression for texture '%s' specified. Texture gets saved uncompressed (Specify a 'Compression' value in the resource info file).", imageFileName);
	}
	else
	{
		if (strcmp(compressionTypeString, "dxt1") == 0)
		{
			compressionType = K15_TEXTURE_DXT1_COMPRESSION;
		}
		else if (strcmp(compressionTypeString, "dxt3") == 0)
		{
			compressionType = K15_TEXTURE_DXT3_COMPRESSION;
		}
		else if(strcmp(compressionTypeString, "dxt5") == 0)
		{
			compressionType = K15_TEXTURE_DXT5_COMPRESSION;
		}
		else if(strcmp(compressionTypeString, "uncompressed") != 0)
		{
			K15_LOG_WARNING_MESSAGE("Unrecognized texture compression '%s' for texture '%s'. Texture gets saved uncompressed.", compressionTypeString, imageFileName);
		}
	}

	//create image array
	if (generateMipMaps)
	{
		numImages += K15_InternalGetMipMapCount(targetWidth, targetHeight);
	}

	imageData = (stbi_uc**)malloc(numImages * K15_PTR_SIZE);
	imageData[0] = stbi_load(p_SourceFile, &width, &height, &numColorComponents, 4);

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
	if (compressionType != K15_TEXTURE_NO_COMPRESSION)
	{
		if (compressionType == K15_TEXTURE_DXT1_COMPRESSION ||
			compressionType == K15_TEXTURE_DXT3_COMPRESSION ||
			compressionType == K15_TEXTURE_DXT5_COMPRESSION)
		{
			int compressionFlags = 0;

			if (compressionType == K15_TEXTURE_DXT1_COMPRESSION)
			{
				compressionFlags = squish::kDxt1;
			}
			else if (compressionType == K15_TEXTURE_DXT3_COMPRESSION)
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
					compressedImageData[imageIndex] = (stbi_uc*)malloc(K15_InternalCalculateImageMemorySizeCompressed(imageWidth, imageHeight, compressionFlags));
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
	textureFormat.compression = compressionType;
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
bool8 K15_CompileFontResourceWithStbTTF(K15_ResourceCompiler* p_ResourceCompiler, K15_ConfigFileContext* p_MeshConfig, const char* p_SourceFile, const char* p_OutputPath)
{
	bool8 compiled = K15_FALSE;

	char* resourceFileName = K15_GetFileNameWithoutPath(p_SourceFile);
	char* resourceName = K15_GetFileNameWithoutExtension(p_SourceFile);

	int baseLine = 0;
	int ascent = 0;
	int descent = 0;
	int lineGap = 0;

	int posX = 0;
	int posY = 0;

	int fontPixelWidth = 0;
	int maxGlyphHeight = 0;
	int startCharacter = K15_GetConfigValueAsInt(p_MeshConfig, "StartChar", 0);
	int endCharacter = K15_GetConfigValueAsInt(p_MeshConfig, "EndChar", 255);

	float fontSize = K15_GetConfigValueAsFloat(p_MeshConfig, "FontSize", 12.f);
	byte* trueTypeData = K15_GetWholeFileContent(p_SourceFile);

	byte* fontPixelData = 0;

	stbtt_fontinfo fontInfo = {};
	K15_FontFormat fontFormat = {};

	if (!trueTypeData)
	{
		K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Could not load load '%s' (%s).", (char*)alloca(512), resourceFileName, K15_CopySystemErrorMessageIntoBuffer((char*)alloca(512))));
		goto free_resources;
	}

	if (stbtt_InitFont(&fontInfo, trueTypeData, 0) == 0)
	{
		K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Could not get font information from '%s' (maybe not a font file?)", (char*)alloca(512), resourceFileName));
		goto free_resources;
	}


	K15_SetFontName(&fontFormat, resourceName);
	K15_SetFontGlyphRange(&fontFormat, startCharacter, endCharacter);

	float scaleFactor = stbtt_ScaleForPixelHeight(&fontInfo, fontSize);
	stbtt_GetFontVMetrics(&fontInfo, &ascent, &descent, &lineGap);
	
	baseLine = (int)(ascent * scaleFactor);

	//loop over all characters to calculate the size of the font texture
	for (int codePoint = startCharacter;
		codePoint < endCharacter;
		++codePoint)
	{
		int boxLeft = 0;
		int boxRight = 0;
		int boxTop = 0;
		int boxBottom = 0;

		stbtt_GetCodepointBitmapBox(&fontInfo, codePoint, scaleFactor, scaleFactor, &boxLeft, &boxTop, &boxRight, &boxBottom);

		int boxWidth = boxRight - boxLeft;
		int boxHeight = boxBottom - boxTop;

		posX += boxWidth;

		K15_GlyphFormat currentGlyphFormat = {};

		currentGlyphFormat.character = codePoint;
		currentGlyphFormat.height = boxHeight;
		currentGlyphFormat.width = boxWidth;
		currentGlyphFormat.posX = posX;
		currentGlyphFormat.posY = posY;

		K15_AddFontGlyphData(&fontFormat, &currentGlyphFormat, codePoint);

		maxGlyphHeight = max(maxGlyphHeight, boxHeight);
		fontPixelWidth += boxWidth;
	}

	//save kerning data
	//save kerning for each possible character combination

	
	//very suboptimal approach
	int fontTextureWidth = K15_InternalGetNextPowerOfTwoSize(fontPixelWidth);
	int fontTextureHeight = K15_InternalGetNextPowerOfTwoSize(maxGlyphHeight);
	int fontTexturePixelCount = fontTextureHeight * fontTextureWidth;

// 	int fontTexturePixelCount = maxGlyphHeight * fontPixelWidth;
// 
//  	int fontTextureWidth = K15_InternalGetNextPowerOfTwoSize(fontTexturePixelCount / 2);
//  	int fontTextureHeight = K15_InternalGetNextPowerOfTwoSize(fontTexturePixelCount / 2);


	int stride = fontTextureWidth;

	fontPixelData = (byte*)malloc(fontTexturePixelCount);

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

			int32 kernAdvance = stbtt_GetCodepointKernAdvance(&fontInfo, char1, char2);

			K15_AddFontKerningData(&fontFormat, char1, char2, kernAdvance);
		}
	}

	//loop again to create the actual texture data
	for (int codePoint = startCharacter;
		codePoint < endCharacter;
		++codePoint)
	{
		K15_GlyphFormat* currentGlyphFormat = K15_GetFontGlyphData(&fontFormat, codePoint);

		uint32 glyphWidth = currentGlyphFormat->width;
		uint32 glyphHeight = currentGlyphFormat->height;
		uint32 glyphX = currentGlyphFormat->posX;

		uint32 pixelIndex = glyphX;

		//Create pixel data for the current character
		stbtt_MakeCodepointBitmap(&fontInfo, &fontPixelData[pixelIndex], glyphWidth, glyphHeight, stride, scaleFactor, scaleFactor, codePoint);
	}

	K15_SetFontTexture(&fontFormat, fontPixelData, fontTextureWidth, fontTextureHeight);

	if(K15_SaveFontFormatToFile(&fontFormat, p_OutputPath, K15_SAVE_FLAG_FREE_DATA) != K15_SUCCESS)
	{
		K15_SetResourceCompilerError(p_ResourceCompiler, K15_GenerateString("Could not open output file '%s'.", (char*)malloc(512), p_OutputPath));
	}
	else
	{
		compiled = K15_TRUE;
	}
	

free_resources:
	free(trueTypeData);
	free(resourceName);
	free(resourceFileName);
	free(fontPixelData);

	return compiled;
}
#endif //K15_RESOURCE_COMPILER_ENABLE_STB_TTF

/*********************************************************************************/
void K15_CreateDefaultResourceCompiler(K15_ResourceCompilerContext* p_ResourceCompilerContext)
{
	K15_ResourceCompiler* meshCompiler = (K15_ResourceCompiler*)malloc(sizeof(K15_ResourceCompiler));
	meshCompiler->name = "Mesh Compiler";
	meshCompiler->supportedExtensions = K15_CreateStringArray(1, ".dae");
	meshCompiler->error = 0;

#ifdef K15_RESOURCE_COMPILER_ENABLE_ASSIMP
	meshCompiler->compileResource = K15_CompileMeshResourceWithAssimp;
#endif //K15_RESOURCE_COMPILER_ENABLE_ASSIMP

	K15_ResourceCompiler* textureCompiler = (K15_ResourceCompiler*)malloc(sizeof(K15_ResourceCompiler));
	textureCompiler->name = "Texture Compiler";
	textureCompiler->supportedExtensions = K15_CreateStringArray(4, ".png", ".jpeg", ".tga");
	textureCompiler->error = 0;

#ifdef K15_RESOURCE_COMPILER_ENABLE_SQUISH
	textureCompiler->compileResource = K15_CompileTextureResourceWithSquish;
#endif //K15_RESOURCE_COMPILER_ENABLE_SQUISH

	K15_ResourceCompiler* fontCompiler = (K15_ResourceCompiler*)malloc(sizeof(K15_ResourceCompiler));
	fontCompiler->name = "Font Compiler";
	fontCompiler->supportedExtensions = K15_CreateStringArray(1, ".ttf");
	fontCompiler->error = 0;

#ifdef K15_RESOURCE_COMPILER_ENABLE_STB_TTF
	fontCompiler->compileResource = K15_CompileFontResourceWithStbTTF;
#endif //K15_RESOURCE_COMPILER_ENABLE_STB_TTF

	//fontCompiler->compileResource = K15_CompileFontResourceWithSTB;

	p_ResourceCompilerContext->resourceCompiler[K15_RESOURCE_COMPILER_MESH] = meshCompiler;
	p_ResourceCompilerContext->resourceCompiler[K15_RESOURCE_COMPILER_TEXTURE] = textureCompiler;
 	p_ResourceCompilerContext->resourceCompiler[K15_RESOURCE_COMPILER_FONT] = fontCompiler;
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
	char* inputPath = K15_GetConfigValueAsString(&resourceFileConfig, "Source");
	const char* argumentInputPath = p_ResourceCompilerContext->argumentParser->inputPath;
	const char* argumentOutputPath = p_ResourceCompilerContext->argumentParser->outputPath;
	char* outputCompletePath = 0;
	char* inputCompletePath = 0;

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

	if (!inputPath)
	{
		K15_LOG_ERROR_MESSAGE("No input path for resource '%s' (Add 'Source' value to the resource file)", p_ResourceFile);
		goto free_resources;
	}

	inputCompletePath = K15_ConcatStrings(argumentInputPath, inputPath);
	outputCompletePath = K15_ConcatStrings(argumentOutputPath, outputPath);

	if (!p_ResourceCompilerContext->argumentParser->replace)
	{
		if (K15_FileExists(outputCompletePath))
		{
			K15_LOG_WARNING_MESSAGE("Will not compile resource '%s' as the resource has already been compiled to '%s'. Provide the '-u' flag to override already compiled resource files.", inputCompletePath, outputCompletePath);
			goto free_resources;
		}
	}

	if (K15_FileExists(inputCompletePath) == K15_FALSE)
	{
		K15_LOG_ERROR_MESSAGE("Input file '%s' does not exist.", inputCompletePath);
		goto free_resources;
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

	compileResult = resourceCompiler->compileResource(resourceCompiler, &resourceFileConfig, inputCompletePath, outputCompletePath);

	if (compileResult == K15_FALSE)
	{
		K15_LOG_ERROR_MESSAGE("Error during compilation of resource '%s' (%s).", p_ResourceFile, resourceCompiler->error);
		goto free_resources;
	}

	K15_LOG_SUCCESS_MESSAGE("Successful compiled resource '%s' to '%s'.", p_ResourceFile, outputPath);

free_resources:
	free(resourceType);
	free(outputPath);
	free(inputPath);
	free(inputCompletePath);
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