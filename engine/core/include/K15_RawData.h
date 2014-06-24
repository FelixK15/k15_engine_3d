/**
 * @file K15_ResourceData.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/20
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

#ifndef _K15Engine_Core_RawData_h_
#define _K15Engine_Core_RawData_h_

#include "K15_Prerequisites.h"

namespace K15_Engine { namespace Core { 
	struct K15_CORE_API RawData
	{
	public:
		RawData();
		RawData(byte* p_Data,uint32 p_Size);
		~RawData();

		byte* data;
		uint32 size;
	}; //end of RawData struct.
}}//end of K15_Engine::Core namespace

#endif