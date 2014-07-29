/**
 * @file K15_VertexBufferCache.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/01/08
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

#ifndef _K15Engine_Rendering_VertexBufferCache_h_
#define _K15Engine_Rendering_VertexBufferCache_h_

#include "K15_Prerequisites.h"
#include "K15_HashedString.h"

namespace K15_Engine { namespace Rendering {
	class K15_CORE_API VertexBufferCache
	{
	private:
		struct VertexBufferChacheEntry
		{
			VertexDeclaration* vertexDeclaration;
			VertexBuffer* vertexBuffer;
		};

	public:
		typedef DynamicArray(VertexBufferChacheEntry) VertexBufferCacheList;

	public:

		static VertexData allocateVertexData(const ObjectName& p_VertexDeclaration, uint32 p_VertexCount);

	private:
		
		static VertexBufferCacheList ms_VertexBufferCache;
	};// end of VertexBufferCache class declaration
}}// end of K15_Engine::Rendering namespace

#endif //_K15Engine_Rendering_VertexBufferCache_h_