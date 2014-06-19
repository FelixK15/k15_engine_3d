/**
 * @file K15_SerialSizer.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/01/31
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

#ifndef _K15Engine_Serialization_SerialSizer_h_
#define _K15Engine_Serialization_SerialSizer_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_SerializerBase.h"

namespace K15_Engine { namespace Serialization {
	class K15_CORE_API SerialSizer : public SerializerBase
	{
	public:
		SerialSizer();
		~SerialSizer();

		virtual byte* getBufferPointer() const OVERRIDE;
	protected:
		virtual void _io(byte* p_Data, uint32 p_Size) OVERRIDE;
		virtual void _io(int32& p_Integer) OVERRIDE;
		virtual void _io(uint32& p_Integer) OVERRIDE;
		virtual void _io(float& p_Float) OVERRIDE;
	};// end of SerialSizer class declaration
}}// end of K15_Engine::Core namespace

#endif //