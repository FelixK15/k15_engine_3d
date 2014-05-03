/**
 * @file K15_Vertex.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/02/11
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

/*********************************************************************************/
template<class Type>
Type K15_Engine::Rendering::Vertex::get(Enum p_Semantic) const
{
	static uint32 typeSize = sizeof(Type);
	Type returnVal;

	for(uint32 i = 0;i < getDeclaration()->getElementCount();++i)
	{
		VertexElement element = getDeclaration()->getElement(i);

		if(element.semantic == p_Semantic)
		{
// 			K15_ASSERT((element.size * VertexDeclaration::ElementTypeSize[element.type]) == typeSize,
// 				StringUtil::format("Size of semantic is unequal to size of object. Semantic size:%u bytes. Type size:%u bytes.",
// 				element.size,typeSize));
			
			uint32 offset = element.offset + (m_Index * m_Buffer->getVertexSize());
			uint32 bytesRead = m_Buffer->readData(typeSize,(byte*)&returnVal,offset);
			K15_ASSERT(bytesRead == typeSize,
				StringUtil::format("Read less data from vertexbuffer than what was needed. Read:%ubytes, needed:%ubytes",
				bytesRead,typeSize));
		
			return returnVal;
		}
	}

	return returnVal;
}
/*********************************************************************************/
template<class Type>
void Vertex::set(const Type& p_Value,Enum p_Semantic)
{
	static uint32 typeSize = sizeof(Type);

	for(uint32 i = 0;i < getDeclaration()->getElementCount();++i)
	{
		VertexElement element = getDeclaration()->getElement(i);
		if(element.semantic == p_Semantic)
		{
// 			K15_ASSERT((element.size * VertexDeclaration::ElementTypeSize[element.type]) == typeSize,
// 				StringUtil::format("Size of semantic is unequal to size of object. Semantic size:%u bytes. Type size:%u bytes.",
// 				element.size,typeSize));

			uint32 offset = element.offset + (m_Index * m_Buffer->getVertexSize());
			uint32 bytesWritten = m_Buffer->writeData(typeSize,(byte*)&p_Value,offset);

			K15_ASSERT(bytesWritten == typeSize,
				StringUtil::format("Wrote less data to vertexbuffer than what was asked for. Wrote:%ubytes, needed:%ubytes",
				bytesWritten,typeSize));

			return;
		}
	}

	return;
}
/*********************************************************************************/
VertexDeclaration* Vertex::getDeclaration() const
{
	return m_Buffer->getVertexDeclaration();
}
/*********************************************************************************/
VertexBuffer* Vertex::getBuffer() const
{
	return m_Buffer;
}
/*********************************************************************************/
uint32 Vertex::getIndex() const
{
	return m_Index;
}
/*********************************************************************************/