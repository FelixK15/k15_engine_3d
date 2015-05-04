/**
 * @file K15_VertexData.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/11/29
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

#ifndef _K15Engine_Rendering_VertexData_h_
#define _K15Engine_Rendering_VertexData_h_

#include "K15_Prerequisites.h"
#include "K15_AllocatedObject.h"

namespace K15_Engine { namespace Rendering { 
  class K15_CORE_API VertexData : public RenderingAllocatedObject
  {
  public:
    typedef DynamicArray(Vertex*) VertexCache;

  public:
    VertexData(VertexDeclaration* p_VertexDeclaration, VertexBuffer* p_VertexBuffer, uint32 p_Offset, uint32 p_VertexCount);
    ~VertexData();

    INLINE VertexDeclaration* getVertexDeclaration() const;
    INLINE VertexBuffer* getVertexBuffer() const;
    INLINE uint32 getOffsetInBytes() const;
    INLINE uint32 getVertexCount() const;

    Vertex* getVertex(uint32 p_Index);

  private:
    VertexCache m_VertexCache;
    VertexDeclaration* m_VertexDeclaration;
    VertexBuffer* m_VertexBuffer;
    uint32 m_Offset;
    uint32 m_VertexCount;
  }; //end of VertexData struct declaration
  #include "K15_VertexData.inl"
}} //end of K15_Engine::Rendering

#endif //_K15Engine_Rendering_VertexData_h_
