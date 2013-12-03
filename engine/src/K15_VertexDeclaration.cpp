/**
 * @file K15_VertexDeclaration.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
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

#include "K15_VertexDeclaration.h"

namespace K15_Engine { namespace Rendering {
  /*********************************************************************************/
  void VertexDeclarationImplBase::setVertexDeclaration(VertexDeclaration* p_Declaration)
  {
    m_VertexDeclaration = p_Declaration;
  }
  /*********************************************************************************/
  VertexDeclaration* VertexDeclarationImplBase::getVertexDeclaration() const
  {
    return m_VertexDeclaration;
  }
  /*********************************************************************************/

  /*********************************************************************************/
  VertexDeclaration::VertexDeclaration()
    : m_Dirty(true),
      m_Elements(),
      m_Size(0)
  {

  }
  /*********************************************************************************/
  VertexDeclaration::VertexDeclaration(const VertexElementArray& p_Elements)
  {
    for(VertexElementArray::const_iterator iter = p_Elements.begin();iter != p_Elements.end();++iter)
    {
      if(_validateElement((*iter)))
      {
        m_Elements.push_back((*iter));
      }
    }
  }
  /*********************************************************************************/
  VertexDeclaration::VertexDeclaration(const String& p_DeclarationString)
  {
    _parseDeclarationString(p_DeclarationString);
  }
  /*********************************************************************************/
  VertexDeclaration::~VertexDeclaration()
  {
    removeAllElements();
  }
  /*********************************************************************************/
  void VertexDeclaration::addElement(VertexElement p_Element)
  {
    if(_validateElement(p_Element))
    {
      p_Element.Offset = getVertexSize();
      p_Element.Index = getElementCount();
      m_Elements.push_back(p_Element);
    }
  }
  /*********************************************************************************/
  void VertexDeclaration::addElement(Enum p_Semantic, Enum p_Type)
  {
    VertexElement element;
    element.Semantic = p_Semantic;
    element.Type = p_Type;
    
    addElement(element);
  }
  /*********************************************************************************/
  void VertexDeclaration::insertElement(uint32 p_Index, VertexElement p_Element)
  {
    if(_validateElement(p_Element))
    {
      p_Element.Index = p_Index;
      p_Element.Offset = getVertexSize();
      m_Elements[p_Index] = p_Element;
    }
  }
  /*********************************************************************************/
  void VertexDeclaration::insertElement(uint32 p_Index, Enum p_Semantic, Enum p_Type)
  {
    VertexElement element;
    element.Semantic = p_Semantic;
    element.Type = p_Type;

    insertElement(p_Index,element);
  }
  /*********************************************************************************/
  void VertexDeclaration::addElements(const VertexElementArray& p_Elements)
  {
    for(VertexElementArray::const_iterator iter = p_Elements.begin();iter != p_Elements.end();++iter)
    {
      addElement((*iter));
    }
  }
  /*********************************************************************************/
  void VertexDeclaration::addElementsByDeclarationString(const String& p_DeclarationString)
  {
    _parseDeclarationString(p_DeclarationString);
  }
  /*********************************************************************************/
  void VertexDeclaration::changeElement(uint32 p_Index, VertexElement p_Element)
  {
    if(_validateElement(p_Element))
    {
      m_Elements[p_Index].Type = p_Element.Type;
      m_Elements[p_Index].Semantic = p_Element.Semantic;
      _recalculateOffsets();
    }
  }
  /*********************************************************************************/
  void VertexDeclaration::changeElement(uint32 p_Index, uint32 p_Offset, Enum p_Semantic, Enum p_Type)
  {
    VertexElement element;
    element.Semantic = p_Semantic;
    element.Type = p_Type;

    changeElement(p_Index,element);
  }
  /*********************************************************************************/
  void VertexDeclaration::removeElement(uint32 p_Index)
  {
    if(m_Elements.size() > p_Index)
    {
      //todo
    }
  }
  /*********************************************************************************/
  void VertexDeclaration::removeAllElements()
  {
    m_Elements.clear();
  }
  /*********************************************************************************/
  const VertexElement& VertexDeclaration::getElement(uint32 p_Index)
  {
    return m_Elements.at(p_Index);
  }
  /*********************************************************************************/
  uint32 VertexDeclaration::getVertexSize()
  {
    if(m_Dirty)
    {
      m_Size = 0;
      for(VertexElementArray::const_iterator iter = m_Elements.begin();iter != m_Elements.end();++iter)
      {
        if(iter->Type > 0 && iter->Type < VertexElement::ET_HALF1)
        {
          m_Size += sizeof(float) * (iter->Type+1); 
        }
        else if(iter->Type >= VertexElement::ET_HALF1)
        {
          m_Size += sizeof(uint16) * ((iter->Type+1)*0.5);
        }
      }

      m_Dirty = false;
    }

    return m_Size;
  }
  /*********************************************************************************/
  uint32 VertexDeclaration::getElementCount() const
  {
    return m_Elements.size();
  }
  /*********************************************************************************/

}} //end of K15_Engine::Rendering namespace