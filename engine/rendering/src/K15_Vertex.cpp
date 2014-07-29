/**
 * @file K15_Vertex.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/02/12
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

#include "K15_Vertex.h"

#include "K15_ColorRGBA.h"

namespace K15_Engine { namespace Rendering {
	/*********************************************************************************/
	Vertex::Vertex(VertexBuffer* p_Buffer, uint32 p_Index)
		: m_Buffer(p_Buffer),
		m_Index(p_Index)
	{

	}
	/*********************************************************************************/
	Vertex::~Vertex()
	{

	}
	/*********************************************************************************/
	Vector4 Vertex::getPosition() const
	{
		return get<Vector4>(VertexElement::ES_POSITION);
	}
	/*********************************************************************************/
	Vector4 Vertex::getNormal() const
	{
		return get<Vector4>(VertexElement::ES_NORMAL);
	}
	/*********************************************************************************/
	Vector2 Vertex::getUV() const
	{
		return get<Vector2>(VertexElement::ES_TEXCOORD);
	}
	/*********************************************************************************/
	ColorRGBA Vertex::getColor() const
	{
		return get<ColorRGBA>(VertexElement::ES_COLOR);
	}
	/*********************************************************************************/
	void Vertex::setPosition(const Vector4& p_Position)
	{
		set<Vector4>(p_Position,VertexElement::ES_POSITION);
	}
	/*********************************************************************************/
	void Vertex::setNormal(const Vector4& p_Normal)
	{
		set<Vector4>(p_Normal,VertexElement::ES_NORMAL);
	}
	/*********************************************************************************/
	void Vertex::setUV(const Vector2& p_UV)
	{
		set<Vector2>(p_UV,VertexElement::ES_TEXCOORD);
	}
	/*********************************************************************************/
	void Vertex::setColor(const ColorRGBA& p_Color)
	{
		set<ColorRGBA>(p_Color,VertexElement::ES_COLOR);
	}
	/*********************************************************************************/
}}// end of K15_Engine::Rendering namespace
