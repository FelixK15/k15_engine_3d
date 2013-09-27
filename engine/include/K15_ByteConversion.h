/**
 * @file K15_ByteConversion.h
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/08/11
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
#pragma once

#ifndef __K15_BYTECONVERSION__
#define __K15_BYTECONVERSION__

#include "K15_UsefulMacros.h"

class K15_API_EXPORT ByteConversion
{
	template <typename T>
	static T ByteToKilobyte(T byte);

	template <typename T>
	static T ByteToMegabyte(T byte);

	template <typename T>
	static T ByteToGigabyte(T byte);

	template <typename T>
	static T KilobyteToByte(size_t kilobyte);

	template <typename T>
	static T KilobyteToMegabyte(size_t kilobyte);

	template <typename T>
	static T KilobyteToGigabyte(size_t kilobyte);

	template <typename T>
	static T MegabyteToByte(size_t megabyte);

	template <typename T>
	static T MegabyteToKilobyte(size_t megabyte);

	template <typename T>
	static T MegabyteToGigabyte(size_t megabyte);

	template <typename T>
	static T GigabyteToByte(size_t gigabyte);

	template <typename T>
	static T GigabyteToKilobyte(size_t gigabyte);

	template <typename T>
	static T GigabyteToMegabyte(size_t gigabyte);
};

#include "../src/K15_ByteConversion.inl"


#endif //__K15_K15_BYTECONVERSION__