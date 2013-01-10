/**
 * @file K15_Geometry.inl
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/22
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

inline BufferPtr Geometry::GetVertexBuffer() const
{
	return m_pVertexBuffer;
}

inline BufferPtr Geometry::GetIndexBuffer() const
{
	return m_pIndexBuffer;
}

inline VertexShaderPtr Geometry::GetVertexShader() const
{
	return m_pVertexShader;
}

inline PixelShaderPtr Geometry::GetPixelShader() const
{
	return m_pPixelShader;
}

inline Geometry::PrimitiveTopolgy Geometry::GetPrimitiveTopology() const
{
	return m_ptTopology;
}

inline void Geometry::SetIndexBuffer(const BufferPtr pIndexBuffer)
{
	m_pIndexBuffer = pIndexBuffer;
}

inline void Geometry::SetVertexBuffer(const BufferPtr pVertexBuffer)
{
	m_pVertexBuffer = pVertexBuffer;
}

inline void Geometry::SetVertexShader(const VertexShaderPtr pVertexShader)
{
	m_pVertexShader = pVertexShader;
}

inline void Geometry::SetPixelShader(const PixelShaderPtr pPixelShader)
{
	m_pPixelShader = pPixelShader;
}

inline void Geometry::SetPrimitiveTopology(Geometry::PrimitiveTopolgy ptTopology)
{
	m_ptTopology = ptTopology;
}