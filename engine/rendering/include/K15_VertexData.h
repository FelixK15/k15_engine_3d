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

namespace K15_Engine { namespace Rendering { 
  struct K15_CORE_API VertexData
  {
    VertexDeclaration* vertexDeclaration;
    VertexBuffer* vertexBuffer;
    uint32 bufferOffset;
    uint32 vertexOffset;
    uint32 vertexCount;
  }; //end of VertexData struct declaration
}} //end of K15_Engine::Rendering

#endif //_K15Engine_Rendering_VertexData_h_