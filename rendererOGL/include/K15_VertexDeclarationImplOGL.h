/**
 * @file K15_VertexDeclarationImplOGL.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_RendererOGL_VertexDeclarationImpl_h_
#define _K15Engine_RendererOGL_VertexDeclarationImpl_h_

#ifndef K15_RENDERER_OGL_USE_PREDEFINED_HEADERS
#	include "K15_RendererOGLPrerequisites.h"
#endif //K15_RENDERER_OGL_USE_PREDEFINED_HEADERS

#include "K15_VertexDeclaration.h"

namespace K15_Engine { namespace Rendering { namespace OGL {
  class VertexDeclarationImplOGL : public VertexDeclarationImplBase
  {
  public:
    /*********************************************************************************/
    static const GLenum GLVertexElementTypeConverter[VertexElement::ET_COUNT];
    /*********************************************************************************/

  public:
    VertexDeclarationImplOGL();
    ~VertexDeclarationImplOGL();

    virtual void updateElements(const VertexDeclaration::VertexElementArray& p_Elements) OVERRIDE;
    GLuint getOGLHandle() const {return m_VertexArrayHandle;}
  private:
    GLuint m_VertexArrayHandle;
  }; // end of VertexDeclarationImplOGL class declaration
}}}//end of K15_Engine::Rendering::OGL namespace

#endif //_K15Engine_RendererOGL_VertexDeclarationImpl_h_