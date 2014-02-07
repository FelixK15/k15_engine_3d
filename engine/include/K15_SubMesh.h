/**
 * @file K15_SubMesh.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/09
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

#ifndef _K15Engine_Rendering_SubMesh_h_
#define _K15Engine_Rendering_SubMesh_h_

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADERS

namespace K15_Engine { namespace Rendering {
	class K15_CORE_API SubMesh
	{
	public:
		SubMesh();
		virtual ~SubMesh();

		RenderOperation* createRenderOperation() const;
	protected:
		VertexBuffer* m_VertexBuffer;
		IndexBuffer* m_IndexBuffer;
		Material* m_Material;
	};// end of SubMesh class declaration
}}// end of K15_Engine::Rendering namespace

#endif //_K15Engine_Rendering_SubMesh_h_