/**
 * @file K15_Serializer.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/01/03
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

#ifndef _K15Engine_Core_Serializer_h_
#define _K15Engine_Core_Serializer_h_

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#	include "K15_Object.h"
#endif //K15_USE_PRECOMPILED_HEADERS

namespace K15_Engine { namespace Core {
	template<class T>
	class SerializableObject
	{
	public:
		void serialize(const String& p_FileName)
		{
			//Serializer::serialize(&serializer,(T*)this);
		}
	};// end of * class declaration

// 	class K15_CORE_API Serializer
// 	{
// 	public:
// 		static void serialize(SerializerBase& p_Serializer,Application* application);
// 		static void serialize(SerializerBase* p_Serializer,AlphaState* alphastate);
// 	};
}}// end of K15_Engine::Core namespace

#endif //