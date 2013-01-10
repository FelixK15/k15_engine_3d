/**
 * @file K15_Geometry.h
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#pragma once

#ifndef __K15_GEOMETRY__
#define __K15_GEOMETRY__

#include "K15_SceneNode.h"

#include "K15_Buffer.h"
#include "K15_VertexShader.h"
#include "K15_PixelShader.h"

#include "K15_Material.h"

namespace K15_Renderer
{
	class Geometry : public SceneNode
	{
	public:
		enum PrimitiveTopolgy{
			PT_UNDEFINED = 0,
			PT_POINTLIST,
			PT_LINELIST,
			PT_LINESTRIP,
			PT_TRIANGLELIST,
			PT_TRIANGLESTRIP
		};

		Geometry();
		Geometry(BufferPtr pVertexBuffer,BufferPtr pIndexBuffer,VertexShaderPtr pVertexShader,PixelShaderPtr pPixelShader,PrimitiveTopolgy ptTopology);
		Geometry(const Geometry &geo);
		
		virtual ~Geometry();

		BufferPtr GetVertexBuffer() const;
		BufferPtr GetIndexBuffer() const;
		VertexShaderPtr GetVertexShader() const;
		PixelShaderPtr GetPixelShader() const;
		PrimitiveTopolgy GetPrimitiveTopology() const;

		void SetVertexBuffer(const BufferPtr pVertexBuffer);
		void SetIndexBuffer(const BufferPtr pIndexBuffer);
		void SetVertexShader(const VertexShaderPtr pVertexShader);
		void SetPixelShader(const PixelShaderPtr pPixelShader);
		void SetPrimitiveTopology(PrimitiveTopolgy ptTopology);

		const Geometry &operator=(const Geometry &geo);

	public:
		Material Material;

	private:
		VertexShaderPtr m_pVertexShader;
		PixelShaderPtr m_pPixelShader;

		BufferPtr m_pVertexBuffer;
		BufferPtr m_pIndexBuffer;

		PrimitiveTopolgy m_ptTopology;
	};
	#include "../src/K15_Geometry.inl"
}

#endif //__K15_GEOMETRY__