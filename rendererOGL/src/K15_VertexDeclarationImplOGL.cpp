/**
 * @file K15_VertexDeclarationImplOGL.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/11
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

#include "K15_VertexDeclarationImplOGL.h"
#include "K15_RendererBase.h"
#include "K15_LogManager.h"

namespace K15_Engine { namespace Rendering { namespace OGL {
  /*********************************************************************************/
  const GLenum VertexDeclarationImplOGL::GLVertexElementTypeConverter[VertexElement::ET_COUNT] = {
    GL_FLOAT,         //ET_FLOAT1
    GL_FLOAT,         //ET_FLOAT2
    GL_FLOAT,         //ET_FLOAT3
    GL_FLOAT,         //ET_FLOAT4

    GL_HALF_FLOAT,    //ET_HALF1
    GL_HALF_FLOAT,    //ET_HALF2
    GL_HALF_FLOAT,    //ET_HALF3
    GL_HALF_FLOAT     //ET_HALF4
  }; //VertexElementTypeConverter
  /*********************************************************************************/

  /*********************************************************************************/
  VertexDeclarationImplOGL::VertexDeclarationImplOGL()
  {
    glGenVertexArrays(1,&m_VertexArrayHandle);
  }
  /*********************************************************************************/
  VertexDeclarationImplOGL::~VertexDeclarationImplOGL()
  {
    glDeleteVertexArrays(1,&m_VertexArrayHandle);
  }
  /*********************************************************************************/
  void VertexDeclarationImplOGL::updateElements(const VertexDeclaration::VertexElementArray& p_Elements)
  {
    glBindVertexArray(m_VertexArrayHandle);
    for(VertexDeclaration::VertexElementArray::const_iterator iter = p_Elements.begin();iter != p_Elements.end();++iter)
    {
      glVertexAttribPointer(iter->index,iter->size,GLVertexElementTypeConverter[iter->type],GL_FALSE,m_Declaration->getVertexSize(),(const void*)iter->offset);
    }
    glBindVertexArray(0);

    if(glGetError() != GL_NO_ERROR)
    {
      _LogError("Could not create vertex declaration. %s",g_Application->getRenderer()->getLastError().c_str());
    }
  }
  /*********************************************************************************/
}}}//end of K15_Engine::Rendering::OGL namespace