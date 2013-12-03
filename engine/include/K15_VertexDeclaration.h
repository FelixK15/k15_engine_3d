/**
 * @file K15_VertexDeclaration.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Renderer_VertexDeclaration_h_
#define _K15Engine_Renderer_VertexDeclaration_h_

#ifndef K15_USE_PRECOMPILED_HEADER
# include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

namespace K15_Engine { namespace Rendering {
  struct K15_CORE_API VertexElement
  {
    /*********************************************************************************/
    enum eElementSemantic
    {
      ES_POSITION = 0,
      ES_NORMAL,
      ES_TEXCOORD,
      ES_COLOR,

      ES_COUNT
    }; //ElementSemantic
    /*********************************************************************************/
    enum eElementType
    {
      ET_FLOAT1 = 0,
      ET_FLOAT2,
      ET_FLOAT3,
      ET_FLOAT4,

      ET_HALF1,
      ET_HALF2,
      ET_HALF3,
      ET_HALF4,

      ET_COUNT
    }; //ElementType
    /*********************************************************************************/

    Enum Semantic;
    Enum Type;
    uint32 Offset;
    uint32 Index;
  }; //end of VertexElement struct
  /*********************************************************************************/
  class K15_CORE_API VertexDeclarationImplBase 
  {
  public:
      virtual void setElement(uint32 p_Index, const VertexElement& p_Element) = 0;

      void setVertexDeclaration(VertexDeclaration* p_Declaration);
      VertexDeclaration* getVertexDeclaration() const;
  protected:
    VertexDeclaration* m_VertexDeclaration;
  }; // end of VertexDeclarationImplBase class declaration
  /*********************************************************************************/
  class K15_CORE_API VertexDeclaration
  {
  public:
    /*********************************************************************************/
    typedef DynamicArray(VertexElement) VertexElementArray;
    static const char SemanticToCharConverter[VertexElement::ES_COUNT];
    static const char* TypeToCharConverter[VertexElement::ET_COUNT];
    /*********************************************************************************/
  public:
    VertexDeclaration();
    VertexDeclaration(const VertexElementArray& p_Elements);
    VertexDeclaration(const String& p_DeclarationString);

    ~VertexDeclaration();

    void addElement(VertexElement p_Element);
    void addElement(Enum p_Semantic, Enum p_Type);

    void insertElement(uint32 p_Index, VertexElement p_Element);
    void insertElement(uint32 p_Index, Enum p_Semantic, Enum p_Type);

    void addElements(const VertexElementArray& p_Elements);
    void addElementsByDeclarationString(const String& p_DeclarationString);

    void changeElement(uint32 p_Index, VertexElement p_Element);
    void changeElement(uint32 p_Index, uint32 p_Offset, Enum p_Semantic, Enum p_Type);

    void removeElement(uint32 p_Index);
    void removeAllElements();

    String getDeclarationString();
    const VertexElement& getElement(uint32 p_Index);
    
    uint32 getVertexSize();
    uint32 getElementCount() const;

  private:
    void _parseDeclarationString(const String& p_DeclarationString);
    void _recalculateOffsets();
    bool _validateElement(const VertexElement& p_Element);

  private:
    VertexElementArray m_Elements;
    uint32 m_Size;
    bool m_Dirty;
  }; //end of VertexDeclaration class declaration
}} //end of K15_Engine::Rendering namespace

#endif //_K15Engine_Renderer_VertexDeclaration_h_