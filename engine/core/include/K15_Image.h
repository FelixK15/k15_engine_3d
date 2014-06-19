/**
 * @file K15_Image.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Core_Image_h_
#define _K15Engine_Core_Image_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_ResourceBase.h"
#include "K15_ColorRGBA.h"

namespace K15_Engine { namespace Core {
	class K15_CORE_API Image : public ResourceBase, public ResourceAllocatedObject
	{
		K15_DECLARE_RTTI;

	public:
		Image();
		Image(const Image& p_Rs);
		Image(uint32 p_Width, uint32 p_Height);
		Image(uint32 p_Width, uint32 p_Height, ColorRGBA* p_Pixels);
		~Image();

		INLINE ColorRGBA* getPixels() const;
		ColorRGBA getPixel(uint32 p_PosX,uint32 p_PosY);
		void setPixel(uint32 p_PosX, uint32 p_PosY, const ColorRGBA& p_Color);

		INLINE uint32 getWidth() const;
		INLINE uint32 getHeight() const;
		INLINE uint32 getAmountPixels() const;

		const Image& operator=(const Image& p_Rs);

		bool operator==(const Image& p_Rs);
		bool operator!=(const Image& p_Rs);

	private:
		ColorRGBA* m_Pixels;
		uint32 m_Width;
		uint32 m_Height;
	}; //Image class declaration
#	include "K15_Image.inl"
}} // end of K15_Engine::Core namespace

#endif //_K15Engine_Core_Image_h_