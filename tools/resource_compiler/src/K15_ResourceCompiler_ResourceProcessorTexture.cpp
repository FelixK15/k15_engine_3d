#include "K15_ResourceCompiler_ResourceProcessorTexture.h"

#include "K15_ResourceCompiler_CompilerEnvironment.h"
#include "K15_ResourceCompiler_ResourceProcessorEnvironment.h"
#include "K15_ResourceCompiler_StringUtil.h"
#include "K15_ResourceCompiler_SearchUtil.h"
#include "K15_ResourceCompiler_ResourceData.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include <K15_TextureFormat.h>
#include <K15_DataAccessHelper.h>

#include <stb_image.h>
#include <stb_image_resize.h>
#include <squish.h>
#include <cmath>

namespace K15_ResourceCompiler
{
	/*********************************************************************************/
	namespace internal
	{
		/*********************************************************************************/
		int Log2(int x)
		{
			return static_cast<int>(floor(log10<int>(x) / log10<int>(2)));
		}
		/*********************************************************************************/



		/*********************************************************************************/
		bool isPowerOfTwo(uint32 p_Number)
		{
			return ((p_Number != 0) && !(p_Number & (p_Number - 1)));
		}
		/*********************************************************************************/



		/*********************************************************************************/
		uint32 getMipMapCount(uint32 p_Width, uint32 p_Height)
		{
			return std::max(0, Log2(std::max(p_Width, p_Height)) - 2); //no 1x1 and 2x2 mip maps
		}
		/*********************************************************************************/



		/*********************************************************************************/
		uint32 getUncompressedTextureSizeIncludingMipMaps(uint32 p_Width, uint32 p_Height, uint32 p_ComponentsCount)
		{
			uint32 mipmapCount = getMipMapCount(p_Width, p_Height) + 1;
			uint32 memoryNeeded = 0;

			for(uint32 mipmapIndex = 0;
				mipmapIndex < mipmapCount;
				++mipmapIndex)
			{
				memoryNeeded += p_Width * p_Height * p_ComponentsCount;

				p_Width /= 2;
				p_Height /= 2;
			}

			return memoryNeeded;
		}
		/*********************************************************************************/



		/*********************************************************************************/
		void convertNormalImageData(byte* p_ImageData, uint32 p_PixelCount)
		{
			byte* currentPixel = 0;

			for (uint32 pixelIndex = 0;
				 pixelIndex < p_PixelCount;
				 ++pixelIndex)
			{
				currentPixel = p_ImageData + (pixelIndex * 4); //4 bytes per pixel

				currentPixel[0] = 0; //disable R channel 
			}
		}
		/*********************************************************************************/



		/*********************************************************************************/
		uint32 getCompressedTextureSizeIncludingMipMaps(uint32 p_Width, uint32 p_Height, uint32 p_CompressionUsed)
		{
			uint32 mipmapCount = getMipMapCount(p_Width, p_Height) + 1;
			uint32 memoryNeeded = 0;

			for(uint32 mipmapIndex = 0;
				mipmapIndex < mipmapCount;
				++mipmapIndex)
			{
				memoryNeeded += squish::GetStorageRequirements(p_Width, p_Height, p_CompressionUsed);

				p_Width /= 2;
				p_Height /= 2;
			}

			return memoryNeeded;
		}
		/*********************************************************************************/
	}
	/*********************************************************************************/


	/*********************************************************************************/
	ResourceprocessorTexture::ResourceprocessorTexture(const CompilerEnvironment& p_CompilerEnvironment)
		: ResourceProcessorBase(p_CompilerEnvironment)
	{

	}
	/*********************************************************************************/
	ProcessorEnvironment ResourceprocessorTexture::getProcessorEnvironment() const
	{
		static ProcessorEnvironment environment = {".k15tex", "Texture Processor"};

		return environment;
	}
	/*********************************************************************************/
	bool ResourceprocessorTexture::canProcess(const String& p_FileExtension) const
	{
		static String extensions[] = {".tga", ".jpg", ".jpeg", ".png", ".tiff"};

		static StringSet imageExtensions = StringSet(extensions, extensions + sizeof(extensions) / sizeof(extensions[0])); 

		return std::find(imageExtensions.begin(), imageExtensions.end(), p_FileExtension) != imageExtensions.end();
	}
	/*********************************************************************************/
	ResourceProcessResult ResourceprocessorTexture::process(const ResourceData& p_ResourceData)
	{
		ResourceProcessResult result;
		result.processState = RPS_FAILURE;

		String texturePath = p_ResourceData.filePath;
		String textureName = StringUtil::getFileNameWithoutExtension(texturePath);

		String outputPath = getCompilerEnvironment().getOutputPath();
		outputPath += textureName;
		outputPath += getProcessorEnvironment().outputExtension;

		int width = 0;
		int height = 0;
		int colorComponents = 0;
		int compressionUsed = squish::kDxt5;

		bool powerOfTwo = false;

		//load imagedata from image file
		byte* imageData = stbi_load(texturePath.c_str(), &width, &height, &colorComponents, 4);

		// get image information from image file
		if(!imageData)
		{
			setError("Could not read \"" + texturePath + "\"");
			return result;
		}

		if(!internal::isPowerOfTwo(width) || !internal::isPowerOfTwo(height))
		{
			setError("Texture must be power of two.");
			stbi_image_free(imageData);
			return result;
		}

		assert(colorComponents == 4);

		//check if the current texture is a normal map
		ResourceParameters::const_iterator normalMapIter = std::find_if(p_ResourceData.parameters.begin(),
																	 p_ResourceData.parameters.end(),
																	 SearchUtil::findTextureParameterNormalMap);

		if (normalMapIter != p_ResourceData.parameters.end())
		{
			internal::convertNormalImageData(imageData, width * height);
		}

		// uncompressed texture size (including mip maps) and allocate memory
		uint32 uncompressedTextureSize = internal::getUncompressedTextureSizeIncludingMipMaps(width, height, colorComponents);
		byte* uncompressedTextureMemory = (byte*)malloc(uncompressedTextureSize);

		//copy imagedata memory to previously allocated uncompressed texture memory
		memcpy(uncompressedTextureMemory, imageData, width * height * colorComponents);
		stbi_image_free(imageData);

		uint32 mipMapCount = internal::getMipMapCount(width, height);
		byte* previousImageBlock = uncompressedTextureMemory;
		byte* currentImageBlock = uncompressedTextureMemory + (width * height * colorComponents);
		uint32 previousImageWidth = width;
		uint32 previousImageHeight = height;
		uint32 currentImageWidth = width / 2;
		uint32 currentImageHeight = height / 2;

		//resize image n times (n = number of mip maps) and write the data of the resized images into the uncompressed texture memory
		for(uint32 mipMapIndex = 0;
			mipMapIndex < mipMapCount;
			++mipMapIndex)
		{
			stbir_resize_uint8(previousImageBlock, previousImageWidth, previousImageHeight, 0,
								currentImageBlock, currentImageWidth, currentImageHeight, 0, colorComponents);

			previousImageBlock = currentImageBlock;
			previousImageHeight = currentImageHeight;
			previousImageWidth = currentImageWidth;

			currentImageHeight = previousImageHeight / 2;
			currentImageWidth = previousImageWidth / 2;

			currentImageBlock += (currentImageHeight * currentImageWidth * colorComponents);
		}
		
		//texture data for each mip map are now tightly packed into uncompressedTextureMemory variable
		//calculate texture size for the compressed image data and allocate memory to store compressed image data
		uint32 compressedTextureSize = internal::getCompressedTextureSizeIncludingMipMaps(width, height, compressionUsed);
		byte* compressedTextureMemory = (byte*)malloc(compressedTextureSize);
		byte* currentCompressedImageBlock = compressedTextureMemory;

		currentImageBlock = uncompressedTextureMemory;
		currentImageHeight = height;
		currentImageWidth = width;

		//compress each mip map individually
		for(uint32 mipMapIndex = 0;
			mipMapIndex < mipMapCount + 1; //+1 because we also need to compress the actual image
			++mipMapIndex)
		{
			squish::CompressImage(uncompressedTextureMemory, currentImageWidth, currentImageHeight, currentCompressedImageBlock, compressionUsed);

			currentCompressedImageBlock += squish::GetStorageRequirements(currentImageWidth, currentImageHeight, compressionUsed);

			currentImageHeight /= 2;
			currentImageWidth /= 2;
		}

		//save texture values and save texture format
		K15_TextureFormat textureFormat;

		K15_SetTextureName(&textureFormat, textureName.c_str());

		textureFormat.width = width;
		textureFormat.height = height;
		textureFormat.colorComponentCount = colorComponents;
		textureFormat.compression = K15_TEXTURE_DXT5_COMPRESSION;
		textureFormat.mipMapCount = mipMapCount;

		K15_SetTextureMipMapCount(&textureFormat, mipMapCount);

		currentImageWidth = width;
		currentImageHeight = height;
		uint32 compressedTextureMemoryOffset = 0;

		for(uint32 mipmapIndex = 0;
			mipmapIndex < mipMapCount + 1; //+1 as we also need to save the original image data
			++mipmapIndex)
		{
			K15_SetTextureMipMap(&textureFormat, mipmapIndex, compressedTextureMemory + compressedTextureMemoryOffset);
			compressedTextureMemoryOffset += squish::GetStorageRequirements(currentImageWidth, currentImageHeight, compressionUsed);

			currentImageHeight /= 2;
			currentImageWidth /= 2;
		}

		K15_DataAccessContext accessContext;

		if (K15_CreateDataAccessContextFromFile(&accessContext, outputPath.c_str()) == K15_SUCCESS)
		{
			//error
		}

		if (K15_InternalSaveTextureFormat(&accessContext, &textureFormat, K15_SAVE_FLAG_FREE_DATA) == K15_SUCCESS)
		{
			result.processState = RPS_SUCCESS;
		}

		return result;
	}
	/*********************************************************************************/
	ResourceData ResourceprocessorTexture::gatherData(const String& p_File) const 
	{
		ResourceData resourceData;

		resourceData.filePath = p_File;

		return resourceData;
	}
	/*********************************************************************************/
} // end of K15_ResourceCompiler namespace
