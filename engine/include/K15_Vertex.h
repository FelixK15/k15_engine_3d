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

#ifndef _K15Engine_Core_*_h_
#define _K15Engine_Core_*_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#	include "K15_AllocatedObject.h"
#endif //K15_USE_PRECOMPILED_HEADER


namespace K15_Engine { namespace Rendering {
	class K15_CORE_API Vertex : public AllocatedObject<AC_VERTEX_POOL>
	{
	public:
		Vertex(VertexBuffer* p_Buffer, VertexDeclaration* p_Declaration);
		~Vertex();

		template<class Type>
		const Type& get(Enum p_Semantic);

		template<class Type>
		void set(const Type& p_Value,Enum p_Semantic);

		VertexDeclaration* getDeclaration() const;
		VertexBuffer* getBuffer() const;
	private:
		VertexBuffer* m_Buffer;
		VertexDeclaration* m_Declaration;
	};// end of * class declaration
#	include "K15_Vertex.inl"
}}// end of K15_Engine::Core namespace

#endif //