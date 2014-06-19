/**
 * @file K15_TiffResourceImporter.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/04/15
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 */

#include "K15_PrecompiledHeader.h"

#include "K15_ResourceImporterTiff.h"
#include "K15_RawData.h"
#include "K15_Texture.h"
#include "K15_RendererBase.h"
#include "K15_ColorRGBA.h"

#include "K15_Image.h"

#include "tiffio.h"
#include "tiffio.hxx"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	K15_IMPLEMENT_RTTI_BASE(Core,ResourceImporterTiff,ResourceImporterBase);
	/*********************************************************************************/

	/*********************************************************************************/
	ResourceImporterTiff::ResourceImporterTiff()
		: ResourceImporterBase("Tiff Image")
	{

	}
	/*********************************************************************************/
	ResourceImporterTiff::~ResourceImporterTiff()
	{

	}
	/*********************************************************************************/
	void ResourceImporterTiff::getExtensionList(ExtensionSet& p_ExtensionSet)
	{
		p_ExtensionSet.insert(".tiff");
		p_ExtensionSet.insert(".TIFF");
		p_ExtensionSet.insert(".tif");
	}
	/*********************************************************************************/
	void ResourceImporterTiff::getMagicNumber(MagicNumberSet& p_MagicNumber)
	{
		static byte MagicNumber_LittleEndian[] = {0x49,0x49,0x2A,0x00};
		static byte MagicNumber_BigEndian[] = {0x4D,0x4D,0x00,0x2A};

		p_MagicNumber.push_back(RawData(MagicNumber_LittleEndian,4));
		p_MagicNumber.push_back(RawData(MagicNumber_BigEndian,4));
	}
	/*********************************************************************************/
	ResourceBase* ResourceImporterTiff::_load(const RawData& p_ResourceData, const Rtti& p_ResourceType)
	{
		StackAllocator* frameAllocator = g_Application->getFrameAllocator();

 		//set data to stream
		StringStream dataStream;
		dataStream.write((const char*)p_ResourceData.data,p_ResourceData.size);
		
		TIFF* tiffImg = TIFFStreamOpen("tiff_image",(std::istream*)&dataStream);

		if(!tiffImg)
		{
			setError("Invalid memory stream.");
			return 0;
		}
		uint32 width = 0;
		uint32 height = 0;
		TIFFGetField(tiffImg,TIFFTAG_IMAGEWIDTH,&width);
		TIFFGetField(tiffImg,TIFFTAG_IMAGELENGTH,&height);

		uint32 pixelCount = width*height;
		uint32 pixelBufferSize = pixelCount * sizeof(uint32);
		uint32* pixelBuffer = (uint32*)K15_NEW_SIZE(Allocators[AC_GENERAL],pixelBufferSize) uint32; //pixels are packed into 32 bit. 8bit per component

		if(!TIFFReadRGBAImage(tiffImg,width,height,pixelBuffer,0))
		{
			setError("Could not retrieve pixels.");
			TIFFClose(tiffImg);
			return 0;
		}
		
		ResourceBase* resource = 0;

		if(p_ResourceType.isInstanceOf(Texture::TYPE))
		{
			resource = _loadTexture(width,height,pixelBuffer);
		}
		else if(p_ResourceType.isInstanceOf(Image::TYPE))
		{
			resource = _loadImage(width,height,pixelBuffer);
		}

		K15_DELETE_SIZE(Allocators[AC_GENERAL],pixelBuffer,pixelBufferSize);

		return resource;
	}
	/*********************************************************************************/
	Texture* ResourceImporterTiff::_loadTexture(uint32 p_Width, uint32 p_Height, uint32* p_PixelBuffer)
	{
		Texture::CreationOptions opts;
		opts.createMipMaps = true;
		opts.height = p_Height;
		opts.width = p_Width;
		opts.pixelFormat = RendererBase::PF_RGBA_8_U;
		opts.pixels.data = (byte*)p_PixelBuffer;
		opts.pixels.size = (p_Width * p_Height) * sizeof(uint32);
		return K15_NEW Texture(opts);
	}
	/*********************************************************************************/
	Image* ResourceImporterTiff::_loadImage(uint32 p_Width, uint32 p_Height, uint32* p_PixelBuffer)
	{
		return K15_NEW Image(p_Width,p_Height,(ColorRGBA*)p_PixelBuffer);
	}
	/*********************************************************************************/
}}// end of K15_Engine::Core namespace