/**
 * @file K15_VertexBufferCache.cpp
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
 */

#include "K15_PrecompiledHeader.h"
#include "K15_VertexBufferCache.h"

#include "K15_VertexData.h"
#include "K15_VertexBuffer.h"
#include "K15_VertexDeclaration.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	VertexBufferCache::VertexBufferCacheList VertexBufferCache::ms_VertexBufferCache;
	/*********************************************************************************/

	/*********************************************************************************/
	VertexData VertexBufferCache::allocateVertexData(const ObjectName& p_VertexDeclaration, uint32 p_VertexCount)
	{
		VertexBuffer* vertexBuffer = 0;
    VertexDeclaration* vertexDeclaration = 0;
		uint32 vertexDataSize = 0;
		uint32 offset = 0;

		for(VertexBufferCacheList::iterator iter = ms_VertexBufferCache.begin();
			iter != ms_VertexBufferCache.end(); ++iter)
		{
			VertexBufferChacheEntry& entry = (*iter);

			if(entry.vertexDeclaration->getDeclarationName() == p_VertexDeclaration)
			{
				vertexBuffer = entry.vertexBuffer;
				vertexDataSize = p_VertexCount * entry.vertexDeclaration->getVertexSize();

				if(vertexBuffer->getFreeSize() >= vertexDataSize)
				{
					vertexBuffer = entry.vertexBuffer;
					offset = vertexBuffer->getUsedSize();
				}
			}
		}

    return VertexData(vertexDeclaration, vertexBuffer, offset, p_VertexCount);
	}
	/*********************************************************************************/
}} // end of K15_Engine::Rendering namespace