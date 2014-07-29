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

#include "K15_Prerequisites.h"
#include "K15_HashedString.h"
#include "K15_AllocatedObject.h"
#include "K15_GpuResource.h"

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

		/*********************************************************************************/
		static const char* SemanticNames[VertexElement::ES_COUNT];
		/*********************************************************************************/
		Enum semantic;
		Enum type;
		uint32 offset;
		uint32 index;
		uint32 slot;
		uint32 size;
	}; //end of VertexElement struct
	/*********************************************************************************/
	class K15_CORE_API VertexDeclaration : public GpuResource,
										   public RenderingAllocatedObject
	{
	public:
		/*********************************************************************************/
		typedef DynamicArray(VertexElement) VertexElementArray;
		typedef HashMap(ObjectName,VertexDeclaration*) VertexDeclarationMap;
		static const uint32 ElementTypeSize[VertexElement::ET_COUNT];
		static const char SemanticToCharConverter[VertexElement::ES_COUNT];
		static const char* TypeToCharConverter[VertexElement::ET_COUNT];
		/*********************************************************************************/

	private:
		/*********************************************************************************/
		static VertexDeclarationMap VertexDeclarationCache;
		/*********************************************************************************/
	public:
		static VertexDeclaration* create(const String& p_DeclarationString);

		virtual ~VertexDeclaration();

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

		void fromDeclarationString(const String& p_DeclarationString);

		String getDeclarationNameAsString();
		const VertexElement& getElement(uint32 p_Index);
    
		uint32 getVertexSize();
		uint32 getElementCount() const;

		INLINE const ObjectName& getDeclarationName() const;
		INLINE VertexDeclarationImplBase* getImpl() const;

	private:
		VertexDeclaration(const String& p_DeclarationString);
		void _parseDeclarationString(const String& p_DeclarationString);
		void _updateElements();
    
	private:
		VertexElementArray m_Elements;
		VertexDeclarationImplBase* m_Impl;
		ObjectName m_Declaration;
		uint32 m_Size;
		bool m_Dirty;
	}; //end of VertexDeclaration class declaration
	/*********************************************************************************/
	class K15_CORE_API VertexDeclarationImplBase : public RenderingAllocatedObject
	{
	public:
		VertexDeclarationImplBase();
		virtual ~VertexDeclarationImplBase();

		INLINE VertexDeclaration* getVertexDeclaration() const;
		INLINE void setVertexDeclaration(VertexDeclaration* p_Declaration);

		virtual void updateElements(const VertexDeclaration::VertexElementArray& p_Elements) = 0;
	protected:
		VertexDeclaration* m_Declaration;
	}; // end of VertexDeclarationImplBase class declaration
	/*********************************************************************************/

  K15_PTR(VertexDeclaration);
  K15_WEAKPTR(VertexDeclaration);
#include "K15_VertexDeclaration.inl"
}} //end of K15_Engine::Rendering namespace

#endif //_K15Engine_Renderer_VertexDeclaration_h_