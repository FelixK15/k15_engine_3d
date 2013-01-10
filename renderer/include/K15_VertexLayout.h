/**
 * @file K15_VertexLayout.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/24
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

#ifndef __K15_VERTEXLAYOUT__
#define __K15_VERTEXLAYOUT__

#include "K15_Bindable.h"
#include "K15_Format.h"

using namespace K15_EngineV2;

namespace K15_Renderer
{
	class VertexShader;

	class VertexLayout : public Bindable
	{
		friend class Renderer;
		DECLARE_RTTI;

	public:
		enum VertexSemantic{
			VS_POSITION = 0,
			VS_NORMAL,
			VS_COLOR,
			VS_TEXCOORD,
			VS_SEMANTICCOUNT
		};

		struct SingleLayout{
			VertexSemantic Semantic;
			FormatType Format;
			U32 Size;
			U8 Slot;
		};

		virtual ~VertexLayout();

		U32 GetSizeOfVertex() const;
	private:
		VertexLayout(U8 iNumAttributes,va_list pArguments);

		U8 GetAmountAttributes() const;
		SingleLayout *GetLayout() const;
	private:
		SingleLayout *m_pLayout;
		U32 m_iVertexSize;
		U8 m_iAmountAttributes;
	};
	#include "../src/K15_VertexLayout.inl"
	typedef Pointer<VertexLayout> VertexLayoutPtr;
}
#endif //__K15_VERTEXLAYOUT__