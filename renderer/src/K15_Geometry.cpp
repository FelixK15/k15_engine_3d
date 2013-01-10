/**
 * @file K15_Geometry.cpp
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

#include "K15_Geometry.h"

using namespace K15_Renderer;

Geometry::Geometry()
	: m_pIndexBuffer(NULL),
	  m_pPixelShader(NULL),
	  m_pVertexBuffer(NULL),
	  m_pVertexShader(NULL)
{
	
}

Geometry::Geometry(BufferPtr pVertexBuffer,BufferPtr pIndexBuffer,VertexShaderPtr pVertexShader,PixelShaderPtr pPixelShader,PrimitiveTopolgy ptTopology)
	: m_pIndexBuffer(pIndexBuffer),
	  m_pPixelShader(pPixelShader),
	  m_pVertexBuffer(pVertexBuffer),
	  m_pVertexShader(pVertexShader),
	  m_ptTopology(ptTopology)
{

}

Geometry::Geometry(const Geometry &geo)
	: m_pIndexBuffer(geo.m_pIndexBuffer),
	  m_pVertexShader(geo.m_pVertexShader),
	  m_pVertexBuffer(geo.m_pVertexBuffer),
	  m_pPixelShader(geo.m_pPixelShader)
{

}

Geometry::~Geometry()
{

}

const Geometry &Geometry::operator=(const Geometry &geo)
{
	m_pPixelShader = geo.m_pPixelShader;
	m_pVertexBuffer = geo.m_pVertexBuffer;
	m_pIndexBuffer = geo.m_pIndexBuffer;
	m_pVertexShader = geo.m_pVertexShader;

	return *this;
}