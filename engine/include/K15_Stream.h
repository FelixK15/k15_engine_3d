/**
 * @file K15_Stream.h
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/08/09
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
 * @todo program
 */
#pragma once

#ifndef __K15_STREAM__
#define __K15_STREAM__

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

#include "K15_DynamicArray.h"

namespace K15_EngineV2
{
	class K15_CORE_API Stream
	{
	public:
		/*Stream();
		~Stream();

		void Save(const byte* pBuffer,U32 iSizeBuffer);
		void Save(const String& sFilename);

		void Load(const byte* pBuffer,U32 iSizeBuffer);
		void Load(const String& sFilename);

		void Insert(Object *pObject);
		void Remove(Object *pObject);

		void Clear();

		U32 Size();

		Object *GetObjectAt(U32 iPos);
		bool IsTopLevel(Object *pObject);

		U32 GetDiscUsage();
		U32 GetMemoryUsage();

	private:
		DynamicArray<Object*> m_arrObjectArray;
		DynamicArray<Object*> m_arrOrderedObjectsArray;

		U32 m_iStreamSize;*/
	};
}

#endif //__K15_STREAM__