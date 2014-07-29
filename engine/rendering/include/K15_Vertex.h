/**
 * @file K15_Vertex.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Renderer_Vertex_h_
#define _K15Engine_Renderer_Vertex_h_

#include "K15_Prerequisites.h"
#include "K15_AllocatedObject.h"

#include "K15_VertexBuffer.h"
#include "K15_VertexDeclaration.h"

#include "K15_Vector4.h"
#include "K15_Vector2.h"

namespace K15_Engine { namespace Rendering {
	class K15_CORE_API Vertex : public AllocatedObject<BaseAllocatedObject::AC_VERTEX_POOL>
	{
	public:
		Vertex(VertexBuffer* p_Buffer, uint32 p_Index);
		~Vertex();

		template<class Type>
		Type get(Enum p_Semantic) const;

		template<class Type>
		void set(const Type& p_Value,Enum p_Semantic);

		Vector4 getPosition() const;
		Vector4 getNormal() const;
		Vector2 getUV() const;
		ColorRGBA getColor() const;

		void setPosition(const Vector4& p_Position);
		void setNormal(const Vector4& p_Normal);
		void setUV(const Vector2& p_UV);
		void setColor(const ColorRGBA& p_Color);

		INLINE VertexDeclaration* getDeclaration() const;
		INLINE VertexBuffer* getBuffer() const;
		INLINE uint32 getIndex() const;
	private:
		uint32 m_Index;
		VertexBuffer* m_Buffer;
	};// end of Vertex class declaration
#	include "K15_Vertex.inl"
}}// end of K15_Engine::Rendering namespace

#endif //_K15Engine_Renderer_Vertex_h_