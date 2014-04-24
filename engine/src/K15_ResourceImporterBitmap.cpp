/**
 * @file K15_ResourceImporterBitmap.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/04/14
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

#include "K15_ResourceImporterBitmap.h"
#include "K15_RawData.h"

namespace K15_Engine { namespace Core {
	/*********************************************************************************/
	ResourceImporterBitmap::ResourceImporterBitmap()
		: ResourceImporterBase("Bitmap")
	{

	}
	/*********************************************************************************/
	ResourceImporterBitmap::~ResourceImporterBitmap()
	{

	}
	/*********************************************************************************/
	void ResourceImporterBitmap::getExtensionList(ExtensionSet& p_ExtensionSet)
	{
		p_ExtensionSet.insert(".bmp");
		p_ExtensionSet.insert(".BMP");
	}
	/*********************************************************************************/
	void ResourceImporterBitmap::getMagicNumber(MagicNumberSet& p_RawData)
	{
		static RawData bmpSignature((byte*)0x424D,4);
		p_RawData.push_back(bmpSignature);
	}
	/*********************************************************************************/
	
}}// end of K15_Engine::Core namespace