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
  const char VertexDeclaration::SemanticToCharConverter[VertexElement::ES_COUNT] = {
    'P',       //ES_POSITION
    'N',      //ES_NORMAL
    'T',      //ES_TEXCOORD
    'C'       //ES_COLOR
  }; //SemanticToCharconverter
  /*********************************************************************************/
  const char* VertexDeclaration::TypeToCharConverter[VertexElement::ET_COUNT] = {
    "F1", //ET_FLOAT1
    "F2", //ET_FLOAT2
    "F3", //ET_FLOAT3
    "F4", //ET_FLOAT4
    
    "H1", //ET_HALF1
    "H2", //ET_HALF2
    "H3", //ET_HALF3
    "H4"  //ET_HALF4
  }; //TypeToCharConverter
  /*********************************************************************************/

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
  void VertexDeclaration::_parseDeclarationString(const String& p_DeclarationString)
  {
    //P = Position      F1 = Float1     H1 = Half1
    //N = Normal        F2 = Float2     H2 = Half2
    //T = TexCoord      F3 = Float3     H3 = Half3
    //C = Color         F4 = Float4     H4 = Half4
    // Example: PF4NF4TF2 =   Position Float4   |   Normal Float4   |   TexCoord Float2
    //                            Element1            Element2            Element3
    static const int SIZE_ELEMENT = 3; //3 chars define 1 element.
    static String elementStr;
    static String::size_type pos;
    static VertexElement element;

    if(!p_DeclarationString.size() < SIZE_ELEMENT)
    {
      _LogError("Invalid vertex declaration string \"%s\"",p_DeclarationString.c_str());
      return;
    }
    
    pos = 0;
    
    while(pos < p_DeclarationString.size())
    {
      elementStr = p_DeclarationString.substr(pos,SIZE_ELEMENT);
      pos += SIZE_ELEMENT;

      if(elementStr[0] == 'P')
      {
        element.Semantic = VertexElement::ES_POSITION;
      }
      else if(elementStr[0] == 'N')
      {
        element.Semantic = VertexElement::ES_NORMAL;
      }
      else if(elementStr[0] == 'T')
      {
        element.Semantic = VertexElement::ES_TEXCOORD;
      }
      else if(elementStr[0] == 'C')
      {
        element.Semantic = VertexElement::ES_COLOR;
      }
      else
      {
        _LogError("Invalid element declaration \"%s\"",elementStr.c_str());
        continue;
      }

      if(elementStr.find_first_of("F1") != String::npos)
      {
        element.Type = VertexElement::ET_FLOAT1;
      }
      else if(elementStr.find_first_of("F2") != String::npos)
      {
        element.Type = VertexElement::ET_FLOAT2;
      }
      else if(elementStr.find_first_of("F3") != String::npos)
      {
        element.Type = VertexElement::ET_FLOAT3;
      }
      else if(elementStr.find_first_of("F4") != String::npos)
      {
        element.Type = VertexElement::ET_FLOAT4;
      }
      else if(elementStr.find_first_of("H1") != String::npos)
      {
        element.Type = VertexElement::ET_HALF1;
      }
      else if(elementStr.find_first_of("H2") != String::npos)
      {
        element.Type = VertexElement::ET_HALF2;
      }
      else if(elementStr.find_first_of("H3") != String::npos)
      {
        element.Type = VertexElement::ET_HALF3;
      }
      else if(elementStr.find_first_of("H4") != String::npos)
      {
        element.Type = VertexElement::ET_HALF4;
      }
      else
      {
        _LogError("Invalid element declaration \"%s\"",elementStr.c_str());
        continue;
      }

      addElement(element);
    }
    
  }
  /*********************************************************************************/
  void VertexDeclaration::_recalculateOffsets()
  {
    int counter = 0;

    for(VertexElementArray::iterator iter = m_Elements.begin();iter != m_Elements.end();++iter)
    {
      if(iter->Type > 0 && iter->Type < VertexElement::ET_HALF1)
      {
        iter->Offset = sizeof(float) * (iter->Type+1) * counter++; 
      }
      else if(iter->Type >= VertexElement::ET_HALF1)
      {
        iter->Offset = sizeof(uint16) * ((iter->Type+1)*0.5) * counter++;
      }
    }
  }
  /*********************************************************************************/
  bool VertexDeclaration::_validateElement(const VertexElement& p_Element)
  {
    bool valid = false;

    if(valid)
    {
      m_Dirty = true;
    }

    return valid;
  }
  /*********************************************************************************/
  String VertexDeclaration::getDeclarationString()
  {
    static String declarationString;

    declarationString.clear();

    for(VertexElementArray::const_iterator iter = m_Elements.begin();iter != m_Elements.end();++iter)
    {
      
    }

    return declarationString;
  }
  /*********************************************************************************/

}} //end of K15_Engine::Rendering namespace