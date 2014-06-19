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
#include "K15_RendererBase.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	VertexDeclaration::VertexDeclarationMap VertexDeclaration::VertexDeclarationCache = VertexDeclaration::VertexDeclarationMap();
	/*********************************************************************************/

	/*********************************************************************************/
	const char* VertexElement::SemanticNames[VertexElement::ES_COUNT] = {
		"a_Position",	//ES_POSITION
		"a_Normal",		//ES_NORMAL
		"a_TexCoord",	//ES_TEXCOORD
		"a_Color"		//ES_COLOR
	}; //SemanticNames
	/*********************************************************************************/

	/*********************************************************************************/
	const char VertexDeclaration::SemanticToCharConverter[VertexElement::ES_COUNT] = {
		'P', //ES_POSITION
		'N', //ES_NORMAL
		'T', //ES_TEXCOORD
		'C'  //ES_COLOR
	}; //SemanticToCharconverter
	/*********************************************************************************/
	const char* VertexDeclaration::TypeToCharConverter[VertexElement::ET_COUNT] = {
		"F1", //ET_FLOAT1
		"F2", //ET_FLOAT2
		"F3", //ET_FLOAT3
		"F4", //ET_FLOA4T
    
		"H1", //ET_HALF1
		"H2", //ET_HALF2
		"H3", //ET_HALF3
		"H4"  //ET_HALF4
	}; //TypeToCharConverter
	/*********************************************************************************/

	/*********************************************************************************/
	VertexDeclarationImplBase::VertexDeclarationImplBase()
	{

	}
	/*********************************************************************************/
	VertexDeclarationImplBase::~VertexDeclarationImplBase()
	{

	}
	/*********************************************************************************/

	/*********************************************************************************/
	const uint32 VertexDeclaration::ElementTypeSize[VertexElement::ET_COUNT] = {
		1,  //ET_FLOAT1
		2,  //ET_FLOAT2
		3,  //ET_FLOAT3
		4,  //ET_FLOAT4

		1,  //ET_HALF1
		2,  //ET_HALF2
		3,  //ET_HALF3
		4   //ET_HALF4
	}; //ElementTypeSize
	/*********************************************************************************/

	VertexDeclaration* VertexDeclaration::create(const String& p_DeclarationString)
	{
		VertexDeclarationMap::iterator iter = VertexDeclarationCache.find(p_DeclarationString);

		if(iter == VertexDeclarationCache.end())
		{
			VertexDeclarationCache.insert(Pair(ObjectName,VertexDeclaration*)(ObjectName(p_DeclarationString),K15_NEW VertexDeclaration(p_DeclarationString)));
			iter = VertexDeclarationCache.find(p_DeclarationString);
		}

		return iter->second;
	}
	/*********************************************************************************/
	VertexDeclaration::VertexDeclaration(const String& p_DeclarationString)
	: m_Impl(g_Application->getRenderer()->createVertexDeclarationImpl())
	{
		m_Impl->setVertexDeclaration(this);

		_parseDeclarationString(p_DeclarationString);
	}
	/*********************************************************************************/
	VertexDeclaration::~VertexDeclaration()
	{
		K15_DELETE m_Impl;
		removeAllElements();
	}
	/*********************************************************************************/
	void VertexDeclaration::addElement(VertexElement p_Element)
	{
		p_Element.offset = getVertexSize();
		p_Element.index = getElementCount();
		m_Elements.push_back(p_Element);
		_updateElements();
	}
	/*********************************************************************************/
	void VertexDeclaration::addElement(Enum p_Semantic, Enum p_Type)
	{
		VertexElement element;
		element.semantic = p_Semantic;
		element.type = p_Type;
    
		addElement(element);
	}
	/*********************************************************************************/
	void VertexDeclaration::insertElement(uint32 p_Index, VertexElement p_Element)
	{
		p_Element.index = p_Index;
		p_Element.offset = getVertexSize();
		m_Elements[p_Index] = p_Element;
		_updateElements();
	}
	/*********************************************************************************/
	void VertexDeclaration::insertElement(uint32 p_Index, Enum p_Semantic, Enum p_Type)
	{
		VertexElement element;
		element.semantic = p_Semantic;
		element.type = p_Type;

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
		K15_ASSERT(p_Index < m_Elements.size(),"Trying to change an element out of bounds.");

		m_Elements[p_Index].type = p_Element.type;
		m_Elements[p_Index].semantic = p_Element.semantic;
		_updateElements();
	}
	/*********************************************************************************/
	void VertexDeclaration::changeElement(uint32 p_Index, uint32 p_Offset, Enum p_Semantic, Enum p_Type)
	{
		VertexElement element;
		element.semantic = p_Semantic;
		element.type = p_Type;

		changeElement(p_Index,element);
	}
	/*********************************************************************************/
	void VertexDeclaration::removeElement(uint32 p_Index)
	{
		for(VertexElementArray::iterator iter = m_Elements.begin();iter != m_Elements.end();++iter)
		{
			if(iter->index == p_Index)
			{
				m_Elements.erase(iter);
			}
		}

		_updateElements();
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
				if(iter->type >= VertexElement::ET_FLOAT1 && iter->type < VertexElement::ET_HALF1)
				{
					m_Size += (uint32)(sizeof(float) * (iter->type+1)); 
				}
				else if(iter->type >= VertexElement::ET_HALF1)
				{
					m_Size += (uint32)(sizeof(uint16) * ((iter->type+1)*0.5));
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

		if(p_DeclarationString.size() < SIZE_ELEMENT)
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
				element.semantic = VertexElement::ES_POSITION;
			}
			else if(elementStr[0] == 'N')
			{
				element.semantic = VertexElement::ES_NORMAL;
			}
			else if(elementStr[0] == 'T')
			{
				element.semantic = VertexElement::ES_TEXCOORD;
			}
			else if(elementStr[0] == 'C')
			{
				element.semantic = VertexElement::ES_COLOR;
			}
			else
			{
				_LogError("Invalid element declaration \"%s\"",elementStr.c_str());
				continue;
			}

			if(elementStr.find("F1") != String::npos)
			{
				element.type = VertexElement::ET_FLOAT1;
			}
			else if(elementStr.find("F2") != String::npos)
			{
				element.type = VertexElement::ET_FLOAT2;
			}
			else if(elementStr.find("F3") != String::npos)
			{
				element.type = VertexElement::ET_FLOAT3;
			}
			else if(elementStr.find("F4") != String::npos)
			{
				element.type = VertexElement::ET_FLOAT4;
			}
			else if(elementStr.find("H1") != String::npos)
			{
				element.type = VertexElement::ET_HALF1;
			}
			else if(elementStr.find("H2") != String::npos)
			{
				element.type = VertexElement::ET_HALF2;
			}
			else if(elementStr.find("H3") != String::npos)
			{
				element.type = VertexElement::ET_HALF3;
			}
			else if(elementStr.find("H4") != String::npos)
			{
				element.type = VertexElement::ET_HALF4;
			}
			else
			{
				_LogError("Invalid element declaration \"%s\"",elementStr.c_str());
				continue;
			}

			addElement(element);
		}

		m_Declaration = ObjectName(p_DeclarationString);

		m_Dirty = true;
	}
	/*********************************************************************************/
	String VertexDeclaration::getDeclarationString()
	{
		static String declarationString;

		declarationString.clear();

		for(VertexElementArray::const_iterator iter = m_Elements.begin();iter != m_Elements.end();++iter)
		{
			declarationString += SemanticToCharConverter[iter->semantic];
			declarationString += TypeToCharConverter[iter->type];
		}

		m_Declaration = ObjectName(declarationString);

		return declarationString;
	}
	/*********************************************************************************/
	void VertexDeclaration::_updateElements()
	{
		static uint32 elementSlots[VertexElement::ES_COUNT] = {0};
		memset(elementSlots,0,sizeof(elementSlots));

		uint32 offset = 0;
		uint32 counter = 0;
		for(VertexElementArray::iterator iter = m_Elements.begin();iter != m_Elements.end();++iter)
		{
			(*iter).slot = elementSlots[(*iter).semantic]++;
			(*iter).offset = offset;
			(*iter).size = ElementTypeSize[iter->type];

			if(iter->type > 0 && iter->type < VertexElement::ET_HALF1)
			{
				offset += (uint32)(sizeof(float) * (iter->type+1)); 
			}
			else if(iter->type >= VertexElement::ET_HALF1)
			{
				offset += (uint32)(sizeof(uint16) * ((iter->type+1)*0.5));
			}
		}

		m_Impl->updateElements(m_Elements);

		m_Dirty = false;
	}
	/*********************************************************************************/
	void VertexDeclaration::fromDeclarationString(const String& p_DeclarationString)
	{
		_parseDeclarationString(p_DeclarationString);
	}
	/*********************************************************************************/
}} //end of K15_Engine::Rendering namespace